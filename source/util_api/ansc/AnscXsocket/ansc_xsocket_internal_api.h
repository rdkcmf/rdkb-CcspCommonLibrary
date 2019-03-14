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

    module:	ansc_xsocket_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the xsocket wrapper.

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


#ifndef  _ANSC_XSOCKET_INTERNAL_API_
#define  _ANSC_XSOCKET_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_XSOCKET_STATE.C
***********************************************************/

ULONG
AnscXsocketGetType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

ULONG
AnscXsocketGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

ULONG
AnscXsocketGetIpProtocol
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetIpProtocol
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipprotocol
    );

PUCHAR
AnscXsocketGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

PUCHAR
AnscXsocketGetHostName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetHostName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      hostname
    );

USHORT
AnscXsocketGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

xskt_addrinfo*
AnscXsocketGetHostAddrInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetHostAddrInfo
    (
        ANSC_HANDLE                 hThisObject,
        xskt_addrinfo*              pAddrInfo
    );

PUCHAR
AnscXsocketGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

PUCHAR
AnscXsocketGetPeerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetPeerName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      peername
    );

USHORT
AnscXsocketGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

xskt_addrinfo*
AnscXsocketGetPeerAddrInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetPeerAddrInfo
    (
        ANSC_HANDLE                 hThisObject,
        xskt_addrinfo*              pAddrInfo
    );

UCHAR
AnscXsocketGetTransportType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetTransportType
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       transportType
    );

ANSC_HANDLE
AnscXsocketGetXsink
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSetXsink
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsink
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_XSOCKET_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscXsocketResolveAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketApplyDSCP
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulDSCP
    );

ANSC_STATUS
AnscXsocketBind
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketConnect
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketListen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    );

ANSC_STATUS
AnscXsocketSend2
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    );

ANSC_STATUS
AnscXsocketOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsocketClose
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_XSOCKET_ACCEPT.C
***********************************************************/

ANSC_STATUS
AnscXsocketAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_XSOCKET_RECV.C
***********************************************************/

ANSC_STATUS
AnscXsocketRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SINK_BASE.C
***********************************************************/

ANSC_HANDLE
AnscXsinkCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscXsinkRemove
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SINK_STATE.C
***********************************************************/

ANSC_HANDLE
AnscXsinkGetXsocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXsinkSetXsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    );

ANSC_STATUS
AnscXsinkAttach
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    );

ANSC_STATUS
AnscXsinkDetach
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_SINK_OPERATION.C
***********************************************************/

PVOID
AnscXsinkGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_HANDLE                phRecvHandle,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXsinkAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNewXsocket
    );

ANSC_STATUS
AnscXsinkRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecvHandle,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscXsinkClose
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bByPeer
    );

ANSC_STATUS
AnscXsinkAbort
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
