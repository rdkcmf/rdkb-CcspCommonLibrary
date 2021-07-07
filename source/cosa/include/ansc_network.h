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

    module:	ansc_network.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        data structures and basic types related to TCP/IP
        networking.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_NETWORK_
#define  _ANSC_NETWORK_


/***********************************************************
      DEFINITION OF SOME PHYSICAL LAYER DATA STRUCTURES
***********************************************************/

/*
 * The following categorization on different physical link types is NOT scientific from a driver
 * or hardware developer's standpoint. A more approriate definition should introduce a hierarhical
 * layering between the physical link characteristics and the driver implementation.
 */
#define  ANSC_LINK_TYPE_LAN                         1
#define  ANSC_LINK_TYPE_WAN                         2
#define  ANSC_LINK_TYPE_DMZ                         3
#define  ANSC_LINK_TYPE_LOOPBACK                    10
#define  ANSC_LINK_TYPE_ETHERNET                    11
#define  ANSC_LINK_TYPE_TOKEN_RING                  12
#define  ANSC_LINK_TYPE_FDDI                        13
#define  ANSC_LINK_TYPE_PPP                         14
#define  ANSC_LINK_TYPE_PPPOE                       15
#define  ANSC_LINK_TYPE_PPTP_PAC                    16
#define  ANSC_LINK_TYPE_L2TP_LAC                    17
#define  ANSC_LINK_TYPE_X25                         18
#define  ANSC_LINK_TYPE_ISDN                        19
#define  ANSC_LINK_TYPE_SERIAL                      20
#define  ANSC_LINK_TYPE_FRAME_RELAY                 21
#define  ANSC_LINK_TYPE_ATM                         22
#define  ANSC_LINK_TYPE_SONET                       23
#define  ANSC_LINK_TYPE_IRDA                        24
#define  ANSC_LINK_TYPE_GRE_TUNNEL                  25
#define  ANSC_LINK_TYPE_RELAY                       26

#define  ANSC_LINK_TYPE_IPSEC_TUNNEL                101
#define  ANSC_LINK_TYPE_PPTP_PNS                    102
#define  ANSC_LINK_TYPE_L2TP_LNS                    103

/*
 * In many occasions, we need to determine whether an IP address is part of an IP address range or
 * an IP address subnet. The following macro encapsulate a simple address comparison mechanism.
 */
#define  AnscIsIpAddrInRange(target_addr, range_addr1, range_addr2, b_range, b_result)      \
         {                                                                                  \
            ULONG                   target_addr_h = 0;                                      \
            ULONG                   range_addr1_h = 0;                                      \
            ULONG                   range_addr2_h = 0;                                      \
                                                                                            \
            if ( b_range )                                                                  \
            {                                                                               \
                target_addr_h = AnscUlongFromNToH(target_addr);                             \
                range_addr1_h = AnscUlongFromNToH(range_addr1);                             \
                range_addr2_h = AnscUlongFromNToH(range_addr2);                             \
                                                                                            \
                if ( (target_addr_h >= range_addr1_h) && (target_addr_h <= range_addr2_h) ) \
                {                                                                           \
                    b_result = TRUE;                                                        \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    b_result = FALSE;                                                       \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                b_result = (target_addr & range_addr2) == (range_addr1 & range_addr2);      \
            }                                                                               \
         }


/***********************************************************
     DEFINITION OF SOME GENERAL ETHERNET DATA STRUCTURES
***********************************************************/

/*
 * There might be some places in the framework make use of a list of ethernet-like mac addresses.
 * For instance, the ethernet bundle object may have to distinguish mac-level traffic based on the
 * source mac address of the ethernet packet.
 */
typedef  struct
_ANSC_ETHERNET_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    UCHAR                           Address[ETHERNET_ADDRESS_SIZE];
}
ANSC_ETHERNET_TRUNK,  *PANSC_ETHERNET_TRUNK;

#define  ACCESS_ANSC_ETHERNET_TRUNK(p)              \
         ACCESS_CONTAINER(p, ANSC_ETHERNET_TRUNK, Linkage)


/***********************************************************
       DEFINITION OF SOME GENERAL IPV4 DATA STRUCTURES
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_MAX_IPV4_HOST_ADDRESS_NUMBER          16
#define  ANSC_MAX_IPV4_GATEWAY_NUMBER               16

/*
 * Defined by IETF RFC 1122, a standard Ipv4 host must be initialized with following information:
 *
 *      $ Ip address(es)
 *      $ address mask(s)
 *      $ a list of default gateways, with a preference level
 */
