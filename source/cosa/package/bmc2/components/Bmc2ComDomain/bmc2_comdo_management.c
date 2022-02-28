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

    module:	bmc2_comdo_management.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bmc2 Com Domain Object.

        *   Bmc2ComdoGetCommandsByPrefix
        *   Bmc2ComdoGetCommandProperty
        *   Bmc2ComdoAddCommandProperty
        *   Bmc2ComdoDelCommandProperty
        *   Bmc2ComdoDelAllCommandProperties
        *   Bmc2ComdoSetCommandHelpSyntax
        *   Bmc2ComdoParseSimpleArg
        *   Bmc2ComdoParseDepArg
        *   Bmc2ComdoParseArgValueType
        *   Bmc2ComdoParseArgHelpItem
        *   Bmc2ComdoParseArgHelpItems
        *   Bmc2ComdoParseArgValueRange

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.
        03/08/10    add inline syntax help and auto completion support

**********************************************************************/


#include "bmc2_comdo_global.h"


static
void
Bmc2ComdoRemoveEscapedVB
    (
        char*                       pValue
    )
{
    char*                           pCur = pValue;
    char*                           pPtr = pValue;

    if ( !pValue || !_ansc_strchr(pValue, '|') )
    {
        return;
    }

    pCur = pPtr = _ansc_strchr(pValue, '\\');

    if(pCur && pPtr) /*RDKB-5886 , CID-24213, CID-33385, NULL check*/
    {
        while ( *pPtr )
        {
            if ( *pPtr == '\\' && *(pPtr+1) == '|' )
            {
                pPtr ++;
                continue;
            }

            *pCur = *pPtr;

            pCur++;
            pPtr++;
        }

        *pCur = 0;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_STRING_ARRAY*
        Bmc2ComdoGetCommandsByPrefix
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommandPrefix
            );

    description:

        This function is called to retrieve all commands that match the
        specified prefix.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommandPrefix
                Specifies the command prefix to be matched.

    return:     command array.

**********************************************************************/

SLAP_STRING_ARRAY*
Bmc2ComdoGetCommandsByPrefix
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandPrefix
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject            = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY     )NULL;
    PSLAP_STRING_ARRAY              pStringArrayCommands = (PSLAP_STRING_ARRAY     )NULL;
    ULONG                           ulCommandCount       = 0;

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    SlapAllocStringArray2(AnscQueueQueryDepth(&pMyObject->CompoQueue), pStringArrayCommands);

    if ( !pStringArrayCommands )
    {
        AnscReleaseLock(&pMyObject->CompoQueueLock);

        return  NULL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pBmc2CommandProperty->bDisabled )
        {
            continue;
        }

        if ( !pCommandPrefix )
        {
            pStringArrayCommands->Array.arrayString[ulCommandCount++] = AnscCloneString(pBmc2CommandProperty->CommandName);

            continue;
        }
        else if ( AnscSizeOfString(pBmc2CommandProperty->CommandName) < AnscSizeOfString(pCommandPrefix) )
        {
            continue;
        }
        else if ( AnscEqualString2
                    (
                        pBmc2CommandProperty->CommandName,
                        pCommandPrefix,
                        AnscSizeOfString(pCommandPrefix),
                        FALSE
                    ) )
        {
            pStringArrayCommands->Array.arrayString[ulCommandCount++] = AnscCloneString(pBmc2CommandProperty->CommandName);

            continue;
        }
    }

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    pStringArrayCommands->VarCount = ulCommandCount;

    if ( ulCommandCount == 0 )
    {
        SlapFreeVarArray(pStringArrayCommands);

        return  NULL;
    }

    return  pStringArrayCommands;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComdoGetCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommandName
            );

    description:

        This function is called to retrieve a command property by
        matching the command name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommandName
                Specifies the command name to be matched.

    return:     command property.

**********************************************************************/

