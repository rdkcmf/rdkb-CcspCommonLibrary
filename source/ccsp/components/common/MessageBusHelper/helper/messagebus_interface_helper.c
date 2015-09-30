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

    module: messagebus_interface_helper.c

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        SSP implementation of the CCSP Message Bus Interface
        Helper Service.

        *   CcspCcMbi_GetParameterValues 
        *   CcspCcMbi_SetParameterValues
        *   CcspCcMbi_SetCommit
        *   CcspCcMbi_SetParameterAttributes
        *   CcspCcMbi_GetParameterAttributes
        *   CcspCcMbi_AddTblRow
        *   CcspCcMbi_DeleteTblRow
        *   CcspCcMbi_GetParameterNames
        *   CcspCcMbi_CurrentSessionIdSignal
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


ULONG    g_uMaxParamInResponse   = 5000/*DSLH_WMPDO_MAX_PARAM_VALUES_IN_RESPONSE*/;
int      g_currentSessionID      = 0;
ULONG    g_lastWriteEntity       = 0;
ULONG    g_currentWriteEntity    = 0;

extern PDSLH_CPE_CONTROLLER_OBJECT      g_pDslhCpeController;


static int 
CcspCcMbi_ValidateBoolean
	(
    	const char * boolStr
    )
{
    /* Only supports "0" or "1" or case free "true" and "false". */
	if ( !boolStr ) return -1;

    if (AnscEqualString(boolStr, "0", FALSE) ||
        AnscEqualString(boolStr, "1", FALSE) ||
        AnscEqualString(boolStr, "true", FALSE) ||
        AnscEqualString(boolStr, "false", FALSE))
	{
		return 0;
	}
	else return -1;
}


static int CcspCcMbi_ValidateINT
	(
    	const char * intStr,
		int signedInt
    )
{
    int i = 0;
    int len = strlen(intStr);
	char* pNext;
	char c;
	char buf[32] = {0};
	int num;

    if(len <= 0) return -1;

    while(*intStr == ' ') {
        intStr++;
        len--;
    }
    if(len <= 0) return -1;

	pNext = intStr;
	while ( c = *pNext )
	{
		if ( c == '+' || c == '-' )
		{
			if ( !signedInt || pNext != intStr )
				return -1;
		} 
		else if ( c < '0' || c > '9' )
			return -1;

		pNext++;
	}

	num = _ansc_atoi(intStr);

	if ( signedInt )
		_ansc_itoa(num, buf, 10);
	else
		_ansc_ultoa((ULONG)num, buf, 10);

	if ( !AnscEqualString(buf, intStr, TRUE) ) return -1;

    return 0;
}


