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

    module:	ansc_dsto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Daemon Server Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DSTO_INTERFACE_
#define  _ANSC_DSTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"
#include "ansc_sdtopo_interface.h"
#include "ansc_dstowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT DAEMON SERVER TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DSTO_MIN_SOCKET_COUNT                 128
#define  ANSC_DSTO_MAX_SOCKET_COUNT                 64 * 32
#define  ANSC_DSTO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_DSTO_DEF_ENGINE_COUNT                 32
#define  ANSC_DSTO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_DSTO_TASK_CLEANUP_TIME                3000        /* in milliseconds */

#define  ANSC_DSTO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_DSTO_ACCEPT_TASK_NAME                 "ansc_dsto_accept_task"
#define  ANSC_DSTO_WORKER_TASK_NAME                 "ansc_dsto_worker_task"

#define  ANSC_DSTO_MODE_ASYNC_SEND                  0x00000001
#define  ANSC_DSTO_MODE_NO_TIMEOUT                  0x00000002
#define  ANSC_DSTO_MODE_EVENT_SYNC                  0x00000004
#define  ANSC_DSTO_MODE_AUTO_CLOSE                  0x00000008
#define  ANSC_DSTO_MODE_DYNAMIC_BUFFER              0x00000010
#define  ANSC_DSTO_MODE_POLLING_ACCEPT              0x00000020
#define  ANSC_DSTO_MODE_FOREIGN_BUFFER              0x00000040
#define  ANSC_DSTO_MODE_COMPACT                     0x00000080
#define  ANSC_DSTO_MODE_TLS_ENABLED                 0x00000100
#define  ANSC_DSTO_MODE_TLS_REQ_CERT                0x00000200
#define  ANSC_DSTO_MODE_XSOCKET                     0x00010000

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_DSTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_DSTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_DSTO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_DSTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_DSTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_DSTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_DSTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_DSTO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_GET_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_GET_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DSTO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSTO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSTO_ASSIGN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_DSTO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

