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

    module:	ansc_deuo_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoRecvTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/10/01    initial revision.

**********************************************************************/


#include "ansc_deuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoRecvTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the start() function
        call to contain async packet-recv processing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject     = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer       = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_WORKER_OBJECT        pWorker       = (PANSC_DSUO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket       = NULL;
    PANSC_DSUO_PACKET_OBJECT        pPacket       = NULL;
    ULONG                           ulLastCleanAt = AnscGetTickInSeconds();

    AnscTrace("AnscDeuoRecvTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bStarted )
    {
        ANSC_COMMIT_TASK();
        
        /*
         * To avoid letting the old half-dead sockets hogging up the system resource, we need to
         * periodically invoke the cleaning routine. The default interval is 10 seconds, and the
         * idle timeout value is 90 seconds.
         */
        if ( (AnscGetTickInSeconds() - ulLastCleanAt) >= ANSC_DEUO_CLEAN_TASK_INTERVAL )
        {
            pMyObject->Clean((ANSC_HANDLE)pMyObject);

            ulLastCleanAt = AnscGetTickInSeconds();
        }

        /*
         * If the reason we're there is that we run of packet objects to serve, we shall stop and
         * take a nap. Unlike the Daemon Server Tcp Object, where engines are balanced via round-
         * robin scheduling, Udp Server uses address-hashing to pick an engine. There're some slim
         * chances that some engines may be overloaded while the others have nothing to do. By
         * sleeping for very short time, idle engines can release the underlying OS scheduler from
         * switching back and forth.
         */
        if ( AnscQueueQueryDepth(&pMyObject->PacketQueue) == 0 )
        {
            if ( pServer->Mode & ANSC_DSUO_MODE_EVENT_SYNC )
            {
                AnscWaitEvent (&pMyObject->NewPacketEvent, ANSC_DEUO_WAIT_EVENT_INTERVAL);
                AnscResetEvent(&pMyObject->NewPacketEvent);

                if ( AnscQueueQueryDepth(&pMyObject->PacketQueue) == 0 )
                {
                    AnscTaskRelinquish();

                    continue;
                }
            }
            else
            {
                AnscSleep(ANSC_DEUO_TASK_BREAK_INTERVAL);

                continue;
            }
        }

        /*
         * Every engine maintains its own packet waiting queue. The main accept() task controlled
         * by the server is responsible for consistently pumping packet objects and distributing
         * them equally among engines. To obey the first-come-first-serve rule, we pop packet out
         * of the waiting queue from the head and associate it with a socket object.
         */
        while (( pPacket =
                    (PANSC_DSUO_PACKET_OBJECT)pMyObject->GetPacket
                        (
                            (ANSC_HANDLE)pMyObject
                        )) )
        {
            if ( (AnscGetTickInSeconds() - pPacket->RecvAt) > pMyObject->PacketTimeOut )
            {
                    pServer->ReleasePacket
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pPacket
                        );

                continue;
            }

            pSocket =
                (PANSC_DAEMON_SOCKET_UDP_OBJECT)pMyObject->GetSocket
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPacket->PeerAddress.Dot,
                        pPacket->PeerPort
                    );

            if ( !pSocket )
            {
                pSocket =
                    (PANSC_DAEMON_SOCKET_UDP_OBJECT)pServer->AcquireSocket
                        (
                            (ANSC_HANDLE)pServer
                        );

                if ( !pSocket )
                {
                        pServer->ReleasePacket
                            (
                                (ANSC_HANDLE)pServer,
                                (ANSC_HANDLE)pPacket
                            );

                    break;
                }

                pSocket->Socket            = pServer->Socket;
                pSocket->HostAddress.Value = pServer->HostAddress.Value;
                pSocket->HostPort          = pServer->HostPort;
                pSocket->PeerAddress.Value = pPacket->PeerAddress.Value;
                pSocket->PeerPort          = pPacket->PeerPort;
                pSocket->LastRecvAt        = AnscGetTickInSeconds();

                /*
                 * Before we hand this socket object to one of the daemon engines, we SHALL give the worker
                 * object a chance to examine the incoming socket request. A bunch of features can further
                 * be implemented within this call:
                 *
                 *      $ authentication based on the IP address and port number
                 *      $ establish session relationships
                 *
                 * However, the original intention was to give the worker an opportunity to associate some
                 * session-specific information with this socket.
                 */
                if ( !pWorker->Accept
                        (
                            pWorker->hWorkerContext,
                            (ANSC_HANDLE)pSocket,
                            &pSocket->hClientContext
                        ) )
                {
                    pSocket->Close ((ANSC_HANDLE)pSocket);
                    pSocket->Return((ANSC_HANDLE)pSocket);

                        pServer->ReleasePacket
                            (
                                (ANSC_HANDLE)pServer,
                                (ANSC_HANDLE)pPacket
                            );

                    continue;
                }

                    pMyObject->AddSocket
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );
            }

            if ( pSocket->bRecvEnabled )
            {
                    pSocket->SetPacket
                        (
                            (ANSC_HANDLE)pSocket,
                            (ANSC_HANDLE)pPacket
                        );

                    pMyObject->Recv
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );
            }
            else
            {
                    pServer->ReleasePacket
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pPacket
                        );
            }

            if ( !pMyObject->bStarted )
            {
                break;
            }
        }
    }

    AnscSetEvent(&pMyObject->RecvEvent);

    return  ANSC_STATUS_SUCCESS;
}
