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

    module:	http_smpo_parse.h

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

#ifndef  _HTTP_SMPO_PARSE_API
#define  _HTTP_SMPO_PARSE_API


/* --------------------------------------------------------------------
 *                      Parsing APIs
 * ----------------------------------------------------------------- */

ANSC_HANDLE
HttpSmpoUtilParseRequestLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoUtilParseStatusLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoUtilParseHeader
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoUtilParseChunkedLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    );


/* all headers' parsing function */
ANSC_HANDLE
HttpSmpoParseAccept
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAcceptCharset
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAcceptEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAcceptLanguage
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAcceptRanges
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAge
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAllow
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseAuthorization
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseCacheControl
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseConnection
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentLanguage
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentLength
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentLocation
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentMD5
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseContentType
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseDate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseETag
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseExpect
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseExpires
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseFrom
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseHost
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseIfMatch
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseIfModifiedSince
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseIfNoneMatch
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseIfRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseLastModified
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseLocation
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseMaxForwards
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParsePragma
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseProxyAuthorization
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseReferer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseRetryAfter
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseServer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseTE
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseTrailer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseTransferEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseUpgrade
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseUserAgent
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseVary
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseVia
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseWarning
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseWwwAuthenticate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseProxyConnection
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseCookie
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseSetCookie
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseCookie2
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseSetCookie2
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );


#endif
