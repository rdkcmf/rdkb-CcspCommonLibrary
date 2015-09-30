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

    module:	ansc_packet_binary_udp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the User Datagram Protocol (UDP).

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


#ifndef  _ANSC_PACKET_BINARY_UDP_
#define  _ANSC_PACKET_BINARY_UDP_


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
                 DATA STRUCTURES USED BY UDP
***********************************************************/

/*
 * The User Datagram Protocol (UDP) is defined to make available a datagram mode of packet-switched
 * computer communication in the environment of an interconnected set of computer networks. This
 * protocol assumes that the Internet Protocol (IP) is used as the underlying protocol. It provides
 * a procedure for application programs to send messages to other programs with a minimum of
 * protocol mechanism. The protocol is transaction oriented, and delivery and duplicate protection
 * are not guaranteed.
 *
 *           0      7 8     15 16    23 24    31  
 *          +--------+--------+--------+--------+ 
 *          |     Source      |   Destination   | 
 *          |      Port       |      Port       | 
 *          +--------+--------+--------+--------+ 
 *          |                 |                 | 
 *          |     Length      |    Checksum     | 
 *          +--------+--------+--------+--------+ 
 *          |                                   |
 *          ~          data octets ...          ~
 *          |                                   |
 *          +-----------------------------------+
 */
#define  MAX_UDP_PAYLOAD_SIZE                       1472

typedef  struct
_UDP_HEADER
{
    USHORT                          SrcPortNumber;
    USHORT                          DstPortNumber;
    USHORT                          TotalLength;
    USHORT                          Checksum;
}_struct_pack_
UDP_HEADER,  *PUDP_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscUdpGetSrcPort(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->SrcPortNumber)) )
#define  AnscUdpGetDstPort(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->DstPortNumber)) )
#define  AnscUdpGetTotalLength(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->TotalLength  )) )
#define  AnscUdpGetChecksum(p)                      ( AnscReadUshort(&p->Checksum)                          )
#define  AnscUdpGetPayload(p)                       ( (PVOID)((ULONG)p + sizeof(UDP_HEADER))                )
#define  AnscUdpGetPayloadSize(p)                   ( AnscUdpGetTotalLength(p) - sizeof(UDP_HEADER)         )

#define  AnscUdpSetSrcPort(p, n)                    AnscWriteUshort(&p->SrcPortNumber, AnscUshortFromHToN(n))
#define  AnscUdpSetDstPort(p, n)                    AnscWriteUshort(&p->DstPortNumber, AnscUshortFromHToN(n))
#define  AnscUdpSetTotalLength(p, l)                AnscWriteUshort(&p->TotalLength,   AnscUshortFromHToN(l))
#define  AnscUdpSetChecksum(p, c)                   AnscWriteUshort(&p->Checksum,      c                    )

#define  AnscUdpPack                                (ULONG)AnscGetPackingNumber

#define  AnscUdpCalculateChecksum(ip4_header, udp_header, udp_body)                         \
         {                                                                                  \
            USHORT                  udp_checksum   = 0;                                     \
            USHORT                  udp_total_size = AnscUdpGetTotalLength(udp_header);     \
            USHORT                  udp_body_size  = AnscUdpGetPayloadSize(udp_header);     \
            ULONG                   dst_ip4_value  = AnscIpv4GetDstAddressValue(ip4_header);\
            ULONG                   src_ip4_value  = AnscIpv4GetSrcAddressValue(ip4_header);\
            ULONG                   tmp_checksum   = 0;                                     \
            IPV4_PSEUDO_HEADER      ip4_pseudo_header;                                      \
                                                                                            \
            AnscIpv4PseudoSetDstAddressValue((&ip4_pseudo_header), dst_ip4_value );         \
            AnscIpv4PseudoSetSrcAddressValue((&ip4_pseudo_header), src_ip4_value );         \
            AnscIpv4PseudoSetReserved       ((&ip4_pseudo_header), 0             );         \
            AnscIpv4PseudoSetType           ((&ip4_pseudo_header), UDP_TRANSPORT );         \
            AnscIpv4PseudoSetLength         ((&ip4_pseudo_header), udp_total_size);         \
                                                                                            \
            AnscUdpSetChecksum(udp_header, 0);                                              \
                                                                                            \
            tmp_checksum += AnscUdpPack(&ip4_pseudo_header, sizeof(IPV4_PSEUDO_HEADER));    \
            tmp_checksum += AnscUdpPack(udp_header,         sizeof(UDP_HEADER)        );    \
            tmp_checksum += AnscUdpPack(udp_body,           udp_body_size             );    \
                                                                                            \
            while ( tmp_checksum >> 16 )                                                    \
            {                                                                               \
                tmp_checksum = (tmp_checksum & 0xFFFF) + (tmp_checksum >> 16);              \
            }                                                                               \
                                                                                            \
            udp_checksum = ~((USHORT)tmp_checksum);                                         \
                                                                                            \
            AnscUdpSetChecksum(udp_header, udp_checksum);                                   \
         }

