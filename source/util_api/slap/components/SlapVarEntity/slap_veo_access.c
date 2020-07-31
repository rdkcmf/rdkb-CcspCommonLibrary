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

    module:	slap_veo_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Var Entity Object.

        *   SlapVeoGetVarSyntax
        *   SlapVeoGetVarContentType
        *   SlapVeoSetVarContentType
        *   SlapVeoGetVarUsageType
        *   SlapVeoSetVarUsageType

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/20/03    initial revision.

**********************************************************************/


#include "slap_veo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVeoGetVarSyntax
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve the SLAP syntax type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     syntax type.

**********************************************************************/

ULONG
SlapVeoGetVarSyntax
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  pTargetVar   = (PSLAP_VARIABLE         )NULL;

    if ( !param_list )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else if ( param_list->ParamCount < 1 )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else
    {
        pTargetVar = &param_list->ParamArray[0];
    }

    return  pTargetVar->Syntax;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVeoGetVarContentType
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve the SLAP content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     content type.

**********************************************************************/

ULONG
SlapVeoGetVarContentType
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  pTargetVar   = (PSLAP_VARIABLE         )NULL;

    if ( !param_list )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else if ( param_list->ParamCount < 1 )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else
    {
        pTargetVar = &param_list->ParamArray[0];
    }

    return  pTargetVar->ContentType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVeoSetVarContentType
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure the SLAP content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVeoSetVarContentType
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_VAR_MAPPER_OBJECT         pMyObject      = (PSLAP_VAR_MAPPER_OBJECT)hThisObject;
    PSLAP_BSS_INTERFACE             pSlapBssIf     = (PSLAP_BSS_INTERFACE    )pMyObject->hSlapBssIf;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper = (PSLAP_VAR_MAPPER_OBJECT)pSlapBssIf->GetVarMapper(pSlapBssIf->hOwnerContext);
    PSLAP_VARIABLE                  pTargetVar     = (PSLAP_VARIABLE         )NULL;
    ULONG                           ulContentType  = (ULONG                  )SLAP_CONTENT_TYPE_UNSPECIFIED;
    char*                           pTypeString    = (char*                  )NULL;

    if ( !param_list )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( param_list->ParamCount < 2 )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( (param_list->ParamArray[1].Syntax != SLAP_VAR_SYNTAX_uint32) &&
              (param_list->ParamArray[1].Syntax != SLAP_VAR_SYNTAX_string) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( (param_list->ParamArray[1].Syntax            == SLAP_VAR_SYNTAX_string) &&
              (param_list->ParamArray[1].Variant.varString == NULL                  ) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pTargetVar = &param_list->ParamArray[0];
    }

    if ( param_list->ParamArray[1].Syntax == SLAP_VAR_SYNTAX_uint32 )
    {
        ulContentType = param_list->ParamArray[1].Variant.varUint32;
    }
    else if ( param_list->ParamArray[1].Syntax == SLAP_VAR_SYNTAX_string )
    {
        pTypeString   = param_list->ParamArray[1].Variant.varString;
        ulContentType =
            pSlapVarMapper->GetVarContentType
                (
                    (ANSC_HANDLE)pSlapVarMapper,
                    pTypeString
                );
    }

    pTargetVar->ContentType = ulContentType;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVeoGetVarUsageType
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve the SLAP usage type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     usage type.

**********************************************************************/

ULONG
SlapVeoGetVarUsageType
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  pTargetVar   = (PSLAP_VARIABLE         )NULL;

    if ( !param_list )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else if ( param_list->ParamCount < 1 )
    {
        return  SLAP_VAR_SYNTAX_other;
    }
    else
    {
        pTargetVar = &param_list->ParamArray[0];
    }

    return  pTargetVar->UsageType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVeoSetVarUsageType
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure the SLAP usage type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVeoSetVarUsageType
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  pTargetVar     = (PSLAP_VARIABLE         )NULL;
    ULONG                           ulUsageType    = (ULONG                  )SLAP_VAR_USAGE_TYPE_default;

    if ( !param_list )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( param_list->ParamCount < 2 )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( (param_list->ParamArray[1].Syntax != SLAP_VAR_SYNTAX_uint32) &&
              (param_list->ParamArray[1].Syntax != SLAP_VAR_SYNTAX_string) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( (param_list->ParamArray[1].Syntax            == SLAP_VAR_SYNTAX_string) &&
              (param_list->ParamArray[1].Variant.varString == NULL                  ) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pTargetVar = &param_list->ParamArray[0];
    }

    if ( param_list->ParamArray[1].Syntax == SLAP_VAR_SYNTAX_uint32 )
    {
        ulUsageType = param_list->ParamArray[1].Variant.varUint32;
    }
    else if ( param_list->ParamArray[1].Syntax == SLAP_VAR_SYNTAX_string )
    {
        ulUsageType = SLAP_VAR_USAGE_TYPE_default;
    }

    pTargetVar->UsageType = ulUsageType;

    return  ANSC_STATUS_SUCCESS;
}
