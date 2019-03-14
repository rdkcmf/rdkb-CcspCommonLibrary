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

    module:	ansc_xsocket_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc Xsocket Objects.

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
        05/28/02    copied from the original ansc socket object to
                    construct a new object which always uses the
                    opposite socket library.

**********************************************************************/


#ifndef  _ANSC_XSOCKET_INTERFACE_
#define  _ANSC_XSOCKET_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"


/***********************************************************
       PLATFORM INDEPENDENT SOCKET OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the socket object definition.
 */
#define  ANSC_XSOCKET_TYPE_TCP                      0x00000001
#define  ANSC_XSOCKET_TYPE_UDP                      0x00000002
#define  ANSC_XSOCKET_TYPE_RAW                      0x00000003

#define  ANSC_XSOCKET_MODE_ASYNC_CLOSE              0x00000001
#define  ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE          0x00000002

#define  ANSC_XSOCKET_RECV_SCRATCH_PAD_SIZE         16384
#define  ANSC_XSOCKET_BACKLOG_VALUE                 3
#define  ANSC_XSOCKET_TASK_CLEANUP_TIME             5000
#define  ANSC_XSOCKET_POLL_INTERVAL                 1

#define  ANSC_XSOCKET_MAX_RECV_RETRY_COUNTS         2
#define  ANSC_XSOCKET_RECV_HOLD_BACK                200

#define  XSOCKET_MAX_HOST_NAME_SIZE                 128

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_XSOCKET_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_XSOCKET_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ULONG
(*PFN_XSOCKET_GET_IPPROTOCOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_IPPROTOCOL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipprotocol
    );

