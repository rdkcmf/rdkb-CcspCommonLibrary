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

    module:	slap_oeo_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Obj Entity
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/05/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OEO_INTERNAL_API_
#define  _SLAP_OEO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_OEO_STATES.C
***********************************************************/

ANSC_STATUS
SlapOeoSetObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescriptor
    );

ANSC_STATUS
SlapOeoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_OEO_TABLES.C
***********************************************************/

ANSC_STATUS
SlapOeoSetupOpoTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOeoSetupMdoTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOeoRegisterCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCallDescriptor
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_OEO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SlapOeoGetObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

ANSC_STATUS
SlapOeoAddObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapOeoDelObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

ANSC_STATUS
SlapOeoDelAllObjectProperties
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapOeoGetMethodDescriptor1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

ANSC_HANDLE
SlapOeoGetMethodDescriptor2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    );

ANSC_STATUS
SlapOeoAddMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

ANSC_STATUS
SlapOeoDelMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

ANSC_STATUS
SlapOeoDelAllMethodDescriptors
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapOeoGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ULONG                       instance_id
    );

ANSC_HANDLE
SlapOeoNewObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hInsContext
    );

ANSC_STATUS
SlapOeoDelObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hSrvComponent,
        BOOL                        bForceful
    );

ANSC_STATUS
SlapOeoDelAllObjRecords
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapOeoAcqPooledObjRecord
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapOeoRelPooledObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

ANSC_STATUS
SlapOeoDelAllPooledObjRecords
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_OEO_UTILITIES.C
***********************************************************/

ANSC_STATUS
SlapOeoResolveCallDescrI
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapOeoResolveCallDescrO
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapOeoResolveCallDescrR
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_VARIABLE*              return_var
    );

ULONG
SlapOeoResolveCallBridge
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

ULONG
SlapOeoResolveCallOption
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_OEO_OLAIF.C
***********************************************************/

ANSC_HANDLE
SlapOeoOlaGetObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

ANSC_HANDLE
SlapOeoOlaGetMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    );


#endif
