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

    module:	web_gso_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web General Session
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/18/02    initial revision.

**********************************************************************/


#ifndef  _WEB_GSO_INTERNAL_API_
#define  _WEB_GSO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_GSO_STATES.C
***********************************************************/

ANSC_HANDLE
WebGsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
WebGsoGetActiveSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetActiveSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
WebGsoGetSessionId
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetSessionId
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId
    );

PUCHAR
WebGsoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
WebGsoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ANSC_HANDLE
WebGsoGetLsmContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetLsmContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

char*
WebGsoGetLsmIdentifier
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetLsmIdentifier
    (
        ANSC_HANDLE                 hThisObject,
        char*                       identifier
    );

ULONG
WebGsoGetLsmMaxAge
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetLsmMaxAge
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
WebGsoGetLsmExpire
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetLsmExpire
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
WebGsoGetMaxLifespan
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoSetMaxLifespan
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ANSC_STATUS
WebGsoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
WebGsoGetAuthSessInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_GSO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebGsoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoTerminate
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoLock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoUnlock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebGsoResetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
