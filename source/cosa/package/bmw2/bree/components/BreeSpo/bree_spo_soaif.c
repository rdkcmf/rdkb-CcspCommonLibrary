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

    module:	bree_spo_soaif.c

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/

#include "bree_spo_global.h"


static const char *s_BreeBuiltinObjs[]  =
{ 
    /* BSP built-in object names */
    "Request",
    "Response",
    "Server",
    "Application",
    "Session",
    "Page",

    /* SCP built-in object names */
    "Command",
    "Input",
    "Output",
    "Domain",
    "Terminal"
};


ANSC_HANDLE
BreeSpoSoaGetSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;

    if ( !pBeepPecIf )
    {
        return (ANSC_HANDLE)NULL;
    }

    return 
        pBeepPecIf->GetBeepObject
            (
                pBeepPecIf->hOwnerContext,
                obj_name
            );
}


ANSC_HANDLE
BreeSpoSoaGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;

    if ( !pBeepPecIf )
    {
        return (ANSC_HANDLE)NULL;
    }

    return
        pBeepPecIf->GetCookedPage
            (
                pBeepPecIf->hOwnerContext,
                page_path
            );
}


ANSC_STATUS
BreeSpoSoaRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    )
{
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;

    if ( !pBeepPecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    return 
        pBeepPecIf->RetCookedPage
            (
                pBeepPecIf->hOwnerContext,
                hCookedPage
            );
}


ANSC_STATUS
BreeSpoSoaGetCookedPageData
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
BreeSpoSoaInvokeObject
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
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;
    ULONG                           ulPecStatus;
    ULONG                           ulSoaStatus;

    if ( !pBeepPecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }
   
    status = 
        pBeepPecIf->InvokeObject
            (
                pBeepPecIf->hOwnerContext,
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
        case    BEEP_PEC_STATUS_continue:

                ulSoaStatus = BSPENG_SOA_STATUS_continue;        

                break;

        case    BEEP_PEC_STATUS_terminate:

                ulSoaStatus = BSPENG_SOA_STATUS_terminate;        

                break;

        case    BEEP_PEC_STATUS_abort:

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
BreeSpoSoaIsBuiltInObject
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

    for (i = 0; i < sizeof(s_BreeBuiltinObjs)/sizeof(s_BreeBuiltinObjs[0]); i ++)
    {
        pObjName        = (PUCHAR)s_BreeBuiltinObjs[i];
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
BreeSpoSoaWriteAString
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pAsciiStr
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;
    ANSC_HANDLE                     hResponse;
    SLAP_PARAMETER_LIST             paramList;
    SLAP_PARAMETER_LIST             *outList    = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal     = NULL;
    ULONG                           ulPecStatus;

    if ( !pBeepPecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    hResponse   = pBeepPecIf->GetResponse(pBeepPecIf->hOwnerContext);
    
    paramList.ParamCount    = 1;
    pParam  = &paramList.ParamArray[0];

    pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
    pParam->Variant.varString   = (SLAP_CHAR *)pAsciiStr;

    /* invoke "Write" method of Response object to output the given string */
    status = 
        pBeepPecIf->InvokeObject
            (
                pBeepPecIf->hOwnerContext,
                hResponse,
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
BreeSpoSoaWriteBString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBinaryStr,
        ULONG                       ulStrLen
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hThisObject;
    ANSC_HANDLE                     hResponse;
    SLAP_PARAMETER_LIST             paramList;
    SLAP_PARAMETER_LIST             *outList    = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal     = NULL;
    ULONG                           ulPecStatus;

    if ( !pBeepPecIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    /* call "Response.BinaryWrite1" to output the stream content */
    hResponse   = pBeepPecIf->GetResponse(pBeepPecIf->hOwnerContext);
    
    paramList.ParamCount    = 2;

    pParam                      = &paramList.ParamArray[0];
    AnscZeroMemory(pParam, sizeof(SLAP_VARIABLE));
    pParam->Syntax              = SLAP_VAR_SYNTAX_TYPE_ptr;
    pParam->Variant.varPtr      = (SLAP_HANDLE)pBinaryStr;

    pParam                      = &paramList.ParamArray[1];
    AnscZeroMemory(pParam, sizeof(SLAP_VARIABLE));
    pParam->Syntax              = SLAP_VAR_SYNTAX_TYPE_uint32;
    pParam->Variant.varUint32   = ulStrLen;

    /* invoke "Write" method of Response object to output the given string */
    status = 
        pBeepPecIf->InvokeObject
            (
                pBeepPecIf->hOwnerContext,
                hResponse,
                (char *)"BinaryWrite1",
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

    SlapCleanParamList((&paramList));    

    if (outList)
    {
        SlapFreeParamList(outList);
    }

    return status;
}


BOOL
BreeSpoSoaIsInterrupted
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return FALSE;
}