ANSC_HANDLE
Bmc2ComdoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject            = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY     )NULL;

    if ( !pCommandName )
    {
        return  (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pBmc2CommandProperty->CommandName,pCommandName) == 0 )
        {
            AnscReleaseLock(&pMyObject->CompoQueueLock);

            return  (ANSC_HANDLE)pBmc2CommandProperty;
        }
    }

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoAddCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCommandProperty
            );

    description:

        This function is called to add a command property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCommandProperty
                Specifies the command property to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoAddCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject               = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pNewBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )hCommandProperty;
    PBMC2_COMMAND_PROPERTY          pTmpBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY     )NULL;

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pTmpBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry             = AnscQueueGetNextEntry(pSLinkEntry);

        if ( _ansc_strcmp
                (
                    pNewBmc2CommandProperty->CommandName,
                    pTmpBmc2CommandProperty->CommandName
                ) <= 0 )
        {
            AnscQueuePushEntryBefore
                (
                    &pMyObject->CompoQueue,
                    &pNewBmc2CommandProperty->Linkage,
                    &pTmpBmc2CommandProperty->Linkage
                );

            AnscReleaseLock(&pMyObject->CompoQueueLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscQueuePushEntry(&pMyObject->CompoQueue, &pNewBmc2CommandProperty->Linkage);

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoDelCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommandName
            );

    description:

        This function is called to delete a command property by
        matching the command name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommandName
                Specifies the command name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoDelCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject            = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY     )NULL;

    if ( !pCommandName )
    {
        return  ANSC_STATUS_BAD_NAME;
    }

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pBmc2CommandProperty->CommandName,pCommandName) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->CompoQueue, &pBmc2CommandProperty->Linkage);

            AnscReleaseLock(&pMyObject->CompoQueueLock);

            Bmc2FreeCommandProperty(pBmc2CommandProperty);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    return  ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoDelAllCommandProperties
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all command properties.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoDelAllCommandProperties
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject            = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY     )NULL;

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueuePopEntry(&pMyObject->CompoQueue);

        Bmc2FreeCommandProperty(pBmc2CommandProperty);
    }

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoSetCommandHelpSyntax
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand,
                ULONG                       ulReqArgCount,
                char**                      pReqArgs,
                ULONG                       ulOptArgCount,
                char**                      pOptArgs
            );

    description:

        This function is called to set command help syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command on which the help syntax
                is set.

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                char**                      pReqArgs
                Specifies the syntax of required arguments.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                char**                      pOptArgs
                Specifies the optional argument help syntax.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoSetCommandHelpSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        ULONG                       ulReqArgCount,
        char**                      pReqArgs,
        ULONG                       ulOptArgCount,
        char**                      pOptArgs
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PBMC2_COM_DOMAIN_OBJECT         pMyObject               = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty    = (PBMC2_COMMAND_PROPERTY )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY     )NULL;

    AnscAcquireLock(&pMyObject->CompoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoQueue);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty    = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry             = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pCommand, pBmc2CommandProperty->CommandName) == 0 )
        {
            PBMC2_CMD_HELP_SYNTAX   pCmdHelpSyntax = NULL;

            if ( pBmc2CommandProperty->pHelpSyntax )
            {
                Bmc2FreeCmdHelpSyntax(pBmc2CommandProperty->pHelpSyntax);
                pBmc2CommandProperty->pHelpSyntax = NULL;
            }

            Bmc2CreateCmdHelpSyntax(pCmdHelpSyntax);

            if ( pCmdHelpSyntax )
            {
                if ( pReqArgs && ulReqArgCount != 0 )
                {
                    pCmdHelpSyntax->pReqArgs        = Bmc2ComdoParseHelpSyntax(ulReqArgCount, pReqArgs);
                    if ( pCmdHelpSyntax->pReqArgs )
                    {
                        pCmdHelpSyntax->ulReqArgCount   = ulReqArgCount;
                    }
                }

                if ( pOptArgs && ulOptArgCount != 0 )
                {
                    pCmdHelpSyntax->pOptArgs        = Bmc2ComdoParseHelpSyntax(ulOptArgCount, pOptArgs);
                    if ( pCmdHelpSyntax->pOptArgs )
                    {
                        pCmdHelpSyntax->ulOptArgCount   = ulOptArgCount;
                    }
                }

                if ( ( pReqArgs && ulReqArgCount != 0 && !pCmdHelpSyntax->pReqArgs ) ||
                     ( pOptArgs && ulOptArgCount != 0 && !pCmdHelpSyntax->pOptArgs ) )
                {
                    AnscTrace("[Bmc2ComdoSetCommandHelpSyntax] - failed to set syntax for command '%s'!\n", pCommand);

                    Bmc2FreeCmdHelpSyntax(pCmdHelpSyntax);
                    returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                }
                else
                {
                    pBmc2CommandProperty->pHelpSyntax = pCmdHelpSyntax;
                }
            }

            AnscReleaseLock(&pMyObject->CompoQueueLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseLock(&pMyObject->CompoQueueLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComdoGetCmdArgType
            (
                char*                       pArgSyntax
            )

    description:

        This function is called to get argument type.

    argument:   char*                       pArgSyntax
                Specifies argument syntax.

    return:     Argument type.

**********************************************************************/

ULONG
Bmc2ComdoGetCmdArgType
    (
        char*                       pArgSyntax
    )
{
    ULONG                           ulArgType = BMC2_CMD_ARG_TYPE_unknown;

    if ( AnscEqualString2
            (
                pArgSyntax, 
                BMC2_CMD_ARG_TYPE_NAME_nameless, 
                AnscSizeOfString(BMC2_CMD_ARG_TYPE_NAME_nameless),
                TRUE
            ) )
    {
        ulArgType = BMC2_CMD_ARG_TYPE_nameless;
    }
    else if ( AnscEqualString2
            (
                pArgSyntax, 
                BMC2_CMD_ARG_TYPE_NAME_named, 
                AnscSizeOfString(BMC2_CMD_ARG_TYPE_NAME_named),
                TRUE
            ) )
    {
        ulArgType = BMC2_CMD_ARG_TYPE_named;
    }
    else if ( '$' == pArgSyntax[0] )
    {
        int                         nRet;
        ULONG                       ulArgIndex = 0;

        nRet = _ansc_sscanf(pArgSyntax, BMC2_CMD_ARG_TYPE_NAME_dependent, (PUINT)&ulArgIndex);

        if ( (1 == nRet && ulArgIndex != 0)                             ||
             pArgSyntax[1] == BMC2_CMD_ARG_DEP_TYPE_OPTARGN_REQUIRED    ||
                  pArgSyntax[1] == BMC2_CMD_ARG_DEP_TYPE_OPTARGN_OPTIONAL )
        {
            ulArgType = BMC2_CMD_ARG_TYPE_dependent;
        }
    }

    return  ulArgType;
}
   

/**********************************************************************

    caller:     owner of this object

    prototype:

        PBMC2_CMD_ARGUMENT
        Bmc2ComdoParseHelpSyntax
            (
                ULONG                       ulArgCount,
                char**                      pArgs
            )

    description:

        This function is called to parse command argument
        help syntax.

    argument:   ULONG                       ulArgCount
                Specifies the number of arguments

                char**                      pArgs
                Specifies argument syntax.

    return:     status of operation.

**********************************************************************/

PBMC2_CMD_ARGUMENT
Bmc2ComdoParseHelpSyntax
    (
        ULONG                       ulArgCount,
        char**                      pArgs
    )
{
    PBMC2_CMD_ARGUMENT              pCmdArgs    = NULL;
    ULONG                           i;
    char*                           pArgSyntax  = NULL;
    BOOL                            bOK         = FALSE;

    pCmdArgs = (PBMC2_CMD_ARGUMENT)AnscAllocateMemory(sizeof(BMC2_CMD_ARGUMENT) * ulArgCount);

    if ( !pCmdArgs )
    {
        return  NULL;
    }

    for ( i = 0; i < ulArgCount; i ++ )
    {
        pArgSyntax = pArgs[i];

        if ( AnscSizeOfString(pArgSyntax) < 6 )
        {
            goto EXIT2;
        }

        pCmdArgs[i].Type = Bmc2ComdoGetCmdArgType(pArgSyntax);

        switch ( pCmdArgs[i].Type )
        {
            case    BMC2_CMD_ARG_TYPE_nameless :
            case    BMC2_CMD_ARG_TYPE_named :

                    bOK = Bmc2ComdoParseSimpleArg(pArgSyntax, &pCmdArgs[i].Arg.Simple);

                    break;

            case    BMC2_CMD_ARG_TYPE_dependent :

                    bOK = Bmc2ComdoParseDepArg(pArgSyntax, &pCmdArgs[i].Arg.Dependent);

                    break;

            default :

                    goto EXIT2;

                    break;
        }

        if ( !bOK )
        {
            goto EXIT2;
        }
    }

    goto EXIT1;

EXIT2:

    for ( i = 0; i < ulArgCount; i ++ )
    {
        Bmc2CleanCmdArgument(&pCmdArgs[i]);
    }

    AnscFreeMemory(pCmdArgs);
    pCmdArgs = NULL;

EXIT1:

    return  pCmdArgs;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComdoParseArgValueType
            (
                char*                       pType,
                ULONG                       ulLen,
                PULONG                      pulMinOccur,
                PULONG                      pulMaxOccur,
                PULONG                      pulMinLength,
                PULONG                      pulMaxLength,
				PBOOL						pbStrDupAllowed
            )

    description:

        This function is called to parse value type of 
        argument.

    argument:   char*                       pType
                Specify syntax string that is to be 
                checked.

                ULONG                       ulLen
                The length of syntax string.

                PULONG                      pulMinOccur
                On successful return, it indicates minimum
                occurrences of the value.

                PULONG                      pulMaxOccur
                On successful return, it indicates maximum
                occurrences of the value.

                PULONG                      pulMinLength
                On successful return, it indicates minimum
                length of the value.

                PULONG                      pulMaxLength
                On successful return, it indicates maximum
                length of the value.

				PBOOL						pbStrDupAllowed
				On successful return, in indicates if
				duplicate string from string value range
				is allowed. 

    return:     value type.

**********************************************************************/

ULONG
Bmc2ComdoParseArgValueType
    (
        char*                       pType,
        ULONG                       ulLen,
        PULONG                      pulMinOccur,
        PULONG                      pulMaxOccur,
        PULONG                      pulMinLength,
        PULONG                      pulMaxLength,
		PBOOL						pbStrDupAllowed
    )
{
    ULONG                           ulType        = BMC2_CMD_ARG_VTYPE_unknown;
    char*                           pEnd          = pType + ulLen;
    char*                           pRep;
    char*                           pRepEnd;
    ULONG                           ulTypeLen     = ulLen;
    char*                           pLen;

    *pulMinOccur     = 1;
    *pulMaxOccur     = 1;
    *pulMinLength    = 0;
    *pulMaxLength	 = BMC2_CMD_ARG_TYPE_UINT_max;
	*pbStrDupAllowed = FALSE;

    pLen = _ansc_strchr(pType, BMC2_CMD_ARG_VALUE_LENGTH_START);
    if ( pLen && pLen < pEnd )
    {
        char*                       pLenEnd = _ansc_strchr(pLen, BMC2_CMD_ARG_VALUE_LENGTH_END);
        char*                       pLenSep = NULL;

        if ( !pLenEnd || pLenEnd >= pEnd )
        {
            return  ulType;
        }

        pLenSep = _ansc_strchr(pLen, BMC2_CMD_ARG_DVALUE_SEPARATOR);

        *pulMinLength = *pulMaxLength = (ULONG)_ansc_atol(pLen+1);

        if ( pLenSep && pLenSep < pLenEnd )
        {
            *pulMaxLength = (ULONG)_ansc_atol(pLenSep+1);
        }

        if ( *pulMaxLength < *pulMinLength )
        {
            return  ulType;
        }

        ulTypeLen = pLen - pType;
    }

    pRep = _ansc_strchr(pType, BMC2_CMD_ARG_HELP_DESC_START_SEPARATOR);
    if ( pRep && pRep < pEnd )
    {
        pRepEnd = _ansc_strchr(pRep, BMC2_CMD_ARG_HELP_DESC_END_SEPARATOR);

        if ( !pRepEnd || pRepEnd >= pEnd )
        {
            return  ulType;
        }

        if ( ulTypeLen == ulLen )
        {
            ulTypeLen = pRep - pType;
        }

        if ( TRUE )
        {
            char*                   pSep    = _ansc_strchr(pRep, BMC2_CMD_ARG_DVALUE_SEPARATOR);
            int                     nOccur  = 0;

            if ( pSep && pSep < pEnd )
            {
                nOccur = _ansc_atol(pRep+1);
                if ( nOccur <= 0 )
                {
                    return  ulType;
                }

                *pulMinOccur = (ULONG)nOccur;

                if ( *(pSep+1) == BMC2_CMD_ARG_HELP_DESC_END_SEPARATOR )
                {
                    *pulMaxOccur = BMC2_CMD_ARG_TYPE_UINT_max;
                }
                else
                {
                    nOccur = _ansc_atol(pSep+1);
                    if ( (ULONG)nOccur < *pulMinOccur )
                    {
                        return  ulType;
                    }

                    *pulMaxOccur = (ULONG)nOccur;
                }
            }
            else
            {
                nOccur = _ansc_atol(pRep+1);
                if ( nOccur <= 0 )
                {
                    return  ulType;
                }

                *pulMinOccur = *pulMaxOccur = (ULONG)nOccur;
            }
        }

		/* check if string duplication is allowed */
		if ( *(pRepEnd +1) == BMC2_CMD_ARG_STRING_VALUE_DUP_OK )
		{
			*pbStrDupAllowed = TRUE;
		}
		else
		{
			*pbStrDupAllowed = FALSE;
		}
    }

    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_string) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_string, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_string;
    }
    else     
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_uint) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_uint, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_uint;
    }
    else     
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_int) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_int, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_int;
    }
    else     
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_ip4addr) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_ip4addr, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_ip4addr;
    }
    else     
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_macaddr) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_macaddr, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_macaddr;
    }
    else     
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_hex) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_hex, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_hex;
    }
    else
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_qstring) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_qstring, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_qstring;
    }
    else
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_ip6addr) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_ip6addr, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_ip6addr;
    }
    else
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_ip6prefix) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_ip6prefix, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_ip6prefix;
    }
    else
    if ( ulTypeLen == AnscSizeOfString(BMC2_CMD_ARG_VTYPE_NAME_double) &&
         AnscEqualString2(BMC2_CMD_ARG_VTYPE_NAME_double, pType, ulTypeLen, TRUE) )
    {
        ulType = BMC2_CMD_ARG_VTYPE_double;
    }

    return  ulType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Bmc2ComdoParseArgHelpItem
            (
                char*                       pHelp,
                ULONG                       ulLen,
                PBMC2_CMD_ARG_HELP          pHelpItem
            )

    description:

        This function is called to parse help items of a
        given argument.

    argument:   char*                       pHelp
                Help item syntax to be checked.

                ULONG                       ulLen
                The length of help items.

                PBMC2_CMD_ARG_HELP          pHelpItem
                Where the parsing result is saved.

    return:     operation status.

