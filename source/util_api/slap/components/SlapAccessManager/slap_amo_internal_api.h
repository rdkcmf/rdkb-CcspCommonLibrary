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

    module:	slap_amo_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Access Manager
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_AMO_INTERNAL_API_
#define  _SLAP_AMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_AMO_STATES.C
***********************************************************/

ANSC_HANDLE
SlapAmoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapAmoGetSlapPoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapAmoGetSlapEnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapAmoGetSlapLoamClient
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapAmoGetSlapLoamServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapAmoGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapAmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SlapAmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_AMO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapAmoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_AMO_UTILITIES.C
***********************************************************/

ANSC_STATUS
SlapAmoGoaToUoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapAmoUoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    );

ANSC_STATUS
SlapAmoGoaToUoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

ANSC_STATUS
SlapAmoUoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    );

ANSC_STATUS
SlapAmoGoaToLoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapAmoLoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    );

ANSC_STATUS
SlapAmoGoaToLoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

ANSC_STATUS
SlapAmoLoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_AMO_GOAIF.C
***********************************************************/

ULONG
SlapAmoGoaGetAggregationMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGoaSetAggregationMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

USHORT
SlapAmoGoaGetLpcPartyPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGoaSetLpcPartyPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ULONG
SlapAmoGoaGetLpcPartyAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGoaSetLpcPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipaddr
    );

ULONG
SlapAmoGoaGetLpcManagerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGoaSetLpcManagerAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipaddr
    );

ANSC_HANDLE
SlapAmoGoaCreateContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    );

ANSC_STATUS
SlapAmoGoaDeleteContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

ANSC_HANDLE
SlapAmoGoaCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    );

ANSC_STATUS
SlapAmoGoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapAmoGoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

ANSC_STATUS
SlapAmoGoaInvokeObject2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        ULONG                       ulTimeout
    );

ANSC_STATUS
SlapAmoGoaAttachObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapContainer
    );

ANSC_STATUS
SlapAmoGoaDetachObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapAmoGoaUnlinkObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_HANDLE
SlapAmoGoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

ANSC_STATUS
SlapAmoGoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_STATUS
SlapAmoGoaFlushAllObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapAmoGoaEnrollObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapAmoGoaUnbindObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapAmoGoaVerifyObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
SlapAmoGoaEnrollMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapAmoGoaUnbindMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapAmoGoaVerifyMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_AMO_POAIF.C
***********************************************************/

ANSC_HANDLE
SlapAmoPoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
SlapAmoPoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

ANSC_HANDLE
SlapAmoPoaCreateProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 hSlapGoaObj
    );

ANSC_STATUS
SlapAmoPoaEnrollProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
SlapAmoPoaUnbindProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

BOOL
SlapAmoPoaVerifyProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );


#endif
