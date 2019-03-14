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

    module:	ansc_bsuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Server Udp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_BSUO_INTERFACE_
#define  _ANSC_BSUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_bsuowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT BROKER SERVER UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BSUO_MIN_SOCKET_COUNT                 128
#define  ANSC_BSUO_MAX_SOCKET_COUNT                 64 * 32
#define  ANSC_BSUO_MIN_PACKET_COUNT                 128
#define  ANSC_BSUO_MAX_PACKET_COUNT                 1024
#define  ANSC_BSUO_PACKET_TABLE_SIZE                64
#define  ANSC_BSUO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_BSUO_DEF_ENGINE_COUNT                 32
#define  ANSC_BSUO_MAX_ENGINE_COUNT                 128
#define  ANSC_BSUO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_BSUO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_BSUO_CLEAN_TASK_INTERVAL              10          /* in seconds      */
#define  ANSC_BSUO_SOCKET_TIMEOUT                   90          /* in seconds      */
#define  ANSC_BSUO_PACKET_TIMEOUT                   60          /* in seconds      */

#define  ANSC_BSUO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_BSUO_ACCEPT_TASK_NAME                 "ansc_bsuo_accept_task"
#define  ANSC_BSUO_WORKER_TASK_NAME                 "ansc_bsuo_worker_task"

#define  ANSC_BSUO_MODE_ASYNC_SEND                  0x00000001
#define  ANSC_BSUO_MODE_NO_TIMEOUT                  0x00000002
#define  ANSC_BSUO_MODE_EVENT_SYNC                  0x00000004
#define  ANSC_BSUO_MODE_AUTO_CLOSE                  0x00000008

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BSUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_BSUO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_BSUO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_BSUO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_BSUO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_BSUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_BSUO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_BSUO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_BSUO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_ADD_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      server_addr,
        PUCHAR                      client_addr,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_BSUO_DEL_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_BSUO_DEL_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BSUO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResource
    );

typedef  ANSC_STATUS
(*PFN_BSUO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSUO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BSUO_ASSIGN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

typedef  ULONG
(*PFN_BSUO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_BSUO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

/*
 * Udp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Udp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Udp Object MUST operate in a multi-tasking capable environment.
 * It opens a Udp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BROKER_SERVER_UDP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
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
    ULONG                           CurPacketCount;                                         \
    ULONG                           MinPacketCount;                                         \
    ULONG                           MaxPacketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ULONG                           PacketTimeOut;                                          \
    ANSC_LOCK                       OpLock;                                                 \
    ANSC_EVENT                      AcceptEvent;                                            \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     EngineArray[ANSC_BSUO_MAX_ENGINE_COUNT];                \
    ANSC_LOCK                       EngineArrayLock;                                        \
    SLIST_HEADER                    SocketSList;                                            \
    ANSC_LOCK                       SocketSListLock;                                        \
    SLIST_HEADER                    PacketSList;                                            \
    ANSC_LOCK                       PacketSListLock;                                        \
                                                                                            \
    PFN_BSUO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_BSUO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_BSUO_GET_PORT               GetHostPort;                                            \
    PFN_BSUO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_BSUO_GET_WORKER             GetWorker;                                              \
    PFN_BSUO_SET_WORKER             SetWorker;                                              \
    PFN_BSUO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_BSUO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_BSUO_GET_COUNT              GetEngineCount;                                         \
    PFN_BSUO_SET_COUNT              SetEngineCount;                                         \
    PFN_BSUO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_BSUO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_BSUO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_BSUO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_BSUO_GET_COUNT              GetMinPacketCount;                                      \
    PFN_BSUO_SET_COUNT              SetMinPacketCount;                                      \
    PFN_BSUO_GET_COUNT              GetMaxPacketCount;                                      \
    PFN_BSUO_SET_COUNT              SetMaxPacketCount;                                      \
    PFN_BSUO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_BSUO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_BSUO_GET_COUNT              GetPacketTimeOut;                                       \
    PFN_BSUO_SET_COUNT              SetPacketTimeOut;                                       \
    PFN_BSUO_GET_MODE               GetMode;                                                \
    PFN_BSUO_SET_MODE               SetMode;                                                \
    PFN_BSUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_BSUO_ASYNC_TASK             AcceptTask;                                             \
    PFN_BSUO_ASYNC_TASK             WorkerTask;                                             \
                                                                                            \
    PFN_BSUO_ENGAGE                 Engage;                                                 \
    PFN_BSUO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_BSUO_START                  StartEngines;                                           \
    PFN_BSUO_STOP                   StopEngines;                                            \
                                                                                            \
    PFN_BSUO_GET_SOCKET             GetSocket;                                              \
    PFN_BSUO_ADD_SOCKET             AddSocket;                                              \
    PFN_BSUO_ADD_SOCKET2            AddSocket2;                                             \
    PFN_BSUO_DEL_SOCKET1            DelSocketByAddr;                                        \
    PFN_BSUO_DEL_SOCKET2            DelSocket;                                              \
    PFN_BSUO_DEL_ALL                DelAllSockets;                                          \
                                                                                            \
    PFN_BSUO_ACQUIRE                AcquireSocket;                                          \
    PFN_BSUO_RELEASE                ReleaseSocket;                                          \
    PFN_BSUO_MANUFACTURE            ManufactureSocketPool;                                  \
    PFN_BSUO_DESTROY                DestroySocketPool;                                      \
                                                                                            \
    PFN_BSUO_ACQUIRE                AcquirePacket;                                          \
    PFN_BSUO_RELEASE                ReleasePacket;                                          \
    PFN_BSUO_MANUFACTURE            ManufacturePacketPool;                                  \
    PFN_BSUO_DESTROY                DestroyPacketPool;                                      \
                                                                                            \
    PFN_BSUO_ASSIGN                 AssignEngine;                                           \
    PFN_BSUO_MANUFACTURE            ManufactureEnginePool;                                  \
    PFN_BSUO_DESTROY                DestroyEnginePool;                                      \
                                                                                            \
    PFN_BSUO_QUERY                  Query;                                                  \
    PFN_BSUO_PROCESS                ProcessSync;                                            \
    PFN_BSUO_PROCESS                ProcessAsync;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BROKER_SERVER_UDP_OBJECT
{
    ANSC_BROKER_SERVER_UDP_CLASS_CONTENT
}
ANSC_BROKER_SERVER_UDP_OBJECT,  *PANSC_BROKER_SERVER_UDP_OBJECT;

#define  ACCESS_ANSC_BROKER_SERVER_UDP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_SERVER_UDP_OBJECT, Linkage)


/***********************************************************
    PACKET OBJECT DEFINITION FOR BROKER SERVER UDP OBJECT
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
_ANSC_BSUO_PACKET_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hServer;
    ANSC_HANDLE                     hEngine;
    ANSC_HANDLE                     hSocket;
    ANSC_IPV4_ADDRESS               PeerAddress;
    USHORT                          PeerPort;

    PVOID                           RecvBuffer;
    ULONG                           RecvBufferSize;
    ULONG                           RecvPacketSize;
    ULONG                           RecvAt;
}
ANSC_BSUO_PACKET_OBJECT,  *PANSC_BSUO_PACKET_OBJECT;

#define  ACCESS_ANSC_BSUO_PACKET_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_BSUO_PACKET_OBJECT, Linkage)


#endif
