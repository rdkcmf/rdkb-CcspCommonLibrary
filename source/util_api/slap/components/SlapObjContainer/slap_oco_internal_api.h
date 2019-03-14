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

    module:	slap_oco_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Obj Container
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/06/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OCO_INTERNAL_API_
#define  _SLAP_OCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_OCO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapOcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOcoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SlapOcoGetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOcoSetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SlapOcoGetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOcoSetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjMapper
    );

char*
SlapOcoGetContainerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOcoSetContainerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

ULONG
SlapOcoGetContainerType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOcoSetContainerType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       container_type
    );

ANSC_STATUS
SlapOcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_OCO_MANAGEMENT.C
***********************************************************/

ULONG
SlapOcoAssignInstanceId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
SlapOcoGetSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ULONG                       instance_id
    );

ANSC_STATUS
SlapOcoAddSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    );

ANSC_STATUS
SlapOcoDelSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    );

ANSC_STATUS
SlapOcoDelAllSrvComponents
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
