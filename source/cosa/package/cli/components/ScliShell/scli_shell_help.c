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

    module:	scli_shell_bic.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

        *   ScliShoTokenizeCommand
        *   ScliShoFreeCmdTokenList
        *   ScliShoIndicateError
        *   ScliShoIndicateHelp
        *   ScliShoShowArgHelp
        *   ScliShoShowOptArgHelp
        *   ScliShoMatchSimpleArg
        *   ScliShoMatchDepArg
        *   ScliShoMatchOptArgs
        *   ScliShoMatchArg
        *   ScliShoMatchCommand
        *   ScliShoGetLastMatchedOptArg
        *   ScliShoIsArgValueValid
        *   ScliShoInitArgMatchResult
        *   ScliShoAutoCompleteCommandArg
        *   ScliShoAutoCompleteOptArg
        *   ScliShoAutoCompleteArg
             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/08/10    initial revision.

**********************************************************************/


#include "scli_shell_global.h"
#include "safec_lib_common.h"

#define MAX_PVALUETYPE_SIZE    64

char*
ScliShoFindSubStrNoCase
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


BOOL
ScliShoIsStringInList
	(
		char*						pCSStringList,      /* comma separated string */
		char*						pString,            /* string to be checked */
		BOOL						bLastExcluded,      /* if last string in list excluded for comparison */
        BOOL                        bCaseSensitive
	)
{
	char*                           pSub;
    int                             nStrListLen = AnscSizeOfString(pCSStringList);
    int                             nStringLen  = AnscSizeOfString(pString);
    char*                           pCSLEnd     = pCSStringList + nStrListLen - 1;
    char*                           pStart      = pCSStringList;

    if ( bLastExcluded )
    {
        while ( pCSLEnd > pCSStringList)
        {
            if ( *pCSLEnd == ',' )
            {
                break;
            }
            pCSLEnd --;
        }
    }
    
    while ( TRUE )
    {
        if ( bCaseSensitive )
        {
            pSub = _ansc_strstr(pStart, pString);
        }
        else
        {
            pSub = ScliShoFindSubStrNoCase(pStart, pString);
        }

        if ( !pSub )
        {
            return  FALSE;
        }
        else
        {
            if ( pSub > pCSLEnd )
            {
                return  FALSE;
            }
            else
            {
                int                     nPos = pSub - pStart;

                if ( nPos + nStringLen < nStrListLen && *(pStart + nPos + nStringLen) != ',' )
                {
                    pStart = pSub + 1;
                    continue;
                }
                else if ( nPos != 0 && *(pStart + nPos - 1) != ',' )
                {
                    pStart = pSub + 1;
                    continue;
                }
            }

            return  TRUE;
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        ScliShoInitArgMatchResult
            (
                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
            )

    description:

        This function is called to initialize argument match
        result.

    argument:   PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                Specifies the match result to be initialized.

    return:     none.

**********************************************************************/

void
ScliShoInitArgMatchResult
    (
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
    )
{
    AnscZeroMemory(pArgMatchResult, sizeof(SCLI_CMD_ARG_MATCH_RESULT));

    pArgMatchResult->nErrorPos         = -1;
    pArgMatchResult->nMatchStart       = -1;
    pArgMatchResult->nMatchEnd         = -1;
    pArgMatchResult->nMatchTokenStart  = -1;
    pArgMatchResult->nMatchTokenEnd    = -1;

    pArgMatchResult->bTried            = FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PSCLI_SHELL_CMD_TLIST
        ScliShoTokenizeCommand
            (
                char*                       pCmd
                char                        pSeparator
            );

    description:

        This function is called to tokenize command user
        inputs.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                char                        pSeparator
                Specifies the separator

    return:     internal presentation of command.

**********************************************************************/

PSCLI_SHELL_CMD_TLIST
ScliShoTokenizeCommand
    (
        char*                       pCmd,
        char                        pSeparator
    )
{
    PSCLI_SHELL_CMD_TLIST           pTokenList      = NULL;
    char*                           pNext;
    ULONG                           ulTokenCount    = 0;
    char*                           pCmdStart       = pCmd;
    BOOL                            bEndWithSpace   = FALSE;

    if ( *(pCmdStart + AnscSizeOfString(pCmdStart) - 1) == SCLI_SHELL_CMD_TOKEN_SEPARATOR )
    {
        bEndWithSpace = TRUE;
    }

    /* count the token */
    while ( *pCmdStart != 0 )
    {
        SCLI_SHELL_SKIP_SPACE(pCmdStart);
        if ( *pCmdStart == 0 )
        {
            break;
        }

        ulTokenCount ++;

        if ( pSeparator == 0 )
        {
            break;
        }

        if ( *pCmdStart == BMC2_CMD_ARG_VALUE_QSTRING_SEP )
        {
            pNext = _ansc_strchr(pCmdStart+1, BMC2_CMD_ARG_VALUE_QSTRING_SEP);
        }
        else
        {
            pNext = _ansc_strchr(pCmdStart, pSeparator);
        }

        if ( !pNext )
        {
            break;
        }
        else
        {
            pCmdStart = pNext + 1;
        }
    }

    /* build token list */

    pTokenList = (PSCLI_SHELL_CMD_TLIST)AnscAllocateMemory(sizeof(SCLI_SHELL_CMD_TLIST));

    if ( !pTokenList )
    {
        return  NULL;
    }

    pTokenList->bEndWithSpace       = bEndWithSpace;
    pTokenList->ulCurToken          = 0;
    pTokenList->ulTokenCount        = ulTokenCount;
    pTokenList->pTokens             = NULL;

    if ( 0 != ulTokenCount )
    {
        PSCLI_SHELL_CMD_TOKEN       pToken;
        ULONG                       ulTokenLen;
        BOOL                        bQuotedToken;

        pTokenList->pTokens = (PSCLI_SHELL_CMD_TOKEN)AnscAllocateMemory(sizeof(SCLI_SHELL_CMD_TOKEN) * ulTokenCount);

        if ( !pTokenList->pTokens )
        {
            goto EXIT2;
        }
    
        ulTokenCount = 0;

        pCmdStart = pCmd;

        while ( *pCmdStart != 0 )
        {
            SCLI_SHELL_SKIP_SPACE(pCmdStart);
            if ( *pCmdStart == 0 )
            {
                break;
            }

            pToken  = &pTokenList->pTokens[ulTokenCount];

            ulTokenCount ++;
            if ( *pCmdStart == BMC2_CMD_ARG_VALUE_QSTRING_SEP )
            {
                pNext        = _ansc_strchr(pCmdStart+1, BMC2_CMD_ARG_VALUE_QSTRING_SEP);
                bQuotedToken = TRUE;
            }
            else
            {
                pNext        = _ansc_strchr(pCmdStart, pSeparator);
                bQuotedToken = FALSE;
            }

            if ( bQuotedToken )
            {
                if ( pNext ) pNext ++;
            }

            ulTokenLen = pNext ? (ULONG)(pNext - pCmdStart) : (ULONG)AnscSizeOfString(pCmdStart);
            pToken->pValue  = (char*)AnscAllocateMemory(ulTokenLen + 1);
            if ( !pToken->pValue )
            {
                goto EXIT2;
            }

            if ( 0 != ulTokenLen )
            {
                AnscCopyMemory(pToken->pValue, pCmdStart, ulTokenLen);
            }
            pToken->pValue[ulTokenLen] = 0;

            pToken->ulStartPos = pCmdStart - pCmd;

            if ( pSeparator == 0 )
            {
                break;
            }

            if ( !pNext || *pNext == 0 )
            {
                break;
            }
            else
            {
                pCmdStart = pNext + 1;
            }
        }
    }

    goto EXIT1;

EXIT2:

    if ( pTokenList )
    {
        ScliShoFreeCmdTokenList(pTokenList);

        pTokenList = NULL;
    }

EXIT1:

    return  pTokenList;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        ScliShoFreeCmdTokenList
            (
                PSCLI_SHELL_CMD_TLIST       pTokenList
            );

    description:

        This function is called to free resource of command token 
        list.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

    return:     none

**********************************************************************/

void
ScliShoFreeCmdTokenList
    (
        PSCLI_SHELL_CMD_TLIST       pTokenList
    )
{
    PSCLI_SHELL_CMD_TOKEN           pCmdToken;
    ULONG                           i;

    if ( !pTokenList )
    {
        return;
    }

    for ( i = 0; i < pTokenList->ulTokenCount; i ++ )
    {
        pCmdToken = &pTokenList->pTokens[i];
        
        if ( pCmdToken->pValue )
        {
            AnscFreeMemory(pCmdToken->pValue);
        }
    }

    if ( pTokenList->pTokens )
    {
        AnscFreeMemory(pTokenList->pTokens);
    }

    AnscFreeMemory(pTokenList);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIndicateError
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pErrorCode,
                char*                       pCmd,
                int                         nErrorPos
            );

    description:

        This function is called to output error and indicate
        where error detected when possible.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                ANSC_HANDLE                 hSrvSession
                Specifies which session the error indicated to.

                char*                       pErrorCode
                Specifies the error code.

                char*                       pCmd
                Specifies the command user just input.

                int                         nErrorPos
                Specifies where error is detected in user's
                input.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIndicateError
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pErrorCode,
        char*                       pCmd,
        int                         nErrorPos
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    int                             i;


    if ( pErrorCode )
    {
        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pErrorCode,
                    AnscSizeOfString(pErrorCode)
                );

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );
    }

    returnStatus =
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                SCLI_SHELL_OUTPUT_INDENT,
                AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
            );

    returnStatus =
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                pCmd,
                AnscSizeOfString(pCmd)
            );

    returnStatus = 
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                TELNET_ANSI_TTY_CODE_Crlf,
                AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
            );

    if ( nErrorPos >= 0 )
    {
        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    SCLI_SHELL_OUTPUT_INDENT,
                    AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                );

        for ( i = 0; i < nErrorPos; i ++ )
        {
            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        " ",
                        1
                    );
        }

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    SCLI_SHELL_OUTPUT_ERROR,
                    AnscSizeOfString(SCLI_SHELL_OUTPUT_ERROR)
                );

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoShowArgHelp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pPrefix,
                PBMC2_CMD_ARGUMENT          pCmdArg,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                ULONG                       ulOptArgCount,
                BOOL                        bReqArg,
                ULONG                       ulCurArg,
                int*                        pErrorPos
            )

    description:

        This function is called to output help text on
        the specified argument.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                ANSC_HANDLE                 hSrvSession
                Specifies which session the error indicated to.

                char*                       pPrefix
                Prefix that partially matches argument
                name or value.

                PBMC2_CMD_ARGUMENT          pCmdArg
                Specifies the command argument.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies how required arguments matched

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies optional argument match result.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                BOOL                        bReqArg
                Specifies if required argument is queried.

                ULONG                       ulCurArg
                Specifies the current argument user is typing.    

                int*                        pErrorPos
                On failure return, it indicates where error
                is detected.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoShowArgHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pCmdArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        BOOL                        bReqArg,
        ULONG                       ulCurArg,
        int*                        pErrorPos
    )
{
    UNREFERENCED_PARAMETER(ulOptArgCount);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBMC2_CMD_SIMPLE_ARG            pSimpleArg   = NULL;
    ULONG                           ulSimArgType = pCmdArg->Type;
    BOOL                            bSkipNamePart;
    PSCLI_SHELL_ARG_HELP_LIST       pHelpList;
    int                             nErrorPos    = 0;

    *pErrorPos = 0;

    if ( ulSimArgType == BMC2_CMD_ARG_TYPE_dependent )
    {
        int                         nArgIndex;
        PBMC2_CMD_DEP_ARG           pDepArg = &pCmdArg->Arg.Dependent;

        nArgIndex = ScliShoDepArgFindMatchedBranch(pDepArg, pCmdTokenList, pReqArgMatched, ulReqArgCount);
        if ( nArgIndex < 0 )
        {
            pSimpleArg = NULL;
        }
        else
        {
            pSimpleArg      = &pDepArg->pArgBranches[nArgIndex].SimpleArg;
            ulSimArgType    = pDepArg->pArgBranches[nArgIndex].Type;
        }
    }
    else if ( ulSimArgType == BMC2_CMD_ARG_TYPE_nameless || 
              ulSimArgType == BMC2_CMD_ARG_TYPE_named )
    {
        pSimpleArg   = &pCmdArg->Arg.Simple;
    }

    if ( !pSimpleArg )
    {
        return  ANSC_STATUS_CONTINUE;
    }

    if ( bReqArg )
    {
        bSkipNamePart = ( pReqArgMatched[ulCurArg].bPartialValue                                 || 
                          pReqArgMatched[ulCurArg].bValueMatched                                 || 
                         (pReqArgMatched[ulCurArg].bNameMatched && pCmdTokenList->bEndWithSpace) || 
                          ulSimArgType == BMC2_CMD_ARG_TYPE_nameless );
    }
    else
    {
        bSkipNamePart = ( pOptArgMatched[ulCurArg].bPartialValue                                 || 
                          pOptArgMatched[ulCurArg].bValueMatched                                 || 
                         (pOptArgMatched[ulCurArg].bNameMatched && pCmdTokenList->bEndWithSpace) || 
                          ulSimArgType == BMC2_CMD_ARG_TYPE_nameless );
    }

    pHelpList = (PSCLI_SHELL_ARG_HELP_LIST)AnscAllocateMemory(sizeof(SCLI_SHELL_ARG_HELP_LIST));

    if ( !pHelpList )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;
        goto EXIT;
    }

    if ( !bSkipNamePart )
    {
        pHelpList->ulArgHelpCount = 1;
        pHelpList->pArgHelps      = (PBMC2_CMD_ARG_HELP)AnscAllocateMemory(sizeof(BMC2_CMD_ARG_HELP) * 1);
        if ( !pHelpList->pArgHelps )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }

        if ( pPrefix && ScliShoFindSubStrNoCase(pSimpleArg->DisplayName, pPrefix) != pSimpleArg->DisplayName )
        {
            ULONG                   i;

            pHelpList->ulArgHelpCount = 0;

            for ( i = 0; i < AnscSizeOfString(pPrefix); i ++ )
            {
                if ( pPrefix[i] != pSimpleArg->DisplayName[i] )
                {
                    nErrorPos = (int)(i + pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].ulStartPos);

                    break;
                }
            }
        }

        /* retrieve help of the specified simple argument */
        pHelpList->pArgHelps[0].pItem           = AnscCloneString(pSimpleArg->DisplayName);
        pHelpList->pArgHelps[0].pDescription    = AnscCloneString(pSimpleArg->DisplayNameHelp);
        pHelpList->ulMaxItemLen                 = pSimpleArg->DisplayName ? AnscSizeOfString(pSimpleArg->DisplayName) : 0;
    }
    else
    {
        pHelpList->ulArgHelpCount = pSimpleArg->HelpItemCount;
        if ( pHelpList->ulArgHelpCount == 0 )
        {
            PBMC2_CMD_ARG_VRANGE    pVrange = pSimpleArg->pValueRange;

            if ( pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_string && pVrange )
            {
                pHelpList->ulArgHelpCount = pVrange->Min.varUint;
            }
            else if ( pVrange )
            {
                pHelpList->ulArgHelpCount = 1 + pVrange->ulStringCount;
            }
            else
            {
                pHelpList->ulArgHelpCount = 1;
            }
        }
        pHelpList->pArgHelps      = (PBMC2_CMD_ARG_HELP)AnscAllocateMemory(sizeof(BMC2_CMD_ARG_HELP) * (pHelpList->ulArgHelpCount+1));
        if ( !pHelpList->pArgHelps )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }

        pHelpList->ulMaxItemLen = 0;

        /* retrieve help of the specified simple argument */
        returnStatus = 
            ScliShoGetSimpleArgHelp
                (
                    ulSimArgType,
                    pSimpleArg,
                    pPrefix,
                    pHelpList,
                    0,
                    &pHelpList->ulArgHelpCount,
                    &nErrorPos
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS || pHelpList->ulArgHelpCount == 0 )
        {
            nErrorPos += (int)pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].ulStartPos;
        }
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS && pHelpList->ulArgHelpCount != 0 )
    {
        /* display help */
        returnStatus = ScliShoShowArgHelpList(hThisObject, hSrvSession, pHelpList);
    }
    else
    {
        *pErrorPos   = nErrorPos;
        returnStatus = ANSC_STATUS_UNMATCHED;
    }

