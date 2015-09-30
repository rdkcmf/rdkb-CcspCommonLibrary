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

    module:	ansc_token.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to token operation.

        *   AnscTcAllocate
        *   AnscTcAllocate2
        *   AnscTcAllocate3
        *   AnscTcFree
        *   AnscTcGetLength
        *   AnscTcEqualString
        *   AnscTcPopToken
        *   AnscGetTokenChain
        *   AnscSetTokenChain
        *   AnscDelTokenChain

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/24/01    initial revision.

**********************************************************************/


#include "ansc_global.h"


ANSC_HANDLE
AnscTcAllocate
    (
        char*                       string,
        char*                       separator
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = string? (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN)) : NULL;
    PANSC_STRING_TOKEN              pToken      = NULL;
    ULONG                           ulTokenSize = 0;
    ULONG                           ulSizeOfStr = string? AnscSizeOfString(string) : 0;
    char*                           pTemp       = string;

    if ( !pTokenChain )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !string )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }
    else
    {
        AnscQueueInitializeHeader(&pTokenChain->TokensQueue);
    }

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || (*string == 0) || !ulTokenSize )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    while ( string && (*string != 0) && ulTokenSize && ((ULONG)(string - pTemp) < ulSizeOfStr) )
    {
        pToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

        if ( !pToken )
        {
            AnscTcFree((ANSC_HANDLE)pTokenChain);

            return  (ANSC_HANDLE)NULL;
        }
        else if ( ulTokenSize >= ANSC_URI_PATH_SIZE )
        {
            AnscFreeMemory(pToken                  );
            AnscTcFree    ((ANSC_HANDLE)pTokenChain);

            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            AnscCopyMemory(pToken->Name, string, ulTokenSize);

            pToken->hChain = (ANSC_HANDLE)pTokenChain;

            AnscQueuePushEntry(&pTokenChain->TokensQueue, &pToken->Linkage);
        }

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    }

    return  (ANSC_HANDLE)pTokenChain;
}


ANSC_HANDLE
AnscTcAllocate2
    (
        char*                       string,
        char*                       separator,
        char*                       alphabet
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain   = string? (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN)) : NULL;
    PANSC_STRING_TOKEN              pToken        = NULL;
    ULONG                           ulTokenSize   = 0;
    ULONG                           ulSizeOfStr   = string? AnscSizeOfString(string) : 0;
    char*                           pTemp         = string;
    BOOL                            bAlphabetOnly = FALSE;
    ULONG                           i             = 0;

    if ( !pTokenChain )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !string )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }
    else
    {
        AnscQueueInitializeHeader(&pTokenChain->TokensQueue);
    }

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || (*string == 0) || !ulTokenSize )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    while ( string && (*string != 0) && ulTokenSize && ((ULONG)(string - pTemp) < ulSizeOfStr) )
    {
        if ( TRUE )
        {
            for ( i = 0; i < ulTokenSize; i++ )
            {
                if ( !AnscCharInString(alphabet, string[i]) )
                {
                    break;
                }
            }

            if ( i < ulTokenSize )
            {
                bAlphabetOnly = FALSE;
            }
            else
            {
                bAlphabetOnly = TRUE;
            }
        }

        if ( bAlphabetOnly )
        {
            pToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

            if ( !pToken )
            {
                AnscTcFree((ANSC_HANDLE)pTokenChain);

                return  (ANSC_HANDLE)NULL;
            }
            else
            {
                AnscCopyMemory(pToken->Name, string, ulTokenSize);

                pToken->hChain = (ANSC_HANDLE)pTokenChain;

                AnscQueuePushEntry(&pTokenChain->TokensQueue, &pToken->Linkage);
            }
        }

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    }

    return  (ANSC_HANDLE)pTokenChain;
}


ANSC_HANDLE
AnscTcAllocate3
    (
        char*                       string,
        char*                       separator
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = string? (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN)) : NULL;
    PANSC_STRING_TOKEN              pToken      = NULL;
    ULONG                           ulTokenSize = 0;
    ULONG                           ulSizeOfStr = string? AnscSizeOfString(string) : 0;
    char*                           pTemp       = string;

    if ( !pTokenChain )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !string )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }
    else
    {
        AnscQueueInitializeHeader(&pTokenChain->TokensQueue);
    }

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken3(string, separator, AnscSizeOfString(string));

    if ( !string || (*string == 0) || !ulTokenSize )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    while ( string && (*string != 0) && ulTokenSize && ((ULONG)(string - pTemp) < ulSizeOfStr) )
    {
        pToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

        if ( !pToken )
        {
            AnscTcFree((ANSC_HANDLE)pTokenChain);

            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            if ( *string == '"' && (ulTokenSize >= 2) )
            {
                AnscCopyMemory(pToken->Name, string + 1, ulTokenSize - 2);
            }
            else
            {
                AnscCopyMemory(pToken->Name, string, ulTokenSize);
            }

            pToken->hChain = (ANSC_HANDLE)pTokenChain;

            AnscQueuePushEntry(&pTokenChain->TokensQueue, &pToken->Linkage);
        }

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken3(string, separator, AnscSizeOfString(string));
    }

    return  (ANSC_HANDLE)pTokenChain;
}


