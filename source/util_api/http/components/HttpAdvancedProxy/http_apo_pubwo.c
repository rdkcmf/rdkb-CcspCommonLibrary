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

    module:	http_apo_pubwo.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced worker functions
        of the Http Advanced Proxy Object.

        *   HttpApoPubwoAccept
        *   HttpApoPubwoRemove
        *   HttpApoPubwoQuery
        *   HttpApoPubwoProcessSync
        *   HttpApoPubwoProcessAsync
        *   HttpApoPubwoSendComplete
        *   HttpApoPubwoNotify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_apo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpApoPubwoAccept
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
HttpApoPubwoAccept
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
                HTTP_APO_MAX_MESSAGE_SIZE,
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
        HttpApoPubwoRemove
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
HttpApoPubwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject     = (PHTTP_ADVANCED_PROXY_OBJECT   )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PSO_VER2_OBJECT           pPsoVer2      = NULL;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord  = NULL;
    BOOL                            bRemovePso    = FALSE;

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

    pPsoVer2 = (PHTTP_PSO_VER2_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

    if ( pPsoVer2 )
    {
        pPsoVer2->AcquireAccess((ANSC_HANDLE)pPsoVer2);

        pTransRecord =
            (PHTTP_TRANS_RECORD_OBJECT)pPsoVer2->AskTroBySocket
                (
                    (ANSC_HANDLE)pPsoVer2,
                    (ANSC_HANDLE)pServerSocket
                );
    }

    /*
     * Just a couple regular clean-up tasks to do:
     *
     *      (1) dis-associate the trans record object from the server socket
     *      (2) clean the foreign buffer associated with the server socket
     *      (3) dis-associate the trans record object from the client socket
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

    if ( pTransRecord )
    {
        pTransRecord->SetServerSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL      );
        pTransRecord->SetTransState  ((ANSC_HANDLE)pTransRecord, HTTP_TRO_STATE_FINISHED);
        pTransRecord->ReleaseAccess  ((ANSC_HANDLE)pTransRecord);
    }

    /*
     * The idea of pipeline the client requests over a single client session is great, however, we
     * may run into a situation where wrongly-implemented clients are waiting forever for server to
     * close the connection. To prevent that, we should close the session if no outstanding trans-
     * action needs to be completed.
     */
    if ( pPsoVer2 )
    {
        pTransRecord = (PHTTP_TRANS_RECORD_OBJECT)pPsoVer2->GetLastTro((ANSC_HANDLE)pPsoVer2);

        if ( pTransRecord )
        {
            if ( (pTransRecord->GetTransState((ANSC_HANDLE)pTransRecord) == HTTP_TRO_STATE_FINISHED) &&
                 (pTransRecord->bCloseConnection                         == TRUE                   ) )
            {
                bRemovePso    = TRUE;
                pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pPsoVer2->GetClientSocket((ANSC_HANDLE)pPsoVer2);

                if ( pClientSocket )
                {
                    pClientSocket->SetClientContext((ANSC_HANDLE)pClientSocket, (ANSC_HANDLE)NULL         );
                    pClientSocket->ToClean         ((ANSC_HANDLE)pClientSocket, FALSE, HTTP_APO_SOCKET_TTC);
                }
            }

            pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);
        }

        pPsoVer2->ReleaseAccess((ANSC_HANDLE)pPsoVer2);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    /*
     * If you're wondering why we have to explicitly close the connection here, why not wait until
     * triggered by the clients-side actions, here's why: the stupid desktop browsers (i.e., Micro-
     * soft IE browsers) will not take any action but waiting for the connection to be closed.
     */
    if ( bRemovePso )
    {
        pPsoVer2->DelAllTros((ANSC_HANDLE)pPsoVer2);
        pPsoVer2->Close     ((ANSC_HANDLE)pPsoVer2);
        pPsoVer2->Return    ((ANSC_HANDLE)pPsoVer2);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpApoPubwoQuery
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
HttpApoPubwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject     = (PHTTP_ADVANCED_PROXY_OBJECT   )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PSO_VER2_OBJECT           pPsoVer2      = NULL;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord  = NULL;
    ULONG                           ulSptoPmode   = ANSC_SPTOWO_PMODE_PROCESS_SYNC;
    ULONG                           ulTroQmode    = HTTP_TRO_QMODE_PROCESS;

    AnscAcquireLock(&pMyObject->SyncLock);

    pPsoVer2 = (PHTTP_PSO_VER2_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

    if ( pPsoVer2 )
    {
        pPsoVer2->AcquireAccess((ANSC_HANDLE)pPsoVer2);

        pTransRecord =
            (PHTTP_TRANS_RECORD_OBJECT)pPsoVer2->AskTroBySocket
                (
                    (ANSC_HANDLE)pPsoVer2,
                    (ANSC_HANDLE)pServerSocket
                );

        pPsoVer2->ReleaseAccess((ANSC_HANDLE)pPsoVer2);
    }
    else
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        return  ANSC_SPTOWO_PMODE_FINISH;
    }

    if ( !pTransRecord )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        return  ANSC_SPTOWO_PMODE_FINISH;
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    pBufferDesp->BlockSize += ulSize;
    ulTroQmode              =
        pTransRecord->QueryForServer
            (
                (ANSC_HANDLE)pTransRecord,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulTroQmode )
    {
        case    HTTP_TRO_QMODE_COLLECT :

                pBufferDesp->BlockSize -= ulSize;
                ulSptoPmode             = ANSC_SPTOWO_PMODE_COLLECT;

                break;

        case    HTTP_TRO_QMODE_FORWARD :
        case    HTTP_TRO_QMODE_PROCESS :

                ulSptoPmode = ANSC_SPTOWO_PMODE_PROCESS_SYNC;

                break;

        default :

                ulSptoPmode = ANSC_SPTOWO_PMODE_FINISH;

                break;
    }

    *phQueryContext = (ANSC_HANDLE)ulTroQmode;

    pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

    return  ulSptoPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPubwoProcessSync
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
HttpApoPubwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject     = (PHTTP_ADVANCED_PROXY_OBJECT   )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PSO_VER2_OBJECT           pPsoVer2      = NULL;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord  = NULL;
    ULONG                           ulTroQmode    = (ULONG)hQueryContext;
    BOOL                            bFinishSocket = FALSE;

    AnscAcquireLock(&pMyObject->SyncLock);

    pPsoVer2 = (PHTTP_PSO_VER2_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

    if ( pPsoVer2 )
    {
        pPsoVer2->AcquireAccess((ANSC_HANDLE)pPsoVer2);

        pTransRecord =
            (PHTTP_TRANS_RECORD_OBJECT)pPsoVer2->AskTroBySocket
                (
                    (ANSC_HANDLE)pPsoVer2,
                    (ANSC_HANDLE)pServerSocket
                );

        pPsoVer2->ReleaseAccess((ANSC_HANDLE)pPsoVer2);
    }
    else
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

        return  ANSC_STATUS_SUCCESS;
    }

    if ( !pTransRecord )
    {
        AnscReleaseLock(&pMyObject->SyncLock);

        pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

        return  ANSC_STATUS_SUCCESS;
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    switch ( ulTroQmode )
    {
        case    HTTP_TRO_QMODE_FORWARD :

                pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pTransRecord->GetClientSocket((ANSC_HANDLE)pTransRecord);

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

        case    HTTP_TRO_QMODE_PROCESS :

                pServerSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pServerSocket,
                        NULL,
                        0,
                        (ANSC_HANDLE)NULL
                    );

                returnStatus =
                    pTransRecord->RecvFromServer
                        (
                            (ANSC_HANDLE)pTransRecord,
                            buffer,
                            ulSize,
                            (ANSC_HANDLE)pBufferDesp
                        );

                if ( pTransRecord->GetTransState((ANSC_HANDLE)pTransRecord) == HTTP_TRO_STATE_FINISHED )
                {
                    bFinishSocket = TRUE;

                    break;
                }

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

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
        HttpApoPubwoProcessAsync
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
HttpApoPubwoProcessAsync
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
        HttpApoPubwoSendComplete
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
HttpApoPubwoSendComplete
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
        HttpApoPubwoNotify
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
HttpApoPubwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject     = (PHTTP_ADVANCED_PROXY_OBJECT   )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pServerSocket->GetBufferContext((ANSC_HANDLE)pServerSocket);
    PHTTP_PSO_VER2_OBJECT           pPsoVer2      = NULL;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord  = NULL;

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

                pServerSocket->ToClean((ANSC_HANDLE)pServerSocket, TRUE, HTTP_APO_SOCKET_TTC);

                break;

        case    ANSC_SPTOWO_EVENT_SOCKET_CLOSED :

                AnscAcquireLock(&pMyObject->SyncLock);

                pPsoVer2 = (PHTTP_PSO_VER2_OBJECT)pServerSocket->GetClientContext((ANSC_HANDLE)pServerSocket);

                if ( pPsoVer2 )
                {
                    pPsoVer2->AcquireAccess((ANSC_HANDLE)pPsoVer2);

                    pTransRecord =
                        (PHTTP_TRANS_RECORD_OBJECT)pPsoVer2->AskTroBySocket
                            (
                                (ANSC_HANDLE)pPsoVer2,
                                (ANSC_HANDLE)pServerSocket
                            );

                    pPsoVer2->ReleaseAccess((ANSC_HANDLE)pPsoVer2);
                }
                else
                {
                    AnscReleaseLock(&pMyObject->SyncLock);

                    pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

                    return  ANSC_STATUS_SUCCESS;
                }

                if ( !pTransRecord )
                {
                    AnscReleaseLock(&pMyObject->SyncLock);

                    pServerSocket->Finish((ANSC_HANDLE)pServerSocket);

                    return  ANSC_STATUS_SUCCESS;
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

                pTransRecord->FinishedByServer
                        (
                            (ANSC_HANDLE)pTransRecord,
                            AnscBdoGetBlock    (pBufferDesp),
                            AnscBdoGetBlockSize(pBufferDesp),
                            (ANSC_HANDLE)pBufferDesp
                        );

                pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

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
