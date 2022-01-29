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

    module:	ansc_scuo_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Simple Client Udp Object.

        *   AnscScuoEngage
        *   AnscScuoCancel

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
        AnscScuoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;
    ansc_socket_addr_in             ansc_client_addr;
    ansc_socket_addr_in             ansc_any_addr;
    xskt_socket_addr_in             xskt_client_addr;
    xskt_socket_addr_in             xskt_remote_addr;
    xskt_socket_addr_in             xskt_any_addr;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        /*
         * Just like any other socket-based ANSC object, we will create a separate async recv task
         * which is dedicated to receiving packets. This async recv task is controlled by 'bActive'
         * flag. What if at this moment right before we're about to enable the socket operation and
         * setting 'bActive' flag to TRUE, the old recv task created by the last call of Engage()
         * is still running? While it may not cause crash, but it certainly confuses the owner
         * object because all async recv tasks share the same worker interface. The most obvious
         * solution is to wait for previous recv task to exit before creating a new one.
         */
        while ( pMyObject->EngineTaskCount != 0 )
        {
            AnscSleep(50);
        }

        pMyObject->bActive = TRUE;
        pMyObject->bClosed = FALSE;
    }

    /*
     * The underlying socket wrapper may require an explicit startup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscStartupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    /*
     * To engage the Udp Client, we need to perform following acts in the respective order:
     *
     *      (1) create the os-dependent socket
     *      (2) bind to the newly socket
     *      (3) allocate a buffer for receiving
     *      (4) spawn a separate thread and start receiving
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        pMyObject->Socket = (ANSC_SOCKET)_xskt_socket(XSKT_SOCKET_AF_INET, XSKT_SOCKET_DGRAM, 0);

        if ( (XSKT_SOCKET)pMyObject->Socket == XSKT_SOCKET_INVALID_SOCKET )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
    }
    else
    {
        pMyObject->Socket = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_DGRAM, 0);

        if ( pMyObject->Socket == ANSC_SOCKET_INVALID_SOCKET )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
    }

    /*
     * Normally we don't need to know which local network interface we shall bind to, and the
     * underlying operating system usually supports such notation as "any address".
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        xskt_client_addr.sin_family = XSKT_SOCKET_AF_INET;
        xskt_client_addr.sin_port   = _xskt_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            ((pansc_socket_addr_in)&xskt_client_addr)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ((pansc_socket_addr_in)&xskt_client_addr)->sin_addr.s_addr = pMyObject->HostAddress.Value;
        }

        xskt_remote_addr.sin_family = XSKT_SOCKET_AF_INET;
        xskt_remote_addr.sin_port   = _xskt_htons(pMyObject->PeerPort);

        if ( pMyObject->PeerAddress.Value == 0 )
        {
            ((pansc_socket_addr_in)&xskt_remote_addr)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ((pansc_socket_addr_in)&xskt_remote_addr)->sin_addr.s_addr = pMyObject->PeerAddress.Value;
        }

        xskt_any_addr.sin_family                                = XSKT_SOCKET_AF_INET;
        xskt_any_addr.sin_port                                  = _xskt_htons(pMyObject->HostPort);
        ((pansc_socket_addr_in)&xskt_any_addr)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
    }
    else
    {
        ansc_client_addr.sin_family = ANSC_SOCKET_AF_INET;
        ansc_client_addr.sin_port   = _ansc_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            ansc_client_addr.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ansc_client_addr.sin_addr.s_addr = pMyObject->HostAddress.Value;
        }


        if ( pMyObject->PeerAddress.Value == 0 )
        {
        }
        else
        {
        }

        ansc_any_addr.sin_family      = ANSC_SOCKET_AF_INET;
        ansc_any_addr.sin_port        = _ansc_htons(pMyObject->HostPort);
        ansc_any_addr.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
    }

    /*
     * Enable address sharing if it's multicast?
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        if ( AnscIpv4IsAddrClassD(pMyObject->PeerAddress.Dot) )
        {
            _xskt_en_reuseaddr(((XSKT_SOCKET)pMyObject->Socket));


            if ( _xskt_bind((XSKT_SOCKET)pMyObject->Socket, (xskt_socket_addr*)&xskt_any_addr, sizeof(xskt_any_addr)) != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT2;
            }

        }
        else
        {
            if ( _xskt_bind((XSKT_SOCKET)pMyObject->Socket, (xskt_socket_addr*)&xskt_client_addr, sizeof(xskt_client_addr)) != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT2;
            }
        }
    }
    else
    {
        if ( AnscIpv4IsAddrClassD(pMyObject->PeerAddress.Dot) )
        {
            _ansc_en_reuseaddr(pMyObject->Socket);


            if ( _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&ansc_any_addr, sizeof(ansc_any_addr)) != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT2;
            }

        }
        else
        {
            if ( _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&ansc_client_addr, sizeof(ansc_client_addr)) != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT2;
            }
        }
    }

    /*
     * If the specified 'PeerAddr' is a multicast address, we need to differentiate the actions
     * based on whether ANSC socket library is used. ANSC socket library supports the multicast
     * socket by allowing the applications to explicitly bind to the multicast IP address while
     * some other socket implementations require applications to configure some special multicast
     * options as proposed by Steve Deering.
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        if ( AnscIpv4IsAddrClassD(pMyObject->PeerAddress.Dot) )
        {
            _xskt_en_multicast(((XSKT_SOCKET)pMyObject->Socket), pMyObject->PeerAddress.Value, ((pansc_socket_addr_in)&xskt_client_addr)->sin_addr.s_addr);
        }
    }
    else
    {
        if ( AnscIpv4IsAddrClassD(pMyObject->PeerAddress.Dot) )
        {
            _ansc_en_multicast(pMyObject->Socket, pMyObject->PeerAddress.Value, ansc_client_addr.sin_addr.s_addr);
        }
    }

    /*
     * We have gone so far that all socket operations succeeded, we want to allocate a buffer that
     * is big enough for any incoming message.
     */
    if ( !pMyObject->RecvBuffer )
    {
        pMyObject->RecvBuffer     = AnscAllocateMemory(pMyObject->RecvBufferSize);
        pMyObject->RecvPacketSize = 0;
        pMyObject->RecvOffset     = 0;

        if ( !pMyObject->RecvBuffer )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
    }

    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = AnscGetTickInSeconds();
    pMyObject->LastSendAt     = AnscGetTickInSeconds();

    /*
     * To save the worker object from having to deal with blocking/non-blocking/async receiving
     * functions provided by underlying socket layer, we create a separate task to do that.
     */
    returnStatus =
        pMyObject->SpawnTask
            (
                (ANSC_HANDLE)pMyObject,
                (void*      )pMyObject->RecvTask,
                (ANSC_HANDLE)pMyObject,
                ANSC_SCUO_RECV_TASK_NAME
            );

    return  ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        _xskt_closesocket((XSKT_SOCKET)pMyObject->Socket);
    }
    else
    {
        _ansc_closesocket(pMyObject->Socket);
    }

EXIT1:

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pMyObject->bActive = FALSE;

        pMyObject->Reset((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscScuoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
        pMyObject->bClosed = TRUE;
    }

    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        if ( (XSKT_SOCKET)pMyObject->Socket != XSKT_SOCKET_INVALID_SOCKET )
        {
            _xskt_shutdown   ((XSKT_SOCKET)pMyObject->Socket, XSKT_SOCKET_SD_RECV);
            _xskt_closesocket((XSKT_SOCKET)pMyObject->Socket);

            pMyObject->Socket = (ANSC_SOCKET)XSKT_SOCKET_INVALID_SOCKET;
        }
    }
    else
    {
        if ( pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET )
        {
            _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_RECV);
            _ansc_closesocket(pMyObject->Socket);

            pMyObject->Socket = ANSC_SOCKET_INVALID_SOCKET;
        }
    }

    /*
     * The underlying socket wrapper may require an explicit cleanup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_SCUO_MODE_XSOCKET )
    {
        AnscCleanupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscCleanupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