int 
CcspCcMbi_GetParameterValues
    (    
        unsigned int writeID,
        char * parameterNames[],
        int size,
        int *val_size,
        parameterValStruct_t ***val,
        void * user_data
    )
{
    ANSC_STATUS                 returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE         pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_MPR_INTERFACE         pDslhMprIf         = (PDSLH_MPR_INTERFACE        )g_pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)g_pDslhCpeController); 
    PSLAP_STRING_ARRAY          pParamNameArray    = (PSLAP_STRING_ARRAY         )NULL;
    PDSLH_CWMP_PARAM_VALUE      pParamValueArray   = (PDSLH_CWMP_PARAM_VALUE     )NULL;
    ULONG                       ulArraySize        = (ULONG                      )0;
    parameterValStruct_t**      ppReturnVal        = NULL;
    ULONG                       i                  = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
        pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pCpeco->GetDslhMprIf((ANSC_HANDLE)pCpeco); 
    }

    if ( !parameterNames || ( size == 0 ) )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbi_GetParameterValues\n"));

        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto EXIT;
    }


    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);       

    SlapAllocStringArray2(size, pParamNameArray);

    if( pParamNameArray == NULL )
    {
        goto EXIT;
    }

    for ( i = 0; i < (ULONG)size; i++ )
    {
        pParamNameArray->Array.arrayString[i] = AnscCloneString(parameterNames[i]);
    }

    if ( pDslhMpaIf != NULL )
    {
        returnStatus =
            pDslhMpaIf->GetParameterValues
                (
                    pDslhMpaIf->hOwnerContext,
                    DSLH_MPA_ENTITY_ACS,
                    pParamNameArray,
				    g_uMaxParamInResponse,
                    &pParamValueArray,
                    &ulArraySize,
                    writeID
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            /*
             * Something serious has screwed up (e.g. we have run out of system memory), abort!
             */
            AnscTraceError(("CcspCcMbi_GetParameterValues -- Something has screwed up. Failure status returned\n"));

            for ( i = 0; i < (ULONG)size; i++ )
            {
                AnscTraceError((" \t\t when get parameters: %s \n",parameterNames[i]));
            }             

            goto  EXIT1;
        }
        else if( ulArraySize == 0)
        {
            AnscTraceError(("CcspCcMbi_GetParameterValues -- No parameter value returned.\n"));

            goto EXIT1;
        }

        ppReturnVal = AnscAllocateMemory(ulArraySize * sizeof(parameterValStruct_t *));

        if ( !ppReturnVal )
        {
            AnscTraceError(("CcspCcMbi_GetParameterValues -- No Resource\n"));
            
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT2;
        }

        AnscZeroMemory(ppReturnVal, ulArraySize * sizeof(parameterValStruct_t *));

        for ( i = 0; i < ulArraySize; i++ )
        {
            ppReturnVal[i] = AnscAllocateMemory(sizeof(parameterValStruct_t));

            if ( !ppReturnVal[i] )
            {
                AnscTraceError(("CcspCcMbi_GetParameterValues -- No Resource\n"));
            
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT2;
            }

            AnscZeroMemory(ppReturnVal[i], sizeof(parameterValStruct_t));

            ppReturnVal[i]->parameterName = AnscCloneString(pParamValueArray[i].Name);

            if( pParamValueArray[i].Value == NULL)
            {
                AnscTraceError(("CcspCcMbi_GetParameterValues -- No value for '%s'\n", ppReturnVal[i]->parameterName));
                ppReturnVal[i]->parameterValue = SlapVcoIntToString(NULL, 0);
                ppReturnVal[i]->type           = ccsp_unsignedInt;
            }
            else if ( pParamValueArray[i].Value->Syntax == SLAP_VAR_SYNTAX_uint32 )
            {
                if ( pParamValueArray[i].Value->ContentType == SLAP_CONTENT_TYPE_IP4_ADDR )
                {
                    ppReturnVal[i]->parameterValue = SlapVcoIp4AddrToString(NULL, pParamValueArray[i].Value->Variant.varUint32);
                    ppReturnVal[i]->type = ccsp_string;
                }
                else
                {
                    ppReturnVal[i]->parameterValue = SlapVcoUint32ToString(NULL, pParamValueArray[i].Value->Variant.varUint32);
                    ppReturnVal[i]->type           = ccsp_unsignedInt;
                }
            }
            else if ( pParamValueArray[i].Value->Syntax == SLAP_VAR_SYNTAX_string )
            {
                if ( pParamValueArray[i].Value->ContentType == SLAP_CONTENT_TYPE_CALENDAR_TIME )
                {
                    ppReturnVal[i]->type = ccsp_dateTime;
                }
                else
                {
                    ppReturnVal[i]->type = ccsp_string;
                }

                ppReturnVal[i]->parameterValue = AnscCloneString(pParamValueArray[i].Value->Variant.varString);
            }
            else if ( pParamValueArray[i].Value->Syntax == SLAP_VAR_SYNTAX_bool )
            {
                if ( ( writeID == DSLH_MPA_ACCESS_CONTROL_SNMP ) && 
                     ( pParamValueArray[i].Value->Variant.varBool == 2 ) )
                {
                    ppReturnVal[i]->parameterValue = AnscCloneString("notready");;
                }
                else
                {
                ppReturnVal[i]->parameterValue = SlapVcoBoolToString(NULL, pParamValueArray[i].Value->Variant.varBool); /* This function will be adjust to be consistent with CCSP */
                }

                ppReturnVal[i]->type           = ccsp_boolean;
            }
            else if ( pParamValueArray[i].Value->Syntax == SLAP_VAR_SYNTAX_int )
            {
                ppReturnVal[i]->parameterValue = SlapVcoIntToString(NULL, pParamValueArray[i].Value->Variant.varInt);
                ppReturnVal[i]->type           = ccsp_int;
            }
        }
    }

    *val      = ppReturnVal;
    *val_size = ulArraySize;

