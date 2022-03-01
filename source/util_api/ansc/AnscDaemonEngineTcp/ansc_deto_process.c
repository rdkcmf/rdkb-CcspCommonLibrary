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

    module:	ansc_deto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoRecv
        *   AnscDetoRecv2
        *   AnscDetoSend
        *   AnscDetoSend2

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
        AnscDetoRecv
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
AnscDetoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulServingT1  = 0;
    ULONG                           ulServingT2  = 0;
    char*                           recv_buffer  = NULL;
    ULONG                           ulBufferSize = 0;
    int                             recv_size    = 0;
    int                             s_result     = 0;
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
    else
    {
        recv_size = (int)ulBufferSize;
    }

#ifdef _ANSC_USE_OPENSSL_
    if ( pSocket->bTlsEnabled )
    {
        /* wait for SSL to negotiate */
        if ( !pSocket->bTlsConnected || !pSocket->hTlsConnection ) {
            AnscSleep(100);
            return  ANSC_STATUS_FAILURE;
        }
        s_result = openssl_read (pSocket->Socket, recv_buffer, recv_size, pSocket->hTlsConnection);
    }
    else
    {
        s_result = _ansc_recv(pSocket->Socket, recv_buffer, recv_size, 0);
    }
#else
    if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        s_result = _xskt_recv((XSKT_SOCKET)pSocket->Socket, recv_buffer, recv_size, 0);
    }
    else
    {
        s_result = _ansc_recv(pSocket->Socket, recv_buffer, recv_size, 0);
    }
#endif

    if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
         ((s_result == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) )
    {
        s_error = (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

        AnscTrace(" !!! Daemon engine CANNOT recv packet, s_error = %d !!!\n", s_error);

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
                    ANSC_DSTOWO_EVENT_SOCKET_ERROR,
                    (ANSC_HANDLE)NULL
                );

        if ( pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }
    else if ( s_result == 0 )
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
                    ANSC_DSTOWO_EVENT_SOCKET_CLOSED,
                    (ANSC_HANDLE)NULL
                );

        if ( pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        recv_size = s_result;
    }

    pSocket->RecvBytesCount += (ULONG)recv_size;
    pSocket->LastRecvAt      = AnscGetTickInSecondsAbs();

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

    pMyObject->AvgServingTime =
        (pMyObject->AvgServingTime == 0)? (ulServingT2 - ulServingT1) : ((ulServingT2 - ulServingT1) + pMyObject->AvgServingTime * 7) / 8;
    pMyObject->TscCount++;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoRecv2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to receive TLS data on a socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoRecv2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

#ifndef _ANSC_USE_OPENSSL_
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pServer->GetTlsScsIf((ANSC_HANDLE)pServer);
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = (PANSC_BUFFER_DESCRIPTOR       )NULL;
    char*                           recv_buffer  = NULL;
    ULONG                           ulBufferSize = 0;
    int                             recv_size    = 0;
    int                             s_result     = 0;
    int                             s_error      = 0;

    if ( !pTlsScsIf )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pSocket->bTlsConnected && 
             !pSocket->GetRecvBuffer((ANSC_HANDLE)pSocket, &recv_size) )
    {
        /*
         * Because the DaemonTcp object series were NOT designed with SSL/TLS in mind, it's a bit
         * awkward to integrate TLS support without compromising the existing interfaces. The
         * consideration here is one example: we consider SSL/TLS negotiation is part of the socket
         * connection establishment process, so we don't notify the 'worker' until the handshake
         * has been completed. However since the 'worker' usually needs to set the foreign buffer
         * context in the accept() callback, we cannot process any incoming message until the
         * buffer setup is complete.
         */
        ULONG                       ulWaitCount = 0;

        while ( !pSocket->GetRecvBuffer((ANSC_HANDLE)pSocket, &recv_size) )
        {
            AnscSleep(10);

            ulWaitCount++;

            if ( ulWaitCount > 30 ) /* sleep for 300ms */
            {
                return  ANSC_STATUS_INTERNAL_ERROR;
            }
        }

        return  ANSC_STATUS_RESOURCES;
    }

    if ( !pSocket->hTlsConnection )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pTlsScsIf->QueryConnection(pTlsScsIf->hOwnerContext, pSocket->hTlsConnection) )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        pBufferDesp = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(TLS_MAX_RECORD_CHUNK_SIZE, 0, 0);

        if ( !pBufferDesp )
        {
            if ( pSocket->bTlsEnabled && pSocket->bTlsConnected )
            {
                returnStatus =
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            (ANSC_HANDLE)pSocket,
                            ANSC_DSTOWO_EVENT_RESOURCES,
                            (ANSC_HANDLE)NULL
                        );
            }

            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            recv_buffer = AnscBdoGetBuffer    (pBufferDesp);
            recv_size   = AnscBdoGetBufferSize(pBufferDesp);
        }
    }

    if ( !recv_buffer )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        s_result = _xskt_recv((XSKT_SOCKET)pSocket->Socket, recv_buffer, recv_size, 0);
    }
    else
    {
        s_result = _ansc_recv(pSocket->Socket, recv_buffer, recv_size, 0);
    }

    if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
         ((s_result == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) )
    {
        s_error = (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

        AnscTrace(" !!! Daemon engine CANNOT recv packet, s_error = %d !!!\n", s_error);

        AnscFreeBdo(pBufferDesp);

        if ( TRUE )
        {
            pSocket->bBroken = TRUE;

            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
        }

        if ( pSocket->bTlsEnabled && pSocket->bTlsConnected && !pSocket->bTlsInitializing )
        {
            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        ANSC_DSTOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );
        }
        else
        {
            AnscSetEvent(&pSocket->TlsConnEvent);
        }

        if ( (pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE) /*|| !pSocket->bTlsConnected*/ )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }
    else if ( s_result == 0 )
    {
        AnscFreeBdo(pBufferDesp);

        if ( pSocket->bTlsEnabled && pSocket->bTlsConnected && !pSocket->bTlsInitializing )
        {
            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        ANSC_DSTOWO_EVENT_SOCKET_CLOSED,
                        (ANSC_HANDLE)NULL
                    );
        }
        else
        {
            pSocket->bBroken = TRUE;

            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);

            AnscSetEvent(&pSocket->TlsConnEvent);
        }

        if ( (pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE) /*|| !pSocket->bTlsConnected*/ )
        {
            pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
        }

        pMyObject->TrcCount++;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        recv_size = s_result;
    }

    pBufferDesp->BlockSize += recv_size;
    returnStatus            =
        pTlsScsIf->RecvTlsMessage
            (
                pTlsScsIf->hOwnerContext,
                pSocket->hTlsConnection,
                (ANSC_HANDLE)pBufferDesp
            );

