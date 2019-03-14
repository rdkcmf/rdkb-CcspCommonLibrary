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

    module:	ansc_socket_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc Socket Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.
        10/06/01    isolate all platform-dependent definitions into
                    separate header files.
        04/15/02    separate from the original ansc_socket.h to
                    accommodate the newly arrived ansc socket
                    library built on our own stack.

**********************************************************************/


#ifndef  _ANSC_SOCKET_INTERFACE_
#define  _ANSC_SOCKET_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"

#include "tls_ifo_tsa.h"
#include "tls_ifo_scs.h"


/***********************************************************
       PLATFORM INDEPENDENT SOCKET OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the socket object definition.
 */
#define  ANSC_SOCKET_TYPE_TCP                       0x00000001
#define  ANSC_SOCKET_TYPE_UDP                       0x00000002
#define  ANSC_SOCKET_TYPE_RAW                       0x00000003

#define  ANSC_SOCKET_MODE_ASYNC_CLOSE               0x00000001
#define  ANSC_SOCKET_MODE_AUTOMATIC_CLOSE           0x00000002
#define  ANSC_SOCKET_MODE_TLS_ENABLED               0x00000004
#define  ANSC_SOCKET_MODE_XSOCKET                   0x00000008

#define  ANSC_SOCKET_RECV_SCRATCH_PAD_SIZE          16384

/* Increase it to 32 in order to be able to respond to Intel Spy's http requests */ 
#define  ANSC_SOCKET_BACKLOG_VALUE                  32        

#define  ANSC_SOCKET_TASK_CLEANUP_TIME              5000
#define  ANSC_SOCKET_POLL_INTERVAL                  1

#define  ANSC_SOCKET_MAX_RECV_RETRY_COUNTS          2
#define  ANSC_SOCKET_RECV_HOLD_BACK                 200

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SOCKET_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_SOCKET_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_SOCKET_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  PUCHAR
(*PFN_SOCKET_GET_HOST_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_HOST_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SOCKET_GET_HOST_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_HOST_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  PUCHAR
(*PFN_SOCKET_GET_PEER_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_PEER_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SOCKET_GET_PEER_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_PEER_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  UCHAR
(*PFN_SOCKET_GET_TRANSPORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_TRANSPORT)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       transportType
    );

typedef  ANSC_HANDLE
(*PFN_SOCKET_GET_SINK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SET_SINK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSink
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_BIND)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_CONNECT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_LISTEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_SEND2)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SOCKET_INIT_TLS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * I'm not so sure whether it's a good idea to encapsulate all the socket-related in a pseudo
 * software object. But for the purpose of isolation, this approach has the benefit of simplicity.
 * The alternative is to simply rename all the socket api functions, but expose the internal data
 * structures to our internal components.
 */