EXIT2:

    if ( pParamValueArray )
    {
        for ( i = 0; i < ulArraySize; i++ )
        {
            DslhCwmpCleanParamValue((&pParamValueArray[i]));
        }

        AnscFreeMemory(pParamValueArray);
    }

EXIT1:

    if( pParamNameArray != NULL )
    {
        SlapFreeVarArray(pParamNameArray);
    }
    
EXIT:

    return (returnStatus == 0) ? CCSP_SUCCESS : returnStatus;
}


int 
CcspCcMbi_SetParameterValues
    (
        int sessionId,
        unsigned int writeID,
        parameterValStruct_t *val,
        int size,
        dbus_bool commit,
        char ** invalidParameterName,
        void * user_data
    )
{
    ANSC_STATUS                 returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE         pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_MPR_INTERFACE         pDslhMprIf         = (PDSLH_MPR_INTERFACE        )g_pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)g_pDslhCpeController); 
    PDSLH_CWMP_PARAM_VALUE      pParamArray        = NULL;
    PDSLH_CWMP_PARAM_VALUE      pParamValue        = NULL;
    PSLAP_VARIABLE              pSlapVariable      = (PSLAP_VARIABLE)NULL;
    PDSLH_CWMP_SOAP_FAULT       pCwmpSoapFault     = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    int                         iStatus            = 0;
    int                         i                  = 0;


    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
        pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pCpeco->GetDslhMprIf((ANSC_HANDLE)pCpeco); 
    }

    if ( (g_currentSessionID != 0) && (sessionId != g_currentSessionID) )
    {
        AnscTraceWarning(("!!! SetParameterValues discarded, session id is NOT match !!!\n"));

        return ANSC_STATUS_DISCARD;
    }

    if ( !val || size == 0  )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbi_GetParameterValues\n"));

        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for ssp_SsdMbiSetParameterValues\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);       

    pParamArray = (PDSLH_CWMP_PARAM_VALUE)
			AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_VALUE) * size);

    if( pParamArray == NULL)
	{
		returnStatus = ANSC_STATUS_RESOURCES;

		goto EXIT;
	}

    for ( i = 0; i < size; i++ )
    {
        pParamValue = &pParamArray[i];

        pParamValue->Name = AnscCloneString(val[i].parameterName);

        pSlapVariable = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT1;
        }

        SlapInitVariable(pSlapVariable);

        pParamValue->Value          = pSlapVariable;

        pSlapVariable->Name        = NULL;
        pSlapVariable->ContentType = 0;
        pSlapVariable->UsageType   = 0;

        if ( val[i].type == ccsp_string )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(val[i].parameterValue);
        }
        else if ( val[i].type == ccsp_int )
        {
			if ( CcspCcMbi_ValidateINT(val[i].parameterValue, 1) != 0 )
			{
				returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;
		
				goto EXIT1;
			}

            pSlapVariable->Syntax         = SLAP_VAR_SYNTAX_int;
            pSlapVariable->Variant.varInt = SlapVcoStringToInt(NULL, val[i].parameterValue);
        }
        else if ( (val[i].type == ccsp_unsignedInt) || (val[i].type == ccsp_long) )
        {
			if ( CcspCcMbi_ValidateINT(val[i].parameterValue, 0) != 0 )
			{
				returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;
		
				goto EXIT1;
			}

            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = SlapVcoStringToUint32(NULL, val[i].parameterValue);
        }
        else if ( val[i].type == ccsp_boolean )
        {
			if ( CcspCcMbi_ValidateBoolean(val[i].parameterValue) != 0 )
			{
				returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;
		
				goto EXIT1;
			}

            pSlapVariable->Syntax          = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool = SlapVcoStringToBool(NULL, val[i].parameterValue); /* This function will be adjust to be consistent with CCSP */
        }
        else if ( val[i].type == ccsp_dateTime )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->ContentType       = SLAP_CONTENT_TYPE_CALENDAR_TIME;
            pSlapVariable->Variant.varString = AnscCloneString(val[i].parameterValue);
        }
        else if ( val[i].type == ccsp_base64 )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(val[i].parameterValue);
        }
		else
		{
			returnStatus = CCSP_ERR_INVALID_PARAMETER_TYPE;		
			goto EXIT1;
		}
    }

    g_currentWriteEntity = writeID;

    returnStatus =
        pDslhMpaIf->SetParameterValues      /* TODO: sessionID */
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                pParamArray,
                size,
                &iStatus,
                sessionId,
                writeID,
                commit,
                invalidParameterName
            );

