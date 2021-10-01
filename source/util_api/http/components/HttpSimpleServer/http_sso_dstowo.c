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

    module:	http_sso_dstowo.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced worker functions
        of the Http Simple Server Object.

        *   HttpSsoDstowoAccept
        *   HttpSsoDstowoSetOut
        *   HttpSsoDstowoRemove
        *   HttpSsoDstowoQuery
        *   HttpSsoDstowoProcessSync
        *   HttpSsoDstowoProcessAsync
        *   HttpSsoDstowoSendComplete
        *   HttpSsoDstowoNotify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#include "http_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpSsoDstowoAccept
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
HttpSsoDstowoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject      = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PHTTP_WSP_INTERFACE             pWspIf         = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PHTTP_HFP_INTERFACE             pHfpIf         = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket     = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession   = (PHTTP_WEBS_SESSION_OBJECT     )pMyObject->AcquireWsso((ANSC_HANDLE)pMyObject);
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = NULL;

    AnscTrace
        (
            "DstowoAccept accepts a new client connection from %d.%d.%d.%d / TCP %d\n",
            pWebSocket->PeerAddress.Dot[0],
            pWebSocket->PeerAddress.Dot[1],
            pWebSocket->PeerAddress.Dot[2],
            pWebSocket->PeerAddress.Dot[3],
            pWebSocket->PeerPort
        );

    *phClientContext = (ANSC_HANDLE)NULL;

    /*
     * This routine is called when there's a new incoming connection (i.e., requested by the HTTP
     * client) is to be established. Following are the processing procedures applied:
     *
     *      (1) create a new HTTP Web Session Object for the connection
     *      (2) call session_accept(), the Web Service Provider either denies the
     *          connection or allows it.
     *      (3) create buffer_descriptor object and associate it with the socket object
     */
    if ( !pWebsSession )
    {
        return  FALSE;
    }
    else
    {
        pWebsSession->SetWspIf    ((ANSC_HANDLE)pWebsSession, (ANSC_HANDLE)pWspIf    );
        pWebsSession->SetHfpIf    ((ANSC_HANDLE)pWebsSession, (ANSC_HANDLE)pHfpIf    );
        pWebsSession->SetWebSocket((ANSC_HANDLE)pWebsSession, (ANSC_HANDLE)pWebSocket);
    }

    if ( !pWebsSession->Accept
            (
                (ANSC_HANDLE)pWebsSession,
                (ANSC_HANDLE)NULL
            ) )
    {
        pWebsSession->Return((ANSC_HANDLE)pWebsSession);

        return  FALSE;
    }
    else
    {
        pWebsSession->SetSessionState((ANSC_HANDLE)pWebsSession, HTTP_WSSO_STATE_CLIENT_CONNECTED);
        pWebsSession->Open           ((ANSC_HANDLE)pWebsSession);
    }

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                HTTP_SSO_MAX_HEADERS_SIZE,
                0,
                0
            );

    if ( pBufferDesp )
    {
        pWebSocket->SetBufferContext
            (
                (ANSC_HANDLE)pWebSocket,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );
    }

    *phClientContext = (ANSC_HANDLE)pWebsSession;

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoDstowoSetOut
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
HttpSsoDstowoSetOut
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
        HttpSsoDstowoRemove
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
HttpSsoDstowoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pWebSocket->GetBufferContext((ANSC_HANDLE)pWebSocket);
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession  = NULL;

    AnscTrace
        (
            "DstowoRemove removes the client connection of %d.%d.%d.%d / TCP %d\n",
            pWebSocket->PeerAddress.Dot[0],
            pWebSocket->PeerAddress.Dot[1],
            pWebSocket->PeerAddress.Dot[2],
            pWebSocket->PeerAddress.Dot[3],
            pWebSocket->PeerPort
        );

    AnscAcquireLock(&pMyObject->SyncLock);

    pWebsSession = (PHTTP_WEBS_SESSION_OBJECT)pWebSocket->GetClientContext((ANSC_HANDLE)pWebSocket);

    if ( pWebsSession )
    {
        pWebsSession->AcquireAccess((ANSC_HANDLE)pWebsSession);
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
    pWebSocket->SetClientContext((ANSC_HANDLE)pWebSocket,          (ANSC_HANDLE)NULL);
    pWebSocket->SetBufferContext((ANSC_HANDLE)pWebSocket, NULL, 0, (ANSC_HANDLE)NULL);

    if ( pBufferDesp )
    {
        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
    }

    if ( !pWebsSession )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pWebsSession->ReleaseAccess((ANSC_HANDLE)pWebsSession);
    pWebsSession->ClearWspServe((ANSC_HANDLE)pWebsSession);
    pWebsSession->DelAllWstos  ((ANSC_HANDLE)pWebsSession);
    pWebsSession->Close        ((ANSC_HANDLE)pWebsSession);
    pWebsSession->Return       ((ANSC_HANDLE)pWebsSession);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSsoDstowoQuery
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
HttpSsoDstowoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pWebSocket->GetBufferContext((ANSC_HANDLE)pWebSocket);
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession  = NULL;
    ULONG                           ulDstoPmode   = ANSC_DSTOWO_PMODE_PROCESS_SYNC;
    ULONG                           ulWssoQmode   = HTTP_WSSO_QMODE_PROCESS;

    AnscAcquireLock(&pMyObject->SyncLock);

    pWebsSession = (PHTTP_WEBS_SESSION_OBJECT)pWebSocket->GetClientContext((ANSC_HANDLE)pWebSocket);

    if ( !pWebsSession )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        return  ANSC_DSTOWO_PMODE_FINISH;
    }
    else
    {
        pWebsSession->AcquireAccess((ANSC_HANDLE)pWebsSession);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    pBufferDesp->BlockSize += ulSize;
    ulWssoQmode              =
        pWebsSession->Query
            (
                (ANSC_HANDLE)pWebsSession,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulWssoQmode )
    {
        case    HTTP_WSSO_QMODE_COLLECT :

                pBufferDesp->BlockSize -= ulSize;
                ulDstoPmode             = ANSC_DSTOWO_PMODE_COLLECT;

                break;

        case    HTTP_WSSO_QMODE_PROCESS :

                ulDstoPmode = ANSC_DSTOWO_PMODE_PROCESS_SYNC;

                break;

        default :

                ulDstoPmode = ANSC_DSTOWO_PMODE_FINISH;

                break;
    }

    *phQueryContext = (ANSC_HANDLE)ulWssoQmode;

    pWebsSession->ReleaseAccess((ANSC_HANDLE)pWebsSession);

    return  ulDstoPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoDstowoProcessSync
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
HttpSsoDstowoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pWebSocket->GetBufferContext((ANSC_HANDLE)pWebSocket);
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession  = NULL;
    ULONG                           ulWssoQmode   = (ULONG)hQueryContext;
    BOOL                            bFinishSocket = FALSE;

#ifdef   _DEBUG
    AnscTrace
        (
            "Processing packet received from client connection of %d.%d.%d.%d / TCP %d\n",
            pWebSocket->PeerAddress.Dot[0],
            pWebSocket->PeerAddress.Dot[1],
            pWebSocket->PeerAddress.Dot[2],
            pWebSocket->PeerAddress.Dot[3],
            pWebSocket->PeerPort
        );
#endif

    AnscAcquireLock(&pMyObject->SyncLock);

    pWebsSession = (PHTTP_WEBS_SESSION_OBJECT)pWebSocket->GetClientContext((ANSC_HANDLE)pWebSocket);

    if ( !pWebsSession )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        pWebSocket->Finish((ANSC_HANDLE)pWebSocket);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pWebsSession->AcquireAccess((ANSC_HANDLE)pWebsSession);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    switch ( ulWssoQmode )
    {
        case    HTTP_WSSO_QMODE_PROCESS :

                pWebSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pWebSocket,
                        NULL,
                        0,
                        (ANSC_HANDLE)NULL
                    );

                returnStatus =
                    pWebsSession->Recv
                        (
                            (ANSC_HANDLE)pWebsSession,
                            buffer,
                            ulSize,
                            (ANSC_HANDLE)pBufferDesp
                        );

                if ( pWebsSession->GetSessionState((ANSC_HANDLE)pWebsSession) == HTTP_WSSO_STATE_FINISHED )
                {
                    bFinishSocket = TRUE;

                    break;
                }

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    pWebsSession->ReleaseAccess((ANSC_HANDLE)pWebsSession);

    if ( bFinishSocket )
    {
        returnStatus = ANSC_STATUS_SUCCESS;

#ifdef   _DEBUG
        AnscTrace
            (
                "Going to close client connection of %d.%d.%d.%d / TCP %d\n",
                pWebSocket->PeerAddress.Dot[0],
                pWebSocket->PeerAddress.Dot[1],
                pWebSocket->PeerAddress.Dot[2],
                pWebSocket->PeerAddress.Dot[3],
                pWebSocket->PeerPort
            );
#endif

        /*pWebSocket->Finish((ANSC_HANDLE)pWebSocket);*/
        pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoDstowoProcessAsync
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
HttpSsoDstowoProcessAsync
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
        HttpSsoDstowoSendComplete
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
HttpSsoDstowoSendComplete
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
        HttpSsoDstowoNotify
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
HttpSsoDstowoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pWebSocket->GetBufferContext((ANSC_HANDLE)pWebSocket);
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession  = NULL;

    AnscTrace
        (
            "Event = %lu is indicated on client connection of %d.%d.%d.%d / TCP %d\n",
            ulEvent,
            pWebSocket->PeerAddress.Dot[0],
            pWebSocket->PeerAddress.Dot[1],
            pWebSocket->PeerAddress.Dot[2],
            pWebSocket->PeerAddress.Dot[3],
            pWebSocket->PeerPort
        );

    switch ( ulEvent )
    {
        case    ANSC_DSTOWO_EVENT_SOCKET_ERROR :

                pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);

                break;

        case    ANSC_DSTOWO_EVENT_SOCKET_CLOSED :

                AnscAcquireLock(&pMyObject->SyncLock);

                pWebsSession = (PHTTP_WEBS_SESSION_OBJECT)pWebSocket->GetClientContext((ANSC_HANDLE)pWebSocket);

                if ( !pWebsSession )
                {
                    AnscReleaseLock(&pMyObject->SyncLock);

                    pWebSocket->Finish((ANSC_HANDLE)pWebSocket);

                    break;
                }
                else
                {
                    pWebsSession->AcquireAccess((ANSC_HANDLE)pWebsSession);
                }

                AnscReleaseLock(&pMyObject->SyncLock);

                if ( pBufferDesp )
                {
                    pBufferDesp->BlockSize += pWebSocket->RecvPacketSize;
                    pWebSocket->SetBufferContext
                            (
                                (ANSC_HANDLE)pWebSocket,
                                NULL,
                                0,
                                (ANSC_HANDLE)NULL
                            );

                    /*
                    returnStatus            =
                        pWebsSession->Finish
                            (
                                (ANSC_HANDLE)pWebsSession,
                                AnscBdoGetBlock    (pBufferDesp),
                                AnscBdoGetBlockSize(pBufferDesp),
                                (ANSC_HANDLE)pBufferDesp
                            );
                    */
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                pWebsSession->ReleaseAccess((ANSC_HANDLE)pWebsSession);

                /*pWebSocket->Finish((ANSC_HANDLE)pWebSocket);*/
                pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);

                break;

        case    ANSC_DSTOWO_EVENT_RESOURCES :

                break;

        case    ANSC_DSTOWO_EVENT_TIME_OUT :

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
