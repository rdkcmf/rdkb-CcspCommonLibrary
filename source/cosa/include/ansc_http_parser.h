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

    module:	ansc_http_parser.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the HTTP message parser mainly
        focusing on header parsing.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.
        03/23/09    lighter revision.

    ---------------------------------------------------------------

    last modified:

        03/23/09

**********************************************************************/

#ifndef  _ANSC_HTTP_PARSER_INTERFACE
#define  _ANSC_HTTP_PARSER_INTERFACE


#include "http_definitions.h"
#include "http_smpo_util_def.h"
#include "ansc_utilbox_def.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_SMPO_HEADER_TABLE_SIZE                32
#define  HTTP_SMPO_HEADER_MAX_LEN                   64
#define  HTTP_COOKIE_MAX_AGE_ABSENT                 (ULONG)-1

typedef
ANSC_HANDLE
(*PFN_SMPO_PARSE_HDR1)
    (
        void                        *pBuf,
        ULONG                       ulSize
    );


typedef  struct
_HTTP_SMPO_HEADER_XINFO1
{
    SINGLE_LINK_ENTRY               Linkage;
    UCHAR                           Name[HTTP_SMPO_HEADER_MAX_LEN];
    ULONG                           HeaderId;
    PFN_SMPO_PARSE_HDR1              xFunc;
}
HTTP_SMPO_HEADER_XINFO1, *PHTTP_SMPO_HEADER_XINFO1;

#define  ACCESS_HTTP_SMPO_HEADER_XINFO1_OBJECT(p)                \
         ACCESS_CONTAINER(p, HTTP_SMPO_HEADER_XINFO1, Linkage)   \

#define  AnscInt2String                             _ansc_ultoa

ANSC_STATUS
AnscHttpBuildRequestLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscHttpBuildStatusLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseHeader
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseRequestLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseStatusLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseChunkedLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseUrl
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscHttpParseQueryString
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    );


char*
AnscHttpGetHeaderParam
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        char*                       parameters,
        char*                       name
    );

ULONG
AnscHttpGetHeaderSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
AnscHttpGetStatusLineSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
AnscHttpGetRequestLineSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    );

ANSC_STATUS
AnscHttpBuildHeader
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );


ANSC_STATUS
AnscHttpRegisterAllHeadersXInfo
    (
        void
    );

ANSC_HANDLE
AnscHttpGetHeaderXInfo
    (
        PUCHAR                      pName,
        ULONG                       ulSize
    );

ULONG
AnscHttpGetHeaderCode
    (
        char*                       name
    );


PUCHAR
AnscMemorySearch
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize,
        PUCHAR                      pPattern,
        ULONG                       ulPatternLen,
        BOOL                        bCaseSensitive
    );


#endif

