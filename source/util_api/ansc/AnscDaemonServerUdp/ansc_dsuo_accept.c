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

    module:	ansc_dsuo_accept.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoAcceptTask

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


#include "ansc_dsuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoAcceptTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the engage() function
        call to handle incoming socket requests..

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject     = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pCurEngine    = NULL;
    PANSC_DSUO_PACKET_OBJECT        pNewPacket    = NULL;
    int                             s_result      = 0;
    char*                           recv_buffer   = NULL;
    int                             recv_size     = 0;
    ansc_fd_set                     read_fd_set;
    xskt_fd_set                     read_fd_set2;
    ansc_timeval                    timeval;
    xskt_timeval                    timeval2;
    ansc_socket_addr_in             client_addr;
    xskt_socket_addr_in             client_addr2;
    int                             addrlen;

    AnscTrace("AnscDsuoAcceptTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bActive )
    {
        ANSC_COMMIT_TASK();
        
        /*
         * Since the original bsd compatible socket api doesn't support asynchronous operation, the
         * nonblocking status polling is the best we can get. As a matter of fact, the current unix
         * and linux actually still don't support asynchronous notification on any socket operation.
         */

        /*
         * Since only one socket is included in the fd_set, we only distinguish the result between
         * one and non-one values. If error is detected, we shall close the socket and notify the
         * socket owner immediately.
         */
        if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(&read_fd_set2);
            XSKT_SOCKET_FD_SET ((XSKT_SOCKET)pMyObject->Socket, &read_fd_set2);
            timeval2.tv_sec  = (ANSC_DSUO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval2.tv_usec = (ANSC_DSUO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */

            s_result = _xskt_select(pMyObject->Socket + 1, &read_fd_set2, NULL, NULL, &timeval2);
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(&read_fd_set);
            ANSC_SOCKET_FD_SET (pMyObject->Socket, &read_fd_set);
            timeval.tv_sec  = (ANSC_DSUO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval.tv_usec = (ANSC_DSUO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */

            s_result = _ansc_select(pMyObject->Socket + 1, &read_fd_set, NULL, NULL, &timeval);
        }

        if ( s_result == 0 )
        {
            continue;
        }
        else if ( s_result == ANSC_SOCKET_ERROR )
        {

            continue;
        }

        /*
         * According to de facto standards of bsd compatible socket api, if the socket is currently
         * in the listen state, it will be marked as readable if an incoming connection request has
         * been received such that an accept is guaranteed to complete without blocking.
         */

        pNewPacket =
            (PANSC_DSUO_PACKET_OBJECT)pMyObject->AcquirePacket
                (
                    (ANSC_HANDLE)pMyObject
                );

        if ( !pNewPacket )
        {
            continue;
        }
        else
        {
            recv_buffer = pNewPacket->RecvBuffer;
            recv_size   = pNewPacket->RecvBufferSize;
        }

        if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            client_addr2.sin_family                                 = ANSC_SOCKET_AF_INET;
            ((pansc_socket_addr_in)&client_addr2)->sin_addr.s_addr  = 0;
            client_addr2.sin_port                                   = _xskt_htons(pMyObject->HostPort);
            addrlen                                                 = sizeof(client_addr2);
            s_result = _xskt_recvfrom(pMyObject->Socket, recv_buffer, recv_size, 0, (xskt_socket_addr*)&client_addr2, (socklen_t *)&addrlen);
        }
        else
        {
            client_addr.sin_family      = ANSC_SOCKET_AF_INET;
            client_addr.sin_addr.s_addr = 0;
            client_addr.sin_port        = _ansc_htons(pMyObject->HostPort);
            addrlen                     = sizeof(client_addr);
            s_result = _ansc_recvfrom(pMyObject->Socket, recv_buffer, recv_size, 0, (ansc_socket_addr*)&client_addr, (socklen_t *)&addrlen);
        }

        if ( s_result == ANSC_SOCKET_ERROR )
        {

            returnStatus =
                pMyObject->ReleasePacket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pNewPacket
                    );

            continue;
        }
        else if ( !pMyObject->bActive )
        {
            returnStatus =
                pMyObject->ReleasePacket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pNewPacket
                    );

            break;
        }
        else
        {
            recv_size = s_result;
        }

        if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            pNewPacket->PeerAddress.Value = ((pansc_socket_addr_in)&client_addr2)->sin_addr.s_addr;
            pNewPacket->PeerPort          = _ansc_ntohs(client_addr2.sin_port);
        }
        else
        {
            pNewPacket->PeerPort          = _ansc_ntohs(client_addr.sin_port);
            pNewPacket->PeerAddress.Value = client_addr.sin_addr.s_addr;
        }
        pNewPacket->RecvPacketSize    = (ULONG)recv_size;
        pNewPacket->RecvAt            = AnscGetTickInSeconds();

        /*
         * We have to assign an engine object to this new packet object. The assignment is done by
         * address-hashing. However, we need to make sure that the assigned engine object is not
         * over-loaded. If it is, there're nothing we can do except throwing the packet away.
         */
        pCurEngine =
            (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->AssignEngine
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pNewPacket
                );

        if ( !pCurEngine )
        {
            returnStatus =
                pMyObject->ReleasePacket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pNewPacket
                    );

            continue;
        }
        else
        {
            returnStatus =
                pCurEngine->AddPacket
                    (
                        (ANSC_HANDLE)pCurEngine,
                        (ANSC_HANDLE)pNewPacket
                    );
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            returnStatus =
                pMyObject->ReleasePacket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pNewPacket
                    );
        }
    }

    AnscSetEvent(&pMyObject->AcceptEvent);

    return  ANSC_STATUS_SUCCESS;
}
