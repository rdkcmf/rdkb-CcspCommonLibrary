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

    module:	ansc_upco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Udp Ping Client Object.

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


#ifndef  _ANSC_UPCO_INTERFACE_
#define  _ANSC_UPCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "ansc_scuowo_interface.h"


/***********************************************************
   PLATFORM INDEPENDENT UDP PING CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_UPCO_MAX_MESSAGE_SIZE                 65536

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_UPCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_UPCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  PUCHAR
(*PFN_UPCO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_UPCO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_UPCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_UPCO_PING)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPingReq
    );

typedef  ANSC_STATUS
(*PFN_UPCO_STOP)
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
#define  ANSC_UDP_PING_CLIENT_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSimpleClientUdp;                                       \
    ANSC_HANDLE                     hClientUdpWorker;                                       \
    ANSC_HANDLE                     hActiveRequest;                                         \
    ANSC_HANDLE                     hActiveStatus;                                          \
                                                                                            \
    PFN_UPCO_GET_ADDR               GetClientAddress;                                       \
    PFN_UPCO_SET_ADDR               SetClientAddress;                                       \
    PFN_UPCO_GET_PORT               GetClientPort;                                          \
    PFN_UPCO_SET_PORT               SetClientPort;                                          \
                                                                                            \
    PFN_UPCO_GET_CONTEXT            GetSimpleClientUdp;                                     \
    PFN_UPCO_GET_CONTEXT            GetClientUdpWorker;                                     \
                                                                                            \
    PFN_UPCO_ENGAGE                 Engage;                                                 \
    PFN_UPCO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_UPCO_PING                   Ping;                                                   \
    PFN_UPCO_STOP                   Stop;                                                   \
                                                                                            \
    PFN_SCUOWO_QUERY                ScuowoQuery;                                            \
    PFN_SCUOWO_PROCESS              ScuowoProcess;                                          \
    PFN_SCUOWO_NOTIFY               ScuowoNotify;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_UDP_PING_CLIENT_OBJECT
{
    ANSC_UDP_PING_CLIENT_CLASS_CONTENT
}
ANSC_UDP_PING_CLIENT_OBJECT,  *PANSC_UDP_PING_CLIENT_OBJECT;

#define  ACCESS_ANSC_UDP_PING_CLIENT_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_UDP_PING_CLIENT_OBJECT, Linkage)

/*
 * UDP Based Echo Service is defined as a datagram based application on UDP. A server listens for
 * UDP datagrams on UDP port 7. When a datagram is received, the data from it is sent back in an
 * answering datagram.
 */
typedef  struct
_ANSC_UPCO_PING_REQUEST
{
    ANSC_IPV4_ADDRESS               Address;
    USHORT                          Port;
    ULONG                           DataSize;
    ULONG                           PacketNumber;
    ULONG                           Interval;
    BOOL                            bQuiet;
}
ANSC_UPCO_PING_REQUEST,  *PANSC_UPCO_PING_REQUEST;

#define  AnscUpcoAllocPingRequest(ping_req)                                                 \
         {                                                                                  \
            ping_req =                                                                      \
                (PANSC_UPCO_PING_REQUEST)AnscAllocateMemory(sizeof(ANSC_UPCO_PING_REQUEST));\
                                                                                            \
            if ( ping_req )                                                                 \
            {                                                                               \
                ping_req->Port         = ECHO_PORT;                                         \
                ping_req->DataSize     = 32;                                                \
                ping_req->PacketNumber = 4;                                                 \
                ping_req->Interval     = 200;                                               \
                ping_req->bQuiet       = FALSE;                                             \
            }                                                                               \
         }

#define  AnscUpcoFreePingRequest(ping_req)                                                  \
         {                                                                                  \
            if ( ping_req )                                                                 \
            {                                                                               \
                AnscFreeMemory(ping_req);                                                   \
            }                                                                               \
         }

typedef  struct
_ANSC_UPCO_PING_STATUS
{
    ULONG                           ReplyReceived;
    ULONG                           AcuRoundTripInMs;
    ULONG                           AvgRoundTripInMs;
    ULONG                           SentTimestamp;
    ULONG                           RecvTimestamp;
    ULONG                           RecvDataSize;
    ANSC_EVENT                      CtrlEvent;
    BOOL                            bStopped;
}
ANSC_UPCO_PING_STATUS,  *PANSC_UPCO_PING_STATUS;

#define  AnscUpcoAllocPingStatus(ping_status)                                               \
         {                                                                                  \
            ping_status =                                                                   \
                (PANSC_UPCO_PING_STATUS)AnscAllocateMemory(sizeof(ANSC_UPCO_PING_STATUS));  \
                                                                                            \
            if ( ping_status )                                                              \
            {                                                                               \
                AnscInitializeEvent(&ping_status->CtrlEvent);                               \
                AnscResetEvent     (&ping_status->CtrlEvent);                               \
            }                                                                               \
         }

#define  AnscUpcoFreePingStatus(ping_status)                                                \
         {                                                                                  \
            if ( ping_status )                                                              \
            {                                                                               \
                AnscFreeEvent (&ping_status->CtrlEvent);                                    \
                AnscFreeMemory( ping_status);                                               \
            }                                                                               \
         }


#endif
