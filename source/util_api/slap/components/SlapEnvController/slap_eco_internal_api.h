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

    module:	slap_eco_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Env Controller
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


#ifndef  _SLAP_ECO_INTERNAL_API_
#define  _SLAP_ECO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_ECO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapEcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoGetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoGetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoGetSlapVarConverter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoGetSlapVarMapper
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoGetSlapDefContainer
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
SlapEcoGetPendingCallInfo
    (
        ANSC_HANDLE                 hThisObject,
        char**                      call_name,
        ULONG*                      call_timestamp,     /* in number of seconds after system is up */
        ULONG*                      call_age            /* in number of seconds */
    );

ANSC_STATUS
SlapEcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapEcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapEcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapEcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_ECO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapEcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_ECO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SlapEcoGetSlapContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

ANSC_STATUS
SlapEcoAddSlapContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

ANSC_STATUS
SlapEcoDelSlapContainer1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

ANSC_STATUS
SlapEcoDelSlapContainer2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

ANSC_STATUS
SlapEcoDelAllSlapContainers
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_ECO_UOAIF.C
***********************************************************/

ANSC_HANDLE
SlapEcoUoaCreateContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    );

ANSC_STATUS
SlapEcoUoaDeleteContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

ANSC_HANDLE
SlapEcoUoaAcqContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

ANSC_STATUS
SlapEcoUoaRelContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

ANSC_HANDLE
SlapEcoUoaCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    );

ANSC_STATUS
SlapEcoUoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapEcoUoaDeleteObject2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapEcoUoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

ANSC_HANDLE
SlapEcoUoaGetObjInsContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapEcoUoaSetObjInsContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hInsContext
    );

ULONG
SlapEcoUoaGetObjCallOption
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in
    );

ANSC_HANDLE
SlapEcoUoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

ANSC_STATUS
SlapEcoUoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapEcoUoaEnrollObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapEcoUoaUnbindObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapEcoUoaVerifyObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
SlapEcoUoaCreateProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hSlapGoaObj
    );

ANSC_STATUS
SlapEcoUoaEnrollProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapEcoUoaUnbindProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapEcoUoaVerifyProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_ECO_BSSIF.C
***********************************************************/

BOOL
SlapEcoBssGetPoolingFlag
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapEcoBssSetPoolingFlag
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bPooling
    );

ULONG
SlapEcoBssGetDefPoolSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapEcoBssSetDefPoolSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPoolSize
    );

ANSC_HANDLE
SlapEcoBssGetVarConverter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapEcoBssGetVarMapper
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