#define  ANSC_SOCKET_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_LOCK                       OpLock;                                                 \
    ANSC_SOCKET                     Socket;                                                 \
    ULONG                           Type;                                                   \
    ULONG                           Mode;                                                   \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
    UCHAR                           TransportType;                                          \
    ANSC_HANDLE                     hSinkHolder;                                            \
    ANSC_HANDLE                     hSinkObject;                                            \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    ANSC_HANDLE                     hTlsScsIf;                                              \
    ANSC_HANDLE                     hTlsConnection;                                         \
    BOOL                            bTlsEnabled;                                            \
    BOOL                            bTlsClient;                                             \
    BOOL                            bTlsConnected;                                          \
    BOOL                            bOpened;                                                \
    BOOL                            bClosed;                                                \
    ANSC_EVENT                      TlsConnEvent;                                           \
                                                                                            \
    PFN_SOCKET_GET_TYPE             GetType;                                                \
    PFN_SOCKET_SET_TYPE             SetType;                                                \
    PFN_SOCKET_GET_MODE             GetMode;                                                \
    PFN_SOCKET_SET_MODE             SetMode;                                                \
    PFN_SOCKET_GET_HOST_ADDRESS     GetHostAddress;                                         \
    PFN_SOCKET_SET_HOST_ADDRESS     SetHostAddress;                                         \
    PFN_SOCKET_GET_HOST_PORT        GetHostPort;                                            \
    PFN_SOCKET_SET_HOST_PORT        SetHostPort;                                            \
    PFN_SOCKET_GET_PEER_ADDRESS     GetPeerAddress;                                         \
    PFN_SOCKET_SET_PEER_ADDRESS     SetPeerAddress;                                         \
    PFN_SOCKET_GET_PEER_PORT        GetPeerPort;                                            \
    PFN_SOCKET_SET_PEER_PORT        SetPeerPort;                                            \
    PFN_SOCKET_GET_TRANSPORT        GetTransportType;                                       \
    PFN_SOCKET_SET_TRANSPORT        SetTransportType;                                       \
    PFN_SOCKET_GET_SINK             GetSink;                                                \
    PFN_SOCKET_SET_SINK             SetSink;                                                \
    PFN_SOCKET_GET_IF               GetTlsScsIf;                                            \
    PFN_SOCKET_SET_IF               SetTlsScsIf;                                            \
                                                                                            \
    PFN_SOCKET_BIND                 Bind;                                                   \
    PFN_SOCKET_CONNECT              Connect;                                                \
    PFN_SOCKET_LISTEN               Listen;                                                 \
    PFN_SOCKET_SEND                 Send;                                                   \
    PFN_SOCKET_SEND2                Send2;                                                  \
    PFN_SOCKET_OPEN                 Open;                                                   \
    PFN_SOCKET_CLOSE                Close;                                                  \
                                                                                            \
    PFN_SOCKET_ASYNC_TASK           AcceptTask;                                             \
    PFN_SOCKET_ASYNC_TASK           RecvTask;                                               \
                                                                                            \
    PFN_SOCKET_INIT_TLS             InitTlsServer;                                          \
    PFN_SOCKET_INIT_TLS             InitTlsClient;                                          \
                                                                                            \
    PFN_TLSTSAIF_RECV               TsaRecvAppMessage;                                      \
    PFN_TLSTSAIF_SEND               TsaSendTlsMessage;                                      \
    PFN_TLSTSAIF_NOTIFY             TsaNotifyEvent;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SOCKET_OBJECT
{
    ANSC_SOCKET_CLASS_CONTENT
}
ANSC_SOCKET_OBJECT,  *PANSC_SOCKET_OBJECT;


/***********************************************************
           GENERAL SOCKET SINK OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the socket object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SINK_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_SINK_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SINK_GET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SINK_SET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_SINK_ATTACH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_SINK_DETACH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_SINK_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_HANDLE                phRecvHandle,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_SINK_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNewSocket
    );

typedef  ANSC_STATUS
(*PFN_SINK_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecvHandle,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SINK_CLOSE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bByPeer
    );

typedef  ANSC_STATUS
(*PFN_SINK_ABORT)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Instead of querying the socket every once awhile to know whether there's data has arrived at the
 * socket, the owner of the socket object may provide a sink object (borrowed from Microsoft COM+)
 * which contains a set of callback functions that the socket object can invoke both when more data
 * arrives and status changes.
 */
#define  ANSC_SINK_CLASS_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ANSC_HANDLE                     hSocketObject;                                          \
                                                                                            \
    PFN_SINK_CREATE                 Create;                                                 \
    PFN_SINK_REMOVE                 Remove;                                                 \
                                                                                            \
    PFN_SINK_GET_SOCKET             GetSocket;                                              \
    PFN_SINK_SET_SOCKET             SetSocket;                                              \
    PFN_SINK_ATTACH                 Attach;                                                 \
    PFN_SINK_DETACH                 Detach;                                                 \
                                                                                            \
    PFN_SINK_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_SINK_ACCEPT                 Accept;                                                 \
    PFN_SINK_RECV                   Recv;                                                   \
    PFN_SINK_CLOSE                  Close;                                                  \
    PFN_SINK_ABORT                  Abort;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SINK_OBJECT
{
    ANSC_SINK_CLASS_CONTENT
}
ANSC_SINK_OBJECT,  *PANSC_SINK_OBJECT;

#define  ACCESS_ANSC_SINK_OBJECT(p)                 \
         ACCESS_CONTAINER(p, ANSC_SINK_OBJECT, Linkage)


#endif
