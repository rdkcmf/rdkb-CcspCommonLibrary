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

    module:	bwrm_pmo_internal_api.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bwrm Page Manager
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


#ifndef  _BWRM_PMO_INTERNAL_API_
#define  _BWRM_PMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWRM_PMO_STATES.C
***********************************************************/

BOOL
BwrmPmoGetCacheEnabled
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoSetCacheEnabled
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

ULONG
BwrmPmoGetCacheEntryCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoSetCacheEntryCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

ULONG
BwrmPmoGetCacheMemorySize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoSetCacheMemorySize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       size
    );

ULONG
BwrmPmoGetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoSetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

ANSC_STATUS
BwrmPmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmPmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmPmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWRM_PMO_OPERATION.C
***********************************************************/

ANSC_STATUS
BwrmPmoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoCacheTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN BWRM_PMO_MANAGEMENT.C
***********************************************************/

ULONG
BwrmPmoGetPageCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BwrmPmoGetOldestPage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BwrmPmoGetPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );

ANSC_STATUS
BwrmPmoAddPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        ANSC_HANDLE                 hCookedPage
    );

ANSC_STATUS
BwrmPmoDelPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );

ANSC_STATUS
BwrmPmoDelAllPages
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
