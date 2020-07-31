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

    module:	slap_amo_utilities.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Slap Access Manager Object.

        *   SlapAmoGoaToUoaParamList
        *   SlapAmoUoaToGoaParamList
        *   SlapAmoGoaToUoaVariable
        *   SlapAmoUoaToGoaVariable
        *   SlapAmoGoaToLoaParamList
        *   SlapAmoLoaToGoaParamList
        *   SlapAmoGoaToLoaVariable
        *   SlapAmoLoaToGoaVariable

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


#include "slap_amo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaToUoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from GOA to
        UOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaToUoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->GoaToUoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    &param_list->ParamArray[i]
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoUoaToGoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list,
                ANSC_HANDLE                 hSlapGoaRtc
            );

    description:

        This function is called to convert parameter list from UOA to
        GOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoUoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->UoaToGoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    &param_list->ParamArray[i],
                    hSlapGoaRtc
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaToUoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from GOA to UOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaToUoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    ULONG                           i               = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT)slap_var->Variant.varObject;

                if ( pSlapObjWrapper )
                {
                    slap_var->Variant.varObject = (SLAP_OBJECT)pSlapObjWrapper->hWrappedObj;
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT)slap_var->Variant.varObjectArray->Array.arrayObject[i];

                        if ( pSlapObjWrapper )
                        {
                            slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_OBJECT)pSlapObjWrapper->hWrappedObj;
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoUoaToGoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var,
                ANSC_HANDLE                 hSlapGoaRtc
            );

    description:

        This function is called to convert a variable from UOA to GOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoUoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapGoaRtc;
    PSLAP_OWO_UOAC_OBJECT           pSlapOwoUoac       = (PSLAP_OWO_UOAC_OBJECT        )pSlapObjWrapperC;
    ULONG                           i                  = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                if ( slap_var->Variant.varObject )
                {
                    pSlapObjWrapperO =
                        (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoao
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( !pSlapObjWrapperO )
                    {
                        /*
                         * We cannot create a new GOA Obj Wrapper, we then have to dereference this
                         * UOA object because we cannot return it the way it is. For SLAP_OBJECT
                         * object, we have to explicitly delete it from UOA.
                         */
                        pSlapUoaIf->DeleteObject
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    (ANSC_HANDLE)slap_var->Variant.varObject
                                );

                        slap_var->Variant.varObject = (SLAP_HANDLE)NULL;
                    }
                    else
                    {
                        pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_uoaObject;
                        pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_created;
                        pSlapObjWrapperO->hWrappedObj   = (ANSC_HANDLE)slap_var->Variant.varObject;
                        pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                        pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                        pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, NULL             );
                        pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)NULL);
                        pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL             );

                        if (  pSlapObjWrapperC &&
                             (pSlapObjWrapperC->ObjType == SLAP_OWO_OBJ_TYPE_uoaContainer) )
                        {
                            pSlapOwoUoac->AttachObjWrapper
                                    (
                                        (ANSC_HANDLE)pSlapOwoUoac,
                                        (ANSC_HANDLE)pSlapObjWrapperO
                                    );
                        }

                        slap_var->Variant.varObject = (SLAP_HANDLE)pSlapObjWrapperO;
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        if ( slap_var->Variant.varObjectArray->Array.arrayObject[i] )
                        {
                            pSlapObjWrapperO =
                                (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoao
                                    (
                                        pMyObject->hContainerContext,
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)NULL
                                    );

                            if ( !pSlapObjWrapperO )
                            {
                                /*
                                 * We cannot create a new GOA Obj Wrapper, we then have to derefer-
                                 * ence this UOA object because we cannot return it the way it is.
                                 * For SLAP_OBJECT object, we have to explicitly delete it from UOA.
                                 */
                                pSlapUoaIf->DeleteObject
                                        (
                                            pSlapUoaIf->hOwnerContext,
                                            (ANSC_HANDLE)slap_var->Variant.varObjectArray->Array.arrayObject[i]
                                        );

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)NULL;
                            }
                            else
                            {
                                pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_uoaObject;
                                pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_created;
                                pSlapObjWrapperO->hWrappedObj   = slap_var->Variant.varObjectArray->Array.arrayObject[i];
                                pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                                pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                                pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, NULL             );
                                pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)NULL);
                                pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL             );

                                if (  pSlapObjWrapperC &&
                                     (pSlapObjWrapperC->ObjType == SLAP_OWO_OBJ_TYPE_uoaContainer) )
                                {
                                    pSlapOwoUoac->AttachObjWrapper
                                            (
                                                (ANSC_HANDLE)pSlapOwoUoac,
                                                (ANSC_HANDLE)pSlapObjWrapperO
                                            );
                                }

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)pSlapObjWrapperO;
                            }
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaToLoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from GOA to
        UOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaToLoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    #ifdef  _ANSC_SLAP_LPC_

    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty    = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->GoaToLoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    &param_list->ParamArray[i]
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;

    #else
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(param_list);
    return  ANSC_STATUS_SUCCESS;

    #endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoLoaToGoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                SLAP_PARAMETER_LIST*        param_list,
                ANSC_HANDLE                 hSlapGoaRtc
            );

    description:

        This function is called to convert parameter list from UOA to
        GOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoLoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    )
{
    #ifdef  _ANSC_SLAP_LPC_

    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty    = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->LoaToGoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    lpc_party_addr,
                    &param_list->ParamArray[i],
                    hSlapGoaRtc
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;

    #else
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(lpc_party_addr);
    UNREFERENCED_PARAMETER(param_list);
    UNREFERENCED_PARAMETER(hSlapGoaRtc);
    return  ANSC_STATUS_SUCCESS;

    #endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaToLoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from GOA to UOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaToLoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    #ifdef  _ANSC_SLAP_LPC_

    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject       = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty       = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    ULONG                           i               = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT)slap_var->Variant.varObject;

                if ( pSlapObjWrapper )
                {
                    slap_var->Variant.varObject = (SLAP_OBJECT)pSlapObjWrapper->hWrappedObj;
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        pSlapObjWrapper = (PSLAP_OBJ_WRAPPER_OBJECT)slap_var->Variant.varObjectArray->Array.arrayObject[i];

                        if ( pSlapObjWrapper )
                        {
                            slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_OBJECT)pSlapObjWrapper->hWrappedObj;
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;

    #else
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(slap_var);
    return  ANSC_STATUS_SUCCESS;

    #endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoLoaToGoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                SLAP_VARIABLE*              slap_var,
                ANSC_HANDLE                 hSlapGoaRtc
            );

    description:

        This function is called to convert a variable from UOA to GOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoLoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    )
{
    #ifdef  _ANSC_SLAP_LPC_

    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapGoaRtc;
    PSLAP_OWO_UOAC_OBJECT           pSlapOwoLoac       = (PSLAP_OWO_UOAC_OBJECT        )pSlapObjWrapperC;
    ULONG                           i                  = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                if ( slap_var->Variant.varObject )
                {
                    pSlapObjWrapperO =
                        (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoLoao
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( !pSlapObjWrapperO )
                    {
                        /*
                         * We cannot create a new GOA Obj Wrapper, we then have to dereference this
                         * UOA object because we cannot return it the way it is. For SLAP_OBJECT
                         * object, we have to explicitly delete it from UOA.
                         */
                        returnStatus =
                            pSlapLoaIf->DeleteObject
                                (
                                    pSlapLoaIf->hOwnerContext,
                                    lpc_party_addr,
                                    (ANSC_HANDLE)slap_var->Variant.varObject
                                );

                        slap_var->Variant.varObject = (SLAP_HANDLE)NULL;
                    }
                    else
                    {
                        pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_loaObject;
                        pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_created;
                        pSlapObjWrapperO->hWrappedObj   = (ANSC_HANDLE)slap_var->Variant.varObject;
                        pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                        pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                        pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, slap_var->Name             );
                        pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)lpc_party_addr);
                        pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL                       );

                        if (  pSlapObjWrapperC &&
                             (pSlapObjWrapperC->ObjType == SLAP_OWO_OBJ_TYPE_uoaContainer) )
                        {
                            returnStatus =
                                pSlapOwoLoac->AttachObjWrapper
                                    (
                                        (ANSC_HANDLE)pSlapOwoLoac,
                                        (ANSC_HANDLE)pSlapObjWrapperO
                                    );
                        }

                        slap_var->Variant.varObject = (SLAP_HANDLE)pSlapObjWrapperO;
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        if ( slap_var->Variant.varObjectArray->Array.arrayObject[i] )
                        {
                            pSlapObjWrapperO =
                                (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoLoao
                                    (
                                        pMyObject->hContainerContext,
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)NULL
                                    );

                            if ( !pSlapObjWrapperO )
                            {
                                /*
                                 * We cannot create a new GOA Obj Wrapper, we then have to derefer-
                                 * ence this UOA object because we cannot return it the way it is.
                                 * For SLAP_OBJECT object, we have to explicitly delete it from UOA.
                                 */
                                returnStatus =
                                    pSlapLoaIf->DeleteObject
                                        (
                                            pSlapLoaIf->hOwnerContext,
                                            lpc_party_addr,
                                            (ANSC_HANDLE)slap_var->Variant.varObjectArray->Array.arrayObject[i]
                                        );

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)NULL;
                            }
                            else
                            {
                                pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_loaObject;
                                pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_created;
                                pSlapObjWrapperO->hWrappedObj   = slap_var->Variant.varObjectArray->Array.arrayObject[i];
                                pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                                pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                                pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, slap_var->Name             );
                                pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)lpc_party_addr);
                                pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL                       );

                                if (  pSlapObjWrapperC &&
                                     (pSlapObjWrapperC->ObjType == SLAP_OWO_OBJ_TYPE_uoaContainer) )
                                {
                                    returnStatus =
                                        pSlapOwoLoac->AttachObjWrapper
                                            (
                                                (ANSC_HANDLE)pSlapOwoLoac,
                                                (ANSC_HANDLE)pSlapObjWrapperO
                                            );
                                }

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)pSlapObjWrapperO;
                            }
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;

    #else
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(lpc_party_addr);
    UNREFERENCED_PARAMETER(slap_var);
    UNREFERENCED_PARAMETER(hSlapGoaRtc);
    return  ANSC_STATUS_SUCCESS;

    #endif
}