EXIT:
    if ( pHelpList )
    {
        /*CID: 71754 Dereference after null check*/
	/* Dereferencing null pointer pHelpList->pArgHelps - added NULL check in the macro*/
        SCLI_SHELL_FREE_ARG_HELP_LIST(pHelpList);
        pHelpList = NULL;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        static
        PSCLI_SHELL_ARG_HELP_LIST
        ScliShoGetOptArgOptions
            (
                char*                       pPrefix,
                PBMC2_CMD_ARGUMENT          pOptArgs,
                ULONG                       ulOptArgCount,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                int*                        pErrorPos
            )

    description:

        This function is called to collect help list of 
        all possible arguments.

    argument:   char*                       pPrefix
                Prefix of Argument name.

                PBMC2_CMD_ARGUMENT          pOptArgs,
                Specifies the optional command argument.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies how required arguments matched

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies how optional arguments matched.

                int*                        pErrorPos
                Indicates where error is detected.

    return:     argument help list.

**********************************************************************/

static
PSCLI_SHELL_ARG_HELP_LIST
ScliShoGetOptArgOptions
    (
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int*                        pErrorPos
    )
{
    PSCLI_SHELL_ARG_HELP_LIST       pHelpList           = NULL;
    ULONG                           ulArgHelpCount      = 0;
    int                             nErrorPos           = 0;
    BOOL                            bNoMoreOptions      = TRUE;
    int                             nLastMatched        = 0;
    char*                           pLastMatchedArgName = NULL;
    int                             nDepArgIndex        = -1;
    BOOL                            bDepArgOptional     = FALSE;
    ULONG                           ulArgStart          = 0;
    ULONG                           ulArgEnd            = ulOptArgCount - 1;
    ULONG                           ulArgCountTBC       = ulOptArgCount;
    ULONG                           i;
    PBMC2_CMD_SIMPLE_ARG            pSimpleArg;
    PSCLI_SHELL_ARG_HELP_MATCH      pHelpMatch;
    ULONG                           ulArgType;

    nLastMatched = 
        ScliShoGetLastMatchedOptArg
            (
                pReqArgMatched, 
                ulReqArgCount, 
                pOptArgMatched,
                ulOptArgCount
            );

    if ( nLastMatched >= 0 )
    {
        int                         nTokenStart = pOptArgMatched[nLastMatched].nMatchTokenStart;
        int                         nTokenEnd   = pOptArgMatched[nLastMatched].nMatchTokenEnd;

        /* first check if last matched argument is named argument */
        if ( nTokenStart != nTokenEnd && nTokenStart >= 0 )
        {
            pLastMatchedArgName = pCmdTokenList->pTokens[nTokenStart].pValue;
        }

        if ( pLastMatchedArgName )
        {
            /* search unmatched optional arguments to see if there's one that 
             * depends on the last argument matched by its name 
             */
            nDepArgIndex = 
                ScliShoFindDepArgOnName
                    (
                        pOptArgs, 
                        pOptArgMatched, 
                        ulOptArgCount, 
                        pLastMatchedArgName, 
                        &bDepArgOptional
                    );

            if ( nDepArgIndex >= 0 && !bDepArgOptional )
            {
                ulArgStart  = (ULONG)nDepArgIndex;
                ulArgEnd    = (ULONG)nDepArgIndex;
            }
        }
    }

    ulArgCountTBC = (ulArgEnd - ulArgStart + 1);

    pHelpMatch = (PSCLI_SHELL_ARG_HELP_MATCH)AnscAllocateMemory(sizeof(SCLI_SHELL_ARG_HELP_MATCH));
    if ( !pHelpMatch )
    {
        return NULL;
    }

    pHelpMatch->ulMatchCount = ulArgCountTBC;

    pHelpMatch->pArgTypes   = (PULONG               )AnscAllocateMemory(sizeof(ULONG)                * ulArgCountTBC);
    pHelpMatch->pMatches    = (PBOOL                )AnscAllocateMemory(sizeof(BOOL)                 * ulArgCountTBC);
    pHelpMatch->pSimpleArgs = (PBMC2_CMD_SIMPLE_ARG*)AnscAllocateMemory(sizeof(PBMC2_CMD_SIMPLE_ARG) * ulArgCountTBC);
    pHelpMatch->pHelpCounts = (PULONG               )AnscAllocateMemory(sizeof(ULONG)                * ulArgCountTBC);

    if ( !pHelpMatch->pArgTypes || !pHelpMatch->pMatches || 
         ! pHelpMatch->pSimpleArgs || !pHelpMatch->pHelpCounts )
    {
        SCLI_SHELL_FREE_ARG_HELP_MATCH(pHelpMatch);
        return  NULL;
    }

    /* scan all available options */
    for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
    {
        pHelpMatch->pMatches[i-ulArgStart] = FALSE;

        if ( pOptArgMatched[i].bValueMatched )
        {
            continue;
        }

        if ( pOptArgs[i].Type == BMC2_CMD_ARG_TYPE_dependent )
        {
            int                     nArgIndex;
            
            nArgIndex = 
                ScliShoDepArgFindMatchedBranch
                    (
                        &pOptArgs[i].Arg.Dependent,
                        pCmdTokenList,
                        pReqArgMatched,
                        ulReqArgCount
                     );

            if ( nArgIndex < 0 )
            {
                continue;
            }

            if ( pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].ulArgDepOn != BMC2_CMD_DEP_ARG_DEP_required )
            {
                char*               pArgName = pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].pArgValue;
                int                 nMasterArgIndex;

                nMasterArgIndex = 
                    ScliShoGetOptNamedArgMatched
                        (
                            pCmdTokenList, 
                            pOptArgs, 
                            ulOptArgCount, 
                            pOptArgMatched, 
                            pArgName
                        );

                if ( nMasterArgIndex < 0 )
                {
                    continue;
                }
            }

            pSimpleArg = &pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].SimpleArg;
            ulArgType  = pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].Type;
        }
        else
        {
            pSimpleArg = &pOptArgs[i].Arg.Simple;
            ulArgType  = pOptArgs[i].Type;
        }

        bNoMoreOptions = FALSE;

        if ( ulArgType == BMC2_CMD_ARG_TYPE_nameless )
        {
            /* we should include help on nameless argument that has string value with prefix */
            PBMC2_CMD_ARG_VRANGE    pValueRange = pSimpleArg->pValueRange;
            ULONG                   j;
            ULONG                   ulItemMatched   = 0;
            int                     nLastError      = 0;
            char*                   pValueOption;
            int                     k;

            if ( !pValueRange || pSimpleArg->ValueType != BMC2_CMD_ARG_VTYPE_string )
            {
                ulItemMatched = 1;
            }
            else
            {
                for ( j = 0; j < pValueRange->ulStringCount; j ++ )
                {
                    pValueOption = pValueRange->pStringArray[j];

                    if ( !pPrefix || (pValueOption && ScliShoFindSubStrNoCase(pValueOption, pPrefix) == pValueOption) )
                    {
                        ulItemMatched ++;
                    }
                    else if ( pPrefix && pValueOption )
                    {
                        SCLI_SHELL_STRING_DIFF_AT(pValueOption, pPrefix, k);

                        if ( k > nLastError )
                        {
                            nLastError = k;
                        }
                    }
                }
            }

            pHelpMatch->pMatches[i-ulArgStart]         = (ulItemMatched != 0);
            pHelpMatch->pHelpCounts[i-ulArgStart]      = ulItemMatched;
            pHelpMatch->pArgTypes[i-ulArgStart]        = BMC2_CMD_ARG_TYPE_nameless;
            pHelpMatch->pSimpleArgs[i-ulArgStart]      = pSimpleArg;
            ulArgHelpCount += ulItemMatched;

            if ( ulItemMatched == 0 && nErrorPos < nLastError )
            {
                nErrorPos = nLastError;
            }

            continue;
        }
        else
        {
            if ( !pPrefix || ScliShoFindSubStrNoCase(pSimpleArg->DisplayName, pPrefix) == pSimpleArg->DisplayName )
            {
                pHelpMatch->pMatches[i-ulArgStart]         = TRUE;
                pHelpMatch->pHelpCounts[i-ulArgStart]      = 1;
                pHelpMatch->pArgTypes[i-ulArgStart]        = BMC2_CMD_ARG_TYPE_named;
                pHelpMatch->pSimpleArgs[i-ulArgStart]      = pSimpleArg;
                ulArgHelpCount += 1;
            }
            else 
            {
                pHelpMatch->pMatches[i-ulArgStart]         = FALSE;
                pHelpMatch->pHelpCounts[i-ulArgStart]      = 1;
                pHelpMatch->pArgTypes[i-ulArgStart]        = BMC2_CMD_ARG_TYPE_named;
                pHelpMatch->pSimpleArgs[i-ulArgStart]      = pSimpleArg;
            }

            continue;
        }
    }

    /* build help on all available options */
    if ( 0 != ulArgHelpCount || bNoMoreOptions )
    {
        ULONG                       ulHelpListCount     = 0;
        ANSC_STATUS                 returnStatus = ANSC_STATUS_FAILURE; /*RDKB-5890 ; CID-24626, initializing variable*/
        ULONG                       ulMatchedHelpCount  = 0;
        BOOL                        bHasOptionCR        = 0;

        if ( !pPrefix || bNoMoreOptions )
        {
            ulArgHelpCount ++;      /* <cr> */
            bHasOptionCR = TRUE;
        }

        pHelpList = (PSCLI_SHELL_ARG_HELP_LIST)AnscAllocateMemory(sizeof(SCLI_SHELL_ARG_HELP_LIST));

        if ( !pHelpList )
        {
            goto EXIT1;
        }

        pHelpList->ulArgHelpCount = ulArgHelpCount;

        pHelpList->pArgHelps = 
            (PBMC2_CMD_ARG_HELP)AnscAllocateMemory
                (
                    sizeof(BMC2_CMD_ARG_HELP) * (ulArgHelpCount+1)
                );

        if ( !pHelpList->pArgHelps )
        {
            goto EXIT2;
        }

        pHelpList->ulMaxItemLen = 0;

        for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
        {
            int                     nErrorOffset = 0;

            if ( !pHelpMatch->pMatches[i-ulArgStart] )
            {
                continue;
            }

            pSimpleArg = pHelpMatch->pSimpleArgs[i-ulArgStart];

            if ( pHelpMatch->pArgTypes[i-ulArgStart] == BMC2_CMD_ARG_TYPE_named )
            {
                pHelpList->pArgHelps[ulHelpListCount].pItem         = AnscCloneString(pSimpleArg->DisplayName);
                pHelpList->pArgHelps[ulHelpListCount].pDescription  = AnscCloneString(pSimpleArg->DisplayNameHelp);

                if ( !pHelpList->pArgHelps[ulHelpListCount].pItem )
                {
                    returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                }
                else
                {
                    if ( AnscSizeOfString(pHelpList->pArgHelps[ulHelpListCount].pItem) > pHelpList->ulMaxItemLen )
                    {
                        pHelpList->ulMaxItemLen = AnscSizeOfString(pHelpList->pArgHelps[ulHelpListCount].pItem);
                    }
                }

                ulMatchedHelpCount = 1;
            }
            else
            {
                returnStatus = 
                    ScliShoGetSimpleArgHelp
                        (
                            pHelpMatch->pArgTypes[i-ulArgStart],
                            pSimpleArg,
                            pPrefix,
                            pHelpList,
                            ulHelpListCount,
                            &ulMatchedHelpCount,
                            &nErrorOffset
                        );
            }

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                if ( nErrorOffset > nErrorPos )
                {
                    nErrorPos = nErrorOffset;
                }
            }

            ulHelpListCount += ulMatchedHelpCount;
        }

        if ( bHasOptionCR )
        {
            pHelpList->pArgHelps[ulHelpListCount].pItem         = AnscCloneString(SCLI_SHELL_OPTION_CR);
            pHelpList->pArgHelps[ulHelpListCount].pDescription  = AnscCloneString(SCLI_SHELL_OPTION_CR_DESC);

            if ( AnscSizeOfString(pHelpList->pArgHelps[ulHelpListCount].pItem) > pHelpList->ulMaxItemLen )
            {
                pHelpList->ulMaxItemLen = AnscSizeOfString(pHelpList->pArgHelps[ulHelpListCount].pItem);
            }
        }
    }

    goto EXIT1;

EXIT2:
    if ( pHelpList )
    {
	/*CID: 62403 Dereference after null check*/
	/* Dereferencing null pointer pHelpList->pArgHelps - added NULL check in the macro*/
        SCLI_SHELL_FREE_ARG_HELP_LIST(pHelpList);
        pHelpList = NULL;
    }

EXIT1:

    SCLI_SHELL_FREE_ARG_HELP_MATCH(pHelpMatch);
    
    if ( !pHelpList || pHelpList->ulArgHelpCount == 0 )
    {
        *pErrorPos = nErrorPos;
    }

    return  pHelpList;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoShowOptArgHelp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                PBMC2_CMD_ARGUMENT          pOptArgs,
                ULONG                       ulOptArgCount,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                int                         nCurArg,
                int*                        pErrorPos
            )

    description:

        This function is called to output help text on
        the specified argument.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                ANSC_HANDLE                 hSrvSession
                Specifies which session the error indicated to.

                PBMC2_CMD_ARGUMENT          pOptArgs,
                Specifies the optional command argument.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies how required arguments matched

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies how optional arguments matched.

                int                         nCurArg
                Specifies the current argument user is typing.    

                int*                        pErrorPos
                On failure return, it indicates where error
                is detected.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoShowOptArgHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int                         nCurArg,
        int*                        pErrorPos
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    BOOL                            bLookupAllOptions   = FALSE;

    *pErrorPos = 0;

    if ( nCurArg < 0 || (ULONG)nCurArg >= ulOptArgCount  )
    {
        bLookupAllOptions = TRUE;
    }
    else
    {
        BOOL                        bPartialName    = pOptArgMatched[nCurArg].bNamePartialMatched;

        if ( !bPartialName )
        {
            returnStatus =  
                ScliShoShowArgHelp
                    (
                        hThisObject,
                        hSrvSession,
                        pPrefix,
                        &pOptArgs[nCurArg],
                        pCmdTokenList,
                        pReqArgMatched,
                        ulReqArgCount,
                        pOptArgMatched,
                        ulOptArgCount,
                        FALSE,
                        nCurArg,
                        pErrorPos
                    );

            return  returnStatus;
        }
        else
        {
            bLookupAllOptions = TRUE;
        }
    }

    /* search all named options with prefix */
    if ( bLookupAllOptions )
    {
        PSCLI_SHELL_ARG_HELP_LIST   pHelpList;
        int                         nErrorPos = 0;

        pHelpList = 
            ScliShoGetOptArgOptions
                (
                    pPrefix,
                    pOptArgs,
                    ulOptArgCount,
                    pCmdTokenList,
                    pReqArgMatched,
                    ulReqArgCount,
                    pOptArgMatched,
                    &nErrorPos
                );

        if ( !pHelpList || pHelpList->ulArgHelpCount == 0 )
        {
            nErrorPos   += (int)pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].ulStartPos;
            *pErrorPos   = nErrorPos;
            returnStatus = ANSC_STATUS_UNMATCHED;
        }
        else
        {
            returnStatus = ScliShoShowArgHelpList(hThisObject, hSrvSession, pHelpList);
        }

        if ( pHelpList )
        {
            SCLI_SHELL_FREE_ARG_HELP_LIST(pHelpList);
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIndicateHelp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pPrefix,
                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                int*                        pErrorPos
            );

    description:

        This function is called to show help when user types
        in question mark.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
    
                ANSC_HANDLE                 hSrvSession
                Server session object.

                char*                       pPrefix
                Prefix that partially matches argument name
                or value.

                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax
                Command syntax.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                On return, it specifies how required arguments
                matched.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies the match result of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                specifies the match result of optional arguments.

                int*                        pErrorPos
                On failure return, it specifies where error is 
                detected.

    return:     operation of status.