typedef  ANSC_STATUS
(*PFN_DSTO_PROCESS)
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
 * server applications. The Daemon Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_DAEMON_SERVER_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    char                            HostName[INET6_ADDRSTRLEN];                             \
    ansc_addrinfo*                  pHostAddr1;                                             \
    xskt_addrinfo*                  pHostAddr2;                                             \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hSdtoPatient;                                           \
    ANSC_HANDLE                     hWorker;                                                \
    ULONG                           MaxMessageSize;                                         \
    ULONG                           EngineCount;                                            \
    ANSC_HANDLE                     hNextEngineToUse;                                       \
    ULONG                           CurSocketCount;                                         \
    ULONG                           MinSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ANSC_LOCK                       OpLock;                                                 \
    ANSC_EVENT                      AcceptEvent;                                            \
    ULONG                           Mode;                                                   \
    ULONG                           StartTime;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hTlsScsIf;                                              \
    BOOL                            bTlsEnabled;                                            \
    BOOL                            bTlsReqCert;                                            \
                                                                                            \
    ULONG                           TccCount;                                               \
    ULONG                           MccCount;                                               \
    ULONG                           TrcCount;                                               \
    ULONG                           MrcCount;                                               \
    ULONG                           TscCount;                                               \
    ULONG                           MscCount;                                               \
                                                                                            \
    QUEUE_HEADER                    EngineQueue;                                            \
    ANSC_LOCK                       EngineQueueLock;                                        \
    SLIST_HEADER                    SocketSList;                                            \
    ANSC_LOCK                       SocketSListLock;                                        \
                                                                                            \
    PFN_DSTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_DSTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_DSTO_GET_PORT               GetHostPort;                                            \
    PFN_DSTO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_DSTO_GET_CONTEXT            GetSdtoPatient;                                         \
    PFN_DSTO_GET_WORKER             GetWorker;                                              \
    PFN_DSTO_SET_WORKER             SetWorker;                                              \
    PFN_DSTO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_DSTO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_DSTO_GET_COUNT              GetEngineCount;                                         \
    PFN_DSTO_SET_COUNT              SetEngineCount;                                         \
    PFN_DSTO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_DSTO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_DSTO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_DSTO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_DSTO_GET_MODE               GetMode;                                                \
    PFN_DSTO_SET_MODE               SetMode;                                                \
    PFN_DSTO_GET_IF                 GetTlsScsIf;                                            \
    PFN_DSTO_SET_IF                 SetTlsScsIf;                                            \
    PFN_DSTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_DSTO_ASYNC_TASK             AcceptTask;                                             \
    PFN_DSTO_ASYNC_TASK             WorkerTask;                                             \
    PFN_DSTO_ASYNC_TASK             AsyncJobTask;                                           \
                                                                                            \
    PFN_DSTO_ENGAGE                 Engage;                                                 \
    PFN_DSTO_CANCEL                 Cancel;                                                 \
    PFN_DSTO_START                  StartEngines;                                           \
    PFN_DSTO_STOP                   StopEngines;                                            \
                                                                                            \
    PFN_DSTO_GET_SOCKET1            GetSocketByAddress;                                     \
    PFN_DSTO_GET_SOCKET2            GetSocketByOsocket;                                     \
    PFN_DSTO_ACQUIRE                AcquireSocket;                                          \
    PFN_DSTO_RELEASE                ReleaseSocket;                                          \
    PFN_DSTO_MANUFACTURE            ManufactureSocketPool;                                  \
    PFN_DSTO_DESTROY                DestroySocketPool;                                      \
                                                                                            \
    PFN_DSTO_ASSIGN                 AssignEngine;                                           \
    PFN_DSTO_MANUFACTURE            ManufactureEnginePool;                                  \
    PFN_DSTO_DESTROY                DestroyEnginePool;                                      \
                                                                                            \
    PFN_DSTO_QUERY                  Query;                                                  \
    PFN_DSTO_PROCESS                ProcessSync;                                            \
    PFN_DSTO_PROCESS                ProcessAsync;                                           \
                                                                                            \
    PFN_SDTOPO_OVERVIEW             PoOverview;                                             \
    PFN_SDTOPO_SNAPSHOT             PoSnapshot;                                             \
    PFN_SDTOPO_TRACE                PoTraceTransaction;                                     \
    PFN_SDTOPO_CLOSE_UP             PoEngineCloseUp;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_DAEMON_SERVER_TCP_OBJECT
{
    ANSC_DAEMON_SERVER_TCP_CLASS_CONTENT
}
ANSC_DAEMON_SERVER_TCP_OBJECT,  *PANSC_DAEMON_SERVER_TCP_OBJECT;

#define  ACCESS_ANSC_DAEMON_SERVER_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_DAEMON_SERVER_TCP_OBJECT, Linkage)


/***********************************************************
    PACKET OBJECT DEFINITION FOR DAEMON SERVER TCP OBJECT
***********************************************************/

/*
 * When each individual Daemon Engine Object processes the incoming message sequentially, the
 * smallest processing unit is message. It means that engine CANNOT switch to processing another
 * pending socket while the message-processing is progress. The benefit of multi-tasking is
 * achieved by the fact that multiple engines coexist in the system. Nonetheless, it MIGHT be
 * desirable for the engine to work on new requests while one particular message-processing may
 * take a long time.
 */
typedef  struct
_ANSC_DSTO_PACKET_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hServer;
    ANSC_HANDLE                     hSocket;
    PVOID                           PacketBuffer;
    ULONG                           PacketSize;
}
ANSC_DSTO_PACKET_OBJECT,  *PANSC_DSTO_PACKET_OBJECT;

#define  ACCESS_ANSC_DSTO_PACKET_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_DSTO_PACKET_OBJECT, Linkage)

/*
 * If the SSL/TLS is enabled, we need to complete SSL/TLS handshake negotiation before notifying
 * the worker that a new socket connection has been established. Since the TLS handshake may take
 * quite sometime to complete, we should spawn a separate task to do just that instead of blocking
 * the 'accept' task.
 */
#define  ANSC_DSTO_JOB_TYPE_initTlsConnection       1

typedef  struct
_ANSC_DSTO_ASYNC_JOB
{
    ULONG                           JobType;
    ANSC_HANDLE                     hThisObject;
    ANSC_HANDLE                     hSocket;
}
ANSC_DSTO_ASYNC_JOB,  *PANSC_DSTO_ASYNC_JOB;


#endif
