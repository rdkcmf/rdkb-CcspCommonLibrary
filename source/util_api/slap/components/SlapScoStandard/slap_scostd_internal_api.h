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

    module:	slap_scostd_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Sco Standard
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/08/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SCOSTD_INTERNAL_API_
#define  _SLAP_SCOSTD_INTERNAL_API_


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapScoStdInvokeDispatch
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_INVOCATION.C
***********************************************************/

void*
SlapScoStdInvokeFixInput0
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput3
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput4
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput5
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput6
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput7
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

void*
SlapScoStdInvokeFixInput8
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_SERVICES.C
***********************************************************/

SLAP_OBJECT
SlapScoStdCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

SLAP_OBJECT
SlapScoStdCreateObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        SLAP_HANDLE                 hInsContext
    );

ANSC_STATUS
SlapScoStdDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_OBJECT                 hSlapObject
    );

SLAP_OBJECT
SlapScoStdCloneObject
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_SERIALIZE.C
***********************************************************/

char*
SlapScoStdToString
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_PROPERTY.C
***********************************************************/

SLAP_VARIABLE*
SlapScoStdGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

SLAP_VARIABLE*
SlapScoStdSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_SCOSTD_CONTENT.C
***********************************************************/

ANSC_STATUS
SlapScoStdImportMe
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapScoStdExportMe
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST**       param_list
    );


#endif
