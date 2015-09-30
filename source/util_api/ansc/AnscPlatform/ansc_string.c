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

    module:	ansc_string.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to string operation.

        *   AnscEqualString1
        *   AnscEqualString2
        *   AnscFromHexToString
        *   AnscFromStringToHex
        *   AnscStringFromAToU
        *   AnscStringFromLToU
        *   AnscStringFromUToL
        *   AnscSizeOfToken
        *   AnscSizeOfToken2
        *   AnscSizeOfToken3
        *   AnscNumberOfCharsInString
        *   AnscNumberOfTokens
        *   AnscMoveToNextToken
        *   AnscMoveToNextToken2
        *   AnscMoveToPreviousToken
        *   AnscExtractToken
        *   AnscExtractToken2
        *   AnscConsumeToken
        *   AnscGetUlongString
        *   AnscGetStringUlong
        *   AnscGetStringUlongHex
        *   AnscCloneString
        *   AnscCreateScanner
        *   AnscRemoveScanner
        *   AnscScanString
        *   AnscIsValidIpString
        *   AnscIsValidIp6String
        *   AnscParseIp6Address
        *   AnscIp6AddressToString

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu 

    ---------------------------------------------------------------

    revision:

        04/24/2001    initial revision.
        08/09/2010    Bin added 2 ipv6 apis

**********************************************************************/


#include "ansc_global.h"


BOOL
AnscEqualString1
    (
        char*                       pString1,
        char*                       pString2,
        BOOL                        bCaseSensitive
    )
{
    ULONG                           ulStringSize1 = pString1? AnscSizeOfString(pString1) : 0;
    ULONG                           ulStringSize2 = pString2? AnscSizeOfString(pString2) : 0;
    ULONG                           i             = 0;

    if ( !pString1 && !pString2 )
    {
        return  TRUE;
    }
    else if ( !pString1 || !pString2 )
    {
        return  FALSE;
    }
    else if ( ulStringSize1 != ulStringSize2 )
    {
        return  FALSE;
    }

    for ( i = 0; i < (ULONG)ulStringSize1; i++ )
    {
        if ( pString1[i] != pString2[i] )
        {
            if ( bCaseSensitive )
            {
                return  FALSE;
            }

            if ( pString1[i] >= 'a' && pString1[i] <= 'z' )
            {
                if ( pString2[i] < 'A' || pString2[i] > 'Z' )
                {
                    return  FALSE;
                }
                else if ( (pString1[i] - 'a') != (pString2[i] - 'A') )
                {
                    return  FALSE;
                }
            }
            else if ( pString1[i] >= 'A' && pString1[i] <= 'Z' )
            {
                if ( pString2[i] < 'a' || pString2[i] > 'z' )
                {
                    return  FALSE;
                }
                else if ( (pString1[i] - 'A') != (pString2[i] - 'a') )
                {
                    return  FALSE;
                }
            }
            else
            {
                return  FALSE;
            }
        }
    }

    return  TRUE;
}


BOOL
AnscEqualString2
    (
        char*                       pString1,
        char*                       pString2,
        ULONG                       length,
        BOOL                        bCaseSensitive
    )
{
    ULONG                           i = 0;

    if ( !pString1 && !pString2 )
    {
        return  TRUE;
    }
    else if ( !pString1 || !pString2 )
    {
        return  FALSE;
    }

    for ( i = 0; i < length; i++ )
    {
        if ( pString1[i] != pString2[i] )
        {
            if ( bCaseSensitive )
            {
                return  FALSE;
            }

            if ( pString1[i] >= 'a' && pString1[i] <= 'z' )
            {
                if ( pString2[i] < 'A' || pString2[i] > 'Z' )
                {
                    return  FALSE;
                }
                else if ( (pString1[i] - 'a') != (pString2[i] - 'A') )
                {
                    return  FALSE;
                }
            }
            else if ( pString1[i] >= 'A' && pString1[i] <= 'Z' )
            {
                if ( pString2[i] < 'a' || pString2[i] > 'z' )
                {
                    return  FALSE;
                }
                else if ( (pString1[i] - 'A') != (pString2[i] - 'a') )
                {
                    return  FALSE;
                }
            }
            else
            {
                return  FALSE;
            }
        }
    }

    return  TRUE;
}


