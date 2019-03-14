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

    module:	http_bmoreq_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Bmo Request Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMOREQ_INTERFACE_
#define  _HTTP_BMOREQ_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"

#include "http_ifo_cgi.h"
#include "http_ifo_rcp.h"


/***********************************************************
   PLATFORM INDEPENDENT HTTP BMO REQUEST OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_BMO_REQ_RCP_IPAD_SIZE                 32
#define  HTTP_BMO_REQ_RCP_SPAD_SIZE                 128
#define  HTTP_BMO_REQ_RCP_MPAD_SIZE                 512
#define  HTTP_BMO_REQ_RCP_BPAD_SIZE                 1024

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPBMOREQ_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_GEN_CONTENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMOREQ_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPBMOREQ_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_HTTPBMOREQ_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  char*
(*PFN_HTTPBMOREQ_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  USHORT
(*PFN_HTTPBMOREQ_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ULONG
(*PFN_HTTPBMOREQ_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMOREQ_GET_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    );

typedef  ULONG
(*PFN_HTTPBMOREQ_GET_METHOD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPBMOREQ_GET_METHOD2)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_METHOD)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       method
    );

typedef  ULONG
(*PFN_HTTPBMOREQ_GET_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMOREQ_GET_URI)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_URI)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUri
    );

typedef  char*
(*PFN_HTTPBMOREQ_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREQ_GET_AINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPBMOREQ_GET_AVALS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPBMOREQ_GET_AVALU)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * HTTP messages consist of requests from client to server and responses from server to client.
 *
 *      HTTP-message = Request | Response ; HTTP/1.1 messages
 *
 * Request and Response messages use the generic message format of RFC 822 for transferring
 * entities (the payload of hte message). Both types of message consist of a start-line, zero or
 * more header fields (also known as "headers"), an empty line (i.e., a line with nothing preceding
 * the CRLF) indicating the end of the header fields, and possibly a message-body.
 *
 *      generic-message = start-line
 *                        *(message-header CRLF)
 *                        CRLF
 *                        [message-body]
 *      start-line      = Request-Line | Status-Line
 *
 * In the interest of robustness, servers SHOULD ignore any empty line(s) received where a Request-
 * Line is expected. In other words, if the server is reading the protocol stream at the begining
 * of a message and receives a CRLF first, it should ignore the CRLF.
 */
#ifdef _ANSC_HTTP_CGI_USED_

