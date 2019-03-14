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

    module:	bwrm_cpo_internal_api.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bwrm Cooked Page
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/02/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_CPO_INTERNAL_API_
#define  _BWRM_CPO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWRM_CPO_STATES.C
***********************************************************/

char*
BwrmCpoGetRootPath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoSetRootPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path
    );

char*
BwrmCpoGetPagePath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoSetPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_HANDLE
BwrmCpoGetOwnerReserved
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoSetOwnerReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
BwrmCpoGetPageContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoSetPageContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ULONG
BwrmCpoGetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoSetTimestamp
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timestamp
    );

ANSC_STATUS
BwrmCpoIncRefCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoDecRefCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
BwrmCpoGetRefCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmCpoReset
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWRM_CPO_OPERATION.C
***********************************************************/

BOOL
BwrmCpoMatchPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    );


#endif