typedef  PUCHAR
(*PFN_XSOCKET_GET_HOST_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_HOST_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  PUCHAR
(*PFN_XSOCKET_GET_HOST_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_HOST_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPeerName
    );

typedef  USHORT
(*PFN_XSOCKET_GET_HOST_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_HOST_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  xskt_addrinfo*
(*PFN_XSOCKET_GET_HOST_ADDRINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_HOST_ADDRINFO)
    (
        ANSC_HANDLE                 hThisObject,
        xskt_addrinfo*              pAddrInfo
    );

typedef  PUCHAR
(*PFN_XSOCKET_GET_PEER_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_PEER_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  PUCHAR
(*PFN_XSOCKET_GET_PEER_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_PEER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPeerName
    );

typedef  USHORT
(*PFN_XSOCKET_GET_PEER_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_PEER_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  xskt_addrinfo*
(*PFN_XSOCKET_GET_PEER_ADDRINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_PEER_ADDRINFO)
    (
        ANSC_HANDLE                 hThisObject,
        xskt_addrinfo*              pAddrInfo
    );

typedef  UCHAR
(*PFN_XSOCKET_GET_TRANSPORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_TRANSPORT)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       transportType
    );

typedef  ANSC_HANDLE
(*PFN_XSOCKET_GET_XSINK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SET_XSINK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsink
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_RESOLVEADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_APPLYDSCP)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulDSCP
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_BIND)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_CONNECT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_LISTEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hAddress
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSOCKET_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * I'm not so sure whether it's a good idea to encapsulate all the socket-related in a pseudo
 * software object. But for the purpose of isolation, this approach has the benefit of simplicity.
 * The alternative is to simply rename all the socket api functions, but expose the internal data
 * structures to our internal components.
 */
#define  ANSC_XSOCKET_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_LOCK                       OpLock;                                                 \
    XSKT_SOCKET                     Xsocket;                                                \
    ULONG                           Type;                                                   \
    ULONG                           Mode;                                                   \
    ULONG                           IpProtocol;                                             \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    xskt_addrinfo*                  pHostAddrInfo;                                          \
    char                            HostName[XSOCKET_MAX_HOST_NAME_SIZE + 1];               \
    USHORT                          HostPort;                                               \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    xskt_addrinfo*                  pPeerAddrInfo;                                          \
    xskt_addrinfo*                  pOriPeerAddrInfo;                                       \
    char                            PeerName[XSOCKET_MAX_HOST_NAME_SIZE + 1];               \
    USHORT                          PeerPort;                                               \
    UCHAR                           TransportType;                                          \
    ANSC_HANDLE                     hXsinkHolder;                                           \
    ANSC_HANDLE                     hXsinkObject;                                           \
    BOOL                            bClosed;                                                \
                                                                                            \
    PFN_XSOCKET_GET_TYPE            GetType;                                                \
    PFN_XSOCKET_SET_TYPE            SetType;                                                \
    PFN_XSOCKET_GET_MODE            GetMode;                                                \
    PFN_XSOCKET_SET_MODE            SetMode;                                                \
    PFN_XSOCKET_GET_IPPROTOCOL      GetIpProtocol;                                          \
    PFN_XSOCKET_SET_IPPROTOCOL      SetIpProtocol;                                          \
    PFN_XSOCKET_GET_HOST_ADDRESS    GetHostAddress;                                         \
    PFN_XSOCKET_SET_HOST_ADDRESS    SetHostAddress;                                         \
    PFN_XSOCKET_GET_HOST_NAME       GetHostName;                                            \
    PFN_XSOCKET_SET_HOST_NAME       SetHostName;                                            \
    PFN_XSOCKET_GET_HOST_PORT       GetHostPort;                                            \
    PFN_XSOCKET_SET_HOST_PORT       SetHostPort;                                            \
    PFN_XSOCKET_GET_HOST_ADDRINFO   GetHostAddrInfo;                                        \
    PFN_XSOCKET_SET_HOST_ADDRINFO   SetHostAddrInfo;                                        \
    PFN_XSOCKET_GET_PEER_ADDRESS    GetPeerAddress;                                         \
    PFN_XSOCKET_SET_PEER_ADDRESS    SetPeerAddress;                                         \
    PFN_XSOCKET_GET_PEER_NAME       GetPeerName;                                            \
    PFN_XSOCKET_SET_PEER_NAME       SetPeerName;                                            \
    PFN_XSOCKET_GET_PEER_PORT       GetPeerPort;                                            \
    PFN_XSOCKET_SET_PEER_PORT       SetPeerPort;                                            \
    PFN_XSOCKET_GET_PEER_ADDRINFO   GetPeerAddrInfo;                                        \
    PFN_XSOCKET_SET_PEER_ADDRINFO   SetPeerAddrInfo;                                        \
    PFN_XSOCKET_GET_TRANSPORT       GetTransportType;                                       \
    PFN_XSOCKET_SET_TRANSPORT       SetTransportType;                                       \
    PFN_XSOCKET_GET_XSINK           GetXsink;                                               \
    PFN_XSOCKET_SET_XSINK           SetXsink;                                               \
                                                                                            \
    PFN_XSOCKET_RESOLVEADDR         ResolveAddr;                                            \
    PFN_XSOCKET_APPLYDSCP           ApplyDSCP;                                              \
    PFN_XSOCKET_BIND                Bind;                                                   \
    PFN_XSOCKET_CONNECT             Connect;                                                \
    PFN_XSOCKET_LISTEN              Listen;                                                 \
    PFN_XSOCKET_SEND                Send;                                                   \
    PFN_XSOCKET_SEND                Send2;                                                  \
    PFN_XSOCKET_OPEN                Open;                                                   \
    PFN_XSOCKET_CLOSE               Close;                                                  \
                                                                                            \
    PFN_XSOCKET_ASYNC_TASK          AcceptTask;                                             \
    PFN_XSOCKET_ASYNC_TASK          RecvTask;                                               \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_XSOCKET_OBJECT
{
    ANSC_XSOCKET_CLASS_CONTENT
}
ANSC_XSOCKET_OBJECT,  *PANSC_XSOCKET_OBJECT;


/***********************************************************
           GENERAL SOCKET XSINK OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the socket object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_XSINK_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_XSINK_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_XSINK_GET_XSOCKET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_XSINK_SET_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    );

typedef  ANSC_STATUS
(*PFN_XSINK_ATTACH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    );

typedef  ANSC_STATUS
(*PFN_XSINK_DETACH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_XSINK_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_HANDLE                phRecvHandle,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_XSINK_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNewXsocket
    );

typedef  ANSC_STATUS
(*PFN_XSINK_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecvHandle,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_XSINK_CLOSE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bByPeer
    );

typedef  ANSC_STATUS
(*PFN_XSINK_ABORT)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Instead of querying the socket every once awhile to know whether there's data has arrived at the
 * socket, the owner of the socket object may provide a sink object (borrowed from Microsoft COM+)
 * which contains a set of callback functions that the socket object can invoke both when more data
 * arrives and status changes.
 */
#define  ANSC_XSINK_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ANSC_HANDLE                     hXsocketObject;                                         \
                                                                                            \
    PFN_XSINK_CREATE                Create;                                                 \
    PFN_XSINK_REMOVE                Remove;                                                 \
                                                                                            \
    PFN_XSINK_GET_XSOCKET           GetXsocket;                                             \
    PFN_XSINK_SET_SOCKET            SetXsocket;                                             \
    PFN_XSINK_ATTACH                Attach;                                                 \
    PFN_XSINK_DETACH                Detach;                                                 \
                                                                                            \
    PFN_XSINK_GET_RECV_BUFFER       GetRecvBuffer;                                          \
    PFN_XSINK_ACCEPT                Accept;                                                 \
    PFN_XSINK_RECV                  Recv;                                                   \
    PFN_XSINK_CLOSE                 Close;                                                  \
    PFN_XSINK_ABORT                 Abort;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_XSINK_OBJECT
{
    ANSC_XSINK_CLASS_CONTENT
}
ANSC_XSINK_OBJECT,  *PANSC_XSINK_OBJECT;

#define  ACCESS_ANSC_XSINK_OBJECT(p)                \
         ACCESS_CONTAINER(p, ANSC_XSINK_OBJECT, Linkage)


#endif