void
AnscFromHexToString
    (
        PUCHAR                      hex,
        char*                       string,
        ULONG                       ulSize,
        BOOL                        bLowerCase
    )
{
    ULONG                           i       = 0;
    char                            c_delta = bLowerCase? 'a' : 'A';

    for ( i = 0; i < ulSize; i++ )
    {
        string[i * 2    ] = (char)((hex[i] & 0xF0) >> 4);
        string[i * 2 + 1] = (char)( hex[i] & 0x0F);
    }

    for ( i = 0; i < ulSize * 2; i++ )
    {
        if ( string[i] <= (char)9 )
        {
            string[i] += '0';
        }
        else
        {
            string[i] -= (char)10;
            string[i] += (char)c_delta;
        }
    }
}


ULONG
AnscFromStringToHex
    (
        char*                       string,
        PUCHAR                      hex
    )
{
    PUCHAR                          hex_string   = hex;
    ULONG                           ulUcharCount = AnscSizeOfString(string) / 2;
    ULONG                           ulTmpValue   = 0;
    ULONG                           i            = 0;
    BOOL                            bZeroOmitted = ((AnscSizeOfString(string) % 2) != 0);
    char                            temp_char[3];

    if ( bZeroOmitted )
    {
        temp_char[0] = '0';
        temp_char[1] = *string; string++;
        temp_char[2] = 0;

        ulTmpValue = AnscGetStringUlongHex(temp_char);

        *hex_string = (UCHAR)ulTmpValue;
         hex_string++;
    }

    for ( i = 0; i < ulUcharCount; i++ )
    {
        temp_char[0] = *string; string++;
        temp_char[1] = *string; string++;
        temp_char[2] = 0;

        ulTmpValue = AnscGetStringUlongHex(temp_char);

        *hex_string = (UCHAR)ulTmpValue;
         hex_string++;
    }

    return  hex_string - hex;
}


void
AnscStringFromAToU
    (
        char*                       string,
        ULONG                       ulSize
    )
{
    int                             i = 0;

    /*
     * Caller is responsible for making sure that string buffer is big enough for holding double-
     * sized unicode string.
     */
    for ( i = (int)ulSize - 1; i >= 0; i-- )
    {
        string[i * 2 + 0] = string[i];
        string[i * 2 + 1] = 0;
    }

    return;
}


void
AnscStringFromLToU
    (
        char*                       lstring,
        char*                       ustring,
        ULONG                       ulSize
    )
{
    int                             delta = 'a' - 'A';
    int                             i     = 0;

    for ( i = 0; i < (int)ulSize; i++ )
    {
        ustring[i] = ( (lstring[i] >= 'a') && (lstring[i] <= 'z') )? (lstring[i] - (char)delta) : lstring[i];
    }

    return;
}


void
AnscStringFromUToL
    (
        char*                       ustring,
        char*                       lstring,
        ULONG                       ulSize
    )
{
    int                             delta = 'a' - 'A';
    int                             i     = 0;

    for ( i = 0; i < (int)ulSize; i++ )
    {
        lstring[i] = ( (ustring[i] >= 'A') && (ustring[i] <= 'Z') )? (ustring[i] + (char)delta) : ustring[i];
    }

    return;
}


ULONG
AnscSizeOfToken
    (
        char*                       string,
        char*                       separator,
        ULONG                       range
    )
{
    ULONG                           count = 0;

    /*
     * Caller is responsible for making sure that string begins with valid non-stop char.
     */
    for ( count = 0; count < range; count++ )
    {
        if ( AnscCharInString(separator, *string) )
        {
            return  count;
        }

        string++;
    }

    return  count;
}


ULONG
AnscSizeOfToken2
    (
        char*                       string,
        char*                       alphabet,
        ULONG                       range
    )
{
    ULONG                           count = 0;

    /*
     * Caller is responsible for making sure that string begins with valid non-stop char.
     */
    for ( count = 0; count < range; count++ )
    {
        if ( !AnscCharInString(alphabet, *string) )
        {
            return  count;
        }

        string++;
    }

    return  count;
}


ULONG
AnscSizeOfToken3
    (
        char*                       string,
        char*                       separator,
        ULONG                       range
    )
{
    ULONG                           count = 0;

    if ( *string != '"' )
    {
        return  AnscSizeOfToken(string, separator, range);
    }

    /*
     * Caller is responsible for making sure that string begins with valid non-stop char.
     */
    if ( (*string == '"') && (range > 0) )
    {
        string++;

        for ( count = 1; count < range; count++ )
        {
            if ( *string == '"' )
            {
                return  count + 1;  /* including the trailing '"' */
            }

            string++;
        }
    }
    else
    {
        for ( count = 0; count < range; count++ )
        {
            if ( AnscCharInString(separator, *string) )
            {
                return  count;
            }

            string++;
        }
    }

    return  count;
}


