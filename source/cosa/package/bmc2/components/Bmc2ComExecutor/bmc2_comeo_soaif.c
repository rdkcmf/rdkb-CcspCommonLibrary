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

    module:	bmc2_comeo_soaif.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoSoaGetSlapObject
        *   Bmc2ComeoSoaGetCookedPage
        *   Bmc2ComeoSoaRetCookedPage
        *   Bmc2ComeoSoaGetCookedPageData
        *   Bmc2ComeoSoaInvokeObject
        *   Bmc2ComeoSoaIsBuiltInObject
        *   Bmc2ComeoSoaWriteAString
        *   Bmc2ComeoSoaWriteBString
        *   Bmc2ComeoSoaIsInterrupted
        
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/

#include "bmc2_comeo_global.h"


static const char *s_Bmc2BuiltinObjs[]  =
{ 
    "Application",
    "Domain",
    "Input",
    "Output",
    "Server",
    "Command",
    "Terminal"
};


ANSC_HANDLE
Bmc2ComeoSoaGetSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PBMC2_PEC_INTERFACE             pBmc2PecIf      = (PBMC2_PEC_INTERFACE)hThisObject;

    if ( !pBmc2PecIf )
    {
        return (ANSC_HANDLE)NULL;
    }

    return 
        pBmc2PecIf->GetBmc2Object
            (
                pBmc2PecIf->hOwnerContext,
                obj_name
            );
}


ANSC_HANDLE
Bmc2ComeoSoaGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PBMC2_PEC_INTERFACE             pBmc2PecIf      = (PBMC2_PEC_INTERFACE)hThisObject;

    if ( !pBmc2PecIf )
    {
        return (ANSC_HANDLE)NULL;
    }

    return
        pBmc2PecIf->GetCookedPage
            (
                pBmc2PecIf->hOwnerContext,
                page_path
            );
}


ANSC_STATUS
Bmc2ComeoSoaRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    )
{
    PBMC2_PEC_INTERFACE             pBmc2PecIf      = (PBMC2_PEC_INTERFACE)hThisObject;

    if ( !pBmc2PecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    return 
        pBmc2PecIf->RetCookedPage
            (
                pBmc2PecIf->hOwnerContext,
                hCookedPage
            );
}


ANSC_STATUS
Bmc2ComeoSoaGetCookedPageData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage,
        PUCHAR                      *ppStream,
        PULONG                      pulStreamSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBREE_COOKED_BSP_PAGE           pCookedPage     = (PBREE_COOKED_BSP_PAGE)hCookedPage;
    PUCHAR                          pData           = NULL;
    ULONG                           ulSize          = 0;

    if ( pCookedPage )
    {
        pData       = pCookedPage->pArchivedBsp;
        ulSize      = pCookedPage->ulSize;
    }

    if ( ppStream )
    {
        *ppStream   = pData;
    }

    if ( pulStreamSize )
    {
        *pulStreamSize  = ulSize;
    }

    return status;
}


ANSC_STATUS
Bmc2ComeoSoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulSoaStatus
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBMC2_PEC_INTERFACE             pBmc2PecIf      = (PBMC2_PEC_INTERFACE)hThisObject;
    ULONG                           ulPecStatus;
    ULONG                           ulSoaStatus;

    if ( !pBmc2PecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }
   
    status = 
        pBmc2PecIf->InvokeObject
            (
                pBmc2PecIf->hOwnerContext,
                hBeepObject,
                method_name,
                params_in,
                params_out,
                return_var,
                &ulPecStatus
            );

    switch ( ulPecStatus )
    {
        default:
        case    BMC2_PEC_STATUS_continue:

                ulSoaStatus = BSPENG_SOA_STATUS_continue;        

                break;

        case    BMC2_PEC_STATUS_terminate:

                ulSoaStatus = BSPENG_SOA_STATUS_terminate;        

                break;

        case    BMC2_PEC_STATUS_abort:

                ulSoaStatus = BSPENG_SOA_STATUS_abort;        

                break;
    }

    if ( pulSoaStatus )
    {
        *pulSoaStatus   = ulSoaStatus;
    }

    return status;
}


BOOL
Bmc2ComeoSoaIsBuiltInObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           i;
    ULONG                           ulLen;
    ULONG                           ulObjNameLen    = obj_name ? AnscSizeOfString(obj_name) : 0;
    PUCHAR                          pObjName;

    for (i = 0; i < sizeof(s_Bmc2BuiltinObjs)/sizeof(s_Bmc2BuiltinObjs[0]); i ++)
    {
        pObjName        = (PUCHAR)s_Bmc2BuiltinObjs[i];
        ulLen           = AnscSizeOfString((const char *)pObjName);

        if ( ulObjNameLen != ulLen )
        {
            continue;
        }

        if ( AnscEqualString2(obj_name, (char *)pObjName, ulLen, TRUE) )
        {
            return TRUE;
        }
    }

    return FALSE;
}


ANSC_STATUS
Bmc2ComeoSoaWriteAString
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pAsciiStr
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBMC2_PEC_INTERFACE             pBmc2PecIf      = (PBMC2_PEC_INTERFACE)hThisObject;
    ANSC_HANDLE                     hOutput;
    SLAP_PARAMETER_LIST             paramList;
    SLAP_PARAMETER_LIST             *outList    = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal     = NULL;
    ULONG                           ulPecStatus;

    if ( !pBmc2PecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    hOutput   = pBmc2PecIf->GetOutput(pBmc2PecIf->hOwnerContext);
    
    paramList.ParamCount    = 1;
    pParam  = &paramList.ParamArray[0];

    pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
    pParam->Variant.varString   = (SLAP_CHAR *)pAsciiStr;

    /* invoke "Write" method of Response object to output the given string */
    status = 
        pBmc2PecIf->InvokeObject
            (
                pBmc2PecIf->hOwnerContext,
                hOutput,
                (char *)"Write",
                &paramList,
                &outList,
                &pRetVal,
                &ulPecStatus
            );

    if (pRetVal)
    {
        if (pRetVal)
        {
            SlapFreeVariable(pRetVal);
            pRetVal = NULL;
        }
    }

    /* 
     * since we don't duplicate the given string to SLAP variable,
     * we don't need to clean up the SLAP variable
     */
    if (outList)
    {
        SlapFreeParamList(outList);
    }

    return status;
}


ANSC_STATUS
Bmc2ComeoSoaWriteBString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBinaryStr,
        ULONG                       ulStrLen
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pBinaryStr);
    UNREFERENCED_PARAMETER(ulStrLen);
    return ANSC_STATUS_UNAPPLICABLE;
}


BOOL
Bmc2ComeoSoaIsInterrupted
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return FALSE;
}



