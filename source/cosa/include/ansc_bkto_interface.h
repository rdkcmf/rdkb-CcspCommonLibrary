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

    module:	ansc_bkto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Socket Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_BKTO_INTERFACE_
#define  _ANSC_BKTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT BROKER SOCKET TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BKTO_MAX_BUFFER_SIZE                  2048

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BKTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_BKTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  PUCHAR
(*PFN_BKTO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_BKTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_HANDLE
(*PFN_BKTO_GET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_BUF_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

typedef  BOOL
(*PFN_BKTO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue,
        ULONG                       ulSeconds
    );

typedef  ANSC_STATUS
(*PFN_BKTO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_FINISH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKTO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PVOID
(*PFN_BKTO_GET_RECV_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

typedef  PVOID
(*PFN_BKTO_GET_SEND_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_BKTO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_BKTO_SEND)
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
 * server applications. The Broker Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BROKER_SOCKET_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               AddrLink;                                               \
    ANSC_SOCKET                     Socket;                                                 \
    ANSC_IPV4_ADDRESS               HostAddress;                                            \
    USHORT                          HostPort;                                               \
    ANSC_IPV4_ADDRESS               PeerAddress;                                            \
    USHORT                          PeerPort;                                               \
    ULONG                           HashIndex;                                              \
                                                                                            \
    ANSC_HANDLE                     hBrokerServer;                                          \
    ANSC_HANDLE                     hBrokerEngine;                                          \
    ANSC_HANDLE                     hClientContext;                                         \
    ULONG                           RecvBytesCount;                                         \
    ULONG                           SendBytesCount;                                         \
    ULONG                           LastRecvAt;                                             \
    ULONG                           LastSendAt;                                             \
    ULONG                           StartTime;                                              \
    BOOL                            bClosed;                                                \
    BOOL                            bBroken;                                                \
    BOOL                            bToBeCleaned;                                           \
                                                                                            \
    ANSC_HANDLE                     hRecvBufferTag;                                         \
    PVOID                           RecvBuffer;                                             \
    ULONG                           RecvBufferSize;                                         \
    ULONG                           RecvPacketSize;                                         \
    ULONG                           RecvOffset;                                             \
                                                                                            \
    PFN_BKTO_GET_ADDRESS            GetHostAddress;                                         \
    PFN_BKTO_SET_ADDRESS            SetHostAddress;                                         \
    PFN_BKTO_GET_PORT               GetHostPort;                                            \
    PFN_BKTO_SET_PORT               SetHostPort;                                            \
    PFN_BKTO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_BKTO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_BKTO_GET_PORT               GetPeerPort;                                            \
    PFN_BKTO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_BKTO_GET_CONTEXT            GetBrokerServer;                                        \
    PFN_BKTO_SET_CONTEXT            SetBrokerServer;                                        \
    PFN_BKTO_GET_CONTEXT            GetBrokerEngine;                                        \
    PFN_BKTO_SET_CONTEXT            SetBrokerEngine;                                        \
    PFN_BKTO_GET_CONTEXT            GetClientContext;                                       \
    PFN_BKTO_SET_CONTEXT            SetClientContext;                                       \
    PFN_BKTO_GET_SIZE               GetBufferSize;                                          \
    PFN_BKTO_SET_SIZE               SetBufferSize;                                          \
    PFN_BKTO_GET_BUF_CONTEXT        GetBufferContext;                                       \
    PFN_BKTO_SET_BUF_CONTEXT        SetBufferContext;                                       \
    PFN_BKTO_GET_BOOL               IsToBeCleaned;                                          \
    PFN_BKTO_SET_BOOL               ToClean;                                                \
    PFN_BKTO_RETURN                 Return;                                                 \
    PFN_BKTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_BKTO_FINISH                 Finish;                                                 \
    PFN_BKTO_OPEN                   Open;                                                   \
    PFN_BKTO_CLOSE                  Close;                                                  \
                                                                                            \
    PFN_BKTO_GET_RECV_BUFFER        GetRecvBuffer;                                          \
    PFN_BKTO_RECV                   Recv;                                                   \
    PFN_BKTO_SEND                   Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BROKER_SOCKET_TCP_OBJECT
{
    ANSC_BROKER_SOCKET_TCP_CLASS_CONTENT
}
ANSC_BROKER_SOCKET_TCP_OBJECT,  *PANSC_BROKER_SOCKET_TCP_OBJECT;

#define  ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_SOCKET_TCP_OBJECT, Linkage)

#define  ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT_ADDRLINK(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_SOCKET_TCP_OBJECT, AddrLink)


#endif
