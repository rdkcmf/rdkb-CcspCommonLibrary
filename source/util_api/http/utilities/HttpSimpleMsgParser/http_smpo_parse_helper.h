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

    module:	http_smpo_parse_helper.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the HTTP message parser
        which are called for generating and constructing HTTP 
        message.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/24/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/24/02

**********************************************************************/

#ifndef  _HTTP_SMPO_PARSE_HELPER
#define  _HTTP_SMPO_PARSE_HELPER


BOOL
HttpSmpoUtilParseRequestLineVersion
    (
        PUCHAR                      pVersion,
        ULONG                       ulVersionLen,
        PULONG                      pMajorVersion,
        PULONG                      pMinorVersion
    );

BOOL
HttpSmpoUtilParseRquestLineUri
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulBufLen
    );

void
HttpSmpoUtilCopyHeaderString
    (
        PUCHAR                      pString,
        ULONG                       ulStringLen,
        char                        *pBuf,
        ULONG                       ulBufSize
    );

void
HttpSmpoUtilInitializeUri
    (
        PHTTP_REQUEST_URI           pUri
    );

PUCHAR
HttpSmpoUtilLinearWhiteSpace
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseHttpDate
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseRfc1123Date
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseRfc850Date
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAsciiTimeDate
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

USHORT
HttpSmpoUtilGetDayOfWeek
    (
        PUCHAR                      pDayOfWeek,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilGetWeekDayName
    (
        USHORT                      usWkDay
    );

USHORT
HttpSmpoUtilGetMonth
    (
        PUCHAR                      pMonth,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilGetMonthName
    (
        USHORT                      usMonth
    );

PUCHAR
HttpSmpoUtilParseDate1
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilParseDate2
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilParseDate3
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilParseTime
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseMediaRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMediaRange,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

ULONG
HttpSmpoUtilGetMediaType
    (
        PUCHAR                      pType,
        ULONG                       ulTypeSize
    );

ULONG
HttpSmpoUtilGetMediaSubType
    (
        ANSC_HANDLE                 hHttpSmpo,
        ULONG                       ulMediaType,
        PUCHAR                      pSubType,
        ULONG                       ulSubTypeSize
    );

ULONG
HttpSmpoUtilGetQuality
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

ULONG
HttpSmpoUtilGetCharsetType
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

ULONG
HttpSmpoUtilGetEncodingType
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

ULONG
HttpSmpoUtilGetMethodId
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseBasicCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseDigestCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAbsoluteURI
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseRelativeURI
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseNetPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAbsPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseRelPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseMailBox
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMailBox,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAddrSpec
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMailBox,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseChallenge
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hChallenge,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

ULONG
HttpSmpoUtilGetTransferEncoding
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAuthority
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseAuthorityServer
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

BOOL
HttpSmpoUtilParseRegName
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

PUCHAR
HttpSmpoUtilGetNextCookieNameValuePair
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    );

#endif
