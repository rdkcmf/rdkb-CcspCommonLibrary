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

    module:	ansc_packet_binary_ip4.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Internet Protocol (IP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/18/01    modify the file name from ansc_protocol_binary
                    to ansc_packet_binary.
        01/19/01    restructure the container object, add and
                    remove a few things
        09/11/01    add ppp related packet structures
        10/03/01    break it up into several header files; one for
                    each specific protocol suite

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_IP4_
#define  _ANSC_PACKET_BINARY_IP4_


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
       REGULARLY USED INTERNET ADDRESS/MASK DEFINITIONS
***********************************************************/

/*
 * There are three types of IP addresses: unicast (destined for a single host), broadcast (destined
 * for all hosts on a given network), and multicast (destined for a set of hosts that belong to a
 * multicast group). Note: all static IP address value appeared in macros and functions MUST be
 * defined in the network order.
 */
#ifdef   _ANSC_LITTLE_ENDIAN_

    #define  IPV4_LOOPBACK_ADDRESS                  0x0100007F      /* 127.0.0.1       */
    #define  IPV4_DEFAULT_BROADCAST_ADDRESS         0xFFFFFFFF      /* 255.255.255.255 */
    #define  IPV4_LIMITED_BROADCAST_ADDRESS         0xFFFFFFFF      /* 255.255.255.255 */

    #define  IPV4_CLASSA_SUBNET_MASK                0x000000FF      /* 255.0.0.0       */
    #define  IPV4_CLASSB_SUBNET_MASK                0x0000FFFF      /* 255.255.0.0     */
    #define  IPV4_CLASSC_SUBNET_MASK                0x00FFFFFF      /* 255.255.255.0   */

    #define  IPV4_CLASSA_ADDRESS_LO                 0x00000000      /* 0.0.0.0         */
    #define  IPV4_CLASSA_ADDRESS_HI                 0xFFFFFF7F      /* 127.255.255.255 */
    #define  IPV4_CLASSB_ADDRESS_LO                 0x00000080      /* 128.0.0.0       */
    #define  IPV4_CLASSB_ADDRESS_HI                 0xFFFFFFBF      /* 191.255.255.255 */
    #define  IPV4_CLASSC_ADDRESS_LO                 0x000000C0      /* 192.0.0.0       */
    #define  IPV4_CLASSC_ADDRESS_HI                 0xFFFFFFDF      /* 223.255.255.255 */
    #define  IPV4_CLASSD_ADDRESS_LO                 0x000000E0      /* 224.0.0.0       */
    #define  IPV4_CLASSD_ADDRESS_HI                 0xFFFFFFEF      /* 239.255.255.255 */
    #define  IPV4_CLASSE_ADDRESS_LO                 0x000000F0      /* 240.0.0.0       */
    #define  IPV4_CLASSE_ADDRESS_HI                 0xFFFFFFF7      /* 247.255.255.255 */

#else

    #define  IPV4_LOOPBACK_ADDRESS                  0x7F000001      /* 127.0.0.1       */
    #define  IPV4_DEFAULT_BROADCAST_ADDRESS         0xFFFFFFFF      /* 255.255.255.255 */
    #define  IPV4_LIMITED_BROADCAST_ADDRESS         0xFFFFFFFF      /* 255.255.255.255 */

    #define  IPV4_CLASSA_SUBNET_MASK                0xFF000000      /* 255.0.0.0       */
    #define  IPV4_CLASSB_SUBNET_MASK                0xFFFF0000      /* 255.255.0.0     */
    #define  IPV4_CLASSC_SUBNET_MASK                0xFFFFFF00      /* 255.255.255.0   */

    #define  IPV4_CLASSA_ADDRESS_LO                 0x00000000      /* 0.0.0.0         */
    #define  IPV4_CLASSA_ADDRESS_HI                 0x7FFFFFFF      /* 127.255.255.255 */
    #define  IPV4_CLASSB_ADDRESS_LO                 0x80000000      /* 128.0.0.0       */
    #define  IPV4_CLASSB_ADDRESS_HI                 0xBFFFFFFF      /* 191.255.255.255 */
    #define  IPV4_CLASSC_ADDRESS_LO                 0xC0000000      /* 192.0.0.0       */
    #define  IPV4_CLASSC_ADDRESS_HI                 0xDFFFFFFF      /* 223.255.255.255 */
    #define  IPV4_CLASSD_ADDRESS_LO                 0xE0000000      /* 224.0.0.0       */
    #define  IPV4_CLASSD_ADDRESS_HI                 0xEFFFFFFF      /* 239.255.255.255 */
    #define  IPV4_CLASSE_ADDRESS_LO                 0xF0000000      /* 240.0.0.0       */
    #define  IPV4_CLASSE_ADDRESS_HI                 0xF7FFFFFF      /* 247.255.255.255 */

#endif


/***********************************************************
                DATA STRUCTURES USED BY IP
***********************************************************/

/*
 * Define transport protocol type that might appear in the "transport_type" field of an IP header.
 */
#define  WILDCARD_TRANSPORT                         0xFF
#define  IP_TRANSPORT                               4
#define  IP4_TRANSPORT                              4
#define  ICMP_TRANSPORT                             1
#define  IGMP_TRANSPORT                             2
#define  TCP_TRANSPORT                              6
#define  UDP_TRANSPORT                              17
#define  GRE_TRANSPORT                              47
#define  AH_TRANSPORT                               51
#define  ESP_TRANSPORT                              50
#define  IPCOMP_TRANSPORT                           108
#define  OSPF_TRANSPORT                             89
#define  PIMDM_TRANSPORT                            103

#define  IP4_PROTOCOL_ICMP                          1
#define  IP4_PROTOCOL_IGMP                          2
#define  IP4_PROTOCOL_TCP                           6
#define  IP4_PROTOCOL_UDP                           17
#define  IP4_PROTOCOL_GRE                           47
#define  IP4_PROTOCOL_AH                            51
#define  IP4_PROTOCOL_ESP                           50
#define  IP4_PROTOCOL_IPCOMP                        108
#define  IP4_PROTOCOL_OSPF                          89
#define  IP4_PROTOCOL_PIMDM                         103

/*
 * Define fragmentation flags
 */
#define  FRAGMENT_RESERVE_FLAG                      0x8000
#define  DONT_FRAGMENT_FLAG                         0x4000
#define  MORE_FRAGMENT_FLAG                         0x2000
#define  FRAGMENT_OFFSET_MASK                       0x1FFF

/*
 * Define the IP header data structure without any extension definitions
 */
#define  MAX_IPV4_PAYLOAD_SIZE                      1480
#define  IPV4_MAX_PAYLOAD_SIZE                      1480
#define  IPV4_MAX_OPTIONS_SIZE                      ( 60 - sizeof(IPV4_HEADER) )
#define  IPV4_MAX_HEADER_SIZE                       60

#define  IPV4_VERSION_NUMBER                        4
#define  IPV4_DEFAULT_HEADER_SIZE                   5
#define  IPV4_DEFAULT_TOS                           0
#define  IPV4_DEFAULT_TTL                           64

#define  AnscIpv4IsAddrClassA(addr)                 ( (addr[0] >= 0  ) && (addr[0] < 128) )
#define  AnscIpv4IsAddrClassB(addr)                 ( (addr[0] >= 128) && (addr[0] < 192) )
#define  AnscIpv4IsAddrClassC(addr)                 ( (addr[0] >= 192) && (addr[0] < 224) )
#define  AnscIpv4IsAddrClassD(addr)                 ( (addr[0] >= 224) && (addr[0] < 240) )
#define  AnscIpv4IsAddrClassE(addr)                 ( (addr[0] >= 240) && (addr[0] < 248) )
#define  AnscIpv4IsAddrClassF(addr)                 ( (addr[0] >= 248)                    )

#define  AnscIpv4GetMaskBitsFromAddr                AnscGetIp4MaskBitsFromAddr
#define  AnscIpv4GetMaskAddrFromBits(mask_bits)     \
         (mask_bits == 0)? 0 : (AnscUlongFromHToN((0xFFFFFFFF >> (32 - mask_bits)) << (32 - mask_bits)))


/*
 * The function or purpose of Internet Protocol is to move datagrams through an interconnected set
 * of networks. This is done by passing the datagrams from one internet module to another until the
 * destination is reached. The internet modules reside in hosts and gateways in the internet system.
 * The datagrams are routed from one internet module to another through individual networks based
 * on the interpretation of an internet address. Thus, one important mechanism of the internet
 * protocol is the internet address.
 *
 *           0                   1                   2                   3   
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |Version|  IHL  |Type of Service|          Total Length         |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |         Identification        |Flags|      Fragment Offset    |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |  Time to Live |    Protocol   |         Header Checksum       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Source Address                          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                    Destination Address                        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                    Options                    |    Padding    |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * In the routing of messages from one internet module to another, datagrams may need to traverse a
 * network whose maximum packet size is smaller than the size of the datagram. To overcome this
 * difficulty, a fragmentation mechanism is provided in the internet protocol.
 */
typedef  struct
_IPV4_HEADER
{
    UCHAR                           VersionAndHeader;
    UCHAR                           Tos;
    USHORT                          TotalLength;
    USHORT                          Identification;
    USHORT                          FragmentInfo;
    UCHAR                           Ttl;
    UCHAR                           TransportType;
    USHORT                          Checksum;
    ANSC_IPV4_ADDRESS               SrcAddress;
    ANSC_IPV4_ADDRESS               DstAddress;
    UCHAR                           Options[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IPV4_HEADER,  *PIPV4_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIpv4HashAddress(a, t)                  AnscHashAddress(a, t)
#define  AnscIpv4EqualAddress(a, b)                 ( (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) )
#define  AnscIpv4IsMulticast(p)                     ( AnscIpv4IsAddrClassD(p->DstAddress.Dot) )
#define  AnscIpv4IsBroadcast(p)                     ( (AnscIpv4GetDstAddressValue(p) == 0xFFFFFFFF) || (AnscIpv4GetDstAddressValue(p) == 0) )

#define  AnscIpv4GetVersion(p)                      ( (p->VersionAndHeader & 0xF0) >> 4 )
#define  AnscIpv4GetHeaderSize(p)                   ( (p->VersionAndHeader & 0x0F) *  4 )
#define  AnscIpv4GetTos(p)                          ( p->Tos )
#define  AnscIpv4GetTotalLength(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->TotalLength   )) )
#define  AnscIpv4GetId(p)                           ( AnscUshortFromNToH(AnscReadUshort(&p->Identification)) )
#define  AnscIpv4GetFragmentInfo(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->FragmentInfo  )) )
#define  AnscIpv4GetTtl(p)                          ( p->Ttl )
#define  AnscIpv4GetTransportType(p)                ( p->TransportType )
#define  AnscIpv4GetChecksum(p)                     ( AnscReadUshort(&p->Checksum) )
#define  AnscIpv4GetSrcAddressDot(p)                ( p->SrcAddress.Dot )
#define  AnscIpv4GetSrcAddressValue(p)              ( AnscReadUlong(&p->SrcAddress.Value) )
#define  AnscIpv4GetDstAddressDot(p)                ( p->DstAddress.Dot )
#define  AnscIpv4GetDstAddressValue(p)              ( AnscReadUlong(&p->DstAddress.Value) )

#define  AnscIpv4SetVersion(p, v)                   ( p->VersionAndHeader = (p->VersionAndHeader & 0x0F) | (UCHAR)(v << 4) )
#define  AnscIpv4SetHeaderSize(p, s)                ( p->VersionAndHeader = (p->VersionAndHeader & 0xF0) | (UCHAR)s        )
#define  AnscIpv4SetTos(p, tos)                     ( p->Tos = (UCHAR)tos )
#define  AnscIpv4SetTotalLength(p, l)               AnscWriteUshort(&p->TotalLength,       AnscUshortFromHToN(l ))
#define  AnscIpv4SetId(p, id)                       AnscWriteUshort(&p->Identification,    AnscUshortFromHToN(id))
#define  AnscIpv4SetFragmentInfo(p, f)              AnscWriteUshort(&p->FragmentInfo,      AnscUshortFromHToN(f ))
#define  AnscIpv4SetTtl(p, ttl)                     ( p->Ttl           = (UCHAR)ttl  )
#define  AnscIpv4SetTransportType(p, type)          ( p->TransportType = (UCHAR)type )
#define  AnscIpv4SetChecksum(p, c)                  AnscWriteUshort(&p->Checksum,          c            )
#define  AnscIpv4SetSrcAddressDot(p, addr)          AnscWriteUlong (&p->SrcAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4SetSrcAddressValue(p, addr)        AnscWriteUlong (&p->SrcAddress.Value,  addr)
#define  AnscIpv4SetDstAddressDot(p, addr)          AnscWriteUlong (&p->DstAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4SetDstAddressValue(p, addr)        AnscWriteUlong (&p->DstAddress.Value,  addr)

#define  AnscIpv4IsFragment(p)                      \
         ( AnscIpv4GetFragmentInfo(p) & (MORE_FRAGMENT_FLAG | FRAGMENT_OFFSET_MASK) )
#define  AnscIpv4IsFirstFragment(p)                 ( (AnscIpv4GetFragmentInfo(p) & FRAGMENT_OFFSET_MASK) == 0 )
#define  AnscIpv4IsLastFragment(p)                  ( (AnscIpv4GetFragmentInfo(p) & MORE_FRAGMENT_FLAG  ) == 0 )
#define  AnscIpv4IsMoreFragment(p)                  ( (AnscIpv4GetFragmentInfo(p) & MORE_FRAGMENT_FLAG  ) != 0 )
#define  AnscIpv4IsDontFragment(p)                  ( (AnscIpv4GetFragmentInfo(p) & DONT_FRAGMENT_FLAG  ) != 0 )

#define  AnscIpv4GetFragmentFlags(p)                ( AnscIpv4GetFragmentInfo(p) & ~FRAGMENT_OFFSET_MASK )
#define  AnscIpv4GetFragmentOffset(p)               ( AnscIpv4GetFragmentInfo(p) &  FRAGMENT_OFFSET_MASK )

#define  AnscIpv4SetFragmentFlags(p, f)             AnscIpv4SetFragmentInfo(p, (USHORT)(AnscIpv4GetFragmentOffset(p) | f))
#define  AnscIpv4SetFragmentOffset(p, o)            AnscIpv4SetFragmentInfo(p, (USHORT)(AnscIpv4GetFragmentFlags(p)  | o))

#define  AnscIpv4GetPayload(p)                      ( (PVOID)((ULONG)p + AnscIpv4GetHeaderSize(p)) )
#define  AnscIpv4GetBodySize(p)                     ( AnscIpv4GetTotalLength(p) - (USHORT)AnscIpv4GetHeaderSize(p) )

/*
 * There're many places in the TCP/IP stack implementation where checksum value needs to be updated
 * due to address translation, TTL decrement or option handling. We define following macro to make
 * the programming easier and more consistent.
 */
#define  AnscIpv4UpdateChecksumUlong(ip_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  ip_checksum = AnscIpv4GetChecksum(ip_header);           \
            ULONG                   ul_value1   = old_value;                                \
            ULONG                   ul_value2   = new_value;                                \
                                                                                            \
            ip_checksum =                                                                   \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &ul_value1,                                                         \
                        sizeof(ul_value1),                                                  \
                        &ul_value2,                                                         \
                        sizeof(ul_value2),                                                  \
                        ip_checksum                                                         \
                    );                                                                      \
                                                                                            \
            AnscIpv4SetChecksum(ip_header, ip_checksum);                                    \
         }

#define  AnscIpv4UpdateChecksumUshort(ip_header, old_value, new_value)                      \
         {                                                                                  \
            USHORT                  ip_checksum = AnscIpv4GetChecksum(ip_header);           \
            USHORT                  us_value1   = old_value;                                \
            USHORT                  us_value2   = new_value;                                \
                                                                                            \
            ip_checksum =                                                                   \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &us_value1,                                                         \
                        sizeof(us_value1),                                                  \
                        &us_value2,                                                         \
                        sizeof(us_value2),                                                  \
                        ip_checksum                                                         \
                    );                                                                      \
                                                                                            \
            AnscIpv4SetChecksum(ip_header, ip_checksum);                                    \
         }

