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

    module:	http_ifo_cgi.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Common Gateway Interface Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_CGI_
#define  _HTTP_IFO_CGI_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  HTTP COMMON GATEWAY INTERFACE INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTPCGIIF_GET_VAR1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       varName,
        char*                       varValue,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPCGIIF_GET_VAR2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCgiVarId,
        char*                       varValue,
        PULONG                      pulSize
    );

typedef  ULONG
(*PFN_HTTPCGIIF_GET_AUTH_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPCGIIF_GET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPCGIIF_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_BOUNDARY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_ROOT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_PATH_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_PATH_TRAN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_QUERY_STR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_FORM_CONT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_HTTPCGIIF_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_HOST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_USER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPCGIIF_GET_METHOD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_SCRIPT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_HTTPCGIIF_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_PROTOCOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_SOFTWARE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_TYPE2)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPCGIIF_GET_INPUT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPCGIIF_GET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  void*
(*PFN_HTTPCGIIF_GET_PARAM2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Requests are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_CGI_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPCGIIF_GET_VAR1          GetEnvVariableByName;                                   \
    PFN_HTTPCGIIF_GET_VAR2          GetEnvVariableById;                                     \
    PFN_HTTPCGIIF_GET_AUTH_TYPE     GetAuthType;                                            \
    PFN_HTTPCGIIF_GET_LENGTH        GetContentLength;                                       \
    PFN_HTTPCGIIF_GET_TYPE          GetContentType;                                         \
    PFN_HTTPCGIIF_GET_BOUNDARY      GetBoundaryDelimiter;                                   \
    PFN_HTTPCGIIF_GET_ROOT          GetDocumentRoot;                                        \
    PFN_HTTPCGIIF_GET_PATH_INFO     GetPathInfo;                                            \
    PFN_HTTPCGIIF_GET_PATH_TRAN     GetPathTranslated;                                      \
    PFN_HTTPCGIIF_GET_QUERY_STR     GetQueryString;                                         \
    PFN_HTTPCGIIF_GET_FORM_CONT     GetFormContent;                                         \
    PFN_HTTPCGIIF_GET_ADDR          GetRemoteAddr;                                          \
    PFN_HTTPCGIIF_GET_HOST          GetRemoteHost;                                          \
    PFN_HTTPCGIIF_GET_USER          GetRemoteUser;                                          \
    PFN_HTTPCGIIF_GET_METHOD        GetRequestMethod;                                       \
    PFN_HTTPCGIIF_GET_SCRIPT        GetScriptName;                                          \
    PFN_HTTPCGIIF_GET_NAME          GetServerName;                                          \
    PFN_HTTPCGIIF_GET_PORT          GetServerPort;                                          \
    PFN_HTTPCGIIF_GET_PROTOCOL      GetServerProtocol;                                      \
    PFN_HTTPCGIIF_GET_SOFTWARE      GetServerSoftware;                                      \
    PFN_HTTPCGIIF_GET_TYPE2         GetServerType;                                          \
    PFN_HTTPCGIIF_GET_INPUT         GetHtmlFormInput;                                       \
    PFN_HTTPCGIIF_GET_PARAM         GetHtmlFormParam;                                       \
    PFN_HTTPCGIIF_GET_PARAM2        GetHtmlFormParam2;                                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_CGI_INTERFACE
{
    HTTP_CGI_INTERFACE_CLASS_CONTENT
}
HTTP_CGI_INTERFACE,  *PHTTP_CGI_INTERFACE;

#define  ACCESS_HTTP_CGI_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_CGI_INTERFACE, Linkage)


#endif
