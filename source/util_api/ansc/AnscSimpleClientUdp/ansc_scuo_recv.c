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

    module:	ansc_scuo_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Simple Client Udp Object.

        *   AnscScuoRecvTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/12/01    initial revision.

**********************************************************************/


#include "ansc_scuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoRecvTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the engage() function
        call to contain async packet-recv processing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;
    char*                           recv_buffer  = NULL;
    ULONG                           ulBufferSize = 0;
    int                             recv_size    = 0;
    int                             s_result     = 0;
    ansc_fd_set                     ansc_read_fd_set;
    ansc_timeval                    ansc_timeval;
    ansc_socket_addr_in             ansc_peer_addr;
    xskt_fd_set                     xskt_read_fd_set;
    xskt_timeval                    xskt_timeval;
    xskt_socket_addr_in             xskt_peer_addr;
    int                             addrlen;

    AnscTrace("AnscScuoRecvTask is activated ...!\n");

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
        if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(&xskt_read_fd_set);
            XSKT_SOCKET_FD_SET ((XSKT_SOCKET)pMyObject->Socket, &xskt_read_fd_set);

            xskt_timeval.tv_sec  = (ANSC_SCUO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            xskt_timeval.tv_usec = (ANSC_SCUO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(&ansc_read_fd_set);
            ANSC_SOCKET_FD_SET (pMyObject->Socket, &ansc_read_fd_set);

            ansc_timeval.tv_sec  = (ANSC_SCUO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            ansc_timeval.tv_usec = (ANSC_SCUO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }

        /*
         * The _ansc_select() function returns the total number of socket handles that are ready
         * and contained in the fd_set structures, zero if the time limit expired, or SOCKET_ERROR
         * if an error occurred. Upon return, the structures are updated to reflect the subset of
         * these sockets that meet the specified condition.
         */
        if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
        {
            s_result = _xskt_select(pMyObject->Socket + 1, &xskt_read_fd_set, NULL, NULL, &xskt_timeval);
        }
        else
        {
            s_result = _ansc_select(pMyObject->Socket + 1, &ansc_read_fd_set, NULL, NULL, &ansc_timeval);
        }

        if ( s_result == 0 )
        {
            continue;
        }
        else if ( ( (pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET) && (s_result == XSKT_SOCKET_ERROR)) ||
                  (!(pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET) && (s_result == ANSC_SOCKET_ERROR)) )
        {

            if ( pMyObject->bActive )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }

            continue;
        }
        else if ( !pMyObject->bActive )
        {
            break;
        }

        /*
         * The underlying socket layer has indicated that there's data available to be processed,
         * we first retrieve the data into our own buffer and then query the external worker object
         * for further instructions.
         */
        recv_buffer =
            pMyObject->GetRecvBuffer
                (
                    (ANSC_HANDLE)pMyObject,
                    &ulBufferSize
                );

        if ( !recv_buffer )
        {
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCUOWO_EVENT_RESOURCES,
                        (ANSC_HANDLE)NULL
                    );

            break;
        }
        else
        {
            recv_size = (int)ulBufferSize;
        }

        if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
        {
            xskt_peer_addr.sin_family                                   = XSKT_SOCKET_AF_INET;
            ((pansc_socket_addr_in)&xskt_peer_addr)->sin_addr.s_addr    = pMyObject->PeerAddress.Value;
            xskt_peer_addr.sin_port                                     = _xskt_htons(pMyObject->PeerPort);
            addrlen                                                     = sizeof(xskt_peer_addr);

            s_result = _xskt_recvfrom((XSKT_SOCKET)pMyObject->Socket, recv_buffer, recv_size, 0, (xskt_socket_addr*)&xskt_peer_addr, (socklen_t *)&addrlen);
        }
        else
        {
            ansc_peer_addr.sin_family      = ANSC_SOCKET_AF_INET;
            ansc_peer_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
            ansc_peer_addr.sin_port        = _ansc_htons(pMyObject->PeerPort);
            addrlen                        = sizeof(ansc_peer_addr);

            s_result = _ansc_recvfrom(pMyObject->Socket, recv_buffer, recv_size, 0, (ansc_socket_addr*)&ansc_peer_addr, (socklen_t *)&addrlen);
        }

        if ( ( (pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET) && (s_result == XSKT_SOCKET_ERROR)) ||
             (!(pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET) && (s_result == ANSC_SOCKET_ERROR)) )
        {

            /*
            if ( pMyObject->bActive )
            {
                returnStatus =
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }
            */

            continue;
        }
        else if ( !pMyObject->bActive )
        {
            break;
        }
        else if ( s_result == 0 )
        {
            continue;
        }
        else
        {
            recv_size = s_result;
        }

        pMyObject->RecvBytesCount += (ULONG)recv_size;
        pMyObject->LastRecvAt      = AnscGetTickInSeconds();

        /*
         * Some old protocols (e.g., TFTP) and old implementations allows the use of a different
         * source port number after the UDP connection is established. To loosely support such
         * scenario, we always update the 'peer_port' field with the newest source port number.
         */
        if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
        {
            if ( xskt_peer_addr.sin_port != _xskt_htons(pMyObject->PeerPort) )
            {
                pMyObject->PeerPort = _xskt_ntohs(xskt_peer_addr.sin_port);
            }

            if ( (((pansc_socket_addr_in)&xskt_peer_addr)->sin_addr.s_addr != 0                           ) &&
                 (((pansc_socket_addr_in)&xskt_peer_addr)->sin_addr.s_addr != pMyObject->PeerAddress.Value) )
            {
                pMyObject->PeerAddress.Value = ((pansc_socket_addr_in)&xskt_peer_addr)->sin_addr.s_addr;
            }
        }
        else
        {
            if ( ansc_peer_addr.sin_port != _ansc_htons(pMyObject->PeerPort) )
            {
                pMyObject->PeerPort = _ansc_ntohs(ansc_peer_addr.sin_port);
            }

            if ( (ansc_peer_addr.sin_addr.s_addr != 0                           ) &&
                 (ansc_peer_addr.sin_addr.s_addr != pMyObject->PeerAddress.Value) )
            {
                pMyObject->PeerAddress.Value = ansc_peer_addr.sin_addr.s_addr;
            }
        }

        /*
         * We have successfully transferred the received data into the buffer supplied by the data
         * owener though may not use up the whole buffer. Now is time to notify our loyal socket
         * owner about this exciting event.
         */
            pMyObject->Recv
                (
                    (ANSC_HANDLE)pMyObject,
                    recv_buffer,
                    (ULONG)recv_size
                );
    }

    return  ANSC_STATUS_SUCCESS;
}
