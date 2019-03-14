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

    module:	bwrm_fmo_internal_api.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bwrm File Manager
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


#ifndef  _BWRM_FMO_INTERNAL_API_
#define  _BWRM_FMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWRM_FMO_STATES.C
***********************************************************/

ANSC_STATUS
BwrmFmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmFmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwrmFmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmFmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWRM_FMO_OPERATION.C
***********************************************************/

ANSC_STATUS
BwrmFmoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmFmoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN BWRM_FMO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
BwrmFmoGetFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void**                      file_data,
        PULONG                      file_size
    );

ANSC_STATUS
BwrmFmoSetFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    );

ANSC_STATUS
BwrmFmoNewFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    );

ANSC_STATUS
BwrmFmoDelFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );


#endif
