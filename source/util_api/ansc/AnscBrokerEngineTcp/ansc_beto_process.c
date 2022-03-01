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

    module:	ansc_beto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Broker Engine Tcp Object.

        *   AnscBetoRecv
        *   AnscBetoSend

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
        AnscBetoRecv
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
AnscBetoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulServingT1  = 0;
    ULONG                           ulServingT2  = 0;
    char*                           recv_buffer  = NULL;
    ULONG                           ulBufferSize = 0;
    int                             recv_size    = 0;
    int                             s_error      = 0;

    recv_buffer =
        pSocket->GetRecvBuffer
            (
                (ANSC_HANDLE)pSocket,
                &ulBufferSize
            );

    if ( !recv_buffer )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        recv_size = _xskt_recv((XSKT_SOCKET)pSocket->Socket, recv_buffer, ulBufferSize, 0);
    }
    else
    {
        recv_size = _ansc_recv(pSocket->Socket, recv_buffer, ulBufferSize, 0);
    }

    

    if ( ((recv_size == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
         ((recv_size == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
    {
        s_error = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

        AnscTrace(" !!! Broker engine CANNOT recv packet, s_error = %d !!!\n", s_error);

        if ( TRUE )
        {
            pSocket->bBroken = TRUE;

            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
        }

        returnStatus =
            pWorker->Notify
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pSocket,
                    ANSC_BSTOWO_EVENT_SOCKET_ERROR,
                    (ANSC_HANDLE)NULL
                );

        if ( pServer->Mode & ANSC_BSTO_MODE_AUTO_CLOSE )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }
    
    if ( recv_size == 0 )
    {
        /*
         * It seems a good idea to prevent any further sending and receiving on this socket after
         * the peer closes the connection. On Linux, calling send() after socket is closed will
         * kill the process (the signal processing doesn't seem to prevent process from dying).
         */
        if ( TRUE )
        {
            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
        }

        returnStatus =
            pWorker->Notify
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pSocket,
                    ANSC_BSTOWO_EVENT_SOCKET_CLOSED,
                    (ANSC_HANDLE)NULL
                );

        if ( pServer->Mode & ANSC_BSTO_MODE_AUTO_CLOSE )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }

    if ( recv_size <= 0 )
    {
        /*
         * We shall disable receiving and return if socket didn't indicate anything...
         */
        pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);

        return ANSC_STATUS_SUCCESS;
    }

    pSocket->RecvBytesCount += (ULONG)recv_size;
    pSocket->LastRecvAt      = AnscGetTickInSeconds();

    /*
     * We have successfully transferred the received data into the buffer supplied by the socket
     * owener though may not use up the while buffer. Now is time to notify our loyal socket
     * owner about this exciting event.
     */

    ulServingT1  = AnscGetTickInMilliSeconds();

    returnStatus =
        pSocket->Recv
            (
                (ANSC_HANDLE)pSocket,
                recv_buffer,
                (ULONG)recv_size
            );
    ulServingT2  = AnscGetTickInMilliSeconds();

    pMyObject->AvgServingTime = (pMyObject->AvgServingTime == 0)? (ulServingT2 - ulServingT1) : 
        ((ulServingT2 - ulServingT1) + pMyObject->AvgServingTime * 7) / 8;
    pMyObject->TscCount++;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoSend
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
AnscBetoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BETO_PACKET_OBJECT        pPacket      = NULL;
    BOOL                            bSendable    = TRUE;
    int                             s_result     = 0;
    int                             s_error      = 0;

    if ( pSocket->bClosed )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->SendSocketSetLock);
    bSendable = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) : ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1);
    AnscReleaseLock(&pMyObject->SendSocketSetLock);

    if ( !bSendable )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pMyObject->ControlFlags & ANSC_BETO_FLAG_ASYNC_SEND )
    {
        pPacket = (PANSC_BETO_PACKET_OBJECT)AnscAllocateMemory(sizeof(ANSC_BETO_PACKET_OBJECT));

        if ( !pPacket )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        pPacket->hServer         = pMyObject->hBrokerServer;
        pPacket->hEngine         = (ANSC_HANDLE)pMyObject;
        pPacket->hSocket         = (ANSC_HANDLE)pSocket;
        pPacket->hWorkerReserved = hReserved;
        pPacket->PacketBuffer    = buffer;
        pPacket->PacketSize      = ulSize;

        AnscAcquireLock   (&pMyObject->PacketQueueLock);
        AnscQueuePushEntry(&pMyObject->PacketQueue, &pPacket->Linkage);
        AnscReleaseLock   (&pMyObject->PacketQueueLock);

        returnStatus = ANSC_STATUS_PENDING;
    }
    else
    {
        if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
        {
            s_result = _xskt_send(((XSKT_SOCKET)pSocket->Socket), buffer, (int)ulSize, 0);
        }
        else
        {
            s_result = _ansc_send(pSocket->Socket, buffer, (int)ulSize, 0);
        }

        /*AnscTrace(" KERNEL AnscBetoSend send result %d\n",s_result);*/

        if ( s_result == ANSC_SOCKET_ERROR )
        {
            s_error = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

            AnscTrace(" !!! Broker engine CANNOT send packet, s_error = %d !!!\n", s_error);

            if ( TRUE )
            {
                pSocket->bBroken = TRUE;

                pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            }

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
                        ANSC_BSTOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );

            if ( pServer->Mode & ANSC_BSTO_MODE_AUTO_CLOSE )
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
