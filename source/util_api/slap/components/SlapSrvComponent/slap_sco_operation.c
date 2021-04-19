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

    module:	slap_sco_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Srv Component Object.

        *   SlapScoEngage
        *   SlapScoCancel
        *   SlapScoMatch1
        *   SlapScoInvokeDispatch
        *   SlapScoConvertParams

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


#include "slap_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapScoMatch1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                ULONG                       instance_id
            );

    description:

        This function is called to match the specified obj_name and
        instance_id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj name to be matched.

                ULONG                       instance_id
                Specifies the instance_id to be matched.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapScoMatch1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ULONG                       instance_id
    )
{
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject    = (PSLAP_SRV_COMPONENT_OBJECT)hThisObject;

    if ( !obj_name || !pMyObject->ObjName )
    {
        return  FALSE;
    }
    else if ( !strcmp(obj_name,pMyObject->ObjName) == 0 )
    {
        return  FALSE;
    }
    else if ( instance_id != 0 )
    {
        if ( pMyObject->ObjInstanceId != instance_id )
        {
            return  FALSE;
        }
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoInvokeDispatch
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var
            );

    description:

        This function is called to invoke a runtime service method.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the object method to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the list of output parameters to be returned.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoInvokeDispatch
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(method_name);
    UNREFERENCED_PARAMETER(params_in);
    UNREFERENCED_PARAMETER(params_out);
    UNREFERENCED_PARAMETER(return_var);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoConvertParams
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        tpl_params,
                SLAP_PARAMETER_LIST*        tbc_params
            );

    description:

        This function is called to convert parameters according to
        the standard syntax and data type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        tpl_params
                Specifies the list of parameters to be used as the
                templates for conversion.

                SLAP_PARAMETER_LIST*        tbc_params
                Specifies the list of parameters to be converted into
                the formats defined by tpl_params.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoConvertParams
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        tpl_params,
        SLAP_PARAMETER_LIST*        tbc_params
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject         = (PSLAP_SRV_COMPONENT_OBJECT)hThisObject;
    PSLAP_BSS_INTERFACE             pSlapBssIf        = (PSLAP_BSS_INTERFACE       )pMyObject->hSlapBssIf;
    PSLAP_VAR_CONVERTER_OBJECT      pSlapVarConverter = (PSLAP_VAR_CONVERTER_OBJECT)pSlapBssIf->GetVarConverter(pSlapBssIf->hOwnerContext);
    ULONG                           i                 = 0;

    if ( !pSlapVarConverter )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else if ( !tpl_params || !tbc_params )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < tpl_params->ParamCount; i++ )
    {
        returnStatus =
            pSlapVarConverter->ConvertVariable
                (
                    (ANSC_HANDLE)pSlapVarConverter,
                    &tpl_params->ParamArray[i],
                    &tbc_params->ParamArray[i]
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;
}