**********************************************************************/

ANSC_STATUS
ScliShoIndicateHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int*                        pErrorPos
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    ULONG                           i;

    *pErrorPos  = 0;


    /* required arguments */
    for (  i = 0; i < pCmdSyntax->ulReqArgCount; i ++ )
    {
        /* skip argument that completely matches user input */
        if ( pReqArgMatched[i].bMatched && pReqArgMatched[i].bValueMatched )
        {
            if ( pReqArgMatched[i].nMatchTokenEnd < (int)pCmdTokenList->ulTokenCount - 1 || pCmdTokenList->bEndWithSpace )
            {
                continue;
            }
        }

        /* we will show argument help that partial match is detected */
        returnStatus =  
            ScliShoShowArgHelp
                (
                    hThisObject,
                    hSrvSession,
                    pPrefix,
                    &pCmdSyntax->pReqArgs[i],
                    pCmdTokenList,
                    pReqArgMatched,
                    pCmdSyntax->ulReqArgCount,
                    pOptArgMatched,
                    pCmdSyntax->ulOptArgCount,
                    TRUE,
                    i,
                    pErrorPos
                );

        if ( ANSC_STATUS_CONTINUE == returnStatus )
        {
            /* in case all conditions of dependent argument do not satisfy */

            continue;
        }
        else
        {
            return  returnStatus;
        }
    }

    /* optional arguments */
    if ( TRUE )
    {

        for ( i = 0; i < pCmdSyntax->ulOptArgCount; i ++ )
        {
            
            if ( pOptArgMatched[i].bMatched && pOptArgMatched[i].bValueMatched )
            {
                if ( pOptArgMatched[i].nMatchTokenEnd < (int)pCmdTokenList->ulTokenCount - 1 || pCmdTokenList->bEndWithSpace )
                {
                    continue;
                }
            }

            if ( pOptArgMatched[i].bNamePartialMatched || pOptArgMatched[i].bNameMatched ) 
            {
                returnStatus =  
                    ScliShoShowOptArgHelp
                        (
                            hThisObject,
                            hSrvSession,
                            pPrefix,
                            pCmdSyntax->pOptArgs,
                            pCmdSyntax->ulOptArgCount,
                            pCmdTokenList,
                            pReqArgMatched,
                            pCmdSyntax->ulReqArgCount,
                            pOptArgMatched,
                            (int)i,
                            pErrorPos
                        );

                break;
            }
        }

        if ( i >= pCmdSyntax->ulOptArgCount )
        {
            returnStatus =  
                ScliShoShowOptArgHelp
                    (
                        hThisObject,
                        hSrvSession,
                        pPrefix,
                        pCmdSyntax->pOptArgs,
                        pCmdSyntax->ulOptArgCount,
                        pCmdTokenList,
                        pReqArgMatched,
                        pCmdSyntax->ulReqArgCount,
                        pOptArgMatched,
                        (int)-1,
                        pErrorPos
                    );
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoMatchDepArg
            (
                ANSC_HANDLE                 hThisObject,
                PBMC2_CMD_DEP_ARG           pDepArg,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                int*                        pErrorPos
            );

    description:

        This function is called to match user input against
        the specified given command syntax. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
    
                PBMC2_CMD_DEP_ARG           pDepArg,
                Specifies argument we're matching now.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                On return, it specifies how required arguments
                matched.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies the match result of required
                arguments.

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                int*                        pErrorPos
                On failure return, it specifies where error is 
                detected.

    return:     TRUE if user input matches command syntax, FALSE
                otherwise.

**********************************************************************/

BOOL
ScliShoMatchDepArg
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_DEP_ARG           pDepArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        int*                        pErrorPos
    )
{
    BOOL                            bMatched    = FALSE;
    PBMC2_CMD_DEP_ARG_BRANCH        pBr;
    int                             nDepBrIndex = 0;

    nDepBrIndex = ScliShoDepArgFindMatchedBranch(pDepArg, pCmdTokenList, pReqArgMatched, ulReqArgCount);

    if ( nDepBrIndex >= 0 )
    {
        pBr = &pDepArg->pArgBranches[nDepBrIndex];

        bMatched = 
            ScliShoMatchSimpleArg
                (
                    hThisObject,
                    pBr->Type,
                    &pBr->SimpleArg,
                    pCmdTokenList,
                    pArgMatchResult,
                    pErrorPos
                );
    }
    else
    {
        ScliShoInitArgMatchResult(pArgMatchResult);

        bMatched                                = TRUE;
        pArgMatchResult->bMatched               = TRUE;
        pArgMatchResult->bNamePartialMatched    = FALSE;
        pArgMatchResult->bValueMatched          = FALSE;
        pArgMatchResult->bPartialValue          = FALSE;
    }

    return  bMatched;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoMatchSimpleArg
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulArgType,
                PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
                int*                        pErrorPos
            );

    description:

        This function is called to match user input against
        the specified given command syntax. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulArgType
                Specifies the argument type.

                PBMC2_CMD_SIMPLE_ARG        pSimpleArg
                Specifies the simple argument.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                On return, it specifies how required arguments
                matched.

                int*                        pErrorPos
                On failure return, it specifies where error is 
                detected.

                PBOOL                       pbMultiMathced
                On successful return, it specifies if multiple
                string options match the user input.

    return:     TRUE if user input matches command syntax, FALSE
                otherwise.

**********************************************************************/

BOOL
ScliShoMatchSimpleArg
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulArgType,
        PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        int*                        pErrorPos
        
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    BOOL                            bMatched    = TRUE;
    BOOL                            bNamedArg   = ( BMC2_CMD_ARG_TYPE_named == ulArgType );
    char*                           pName       = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].pValue;
    char*                           pValue      = NULL;
    ULONG                           ulValueStart= 0;

    if ( bNamedArg && pCmdTokenList->ulCurToken < pCmdTokenList->ulTokenCount - 1 )
    {
        pValue       = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken + 1].pValue;
        ulValueStart = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken + 1].ulStartPos;
    }
    else if ( !bNamedArg )
    {
        pValue       = pName;
        ulValueStart = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos;
    }

    pArgMatchResult->bMatched             = FALSE;
    pArgMatchResult->bNameMatched         = FALSE;
    pArgMatchResult->bNamePartialMatched  = FALSE;
    pArgMatchResult->bNamedArg            = bNamedArg;

    /* match name */
    if ( bNamedArg )
    {
        if ( strcasecmp(pName, pSimpleArg->DisplayName) == 0 )
        {
            pArgMatchResult->bMatched           = TRUE;
            pArgMatchResult->bNameMatched       = TRUE;
            pArgMatchResult->bNamePartialMatched  = FALSE;
        }
        else
        {
            /* find where name goes wrong */
            int                     nErrorPos = 0;

            SCLI_SHELL_STRING_DIFF_AT(pName, pSimpleArg->DisplayName, nErrorPos);

            if ( *(pName+nErrorPos) == 0 )
            {
                pArgMatchResult->bMatched           = TRUE;
                pArgMatchResult->bNameMatched       = FALSE;
                pArgMatchResult->bNamePartialMatched  = TRUE;

                goto EXIT1;
            }
            else
            {
                *pErrorPos = nErrorPos + (int)pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos;

                pArgMatchResult->bMatched     = FALSE;
                pArgMatchResult->bNameMatched = FALSE;

                pArgMatchResult->nErrorPos   = nErrorPos;

                goto EXIT2;
            }
        }
    }

    /* match value */
    if ( pValue )
    {
        BOOL                        bMultiMatched = FALSE;

        bMatched = 
            ScliShoIsArgValueValid
                (
                    pSimpleArg->ValueType, 
                    pSimpleArg->ValueMinCount, 
                    pSimpleArg->ValueMaxCount,
                    pSimpleArg->ValueMinLength,
                    pSimpleArg->ValueMaxLength,
					pSimpleArg->bStrDupAllowed,
                    pValue,
                    pSimpleArg->pValueRange,
                    pErrorPos,
                    &bMultiMatched
                );

        if ( !bMatched )
        {
            if ( !pCmdTokenList->bEndWithSpace                                 && 
                  pCmdTokenList->ulCurToken >= pCmdTokenList->ulTokenCount - 2 &&
                  *pErrorPos >= (int)AnscSizeOfString(pValue) )
            {
                pArgMatchResult->bMatched       = TRUE;
                pArgMatchResult->bValueMatched  = FALSE;
                pArgMatchResult->bPartialValue  = TRUE;

                bMatched                        = TRUE;
            }
            else
            {
                pArgMatchResult->bMatched   = FALSE;
                *pErrorPos = pArgMatchResult->nErrorPos = *pErrorPos + ulValueStart;
            }
            goto EXIT1;
        }
        else
        {
            ULONG                   ulValueTokenIndex   = 0;

            if ( ulArgType == BMC2_CMD_ARG_TYPE_nameless )
            {
                ulValueTokenIndex = pCmdTokenList->ulCurToken;
            }
            else
            {
                ulValueTokenIndex = pCmdTokenList->ulCurToken + 1;
            }

            pArgMatchResult->bMatched       = TRUE;
            pArgMatchResult->bValueMatched  = TRUE;
            pArgMatchResult->bPartialValue  = FALSE;
            pArgMatchResult->bMultiMatched  = bMultiMatched;

            /* only care about the last argument */
            if ( !pCmdTokenList->bEndWithSpace && 
                 ulValueTokenIndex >= pCmdTokenList->ulTokenCount - 1 )
            {
                int                 LastTokenLen = 0;
                
                if ( ulArgType == BMC2_CMD_ARG_TYPE_nameless )
                {
                    ulValueTokenIndex = pCmdTokenList->ulCurToken;
                }
                else
                {
                    ulValueTokenIndex = pCmdTokenList->ulCurToken + 1;
                }

                LastTokenLen = AnscSizeOfString(pCmdTokenList->pTokens[ulValueTokenIndex].pValue);
               
                if ( *(pCmdTokenList->pTokens[ulValueTokenIndex].pValue+LastTokenLen-1) == ',' &&
                  (NULL == pSimpleArg->pValueRange || pSimpleArg->pValueRange->ulStringCount > 1) )
                {
                    pArgMatchResult->bValueMatched  = FALSE;
                    pArgMatchResult->bPartialValue  = TRUE;
                }
            }
            else if ( pCmdTokenList->bEndWithSpace && pSimpleArg->ValueMaxCount > 1 && pValue &&
                      pValue[AnscSizeOfString(pValue) - 1] == ',' )
            {
                pArgMatchResult->bValueMatched  = FALSE;
                *pErrorPos = pArgMatchResult->nErrorPos = ulValueStart + AnscSizeOfString(pValue) - 1;
                goto EXIT2;
            }
        }
    }
    else if ( !bNamedArg )
    {
        pArgMatchResult->bMatched       = TRUE;
        pArgMatchResult->bPartialValue  = FALSE;
    }

    goto EXIT1;

EXIT2:

    bMatched = FALSE;

EXIT1:

    if ( bMatched )
    {
        pArgMatchResult->nMatchTokenStart = pCmdTokenList->ulCurToken;
        if ( bNamedArg && (pArgMatchResult->bValueMatched || pArgMatchResult->bPartialValue) )
        {
            pArgMatchResult->nMatchTokenEnd = pCmdTokenList->ulCurToken + 1;
        }
        else
        {
            pArgMatchResult->nMatchTokenEnd = pCmdTokenList->ulCurToken;
        }

        pArgMatchResult->nMatchStart = (int)pCmdTokenList->pTokens[pArgMatchResult->nMatchTokenStart].ulStartPos;
        pArgMatchResult->nMatchEnd   = (int)pCmdTokenList->pTokens[pArgMatchResult->nMatchTokenEnd  ].ulStartPos;

        pArgMatchResult->nErrorPos   = -1;
    }

    return  bMatched;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoMatchArg
            (
                ANSC_HANDLE                 hThisObject,
                PBMC2_CMD_ARGUMENT          pArg,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
                int*                        pErrorPos
            );

    description:

        This function is called to match user input against
        the specified given command syntax. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBMC2_CMD_ARGUMENT          pArg
                Specifies argument we're matching now.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                On return, it specifies how required arguments
                matched.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies the match result of required arguments

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                int*                        pErrorPos
                On failure return, it specifies where error is 
                detected.

    return:     TRUE if user input matches command syntax, FALSE
                otherwise.

**********************************************************************/