typedef  struct
_ANSC_IPV4_HOST_ADDRESS
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    BOOL                            bStatic;
    ANSC_IPV4_ADDRESS               Address;
    ANSC_IPV4_ADDRESS               Mask;
    ANSC_IPV4_ADDRESS               NextHop;
}
ANSC_IPV4_HOST_ADDRESS,  *PANSC_IPV4_HOST_ADDRESS;

#define  ACCESS_ANSC_IPV4_HOST_ADDRESS(p)           \
         ACCESS_CONTAINER(p, ANSC_IPV4_HOST_ADDRESS, Linkage)

typedef  struct
_ANSC_IPV4_HOST_INFO
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           AddressCount;
    ULONG                           GatewayCount;
    ANSC_IPV4_HOST_ADDRESS          AddressArray[ANSC_MAX_IPV4_HOST_ADDRESS_NUMBER];
    ANSC_IPV4_ADDRESS               GatewayArray[ANSC_MAX_IPV4_GATEWAY_NUMBER];
}
ANSC_IPV4_HOST_INFO,  *PANSC_IPV4_HOST_INFO;

#define  ACCESS_ANSC_IPV4_HOST_INFO(p)              \
         ACCESS_CONTAINER(p, ANSC_IPV4_HOST_INFO, Linkage)

/*
 * There're many places in the container make use of a pair of IP addresses. For example, the network
 * layer bundle objects identify flows based on their source or destination address. Not all objects
 * make use of the "transport" and "tos" fields.
 */
typedef  struct
_ANSC_IPV4_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    BOOL                            bSrcRange;
    BOOL                            bDstRange;
    ANSC_IPV4_ADDRESS               SrcAddress1;
    ANSC_IPV4_ADDRESS               SrcAddress2;
    ANSC_IPV4_ADDRESS               DstAddress1;
    ANSC_IPV4_ADDRESS               DstAddress2;
}
ANSC_IPV4_TRUNK,  *PANSC_IPV4_TRUNK;

#define  ACCESS_ANSC_IPV4_TRUNK(p)                  \
         ACCESS_CONTAINER(p, ANSC_IPV4_TRUNK, Linkage)

/*
 * There're many places in the container make use of a pair of IP addresses. For example, the network
 * layer bundle objects identify flows based on their source or destination address. Not all objects
 * make use of the "transport" and "tos" fields.
 */
typedef  struct
_ANSC_IPV4_CIRCUIT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_IPV4_ADDRESS               SrcAddress;
    ANSC_IPV4_ADDRESS               DstAddress;
}
ANSC_IPV4_CIRCUIT,  *PANSC_IPV4_CIRCUIT;

#define  ACCESS_ANSC_IPV4_CIRCUIT(p)                \
         ACCESS_CONTAINER(p, ANSC_IPV4_CIRCUIT, Linkage)

/*
 * There're many places in the container make use of a pair of IP addresses. For example, the network
 * layer bundle objects identify flows based on their source or destination address. Not all objects
 * make use of the "transport" and "tos" fields.
 */
typedef  struct
_ANSC_IPV4_ADDR_POOL
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ANSC_HANDLE                     hAnscReserved;
    BOOL                            bRange;
    ANSC_IPV4_ADDRESS               Address1;
    ANSC_IPV4_ADDRESS               Address2;
    ULONG                           From;
    ULONG                           To;
    ULONG                           Size;
}
ANSC_IPV4_ADDR_POOL,  *PANSC_IPV4_ADDR_POOL;

#define  ACCESS_ANSC_IPV4_ADDR_POOL(p)              \
         ACCESS_CONTAINER(p, ANSC_IPV4_ADDR_POOL, Linkage)

/*
 * When we combine the subnet-based and range-based address pools together, the best reasonable
 * search algorithm is binary search. To make the generation of the final array easier, we define
 * the "area" data structure, which will be chained together and updated every time when a new
 * pool is added or an old pool is removed.
 */
typedef  struct
_ANSC_IPV4_ADDR_AREA
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           LowerEdge;
    ULONG                           UpperEdge;
    QUEUE_HEADER                    RegionQueue;
}
ANSC_IPV4_ADDR_AREA,  *PANSC_IPV4_ADDR_AREA;

