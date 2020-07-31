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

    module:	slap_scostd_property.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object property-access
        functions of the Slap Sco Standard Object.

        *   SlapScoStdGetProperty
        *   SlapScoStdSetProperty

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/09/03    initial revision.

**********************************************************************/


#include "slap_scostd_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoStdGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve a pre-registered object
        property value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoStdGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject        = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY    )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST     )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE           )NULL;
    char*                           property_name    = (char*                    )NULL;

    if ( !param_list )
    {
        return  NULL;
    }
    else if ( param_list->ParamCount < 1 )
    {
        return  NULL;
    }
    else if ( (param_list->ParamArray[0].Syntax            != SLAP_VAR_SYNTAX_string) ||
              (param_list->ParamArray[0].Variant.varString == NULL                  ) )
    {
        return  NULL;
    }
    else
    {
        property_name = param_list->ParamArray[0].Variant.varString;
    }

    if ( TRUE )
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }
    else
    {
        SlapShiftLeftParamList(param_list, 1);
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->GetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoStdSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure a pre-registered object
        property value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoStdSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject        = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY    )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST     )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE           )NULL;
    char*                           property_name    = (char*                    )NULL;

    if ( !param_list )
    {
        return  NULL;
    }
    else if ( param_list->ParamCount < 1 )
    {
        return  NULL;
    }
    else if ( (param_list->ParamArray[0].Syntax            != SLAP_VAR_SYNTAX_string) ||
              (param_list->ParamArray[0].Variant.varString == NULL                  ) )
    {
        return  NULL;
    }
    else
    {
        property_name = param_list->ParamArray[0].Variant.varString;
    }

    if ( TRUE )
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }
    else
    {
        SlapShiftLeftParamList(param_list, 1);
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->SetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}
