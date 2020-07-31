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

    module:	ansc_scuo_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Simple Client Udp Object.

        *   AnscScuoGetRecvBuffer
        *   AnscScuoRecv
        *   AnscScuoSend
        *   AnscScuoSend2

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

        PVOID
        AnscScuoGetRecvBuffer
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve a data buffer for holding
        incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulSize
                Specifies the size of the available buffer.

    return:     buffer pointer.

**********************************************************************/

PVOID
AnscScuoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  NULL;
    }
    else if ( !pMyObject->RecvBuffer )
    {
        return  NULL;
    }

    *pulSize = pMyObject->RecvBufferSize - pMyObject->RecvOffset;

    return  (PVOID)((ULONG)pMyObject->RecvBuffer + pMyObject->RecvOffset);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoRecv
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;
    ULONG                           ulPmode      = ANSC_SCUOWO_PMODE_DISCARD;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pMyObject->RecvPacketSize += ulSize;
    pMyObject->RecvOffset     += ulSize;

    ulPmode =
        pWorker->Query
            (
                pWorker->hWorkerContext,
                pMyObject->RecvBuffer,
                pMyObject->RecvPacketSize
            );

    switch ( ulPmode )
    {
        case    ANSC_SCUOWO_PMODE_DISCARD :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;

        case    ANSC_SCUOWO_PMODE_PROCESS :

                returnStatus =
                    pWorker->Process
                        (
                            pWorker->hWorkerContext,
                            pMyObject->RecvBuffer,
                            pMyObject->RecvPacketSize
                        );

                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;

        default :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoSend
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an outgoing packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;
    int                             s_result     = 0;
    ansc_socket_addr_in             ansc_to_addr;
    xskt_socket_addr_in             xskt_to_addr;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( pMyObject->PeerAddress.Value == 0 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        xskt_to_addr.sin_family                                 = XSKT_SOCKET_AF_INET;
        ((pansc_socket_addr_in)&xskt_to_addr)->sin_addr.s_addr  = pMyObject->PeerAddress.Value;
        xskt_to_addr.sin_port                                   = _xskt_htons(pMyObject->PeerPort);

        s_result = _xskt_sendto((XSKT_SOCKET)pMyObject->Socket, buffer, (int)ulSize, 0, (xskt_socket_addr*)&xskt_to_addr, sizeof(xskt_to_addr));

        if ( s_result == XSKT_SOCKET_ERROR )
        {
            _xskt_get_last_error();

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            pMyObject->SendBytesCount += ulSize;
            pMyObject->LastSendAt      = AnscGetTickInSeconds();

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        ansc_to_addr.sin_family      = ANSC_SOCKET_AF_INET;
        ansc_to_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        ansc_to_addr.sin_port        = _ansc_htons(pMyObject->PeerPort);

        s_result = _ansc_sendto(pMyObject->Socket, buffer, (int)ulSize, 0, (ansc_socket_addr*)&ansc_to_addr, sizeof(ansc_to_addr));

        if ( s_result == ANSC_SOCKET_ERROR )
        {
            _ansc_get_last_error();

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            pMyObject->SendBytesCount += ulSize;
            pMyObject->LastSendAt      = AnscGetTickInSeconds();

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoSend2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPeerAddr,
                USHORT                      usPeerPort,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an outgoing packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPeerAddr
                Specifies the peer IP address.

                USHORT                      usPeerPort
                Specifies the peer port number.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoSend2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPeerAddr,
        USHORT                      usPeerPort,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;
    int                             s_result     = 0;
    ansc_socket_addr_in             ansc_to_addr;
    xskt_socket_addr_in             xskt_to_addr;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        xskt_to_addr.sin_family                                 = XSKT_SOCKET_AF_INET;
        ((pansc_socket_addr_in)&xskt_to_addr)->sin_addr.s_addr  = ulPeerAddr;
        xskt_to_addr.sin_port                                   = _xskt_htons(usPeerPort);

        s_result = _xskt_sendto((XSKT_SOCKET)pMyObject->Socket, buffer, (int)ulSize, 0, (xskt_socket_addr*)&xskt_to_addr, sizeof(xskt_to_addr));

        if ( s_result == XSKT_SOCKET_ERROR )
        {
             _xskt_get_last_error();

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            pMyObject->SendBytesCount += ulSize;
            pMyObject->LastSendAt      = AnscGetTickInSeconds();

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        ansc_to_addr.sin_family      = ANSC_SOCKET_AF_INET;
        ansc_to_addr.sin_addr.s_addr = ulPeerAddr;
        ansc_to_addr.sin_port        = _ansc_htons(usPeerPort);

        s_result = _ansc_sendto(pMyObject->Socket, buffer, (int)ulSize, 0, (ansc_socket_addr*)&ansc_to_addr, sizeof(ansc_to_addr));

        if ( s_result == ANSC_SOCKET_ERROR )
        {
            _ansc_get_last_error();

            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCUOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            pMyObject->SendBytesCount += ulSize;
            pMyObject->LastSendAt      = AnscGetTickInSeconds();

            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }

    return  returnStatus;
}
