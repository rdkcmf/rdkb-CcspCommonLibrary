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

    description:

        This module implements the some platform-dependent and
        general utility functions related to token operation.

        *   AnscTcAllocate
        *   AnscTcAllocate2
        *   AnscTcAllocate3
        *   AnscTcFree
        *   AnscTcGetLength
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
    /*RDKB-6185, CID-24092, null check before use*/
    if ( !string || (*string == 0) )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    if ( !ulTokenSize )
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

	/*CID: 59825 Dereference before null check*/
        if ( !string )
        {
            return  (ANSC_HANDLE)pTokenChain;
        }
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
    /*RDKB-6185, CID-24126, null check before use*/
    if ( !string || (*string == 0) )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    if ( !ulTokenSize )
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

	/*CID: 61614 Dereference before null check*/
        if ( !string )
        {
            return  (ANSC_HANDLE)pTokenChain;
        }
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
    /*RDKB-6185, CID-24426, null check before use*/
    if ( !string || (*string == 0) )
    {
        return  (ANSC_HANDLE)pTokenChain;
    }

    ulTokenSize = AnscSizeOfToken3(string, separator, AnscSizeOfString(string));

    if ( !ulTokenSize )
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
	/*CID: 55215 Dereference before null check*/
        if ( !string )
        {
           return  (ANSC_HANDLE)pTokenChain;
        }
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

    AnscDelTokenChain((ANSC_HANDLE)pTokenChain);
    AnscFreeMemory   (pTokenChain);
    /* CID 154673 fix */
    (pTokenChain) = NULL;

    return;
}


ULONG
AnscTcGetLength
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       separator
    )
{
    UNREFERENCED_PARAMETER(separator);
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
    ULONG                           ulSizeOfStr = 0; /*RDKB-6185, CID-24307, use after null check*/
    char*                           pTemp       = NULL;

    if ( !string )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pTemp       = string;
    ulSizeOfStr = AnscSizeOfString(string);

    if ( !pTokenChain )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        AnscQueueInitializeHeader(&pTokenChain->TokensQueue);
    }

    string      = AnscMoveToNextToken(string, separator);

    /*CID: 62792 Dereference before null check*/
    if ( !string )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !ulTokenSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
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
 
	/*CID: 62792 Dereference before null check*/
        if ( !string )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
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
