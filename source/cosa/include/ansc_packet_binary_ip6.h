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

    module:	ansc_packet_binary_ip6.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Internet Protocol Version 6 (IPv6).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        14/09/11    initial revision.


**********************************************************************/

#ifndef  _ANSC_PACKET_BINARY_IP6_
#define  _ANSC_PACKET_BINARY_IP6_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif


/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */
#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
                DATA STRUCTURES USED BY IPv6
***********************************************************/

#define  IP6_HEADER_LENGTH                          40

/*
 * Define the type of header immediately following the IPv6 header.
 */

#define  IP6_PROTOCOL_ICMP                          58


/*
 * The function or purpose of Internet Protocol is to move datagrams through an interconnected set
 * of networks. This is done by passing the datagrams from one internet module to another until the
 * destination is reached. The internet modules reside in hosts and gateways in the internet system.
 * The datagrams are routed from one internet module to another through individual networks based
 * on the interpretation of an internet address. Thus, one important mechanism of the internet
 * protocol is the internet address.
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |Version| Traffic Class |           Flow Label                  |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |         Payload Length        |  Next Header  |   Hop Limit   |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                                                               |
 *     +                                                               +
 *     |                                                               |
 *     +                         Source Address                        +
 *     |                                                               |
 *     +                                                               +
 *     |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                                                               |
 *     +                                                               +
 *     |                                                               |
 *     +                      Destination Address                      +
 *     |                                                               |
 *     +                                                               +
 *     |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */

 typedef  struct
_IPV6_HEADER
{
    ULONG                           VersionTcAndFl; /* 4 bits version, 8 bits TC, 20 bits flow-ID */
    USHORT                          PayloadLength;
    UCHAR                           NextHeader;
    UCHAR                           HopLimit;
    ULONG                           SourceAddress[4];
    ULONG                           DestinationAddress[4];
    UCHAR                           Extension[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IPV6_HEADER,  *PIPV6_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIpv6GetVersion(p)                      ( (p->VersionTcAndFl & 0xF0000000) >> 28 )
#define  AnscIpv6GetTrafficClass(p)                 ( (p->VersionTcAndFl & 0x0FF00000) >> 20 )
#define  AnscIpv6GetFlowLable(p)                    (  p->VersionTcAndFl & 0x000FFFFF        )
#define  AnscIpv6GetPayloadLength(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->PayloadLength)) )
#define  AnscIpv6GetNextHeader(p)                   ( p->NextHeader )
#define  AnscIpv6GetHopLimit(p)                     ( p->HopLimit )
#define  AnscIpv6GetSrcAddress(p)                   ( p->SourceAddress )
#define  AnscIpv6GetDstAddress(p)                   ( p->DestinationAddress )
#define  AnscIpv6GetSecondHeader(p)                 ( (PVOID)((ULONG)p + IP6_HEADER_LENGTH) )

#endif