#define  HTTP_BMO_REQ_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    HTTP_BASIC_MESSAGE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               ClientAddr;                                             \
    USHORT                          ClientPort;                                             \
    ANSC_IPV4_ADDRESS               DaemonAddr;                                             \
    USHORT                          DaemonPort;                                             \
    ULONG                           ServerMode;                                             \
    ANSC_HANDLE                     hCgiIf;                                                 \
    ANSC_HANDLE                     hRcpIf;                                                 \
    ANSC_HANDLE                     hReqInfo;                                               \
    ANSC_HANDLE                     hFormContent;                                           \
    char*                           MappedPath;                                             \
    char*                           OriginalPagePath;                                       \
    char*                           FilteredPagePath;                                       \
                                                                                            \
    char                            RcpIpad[HTTP_BMO_REQ_RCP_IPAD_SIZE];                    \
    char                            RcpSpad[HTTP_BMO_REQ_RCP_SPAD_SIZE];                    \
    char                            RcpMpad[HTTP_BMO_REQ_RCP_MPAD_SIZE];                    \
    char                            RcpBpad[HTTP_BMO_REQ_RCP_BPAD_SIZE];                    \
    char*                           RcpDpad;                                                \
    ULONG                           RcpIpadSize;                                            \
    ULONG                           RcpSpadSize;                                            \
    ULONG                           RcpMpadSize;                                            \
    ULONG                           RcpBpadSize;                                            \
    ULONG                           RcpDpadSize;                                            \
                                                                                            \
    ANSC_HANDLE                     hAuthInfo;                                              \
                                                                                            \
    PFN_HTTPBMOREQ_GET_ADDR         GetClientAddr;                                          \
    PFN_HTTPBMOREQ_SET_ADDR         SetClientAddr;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetClientPort;                                          \
    PFN_HTTPBMOREQ_SET_PORT         SetClientPort;                                          \
    PFN_HTTPBMOREQ_GET_ADDR         GetDaemonAddr;                                          \
    PFN_HTTPBMOREQ_SET_ADDR         SetDaemonAddr;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetDaemonPort;                                          \
    PFN_HTTPBMOREQ_SET_PORT         SetDaemonPort;                                          \
    PFN_HTTPBMOREQ_GET_MODE         GetServerMode;                                          \
    PFN_HTTPBMOREQ_SET_MODE         SetServerMode;                                          \
    PFN_HTTPBMOREQ_GET_NAME         GetServerName;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetServerPort;                                          \
                                                                                            \
    PFN_HTTPBMOREQ_GET_INFO         GetReqInfo;                                             \
    PFN_HTTPBMOREQ_GET_METHOD       GetMethod;                                              \
    PFN_HTTPBMOREQ_GET_METHOD2      GetMethodName;                                          \
    PFN_HTTPBMOREQ_GET_VERSION      GetMajorVersion;                                        \
    PFN_HTTPBMOREQ_GET_VERSION      GetMinorVersion;                                        \
    PFN_HTTPBMOREQ_GET_URI          GetUri;                                                 \
                                                                                            \
    PFN_HTTPBMOREQ_SET_INFO         SetReqInfo;                                             \
    PFN_HTTPBMOREQ_SET_METHOD       SetMethod;                                              \
    PFN_HTTPBMOREQ_SET_VERSION      SetMajorVersion;                                        \
    PFN_HTTPBMOREQ_SET_VERSION      SetMinorVersion;                                        \
    PFN_HTTPBMOREQ_SET_URI          SetUri;                                                 \
                                                                                            \
    PFN_HTTPBMOREQ_GET_IF           GetRcpIf;                                               \
    PFN_HTTPBMOREQ_SET_IF           SetRcpIf;                                               \
    PFN_HTTPBMOREQ_GET_CONTEXT      GetFormContent;                                         \
    PFN_HTTPBMOREQ_GEN_CONTENT      GenFormContent;                                         \
    PFN_HTTPBMOREQ_GET_PATH         GetMappedPath;                                          \
    PFN_HTTPBMOREQ_SET_PATH         SetMappedPath;                                          \
    PFN_HTTPBMOREQ_GET_PATH         GetOriginalPagePath;                                    \
    PFN_HTTPBMOREQ_SET_PATH         SetOriginalPagePath;                                    \
    PFN_HTTPBMOREQ_GET_PATH         GetFilteredPagePath;                                    \
    PFN_HTTPBMOREQ_SET_PATH         SetFilteredPagePath;                                    \
                                                                                            \
    PFN_HTTPBMOREQ_GET_IF           GetCgiIf;                                               \
    PFN_HTTPCGIIF_GET_VAR1          CgiGetEnvVariableByName;                                \
    PFN_HTTPCGIIF_GET_VAR2          CgiGetEnvVariableById;                                  \
    PFN_HTTPCGIIF_GET_AUTH_TYPE     CgiGetAuthType;                                         \
    PFN_HTTPCGIIF_GET_LENGTH        CgiGetContentLength;                                    \
    PFN_HTTPCGIIF_GET_TYPE          CgiGetContentType;                                      \
    PFN_HTTPCGIIF_GET_BOUNDARY      CgiGetBoundaryDelimiter;                                \
    PFN_HTTPCGIIF_GET_ROOT          CgiGetDocumentRoot;                                     \
    PFN_HTTPCGIIF_GET_PATH_INFO     CgiGetPathInfo;                                         \
    PFN_HTTPCGIIF_GET_PATH_TRAN     CgiGetPathTranslated;                                   \
    PFN_HTTPCGIIF_GET_QUERY_STR     CgiGetQueryString;                                      \
    PFN_HTTPCGIIF_GET_FORM_CONT     CgiGetFormContent;                                      \
    PFN_HTTPCGIIF_GET_ADDR          CgiGetRemoteAddr;                                       \
    PFN_HTTPCGIIF_GET_HOST          CgiGetRemoteHost;                                       \
    PFN_HTTPCGIIF_GET_USER          CgiGetRemoteUser;                                       \
    PFN_HTTPCGIIF_GET_METHOD        CgiGetRequestMethod;                                    \
    PFN_HTTPCGIIF_GET_SCRIPT        CgiGetScriptName;                                       \
    PFN_HTTPCGIIF_GET_NAME          CgiGetServerName;                                       \
    PFN_HTTPCGIIF_GET_PORT          CgiGetServerPort;                                       \
    PFN_HTTPCGIIF_GET_PROTOCOL      CgiGetServerProtocol;                                   \
    PFN_HTTPCGIIF_GET_SOFTWARE      CgiGetServerSoftware;                                   \
    PFN_HTTPCGIIF_GET_TYPE2         CgiGetServerType;                                       \
    PFN_HTTPCGIIF_GET_INPUT         CgiGetHtmlFormInput;                                    \
    PFN_HTTPCGIIF_GET_PARAM         CgiGetHtmlFormParam;                                    \
    PFN_HTTPCGIIF_GET_PARAM2        CgiGetHtmlFormParam2;                                   \
                                                                                            \
    PFN_HTTPRCPIF_GET_ROOT          RcpGetDocumentRoot;                                     \
    PFN_HTTPRCPIF_GET_PATH_INFO     RcpGetPathInfo;                                         \
    PFN_HTTPRCPIF_GET_PATH_TRAN     RcpGetPathTranslated;                                   \
    PFN_HTTPRCPIF_GET_USER          RcpGetRemoteUser;                                       \
    PFN_HTTPRCPIF_GET_SCRIPT        RcpGetScriptName;                                       \
    PFN_HTTPRCPIF_GET_NAME          RcpGetServerName;                                       \
    PFN_HTTPRCPIF_GET_PORT          RcpGetServerPort;                                       \
    PFN_HTTPRCPIF_GET_PROTOCOL      RcpGetServerProtocol;                                   \
    PFN_HTTPRCPIF_GET_SOFTWARE      RcpGetServerSoftware;                                   \
                                                                                            \
    PFN_HTTPBMOREQ_GET_AINFO        GetAuthInfo;                                            \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthUser;                                            \
    PFN_HTTPBMOREQ_GET_AVALU        GetAuthScheme;                                          \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthRealm;                                           \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthDigestUri;                                       \
    /* end of object class content */                                                       \

