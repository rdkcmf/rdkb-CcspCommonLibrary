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

    module:	slap_oeo_utilities.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Slap Obj Entity Object.

        *   SlapOeoResolveCallDescrI
        *   SlapOeoResolveCallDescrO
        *   SlapOeoResolveCallDescrR
        *   SlapOeoResolveCallBridge
        *   SlapOeoResolveCallOption

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#include "slap_oeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoResolveCallDescrI
            (
                ANSC_HANDLE                 hThisObject,
                char*                       call_description,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to resolve the call description of
        input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       call_description
                Specifies the call description to be resolved.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the target parameter list to be filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoResolveCallDescrI
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject            = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_BSS_INTERFACE             pSlapBssIf           = (PSLAP_BSS_INTERFACE    )pMyObject->hSlapBssIf;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper       = (PSLAP_VAR_MAPPER_OBJECT)pSlapBssIf->GetVarMapper(pSlapBssIf->hOwnerContext);
    PANSC_TOKEN_CHAIN               pTokenChainParamList = NULL;
    PANSC_TOKEN_CHAIN               pTokenChainParamDesp = NULL;
    PANSC_STRING_TOKEN              pTokenParamDesp      = NULL;
    PANSC_STRING_TOKEN              pTokenSyntax         = NULL;
    PANSC_STRING_TOKEN              pTokenContentType    = NULL;
    ULONG                           ulParamCount         = 0;

    if ( !pSlapVarMapper )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        SlapCleanParamList(param_list);
        SlapInitParamList (param_list);
    }

    pTokenChainParamList =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                call_description,
                SLAP_CALL_PARAMETER_LIST_SEPARATORS
            );

    if ( !pTokenChainParamList )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;

        goto  EXIT1;
    }

    while (( pTokenParamDesp = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainParamList) ))
    {
        pTokenChainParamDesp =
            (PANSC_TOKEN_CHAIN)AnscTcAllocate
                (
                    pTokenParamDesp->Name,
                    SLAP_CALL_PARAMETER_SEPARATORS
                );

        if ( !pTokenChainParamDesp )
        {
            AnscFreeMemory(pTokenParamDesp);

            returnStatus = ANSC_STATUS_UNAPPLICABLE;

            goto  EXIT2;
        }
        else
        {
            AnscFreeMemory(pTokenParamDesp);

            pTokenSyntax      = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainParamDesp);
            pTokenContentType = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainParamDesp);

            AnscTcFree(pTokenChainParamDesp);
        }

        if ( !pTokenSyntax )
        {
            returnStatus = ANSC_STATUS_UNAPPLICABLE;

            goto  EXIT2;
        }
        else
        {
            param_list->ParamArray[ulParamCount].Name        = NULL;
            param_list->ParamArray[ulParamCount].ContentType = SLAP_CONTENT_TYPE_UNSPECIFIED;
            param_list->ParamArray[ulParamCount].UsageType   = 0;
            param_list->ParamArray[ulParamCount].Syntax      =
                pSlapVarMapper->GetVarSyntax
                    (
                        (ANSC_HANDLE)pSlapVarMapper,
                        pTokenSyntax->Name
                    );

            AnscFreeMemory(pTokenSyntax);
        }

        if ( pTokenContentType )
        {
            param_list->ParamArray[ulParamCount].ContentType =
                pSlapVarMapper->GetVarContentType
                    (
                        (ANSC_HANDLE)pSlapVarMapper,
                        pTokenContentType->Name
                    );

            AnscFreeMemory(pTokenContentType);
        }

        ulParamCount++;
    }

    param_list->ParamCount = ulParamCount;

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscTcFree((ANSC_HANDLE)pTokenChainParamList);

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoResolveCallDescrO
            (
                ANSC_HANDLE                 hThisObject,
                char*                       call_description,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to resolve the call description of
        output parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       call_description
                Specifies the call description to be resolved.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the target parameter list to be filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoResolveCallDescrO
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;

    returnStatus =
        pMyObject->ResolveCallDescrI
            (
                (ANSC_HANDLE)pMyObject,
                call_description,
                param_list
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoResolveCallDescrR
            (
                ANSC_HANDLE                 hThisObject,
                char*                       call_description,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to resolve the call description of
        returned value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       call_description
                Specifies the call description to be resolved.

                SLAP_VARIABLE*              return_var
                Specifies the target variable to be filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoResolveCallDescrR
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_VARIABLE*              return_var
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject            = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_BSS_INTERFACE             pSlapBssIf           = (PSLAP_BSS_INTERFACE    )pMyObject->hSlapBssIf;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper       = (PSLAP_VAR_MAPPER_OBJECT)pSlapBssIf->GetVarMapper(pSlapBssIf->hOwnerContext);
    PANSC_TOKEN_CHAIN               pTokenChainParamDesp = NULL;
    PANSC_STRING_TOKEN              pTokenSyntax         = NULL;
    PANSC_STRING_TOKEN              pTokenContentType    = NULL;

    if ( !pSlapVarMapper )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        SlapCleanVariable(return_var);
        SlapInitVariable (return_var);
    }

    pTokenChainParamDesp =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                call_description,
                SLAP_CALL_PARAMETER_SEPARATORS
            );

    if ( !pTokenChainParamDesp )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pTokenSyntax      = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainParamDesp);
        pTokenContentType = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainParamDesp);

        AnscTcFree(pTokenChainParamDesp);
    }

    if ( !pTokenSyntax )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        return_var->Name        = NULL;
        return_var->ContentType = SLAP_CONTENT_TYPE_UNSPECIFIED;
        return_var->UsageType   = 0;
        return_var->Syntax      =
            pSlapVarMapper->GetVarSyntax
                (
                    (ANSC_HANDLE)pSlapVarMapper,
                    pTokenSyntax->Name
                );

        AnscFreeMemory(pTokenSyntax);
    }

    if ( pTokenContentType )
    {
        return_var->ContentType =
            pSlapVarMapper->GetVarContentType
                (
                    (ANSC_HANDLE)pSlapVarMapper,
                    pTokenContentType->Name
                );

        AnscFreeMemory(pTokenContentType);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOeoResolveCallBridge
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name
            );

    description:

        This function is called to resolve the call bridge of the
        specified method.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the object method to be resolved.

    return:     call bridge.

