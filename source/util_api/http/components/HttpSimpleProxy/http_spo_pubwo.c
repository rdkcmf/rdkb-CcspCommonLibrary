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

    module:	http_spo_pubwo.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced worker functions
        of the Http Simple Proxy Object.

        *   HttpSpoPubwoAccept
        *   HttpSpoPubwoRemove
        *   HttpSpoPubwoQuery
        *   HttpSpoPubwoProcessSync
        *   HttpSpoPubwoProcessAsync
        *   HttpSpoPubwoSendComplete
        *   HttpSpoPubwoNotify

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
        HttpSpoPubwoAccept
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
HttpSpoPubwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    )
{
    UNREFERENCED_PARAMETER(phClientContext);
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = NULL;

    AnscTrace
        (
            "PubwoAccept creates a new server connection to %d.%d.%d.%d / TCP %d\n",
            pServerSocket->PeerAddress.Dot[0],
            pServerSocket->PeerAddress.Dot[1],
            pServerSocket->PeerAddress.Dot[2],
            pServerSocket->PeerAddress.Dot[3],
            pServerSocket->PeerPort
        );

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                HTTP_SPO_MAX_MESSAGE_SIZE,
                0,
                0
            );

    if ( pBufferDesp )
    {
        pServerSocket->SetBufferContext
            (
                (ANSC_HANDLE)pServerSocket,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPubwoRemove
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
HttpSpoPubwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession      = NULL;

    AnscTrace
        (
            "PubwoRemove removes the server connection of %d.%d.%d.%d / TCP %d\n",
            pServerSocket->PeerAddress.Dot[0],
            pServerSocket->PeerAddress.Dot[1],
            pServerSocket->PeerAddress.Dot[2],
            pServerSocket->PeerAddress.Dot[3],
            pServerSocket->PeerPort
        );

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

    if ( pSession )
    {
        pSession->AcquireAccess((ANSC_HANDLE)pSession);

        pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pSession->GetClientSocket((ANSC_HANDLE)pSession);
    }
    else
    {
        pClientSocket = NULL;
    }

    if ( pClientSocket )
    {
        pClientSocket->SetClientContext((ANSC_HANDLE)pClientSocket, (ANSC_HANDLE)NULL        );
        pClientSocket->ToClean         ((ANSC_HANDLE)pClientSocket, TRUE, HTTP_SPO_SOCKET_TTC);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    /*
     * Just a couple regular clean-up tasks to do:
     *
     *      (1) dis-associate the proxy session object from the server socket
     *      (2) clean the foreign buffer associated with the server socket
     *      (3) dis-associate the proxy session object from the client socket
     *
     * Note that we CANNOT clean the foreign buffer associated with the client socket because the
     * access to that buffer is NOT synchornized between the prv_recv() and pub_recv routines. It
     * SHOULD be done in the prv_remove() function.
     */
    pServerSocket->SetClientContext((ANSC_HANDLE)pServerSocket,          (ANSC_HANDLE)NULL);
    pServerSocket->SetBufferContext((ANSC_HANDLE)pServerSocket, NULL, 0, (ANSC_HANDLE)NULL);

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
        HttpSpoPubwoQuery
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
HttpSpoPubwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession      = NULL;
    ULONG                           ulSptoPmode   = ANSC_SPTOWO_PMODE_PROCESS_SYNC;
    ULONG                           ulPsoQmode    = HTTP_PSO_QMODE_PROCESS;

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

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
        pSession->QueryForServer
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
        HttpSpoPubwoProcessSync
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
HttpSpoPubwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession      = NULL;
    ULONG                           ulPsoQmode    = (ULONG)hQueryContext;
    BOOL                            bFinishSocket = FALSE;

    AnscAcquireLock(&pMyObject->SyncLock);

    pSession = (PHTTP_PROXY_SESSION_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

    if ( !pSession )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

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

                pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pSession->GetClientSocket((ANSC_HANDLE)pSession);

                if ( !pClientSocket)
                {
                    bFinishSocket = TRUE;
                    returnStatus  = ANSC_STATUS_INTERNAL_ERROR;

                    break;
                }
                else
                {
                    returnStatus =
                        pClientSocket->Send
                            (
                                (ANSC_HANDLE)pClientSocket,
                                buffer,
                                ulSize,
                                (ANSC_HANDLE)NULL
                            );
                }

                pBufferDesp->BlockSize = 0;

                break;

        case    HTTP_PSO_QMODE_PROCESS :

                pServerSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pServerSocket,
                        NULL,
                        0,
                        (ANSC_HANDLE)NULL
                    );

                returnStatus =
                    pSession->RecvFromServer
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
        pServerSocket->Finish((ANSC_HANDLE)pServerSocket);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoPubwoProcessAsync
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
HttpSpoPubwoProcessAsync
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
        HttpSpoPubwoSendComplete
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
HttpSpoPubwoSendComplete
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
        HttpSpoPubwoNotify
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
HttpSpoPubwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT     )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PROXY_SESSION_OBJECT      pSession      = NULL;

    AnscTrace
        (
            "Event = %lu is indicated on server connection of %d.%d.%d.%d / TCP %d\n",
            ulEvent,
            pServerSocket->PeerAddress.Dot[0],
            pServerSocket->PeerAddress.Dot[1],
            pServerSocket->PeerAddress.Dot[2],
            pServerSocket->PeerAddress.Dot[3],
            pServerSocket->PeerPort
        );

    switch ( ulEvent )
    {
        case    ANSC_SPTOWO_EVENT_SOCKET_ERROR :

                pServerSocket->ToClean((ANSC_HANDLE)pServerSocket, TRUE, HTTP_SPO_SOCKET_TTC);

                break;

        case    ANSC_SPTOWO_EVENT_SOCKET_CLOSED :

                AnscAcquireLock(&pMyObject->SyncLock);

                pSession = (PHTTP_PROXY_SESSION_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

                if ( !pSession )
                {
                    AnscReleaseLock(&pMyObject->SyncLock);

                    pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

                    break;
                }
                else
                {
                    pSession->AcquireAccess((ANSC_HANDLE)pSession);
                }

                AnscReleaseLock(&pMyObject->SyncLock);

                pBufferDesp->BlockSize += pServerSocket->RecvPacketSize;
                pServerSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pServerSocket,
                            NULL,
                            0,
                            (ANSC_HANDLE)NULL
                        );

                pSession->FinishedByServer
                        (
                            (ANSC_HANDLE)pSession,
                            AnscBdoGetBlock    (pBufferDesp),
                            AnscBdoGetBlockSize(pBufferDesp),
                            (ANSC_HANDLE)pBufferDesp
                        );

                pSession->ReleaseAccess((ANSC_HANDLE)pSession);

                pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

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
