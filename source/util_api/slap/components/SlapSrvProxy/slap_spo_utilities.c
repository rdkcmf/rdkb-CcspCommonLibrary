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

    module:	slap_spo_utilities.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Slap Srv Proxy Object.

        *   SlapSpoPoaToGoaParamList
        *   SlapSpoGoaToPoaParamList
        *   SlapSpoPoaToGoaVariable
        *   SlapSpoGoaToPoaVariable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#include "slap_spo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoPoaToGoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from POA to
        GOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoPoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->PoaToGoaVariable
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
        SlapSpoGoaToPoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from GOA to
        POA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoGoaToPoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->GoaToPoaVariable
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
        SlapSpoPoaToGoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from POA to GOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoPoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy = (PSLAP_SRV_PROXY_OBJECT)NULL;
    ULONG                           i             = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                pSlapSrvProxy = (PSLAP_SRV_PROXY_OBJECT)slap_var->Variant.varObject;

                if ( pSlapSrvProxy )
                {
                    slap_var->Variant.varObject = pSlapSrvProxy->GetSlapGoaObj((ANSC_HANDLE)pSlapSrvProxy);
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        pSlapSrvProxy = (PSLAP_SRV_PROXY_OBJECT)slap_var->Variant.varObjectArray->Array.arrayObject[i];

                        if ( pSlapSrvProxy )
                        {
                            slap_var->Variant.varObjectArray->Array.arrayObject[i] = pSlapSrvProxy->GetSlapGoaObj((ANSC_HANDLE)pSlapSrvProxy);
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
        SlapSpoGoaToPoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from GOA to POA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoGoaToPoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PSLAP_SRV_PROXY_OBJECT          pMyObject        = (PSLAP_SRV_PROXY_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf       = (PSLAP_GOA_INTERFACE   )pMyObject->hSlapGoaIf;
    PSLAP_POA_INTERFACE             pSlapPoaIf       = (PSLAP_POA_INTERFACE   )pMyObject->hSlapPoaIf;
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy    = (PSLAP_SRV_PROXY_OBJECT)NULL;
    char*                           pSlapObjName     = (char*                 )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST  )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE        )NULL;
    ULONG                           i                = 0;
    SLAP_PARAMETER_LIST             inputParamList;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                if ( slap_var->Variant.varObject )
                {
                    if ( slap_var->Name )
                    {
                        pSlapObjName = slap_var->Name;
                    }
                    else
                    {
                        if ( TRUE )
                        {
                            inputParamList.ParamCount = 0;
                            returnStatus              =
                                pSlapGoaIf->InvokeObject
                                    (
                                        pSlapGoaIf->hOwnerContext,
                                        slap_var->Variant.varObject,
                                        (ANSC_HANDLE)NULL,
                                        "GetObjName",
                                        &inputParamList,
                                        &pOutputParamList,
                                        &pReturnedVar
                                    );

                            if ( returnStatus != ANSC_STATUS_SUCCESS )
                            {
                                return  returnStatus;
                            }
                            else if ( !pReturnedVar                                               ||
                                      (pReturnedVar->Syntax            != SLAP_VAR_SYNTAX_string) ||
                                      (pReturnedVar->Variant.varString == NULL                  ) )
                            {
                                return  ANSC_STATUS_FAILURE;
                            }
                            else
                            {
                                pSlapObjName = pReturnedVar->Variant.varString;
                            }
                        }
                    }

                    pSlapSrvProxy =
                        (PSLAP_SRV_PROXY_OBJECT)pSlapPoaIf->CreateProxyObject
                            (
                                pSlapPoaIf->hOwnerContext,
                                pSlapObjName,
                                slap_var->Variant.varObject
                            );

                    if ( !pSlapSrvProxy )
                    {
                        return  ANSC_STATUS_FAILURE;
                    }
                    else
                    {
                        slap_var->Variant.varObject = (ANSC_HANDLE)pSlapSrvProxy;
                    }

                    if ( TRUE )
                    {
                        if ( pOutputParamList )
                        {
                            SlapFreeParamList(pOutputParamList);

                            pOutputParamList = NULL;
                        }

                        if ( pReturnedVar )
                        {
                            SlapFreeVariable(pReturnedVar);

                            pReturnedVar = NULL;
                        }
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
                            if ( slap_var->Name )
                            {
                                pSlapObjName = slap_var->Name;
                            }
                            else
                            {
                                if ( TRUE )
                                {
                                    inputParamList.ParamCount = 0;
                                    returnStatus              =
                                        pSlapGoaIf->InvokeObject
                                            (
                                                pSlapGoaIf->hOwnerContext,
                                                slap_var->Variant.varObjectArray->Array.arrayObject[i],
                                                (ANSC_HANDLE)NULL,
                                                "GetObjName",
                                                &inputParamList,
                                                &pOutputParamList,
                                                &pReturnedVar
                                            );

                                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                                    {
                                        return  returnStatus;
                                    }
                                    else if ( !pReturnedVar                                               ||
                                              (pReturnedVar->Syntax            != SLAP_VAR_SYNTAX_string) ||
                                              (pReturnedVar->Variant.varString == NULL                  ) )
                                    {
                                        return  ANSC_STATUS_FAILURE;
                                    }
                                    else
                                    {
                                        pSlapObjName = pReturnedVar->Variant.varString;
                                    }
                                }
                            }

                            pSlapSrvProxy =
                                (PSLAP_SRV_PROXY_OBJECT)pSlapPoaIf->CreateProxyObject
                                    (
                                        pSlapPoaIf->hOwnerContext,
                                        pSlapObjName,
                                        slap_var->Variant.varObjectArray->Array.arrayObject[i]
                                    );

                            if ( !pSlapSrvProxy )
                            {
                                return  ANSC_STATUS_FAILURE;
                            }
                            else
                            {
                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (ANSC_HANDLE)pSlapSrvProxy;
                            }

                            if ( TRUE )
                            {
                                if ( pOutputParamList )
                                {
                                    SlapFreeParamList(pOutputParamList);

                                    pOutputParamList = NULL;
                                }

                                if ( pReturnedVar )
                                {
                                    SlapFreeVariable(pReturnedVar);

                                    pReturnedVar = NULL;
                                }
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