BOOL
ScliShoMatchArg
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_ARGUMENT          pArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        int*                        pErrorPos
    )
{
    if ( pArg->Type == BMC2_CMD_ARG_TYPE_nameless || 
         pArg->Type == BMC2_CMD_ARG_TYPE_named )
    {
        return  
            ScliShoMatchSimpleArg
                (
                    hThisObject,
                    pArg->Type,
                    &pArg->Arg.Simple,
                    pCmdTokenList,
                    pArgMatchResult,
                    pErrorPos
                );
    }
    else if ( pArg->Type == BMC2_CMD_ARG_TYPE_dependent )
    {
        return  
            ScliShoMatchDepArg
                (
                    hThisObject,
                    &pArg->Arg.Dependent,
                    pCmdTokenList,
                    pArgMatchResult,
                    pReqArgMatched,
                    ulReqArgCount,
                    pErrorPos
                );
    }
    else
    {
        *pErrorPos = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos;
        return  FALSE;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoMatchOptArgs
            (
                ANSC_HANDLE                 hThisObject,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PBMC2_CMD_ARGUMENT          pOptArgs,
                ULONG                       ulOptArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                PULONG                      pulOptArgIndex,
                PBOOL                       pbPartialMatched,
                PULONG                      pulTokenMatched,
                int*                        pErrorPos
            );

    description:

        This function is called to find out which optional 
        argument matches the given user input.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PBMC2_CMD_ARGUMENT          pOptArgs
                Specifies number of required arguments.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies required argument match result.

                ULONG                       ulReqArgCount
                Number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies optional argument match result.

                PBOOL                       pbPartialMatched
                On successful return, it indicates if
                partial match is detected.

                PULONG                      pulTokenMatched
                On successful return, it indicates how many
                token matched.

                int*                        pErrorPos
                On failure return, it indicates where error is 
                detected.

    return:     TRUE if user input matches command syntax, FALSE
                otherwise.

**********************************************************************/

BOOL
ScliShoMatchOptArgs
    (
        ANSC_HANDLE                 hThisObject,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        PBOOL                       pbPartialMatched,
        PULONG                      pulTokenMatched,
        int*                        pErrorPos
    )
{
    BOOL                            bMatched            = FALSE;
    BOOL                            bArgMatched         = FALSE;
    int                             nArgIndex           = -1;
    BOOL                            bValueMatched       = FALSE;
    ULONG                           ulTokenMatched      = 0;
    int                             nErrorPos           = 0;
    int                             nLastMatched        = 0;
    char*                           pLastMatchedArgName = NULL;
    int                             nDepArgIndex        = -1;
    BOOL                            bDepArgOptional     = FALSE;
    ULONG                           i;
    ULONG                           ulArgStart          = 0;
    ULONG                           ulArgEnd            = ulOptArgCount - 1;
    BOOL                            bPartialNameMatched = FALSE;

    nLastMatched = 
        ScliShoGetLastMatchedOptArg
            (
                pReqArgMatched, 
                ulReqArgCount, 
                pOptArgMatched,
                ulOptArgCount
            );

    if ( nLastMatched >= 0 )
    {
        int                         nTokenStart = pOptArgMatched[nLastMatched].nMatchTokenStart;
        int                         nTokenEnd   = pOptArgMatched[nLastMatched].nMatchTokenEnd;

        /* first check if last matched argument is named argument */
        if ( nTokenStart != nTokenEnd && nTokenStart >= 0 )
        {
            pLastMatchedArgName = pCmdTokenList->pTokens[nTokenStart].pValue;
        }

        if ( pLastMatchedArgName )
        {
            /* search unmatched optional arguments to see if there's one that 
             * depends on the last argument matched by its name 
             */
            nDepArgIndex = 
                ScliShoFindDepArgOnName
                    (
                        pOptArgs, 
                        pOptArgMatched, 
                        ulOptArgCount, 
                        pLastMatchedArgName, 
                        &bDepArgOptional
                    );
        }
    }

    *pulTokenMatched = 0;

    if ( nDepArgIndex >= 0 && !bDepArgOptional )
    {
        ulArgStart = (ULONG)nDepArgIndex;
        ulArgEnd   = (ULONG)nDepArgIndex;
    }

    for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
    {
        if ( pOptArgMatched[i].bMatched )
        {
            continue;
        }

        pOptArgMatched[i].bTried = TRUE;

        bArgMatched = 
            ScliShoMatchArg
                (
                    hThisObject,
                    &pOptArgs[i],
                    pCmdTokenList,
                    &pOptArgMatched[i],
                    pReqArgMatched,
                    ulReqArgCount,
                    pErrorPos
                );

        if ( !bArgMatched )
        {
            ScliShoInitArgMatchResult(&pOptArgMatched[i]);

            if ( *pErrorPos > nErrorPos )
            {
                nErrorPos = *pErrorPos;
            }
        }
        else
        {
            if ( pOptArgMatched[i].bMatched )
            {
                if ( pOptArgMatched[i].bValueMatched || pOptArgMatched[i].bNameMatched )
                {
                    bMatched            = TRUE;
                    nArgIndex           = (int)i;
                    bValueMatched       = pOptArgMatched[i].bValueMatched;

                    *pbPartialMatched   = !bValueMatched;
                    ulTokenMatched      = pOptArgMatched[i].nMatchTokenEnd - pOptArgMatched[i].nMatchTokenStart + 1;

                    /* we can not be sure if there's other named option whose name also partially match the name */
                    if ( !pOptArgMatched[i].bValueMatched && !pOptArgMatched[i].bPartialValue &&
                         pCmdTokenList->bEndWithSpace && pOptArgMatched[i].nMatchTokenStart >= (int)pCmdTokenList->ulTokenCount - 1 )
                    {
                        continue;
                    }

                    /* no ambiguity here - we found the optional argument */
                    break;
                }
                else if ( pOptArgMatched[i].bNamePartialMatched )
                {
                    bPartialNameMatched = TRUE;
                }
            }
        }
    }

    if ( bMatched && bPartialNameMatched && !pCmdTokenList->bEndWithSpace )
    {
        /* we have multiple options that can match the argument name - 
          so matching decision is hold until user tells us more */
        bMatched = FALSE;

        pOptArgMatched[nArgIndex].bNameMatched          = FALSE;
        pOptArgMatched[nArgIndex].bNamePartialMatched   = TRUE;
    }

    if ( bMatched )
    {
        BOOL                        bJustTried;

        for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
        {
            bJustTried = pOptArgMatched[i].bTried;
            pOptArgMatched[i].bTried = FALSE;

            if ( pOptArgMatched[i].bMatched )
            {
                continue;
            }

            if ( bJustTried )
            {
                if ( !pOptArgMatched[i].bMatched || pOptArgMatched[i].bNamePartialMatched )
                {
                    ScliShoInitArgMatchResult(&pOptArgMatched[i]);
                }
            }
        }
    }
    else
    {
        ULONG                       ulArgTried = 0;

        for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
        {
            if ( pOptArgMatched[i].bMatched && pOptArgMatched[i].bTried )
            {
                if ( !pCmdTokenList->bEndWithSpace )
                {
                    bMatched          = TRUE;
                    *pbPartialMatched = TRUE;
                    ulTokenMatched    = pOptArgMatched[i].nMatchTokenEnd - pOptArgMatched[i].nMatchTokenStart + 1;
                }

                pOptArgMatched[i].bTried = FALSE;

                ulArgTried ++;
            }
        }        

        if ( !ulArgTried && pCmdTokenList->ulCurToken < pCmdTokenList->ulTokenCount )
        {
            /* no match at the current token */
            if ( nErrorPos < (int)pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos )
            {
                nErrorPos = pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos;
            }
        }

        if ( ulArgTried != 0 && !bMatched )
        {
            nErrorPos += (int)AnscSizeOfString(pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].pValue);
        }
    }
    
    *pulTokenMatched = ulTokenMatched;

    if ( !bMatched )
    {
        *pErrorPos = nErrorPos;
    }
    else
    {
        *pErrorPos = -1;
    }

    return  bMatched;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoMatchCommand
            (
                ANSC_HANDLE                 hThisObject,
                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                int*                        pErrorPos
            );

    description:

        This function is called to match user input against
        the specified given command syntax. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax
                Command syntax to match against.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                User input token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                On return, it specifies how required arguments
                matched.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                On return, it specifies how optional arguments
                matched.

                int*                        pErrorPos
                On failure return, it specifies where error is 
                detected.

    return:     TRUE if user input matches command syntax, FALSE
                otherwise.

**********************************************************************/

BOOL
ScliShoMatchCommand
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int*                        pErrorPos
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    BOOL                            bMatched        = TRUE;
    BOOL                            bPartialMatched = FALSE;
    ULONG                           i;

    *pErrorPos  = 0;

    pCmdTokenList->ulCurToken = 1;      /* skip the command name */

    for ( i = 0; i < pCmdSyntax->ulReqArgCount; i ++ )
    {
        if ( pCmdTokenList->ulCurToken >= pCmdTokenList->ulTokenCount )
        {
            break;
        }

        bMatched = 
            ScliShoMatchArg
                (
                    hThisObject,
                    &pCmdSyntax->pReqArgs[i],
                    pCmdTokenList,
                    &pReqArgMatched[i],
                    pReqArgMatched,
                    pCmdSyntax->ulReqArgCount,
                    pErrorPos
                );

        if ( returnStatus == ANSC_STATUS_CONTINUE )
        {
            /* dependent argument might be skipped in case no branch matches */
            continue;
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto EXIT;
        }

        if ( pReqArgMatched[i].nMatchTokenEnd >= 0 && 
             pReqArgMatched[i].nMatchTokenEnd >= pReqArgMatched[i].nMatchTokenStart )
        {
            pCmdTokenList->ulCurToken += (pReqArgMatched[i].nMatchTokenEnd - pReqArgMatched[i].nMatchTokenStart + 1);
        }

        if ( pReqArgMatched[i].bValueMatched )
        {
            continue;
        }

        /* partially matched or no match */
        if ( pCmdTokenList->bEndWithSpace && pCmdTokenList->ulCurToken >= pCmdTokenList->ulTokenCount - 1 && 
            (pReqArgMatched[i].bPartialValue || pReqArgMatched[i].bNamePartialMatched) )
        {
            int                     nCurToken = pReqArgMatched[i].nMatchTokenEnd;

            bMatched = FALSE;
            if ( nCurToken < (int)pCmdTokenList->ulTokenCount )
            {
                *pErrorPos  = 
                    (int)pCmdTokenList->pTokens[nCurToken].ulStartPos + 
                    (int)AnscSizeOfString(pCmdTokenList->pTokens[nCurToken].pValue);
            }
        }

        break;
    }

    if ( !bMatched || (pCmdSyntax->ulReqArgCount != 0 && i < pCmdSyntax->ulReqArgCount - 1) )
    {
        goto EXIT;
    }

    /* match optional arguments */
    while ( !bPartialMatched && pCmdTokenList->ulCurToken < pCmdTokenList->ulTokenCount )
    {
        ULONG                       ulTokenMatched = 0;

        bMatched = 
            ScliShoMatchOptArgs
                (
                    hThisObject,
                    pCmdTokenList,
                    pCmdSyntax->pOptArgs,
                    pCmdSyntax->ulOptArgCount,
                    pReqArgMatched,
                    pCmdSyntax->ulReqArgCount,
                    pOptArgMatched,
                    &bPartialMatched,
                    &ulTokenMatched,
                    pErrorPos
                );

        if ( bMatched )
        {
            pCmdTokenList->ulCurToken += ulTokenMatched;
        }
        else
        {
            break;
        }
    }
    
EXIT:

    if ( bMatched && pCmdTokenList->ulCurToken < pCmdTokenList->ulTokenCount )
    {
        /* too many arguments */
        bMatched    = FALSE;
        *pErrorPos   = (int)pCmdTokenList->pTokens[pCmdTokenList->ulCurToken].ulStartPos;
    }

    return  bMatched;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        ScliShoGetLastMatchedOptArg
            (
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                ULONG                       ulOptArgCount
            );

    description:

        This function is called to the last matched 
        optional argument.

    argument:   pReqArgMatched
                On return, it specifies how required arguments
                matched.

                ULONG                       ulReqArgCount
                The number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                On return, it specifies how optional arguments
                matched.

                ULONG                       ulOptArgCount
                The number of optional arguments.

    return:     The index to optional argument that is 0 based
                if the last matched argument is an optional one.
                Otherwise, -1 is returned.

**********************************************************************/

int
ScliShoGetLastMatchedOptArg
    (
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount
    )
{
    int                             nLast = -1;

    /*CID: 74816 Dereference after null check*/
    if ( pReqArgMatched         || 
        ulReqArgCount == 0      || 
        (pReqArgMatched && pReqArgMatched[ulReqArgCount - 1].bMatched && pReqArgMatched[ulReqArgCount - 1].bValueMatched) )
    {
        int                         i;
        int                         nMaxTokenPos    = -1;
        PSCLI_CMD_ARG_MATCH_RESULT  pMatchResult;

        for ( i = 0; i < (int)ulOptArgCount; i ++ )
        {
            pMatchResult = &pOptArgMatched[i];

            if ( pMatchResult->bMatched && pMatchResult->bValueMatched )
            {
                if ( pMatchResult->nMatchTokenStart > nMaxTokenPos )
                {
                    nMaxTokenPos = pMatchResult->nMatchTokenStart;
                    nLast        = i;
                }
            }
        }
    }

    return  nLast;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        ScliShoFindDepArgOnName
            (
                PBMC2_CMD_ARGUMENT          pOptArgs,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                ULONG                       ulOptArgCount,
                char*                       pOptArgName
            );

    description:

        This function is called to get an unmatched optional 
        argument that depends on a named optional argument with
        specified name.

    argument:   PBMC2_CMD_ARGUMENT          pOptArgs
                Optional arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                On return, it specifies how optional arguments
                matched.

                ULONG                       ulOptArgCount
                The number of optional arguments.

                char*                       pOptArgName
                Specifies the name of the depending optional 
                argument.

                PBOOL                       pbOptional
                On successful return, it indicates if the 
                argument is optional or required.

    return:     The index to optional argument that is 0 based.
                -1 is returned if no depending argument found.

**********************************************************************/

int
ScliShoFindDepArgOnName
    (
        PBMC2_CMD_ARGUMENT          pOptArgs,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        char*                       pOptArgName,
        PBOOL                       pbOptional
    )
{
    int                         i;
    PSCLI_CMD_ARG_MATCH_RESULT  pMatchResult;
    PBMC2_CMD_ARGUMENT          pArg;
    PBMC2_CMD_DEP_ARG           pDepArg;
    PBMC2_CMD_DEP_ARG_BRANCH    pBr;

    for ( i = 0; i < (int)ulOptArgCount; i ++ )
    {
        pMatchResult = &pOptArgMatched[i];

        if ( pMatchResult->bMatched )
        {
            continue;
        }

        pArg = &pOptArgs[i];

        if ( pArg->Type != BMC2_CMD_ARG_TYPE_dependent )
        {
            continue;
        }

        pDepArg = &pArg->Arg.Dependent;

        if ( pDepArg->ulArgBranchCount > 1 )
        {
            continue;
        }

        pBr = &pDepArg->pArgBranches[0];

        if ( pBr->ulArgDepOn == BMC2_CMD_DEP_ARG_DEP_opt_arg_name || 
             pBr->ulArgDepOn == BMC2_CMD_DEP_ARG_DEP_opt_arg_name_optional )
        {
            if ( strcasecmp(pBr->pArgValue, pOptArgName) == 0 )
            {
                *pbOptional = (pBr->ulArgDepOn == BMC2_CMD_DEP_ARG_DEP_opt_arg_name_optional);
                return  i;
            }
        }
    }

    return  -1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        ScliShoGetOptNamedArgMatched
            (
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PBMC2_CMD_ARGUMENT          pOptArgs,
                ULONG                       ulOptArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                char*                       pArgName
            );

    description:

        This function is called to check if named optional 
        argument with specified name present in user input.

    argument:   PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Command token list.
    
                PBMC2_CMD_ARGUMENT          pOptArgs
                Optional arguments.

                ULONG                       ulOptArgCount
                The number of optional arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                On return, it specifies how optional arguments
                matched.

                char*                       pArgName
                Specifies the argument name of optional argument.

    return:     Specifies the index to the argument queried.

**********************************************************************/

int
ScliShoGetOptNamedArgMatched
    (
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        char*                       pArgName
    )
{
    UNREFERENCED_PARAMETER(pOptArgs);
    int                             i;

    for ( i = 0; i < (int)ulOptArgCount; i ++ )
    {
        if ( !pOptArgMatched[i].bMatched )
        {
            continue;
        }

        if ( pOptArgMatched[i].nMatchTokenEnd > pOptArgMatched[i].nMatchTokenStart )
        {
            int                     nTokenIndex = pOptArgMatched[i].nMatchTokenStart;
            char*                   pTokenValue;

            if ( nTokenIndex < 0 || nTokenIndex >= (int)pCmdTokenList->ulTokenCount )
            {
                continue;
            }

            pTokenValue = pCmdTokenList->pTokens[nTokenIndex].pValue;

            if ( pTokenValue && strcasecmp(pTokenValue, pArgName) == 0 )
            {
                return i;
            }
        }
    }

    return  -1;
}


/**********************************************************************
 * helper functions to validate data type and range
 **********************************************************************/


#define  SCLI_SHELL_IS_DIGIT(c)                                     \
    ( (c) >= '0' && (c) <= '9' )

#define  SCLI_SHELL_IS_ATOF(c)                                      \
    ( ( (c) >= 'a' && (c) <= 'f' ) ||                               \
      ( (c) >= 'A' && (c) <= 'F' ) )

#define  SCLI_SHELL_IS_HEX(c)                                       \
    ( SCLI_SHELL_IS_DIGIT(c) || SCLI_SHELL_IS_ATOF(c) )


static
BOOL
ScliShoIsValidInt
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pNext       = pValue;
    char                            buf[16];
    int                             nValue;
    char*                           pNonZero;
    char*                           pBufStart;
    int                             nValueSigned = 0;
    errno_t   rc = -1;

    *pErrorPos = 0;

    if ( !pNext )
    {
        return  FALSE;
    }

    if ( *pNext == '-' )
    {
        pNext ++;
        nValueSigned = -1;
    }
    else if ( *pNext == '+' )
    {
        pNext ++;
        nValueSigned = 1;
    }

    pNonZero = pNext;

    while ( *pNonZero != 0 && *pNonZero == '0' )
    {
        pNonZero ++;
    }

    if ( *pNonZero == 0 )
    {
        nValue       = 0;
    }
    else 
    {
        while ( *pNext != 0 )
        {
            if ( !SCLI_SHELL_IS_DIGIT(*pNext) )
            {
                *pErrorPos = pNext - pValue;
                return  FALSE;
            }
            pNext ++;
        }

        nValue = (int)_ansc_atoi(pValue);
        rc = sprintf_s(buf, sizeof(buf), "%d", nValue);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return  FALSE;
        }

        if ( (buf[0] == '-' && nValueSigned >= 0) || (buf[0] != '-' && nValueSigned < 0))
        {
            return  FALSE;
        }

        pBufStart = buf;
        if ( *buf == '+' || *buf == '-' )
        {
            pBufStart   = buf + 1;
        }

        if ( AnscSizeOfString(pBufStart) != AnscSizeOfString(pNonZero) ||
             !strcmp(pBufStart, pNonZero) == 0 )
        {
            SCLI_SHELL_STRING_DIFF_AT(pBufStart, pNonZero, *pErrorPos);

            (*pErrorPos) += (pNonZero - pValue);

            if ( nValueSigned )
            {
                (*pErrorPos) ++;
            }

            return  FALSE;
        }
    }

    pArgValue->Val.varInt = nValue;

    return  TRUE;
}


static
BOOL
ScliShoIsValidUint
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pNext = pValue;
    char                            buf[16];
    ULONG                           ulValue;
    char*                           pNonZero;
    errno_t    rc  = -1;

    *pErrorPos = 0;

    if ( !pNext )
    {
        return  FALSE;
    }

    pNonZero = pValue;
    while ( *pNonZero != 0 && *pNonZero == '0' )
    {
        pNonZero ++;
    }

    if ( *pNonZero == 0 )
    {
        ulValue      = 0;
    }
    else
    {
        while ( *pNext != 0 )
        {
            if ( !SCLI_SHELL_IS_DIGIT(*pNext) )
            {
                *pErrorPos = (pNext - pValue);
                return  FALSE;
            }
            pNext ++;
        }

        ulValue = (ULONG)_ansc_atol(pValue);

        rc = sprintf_s(buf, sizeof(buf), "%lu", ulValue);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        if ( AnscSizeOfString(buf) != AnscSizeOfString(pNonZero) ||
             !strcmp(buf, pNonZero) == 0 )
        {
            SCLI_SHELL_STRING_DIFF_AT(buf, pNonZero, *pErrorPos);

            (*pErrorPos) += (pNonZero - pValue);

            return  FALSE;
        }
    }

    pArgValue->Val.varUint = ulValue;

    return  TRUE;
}


static
BOOL
ScliShoIsValidDouble
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pNext = pValue;
    char                            cFirst;
    BOOL                            bNoInt      = FALSE;
    BOOL                            bExp        = FALSE;
    ULONG                           ulInt       = 0;
    ULONG                           ulFrac      = 0;
    ULONG                           ulExp       = 0;
    ULONG                           ulFracLZC   = 0;
    BOOL                            bCountLZC   = FALSE;
    errno_t   rc   = -1;

    *pErrorPos = 0;

    if ( !pNext )
    {
        return  FALSE;
    }

    cFirst = pValue[0];

    if ( cFirst == '+' || cFirst == '-' )
    {
        pNext ++;
        if ( *pNext == '.' )
        {
            bNoInt  = TRUE;
        }
        else if ( !SCLI_SHELL_IS_DIGIT(*pNext) )
        {
            *pErrorPos = pNext - pValue;
            return  FALSE;
        }
    }
    else if ( cFirst == '.' )
    {
        bNoInt  = TRUE;
    }
    else if ( !SCLI_SHELL_IS_DIGIT(cFirst) )
    {
        *pErrorPos = pNext - pValue;
        return  FALSE;
    }

    if ( !bNoInt )
    {

        ulInt = (ULONG)_ansc_atol(pNext);

        while ( SCLI_SHELL_IS_DIGIT(*pNext) )
        {
            pNext ++;
        }
    }

    if ( *pNext == '.' )
    {
        pNext ++;

        if ( *pNext == 'e' )
        {
        }
        else if ( !SCLI_SHELL_IS_DIGIT(*pNext) )
        {
            *pErrorPos = pNext - pValue;
            return  FALSE;
        }
        else
        {

            ulFrac = (ULONG)_ansc_atol(pNext);

            bCountLZC = TRUE;
            while ( SCLI_SHELL_IS_DIGIT(*pNext) )
            {
                if ( bCountLZC )
                {
                    if ( *pNext == '0' )
                    {
                        ulFracLZC ++;
                    }
                    else
                    {
                        bCountLZC = FALSE;
                    }
                }
                pNext ++;
            }
        }
    }

    if ( *pNext != 0 && *pNext != 'e' )
    {
        *pErrorPos = pNext - pValue;
        return  FALSE;
    }
    
    if ( *pNext == 'e' )
    {
        pNext ++;
        if ( *pNext == '+' || *pNext == '-' )
        {
            pNext ++;
        }

        if ( *pNext != 0 && !SCLI_SHELL_IS_DIGIT(*pNext) )
        {
            *pErrorPos = pNext - pValue;
            return  FALSE;
        }

        ulExp = (ULONG)_ansc_atol(pNext);

        bExp = TRUE;

        while ( SCLI_SHELL_IS_DIGIT(*pNext) )
        {
            pNext ++;
        }

        if ( *pNext != 0 )
        {
            *pErrorPos = pNext - pValue;
            return  FALSE;
        }
    }

    if ( bExp )
    {
        ULONG                       ulAddExp = 0;
        ULONG                       ulIntVal = ulInt;

        ulIntVal/=10;
        while ( ulIntVal != 0 )
        {
            ulAddExp ++;
            ulIntVal /= 10;
        }

        if ( ulAddExp == 0 && ulFracLZC != 0 )
        {
            if ( ulExp >= ulFracLZC )
            {
                ulExp -= ulFracLZC;
            }
            else
            {
                ulExp = ulFracLZC - ulExp;
            }
        }

        if ( ulAddExp + ulExp >= BMC_CMD_ARG_TYPE_DOUBLE_exp_max + 1 )
        {
            return  FALSE;
        }
        else if ( ulAddExp + ulExp == BMC_CMD_ARG_TYPE_DOUBLE_exp_max )
        {
            char                    buf[32] = {'0'};
            int                     i;
            int                     max_digits_len = AnscSizeOfString(BMC_CMD_ARG_TYPE_DOUBLE_max_digits);
            int                     nBufLen = 0;

            rc = sprintf_s(buf, sizeof(buf), "%lu%lu", ulInt, ulFrac);
            if(rc < EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }

            nBufLen = AnscSizeOfString(buf);

            for ( i = 0; i < max_digits_len - nBufLen; i ++ )
            {
                buf[nBufLen + i] = '0';
            }

            buf[max_digits_len] = 0;

            if ( _ansc_atol(buf) > _ansc_atol(BMC_CMD_ARG_TYPE_DOUBLE_max_digits) || nBufLen > max_digits_len )
            {
                return FALSE;
            }
        }
    }

    pArgValue->Val.varDouble = _ansc_atof(pValue);

    return  TRUE;
}