#endif
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSend
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
AnscDetoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;

    if ( pSocket->bClosed || pSocket->bBroken )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
#ifdef _ANSC_USE_OPENSSL_
    return  pMyObject->Send2
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSocket,
                    buffer,
                    ulSize,
                    hReserved
                );
#else
    else if ( !pSocket->bTlsEnabled )
    {
        pSocket->SendBytesCount += ulSize;
        pSocket->LastSendAt      = AnscGetTickInSecondsAbs();

        return  pMyObject->Send2
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pSocket,
                        buffer,
                        ulSize,
                        hReserved
                    );
    }
    else if ( !pSocket->bTlsConnected || !pServer->GetTlsScsIf((ANSC_HANDLE)pServer) || !pSocket->hTlsConnection )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pServer->GetTlsScsIf((ANSC_HANDLE)pServer);
        pBufferDesp = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(0, 0, 0);

        if ( !pBufferDesp )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscBdoSetOwner     (pBufferDesp, FALSE );
            AnscBdoSetBuffer    (pBufferDesp, buffer);
            AnscBdoSetBufferSize(pBufferDesp, ulSize);
            AnscBdoSetOffset    (pBufferDesp, 0     );
            AnscBdoSetBlockSize (pBufferDesp, ulSize);
        }

        pSocket->SendBytesCount += ulSize;
        pSocket->LastSendAt      = AnscGetTickInSecondsAbs();

        returnStatus =
            pTlsScsIf->SendAppMessage
                (
                    pTlsScsIf->hOwnerContext,
                    pSocket->hTlsConnection,
                    (ANSC_HANDLE)pBufferDesp
                );
    }
#endif
    
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSend2
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
AnscDetoSend2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DETO_PACKET_OBJECT        pPacket      = NULL;
    BOOL                            bSendable    = TRUE;
    int                             s_result     = 0;
    int                             s_error      = 0;

    if ( pSocket->bClosed || pSocket->bBroken )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        AnscAcquireLock(&pMyObject->SendSocketSetLock);
        bSendable = (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)? XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) : ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1);
        AnscReleaseLock(&pMyObject->SendSocketSetLock);

        if ( !bSendable )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
    }

    if ( pMyObject->ControlFlags & ANSC_DETO_FLAG_ASYNC_SEND )
    {
        pPacket = (PANSC_DETO_PACKET_OBJECT)AnscAllocateMemory(sizeof(ANSC_DETO_PACKET_OBJECT));

        if ( !pPacket )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        pPacket->hServer         = pMyObject->hDaemonServer;
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
#ifdef _ANSC_USE_OPENSSL_
        while ( ulSize > 0)
        {
            if ( pSocket->bTlsEnabled )
            {
                if ( !pSocket->bTlsConnected || !pSocket->hTlsConnection ) {
                    AnscTrace("AnscSctoSend2 - SSL connection is broken.\n");
                    returnStatus = ANSC_STATUS_FAILURE;
                    break;
                }
                s_result = openssl_write (pSocket->Socket, buffer, (int)ulSize, pSocket->hTlsConnection);
            }
            else
            {
                s_result = _ansc_send (pSocket->Socket, buffer, (int)ulSize, 0);
            }

            if (s_result <= 0)
            {
                AnscTrace("AnscSctoSend2 - send() returned error %d, %s\n", s_result, strerror (_ansc_get_last_error()));
                break;
            }

            buffer += s_result;
            ulSize -= s_result;
        }
#else
        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            s_result = _xskt_send(((XSKT_SOCKET)pSocket->Socket), buffer, (int)ulSize, 0);
        }
        else
        {
            s_result = _ansc_send(pSocket->Socket, buffer, (int)ulSize, 0);
        }
#endif

        if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
             ((s_result == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) )
        {
            s_error = (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

            AnscTrace(" !!! Daemon engine CANNOT send packet, s_error = %d !!!\n", s_error);

            if ( TRUE )
            {
                pSocket->bBroken = TRUE;

                pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            }

            if ( pSocket->bTlsEnabled && pSocket->bTlsConnected )
            {
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
                            ANSC_DSTOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }

            if ( (pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE) && pSocket->bTlsConnected )
            {
                pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
            }

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            if ( pSocket->bTlsEnabled && pSocket->bTlsConnected )
            {
                returnStatus =
                    pWorker->SendComplete
                        (
                            pWorker->hWorkerContext,
                            (ANSC_HANDLE)pSocket,
                            hReserved,
                            ANSC_STATUS_SUCCESS
                        );
            }

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }

    return  returnStatus;
}