#define  ACCESS_ANSC_IPV4_ADDR_AREA(p)              \
         ACCESS_CONTAINER(p, ANSC_IPV4_ADDR_AREA, Linkage)

/*
 * A single address pool may be referenced by multiple address areas, so we cannot link the pool
 * objects directly within in the area structure. We use the following data structure instead to
 * reference the address pool from within the area object.
 */
typedef  struct
_ANSC_IPV4_REGION
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hAddrPool;
}
ANSC_IPV4_REGION,  *PANSC_IPV4_REGION;

#define  ACCESS_ANSC_IPV4_REGION(p)                 \
         ACCESS_CONTAINER(p, ANSC_IPV4_REGION, Linkage)


/***********************************************************
     DEFINITION OF SOME GENERAL TCP/UDP DATA STRUCTURES
***********************************************************/

/*
 * There're many places in the container make use of a pair of TCP/UDP port numbers. For example, the
 * transport layer bundle objects identify flows based on their source or destination port numbers.
 */
typedef  struct
_ANSC_TCP_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    USHORT                          SrcPort1;
    USHORT                          SrcPort2;
    USHORT                          DstPort1;
    USHORT                          DstPort2;
}
ANSC_TCP_TRUNK,  *PANSC_TCP_TRUNK;

#define  ACCESS_ANSC_TCP_TRUNK(p)                   \
         ACCESS_CONTAINER(p, ANSC_TCP_TRUNK, Linkage)

typedef  struct
_ANSC_UDP_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    USHORT                          SrcPort1;
    USHORT                          SrcPort2;
    USHORT                          DstPort1;
    USHORT                          DstPort2;
}
ANSC_UDP_TRUNK,  *PANSC_UDP_TRUNK;

#define  ACCESS_ANSC_UDP_TRUNK(p)                   \
         ACCESS_CONTAINER(p, ANSC_UDP_TRUNK, Linkage)

typedef  struct
_ANSC_TCP_CIRCUIT
{
    SINGLE_LINK_ENTRY               Linkage;
    USHORT                          SrcPort;
    USHORT                          DstPort;
}
ANSC_TCP_CIRCUIT,  *PANSC_TCP_CIRCUIT;

#define  ACCESS_ANSC_TCP_CIRCUIT(p)                 \
         ACCESS_CONTAINER(p, ANSC_TCP_CIRCUIT, Linkage)

typedef  struct
_ANSC_UDP_CIRCUIT
{
    SINGLE_LINK_ENTRY               Linkage;
    USHORT                          SrcPort;
    USHORT                          DstPort;
}
ANSC_UDP_CIRCUIT,  *PANSC_UDP_CIRCUIT;

#define  ACCESS_ANSC_UDP_CIRCUIT(p)                 \
         ACCESS_CONTAINER(p, ANSC_UDP_CIRCUIT, Linkage)

typedef  struct
_ANSC_TCP_ADDR_POOL
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ANSC_HANDLE                     hAnscReserved;
    USHORT                          Port1;
    USHORT                          Port2;
    ULONG                           Size;
}
ANSC_TCP_ADDR_POOL,  *PANSC_TCP_ADDR_POOL;

#define  ACCESS_ANSC_TCP_ADDR_POOL(p)               \
         ACCESS_CONTAINER(p, ANSC_TCP_ADDR_POOL, Linkage)

typedef  struct
_ANSC_UDP_ADDR_POOL
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ANSC_HANDLE                     hAnscReserved;
    USHORT                          Port1;
    USHORT                          Port2;
    ULONG                           Size;
}
ANSC_UDP_ADDR_POOL,  *PANSC_UDP_ADDR_POOL;

#define  ACCESS_ANSC_UDP_ADDR_POOL(p)               \
         ACCESS_CONTAINER(p, ANSC_UDP_ADDR_POOL, Linkage)

typedef  struct
_ANSC_TCP_ADDR_AREA
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           LowerEdge;
    ULONG                           UpperEdge;
    QUEUE_HEADER                    RegionQueue;
}
ANSC_TCP_ADDR_AREA,  *PANSC_TCP_ADDR_AREA;

#define  ACCESS_ANSC_TCP_ADDR_AREA(p)               \
         ACCESS_CONTAINER(p, ANSC_TCP_ADDR_AREA, Linkage)

typedef  struct
_ANSC_UDP_ADDR_AREA
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           LowerEdge;
    ULONG                           UpperEdge;
    QUEUE_HEADER                    RegionQueue;
}
ANSC_UDP_ADDR_AREA,  *PANSC_UDP_ADDR_AREA;

