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

    module:	slap_lbo_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Loam Broker
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_LBO_INTERNAL_API_
#define  _SLAP_LBO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_LBO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapLboGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapLboGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLboGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLboSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLboResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLboReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_LBO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapLboAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLboRelAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLboEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLboCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_LBO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SlapLboGetObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
SlapLboAddObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char*                       party_name
    );

ANSC_STATUS
SlapLboDelObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
SlapLboDelObjBinding2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

ANSC_STATUS
SlapLboDelAllObjBindings
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN SLAP_LBO_PROCESS.C
***********************************************************/

ANSC_STATUS
SlapLboAsyncCallTaskLoco
    (
        ANSC_HANDLE                 hCallInfo
    );

ANSC_STATUS
SlapLboProcessCallAcqc
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
SlapLboProcessCallRelc
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
SlapLboProcessCallLoco
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
SlapLboProcessCallDelo
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
SlapLboProcessCallInvo
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
SlapLboProcessCallAcqo
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
SlapLboProcessCallRelo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_LBO_CPCIF.C
***********************************************************/

ANSC_STATUS
SlapLboCpcCallDispatch
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
SlapLboCpcFreeOutputBuffer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pOutputBuffer
    );

ANSC_STATUS
SlapLboCpcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
