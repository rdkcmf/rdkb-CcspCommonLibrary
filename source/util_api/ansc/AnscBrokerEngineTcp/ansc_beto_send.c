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

    module:	ansc_beto_send.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Broker Engine Tcp Object.

        *   AnscBetoSendTask

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
        AnscBetoSendTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the start() function
        call to contain async packet-send processing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBetoSendTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BETO_PACKET_OBJECT        pPacket      = NULL;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    BOOL                            bSendable    = FALSE;
    int                             s_result     = 0;

    AnscTrace("AnscBetoSendTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bStarted )
    {
        AnscAcquireLock(&pMyObject->PacketQueueLock);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->PacketQueue);
        AnscReleaseLock(&pMyObject->PacketQueueLock);

        if ( !pSLinkEntry )
        {
            continue;
        }
        else
        {
            pPacket = ACCESS_ANSC_BETO_PACKET_OBJECT(pSLinkEntry);
            pSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)pPacket->hSocket;
        }

        AnscAcquireLock(&pMyObject->SendSocketSetLock);
        bSendable = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) : ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1);
        AnscReleaseLock(&pMyObject->SendSocketSetLock);

        if ( !bSendable )
        {
                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        pPacket->hWorkerReserved,
                        ANSC_STATUS_FAILURE
                    );

            AnscFreeMemory(pPacket);

            continue;
        }

        if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
        {
            s_result = _xskt_send(((XSKT_SOCKET)pSocket->Socket), pPacket->PacketBuffer, (int)pPacket->PacketSize, 0);
        }
        else
        {
            s_result = _ansc_send(pSocket->Socket, pPacket->PacketBuffer, (int)pPacket->PacketSize, 0);
        }

        if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
             ((s_result == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
        {

                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        pPacket->hWorkerReserved,
                        ANSC_STATUS_FAILURE
                    );

            if ( pServer->Mode & ANSC_BSTO_MODE_AUTO_CLOSE )
            {
                pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
            }
        }
        else
        {
                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        pPacket->hWorkerReserved,
                        ANSC_STATUS_SUCCESS
                    );

            pSocket->SendBytesCount += pPacket->PacketSize;
            pSocket->LastSendAt      = AnscGetTickInSeconds();
        }
    }

    AnscSetEvent(&pMyObject->SendEvent);

    return  ANSC_STATUS_SUCCESS;
}