EXIT1:

    if( pParamArray != NULL)
    {
        for( i = 0; i < size; i ++)
        {
            pParamValue = &pParamArray[i];

            if( pParamValue->Value == (ANSC_HANDLE)DSLH_CWMP_DATA_INVALID_VALUE_TYPE)
            {
                pParamValue->Value = NULL;
            }

            DslhCwmpCleanParamValue(pParamValue);
        }

        AnscFreeMemory(pParamArray);
    }

EXIT:

    if( *invalidParameterName != NULL)
    {
    	AnscTrace("Invalid parameter name '%s'\n", *invalidParameterName);
    }
    
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        return CCSP_SUCCESS;
    }
    
    AnscTrace("Error = %d in 'CcspCcMbi_SetParameterValues'\n", returnStatus);
    
    return returnStatus;
}


int 
CcspCcMbi_SetCommit
    (
        int sessionId,
        unsigned int writeID,
        dbus_bool commit,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_CWMP_SOAP_FAULT           pCwmpSoapFault     = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    int                             iStatus            = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
    }

    if ( (g_currentSessionID != 0) && (sessionId != g_currentSessionID) )
    {
        AnscTraceWarning(("!!! SetCommit discarded, session id is NOT match !!!\n"));

        return ANSC_STATUS_DISCARD;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for ssp_SsdMbi_AddTblRow\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    returnStatus =
        pDslhMpaIf->SetCommit
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                &iStatus,
                sessionId,
                writeID,
                commit
            );

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : CCSP_FAILURE;
}


int 
CcspCcMbi_SetParameterAttributes
    (
        int sessionId,
        parameterAttributeStruct_t *val,
        int size,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )g_pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)g_pDslhCpeController); 
    ULONG                           uMaxParam          = 128;
    PDSLH_CWMP_SET_PARAM_ATTRIB     pParamArray        = NULL;
    PDSLH_CWMP_SET_PARAM_ATTRIB     pParamAttr         = NULL;
    int                             i                  = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
        pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pCpeco->GetDslhMprIf((ANSC_HANDLE)pCpeco); 
    }

    if ( (g_currentSessionID != 0) && (sessionId != g_currentSessionID) )
    {
        AnscTraceWarning(("!!! SetParameterAttributes discarded, session id is NOT match !!!\n"));

        return ANSC_STATUS_DISCARD;
    }

    if ( !val || size == 0 )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbiSetParameterAttributes\n"));

        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    if( (ULONG)size >= uMaxParam )
    {
        AnscTrace("Too many params in 'SetParameterAttributes'\n");

        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for ssp_SsdMbiSetParameterValues\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);       

    pParamArray = (PDSLH_CWMP_SET_PARAM_ATTRIB)
            AnscAllocateMemory(sizeof(DSLH_CWMP_SET_PARAM_ATTRIB) * size);

    if( pParamArray == NULL)
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    for ( i = 0; i < size; i++ )
    {
        pParamAttr = &pParamArray[i];
        
        pParamAttr->Name                = AnscCloneString(val[i].parameterName);
        pParamAttr->bNotificationChange = val[i].notificationChanged;
        pParamAttr->Notification        = val[i].notification ? DSLH_CWMP_NOTIFICATION_passive : DSLH_CWMP_NOTIFICATION_off;
        pParamAttr->bAccessListChange   = val[i].accessControlChanged;
        pParamAttr->AccessList          = AccessControlBitMaskToAccessList(val[i].accessControlBitmask);
    }

    returnStatus =
        pDslhMpaIf->SetParameterAttributes
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                (ANSC_HANDLE)pParamArray,
                size
            );

    if( pParamArray != NULL)
    {
        for( i = 0; i < size; i ++ )
        {
            pParamAttr = &pParamArray[i];

            DslhCwmpCleanSetParamAttrib(pParamAttr);
        }

        AnscFreeMemory(pParamArray);
    }

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : returnStatus;
}


