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

    module:	http_psover2_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Pso Ver2
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_PSOVER2_INTERNAL_API_
#define  _HTTP_PSOVER2_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_PSOVER2_STATES.C
***********************************************************/

ANSC_HANDLE
HttpPsoVer2GetWamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoVer2GetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoVer2GetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoVer2GetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoVer2GetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoVer2GetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpPsoVer2GetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
HttpPsoVer2GetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ULONG
HttpPsoVer2GetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpPsoVer2GetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpPsoVer2GetSessionState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2SetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpPsoVer2Return
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2Reset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN HTTP_PSOVER2_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpPsoVer2Open
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2Close
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2AcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2ReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN HTTP_PSOVER2_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpPsoVer2AskTroBySocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpPsoVer2PopTroBySocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpPsoVer2GetLastTro
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoVer2GetCurTro
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoVer2AddNewTro
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoVer2DelAllTros
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_PSOVER2_CLIENT.C
***********************************************************/

ULONG
HttpPsoVer2QueryForClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoVer2RecvFromClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoVer2FinishedByClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

BOOL
HttpPsoVer2AcceptClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpPsoVer2Authenticate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );


#endif