ULONG
AnscNumberOfCharsInString
    (
        char*                       string,
        char                        charToFind
    )
{
    ULONG                           ulNumberOfChars = AnscSizeOfString(string);
    ULONG                           i               = 0;
    ULONG                           count           = 0;

    for ( i = 0; i < ulNumberOfChars; i++ )
    {
        if ( string[i] == charToFind )
        {
            return  count;
        }
    }

    return  count;
}


ULONG
AnscNumberOfTokens
    (
        char*                       string,
        char*                       separator
    )
{
    ULONG                           ulSizeOfStr = AnscSizeOfString(string);
    ULONG                           count       = 0;
    ULONG                           ulTokenSize = 0;
    char*                           pTempChar   = string;

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || !ulTokenSize )
    {
        return  0;
    }

    while ( string && ulTokenSize && ((ULONG)(string - pTempChar) < ulSizeOfStr) )
    {
        count++;

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    }

    return  count;
}


char*
AnscMoveToNextToken
    (
        char*                       string,
        char*                       separtor
    )
{
    while ( AnscCharInString(separtor, *string) && (*string != 0) )
    {
        string++;
    }

    return  string;
}


char*
AnscMoveToNextToken2
    (
        char*                       string,
        char*                       alphabet
    )
{
    while ( !AnscCharInString(alphabet, *string) && (*string != 0) )
    {
        string++;
    }

    return  string;
}


char*
AnscMoveToPreviousToken
    (
        char*                       string,
        char*                       separator,
        PULONG                      pulTokenLength
    )
{
    ULONG                           count = 0;

    while ( AnscCharInString(separator, *string) )
    {
        string--;
    }

    while ( !AnscCharInString(separator, *string) )
    {
        string--;
        count++;
    }

    string++;

    *pulTokenLength = count;

    return  string;
}


