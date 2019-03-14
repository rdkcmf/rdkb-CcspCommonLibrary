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

    module:	dslh_varro_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Var Record
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/28/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_VARRO_INTERNAL_API_
#define  _DSLH_VARRO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_VARRO_STATES.C
***********************************************************/

char*
DslhVarroGetLastName
    (
        ANSC_HANDLE                 hThisObject
    );

char*
DslhVarroGetFullName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhVarroSetNotification
    (
        ANSC_HANDLE                 hThisObject,
        int                         iNotification
    );

ANSC_STATUS
DslhVarroSetAccessList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessList
    );

ANSC_STATUS
DslhVarroReset
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
DslhVarroIsRecordWritable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhVarroNotifyValueChanged
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_VARRO_CONTROL.C
***********************************************************/

ANSC_STATUS
DslhVarroLoadConfig
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhVarroSaveConfig
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_VARRO_ACCESS.C
***********************************************************/

SLAP_VARIABLE*
DslhVarroGetValue
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
DslhVarroTstValue
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    );

ANSC_STATUS
DslhVarroSetValue
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    );

ANSC_STATUS
DslhVarroSynValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhVarroSaveOldValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhVarroCommitChange
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs
    );

ANSC_STATUS
DslhVarroCancelChange
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_VARRO_UTILITY.C
***********************************************************/

BOOL
DslhVarroIsValidMacAddress
    (
        PCHAR						pAddress
    );

BOOL
DslhVarroIsValidIpAddList
    (
        PCHAR						pAddList
    );

BOOL
DslhVarroIsValidIpAddRangeList
    (
        PCHAR                       pAddRangeList
    );

BOOL
DslhVarroIsValidIp6AddList
    (
        PCHAR						pAddList
    );

BOOL
DslhVarroIsValidMacAddList
    (
        PCHAR						pAddList
    );

BOOL
DslhVarroIsValidHexString
    (
        PCHAR						pValue
    );

BOOL
DslhVarroDelSpaceInCommaSeparator
    (
        PCHAR                       pListString
    );

BOOL
DslhVarroIsValidCalendarString
    (
        PCHAR						pValue
    );

BOOL
DslhVarroIsValidBase64String
    (
        PCHAR						pString
    );

BOOL
DslhVarroIsValidUrlString
    (
        PCHAR                       pStringValue
    );

BOOL
DslhVarroIsValidMaskString
	(
		PCHAR						pStringValue
	);

char*
DslhVarroMacAddrListToString
    (
        SLAP_UCHAR_ARRAY*           mac_addr_list
    );

BOOL
DslhVarroDelSpaceInCommaSeparator
    (
        PCHAR                       pListString
    );
#endif
