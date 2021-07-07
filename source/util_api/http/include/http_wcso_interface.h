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

    module:	http_wcso_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Webc Session Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WCSO_INTERFACE_
#define  _HTTP_WCSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"

#include "ansc_scto_interface.h"
#include "ansc_scto_external_api.h"
#include "ansc_sctowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP WEBC SESSION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_WCSO_STATE_INITIALIZED                0
#define  HTTP_WCSO_STATE_SERVER_CONNECTED           1
#define  HTTP_WCSO_STATE_FINISHED                   2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPWCSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_HTTPWCSO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  PUCHAR
(*PFN_HTTPWCSO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_HTTPWCSO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ULONG
(*PFN_HTTPWCSO_GET_FLAG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_FLAG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlag
    );

typedef  ULONG
(*PFN_HTTPWCSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPWCSO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPWCSO_MATCH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        USHORT                      port,
        ULONG                       flags
    );

typedef  BOOL
(*PFN_HTTPWCSO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCSO_GET_WCTO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCSO_ADD_WCTO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_REQUEST)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_OPTIONS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_GET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_HEAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_POST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_PUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_TRACE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_DO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_CONNECT_PEER)
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddr
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCSO_CONNECT_FAILED)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * A significant difference between HTTP/1.1 and earlier versions of HTTP is that persistent co-
 * nnetions are the default behavior of any HTTP connection. That is, unless otherwise indicated,
 * the client SHOULD assme that the server will maintain a persistent connection, even after error
 * responses from the server.
 *
 * The HTTP Simple Proxy Object creates a unique outbound session for each inbound HTTP session:
 * the closing of either session will bring down the other. The proxy itself does NOT contribute
 * to the maintainence of the persistent connection. Note that more sophisticated HTTP proxies MAY
 * employ different session management to cope with following requirements:
 *
 *      $ providing cached response directly to the clients
 *      $ enable outbound session sharing if multiple clients is requesting the same URL
 *      $ maintain inbound and outbound sessions separately for better efficiency
 */
#define  HTTP_WEBC_SESSION_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    HTTP_WEBC_SESSION_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hBspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTcpSimpleClient;                                       \
    ANSC_HANDLE                     hBspReqContext;                                         \
    ANSC_HANDLE                     hSctoWorker;                                            \
    ULONG                           SessionState;                                           \
    ULONG                           Timestamp;                                              \
    ULONG                           HashIndex;                                              \
    BOOL                            bRemoveMe;                                              \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    SLIST_HEADER                    WctoSList;                                              \
    ANSC_LOCK                       WctoSListLock;                                          \
    ULONG                           ReqMethod;                                              \
    ANSC_HANDLE                     hReqUri;                                                \
    ULONG                           RetryPeerAddr;                                          \
                                                                                            \
    PFN_HTTPWCSO_GET_NAME           GetPeerName;                                            \
    PFN_HTTPWCSO_SET_NAME           SetPeerName;                                            \
    PFN_HTTPWCSO_GET_PORT           GetPeerPort;                                            \
    PFN_HTTPWCSO_SET_PORT           SetPeerPort;                                            \
    PFN_HTTPWCSO_GET_ADDR           GetHostAddr;                                            \
    PFN_HTTPWCSO_SET_ADDR           SetHostAddr;                                            \
    PFN_HTTPWCSO_GET_PORT           GetHostPort;                                            \
    PFN_HTTPWCSO_SET_PORT           SetHostPort;                                            \
    PFN_HTTPWCSO_GET_FLAG           GetSessionFlags;                                        \
    PFN_HTTPWCSO_SET_FLAG           SetSessionFlags;                                        \
                                                                                            \
    PFN_HTTPWCSO_GET_IF             GetBspIf;                                               \
    PFN_HTTPWCSO_SET_IF             SetBspIf;                                               \
    PFN_HTTPWCSO_GET_IF             GetHfpIf;                                               \
    PFN_HTTPWCSO_SET_IF             SetHfpIf;                                               \
    PFN_HTTPWCSO_GET_STATE          GetSessionState;                                        \
    PFN_HTTPWCSO_SET_STATE          SetSessionState;                                        \
                                                                                            \
    PFN_HTTPWCSO_GET_PROPERTY       GetProperty;                                            \
    PFN_HTTPWCSO_SET_PROPERTY       SetProperty;                                            \
    PFN_HTTPWCSO_RESET              ResetProperty;                                          \
    PFN_HTTPWCSO_RESET              Reset;                                                  \
    PFN_HTTPWCSO_RETURN             Return;                                                 \
                                                                                            \
    PFN_HTTPWCSO_MATCH              MatchServer;                                            \
    PFN_HTTPWCSO_IS_SOMETHING       IsReadyToRequest;                                       \
    PFN_HTTPWCSO_OPEN               Open;                                                   \
    PFN_HTTPWCSO_CLOSE              Close;                                                  \
                                                                                            \
    PFN_HTTPWCSO_ACQUIRE            AcquireAccess;                                          \
    PFN_HTTPWCSO_RELEASE            ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPWCSO_GET_WCTO           GetEndWcto;                                             \
    PFN_HTTPWCSO_GET_WCTO           GetCurWcto;                                             \
    PFN_HTTPWCSO_ADD_WCTO           AddNewWcto;                                             \
    PFN_HTTPWCSO_DEL_ALL            DelAllWctos;                                            \
                                                                                            \
    PFN_HTTPWCSO_CONNECT            Connect;                                                \
    PFN_HTTPWCSO_REQUEST            Request;                                                \
    PFN_HTTPWCSO_DO_OPTIONS         DoOptions;                                              \
    PFN_HTTPWCSO_DO_GET             DoGet;                                                  \
    PFN_HTTPWCSO_DO_HEAD            DoHead;                                                 \
    PFN_HTTPWCSO_DO_POST            DoPost;                                                 \
    PFN_HTTPWCSO_DO_PUT             DoPut;                                                  \
    PFN_HTTPWCSO_DO_DELETE          DoDelete;                                               \
    PFN_HTTPWCSO_DO_TRACE           DoTrace;                                                \
    PFN_HTTPWCSO_DO_CONNECT         DoConnect;                                              \
                                                                                            \
    PFN_SCTOWO_QUERY                SctowoQuery;                                            \
    PFN_SCTOWO_PROCESS              SctowoProcess;                                          \
    PFN_SCTOWO_NOTIFY               SctowoNotify;                                           \
                                                                                            \
    PFN_HTTPWCSO_CONNECT_PEER       ConnectPeer;                                            \
    PFN_HTTPWCSO_CONNECT_FAILED     ConnectionFailed;                                       \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_WEBC_SESSION_OBJECT
{
    HTTP_WEBC_SESSION_CLASS_CONTENT
}
HTTP_WEBC_SESSION_OBJECT,  *PHTTP_WEBC_SESSION_OBJECT;

#define  ACCESS_HTTP_WEBC_SESSION_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_WEBC_SESSION_OBJECT, Linkage)


#endif
