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

    module:	http_psover2_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Pso Ver2 Object.

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


#ifndef  _HTTP_PSOVER2_INTERFACE_
#define  _HTTP_PSOVER2_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT HTTP PSO VER2 OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_PSOVER2_STATE_INITIALIZED             0
#define  HTTP_PSOVER2_STATE_CLIENT_CONNECTED        1
#define  HTTP_PSOVER2_STATE_FINISHED                2

#define  HTTP_PSOVER2_QMODE_COLLECT                 1
#define  HTTP_PSOVER2_QMODE_PROCESS                 2

#define  HTTP_PSOVER2_SFLAG_HEADERS                 0x00000001
#define  HTTP_PSOVER2_SFLAG_BODY                    0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPPSOVER2_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPPSOVER2_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_ASK_TRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_POP_TRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_GET_TRO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPPSOVER2_ADD_TRO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPPSOVER2_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

typedef  BOOL
(*PFN_HTTPPSOVER2_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPPSOVER2_AUTH)
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
#define  HTTP_PSO_VER2_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWamIf;                                                 \
    ANSC_HANDLE                     hSbcIf;                                                 \
    ANSC_HANDLE                     hCbcIf;                                                 \
    ANSC_HANDLE                     hPbcIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hClientSocket;                                          \
    ANSC_HANDLE                     hSbcContext;                                            \
    ANSC_HANDLE                     hCbcContext;                                            \
    ULONG                           HashIndex;                                              \
    ULONG                           SbcPmode;                                               \
    ULONG                           CbcPmode;                                               \
    ULONG                           SessionState;                                           \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    SLIST_HEADER                    TroSList;                                               \
    ANSC_LOCK                       TroSListLock;                                           \
                                                                                            \
    PFN_HTTPPSOVER2_GET_IF          GetWamIf;                                               \
    PFN_HTTPPSOVER2_SET_IF          SetWamIf;                                               \
    PFN_HTTPPSOVER2_GET_IF          GetSbcIf;                                               \
    PFN_HTTPPSOVER2_SET_IF          SetSbcIf;                                               \
    PFN_HTTPPSOVER2_GET_IF          GetCbcIf;                                               \
    PFN_HTTPPSOVER2_SET_IF          SetCbcIf;                                               \
    PFN_HTTPPSOVER2_GET_IF          GetPbcIf;                                               \
    PFN_HTTPPSOVER2_SET_IF          SetPbcIf;                                               \
    PFN_HTTPPSOVER2_GET_IF          GetHfpIf;                                               \
    PFN_HTTPPSOVER2_SET_IF          SetHfpIf;                                               \
                                                                                            \
    PFN_HTTPPSOVER2_GET_CONTEXT     GetClientSocket;                                        \
    PFN_HTTPPSOVER2_SET_CONTEXT     SetClientSocket;                                        \
    PFN_HTTPPSOVER2_GET_CONTEXT     GetSbcContext;                                          \
    PFN_HTTPPSOVER2_SET_CONTEXT     SetSbcContext;                                          \
    PFN_HTTPPSOVER2_GET_CONTEXT     GetCbcContext;                                          \
    PFN_HTTPPSOVER2_SET_CONTEXT     SetCbcContext;                                          \
                                                                                            \
    PFN_HTTPPSOVER2_GET_MODE        GetSbcPmode;                                            \
    PFN_HTTPPSOVER2_SET_MODE        SetSbcPmode;                                            \
    PFN_HTTPPSOVER2_GET_MODE        GetCbcPmode;                                            \
    PFN_HTTPPSOVER2_SET_MODE        SetCbcPmode;                                            \
    PFN_HTTPPSOVER2_GET_STATE       GetSessionState;                                        \
    PFN_HTTPPSOVER2_SET_STATE       SetSessionState;                                        \
                                                                                            \
    PFN_HTTPPSOVER2_RETURN          Return;                                                 \
    PFN_HTTPPSOVER2_RESET           Reset;                                                  \
                                                                                            \
    PFN_HTTPPSOVER2_OPEN            Open;                                                   \
    PFN_HTTPPSOVER2_CLOSE           Close;                                                  \
                                                                                            \
    PFN_HTTPPSOVER2_ACQUIRE         AcquireAccess;                                          \
    PFN_HTTPPSOVER2_RELEASE         ReleaseAccess;                                          \
                                                                                            \
    PFN_HTTPPSOVER2_ASK_TRO         AskTroBySocket;                                         \
    PFN_HTTPPSOVER2_POP_TRO         PopTroBySocket;                                         \
    PFN_HTTPPSOVER2_GET_TRO         GetLastTro;                                             \
    PFN_HTTPPSOVER2_GET_TRO         GetCurTro;                                              \
    PFN_HTTPPSOVER2_ADD_TRO         AddNewTro;                                              \
    PFN_HTTPPSOVER2_DEL_ALL         DelAllTros;                                             \
                                                                                            \
    PFN_HTTPPSOVER2_QUERY           QueryForClient;                                         \
    PFN_HTTPPSOVER2_RECV            RecvFromClient;                                         \
    PFN_HTTPPSOVER2_FINISH          FinishedByClient;                                       \
    PFN_HTTPPSOVER2_ACCEPT          AcceptClient;                                           \
    PFN_HTTPPSOVER2_AUTH            Authenticate;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_PSO_VER2_OBJECT
{
    HTTP_PSO_VER2_CLASS_CONTENT
}
HTTP_PSO_VER2_OBJECT,  *PHTTP_PSO_VER2_OBJECT;

#define  ACCESS_HTTP_PSO_VER2_OBJECT(p)             \
         ACCESS_CONTAINER(p, HTTP_PSO_VER2_OBJECT, Linkage)


#endif