ANSC_STATUS
AnscExtractToken
    (
        char*                       string,
        char*                       separator,
        ULONG                       index,
        char*                       output
    )
{
    ULONG                           ulSizeOfStr = AnscSizeOfString(string);
    ULONG                           ulSizeOfSep = AnscSizeOfString(separator);
    ULONG                           count       = 0;
    ULONG                           ulTokenSize = 0;

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || !ulTokenSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    while ( index && string && ulTokenSize )
    {
        index--;

        string     += ulTokenSize;
        string      = AnscMoveToNextToken(string, separator);
        ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));
    }

    if ( string && ulTokenSize )
    {
        AnscCopyMemory(output, string, ulTokenSize);

        output[ulTokenSize] = 0;
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscExtractToken2
    (
        char*                       string,
        char*                       alphabet,
        ULONG                       index,
        char*                       output
    )
{
    ULONG                           ulSizeOfStr = AnscSizeOfString(string);
    ULONG                           ulSizeOfSep = AnscSizeOfString(alphabet);
    ULONG                           count       = 0;
    ULONG                           ulTokenSize = 0;

    string      = AnscMoveToNextToken2(string, alphabet);
    ulTokenSize = AnscSizeOfToken2(string, alphabet, AnscSizeOfString(string));

    if ( !string || !ulTokenSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    while ( index && string && ulTokenSize )
    {
        index--;

        string     += ulTokenSize;
        string      = AnscMoveToNextToken2(string, alphabet);
        ulTokenSize = AnscSizeOfToken2(string, alphabet, AnscSizeOfString(string));
    }

    if ( string && ulTokenSize )
    {
        AnscCopyMemory(output, string, ulTokenSize);

        output[ulTokenSize] = 0;
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  ANSC_STATUS_SUCCESS;
}


void
AnscConsumeToken
    (
        char*                       string,
        char*                       separator
    )
{
    ULONG                           ulSizeOfStr = AnscSizeOfString(string);
    ULONG                           ulSizeOfSep = AnscSizeOfString(separator);
    ULONG                           ulTokenSize = 0;
    char*                           pTemp       = string;
    char                            tempChar[128];

    AnscZeroMemory(tempChar, 128);

    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || !ulTokenSize || ((ULONG)(string - pTemp) >= ulSizeOfStr) )
    {
        return;
    }

    string     += ulTokenSize;
    string      = AnscMoveToNextToken(string, separator);
    ulTokenSize = AnscSizeOfToken(string, separator, AnscSizeOfString(string));

    if ( !string || !ulTokenSize || ((ULONG)(string - pTemp) >= ulSizeOfStr) )
    {
        pTemp[0] = 0;
    }
    else
    {
        AnscCopyString(tempChar, string);
        AnscCopyString(pTemp, tempChar);

        pTemp[AnscSizeOfString(tempChar)] = 0;
    }

    return;
}


void
AnscGetUlongString
    (
        ULONG                       ulValue,
        char*                       ulong_str
    )
{
    ULONG                           remain_value1 = 0;
    ULONG                           remain_value2 = ulValue;
    ULONG                           char_count    = 0;
    ULONG                           char_index    = 0;
    char                            reverse_str[32];

    if ( ulValue == 0 )
    {
        ulong_str[0] = '0';

        return;
    }

    AnscZeroMemory(reverse_str, 32);

    while ( remain_value2 > 0 )
    {
        remain_value1 = remain_value2 % 10;
        remain_value2 = remain_value2 / 10;

        reverse_str[char_count++] = '0' + (char)remain_value1;
    }

    for ( char_index = 0; char_index < char_count; char_index++ )
    {
        ulong_str[char_index] = reverse_str[char_count - 1 - char_index];
    }

    return;
}


ULONG
AnscGetStringUlong
    (
        char*                       ulong_str
    )
{
    ULONG                           str_len       = AnscSizeOfString(ulong_str);
    ULONG                           total_value   = 0;
    ULONG                           char_index    = 0;
    ULONG                           decimal_times = 1;

    if ( str_len > 10 )
    {
        total_value = 0;
    }
    else
    {
        for ( char_index = 0; char_index < str_len; char_index++ )
        {
            total_value   += (ULONG)(ulong_str[str_len - char_index - 1] - '0') * decimal_times;
            decimal_times *= 10;
        }
    }

    return  total_value;
}


ULONG
AnscGetStringUlongHex
    (
        char*                       ulong_str_hex
    )
{
    ULONG                           str_len     = AnscSizeOfString(ulong_str_hex);
    ULONG                           total_value = 0;
    ULONG                           char_value  = 0;
    ULONG                           char_index  = 0;
    ULONG                           hex_times   = 1;

    if ( str_len > 10 )
    {
        total_value = 0;
    }
    else
    {
        for ( char_index = 0; char_index < str_len; char_index++ )
        {
            if ( (ulong_str_hex[str_len - char_index - 1] >= '0') &&
                 (ulong_str_hex[str_len - char_index - 1] <= '9') )
            {
                char_value = (ULONG)(ulong_str_hex[str_len - char_index - 1] - '0');
            }
            else if ( (ulong_str_hex[str_len - char_index - 1] >= 'a') &&
                      (ulong_str_hex[str_len - char_index - 1] <= 'f') )
            {
                char_value = (ULONG)(ulong_str_hex[str_len - char_index - 1] - 'a') + 10;
            }
            else if ( (ulong_str_hex[str_len - char_index - 1] >= 'A') &&
                      (ulong_str_hex[str_len - char_index - 1] <= 'F') )
            {
                char_value = (ULONG)(ulong_str_hex[str_len - char_index - 1] - 'A') + 10;
            }

            total_value += char_value * hex_times;
            hex_times   *= 16;
        }
    }

    return  total_value;
}

#ifdef _ANSC_WINDOWSNT
char*
AnscCloneString2
    (
        char*                       src_string,
        char*                       pFileName,
        ULONG                       ulLineNumber
    )
{
    char*                           dst_string = NULL;
	ULONG							len		   = 0;

    if ( !src_string )
    {
        return  NULL;
    }
    else
    {
		len = AnscSizeOfString(src_string);

        dst_string = (char*)AnscAllocateMemory5(AnscSizeOfString(src_string) + 1, pFileName, ulLineNumber);

        if ( !dst_string )
        {
            return  NULL;
        }
        else
        {
            AnscCopyString(dst_string, src_string);
        }
    }

    return  dst_string;
}

#else

char*
AnscCloneString
    (
        char*                       src_string


    )
{
    char*                           dst_string = NULL;


    if ( !src_string )
    {
        return  NULL;
    }
    else
    {
        dst_string = (char*)AnscAllocateMemory(AnscSizeOfString(src_string) + 1);



        if ( !dst_string )
        {
            return  NULL;
        }
        else
        {
            AnscCopyString(dst_string, src_string);
        }
    }

    return  dst_string;
}

#endif

ANSC_HANDLE
AnscCreateScanner
    (
        char**                      string_array,
        ULONG                       string_count
    )
{
    return  (ANSC_HANDLE)NULL;
}


ANSC_STATUS
AnscRemoveScanner
    (
        ANSC_HANDLE                 hStringScanner
    )
{
    return  ANSC_STATUS_SUCCESS;
}


char*
AnscScanString
    (
        ANSC_HANDLE                 hStringScanner,
        char*                       tbs_string
    )
{
    return  NULL;
}


BOOL
AnscIsValidIpString
    (
        char*                       ip_addr_string
    )
{
    PANSC_TOKEN_CHAIN               pTokenChain    = NULL;
    PANSC_STRING_TOKEN              pToken         = NULL;
    BOOL                            bValidIpString = FALSE;
    ULONG                           ulTokenCount   = 0;
    ANSC_IPV4_ADDRESS               ip4Addr;

    if ( !ip_addr_string )
    {
        return  FALSE;
    }
    else if ( (AnscSizeOfString(ip_addr_string) == 0 ) ||
              (AnscSizeOfString(ip_addr_string) >= 16) )
    {
        return  FALSE;
    }
    else
    {
        pTokenChain =
            (PANSC_TOKEN_CHAIN)AnscTcAllocate
                (
                    ip_addr_string,
                    "."
                );
    }

    if ( !pTokenChain )
    {
        return  FALSE;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) != 4 )
    {
        bValidIpString = FALSE;

        goto  EXIT1;
    }

    while ( pToken = (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)pTokenChain) )
    {
        if ( AnscSizeOfString(pToken->Name) > 3 )
        {
            AnscFreeMemory(pToken);

            bValidIpString = FALSE;

            goto  EXIT1;
        }
        else
        {
            AnscIsDigitString(pToken->Name, bValidIpString);
        }

        if ( !bValidIpString )
        {
            AnscFreeMemory(pToken);

            bValidIpString = FALSE;

            goto  EXIT1;
        }
        else if ( _ansc_atoi(pToken->Name) > 255 )
        {
            AnscFreeMemory(pToken);

            bValidIpString = FALSE;

            goto  EXIT1;
        }
        else
        {
            ip4Addr.Dot[ulTokenCount++] = _ansc_atoi(pToken->Name);
        }

        AnscFreeMemory(pToken);
    }

    bValidIpString = TRUE;


EXIT1:

    if ( pTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTokenChain);
    }

    return  bValidIpString;
}

