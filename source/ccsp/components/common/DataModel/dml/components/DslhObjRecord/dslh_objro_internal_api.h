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

    module:	dslh_objro_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Obj Record
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin  Zhu

    ---------------------------------------------------------------

    revision:

        09/26/2005    initial revision.
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#ifndef  _DSLH_OBJRO_INTERNAL_API_
#define  _DSLH_OBJRO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_OBJRO_STATES.C
***********************************************************/

ANSC_STATUS
DslhObjroSetDslhObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjEntity
    );

ANSC_STATUS
DslhObjroSetLastName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName
    );

ANSC_STATUS
DslhObjroSetFullName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName
    );

ANSC_STATUS
DslhObjroDestroy
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroReset
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_OBJRO_CONTROL.C
***********************************************************/

ANSC_STATUS
DslhObjroPopulateObjRecords
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroPopulateVarRecords
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroParamValueChanged
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhObjroChildObjectCreated
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildObjController,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
DslhObjroChildObjectRemoved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
DslhObjroPopulateObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

ANSC_STATUS
DslhObjroDeleteObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

/***********************************************************
      FUNCTIONS IMPLEMENTED IN DSLH_OBJRO_MANAGEMENT.C
***********************************************************/

ULONG
DslhObjroGetObjRecordCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhObjroGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_HANDLE
DslhObjroGetObjRecord2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_STATUS
DslhObjroAddObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

ANSC_STATUS
DslhObjroDelAllObjRecords
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhObjroGetVarRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bExcludeHidden
    );

ANSC_HANDLE
DslhObjroGetVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
DslhObjroAddVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_OBJRO_ACCESS.C
***********************************************************/

ULONG
DslhObjroGetTotalParamNumber
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs,
		BOOL						bSelfIncluded
    );

ANSC_STATUS
DslhObjroGetAllParamValues
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamValueArray,
        PULONG                      pulArraySize,
        BOOL                        bFromACS,
        unsigned int                writeID
    );

ANSC_STATUS
DslhObjroGetAllParamInfos
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamInfoArray,
        PULONG                      pulArraySize,
        BOOL                        bNextLevelOnly,
        BOOL                        bFromACS
    );

ANSC_STATUS
DslhObjroGetAllParamAttribs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamAttribArray,
        PULONG                      pulArraySize,
        BOOL                        bFromACS
    );

ANSC_STATUS
DslhObjroSetAllParamAttribs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSetParamAttrib
    );

BOOL
DslhObjroVerifyChanges
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppFaultParamName
    );

ANSC_STATUS
DslhObjroCommitChanges
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroCommitChanges2
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroCancelChanges
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjroAddChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulInstanceNumber
    );

ANSC_STATUS
DslhObjroDelChildObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_OBJRO_RVQIF.C
***********************************************************/

BOOL
DslhObjroRvqIsParamUpdated
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhObjroRvqClearParamUpdated
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

SLAP_VARIABLE*
DslhObjroRvqGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

char*
DslhObjroRvqGetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

int
DslhObjroRvqGetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ULONG
DslhObjroRvqGetParamValueUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

BOOL
DslhObjroRvqGetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

SLAP_UINT32_ARRAY*
DslhObjroRvqGetParamValueUint32Array
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

SLAP_UCHAR_ARRAY*
DslhObjroRvqGetParamValueUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_HANDLE
DslhObjroRvqGetParamValueHandle
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );


#endif
