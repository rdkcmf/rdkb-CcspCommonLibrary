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

    module:	web_roo_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Resource Owner Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/02    initial revision.

**********************************************************************/


#ifndef  _WEB_ROO_INTERFACE_
#define  _WEB_ROO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "web_properties.h"

#include "http_ifo_cgi.h"
#include "http_ifo_rcp.h"


/***********************************************************
  PLATFORM INDEPENDENT WEB RESOURCE OWNER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBROO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBROO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBROO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  char*
(*PFN_WEBROO_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  BOOL
(*PFN_WEBROO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_WEBROO_MATCH_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_WEBROO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_OPTIONS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_GET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_HEAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_POST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_PUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_TRACE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_SEARCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_M_SEARCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_M_POST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_SUBSCRIBE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_DO_UNSUBSCRIBE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_REPLY_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_WEBROO_REPLY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );


/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  WEB_RESOURCE_OWNER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    WEB_RESOURCE_OWNER_PROPERTY     Property;                                               \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hRcpIf;                                                 \
    ANSC_LOCK                       SyncLock;                                               \
    ANSC_HANDLE                     hAuthServer;                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_WEBROO_GET_CONTEXT          GetAuthServer;                                          \
    PFN_WEBROO_GET_IF               GetHfpIf;                                               \
    PFN_WEBROO_SET_IF               SetHfpIf;                                               \
    PFN_WEBROO_GET_IF               GetRcpIf;                                               \
    PFN_WEBROO_GET_PATH             GetPathName;                                            \
    PFN_WEBROO_SET_PATH             SetPathName;                                            \
    PFN_WEBROO_IS_SOMETHING         IsDefaultOwner;                                         \
    PFN_WEBROO_GET_PROPERTY         GetProperty;                                            \
    PFN_WEBROO_SET_PROPERTY         SetProperty;                                            \
    PFN_WEBROO_RESET                ResetProperty;                                          \
    PFN_WEBROO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBROO_ACQUIRE              AcquireAccess;                                          \
    PFN_WEBROO_RELEASE              ReleaseAccess;                                          \
    PFN_WEBROO_ENGAGE               Engage;                                                 \
    PFN_WEBROO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_WEBROO_QUERY                Query;                                                  \
    PFN_WEBROO_PROCESS              Process;                                                \
                                                                                            \
    PFN_WEBROO_DO_OPTIONS           DoOptions;                                              \
    PFN_WEBROO_DO_GET               DoGet;                                                  \
    PFN_WEBROO_DO_HEAD              DoHead;                                                 \
    PFN_WEBROO_DO_POST              DoPost;                                                 \
    PFN_WEBROO_DO_PUT               DoPut;                                                  \
    PFN_WEBROO_DO_DELETE            DoDelete;                                               \
    PFN_WEBROO_DO_TRACE             DoTrace;                                                \
    PFN_WEBROO_DO_CONNECT           DoConnect;                                              \
                                                                                            \
    PFN_WEBROO_DO_NOTIFY            DoNotify;                                               \
    PFN_WEBROO_DO_SEARCH            DoSearch;                                               \
    PFN_WEBROO_DO_M_SEARCH          DoMSearch;                                              \
    PFN_WEBROO_DO_M_POST            DoMPost;                                                \
    PFN_WEBROO_DO_SUBSCRIBE         DoSubscribe;                                            \
    PFN_WEBROO_DO_UNSUBSCRIBE       DoUnsubscribe;                                          \
                                                                                            \
    PFN_WEBROO_REPLY_REQ            ReplyReq;                                               \
    PFN_WEBROO_REPLY                Reply300;                                               \
    PFN_WEBROO_REPLY                Reply301;                                               \
    PFN_WEBROO_REPLY                Reply302;                                               \
    PFN_WEBROO_REPLY                Reply303;                                               \
    PFN_WEBROO_REPLY                Reply304;                                               \
    PFN_WEBROO_REPLY                Reply305;                                               \
    PFN_WEBROO_REPLY                Reply307;                                               \
    PFN_WEBROO_REPLY                Reply400;                                               \
    PFN_WEBROO_REPLY                Reply403;                                               \
    PFN_WEBROO_REPLY                Reply404;                                               \
    PFN_WEBROO_REPLY                Reply405;                                               \
    PFN_WEBROO_REPLY                Reply409;                                               \
    PFN_WEBROO_REPLY                Reply410;                                               \
    PFN_WEBROO_REPLY                Reply415;                                               \
    PFN_WEBROO_REPLY                Reply417;                                               \
    PFN_WEBROO_REPLY                Reply500;                                               \
    PFN_WEBROO_REPLY                Reply501;                                               \
    PFN_WEBROO_REPLY                Reply503;                                               \
    PFN_WEBROO_REPLY                Reply505;                                               \
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
    /* end of object class content */                                                       \

#define  HTTP_METHOD_NAME_NOTIFY                    "NOTIFY"
#define  HTTP_METHOD_NAME_SEARCH                    "SEARCH"
#define  HTTP_METHOD_NAME_M_SEARCH                  "M-SEARCH"
#define  HTTP_METHOD_NAME_M_POST                    "M-POST"
#define  HTTP_METHOD_NAME_SUBSCRIBE                 "SUBSCRIBE"
#define  HTTP_METHOD_NAME_UNSUBSCRIBE               "UNSUBSCRIBE"

typedef  struct
_WEB_RESOURCE_OWNER_OBJECT
{
    WEB_RESOURCE_OWNER_CLASS_CONTENT
}
WEB_RESOURCE_OWNER_OBJECT,  *PWEB_RESOURCE_OWNER_OBJECT;

#define  ACCESS_WEB_RESOURCE_OWNER_OBJECT(p)        \
         ACCESS_CONTAINER(p, WEB_RESOURCE_OWNER_OBJECT, Linkage)


#endif
