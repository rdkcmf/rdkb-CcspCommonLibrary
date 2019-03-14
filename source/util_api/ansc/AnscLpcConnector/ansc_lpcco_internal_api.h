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

    module:	ansc_lpcco_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Lpc Connector Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/23/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_LPCCO_INTERNAL_API_
#define  _ANSC_LPCCO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_LPCCO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscLpccoGetAnscImcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscLpccoGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoSetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
AnscLpccoGetPartyAddrSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscLpccoGetCallTimeout
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoSetCallTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_LPCCO_MANAGEMENT.C
***********************************************************/

ULONG
AnscLpccoGetPartyCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoGetPartyArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE*                pPartyArray,
        PULONG                      pulCount
    );

ANSC_HANDLE
AnscLpccoGetPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

ANSC_HANDLE
AnscLpccoGetPartyAddr2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

ANSC_STATUS
AnscLpccoAddPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 party_addr
    );

ANSC_STATUS
AnscLpccoDelPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

ANSC_STATUS
AnscLpccoDelAllPartyAddrs
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscLpccoGetPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    );

ANSC_HANDLE
AnscLpccoPopPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        ULONG                       msg_type,
        ULONG                       seq_number,
        ULONG                       error_code,
        ANSC_HANDLE                 output_bdo
    );

ANSC_STATUS
AnscLpccoAddPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPendingCall
    );

ANSC_STATUS
AnscLpccoDelPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    );

ANSC_STATUS
AnscLpccoPopAllPendingCalls
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       error_code
    );

ANSC_STATUS
AnscLpccoDelAllPendingCalls
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_LPCCO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscLpccoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoEnroll
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoUnbind
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoConnectToManager
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoConnTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_LPCCO_IMCIF.C
***********************************************************/

ANSC_HANDLE
AnscLpccoImcGenPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ULONG
AnscLpccoImcGetLpcOpmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoImcSetLpcOpmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

char*
AnscLpccoImcGetPartyName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoImcSetPartyName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

BOOL
AnscLpccoImcIsPartyConnected
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        BOOL                        bOutgoingConn
    );

ANSC_STATUS
AnscLpccoImcEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoImcCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscLpccoImcCallRequest
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
AnscLpccoImcCallRequest2
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
         FUNCTIONS IMPLEMENTED IN ANSC_LPCCO_CPCIF.C
***********************************************************/

ANSC_STATUS
AnscLpccoCpcCallDispatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    );

ANSC_STATUS
AnscLpccoCpcFreeOutputBuffer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pOutputBuffer
    );

ANSC_STATUS
AnscLpccoCpcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
