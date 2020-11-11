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


#include "ansc_platform.h"
#include "ansc_string_util.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrBufGetCharNum
            (
                PUCHAR                      pString,
                ULONG                       ulStrLen,
                UCHAR                       uChar
            )

    description:

        This function is called to count the specified character
        in the given string.

    argument:   PUCHAR                      pString
                The string to be processed.

                ULONG                       ulStrLen
                The length of string.

                UCHAR                       uChar
                The character to be counted.

    return:     the number of characters in the string.

**********************************************************************/

ULONG
AnscStrBufGetCharNum
    (
        PUCHAR                      pString,
        ULONG                       ulStrLen,
        UCHAR                       uChar
    )
{
    ULONG                           i;
    ULONG                           ulCount = 0;
    PUCHAR                          pBuf;

    if (pString && ulStrLen != 0)
    {
        pBuf    = pString;
        for (i = 0; i < ulStrLen; i ++)
        {
            if (*pBuf == uChar)
                ulCount ++;

            pBuf ++;
        }
    }

    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrGetCharNum
            (
                PUCHAR                      pString,
                UCHAR                       uChar
            )

    description:

        This function is called to get specific character
        in given string.

    argument:   PUCHAR                      pString
                The string to be processed.

                UCHAR                       uChar
                The character to be counted.

    return:     the number of characters in the string.

**********************************************************************/

ULONG
AnscStrGetCharNum
    (
        PUCHAR                      pString,
        UCHAR                       uChar
    )
{
    ULONG                           ulCount = 0;

    if (pString)
    {
        ULONG                       ulStrLen;

        ulStrLen    = AnscSizeOfString((const char *)pString);

        ulCount     = AnscStrBufGetCharNum(pString, ulStrLen, uChar);
    }

    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscStrUpper
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to captilize the given string.

    argument:   PUCHAR                      pStr
                The string to be processed.

    return:     the uppercase string.

**********************************************************************/

PUCHAR
AnscStrUpper
    (
        PUCHAR                      pStr
    )
{
    ULONG                           i, ulStrLen;
    UCHAR                           ch;

    ulStrLen    = AnscSizeOfString((const char *)pStr);

    for (i = 0; i < ulStrLen; i ++)
    {
        ch  = pStr[i];

        if (ch >= 'a' && ch <= 'z')
            pStr[i] = ch - 'a' + 'A';
    }

    return pStr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrLower
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to convert all letters to
        lower case.

    argument:   PUCHAR                      pStr
                The string to be processed.

    return:     the string with all letters in lower case.

**********************************************************************/

PUCHAR
AnscStrLower
    (
        PUCHAR                      pStr
    )
{
    ULONG                           i, ulStrLen;
    UCHAR                           ch;

    ulStrLen    = AnscSizeOfString((const char *)pStr);

    for (i = 0; i < ulStrLen; i ++)
    {
        ch  = pStr[i];

        if (ch >= 'A' && ch <= 'Z')
            pStr[i] = ch - 'A' + 'a';
    }

    return pStr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDupString
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to duplicate the given string.

    argument:   PUCHAR                      pStr
                The string to be duplicated.

    return:     the copy of the given string.

**********************************************************************/

PUCHAR
AnscDupString
    (
        PUCHAR                      pStr
    )
{
    if (!pStr)
    {
        return NULL;
    }
    else
    {
        ULONG                       ulStrLen;
        PUCHAR                      pDupStr;

        ulStrLen    = AnscSizeOfString((const char *)pStr);

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen + 1);
        if (ulStrLen != 0)
            AnscCopyString((char *)pDupStr, (char *)pStr);
        else
            pDupStr[0]  = 0;

        return pDupStr;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        AnscMemUtilRealloc
            (
                PUCHAR                      pMem,
                ULONG                       ulSize,
                ULONG                       ulNewSize
            )

    description:

        This function is called to reallocate memory.

    argument:   PUCHAR                      pMem
                The memory needs to be reallocated.

                ULONG                       ulSize
                The old size of memory.

                ULONG                       ulNewSize
                The new memory size.

    return:     the new memory block.

**********************************************************************/

PVOID
AnscMemUtilRealloc
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        ULONG                       ulNewSize
    )
{
    PVOID                           pNewMem;
    ULONG                           ulCopySize;

    if (ulNewSize == 0)
    {
        if (pMem)
        {
            AnscFreeMemory(pMem);
        }

        return NULL;
    }

    pNewMem = AnscAllocateMemory(ulNewSize);

    if (ulSize == 0 || !pMem)
    {
        return pNewMem;
    }

    if (pNewMem)
    {
        /* copy content from pMem to pNewMem */
        ulCopySize  = (ulNewSize > ulSize)?ulSize:ulNewSize;

        AnscCopyMemory(pNewMem, pMem, ulCopySize);

        /* free old memory block */
        AnscFreeMemory(pMem);
    }

    return pNewMem;
}

BOOL
is_IpAddress
    (
        PUCHAR                      pString
    )
{
    struct sockaddr_in sa;
    if(inet_pton(AF_INET, (const char *)pString, &(sa.sin_addr))==1)
        return TRUE;
    else
        return FALSE;
}

BOOL
is_Ipv6_address
    (
        PUCHAR                      pString
    )
{
    struct sockaddr_in6 sa;
    if(inet_pton(AF_INET6, (const char *)pString, &(sa.sin6_addr))==1)
        return TRUE;
    else
        return FALSE;
}

BOOL
AnscValidStringCheck
    (
        PUCHAR                      pString
    )
{
    int i =0;
    
    /* check if pstring doesn't hold NULL or whitespaces */
    if((pString == NULL) || (*pString=='\0'))
    {
        return FALSE;
    }
    while(pString[i] != '\0')
    {
        if ((pString[i] == ' ') || (pString[i] == '<') || (pString[i] == '>') || (pString[i] == '&') || (pString[i] == '\'') || (pString[i] == '\"') || (pString[i] == '|'))
        {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
