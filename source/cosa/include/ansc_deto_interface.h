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

    module:	ansc_deto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Daemon Engine Tcp Object.

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


#ifndef  _ANSC_DETO_INTERFACE_
#define  _ANSC_DETO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_platform.h"
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT DAEMON ENGINE TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DETO_SOCKET_SET_SIZE                  1024
#define  ANSC_DETO_MAX_SOCKET_COUNT                 64          /* default FD_SETSIZE is 64 on MS windows */
#define  ANSC_DETO_SOCKET_TABLE_SIZE                64
#define  ANSC_DETO_RECV_TASK_NAME                   "ansc_deto_recv_task"
#define  ANSC_DETO_SEND_TASK_NAME                   "ansc_deto_send_task"
#define  ANSC_DETO_RECV_TASK_STACK_SIZE             2*ANSC_DEFAULT_TASK_STACK_SIZE

#define  ANSC_DETO_WAIT_EVENT_INTERVAL              500         /* in milliseconds */
#define  ANSC_DETO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_DETO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_DETO_POLL_INTERVAL_MS                 1000        /* in milliseconds */
#define  ANSC_DETO_CLEAN_TASK_INTERVAL              3           /* in seconds      */
#define  ANSC_DETO_SOCKET_TIMEOUT                   45          /* in seconds      */
#define  ANSC_DETO_SOCKET_TIMEOUT2                  5           /* in seconds      */

#define  ANSC_DETO_FLAG_ASYNC_SEND                  0x00000001
#define  ANSC_DETO_FLAG_NO_TIMEOUT                  0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DETO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_DETO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ULONG
(*PFN_DETO_GET_FLAGS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_SET_FLAGS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

typedef  ANSC_STATUS
(*PFN_DETO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DETO_GET_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_DETO_GET_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

typedef  ANSC_STATUS
(*PFN_DETO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DETO_DEL_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DETO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_EXP_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_ENABLE_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_DETO_ENABLE_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_DETO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DETO_CLEAN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DETO_SNAPSHOT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

typedef  ANSC_STATUS
(*PFN_DETO_CLOSE_UP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

typedef  ANSC_STATUS
(*PFN_DETO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DETO_RECV2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_DETO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_DETO_SEND2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
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
#define  ANSC_DAEMON_ENGINE_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_EVENT                      RecvEvent;                                              \
    ANSC_EVENT                      SendEvent;                                              \
    ANSC_EVENT                      NewSocketEvent;                                         \
    ULONG                           CurSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           ControlFlags;                                           \
    ULONG                           StartTime;                                              \
    ULONG                           AvgServingTime;                                         \
    ULONG                           SocketTimeOut;                                          \
    BOOL                            bStarted;                                               \
    BOOL                            bCleaningDemanded;                                      \
    BOOL                            bBusy;                                                  \
                                                                                            \
    ULONG                           TtcCount;                                               \
    ULONG                           MtcCount;                                               \
    ULONG                           TrcCount;                                               \
    ULONG                           MrcCount;                                               \
    ULONG                           TscCount;                                               \
    ULONG                           MscCount;                                               \
                                                                                            \
    char                            RecvSocketSet[ANSC_DETO_SOCKET_SET_SIZE];               \
    ANSC_LOCK                       RecvSocketSetLock;                                      \
    char                            SendSocketSet[ANSC_DETO_SOCKET_SET_SIZE];               \
    ANSC_LOCK                       SendSocketSetLock;                                      \
                                                                                            \
    SLIST_HEADER                    SocketTable[ANSC_DETO_SOCKET_TABLE_SIZE];               \
    ANSC_LOCK                       SocketTableLock;                                        \
    QUEUE_HEADER                    PacketQueue;                                            \
    ANSC_LOCK                       PacketQueueLock;                                        \
                                                                                            \
    PFN_DETO_GET_CONTEXT            GetDaemonServer;                                        \
    PFN_DETO_SET_CONTEXT            SetDaemonServer;                                        \
    PFN_DETO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_DETO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_DETO_GET_FLAGS              GetControlFlags;                                        \
    PFN_DETO_SET_FLAGS              SetControlFlags;                                        \
    PFN_DETO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_DETO_GET_SOCKET1            GetSocketByAddress;                                     \
    PFN_DETO_GET_SOCKET1            GetSocketByAddress2;                                    \
    PFN_DETO_GET_SOCKET2            GetSocketByOsocket;                                     \
    PFN_DETO_GET_SOCKET2            GetSocketByOsocket2;                                    \
    PFN_DETO_ADD_SOCKET             AddSocket;                                              \
    PFN_DETO_DEL_SOCKET             DelSocket;                                              \
    PFN_DETO_DEL_ALL                DelAllSockets;                                          \
    PFN_DETO_EXP_ALL                ExpAllSockets;                                          \
                                                                                            \
    PFN_DETO_ENABLE_RECV            EnableRecv;                                             \
    PFN_DETO_ENABLE_SEND            EnableSend;                                             \
                                                                                            \
    PFN_DETO_ASYNC_TASK             RecvTask;                                               \
    PFN_DETO_ASYNC_TASK             SendTask;                                               \
                                                                                            \
    PFN_DETO_START                  Start;                                                  \
    PFN_DETO_STOP                   Stop;                                                   \
    PFN_DETO_CANCEL                 Cancel;                                                 \
    PFN_DETO_CLEAN                  Clean;                                                  \
                                                                                            \
    PFN_DETO_SNAPSHOT               Snapshot;                                               \
    PFN_DETO_CLOSE_UP               CloseUp;                                                \
                                                                                            \
    PFN_DETO_RECV                   Recv;                                                   \
    PFN_DETO_RECV2                  Recv2;                                                  \
    PFN_DETO_SEND                   Send;                                                   \
    PFN_DETO_SEND2                  Send2;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_DAEMON_ENGINE_TCP_OBJECT
{
    ANSC_DAEMON_ENGINE_TCP_CLASS_CONTENT
}
ANSC_DAEMON_ENGINE_TCP_OBJECT,  *PANSC_DAEMON_ENGINE_TCP_OBJECT;

#define  ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_DAEMON_ENGINE_TCP_OBJECT, Linkage)


/***********************************************************
    PACKET OBJECT DEFINITION FOR DAEMON ENGINE TCP OBJECT
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
_ANSC_DETO_PACKET_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hServer;
    ANSC_HANDLE                     hEngine;
    ANSC_HANDLE                     hSocket;
    ANSC_HANDLE                     hWorkerReserved;
    PVOID                           PacketBuffer;
    ULONG                           PacketSize;
}
ANSC_DETO_PACKET_OBJECT,  *PANSC_DETO_PACKET_OBJECT;

#define  ACCESS_ANSC_DETO_PACKET_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_DETO_PACKET_OBJECT, Linkage)


#endif
