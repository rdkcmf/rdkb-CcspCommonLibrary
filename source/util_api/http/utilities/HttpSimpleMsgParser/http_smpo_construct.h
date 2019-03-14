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

    module:	http_smpo_construct.h

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

#ifndef  _HTTP_SMPO_CONSTRUCT
#define  _HTTP_SMPO_CONSTRUCT


typedef
ANSC_STATUS
(*PFN_HTTP_SMPO_BUILD_HEADER)
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef
ULONG
(*PFN_HTTP_SMPO_GET_HEADER_SIZE)
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );


ULONG
HttpSmpoUtilGetSizeRequestLine
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeStatusLine
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeHeader
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );


ANSC_STATUS
HttpSmpoUtilBuildRequestLine
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildStatusLine
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildHeader
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

/* get size */

ULONG
HttpSmpoUtilGetSizeAccept
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAcceptCharset
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAcceptEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAcceptLanguage
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAcceptRanges
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAge
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAllow
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeAuthorization
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeCacheControl
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeConnection
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentLanguage
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentLength
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentLocation
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentMD5
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeContentType
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeDate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeETag
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeExpect
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeExpires
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeFrom
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeHost
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeIfMatch
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeIfModifiedSince
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeIfNoneMatch
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeIfRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeLastModified
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeLocation
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeMaxForwards
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizePragma
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeProxyAuthorization
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeReferer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeTryAfter
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeServer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeTE
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeTrailer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeTransferEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeUpgrade
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeUserAgent
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeVary
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeVia
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeWarning
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeProxyConnection
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeCookie
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoUtilGetSizeSetCookie
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    );

/* construction */

ANSC_STATUS
HttpSmpoUtilBuildAccept
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAcceptCharset
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAcceptEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAcceptLanguage
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAcceptRanges
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAge
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAllow
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildAuthorization
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildCacheControl
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentLanguage
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentLength
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentLocation
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentMD5
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildContentType
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildDate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildETag
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildExpect
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildExpires
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildFrom
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildHost
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildIfMatch
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildIfModifiedSince
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildIfNoneMatch
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuild
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildIfRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildLastModified
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildLocation
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildMaxForwards
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildPragma
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildProxyAuthorization
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildReferer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildTryAfter
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildServer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildTE
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildTrailer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildTransferEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildUpgrade
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildUserAgent
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildVary
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildVia
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildWarning
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildProxyConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildSetCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoUtilBuildSetCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );



#endif

