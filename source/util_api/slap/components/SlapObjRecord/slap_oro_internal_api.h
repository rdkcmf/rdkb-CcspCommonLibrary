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

    module:	slap_oro_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Obj Record
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


#ifndef  _SLAP_ORO_INTERNAL_API_
#define  _SLAP_ORO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_ORO_STATES.C
***********************************************************/


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_ORO_OPERATION.C
***********************************************************/

ULONG
SlapOroGetRef
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
SlapOroIncRef
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
SlapOroDecRef
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOroAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOroRelAccess
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
SlapOroMatch1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ULONG                       instance_id
    );

BOOL
SlapOroMatch2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hSrvComponent
    );


#endif