**********************************************************************/

BOOL
Bmc2ComdoParseArgHelpItem
    (
        char*                       pHelp,
        ULONG                       ulLen,
        PBMC2_CMD_ARG_HELP          pHelpItem
    )
{
    char*                           pEnd        = pHelp + ulLen;
    char*                           pDesc       = NULL;
    char*                           pDescEnd    = NULL;
    ULONG                           ulItemLen;
    char*                           pItem;

    pDesc = _ansc_strchr(pHelp, BMC2_CMD_ARG_HELP_DESC_START_SEPARATOR);
    if ( pDesc && pDesc < pEnd )
    {
        pDescEnd = _ansc_strchr(pDesc, BMC2_CMD_ARG_HELP_DESC_END_SEPARATOR);
        if ( !pDescEnd || pDescEnd >= pEnd )
        {
            return  FALSE;
        }
    }
    else
    {
        pDesc = NULL;
    }

    ulItemLen = pDesc ? pDesc - pHelp : pEnd - pHelp;
    if ( 0 != ulItemLen )
    {
        pItem = (char*)AnscAllocateMemory(ulItemLen+1);
        if ( !pItem )
        {
            return FALSE;
        }

        AnscCopyMemory(pItem, pHelp, ulItemLen);
        pItem[ulItemLen] = 0;
        Bmc2ComdoRemoveEscapedVB(pItem);
        pHelpItem->pItem = pItem;
    }

    if ( pDesc && pDescEnd )
    {
        ulItemLen = pDescEnd - pDesc - 1;

        if ( 0 != ulItemLen )
        {
            pItem = (char*)AnscAllocateMemory(ulItemLen+1);
            if ( !pItem )
            {
                return FALSE;
            }

            AnscCopyMemory(pItem, pDesc+1, ulItemLen);
            pItem[ulItemLen] = 0;
            Bmc2ComdoRemoveEscapedVB(pItem);
            pHelpItem->pDescription= pItem;
        }
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PBMC2_CMD_ARG_HELP
        Bmc2ComdoParseArgHelpItems
            (
                char*                       pHelp,
                ULONG                       ulLen,
                PULONG                      pulItemCount
            )

    description:

        This function is called to parse help items of a
        given argument.

    argument:   char*                       pHelp
                Help item syntax to be checked.

                ULONG                       ulLen
                The length of help items.

                PULONG                      pulItemCount
                On successful return, it indicates the number
                of help items parsed.

    return:     argument help items.

**********************************************************************/

PBMC2_CMD_ARG_HELP
Bmc2ComdoParseArgHelpItems
    (
        char*                       pHelp,
        ULONG                       ulLen,
        PULONG                      pulItemCount
    )
{
    PBMC2_CMD_ARG_HELP              pArgHelp = NULL;
    ULONG                           ulItems  = 0;
    char*                           pEnd     = pHelp + ulLen;
    char*                           pNext    = pHelp + ulLen;
    char*                           pBar;
    char*                           pCur = pHelp;
    ULONG                           i, ulTotalHelpCount = 0;

    pBar    = pHelp;

    while ( pBar && pBar < pEnd )
    {
        pBar = _ansc_strchr(pBar, BMC2_CMD_ARG_HELP_SEPARATOR);

        /* escaped vertical bar may appear in string value itself */
        if ( pBar > pHelp && *(pBar-1) == '\\' )
        {
            pNext = pBar + 1;

            while ( pNext < pEnd )
            {
                pBar = _ansc_strchr(pNext, BMC2_CMD_ARG_HELP_SEPARATOR);
                if ( pBar && *(pBar-1) == '\\' )
                {
                    pNext = pBar + 1;
                    pBar = NULL;
                }
                else
                {
                    break;
                }
            }
        }

        ulItems ++;
        
        if ( pBar && pBar < pEnd )
        {
            pBar ++;
        }
        else
        {
            break;
        }
    }

    pArgHelp = (PBMC2_CMD_ARG_HELP)AnscAllocateMemory(sizeof(BMC2_CMD_ARG_HELP) * ulItems);

    if ( !pArgHelp )
    {
        return  NULL;
    }

    ulTotalHelpCount = ulItems;

    ulItems = 0;
    while ( pCur )
    {
        ULONG                       ulItemLen;
        BOOL                        bOK;

        pBar = _ansc_strchr(pCur, BMC2_CMD_ARG_HELP_SEPARATOR);

        /* escaped vertical bar may appear in string value itself */
        if ( pBar > pHelp && *(pBar-1) == '\\' )
        {
            pNext = pBar + 1;

            while ( pNext < pEnd )
            {
                pBar = _ansc_strchr(pNext, BMC2_CMD_ARG_HELP_SEPARATOR);
                if ( pBar && *(pBar-1) == '\\' )
                {
                    pNext = pBar + 1;
                    pBar = NULL;
                }
                else
                {
                    break;
                }
            }
        }
        
        if ( pBar && pBar < pEnd )
        {
            ulItemLen = pBar - pCur;
        }
        else 
        {
            ulItemLen = pEnd - pCur;
        }

        bOK = Bmc2ComdoParseArgHelpItem(pCur, ulItemLen, &pArgHelp[ulItems]);
        if ( !bOK )
        {
            goto EXIT2;
        }

        ulItems ++;
        if ( pBar && pBar < pEnd )
        {
            pCur = pBar + 1;
        }
        else 
        {
            break;
        }
    }

    if ( ulTotalHelpCount != ulItems )
    {
        goto EXIT2;
    }

    goto EXIT1;

EXIT2:

    if ( pArgHelp )
    {
        for ( i = 0; i < ulTotalHelpCount; i ++ )
        {
            Bmc2CleanCmdArgHelp(&pArgHelp[i]);
        }

        AnscFreeMemory(pArgHelp);
        ulTotalHelpCount = 0;
        pArgHelp = NULL;
    }

EXIT1:

    if ( pulItemCount )
    {
        *pulItemCount = ulTotalHelpCount;
    }

    return  pArgHelp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PBMC2_CMD_ARG_VRANGE
        Bmc2ComdoParseArgValueRange
            (
                char*                       pValue,
                ULONG                       ulLen,
                PBMC2_CMD_ARG_VRANGE        pArgVrange
            )

    description:

        This function is called to parse value range of
        an argument.

    argument:   char*                       pValue
                Value range string representation.

                ULONG                       ulLen
                The length of value range string.

                PBMC2_CMD_ARG_VRANGE        pArgVrange
                Specifies argument value range.

    return:     operation status.

**********************************************************************/


static
BOOL
Bmc2ComdoParseStringArgValueRange
    (
        char*                       pValue,
        ULONG                       ulLen,
        PBMC2_CMD_ARG_VRANGE        pArgVrange
    )
{
    char*                           pCur        = pValue;
    char*                           pEnd        = pValue + ulLen;
    ULONG                           ulItemCount = 0;
    ULONG                           ulItemLen;
    char*                           pSep;
    BOOL                            bSucc       = FALSE;
    char**                          pStrArray;
    char*                           pNext;

    while ( pCur )
    {
        pSep = _ansc_strchr(pCur, BMC2_CMD_ARG_HELP_SEPARATOR);

        /* escaped vertical bar may appear in string value itself */
        if ( pSep > pValue && *(pSep-1) == '\\' )
        {
            pNext = pSep + 1;

            while ( pNext < pEnd )
            {
                pSep = _ansc_strchr(pNext, BMC2_CMD_ARG_HELP_SEPARATOR);
                if ( pSep && *(pSep-1) == '\\' )
                {
                    pNext = pSep + 1;
                    pSep = NULL;
                }
                else
                {
                    break;
                }
            }
        }

        ulItemCount ++;

        if ( pSep && pSep < pEnd )
        {
            pCur = pSep + 1;
        }
        else
        {
            break;
        }
    }

    pStrArray   = (char**)AnscAllocateMemory(sizeof(char*) * ulItemCount);
    if ( !pStrArray )
    {
        goto EXIT2;
    }
    pArgVrange->pStringArray = pStrArray;

    pCur = pValue;
    ulItemCount = 0;
    while ( pCur )
    {
        char*                   pItem;

        pSep = _ansc_strchr(pCur, BMC2_CMD_ARG_HELP_SEPARATOR);

        /* escaped vertical bar may appear in string value itself */
        if ( pSep > pValue && *(pSep-1) == '\\' )
        {
            pNext = pSep + 1;

            while ( pNext < pEnd )
            {
                pSep = _ansc_strchr(pNext, BMC2_CMD_ARG_HELP_SEPARATOR);
                if ( pSep && *(pSep-1) == '\\' )
                {
                    pNext = pSep + 1;
                    pSep = NULL;
                }
                else
                {
                    break;
                }
            }
        }

        ulItemLen = (pSep && pSep < pEnd) ? pSep - pCur : pEnd - pCur;
        if ( 0 == ulItemLen )
        {
            goto EXIT2;
        }

        pItem = (char*)AnscAllocateMemory(ulItemLen+1);
        if ( !pItem )
        {
            goto EXIT2;
        }

        AnscCopyMemory(pItem, pCur, ulItemLen);
        pItem[ulItemLen] = 0;

        Bmc2ComdoRemoveEscapedVB(pItem);

        pStrArray[ulItemCount] = pItem;

        ulItemCount ++;

        if ( pSep && pSep < pEnd )
        {
            pCur = pSep + 1;
        }
        else
        {
            break;
        }
    }

    /* pArgVrange->Min.varUint   = pArgVrange->Max.varUint = ulItemCount; */
    pArgVrange->ulStringCount = ulItemCount;

    bSucc = TRUE;

    goto EXIT1;

EXIT2:

    bSucc = FALSE;

EXIT1:

    return  bSucc;
}


PBMC2_CMD_ARG_VRANGE
Bmc2ComdoParseArgValueRange
    (
        ULONG                       ValueType,
        char*                       pValue,
        ULONG                       ulLen,
        PBOOL                       pbSucc
    )
{
    PBMC2_CMD_ARG_VRANGE            pArgVrange = NULL;
    char*                           pEnd       = pValue + ulLen;
    char*                           pSep;

    *pbSucc = FALSE;

    switch ( ValueType )
    {
        case    BMC2_CMD_ARG_VTYPE_macaddr :

                *pbSucc = TRUE;

                return  NULL;

                break;
    }

    Bmc2CreateCmdArgValueRange(pArgVrange);

    if ( !pArgVrange )
    {
        return NULL;
    }

    switch ( ValueType )
    {
        case    BMC2_CMD_ARG_VTYPE_int :
        case    BMC2_CMD_ARG_VTYPE_uint :
        case    BMC2_CMD_ARG_VTYPE_double :
        case    BMC2_CMD_ARG_VTYPE_hex :

                {
                    char*           pMin;
                    char*           pMax;
                    char*           pStrEnum = pValue;

                    if ( *pValue == BMC2_CMD_ARG_HELP_DESC_START_SEPARATOR )
                    {
                        pMin = pValue + 1;
                        pSep = _ansc_strchr(pMin, BMC2_CMD_ARG_DVALUE_SEPARATOR);

                        if ( !pSep || pSep > pEnd )
                        {
                            /* min and max are the same */
                            if ( ValueType != BMC2_CMD_ARG_VTYPE_double )
                            {
                                pArgVrange->Min.varInt = pArgVrange->Max.varInt = _ansc_atol(pMin);
                            }
                            else
                            {
                                pArgVrange->Min.varDouble = pArgVrange->Max.varDouble = _ansc_atof(pMin);
                            }
                        }
                        else
                        {
                            if ( pSep == pMin )
                            {
                                if ( ValueType == BMC2_CMD_ARG_VTYPE_int )
                                {
                                    pArgVrange->Min.varInt = BMC2_CMD_ARG_TYPE_INT_min;
                                }
                                else if ( ValueType == BMC2_CMD_ARG_VTYPE_uint )
                                {
                                    pArgVrange->Min.varUint = BMC2_CMD_ARG_TYPE_UINT_min;
                                }
                                else
                                {
                                    pArgVrange->Min.varDouble = BMC2_CMD_ARG_TYPE_DOUBLE_min;
                                }
                            }
                            else if ( ValueType != BMC2_CMD_ARG_VTYPE_double )
                            {
                                pArgVrange->Min.varInt = _ansc_atol(pMin);
                            }
                            else
                            {
                                pArgVrange->Min.varDouble = _ansc_atof(pMin);
                            }

                            pMax = pSep+1;

                            if ( pMax+1 >= pEnd || *(pMax+1) == BMC2_CMD_ARG_HELP_SEPARATOR )
                            {
                                if ( ValueType == BMC2_CMD_ARG_VTYPE_int )
                                {
                                    pArgVrange->Max.varInt = BMC2_CMD_ARG_TYPE_INT_max;
                                }
                                else if ( ValueType == BMC2_CMD_ARG_VTYPE_uint )
                                {
                                    pArgVrange->Max.varUint = BMC2_CMD_ARG_TYPE_UINT_max;
                                }
                                else
                                {
                                    pArgVrange->Max.varDouble = BMC2_CMD_ARG_TYPE_DOUBLE_max;
                                }
                            }
                            else if ( ValueType != BMC2_CMD_ARG_VTYPE_double )
                            {
                                pArgVrange->Max.varInt = _ansc_atol(pMax);
                            }
                            else
                            {
                                pArgVrange->Max.varDouble = _ansc_atof(pMax);
                            }

                            if ( ValueType == BMC2_CMD_ARG_VTYPE_int &&
                                 pArgVrange->Min.varInt > pArgVrange->Max.varInt )
                            {
                                goto EXIT2;
                            }
                            else if ( ValueType == BMC2_CMD_ARG_VTYPE_uint &&
                                      pArgVrange->Min.varUint > pArgVrange->Max.varUint )
                            {
                                goto EXIT2;
                            }
                            else if ( ValueType == BMC2_CMD_ARG_VTYPE_double &&
                                      pArgVrange->Min.varDouble > pArgVrange->Max.varDouble )
                            {
                                goto EXIT2;
                            }
                        }

                        pStrEnum = _ansc_strchr(pValue, BMC2_CMD_ARG_HELP_SEPARATOR);
                        if ( pStrEnum && pStrEnum < pEnd - 1 )
                        {
                            pStrEnum ++;
                        }
                        else
                        {
                            pStrEnum = NULL;
                        }
                    }

                    if ( pStrEnum && !Bmc2ComdoParseStringArgValueRange(pStrEnum, pEnd - pStrEnum, pArgVrange) )
                    {
                        goto EXIT2;
                    }
                }

                break;


        case    BMC2_CMD_ARG_VTYPE_string :

                *pbSucc = Bmc2ComdoParseStringArgValueRange(pValue, ulLen, pArgVrange);

                if ( *pbSucc )
                {
                    pArgVrange->Min.varUint = pArgVrange->Max.varUint = pArgVrange->ulStringCount;
                }
                else
                {
                    goto EXIT2;
                }

                break;

        case    BMC2_CMD_ARG_VTYPE_ip4addr :

                {
                    char*                       pFrom;
                    char*                       pTo;
                    UCHAR                       a, b, c, d;
                    int                         nRet;

                    pFrom = pValue + 1;
                    pSep = _ansc_strchr(pFrom, BMC2_CMD_ARG_DVALUE_SEPARATOR);

                    if ( !pSep || pSep > pEnd )
                    {
                        /* min and max are the same */
                        nRet = _ansc_sscanf("%u.%u.%u.%u", pFrom, &a, &b, &c, &d);
                        if ( nRet != 4 )
                        {
                            goto EXIT2;
                        }
                        pArgVrange->Min.varUint = pArgVrange->Max.varUint = (a << 24) + (b << 16) + (c << 8) + d;
                    }
                    else
                    {
                        pTo = pSep + 1;

                        nRet = _ansc_sscanf("%u.%u.%u.%u", pTo, &a, &b, &c, &d);
                        if ( nRet != 4 )
                        {
                            goto EXIT2;
                        }
                        pArgVrange->Min.varUint = (a << 24) + (b << 16) + (c << 8) + d;

                        nRet = _ansc_sscanf("%u.%u.%u.%u", pSep+1, &a, &b, &c, &d);
                        if ( nRet != 4 )
                        {
                            goto EXIT2;
                        }
                        pArgVrange->Max.varUint = (a << 24) + (b << 16) + (c << 8) + d;
                    }
                }

                break;

    }

    *pbSucc = TRUE;
    goto EXIT1;

EXIT2:

    Bmc2CleanCmdArgValueRange(pArgVrange);
    pArgVrange = NULL;

EXIT1:

    return  pArgVrange;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Bmc2ComdoParseSimpleArg
            (
                char*                       pArgSyntax,
                PBMC2_CMD_SIMPLE_ARG        pCmdSimpleArg
            )

    description:

        This function is called to parse value range of
        the specified simple argument.

    argument:   char*                       pArgSyntax
                Specifies argument syntax to be parsed.

                PBMC2_CMD_SIMPLE_ARG        pCmdSimpleArg
                The parsing result of the specified simple
                argument - named or nameless argument.

    return:     operation status.

**********************************************************************/

BOOL
Bmc2ComdoParseSimpleArg
    (
        char*                       pArgSyntax,
        PBMC2_CMD_SIMPLE_ARG        pCmdSimpleArg
    )
{
    char*                           pCur;
    char*                           pNext;
    ULONG                           ulLen;
    BOOL                            bSucc  = FALSE;

    pCur = pArgSyntax;

    /* skip arg type field */
    pNext = _ansc_strchr(pCur, BMC2_CMD_ARG_FIELD_SEPARATOR);
    if ( !pNext )
    {
        goto EXIT;
    }

    /* parse field argument name or display name */
    pCur  = pNext + 1;
    pNext = _ansc_strchr(pCur, BMC2_CMD_ARG_FIELD_SEPARATOR);
    if ( !pNext )
    {
        goto EXIT;
    }

    ulLen = pNext - pCur;
    if ( ulLen != 0 )
    {
        char*                       pNameHelpStart = _ansc_strchr(pCur, BMC2_CMD_ARG_HELP_DESC_START_SEPARATOR);
        char*                       pNameHelpEnd   = NULL;

        if ( pNameHelpStart && pNameHelpStart < pNext )
        {
            pNameHelpEnd = _ansc_strchr(pNameHelpStart+1, BMC2_CMD_ARG_HELP_DESC_END_SEPARATOR);

            if ( !pNameHelpEnd || pNameHelpEnd > pNext )
            {
                goto EXIT;
            }
        }
        else
        {
            pNameHelpStart = NULL;
        }

        if ( pNameHelpStart )
        {
            ulLen = pNameHelpStart - pCur;
        }

        if ( ulLen != 0 )
        {
            if ( !pCmdSimpleArg ) /*RDKB-5886 , CID-24104, null check*/
            {
                goto EXIT;
            }
            pCmdSimpleArg->DisplayName = (char*)AnscAllocateMemory(ulLen + 1);
            if(! pCmdSimpleArg->DisplayName)
            {
                goto EXIT;
            }

            AnscCopyMemory(pCmdSimpleArg->DisplayName, pCur, ulLen);
            pCmdSimpleArg->DisplayName[ulLen] = 0;
        }

        if ( pNameHelpEnd && pNameHelpEnd - pNameHelpStart > 1 )
        {
            ulLen = pNameHelpEnd - pNameHelpStart - 1;

            pCmdSimpleArg->DisplayNameHelp = (char*)AnscAllocateMemory(ulLen+1);

            if ( !pCmdSimpleArg->DisplayNameHelp )
            {
                goto EXIT;
            }

            AnscCopyMemory(pCmdSimpleArg->DisplayNameHelp, pNameHelpStart+1, ulLen);
            pCmdSimpleArg->DisplayNameHelp[ulLen] = 0;
        }
    }

    /* parse field data type */
    pCur  = pNext + 1;
    pNext = _ansc_strchr(pCur, BMC2_CMD_ARG_FIELD_SEPARATOR);
    if ( !pNext )
    {
        goto EXIT;
    }

    ulLen = pNext - pCur;
    pCmdSimpleArg->ValueType = 
        Bmc2ComdoParseArgValueType
            (
                pCur, 
                ulLen, 
                &pCmdSimpleArg->ValueMinCount, 
                &pCmdSimpleArg->ValueMaxCount,
                &pCmdSimpleArg->ValueMinLength,
                &pCmdSimpleArg->ValueMaxLength,
				&pCmdSimpleArg->bStrDupAllowed
            );

    if ( pCmdSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_unknown )
    {
        /* unsupported data type */
        goto EXIT;
    }

    /* parse help text */
    pCur  = pNext + 1;
    pNext = _ansc_strchr(pCur, BMC2_CMD_ARG_FIELD_SEPARATOR);
    if ( !pNext )
    {
        goto EXIT;
    }

    ulLen = pNext - pCur;

    if ( ulLen != 0 )
    {
        pCmdSimpleArg->pHelpItems = 
            Bmc2ComdoParseArgHelpItems
                (
                    pCur, 
                    ulLen, 
                    &pCmdSimpleArg->HelpItemCount
                );
    }

    /* parse value range */
    pCur  = pNext + 1;
    pNext = _ansc_strchr(pCur, BMC2_CMD_ARG_FIELD_SEPARATOR);
    ulLen = pNext ? (ULONG)(pNext - pCur) : AnscSizeOfString(pCur);

    if ( ulLen != 0 )
    {
        BOOL                        bOK;

        pCmdSimpleArg->pValueRange = 
            Bmc2ComdoParseArgValueRange
                (
                    pCmdSimpleArg->ValueType, 
                    pCur, 
                    ulLen, 
                    &bOK
                );

        if ( !bOK )
        {
            goto EXIT;
        }
    }

    bSucc = TRUE;

EXIT:

    return  bSucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Bmc2ComdoParseDepArg
            (
                char*                       pArgSyntax,
                PBMC2_CMD_DEP_ARG           pCmdDepArg
            )

    description:

        This function is called to parse value range of
        the specified dependent argument.

    argument:   char*                       pArgSyntax
                Specifies argument syntax to be parsed.

                PBMC2_CMD_DEP_ARG           pCmdDepArg
                The parsing result of the specified dependent
                argument.

    return:     operation status.

**********************************************************************/

BOOL
Bmc2ComdoParseDepArg
    (
        char*                       pArgSyntax,
        PBMC2_CMD_DEP_ARG           pCmdDepArg
    )
{
    char*                           pCur;
    char*                           pDesc;
    BOOL                            bSucc       = FALSE;
    char*                           pEqual;
    ULONG                           ulBrCount   = 0;
    PBMC2_CMD_DEP_ARG_BRANCH        pDepBranch  = NULL;
    ULONG                           ulItemLen;
    char*                           pItem;
    char*                           pSimArgSyn;

    pCur        = pArgSyntax;

    while ( pCur && *pCur != 0 )
    {
        pDesc = _ansc_strchr(pCur, BMC2_CMD_ARG_DEP_SEP);
        if ( !pDesc )
        {
            goto EXIT;
        }

        ulBrCount ++;

        pCur  = pDesc + 1;
        pDesc = _ansc_strchr(pCur, BMC2_CMD_ARG_DEP_SEP);

        if ( !pDesc )
        {
            break;
        }
        else
        {
            pCur = pDesc + 1;
        }
    }

    pCmdDepArg->pArgBranches = 
        (PBMC2_CMD_DEP_ARG_BRANCH)AnscAllocateMemory
            (
                sizeof(BMC2_CMD_DEP_ARG_BRANCH) * ulBrCount
            );
    if ( !pCmdDepArg->pArgBranches )
    {
        goto EXIT;
    }
    pCmdDepArg->ulArgBranchCount = ulBrCount;

    pCur        = pArgSyntax;
    ulBrCount   = 0;

    while ( pCur && *pCur != 0 )
    {
        pDesc = _ansc_strchr(pCur, BMC2_CMD_ARG_DEP_SEP);
        if ( !pDesc )
        {
            goto EXIT;
        }

        if ( *pCur != '$' )
        {
            goto EXIT;
        }

        pEqual = _ansc_strchr(pCur, BMC2_CMD_ARG_DEP_CONDITION_SEP);

        if ( !pEqual || pEqual > pDesc )
        {
            goto EXIT;
        }

        pDepBranch = &pCmdDepArg->pArgBranches[ulBrCount];

        pDepBranch->ulArgDepOn = BMC2_CMD_DEP_ARG_DEP_required;
        pDepBranch->ulArgIndex = (ULONG)_ansc_atol(pCur+1);
        if ( pDepBranch->ulArgIndex == 0 )
        {
            /* check if this argument depends on an optional named argument */
            if ( *(pCur+1) == BMC2_CMD_ARG_DEP_TYPE_OPTARGN_REQUIRED )
            {
                pDepBranch->ulArgDepOn = BMC2_CMD_DEP_ARG_DEP_opt_arg_name;
            }
            else if ( *(pCur+1) == BMC2_CMD_ARG_DEP_TYPE_OPTARGN_OPTIONAL )
            {
                pDepBranch->ulArgDepOn = BMC2_CMD_DEP_ARG_DEP_opt_arg_name_optional;
            }
            else
            {
                return FALSE;
            }

            if ( ulBrCount > 1 )
            {
                /* only 1 branch is allowed for argument depends on a named optional argument */
                return FALSE;
            }
        }

        ulItemLen = pDesc - pEqual - 1;
        if ( ulItemLen != 0 )
        {
            pItem = (char*)AnscAllocateMemory(ulItemLen+1);
            if ( !pItem )
            {
                return FALSE;
            }
            AnscCopyMemory(pItem, pEqual+1, ulItemLen);
            pItem[ulItemLen] = 0;

            pDepBranch->pArgValue = pItem;
        }
        else if ( pDepBranch->ulArgDepOn != BMC2_CMD_DEP_ARG_DEP_required )
        {
            /* name must be specified if this argument depends on an named optional argument */
            return  FALSE;
        }

        pSimArgSyn = pDesc+1;

        /* next branch */
        pDesc = _ansc_strchr(pDesc+1, BMC2_CMD_ARG_DEP_SEP);
        if ( pDesc )
        {
            *pDesc = 0;
        }

        pDepBranch->Type = Bmc2ComdoGetCmdArgType(pSimArgSyn);
        bSucc = Bmc2ComdoParseSimpleArg(pSimArgSyn, &pDepBranch->SimpleArg);

        if ( !bSucc )
        {
            return FALSE;
        }

        ulBrCount ++;

        pCur = pDesc ? pDesc + 1 : NULL;
    }

    bSucc = TRUE;

EXIT:

    return  bSucc;
}