static
BOOL
ScliShoIsValidHexString
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos,
        BOOL*                       pbIncomplete
    )
{
    char*                           pNext = pValue;

    *pErrorPos      = 0;
    *pbIncomplete   = FALSE;

    if ( !pNext )
    {
        return  FALSE;
    }

    while ( *pNext != 0 )
    {
        if ( !SCLI_SHELL_IS_HEX(*pNext) )
        {
            *pErrorPos = (pNext - pValue);
            return  FALSE;
        }
        pNext ++;
    }

    if ( AnscSizeOfString(pValue) % 2 )
    {
        *pErrorPos    = AnscSizeOfString(pValue) - 1;
        *pbIncomplete = TRUE;

        return  FALSE;
    }

    pArgValue->Val.varString = pValue;

    return  TRUE;
}


static
BOOL
ScliShoIsValidIp4Addr
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pNext = pValue;
    char                            buf[16];
    ULONG                           a, b, c, d;
    int                             nRet;
    errno_t    rc   = -1;

    *pErrorPos = 0;

    if ( !pNext )
    {
        return  FALSE;
    }

    nRet = _ansc_sscanf(pValue, "%lu.%lu.%lu.%lu", &a, &b, &c, &d);

    if ( nRet == 0 )
    {
        return  FALSE;
    }

    rc = sprintf_s(buf, sizeof(buf), "%u.%u.%u.%u", (UCHAR)a, (UCHAR)b, (UCHAR)c, (UCHAR)d);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }

    if ( AnscSizeOfString(buf) != AnscSizeOfString(pValue) ||
         !strcmp(buf, pValue) == 0 )
    {
        SCLI_SHELL_STRING_DIFF_AT(buf, pValue, *pErrorPos);

        return  FALSE;
    }

    pArgValue->Val.varUint = (((UCHAR)a) << 24) + (((UCHAR)b) << 16) + (((UCHAR)c) << 8) + ((UCHAR)d);

    return  TRUE;
}


static
BOOL
ScliShoIsValidIp6Addr
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    BOOL                            bValid;

    *pErrorPos = 0;

    bValid = AnscParseIp6Address(pValue, pArgValue->Val.varIp6Addr);

    return  bValid;
}


static
BOOL
ScliShoIsValidIp6Prefix
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pCur    = pValue;
    char*                           pLength = NULL;
    BOOL                            bValid;
    ULONG                           ulLength= 0;
    char                            buf[16];
    errno_t    rc   = -1;

    *pErrorPos = 0;

    pLength = _ansc_strchr(pCur, '/');
    if ( !pLength )
    {
        return  FALSE;
    }

    *pLength = 0;
    pLength ++;

    bValid = AnscParseIp6Address(pValue, pArgValue->Val.varIp6Prefix.Ip6Addr);

    if ( !bValid )
    {
        return  FALSE;
    }

    ulLength = (ULONG)_ansc_atoi(pLength);
    rc = sprintf_s(buf, sizeof(buf), "%lu", ulLength);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }
    if ( ulLength > 128 || !strcmp(buf, pLength) == 0 )
    {
        *pErrorPos = (pLength - pCur);
        return  FALSE;
    }

    pArgValue->Val.varIp6Prefix.Length = ulLength;

    return  bValid;
}


static
BOOL
ScliShoIsValidMacAddr
    (
        char*                       pValue,
        PBMC2_CMD_ARG_VALUE         pArgValue,
        int*                        pErrorPos
    )
{
    char*                           pNext = pValue;
    char                            buf[16];
    ULONG                           a, b, c, d, e, f;
    int                             nRet, nMaxInput = 0;
    BOOL                            bColonSeparated = FALSE;
    errno_t   rc  = -1;

    *pErrorPos = 0;

    if ( !pNext )
    {
        return  FALSE;
    }

    nRet = _ansc_sscanf(pValue, "%2x-%2x-%2x-%2x-%2x-%2x", (PUINT)&a, (PUINT)&b, (PUINT)&c, (PUINT)&d, (PUINT)&e, (PUINT)&f);

    if ( nRet != 6 )
    {
        nMaxInput = nRet;

        nRet = _ansc_sscanf(pValue, "%2x:%2x:%2x:%2x:%2x:%2x", (PUINT)&a, (PUINT)&b, (PUINT)&c, (PUINT)&d, (PUINT)&e, (PUINT)&f);

        if ( nRet == 0 )
        {
            return  FALSE;
        }
        if ( nRet > nMaxInput )
        {
            bColonSeparated = TRUE;
        }
    }

    if ( bColonSeparated )
    {
        rc = sprintf_s(buf, sizeof(buf), "%.2x:%.2x:%.2x:%.2x:%.2x;%.2x", (UCHAR)a, (UCHAR)b, (UCHAR)c, (UCHAR)d, (UCHAR)e, (UCHAR)f);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
    }
    else
    {
        rc = sprintf_s(buf, sizeof(buf), "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x", (UCHAR)a, (UCHAR)b, (UCHAR)c, (UCHAR)d, (UCHAR)e, (UCHAR)f);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
    }

    if ( AnscSizeOfString(buf) != AnscSizeOfString(pValue) ||
         !strcasecmp(buf, pValue) == 0 )
    {
        SCLI_SHELL_STRING_DIFF_AT(buf, pValue, *pErrorPos);

        return  FALSE;
    }

    pArgValue->Val.varMacAddr[0] = (UCHAR)a;
    pArgValue->Val.varMacAddr[1] = (UCHAR)b;
    pArgValue->Val.varMacAddr[2] = (UCHAR)c;
    pArgValue->Val.varMacAddr[3] = (UCHAR)d;
    pArgValue->Val.varMacAddr[4] = (UCHAR)e;
    pArgValue->Val.varMacAddr[5] = (UCHAR)f;

    return  TRUE;
}


static
BOOL
ScliShoIsIntValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    return  
        ( pArgValue->Val.varInt >= pRange->Min.varInt && 
          pArgValue->Val.varInt <= pRange->Max.varInt );
}


static
BOOL
ScliShoIsUintValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    return  
        ( pArgValue->Val.varUint >= pRange->Min.varUint && 
          pArgValue->Val.varUint <= pRange->Max.varUint );
}


static
BOOL
ScliShoIsDoubleValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    return  
        ( pArgValue->Val.varDouble >= pRange->Min.varDouble && 
          pArgValue->Val.varDouble <= pRange->Max.varDouble );
}


static
BOOL
ScliShoIsStringValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange,
        int*                        pErrorPos,
        PBOOL                       pbMultiMatched
    )
{
    ULONG                           i;
    char*                           pValueOption    = NULL;
    char*                           pValue          = pArgValue->Val.varString;
    int                             nErrorPos       = 0;
    int                             j = 0;
    BOOL                            bMatched        = FALSE;

    *pErrorPos      = 0;
    *pbMultiMatched = FALSE;

    for ( i = 0; i < pRange->ulStringCount; i ++ )
    {
        pValueOption = pRange->pStringArray[i];
        if ( strcasecmp(pValue, pValueOption) == 0 )
        {
            bMatched = TRUE;
        }
        else
        {   
            SCLI_SHELL_STRING_DIFF_AT(pValueOption, pValue, j);

            if ( nErrorPos < j )
            {
                nErrorPos = j;
            }

            if ( j >= (int)AnscSizeOfString(pValue) )
            {
                *pbMultiMatched = TRUE;
            }
        }
    }

    *pErrorPos = nErrorPos;

    return  bMatched;
}


static
ULONG
ScliShoIsStringValueInRange2
    (
        char*						pValue,
        PBMC2_CMD_ARG_VRANGE        pRange
    )
{
    ULONG                           i;
	ULONG							ulCount = 0;
	int								vLen	= AnscSizeOfString(pValue);
	int								vsLen;

    for ( i = 0; i < pRange->ulStringCount; i ++ )
    {
        char*						pValueOption = pRange->pStringArray[i];

		vsLen		 = AnscSizeOfString(pValueOption);

		if ( vsLen >= vLen )
		{
			if ( AnscEqualString2(pValueOption, pValue, vLen, FALSE) )
			{
				ulCount ++;
			}
		}
    }

    return  ulCount;
}


static
BOOL
ScliShoIsHexValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    UNREFERENCED_PARAMETER(pArgValue);
    UNREFERENCED_PARAMETER(pRange);
    return  TRUE;
}


static
BOOL
ScliShoIsIp4AddrValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    return  
        ( pArgValue->Val.varUint >= pRange->Min.varUint && 
          pArgValue->Val.varUint <= pRange->Max.varUint );
}