**********************************************************************/

ULONG
SlapOeoResolveCallBridge
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    if ( !method_name )
    {
        return  0;
    }
    else if ( strcasecmp(method_name, "GetProperty") == 0 )
    {
        return  SLAP_BRIDGE_ID_GetProperty;
    }
    else if ( strcasecmp(method_name, "SetProperty") == 0 )
    {
        return  SLAP_BRIDGE_ID_SetProperty;
    }
    else if ( strcasecmp(method_name, "GetItem") == 0 )
    {
        return  SLAP_BRIDGE_ID_GetItem;
    }
    else if ( strcasecmp(method_name, "SetItem") == 0 )
    {
        return  SLAP_BRIDGE_ID_SetItem;
    }
    else if ( strcasecmp(method_name, "GetItemByIndex") == 0 )
    {
        return  SLAP_BRIDGE_ID_GetItemByIndex;
    }
    else if ( strcasecmp(method_name, "SetItemByIndex") == 0 )
    {
        return  SLAP_BRIDGE_ID_SetItemByIndex;
    }

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOeoResolveCallOption
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to resolve the call options of the
        specified method.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the object method to be resolved.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the input parameter list passed to the method.

    return:     call option.

**********************************************************************/

ULONG
SlapOeoResolveCallOption
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject          = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor1 = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor2 = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSLAP_OBJECT_PROPERTY           pObjectProperty    = (PSLAP_OBJECT_PROPERTY  )NULL;
    ULONG                           ulCallOptions      = (ULONG                  )0;
    char*                           property_name      = (char*                  )NULL;

    pMethodDescriptor1 =
        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
            (
                (ANSC_HANDLE)pMyObject,
                method_name,
                param_list
            );

    if ( !pMethodDescriptor1 )
    {
        return  0;
    }
    else
    {
        ulCallOptions = pMethodDescriptor1->CallOptions;
    }

    /*
     * If the specified method is one the pre-defined 'bridge' functions, namely 'GetProperty',
     * 'SetProperty', 'GetItem', 'SetItem', 'GetItemByIndex', and 'SetItemByIndex', we need to
     * locate the target method and examine it's call options.
     */
    switch ( pMethodDescriptor1->BridgeId )
    {
        case    SLAP_BRIDGE_ID_GetProperty :

                if ( !param_list )
                {
                    break;
                }
                else if ( param_list->ParamCount < 1 )
                {
                    break;
                }
                else if ( (param_list->ParamArray[0].Syntax            != SLAP_VAR_SYNTAX_string) ||
                          (param_list->ParamArray[0].Variant.varString == NULL                  ) )
                {
                    break;
                }
                else
                {
                    property_name = param_list->ParamArray[0].Variant.varString;
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    /*
                     * When searching for the target method, we need to make sure the number of
                     * input parameters match the one specified in the target method descriptor.
                     * Since the content of the parameters don't really matter when perform the
                     * lookup, we simply modify the parameter count.
                     */
                    param_list->ParamCount--;
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->GetMethodName,
                                param_list
                            );
                    param_list->ParamCount++;
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        case    SLAP_BRIDGE_ID_SetProperty :

                if ( !param_list )
                {
                    break;
                }
                else if ( param_list->ParamCount < 1 )
                {
                    break;
                }
                else if ( (param_list->ParamArray[0].Syntax            != SLAP_VAR_SYNTAX_string) ||
                          (param_list->ParamArray[0].Variant.varString == NULL                  ) )
                {
                    break;
                }
                else
                {
                    property_name = param_list->ParamArray[0].Variant.varString;
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    /*
                     * When searching for the target method, we need to make sure the number of
                     * input parameters match the one specified in the target method descriptor.
                     * Since the content of the parameters don't really matter when perform the
                     * lookup, we simply modify the parameter count.
                     */
                    param_list->ParamCount--;
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->SetMethodName,
                                param_list
                            );
                    param_list->ParamCount++;
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        case    SLAP_BRIDGE_ID_GetItem :

                if ( TRUE )
                {
                    property_name = "CollectionItem";
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->GetMethodName,
                                param_list
                            );
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        case    SLAP_BRIDGE_ID_SetItem :

                if ( TRUE )
                {
                    property_name = "CollectionItem";
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->SetMethodName,
                                param_list
                            );
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        case    SLAP_BRIDGE_ID_GetItemByIndex :

                if ( TRUE )
                {
                    property_name = "ItemByIndex";
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->GetMethodName,
                                param_list
                            );
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        case    SLAP_BRIDGE_ID_SetItemByIndex :

                if ( TRUE )
                {
                    property_name = "ItemByIndex";
                }

                pObjectProperty =
                    (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty
                        (
                            (ANSC_HANDLE)pMyObject,
                            property_name
                        );

                if ( !pObjectProperty )
                {
                    break;
                }
                else
                {
                    pMethodDescriptor2 =
                        (PSLAP_METHOD_DESCRIPTOR)pMyObject->GetMethodDescriptor2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectProperty->SetMethodName,
                                param_list
                            );
                }

                if ( !pMethodDescriptor2 )
                {
                    break;
                }
                else if ( pMethodDescriptor2->CallOptions & SLAP_CALL_OPTION_skipGoaTranslation )
                {
                    ulCallOptions |= SLAP_CALL_OPTION_skipGoaTranslation;
                }

                break;

        default :

                break;
    }

    return  ulCallOptions;
}
