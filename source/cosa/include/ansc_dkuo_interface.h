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

    module:	ansc_dkuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Daemon Socket Udp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/10/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DKUO_INTERFACE_
#define  _ANSC_DKUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT DAEMON SOCKET UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DKUO_MAX_BUFFER_SIZE                  2048

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DKUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_DKUO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  PUCHAR
(*PFN_DKUO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_DKUO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_DKUO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_FINISH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DKUO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_DKUO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_DKUO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
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
#define  ANSC_DAEMON_SOCKET_UDP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_SOCKET                     Socket;                                                 \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
    ULONG                           HashIndex;                                              \
                                                                                            \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hDaemonEngine;                                          \
    ANSC_HANDLE                     hClientContext;                                         \
    ANSC_HANDLE                     hPacket;                                                \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
    BOOL                            bClosed;                                                \
    BOOL                            bRecvEnabled;                                           \
    BOOL                            bSendEnabled;                                           \
    ANSC_LOCK                       OpLock;                                                 \
                                                                                            \
    PFN_DKUO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_DKUO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_DKUO_GET_PORT               GetPeerPort;                                            \
    PFN_DKUO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_DKUO_GET_CONTEXT            GetDaemonServer;                                        \
    PFN_DKUO_SET_CONTEXT            SetDaemonServer;                                        \
    PFN_DKUO_GET_CONTEXT            GetDaemonEngine;                                        \
    PFN_DKUO_SET_CONTEXT            SetDaemonEngine;                                        \
    PFN_DKUO_GET_CONTEXT            GetClientContext;                                       \
    PFN_DKUO_SET_CONTEXT            SetClientContext;                                       \
    PFN_DKUO_GET_CONTEXT            GetPacket;                                              \
    PFN_DKUO_SET_CONTEXT            SetPacket;                                              \
    PFN_DKUO_RETURN                 Return;                                                 \
    PFN_DKUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_DKUO_FINISH                 Finish;                                                 \
    PFN_DKUO_OPEN                   Open;                                                   \
    PFN_DKUO_CLOSE                  Close;                                                  \
    PFN_DKUO_ENABLE                 EnableRecv;                                             \
    PFN_DKUO_ENABLE                 EnableSend;                                             \
                                                                                            \
    PFN_DKUO_RECV                   Recv;                                                   \
    PFN_DKUO_SEND                   Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_DAEMON_SOCKET_UDP_OBJECT
{
    ANSC_DAEMON_SOCKET_UDP_CLASS_CONTENT
}
ANSC_DAEMON_SOCKET_UDP_OBJECT,  *PANSC_DAEMON_SOCKET_UDP_OBJECT;

#define  ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_DAEMON_SOCKET_UDP_OBJECT, Linkage)


#endif