static
BOOL
ScliShoIsIp6AddrValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    BOOL                            bValid = TRUE;
    ULONG                           i;

    for ( i = 0; i < 16; i ++ )
    {
        if ( pArgValue->Val.varIp6Addr[i] < pRange->Min.varIp6Addr[i] ||
             pArgValue->Val.varIp6Addr[i] > pRange->Max.varIp6Addr[i] )
        {
            return  FALSE;
        }
    }

    return  bValid;
}


static
BOOL
ScliShoIsMacAddrValueInRange
    (
        PBMC2_CMD_ARG_VALUE         pArgValue,
        PBMC2_CMD_ARG_VRANGE        pRange        
    )
{
    UNREFERENCED_PARAMETER(pArgValue);
    UNREFERENCED_PARAMETER(pRange);
    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoIsArgValueValid
            (
                ULONG                       ulDataType,
                ULONG                       ulMinOccur,
                ULONG                       ulMaxOccur,
                ULONG                       ulMinLength,
                ULONG                       ulMaxLength,
                char*                       pValue,
                PBMC2_CMD_ARG_VRANGE        pRange,
                PULONG                      pulErroPos,
                PBOOL                       pbMultiMathced
            );

    description:

        This function is called to determine if given value
        is valid checked against the given data type
        and value range.

    argument:   ULONG                       ulDataType
                Specifies the data type.

                ULONG                       ulMinOccur
                Specifies the min occurrence of the value.

                ULONG                       ulMaxOccur
                Specifies the max occurrence of the value.

                ULONG                       ulMinLength
                Specifies the minimum value length.

                ULONG                       ulMaxLength
                Specifies the maximum value length.

                char*                       pValue
                The value to be checked.

                PBMC2_CMD_ARG_VRANGE        pRange
                Specifies the value range.

                PULONG                      pulErroPos
                Specifies where error occurs.
    
                PBOOL                       pbMultiMatched
                Specifies if multiple options <got to be
                string> match the value.

    return:     internal presentation of command.

**********************************************************************/

BOOL
ScliShoIsArgValueValid
    (
        ULONG                       ulDataType,
        ULONG                       ulMinOccur,
        ULONG                       ulMaxOccur,
        ULONG                       ulMinLength,
        ULONG                       ulMaxLength,
		BOOL						bStrDupAllowed,
        char*                       pValue,
        PBMC2_CMD_ARG_VRANGE        pRange,
        int*                        pErrorPos,
        PBOOL                       pbMultiMatched
    )
{
    BOOL                            bValid      = TRUE;
    PSCLI_SHELL_CMD_TLIST           pValueList;
    ULONG                           i;
    BMC2_CMD_ARG_VALUE              ArgValue;
    int                             nErrorPos   = 0;
	int								vLen		= AnscSizeOfString(pValue);
	BOOL							bCommaEnded = ( pValue[vLen - 1] == ',' );
	BOOL							bLastItemDup= FALSE;
	ULONG							ulStrCount	= 0;

    *pbMultiMatched = FALSE;

    if ( ulMaxOccur > 1 )
    {
        pValueList = ScliShoTokenizeCommand(pValue, SCLI_SHELL_CMD_VLIST_SEPARATOR);
    }
    else
    {
        pValueList = ScliShoTokenizeCommand(pValue, 0);
    }

    if ( !pValueList )
    {
        goto EXIT2;
    }

    if ( pValueList->ulTokenCount < ulMinOccur || pValueList->ulTokenCount > ulMaxOccur )
    {
        goto EXIT2;
    }

	/* check duplication */
	if ( !bStrDupAllowed && ulMaxOccur > 1 && pValueList->ulTokenCount > 1 )
	{
		ULONG						j;
		ULONG						slen;
		char*						pStr;

	    for ( i = 1; i < pValueList->ulTokenCount; i ++ )
		{
			pStr = pValueList->pTokens[i].pValue;
			slen = AnscSizeOfString(pStr);
			for ( j = 0; j < i; j ++ )
			{
				if ( AnscSizeOfString(pValueList->pTokens[j].pValue) == slen &&
					 AnscEqualString2(pValueList->pTokens[j].pValue, pStr, slen, FALSE) )
				{
					if ( !bCommaEnded && i == pValueList->ulTokenCount - 1 )
					{
						/* determine if last one can match multiple selections and
						 * if all of them are specified in user input
						 */
                        ulStrCount = ScliShoIsStringValueInRange2(pValueList->pTokens[i].pValue, pRange);

						if ( ulStrCount > 1 )
						{
							bLastItemDup = TRUE;

							/* we assume this is good, we will check further later */
							break;
						}
					}

		            *pErrorPos = (int)pValueList->pTokens[i].ulStartPos + nErrorPos;
					goto EXIT2;
				}
			}
		}
	}

    for ( i = 0; i < pValueList->ulTokenCount; i ++ )
    {
        switch ( ulDataType )
        {
            case    BMC2_CMD_ARG_VTYPE_int:

                    bValid = ScliShoIsValidInt(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsIntValueInRange(&ArgValue, pRange);
                    }
                    else if ( !bValid && pRange )
                    {
                        ArgValue.Val.varString = pValueList->pTokens[i].pValue;
                        bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_uint:

                    bValid = ScliShoIsValidUint(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsUintValueInRange(&ArgValue, pRange);
                    }
                    else if ( !bValid && pRange )
                    {
                        ArgValue.Val.varString = pValueList->pTokens[i].pValue;
                        bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_double:

                    bValid = ScliShoIsValidDouble(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsDoubleValueInRange(&ArgValue, pRange);
                    }
                    else if ( !bValid && pRange )
                    {
                        ArgValue.Val.varString = pValueList->pTokens[i].pValue;
                        bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_hex:

                    {
                        BOOL        bIncomplete = FALSE;

                        bValid = ScliShoIsValidHexString(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos, &bIncomplete);

                        if ( bIncomplete )
                        {
                            ULONG       ulValueLength = AnscSizeOfString(pValueList->pTokens[i].pValue);

                            if ( nErrorPos >= 0 && ulValueLength <= ulMaxLength*2 )
                            {
                                nErrorPos ++;   /* make sure calling function wouldn't consider this as unmatched */
                            }
                        }
                        else
                        {
                            if ( !bValid && pRange )
                            {
                                ArgValue.Val.varString = pValueList->pTokens[i].pValue;
                                bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                            }
                            else if ( bValid )
                            {
                                if ( pRange )
                                {
                                    bValid = ScliShoIsHexValueInRange(&ArgValue, pRange);
                                }

                                if ( bValid )
                                {
                                    ULONG       ulValueLength = AnscSizeOfString(pValueList->pTokens[i].pValue)/2;

                                    bValid = ( ulValueLength >= ulMinLength && ulValueLength <= ulMaxLength );
                                    if ( !bValid )
                                    {
                                        nErrorPos = ((ulValueLength < ulMinLength ) ? ulValueLength : ulMaxLength)*2;
                                    }
                                }
                            }
                        }
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip4addr:

                    bValid = ScliShoIsValidIp4Addr(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsIp4AddrValueInRange(&ArgValue, pRange);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip6addr:

                    bValid = ScliShoIsValidIp6Addr(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsIp6AddrValueInRange(&ArgValue, pRange);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip6prefix:

                    bValid = ScliShoIsValidIp6Prefix(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);

                    break;

            case    BMC2_CMD_ARG_VTYPE_macaddr:

                    bValid = ScliShoIsValidMacAddr(pValueList->pTokens[i].pValue, &ArgValue, &nErrorPos);
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsMacAddrValueInRange(&ArgValue, pRange);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_string:

                    bValid = TRUE;
                    ArgValue.Val.varString = pValueList->pTokens[i].pValue;
                    if ( bValid && pRange )
                    {
                        bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                    }
                    if ( bValid )
                    {
                        ULONG       ulValueLength = AnscSizeOfString(pValueList->pTokens[i].pValue);

                        bValid = ( ulValueLength >= ulMinLength && ulValueLength <= ulMaxLength );
                        if ( !bValid )
                        {
                            nErrorPos = (ulValueLength < ulMinLength ) ? ulValueLength : ulMaxLength;
                        }
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_qstring:

                    /* check if the string is quoted correctly */
                    if ( TRUE )
                    {
                        char*       pQValue     = pValueList->pTokens[i].pValue;
                        int         nQVLen      = AnscSizeOfString(pQValue);
                        BOOL        bNotQuoted  = (*pQValue != BMC2_CMD_ARG_VALUE_QSTRING_SEP && *(pQValue+nQVLen-1) != BMC2_CMD_ARG_VALUE_QSTRING_SEP);

                        bValid = (*pQValue == BMC2_CMD_ARG_VALUE_QSTRING_SEP && *(pQValue+nQVLen-1) == BMC2_CMD_ARG_VALUE_QSTRING_SEP) || bNotQuoted;

                        if ( !bValid )
                        {
                            nErrorPos = (*pQValue == BMC2_CMD_ARG_VALUE_QSTRING_SEP) ? nQVLen : 0;
                        }
                        else
                        {
                            ULONG       ulValueLength;

                            if ( !bNotQuoted )
                            {
                                *(pQValue + nQVLen - 1) = 0;
                                pQValue ++;
                            }

                            ulValueLength = AnscSizeOfString(pQValue);
                            bValid        = ( ulValueLength >= ulMinLength && ulValueLength <= ulMaxLength );
                            if ( !bValid )
                            {
                                nErrorPos = ( ulValueLength < ulMinLength ) ? ulValueLength : ulMaxLength;
                            }

                            if ( bValid && pRange )
                            {
                                ArgValue.Val.varString = pQValue;
                                bValid = ScliShoIsStringValueInRange(&ArgValue, pRange, &nErrorPos, pbMultiMatched);
                            }

                            if ( bValid )
                            {
                                ULONG       ulValueLength = AnscSizeOfString(pValueList->pTokens[i].pValue);

                                bValid = ( ulValueLength >= ulMinLength && ulValueLength <= ulMaxLength );

                                if ( !bValid )
                                {
                                    nErrorPos = (ulValueLength < ulMinLength ) ? ulValueLength : ulMaxLength;

                                    if ( !bNotQuoted )
                                    {
                                        nErrorPos ++;
                                    }
                                }
                            }
                        }
                    }

                    break;
        }

        if ( !bValid )
        {
            *pErrorPos = (int)pValueList->pTokens[i].ulStartPos + nErrorPos;
            goto EXIT2;
        }
    }

	if ( bValid && bLastItemDup )
	{
        char*						pLast = pValueList->pTokens[pValueList->ulTokenCount - 1].pValue;
		int							vLen  = AnscSizeOfString(pLast);
		char*						pSel;
		ULONG						ulDup = 0;

	    for ( i = 0; i < pValueList->ulTokenCount - 1; i ++ )
		{
			pSel = pValueList->pTokens[i].pValue;

			if ( (int)AnscSizeOfString(pSel) > vLen &&
				 AnscEqualString2(pSel, pLast, vLen, FALSE) == 0 )
			{
				ulDup ++;
			}
		}

		if ( ulDup >= ulStrCount )
		{
            *pErrorPos = (int)pValueList->pTokens[pValueList->ulTokenCount - 1].ulStartPos + nErrorPos;
			goto EXIT2;
		}
	}

    goto EXIT1;

EXIT2:

    bValid = FALSE;

EXIT1:

    if ( pValueList )
    {
        ScliShoFreeCmdTokenList(pValueList);
    }

    return  bValid;
}


ANSC_STATUS
ScliShoShowArgHelpList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PSCLI_SHELL_ARG_HELP_LIST   pHelpList
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    ULONG                           i, j;
    ULONG                           ulItemMaxLen = (pHelpList->ulMaxItemLen)/4*4 + 8;
    ULONG                           ulItemLen;


    for ( i = 0;  i < pHelpList->ulArgHelpCount; i ++ )
    {
        if ( !pHelpList->pArgHelps[i].pItem )
        {
            break;
        }
        ulItemLen = AnscSizeOfString(pHelpList->pArgHelps[i].pItem);

        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    SCLI_SHELL_OUTPUT_INDENT,
                    AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                );

        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pHelpList->pArgHelps[i].pItem,
                    ulItemLen
                );

        if ( pHelpList->pArgHelps[i].pDescription )
        {
            for ( j = 0; j < ulItemMaxLen - ulItemLen; j ++ )
            {
                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            " ",
                            1
                        );
            }

            returnStatus =
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        pHelpList->pArgHelps[i].pDescription,
                        AnscSizeOfString(pHelpList->pArgHelps[i].pDescription)
                    );
        }

        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );
    }

    return  returnStatus;
}


int
ScliShoDepArgFindMatchedBranch
    (
        PBMC2_CMD_DEP_ARG           pDepArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount
    )
{
    UNREFERENCED_PARAMETER(ulReqArgCount);
    ULONG                       i;
    PBMC2_CMD_DEP_ARG_BRANCH    pDepBr;
    int                         nArgIndex;
    int                         nTokenIndex;
    char*                       pValue;

    for ( i = 0; i < pDepArg->ulArgBranchCount; i ++ )
    {
        pDepBr = &pDepArg->pArgBranches[i];

        nArgIndex = (int)pDepBr->ulArgIndex;

        if ( pDepBr->ulArgDepOn == BMC2_CMD_DEP_ARG_DEP_required && nArgIndex < 1 )
        {
            continue;
        }
        else if ( pDepBr->ulArgDepOn != BMC2_CMD_DEP_ARG_DEP_required )
        {
            return (int)i;
        }

        if ( pDepBr->pArgValue )
        {
            nTokenIndex  = pReqArgMatched[nArgIndex-1].nMatchTokenEnd;
            if ( nTokenIndex < 0 )
            {
                continue;
            }
            pValue       = pCmdTokenList->pTokens[nTokenIndex].pValue;

            if ( pValue && strcasecmp(pValue, pDepBr->pArgValue) == 0 ) 
            {
                return (int)i;
            }
        }
        else
        {
            return  (int)i;
        }
    }

    return  -1;
}


static
void
ScliShoBuildValueType
    (
        char*                       pValueType,
        PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
        ULONG                       ulHelpIndex
    )
{
    PBMC2_CMD_ARG_VRANGE            pVrange     = pSimpleArg->pValueRange;
    char                            rep[64]     = {0};
    BOOL                            bShowVLen;
    errno_t    rc  = -1;

    *pValueType = 0;

    bShowVLen = 
        ( pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_string  || 
          pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_qstring /* ||
          pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_hex */ );

    if ( bShowVLen && !(pSimpleArg->ValueMinLength == 0 && pSimpleArg->ValueMaxLength == (ULONG)BMC2_CMD_ARG_TYPE_UINT_max) )
    {
        if ( pSimpleArg->ValueMaxCount == BMC2_CMD_ARG_TYPE_UINT_max )
        {
            rc = sprintf_s(rep, sizeof(rep), "(%lu-)", pSimpleArg->ValueMinLength);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
        else
        {
            rc = sprintf_s(rep, sizeof(rep), "(%lu-%lu)", pSimpleArg->ValueMinLength, pSimpleArg->ValueMaxLength);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
    }

    if ( pSimpleArg->ValueMinCount != 1 || pSimpleArg->ValueMaxCount != 1 )
    {
        if ( pSimpleArg->ValueMaxCount == BMC2_CMD_ARG_TYPE_UINT_max )
        {
            rc = sprintf_s(rep, sizeof(rep), "[%lu-]", pSimpleArg->ValueMinCount);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
        else
        {
            rc = sprintf_s(rep, sizeof(rep), "[%lu-%lu]", pSimpleArg->ValueMinCount, pSimpleArg->ValueMaxCount);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
    }

    if ( pVrange && ulHelpIndex == 0 )
    {
        switch ( pSimpleArg->ValueType )
        {
            case    BMC2_CMD_ARG_VTYPE_int:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%d ~ %d> %s", pVrange->Min.varInt, pVrange->Max.varInt, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                    break;

            case    BMC2_CMD_ARG_VTYPE_uint:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%lu ~ %lu> %s", pVrange->Min.varUint, pVrange->Max.varUint, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                    break;

            case    BMC2_CMD_ARG_VTYPE_double:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%g ~ %g> %s", pVrange->Min.varDouble, pVrange->Max.varDouble, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip4addr: 

                    if ( TRUE )
                    {
                        UCHAR   a1, b1, c1, d1, a2, b2, c2, d2;

                        a1 = (UCHAR)((pVrange->Min.varUint) >> 24);
                        b1 = (UCHAR)((pVrange->Min.varUint) >> 16);
                        c1 = (UCHAR)((pVrange->Min.varUint) >> 8 );
                        d1 = (UCHAR)((pVrange->Min.varUint)      );

                        a2 = (UCHAR)((pVrange->Max.varUint) >> 24);
                        b2 = (UCHAR)((pVrange->Max.varUint) >> 16);
                        c2 = (UCHAR)((pVrange->Max.varUint) >> 8 );
                        d2 = (UCHAR)((pVrange->Max.varUint)      );

                        rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%u.%u.%u.%u ~ %u.%u.%u.%u> %s", a1, b1, c1, d1, a2, b2, c2, d2, rep);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_string:

                    if ( ulHelpIndex < pVrange->ulStringCount )
                    {
                        rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "%s %s", pVrange->pStringArray[ulHelpIndex], rep);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }

                    break;


            case    BMC2_CMD_ARG_VTYPE_qstring:

                    if ( ulHelpIndex < pVrange->ulStringCount )
                    {
                        rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "%s %s", pVrange->pStringArray[ulHelpIndex], rep);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }

                    break;

        }
    }
    else if ( pVrange )
    {
        if ( pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_string && ulHelpIndex < pVrange->ulStringCount )
        {
            rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "%s %s", pVrange->pStringArray[ulHelpIndex], rep);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
        else if ( pSimpleArg->ValueType != BMC2_CMD_ARG_VTYPE_string && ulHelpIndex <= pVrange->ulStringCount )
        {
            rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "%s %s", pVrange->pStringArray[ulHelpIndex-1], rep);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
    }

    if ( !pValueType[0] )
    {
        switch ( pSimpleArg->ValueType )
        {
            case    BMC2_CMD_ARG_VTYPE_int:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_int, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                    break;

            case    BMC2_CMD_ARG_VTYPE_uint:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_uint, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_double:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_double, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip4addr:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<a.b.c.d> %s", rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip6addr:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<IPv6 address> %s", rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_ip6prefix:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<IPv6 prefix> %s", rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_string:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_string, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_qstring:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_qstring, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_macaddr:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<HH-HH-HH-HH-HH-HH> %s", rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;

            case    BMC2_CMD_ARG_VTYPE_hex:

                    rc = sprintf_s(pValueType, MAX_PVALUETYPE_SIZE, "<%s> %s", BMC2_CMD_ARG_VTYPE_NAME_hex, rep);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }

                    break;
        }
    }
}


ANSC_STATUS
ScliShoGetSimpleArgHelp
    (
        ULONG                       ulArgType,
        PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
        char*                       pPrefix,
        PSCLI_SHELL_ARG_HELP_LIST   pHelpList,
        ULONG                       ulListStartFrom,
        PULONG                      pulMatchedHelpCount,
        int*                        pErrorPos
    )
{
    UNREFERENCED_PARAMETER(ulArgType);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
	char*							pOrgPrefix          = pPrefix;
    ULONG                           i;
    PBMC2_CMD_ARG_HELP              pArgHelp, pArgHelpTBS;
    PBMC2_CMD_ARG_VRANGE            pVrange             = NULL;
    ULONG                           ulMatchedHelpCount  = 0;
    int                             nErrorPos           = 0;
    ULONG                           ulHelpCount         = pSimpleArg->HelpItemCount;
    BOOL                            bTypeArgHelpGen     = FALSE;
    BOOL                            bStringArgType;
    BOOL                            bNoMainHelpItem     = TRUE;

    bStringArgType = 
        ( pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_string  || 
          pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_qstring /*|| 
          pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_hex*/);

    pVrange = pSimpleArg->pValueRange;

    if ( ulHelpCount == 0 )
    {
        ulHelpCount = 1;    /* no help item specified */

        if ( pSimpleArg->ValueType == BMC2_CMD_ARG_VTYPE_string && pVrange )
        {
            ulHelpCount = pVrange->Min.varUint;
        }
        else if ( pVrange )
        {
            ulHelpCount = 1 + pVrange->ulStringCount;
        }
    }

    *pErrorPos = 0;

    if ( !pPrefix && ulListStartFrom + ulHelpCount > pHelpList->ulArgHelpCount )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    /* considering comma-separated input */
    if ( pPrefix && _ansc_strchr(pPrefix, ',') != NULL && 
        pVrange && pVrange->pStringArray && pVrange->ulStringCount > 1 &&
        pSimpleArg->ValueMaxCount > 1 )
    {
        /* count how many items we have based on assumption that comma ',' is 
         * not allowed in value options, meaning comma needs to be escaped if
         * this assumption becomes invalid in future
         */
        char*                       pNext       = pPrefix;

        while ( pPrefix && *pPrefix != '\0' )
        {
            pNext = _ansc_strchr(pPrefix, ',');

            if ( !pNext )
            {
                /* last input item */
                break;
            }

#if 0       /* we don't need to validate value(s) here since we should have done so previously */
            /* check if current item is acceptable */
            for ( i = 0; i < pVrange->ulStringCount; i ++ )
            {
                if ( AnscEqualString2(pVrange->pStringArray[i], pPrefix, pNext - pPrefix, FALSE) )
                {
                    break;
                }
            }

            if ( i >= pVrange->ulStringCount )
            {
                /* no match found, we found an error */
                nErrorPos = pPrefix - pOrgPrefix;
                goto EXIT;
            }

            nItemIndex ++;
#endif

            pPrefix = pNext + 1;
        }

        if ( pPrefix && *pPrefix == '\0' )
        {
            pPrefix = NULL;
        }
    }

    for ( i = 0 ; i < ulHelpCount; i ++ )
    {
        if ( pSimpleArg->pHelpItems )
        {
            pArgHelp = &pSimpleArg->pHelpItems[i];
        }
        else
        {
            pArgHelp = NULL;
        }
        pArgHelpTBS = &pHelpList->pArgHelps[ulListStartFrom + ulMatchedHelpCount];

        if ( i == 0 && pPrefix && !bStringArgType )
        {
            if ( !pArgHelp || !pArgHelp->pItem )
            {
                /* skip the main type of this argument and we will get back to this if
                 * no predefined strings are available
                 */
                bNoMainHelpItem = FALSE;

                continue;
            }
        }

        /* check if prefix matches string value which has value range defined */
        if ( pPrefix && pVrange && pVrange->pStringArray && pVrange->ulStringCount != 0 )
        {
            ULONG                   ulStringIndex = bNoMainHelpItem ? i : i - 1;

            if ( ulStringIndex < pVrange->ulStringCount && 
                 ScliShoFindSubStrNoCase(pVrange->pStringArray[ulStringIndex], pPrefix) != pVrange->pStringArray[ulStringIndex] )
            {
                int                 j;

                for ( j = 0; j < (int)AnscSizeOfString(pPrefix); j ++ )
                {
                    if ( pPrefix[j] != pVrange->pStringArray[ulStringIndex][j] )
                    {
                        if ( j > nErrorPos )
                        {
                            nErrorPos = j;
                        }
                        break;
                    }
                }

                continue;
            }
        }

        if ( pArgHelp && pArgHelp->pItem )
        {
            pArgHelpTBS->pItem = AnscCloneString(pArgHelp->pItem);
        }
        else /* if ( !pPrefix || bStringArgType ) */
        {
            char                    item[64]    = {0};

			/* single out duplicate items */
			if ( !pSimpleArg->bStrDupAllowed && pSimpleArg->ValueMaxCount > 1 && pVrange && pVrange->pStringArray && pVrange->ulStringCount >= 1 )
			{
                ULONG               ulStringIndex = bNoMainHelpItem ? i : i - 1;

                if ( pOrgPrefix && pOrgPrefix != pPrefix && 
                     ScliShoIsStringInList(pOrgPrefix, pVrange->pStringArray[ulStringIndex], TRUE, FALSE ) )
                {
                    continue;
                }
			}

            ScliShoBuildValueType(item, pSimpleArg, i);

            if ( item[0] )
            {
                pArgHelpTBS->pItem = AnscCloneString(item);

                bTypeArgHelpGen = TRUE;
            }
            else
            {
                returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                goto EXIT;
            }
        }

        if ( pArgHelpTBS->pItem && AnscSizeOfString(pArgHelpTBS->pItem) > pHelpList->ulMaxItemLen )
        {
            pHelpList->ulMaxItemLen = AnscSizeOfString(pArgHelpTBS->pItem);
        }

        if ( pArgHelp && pArgHelp->pDescription && AnscSizeOfString(pArgHelp->pDescription) != 0 )
        {
            pArgHelpTBS->pDescription = AnscCloneString(pArgHelp->pDescription);
        }

        ulMatchedHelpCount ++;
    }

    if ( !bTypeArgHelpGen && !bStringArgType )
    {
        if ( !pPrefix || ulMatchedHelpCount == 0 )
        {
            /* except help count, we need to add help item for the main numeric value */
            char                    item[64]    = {0};

            ScliShoBuildValueType(item, pSimpleArg, i);

            pArgHelpTBS = &pHelpList->pArgHelps[ulListStartFrom + ulMatchedHelpCount];

            if ( item[0] )
            {
                pArgHelpTBS->pItem = AnscCloneString(item);
                ulMatchedHelpCount ++;
            }
        }
    }

EXIT:
    *pulMatchedHelpCount = ulMatchedHelpCount;

    if ( ulMatchedHelpCount == 0 && nErrorPos == 0 && 
        !pSimpleArg->bStrDupAllowed && pSimpleArg->ValueMaxCount > 1 &&
        pOrgPrefix && *(pOrgPrefix + AnscSizeOfString(pOrgPrefix) - 1) == ',' )
    {
        nErrorPos = AnscSizeOfString(pOrgPrefix) - 1;
    }

    *pErrorPos           = nErrorPos;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoAutoCompleteArg
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pPrefix,
                PBMC2_CMD_ARGUMENT          pCmdArg,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                ULONG                       ulOptArgCount,
                BOOL                        bReqArg,
                ULONG                       ulCurArg,
                SLAP_STRING_ARRAY**         ppStringArray
            );

    description:

        This function is called to output help text on
        the specified argument.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                ANSC_HANDLE                 hSrvSession
                Specifies which session the error indicated to.

                char*                       pPrefix
                Prefix of argument name or value.

                PBMC2_CMD_ARGUMENT          pCmdArg
                Specifies the command argument.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies how required arguments matched

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies optional argument match result.

                ULONG                       ulOptArgCount
                Specifies number of optional arguments.

                BOOL                        bReqArg
                Specifies if required argument is being 
                queried.

                ULONG                       ulCurArg
                Specifies the current argument user is typing.    

                SLAP_STRING_ARRAY**         ppStringArray
                On successful return, it contains options
                for auto completion.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoAutoCompleteArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pCmdArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        BOOL                        bReqArg,
        ULONG                       ulCurArg,
        SLAP_STRING_ARRAY**         ppStringArray
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSrvSession);
    UNREFERENCED_PARAMETER(ulOptArgCount);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_STRING_ARRAY              pAcStrArray  = (PSLAP_STRING_ARRAY   )NULL;
    PBMC2_CMD_SIMPLE_ARG            pSimpleArg   = NULL;
    ULONG                           ulSimArgType = pCmdArg->Type;
    BOOL                            bSkipNamePart;
    ULONG                           i;

    if ( ulSimArgType == BMC2_CMD_ARG_TYPE_dependent )
    {
        int                         nArgIndex;
        PBMC2_CMD_DEP_ARG           pDepArg = &pCmdArg->Arg.Dependent;

        nArgIndex = ScliShoDepArgFindMatchedBranch(pDepArg, pCmdTokenList, pReqArgMatched, ulReqArgCount);
        if ( nArgIndex < 0 )
        {
            pSimpleArg = NULL;
        }
        else
        {
            pSimpleArg      = &pDepArg->pArgBranches[nArgIndex].SimpleArg;
            ulSimArgType    = pDepArg->pArgBranches[nArgIndex].Type;
        }
    }
    else if ( ulSimArgType == BMC2_CMD_ARG_TYPE_nameless || 
              ulSimArgType == BMC2_CMD_ARG_TYPE_named )
    {
        pSimpleArg = &pCmdArg->Arg.Simple;
    }

    if ( !pSimpleArg )
    {
        return  ANSC_STATUS_CONTINUE;
    }

    if ( bReqArg )
    {
        bSkipNamePart = ( pReqArgMatched[ulCurArg].bPartialValue                                 || 
                          pReqArgMatched[ulCurArg].bValueMatched                                 || 
                         (pReqArgMatched[ulCurArg].bNameMatched && pCmdTokenList->bEndWithSpace) || 
                          ulSimArgType == BMC2_CMD_ARG_TYPE_nameless );
    }
    else
    {
        bSkipNamePart = ( pOptArgMatched[ulCurArg].bPartialValue                                 || 
                          pOptArgMatched[ulCurArg].bValueMatched                                 || 
                         (pOptArgMatched[ulCurArg].bNameMatched && pCmdTokenList->bEndWithSpace) || 
                          ulSimArgType == BMC2_CMD_ARG_TYPE_nameless );
    }

    if ( !bSkipNamePart )
    {
        /* must be a named argument */
        if ( (pPrefix && ScliShoFindSubStrNoCase(pSimpleArg->DisplayName, pPrefix) == pSimpleArg->DisplayName) || !pPrefix )
        {
            SlapAllocStringArray2(1, pAcStrArray);

            pAcStrArray->Array.arrayString[0]   = AnscCloneString(pSimpleArg->DisplayName);
        }
        else
        {
            returnStatus = ANSC_STATUS_UNMATCHED;
        }
    }
    else
    {
        if ( !pSimpleArg->pValueRange || 0 == pSimpleArg->pValueRange->ulStringCount )
        {
            returnStatus = ANSC_STATUS_UNMATCHED;
        }
        else
        {
            ULONG                   ulItems     = 0;
            char*                   pOrgPrefix  = pPrefix;

            /* if prefix contains comma(s), only the last item should be concerned */
            if ( pPrefix && pSimpleArg->ValueMaxCount > 1 )
            {
                int                 nPrefixLen = AnscSizeOfString(pPrefix);
                int                 k;
                
                for ( k = nPrefixLen - 1; k >= 0; k -- )
                {
                    if ( pPrefix[k] == ',' )
                    {
                        pPrefix = (k >= nPrefixLen - 1) ? NULL : pPrefix + k + 1;
                        break;
                    }
                }
            }

            SlapAllocStringArray2(pSimpleArg->pValueRange->ulStringCount, pAcStrArray);

            for ( i = 0; i < pSimpleArg->pValueRange->ulStringCount; i ++ )
            {
                if ( pPrefix && ScliShoFindSubStrNoCase(pSimpleArg->pValueRange->pStringArray[i], pPrefix) != pSimpleArg->pValueRange->pStringArray[i] )
                {
                    continue;
                }

                if ( !pSimpleArg->bStrDupAllowed && pOrgPrefix && ScliShoIsStringInList(pOrgPrefix, pSimpleArg->pValueRange->pStringArray[i], TRUE, FALSE ) )
                {
                    continue;
                }
                
                /*
                if ( pPrefix && pOrgPrefix != pPrefix )
                {
                    int             nDiff = AnscSizeOfString(pSimpleArg->pValueRange->pStringArray[i]) - AnscSizeOfString(pPrefix);
                    char*           pComp = (char*)AnscAllocateMemory(sizeof(char) * (nDiff + AnscSizeOfString(pOrgPrefix) + 1));

                    if ( pComp )
                    {
                        AnscCopyString(pComp, pOrgPrefix);
                        AnscCopyString
                            (
                                pComp + AnscSizeOfString(pComp), 
                                (pSimpleArg->pValueRange->pStringArray[i]) + AnscSizeOfString(pSimpleArg->pValueRange->pStringArray[i]) - nDiff
                            );
                    }

                    pAcStrArray->Array.arrayString[ulItems ++] = pComp;
                }
                else
                */
                {
                    pAcStrArray->Array.arrayString[ulItems ++] = AnscCloneString(pSimpleArg->pValueRange->pStringArray[i]);
                }
            }

            pAcStrArray->VarCount = ulItems;
        }
    }

    *ppStringArray = pAcStrArray;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoAutoCompleteOptArg
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pPrefix,
                PBMC2_CMD_ARGUMENT          pOptArgs,
                ULONG                       ulOptArgCount,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                int                         nCurArg,
                SLAP_STRING_ARRAY**         ppStringArray
            )

    description:

        This function is called to output help text on
        the specified argument.

    argument:   ANSC_HANDLE                 hThisObject
                Specifies this object.

                ANSC_HANDLE                 hSrvSession
                Specifies which session the error indicated to.

                char*                       pPrefix
                Prefix of argument name or value.

                PBMC2_CMD_ARGUMENT          pOptArgs,
                Specifies the optional command argument.

                ULONG                       ulOptArgCount
                Specifies the number of optional arguments.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the command token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies how required arguments matched

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                Specifies how optional arguments matched.

                int                         nCurArg
                Specifies the current argument user is typing.    

                SLAP_STRING_ARRAY**         ppStringArray
                Specifies options that can auto completion has.

    return:     status of operation.

**********************************************************************/

static
ANSC_STATUS
ScliShoGetOptArgsAcOptions
    (
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        SLAP_STRING_ARRAY**         ppStringArray
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    ULONG                           ulAcCount           = 0;
    ULONG                           ulMaxAcCount        = 32;
    SLAP_STRING_ARRAY*              pAcStrArray         = NULL;
    int                             nLastMatched        = 0;
    char*                           pLastMatchedArgName = NULL;
    int                             nDepArgIndex        = -1;
    BOOL                            bDepArgOptional     = FALSE;
    ULONG                           ulArgStart          = 0;
    ULONG                           ulArgEnd            = ulOptArgCount - 1;
    ULONG                           i;
    PBMC2_CMD_SIMPLE_ARG            pSimpleArg;
    ULONG                           ulArgType;

    nLastMatched = 
        ScliShoGetLastMatchedOptArg
            (
                pReqArgMatched, 
                ulReqArgCount, 
                pOptArgMatched,
                ulOptArgCount
            );

    if ( nLastMatched >= 0 )
    {
        int                         nTokenStart = pOptArgMatched[nLastMatched].nMatchTokenStart;
        int                         nTokenEnd   = pOptArgMatched[nLastMatched].nMatchTokenEnd;

        /* first check if last matched argument is named argument */
        if ( nTokenStart != nTokenEnd && nTokenStart >= 0 )
        {
            pLastMatchedArgName = pCmdTokenList->pTokens[nTokenStart].pValue;
        }

        if ( pLastMatchedArgName )
        {
            /* search unmatched optional arguments to see if there's one that 
             * depends on the last argument matched by its name 
             */
            nDepArgIndex = 
                ScliShoFindDepArgOnName
                    (
                        pOptArgs, 
                        pOptArgMatched, 
                        ulOptArgCount, 
                        pLastMatchedArgName, 
                        &bDepArgOptional
                    );

            if ( nDepArgIndex >= 0 && !bDepArgOptional )
            {
                ulArgStart  = (ULONG)nDepArgIndex;
                ulArgEnd    = (ULONG)nDepArgIndex;
            }
        }
    }

    SlapAllocStringArray2(ulMaxAcCount, pAcStrArray);

    /* scan all available options */
    for ( i = ulArgStart; ulOptArgCount != 0 && i <= ulArgEnd; i ++ )
    {
        if ( pOptArgMatched[i].bValueMatched )
        {
            continue;
        }

        if ( pOptArgs[i].Type == BMC2_CMD_ARG_TYPE_dependent )
        {
            int                     nArgIndex;
            
            nArgIndex = 
                ScliShoDepArgFindMatchedBranch
                    (
                        &pOptArgs[i].Arg.Dependent,
                        pCmdTokenList,
                        pReqArgMatched,
                        ulReqArgCount
                     );

            if ( nArgIndex < 0 )
            {
                continue;
            }

            if ( pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].ulArgDepOn != BMC2_CMD_DEP_ARG_DEP_required )
            {
                char*               pArgName = pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].pArgValue;
                int                 nMasterArgIndex;

                nMasterArgIndex = 
                    ScliShoGetOptNamedArgMatched
                        (
                            pCmdTokenList, 
                            pOptArgs, 
                            ulOptArgCount, 
                            pOptArgMatched, 
                            pArgName
                        );

                if ( nMasterArgIndex < 0 )
                {
                    continue;
                }
            }

            pSimpleArg = &pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].SimpleArg;
            ulArgType  = pOptArgs[i].Arg.Dependent.pArgBranches[nArgIndex].Type;
        }
        else
        {
            pSimpleArg = &pOptArgs[i].Arg.Simple;
            ulArgType  = pOptArgs[i].Type;
        }

        if ( ulArgType == BMC2_CMD_ARG_TYPE_nameless )
        {
            /* we should include help on nameless argument that has string value with prefix */
            PBMC2_CMD_ARG_VRANGE    pValueRange = pSimpleArg->pValueRange;

            if ( !pValueRange && pSimpleArg->ValueType != BMC2_CMD_ARG_VTYPE_string )
            {
                continue;
            }
            else if ( pValueRange )
            {
                ULONG               j;

                for ( j = 0; j < pValueRange->ulStringCount; j ++ )
                {
                    if ( !pPrefix || ScliShoFindSubStrNoCase(pValueRange->pStringArray[j], pPrefix) == pValueRange->pStringArray[j] )
                    {
                        pAcStrArray->Array.arrayString[ulAcCount++] = AnscCloneString(pValueRange->pStringArray[j]);
                    }
                }
            }
        }
        else if ( !pPrefix || ScliShoFindSubStrNoCase(pSimpleArg->DisplayName, pPrefix) == pSimpleArg->DisplayName )
        {
            pAcStrArray->Array.arrayString[ulAcCount++] = AnscCloneString(pSimpleArg->DisplayName);
        }
    }

    if ( !ulAcCount )
    {
        SlapFreeVarArray(pAcStrArray);
    }
    else
    {
        if ( !pPrefix )
        {
            pAcStrArray->Array.arrayString[ulAcCount++] = AnscCloneString(SCLI_SHELL_OPTION_CR);
        }

        pAcStrArray->VarCount   = ulAcCount;
        *ppStringArray          = pAcStrArray;
    }
    
    return  returnStatus;
}


