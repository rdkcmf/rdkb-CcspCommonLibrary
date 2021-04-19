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

    module:	dslh_wmpdo_utilities.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoParseParamDataType
        *   DslhWmpdoParseParamSyntax
        *   DslhWmpdoParseParamEnumTokens
        *   DslhWmpdoAddVarRecordIntoArray
        *   DslhWmpdoAddObjRecordIntoArray
        *   DslhWmpdoInitObjVarArray
        *   DslhWmpdoResetObjVarArray

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/22/05    initial revision.

**********************************************************************/


#include "dslh_wmpdo_global.h"
#include <stdint.h>
#define  DSLH_WMPDO_MAX_OBJ_INARRAY                         32
#define  DSLH_WMPDO_MAX_VAR_INARRAY                         256


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoParseParamDataType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pDataTypeName,
                PULONG                      pulDataType,
                PULONG                      pulFormatValue1,
                PULONG                      pulFormatValue2,
                char**                      ppEnumTokens
            );

    description:

        This function is called to parse the syntax statement of a
        registered parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pDataTypeName
                Specifies the syntax statement to be parsed.

                PULONG                      pulDataType
                Returns the syntax type of the parameter.

                PULONG                      pulFormatValue1
                Returns the syntax value limiter of the parameter.

                PULONG                      pulFormatValue2
                Returns the syntax value limiter of the parameter.

                char**                      ppEnumTokens
                Returns the enum tokens string of the parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoParseParamDataType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDataTypeName,
        PULONG                      pulDataType,
        PULONG                      pulFormatValue1,
        PULONG                      pulFormatValue2,
        char**                      ppEnumTokens
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PANSC_TOKEN_CHAIN               pDataTypeTokenChain  = (PANSC_TOKEN_CHAIN          )NULL;
    PANSC_STRING_TOKEN              pDataTypeToken       = (PANSC_STRING_TOKEN         )NULL;
    PANSC_TOKEN_CHAIN               pFormatValueChain    = (PANSC_TOKEN_CHAIN          )NULL;
    PANSC_STRING_TOKEN              pFormatValue1        = (PANSC_STRING_TOKEN         )NULL;
    PANSC_STRING_TOKEN              pFormatValue2        = (PANSC_STRING_TOKEN         )NULL;
    char*                           pEnumTokensString    = (char*                      )NULL;
    BOOL                            bNewDataTypeName     = FALSE;
    BOOL                            bKiloBytes           = FALSE;
    ULONG                           uSize1               = 0;
    ULONG                           uSize2               = 0;
    PCHAR                           pFormat              = NULL;

    /*
     * Bin Zhu added here:
     *
     * Since we fail to register "base64" type here, we need another way to take care of it
     */
    if( _ansc_strstr( pDataTypeName,DSLH_CWMP_DATA_NAME_base64) ==  pDataTypeName)
    {
        uSize1 = AnscSizeOfString(DSLH_CWMP_DATA_NAME_base64);
        uSize2 = AnscSizeOfString(pDataTypeName);

        if(uSize1 != uSize2)
        {
            if( pDataTypeName[uSize1] != '(' || pDataTypeName[uSize2 - 1] != ')')
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        *pulDataType     = DSLH_CWMP_DATA_TYPE_base64;
        *pulFormatValue1 = 0;
        *pulFormatValue2 = 0;

        /*
         * A maximum allowed length may be listed using the form base64(N), where N is the maximum
         * length in characters after Base64 encoding.
         */
        pFormat = (PCHAR)(pDataTypeName + uSize1 + 1);

        *pulFormatValue1 = (ULONG)_ansc_atoi(pFormat);

        return returnStatus;
    }

    *pulDataType     = DSLH_CWMP_DATA_TYPE_string;
    *pulFormatValue1 = 0;
    *pulFormatValue2 = 0;
    *ppEnumTokens    = NULL;

    /*
     * For enumerated strings, we create a mapping between each token and its corresponding integer
     * value to simplify the underlying implementation. We MUST remove the enum string tokens
     * before passing the data type into the following parsing algorithm. The simplest way of doing
     * this is changing the ':' characters to the NULL terminator.
     */
    pEnumTokensString = _ansc_strstr(pDataTypeName, "string:");

    if ( pEnumTokensString )
    {
        bNewDataTypeName = TRUE;
        pDataTypeName    = AnscCloneString("string");
        *ppEnumTokens    = pEnumTokensString + AnscSizeOfString("string:");
    }

    pDataTypeTokenChain =
        AnscTcAllocate2
            (
                pDataTypeName,
                DSLH_DATA_TYPE_TOKEN_SEPARATORS,
                DSLH_DATA_TYPE_TOKEN_ALPHABETS
            );

    if ( !pDataTypeTokenChain )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pDataTypeTokenChain) != 1 )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT2;
    }
    else
    {
        pDataTypeToken = AnscTcUnlinkToken(pDataTypeTokenChain);
    }

    if ( strcmp(pDataTypeToken->Name,DSLH_CWMP_DATA_NAME_string) == 0 )
    {
        *pulDataType     = DSLH_CWMP_DATA_TYPE_string;
        *pulFormatValue1 = 0;     /* 16; */
        *pulFormatValue2 = 0;

        pFormatValueChain =
            AnscTcAllocate2
                (
                    pDataTypeName,
                    DSLH_DATA_TYPE_LIMIT_SEPARATORS,
                    DSLH_DATA_TYPE_LIMIT_ALPHABETS
                );

        /*
         * For all strings a maximum length is either explicitly indicated or implied by the size
         * of the elements composing the string. For strings in which the content is an enumeration,
         * the longest enumerated value determines the maximum length. If a string does not have an
         * explicitly indicated maximum length or is not an enumeration, the default maximum is 16
         * characters.
         */
        if ( pFormatValueChain )
        {
            pFormatValue1 = AnscTcUnlinkToken(pFormatValueChain);

            if ( pFormatValue1 )
            {
                if ( (pFormatValue1->Name[AnscSizeOfString(pFormatValue1->Name) - 1] == 'K') ||
                     (pFormatValue1->Name[AnscSizeOfString(pFormatValue1->Name) - 1] == 'k') )
                {
                    bKiloBytes = TRUE;

                    pFormatValue1->Name[AnscSizeOfString(pFormatValue1->Name) - 1] = 0;
                }
                else
                {
                    bKiloBytes = FALSE;
                }

                *pulFormatValue1 = (ULONG)_ansc_atoi(pFormatValue1->Name);

                if ( bKiloBytes )
                {
                    *pulFormatValue1 = (*pulFormatValue1) * 1024;
                }

                AnscFreeMemory(pFormatValue1);
            }

            AnscTcFree((ANSC_HANDLE)pFormatValueChain);
        }
    }
    else if ( strcmp(pDataTypeToken->Name,DSLH_CWMP_DATA_NAME_int) == 0 )
    {
   
        *pulDataType     = DSLH_CWMP_DATA_TYPE_int;
        *pulFormatValue1 = (ULONG)(0 + INT_MIN);
        *pulFormatValue2 = (ULONG)(0 + INT_MAX);

        pFormatValueChain =
            AnscTcAllocate2
                (
                    pDataTypeName,
                    DSLH_DATA_TYPE_LIMIT_SEPARATORS,
                    DSLH_DATA_TYPE_LIMIT_ALPHABETS
                );

        /*
         * Integer in the range -2147483648 to +2147483647, inclusive.
         *
         * For some int types listed, a value range is given using hte form int[Min:Max], where the
         * Min and Max values are inclusive. If either Min or Max are missing, this indicates no
         * limit.
         */
        if ( pFormatValueChain )
        {
            pFormatValue1 = AnscTcUnlinkToken(pFormatValueChain);
            pFormatValue2 = AnscTcUnlinkToken(pFormatValueChain);

            if ( pFormatValue2 )
            {
                *pulFormatValue2 = (ULONG)_ansc_atoi(pFormatValue2->Name);

                AnscFreeMemory(pFormatValue2);
                
	            if ( pFormatValue1 )
	            {
	                *pulFormatValue1 = (ULONG)_ansc_atoi(pFormatValue1->Name);
	
	                AnscFreeMemory(pFormatValue1);
	            }

                AnscTraceVerbose(("'%s' Integer range [%d:%d]\n", pDataTypeName, (int)*pulFormatValue1, (int)*pulFormatValue2));

	        }
			else
            {
	            if ( pFormatValue1 )
	            {
	                if( _ansc_strstr(pDataTypeName, "int[:") != pDataTypeName)
	                {
		                *pulFormatValue1 = (ULONG)_ansc_atoi(pFormatValue1->Name);
					}
					else
					{
		                *pulFormatValue2 = (ULONG)_ansc_atoi(pFormatValue1->Name);					
					}
			
                    AnscTraceVerbose(("'%s' Integer range [%d:%d]\n", pDataTypeName, (int)*pulFormatValue1, (int)*pulFormatValue2));

	                AnscFreeMemory(pFormatValue1);
	            }
            }

            AnscTcFree((ANSC_HANDLE)pFormatValueChain);
        }
    }
    else if ( strcmp(pDataTypeToken->Name,DSLH_CWMP_DATA_NAME_unsignedInt) == 0 )
    {
    
        *pulDataType     = DSLH_CWMP_DATA_TYPE_unsignedInt;
        *pulFormatValue1 = (ULONG)0;
        *pulFormatValue2 = (ULONG)0xFFFFFFFF;

        pFormatValueChain =
            AnscTcAllocate2
                (
                    pDataTypeName,
                    DSLH_DATA_TYPE_LIMIT_SEPARATORS,
                    DSLH_DATA_TYPE_LIMIT_ALPHABETS
                );

        /*
         * Unsigned integer in the range 0 to 4294967295, inclusive.
         *
         * For some unsingedInt types listed, a value range is given using hte form int[Min:Max],
         * where the Min and Max values are inclusive. If either Min or Max are missing, this
         * indicates no limit.
         */
        if ( pFormatValueChain )
        {
            pFormatValue1 = AnscTcUnlinkToken(pFormatValueChain);
            pFormatValue2 = AnscTcUnlinkToken(pFormatValueChain);

            if ( pFormatValue2 )
            {
                *pulFormatValue2 = (ULONG)_ansc_atoi(pFormatValue2->Name);

                AnscFreeMemory(pFormatValue2);
                
	            if ( pFormatValue1 )
	            {
	                *pulFormatValue1 = (ULONG)_ansc_atoi(pFormatValue1->Name);
	
	                AnscFreeMemory(pFormatValue1);
	            }

    			AnscTraceVerbose(("'%s' UINT range [%lu:%lu]\n", pDataTypeName,*pulFormatValue1, *pulFormatValue2));

	        }
			else
            {
	            if ( pFormatValue1 )
	            {
	                if( _ansc_strstr(pDataTypeName, "unsignedInt[:") != pDataTypeName)
	                {
		                *pulFormatValue1 = (ULONG)_ansc_atoi(pFormatValue1->Name);
					}
					else
					{
		                *pulFormatValue2 = (ULONG)_ansc_atoi(pFormatValue1->Name);					
					}
						
        			AnscTraceVerbose(("'%s' UINT range [%lu:%lu]\n", pDataTypeName,*pulFormatValue1, *pulFormatValue2));
	                AnscFreeMemory(pFormatValue1);
	            }
            }


            AnscTcFree((ANSC_HANDLE)pFormatValueChain);
        }
    }
    else if ( strcmp(pDataTypeToken->Name,DSLH_CWMP_DATA_NAME_boolean) == 0 )
    {
        *pulDataType = DSLH_CWMP_DATA_TYPE_boolean;
    }
    else if ( strcmp(pDataTypeToken->Name,DSLH_CWMP_DATA_NAME_dateTime) == 0 )
    {
        *pulDataType     = DSLH_CWMP_DATA_TYPE_dateTime;
        *pulFormatValue1 = AnscSizeOfString("0000-00-00T00:00:00"      );   /* ISO 8601 date-time format */
        *pulFormatValue2 = AnscSizeOfString("0000-00-00T00:00:00-00:00");   /* ISO 8601 date-time format */
    }

    AnscFreeMemory(pDataTypeToken);

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pDataTypeTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pDataTypeTokenChain);
    }

