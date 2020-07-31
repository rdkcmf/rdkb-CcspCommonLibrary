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

    module:	ansc_beto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Broker Engine Tcp Object.

        *   AnscBetoStart
        *   AnscBetoStop
        *   AnscBetoCancel
        *   AnscBetoClean

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#include "ansc_beto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoStart
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
AnscBetoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;

    if ( pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);

        pMyObject->StartTime = AnscGetTickInSeconds();
        pMyObject->bStarted  = TRUE;
    }

    if ( TRUE )
    {
        AnscResetEvent(&pMyObject->RecvEvent);
            AnscSpawnTask
                (
                    (void*)pMyObject->RecvTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_BETO_RECV_TASK_NAME
                );
    }

    if ( pMyObject->ControlFlags & ANSC_BETO_FLAG_ASYNC_SEND )
    {
        AnscResetEvent(&pMyObject->SendEvent);
            AnscSpawnTask
                (
                    (void*)pMyObject->SendTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_BETO_SEND_TASK_NAME
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoStop
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
AnscBetoStop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = FALSE;
    }

    AnscWaitEvent(&pMyObject->RecvEvent, ANSC_BETO_TASK_CLEANUP_TIME);
    AnscWaitEvent(&pMyObject->SendEvent, ANSC_BETO_TASK_CLEANUP_TIME);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoCancel
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
AnscBetoCancel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BETO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( (pMyObject->ControlFlags & ANSC_BETO_FLAG_ASYNC_SEND) == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock(&pMyObject->PacketQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PacketQueue);

    while ( pSLinkEntry )
    {
        pPacket     = ACCESS_ANSC_BETO_PACKET_OBJECT(pSLinkEntry);
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

    return ANSC_STATUS_SUCCESS; 
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoClean
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
AnscBetoClean
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = 0;
    BOOL                            bSocketFound = FALSE;
    ULONG                           ulCurTime    = AnscGetTickInSeconds();

    if ( !pMyObject->bStarted )
    {
        return  returnStatus;
    }
    /*
    else if ( pMyObject->ControlFlags & ANSC_BETO_FLAG_NO_TIMEOUT )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    */

    ulHashIndex  = 0;
    bSocketFound = FALSE;

    while ( ulHashIndex < ANSC_BETO_SOCKET_TABLE_SIZE )
    {
        AnscAcquireLock(&pMyObject->SocketTableLock);

        bSocketFound = FALSE;
        pSLinkEntry  = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( pSocket->bToBeCleaned )
            {
                bSocketFound = TRUE;

                break;
            }
            else if ( pMyObject->ControlFlags & ANSC_BETO_FLAG_NO_TIMEOUT )
            {
                continue;
            }
            else if ( ((ulCurTime - pSocket->LastRecvAt) >= pServer->SocketTimeOut) &&
                      ((ulCurTime - pSocket->LastSendAt) >= pServer->SocketTimeOut) )
            {
                bSocketFound = TRUE;

                break;
            }
        }

        AnscReleaseLock(&pMyObject->SocketTableLock);

        if ( bSocketFound )
        {
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        ANSC_BSTOWO_EVENT_TIME_OUT,
                        (ANSC_HANDLE)NULL
                    );

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
