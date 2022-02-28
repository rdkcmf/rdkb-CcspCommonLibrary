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

    module: messagebus_interface_utility.c

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    description:

        SSP implementation of the CCSP Message Bus Interface
        Helper Service.

        *   AccessControlBitMaskToAccessList
        *   AccessListToAccessControlBitMask
        *   MsgHelper_CreateCcdMbiIf

    ---------------------------------------------------------------

    environment:

        Embedded Linux

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        06/23/2011  initial revision.

**********************************************************************/

#include "messagebus_interface_global.h"
#include "ccc_ifo_mbi.h"
#include "slap_vco_internal_api.h"
#include "safec_lib_common.h"

char                     g_SubPrefix[32] = {0 };

char* 
AccessControlBitMaskToAccessList
    (
        ULONG AccessControlBitMask
    )
{
    char* pAccessList = NULL;

    /* Should start with 0x00000001 rather than 0x00000000 */
    if ( AccessControlBitMask == 0x0 )
    {
        pAccessList = AnscCloneString("ACS");
    }
    else if ( AccessControlBitMask == 0x00000001 )
    {
        pAccessList = AnscCloneString("XMPP");
    }
    else if ( AccessControlBitMask == 0x00000002 )
    {
        pAccessList = AnscCloneString("CLI");
    }
    else if ( AccessControlBitMask == 0x00000004 )
    {
        pAccessList = AnscCloneString("WebUI");
    }
    else
    {
        pAccessList = AnscCloneString("ANYBODY");
    }

    return pAccessList;

}


ULONG 
AccessListToAccessControlBitMask
    (
        char * pAccessList
    )
{
    if ( !pAccessList )
    {
        return 0xFFFFFFFF;
    }
    else if ( _ansc_strstr(pAccessList, "ACS") )
    {
        return 0x00000000;
    }
    else if ( _ansc_strstr(pAccessList, "XMPP") )
    {
        return 0x00000001;
    }
    else if ( _ansc_strstr(pAccessList, "CLI") )
    {
        return 0x00000002;
    }
    else if ( _ansc_strstr(pAccessList, "WebUI") )
    {
        return 0x00000004;
    }
    else
    {
        return 0xFFFFFFFF;
    }
}

ANSC_STATUS
mbiIf_SendParameterValueChangeSignal
    (
        void*         hBusHandle,  
        char*         pPamameterName,
        SLAP_VARIABLE * pOldValue,
        SLAP_VARIABLE * pNewValue,
        char * pAccessList
    )
{
    UNREFERENCED_PARAMETER(pAccessList);
    ANSC_STATUS                     returnStatus       = CCSP_SUCCESS;
    parameterSigStruct_t*           pParamSignal       = (parameterSigStruct_t*)NULL;

    pParamSignal = (parameterSigStruct_t*)AnscAllocateMemory(sizeof(parameterSigStruct_t));

    if ( !pParamSignal )
    {
        CcspTraceError(("!!! mbiIf_SendParameterValueChangeSignal No Memory... !!!\n"));

        return ANSC_STATUS_FAILURE;
    }

    pParamSignal->parameterName = AnscCloneString(pPamameterName);

    if ( pOldValue )
    {
        if ( pOldValue->Syntax == SLAP_VAR_SYNTAX_uint32 )
        {
            if ( pOldValue->ContentType == SLAP_CONTENT_TYPE_IP4_ADDR )
            {
                pParamSignal->oldValue = SlapVcoIp4AddrToString(NULL, pOldValue->Variant.varUint32);
                pParamSignal->type = ccsp_string;
            }
            else
            {
                pParamSignal->oldValue = SlapVcoUint32ToString(NULL, pOldValue->Variant.varUint32);
                pParamSignal->type     = ccsp_unsignedInt;
            }
        }
        else if ( pOldValue->Syntax == SLAP_VAR_SYNTAX_string )
        {
            if ( pOldValue->ContentType == SLAP_CONTENT_TYPE_CALENDAR_TIME )
            {
                pParamSignal->type = ccsp_dateTime;
            }
            else
            {
                pParamSignal->type = ccsp_string;
            }

            pParamSignal->oldValue = AnscCloneString(pOldValue->Variant.varString);
        }
        else if ( pOldValue->Syntax == SLAP_VAR_SYNTAX_bool )
        {
            pParamSignal->oldValue = SlapVcoBoolToString(NULL, pOldValue->Variant.varBool); /* This function will be adjust to be consistent with CCSP */
            pParamSignal->type     = ccsp_boolean;
        }
        else if ( pOldValue->Syntax == SLAP_VAR_SYNTAX_int )
        {
            pParamSignal->oldValue = SlapVcoIntToString(NULL, pOldValue->Variant.varInt);
            pParamSignal->type     = ccsp_int;
        }
    }
    else
    {
        pParamSignal->oldValue = NULL;
    }

    if ( pNewValue )
    {
        if ( pNewValue->Syntax == SLAP_VAR_SYNTAX_uint32 )
        {
            pParamSignal->newValue = SlapVcoUint32ToString(NULL, pNewValue->Variant.varUint32);
            pParamSignal->type     = ccsp_unsignedInt;
        }
        else if ( pNewValue->Syntax == SLAP_VAR_SYNTAX_string )
        {
            if ( pNewValue->ContentType == SLAP_CONTENT_TYPE_CALENDAR_TIME )
            {
                pParamSignal->type = ccsp_dateTime;
            }
            else
            {
                pParamSignal->type = ccsp_string;
            }

            pParamSignal->newValue = AnscCloneString(pNewValue->Variant.varString);
        }
        else if ( pNewValue->Syntax == SLAP_VAR_SYNTAX_bool )
        {
            pParamSignal->newValue = SlapVcoBoolToString(NULL, pNewValue->Variant.varBool); /* This function will be adjust to be consistent with CCSP */
            pParamSignal->type      = ccsp_boolean;
        }
        else if ( pNewValue->Syntax == SLAP_VAR_SYNTAX_int )
        {
            pParamSignal->newValue = SlapVcoIntToString(NULL, pNewValue->Variant.varInt);
            pParamSignal->type      = ccsp_int;
        }
			pParamSignal->writeID          = pNewValue->ReqSenderID;
    }
    else
    {
        pParamSignal->newValue = NULL;
    }

    //pParamSignal->writeID          = AccessListToAccessControlBitMask(pAccessList);
    pParamSignal->subsystem_prefix = AnscCloneString(g_SubPrefix);

    returnStatus = 
        CcspBaseIf_SendparameterValueChangeSignal
            (
                hBusHandle,         /* TODO hBusHandle */
                pParamSignal,
                1
            );

    if ( pParamSignal->parameterName )
    {
        AnscFreeMemory((char*)pParamSignal->parameterName);
    }

    if ( pParamSignal->oldValue )
    {
        AnscFreeMemory((char*)pParamSignal->oldValue);
    }

    if ( pParamSignal->newValue )
    {
        AnscFreeMemory((char*)pParamSignal->newValue);
    }

    //Handling memory leak
    if ( pParamSignal->subsystem_prefix )
    {
        AnscFreeMemory((char*)pParamSignal->subsystem_prefix);
    }

    AnscFreeMemory(pParamSignal);

    return (returnStatus == CCSP_SUCCESS) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE;
}


