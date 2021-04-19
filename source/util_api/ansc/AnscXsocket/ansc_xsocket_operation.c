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

    module:	ansc_xsocket_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xsocket Object.

        *   AnscXsocketResolveAddr
        *   AnscXsocketApplyDSCP
        *   AnscXsocketBind
        *   AnscXsocketConnect
        *   AnscXsocketListen
        *   AnscXsocketSend
        *   AnscXsocketOpen
        *   AnscXsocketClose

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/02    initial revision.

**********************************************************************/


#include "ansc_xsocket_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketResolveAddr
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketResolveAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject           = (PANSC_XSOCKET_OBJECT)hThisObject;
    xskt_addrinfo                   xskt_hints          = {0};
    USHORT                          usPort              = 0;
    char                            port[6]             = {0};
    xskt_addrinfo*                  pxskt_peer_addrinfo = NULL;
    char*                           pPort;
    errno_t                         rc                  = -1;

    if ( TRUE )
    {
        xskt_hints.ai_family   = AF_UNSPEC;
        /*xskt_hints.ai_socktype = XSKT_SOCKET_STREAM;*/
        xskt_hints.ai_flags    = AI_CANONNAME;

        usPort = pMyObject->GetPeerPort((ANSC_HANDLE)pMyObject);
        rc = sprintf_s(port, sizeof(port), "%d", usPort);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        /**
         * XXX: some uclibc version getaddrinfo cannot accept "0"
         * but NULL is acceptable.
         */
        if (strcmp(port, "0") == 0 )
            pPort = NULL;
        else
            pPort = port;

        CcspTraceInfo(("!!! Peer Port: %s !!!\n", port));

        CcspTraceInfo(("!!! Peer Name: %s !!!\n", pMyObject->GetPeerName((ANSC_HANDLE)pMyObject)));
        CcspTraceInfo(("!!! Host Name: %s !!!\n", pMyObject->GetHostName((ANSC_HANDLE)pMyObject)));

        if ( _xskt_getaddrinfo
                (
                    (const char *)pMyObject->GetPeerName((ANSC_HANDLE)pMyObject),
                    pPort,
                    &xskt_hints,
                    &pMyObject->pPeerAddrInfo
                ) ||
             _xskt_getaddrinfo
                (
                    (const char *)pMyObject->GetHostName((ANSC_HANDLE)pMyObject),
                    NULL,
                    &xskt_hints,
                    &pMyObject->pHostAddrInfo
                )

            )
        {
            CcspTraceError(("getaddrinfo returns error!\n"));

            return  ANSC_STATUS_FAILURE;
        }

        if ( !pMyObject->pPeerAddrInfo || !pMyObject->pHostAddrInfo )
        {
            CcspTraceError(("pPeerAddrInfo or pHostAddrInfo is NULL!\n"));

            return  ANSC_STATUS_FAILURE;
        }
        else
        {
            if (_xskt_getaddrinfo
                (
                   (const char *) pMyObject->GetPeerName((ANSC_HANDLE)pMyObject),
                    pPort,
                    &xskt_hints,
                    &pMyObject->pOriPeerAddrInfo
                ) != 0)
            {
                CcspTraceError(("!!! FAIL to get orig peer addr info !!!\n"));
                return  ANSC_STATUS_FAILURE;
            }

            pxskt_peer_addrinfo = pMyObject->pPeerAddrInfo;

            if ( pxskt_peer_addrinfo->ai_family == XSKT_SOCKET_AF_INET6 )
            {
                /*pMyObject->SetTransportType((ANSC_HANDLE)pMyObject, IP6_PROTOCOL_ICMP);*/
                pMyObject->SetIpProtocol((ANSC_HANDLE)pMyObject, XSKT_SOCKET_AF_INET6);
            }
            else if ( pxskt_peer_addrinfo->ai_family == XSKT_SOCKET_AF_INET )
            {
                pMyObject->SetIpProtocol((ANSC_HANDLE)pMyObject, XSKT_SOCKET_AF_INET);
            }
        }

        AnscTrace("!!! after getaddrinfo !!!\n");
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketApplyDSCP
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulDSCP
            );

    description:

        This function applies DSCP to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketApplyDSCP
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulDSCP
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject           = (PANSC_XSOCKET_OBJECT)hThisObject;
    int                             tos                 = 0;

    /* 6-bit DSCP, 2-bit ENC */
    tos = ulDSCP << 2;

    if ( pMyObject->IpProtocol == XSKT_SOCKET_AF_INET )
    {
        if ( _xskt_setsocketopt(pMyObject->Xsocket, XSKT_SOCKET_SOL_IPPROTO_IP, IP_TOS, &tos, sizeof(tos)) != 0 )
        {
            CcspTraceWarning(("Fail to set IPv4 DSCP!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
        }
    }
    else if ( pMyObject->IpProtocol == XSKT_SOCKET_AF_INET6 )
    {
        if ( _xskt_setsocketopt(pMyObject->Xsocket, IPPROTO_IPV6, IPV6_TCLASS, &tos, sizeof(tos)) !=0 )
        {
            CcspTraceWarning(("Fail to set IPv6 DSCP!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
        }
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketBind
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketBind
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject           = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder        = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;
    xskt_addrinfo*                  pxskt_peer_addrinfo = pMyObject->pPeerAddrInfo;
    xskt_addrinfo*                  pxskt_host_addrinfo = pMyObject->pHostAddrInfo;
    /*xskt_socket_addr_in             local_addr;*/

    switch ( pMyObject->Type )
    {
        case    ANSC_XSOCKET_TYPE_TCP :

                pMyObject->Xsocket = _xskt_socket(pxskt_peer_addrinfo->ai_family, XSKT_SOCKET_STREAM, 0);

                break;

        case    ANSC_XSOCKET_TYPE_UDP :

                pMyObject->Xsocket = _xskt_socket(pxskt_peer_addrinfo->ai_family, XSKT_SOCKET_DGRAM, 0);

                break;

        case    ANSC_XSOCKET_TYPE_RAW :

                pMyObject->Xsocket = _xskt_socket(pxskt_peer_addrinfo->ai_family, XSKT_SOCKET_RAWIP, pMyObject->TransportType);

                break;

        default :

                pMyObject->Xsocket = XSKT_SOCKET_INVALID_SOCKET;

                break;
    }

    if ( pMyObject->Xsocket == XSKT_SOCKET_INVALID_SOCKET )
    {
        AnscTraceError(("socket creation error: %s\n", strerror(errno)));

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->bClosed = FALSE;
    }
/*
    local_addr.sin_family = XSKT_SOCKET_AF_INET;
    local_addr.sin_port   = _xskt_htons(pMyObject->HostPort);

    if ( pMyObject->HostAddress.Value == 0 )
    {
        local_addr.sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
    }
    else
    {
        local_addr.sin_addr.s_addr = pMyObject->HostAddress.Value;
    }
*/
    if ( pMyObject->Type != ANSC_XSOCKET_TYPE_TCP )
    {
        _xskt_en_broadcast(pMyObject->Xsocket);
    }
    else
    {
        _xskt_en_reuseaddr(pMyObject->Xsocket);
    }

    /* TODO bind to different interfaces */
    if ( _xskt_bind(pMyObject->Xsocket, pxskt_host_addrinfo->ai_addr, pxskt_host_addrinfo->ai_addrlen) != 0 )
    {
        CcspTraceError(("bind error: %s\n", strerror(errno)));
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
    {
        if ( pXsinkHolder )
        {
            pXsinkHolder->Abort((ANSC_HANDLE)pXsinkHolder);
        }

        pMyObject->Remove((ANSC_HANDLE)pMyObject);

        returnStatus = ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketConnect
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketConnect
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject           = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder        = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;
    xskt_addrinfo*                  pxskt_peer_addrinfo = (xskt_addrinfo*      )pMyObject->pPeerAddrInfo;
    /*xskt_socket_addr_in             server_addr;*/
/*
    server_addr.sin_family      = XSKT_SOCKET_AF_INET;
    server_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
    server_addr.sin_port        = _xskt_htons(pMyObject->PeerPort);
*/
/*
    if ( _xskt_connect(pMyObject->Xsocket, (xskt_socket_addr*)&server_addr, sizeof(server_addr)) == 0 )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }
*/
    if ( _xskt_connect(pMyObject->Xsocket, pxskt_peer_addrinfo->ai_addr, pxskt_peer_addrinfo->ai_addrlen) == 0 )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
    {
        if ( pXsinkHolder )
        {
            pXsinkHolder->Abort((ANSC_HANDLE)pXsinkHolder);
        }

        pMyObject->Remove((ANSC_HANDLE)pMyObject);

        returnStatus = ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketListen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketListen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;

    /*
     * Only Tcp (stream type) xsockets support per-connection binding, hence supports listen()
     * operation. Udp xsockets should be able to receive data once the xsocket is created. Since we
     * want to provide a unified xsocket-based interface to both types of xsocket, we route the
     * Udp-based listen() request to open() instead.
     */
    if ( pMyObject->Type == ANSC_XSOCKET_TYPE_TCP )
    {
        if ( _xskt_listen(pMyObject->Xsocket, ANSC_XSOCKET_BACKLOG_VALUE) != 0 )
        {
            if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
            {
                if ( pXsinkHolder )
                {
                    pXsinkHolder->Abort((ANSC_HANDLE)pXsinkHolder);
                }

                pMyObject->Remove((ANSC_HANDLE)pMyObject);
            }

            return  ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        return  pMyObject->Open((ANSC_HANDLE)hThisObject);
    }

    /*
     * This member function should be called when the xsocket owner is ready to accept incoming
     * connections. Since we want to shield the owner from dealing with either synchronous
     * (nonblocking polling or blocking accept call) or asynchronous (callback notification as
     * in winsock2) xsocket operation, we implement a nonblocking polling-based accept mechanism
     * in a separate task. But of course, the underlying operating system has to support multi-
     * task programming. Fortunately, most real-time operating systems and PC-based operating
     * systems support multi-tasking.
     */
    returnStatus =
        pMyObject->SpawnTask
            (
                (ANSC_HANDLE)pMyObject,
                (void*      )pMyObject->AcceptTask,
                (ANSC_HANDLE)pMyObject,
                "xsocket_accept_task"
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketSend
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hAddress
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer holding the data to be sent.

                ULONG                       ulSize
                Specifies the size of the data buffer.

                ANSC_HANDLE                 hAddress
                Specifies the address information associated with peer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject       = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_SOCKET_ADDRESS            pXsocketAddress = (PANSC_SOCKET_ADDRESS)hAddress;
    xskt_socket_addr_in             to_addr;
    int				    s_result = 0;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock(&pMyObject->OpLock);

    if ( (pMyObject->Type == ANSC_XSOCKET_TYPE_TCP) || !pXsocketAddress )
    {
        to_addr.sin_family      = XSKT_SOCKET_AF_INET;
        to_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        to_addr.sin_port        = _xskt_htons(pMyObject->PeerPort);
    }
    else
    {
        to_addr.sin_family      = XSKT_SOCKET_AF_INET;
        to_addr.sin_addr.s_addr = pXsocketAddress->Address.Value;
        to_addr.sin_port        = _xskt_htons(pXsocketAddress->Port);
    }

    if ( pMyObject->Type == ANSC_XSOCKET_TYPE_TCP )
    {
	/*CID: 58486 Unchecked return value from library*/
        s_result = _xskt_send(pMyObject->Xsocket, buffer, (int)ulSize, 0);
	if ( s_result == XSKT_SOCKET_ERROR || s_result < (int)ulSize )
	{
	     CcspTraceError(("AnscXsocketSend:Failed to send request\n"));
	     AnscReleaseLock(&pMyObject->OpLock);
	     return ANSC_STATUS_FAILURE;
	}          
    }
    else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_UDP )
    {
	/*CID: 73772 Unchecked return value from library*/
        s_result = _xskt_sendto(pMyObject->Xsocket, buffer, (int)ulSize, 0, (xskt_socket_addr*)&to_addr, sizeof(to_addr));
    }
    else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_RAW )
    {
	 /*CID: 73772 Unchecked return value from library*/
        s_result = _xskt_sendto(pMyObject->Xsocket, buffer, (int)ulSize, 0, (xskt_socket_addr*)&to_addr, sizeof(to_addr));
    }

    if ( s_result < 0 )
    {
        CcspTraceError(("sendto error: %s\n", strerror(errno)));
    }

    AnscReleaseLock(&pMyObject->OpLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketSend2
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hAddress
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.
        AnscXsocketSend2 is IPv6 compatible.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer holding the data to be sent.

                ULONG                       ulSize
                Specifies the size of the data buffer.

                ANSC_HANDLE                 hAddress
                Specifies the address information associated with peer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketSend2
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject       = (PANSC_XSOCKET_OBJECT)hThisObject;
    xskt_addrinfo*                  pXsktAddrInfo   = (xskt_addrinfo*      )hAddress;
    int                             iSent           = 0;
    /*xskt_socket_addr_in             to_addr;*/

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock(&pMyObject->OpLock);
/*
    if ( (pMyObject->Type == ANSC_XSOCKET_TYPE_TCP) || !pXsocketAddress )
    {
        to_addr.sin_family      = XSKT_SOCKET_AF_INET;
        to_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        to_addr.sin_port        = _xskt_htons(pMyObject->PeerPort);
    }
    else
    {
        to_addr.sin_family      = XSKT_SOCKET_AF_INET;
        to_addr.sin_addr.s_addr = pXsocketAddress->Address.Value;
        to_addr.sin_port        = _xskt_htons(pXsocketAddress->Port);
    }
*/
/*
    to_addr.sin_family      = XSKT_SOCKET_AF_INET;
    to_addr.sin_addr.s_addr = inet_addr("64.104.123.144");
    to_addr.sin_port        = _xskt_htons(0);
*/

    if ( pMyObject->Type == ANSC_XSOCKET_TYPE_TCP )
    {
	/*CID: 65484 Unchecked return value from library*/
        iSent = _xskt_send(pMyObject->Xsocket, buffer, (int)ulSize, 0);
	if ( iSent == XSKT_SOCKET_ERROR || iSent  < (int)ulSize )
        {
             CcspTraceError(("AnscXsocketSend2: Failed to send request\n"));
             AnscReleaseLock(&pMyObject->OpLock);
             return ANSC_STATUS_FAILURE;
        }
    }
    else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_UDP )
    {
        iSent = _xskt_sendto(pMyObject->Xsocket, buffer, (int)ulSize, 0, pXsktAddrInfo->ai_addr, pXsktAddrInfo->ai_addrlen);
    }
    else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_RAW )
    {
        iSent = _xskt_sendto(pMyObject->Xsocket, buffer, (int)ulSize, 0, pXsktAddrInfo->ai_addr, pXsktAddrInfo->ai_addrlen);
    }

    if ( iSent < 0 )
    {
        CcspTraceError(("sendto error: %s\n", strerror(errno)));
    }

    AnscReleaseLock(&pMyObject->OpLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;

    /*
     * There's actually no corresponding bsd xsocket api function for this. This member function
     * should be called when the xsocket owner is ready to receive data. Since we want to shield
     * the owner from dealing with either synchronous (nonblocking polling or blocking receive)
     * or asynchronous (callback notification as in winsock2) xsocket operation, we implement an
     * universal nonblocking polling-based receive mechanism in a separate task. But of course,
     * the underlying operating system has to support multi-task programming. Fortunately, most
     * real-time operating systems and PC-based operating systems support multi-tasking.
     */
    returnStatus =
        pMyObject->SpawnTask
            (
                (ANSC_HANDLE)pMyObject,
                (void*      )pMyObject->RecvTask,
                (ANSC_HANDLE)pMyObject,
                "xsocket_recv_task"
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function routes the request to the real xsocket function
        provided by the underlying operating system.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bClosed = TRUE;
    }

    if ( pMyObject->Xsocket != XSKT_SOCKET_INVALID_SOCKET )
    {
        _xskt_shutdown   (pMyObject->Xsocket, XSKT_SOCKET_SD_SEND);
        _xskt_closesocket(pMyObject->Xsocket);

        pMyObject->Xsocket = XSKT_SOCKET_INVALID_SOCKET;
    }

    if ( pXsinkHolder )
    {
        pXsinkHolder->Close((ANSC_HANDLE)pXsinkHolder, FALSE);
    }

    return  ANSC_STATUS_SUCCESS;
}
