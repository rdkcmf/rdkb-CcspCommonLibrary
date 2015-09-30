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

    module:	slap_scostd_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Sco Standard Object.

        *   SlapScoStdInvokeDispatch

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


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4047)                     /* 'identifier1' differs in levels of indirection from 'identifier2' */
#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

    ANSC_STATUS
    SlapScoStdInvokeDispatch
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
SlapScoStdInvokeDispatch
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList  = (PSLAP_PARAMETER_LIST     )NULL;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    /*
     * First of all, we need to verify the input parameters are valid of course. We do expect the
     * caller to allocate the memory for params_in even if there's no input parameters. We will be
     * responsible for allocating params_out and return_var.
     */
    if ( !method_name || !params_in || !params_out || !return_var )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        *params_out = NULL;
        *return_var = NULL;

        pMethodDescriptor =
            (PSLAP_METHOD_DESCRIPTOR)pSlapOlaIf->GetMethodDescriptor
                (
                    pSlapOlaIf->hOwnerContext,
                    method_name,
                    params_in
                );
    }

    /*
     * Ooops! There're no method has been associated with the specified method name or no matched
     * parameter list has been identified. Note that above we always match the number of the input
     * parameters with the existing method descriptors. The caller must make sure the number of
     * input parameters is accurate.
     */
    if ( !pMethodDescriptor )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);
    }

    /*
     * We shall always perform data type conversion on all input parameters unless the method
     * descriptor indicates that the input parameters should be passed in without any translation.
     * However only internal basic SLAP functions may specify that. Derived service objects MUST
     * NEVER attempt it.
     */
    if ( !(pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListI) )
    {
        returnStatus =
            pMyObject->ConvertParams
                (
                    (ANSC_HANDLE)pMyObject,
                    pMethodDescriptor->ParamListI,
                    params_in
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
    }

    /*
     * Now it comes the hardest and most time-consuming part: making sensible calls into the target
     * object methods. Apparently, we have two choices:
     *
     *  (1) We can require all the derived service objects MUST override this member
     *      function. Within the overrided function, the drived object shall map the
     *      method name to the desired function call and arrange the function parameters.
     *
     *  (2) We can invoke the target method calls directly since we're supposed to have
     *      everything we need to arrange the function parameters. However this means we
     *      MUST enumerate all possible calling scenarios, which can be extremely boring
     *      and weird (that's how I feel somehow).
     *
     * Guess which option we're going to choose...
     */
    if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passReturnVar )
    {
        if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListI )
        {
            if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListO )
            {
                pOutputParamList = NULL;
                pReturnedVar     =
                    ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                        (
                            (ANSC_HANDLE)pMyObject,
                            params_in,
                            params_out
                        );

                pOutputParamList = *params_out;
            }
            else
            {
                pOutputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

                if ( !pOutputParamList )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT1;
                }
                else
                {
                    SlapCloneParamList(pMethodDescriptor->ParamListO, pOutputParamList);
                }

                switch ( pMethodDescriptor->ParamListO->ParamCount )
                {
                    case    0 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in
                                    );

                            break;

                    case    1 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool
                                    );

                            break;

                    case    2 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool
                                    );

                            break;

                    case    3 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool
                                    );

                            break;

                    case    4 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool
                                    );

                            break;

                    case    5 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool
                                    );

                            break;

                    case    6 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool
                                    );

                            break;

                    case    7 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool,
                                        &pOutputParamList->ParamArray[6].Variant.varBool
                                    );

                            break;

                    case    8 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool,
                                        &pOutputParamList->ParamArray[6].Variant.varBool,
                                        &pOutputParamList->ParamArray[7].Variant.varBool
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT2;

                            break;
                }
            }
        }
        else
        {
            if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListO )
            {
                pOutputParamList = NULL;

                switch ( pMethodDescriptor->ParamListI->ParamCount )
                {
                    case    0 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_out
                                    );

                            break;

                    case    1 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        params_out
                                    );

                            break;

                    case    2 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        params_out
                                    );

                            break;

                    case    3 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        params_out
                                    );

                            break;

                    case    4 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        params_out
                                    );

                            break;

                    case    5 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        params_out
                                    );

                            break;

                    case    6 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        params_out
                                    );

                            break;

                    case    7 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 6),
                                        params_out
                                    );

                            break;

                    case    8 :

                            pReturnedVar =
                                ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 6),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 7),
                                        params_out
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT1;

                            break;
                }

                pOutputParamList = *params_out;
            }
            else
            {
                pOutputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

                if ( !pOutputParamList )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT1;
                }
                else
                {
                    SlapCloneParamList(pMethodDescriptor->ParamListO, pOutputParamList);
                }

                switch ( pMethodDescriptor->ParamListI->ParamCount )
                {
                    case    0 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput0
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    1 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput1
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    2 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput2
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    3 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput3
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    4 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput4
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    5 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput5
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    6 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput6
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    7 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput7
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    8 :

                            pReturnedVar =
                                (PSLAP_VARIABLE)pMyObject->InvokeFixInput8
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT2;

                            break;
                }
            }
        }

        *params_out = pOutputParamList;
        *return_var = pReturnedVar;
    }
    else
    {
        pReturnedVar = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pReturnedVar )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            SlapCloneVariable(pMethodDescriptor->ReturnVar, pReturnedVar);
        }

        if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListI )
        {
            if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListO )
            {
                pOutputParamList              = NULL;
                pReturnedVar->Variant.varBool =
                    ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                        (
                            (ANSC_HANDLE)pMyObject,
                            params_in,
                            params_out
                        );

                pOutputParamList = *params_out;
            }
            else
            {
                pOutputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

                if ( !pOutputParamList )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT1;
                }
                else
                {
                    SlapCloneParamList(pMethodDescriptor->ParamListO, pOutputParamList);
                }

                switch ( pMethodDescriptor->ParamListO->ParamCount )
                {
                    case    0 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in
                                    );

                            break;

                    case    1 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool
                                    );

                            break;

                    case    2 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool
                                    );

                            break;

                    case    3 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool
                                    );

                            break;

                    case    4 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool
                                    );

                            break;

                    case    5 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool
                                    );

                            break;

                    case    6 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool
                                    );

                            break;

                    case    7 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool,
                                        &pOutputParamList->ParamArray[6].Variant.varBool
                                    );

                            break;

                    case    8 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_in,
                                        &pOutputParamList->ParamArray[0].Variant.varBool,
                                        &pOutputParamList->ParamArray[1].Variant.varBool,
                                        &pOutputParamList->ParamArray[2].Variant.varBool,
                                        &pOutputParamList->ParamArray[3].Variant.varBool,
                                        &pOutputParamList->ParamArray[4].Variant.varBool,
                                        &pOutputParamList->ParamArray[5].Variant.varBool,
                                        &pOutputParamList->ParamArray[6].Variant.varBool,
                                        &pOutputParamList->ParamArray[7].Variant.varBool
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT2;

                            break;
                }
            }
        }
        else
        {
            if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListO )
            {
                pOutputParamList = NULL;

                switch ( pMethodDescriptor->ParamListI->ParamCount )
                {
                    case    0 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        params_out
                                    );

                            break;

                    case    1 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        params_out
                                    );

                            break;

                    case    2 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        params_out
                                    );

                            break;

                    case    3 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        params_out
                                    );

                            break;

                    case    4 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        params_out
                                    );

                            break;

                    case    5 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        params_out
                                    );

                            break;

                    case    6 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        params_out
                                    );

                            break;

                    case    7 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 6),
                                        params_out
                                    );

                            break;

                    case    8 :

                            pReturnedVar->Variant.varBool =
                                ((PFN_SLAPOBJ_CALL_BOOL)pfnTargetMethod)
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 5),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 6),
                                        SlapResolveInvokeParam(pMethodDescriptor, params_in, 7),
                                        params_out
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT1;

                            break;
                }

                pOutputParamList = *params_out;
            }
            else
            {
                pOutputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

                if ( !pOutputParamList )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT1;
                }
                else
                {
                    SlapCloneParamList(pMethodDescriptor->ParamListO, pOutputParamList);
                }

                switch ( pMethodDescriptor->ParamListI->ParamCount )
                {
                    case    0 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput0
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    1 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput1
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    2 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput2
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    3 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput3
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    4 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput4
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    5 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput5
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    6 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput6
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    7 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput7
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    case    8 :

                            pReturnedVar->Variant.varBool =
                                (SLAP_BOOL)pMyObject->InvokeFixInput8
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pMethodDescriptor,
                                        params_in,
                                        pOutputParamList
                                    );

                            break;

                    default :

                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                            goto  EXIT2;

                            break;
                }
            }
        }

        *params_out = pOutputParamList;
        *return_var = pReturnedVar;
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    if ( pReturnedVar )
    {
        SlapFreeVariable(pReturnedVar);
    }

EXIT1:

    return  returnStatus;
}
