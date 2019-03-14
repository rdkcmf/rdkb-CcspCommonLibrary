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

    module:	http_smpo_internal_api.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the HTTP message parser.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/22/02

**********************************************************************/

#ifndef  _HTTP_SMPO_INTERNAL_API_
#define  _HTTP_SMPO_INTERNAL_API_


ANSC_HANDLE
HttpSmpoParseHeader
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseRequestLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseStatusLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseChunkedLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseUrl
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoParseUrlList
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize,
        PULONG                      pulUrlCount,
        PANSC_HANDLE                phUrlArray
    );

ANSC_HANDLE
HttpSmpoParseQueryString
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoParseMultipart
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        char*                       pBoundary
    );

ANSC_HANDLE
HttpSmpoParseHtmlFormInput
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ULONG                       ulEncType,
        char*                       pBoundary
    );

ANSC_STATUS
HttpSmpoSaveFormPartAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        ULONG                       ulEncType,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        BOOL                        bFlushBody
    );

char*
HttpSmpoGetFormDataNoFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        PULONG                      pulFormLen
    );

char*
HttpSmpoGetHeaderParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       parameters,
        char*                       name
    );

ULONG
HttpSmpoGetHeaderSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoGetRequestLineSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    );

ULONG
HttpSmpoGetStatusLineSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    );

ANSC_STATUS
HttpSmpoBuildHeader
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoBuildRequestLine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpSmpoBuildStatusLine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_HANDLE
HttpSmpoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSmpoRegisterHeaderXInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulHeaderId,
        PFN_SMPO_PARSE_HDR          xFunc
    );

ANSC_STATUS
HttpSmpoRemoveXTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSmpoRegisterAllHeadersXInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSmpoGetHeaderXInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulSize
    );


#endif
