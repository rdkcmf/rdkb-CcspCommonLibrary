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

    module:	bmc2_envco_management.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bmc2 Env Controller Object.

        *   Bmc2EnvcoGetComTerminal
        *   Bmc2EnvcoAddComTerminal
        *   Bmc2EnvcoDelComTerminal
        *   Bmc2EnvcoDelAllComTerminals
        *   Bmc2EnvcoGetCommandProperty
        *   Bmc2EnvcoAddCommandProperty
        *   Bmc2EnvcoDelCommandProperty
        *   Bmc2EnvcoDelAllCommandProperties

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#include "bmc2_envco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetComTerminal
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pUserIdentifier
            );

    description:

        This function is called to retrieve a com terminal by matching
        the user identifier.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pUserIdentifier
                Specifies the user identifier to be matched.

    return:     com terminal.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject        = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY           )NULL;

    AnscAcquireLock(&pMyObject->ComtoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ComtoQueue);

    while ( pSLinkEntry )
    {
        pBmc2ComTerminal = ACCESS_BMC2_COM_TERMINAL_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pBmc2ComTerminal->GetUserIdentifier((ANSC_HANDLE)pBmc2ComTerminal),pUserIdentifier) == 0 )
        {
            AnscReleaseLock(&pMyObject->ComtoQueueLock);

            return  (ANSC_HANDLE)pBmc2ComTerminal;
        }
    }

    AnscReleaseLock(&pMyObject->ComtoQueueLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoAddComTerminal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hComTerminal
            );

    description:

        This function is called to add a com terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hComTerminal
                Specifies the com terminal to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoAddComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject        = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )hComTerminal;

    AnscAcquireLock   (&pMyObject->ComtoQueueLock);
    AnscQueuePushEntry(&pMyObject->ComtoQueue, &pBmc2ComTerminal->Linkage);
    AnscReleaseLock   (&pMyObject->ComtoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoDelComTerminal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hComTerminal
            );

    description:

        This function is called to delete a com terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hComTerminal
                Specifies the com terminal to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoDelComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject        = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )hComTerminal;

    AnscAcquireLock        (&pMyObject->ComtoQueueLock);
    AnscQueuePopEntryByLink(&pMyObject->ComtoQueue, &pBmc2ComTerminal->Linkage);
    AnscReleaseLock        (&pMyObject->ComtoQueueLock);

    pBmc2ComTerminal->Cancel((ANSC_HANDLE)pBmc2ComTerminal);
    pBmc2ComTerminal->Remove((ANSC_HANDLE)pBmc2ComTerminal);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoDelAllComTerminals
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all com terminals.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoDelAllComTerminals
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject        = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY           )NULL;

    AnscAcquireLock(&pMyObject->ComtoQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->ComtoQueue);

    while ( pSLinkEntry )
    {
        pBmc2ComTerminal = ACCESS_BMC2_COM_TERMINAL_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueuePopEntry(&pMyObject->ComtoQueue);

        pBmc2ComTerminal->Cancel((ANSC_HANDLE)pBmc2ComTerminal);
        pBmc2ComTerminal->Remove((ANSC_HANDLE)pBmc2ComTerminal);
    }

    AnscReleaseLock(&pMyObject->ComtoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommandName,
                char*                       pDomainName
            );

    description:

        This function is called to retrieve a command property by
        matching the command name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommandName
                Specifies the command name to be matched.

                char*                       pDomainName
                Specifies the domain in which the command is 
                te be looked up.

    return:     command property.

**********************************************************************/

static
char*
Bmc2EnvcoFindSubStrNoCase
    (
        char*                       pString,
        char*                       pSub
    )
{
    int                             i, nStringLen, nSubLen;

    if ( !pString || !pSub )
    {
        return NULL;
    }

    nStringLen = AnscSizeOfString(pString);
    nSubLen    = AnscSizeOfString(pSub);

    for ( i = 0; i <= nStringLen - nSubLen; i ++ )
    {
        if ( AnscEqualString2(pString+i, pSub, nSubLen, FALSE) )
        {
            return pString+i;
        }
    }

    return  NULL;
}


