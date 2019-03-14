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

    module:	ansc_bsto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Server Tcp Object.

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


#ifndef  _ANSC_BSTO_INTERFACE_
#define  _ANSC_BSTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_bstowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT BROKER SERVER TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BSTO_MIN_SOCKET_COUNT                 128
#define  ANSC_BSTO_MAX_SOCKET_COUNT                 64 * 32
#define  ANSC_BSTO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_BSTO_DEF_ENGINE_COUNT                 32
#define  ANSC_BSTO_MAX_ENGINE_COUNT                 128
#define  ANSC_BSTO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_BSTO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_BSTO_SOCKET_TIMEOUT                   90          /* in seconds      */

#define  ANSC_BSTO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_BSTO_ACCEPT_TASK_NAME                 "ansc_bsto_accept_task"
#define  ANSC_BSTO_WORKER_TASK_NAME                 "ansc_bsto_worker_task"

#define  ANSC_BSTO_MODE_ASYNC_SEND                  0x00000001
#define  ANSC_BSTO_MODE_NO_TIMEOUT                  0x00000002
#define  ANSC_BSTO_MODE_EVENT_SYNC                  0x00000004
#define  ANSC_BSTO_MODE_AUTO_CLOSE                  0x00000008
#define  ANSC_BSTO_MODE_DYNAMIC_BUFFER              0x00000010
#define  ANSC_BSTO_MODE_POLLING_ACCEPT              0x00000020
#define  ANSC_BSTO_MODE_FOREIGN_BUFFER              0x00000040
#define  ANSC_BSTO_MODE_COMPACT                     0x00000080
#define  ANSC_BSTO_MODE_XSOCKET                     0x00010000

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BSTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BSTO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_BSTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_BSTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_BSTO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_BSTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_BSTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_BSTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_BSTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_BSTO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSTO_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

typedef  ANSC_HANDLE
(*PFN_BSTO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    );

typedef  ANSC_STATUS
(*PFN_BSTO_DEL_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

typedef  ANSC_STATUS
(*PFN_BSTO_DEL_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BSTO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSTO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BSTO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSTO_ASSIGN)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr
    );

typedef  ULONG
(*PFN_BSTO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

typedef  ANSC_STATUS
(*PFN_BSTO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

/*
 * Tcp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Tcp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Broker Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BROKER_SERVER_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hWorker;                                                \
    ULONG                           MaxMessageSize;                                         \
    ULONG                           EngineCount;                                            \
    ANSC_HANDLE                     hNextEngineToUse;                                       \
    ULONG                           CurSocketCount;                                         \
    ULONG                           MinSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ANSC_LOCK                       OpLock;                                                 \
    ULONG                           Mode;                                                   \
    ULONG                           StartTime;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    ULONG                           TccCount;                                               \
    ULONG                           MccCount;                                               \
    ULONG                           TrcCount;                                               \
    ULONG                           MrcCount;                                               \
    ULONG                           TscCount;                                               \
    ULONG                           MscCount;                                               \
                                                                                            \
    ANSC_HANDLE                     EngineArray[ANSC_BSTO_MAX_ENGINE_COUNT];                \
    ANSC_LOCK                       EngineArrayLock;                                        \
    SLIST_HEADER                    SocketSList;                                            \
    ANSC_LOCK                       SocketSListLock;                                        \
                                                                                            \
    PFN_BSTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_BSTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_BSTO_GET_PORT               GetHostPort;                                            \
    PFN_BSTO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_BSTO_GET_WORKER             GetWorker;                                              \
    PFN_BSTO_SET_WORKER             SetWorker;                                              \
    PFN_BSTO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_BSTO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_BSTO_GET_COUNT              GetEngineCount;                                         \
    PFN_BSTO_SET_COUNT              SetEngineCount;                                         \
    PFN_BSTO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_BSTO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_BSTO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_BSTO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_BSTO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_BSTO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_BSTO_GET_MODE               GetMode;                                                \
    PFN_BSTO_SET_MODE               SetMode;                                                \
    PFN_BSTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_BSTO_ASYNC_TASK             AcceptTask;                                             \
    PFN_BSTO_ASYNC_TASK             WorkerTask;                                             \
                                                                                            \
    PFN_BSTO_ENGAGE                 Engage;                                                 \
    PFN_BSTO_CANCEL                 Cancel;                                                 \
    PFN_BSTO_START                  StartEngines;                                           \
    PFN_BSTO_STOP                   StopEngines;                                            \
                                                                                            \
    PFN_BSTO_GET_SOCKET             GetSocket;                                              \
    PFN_BSTO_ADD_SOCKET             AddSocket;                                              \
    PFN_BSTO_DEL_SOCKET1            DelSocketByAddr;                                        \
    PFN_BSTO_DEL_SOCKET2            DelSocket;                                              \
    PFN_BSTO_DEL_ALL                DelAllSockets;                                          \
                                                                                            \
    PFN_BSTO_ACQUIRE                AcquireSocket;                                          \
    PFN_BSTO_RELEASE                ReleaseSocket;                                          \
    PFN_BSTO_MANUFACTURE            ManufactureSocketPool;                                  \
    PFN_BSTO_DESTROY                DestroySocketPool;                                      \
                                                                                            \
    PFN_BSTO_ASSIGN                 AssignEngine;                                           \
    PFN_BSTO_MANUFACTURE            ManufactureEnginePool;                                  \
    PFN_BSTO_DESTROY                DestroyEnginePool;                                      \
                                                                                            \
    PFN_BSTO_QUERY                  Query;                                                  \
    PFN_BSTO_PROCESS                ProcessSync;                                            \
    PFN_BSTO_PROCESS                ProcessAsync;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BROKER_SERVER_TCP_OBJECT
{
    ANSC_BROKER_SERVER_TCP_CLASS_CONTENT
}
ANSC_BROKER_SERVER_TCP_OBJECT,  *PANSC_BROKER_SERVER_TCP_OBJECT;

#define  ACCESS_ANSC_BROKER_SERVER_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_SERVER_TCP_OBJECT, Linkage)


#endif