#define  AnscIpv4UpdateChecksumUchar(ip_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  ip_checksum = AnscIpv4GetChecksum(ip_header);           \
            UCHAR                   uc_value1   = old_value;                                \
            UCHAR                   uc_value2   = new_value;                                \
                                                                                            \
            ip_checksum =                                                                   \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &uc_value1,                                                         \
                        sizeof(uc_value1),                                                  \
                        &uc_value2,                                                         \
                        sizeof(uc_value2),                                                  \
                        ip_checksum                                                         \
                    );                                                                      \
                                                                                            \
            AnscIpv4SetChecksum(ip_header, ip_checksum);                                    \
         }

#define  AnscIpv4CalculateChecksum(ip_header)                                               \
         {                                                                                  \
            USHORT                  ip_checksum = 0;                                        \
                                                                                            \
            AnscIpv4SetChecksum(ip_header, 0);                                              \
                                                                                            \
            ip_checksum =                                                                   \
                AnscCalculateChecksum                                                       \
                    (                                                                       \
                        (PVOID)ip_header,                                                   \
                        AnscIpv4GetHeaderSize(ip_header)                                    \
                    );                                                                      \
                                                                                            \
            AnscIpv4SetChecksum(ip_header, ip_checksum);                                    \
         }

#define  AnscIpv4ResetChecksum                      AnscIpv4CalculateChecksum


