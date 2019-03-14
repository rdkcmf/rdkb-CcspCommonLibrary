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

    module:	slap_omo_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Obj Mapper
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OMO_INTERNAL_API_
#define  _SLAP_OMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_OMO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapOmoGetRootObjBroker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapOmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapOmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_OMO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapOmoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOmoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_OMO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SlapOmoMakObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_HANDLE
SlapOmoGetObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_HANDLE
SlapOmoGetObjEntity1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
SlapOmoGetObjEntity2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_STATUS
SlapOmoAddObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjEntity
    );

ANSC_STATUS
SlapOmoPopObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
SlapOmoDelObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjEntity
    );

ANSC_HANDLE
SlapOmoGetObjProxy1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
SlapOmoGetObjProxy2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_STATUS
SlapOmoAddObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjProxy
    );

ANSC_STATUS
SlapOmoPopObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
SlapOmoDelObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjProxy
    );


#endif