#define  AnscUdpCalculateChecksum2(ip4_header, udp_header, udp_bpn)                         \
         {                                                                                  \
            USHORT                  udp_checksum   = 0;                                     \
            USHORT                  udp_total_size = AnscUdpGetTotalLength(udp_header);     \
            USHORT                  udp_body_size  = AnscUdpGetPayloadSize(udp_header);     \
            ULONG                   dst_ip4_value  = AnscIpv4GetDstAddressValue(ip4_header);\
            ULONG                   src_ip4_value  = AnscIpv4GetSrcAddressValue(ip4_header);\
            ULONG                   tmp_checksum   = 0;                                     \
            IPV4_PSEUDO_HEADER      ip4_pseudo_header;                                      \
                                                                                            \
            AnscIpv4PseudoSetDstAddressValue((&ip4_pseudo_header), dst_ip4_value );         \
            AnscIpv4PseudoSetSrcAddressValue((&ip4_pseudo_header), src_ip4_value );         \
            AnscIpv4PseudoSetReserved       ((&ip4_pseudo_header), 0             );         \
            AnscIpv4PseudoSetType           ((&ip4_pseudo_header), UDP_TRANSPORT );         \
            AnscIpv4PseudoSetLength         ((&ip4_pseudo_header), udp_total_size);         \
                                                                                            \
            AnscUdpSetChecksum(udp_header, 0);                                              \
                                                                                            \
            tmp_checksum += AnscUdpPack(&ip4_pseudo_header, sizeof(IPV4_PSEUDO_HEADER));    \
            tmp_checksum += AnscUdpPack(udp_header,         sizeof(UDP_HEADER)        );    \
            tmp_checksum += udp_bpn;                                                        \
                                                                                            \
            while ( tmp_checksum >> 16 )                                                    \
            {                                                                               \
                tmp_checksum = (tmp_checksum & 0xFFFF) + (tmp_checksum >> 16);              \
            }                                                                               \
                                                                                            \
            udp_checksum = ~((USHORT)tmp_checksum);                                         \
                                                                                            \
            AnscUdpSetChecksum(udp_header, udp_checksum);                                   \
         }

#define  AnscUdpVerifyChecksum(ip4_header, udp_header, udp_body, v_result)                  \
         {                                                                                  \
            USHORT                  udp_checksum1 = AnscUdpGetChecksum(udp_header);         \
            USHORT                  udp_checksum2 = 0;                                      \
                                                                                            \
            AnscUdpCalculateChecksum(ip4_header, udp_header, udp_body);                     \
                                                                                            \
            udp_checksum2 = AnscUdpGetChecksum(udp_header);                                 \
                                                                                            \
            AnscUdpSetChecksum(udp_header, udp_checksum1);                                  \
                                                                                            \
            v_result = (udp_checksum1 == udp_checksum2)? TRUE : FALSE;                      \
         }

#define  AnscUdpVerifyChecksum2(ip4_header, udp_header, udp_bpn, v_result)                  \
         {                                                                                  \
            USHORT                  udp_checksum1 = AnscUdpGetChecksum(udp_header);         \
            USHORT                  udp_checksum2 = 0;                                      \
                                                                                            \
            AnscUdpCalculateChecksum2(ip4_header, udp_header, udp_bpn);                     \
                                                                                            \
            udp_checksum2 = AnscUdpGetChecksum(udp_header);                                 \
                                                                                            \
            AnscUdpSetChecksum(udp_header, udp_checksum1);                                  \
                                                                                            \
            v_result = (udp_checksum1 == udp_checksum2)? TRUE : FALSE;                      \
         }

#define  AnscUdpUpdateChecksumUlong(udp_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  udp_checksum = AnscUdpGetChecksum(udp_header);          \
            ULONG                   ul_value1    = old_value;                               \
            ULONG                   ul_value2    = new_value;                               \
                                                                                            \
            if ( udp_checksum != 0 )                                                        \
            {                                                                               \
                udp_checksum =                                                              \
                    AnscReplaceChecksum                                                     \
                        (                                                                   \
                            &ul_value1,                                                     \
                            sizeof(ul_value1),                                              \
                            &ul_value2,                                                     \
                            sizeof(ul_value2),                                              \
                            udp_checksum                                                    \
                        );                                                                  \
                                                                                            \
                AnscUdpSetChecksum(udp_header, udp_checksum);                               \
            }                                                                               \
         }

#define  AnscUdpUpdateChecksumUshort(udp_header, old_value, new_value)                      \
         {                                                                                  \
            USHORT                  udp_checksum = AnscUdpGetChecksum(udp_header);          \
            USHORT                  us_value1    = old_value;                               \
            USHORT                  us_value2    = new_value;                               \
                                                                                            \
            if ( udp_checksum != 0 )                                                        \
            {                                                                               \
                udp_checksum =                                                              \
                    AnscReplaceChecksum                                                     \
                        (                                                                   \
                            &us_value1,                                                     \
                            sizeof(us_value1),                                              \
                            &us_value2,                                                     \
                            sizeof(us_value2),                                              \
                            udp_checksum                                                    \
                        );                                                                  \
                                                                                            \
                AnscUdpSetChecksum(udp_header, udp_checksum);                               \
            }                                                                               \
         }

#define  AnscUdpUpdateChecksumUchar(udp_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  udp_checksum = AnscUdpGetChecksum(udp_header);          \
            UCHAR                   uc_value1    = old_value;                               \
            UCHAR                   uc_value2    = new_value;                               \
                                                                                            \
            if ( udp_checksum != 0 )                                                        \
            {                                                                               \
                udp_checksum =                                                              \
                    AnscReplaceChecksum                                                     \
                        (                                                                   \
                            &uc_value1,                                                     \
                            sizeof(uc_value1),                                              \
                            &uc_value2,                                                     \
                            sizeof(uc_value2),                                              \
                            udp_checksum                                                    \
                        );                                                                  \
                                                                                            \
                AnscUdpSetChecksum(udp_header, udp_checksum);                               \
            }                                                                               \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
