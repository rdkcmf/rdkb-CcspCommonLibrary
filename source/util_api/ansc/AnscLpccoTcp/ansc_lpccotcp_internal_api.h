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

    module:	ansc_lpccotcp_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Lpcco Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_LPCCOTCP_INTERNAL_API_
#define  _ANSC_LPCCOTCP_INTERNAL_API_


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_STATES.C
***********************************************************/

PUCHAR
AnscLpccoTcpGetMyAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpSetMyAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscLpccoTcpGetMyPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpSetMyPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

PUCHAR
AnscLpccoTcpGetManagerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpSetManagerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscLpccoTcpGetManagerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpSetManagerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ANSC_STATUS
AnscLpccoTcpReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpEnroll
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpUnbind
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpConnectToManager
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_IMCIF.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpImcCallRequest
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    );

ANSC_STATUS
AnscLpccoTcpImcCallRequest2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_SEND.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpSendHello
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscLpccoTcpSendAck
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscLpccoTcpSendRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType
    );

ANSC_STATUS
AnscLpccoTcpSendReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType,
        ULONG                       ulRepCode
    );

ANSC_STATUS
AnscLpccoTcpSendBye
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_RECV.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );

ANSC_STATUS
AnscLpccoTcpRecvHello
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );

ANSC_STATUS
AnscLpccoTcpRecvAck
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );

ANSC_STATUS
AnscLpccoTcpRecvRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );

ANSC_STATUS
AnscLpccoTcpRecvReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );

ANSC_STATUS
AnscLpccoTcpRecvBye
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_DSUOWO.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpDwoInit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpDwoUnload
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
AnscLpccoTcpDwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
AnscLpccoTcpDwoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscLpccoTcpDwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
AnscLpccoTcpDwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
AnscLpccoTcpDwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscLpccoTcpDwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscLpccoTcpDwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
AnscLpccoTcpDwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_LPCCOTCP_BSUOWO.C
***********************************************************/

ANSC_STATUS
AnscLpccoTcpBwoInit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoTcpBwoUnload
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
AnscLpccoTcpBwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
AnscLpccoTcpBwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
AnscLpccoTcpBwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
AnscLpccoTcpBwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscLpccoTcpBwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscLpccoTcpBwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
AnscLpccoTcpBwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