BOOL
AnscIsValidIp6String
    (
        char*                       ip6_addr_string
    )
{
    unsigned char                   pResult[16];

    return AnscParseIp6Address(ip6_addr_string, pResult);
}

/**********************************************************************

    prototype:

        BOOL
        AnscParseIp6Address
            (
                char*                       ip6_addr,
                unsigned char*              ipAddrArray
            );

    description:

        This function converts ipv6 address to binary format.

    argument:   char*                       ip6_addr,
                ipv6 adress. It's supposed to be a valid RFC1884 address.
                itself.

                unsigned char*              ipAddrArray
                The output binary buffer;

    return:     TRUE when succeeds while FALSE when fails.

**********************************************************************/
#define ANSC_INT16_SIZE                     2

BOOL
AnscParseIp6Address
    (
        char*                       ip6_addr,
        unsigned char*              ipAddrArray
    )
{
    ANSC_IPV4_ADDRESS   ip4Addr;
    unsigned char       tmp[IPV6_ADDRESS_SIZE], *tp, *endp, *colonp;
    char*               src                     = ip6_addr;
    const char          xdigits[]               = "0123456789abcdef";
    char                tmpIp6[128]             = { 0 };
    char*               curtok;
    int                 ch, saw_xdigit, i , n;
    unsigned            val;
    char*               pch;

    if( AnscEqualString(ip6_addr, "::", FALSE))
    {
        AnscZeroMemory(ipAddrArray, IPV6_ADDRESS_SIZE);

        return TRUE;
    }

    AnscZeroMemory(tmp, IPV6_ADDRESS_SIZE);

    tp     = tmp;
    endp   = tp + IPV6_ADDRESS_SIZE;
    colonp = NULL;

    /* Leading :: requires some special handling. */
    if (src[0] == ':')
    {
        if (src[1] != ':')
        {
            return FALSE;
        }
    }

    if( _ansc_strstr(src, "::") == src)
    {
        /* we need to handle strings like "::1". an '0' is insert at front */
        tmpIp6[0] = '0';

        AnscCopyMemory( tmpIp6 + 1, ip6_addr, AnscSizeOfString(ip6_addr));

        src = tmpIp6;
    }

    curtok = src;
    saw_xdigit = 0;
    val = 0;
    while ((ch = tolower (*src++)) != '\0')
    {
        pch = _ansc_strchr (xdigits, ch);
        if (pch != NULL)
        {
            val <<= 4;
            val |= (pch - xdigits);
            if (val > 0xffff)   return FALSE;

            saw_xdigit = 1;
            continue;
        }

        if (ch == ':')
        {
          curtok = src;
          if (!saw_xdigit)
	      {
	           if (colonp)  return FALSE;

	          colonp = tp;
	          continue;
	      }
          else if (*src == '\0')
	      {
	          return FALSE;
	      }

          if (tp + ANSC_INT16_SIZE > endp)
          {
              return FALSE;
          }

          *tp++ = (u_char) (val >> 8) & 0xff;
          *tp++ = (u_char) val & 0xff;
          saw_xdigit = 0;
          val = 0;
          continue;
        }

        if (ch == '.' && ((tp + IPV4_ADDRESS_SIZE) <= endp))
        {   
            if(AnscIsValidIpString(curtok))
            {
                ip4Addr.Value = _ansc_inet_addr(curtok);

                AnscCopyMemory(tp, ip4Addr.Dot, IPV4_ADDRESS_SIZE);

                tp += IPV4_ADDRESS_SIZE;

                saw_xdigit = 0;
  
                break;
            }
        }
        
        return FALSE;
    }

    if (saw_xdigit)
    {
      if (tp + ANSC_INT16_SIZE > endp)
      {
         return FALSE;
      }

      *tp++ = (u_char) (val >> 8) & 0xff;
      *tp++ = (u_char) val & 0xff;
    }

    if (colonp != NULL)
    {
      /*
       * Shift
       */
      n = tp - colonp;

      if (tp == endp)  return FALSE;

      for (i = 1; i <= n; i++)
      {
          endp[-i] = colonp[n - i];
          colonp[n - i] = 0;
      }
      
      tp = endp;
    }

    if (tp != endp)  return FALSE;

    AnscCopyMemory(ipAddrArray, tmp, IPV6_ADDRESS_SIZE);
    
    return TRUE;
}

