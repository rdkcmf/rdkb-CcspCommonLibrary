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

    module:	slap_scostd_invocation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced invocation functions
        of the Slap Sco Standard Object.

        *   SlapScoStdInvokeFixInput0
        *   SlapScoStdInvokeFixInput1
        *   SlapScoStdInvokeFixInput2
        *   SlapScoStdInvokeFixInput3
        *   SlapScoStdInvokeFixInput4
        *   SlapScoStdInvokeFixInput5
        *   SlapScoStdInvokeFixInput6
        *   SlapScoStdInvokeFixInput7
        *   SlapScoStdInvokeFixInput8

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/05    initial revision.

**********************************************************************/


#include "slap_scostd_global.h"


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4047)                     /* 'identifier1' differs in levels of indirection from 'identifier2' */
#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput0
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput0
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput1
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput3
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput3
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput4
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput4
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput5
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput5
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput6
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput6
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
        pReturnedVar =
            ((PFN_SLAPOBJ_CALL_VARIABLE)pfnTargetMethod)
                (
                    (ANSC_HANDLE)pMyObject,
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 0),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 1),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 2),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 3),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 4),
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 5)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput7
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput7
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
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
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 6)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        SlapScoStdInvokeFixInput8
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMethodDescriptor,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST*        params_out
            );

    description:

        This function is called to invoke a runtime service method with
        fixed number of input parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMethodDescriptor
                Specifies the object method descriptor to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST*        params_out
                Specifies the list of output parameters to be passed.

    return:     return variable interrupted by caller.

**********************************************************************/

void*
SlapScoStdInvokeFixInput8
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject         = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf        = (PSLAP_OLA_INTERFACE      )pMyObject->hSlapOlaIf;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR  )hMethodDescriptor;
    PSLAP_VARIABLE                  pReturnedVar      = (PSLAP_VARIABLE           )NULL;
    void*                           pfnTargetMethod   = (void*                    )NULL;

    pfnTargetMethod = *(void**)((ULONG)pMyObject + pMethodDescriptor->MethodOffset);

    if ( pMethodDescriptor->ParamListO->ParamCount == 0 )
    {
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
                    SlapResolveInvokeParam(pMethodDescriptor, params_in, 7)
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 1 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 2 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 3 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 4 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 5 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 6 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 7 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool
                );
    }
    else if ( pMethodDescriptor->ParamListO->ParamCount == 8 )
    {
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
                    &params_out->ParamArray[0].Variant.varBool,
                    &params_out->ParamArray[1].Variant.varBool,
                    &params_out->ParamArray[2].Variant.varBool,
                    &params_out->ParamArray[3].Variant.varBool,
                    &params_out->ParamArray[4].Variant.varBool,
                    &params_out->ParamArray[5].Variant.varBool,
                    &params_out->ParamArray[6].Variant.varBool,
                    &params_out->ParamArray[7].Variant.varBool
                );
    }

    return  pReturnedVar;
}