ANSC_HANDLE
Bmc2EnvcoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainName
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex          = AnscHashString(pCommandName, AnscSizeOfString(pCommandName), BMC2_ENVCO_COMPO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->CompoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pBmc2CommandProperty->CommandName,pCommandName) == 0 )
        {
            BOOL                    bDomainMatched = FALSE;

            /* check if the specified domain matches pattern defined in command property */
            if ( !pBmc2CommandProperty->DomainNamePattern || 
                 strcmp(pBmc2CommandProperty->DomainNamePattern, "*") == 0 )
            {
                bDomainMatched = TRUE;
            }
            else
            {
                char*               pStar = _ansc_strstr(pBmc2CommandProperty->DomainNamePattern, "*");

                if ( !pStar )
                {
                    bDomainMatched = 
                        strcasecmp(pBmc2CommandProperty->DomainNamePattern,pDomainName) == 0;
                }
                else
                {
                    PANSC_TOKEN_CHAIN   pTokenChain  = (PANSC_TOKEN_CHAIN)NULL;
                    PANSC_STRING_TOKEN  pStringToken = (PANSC_STRING_TOKEN)NULL;
                    char*               pCurDomain   = pDomainName;
                    char*               pSubStr      = NULL;
                    
                    bDomainMatched = TRUE;

                    pTokenChain =
                        AnscTcAllocate3
                            (
                                pBmc2CommandProperty->DomainNamePattern,
                                "*"
                            );

                    if(pTokenChain) /*RDKB-5887 , CID-24415, null check before use*/
                    {

                        while (( pStringToken = AnscTcUnlinkToken(pTokenChain) ))
                        {
                            pSubStr = Bmc2EnvcoFindSubStrNoCase(pCurDomain, pStringToken->Name);

                            if ( !pSubStr )
                            {
                                bDomainMatched = FALSE;
                            }
                            else if ( pCurDomain == pDomainName && *pBmc2CommandProperty->DomainNamePattern != '*' )
                            {
                                if ( pSubStr != pCurDomain )
                                {
                                    bDomainMatched = FALSE;
                                }
                            }

                            if ( pSubStr )
                            {
                                pCurDomain = pSubStr + AnscSizeOfString(pStringToken->Name);    /* move to next part */
                            }

                            AnscFreeMemory(pStringToken);

                            if ( !bDomainMatched )
                            {
                                break;
                            }
                        }

                        AnscTcFree((ANSC_HANDLE)pTokenChain);

                    }

                }
            }

            if ( !bDomainMatched )
            {
                continue;
            }

            AnscReleaseLock(&pMyObject->CompoTableLock);

            return  (ANSC_HANDLE)pBmc2CommandProperty;
        }
    }

    AnscReleaseLock(&pMyObject->CompoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoAddCommandProperty
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

                ANSC_HANDLE                 hCommandInsertBefore
                If it's not NULL, it indicates the given command
                should be insert before this one.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoAddCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty,
        ANSC_HANDLE                 hCommandInsertBefore
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )hCommandProperty;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandPropRef  = (PBMC2_COMMAND_PROPERTY       )hCommandInsertBefore;
    ULONG                           ulHashIndex          = AnscHashString(pBmc2CommandProperty->CommandName, AnscSizeOfString(pBmc2CommandProperty->CommandName), BMC2_ENVCO_COMPO_TABLE_SIZE);

    AnscAcquireLock   (&pMyObject->CompoTableLock);
    if ( !pBmc2CommandPropRef )
    {
        AnscQueuePushEntry(&pMyObject->CompoTable[ulHashIndex], &pBmc2CommandProperty->Linkage);
    }
    else
    {
        /* This makes sure that the command property with more specific domain pattern name appears before those
         * with less specific name. So when the command is issued, framework always tries to match domain
         * with longer domain name, therefore there will be no mismatch.
         */
        AnscQueuePushEntryBefore(&pMyObject->CompoTable[ulHashIndex], &pBmc2CommandProperty->Linkage, &pBmc2CommandPropRef->Linkage);
    }
    AnscReleaseLock   (&pMyObject->CompoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoDelCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommandName,
                char*                       pDomainNamePattern
            );

    description:

        This function is called to delete a command property by
        matching the command name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommandName
                Specifies the command name to be matched.

                char*                       pDomainNamePattern
                Specifies the domain name pattern.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoDelCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainNamePattern
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex          = AnscHashString(pCommandName, AnscSizeOfString(pCommandName), BMC2_ENVCO_COMPO_TABLE_SIZE);
    PBMC2_COMMAND_PROPERTY          pBmc2CommandRef      = (PBMC2_COMMAND_PROPERTY       )NULL;

    AnscAcquireLock(&pMyObject->CompoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->CompoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
        pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pBmc2CommandProperty->CommandName,pCommandName) == 0 )
        {
            BOOL                    bDomainMatched = FALSE;

            /* check if the specified domain matches pattern defined in command property */
            if ( !pDomainNamePattern && !pBmc2CommandProperty->DomainNamePattern )
            {
                bDomainMatched = TRUE;
            }
            else if ( !pDomainNamePattern && pBmc2CommandProperty->DomainNamePattern )
            {
                if ( strcmp(pBmc2CommandProperty->DomainNamePattern, "*") == 0 )
                {
                    bDomainMatched = TRUE;
                }
            }
            else if ( !pBmc2CommandProperty->DomainNamePattern || 
                 strcmp(pBmc2CommandProperty->DomainNamePattern, "*") == 0 )
            {
                if ( !pDomainNamePattern || strcmp(pDomainNamePattern, "*") == 0 )
                {
                    bDomainMatched = TRUE;
                }
            }
            else
            {
                ULONG               ulDNLen      = AnscSizeOfString(pBmc2CommandProperty->DomainNamePattern);
                ULONG               ulCmdDNLen   = AnscSizeOfString(pDomainNamePattern);
                ULONG               ulCmpLen     = (ulDNLen > ulCmdDNLen) ? ulCmdDNLen : ulDNLen;

                if ( !pBmc2CommandRef && AnscEqualString2(pDomainNamePattern, pBmc2CommandProperty->DomainNamePattern, ulCmpLen, FALSE) )
                {
                    pBmc2CommandRef = pBmc2CommandProperty;
                }
                else if ( !pBmc2CommandRef && 
                          (!pBmc2CommandProperty->DomainNamePattern || 
                            strcmp(pBmc2CommandProperty->DomainNamePattern, "*") == 0 ) )
                {
                    pBmc2CommandRef = pBmc2CommandProperty;
                }
                else
                {
                    BOOL                bEndWithStar = (pBmc2CommandProperty->DomainNamePattern[ulDNLen - 1] == '*');

                    if ( bEndWithStar &&
                         AnscEqualString2(pDomainNamePattern, pBmc2CommandProperty->DomainNamePattern, ulDNLen - 1, FALSE) )
                    {
                        pBmc2CommandRef = pBmc2CommandProperty;
                    }
                }

                bDomainMatched = 
                    strcasecmp(pBmc2CommandProperty->DomainNamePattern,  pDomainNamePattern) == 0;
            }

            if ( !bDomainMatched )
            {
                continue;
            }

            AnscQueuePopEntryByLink(&pMyObject->CompoTable[ulHashIndex], &pBmc2CommandProperty->Linkage);

            AnscReleaseLock(&pMyObject->CompoTableLock);

            Bmc2FreeCommandProperty(pBmc2CommandProperty);

            return  (ANSC_HANDLE)NULL;
        }
    }

    AnscReleaseLock(&pMyObject->CompoTableLock);

    return  (ANSC_HANDLE)pBmc2CommandRef;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoDelAllCommandProperties
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
Bmc2EnvcoDelAllCommandProperties
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           i                    = 0;

    AnscAcquireLock(&pMyObject->CompoTableLock);

    for ( i = 0; i < BMC2_ENVCO_COMPO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->CompoTable[i]);

        while ( pSLinkEntry )
        {
            pBmc2CommandProperty = ACCESS_BMC2_COMMAND_PROPERTY(pSLinkEntry);
            pSLinkEntry          = AnscQueuePopEntry(&pMyObject->CompoTable[i]);

            Bmc2FreeCommandProperty(pBmc2CommandProperty);
        }
    }

    AnscReleaseLock(&pMyObject->CompoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


void
Bmc2CleanCommandProperty
    (
        PBMC2_COMMAND_PROPERTY      pCommandProperty
    )
{
    if ( pCommandProperty->DomainNamePattern )              
    {                                                   
        AnscFreeMemory(pCommandProperty->DomainNamePattern);
                                                        
        pCommandProperty->DomainNamePattern = NULL;         
    }                                                   
                                                        
    if ( pCommandProperty->CommandName )                    
    {                                                   
        AnscFreeMemory(pCommandProperty->CommandName);      
                                                        
        pCommandProperty->CommandName = NULL;               
    }                                                   
                                                        
    if ( pCommandProperty->ScpPageName )                    
    {                                                   
        AnscFreeMemory(pCommandProperty->ScpPageName);      
                                                        
        pCommandProperty->ScpPageName = NULL;               
    }                                                   
    if ( pCommandProperty->pHelpSyntax )                    
    {                                                   
        Bmc2FreeCmdHelpSyntax(pCommandProperty->pHelpSyntax);
    }                                                   
}


void
Bmc2FreeCommandProperty
    (
        PBMC2_COMMAND_PROPERTY      pCommandProperty
    )
{
    Bmc2CleanCommandProperty(pCommandProperty);
    AnscFreeMemory          (pCommandProperty);
}

void 
Bmc2CleanCmdArgument
    (
        PBMC2_CMD_ARGUMENT          pCmdArg
    )
{
    if ( (pCmdArg)->Type == BMC2_CMD_ARG_TYPE_nameless ||        
         (pCmdArg)->Type == BMC2_CMD_ARG_TYPE_named )            
    {                                                            
        Bmc2CleanCmdSimpleArg(&((pCmdArg)->Arg.Simple));         
    }                                                            
    else if ( (pCmdArg)->Type == BMC2_CMD_ARG_TYPE_dependent )   
    {                                                            
        Bmc2CleanCmdDepArg(&((pCmdArg)->Arg.Dependent));         
    }                                                            
}


void
Bmc2FreeCmdArgument
    (
        PBMC2_CMD_ARGUMENT          pCmdArg
    )
{
    Bmc2CleanCmdArgument(pCmdArg);  
    AnscFreeMemory(pCmdArg);        
}


void
Bmc2CleanCmdDepArg
    (
        PBMC2_CMD_DEP_ARG           pCmdDepArg
    )
{
    ULONG                       i;                                     
                                                                       
    for (i = 0; i < (pCmdDepArg)->ulArgBranchCount; i ++)              
    {                                                                  
        Bmc2CleanCmdDepArgBranch(&((pCmdDepArg)->pArgBranches[i]));    
    }                                                                  
    if ( (pCmdDepArg)->pArgBranches )                                  
    {                                                                  
        AnscFreeMemory((pCmdDepArg)->pArgBranches);                    
    }                                                                  
}


void
Bmc2CleanCmdDepArgBranch
    (
        PBMC2_CMD_DEP_ARG_BRANCH    pCmdDepArgBranch
    )
{
    if ( (pCmdDepArgBranch)->pArgValue )                       
    {                                                          
        AnscFreeMemory((pCmdDepArgBranch)->pArgValue);         
    }                                                          
    Bmc2CleanCmdSimpleArg(&((pCmdDepArgBranch)->SimpleArg));   
}


void
Bmc2CleanCmdSimpleArg
    (
        PBMC2_CMD_SIMPLE_ARG        pCmdArg
    )
{
    ULONG                       i;                                     
                                                                       
    for (i = 0; i < (pCmdArg)->HelpItemCount; i ++)                    
    {                                                                  
        Bmc2CleanCmdArgHelp(&((pCmdArg)->pHelpItems[i]));              
    }                                                                  
    if ( (pCmdArg)->pHelpItems )                                       
    {                                                                  
        AnscFreeMemory((pCmdArg)->pHelpItems);                         
    }                                                                  
    for (i = 0; i < (pCmdArg)->ValueCount; i ++)                       
    {                                                                  
        if ( (pCmdArg)->ValueType == BMC2_CMD_ARG_VTYPE_string &&      
             (pCmdArg)->pValue[i].Val.varString )                      
        {                                                              
            AnscFreeMemory((pCmdArg)->pValue[i].Val.varString);        
        }                                                              
    }                                                                  
    if ( (pCmdArg)->pValue )                                           
    {                                                                  
        AnscFreeMemory((pCmdArg)->pValue);                             
    }                                                                  
    if ( (pCmdArg)->DisplayName )                                      
    {                                                                  
        AnscFreeMemory((pCmdArg)->DisplayName);                        
    }                          
    if ( (pCmdArg)->DisplayNameHelp ) 
    {
        AnscFreeMemory((pCmdArg)->DisplayNameHelp);
    }
    if ( (pCmdArg)->pValueRange )                                      
    {                                                                  
        Bmc2FreeCmdArgValueRange((pCmdArg)->pValueRange);             
    }                                                                  
}


void
Bmc2CleanCmdArgValueRange
    (
        PBMC2_CMD_ARG_VRANGE        pCmdArgVrange
    )
{
    if(pCmdArgVrange) /*RDKB-5887 , CID-24393, null check before use*/
    {
        if ( (pCmdArgVrange)->pStringArray )
        {
            ULONG i;

            for (i = 0; i < (pCmdArgVrange)->ulStringCount; i ++)
            {
                AnscFreeMemory((pCmdArgVrange)->pStringArray[i]);
            }
            if ( (pCmdArgVrange)->pStringArray )
            {
                AnscFreeMemory((pCmdArgVrange)->pStringArray);
            }
        }

        AnscFreeMemory(pCmdArgVrange);
    }
}


void
Bmc2FreeCmdArgValueRange
    (
        PBMC2_CMD_ARG_VRANGE        pCmdArgVrange
    )
{
    if ( pCmdArgVrange )                                     
    {                                                        
        Bmc2CleanCmdArgValueRange(pCmdArgVrange);           
        /*CID: 68730 Double free*/ 
    }
}


void
Bmc2CleanCmdArgHelp
    (
        PBMC2_CMD_ARG_HELP          pCmdArgHelp
    )
{
    if ( (pCmdArgHelp)->pItem )                             
    {                                                       
        AnscFreeMemory((pCmdArgHelp)->pItem);               
    }                                                       
    if ( (pCmdArgHelp)->pDescription )                      
    {                                                       
        AnscFreeMemory((pCmdArgHelp)->pDescription);        
    }                                                       
}


void
Bmc2FreeCmdHelpSyntax
    (
        PBMC2_CMD_HELP_SYNTAX       pCmdHelpSyntax
    )
{
    ULONG                       i;                             
                                                               
    for (i = 0; i < (pCmdHelpSyntax)->ulReqArgCount; i ++)     
    {                                                          
        Bmc2CleanCmdArgument(&((pCmdHelpSyntax)->pReqArgs[i]));
    }                                                          
                                                               
    for (i = 0; i < (pCmdHelpSyntax)->ulOptArgCount; i ++)     
    {                                                          
        Bmc2CleanCmdArgument(&((pCmdHelpSyntax)->pOptArgs[i]));
    }                                                          
                                                               
    if ( (pCmdHelpSyntax)->pReqArgs )                          
    {                                                          
        AnscFreeMemory((pCmdHelpSyntax)->pReqArgs);            
    }                                                          
    if ( (pCmdHelpSyntax)->pOptArgs )                          
    {                                                          
        AnscFreeMemory((pCmdHelpSyntax)->pOptArgs);            
    }      
    
    AnscFreeMemory(pCmdHelpSyntax);
}