int 
CcspCcMbi_GetParameterAttributes
    (
        char* parameterNames[],
        int size,
        int* val_size,
        parameterAttributeStruct_t ***val,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )g_pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)g_pDslhCpeController); 
    PSLAP_STRING_ARRAY              pParamNameArray    = (PSLAP_STRING_ARRAY         )NULL;
    PDSLH_CWMP_PARAM_ATTRIB         pParamAttribArray  = (PDSLH_CWMP_PARAM_ATTRIB    )NULL;
    ULONG                           ulArraySize        = (ULONG                      )0;
    parameterAttributeStruct_t**    ppReturnVal        = NULL;
    ULONG                           i                  = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
        pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pCpeco->GetDslhMprIf((ANSC_HANDLE)pCpeco); 
    }

    *val = ppReturnVal;
    *val_size = ulArraySize;

    if ( !parameterNames || ( size == 0 ) )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbi_GetParameterAttributes\n"));

        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto EXIT;
    }

    SlapAllocStringArray2(size, pParamNameArray);

    if( pParamNameArray == NULL )
    {
        goto EXIT;
    }

    for ( i = 0; i < (ULONG)size; i++ )
    {
        pParamNameArray->Array.arrayString[i] = AnscCloneString(parameterNames[i]);
    }

    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);       

    if ( pDslhMpaIf != NULL )
    {
        returnStatus =
            pDslhMpaIf->GetParameterAttributes
                (
                    pDslhMpaIf->hOwnerContext,
                    DSLH_MPA_ENTITY_ACS,
                    pParamNameArray,
				    g_uMaxParamInResponse,
                    &pParamAttribArray,
                    &ulArraySize
                );

        if ( (returnStatus != ANSC_STATUS_SUCCESS) || (ulArraySize == 0) )
        {
            /*
             * Something serious has screwed up (e.g. we have run out of system memory), abort!
             */
            goto  EXIT1;
        }

        ppReturnVal = AnscAllocateMemory(ulArraySize * sizeof(parameterAttributeStruct_t *));

        if ( !ppReturnVal )
        {
            AnscTraceError(("ssp_SsdMbi_GetParameterAttributes -- No Resource\n"));
            
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT2;
        }

        AnscZeroMemory(ppReturnVal, ulArraySize * sizeof(parameterAttributeStruct_t *));

        for ( i = 0; i < ulArraySize; i++ )
        {
            ppReturnVal[i] = AnscAllocateMemory(sizeof(parameterAttributeStruct_t));

            if ( !ppReturnVal[i] )
            {
                AnscTraceError(("ssp_SsdMbi_GetParameterAttributes -- No Resource\n"));
            
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT2;
            }

            AnscZeroMemory(ppReturnVal[i], sizeof(parameterAttributeStruct_t));

            ppReturnVal[i]->parameterName        = AnscCloneString(pParamAttribArray[i].Name);
            ppReturnVal[i]->notification         = (pParamAttribArray[i].Notification == DSLH_CWMP_NOTIFICATION_off) ? FALSE : TRUE;
            ppReturnVal[i]->notificationChanged  = FALSE; /*pParamAttribArray[i].bNotificationChange*/
            ppReturnVal[i]->accessControlBitmask = AccessListToAccessControlBitMask(pParamAttribArray[i].AccessList);
            
        }
    }

    *val = ppReturnVal;
    *val_size = ulArraySize;

EXIT2:

    if ( pParamAttribArray )
    {
        for ( i = 0; i < ulArraySize; i++ )
        {
            DslhCwmpCleanParamAttrib((&pParamAttribArray[i]));
        }

        AnscFreeMemory(pParamAttribArray);
    }

EXIT1:
 
    if( pParamNameArray != NULL)
    {
        SlapFreeVarArray(pParamNameArray);
    }

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : returnStatus;
}


int  
CcspCcMbi_AddTblRow
    (
        int sessionId,
        char * objectName,
        int * instanceNumber,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    int                             iStatus            = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
    }

    if ( (g_currentSessionID != 0) && (sessionId != g_currentSessionID) )
    {
        AnscTraceWarning(("!!! AddTblRow discarded, session id is NOT match !!!\n"));

        return ANSC_STATUS_DISCARD;
    }

    if ( !objectName || !instanceNumber )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbi_AddTblRow\n"));

        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for ssp_SsdMbi_AddTblRow\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    returnStatus =
        pDslhMpaIf->AddObject
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                objectName,
                instanceNumber,
                &iStatus
            );

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : returnStatus;
}