EXIT1:

    if ( bNewDataTypeName && pDataTypeName )
    {
        AnscFreeMemory(pDataTypeName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoParseParamSyntax
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pSyntaxString,
                PULONG                      pulSyntax,
                PULONG                      pulContentType
            );

    description:

        This function is called to parse the syntax statement of a
        registered parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pSyntaxString
                Specifies the syntax statement to be parsed.

                PULONG                      pulSyntax
                Returns the syntax of the parameter.

                PULONG                      pulContentType
                Returns the content type of the parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoParseParamSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSyntaxString,
        PULONG                      pulSyntax,
        PULONG                      pulContentType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_TOKEN_CHAIN               pTokenChainSyntaxDesp = NULL;
    PANSC_STRING_TOKEN              pTokenSyntax          = NULL;
    PANSC_STRING_TOKEN              pTokenContentType     = NULL;

    *pulSyntax      = SLAP_VAR_SYNTAX_string;
    *pulContentType = SLAP_CONTENT_TYPE_UNSPECIFIED;

    pTokenChainSyntaxDesp =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pSyntaxString,
                DSLH_SYNTAX_TOKEN_SEPARATORS
            );

    if ( !pTokenChainSyntaxDesp )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pTokenSyntax      = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainSyntaxDesp);
        pTokenContentType = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChainSyntaxDesp);

        AnscTcFree(pTokenChainSyntaxDesp);
    }

    if ( pTokenSyntax )
    {
    	/* make sure most used types are in the front */
    	if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_bool, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_bool;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_int, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_int;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_string, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_string;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_uint32, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_uint32;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_int, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_int;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_handle, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_handle;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_other, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_other;

    	/* followings are not used today */
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_boolArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_boolArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_charArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_charArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_intArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_intArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_stringArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_stringArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_ucharArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_ucharArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_uint32Array, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_uint32Array;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_ptrArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_ptrArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_handleArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_handleArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_objectArray, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_objectArray;
    	else if ( _ansc_strcmp(SLAP_VAR_SYNTAX_NAME_variable, pTokenSyntax->Name) == 0 )
            *pulSyntax = SLAP_VAR_SYNTAX_TYPE_variable;

        AnscFreeMemory(pTokenSyntax);
    }

    if ( pTokenContentType )
    {
    	/* most used */
    	if ( _ansc_strcmp(SLAP_CONTENT_NAME_MAPPED, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_MAPPED;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_IP4_ADDR, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_IP4_ADDR;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_CALENDAR_TIME, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_CALENDAR_TIME;

    	/* not used today */
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_IP6_ADDR, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_IP6_ADDR;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_IP6_ADDR_LIST, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_IP6_ADDR_LIST;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_ASCII_STRING, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_ASCII_STRING;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_HEX_STRING, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_HEX_STRING;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_URL_STRING, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_URL_STRING;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_SUBNET_MASK, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_SUBNET_MASK;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_MAC_ADDR, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_MAC_ADDR;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_IP4_ADDR_LIST, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_IP4_ADDR_LIST;
        else if ( _ansc_strcmp(SLAP_CONTENT_NAME_IP4_ADDR_RANGE_LIST, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_IP4_ADDR_RANGE_LIST;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_MAC_ADDR_LIST, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_MAC_ADDR_LIST;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_TCPUDP_PORT, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_TCPUDP_PORT;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_PASSWORD, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_PASSWORD;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_MASKED, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_MASKED;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_OID_LIST, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_OID_LIST;
    	else if ( _ansc_strcmp(SLAP_CONTENT_NAME_UNSPECIFIED, pTokenContentType->Name) == 0 )
            *pulContentType = SLAP_CONTENT_TYPE_UNSPECIFIED;

    	AnscFreeMemory(pTokenContentType);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoParseParamEnumTokens
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarEntity,
                char*                       pEnumTokens
            );

    description:

        This function is called to parse the enum toke string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarEntity
                Specifies the var entity whose tokens to be pased.

                char*                       pEnumTokens
                Specifies the token string to be parsed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoParseParamEnumTokens
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity,
        char*                       pEnumTokens
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity       = (PDSLH_VAR_ENTITY_OBJECT    )hVarEntity;
    PANSC_TOKEN_CHAIN               pTokenChainEnums = NULL;
    PANSC_STRING_TOKEN              pTokenEnumString = NULL;
    PANSC_STRING_TOKEN              pTokenEnumCode   = NULL;
    ULONG                           ulTokenCount     = 0;
    ULONG                           ulCharOffset     = 0;
    ULONG                           i                = 0;

    /*
     * For strings listed in TR-069 specification, a maximum allowed length may be listed using the
     * form string(N), where N is the maximum string length in characters. For all strings a
     * maximum length is either explicitly indicated or implied by the size of the elements compos-
     * ing the string. For strings in which the content is an enumeration, the longest enumerated
     * value determines the maximum length. If a string does not have an explicitly indicated
     * maximum length or is not an enumeration, the default maximum is 16 characters.
     *
     * A typical enumerated string should be defined like this:
     *
     *      "string: Normal(1), UseAllocatedSubnet(2), Passthrough(3)"
     */
    pTokenChainEnums =
        AnscTcAllocate
            (
                pEnumTokens,
                ",;()[]{}"
            );
    /*
    pTokenChainEnums =
        AnscTcAllocate2
            (
                pEnumTokens,
                ",:;()[]{}",
                DSLH_DATA_TYPE_ENUMS_ALPHABETS
            );
    */

    if ( !pTokenChainEnums )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        ulTokenCount = AnscTcGetTokenCount(pTokenChainEnums);
    }

    DslhVareoCfgTokenTable((ANSC_HANDLE)pVarEntity, ulTokenCount / 2 + 1);

    for ( i = 0; i < ulTokenCount / 2; i++ )
    {
        ulCharOffset     = 0;
        pTokenEnumString = AnscTcUnlinkToken(pTokenChainEnums);
        pTokenEnumCode   = AnscTcUnlinkToken(pTokenChainEnums);

        while ( pTokenEnumString->Name[ulCharOffset] == ' ' )
        {
            ulCharOffset++;
        }

        DslhVareoAddTokenValue
            (
                (ANSC_HANDLE)pVarEntity,
                &pTokenEnumString->Name[ulCharOffset],
                _ansc_atoi(pTokenEnumCode->Name)
            );

        AnscFreeMemory(pTokenEnumString);
        AnscFreeMemory(pTokenEnumCode  );
    }

    if ( pTokenChainEnums )
    {
        AnscTcFree((ANSC_HANDLE)pTokenChainEnums);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhWmpdoAddVarRecordIntoArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord
            );

    description:

        This function is called to add a VarRecord into the array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the VarRecord to be added.

    return:     array size.

**********************************************************************/

ULONG
DslhWmpdoAddVarRecordIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT*        pVarRecordArray    = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;

    if( pMyObject->ulVarRecordCount < DSLH_WMPDO_MAX_VAR_INARRAY)
    {
        pVarRecordArray[pMyObject->ulVarRecordCount++] = hVarRecord;
    }
    else
    {
        AnscTrace(("Warning: failed to add varrecord, reach MAX_VAR_IN_ARRAY \n"));
    }

    return  pMyObject->ulVarRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhWmpdoAddObjRecordIntoArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to add a ObjRecord into the array.
        If this ObjRecord is added already, it will be ignored.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the ObjRecord to be added.

    return:     array size.

**********************************************************************/

ULONG
DslhWmpdoAddObjRecordIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = (PDSLH_OBJ_RECORD_OBJECT    )hObjRecord;
    PDSLH_OBJ_RECORD_OBJECT*        pObjRecordArray    = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
    ULONG                           i                  = 0;

    /* Too many trace. Disable this -- Yan*/
    /* AnscTrace("pMyObject->ulObjRecordCount == %d.\n", pMyObject->ulObjRecordCount); */

    if( pMyObject->ulObjRecordCount >= DSLH_WMPDO_MAX_OBJ_INARRAY)
    {
        AnscTrace(("Warning: failed to add ObjRecord, reach MAX_OBJ_IN_ARRAY \n"));
    }
    else
    {
        for ( i = 0; i < pMyObject->ulObjRecordCount; i++ )
        {
            if ( pObjRecordArray[i] == pObjRecord )
            {
                return  pMyObject->ulObjRecordCount;
            }
        }

        pObjRecordArray[pMyObject->ulObjRecordCount++] = hObjRecord;
    }

    /* Too many trace. Disable this -- Yan*/
    /* AnscTrace("pMyObject->ulObjRecordCount == %d.\n", pMyObject->ulObjRecordCount); */
    
    return  pMyObject->ulObjRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoInitObjVarArray
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to init the object and variable arrays for
        SetParameterValues.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     Status of the operation;

**********************************************************************/

ANSC_STATUS
DslhWmpdoInitObjVarArray
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    pMyObject->ResetObjVarArray(hThisObject);

    pMyObject->hVarRecordArray = (ANSC_HANDLE)AnscAllocateMemory(sizeof(PDSLH_VAR_RECORD_OBJECT) * DSLH_WMPDO_MAX_VAR_INARRAY);
    pMyObject->hObjRecordArray = (ANSC_HANDLE)AnscAllocateMemory(sizeof(PDSLH_OBJ_RECORD_OBJECT) * DSLH_WMPDO_MAX_OBJ_INARRAY);

    pMyObject->ulVarRecordCount = 0;
    pMyObject->ulObjRecordCount = 0;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoResetObjVarArray
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the object and variable arrays for
        SetParameterValues.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     Status of the operation;

**********************************************************************/

ANSC_STATUS
DslhWmpdoResetObjVarArray
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT*        pVarRecordArray    = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
    PDSLH_OBJ_RECORD_OBJECT*        pObjRecordArray    = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;

    if( pVarRecordArray != NULL)
    {
        AnscFreeMemory(pVarRecordArray);
    }

    if( pObjRecordArray != NULL)
    {
        AnscFreeMemory(pObjRecordArray);
    }

    pMyObject->hVarRecordArray  = NULL;
    pMyObject->hObjRecordArray  = NULL;
    pMyObject->ulVarRecordCount = 0;
    pMyObject->ulObjRecordCount = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoFlushDynObjVar
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDynObjRecord
            );

    description:

        This function is called to flush the old dyn table entry object
        and variable in arrays for SetParameterValues.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
 
                ANSC_HANDLE                 hDynObjRecord
                This handle is the dyn table obj record.

    return:     Status of the operation;

**********************************************************************/

ANSC_STATUS
DslhWmpdoFlushDynObjVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDynObjRecord
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT*        ppVarRecordArray   = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
    PDSLH_OBJ_RECORD_OBJECT*        ppObjRecordArray   = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord         = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pParentObjRecord   = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    ULONG                           ulParameterCount   = (ULONG                      )pMyObject->ulVarRecordCount;
    ULONG                           ulObjectCount      = (ULONG                      )pMyObject->ulObjRecordCount;
    ULONG                           i                  = 0;

    /*CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar !!!!!!!!!!\n"));*/
    /* Search the ppVarRecordArray first */
    if( ppVarRecordArray != NULL)
    {
        for ( i = 0; i < ulParameterCount; i++ )
        {
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 2 !!!!!!!!!!\n"));
            pVarRecord = (PDSLH_VAR_RECORD_OBJECT)ppVarRecordArray[i];
            if ( !pVarRecord )
            {
                continue;
            }
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 2.1  pVarRecord: 0x%p !!!!!!!!!!\n", pVarRecord)); 
            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord;
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 2.2  pObjRecord: 0x%p !!!!!!!!!!\n", pObjRecord));
            if ( !pObjRecord)
            {
                continue;
            }
            pParentObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecord->hParentObjRecord;
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 2.3  pParentObjRecord: 0x%p, hDynObjRecord: 0x%p !!!!!!!!!!\n", pParentObjRecord, hDynObjRecord));

            if ( (ANSC_HANDLE)pParentObjRecord == hDynObjRecord)
            {
                CcspTraceWarning((" !!!!!! We found a matching pVarRecord: %s, delete... !!!!!!!!!!\n", pVarRecord->GetFullName((ANSC_HANDLE)pVarRecord)));
                ppVarRecordArray[i] = NULL;
            }
        }
    }

    /* Secondly we go through the ppObjRecordArray */
    if( ppObjRecordArray != NULL)
    {
        for ( i = 0; i < ulObjectCount; i++ )
        {
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 3 !!!!!!!!!!\n"));
            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)ppObjRecordArray[i];
            if ( !pObjRecord )
            {
                continue;
            }
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 3.1 pObjRecord: 0x%p !!!!!!!!!!\n", pObjRecord)); 
            pParentObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecord->hParentObjRecord;
            CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 3.2 pParentObjRecord: 0x%p, hDynObjRecord: 0x%p !!!!!!!!!!\n", pParentObjRecord, hDynObjRecord));

            if ( (ANSC_HANDLE)pParentObjRecord == hDynObjRecord)
            {
                CcspTraceWarning((" !!!!!! We found a matching pObjRecord: %s, delete... !!!!!!!!!!\n", pObjRecord->FullName));
                ppObjRecordArray[i] = NULL;
            }
        }
    }

    /*CcspTraceWarning(("!!!!!!!!!!!!!!!! in DslhWmpdoFlushDynObjVar 4 !!!!!!!!!!\n"));*/

    return  ANSC_STATUS_SUCCESS;
}
