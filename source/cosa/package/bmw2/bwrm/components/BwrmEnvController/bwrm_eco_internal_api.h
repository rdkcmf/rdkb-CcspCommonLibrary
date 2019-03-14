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

    module:	bwrm_eco_internal_api.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bwrm Env Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_ECO_INTERNAL_API_
#define  _BWRM_ECO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWRM_ECO_STATES.C
***********************************************************/

ANSC_HANDLE
BwrmEcoGetBwrmRamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BwrmEcoGetBwrmFileManager
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BwrmEcoGetBwrmPageManager
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmEcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmEcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWRM_ECO_OPERATION.C
***********************************************************/

ANSC_STATUS
BwrmEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWRM_ECO_RAMIF.C
***********************************************************/

ULONG
BwrmEcoRamGetCacheNumber
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoRamSetCacheNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_number
    );

ULONG
BwrmEcoRamGetCacheSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoRamSetCacheSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_size
    );

ULONG
BwrmEcoRamGetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmEcoRamSetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_timeout
    );

ANSC_STATUS
BwrmEcoRamControlCache
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ctrl_code
    );

ANSC_STATUS
BwrmEcoRamGetRawPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        void**                      page_data,
        PULONG                      page_size
    );

ANSC_STATUS
BwrmEcoRamGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE*                phCookedPage
    );

ANSC_STATUS
BwrmEcoRamAddCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE                 hCookedPage
    );

ANSC_STATUS
BwrmEcoRamDelCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    );

BOOL
BwrmEcoRamIsPageCachedForced
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_STATUS
BwrmEcoRamForcePageCache
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        BOOL                        bCachdForced
    );

#endif
