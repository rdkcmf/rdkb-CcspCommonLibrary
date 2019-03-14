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

    module:	ansc_scuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Client Udp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/12/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCUO_INTERFACE_
#define  _ANSC_SCUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"
#include "ansc_scuowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SIMPLE CLIENT UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SCUO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_SCUO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_SCUO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_SCUO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_SCUO_RECV_TASK_NAME                   "ansc_scuo_recv_task"

#define  ANSC_SCUO_MODE_XSOCKET                     0x00000001

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SCUO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SCUO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SCUO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_SCUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_SCUO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SCUO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_SCUO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCUO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_SCUO_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_SCUO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SCUO_SEND2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPeerAddr,
        USHORT                      usPeerPort,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
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
#define  ANSC_SIMPLE_CLIENT_UDP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hWorker;                                                \
    ULONG                           MaxMessageSize;                                         \
    ANSC_LOCK                       OpLock;                                                 \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
    BOOL                            bClosed;                                                \
                                                                                            \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
                                                                                            \
    PVOID                           RecvBuffer;                                             \
    ULONG                           RecvBufferSize;                                         \
    ULONG                           RecvPacketSize;                                         \
    ULONG                           RecvOffset;                                             \
                                                                                            \
    PFN_SCUO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SCUO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SCUO_GET_PORT               GetHostPort;                                            \
    PFN_SCUO_SET_PORT               SetHostPort;                                            \
    PFN_SCUO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_SCUO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_SCUO_GET_PORT               GetPeerPort;                                            \
    PFN_SCUO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_SCUO_GET_WORKER             GetWorker;                                              \
    PFN_SCUO_SET_WORKER             SetWorker;                                              \
    PFN_SCUO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SCUO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SCUO_GET_MODE               GetMode;                                                \
    PFN_SCUO_SET_MODE               SetMode;                                                \
    PFN_SCUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SCUO_ASYNC_TASK             RecvTask;                                               \
                                                                                            \
    PFN_SCUO_ENGAGE                 Engage;                                                 \
    PFN_SCUO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_SCUO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_SCUO_RECV                   Recv;                                                   \
    PFN_SCUO_SEND                   Send;                                                   \
    PFN_SCUO_SEND2                  Send2;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLE_CLIENT_UDP_OBJECT
{
    ANSC_SIMPLE_CLIENT_UDP_CLASS_CONTENT
}
ANSC_SIMPLE_CLIENT_UDP_OBJECT,  *PANSC_SIMPLE_CLIENT_UDP_OBJECT;

#define  ACCESS_ANSC_SIMPLE_CLIENT_UDP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_CLIENT_UDP_OBJECT, Linkage)


#endif