char*
AnscIp6AddressToString
    (
        unsigned char*              ipAddrArray
    )
{
    char*                           var_string   = NULL;
    char                            tmp[16]      = { 0 };
    int                             i            = 0;
    BOOL                            bNonZero     = FALSE;
    
    if( ipAddrArray == NULL)
    {
        return AnscCloneString("::");
    }
    
    for( i = 0; i < IPV6_ADDRESS_SIZE; i ++)
    {
        if( ipAddrArray[i] != 0x00)
        {
            bNonZero = TRUE;
            break;
        }
    }

    if( !bNonZero)
    {
        return AnscCloneString("::");
    }

    var_string = (char*)AnscAllocateMemory(128);

    if ( !var_string )
    {
        return  NULL;
    }

    for( i = 0; i < IPV6_ADDRESS_SIZE; i= i + 2)
    {

        if( i == 0)
        {
            if( ipAddrArray[i] == 0x00 && ipAddrArray[i + 1] == 0x00)
            {
                _ansc_sprintf(tmp, "0");
            }
            else if( ipAddrArray[i] == 0x00)
            { 
                _ansc_sprintf(tmp, "%02X",ipAddrArray[i + 1]);
            }
            else
            {
                _ansc_sprintf(tmp, "%02X%02X",ipAddrArray[i],ipAddrArray[i + 1]);
            }
        }
        else
        {
            if( ipAddrArray[i] == 0x00 && ipAddrArray[i + 1] == 0x00)
            {
                _ansc_sprintf(tmp, ":0");
            }
            else if( ipAddrArray[i] == 0x00)
            { 
                _ansc_sprintf(tmp, ":%02X",ipAddrArray[i + 1]);
            }
            else
            {
                _ansc_sprintf(tmp, ":%02X%02X",ipAddrArray[i],ipAddrArray[i + 1]);
            }
        }

        _ansc_strcat(var_string, tmp);
    }

    return var_string;
}
