/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module:	http_spo_prvwo.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced worker functions
        of the Http Simple Proxy Object.

        *   HttpSpoPrvwoAccept
        *   HttpSpoPrvwoSetOut
        *   HttpSpoPrvwoRemove
        *   HttpSpoPrvwoQuery
        *   HttpSpoPrvwoProcessSync
        *   HttpSpoPrvwoProcessAsync
        *   HttpSpoPrvwoSendComplete
        *   HttpSpoPrvwoNotify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/21/02    initial revision.

**********************************************************************/


#include "http_spo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpSpoPrvwoAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PANSC_HANDLE                phClientContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PANSC_HANDLE                phClientContext
                Specifies the client context to be returned.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpSpoPrvwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject      = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf         = (PHTTP_WAM_INTERFACE           )pMyObject->hWamIf;
    PHTTP_SBC_INTERFACE             pSbcIf         = (PHTTP_SBC_INTERFACE           )pMyObject->hSbcIf;
    PHTTP_CBC_INTERFACE             pCbcIf         = (PHTTP_CBC_INTERFACE           )pMyObject->hCbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf         = (PHTTP_PBC_INTERFACE           )pMyObject->hPbcIf;
    PHTTP_HFP_INTERFACE             pHfpIf         = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PHTTP_PROXY_SESSION_OBJECT      pSession       = (PHTTP_PROXY_SESSION_OBJECT    )pMyObject->AcquirePso((ANSC_HANDLE)pMyObject);
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = NULL;

    AnscTrace
        (
            "PrvwoAccept accepts a new client connection from %d.%d.%d.%d / TCP %d\n",
            pClientSocket->PeerAddress.Dot[0],
            pClientSocket->PeerAddress.Dot[1],
            pClientSocket->PeerAddress.Dot[2],
            pClientSocket->PeerAddress.Dot[3],
            pClientSocket->PeerPort
        );

    *phClientContext = (ANSC_HANDLE)NULL;

    /*
     * This routine is called when there's a new incoming connection (i.e., requested by the HTTP
     * client) is to be established. Following are the processing procedures applied:
     *
     *      (1) create a new HTTP Proxy Session Object for the connection
     *      (2) call sbc_connect(), the Server Behavior Controller either denies the
     *          connection or allows it.
     *      (3) create buffer_descriptor object and associate it with the socket object
     */
    if ( !pSession )
    {
        return  FALSE;
    }
    else
    {
        pSession->SetWamIf       ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pWamIf       );
        pSession->SetSbcIf       ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pSbcIf       );
        pSession->SetCbcIf       ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pCbcIf       );
        pSession->SetPbcIf       ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pPbcIf       );
        pSession->SetHfpIf       ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pHfpIf       );
        pSession->SetClientSocket((ANSC_HANDLE)pSession, (ANSC_HANDLE)pClientSocket);
    }

    if ( !pSession->AcceptClient
            (
                (ANSC_HANDLE)pSession,
                (ANSC_HANDLE)NULL
            ) )
    {
        pSession->Return((ANSC_HANDLE)pSession);

        return  FALSE;
    }
    else
    {
        pSession->SetSessionState((ANSC_HANDLE)pSession, HTTP_PSO_STATE_CLIENT_CONNECTED);
        pSession->Open           ((ANSC_HANDLE)pSession);
    }

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                HTTP_SPO_MAX_MESSAGE_SIZE,
                0,
                0
            );

    if ( pBufferDesp )
    {
        pClientSocket->SetBufferContext
            (
                (ANSC_HANDLE)pClientSocket,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );
    }

    *phClientContext = (ANSC_HANDLE)pSession;

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoSetOut
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoRemove
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject      = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )pClientSocket->GetBufferContext((ANSC_HANDLE)pClientSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession       = NULL;

    AnscTrace
        (
            "PrvwoRemove removes the client connection of %d.%d.%d.%d / TCP %d\n",
            pClientSocket->PeerAddress.Dot[0],
            pClientSocket->PeerAddress.Dot[1],
            pClientSocket->PeerAddress.Dot[2],
            pClientSocket->PeerAddress.Dot[3],
            pClientSocket->PeerPort
        );

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pClientSocket->GetClientContext((ANSC_HANDLE)pClientSocket);

    if ( pSession )
    {
        pSession->AcquireAccess((ANSC_HANDLE)pSession);

        pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)pSession->GetServerSocket((ANSC_HANDLE)pSession);
    }
    else
    {
        pServerSocket = NULL;
    }

    if ( pServerSocket )
    {
        pServerSocket->SetClientContext((ANSC_HANDLE)pServerSocket, (ANSC_HANDLE)NULL        );
        pServerSocket->ToClean         ((ANSC_HANDLE)pServerSocket, TRUE, HTTP_SPO_SOCKET_TTC);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    /*
     * Just a couple regular clean-up tasks to do:
     *
     *      (1) dis-associate the proxy session object from the client socket
     *      (2) clean the foreign buffer associated with the client socket
     *      (3) dis-associate the proxy session object from the server socket
     *
     * Note that we CANNOT clean the foreign buffer associated with the server socket because the
     * access to that buffer is NOT synchornized between the prv_recv() and pub_recv routines. It
     * SHOULD be done in the pub_remove() function.
     */
    pClientSocket->SetClientContext((ANSC_HANDLE)pClientSocket,          (ANSC_HANDLE)NULL);
    pClientSocket->SetBufferContext((ANSC_HANDLE)pClientSocket, NULL, 0, (ANSC_HANDLE)NULL);

    if ( pBufferDesp )
    {
        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
    }

    if ( !pSession )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pSession->ReleaseAccess((ANSC_HANDLE)pSession);
    pSession->Close        ((ANSC_HANDLE)pSession);
    pSession->Return       ((ANSC_HANDLE)pSession);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSpoPrvwoQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                PANSC_HANDLE                phQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                PANSC_HANDLE                phQueryContext
                Specifies the recv context associated with buffer.

    return:     process mode.

**********************************************************************/

ULONG
HttpSpoPrvwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject      = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )pClientSocket->GetBufferContext((ANSC_HANDLE)pClientSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession       = NULL;
    ULONG                           ulSptoPmode    = ANSC_SPTOWO_PMODE_PROCESS_SYNC;
    ULONG                           ulPsoQmode     = HTTP_PSO_QMODE_PROCESS;

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pClientSocket->GetClientContext((ANSC_HANDLE)pClientSocket);

    if ( !pSession )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        return  ANSC_SPTOWO_PMODE_FINISH;
    }
    else
    {
        pSession->AcquireAccess((ANSC_HANDLE)pSession);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    pBufferDesp->BlockSize += ulSize;
    ulPsoQmode              =
        pSession->QueryForClient
            (
                (ANSC_HANDLE)pSession,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulPsoQmode )
    {
        case    HTTP_PSO_QMODE_COLLECT :

                pBufferDesp->BlockSize -= ulSize;
                ulSptoPmode             = ANSC_SPTOWO_PMODE_COLLECT;

                break;

        case    HTTP_PSO_QMODE_FORWARD :
        case    HTTP_PSO_QMODE_PROCESS :

                ulSptoPmode = ANSC_SPTOWO_PMODE_PROCESS_SYNC;

                break;

        default :

                ulSptoPmode = ANSC_SPTOWO_PMODE_FINISH;

                break;
    }

    *phQueryContext = (ANSC_HANDLE)ulPsoQmode;

    pSession->ReleaseAccess((ANSC_HANDLE)pSession);

    return  ulSptoPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoProcessSync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hQueryContext
                Specifies the recv context associated with buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject      = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )pClientSocket->GetBufferContext((ANSC_HANDLE)pClientSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession       = NULL;
    ULONG                           ulPsoQmode     = (ULONG)hQueryContext;
    BOOL                            bFinishSocket  = FALSE;

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pClientSocket->GetClientContext((ANSC_HANDLE)pClientSocket);

    if ( !pSession )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        pClientSocket->Finish((ANSC_HANDLE)pClientSocket);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pSession->AcquireAccess((ANSC_HANDLE)pSession);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    switch ( ulPsoQmode )
    {
        case    HTTP_PSO_QMODE_FORWARD :

                pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)pSession->GetServerSocket((ANSC_HANDLE)pSession);

                if ( !pServerSocket)
                {
                    bFinishSocket = TRUE;
                    returnStatus  = ANSC_STATUS_INTERNAL_ERROR;

                    break;
                }
                else
                {
                    returnStatus =
                        pServerSocket->Send
                            (
                                (ANSC_HANDLE)pServerSocket,
                                buffer,
                                ulSize,
                                (ANSC_HANDLE)NULL
                            );
                }

                pBufferDesp->BlockSize = 0;

                break;

        case    HTTP_PSO_QMODE_PROCESS :

                pClientSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pClientSocket,
                        NULL,
                        0,
                        (ANSC_HANDLE)NULL
                    );

                returnStatus =
                    pSession->RecvFromClient
                        (
                            (ANSC_HANDLE)pSession,
                            buffer,
                            ulSize,
                            (ANSC_HANDLE)pBufferDesp
                        );

                if ( pSession->GetSessionState((ANSC_HANDLE)pSession) == HTTP_PSO_STATE_FINISHED )
                {
                    bFinishSocket = TRUE;

                    break;
                }

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    pSession->ReleaseAccess((ANSC_HANDLE)pSession);

    if ( bFinishSocket )
    {
        pClientSocket->Finish((ANSC_HANDLE)pClientSocket);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoProcessAsync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hQueryContext
                Specifies the recv context associated with buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    UNREFERENCED_PARAMETER(hQueryContext);
 
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoSendComplete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hReserved,
                ANSC_STATUS                 status
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                ANSC_HANDLE                 hReserved
                Specifies the transparent context passed in send().

                ANSC_STATUS                 status
                Specifies the status of the send operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    UNREFERENCED_PARAMETER(hReserved);
    UNREFERENCED_PARAMETER(status);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPrvwoNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                ULONG                       ulEvent
                Specifies the event to be notified.

                ANSC_HANDLE                 hReserved
                Specifies the transparent context associated with event.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoPrvwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pClientSocket->GetBufferContext((ANSC_HANDLE)pClientSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession      = NULL;

    AnscTrace
        (
            "Event = %lu is indicated on client connection of %d.%d.%d.%d / TCP %d\n",
            ulEvent,
            pClientSocket->PeerAddress.Dot[0],
            pClientSocket->PeerAddress.Dot[1],
            pClientSocket->PeerAddress.Dot[2],
            pClientSocket->PeerAddress.Dot[3],
            pClientSocket->PeerPort
        );

    switch ( ulEvent )
    {
        case    ANSC_SPTOWO_EVENT_SOCKET_ERROR :

                pClientSocket->ToClean((ANSC_HANDLE)pClientSocket, TRUE, HTTP_SPO_SOCKET_TTC);

                break;

        case    ANSC_SPTOWO_EVENT_SOCKET_CLOSED :

                AnscAcquireLock(&pMyObject->SyncLock);

                pSession = (PHTTP_PROXY_SESSION_OBJECT)pClientSocket->GetClientContext((ANSC_HANDLE)pClientSocket);

                if ( !pSession )
                {
                    AnscReleaseLock(&pMyObject->SyncLock);

                    pClientSocket->Finish((ANSC_HANDLE)pClientSocket);

                    break;
                }
                else
                {
                    pSession->AcquireAccess((ANSC_HANDLE)pSession);
                }

                AnscReleaseLock(&pMyObject->SyncLock);

                pBufferDesp->BlockSize += pClientSocket->RecvPacketSize;
                pClientSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pClientSocket,
                            NULL,
                            0,
                            (ANSC_HANDLE)NULL
                        );

                pSession->FinishedByClient
                        (
                            (ANSC_HANDLE)pSession,
                            AnscBdoGetBlock    (pBufferDesp),
                            AnscBdoGetBlockSize(pBufferDesp),
                            (ANSC_HANDLE)pBufferDesp
                        );

                pSession->ReleaseAccess((ANSC_HANDLE)pSession);

                pClientSocket->Finish((ANSC_HANDLE)pClientSocket);

                break;

        case    ANSC_SPTOWO_EVENT_RESOURCES :

                break;

        case    ANSC_SPTOWO_EVENT_TIME_OUT :

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
