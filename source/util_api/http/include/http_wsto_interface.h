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

    module:	http_wsto_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Webs Trans Object.

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


#ifndef  _HTTP_WSTO_INTERFACE_
#define  _HTTP_WSTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
               HTTP WEBS TRANS OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_WSTO_STATE_INITIALIZED                0
#define  HTTP_WSTO_STATE_CLIENT_CONNECTED           1
#define  HTTP_WSTO_STATE_ESTABLISHED                2
#define  HTTP_WSTO_STATE_FINISHED                   3

#define  HTTP_WSTO_QMODE_COLLECT                    1
#define  HTTP_WSTO_QMODE_PROCESS                    2

#define  HTTP_WSTO_SFLAG_HEADERS                    0x00000001
#define  HTTP_WSTO_SFLAG_BODY                       0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPWSTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPWSTO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  PUCHAR
(*PFN_HTTPWSTO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_HTTPWSTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_GET_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_GET_BMO_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_ADD_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPWSTO_GET_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPWSTO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_TMH_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPWSTO_TMH_SERIALIZE)
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
#define  HTTP_WEBS_TRANS_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTmhIf;                                                 \
    ANSC_HANDLE                     hWebsSession;                                           \
    ANSC_HANDLE                     hBmoReq;                                                \
    ANSC_HANDLE                     hBmoRep;                                                \
    ANSC_HANDLE                     hWebSocket;                                             \
    ULONG                           TransState;                                             \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_HTTPWSTO_GET_IF             GetWspIf;                                               \
    PFN_HTTPWSTO_SET_IF             SetWspIf;                                               \
    PFN_HTTPWSTO_GET_IF             GetHfpIf;                                               \
    PFN_HTTPWSTO_SET_IF             SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPWSTO_GET_CONTEXT        GetWebsSession;                                         \
    PFN_HTTPWSTO_SET_CONTEXT        SetWebsSession;                                         \
    PFN_HTTPWSTO_GET_CONTEXT        GetBmoReq;                                              \
    PFN_HTTPWSTO_SET_CONTEXT        SetBmoReq;                                              \
    PFN_HTTPWSTO_GET_CONTEXT        GetBmoRep;                                              \
    PFN_HTTPWSTO_SET_CONTEXT        SetBmoRep;                                              \
    PFN_HTTPWSTO_GET_CONTEXT        GetWebSocket;                                           \
    PFN_HTTPWSTO_SET_CONTEXT        SetWebSocket;                                           \
    PFN_HTTPWSTO_GET_ADDR           GetClientAddr;                                          \
    PFN_HTTPWSTO_GET_PORT           GetClientPort;                                          \
    PFN_HTTPWSTO_GET_STATE          GetTransState;                                          \
    PFN_HTTPWSTO_SET_STATE          SetTransState;                                          \
                                                                                            \
    PFN_HTTPWSTO_RETURN             Return;                                                 \
    PFN_HTTPWSTO_RESET              Reset;                                                  \
                                                                                            \
    PFN_HTTPWSTO_OPEN               Open;                                                   \
    PFN_HTTPWSTO_CLOSE              Close;                                                  \
                                                                                            \
    PFN_HTTPWSTO_ACQUIRE            AcquireAccess;                                          \
    PFN_HTTPWSTO_RELEASE            ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPWSTO_QUERY              Query;                                                  \
    PFN_HTTPWSTO_RECV               Recv;                                                   \
    PFN_HTTPWSTO_SEND               Send;                                                   \
    PFN_HTTPWSTO_FINISH             Finish;                                                 \
                                                                                            \
    PFN_HTTPWSTO_TMH_NOTIFY         TmhNotify;                                              \
    PFN_HTTPWSTO_TMH_SERIALIZE      TmhSerialize;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_WEBS_TRANS_OBJECT
{
    HTTP_WEBS_TRANS_CLASS_CONTENT
}
HTTP_WEBS_TRANS_OBJECT,  *PHTTP_WEBS_TRANS_OBJECT;

#define  ACCESS_HTTP_WEBS_TRANS_OBJECT(p)           \
         ACCESS_CONTAINER(p, HTTP_WEBS_TRANS_OBJECT, Linkage)


#endif
