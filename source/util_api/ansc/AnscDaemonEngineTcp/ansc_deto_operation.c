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

    module:	ansc_deto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoStart
        *   AnscDetoStop
        *   AnscDetoCancel
        *   AnscDetoClean
        *   AnscDetoSnapshot
        *   AnscDetoCloseUp

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/09/01    initial revision.

**********************************************************************/


#include "ansc_deto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start the engine running.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    if ( pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);

        pMyObject->StartTime = AnscGetTickInSecondsAbs();
        pMyObject->bStarted  = TRUE;
    }

    if ( TRUE )
    {
        AnscResetEvent(&pMyObject->RecvEvent);
            AnscSpawnTask3
                (
                    (void*)pMyObject->RecvTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_DETO_RECV_TASK_NAME,
                    ANSC_TASK_PRIORITY_NORMAL,
                    ANSC_DETO_RECV_TASK_STACK_SIZE
                );
    }

    if ( pMyObject->ControlFlags & ANSC_DETO_FLAG_ASYNC_SEND )
    {
        AnscResetEvent(&pMyObject->SendEvent);
            AnscSpawnTask
                (
                    (void*)pMyObject->SendTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_DETO_SEND_TASK_NAME
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoStop
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to stop the engine running.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoStop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = FALSE;
    }

    AnscWaitEvent(&pMyObject->RecvEvent, ANSC_DETO_TASK_CLEANUP_TIME);
    AnscWaitEvent(&pMyObject->SendEvent, ANSC_DETO_TASK_CLEANUP_TIME);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoCancel
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to cancel the pending send requests.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be cancelled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoCancel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DETO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( (pMyObject->ControlFlags & ANSC_DETO_FLAG_ASYNC_SEND) == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock(&pMyObject->PacketQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PacketQueue);

    while ( pSLinkEntry )
    {
        pPacket     = ACCESS_ANSC_DETO_PACKET_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pPacket->hSocket != (ANSC_HANDLE)pSocket )
        {
            continue;
        }

        AnscQueuePopEntryByLink(&pMyObject->PacketQueue, &pPacket->Linkage);

            pWorker->SendComplete
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pSocket,
                    pPacket->hWorkerReserved,
                    ANSC_STATUS_FAILURE
                );

        AnscFreeMemory(pPacket);
    }

    AnscReleaseLock(&pMyObject->PacketQueueLock);



    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoClean
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean out the socket table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoClean
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = 0;
    BOOL                            bSocketFound = FALSE;
    ULONG                           ulCurTime    = AnscGetTickInSecondsAbs();
    ULONG                           ulTimeout    = ANSC_DETO_SOCKET_TIMEOUT;

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    /*
    else if ( pMyObject->ControlFlags & ANSC_DETO_FLAG_NO_TIMEOUT )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    */

    if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
    {
        ulTimeout = ANSC_DETO_SOCKET_TIMEOUT2;
    }
    else
    {
        ulTimeout = ANSC_DETO_SOCKET_TIMEOUT;
    }

    ulHashIndex  = 0;
    bSocketFound = FALSE;

    while ( ulHashIndex < ANSC_DETO_SOCKET_TABLE_SIZE )
    {
        AnscAcquireLock(&pMyObject->SocketTableLock);

        bSocketFound = FALSE;
        pSLinkEntry  = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( pSocket->bLocked )
            {
                continue;
            }
            else if ( pSocket->bToBeCleaned )
            {
                bSocketFound = TRUE;

                break;
            }
            else if ( pMyObject->ControlFlags & ANSC_DETO_FLAG_NO_TIMEOUT )
            {
                continue;
            }
            else
            {
                ulCurTime = AnscGetTickInSecondsAbs();
            }

            if ( ((ulCurTime - pSocket->LastRecvAt) >= ulTimeout) &&
                 ((ulCurTime - pSocket->LastSendAt) >= ulTimeout) )
            {
                bSocketFound = TRUE;

                break;
            }
        }

        AnscReleaseLock(&pMyObject->SocketTableLock);

        if ( bSocketFound )
        {
            if ( !pSocket->bTlsEnabled ||
                 (pSocket->bTlsEnabled && pSocket->bTlsConnected) )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            (ANSC_HANDLE)pSocket,
                            ANSC_DSTOWO_EVENT_TIME_OUT,
                            (ANSC_HANDLE)NULL
                        );
            }

                pMyObject->DelSocket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pSocket
                    );

            pMyObject->TtcCount++;
        }
        else
        {
            ulHashIndex++;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSnapshot
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hForm
            );

    description:

        This function is called to fill in the "snapshot form" for
        diagnostic purpose.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hForm
                Specifies the data structure to be filled in.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoSnapshot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    ansc_fd_set*                    pRecvSet      = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    ansc_fd_set*                    pSendSet      = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    PANSC_DSTO_SNAPSHOT_FORM        pSnapshotForm = (PANSC_DSTO_SNAPSHOT_FORM      )hForm;
    ULONG                           ulClientCount = 0;
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);
    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        ulClientCount += AnscSListQueryDepth(&pMyObject->SocketTable[i]);
    }
    AnscReleaseLock(&pMyObject->SocketTableLock);

    pSnapshotForm->StartTime      = pMyObject->StartTime;
    pSnapshotForm->MaxClientCount = pMyObject->MaxSocketCount;
    pSnapshotForm->OffClientCount = pMyObject->CurSocketCount;
    pSnapshotForm->CurClientCount = ulClientCount;
    pSnapshotForm->CurRecvSetSize = (ULONG)pRecvSet->fd_count;
    pSnapshotForm->CurSendSetSize = (ULONG)pSendSet->fd_count;
    pSnapshotForm->AvgServingTime = pMyObject->AvgServingTime;
    pSnapshotForm->TtcCount       = pMyObject->TtcCount;
    pSnapshotForm->MtcCount       = pMyObject->MtcCount;
    pSnapshotForm->TrcCount       = pMyObject->TrcCount;
    pSnapshotForm->MrcCount       = pMyObject->MrcCount;
    pSnapshotForm->TscCount       = pMyObject->TscCount;
    pSnapshotForm->MscCount       = pMyObject->MscCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoCloseUp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hForm
            );

    description:

        This function is called to fill in the "close-up form" for
        diagnostic purpose.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hForm
                Specifies the data structure to be filled in.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DSTO_CLOSE_UP_FORM        pCloseUpForm = (PANSC_DSTO_CLOSE_UP_FORM      )hForm;
    PANSC_DSTO_SOCKET_INFO          pSocketInfo  = NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCount      = 0;
    ULONG                           i            = 0;
    ULONG                           ulCurTime    = AnscGetTickInSecondsAbs();

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            pSocketInfo = (PANSC_DSTO_SOCKET_INFO)&pCloseUpForm->SocketArray[ulCount++];

            pSocketInfo->StartTime      = pSocket->StartTime;
            pSocketInfo->CurrentTime    = ulCurTime;
            pSocketInfo->LastRecvAt     = pSocket->LastRecvAt;
            pSocketInfo->LastSendAt     = pSocket->LastSendAt;
            pSocketInfo->RecvBytesCount = pSocket->RecvBytesCount;
            pSocketInfo->SendBytesCount = pSocket->SendBytesCount;
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    pCloseUpForm->SocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}