ANSC_STATUS
ScliShoAutoCompleteOptArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int                         nCurArg,
        SLAP_STRING_ARRAY**         ppStringArray
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    BOOL                            bLookupAllOptions   = FALSE;
    SLAP_STRING_ARRAY*              pSlapStrArray       = *ppStringArray;
    SLAP_STRING_ARRAY*              pAcStrArray         = NULL;

    if ( nCurArg < 0 || (ULONG)nCurArg >= ulOptArgCount  )
    {
        bLookupAllOptions = TRUE;
    }
    else
    {
        BOOL                        bPartialName    = pOptArgMatched[nCurArg].bNamePartialMatched;

        if ( !bPartialName )
        {
            returnStatus =  
                ScliShoAutoCompleteArg
                    (
                        hThisObject,
                        hSrvSession,
                        pPrefix,
                        &pOptArgs[nCurArg],
                        pCmdTokenList,
                        pReqArgMatched,
                        ulReqArgCount,
                        pOptArgMatched,
                        ulOptArgCount,
                        FALSE,
                        nCurArg,
                        &pAcStrArray
                    );

            if ( returnStatus == ANSC_STATUS_SUCCESS && pAcStrArray )
            {
                if ( !pSlapStrArray )
                {
                    *ppStringArray  = pAcStrArray;
                }
                else
                {
                    ULONG               ulAcCount       = pAcStrArray->VarCount + pSlapStrArray->VarCount;
                    SLAP_STRING_ARRAY*  pNewAcStrArray  = NULL;
                    ULONG               i;

                    SlapAllocStringArray2(ulAcCount, pNewAcStrArray);

                    if ( !pNewAcStrArray )
                    {
                        returnStatus = ANSC_STATUS_RESOURCES;
                    }
                    else
                    {
                        for ( i = 0; i < pSlapStrArray->VarCount; i ++ )
                        {
                            pNewAcStrArray->Array.arrayString[i] = AnscCloneString(pSlapStrArray->Array.arrayString[i]);
                        }

                        for ( i = 0; i < pAcStrArray->VarCount; i ++ )
                        {
                            pNewAcStrArray->Array.arrayString[i + pSlapStrArray->VarCount] = AnscCloneString(pAcStrArray->Array.arrayString[i]);
                        }

                        SlapFreeVarArray(pSlapStrArray);
                        SlapFreeVarArray(pAcStrArray);

                        *ppStringArray = pNewAcStrArray;
                    }
                }
            }

            return  returnStatus;
        }
        else
        {
            /* show all options with partial name as prefix */
            int                     nTokenPos = pOptArgMatched[nCurArg].nMatchTokenStart;

            if ( nTokenPos >= 0 && (ULONG)nTokenPos < pCmdTokenList->ulTokenCount )
            {
                pPrefix = pCmdTokenList->pTokens[nTokenPos].pValue;
            }

            bLookupAllOptions = TRUE;
        }
    }

    /* search all named options with prefix */
    if ( bLookupAllOptions )
    {
        returnStatus = 
            ScliShoGetOptArgsAcOptions
                (
                    pPrefix,
                    pOptArgs,
                    ulOptArgCount,
                    pCmdTokenList,
                    pReqArgMatched,
                    ulReqArgCount,
                    pOptArgMatched,
                    ppStringArray
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoAutoCompleteCommandArg
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                char*                       pPrefix,
                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
                SLAP_STRING_ARRAY**         ppStringArray
            );

    description:

        This function is called to show help when user types
        in question mark.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
    
                ANSC_HANDLE                 hSrvSession
                Server session object.

                char*                       pPrefix
                Prefix of argument name or value.

                PBMC2_CMD_HELP_SYNTAX       pCmdSyntax
                Command syntax.

                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the token list.

                PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
                On return, it specifies how required arguments
                matched.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies the match result of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched
                specifies the match result of optional arguments.

                SLAP_STRING_ARRAY**         ppStringArray
                On successful return, it contains all possible
                match for auto completion.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoAutoCompleteCommandArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        SLAP_STRING_ARRAY**         ppStringArray
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    ULONG                           i;

    *ppStringArray = NULL;

    /* required arguments */
    for (  i = 0; i < pCmdSyntax->ulReqArgCount; i ++ )
    {
        /* skip argument that completely matches user input */
        if ( pReqArgMatched[i].bMatched && pReqArgMatched[i].bValueMatched )
        {
            if ( pReqArgMatched[i].nMatchTokenEnd < (int)pCmdTokenList->ulTokenCount - 1 || !pReqArgMatched[i].bMultiMatched )
            {
                continue;
            }

            if ( pCmdTokenList->bEndWithSpace && pReqArgMatched[i].nMatchTokenEnd >= (int)pCmdTokenList->ulTokenCount - 1 )
            {
                continue;
            }
        }

        /* we will show argument help that partial match is detected */
        returnStatus =  
            ScliShoAutoCompleteArg
                (
                    hThisObject,
                    hSrvSession,
                    pPrefix,
                    &pCmdSyntax->pReqArgs[i],
                    pCmdTokenList,
                    pReqArgMatched,
                    pCmdSyntax->ulReqArgCount,
                    pOptArgMatched,
                    pCmdSyntax->ulOptArgCount,
                    TRUE,
                    i,
                    ppStringArray
                );

        if ( ANSC_STATUS_CONTINUE == returnStatus )
        {
            /* in case all conditions of dependent argument do not satisfy */

            continue;
        }
        else
        {
            return  returnStatus;
        }
    }

    /* optional arguments */
    if ( TRUE )
    {

        for ( i = 0; i < pCmdSyntax->ulOptArgCount; i ++ )
        {
            
            if ( pOptArgMatched[i].bMatched && pOptArgMatched[i].bValueMatched )
            {
                if ( pOptArgMatched[i].nMatchTokenEnd < (int)pCmdTokenList->ulTokenCount - 1 || !pOptArgMatched[i].bMultiMatched )
                {
                    continue;
                }
            }

            if ( pOptArgMatched[i].bNamePartialMatched || pOptArgMatched[i].bNameMatched ) 
            {
                returnStatus =  
                    ScliShoAutoCompleteOptArg
                        (
                            hThisObject,
                            hSrvSession,
                            pPrefix,
                            pCmdSyntax->pOptArgs,
                            pCmdSyntax->ulOptArgCount,
                            pCmdTokenList,
                            pReqArgMatched,
                            pCmdSyntax->ulReqArgCount,
                            pOptArgMatched,
                            (int)i,
                            ppStringArray
                        );

                break;
            }
        }

        if ( i >= pCmdSyntax->ulOptArgCount )
        {
            returnStatus =  
                ScliShoAutoCompleteOptArg
                    (
                        hThisObject,
                        hSrvSession,
                        pPrefix,
                        pCmdSyntax->pOptArgs,
                        pCmdSyntax->ulOptArgCount,
                        pCmdTokenList,
                        pReqArgMatched,
                        pCmdSyntax->ulReqArgCount,
                        pOptArgMatched,
                        (int)-1,
                        ppStringArray
                    );
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoCanReqArgsSkipped
            (
                ANSC_HANDLE                 hThisObject,
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
                PBMC2_CMD_ARGUMENT          pReqArgs,
                ULONG                       ulReqArgCount,
                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
                ULONG                       ulReqArgFrom
            )

    description:

        This function is called to check if the required
        arguments from the given position can be skipped
        in command argument validation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
    
                PSCLI_SHELL_CMD_TLIST       pCmdTokenList
                Specifies the token list.

                PBMC2_CMD_ARGUMENT          pReqArgs
                Specifies required arguments.

                ULONG                       ulReqArgCount
                Specifies the number of required arguments.

                PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched
                Specifies the match result of required arguments.

                ULONG                       ulReqArgFrom
                The index of required argument to be checked from.

    return:     status of operation.

**********************************************************************/

BOOL
ScliShoCanReqArgsSkipped
    (
        ANSC_HANDLE                 hThisObject,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pReqArgs,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgFrom
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           i;
    PBMC2_CMD_DEP_ARG               pDepArg;
    int                             nArgIndex;

    for ( i = ulReqArgFrom; i < ulReqArgCount; i ++ )
    {
        if ( pReqArgs[i].Type != BMC2_CMD_ARG_TYPE_dependent )
        {
            return  FALSE;
        }

        pDepArg = &pReqArgs[i].Arg.Dependent;

        nArgIndex = ScliShoDepArgFindMatchedBranch(pDepArg, pCmdTokenList, pReqArgMatched, ulReqArgCount);

        if ( nArgIndex >= 0 )
        {
            return  FALSE;
        }
    }

    return  TRUE;
}

