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

    module:	slap_lco_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Loam Client
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_LCO_INTERNAL_API_
#define  _SLAP_LCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_LCO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapLcoGetSlapLoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapLcoGetSlapEnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapLcoGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoSetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcConnector
    );

BOOL
SlapLcoGetMobilityOn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoSetMobilityOn
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bMobilityOn
    );

ANSC_STATUS
SlapLcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapLcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_LCO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapLcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoPatrolTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_LCO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SlapLcoGetObjectStubByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
SlapLcoGetObjectStubByHandle
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    );

ANSC_STATUS
SlapLcoAddObjectStub
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle,
        char*                       obj_name,
        char*                       lpc_party_name,
        ANSC_HANDLE                 lpc_party_addr
    );

ANSC_STATUS
SlapLcoDelObjectStub
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    );

ANSC_STATUS
SlapLcoDelAllObjectStubs
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_LCO_UTILITIES.C
***********************************************************/

ANSC_STATUS
SlapLcoLpcToLoaImcpInvoAnswer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpAnswer
    );

ANSC_HANDLE
SlapLcoLpcToLoaSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_type,
        char*                       obj_name,
        SLAP_PARAMETER_LIST*        import_param_list
    );

ANSC_STATUS
SlapLcoLoaToLpcImcpInvoCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpCall
    );

ANSC_HANDLE
SlapLcoLoaToLpcSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle
    );

ANSC_STATUS
SlapLcoLoaToUoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapLcoUoaToLoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapLcoLoaToUoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

ANSC_STATUS
SlapLcoUoaToLoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_LCO_UOAIF.C
***********************************************************/

ANSC_HANDLE
SlapLcoLoaAcqContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        char*                       container_name
    );

ANSC_STATUS
SlapLcoLoaRelContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer
    );

ANSC_STATUS
SlapLcoLoaLocateObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char**                      lpc_party_name,
        ANSC_HANDLE*                lpc_party_addr
    );

ANSC_STATUS
SlapLcoLoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapLcoLoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        BOOL                        bTranslate,
        ULONG                       ulTimeout
    );

ANSC_HANDLE
SlapLcoLoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

ANSC_STATUS
SlapLcoLoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapLcoLoaRelObjectAccess2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapLcoLoaFlushAllObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapLcoLoaEnrollMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapLcoLoaUnbindMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapLcoLoaVerifyMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );


#endif
