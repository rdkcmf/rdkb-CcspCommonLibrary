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

    module:	ansc_bkto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Broker Socket Tcp Object.

        *   AnscBktoFinish
        *   AnscBktoOpen
        *   AnscBktoClose

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


#include "ansc_bkto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoFinish
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the external Worker Object to close
        the socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoFinish
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->hBrokerEngine;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pEngine )
    {
        returnStatus =
            pEngine->DelSocket
                (
                    (ANSC_HANDLE)pEngine,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to initialize
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    int                             s_result     = 0;
    int                             addrlen      = 0;
    /*RDKB-6144, CID-24563, 24511, 24697, 24716; init before use*/
    ansc_socket_addr_in             host_addr1 = {0};
    ansc_socket_addr_in             peer_addr1 = {0};
    xskt_socket_addr_in             host_addr2 = {0};
    xskt_socket_addr_in             peer_addr2 = {0};

    if ( !pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->StartTime = AnscGetTickInSeconds();
        pMyObject->bClosed   = FALSE;
        pMyObject->bBroken   = FALSE;
    }

    if ( !pMyObject->RecvBuffer && !(pServer->Mode & ANSC_BSTO_MODE_FOREIGN_BUFFER) )
    {
        pMyObject->RecvBuffer     = AnscAllocateMemory(pMyObject->RecvBufferSize);
        pMyObject->RecvPacketSize = 0;
        pMyObject->RecvOffset     = 0;

        if ( !pMyObject->RecvBuffer )
        {
            pMyObject->bClosed = TRUE;

            return  ANSC_STATUS_RESOURCES;
        }
    }

    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = AnscGetTickInSeconds();
    pMyObject->LastSendAt     = AnscGetTickInSeconds();

    /*
     * Unlike the Daemon Socket Object, which is already opened by the accept() socket call, we
     * shall explicitly bind the Broker Socket to a local Tcp port and connect to the remote host
     * defined by the peer's address and port number.
     */
    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        pMyObject->Socket = (ANSC_SOCKET)_xskt_socket(XSKT_SOCKET_AF_INET, XSKT_SOCKET_STREAM, 0);
    }
    else
    {
        pMyObject->Socket = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_STREAM, 0);
    }

    if ( ((pMyObject->Socket == (ANSC_SOCKET)XSKT_SOCKET_INVALID_SOCKET) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
         ((pMyObject->Socket == ANSC_SOCKET_INVALID_SOCKET) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
    {
        return  ANSC_STATUS_FAILURE;
    }

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        host_addr2.sin_family = XSKT_SOCKET_AF_INET;
        host_addr2.sin_port   = _xskt_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            ((pansc_socket_addr_in)&host_addr2)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ((pansc_socket_addr_in)&host_addr2)->sin_addr.s_addr = pMyObject->HostAddress.Value;
        }
    }
    else
    {
        host_addr1.sin_family = ANSC_SOCKET_AF_INET;
        host_addr1.sin_port   = _ansc_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            host_addr1.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
        }
        else
        {
            host_addr1.sin_addr.s_addr = pMyObject->HostAddress.Value;
        }
    }

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        s_result = _xskt_bind(pMyObject->Socket, (xskt_socket_addr*)&host_addr2, sizeof(host_addr2));
    }
    else
    {
        s_result = _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&host_addr1, sizeof(host_addr1));
    }

    if ( s_result == 0 )
    {
        returnStatus = ANSC_STATUS_SUCCESS;

        if ( pMyObject->HostPort == 0 )
        {
            addrlen  = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? sizeof(host_addr2) : sizeof(host_addr1);
            s_result = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)?
                _xskt_getsocketname
                    (
                        pMyObject->Socket,
                        (xskt_socket_addr*)&host_addr2,
                        (unsigned int*)&addrlen
                    ) :
                _ansc_getsocketname
                    (
                        pMyObject->Socket,
                        (ansc_socket_addr*)&host_addr1,
                        (unsigned int*)&addrlen
                    );

            pMyObject->HostPort = (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)? _xskt_ntohs(host_addr2.sin_port) : _ansc_ntohs(host_addr1.sin_port);
        }
    }
    else
    {
        return  ANSC_STATUS_FAILURE;
    }

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        peer_addr2.sin_family                                = XSKT_SOCKET_AF_INET;
        ((pansc_socket_addr_in)&peer_addr2)->sin_addr.s_addr = pMyObject->PeerAddress.Value;
        peer_addr2.sin_port                                  = _xskt_htons(pMyObject->PeerPort);
    }
    else
    {
        peer_addr1.sin_family      = ANSC_SOCKET_AF_INET;
        peer_addr1.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        peer_addr1.sin_port        = _ansc_htons(pMyObject->PeerPort);
    }

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        s_result = _xskt_connect(pMyObject->Socket, (xskt_socket_addr*)&peer_addr2, sizeof(peer_addr2));
    }
    else
    {
        s_result = _ansc_connect(pMyObject->Socket, (ansc_socket_addr*)&peer_addr1, sizeof(peer_addr1));
    }

    if ( s_result == 0 )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else
    {
        return  ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to unload
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;

    if ( ((pMyObject->Socket != (ANSC_SOCKET)XSKT_SOCKET_INVALID_SOCKET) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
         ((pMyObject->Socket != (ANSC_SOCKET)ANSC_SOCKET_INVALID_SOCKET) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
    {
        if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
        {
            _xskt_shutdown   (pMyObject->Socket, XSKT_SOCKET_SD_SEND);
            _xskt_closesocket(pMyObject->Socket);
        }
        else
        {
            _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_SEND);
            _ansc_closesocket(pMyObject->Socket);
        }
    }

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bClosed = TRUE;
        pMyObject->bBroken = FALSE;
    }

    returnStatus =
        pWorker->Remove
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}
