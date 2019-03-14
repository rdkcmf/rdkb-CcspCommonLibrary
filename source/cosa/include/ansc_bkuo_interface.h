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

    module:	ansc_bkuo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Socket Udp Object.

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


#ifndef  _ANSC_BKUO_INTERFACE_
#define  _ANSC_BKUO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT BROKER SOCKET UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BKUO_MAX_BUFFER_SIZE                  2048

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BKUO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_BKUO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  PUCHAR
(*PFN_BKUO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_BKUO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_BKUO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_FINISH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BKUO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_BKUO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_BKUO_SEND)
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
 * server applications. The Broker Udp Object MUST operate in a multi-tasking capable environment.
 * It opens a Udp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BROKER_SOCKET_UDP_CLASS_CONTENT                                               \
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
    ANSC_HANDLE                     hBrokerServer;                                          \
    ANSC_HANDLE                     hBrokerEngine;                                          \
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
    PFN_BKUO_GET_ADDRESS            GetPeerAddress;                                         \
    PFN_BKUO_SET_ADDRESS            SetPeerAddress;                                         \
    PFN_BKUO_GET_PORT               GetPeerPort;                                            \
    PFN_BKUO_SET_PORT               SetPeerPort;                                            \
                                                                                            \
    PFN_BKUO_GET_CONTEXT            GetBrokerServer;                                        \
    PFN_BKUO_SET_CONTEXT            SetBrokerServer;                                        \
    PFN_BKUO_GET_CONTEXT            GetBrokerEngine;                                        \
    PFN_BKUO_SET_CONTEXT            SetBrokerEngine;                                        \
    PFN_BKUO_GET_CONTEXT            GetClientContext;                                       \
    PFN_BKUO_SET_CONTEXT            SetClientContext;                                       \
    PFN_BKUO_GET_CONTEXT            GetPacket;                                              \
    PFN_BKUO_SET_CONTEXT            SetPacket;                                              \
    PFN_BKUO_RETURN                 Return;                                                 \
    PFN_BKUO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_BKUO_FINISH                 Finish;                                                 \
    PFN_BKUO_OPEN                   Open;                                                   \
    PFN_BKUO_CLOSE                  Close;                                                  \
    PFN_BKUO_ENABLE                 EnableRecv;                                             \
    PFN_BKUO_ENABLE                 EnableSend;                                             \
                                                                                            \
    PFN_BKUO_RECV                   Recv;                                                   \
    PFN_BKUO_SEND                   Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BROKER_SOCKET_UDP_OBJECT
{
    ANSC_BROKER_SOCKET_UDP_CLASS_CONTENT
}
ANSC_BROKER_SOCKET_UDP_OBJECT,  *PANSC_BROKER_SOCKET_UDP_OBJECT;

#define  ACCESS_ANSC_BROKER_SOCKET_UDP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_BROKER_SOCKET_UDP_OBJECT, Linkage)


#endif
