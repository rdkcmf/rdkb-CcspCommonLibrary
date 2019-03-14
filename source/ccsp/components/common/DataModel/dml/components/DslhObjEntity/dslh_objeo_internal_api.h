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

    module:	dslh_objeo_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Obj Entity
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        01/11/2011    Bin added "GetVarEntity";
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#ifndef  _DSLH_OBJEO_INTERNAL_API_
#define  _DSLH_OBJEO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_OBJEO_STATES.C
***********************************************************/

ANSC_STATUS
DslhObjeoEnableForbidNotify
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

ANSC_STATUS
DslhObjeoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN DSLH_OBJEO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
DslhObjeoGetObjEntity2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_HANDLE
DslhObjeoGetObjEntity3
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjeoAddObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjEntity
    );

ANSC_STATUS
DslhObjeoDelObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

ANSC_STATUS
DslhObjeoAddVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity
    );

ANSC_STATUS
DslhObjeoDelVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pVarName
    );

ANSC_HANDLE
DslhObjeoGetVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );


ULONG
DslhObjeoGetObjRecordCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhObjeoGetObjRecordCount2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath
    );

ANSC_HANDLE
DslhObjeoGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_HANDLE
DslhObjeoGetObjRecord2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath,
        ULONG                       ulIndex
    );

ANSC_STATUS
DslhObjeoAddObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

ANSC_STATUS
DslhObjeoDelObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

ANSC_STATUS
DslhObjeoPopulateObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

ANSC_STATUS
DslhObjeoDeleteObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

#endif
