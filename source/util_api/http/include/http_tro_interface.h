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

    module:	http_tro_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Trans Record Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_TRO_INTERFACE_
#define  _HTTP_TRO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
             HTTP TRANS RECORD OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_TRO_STATE_INITIALIZED                 0
#define  HTTP_TRO_STATE_CLIENT_CONNECTED            1
#define  HTTP_TRO_STATE_PENDING                     2
#define  HTTP_TRO_STATE_RESOLVING_DNS               3
#define  HTTP_TRO_STATE_SERVER_CONNECTED            4
#define  HTTP_TRO_STATE_ESTABLISHED                 5
#define  HTTP_TRO_STATE_FINISHED                    6

#define  HTTP_TRO_QMODE_COLLECT                     1
#define  HTTP_TRO_QMODE_FORWARD                     2
#define  HTTP_TRO_QMODE_PROCESS                     3

#define  HTTP_TRO_SFLAG_HEADERS                     0x00000001
#define  HTTP_TRO_SFLAG_BODY                        0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPTRO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPTRO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPTRO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPTRO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  char*
(*PFN_HTTPTRO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  PUCHAR
(*PFN_HTTPTRO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_HTTPTRO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPTRO_GET_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPTRO_GET_BMO_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_HANDLE
(*PFN_HTTPTRO_ADD_BMO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPTRO_GET_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPTRO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

typedef  BOOL
(*PFN_HTTPTRO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_RESUME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved,
        ULONG                       ulSbcPmode
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_SETUP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_TMH_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPTRO_TMH_SERIALIZE)
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
#define  HTTP_TRANS_RECORD_CLASS_CONTENT                                                    \
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
    ANSC_HANDLE                     hBmoReq;                                                \
    ANSC_HANDLE                     hBmoRep;                                                \
    ANSC_HANDLE                     hClientSocket;                                          \
    ANSC_HANDLE                     hServerSocket;                                          \
    ANSC_HANDLE                     hSbcContext;                                            \
    ANSC_HANDLE                     hCbcContext;                                            \
    ULONG                           HashIndex;                                              \
    ULONG                           SbcPmode;                                               \
    ULONG                           CbcPmode;                                               \
    ULONG                           TransState;                                             \
    BOOL                            bCloseConnection;                                       \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_HTTPTRO_GET_IF              GetWamIf;                                               \
    PFN_HTTPTRO_SET_IF              SetWamIf;                                               \
    PFN_HTTPTRO_GET_IF              GetSbcIf;                                               \
    PFN_HTTPTRO_SET_IF              SetSbcIf;                                               \
    PFN_HTTPTRO_GET_IF              GetCbcIf;                                               \
    PFN_HTTPTRO_SET_IF              SetCbcIf;                                               \
    PFN_HTTPTRO_GET_IF              GetPbcIf;                                               \
    PFN_HTTPTRO_SET_IF              SetPbcIf;                                               \
    PFN_HTTPTRO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPTRO_SET_IF              SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPTRO_GET_CONTEXT         GetBmoReq;                                              \
    PFN_HTTPTRO_SET_CONTEXT         SetBmoReq;                                              \
    PFN_HTTPTRO_GET_CONTEXT         GetBmoRep;                                              \
    PFN_HTTPTRO_SET_CONTEXT         SetBmoRep;                                              \
    PFN_HTTPTRO_GET_CONTEXT         GetClientSocket;                                        \
    PFN_HTTPTRO_SET_CONTEXT         SetClientSocket;                                        \
    PFN_HTTPTRO_GET_CONTEXT         GetServerSocket;                                        \
    PFN_HTTPTRO_SET_CONTEXT         SetServerSocket;                                        \
    PFN_HTTPTRO_GET_CONTEXT         GetSbcContext;                                          \
    PFN_HTTPTRO_SET_CONTEXT         SetSbcContext;                                          \
    PFN_HTTPTRO_GET_CONTEXT         GetCbcContext;                                          \
    PFN_HTTPTRO_SET_CONTEXT         SetCbcContext;                                          \
                                                                                            \
    PFN_HTTPTRO_GET_NAME            GetServerName;                                          \
    PFN_HTTPTRO_SET_NAME            SetServerName;                                          \
    PFN_HTTPTRO_GET_ADDR            GetClientAddr;                                          \
    PFN_HTTPTRO_GET_ADDR            GetServerAddr;                                          \
    PFN_HTTPTRO_GET_PORT            GetClientPort;                                          \
    PFN_HTTPTRO_GET_PORT            GetServerPort;                                          \
                                                                                            \
    PFN_HTTPTRO_GET_MODE            GetSbcPmode;                                            \
    PFN_HTTPTRO_SET_MODE            SetSbcPmode;                                            \
    PFN_HTTPTRO_GET_MODE            GetCbcPmode;                                            \
    PFN_HTTPTRO_SET_MODE            SetCbcPmode;                                            \
    PFN_HTTPTRO_GET_STATE           GetTransState;                                          \
    PFN_HTTPTRO_SET_STATE           SetTransState;                                          \
                                                                                            \
    PFN_HTTPTRO_RETURN              Return;                                                 \
    PFN_HTTPTRO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPTRO_TERMINATE           Terminate;                                              \
    PFN_HTTPTRO_OPEN                Open;                                                   \
    PFN_HTTPTRO_CLOSE               Close;                                                  \
                                                                                            \
    PFN_HTTPTRO_ACQUIRE             AcquireAccess;                                          \
    PFN_HTTPTRO_RELEASE             ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPTRO_QUERY               QueryForClient;                                         \
    PFN_HTTPTRO_RECV                RecvFromClient;                                         \
    PFN_HTTPTRO_FINISH              FinishedByClient;                                       \
    PFN_HTTPTRO_SEND                SendToClient;                                           \
    PFN_HTTPTRO_RESUME              ResumeTransaction;                                      \
                                                                                            \
    PFN_HTTPTRO_QUERY               QueryForServer;                                         \
    PFN_HTTPTRO_RECV                RecvFromServer;                                         \
    PFN_HTTPTRO_FINISH              FinishedByServer;                                       \
    PFN_HTTPTRO_SEND                SendToServer;                                           \
    PFN_HTTPTRO_CONNECT             ConnectToServer;                                        \
    PFN_HTTPTRO_SETUP               SetUpConnection;                                        \
                                                                                            \
    PFN_HTTPTRO_TMH_NOTIFY          TmhClientNotify;                                        \
    PFN_HTTPTRO_TMH_SERIALIZE       TmhClientSerialize;                                     \
    PFN_HTTPTRO_TMH_NOTIFY          TmhServerNotify;                                        \
    PFN_HTTPTRO_TMH_SERIALIZE       TmhServerSerialize;                                     \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_TRANS_RECORD_OBJECT
{
    HTTP_TRANS_RECORD_CLASS_CONTENT
}
HTTP_TRANS_RECORD_OBJECT,  *PHTTP_TRANS_RECORD_OBJECT;

#define  ACCESS_HTTP_TRANS_RECORD_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_TRANS_RECORD_OBJECT, Linkage)


#endif
