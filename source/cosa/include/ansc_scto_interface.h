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

    module:	ansc_scto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Client Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/11/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCTO_INTERFACE_
#define  _ANSC_SCTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"
#include "ansc_sctowo_interface.h"

#ifndef _ANSC_USE_OPENSSL_
#include "tls_ifo_tsa.h"
#include "tls_ifo_scs.h"
#endif

#include "web_properties.h"

/***********************************************************
  PLATFORM INDEPENDENT SIMPLE CLIENT TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SCTO_MAX_MESSAGE_SIZE                 2048

#define  ANSC_SCTO_TASK_BREAK_INTERVAL              200         /* in milliseconds */
#define  ANSC_SCTO_TASK_CLEANUP_TIME                3000        /* in milliseconds */
#define  ANSC_SCTO_POLL_INTERVAL_MS                 1000        /* in milliseconds */

#define  ANSC_SCTO_RECV_TASK_NAME                   "ansc_scto_recv_task"

#define  ANSC_SCTO_MODE_ASYNC_SEND                  0x00000001
#define  ANSC_SCTO_MODE_FOREIGN_BUFFER              0x00000002
#define  ANSC_SCTO_MODE_TLS_ENABLED                 0x00000004
#define  ANSC_SCTO_MODE_XSOCKET                     0x00000008
#define  ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR      0x00000010

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SCTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_SCTO_GET_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_WORKER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SCTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_HANDLE
(*PFN_SCTO_GET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

typedef  ULONG
(*PFN_SCTO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_SCTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_SCTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  BOOL
(*PFN_SCTO_GET_BINDTODEVICE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_BINDTODEVICE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue
    );

typedef  char*
(*PFN_SCTO_GET_DEVICENAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_DEVICENAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDeviceName
    );

typedef  PUCHAR
(*PFN_SCTO_GET_PEER_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_PEER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPeerName
    );

typedef  USHORT
(*PFN_SCTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_SCTO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCTO_INIT_TLS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_SCTO_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_SCTO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCTO_RECV2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPayloadBdo
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SEND2)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  struct
_HTTP_SCTO_HOST_NAMES
{
	BOOL peerVerify;
        char **hostNames;
        int numHosts;
}
HTTP_SCTO_HOST_NAMES, *PHTTP_SCTO_HOST_NAMES;

typedef  ANSC_STATUS
(*PFN_SCTO_GET_HOSTNAMES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
    );

typedef  ANSC_STATUS
(*PFN_SCTO_SET_HOSTNAMES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
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

#ifndef _ANSC_USE_OPENSSL_

#define  ANSC_SIMPLE_CLIENT_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    BOOL                            bSocketBindToDevice;                                    \
    char                            SocketDeviceName[ANSC_OBJ_IF_NAME_SIZE];                \
                                                                                            \
    /* used to check certificate validity */                                                \
    char                            HostName[WEB_MAX_HOST_NAME_SIZE];                       \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    char                            PeerName[ANSC_DOMAIN_NAME_SIZE];                        \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hWorker;                                                \
    ULONG                           MaxMessageSize;                                         \
    ANSC_LOCK                       OpLock;                                                 \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
    BOOL                            bClosed;                                                \
    BOOL                            bBroken;                                                \
    ULONG                           LastEvent;                                              \
    ULONG                           LastError;                                              \
                                                                                            \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
                                                                                            \
    ANSC_HANDLE                     hRecvBufferTag;                                         \
    PVOID                           RecvBuffer;                                             \
    ULONG                           RecvBufferSize;                                         \
    ULONG                           RecvPacketSize;                                         \
    ULONG                           RecvOffset;                                             \
                                                                                            \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    ANSC_HANDLE                     hTlsScsIf;                                              \
    ANSC_HANDLE                     hTlsConnection;                                         \
    BOOL                            bTlsEnabled;                                            \
    BOOL                            bTlsConnected;                                          \
    ANSC_EVENT                      TlsConnEvent;                                           \
                                                                                            \
    PFN_SCTO_GET_BINDTODEVICE       GetSocketBindToDevice;                                  \
    PFN_SCTO_SET_BINDTODEVICE       SetSocketBindToDevice;                                  \
    PFN_SCTO_GET_DEVICENAME         GetSocketDeviceName;                                    \
    PFN_SCTO_SET_DEVICENAME         SetSocketDeviceName;                                    \
                                                                                            \
    PFN_SCTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SCTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SCTO_GET_PORT               GetHostPort;                                            \
    PFN_SCTO_SET_PORT               SetHostPort;                                            \
    PFN_SCTO_GET_PEER_NAME          GetPeerName;                                            \
    PFN_SCTO_SET_PEER_NAME          SetPeerName;                                            \
    PFN_SCTO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_SCTO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_SCTO_GET_PORT               GetPeerPort;                                            \
    PFN_SCTO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_SCTO_GET_WORKER             GetWorker;                                              \
    PFN_SCTO_SET_WORKER             SetWorker;                                              \
    PFN_SCTO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SCTO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SCTO_GET_BUF_CONTEXT        GetBufferContext;                                       \
    PFN_SCTO_SET_BUF_CONTEXT        SetBufferContext;                                       \
    PFN_SCTO_GET_MODE               GetMode;                                                \
    PFN_SCTO_SET_MODE               SetMode;                                                \
    PFN_SCTO_GET_IF                 GetTlsScsIf;                                            \
    PFN_SCTO_SET_IF                 SetTlsScsIf;                                            \
    PFN_SCTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SCTO_ASYNC_TASK             RecvTask;                                               \
                                                                                            \
    PFN_SCTO_ENGAGE                 Engage;                                                 \
    PFN_SCTO_CANCEL                 Cancel;                                                 \
    PFN_SCTO_INIT_TLS               InitTlsClient;                                          \
                                                                                            \
    PFN_SCTO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_SCTO_RECV                   Recv;                                                   \
    PFN_SCTO_RECV2                  Recv2;                                                  \
    PFN_SCTO_SEND                   Send;                                                   \
    PFN_SCTO_SEND2                  Send2;                                                  \
                                                                                            \
    PFN_TLSTSAIF_RECV               TsaRecvAppMessage;                                      \
    PFN_TLSTSAIF_SEND               TsaSendTlsMessage;                                      \
    PFN_TLSTSAIF_NOTIFY             TsaNotifyEvent;                                         \
    /* end of object class content */                                                       \

#else
#define  ANSC_SIMPLE_CLIENT_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    BOOL                            bSocketBindToDevice;                                    \
    char                            SocketDeviceName[ANSC_OBJ_IF_NAME_SIZE];                \
                                                                                            \
    /* used to check certificate validity */                                                \
    char                            HostName[WEB_MAX_HOST_NAME_SIZE+1];                     \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    char                            PeerName[ANSC_DOMAIN_NAME_SIZE];                        \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
                                                                                            \
    ANSC_HANDLE                     hWorker;                                                \
    ULONG                           MaxMessageSize;                                         \
    ANSC_LOCK                       OpLock;                                                 \
    ULONG                           Mode;                                                   \
    BOOL                            bActive;                                                \
    BOOL                            bClosed;                                                \
    BOOL                            bBroken;                                                \
    ULONG                           LastEvent;                                              \
    ULONG                           LastError;                                              \
											    \
    HTTP_SCTO_HOST_NAMES	    hostNames;						    \
                                                                                            \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
                                                                                            \
    ANSC_HANDLE                     hRecvBufferTag;                                         \
    PVOID                           RecvBuffer;                                             \
    ULONG                           RecvBufferSize;                                         \
    ULONG                           RecvPacketSize;                                         \
    ULONG                           RecvOffset;                                             \
                                                                                            \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    ANSC_HANDLE                     hTlsScsIf;                                              \
    ANSC_HANDLE                     hTlsConnection;                                         \
    BOOL                            bTlsEnabled;                                            \
    BOOL                            bTlsConnected;                                          \
    ANSC_EVENT                      TlsConnEvent;                                           \
                                                                                            \
    PFN_SCTO_GET_BINDTODEVICE       GetSocketBindToDevice;                                  \
    PFN_SCTO_SET_BINDTODEVICE       SetSocketBindToDevice;                                  \
    PFN_SCTO_GET_DEVICENAME         GetSocketDeviceName;                                    \
    PFN_SCTO_SET_DEVICENAME         SetSocketDeviceName;                                    \
                                                                                            \
    PFN_SCTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_SCTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_SCTO_GET_PORT               GetHostPort;                                            \
    PFN_SCTO_SET_PORT               SetHostPort;                                            \
    PFN_SCTO_GET_PEER_NAME          GetPeerName;                                            \
    PFN_SCTO_SET_PEER_NAME          SetPeerName;                                            \
    PFN_SCTO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_SCTO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_SCTO_GET_PORT               GetPeerPort;                                            \
    PFN_SCTO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_SCTO_GET_HOSTNAMES          GetHostNames;                                          \
    PFN_SCTO_SET_HOSTNAMES          SetHostNames;                                          \
											    \
    PFN_SCTO_GET_WORKER             GetWorker;                                              \
    PFN_SCTO_SET_WORKER             SetWorker;                                              \
    PFN_SCTO_GET_SIZE               GetMaxMessageSize;                                      \
    PFN_SCTO_SET_SIZE               SetMaxMessageSize;                                      \
    PFN_SCTO_GET_BUF_CONTEXT        GetBufferContext;                                       \
    PFN_SCTO_SET_BUF_CONTEXT        SetBufferContext;                                       \
    PFN_SCTO_GET_MODE               GetMode;                                                \
    PFN_SCTO_SET_MODE               SetMode;                                                \
    PFN_SCTO_GET_IF                 GetTlsScsIf;                                            \
    PFN_SCTO_SET_IF                 SetTlsScsIf;                                            \
    PFN_SCTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SCTO_ASYNC_TASK             RecvTask;                                               \
                                                                                            \
    PFN_SCTO_ENGAGE                 Engage;                                                 \
    PFN_SCTO_CANCEL                 Cancel;                                                 \
    PFN_SCTO_INIT_TLS               InitTlsClient;                                          \
                                                                                            \
    PFN_SCTO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_SCTO_RECV                   Recv;                                                   \
    PFN_SCTO_RECV2                  Recv2;                                                  \
    PFN_SCTO_SEND                   Send;                                                   \
    PFN_SCTO_SEND2                  Send2;                                                  \
                                                                                            \

#endif

typedef  struct
_ANSC_SIMPLE_CLIENT_TCP_OBJECT
{
    ANSC_SIMPLE_CLIENT_TCP_CLASS_CONTENT
}
ANSC_SIMPLE_CLIENT_TCP_OBJECT,  *PANSC_SIMPLE_CLIENT_TCP_OBJECT;

#define  ACCESS_ANSC_SIMPLE_CLIENT_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_CLIENT_TCP_OBJECT, Linkage)


#endif
