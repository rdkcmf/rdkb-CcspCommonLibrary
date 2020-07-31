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

    module:	ansc_deuo_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoRecv
        *   AnscDeuoSend

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
        AnscDeuoRecv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to receive data on a socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )pSocket->hPacket;

    pSocket->RecvBytesCount += pPacket->RecvPacketSize;
    pSocket->LastRecvAt      = AnscGetTickInSeconds();

    /*
     * We have successfully transferred the received data into the buffer supplied by the socket
     * owener though may not use up the while buffer. Now is time to notify our loyal socket
     * owner about this exciting event.
     */
    returnStatus =
        pSocket->Recv
            (
                (ANSC_HANDLE)pSocket,
                pPacket->RecvBuffer,
                pPacket->RecvPacketSize
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSend
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to send out data on a socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be processed.

                PVOID                       buffer
                Specifies the packet buffer to be processed.

                ULONG                       ulSize
                Specifies the size of the packet buffer.

                ANSC_HANDLE                 hReserved
                Specifies the opaque handle associated with the buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    int                             s_result     = 0;
    ansc_socket_addr_in             to_addr;
    xskt_socket_addr_in             xskt_to_addr;

    if ( pSocket->bClosed )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if ( pServer->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            xskt_to_addr.sin_family                                 = XSKT_SOCKET_AF_INET;
            ((pansc_socket_addr_in)&xskt_to_addr)->sin_addr.s_addr  = pSocket->PeerAddress.Value;
            xskt_to_addr.sin_port                                   = _xskt_htons(pSocket->PeerPort);
        }
        else
        {
            to_addr.sin_family      = ANSC_SOCKET_AF_INET;
            to_addr.sin_addr.s_addr = pSocket->PeerAddress.Value;
            to_addr.sin_port        = _ansc_htons(pSocket->PeerPort);
        }
    }

    if ( pMyObject->ControlFlags & ANSC_DEUO_FLAG_ASYNC_SEND )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if ( pServer->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            s_result = _xskt_sendto(pSocket->Socket, buffer, (int)ulSize, 0, (xskt_socket_addr*)&xskt_to_addr, sizeof(xskt_to_addr));
        }
        else
        {
            s_result = _ansc_sendto(pSocket->Socket, buffer, (int)ulSize, 0, (ansc_socket_addr*)&to_addr, sizeof(to_addr));
        }

        if ( s_result == ANSC_SOCKET_ERROR )
        {
            _ansc_get_last_error();

            returnStatus =
                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        hReserved,
                        ANSC_STATUS_FAILURE
                    );

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        ANSC_DSUOWO_EVENT_SOCKET_ERROR,
                        hReserved
                    );

            if ( pServer->Mode & ANSC_DSUO_MODE_AUTO_CLOSE )
            {
                pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
            }

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            returnStatus =
                pWorker->SendComplete
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        hReserved,
                        ANSC_STATUS_SUCCESS
                    );

            pSocket->SendBytesCount += ulSize;
            pSocket->LastSendAt      = AnscGetTickInSeconds();

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }

    return  returnStatus;
}