/***********************************************************
          DEFINITIONS FOR VARIOUS IP HEADER OPTIONS
***********************************************************/

/*
 * The IP header options may appear or not in datagrams. They must be implemented by all IP modules
 * (host and gateways). What is optional is their transmission in any particular datagram, not
 * their implementation. In some environments the security option may be required in all datagrams.
 *
 * The option field is variable in length. There may be zero or more options. There are two cases
 * for the format of an option:
 *
 *      - A single octet of option-type.
 *      - An option-type octet, an option-length octet, and the actual option-data
 *        octets. The option-length octet counts the option-type octet and the option
 *        -length octet as well as the option-data octets.
 */
#define  IPV4_OPTION_FLAG_COPY                      0x80

#define  IPV4_OPTION_TYPE_EOL                       0           /* end of option list             */
#define  IPV4_OPTION_TYPE_NOP                       1           /* no operation                   */
#define  IPV4_OPTION_TYPE_SEC                       130         /* security                       */
#define  IPV4_OPTION_TYPE_LSRR                      131         /* loose source and record route  */
#define  IPV4_OPTION_TYPE_SSRR                      137         /* strict source and record route */
#define  IPV4_OPTION_TYPE_RR                        7           /* record route                   */
#define  IPV4_OPTION_TYPE_SID                       136         /* stream identifier              */
#define  IPV4_OPTION_TYPE_TS                        68          /* internet timestamp             */
#define  IPV4_OPTION_TYPE_RA                        148         /* router alert                   */

#define  AnscIpv4HasOptions(p)                      ( AnscIpv4GetHeaderSize(p) > sizeof(IPV4_HEADER) )

#define  AnscIpv4CopyOption(dst_option, src_option)                                         \
         {                                                                                  \
            AnscCopyMemory(dst_option, src_option, (ULONG)src_option->Length);              \
         }

#define  AnscIpv4GetFirstOption(ip_header, ip_option)                                       \
         {                                                                                  \
            if ( AnscIpv4GetHeaderSize(ip_header) <= sizeof(IPV4_HEADER) )                  \
            {                                                                               \
                ip_option = NULL;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                ip_option = (PIPV4_OPTION)ip_header->Options;                               \
            }                                                                               \
         }