int 
CcspCcMbi_DeleteTblRow
    (
        int sessionId,
        char * objectName,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    int                             iStatus            = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
    }

    if ( (g_currentSessionID != 0) && (sessionId != g_currentSessionID) )
    {
        AnscTraceWarning(("!!! DeleteTblRow discarded, session id is not match !!!\n"));

        return ANSC_STATUS_DISCARD;
    }

    if ( !objectName )
    {
        AnscTraceError(("Input parameter invalid for SsdMbi_DeleteTblRow\n"));

        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto EXIT;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for SsdMbi_DeleteTblRow\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    returnStatus =
        pDslhMpaIf->DeleteObject
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                objectName,
                &iStatus
            );

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : returnStatus;
}


int 
CcspCcMbi_GetParameterNames
    (
        char * parameterName,
        dbus_bool nextLevel,
        int *size ,
        parameterInfoStruct_t ***val,
        void * user_data
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )g_pDslhCpeController->GetDslhMpaIf((ANSC_HANDLE)g_pDslhCpeController);
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )g_pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)g_pDslhCpeController); 
    PDSLH_CWMP_PARAM_INFO           pParamInfoArray    = (PDSLH_CWMP_PARAM_INFO      )NULL;
    ULONG                           ulArraySize        = (ULONG                      )0;
    parameterInfoStruct_t**         ppReturnVal        = NULL;
    ULONG                           i                  = 0;

    /*with component binding, we have multiple CpeController handle, each for a component*/
    if (user_data)
    {
        PDSLH_CPE_CONTROLLER_OBJECT      pCpeco = user_data;

        pDslhMpaIf         = (PDSLH_MPA_INTERFACE        )pCpeco->GetDslhMpaIf((ANSC_HANDLE)pCpeco);
        pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pCpeco->GetDslhMprIf((ANSC_HANDLE)pCpeco); 
    }

    if ( !parameterName )
    {
        AnscTraceError(("Input parameter invalid for ssp_SsdMbi_GetParameterNames\n"));

        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto EXIT;
    }

    if ( !pDslhMpaIf )
    {
        AnscTraceError(("pDslhMpaIf is NULL for ssp_SsdMbi_GetParameterNames\n"));

        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto EXIT;
    }

    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);       

    returnStatus =
        pDslhMpaIf->GetParameterNames
            (
                pDslhMpaIf->hOwnerContext,
                DSLH_MPA_ENTITY_ACS,
                parameterName,
                nextLevel,
                &pParamInfoArray,
                &ulArraySize
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        /*
         * Something serious has screwed up (e.g. we have run out of system memory), abort!
         */
        goto  EXIT1;
    }

    ppReturnVal = AnscAllocateMemory(ulArraySize * sizeof(parameterInfoStruct_t *));

    if ( !ppReturnVal )
    {
        AnscTraceError(("ssp_SsdMbi_GetParameterNames -- No Resource\n"));
            
        returnStatus = ANSC_STATUS_RESOURCES;

        goto EXIT1;
    }

    AnscZeroMemory(ppReturnVal, ulArraySize * sizeof(parameterInfoStruct_t *));

    for ( i = 0; i < ulArraySize; i++ )
    {
        ppReturnVal[i] = AnscAllocateMemory(sizeof(parameterInfoStruct_t));

        if ( !ppReturnVal[i] )
        {
            AnscTraceError(("ssp_SsdMbi_GetParameterNames -- No Resource\n"));
            
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT1;
        }

        AnscZeroMemory(ppReturnVal[i], sizeof(parameterInfoStruct_t));

        ppReturnVal[i]->parameterName = AnscCloneString(pParamInfoArray[i].Name);
        ppReturnVal[i]->writable      = pParamInfoArray[i].bWritable;
    }

EXIT1:

    *val = ppReturnVal;
    *size = i;

    if ( pParamInfoArray )
    {
        for ( i = 0; i < ulArraySize; i++ )
        {
            DslhCwmpCleanParamInfo((&pParamInfoArray[i]));
        }

        AnscFreeMemory(pParamInfoArray);
    }

EXIT:

    return (returnStatus == ANSC_STATUS_SUCCESS) ? CCSP_SUCCESS : returnStatus;
}


void 
CcspCcMbi_CurrentSessionIdSignal
    (
        int priority, /* Can be ignored by Common Components */
        int sessionID,
        void * user_data
    )
{
    AnscTrace("!!! set curent session id to %d !!!\n", sessionID);

    g_currentSessionID = sessionID;
}
