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

    module:	ansc_dkto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Daemon Socket Tcp Object.

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


#ifndef  _ANSC_DKTO_INTERFACE_
#define  _ANSC_DKTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#ifndef _ANSC_USE_OPENSSL_
#include "tls_ifo_tsa.h"
#include "tls_ifo_scs.h"
#endif

/***********************************************************
  PLATFORM INDEPENDENT DAEMON SOCKET TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DKTO_MAX_BUFFER_SIZE                  2048

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DKTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DKTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_DKTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  PUCHAR
(*PFN_DKTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_DKTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  BOOL
(*PFN_DKTO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue,
        ULONG                       ulSeconds
    );

typedef  ANSC_HANDLE
(*PFN_DKTO_GET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

typedef  ANSC_STATUS
(*PFN_DKTO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_FINISH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_INIT_TLS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_OPEN_TLS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_LOCK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKTO_UNLOCK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_DKTO_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

typedef  PVOID
(*PFN_DKTO_GET_SEND_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_DKTO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_DKTO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
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
#ifndef _ANSC_USE_OPENSSL_
#define  ANSC_DAEMON_SOCKET_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    char                            HostAddr[INET6_ADDRSTRLEN];                             \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    char                            PeerAddr[INET6_ADDRSTRLEN];                             \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
    ULONG                           HashIndex;                                              \
                                                                                            \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hDaemonEngine;                                          \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
    ULONG                           StartTime;                                              \
    BOOL                            bClosed;                                                \
    BOOL                            bBroken;                                                \
    BOOL                            bToBeCleaned;                                           \
    BOOL                            bLocked;                                                \
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
    BOOL                            bTlsInitializing;                                       \
    BOOL                            bTlsConnected;                                          \
    ANSC_EVENT                      TlsConnEvent;                                           \
    ANSC_LOCK                       TlsConnLock;                                            \
                                                                                            \
    PFN_DKTO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_DKTO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_DKTO_GET_PORT               GetPeerPort;                                            \
    PFN_DKTO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_DKTO_GET_CONTEXT            GetDaemonServer;                                        \
    PFN_DKTO_SET_CONTEXT            SetDaemonServer;                                        \
    PFN_DKTO_GET_CONTEXT            GetDaemonEngine;                                        \
    PFN_DKTO_SET_CONTEXT            SetDaemonEngine;                                        \
    PFN_DKTO_GET_CONTEXT            GetClientContext;                                       \
    PFN_DKTO_SET_CONTEXT            SetClientContext;                                       \
    PFN_DKTO_GET_SIZE               GetBufferSize;                                          \
    PFN_DKTO_SET_SIZE               SetBufferSize;                                          \
    PFN_DKTO_GET_BUF_CONTEXT        GetBufferContext;                                       \
    PFN_DKTO_SET_BUF_CONTEXT        SetBufferContext;                                       \
    PFN_DKTO_GET_IF                 GetTlsScsIf;                                            \
    PFN_DKTO_SET_IF                 SetTlsScsIf;                                            \
    PFN_DKTO_GET_BOOL               IsToBeCleaned;                                          \
    PFN_DKTO_SET_BOOL               ToClean;                                                \
    PFN_DKTO_RETURN                 Return;                                                 \
    PFN_DKTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_DKTO_FINISH                 Finish;                                                 \
    PFN_DKTO_OPEN                   Open;                                                   \
    PFN_DKTO_CLOSE                  Close;                                                  \
    PFN_DKTO_INIT_TLS               InitTlsServer;                                          \
    PFN_DKTO_OPEN_TLS               OpenTlsServer;                                          \
    PFN_DKTO_LOCK                   Lock;                                                   \
    PFN_DKTO_UNLOCK                 Unlock;                                                 \
    PFN_DKTO_RESET                  ResetTimestamp;                                         \
                                                                                            \
    PFN_DKTO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_DKTO_RECV                   Recv;                                                   \
    PFN_DKTO_SEND                   Send;                                                   \
                                                                                            \
    PFN_TLSTSAIF_RECV               TsaRecvAppMessage;                                      \
    PFN_TLSTSAIF_SEND               TsaSendTlsMessage;                                      \
    PFN_TLSTSAIF_NOTIFY             TsaNotifyEvent;                                         \
    /* end of object class content */                                                       \

#else
#define  ANSC_DAEMON_SOCKET_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    char                            HostAddr[INET6_ADDRSTRLEN];                             \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    char                            PeerAddr[INET6_ADDRSTRLEN];                             \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
    ULONG                           HashIndex;                                              \
                                                                                            \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hDaemonEngine;                                          \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
    ULONG                           StartTime;                                              \
    BOOL                            bClosed;                                                \
    BOOL                            bBroken;                                                \
    BOOL                            bToBeCleaned;                                           \
    BOOL                            bLocked;                                                \
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
    BOOL                            bTlsInitializing;                                       \
    BOOL                            bTlsConnected;                                          \
    ANSC_EVENT                      TlsConnEvent;                                           \
    ANSC_LOCK                       TlsConnLock;                                            \
                                                                                            \
    PFN_DKTO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_DKTO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_DKTO_GET_PORT               GetPeerPort;                                            \
    PFN_DKTO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_DKTO_GET_CONTEXT            GetDaemonServer;                                        \
    PFN_DKTO_SET_CONTEXT            SetDaemonServer;                                        \
    PFN_DKTO_GET_CONTEXT            GetDaemonEngine;                                        \
    PFN_DKTO_SET_CONTEXT            SetDaemonEngine;                                        \
    PFN_DKTO_GET_CONTEXT            GetClientContext;                                       \
    PFN_DKTO_SET_CONTEXT            SetClientContext;                                       \
    PFN_DKTO_GET_SIZE               GetBufferSize;                                          \
    PFN_DKTO_SET_SIZE               SetBufferSize;                                          \
    PFN_DKTO_GET_BUF_CONTEXT        GetBufferContext;                                       \
    PFN_DKTO_SET_BUF_CONTEXT        SetBufferContext;                                       \
    PFN_DKTO_GET_IF                 GetTlsScsIf;                                            \
    PFN_DKTO_SET_IF                 SetTlsScsIf;                                            \
    PFN_DKTO_GET_BOOL               IsToBeCleaned;                                          \
    PFN_DKTO_SET_BOOL               ToClean;                                                \
    PFN_DKTO_RETURN                 Return;                                                 \
    PFN_DKTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_DKTO_FINISH                 Finish;                                                 \
    PFN_DKTO_OPEN                   Open;                                                   \
    PFN_DKTO_CLOSE                  Close;                                                  \
    PFN_DKTO_INIT_TLS               InitTlsServer;                                          \
    PFN_DKTO_OPEN_TLS               OpenTlsServer;                                          \
    PFN_DKTO_LOCK                   Lock;                                                   \
    PFN_DKTO_UNLOCK                 Unlock;                                                 \
    PFN_DKTO_RESET                  ResetTimestamp;                                         \
                                                                                            \
    PFN_DKTO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_DKTO_RECV                   Recv;                                                   \
    PFN_DKTO_SEND                   Send;                                                   \

#endif
typedef  struct
_ANSC_DAEMON_SOCKET_TCP_OBJECT
{
    ANSC_DAEMON_SOCKET_TCP_CLASS_CONTENT
}
ANSC_DAEMON_SOCKET_TCP_OBJECT,  *PANSC_DAEMON_SOCKET_TCP_OBJECT;

#define  ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_DAEMON_SOCKET_TCP_OBJECT, Linkage)


#endif
