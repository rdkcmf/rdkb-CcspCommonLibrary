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

    module:	http_pso_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Proxy Session Object.

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


#ifndef  _HTTP_PSO_INTERFACE_
#define  _HTTP_PSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
 PLATFORM INDEPENDENT HTTP PROXY SESSION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_PSO_STATE_INITIALIZED                 0
#define  HTTP_PSO_STATE_CLIENT_CONNECTED            1
#define  HTTP_PSO_STATE_RESOLVING_DNS               2
#define  HTTP_PSO_STATE_SERVER_CONNECTED            3
#define  HTTP_PSO_STATE_ESTABLISHED                 4
#define  HTTP_PSO_STATE_FINISHED                    5

#define  HTTP_PSO_QMODE_COLLECT                     1
#define  HTTP_PSO_QMODE_FORWARD                     2
#define  HTTP_PSO_QMODE_PROCESS                     3

#define  HTTP_PSO_SFLAG_HEADERS                     0x00000001
#define  HTTP_PSO_SFLAG_BODY                        0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPPSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPPSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPPSO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  char*
(*PFN_HTTPPSO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  PUCHAR
(*PFN_HTTPPSO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_HTTPPSO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSO_GET_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSO_GET_BMO_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSO_ADD_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSO_GET_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPPSO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

typedef  BOOL
(*PFN_HTTPPSO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_TMH_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSO_TMH_SERIALIZE)
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
#define  HTTP_PROXY_SESSION_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    char                            ServerName[ANSC_DOMAIN_NAME_SIZE];                      \
    ANSC_IPV4_ADDRESS               ServerAddr;                                             \
    USHORT                          ServerPort;                                             \
                                                                                            \
    ANSC_HANDLE                     hWamIf;                                                 \
    ANSC_HANDLE                     hSbcIf;                                                 \
    ANSC_HANDLE                     hCbcIf;                                                 \
    ANSC_HANDLE                     hPbcIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTmhClientIf;                                           \
    ANSC_HANDLE                     hTmhServerIf;                                           \
    ANSC_HANDLE                     hClientSocket;                                          \
    ANSC_HANDLE                     hServerSocket;                                          \
    ANSC_HANDLE                     hSbcContext;                                            \
    ANSC_HANDLE                     hCbcContext;                                            \
    ULONG                           HashIndex;                                              \
    ULONG                           SbcPmode;                                               \
    ULONG                           CbcPmode;                                               \
    ULONG                           SessionState;                                           \
    BOOL                            bEstablished;                                           \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    SLIST_HEADER                    BmoReqSList;                                            \
    ANSC_LOCK                       BmoReqSListLock;                                        \
    SLIST_HEADER                    BmoRepSList;                                            \
    ANSC_LOCK                       BmoRepSListLock;                                        \
                                                                                            \
    PFN_HTTPPSO_GET_IF              GetWamIf;                                               \
    PFN_HTTPPSO_SET_IF              SetWamIf;                                               \
    PFN_HTTPPSO_GET_IF              GetSbcIf;                                               \
    PFN_HTTPPSO_SET_IF              SetSbcIf;                                               \
    PFN_HTTPPSO_GET_IF              GetCbcIf;                                               \
    PFN_HTTPPSO_SET_IF              SetCbcIf;                                               \
    PFN_HTTPPSO_GET_IF              GetPbcIf;                                               \
    PFN_HTTPPSO_SET_IF              SetPbcIf;                                               \
    PFN_HTTPPSO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPPSO_SET_IF              SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPPSO_GET_CONTEXT         GetClientSocket;                                        \
    PFN_HTTPPSO_SET_CONTEXT         SetClientSocket;                                        \
    PFN_HTTPPSO_GET_CONTEXT         GetServerSocket;                                        \
    PFN_HTTPPSO_SET_CONTEXT         SetServerSocket;                                        \
    PFN_HTTPPSO_GET_CONTEXT         GetSbcContext;                                          \
    PFN_HTTPPSO_SET_CONTEXT         SetSbcContext;                                          \
    PFN_HTTPPSO_GET_CONTEXT         GetCbcContext;                                          \
    PFN_HTTPPSO_SET_CONTEXT         SetCbcContext;                                          \
                                                                                            \
    PFN_HTTPPSO_GET_NAME            GetServerName;                                          \
    PFN_HTTPPSO_SET_NAME            SetServerName;                                          \
    PFN_HTTPPSO_GET_ADDR            GetClientAddr;                                          \
    PFN_HTTPPSO_GET_ADDR            GetServerAddr;                                          \
    PFN_HTTPPSO_GET_PORT            GetClientPort;                                          \
    PFN_HTTPPSO_GET_PORT            GetServerPort;                                          \
                                                                                            \
    PFN_HTTPPSO_GET_MODE            GetSbcPmode;                                            \
    PFN_HTTPPSO_SET_MODE            SetSbcPmode;                                            \
    PFN_HTTPPSO_GET_MODE            GetCbcPmode;                                            \
    PFN_HTTPPSO_SET_MODE            SetCbcPmode;                                            \
    PFN_HTTPPSO_GET_STATE           GetSessionState;                                        \
    PFN_HTTPPSO_SET_STATE           SetSessionState;                                        \
                                                                                            \
    PFN_HTTPPSO_RETURN              Return;                                                 \
    PFN_HTTPPSO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPPSO_OPEN                Open;                                                   \
    PFN_HTTPPSO_CLOSE               Close;                                                  \
                                                                                            \
    PFN_HTTPPSO_ACQUIRE             AcquireAccess;                                          \
    PFN_HTTPPSO_RELEASE             ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPPSO_GET_BMO             GetLastBmoReq;                                          \
    PFN_HTTPPSO_GET_BMO             GetLastBmoRep;                                          \
    PFN_HTTPPSO_GET_BMO             GetCurBmoReq;                                           \
    PFN_HTTPPSO_GET_BMO             GetCurBmoRep;                                           \
    PFN_HTTPPSO_GET_BMO_BYID        GetBmoReqByTid;                                         \
    PFN_HTTPPSO_GET_BMO_BYID        GetBmoRepByTid;                                         \
    PFN_HTTPPSO_ADD_BMO             AddNewBmoReq;                                           \
    PFN_HTTPPSO_ADD_BMO             AddNewBmoRep;                                           \
    PFN_HTTPPSO_DEL_ALL             DelAllBmoReqs;                                          \
    PFN_HTTPPSO_DEL_ALL             DelAllBmoReps;                                          \
                                                                                            \
    PFN_HTTPPSO_GET_BUFFER          GetClientBuffer;                                        \
    PFN_HTTPPSO_QUERY               QueryForClient;                                         \
    PFN_HTTPPSO_RECV                RecvFromClient;                                         \
    PFN_HTTPPSO_FINISH              FinishedByClient;                                       \
    PFN_HTTPPSO_SEND                SendToClient;                                           \
    PFN_HTTPPSO_ACCEPT              AcceptClient;                                           \
    PFN_HTTPPSO_AUTH                Authenticate;                                           \
                                                                                            \
    PFN_HTTPPSO_GET_BUFFER          GetServerBuffer;                                        \
    PFN_HTTPPSO_QUERY               QueryForServer;                                         \
    PFN_HTTPPSO_RECV                RecvFromServer;                                         \
    PFN_HTTPPSO_FINISH              FinishedByServer;                                       \
    PFN_HTTPPSO_SEND                SendToServer;                                           \
    PFN_HTTPPSO_CONNECT             ConnectToServer;                                        \
                                                                                            \
    PFN_HTTPPSO_TMH_NOTIFY          TmhClientNotify;                                        \
    PFN_HTTPPSO_TMH_SERIALIZE       TmhClientSerialize;                                     \
    PFN_HTTPPSO_TMH_NOTIFY          TmhServerNotify;                                        \
    PFN_HTTPPSO_TMH_SERIALIZE       TmhServerSerialize;                                     \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_PROXY_SESSION_OBJECT
{
    HTTP_PROXY_SESSION_CLASS_CONTENT
}
HTTP_PROXY_SESSION_OBJECT,  *PHTTP_PROXY_SESSION_OBJECT;

#define  ACCESS_HTTP_PROXY_SESSION_OBJECT(p)        \
         ACCESS_CONTAINER(p, HTTP_PROXY_SESSION_OBJECT, Linkage)


#endif