#define  AnscIpv4GetNextOption(ip_header, this_option, next_option)                         \
         {                                                                                  \
            ULONG                   ip_header_size = AnscIpv4GetHeaderSize(ip_header);      \
                                                                                            \
            if ( !this_option )                                                             \
            {                                                                               \
                if ( AnscIpv4GetHeaderSize(ip_header) <= sizeof(IPV4_HEADER) )              \
                {                                                                           \
                    next_option = NULL;                                                     \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    next_option = (PIPV4_OPTION)ip_header->Options;                         \
                }                                                                           \
            }                                                                               \
            else if ( (this_option->Type == IPV4_OPTION_TYPE_EOL) ||                        \
                      (this_option->Type == IPV4_OPTION_TYPE_NOP) )                         \
            {                                                                               \
                next_option = (PIPV4_OPTION)((ULONG)this_option + 1);                       \
            }                                                                               \
            else if ( this_option->Length >= sizeof(IPV4_OPTION) )                          \
            {                                                                               \
                next_option = (PIPV4_OPTION)((ULONG)this_option + this_option->Length);     \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                next_option = NULL;                                                         \
            }                                                                               \
                                                                                            \
            if ( next_option && ((ULONG)next_option - (ULONG)ip_header) >= ip_header_size ) \
            {                                                                               \
                next_option = NULL;                                                         \
            }                                                                               \
         }

