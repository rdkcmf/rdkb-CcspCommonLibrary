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

    module:	slap_lso_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Loam Server
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


#ifndef  _SLAP_LSO_INTERNAL_API_
#define  _SLAP_LSO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_LSO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapLsoGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapLsoGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoSetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcConnector
    );

ANSC_HANDLE
SlapLsoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

BOOL
SlapLsoGetMobilityOn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoSetMobilityOn
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bMobilityOn
    );

ANSC_STATUS
SlapLsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_LSO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapLsoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLsoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN SLAP_LSO_PROCESS.C
***********************************************************/

ANSC_STATUS
SlapLsoProcessCallAcqc
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
SlapLsoProcessCallRelc
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
SlapLsoProcessCallLoco
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
SlapLsoProcessCallDelo
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
SlapLsoProcessCallInvo
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
SlapLsoProcessCallAcqo
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
SlapLsoProcessCallRelo
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
          FUNCTIONS IMPLEMENTED IN SLAP_LSO_CPCIF.C
***********************************************************/

ANSC_STATUS
SlapLsoCpcCallDispatch
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
SlapLsoCpcFreeOutputBuffer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pOutputBuffer
    );

ANSC_STATUS
SlapLsoCpcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
