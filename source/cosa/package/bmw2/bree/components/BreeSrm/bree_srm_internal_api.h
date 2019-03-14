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

    module:	bree_srm_internal_api.h

        For BREE Site Resource Manager (BMW 2.0),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the BREE Site Resource
        Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/17/04    initial revision.


    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/


#ifndef  _BREE_SRM_INTERNAL_API
#define  _BREE_SRM_INTERNAL_API


ANSC_STATUS
BreeSrmoRegisterCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    );

ANSC_STATUS
BreeSrmoUnregisterCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    );

ANSC_HANDLE
BreeSrmoMapCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl
    );

ANSC_STATUS
BreeSrmoLoadCookedResources
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