ANSC_STATUS
mbiIf_SendTransferCompleteSignal
    (
        void*         hBusHandle
    )
{
   if ( !hBusHandle )
   {
       CcspTraceError(("!!! Message bus is not ready, CAN NOT signal !!!\n"));
       
       return ANSC_STATUS_NOT_READY;
   }

   CcspTraceInfo(("!!! Ready to send Transfer Complete signal... !!!\n"));

   return CcspBaseIf_SendtransferCompleteSignal(hBusHandle); /* TODO hBusHandle */

}


ANSC_STATUS
mbiIf_SendTransferFailedSignal
    (
        void*         hBusHandle
    )
{
   if ( !hBusHandle )
   {
       CcspTraceError(("!!! Message bus is not ready, CAN NOT signal !!!\n"));

       return ANSC_STATUS_NOT_READY;
   }

   CcspTraceInfo(("!!! Ready to send Transfer Failed signal... !!!\n"));

   return CcspBaseIf_SendtransferFailedSignal(hBusHandle); /* TODO hBusHandle */

}


void*
MsgHelper_CreateCcdMbiIf
    (
        void*                               dbusHandle,
        char*                               pPrefix
    )
{
    errno_t                                 rc        = -1;
    PCCC_MBI_INTERFACE                      pCccMbiIf = NULL;

    if( pPrefix != NULL)
    {
        rc = STRCPY_S_NOCLOBBER(g_SubPrefix, sizeof(g_SubPrefix), pPrefix);
        ERR_CHK(rc);
    }

    pCccMbiIf = (PCCC_MBI_INTERFACE)AnscAllocateMemory(sizeof(CCC_MBI_INTERFACE));

    if ( !pCccMbiIf )
    {
        return NULL;
    }
    else
    {
        rc = STRCPY_S_NOCLOBBER(pCccMbiIf->Name, sizeof(pCccMbiIf->Name), CCC_MBI_INTERFACE_NAME);
        ERR_CHK(rc);

        pCccMbiIf->InterfaceId              = CCC_MBI_INTERFACE_ID;
        pCccMbiIf->hOwnerContext            = (ANSC_HANDLE)dbusHandle;
        pCccMbiIf->Size                     = sizeof(CCC_MBI_INTERFACE);

        pCccMbiIf->SendParameterValueChangeSignal         
                                            = mbiIf_SendParameterValueChangeSignal;
        pCccMbiIf->SendTransferCompleteSignal      
                                            = mbiIf_SendTransferCompleteSignal;
        pCccMbiIf->SendTransferFailedSignal
                                            = mbiIf_SendTransferFailedSignal;
    }

    return (void*)pCccMbiIf;    
}
