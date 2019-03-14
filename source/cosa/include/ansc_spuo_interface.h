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

    module:	ansc_spuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Proxy Udp Object.

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


#ifndef  _ANSC_SPUO_INTERFACE_
#define  _ANSC_SPUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_spuowo_interface.h"
#include "ansc_dsuowo_interface.h"
#include "ansc_bsuowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SIMPLE PROXY UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SPUO_MIN_SOCKET_COUNT                 64
#define  ANSC_SPUO_MAX_SOCKET_COUNT                 64
#define  ANSC_SPUO_MIN_PACKET_COUNT                 128
#define  ANSC_SPUO_MAX_PACKET_COUNT                 128
#define  ANSC_SPUO_PACKET_TABLE_SIZE                64
#define  ANSC_SPUO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_SPUO_DEF_ENGINE_COUNT                 1
#define  ANSC_SPUO_MAX_ENGINE_COUNT                 128
#define  ANSC_SPUO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_SPUO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_SPUO_CLEAN_TASK_INTERVAL              10          /* in seconds      */
#define  ANSC_SPUO_SOCKET_TIMEOUT                   90          /* in seconds      */
#define  ANSC_SPUO_PACKET_TIMEOUT                   60          /* in seconds      */

#define  ANSC_SPUO_MODE_DAEMON_TIMEOUT              0x00000001
#define  ANSC_SPUO_MODE_BROKER_TIMEOUT              0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SPUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SPUO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SPUO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SPUO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_SPUO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_SPUO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ANSC_STATUS
(*PFN_SPUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_SPUO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SPUO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_SPUO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SPUO_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_SPUO_ADD_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_SPUO_ADD_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      server_addr,
        PUCHAR                      client_addr,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_SPUO_DEL_SOCKET1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_SPUO_DEL_SOCKET2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_SPUO_DEL_ALL)
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
#define  ANSC_SIMPLE_PROXY_UDP_CLASS_CONTENT                                                \
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
    ULONG                           MinPacketCount;                                         \
    ULONG                           MaxPacketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
    ULONG                           PacketTimeOut;                                          \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SPUO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SPUO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SPUO_GET_PORT               GetHostPort;                                            \
    PFN_SPUO_SET_PORT               SetHostPort;                                            \
                                                                                            \
    PFN_SPUO_GET_WORKER             GetProxyWorker;                                         \
    PFN_SPUO_SET_WORKER             SetProxyWorker;                                         \
    PFN_SPUO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SPUO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SPUO_GET_COUNT              GetEngineCount;                                         \
    PFN_SPUO_SET_COUNT              SetEngineCount;                                         \
    PFN_SPUO_GET_COUNT              GetMinSocketCount;                                      \
    PFN_SPUO_SET_COUNT              SetMinSocketCount;                                      \
    PFN_SPUO_GET_COUNT              GetMaxSocketCount;                                      \
    PFN_SPUO_SET_COUNT              SetMaxSocketCount;                                      \
    PFN_SPUO_GET_COUNT              GetMinPacketCount;                                      \
    PFN_SPUO_SET_COUNT              SetMinPacketCount;                                      \
    PFN_SPUO_GET_COUNT              GetMaxPacketCount;                                      \
    PFN_SPUO_SET_COUNT              SetMaxPacketCount;                                      \
    PFN_SPUO_GET_COUNT              GetSocketTimeOut;                                       \
    PFN_SPUO_SET_COUNT              SetSocketTimeOut;                                       \
    PFN_SPUO_GET_COUNT              GetPacketTimeOut;                                       \
    PFN_SPUO_SET_COUNT              SetPacketTimeOut;                                       \
    PFN_SPUO_GET_MODE               GetMode;                                                \
    PFN_SPUO_SET_MODE               SetMode;                                                \
    PFN_SPUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SPUO_ENGAGE                 Engage;                                                 \
    PFN_SPUO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_SPUO_GET_SOCKET             PubGetSocket;                                           \
    PFN_SPUO_ADD_SOCKET             PubAddSocket;                                           \
    PFN_SPUO_ADD_SOCKET2            PubAddSocket2;                                          \
    PFN_SPUO_DEL_SOCKET1            PubDelSocketByAddr;                                     \
    PFN_SPUO_DEL_SOCKET2            PubDelSocket;                                           \
    PFN_SPUO_DEL_ALL                PubDelAllSockets;                                       \
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
                                                                                            \
    PFN_BSUOWO_INIT                 BwoInit;                                                \
    PFN_BSUOWO_UNLOAD               BwoUnload;                                              \
    PFN_BSUOWO_REMOVE               BwoRemove;                                              \
    PFN_BSUOWO_QUERY                BwoQuery;                                               \
    PFN_BSUOWO_PROCESS              BwoProcessSync;                                         \
    PFN_BSUOWO_PROCESS              BwoProcessAsync;                                        \
    PFN_BSUOWO_COMPLETE             BwoSendComplete;                                        \
    PFN_BSUOWO_NOTIFY               BwoNotify;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLE_PROXY_UDP_OBJECT
{
    ANSC_SIMPLE_PROXY_UDP_CLASS_CONTENT
}
ANSC_SIMPLE_PROXY_UDP_OBJECT,  *PANSC_SIMPLE_PROXY_UDP_OBJECT;

#define  ACCESS_ANSC_SIMPLE_PROXY_UDP_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_PROXY_UDP_OBJECT, Linkage)


/***********************************************************
   SESSION OBJECT DEFINITION FOR SIMPLE PROXY UDP OBJECT
***********************************************************/

/*
 * The Simple Proxy Object is actually a "glue" object wrapping up a Daemon Server and a Broker
 * Server Object back to back. In many cases, there's a unique one-on-one mapping between a Daemon
 * and a Broker Socket Object. We use following data structure to refer to such a relationship.
 */
typedef  struct
_ANSC_SPUO_SESSION_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hProxy;
    ANSC_HANDLE                     hDaemonSocket;
    ANSC_HANDLE                     hBrokerSocket;
}
ANSC_SPUO_SESSION_OBJECT,  *PANSC_SPUO_SESSION_OBJECT;

#define  ACCESS_ANSC_SPUO_SESSION_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_SPUO_SESSION_OBJECT, Linkage)


#endif
