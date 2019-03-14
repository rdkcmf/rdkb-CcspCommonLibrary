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

    module:	ansc_scgo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Client Gtp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/17/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCGO_INTERFACE_
#define  _ANSC_SCGO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"
#include "ansc_scgowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SIMPLE CLIENT GTP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SCGO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_SCGO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_SCGO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_SCGO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_SCGO_RECV_TASK_NAME                   "ansc_scgo_recv_task"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCGO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SCGO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SCGO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SCGO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_SCGO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  UCHAR
(*PFN_SCGO_GET_PROT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_PROT)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       protocol
    );

typedef  PUCHAR
(*PFN_SCGO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  ANSC_STATUS
(*PFN_SCGO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_SCGO_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_SCGO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCGO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

/*
 * Gtp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Gtp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Gtp Object MUST operate in a multi-tasking capable environment.
 * It opens a Gtp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_SIMPLE_CLIENT_GTP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    UCHAR                           ProtType;                                               \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
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
    PFN_SCGO_GET_PROT               GetProtType;                                            \
    PFN_SCGO_SET_PROT               SetProtType;                                            \
    PFN_SCGO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SCGO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SCGO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_SCGO_SET_ADDRESS            SetPeerAddress;                                         \
                                                                                            \
    PFN_SCGO_GET_WORKER             GetWorker;                                              \
    PFN_SCGO_SET_WORKER             SetWorker;                                              \
    PFN_SCGO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SCGO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SCGO_GET_MODE               GetMode;                                                \
    PFN_SCGO_SET_MODE               SetMode;                                                \
    PFN_SCGO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SCGO_ASYNC_TASK             RecvTask;                                               \
                                                                                            \
    PFN_SCGO_ENGAGE                 Engage;                                                 \
    PFN_SCGO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_SCGO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_SCGO_RECV                   Recv;                                                   \
    PFN_SCGO_SEND                   Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLE_CLIENT_GTP_OBJECT
{
    ANSC_SIMPLE_CLIENT_GTP_CLASS_CONTENT
}
ANSC_SIMPLE_CLIENT_GTP_OBJECT,  *PANSC_SIMPLE_CLIENT_GTP_OBJECT;

#define  ACCESS_ANSC_SIMPLE_CLIENT_GTP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_CLIENT_GTP_OBJECT, Linkage)


#endif