#define  ACCESS_ANSC_UDP_ADDR_AREA(p)               \
         ACCESS_CONTAINER(p, ANSC_UDP_ADDR_AREA, Linkage)

typedef  struct
_ANSC_TCP_REGION
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hAddrPool;
}
ANSC_TCP_REGION,  *PANSC_TCP_REGION;

#define  ACCESS_ANSC_TCP_REGION(p)                  \
         ACCESS_CONTAINER(p, ANSC_TCP_REGION, Linkage)

typedef  struct
_ANSC_UDP_REGION
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hAddrPool;
}
ANSC_UDP_REGION,  *PANSC_UDP_REGION;

#define  ACCESS_ANSC_UDP_REGION(p)                  \
         ACCESS_CONTAINER(p, ANSC_UDP_REGION, Linkage)


/***********************************************************
      DEFINITION OF SOME GENERAL IPSEC DATA STRUCTURES
***********************************************************/

/*
 * Although not a straighforward approach, we implement similar bundle and selector objects for all
 * the Ipsec protocols. For Esp, Ah and Ipcomp, the general selectors are Spi and Cpi values.
 */
typedef  struct
_ANSC_ESP_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ULONG                           Spi1;
    ULONG                           Spi2;
}
ANSC_ESP_TRUNK,  *PANSC_ESP_TRUNK;

#define  ACCESS_ANSC_ESP_TRUNK(p)                   \
         ACCESS_CONTAINER(p, ANSC_ESP_TRUNK, Linkage)

typedef  struct
_ANSC_AH_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ULONG                           Spi1;
    ULONG                           Spi2;
}
ANSC_AH_TRUNK,  *PANSC_AH_TRUNK;

#define  ACCESS_ANSC_AH_TRUNK(p)                    \
         ACCESS_CONTAINER(p, ANSC_AH_TRUNK, Linkage)

typedef  struct
_ANSC_IPCOMP_TRUNK
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ULONG                           Spi1;
    ULONG                           Spi2;
}
ANSC_IPCOMP_TRUNK,  *PANSC_IPCOMP_TRUNK;

#define  ACCESS_ANSC_IPCOMP_TRUNK(p)                \
         ACCESS_CONTAINER(p, ANSC_IPCOMP_TRUNK, Linkage)

typedef  struct
_ANSC_SPI_CIRCUIT
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           Spi;
}
ANSC_SPI_CIRCUIT,  *PANSC_SPI_CIRCUIT;

#define  ACCESS_ANSC_SPI_CIRCUIT(p)                 \
         ACCESS_CONTAINER(p, ANSC_SPI_CIRCUIT, Linkage)

typedef  struct
_ANSC_SPI_ADDR_POOL
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ANSC_HANDLE                     hAnscReserved;
    ULONG                           Spi1;
    ULONG                           Spi2;
    ULONG                           Size;
}
ANSC_SPI_ADDR_POOL,  *PANSC_SPI_ADDR_POOL;

#define  ACCESS_ANSC_SPI_ADDR_POOL(p)               \
         ACCESS_CONTAINER(p, ANSC_SPI_ADDR_POOL, Linkage)

typedef  struct
_ANSC_SPI_ADDR_AREA
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           LowerEdge;
    ULONG                           UpperEdge;
    QUEUE_HEADER                    RegionQueue;
}
ANSC_SPI_ADDR_AREA,  *PANSC_SPI_ADDR_AREA;

#define  ACCESS_ANSC_SPI_ADDR_AREA(p)               \
         ACCESS_CONTAINER(p, ANSC_SPI_ADDR_AREA, Linkage)

typedef  struct
_ANSC_SPI_REGION
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hAddrPool;
}
ANSC_SPI_REGION,  *PANSC_SPI_REGION;

#define  ACCESS_ANSC_SPI_REGION(p)                  \
         ACCESS_CONTAINER(p, ANSC_SPI_REGION, Linkage)


/***********************************************************
    DEFINITION OF SOME GENERAL TCP/IP NETWORK STRUCTURES
***********************************************************/

/*
 * A socket entry encapsulates both network and transporot layer addresses into a single data structure.
 * Within our TCP/IP gateway framework, a socket entry contains the IPv4 address, transport type and
 * the port number.
 */
