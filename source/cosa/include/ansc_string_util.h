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

    module:	ansc_string_util.h

        For 
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for the
        functions which are called to convert string to number and
        vice versa.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/

#ifndef  _ANSC_STRING_CONVERT
#define  _ANSC_STRING_CONVERT

#include "ansc_memory_complement.h"

#define  ANSC_STRING_FORMAT                         _ansc_sprintf
#define  AnscStrStr                                 _ansc_strstr
#define  AnscStrChr                                 _ansc_strchr
#define  AnscStrSpn                                 _ansc_strspn
#define  AnscStrCSpn                                _ansc_strcspn

#define  AnscString2Int                             _ansc_atoi
#define  AnscString2Double                          _ansc_atof
#define  AnscInt2String                             _ansc_ultoa

#define  AnscStrEqualNoCase(s1, s2)                 AnscEqualString1(s1, s2, FALSE)


/*
 *  AnscStrGetCharNum counts the specified character in a string
 */
ULONG
AnscStrBufGetCharNum
    (
        PUCHAR                      pString,
        ULONG                       ulStrLen,
        UCHAR                       uChar
    );


ULONG
AnscStrGetCharNum
    (
        PUCHAR                      pString,
        UCHAR                       uChar
    );


PUCHAR
AnscStrUpper
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscStrLower
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscDupString
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscDupString2
    (
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        ULONG                       ulNumPadding
    );


PUCHAR
AnscDupIp4Addr
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscDupMacAddr
    (
        PUCHAR                      pStr
    );


void
AnscStr2Ip4Addr
    (
        PUCHAR                      pStr,
        PUCHAR                      pIp4Addr
    );


PVOID
AnscMemUtilRealloc
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        ULONG                       ulNewSize
    );


PUCHAR
AnscStringUtf16ToUtf8
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    );


ULONG
AnscUtf16CharToUcs4Code
    (
        BOOL                        bBigEndian,
        PUCHAR                      pChar,
        ULONG                       ulBufSize,
        PULONG                      pulCharSize
    );


ANSC_STATUS
AnscUcs4CodeToUtf8
    (
        ULONG                       ulUcs4Code,
        PUCHAR                      pBuf,
        PULONG                      pulLen
    );


ULONG
AnscStringUtf16ToUtf8Size
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    );

PUCHAR
AnscStringUcs2ToUtf8
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    );

ULONG
AnscStringUcs2ToUtf8Size
    (
        BOOL                        bBigEndian,
        PUCHAR                      pStr,
        ULONG                       ulStrLen
    );

ANSC_STATUS
AnscUtf8CharToUcs4Code
    (
        PUCHAR                      pChar,
        ULONG                       ulBufSize,
        PULONG                      pulUcs4Code,
        PULONG                      pulCharSize
    );


ANSC_STATUS
AnscUcs4CodeToUtf16
    (
        ULONG                       ulUcs4Code,
        BOOL                        bBigEndian,
        PUCHAR                      pBuf,
        PULONG                      pulLen
    );


PUCHAR
AnscStringUtf8ToUtf16
    (
        PUCHAR                      pStr,
        BOOL                        bBigEndian,
        PULONG                      pulUtf16StrSize
    );


ULONG
AnscStringUtf8T0Utf16Size
    (
        PUCHAR                      pStr
    );


#if ( defined(_ANSC_WINDOWSNT) ||  defined(_ANSC_WINDOWS9X) ) && !defined(_ANSC_KERNEL)

static __inline  int
AnscMbstoWcs
    (
        PVOID                       pMbStr,
        ULONG                       ulMbStrLen, /* in bytes */
        PVOID                       pWcBuf,
        ULONG                       ulWcBufSize,
        PBOOL                       pbLittleEndian
    )
{
    int                             nRet    = -1;

#ifdef   _ANSC_LITTLE_ENDIAN_
    *pbLittleEndian     = TRUE;
#else
    *pbLittleEndian     = FALSE;
#endif

    nRet = 
        MultiByteToWideChar
            (
                CP_ACP,
                0,
                (LPCSTR)pMbStr,
                ulMbStrLen,
                (LPWSTR)pWcBuf,
                ulWcBufSize
            );

    return nRet;
}

#elif defined(_ANSC_LINUX) && !defined(_ANSC_KERNEL)

#include <stddef.h>

static __inline  int
AnscMbstoWcs
    (
        PVOID                       pMbStr,
        ULONG                       ulMbStrLen, /* in bytes */
        PVOID                       pWcBuf,
        ULONG                       ulWcBufSize,
        PBOOL                       pbLittleEndian
    )
{
    int                             nRet    = -1;

#ifdef   _ANSC_LITTLE_ENDIAN_
    *pbLittleEndian     = TRUE;
#else
    *pbLittleEndian     = FALSE;
#endif

    nRet    = mbstowcs((wchar_t *)pWcBuf, (const char *)pMbStr, ulWcBufSize);

    return nRet;
}

#else   /* you may need to find out the solution on platforms other than Windows and Linux */

static __inline  int
AnscMbstoWcs
    (
        PVOID                       pMbStr,
        ULONG                       ulMbStrLen, /* in bytes */
        PVOID                       pWcBuf,
        ULONG                       ulWcBufSize,
        PBOOL                       pbLittleEndian
    )
{
    int                             nRet    = -1;

#ifdef   _ANSC_LITTLE_ENDIAN_
    *pbLittleEndian     = TRUE;
#else
    *pbLittleEndian     = FALSE;
#endif

    return nRet;
}


#endif


#endif

