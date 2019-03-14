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

    module:	ansc_spto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Proxy Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SPTO_INTERFACE_
#define  _ANSC_SPTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_sptowo_interface.h"
#include "ansc_dstowo_interface.h"
#include "ansc_bstowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SIMPLE PROXY TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SPTO_MIN_SOCKET_COUNT                 64
#define  ANSC_SPTO_MAX_SOCKET_COUNT                 64 * 32
#define  ANSC_SPTO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_SPTO_DEF_ENGINE_COUNT                 1
#define  ANSC_SPTO_MAX_ENGINE_COUNT                 32
#define  ANSC_SPTO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_SPTO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_SPTO_CLEAN_TASK_INTERVAL              10          /* in seconds      */
#define  ANSC_SPTO_SOCKET_TIMEOUT                   90          /* in seconds      */

#define  ANSC_SPTO_MODE_DAEMON_TIMEOUT              0x00000001
#define  ANSC_SPTO_MODE_BROKER_TIMEOUT              0x00000002
#define  ANSC_SPTO_MODE_FOREIGN_BUFFER              0x00000004

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SPTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SPTO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SPTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SPTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_SPTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_SPTO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ANSC_STATUS
(*PFN_SPTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_SPTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SPTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_SPTO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SPTO_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

typedef  ANSC_HANDLE
(*PFN_SPTO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    );

typedef  ANSC_STATUS
(*PFN_SPTO_DEL_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

typedef  ANSC_STATUS
(*PFN_SPTO_DEL_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_SPTO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Tcp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Tcp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_SIMPLE_PROXY_TCP_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hBrokerServer;                                          \
    ANSC_HANDLE                     hDaemonWorker;                                          \
    ANSC_HANDLE                     hBrokerWorker;                                          \
    ANSC_HANDLE                     hProxyWorker;                                           \
                                                                                            \
    ULONG                           MaxMessageSize;                                         \
    ULONG                           EngineCount;                                            \
    ULONG                           MinSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SPTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SPTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SPTO_GET_PORT               GetHostPort;                                            \
    PFN_SPTO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_SPTO_GET_WORKER             GetProxyWorker;                                         \
    PFN_SPTO_SET_WORKER             SetProxyWorker;                                         \
    PFN_SPTO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SPTO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SPTO_GET_COUNT              GetEngineCount;                                         \
    PFN_SPTO_SET_COUNT              SetEngineCount;                                         \
    PFN_SPTO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_SPTO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_SPTO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_SPTO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_SPTO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_SPTO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_SPTO_GET_MODE               GetMode;                                                \
    PFN_SPTO_SET_MODE               SetMode;                                                \
    PFN_SPTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SPTO_ENGAGE                 Engage;                                                 \
    PFN_SPTO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_SPTO_GET_SOCKET             PubGetSocket;                                           \
    PFN_SPTO_ADD_SOCKET             PubAddSocket;                                           \
    PFN_SPTO_DEL_SOCKET1            PubDelSocketByAddr;                                     \
    PFN_SPTO_DEL_SOCKET2            PubDelSocket;                                           \
    PFN_SPTO_DEL_ALL                PubDelAllSockets;                                       \
                                                                                            \
    PFN_DSTOWO_INIT                 DwoInit;                                                \
    PFN_DSTOWO_UNLOAD               DwoUnload;                                              \
    PFN_DSTOWO_ACCEPT               DwoAccept;                                              \
    PFN_DSTOWO_SETOUT               DwoSetOut;                                              \
    PFN_DSTOWO_REMOVE               DwoRemove;                                              \
    PFN_DSTOWO_QUERY                DwoQuery;                                               \
    PFN_DSTOWO_PROCESS              DwoProcessSync;                                         \
    PFN_DSTOWO_PROCESS              DwoProcessAsync;                                        \
    PFN_DSTOWO_COMPLETE             DwoSendComplete;                                        \
    PFN_DSTOWO_NOTIFY               DwoNotify;                                              \
                                                                                            \
    PFN_BSTOWO_INIT                 BwoInit;                                                \
    PFN_BSTOWO_UNLOAD               BwoUnload;                                              \
    PFN_BSTOWO_ACCEPT               BwoAccept;                                              \
    PFN_BSTOWO_REMOVE               BwoRemove;                                              \
    PFN_BSTOWO_QUERY                BwoQuery;                                               \
    PFN_BSTOWO_PROCESS              BwoProcessSync;                                         \
    PFN_BSTOWO_PROCESS              BwoProcessAsync;                                        \
    PFN_BSTOWO_COMPLETE             BwoSendComplete;                                        \
    PFN_BSTOWO_NOTIFY               BwoNotify;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLE_PROXY_TCP_OBJECT
{
    ANSC_SIMPLE_PROXY_TCP_CLASS_CONTENT
}
ANSC_SIMPLE_PROXY_TCP_OBJECT,  *PANSC_SIMPLE_PROXY_TCP_OBJECT;

#define  ACCESS_ANSC_SIMPLE_PROXY_TCP_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_PROXY_TCP_OBJECT, Linkage)


/***********************************************************
   SESSION OBJECT DEFINITION FOR SIMPLE PROXY TCP OBJECT
***********************************************************/

/*
 * The Simple Proxy Object is actually a "glue" object wrapping up a Daemon Server and a Broker
 * Server Object back to back. In many cases, there's a unique one-on-one mapping between a Daemon
 * and a Broker Socket Object. We use following data structure to refer to such a relationship.
 */
typedef  struct
_ANSC_SPTO_SESSION_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hProxy;
    ANSC_HANDLE                     hDaemonSocket;
    ANSC_HANDLE                     hBrokerSocket;
}
ANSC_SPTO_SESSION_OBJECT,  *PANSC_SPTO_SESSION_OBJECT;

#define  ACCESS_ANSC_SPTO_SESSION_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_SPTO_SESSION_OBJECT, Linkage)


#endif
