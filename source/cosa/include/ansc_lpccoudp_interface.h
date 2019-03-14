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

    module:	ansc_lpccoudp_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Lpcco Udp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/10/10    initial revision.

**********************************************************************/


#ifndef  _ANSC_LPCCOUDP_INTERFACE_
#define  _ANSC_LPCCOUDP_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lpcco_interface.h"
#include "ansc_lpcco_external_api.h"
#include "ansc_dsuowo_interface.h"
#include "ansc_bsuowo_interface.h"


/***********************************************************
      PLATFORM INDEPENDENT LPCCO UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_LPCCOUDP_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_LPCCOUDP_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  PUCHAR
(*PFN_LPCCOUDP_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_LPCCOUDP_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SEND1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SEND2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_SEND3)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType,
        ULONG                       ulRepCode
    );

typedef  ANSC_STATUS
(*PFN_LPCCOUDP_RECV1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize
    );

/*
 * The Ansc Lpcco Udp object implements the IMC interface by encapsulating the LPC calls as udp
 * messages and transmit them over UDP socket. Such mechanism applies to most of the communication
 * scenarios on desktop operating systems, except the interaction between a User-mode module and a
 * Kernel-mode module, which requires some special consideration.
 */
#define  ANSC_LPCCO_UDP_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSC_LPC_CONNECTOR_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hBrokerServer;                                          \
    ANSC_HANDLE                     hDaemonWorker;                                          \
    ANSC_HANDLE                     hBrokerWorker;                                          \
                                                                                            \
    ANSC_IPV4_ADDRESS               MyAddress;                                              \
    USHORT                          MyPort;                                                 \
    ANSC_IPV4_ADDRESS               ManagerAddress;                                         \
    USHORT                          ManagerPort;                                            \
    ULONG                           MaxMessageSize;                                         \
    ULONG                           EngineCount;                                            \
    ULONG                           MinSocketCount;                                         \
    ULONG                           MaxSocketCount;                                         \
    ULONG                           SocketTimeOut;                                          \
                                                                                            \
    PFN_LPCCOUDP_GET_ADDR           GetMyAddress;                                           \
    PFN_LPCCOUDP_SET_ADDR           SetMyAddress;                                           \
    PFN_LPCCOUDP_GET_PORT           GetMyPort;                                              \
    PFN_LPCCOUDP_SET_PORT           SetMyPort;                                              \
    PFN_LPCCOUDP_GET_ADDR           GetManagerAddress;                                      \
    PFN_LPCCOUDP_SET_ADDR           SetManagerAddress;                                      \
    PFN_LPCCOUDP_GET_PORT           GetManagerPort;                                         \
    PFN_LPCCOUDP_SET_PORT           SetManagerPort;                                         \
                                                                                            \
    PFN_LPCCOUDP_SEND1              SendHello;                                              \
    PFN_LPCCOUDP_SEND1              SendAck;                                                \
    PFN_LPCCOUDP_SEND2              SendRequest;                                            \
    PFN_LPCCOUDP_SEND3              SendReply;                                              \
    PFN_LPCCOUDP_SEND1              SendBye;                                                \
                                                                                            \
    PFN_LPCCOUDP_RECV1              Recv;                                                   \
    PFN_LPCCOUDP_RECV1              RecvHello;                                              \
    PFN_LPCCOUDP_RECV1              RecvAck;                                                \
    PFN_LPCCOUDP_RECV1              RecvRequest;                                            \
    PFN_LPCCOUDP_RECV1              RecvReply;                                              \
    PFN_LPCCOUDP_RECV1              RecvBye;                                                \
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
_ANSC_LPCCO_UDP_OBJECT
{
    ANSC_LPCCO_UDP_CLASS_CONTENT
}
ANSC_LPCCO_UDP_OBJECT,  *PANSC_LPCCO_UDP_OBJECT;

#define  ACCESS_ANSC_LPCCO_UDP_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_LPCCO_UDP_OBJECT, Linkage)


#endif