#define  AnscIpv4GetOptionByType(ip_header1, option_type1, ip_option1)                      \
         {                                                                                  \
            PIPV4_OPTION            cur_option1 = NULL;                                     \
            PIPV4_OPTION            cur_option2 = NULL;                                     \
                                                                                            \
            AnscIpv4GetFirstOption(ip_header1, cur_option1);                                \
                                                                                            \
            while ( cur_option1 )                                                           \
            {                                                                               \
                if ( AnscIpv4OptionGetType(cur_option1) == option_type1 )                   \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else if ( AnscIpv4OptionGetType(cur_option1) == IPV4_OPTION_TYPE_EOL )      \
                {                                                                           \
                    cur_option1 = NULL;                                                     \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                AnscIpv4GetNextOption(ip_header1, cur_option1, cur_option2);                \
                                                                                            \
                cur_option1 = cur_option2;                                                  \
            }                                                                               \
                                                                                            \
            ip_option1 = cur_option1;                                                       \
         }

#define  AnscIpv4ValidateOptions(ip_header, b_result)                                       \
         {                                                                                  \
            PIPV4_OPTION            cur_option     = NULL;                                  \
            ULONG                   ip_header_size = AnscIpv4GetHeaderSize(ip_header);      \
                                                                                            \
            b_result = TRUE;                                                                \
                                                                                            \
            AnscIpv4GetFirstOption(ip_header, cur_option);                                  \
                                                                                            \
            while ( cur_option )                                                            \
            {                                                                               \
                if ( ((ULONG)cur_option - (ULONG)ip_header) == ip_header_size )             \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else if ( ((ULONG)cur_option - (ULONG)ip_header) > ip_header_size )         \
                {                                                                           \
                    b_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( AnscIpv4OptionGetType(cur_option) == IPV4_OPTION_TYPE_EOL )       \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else if ( AnscIpv4OptionGetType(cur_option) == IPV4_OPTION_TYPE_NOP )       \
                {                                                                           \
                    cur_option = (PIPV4_OPTION)((ULONG)cur_option + 1);                     \
                                                                                            \
                    continue;                                                               \
                }                                                                           \
                else if ( cur_option->Length < sizeof(IPV4_OPTION) )                        \
                {                                                                           \
                    b_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    cur_option = (PIPV4_OPTION)((ULONG)cur_option + cur_option->Length);    \
                                                                                            \
                    continue;                                                               \
                }                                                                           \
            }                                                                               \
         }

/*
 * The following general data structure can represent most IP header options except the "end-of-
 * option-list" and the "no-operation" options.
 */
typedef  struct
_IPV4_OPTION
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
IPV4_OPTION,  *PIPV4_OPTION;

#define  AnscIpv4OptionGetType(p)                   ( p->Type                         )
#define  AnscIpv4OptionGetLength(p)                 ( p->Length                       )
#define  AnscIpv4OptionGetData(p)                   ( p->Data                         )
#define  AnscIpv4OptionGetDataSize(p)               ( p->Length - sizeof(IPV4_OPTION) )

#define  AnscIpv4OptionSetType(p, t)                ( p->Type   = (UCHAR)t            )
#define  AnscIpv4OptionSetLength(p, l)              ( p->Length = (UCHAR)l            )
#define  AnscIpv4OptionSetData(p, d)                AnscCopyMemory(p->Data, d, p->Length - 2)


/***********************************************************
    DEFINITIONS FOR LOOSE SOURCE AND RECORD ROUTE OPTION
***********************************************************/

/*
 * The loose source and record route (LSRR) option provides a means for the source of an internet
 * datagram to supply routing information to be used by the gateways in forwarding the datagram to
 * the destination, and to record the route information.
 *
 *          +--------+--------+--------+---------//--------+
 *          |10000011| length | pointer|     route data    |
 *          +--------+--------+--------+---------//--------+
 *
 * The option begins with the option type code. The second octet is the option length which
 * includes the option type code and the length octet, the pointer octet, and length-3 octets of
 * route data. The third octet is the pointer into the route data indicating the octet which begins
 * the next source address to be processed. The pointer is relative to this option, and the
 * smallest legal value for the pointer is 4.
 */
typedef  struct
_IPV4_OPTION_LSRR
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Pointer;
    ANSC_IPV4_ADDRESS               AddrArray[ANSC_ZERO_ARRAY_SIZE];
}
IPV4_OPTION_LSRR,  *PIPV4_OPTION_LSRR;

#define  AnscIpv4OptionLsrrGetType                  AnscIpv4OptionGetType
#define  AnscIpv4OptionLsrrGetLength                AnscIpv4OptionGetLength
#define  AnscIpv4OptionLsrrSetType                  AnscIpv4OptionSetType
#define  AnscIpv4OptionLsrrSetLength                AnscIpv4OptionSetLength

#define  AnscIpv4GetLsrrOption(ip_header2, lsrr_option)                                     \
         {                                                                                  \
            PIPV4_OPTION            gen_ip_option = NULL;                                   \
                                                                                            \
            AnscIpv4GetOptionByType(ip_header2, IPV4_OPTION_TYPE_LSRR, gen_ip_option);      \
                                                                                            \
            lsrr_option = (PIPV4_OPTION_LSRR)gen_ip_option;                                 \
         }

#define  AnscIpv4OptionLsrrValidate(p, v_result)                                            \
         {                                                                                  \
            if ( p->Length < sizeof(IPV4_OPTION_LSRR) )                                     \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer < (sizeof(IPV4_OPTION_LSRR) + 1) )                         \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Length - 3) % IPV4_ADDRESS_SIZE) != 0 )                          \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Pointer - 4) % IPV4_ADDRESS_SIZE) != 0 )                         \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer > (p->Length + 1) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionLsrrAdvance(p, addr)                                                 \
         {                                                                                  \
            ULONG                   next_addr_index = 0;                                    \
                                                                                            \
            if ( !AnscIpv4OptionLsrrIsOverflow(p) )                                         \
            {                                                                               \
                next_addr_index = AnscIpv4OptionLsrrGetNextAddrIndex(p);                    \
                                                                                            \
                AnscIpv4OptionLsrrSetAddrValue(p, next_addr_index, addr);                   \
                                                                                            \
                p->Pointer += IPV4_ADDRESS_SIZE;                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionLsrrIsOverflow(p)            ( p->Pointer >= (p->Length + 1)               )
#define  AnscIpv4OptionLsrrGetNextAddrIndex(p)      ( ((ULONG)p->Pointer - 4) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionLsrrGetNextAddrDot(p)        \
         ( &p->AddrArray[AnscIpv4OptionLsrrGetNextAddrIndex(p)].Dot[0]               )
#define  AnscIpv4OptionLsrrGetNextAddrValue(p)      \
         ( AnscReadUlong(&p->AddrArray[AnscIpv4OptionLsrrGetNextAddrIndex(p)].Value) )

#define  AnscIpv4OptionLsrrGetPointer(p)            ( p->Pointer                                         )
#define  AnscIpv4OptionLsrrSetPointer(p, pt)        ( p->Pointer = pt                                    )
#define  AnscIpv4OptionLsrrSetAddrPointer(p, pt)    ( p->Pointer = (UCHAR)((pt + 1) * IPV4_ADDRESS_SIZE) )

#define  AnscIpv4OptionLsrrGetAddrCount(p)          ( ((ULONG)p->Length - 3) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionLsrrGetAddrDot(p, i)         ( &p->AddrArray[i].Dot[0]                    )
#define  AnscIpv4OptionLsrrGetAddrValue(p, i)       ( AnscReadUlong(&p->AddrArray[i].Value)      )

#define  AnscIpv4OptionLsrrSetAddrCount(p, c)       ( p->Length = (UCHAR)(c * IPV4_ADDRESS_SIZE + 3)     )
#define  AnscIpv4OptionLsrrSetAddrDot(p, i, addr)   AnscWriteUlong(&p->AddrArray[i].Dot[0], *(PULONG)addr)
#define  AnscIpv4OptionLsrrSetAddrValue(p, i, addr) AnscWriteUlong(&p->AddrArray[i].Value,  addr         )


/***********************************************************
    DEFINITIONS FOR STRICT SOURCE AND RECORD ROUTE OPTION
***********************************************************/

/*
 * The strict source and record route (SSRR) option provides a means for the source of an internet
 * datagram to supply routing information to be used by the gateways in forwarding the datagram to
 * the destination, and to record the route information.
 *
 *          +--------+--------+--------+---------//--------+
 *          |10001001| length | pointer|     route data    |
 *          +--------+--------+--------+---------//--------+
 *
 * The option begins with the option type code. The second octet is the option length which
 * includes the option type code and the length octet, the pointer octet, and length-3 octets of
 * route data. The third octet is the pointer into the route data indicating the octet which begins
 * the next source address to be processed. The pointer is relative to this option, and the
 * smallest legal value for the pointer is 4.
 */
typedef  struct
_IPV4_OPTION_SSRR
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Pointer;
    ANSC_IPV4_ADDRESS               AddrArray[ANSC_ZERO_ARRAY_SIZE];
}
IPV4_OPTION_SSRR,  *PIPV4_OPTION_SSRR;

#define  AnscIpv4OptionSsrrGetType                  AnscIpv4OptionGetType
#define  AnscIpv4OptionSsrrGetLength                AnscIpv4OptionGetLength
#define  AnscIpv4OptionSsrrSetType                  AnscIpv4OptionSetType
#define  AnscIpv4OptionSsrrSetLength                AnscIpv4OptionSetLength

#define  AnscIpv4GetSsrrOption(ip_header2, ssrr_option)                                     \
         {                                                                                  \
            PIPV4_OPTION            gen_ip_option = NULL;                                   \
                                                                                            \
            AnscIpv4GetOptionByType(ip_header2, IPV4_OPTION_TYPE_SSRR, gen_ip_option);      \
                                                                                            \
            ssrr_option = (PIPV4_OPTION_SSRR)gen_ip_option;                                 \
         }

#define  AnscIpv4OptionSsrrValidate(p, v_result)                                            \
         {                                                                                  \
            if ( p->Length < sizeof(IPV4_OPTION_SSRR) )                                     \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer < (sizeof(IPV4_OPTION_SSRR) + 1) )                         \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Length - 3) % IPV4_ADDRESS_SIZE) != 0 )                          \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Pointer - 4) % IPV4_ADDRESS_SIZE) != 0 )                         \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer > (p->Length + 1) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionSsrrAdvance(p, addr)                                                 \
         {                                                                                  \
            ULONG                   next_addr_index = 0;                                    \
                                                                                            \
            if ( !AnscIpv4OptionSsrrIsOverflow(p) )                                         \
            {                                                                               \
                next_addr_index = AnscIpv4OptionSsrrGetNextAddrIndex(p);                    \
                                                                                            \
                AnscIpv4OptionSsrrSetAddrValue(p, next_addr_index, addr);                   \
                                                                                            \
                p->Pointer += IPV4_ADDRESS_SIZE;                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionSsrrIsOverflow(p)            ( p->Pointer >= (p->Length + 1)               )
#define  AnscIpv4OptionSsrrGetNextAddrIndex(p)      ( ((ULONG)p->Pointer - 4) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionSsrrGetNextAddrDot(p)        \
         ( &p->AddrArray[AnscIpv4OptionSsrrGetNextAddrIndex(p)].Dot[0]               )
#define  AnscIpv4OptionSsrrGetNextAddrValue(p)      \
         ( AnscReadUlong(&p->AddrArray[AnscIpv4OptionSsrrGetNextAddrIndex(p)].Value) )

#define  AnscIpv4OptionSsrrGetPointer(p)            ( p->Pointer                                         )
#define  AnscIpv4OptionSsrrSetPointer(p, pt)        ( p->Pointer = pt                                    )
#define  AnscIpv4OptionSsrrSetAddrPointer(p, pt)    ( p->Pointer = (UCHAR)((pt + 1) * IPV4_ADDRESS_SIZE) )

#define  AnscIpv4OptionSsrrGetAddrCount(p)          ( ((ULONG)p->Length - 3) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionSsrrGetAddrDot(p, i)         ( &p->AddrArray[i].Dot[0]                    )
#define  AnscIpv4OptionSsrrGetAddrValue(p, i)       ( AnscReadUlong(&p->AddrArray[i].Value)      )

#define  AnscIpv4OptionSsrrSetAddrCount(p, c)       ( p->Length = (UCHAR)(c * IPV4_ADDRESS_SIZE + 3)     )
#define  AnscIpv4OptionSsrrSetAddrDot(p, i, addr)   AnscWriteUlong(&p->AddrArray[i].Dot[0], *(PULONG)addr)
#define  AnscIpv4OptionSsrrSetAddrValue(p, i, addr) AnscWriteUlong(&p->AddrArray[i].Value,  addr         )


/***********************************************************
             DEFINITIONS FOR RECORD ROUTE OPTION
***********************************************************/

/*
 * The record route option provides a means to record the route of an internet datagram.
 *
 *          +--------+--------+--------+---------//--------+
 *          |00000111| length | pointer|     route data    |
 *          +--------+--------+--------+---------//--------+
 *
 * The option begins with the option type code. The second octet is the option length which
 * includes the option type code and the length octet, the pointer octet, and length-3 octets of
 * route data. The third octet is the pointer into the route data indicating the octet which begins
 * the next source address to be processed. The pointer is relative to this option, and the
 * smallest legal value for the pointer is 4.
 */
typedef  struct
_IPV4_OPTION_RR
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Pointer;
    ANSC_IPV4_ADDRESS               AddrArray[ANSC_ZERO_ARRAY_SIZE];
}
IPV4_OPTION_RR,  *PIPV4_OPTION_RR;

#define  AnscIpv4OptionRrGetType                    AnscIpv4OptionGetType
#define  AnscIpv4OptionRrGetLength                  AnscIpv4OptionGetLength
#define  AnscIpv4OptionRrSetType                    AnscIpv4OptionSetType
#define  AnscIpv4OptionRrSetLength                  AnscIpv4OptionSetLength

#define  AnscIpv4GetRrOption(ip_header2, rr_option)                                         \
         {                                                                                  \
            PIPV4_OPTION            gen_ip_option = NULL;                                   \
                                                                                            \
            AnscIpv4GetOptionByType(ip_header2, IPV4_OPTION_TYPE_RR, gen_ip_option);        \
                                                                                            \
            rr_option = (PIPV4_OPTION_RR)gen_ip_option;                                     \
         }

#define  AnscIpv4OptionRrValidate(p, v_result)                                              \
         {                                                                                  \
            if ( p->Length < sizeof(IPV4_OPTION_RR) )                                       \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer < (sizeof(IPV4_OPTION_RR) + 1) )                           \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Length - 3) % IPV4_ADDRESS_SIZE) != 0 )                          \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Pointer - 4) % IPV4_ADDRESS_SIZE) != 0 )                         \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer > (p->Length + 1) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionRrAdvance(p, addr)                                                   \
         {                                                                                  \
            ULONG                   next_addr_index = 0;                                    \
                                                                                            \
            if ( !AnscIpv4OptionRrIsOverflow(p) )                                           \
            {                                                                               \
                next_addr_index = AnscIpv4OptionRrGetNextAddrIndex(p);                      \
                                                                                            \
                AnscIpv4OptionRrSetAddrValue(p, next_addr_index, addr);                     \
                                                                                            \
                p->Pointer += IPV4_ADDRESS_SIZE;                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionRrIsOverflow(p)              ( p->Pointer >= (p->Length + 1)               )
#define  AnscIpv4OptionRrGetNextAddrIndex(p)        ( ((ULONG)p->Pointer - 4) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionRrGetNextAddrDot(p)          \
         ( &p->AddrArray[AnscIpv4OptionRrGetNextAddrIndex(p)].Dot[0]               )
#define  AnscIpv4OptionRrGetNextAddrValue(p)        \
         ( AnscReadUlong(&p->AddrArray[AnscIpv4OptionRrGetNextAddrIndex(p)].Value) )

#define  AnscIpv4OptionRrGetPointer(p)              ( p->Pointer                                         )
#define  AnscIpv4OptionRrSetPointer(p, pt)          ( p->Pointer = pt                                    )
#define  AnscIpv4OptionRrSetAddrPointer(p, pt)      ( p->Pointer = (UCHAR)((pt + 1) * IPV4_ADDRESS_SIZE) )

#define  AnscIpv4OptionRrGetAddrCount(p)            ( ((ULONG)p->Length - 3) / IPV4_ADDRESS_SIZE )
#define  AnscIpv4OptionRrGetAddrDot(p, i)           ( &p->AddrArray[i].Dot[0]                    )
#define  AnscIpv4OptionRrGetAddrValue(p, i)         ( AnscReadUlong(&p->AddrArray[i].Value)      )

#define  AnscIpv4OptionRrSetAddrCount(p, c)         ( p->Length = (UCHAR)(c * IPV4_ADDRESS_SIZE + 3)     )
#define  AnscIpv4OptionRrSetAddrDot(p, i, addr)     AnscWriteUlong(&p->AddrArray[i].Dot[0], *(PULONG)addr)
#define  AnscIpv4OptionRrSetAddrValue(p, i, addr)   AnscWriteUlong(&p->AddrArray[i].Value,  addr         )

#define  AnscIpv4OptionRrPrint(p)                                                           \
         {                                                                                  \
            ULONG                   rr_addr_count = AnscIpv4OptionRrGetNextAddrIndex(p);    \
            ULONG                   rr_addr_index = 0;                                      \
            PUCHAR                  rr_addr_dot   = NULL;                                   \
                                                                                            \
            AnscTrace("\n    Recorded Route:\n\n");                                         \
                                                                                            \
            for ( rr_addr_index = 0; rr_addr_index < rr_addr_count; rr_addr_index++ )       \
            {                                                                               \
                rr_addr_dot = AnscIpv4OptionRrGetAddrDot(p, rr_addr_index);                 \
                                                                                            \
                AnscTrace                                                                   \
                    (                                                                       \
                        "        %d - %d.%d.%d.%d\n",                                       \
                        rr_addr_index,                                                      \
                        *(rr_addr_dot + 0),                                                 \
                        *(rr_addr_dot + 1),                                                 \
                        *(rr_addr_dot + 2),                                                 \
                        *(rr_addr_dot + 3)                                                  \
                    );                                                                      \
            }                                                                               \
         }


/***********************************************************
          DEFINITIONS FOR INTERNET TIMESTAMP OPTION
***********************************************************/

/*
 * The Timestamp is a right-justified, 32-bit timestamp in milliseconds since midnight UT. If the
 * time is not available in milliseconds or cannot be provided with respect to midnight UT then any
 * time may be inserted as a timestamp provided the high order bit of the timestamp field is set to
 * one to indicate the use of a non-standard value.
 *
 *          +--------+--------+--------+--------+
 *          |01000100| length | pointer|oflw|flg|
 *          +--------+--------+--------+--------+
 *          |         internet address          |
 *          +--------+--------+--------+--------+
 *          |             timestamp             |
 *          +--------+--------+--------+--------+
 *          |                 .                 |
 *          |                 .                 |
 *          |                 .                 |
 *
 * The Option Length is the number of octets in the option counting the type, length, pointer, and
 * overflow/flag octets (maximum length 40). The Pointer is the number of octets from the beginning
 * of this option to the end of timestamps plus one (i.e., it points to the octet beginning the
 * space for next timestamp). The smallest legal value is 5.  The timestamp area is full when the
 * pointer is greater than the length.
 */
#define  IPV4_TS_FLAG_TSONLY                        0
#define  IPV4_TS_FLAG_TSADDR                        1
#define  IPV4_TS_FLAG_SEADDR                        3

typedef  struct
_IPV4_OPTION_TS
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Pointer;
    UCHAR                           OverflowAndFlag;
    ULONG                           Data[ANSC_ZERO_ARRAY_SIZE];
}
IPV4_OPTION_TS,  *PIPV4_OPTION_TS;

#define  AnscIpv4OptionTsGetType                    AnscIpv4OptionGetType
#define  AnscIpv4OptionTsGetLength                  AnscIpv4OptionGetLength
#define  AnscIpv4OptionTsSetType                    AnscIpv4OptionSetType
#define  AnscIpv4OptionTsSetLength                  AnscIpv4OptionSetLength

#define  AnscIpv4GetTsOption(ip_header2, ts_option)                                         \
         {                                                                                  \
            PIPV4_OPTION            gen_ip_option = NULL;                                   \
                                                                                            \
            AnscIpv4GetOptionByType(ip_header2, IPV4_OPTION_TYPE_TS, gen_ip_option);        \
                                                                                            \
            ts_option = (PIPV4_OPTION_TS)gen_ip_option;                                     \
         }

#define  AnscIpv4OptionTsValidate(p, v_result)                                              \
         {                                                                                  \
            if ( p->Length < sizeof(IPV4_OPTION_TS) )                                       \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer < (sizeof(IPV4_OPTION_TS) + 1) )                           \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Length - 4) % sizeof(ULONG)) != 0 )                              \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( ((p->Pointer - 5) % sizeof(ULONG)) != 0 )                             \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Pointer > (p->Length + 1) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionTsSetTs(p, addr, ts)                                                 \
         {                                                                                  \
            ULONG                   ts_index   = 0;                                         \
            ULONG                   ts_reverse = AnscUlongFromHToN(ts);                     \
                                                                                            \
            if ( !AnscIpv4OptionTsIsOverflow(p) )                                           \
            {                                                                               \
                ts_index = AnscIpv4OptionTsGetNextTsIndex(p);                               \
                                                                                            \
                switch ( AnscIpv4OptionTsGetFlag(p) )                                       \
                {                                                                           \
                    case    IPV4_TS_FLAG_TSONLY :                                           \
                                                                                            \
                            AnscWriteUlong(&p->Data[ts_index], ts_reverse);                 \
                                                                                            \
                            p->Pointer += (UCHAR)sizeof(ULONG);                             \
                                                                                            \
                            break;                                                          \
                                                                                            \
                    case    IPV4_TS_FLAG_TSADDR :                                           \
                                                                                            \
                            AnscWriteUlong(&p->Data[ts_index + 0], addr      );             \
                            AnscWriteUlong(&p->Data[ts_index + 1], ts_reverse);             \
                                                                                            \
                            p->Pointer += (UCHAR)(sizeof(ULONG) * 2);                       \
                                                                                            \
                            break;                                                          \
                                                                                            \
                    case    IPV4_TS_FLAG_SEADDR :                                           \
                                                                                            \
                            if ( AnscReadUlong(&p->Data[ts_index]) == addr )                \
                            {                                                               \
                                AnscWriteUlong(&p->Data[ts_index + 1], ts_reverse);         \
                                                                                            \
                                p->Pointer += (UCHAR)(sizeof(ULONG) * 2);                   \
                            }                                                               \
                                                                                            \
                            break;                                                          \
                                                                                            \
                    default :                                                               \
                                                                                            \
                            break;                                                          \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                AnscIpv4OptionTsIncOflwCount(p);                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionTsIsOverflow(p)              ( p->Pointer >= (p->Length + 1)           )
#define  AnscIpv4OptionTsGetNextTsIndex(p)          ( ((ULONG)p->Pointer - 5) / sizeof(ULONG) )
#define  AnscIpv4OptionTsGetOflwCount(p)            ( (p->OverflowAndFlag & 0xF0) >> 4        )
#define  AnscIpv4OptionTsGetFlag(p)                 ( (p->OverflowAndFlag & 0x0F)             )

#define  AnscIpv4OptionTsSetOflwCount(p, c)         ( p->OverflowAndFlag = (p->OverflowAndFlag & 0x0F) | (UCHAR)(c << 4) )
#define  AnscIpv4OptionTsSetFlag(p, f)              ( p->OverflowAndFlag = (p->OverflowAndFlag & 0xF0) | (UCHAR)f        )

#define  AnscIpv4OptionTsIncOflwCount(p)                                                    \
         {                                                                                  \
            UCHAR                   cur_oflw_count = AnscIpv4OptionTsGetOflwCount(p);       \
                                                                                            \
            if ( cur_oflw_count < 0x0F )                                                    \
            {                                                                               \
                AnscIpv4OptionTsSetOflwCount(p, (cur_oflw_count + 1));                      \
            }                                                                               \
         }


/***********************************************************
             DEFINITIONS FOR ROUTER ALERT OPTION
***********************************************************/

/*
 * The Router Alert option has the semantic "routers should examine this packet more closely". By
 * including the Router Alert option in the IP header of its protocol message, RSVP can cause the
 * message to be intercepted while causing little or no performance penalty on the forwarding of
 * normal data packets. This option is defined in RFC 2113.
 *
 *          +--------+--------+--------+--------+
 *          |10010100|00000100|  2 octet value  |
 *          +--------+--------+--------+--------+
 *
 * Type:
 *
 *      $ Copied flag   - 1  (all fragments must carry the option)
 *      $ Option class  - 0  (control)
 *      $ Option number - 20 (decimal)
 *
 * Length: 4
 *
 * Value: A two octet code with the following values
 *
 *      $ 0         - Router shall examine packet
 *      $ 1 ~ 65535 - Reserved
 */
#define  IPV4_RA_VALUE_RSEP                         0

typedef  struct
_IPV4_OPTION_RA
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          Value;
}
IPV4_OPTION_RA,  *PIPV4_OPTION_RA;

#define  AnscIpv4OptionRaGetType                    AnscIpv4OptionGetType
#define  AnscIpv4OptionRaGetLength                  AnscIpv4OptionGetLength
#define  AnscIpv4OptionRaSetType                    AnscIpv4OptionSetType
#define  AnscIpv4OptionRaSetLength                  AnscIpv4OptionSetLength

#define  AnscIpv4GetRaOption(ip_header2, ra_option)                                         \
         {                                                                                  \
            PIPV4_OPTION            gen_ip_option = NULL;                                   \
                                                                                            \
            AnscIpv4GetOptionByType(ip_header2, IPV4_OPTION_TYPE_RA, gen_ip_option);        \
                                                                                            \
            ra_option = (PIPV4_OPTION_RA)gen_ip_option;                                     \
         }

#define  AnscIpv4OptionRaValidate(p, v_result)                                              \
         {                                                                                  \
            if ( p->Length != sizeof(IPV4_OPTION_RA) )                                      \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscIpv4OptionRaGetValue(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->Value)) )
#define  AnscIpv4OptionRaSetValue(p, v)             AnscWriteUshort(&p->Value, AnscUshortFromHToN(v))


/***********************************************************
          DATA STRUCTURES USED BY IP PSEUDO HEADER
***********************************************************/

/*
 * Define the pseudo header that will be used in the checksum calculation for some transport
 * protocols, such as Tcp and Udp.
 */
typedef  struct
_IPV4_PSEUDO_HEADER
{
    ANSC_IPV4_ADDRESS               SrcAddress;
    ANSC_IPV4_ADDRESS               DstAddress;
    UCHAR                           Reserved;
    UCHAR                           Type;
    USHORT                          Length;
}_struct_pack_
IPV4_PSEUDO_HEADER,  *PIPV4_PSEUDO_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIpv4PseudoGetSrcAddressDot(p)          ( p->SrcAddress.Dot )
#define  AnscIpv4PseudoGetSrcAddressValue(p)        ( AnscReadUlong(&p->SrcAddress.Value) )
#define  AnscIpv4PseudoGetDstAddressDot(p)          ( p->DstAddress.Dot )
#define  AnscIpv4PseudoGetDstAddressValue(p)        ( AnscReadUlong(&p->DstAddress.Value) )
#define  AnscIpv4PseudoGetReserved(p)               ( p->Reserved )
#define  AnscIpv4PseudoGetType(p)                   ( p->Type )
#define  AnscIpv4PseudoGetLength(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )

#define  AnscIpv4PseudoSetSrcAddressDot(p, addr)    AnscWriteUlong (&p->SrcAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4PseudoSetSrcAddressValue(p, addr)  AnscWriteUlong (&p->SrcAddress.Value,  addr)
#define  AnscIpv4PseudoSetDstAddressDot(p, addr)    AnscWriteUlong (&p->DstAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4PseudoSetDstAddressValue(p, addr)  AnscWriteUlong (&p->DstAddress.Value,  addr)
#define  AnscIpv4PseudoSetReserved(p, r)            ( p->Reserved = (UCHAR)r    )
#define  AnscIpv4PseudoSetType(p, type)             ( p->Type     = (UCHAR)type )
#define  AnscIpv4PseudoSetLength(p, l)              AnscWriteUshort(&p->Length,            AnscUshortFromHToN(l))


/***********************************************************
          DATA STRUCTURES USED BY SOFTNPU COMPONENTS
***********************************************************/

/*
 * We pack all the selector fields of layer 3 and layer 4 into a sixteen bytes 'path_string'. The
 * format of the typical 'path_string' is shown below:
 *
 *           0                   1                   2                   3   
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                    Destination Address                        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Source Address                          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |       TOS     |    Protocol   | Fragment Flag |   TCP Flags   |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |       Destination Port        |          Source Port          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_IPV4_CIRCUIT_INFO
{
    ANSC_IPV4_ADDRESS               DstAddress;
    ANSC_IPV4_ADDRESS               SrcAddress;
    UCHAR                           Tos;
    UCHAR                           Protocol;
    UCHAR                           FragmentFlags;
    UCHAR                           TcpFlags;
    USHORT                          DstPort;
    USHORT                          SrcPort;
}_struct_pack_
IPV4_CIRCUIT_INFO,  *PIPV4_CIRCUIT_INFO;

#define  AnscIpv4CircuitGetDstAddressDot(p)         ( p->DstAddress.Dot                               )
#define  AnscIpv4CircuitGetDstAddressValue(p)       ( AnscReadUlong(&p->DstAddress.Value)             )
#define  AnscIpv4CircuitGetSrcAddressDot(p)         ( p->SrcAddress.Dot                               )
#define  AnscIpv4CircuitGetSrcAddressValue(p)       ( AnscReadUlong(&p->SrcAddress.Value)             )
#define  AnscIpv4CircuitGetTos(p)                   ( p->Tos                                          )
#define  AnscIpv4CircuitGetProtocol(p)              ( p->Protocol                                     )
#define  AnscIpv4CircuitGetFragmentFlags(p)         ( p->FragmentFlags                                )
#define  AnscIpv4CircuitGetTcpFlags(p)              ( p->TcpFlags                                     )
#define  AnscIpv4CircuitGetDstPort(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->DstPort)) )
#define  AnscIpv4CircuitGetSrcPort(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->SrcPort)) )

#define  AnscIpv4CircuitSetDstAddressDot(p, addr)   AnscWriteUlong(&p->DstAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4CircuitSetDstAddressValue(p, addr) AnscWriteUlong(&p->DstAddress.Value,  addr         )
#define  AnscIpv4CircuitSetSrcAddressDot(p, addr)   AnscWriteUlong(&p->SrcAddress.Dot[0], *(PULONG)addr)
#define  AnscIpv4CircuitSetSrcAddressValue(p, addr) AnscWriteUlong(&p->SrcAddress.Value,  addr         )
#define  AnscIpv4CircuitSetTos(p, tos)              ( p->Tos           = (UCHAR)tos  )
#define  AnscIpv4CircuitSetProtocol(p, prot)        ( p->Protocol      = (UCHAR)prot )
#define  AnscIpv4CircuitSetFragmentFlags(p, flag)   ( p->FragmentFlags = (UCHAR)flag )
#define  AnscIpv4CircuitSetTcpFlags(p, flag)        ( p->TcpFlags      = (UCHAR)flag )
#define  AnscIpv4CircuitSetDstPort(p, port)         AnscWriteUshort(&p->DstPort, AnscUshortFromHToN(port))
#define  AnscIpv4CircuitSetSrcPort(p, port)         AnscWriteUshort(&p->SrcPort, AnscUshortFromHToN(port))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
