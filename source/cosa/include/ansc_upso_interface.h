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

    module:	ansc_upso_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Udp Ping Server Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/10/07    initial revision.

**********************************************************************/


#ifndef  _ANSC_UPSO_INTERFACE_
#define  _ANSC_UPSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_dsuowo_interface.h"


/***********************************************************
   PLATFORM INDEPENDENT UDP PING SERVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_UPSO_MIN_SOCKET_COUNT                 8
#define  ANSC_UPSO_MAX_SOCKET_COUNT                 8
#define  ANSC_UPSO_MIN_PACKET_COUNT                 2
#define  ANSC_UPSO_MAX_PACKET_COUNT                 4
#define  ANSC_UPSO_PACKET_TABLE_SIZE                64
#define  ANSC_UPSO_MAX_MESSAGE_SIZE                 65536

#define  ANSC_UPSO_DEF_ENGINE_COUNT                 1
#define  ANSC_UPSO_MAX_ENGINE_COUNT                 4
#define  ANSC_UPSO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_UPSO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_UPSO_CLEAN_TASK_INTERVAL              10          /* in seconds      */
#define  ANSC_UPSO_SOCKET_TIMEOUT                   90          /* in seconds      */
#define  ANSC_UPSO_PACKET_TIMEOUT                   60          /* in seconds      */

#define  ANSC_UPSO_MODE_DAEMON_TIMEOUT              0x00000001

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_UPSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_UPSO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_UPSO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_UPSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_UPSO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_UPSO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ANSC_STATUS
(*PFN_UPSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_UPSO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_UPSO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_UPSO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
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
#define  ANSC_UDP_PING_SERVER_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hDaemonWorker;                                          \
                                                                                            \
    ULONG                           MaxMessageSize;                                         \
    ULONG                           EngineCount;                                            \
    ULONG                           MinSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           MinPacketCount;                                         \
    ULONG                           MaxPacketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ULONG                           PacketTimeOut;                                          \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_UPSO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_UPSO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_UPSO_GET_PORT               GetHostPort;                                            \
    PFN_UPSO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_UPSO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_UPSO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_UPSO_GET_COUNT              GetEngineCount;                                         \
    PFN_UPSO_SET_COUNT              SetEngineCount;                                         \
    PFN_UPSO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_UPSO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_UPSO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_UPSO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_UPSO_GET_COUNT              GetMinPacketCount;                                      \
    PFN_UPSO_SET_COUNT              SetMinPacketCount;                                      \
    PFN_UPSO_GET_COUNT              GetMaxPacketCount;                                      \
    PFN_UPSO_SET_COUNT              SetMaxPacketCount;                                      \
    PFN_UPSO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_UPSO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_UPSO_GET_COUNT              GetPacketTimeOut;                                       \
    PFN_UPSO_SET_COUNT              SetPacketTimeOut;                                       \
    PFN_UPSO_GET_MODE               GetMode;                                                \
    PFN_UPSO_SET_MODE               SetMode;                                                \
    PFN_UPSO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_UPSO_ENGAGE                 Engage;                                                 \
    PFN_UPSO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_DSUOWO_INIT                 DwoInit;                                                \
    PFN_DSUOWO_UNLOAD               DwoUnload;                                              \
    PFN_DSUOWO_ACCEPT               DwoAccept;                                              \
    PFN_DSUOWO_REMOVE               DwoRemove;                                              \
    PFN_DSUOWO_QUERY                DwoQuery;                                               \
    PFN_DSUOWO_PROCESS              DwoProcessSync;                                         \
    PFN_DSUOWO_PROCESS              DwoProcessAsync;                                        \
    PFN_DSUOWO_COMPLETE             DwoSendComplete;                                        \
    PFN_DSUOWO_NOTIFY               DwoNotify;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_UDP_PING_SERVER_OBJECT
{
    ANSC_UDP_PING_SERVER_CLASS_CONTENT
}
ANSC_UDP_PING_SERVER_OBJECT,  *PANSC_UDP_PING_SERVER_OBJECT;

#define  ACCESS_ANSC_UDP_PING_SERVER_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_UDP_PING_SERVER_OBJECT, Linkage)


#endif