#else

#define  HTTP_BMO_REQ_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    HTTP_BASIC_MESSAGE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               ClientAddr;                                             \
    USHORT                          ClientPort;                                             \
    ANSC_IPV4_ADDRESS               DaemonAddr;                                             \
    USHORT                          DaemonPort;                                             \
    ULONG                           ServerMode;                                             \
    ANSC_HANDLE                     hCgiIf;                                                 \
    ANSC_HANDLE                     hRcpIf;                                                 \
    ANSC_HANDLE                     hReqInfo;                                               \
    ANSC_HANDLE                     hFormContent;                                           \
    char*                           MappedPath;                                             \
    char*                           OriginalPagePath;                                       \
    char*                           FilteredPagePath;                                       \
                                                                                            \
    char                            RcpIpad[HTTP_BMO_REQ_RCP_IPAD_SIZE];                    \
    char                            RcpSpad[HTTP_BMO_REQ_RCP_SPAD_SIZE];                    \
    char                            RcpMpad[HTTP_BMO_REQ_RCP_MPAD_SIZE];                    \
    char                            RcpBpad[HTTP_BMO_REQ_RCP_BPAD_SIZE];                    \
    char*                           RcpDpad;                                                \
    ULONG                           RcpIpadSize;                                            \
    ULONG                           RcpSpadSize;                                            \
    ULONG                           RcpMpadSize;                                            \
    ULONG                           RcpBpadSize;                                            \
    ULONG                           RcpDpadSize;                                            \
                                                                                            \
    ANSC_HANDLE                     hAuthInfo;                                              \
                                                                                            \
    PFN_HTTPBMOREQ_GET_ADDR         GetClientAddr;                                          \
    PFN_HTTPBMOREQ_SET_ADDR         SetClientAddr;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetClientPort;                                          \
    PFN_HTTPBMOREQ_SET_PORT         SetClientPort;                                          \
    PFN_HTTPBMOREQ_GET_ADDR         GetDaemonAddr;                                          \
    PFN_HTTPBMOREQ_SET_ADDR         SetDaemonAddr;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetDaemonPort;                                          \
    PFN_HTTPBMOREQ_SET_PORT         SetDaemonPort;                                          \
    PFN_HTTPBMOREQ_GET_MODE         GetServerMode;                                          \
    PFN_HTTPBMOREQ_SET_MODE         SetServerMode;                                          \
    PFN_HTTPBMOREQ_GET_NAME         GetServerName;                                          \
    PFN_HTTPBMOREQ_GET_PORT         GetServerPort;                                          \
                                                                                            \
    PFN_HTTPBMOREQ_GET_INFO         GetReqInfo;                                             \
    PFN_HTTPBMOREQ_GET_METHOD       GetMethod;                                              \
    PFN_HTTPBMOREQ_GET_METHOD2      GetMethodName;                                          \
    PFN_HTTPBMOREQ_GET_VERSION      GetMajorVersion;                                        \
    PFN_HTTPBMOREQ_GET_VERSION      GetMinorVersion;                                        \
    PFN_HTTPBMOREQ_GET_URI          GetUri;                                                 \
                                                                                            \
    PFN_HTTPBMOREQ_SET_INFO         SetReqInfo;                                             \
    PFN_HTTPBMOREQ_SET_METHOD       SetMethod;                                              \
    PFN_HTTPBMOREQ_SET_VERSION      SetMajorVersion;                                        \
    PFN_HTTPBMOREQ_SET_VERSION      SetMinorVersion;                                        \
    PFN_HTTPBMOREQ_SET_URI          SetUri;                                                 \
                                                                                            \
    PFN_HTTPBMOREQ_GET_IF           GetRcpIf;                                               \
    PFN_HTTPBMOREQ_SET_IF           SetRcpIf;                                               \
                                                                                            \
    PFN_HTTPRCPIF_GET_ROOT          RcpGetDocumentRoot;                                     \
    PFN_HTTPRCPIF_GET_PATH_INFO     RcpGetPathInfo;                                         \
    PFN_HTTPRCPIF_GET_PATH_TRAN     RcpGetPathTranslated;                                   \
    PFN_HTTPRCPIF_GET_USER          RcpGetRemoteUser;                                       \
    PFN_HTTPRCPIF_GET_SCRIPT        RcpGetScriptName;                                       \
    PFN_HTTPRCPIF_GET_NAME          RcpGetServerName;                                       \
    PFN_HTTPRCPIF_GET_PORT          RcpGetServerPort;                                       \
    PFN_HTTPRCPIF_GET_PROTOCOL      RcpGetServerProtocol;                                   \
    PFN_HTTPRCPIF_GET_SOFTWARE      RcpGetServerSoftware;                                   \
                                                                                            \
    PFN_HTTPBMOREQ_GET_AINFO        GetAuthInfo;                                            \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthUser;                                            \
    PFN_HTTPBMOREQ_GET_AVALU        GetAuthScheme;                                          \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthRealm;                                           \
    PFN_HTTPBMOREQ_GET_AVALS        GetAuthDigestUri;                                       \
    /* end of object class content */                                                       \

#endif

typedef  struct
_HTTP_BMO_REQ_OBJECT
{
    HTTP_BMO_REQ_CLASS_CONTENT
}
HTTP_BMO_REQ_OBJECT,  *PHTTP_BMO_REQ_OBJECT;

#define  ACCESS_HTTP_BMO_REQ_OBJECT(p)              \
         ACCESS_CONTAINER(p, HTTP_BMO_REQ_OBJECT, Linkage)


#endif
