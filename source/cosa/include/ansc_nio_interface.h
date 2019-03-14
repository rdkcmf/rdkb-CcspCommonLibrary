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

    module:	ansc_nio_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc Network Interface Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/13/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_NIO_INTERFACE_
#define  _ANSC_NIO_INTERFACE_


/***********************************************************
      GENERAL ANSC NETWORK INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCNIO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ANSCNIO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_ANSCNIO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

typedef  PUCHAR
(*PFN_ANSCNIO_GET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIO_SET_ADDRESS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        ULONG                       ulSize
    );

/*
 * According to the original design, each network interface shall employs unique network service
 * module. Multiple interfaces are plugged into the switch fabric module. So there was no real need
 * to distinguish between the network interface and network service module. However, this approach
 * causes some problems on a software-based/general-purpose CPU based implementation:
 *
 *      (1) Unnecessary Memory Consumption - each service module may contain many active
 *          objects; while the code section of the same type of object is shared by all
 *          the instances, the data section has to be private.
 *      (2) Unnecessary Inter-Module Communication - under normal circumstances, each
 *          module may operate independently from each other. But if there're more than
 *          two interfaces connected to the same network (such as Internet), the session
 *          tables on these two interfaces have to be synchronized because packets that
 *          belong to the same session may arrive on either interface.
 *
 * While the distributed (Npu-based head-end equipment) architecture still has the same problems
 * (and even more), the thoughput/performance requirement justifies the tradeoff. But for the
 * exact same reason, we cannot justify this design on CPE simply because all software components
 * run on the same CPU.
 *
 * By defining the following object, we will be able to associate multiple interfaces to the same
 * network service module, which is represented by the field "hServiceModule". Note this object is
 * merely a place-holder, it doesn't interact directly with the service module itself.
 */
#define  ANSC_NETWORK_INTERFACE_CLASS_CONTENT                                               \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ULONG                           MacAddressSize;                                         \
    char                            MacAddress[ANSC_MAX_MAC_ADDRESS_SIZE];                  \
    ULONG                           InterfaceType;                                          \
    ULONG                           NetworkType;                                            \
    ANSC_HANDLE                     hServiceModule;                                         \
    ANSC_HANDLE                     hAnscReserved;                                          \
                                                                                            \
    PFN_ANSCNIO_CREATE              Create;                                                 \
    PFN_ANSCNIO_REMOVE              Remove;                                                 \
    PFN_ANSCNIO_RESET               Reset;                                                  \
                                                                                            \
    PFN_ANSCNIO_GET_ADDRESS         GetMacAddress;                                          \
    PFN_ANSCNIO_SET_ADDRESS         SetMacAddress;                                          \
    PFN_ANSCNIO_GET_TYPE            GetInterfaceType;                                       \
    PFN_ANSCNIO_SET_TYPE            SetInterfaceType;                                       \
    PFN_ANSCNIO_GET_TYPE            GetNetworkType;                                         \
    PFN_ANSCNIO_SET_TYPE            SetNetworkType;                                         \
                                                                                            \
    PFN_ANSCNIO_GET_CONTEXT         GetServiceModule;                                       \
    PFN_ANSCNIO_SET_CONTEXT         SetServiceModule;                                       \
    PFN_ANSCNIO_GET_CONTEXT         GetAnscReserved;                                        \
    PFN_ANSCNIO_SET_CONTEXT         SetAnscReserved;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NETWORK_INTERFACE_OBJECT
{
    ANSC_NETWORK_INTERFACE_CLASS_CONTENT
}
ANSC_NETWORK_INTERFACE_OBJECT,  *PANSC_NETWORK_INTERFACE_OBJECT;

#define  ACCESS_ANSC_NETWORK_INTERFACE_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_NETWORK_INTERFACE_OBJECT, Linkage)


/***********************************************************
        ANSC NETWORK INTERFACE IPV4 OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCNIOIP4O_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_ADD_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIpv4Addr
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_DEL_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIpv4Addr
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_DEL_ALL_ADDRS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_ADD_GATE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_DEL_GATE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  ANSC_STATUS
(*PFN_ANSCNIOIP4O_DEL_ALL_GATES)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The derived interface objects are categorized by their network types instead of the interface
 * types. For Ipv4 capable interface, we need maintain following information:
 *
 *      $ Ip address
 *      $ subnet mask
 *      $ next hop
 */
#define  ANSC_NIO_IP4_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSC_NETWORK_INTERFACE_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ANSC_IPV4_HOST_INFO             HostInfo;                                               \
                                                                                            \
    PFN_ANSCNIOIP4O_GET_CONTEXT     GetHostInfo;                                            \
    PFN_ANSCNIOIP4O_SET_CONTEXT     SetHostInfo;                                            \
                                                                                            \
    PFN_ANSCNIOIP4O_ADD_ADDR        AddAddress;                                             \
    PFN_ANSCNIOIP4O_DEL_ADDR        DelAddress;                                             \
    PFN_ANSCNIOIP4O_DEL_ALL_ADDRS   DelAllAddresses;                                        \
                                                                                            \
    PFN_ANSCNIOIP4O_ADD_GATE        AddGateway;                                             \
    PFN_ANSCNIOIP4O_DEL_GATE        DelGateway;                                             \
    PFN_ANSCNIOIP4O_DEL_ALL_GATES   DelAllGateways;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NIO_IP4_OBJECT
{
    ANSC_NIO_IP4_CLASS_CONTENT
}
ANSC_NIO_IP4_OBJECT,  *PANSC_NIO_IP4_OBJECT;

#define  ACCESS_ANSC_NIO_IP4_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_NIO_IP4_OBJECT, Linkage)


#endif