typedef  struct
_ANSC_IPV4_SOCKET_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_IPV4_ADDRESS               Address;
    UCHAR                           Transport;
    USHORT                          Port;
}
ANSC_IPV4_SOCKET_ENTRY,  *PANSC_IPV4_SOCKET_ENTRY;

#define  ACCESS_ANSC_IPV4_SOCKET_ENTRY(p)           \
         ACCESS_CONTAINER(p, ANSC_IPV4_SOCKET_ENTRY, Linkage)

typedef  struct
_ANSC_IPV4_SOCKET_BUNDLE
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_IPV4_ADDRESS               Address;
    ULONG                           AddressRange;
    UCHAR                           Transport;
    USHORT                          Port;
    USHORT                          PortRange;
}
ANSC_IPV4_SOCKET_BUNDLE,  *PANSC_IPV4_SOCKET_BUNDLE;

#define  ACCESS_ANSC_IPV4_SOCKET_BUNDLE(p)          \
         ACCESS_CONTAINER(p, ANSC_IPV4_SOCKET_BUNDLE, Linkage)

/*
 * Packet processing is nothing more than classification-action. In order to perform classification,
 * packets have to passed through a set of rules. Rule object matches the packets based on different
 * header fields. These header fields are known as "selector".
 */
typedef  struct
_ANSC_IPV4_SELECTOR
{
    ANSC_IPV4_ADDRESS               SrcAddress;
    ANSC_IPV4_ADDRESS               DstAddress;
    UCHAR                           Transport;
    UCHAR                           Tos;
}
ANSC_IPV4_SELECTOR,  *PANSC_IPV4_SELECTOR;

typedef  struct
_ANSC_TCP_SELECTOR
{
    USHORT                          SrcPort;
    USHORT                          DstPort;
}
ANSC_TCP_SELECTOR,  *PANSC_TCP_SELECTOR;

typedef  struct
_ANSC_UDP_SELECTOR
{
    USHORT                          SrcPort;
    USHORT                          DstPort;
}
ANSC_UDP_SELECTOR,  *PANSC_UDP_SELECTOR;

typedef  struct
_ANSC_ICMP_SELECTOR
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
}
ANSC_ICMP_SELECTOR,  *PANSC_ICMP_SELECTOR;

typedef  struct
_ANSC_ESP_SELECTOR
{
    ULONG                           Spi;
}
ANSC_ESP_SELECTOR,  *PANSC_ESP_SELECTOR;

typedef  struct
_ANSC_AH_SELECTOR
{
    ULONG                           Spi;
}
ANSC_AH_SELECTOR,  *PANSC_AH_SELECTOR;

typedef  struct
_ANSC_IPCOMP_SELECTOR
{
    USHORT                          Cpi;
}
ANSC_IPCOMP_SELECTOR,  *PANSC_IPCOMP_SELECTOR;


/***********************************************************
    DEFINITION OF SOME SPECIAL TCP/IP NETWORK STRUCTURES
***********************************************************/

/*
 * We shall define a similar structure with similar function as the "tcp_modification_block"
 * structure defined in VPCom and CyberWall. The Slee Session Tracking objects use this structure
 * to record all the changes been made through a Tcp session's life.
 */
typedef  struct
_ANSC_TCP_SESSION_CHANGE
{
    SINGLE_LINK_ENTRY               Linkage;
    LONG                            RelativeSeqDelta;
    LONG                            AbsoluteSeqDelta;
    ULONG                           OrgSeqNumber;
}
ANSC_TCP_SESSION_CHANGE,  *PANSC_TCP_SESSION_CHANGE;

#define  ACCESS_ANSC_TCP_SESSION_CHANGE(p)          \
         ACCESS_CONTAINER(p, ANSC_TCP_SESSION_CHANGE, Linkage)


/***********************************************************
       DEFINITION OF SWITCH FABRIC RELATED STRUCTURES
***********************************************************/

/*
 * When packet is passed from a service module to the switch fabric module, the fabric needs to
 * extract the destination port number from the packet. In a typical hardware implementation, this
 * information is usually prepended in front of the packet. But in our software-simulated switch
 * environment, we can pass more information without the physical limitation that normally imposed
 * to hardware solutions.
 */
typedef  struct
_ANSC_SWITCHING_PATH
{
    ULONG                           SrcModuleId;
    ULONG                           DstModuleId;
    ULONG                           Reserved;
}
ANSC_SWITCHING_PATH,  *PANSC_SWITCHING_PATH;


#endif
