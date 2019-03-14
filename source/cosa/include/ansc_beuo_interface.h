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

    module:	ansc_beuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Engine Udp Object.

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


#ifndef  _ANSC_BEUO_INTERFACE_
#define  _ANSC_BEUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT BROKER ENGINE UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BEUO_SOCKET_SET_SIZE                  1024
#define  ANSC_BEUO_MAX_SOCKET_COUNT                 64          /* default FD_SETSIZE is 64 on MS windows */
#define  ANSC_BEUO_SOCKET_TABLE_SIZE                64
#define  ANSC_BEUO_RECV_TASK_NAME                   "ansc_beuo_recv_task"
#define  ANSC_BEUO_SEND_TASK_NAME                   "ansc_beuo_send_task"

#define  ANSC_BEUO_WAIT_EVENT_INTERVAL              500         /* in milliseconds */
#define  ANSC_BEUO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_BEUO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_BEUO_POLL_INTERVAL_MS                 1000        /* in milliseconds */
#define  ANSC_BEUO_CLEAN_TASK_INTERVAL              10          /* in seconds      */
#define  ANSC_BEUO_SOCKET_TIMEOUT                   90          /* in seconds      */
#define  ANSC_BEUO_PACKET_TIMEOUT                   60          /* in seconds      */

#define  ANSC_BEUO_FLAG_ASYNC_SEND                  0x00000001
#define  ANSC_BEUO_FLAG_NO_TIMEOUT                  0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_BEUO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ULONG
(*PFN_BEUO_GET_FLAGS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_SET_FLAGS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

typedef  ANSC_STATUS
(*PFN_BEUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEUO_GET_PACKET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_ADD_PACKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

typedef  ANSC_STATUS
(*PFN_BEUO_DEL_PACKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

typedef  ANSC_STATUS
(*PFN_BEUO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEUO_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_BEUO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_BEUO_ADD_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      server_addr,
        PUCHAR                      client_addr,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_BEUO_DEL_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_BEUO_DEL_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BEUO_ENABLE_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_BEUO_ENABLE_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_BEUO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BEUO_CLEAN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_BEUO_IS_FULL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEUO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_BEUO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

/*
 * Udp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Udp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Broker Udp Object MUST operate in a multi-tasking capable environment.
 * It opens a Udp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BROKER_ENGINE_UDP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBrokerServer;                                          \
    ANSC_EVENT                      RecvEvent;                                              \
    ANSC_EVENT                      SendEvent;                                              \
    ANSC_EVENT                      NewPacketEvent;                                         \
    ULONG                           CurSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ULONG                           PacketTimeOut;                                          \
    ULONG                           ControlFlags;                                           \
    BOOL                            bStarted;                                               \
                                                                                            \
    QUEUE_HEADER                    PacketQueue;                                            \
    ANSC_LOCK                       PacketQueueLock;                                        \
    SLIST_HEADER                    SocketTable[ANSC_BEUO_SOCKET_TABLE_SIZE];               \
    ANSC_LOCK                       SocketTableLock;                                        \
                                                                                            \
    PFN_BEUO_GET_CONTEXT            GetBrokerServer;                                        \
    PFN_BEUO_SET_CONTEXT            SetBrokerServer;                                        \
    PFN_BEUO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_BEUO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_BEUO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_BEUO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_BEUO_GET_COUNT              GetPacketTimeOut;                                       \
    PFN_BEUO_SET_COUNT              SetPacketTimeOut;                                       \
    PFN_BEUO_GET_FLAGS              GetControlFlags;                                        \
    PFN_BEUO_SET_FLAGS              SetControlFlags;                                        \
    PFN_BEUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_BEUO_GET_PACKET             GetPacket;                                              \
    PFN_BEUO_ADD_PACKET             AddPacket;                                              \
    PFN_BEUO_DEL_PACKET             DelPacket;                                              \
    PFN_BEUO_DEL_ALL                DelAllPackets;                                          \
                                                                                            \
    PFN_BEUO_GET_SOCKET             GetSocket;                                              \
    PFN_BEUO_ADD_SOCKET             AddSocket;                                              \
    PFN_BEUO_ADD_SOCKET2            AddSocket2;                                             \
    PFN_BEUO_DEL_SOCKET1            DelSocketByAddr;                                        \
    PFN_BEUO_DEL_SOCKET2            DelSocket;                                              \
    PFN_BEUO_DEL_ALL                DelAllSockets;                                          \
                                                                                            \
    PFN_BEUO_ENABLE_RECV            EnableRecv;                                             \
    PFN_BEUO_ENABLE_SEND            EnableSend;                                             \
                                                                                            \
    PFN_BEUO_ASYNC_TASK             RecvTask;                                               \
    PFN_BEUO_ASYNC_TASK             SendTask;                                               \
                                                                                            \
    PFN_BEUO_START                  Start;                                                  \
    PFN_BEUO_STOP                   Stop;                                                   \
    PFN_BEUO_CLEAN                  Clean;                                                  \
                                                                                            \
    PFN_BEUO_RECV                   Recv;                                                   \
    PFN_BEUO_SEND                   Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BROKER_ENGINE_UDP_OBJECT
{
    ANSC_BROKER_ENGINE_UDP_CLASS_CONTENT
}
ANSC_BROKER_ENGINE_UDP_OBJECT,  *PANSC_BROKER_ENGINE_UDP_OBJECT;

#define  ACCESS_ANSC_BROKER_ENGINE_UDP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_ENGINE_UDP_OBJECT, Linkage)


#endif