void
AnscTcFree
    (
        ANSC_HANDLE                 hTokenChain
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    AnscDelTokenChain((ANSC_HANDLE)pTokenChain);
    AnscFreeMemory   (pTokenChain);

    return;
}


ULONG
AnscTcGetLength
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       separator
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    ULONG                           ulStringLen = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pToken      = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        ulStringLen += AnscSizeOfString(pToken->Name);
        ulStringLen += 1;       /* separator */
    }

    return  ulStringLen;
}


BOOL
AnscTcEqualString
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator,
        BOOL                        bCaseSensitive,
        BOOL                        bStrict
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_TOKEN_CHAIN               pSrcTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_TOKEN_CHAIN               pDstTokenChain = NULL;
    PANSC_STRING_TOKEN              pSrcToken      = NULL;
    PANSC_STRING_TOKEN              pDstToken      = NULL;
    ULONG                           ulTokenCount   = AnscTcGetTokenCount(pSrcTokenChain);
    ULONG                           ulStrLen       = AnscTcGetLength(hTokenChain, separator);
    char*                           pSrcString     = string;
    char*                           pDstString     = NULL;
    BOOL                            bStrMatched    = FALSE;
    ULONG                           i              = 0;
    ANSC_TOKEN_CHAIN                tempTc;

    if ( bStrict )
    {
        bStrMatched = FALSE;
        pDstString  = (char*)AnscAllocateMemory(ulStrLen + 16);

        if ( !pDstString )
        {
            return  FALSE;
        }
        else
        {
            returnStatus =
                AnscGetTokenChain
                    (
                        (ANSC_HANDLE)pSrcTokenChain,
                        pDstString,
                        separator
                    );
        }

        if ( AnscEqualString
                (
                    pSrcString,
                    pDstString,
                    bCaseSensitive
                ) )
        {
            bStrMatched = TRUE;
        }
        else
        {
            bStrMatched = FALSE;
        }

        AnscFreeMemory(pDstString);
    }
    else
    {
        bStrMatched    = FALSE;
        pDstTokenChain = (PANSC_TOKEN_CHAIN)&tempTc;
        returnStatus   =
            AnscSetTokenChain
                (
                    (ANSC_HANDLE)pDstTokenChain,
                    pSrcString,
                    separator
                );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            if ( ulTokenCount != AnscTcGetTokenCount(pDstTokenChain) )
            {
                bStrMatched = FALSE;
            }
            else
            {
                for ( i = 0; i < ulTokenCount; i++ )
                {
                    pSrcToken = (PANSC_STRING_TOKEN)AnscQueueSearchEntryByIndex(&pSrcTokenChain->TokensQueue, i);
                    pDstToken = (PANSC_STRING_TOKEN)AnscQueueSearchEntryByIndex(&pDstTokenChain->TokensQueue, i);

                    if ( !AnscEqualString
                            (
                                pSrcToken->Name,
                                pDstToken->Name,
                                bCaseSensitive
                            ) )
                    {
                        break;
                    }
                }

                if ( i >= ulTokenCount )
                {
                    bStrMatched = TRUE;
                }
                else
                {
                    bStrMatched = FALSE;
                }
            }
        }

        AnscDelTokenChain((ANSC_HANDLE)pDstTokenChain);
    }

    return  bStrMatched;
}


ANSC_HANDLE
AnscTcPopToken
    (
        ANSC_HANDLE                 hTokenChain
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pTokenChain->TokensQueue);

    if ( pSLinkEntry )
    {
        pToken = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pToken;
}


ANSC_STATUS
AnscGetTokenChain
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    ULONG                           ulIndex     = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pToken      = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        AnscCopyString(&string[ulIndex], pToken->Name);

        ulIndex        += AnscSizeOfString(pToken->Name);
        string[ulIndex] = separator[0];
        ulIndex        ++;
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscSetTokenChain
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    ULONG                           ulTokenSize = 0;
    ULONG                           ulSizeOfStr = AnscSizeOfString(string);
    char*                           pTemp       = string;

    if ( !string )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( !pTokenChain )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        AnscQueueInitializeHeader(&pTokenChain->TokensQueue);
    }

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || (*string == 0) || !ulTokenSize )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    while ( string && (*string != 0) && ulTokenSize && ((ULONG)(string - pTemp) < ulSizeOfStr) )
    {
        pToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

        if ( !pToken )
        {
            AnscDelTokenChain((ANSC_HANDLE)pTokenChain);

            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            AnscCopyMemory(pToken->Name, string, ulTokenSize);

            pToken->hChain = (ANSC_HANDLE)pTokenChain;

            AnscQueuePushEntry(&pTokenChain->TokensQueue, &pToken->Linkage);
        }

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    }

    return  ANSC_STATUS_SUCCESS;
}


void
AnscDelTokenChain
    (
        ANSC_HANDLE                 hTokenChain
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain = (PANSC_TOKEN_CHAIN)hTokenChain;
    PANSC_STRING_TOKEN              pToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pToken      = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pTokenChain->TokensQueue);

        AnscFreeMemory(pToken);
    }

    return;
}
