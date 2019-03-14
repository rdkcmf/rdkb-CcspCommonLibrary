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

    module:	http_wsso_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Webs Session Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/07/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WSSO_INTERFACE_
#define  _HTTP_WSSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP WEBS SESSION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_WSSO_STATE_INITIALIZED                0
#define  HTTP_WSSO_STATE_CLIENT_CONNECTED           1
#define  HTTP_WSSO_STATE_FINISHED                   2

#define  HTTP_WSSO_QMODE_COLLECT                    1
#define  HTTP_WSSO_QMODE_PROCESS                    2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPWSSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPWSSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPWSSO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_ENTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_LEAVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSSO_GET_WSTO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSSO_ADD_WSTO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPWSSO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSSO_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  BOOL
(*PFN_HTTPWSSO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
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
#define  HTTP_WEBS_SESSION_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hWebSocket;                                             \
    ULONG                           SessionState;                                           \
    ULONG                           ServeCount;                                             \
    ANSC_EVENT                      ServeEvent;                                             \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    SLIST_HEADER                    WstoSList;                                              \
    ANSC_LOCK                       WstoSListLock;                                          \
                                                                                            \
    PFN_HTTPWSSO_GET_IF             GetWspIf;                                               \
    PFN_HTTPWSSO_SET_IF             SetWspIf;                                               \
    PFN_HTTPWSSO_GET_IF             GetHfpIf;                                               \
    PFN_HTTPWSSO_SET_IF             SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPWSSO_GET_CONTEXT        GetWebSocket;                                           \
    PFN_HTTPWSSO_SET_CONTEXT        SetWebSocket;                                           \
    PFN_HTTPWSSO_GET_STATE          GetSessionState;                                        \
    PFN_HTTPWSSO_SET_STATE          SetSessionState;                                        \
                                                                                            \
    PFN_HTTPWSSO_RETURN             Return;                                                 \
    PFN_HTTPWSSO_RESET              Reset;                                                  \
                                                                                            \
    PFN_HTTPWSSO_OPEN               Open;                                                   \
    PFN_HTTPWSSO_CLOSE              Close;                                                  \
                                                                                            \
    PFN_HTTPWSSO_ACQUIRE            AcquireAccess;                                          \
    PFN_HTTPWSSO_RELEASE            ReleaseAccess;                                          \
    PFN_HTTPWSSO_ENTER              EnterWspServe;                                          \
    PFN_HTTPWSSO_LEAVE              LeaveWspServe;                                          \
    PFN_HTTPWSSO_CLEAR              ClearWspServe;                                          \
    PFN_HTTPWSSO_CLOSE              CloseConnection;                                        \
                                                                                            \
    PFN_HTTPWSSO_GET_WSTO           GetEndWsto;                                             \
    PFN_HTTPWSSO_GET_WSTO           GetCurWsto;                                             \
    PFN_HTTPWSSO_ADD_WSTO           AddNewWsto;                                             \
    PFN_HTTPWSSO_DEL_ALL            DelAllWstos;                                            \
                                                                                            \
    PFN_HTTPWSSO_QUERY              Query;                                                  \
    PFN_HTTPWSSO_RECV               Recv;                                                   \
    PFN_HTTPWSSO_FINISH             Finish;                                                 \
    PFN_HTTPWSSO_ACCEPT             Accept;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_WEBS_SESSION_OBJECT
{
    HTTP_WEBS_SESSION_CLASS_CONTENT
}
HTTP_WEBS_SESSION_OBJECT,  *PHTTP_WEBS_SESSION_OBJECT;

#define  ACCESS_HTTP_WEBS_SESSION_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_WEBS_SESSION_OBJECT, Linkage)


#endif
