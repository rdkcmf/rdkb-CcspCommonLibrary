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

    module:	http_bmoreq_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Bmo Req
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMOREQ_INTERNAL_API_
#define  _HTTP_BMOREQ_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_ACCESS.C
***********************************************************/

ULONG
HttpBmoReqGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoReqIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetServerName
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpBmoReqGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoReqGetReqInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqGetMethod
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetMethodName
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqGetMajorVersion
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqGetMinorVersion
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoReqGetUri
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetReqInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    );

ANSC_STATUS
HttpBmoReqSetMethod
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       method
    );

ANSC_STATUS
HttpBmoReqSetMajorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

ANSC_STATUS
HttpBmoReqSetMinorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

ANSC_STATUS
HttpBmoReqSetUri
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUri
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_AUTH.C
***********************************************************/

ANSC_STATUS
HttpBmoReqGetAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetAuthUser
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqGetAuthScheme
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetAuthRealm
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetAuthDigestUri
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_HEADER.C
***********************************************************/

ANSC_STATUS
HttpBmoReqDelStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqParseStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqCopyStartLineFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_STATES.C
***********************************************************/

PUCHAR
HttpBmoReqGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
HttpBmoReqGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

PUCHAR
HttpBmoReqGetDaemonAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetDaemonAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
HttpBmoReqGetDaemonPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetDaemonPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ULONG
HttpBmoReqGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

ANSC_HANDLE
HttpBmoReqGetCgiIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoReqGetRcpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetRcpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpBmoReqGetFormContent
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqGenFormContent
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqGetMappedPath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetMappedPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

char*
HttpBmoReqGetOriginalPagePath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetOriginalPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

char*
HttpBmoReqGetFilteredPagePath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqSetFilteredPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_STATUS
HttpBmoReqReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_CGI.C
***********************************************************/

ANSC_STATUS
HttpBmoReqCgiGetEnvVariableByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       varName,
        char*                       varValue,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpBmoReqCgiGetEnvVariableById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCgiVarId,
        char*                       varValue,
        PULONG                      pulSize
    );

ULONG
HttpBmoReqCgiGetAuthType
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqCgiGetContentLength
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqCgiGetContentType
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    );

char*
HttpBmoReqCgiGetBoundaryDelimiter
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetDocumentRoot
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetPathInfo
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetPathTranslated
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetQueryString
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetFormContent
    (
        ANSC_HANDLE                 hThisObject
    );

PUCHAR
HttpBmoReqCgiGetRemoteAddr
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetRemoteHost
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetRemoteUser
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoReqCgiGetRequestMethod
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetScriptName
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetServerName
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpBmoReqCgiGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetServerProtocol
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetServerSoftware
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetServerType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoReqCgiGetHtmlFormInput
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetHtmlFormParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

void*
HttpBmoReqCgiGetHtmlFormParam2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_RCP.C
***********************************************************/

char*
HttpBmoReqRcpGetDocumentRoot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetPathInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetPathTranslated
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetRemoteUser
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetScriptName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetServerName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

USHORT
HttpBmoReqRcpGetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetServerProtocol
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
HttpBmoReqRcpGetServerSoftware
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );


#endif
