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

    module:	ansc_deto_send.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoSendTask

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
        AnscDetoSendTask
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
AnscDetoSendTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DETO_PACKET_OBJECT        pPacket      = NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    BOOL                            bSendable    = FALSE;
    int                             s_result     = 0;

    AnscTrace("AnscDetoSendTask is activated ...!\n");

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
            pPacket = ACCESS_ANSC_DETO_PACKET_OBJECT(pSLinkEntry);
            pSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pPacket->hSocket;
        }

        AnscAcquireLock(&pMyObject->SendSocketSetLock);
        bSendable = (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)? XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) : ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1);
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

#ifdef _ANSC_USE_OPENSSL_
        if (TRUE)
        {
            int size = pPacket->PacketSize;
            unsigned char * buffer = pPacket->PacketBuffer;
            
            while ( size > 0)
            {
                if ( pSocket->bTlsEnabled )
                {
                    if ( !pSocket->bTlsConnected || !pSocket->hTlsConnection ) {
                        AnscTrace("%s - SSL connection is broken.\n", __FUNCTION__);
                        s_result = -1; /*XSKT_SOCKET_ERROR == ANSC_SOCKET_ERROR == -1*/
                        break;
                    }
                    s_result = openssl_write (pSocket->Socket, (char *)buffer, size, pSocket->hTlsConnection);
                }
                else
                {
                    s_result = _ansc_send (pSocket->Socket, (char *)buffer, size, 0);
                }

                if (s_result <= 0)
                {
                    AnscTrace("AnscSctoSend2 - send() returned error %d, %s\n", s_result, strerror (_ansc_get_last_error()));
                    break;
                }

                buffer += s_result;
                size -= s_result;
            }
        }
#else
        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            s_result = _xskt_send(((XSKT_SOCKET)pSocket->Socket), pPacket->PacketBuffer, (int)pPacket->PacketSize, 0);
        }
        else
        {
            s_result = _ansc_send(pSocket->Socket, pPacket->PacketBuffer, (int)pPacket->PacketSize, 0);
        }
#endif

        if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
             ((s_result == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) )
        {

                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        pPacket->hWorkerReserved,
                        ANSC_STATUS_FAILURE
                    );

            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
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

            /*
            pSocket->SendBytesCount += pPacket->PacketSize;
            pSocket->LastSendAt      = AnscGetTickInSecondsAbs();
            */
        }
    }

    AnscSetEvent(&pMyObject->SendEvent);

    return  ANSC_STATUS_SUCCESS;
}
