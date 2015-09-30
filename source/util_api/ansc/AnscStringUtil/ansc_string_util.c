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

        ulStrLen    = AnscSizeOfString(pString);

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

    ulStrLen    = AnscSizeOfString(pStr);

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

    ulStrLen    = AnscSizeOfString(pStr);

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

        ulStrLen    = AnscSizeOfString(pStr);

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen + 1);
        if (ulStrLen != 0)
            AnscCopyString(pDupStr, pStr);
        else
            pDupStr[0]  = 0;

        return pDupStr;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscDupString2
            (
                PUCHAR                      pStr,
                ULONG                       ulStrLen,
                ULONG                       ulNumPadding
            );

    description:

        This function is called to duplicate the given string.

    argument:   PUCHAR                      pStr
                The string to be duplicated.

                ULONG                       ulStrLen
                Length to string to be copied.

                ULONG                       ulNumPadding
                The number of padding in bytes at the end of
                the new string and must be zeroed out.

    return:     the copy of the given string.

**********************************************************************/

PUCHAR
AnscDupString2
    (
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        ULONG                       ulNumPadding
    )
{
    if (!pStr)
    {
        return NULL;
    }
    else
    {
        PUCHAR                      pDupStr;
        ULONG                       i;

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen + 1 + ulNumPadding);

        if (ulStrLen != 0)
        {
            AnscCopyMemory(pDupStr, pStr, ulStrLen);
        }
        else
        {
            pDupStr[0]  = 0;
        }

        for (i = 0; i <= ulNumPadding; i ++)
        {
            pDupStr[i + ulStrLen]    = 0;
        }

        return pDupStr;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDupIp4Addr
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to duplicate a string that
        represent an IP address.

    argument:   PUCHAR                      pStr
                The IP address string to be duplicated.

    return:     the copy of IP address string.

**********************************************************************/

PUCHAR
AnscDupIp4Addr
    (
        PUCHAR                      pStr
    )
{
    /* this function generate string format of a IP4 address */
    if (!pStr)
    {
        return NULL;
    }
    else
    {
        ULONG                       ulStrLen;
        PUCHAR                      pDupStr;

        /* xxx.xxx.xxx.xxx */
        ulStrLen    = 15;

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen + 1);

        if (pDupStr)
        {
            _ansc_sprintf(pDupStr, "%d.%d.%d.%d", pStr[0], pStr[1], pStr[2], pStr[3]);
        }

        return pDupStr;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDupMacAddr
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to duplicate MAC address string.

    argument:   PUCHAR                      pStr
                The MAC address string.

    return:     the copy MAC address string.

**********************************************************************/

PUCHAR
AnscDupMacAddr
    (
        PUCHAR                      pStr
    )
{
    /* this function generate string format of a IP4 address */
    if (!pStr)
    {
        return NULL;
    }
    else
    {
        ULONG                       ulStrLen;
        PUCHAR                      pDupStr;

        /* xx-xx-xx-xx-xx-xx */
        ulStrLen    = 3 * ANSC_MAX_MAC_ADDRESS_SIZE;

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen);

        if (pDupStr)
        {
            _ansc_sprintf
                (
                    pDupStr, 
                    "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", 
                    pStr[0], 
                    pStr[1], 
                    pStr[2], 
                    pStr[3],
                    pStr[4],
                    pStr[5]
                );
        }

        return pDupStr;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStr2Ip4Addr
            (
                PUCHAR                      pStr,
                PUCHAR                      pIp4Addr
            )

    description:

        This function is called to convert a string to IP address.

    argument:   PUCHAR                      pStr
                The string to be processed.

                PUCHAR                      pIp4Addr
                The buffer for IP address.

    return:     void.

**********************************************************************/

void
AnscStr2Ip4Addr
    (
        PUCHAR                      pStr,
        PUCHAR                      pIp4Addr
    )
{
    if (pStr && pIp4Addr)
    {
        /* we don't valid the format of string format IP4 address */

        *(PULONG)pIp4Addr   = _ansc_inet_addr(pStr);
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


/* 
 * This API converts UTF-16 string to UTF-8 string.
 */

PUCHAR
AnscStringUtf16ToUtf8
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    )
{
    ULONG                           ulBufSize   = 0;
    PUCHAR                          pBuf        = NULL;
    ULONG                           ulCharSize  = 0;
    ULONG                           ulUcs4Code  = 0;
    PUCHAR                          pLast       = pStr + ulStrLen - 1;
    ULONG                           ulSize      = 0;

    ulBufSize   = AnscStringUtf16ToUtf8Size(bBigEndian, pStr, ulStrLen);

    if (ulBufSize == 0)
    {
        return NULL;
    }

    pBuf    = (PUCHAR)AnscAllocateMemory(ulBufSize + 1);

    while (pStr < pLast)
    {
        ulUcs4Code  = AnscUtf16CharToUcs4Code(bBigEndian, pStr, pLast - pStr + 1, &ulCharSize);

        if (ulCharSize == 0)
        {
            /* invalid character encountered */
            break;
        }

        AnscUcs4CodeToUtf8(ulUcs4Code, pBuf + ulSize, &ulSize);

        pStr    += ulCharSize;
    }

    pBuf[ulBufSize] = 0;

    return pBuf;
}


/* 
 * This API converts UTF-16 char to UCS-4 char code.
 */

ULONG
AnscUtf16CharToUcs4Code
    (
        BOOL                        bBigEndian,
        PUCHAR                      pChar,
        ULONG                       ulBufSize,
        PULONG                      pulCharSize
    )
{
    ULONG                           ulUcs4Code  = 0;
    ULONG                           ulW1, ulW2;

    if (pulCharSize)
    {
        *pulCharSize    = 0;
    }

    if (ulBufSize < 2)
    {
        return 0;
    }

#define  _UTF16LE_GET_16BIT(pChar)                  ((*(pChar)) + ((*(pChar + 1)) << 8))
#define  _UTF16BE_GET_16BIT(pChar)                  (((*(pChar)) << 8) + (*(pChar + 1)))

    if (bBigEndian)
    {
        ulW1    = _UTF16BE_GET_16BIT(pChar);
    }
    else
    {
        ulW1    = _UTF16LE_GET_16BIT(pChar);
    }

    if (ulW1 < 0xD800 || ulW1 > 0xDFFF)
    {
        if (pulCharSize)
        {
            *pulCharSize    = 2;
        }

        return ulW1;
    }

    if (ulW1 > 0xDBFF)
    {
        return 0;
    }

    if (ulBufSize < 4)
    {
        return 0;
    }

    if (bBigEndian)
    {
        ulW2    = _UTF16BE_GET_16BIT(pChar + 2);
    }
    else
    {
        ulW2    = _UTF16LE_GET_16BIT(pChar + 2);
    }

    if (ulW2 < 0xDC00 || ulW2 > 0xDFFF)
    {
        return 0;
    }

    /* ulUcs4Code  = ((ulW1 & 0x000003FF) << 10) + ((ulW2 & 0x000003FF)); */
    ulW1 &= 0x000003FF;
    ulW2 &= 0x000003FF;

    ulUcs4Code  = (ulW1 << 10) + ulW2;

#undef   _UTF16LE_GET_16BIT
#undef   _UTF16BE_GET_16BIT

    if (pulCharSize)
    {
        *pulCharSize    = 4;
    }

    return ulUcs4Code;
}


/* 
 * This API convert Ucs4 code into UTF-8 char and put it into given buffer.
 */

const static UCHAR                  s_Utf8BitMasks[6]   = 
{
    0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};

ANSC_STATUS
AnscUcs4CodeToUtf8
    (
        ULONG                       ulUcs4Code,
        PUCHAR                      pBuf,
        PULONG                      pulLen
    )
{
    UCHAR                           Utf8Code[6];
    ULONG                           ulSize  = 0;
    ULONG                           i;
    UCHAR                           ucBits;
    ULONG                           ulValue;

    if (ulUcs4Code <= 0x7F)
    {
        ulSize  = 1;
    }
    else if (ulUcs4Code <= 0x7FF)
    {
        ulSize  = 2;
    }
    else if (ulUcs4Code <= 0xFFFF)
    {
        ulSize  = 3;
    }
    else if (ulUcs4Code <= 0x1FFFFF)
    {
        ulSize  = 4;
    }
    else if (ulUcs4Code <= 0x3FFFFFF)
    {
        ulSize  = 5;
    }
    else if (ulUcs4Code <= 0x7FFFFFFF)
    {
        ulSize  = 6;
    }
    else
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    ulValue = ulUcs4Code;

    for (i = 0;i < ulSize - 1; i ++)
    {
        ucBits  = (UCHAR)ulValue;
        Utf8Code[ulSize - 1 - i]    = (ucBits & 0x3F) | 0x80;
        ulValue >>= 6;
    }

    Utf8Code[0] = ((UCHAR)ulValue) | s_Utf8BitMasks[ulSize - 1];

    AnscCopyMemory(pBuf, Utf8Code, ulSize);

    (*pulLen) += ulSize;

    return ANSC_STATUS_SUCCESS;
}


/* 
 * This API is called to determine the UTF-8 string size after
 * converting the given UTF-16 string.
 */

ULONG
AnscStringUtf16ToUtf8Size
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    )
{
    ULONG                           ulCharSize  = 0;
    ULONG                           ulUcs4Code  = 0;
    PUCHAR                          pLast       = pStr + ulStrLen - 1;
    ULONG                           ulSize      = 0;

    if (!pStr || ulStrLen == 0)
    {
        return 0;
    }

    while (pStr < pLast)
    {
        ulUcs4Code  = AnscUtf16CharToUcs4Code(bBigEndian, pStr, pLast - pStr + 1, &ulCharSize);

        if (ulCharSize == 0)
        {
            /* invalid character encountered */
            break;
        }

        if (ulUcs4Code <= 0x7F)
        {
            ulSize  += 1;
        }
        else if (ulUcs4Code <= 0x7FF)
        {
            ulSize  += 2;
        }
        else if (ulUcs4Code <= 0xFFFF)
        {
            ulSize  += 3;
        }
        else if (ulUcs4Code <= 0x1FFFFF)
        {
            ulSize  += 4;
        }
        else if (ulUcs4Code <= 0x3FFFFFF)
        {
            ulSize  += 5;
        }
        else if (ulUcs4Code <= 0x7FFFFFFF)
        {
            ulSize  += 6;
        }

        pStr    += ulCharSize;
    }

    return ulSize;
}


/* 
 * This API converts UCS-2 (unicode) string to UTF-8 string.
 */

PUCHAR
AnscStringUcs2ToUtf8
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    )
{
    ULONG                           ulBufSize   = 0;
    PUCHAR                          pBuf        = NULL;
    ULONG                           ulCharSize  = 0;
    ULONG                           ulUcs4Code  = 0;
    PUCHAR                          pLast       = pStr + ulStrLen - 1;
    ULONG                           ulSize      = 0;

    ulBufSize   = AnscStringUcs2ToUtf8Size(bBigEndian, pStr, ulStrLen);

    if (ulBufSize == 0)
    {
        return NULL;
    }

    pBuf    = (PUCHAR)AnscAllocateMemory(ulBufSize + 1);

    while (pStr < pLast)
    {
        if (bBigEndian)
        {
            ulUcs4Code  = ((*pStr) << 8) + *(pStr+1);
        }
        else
        {
            ulUcs4Code  = ((*(pStr+1)) << 8) + *(pStr);
        }

        AnscUcs4CodeToUtf8(ulUcs4Code, pBuf + ulSize, &ulSize);

        pStr    += 2;
    }

    pBuf[ulBufSize] = 0;

    return pBuf;
}


/* 
 * This API is called to determine the UTF-8 string size after
 * converting the given UCS-2 (unicode) string.
 */

ULONG
AnscStringUcs2ToUtf8Size
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    )
{
    PUCHAR                          pLast       = pStr + ulStrLen - 1;
    USHORT                          usUcs2Code  = 0;
    ULONG                           ulSize      = 0;

    if (!pStr || ulStrLen == 0 || (ulStrLen % 2) != 0)
    {
        return 0;
    }

    while (pStr < pLast)
    {
        if (bBigEndian)
        {
            usUcs2Code  = ((*pStr) << 8) + *(pStr+1);
        }
        else
        {
            usUcs2Code  = (*(pStr+1) << 8) + *(pStr);
        }

        if (usUcs2Code <= 0x7F)
        {
            ulSize  += 1;
        }
        else if (usUcs2Code <= 0x7FF)
        {
            ulSize  += 2;
        }
        else if (usUcs2Code <= 0xFFFF)
        {
            ulSize  += 3;
        }

        pStr    += 2;
    }

    return ulSize;
}


ANSC_STATUS
AnscUtf8CharToUcs4Code
    (
        PUCHAR                      pChar,
        ULONG                       ulBufSize,
        PULONG                      pulUcs4Code,
        PULONG                      pulCharSize
    )
{
    ULONG                           ulUcs4Code  = 0;

    if ( *pChar <= 0x7F )
    {
        ulUcs4Code      = *pChar;
        *pulCharSize    = 1;
    }
    else
    {
        UCHAR                       ucFirst     = (*pChar) >> 1;
        ULONG                       ulMask      = 0xFC >> 1;
        ULONG                       i           = 5;
        ULONG                       ulLen       = 6;
        ULONG                       ulBitMask   = 0x01;

        while ( ucFirst != ulMask && i > 0 )
        {
            ucFirst >>= 1;
            ulMask >>= 1;
            ulMask &= 0xFE;
            ulBitMask <<= 1;
            ulBitMask |= 0x01;

            i --;
        }

        if ( i == 0 || i + 1 > ulBufSize )
        {
            return ANSC_STATUS_BAD_PARAMETER;
        }

        ulLen = i + 1;

        ulUcs4Code = ( (*pChar) & ulBitMask );

        for ( i = 0; i < ulLen - 1; i ++ )
        {
            ulUcs4Code <<= 6;
            ulUcs4Code += (*(pChar+i+1) & 0x3F);
        }

        *pulCharSize = ulLen;
    }

    *pulUcs4Code    = ulUcs4Code;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscUcs4CodeToUtf16
    (
        ULONG                       ulUcs4Code,
        BOOL                        bBigEndian,
        PUCHAR                      pBuf,
        PULONG                      pulLen
    )
{
    ANSC_STATUS                     status = ANSC_STATUS_SUCCESS;

    if ( ulUcs4Code <0x10000 )
    {
        UCHAR                       ucHigh, ucLow;

        ucHigh = (UCHAR)(( ulUcs4Code & 0xFF00 ) >> 8);
        ucLow  = (UCHAR)( ulUcs4Code & 0x00FF );

        *pBuf       = bBigEndian ? ucHigh : ucLow;
        *(pBuf+1)   = bBigEndian ? ucLow : ucHigh;

        *pulLen     = 2;
    }
    else
    {
        UCHAR                       uc1, uc2, uc3, uc4;

        ulUcs4Code -= 0x10000;

        uc1 = 0xd8 | ((UCHAR)(ulUcs4Code >> 18));
        uc2 = (UCHAR)((ulUcs4Code >> 10) & 0xFF);
        uc3 = 0xdc | ((UCHAR)((ulUcs4Code >> 8) & 0x03));
        uc4 = (UCHAR)(ulUcs4Code & 0xFF);

        if ( bBigEndian )
        {
            *(pBuf)     = uc1;
            *(pBuf + 1) = uc2;
            *(pBuf + 2) = uc3;
            *(pBuf + 3) = uc4;
        }
        else
        {
            *(pBuf)     = uc4;
            *(pBuf + 1) = uc3;
            *(pBuf + 2) = uc2;
            *(pBuf + 3) = uc1;
        }

        *pulLen     = 4;
    }

    return status;
}


PUCHAR
AnscStringUtf8ToUtf16
    (
        PUCHAR                      pStr,
        BOOL                        bBigEndian,
        PULONG                      pulUtf16StrSize
    )
{
    ANSC_STATUS                     status          = 0;
    ULONG                           ulUcs4Code      = 0;
    ULONG                           ulUtf16Size     = 0;
    ULONG                           ulLen           = AnscSizeOfString(pStr);
    PUCHAR                          pUtf16Str       = NULL;
    ULONG                           ulOffset        = 0, ulInc;
    ULONG                           ulUtf16Offset   = 0;

    ulUtf16Size = AnscStringUtf8T0Utf16Size(pStr);

    if ( ulUtf16Size == 0 )
    {
        return NULL;
    }

    pUtf16Str   = (PUCHAR)AnscAllocateMemory(ulUtf16Size + 2);

    if ( pUtf16Str )
    {
        while ( ulOffset < ulLen )
        {
            status = AnscUtf8CharToUcs4Code(pStr + ulOffset, ulLen - ulOffset, &ulUcs4Code, &ulInc);
            if ( status != ANSC_STATUS_SUCCESS )
            {
                break;
            }

            ulOffset += ulInc;

            status = AnscUcs4CodeToUtf16(ulUcs4Code, bBigEndian, pUtf16Str + ulUtf16Offset, &ulInc);
            if ( status != ANSC_STATUS_SUCCESS )
            {
                break;
            }
            ulUtf16Offset += ulInc;
        }
    }

    if ( status != ANSC_STATUS_SUCCESS )
    {
        AnscFreeMemory(pUtf16Str);
        pUtf16Str = NULL;
    }

    if ( pUtf16Str )
    {
        *pulUtf16StrSize = ulUtf16Size + 2;
        pUtf16Str[ulUtf16Size]      = 0;
        pUtf16Str[ulUtf16Size + 1]  = 0;
    }

    return pUtf16Str;
}


ULONG
AnscStringUtf8T0Utf16Size
    (
        PUCHAR                      pStr
    )
{
    ANSC_STATUS                     status      = 0;
    ULONG                           ulUcs4Code;
    ULONG                           ulUtf16Size = 0;
    ULONG                           ulLen   = AnscSizeOfString(pStr);
    ULONG                           i       = 0;
    ULONG                           ulInc;
    UCHAR                           utf16Buf[4];

    while ( i < ulLen )
    {
        status = AnscUtf8CharToUcs4Code(pStr + i, ulLen - i, &ulUcs4Code, &ulInc);

        if ( status != ANSC_STATUS_SUCCESS )
        {
            return 0;
        }

        i += ulInc;

        status      = AnscUcs4CodeToUtf16(ulUcs4Code, TRUE, utf16Buf, &ulInc);
        if ( status != ANSC_STATUS_SUCCESS )
        {
            return 0;
        }
        ulUtf16Size += ulInc;
    }

    return ulUtf16Size;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        DslhTcpcrhoMemSearch
            (
                PUCHAR                      pBuf,
                ULONG                       ulSize,
                PUCHAR                      pPattern,
                ULONG                       ulPatternLen,
                BOOL                        bCaseSensitive
            );

    description:

        This function is called to find specific 'string'
        pattern in given memory buffer.

    argument:   PUCHAR                      pBuf
                Memory to be searched.

                ULONG                       ulSize
                The size of memory buffer.

                PUCHAR                      pPattern
                The search pattern.

                ULONG                       ulPatternLen
                Size of pattern string.

                BOOL                        bCaseSensitive
                Sensitiveness of string comparison.

    return:     the number of characters in the string.

**********************************************************************/

PUCHAR
AnscMemorySearch
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize,
        PUCHAR                      pPattern,
        ULONG                       ulPatternLen,
        BOOL                        bCaseSensitive
    )
{
    PUCHAR                          pBufEnd     = pBuf + ulSize - 1;
    PUCHAR                          pNext, pNext1;
    UCHAR                           fc          = *pPattern;
    UCHAR                           fc1         = 0;

    if ( fc >= 'a' && fc <= 'z')
    {
        fc1 = fc - 'a' + 'A';
    }
    else if ( fc >= 'A' && fc <= 'Z' )
    {
        fc1 = fc - 'A' + 'a';
    }

    pNext1 = NULL;

    while ( pBuf && pBuf <= pBufEnd )
    {
        pNext = _ansc_memchr(pBuf, fc, pBufEnd - pBuf + 1);
        if ( fc1 != 0 )
        {
            pNext1 = _ansc_memchr(pBuf, fc, pBufEnd - pBuf + 1);
        }

        if ( !pNext )
        {
            pNext = pNext1;
        }
        else if ( pNext1 && pNext1 < pNext )
        {
            pNext = pNext1;
        }

        if ( !pNext )
        {
            return  NULL;
        }

        if ( pBufEnd - pNext + 1 >= ulPatternLen &&
             AnscEqualString2(pNext, pPattern, ulPatternLen, bCaseSensitive) )
        {
            return  pNext;
        }

        pBuf = pNext + 1;
    }

    return  NULL;
}


