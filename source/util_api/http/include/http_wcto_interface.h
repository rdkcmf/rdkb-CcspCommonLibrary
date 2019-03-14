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

    module:	http_wcto_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Webc Trans Object.

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


#ifndef  _HTTP_WCTO_INTERFACE_
#define  _HTTP_WCTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
               HTTP WEBC TRANS OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_WCTO_STATE_INITIALIZED                0
#define  HTTP_WCTO_STATE_REQ_SENT                   1
#define  HTTP_WCTO_STATE_REP_RECEIVING              2
#define  HTTP_WCTO_STATE_ESTABLISHED                3
#define  HTTP_WCTO_STATE_FINISHED                   4

#define  HTTP_WCTO_QMODE_COLLECT                    1
#define  HTTP_WCTO_QMODE_PROCESS                    2

#define  HTTP_WCTO_SFLAG_HEADERS                    0x00000001
#define  HTTP_WCTO_SFLAG_BODY                       0x00000002

#define  HTTP_WCTO_DEFAULT_MAX_AUTH_RETRIES         3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPWCTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPWCTO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  PUCHAR
(*PFN_HTTPWCTO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_HTTPWCTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPWCTO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPWCTO_DO_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_GET_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_GET_BMO_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_ADD_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWCTO_GET_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_REQUEST)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_OPTIONS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_GET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_HEAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_POST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_PUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_TRACE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_DO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ULONG
(*PFN_HTTPWCTO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_ABORT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_TMH_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPWCTO_TMH_SERIALIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
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
#define  HTTP_WEBC_TRANS_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTmhIf;                                                 \
    ANSC_HANDLE                     hWebcSession;                                           \
    ANSC_HANDLE                     hBmoReq;                                                \
    ANSC_HANDLE                     hBmoRep;                                                \
    ANSC_HANDLE                     hBspReqContext;                                         \
    ANSC_HANDLE                     hTcpSimpleClient;                                       \
    ULONG                           TransState;                                             \
    BOOL                            bLastTrans;                                             \
    ANSC_LOCK                       AccessLock;                                             \
    ULONG                           AuthRetries;                                            \
    ULONG                           MaxAuthRetries;                                         \
                                                                                            \
    PFN_HTTPWCTO_GET_IF             GetBspIf;                                               \
    PFN_HTTPWCTO_SET_IF             SetBspIf;                                               \
    PFN_HTTPWCTO_GET_IF             GetHfpIf;                                               \
    PFN_HTTPWCTO_SET_IF             SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPWCTO_GET_CONTEXT        GetWebcSession;                                         \
    PFN_HTTPWCTO_SET_CONTEXT        SetWebcSession;                                         \
    PFN_HTTPWCTO_GET_CONTEXT        GetBmoReq;                                              \
    PFN_HTTPWCTO_SET_CONTEXT        SetBmoReq;                                              \
    PFN_HTTPWCTO_GET_CONTEXT        GetBmoRep;                                              \
    PFN_HTTPWCTO_SET_CONTEXT        SetBmoRep;                                              \
    PFN_HTTPWCTO_GET_CONTEXT        GetTcpSimpleClient;                                     \
    PFN_HTTPWCTO_SET_CONTEXT        SetTcpSimpleClient;                                     \
    PFN_HTTPWCTO_GET_STATE          GetTransState;                                          \
    PFN_HTTPWCTO_SET_STATE          SetTransState;                                          \
                                                                                            \
    PFN_HTTPWCTO_IS_SOMETHING       IsLastTrans;                                            \
    PFN_HTTPWCTO_DO_SOMETHING       DoLastTrans;                                            \
    PFN_HTTPWCTO_RETURN             Return;                                                 \
    PFN_HTTPWCTO_RESET              Reset;                                                  \
                                                                                            \
    PFN_HTTPWCTO_OPEN               Open;                                                   \
    PFN_HTTPWCTO_CLOSE              Close;                                                  \
                                                                                            \
    PFN_HTTPWCTO_ACQUIRE            AcquireAccess;                                          \
    PFN_HTTPWCTO_RELEASE            ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPWCTO_REQUEST            Request;                                                \
    PFN_HTTPWCTO_DO_OPTIONS         DoOptions;                                              \
    PFN_HTTPWCTO_DO_GET             DoGet;                                                  \
    PFN_HTTPWCTO_DO_HEAD            DoHead;                                                 \
    PFN_HTTPWCTO_DO_POST            DoPost;                                                 \
    PFN_HTTPWCTO_DO_PUT             DoPut;                                                  \
    PFN_HTTPWCTO_DO_DELETE          DoDelete;                                               \
    PFN_HTTPWCTO_DO_TRACE           DoTrace;                                                \
    PFN_HTTPWCTO_DO_CONNECT         DoConnect;                                              \
                                                                                            \
    PFN_HTTPWCTO_QUERY              Query;                                                  \
    PFN_HTTPWCTO_RECV               Recv;                                                   \
    PFN_HTTPWCTO_SEND               Send;                                                   \
    PFN_HTTPWCTO_FINISH             Finish;                                                 \
    PFN_HTTPWCTO_ABORT              Abort;                                                  \
                                                                                            \
    PFN_HTTPWCTO_TMH_NOTIFY         TmhNotify;                                              \
    PFN_HTTPWCTO_TMH_SERIALIZE      TmhSerialize;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_WEBC_TRANS_OBJECT
{
    HTTP_WEBC_TRANS_CLASS_CONTENT
}
HTTP_WEBC_TRANS_OBJECT,  *PHTTP_WEBC_TRANS_OBJECT;

#define  ACCESS_HTTP_WEBC_TRANS_OBJECT(p)           \
         ACCESS_CONTAINER(p, HTTP_WEBC_TRANS_OBJECT, Linkage)


#endif
