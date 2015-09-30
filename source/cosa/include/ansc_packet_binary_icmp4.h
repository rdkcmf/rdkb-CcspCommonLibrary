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

    module:	ansc_packet_binary_icmp4.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Internet Control Message Protocol
        (ICMP).

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


#ifndef  _ANSC_PACKET_BINARY_ICMP4_
#define  _ANSC_PACKET_BINARY_ICMP4_


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
                DATA STRUCTURES USED BY ICMP
***********************************************************/

/*
 * Define some const values used in the ICMP header definition
 */
#define  ICMP_TYPE_ECHO_REPLY                       0
#define  ICMP_TYPE_DESTINATION_UNREACHABLE          3
#define  ICMP_TYPE_SOURCE_QUENCH                    4
#define  ICMP_TYPE_REDIRECT                         5
#define  ICMP_TYPE_ECHO_REQUEST                     8
#define  ICMP_TYPE_ROUTER_ADVERTISEMENT             9
#define  ICMP_TYPE_ROUTER_SOLICITATION              10
#define  ICMP_TYPE_TIME_EXCEEDED                    11
#define  ICMP_TYPE_PARAMETER_PROBLEM                12
#define  ICMP_TYPE_TIMESTAMP_REQUEST                13
#define  ICMP_TYPE_TIMESTAMP_REPLY                  14
#define  ICMP_TYPE_INFORMATION_REQUEST              15
#define  ICMP_TYPE_INFORMATION_REPLY                16
#define  ICMP_TYPE_ADDRESS_MASK_REQUEST             17
#define  ICMP_TYPE_ADDRESS_MASK_REPLY               18

#define  ICMP_TYPE_RESERVED                         128
#define  ICMP_TYPE_HOST_TRAP                        ICMP_TYPE_RESERVED  + 1

/*
 * Following is a list of codes may appear under icmp_destination_unreachable_type.
 */
#define  ICMP_DUCODE_NETWORK_UNREACHABLE            0
#define  ICMP_DUCODE_HOST_UNREACHABLE               1
#define  ICMP_DUCODE_PROTOCOL_UNREACHABLE           2
#define  ICMP_DUCODE_PORT_UNREACHABLE               3
#define  ICMP_DUCODE_DF_SET                         4
#define  ICMP_DUCODE_SOURCE_ROUTE_FAILURE           5
#define  ICMP_DUCODE_DST_NETWORK_UNKNOWN            6
#define  ICMP_DUCODE_DST_HOST_UNKNOWN               7
#define  ICMP_DUCODE_SOURCE_HOST_ISOLATED           8           /* obsolete */
#define  ICMP_DUCODE_DST_NETWORK_PROHIBITED         9
#define  ICMP_DUCODE_DST_HOST_PROHIBITED            10
#define  ICMP_DUCODE_NETWORK_UNREACHABLE_TOS        11
#define  ICMP_DUCODE_HOST_UNREACHABLE_TOS           12
#define  ICMP_DUCODE_PROHIBITED_BYFILTER            13
#define  ICMP_DUCODE_PRECEDENCE_VIOLATION           14
#define  ICMP_DUCODE_PRECEDENCE_CUTOFF              15

#define  ICMP_DUCODE_RESERVED                       128
#define  ICMP_DUCODE_DEAD_NEXTHOP                   ICMP_DUCODE_RESERVED    + 1

/*
 * Following is a list of codes may appear under icmp_redirect_type.
 */
#define  ICMP_RDCODE_NETWORK                        0
#define  ICMP_RDCODE_HOST                           1
#define  ICMP_RDCODE_TOS_NETWORK                    2
#define  ICMP_RDCODE_TOS_HOST                       3

/*
 * Available codes for icmp_time_exceeded type of error.
 */
#define  ICMP_TECODE_TRANSIT_TIMEOUT                0
#define  ICMP_TECODE_REASSEMBLY_TIMEOUT             1

/*
 * Available codes for icmp_parameter_problem type of error.
 */
#define  ICMP_PPCODE_BAD_IP_HEADER                  0
#define  ICMP_PPCODE_MISSING_REQUIRED_OPTION        1

/*
 * The "icmp_host_trap" is used to indicate network host errors between internal modules. This ICMP
 * type should never appear outside the local module. Its sole purpose is to unifying internal
 * logging and error-handling.
 */
#define  ICMP_HTCODE_DUPLICATED_FRAG                1
#define  ICMP_HTCODE_OVERLAPPED_FRAG                2
#define  ICMP_HTCODE_CANT_REASSEMBLE                3

/*
 * ICMP messages are sent using the basic IP header. The first octet of the data portion of the
 * datagram is a ICMP type field; the value of this field determines the format of the remaining
 * data. Any field labeled "unused" is reserved for later extensions and must be zero when sent,
 * but receivers should not use these fields (except to include them in the checksum).
 */
#define  ICMP_DEF_LOOKAHEAD                         8

typedef  struct
_ICMPV4_HEADER
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_HEADER,  *PICMPV4_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIcmpv4GetType(p)                       ( p->MessageType )
#define  AnscIcmpv4GetCode(p)                       ( p->MessageCode )
#define  AnscIcmpv4GetChecksum(p)                   ( AnscReadUshort(&p->Checksum) )
#define  AnscIcmpv4GetReserved(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv4GetOrgIp(p)                      ( p->OrgIpData )

#define  AnscIcmpv4SetType(p, type)                 ( p->MessageType = type )
#define  AnscIcmpv4SetCode(p, code)                 ( p->MessageCode = code )
#define  AnscIcmpv4SetChecksum(p, c)                AnscWriteUshort(&p->Checksum, c)
#define  AnscIcmpv4SetReserved(p, r)                AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv4SetOrgIp(p, ip, s)               AnscCopyMemory(p->OrgIpData, ip, s)

#define  AnscIcmpv4Pack                             (ULONG)AnscGetPackingNumber

/*
 * There're many places in the TCP/IP stack implementation where checksum value needs to be updated
 * due to address translation, TTL decrement or option handling. We define following macro to make
 * the programming easier and more consistent.
 */
#define  AnscIcmpv4UpdateChecksumUlong(icmp_header, old_value, new_value)                   \
         {                                                                                  \
            USHORT                  icmp_checksum = AnscIcmpv4GetChecksum(icmp_header);     \
            ULONG                   ul_value1     = old_value;                              \
            ULONG                   ul_value2     = new_value;                              \
                                                                                            \
            icmp_checksum =                                                                 \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &ul_value1,                                                         \
                        sizeof(ul_value1),                                                  \
                        &ul_value2,                                                         \
                        sizeof(ul_value2),                                                  \
                        icmp_checksum                                                       \
                    );                                                                      \
                                                                                            \
            AnscIcmpv4SetChecksum(icmp_header, icmp_checksum);                              \
         }

#define  AnscIcmpv4UpdateChecksumUshort(icmp_header, old_value, new_value)                  \
         {                                                                                  \
            USHORT                  icmp_checksum = AnscIcmpv4GetChecksum(icmp_header);     \
            USHORT                  us_value1     = old_value;                              \
            USHORT                  us_value2     = new_value;                              \
                                                                                            \
            icmp_checksum =                                                                 \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &us_value1,                                                         \
                        sizeof(us_value1),                                                  \
                        &us_value2,                                                         \
                        sizeof(us_value2),                                                  \
                        icmp_checksum                                                       \
                    );                                                                      \
                                                                                            \
            AnscIcmpv4SetChecksum(icmp_header, icmp_checksum);                              \
         }

#define  AnscIcmpv4UpdateChecksumUchar(icmp_header, old_value, new_value)                   \
         {                                                                                  \
            USHORT                  icmp_checksum = AnscIcmpv4GetChecksum(icmp_header);     \
            UCHAR                   uc_value1     = old_value;                              \
            UCHAR                   uc_value2     = new_value;                              \
                                                                                            \
            icmp_checksum =                                                                 \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &uc_value1,                                                         \
                        sizeof(uc_value1),                                                  \
                        &uc_value2,                                                         \
                        sizeof(uc_value2),                                                  \
                        icmp_checksum                                                       \
                    );                                                                      \
                                                                                            \
            AnscIcmpv4SetChecksum(icmp_header, icmp_checksum);                              \
         }

#define  AnscIcmpv4CalculateChecksum(icmp_header, icmp_total_size)                          \
         {                                                                                  \
            USHORT                  icmp_checksum = 0;                                      \
                                                                                            \
            AnscIcmpv4SetChecksum(icmp_header, 0);                                          \
                                                                                            \
            icmp_checksum =                                                                 \
                AnscCalculateChecksum                                                       \
                    (                                                                       \
                        (PVOID)icmp_header,                                                 \
                        icmp_total_size                                                     \
                    );                                                                      \
                                                                                            \
            AnscIcmpv4SetChecksum(icmp_header, icmp_checksum);                              \
         }


/***********************************************************
   HEADER FORMAT OF ICMP DESTINATION UNREACHABLE MESSAGE
***********************************************************/

/*
 * If, according to the information in the gateway's routing tables, the network specified in the
 * internet destination field of a datagram is unreachable, e.g., the distance to the network is
 * infinity, the gateway may send a destination unreachable message to the internet source host
 * of the datagram.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             unused                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Data Datagram      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_DST_UNREACHABLE_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;                               /* unused, must be zero */
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_DST_UNREACHABLE_MESSAGE,  *PICMPV4_DST_UNREACHABLE_MESSAGE;

#define  AnscIcmpv4DuGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4DuGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4DuGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4DuGetReserved(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv4DuGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv4DuSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4DuSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4DuSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4DuSetReserved(p, r)              AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv4DuSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)

/*
 * When a router is unable to forward a datagram because it exceeds the MTU of the next-hop network
 * and its Don't Fragment bit is set, the router is required to return an ICMP Destination Unreach-
 * able message to the source of the datagram, with the Code indicating "fragmentation needed and
 * DF set". To support the Path MTU Discovery technique specified in this memo, the router MUST
 * include the MTU of that next-hop network in the low-order 16 bits of the ICMP header field that
 * is labelled "unused" in the ICMP specification. The high-order 16 bits remain unused, and MUST
 * be set to zero. Thus, the message has the following format:
 *          
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |   Type = 3    |   Code = 4    |           Checksum            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           unused = 0          |         Next-Hop MTU          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Datagram Data      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The value carried in the Next-Hop MTU field is:
 *
 *          The size in octets of the largest datagram that could be forwarded, along the
 *          path of the original datagram, without being fragmented at this router. The
 *          size includes the IP header and IP data, and does not include any lower-level
 *          headers.
 */
typedef  struct
_ICMPV4_DU_DFSET_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Unused;                                 /* unused, must be zero */
    USHORT                          NextHopMtu;
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_DU_DFSET_MESSAGE,  *PICMPV4_DU_DFSET_MESSAGE;

#define  AnscIcmpv4DuDfsetGetUnused(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Unused    )) )
#define  AnscIcmpv4DuDfsetGetNextHopMtu(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->NextHopMtu)) )

#define  AnscIcmpv4DuDfsetSetUnused(p, u)           AnscWriteUshort(&p->Unused,     AnscUshortFromHToN(u))
#define  AnscIcmpv4DuDfsetSetNextHopMtu(p, m)       AnscWriteUshort(&p->NextHopMtu, AnscUshortFromHToN(m))


/***********************************************************
         HEADER FORMAT OF ICMP TIME EXCEEDED MESSAGE
***********************************************************/

/*
 * If the gateway processing a datagram finds the time to live field is zero it must discard the
 * datagram. The gateway may also notify the source host via the time exceeded message. If a host
 * reassembling a fragmented datagram cannot complete the reassembly due to missing fragments
 * within its time limit it discards the datagram, and it may send a time exceeded message.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             unused                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Data Datagram      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_TIME_EXCEEDED_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;                               /* unused, must be zero */
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_TIME_EXCEEDED_MESSAGE,  *PICMPV4_TIME_EXCEEDED_MESSAGE;

#define  AnscIcmpv4TeGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4TeGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4TeGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4TeGetReserved(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv4TeGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv4TeSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4TeSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4TeSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4TeSetReserved(p, r)              AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv4TeSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)


/***********************************************************
         HEADER FORMAT OF ICMP PARAM PROBLEM MESSAGE
***********************************************************/

/*
 * If the gateway or host processing a datagram finds a problem with the header parameters such
 * that it cannot complete processing the datagram it must discard the datagram. One potential
 * source of such a problem is with incorrect arguments in an option. The gateway or host may also
 * notify the soruce host via the parameter problem message. This message is only sent if the error
 * caused the datagram to be discarded.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    Pointer    |                   unused                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Data Datagram      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_PARAM_PROBLEM_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    UCHAR                           Pointer;
    UCHAR                           Reserved1;                              /* unused, must be zero */
    USHORT                          Reserved2;                              /* unused, must be zero */
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_PARAM_PROBLEM_MESSAGE,  *PICMPV4_PARAM_PROBLEM_MESSAGE;

#define  AnscIcmpv4PpGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4PpGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4PpGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4PpGetPointer(p)                  ( p->Pointer   )
#define  AnscIcmpv4PpGetReserved1(p)                ( p->Reserved1 )
#define  AnscIcmpv4PpGetReserved2(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved2) )
#define  AnscIcmpv4PpGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv4PpSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4PpSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4PpSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4PpSetPointer(p, o)               ( p->Pointer   = o )
#define  AnscIcmpv4PpSetReserved1(p, r)             ( p->Reserved1 = r )
#define  AnscIcmpv4PpSetReserved2(p, r)             AnscWriteUshort(&p->Reserved2, AnscUshortFromHToN(r))
#define  AnscIcmpv4PpSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)


/***********************************************************
         HEADER FORMAT OF ICMP SOURCE QUENCH MESSAGE
***********************************************************/

/*
 * A gateway may discard internet datagram if it does not have the buffer space needed to queue the
 * datagrams for output to the next network on the route to the destination network. If a gateway
 * discards a datagram, it may send a source quench message to the internet source host of the
 * datagram. A destination host may also send a source quench message if datagrams arrive too fast
 * to be processed.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             unused                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Data Datagram      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_SRC_QUENCH_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;                               /* unused, must be zero */
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_SRC_QUENCH_MESSAGE,  *PICMPV4_SRC_QUENCH_MESSAGE;

#define  AnscIcmpv4SqGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4SqGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4SqGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4SqGetReserved(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv4SqGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv4SqSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4SqSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4SqSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4SqSetReserved(p, r)              AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv4SqSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)


/***********************************************************
            HEADER FORMAT OF ICMP REDIRECT MESSAGE
***********************************************************/

/*
 * The gateway sends a redirect message to a host in the following situation. A gateway, G1,
 * receives an internet datagram from a host on a network to which the gateway is attached. The
 * gateway, G1, checks its routing table and obtains the address of the next gateway, G2, on the
 * route to the datagram's internet destination network, X. If G2 and the host identified by the
 * internet source address of the datagram are on the same network, a redirect message is sent to
 * the host. The redirect message advises the host to send its traffic for network X directly to
 * gateway G2 as this is a shorter path to the destination. The gateway forwards the original
 * datagram's data to its internet destination.
 *
 * For datagrams with the IP source route options and the gateway address in the destination
 * address field, a redirect message is not sent even if there is a better route to the ultimate
 * destination than the next address in the source route.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                 Gateway Internet Address                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Internet Header + 64 bits of Original Data Datagram      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_REDIRECT_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ANSC_IPV4_ADDRESS               GatewayAddr;
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_REDIRECT_MESSAGE,  *PICMPV4_REDIRECT_MESSAGE;

#define  AnscIcmpv4RdGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4RdGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4RdGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4RdGetGwAddrDot(p)                ( p->GatewayAddr.Dot                   )
#define  AnscIcmpv4RdGetGwAddrValue(p)              ( AnscReadUlong(&p->GatewayAddr.Value) )
#define  AnscIcmpv4RdGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv4RdSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4RdSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4RdSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4RdSetGwAddrDot(p, addr)          AnscWriteUlong(&p->GatewayAddr.Dot[0], *(PULONG)addr)
#define  AnscIcmpv4RdSetGwAddrValue(p, addr)        AnscWriteUlong(&p->GatewayAddr.Value,  addr         )
#define  AnscIcmpv4RdSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)


/***********************************************************
       HEADER FORMAT OF ICMP ECHO OR ECHO REPLY MESSAGE
***********************************************************/

/*
 * The data received in the echo message must be returned in the echo reply message. The identifier
 * and sequence number may be used by the echo sender to aid in matching the replies with the echo
 * requests. For example, the identifier might be used like a port in TCP or UDP to identify a
 * session, and the sequence number might be incremented on each echo request sent. The echoer
 * returns these same values in the echo reply.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Identifier          |        Sequence Number        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Data ...
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_ECHO_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Identifier;
    USHORT                          SeqNumber;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_ECHO_MESSAGE,  *PICMPV4_ECHO_MESSAGE;

#define  AnscIcmpv4EchoGetType(p)                   AnscIcmpv4GetType(p)
#define  AnscIcmpv4EchoGetCode(p)                   AnscIcmpv4GetCode(p)
#define  AnscIcmpv4EchoGetChecksum(p)               AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4EchoGetId(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier)) )
#define  AnscIcmpv4EchoGetSeqNumber(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber )) )
#define  AnscIcmpv4EchoGetData(p)                   ( p->Data )

#define  AnscIcmpv4EchoSetType(p, t)                AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4EchoSetCode(p, c)                AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4EchoSetChecksum(p, c)            AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4EchoSetId(p, id)                 AnscWriteUshort(&p->Identifier, AnscUshortFromHToN(id ))
#define  AnscIcmpv4EchoSetSeqNumber(p, num)         AnscWriteUshort(&p->SeqNumber,  AnscUshortFromHToN(num))
#define  AnscIcmpv4EchoSetData(p, d, s)             AnscCopyMemory(p->Data, d, s)


/***********************************************************
       HEADER FORMAT OF ICMP TIMESTAMP OR REPLY MESSAGE
***********************************************************/

/*
 * The data received (a timestamp) in the message is returned in the reply together with an
 * additional timestamp. The timestamp is 32 bits of milliseconds since midnight UT. One use of
 * these timestamps is described by Mills [5].
 *
 * The Originate Timestamp is the time the sender last touched the message before sending it, the
 * Receive Timestamp is the time the echoer first touched it on receipt, and the Transmit Timestamp
 * is the time the echoer last touched the message on sending it.
 *
 * If the time is not available in miliseconds or cannot be provided with respect to midnight UT
 * then any time can be inserted in a timestamp provided the high order bit of the timestamp is
 * also set to indicate this non-standard value.
 *
 * The identifier and sequence number may be used by the echo sender to aid in matching the replies
 * with the requests. For example, the identifier might be used like a port in TCP or UDP to
 * identify a session, and the sequence number might be incremented on each request sent. The
 * destination returns these same values in the reply.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Identifier          |        Sequence Number        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Originate Timestamp                                       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Receive Timestamp                                         |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Transmit Timestamp                                        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_TIMESTAMP_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Identifier;
    USHORT                          SeqNumber;
    ULONG                           OrgtTimeStamp;
    ULONG                           RecvTimeStamp;
    ULONG                           SendTimeStamp;
}_struct_pack_
ICMPV4_TIMESTAMP_MESSAGE,  *PICMPV4_TIMESTAMP_MESSAGE;

#define  AnscIcmpv4TsGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4TsGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4TsGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4TsGetId(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier    )) )
#define  AnscIcmpv4TsGetSeqNumber(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber     )) )
#define  AnscIcmpv4TsGetOrgtTime(p)                 ( AnscUlongFromNToH (AnscReadUlong (&p->OrgtTimeStamp )) )
#define  AnscIcmpv4TsGetRecvTime(p)                 ( AnscUlongFromNToH (AnscReadUlong (&p->RecvTimeStamp )) )
#define  AnscIcmpv4TsGetSendTime(p)                 ( AnscUlongFromNToH (AnscReadUlong (&p->SendTimeStamp )) )

#define  AnscIcmpv4TsSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4TsSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4TsSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4TsSetId(p, id)                   AnscWriteUshort(&p->Identifier,    AnscUshortFromHToN(id ))
#define  AnscIcmpv4TsSetSeqNumber(p, num)           AnscWriteUshort(&p->SeqNumber,     AnscUshortFromHToN(num))
#define  AnscIcmpv4TsSetOrgtTime(p, t)              AnscWriteUlong (&p->OrgtTimeStamp, AnscUlongFromHToN (t  ))
#define  AnscIcmpv4TsSetRecvTime(p, t)              AnscWriteUlong (&p->RecvTimeStamp, AnscUlongFromHToN (t  ))
#define  AnscIcmpv4TsSetSendTime(p, t)              AnscWriteUlong (&p->SendTimeStamp, AnscUlongFromHToN (t  ))


/***********************************************************
     HEADER FORMAT OF ICMP INFORMATION OR REPLY MESSAGE
***********************************************************/

/*
 * This message may be sent with the source network in the IP header source and destination address
 * fields zero (which means "this" network). The replying IP module should send the reply with the
 * addresses fully specified. This message is a way for a host to find out the number of the
 * network it is on.
 *
 * The identifier and sequence number may be used by the echo sender to aid in matching the replies
 * with the requests. For example, the identifier might be used like a port in TCP or UDP to
 * identify a session, and the sequence number might be incremented on each request sent. The
 * destination returns these same values in the reply.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Identifier          |        Sequence Number        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * NOTE: this ICMP message is now obsolete.
 */
typedef  struct
_ICMPV4_INFORMATION_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Identifier;
    USHORT                          SeqNumber;
}_struct_pack_
ICMPV4_INFORMATION_MESSAGE,  *PICMPV4_INFORMATION_MESSAGE;

#define  AnscIcmpv4InfoGetType(p)                   AnscIcmpv4GetType(p)
#define  AnscIcmpv4InfoGetCode(p)                   AnscIcmpv4GetCode(p)
#define  AnscIcmpv4InfoGetChecksum(p)               AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4InfoGetId(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier)) )
#define  AnscIcmpv4InfoGetSeqNumber(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber )) )

#define  AnscIcmpv4InfoSetType(p, t)                AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4InfoSetCode(p, c)                AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4InfoSetChecksum(p, c)            AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4InfoSetId(p, id)                 AnscWriteUshort(&p->Identifier, AnscUshortFromHToN(id ))
#define  AnscIcmpv4InfoSetSeqNumber(p, num)         AnscWriteUshort(&p->SeqNumber,  AnscUshortFromHToN(num))


/***********************************************************
    FORMAT OF ICMP ADDRESS MASK REQUEST AND REPLY MESSAGE
***********************************************************/

/*
 * The ICMP address mask request is intended for a diskless system to obtain its subnet mask at
 * bootstrap time. The requesting system broadcasts its ICMP request. (This is similar to a disk-
 * less system using RARP to obtain its IP address at bootstrap time.) An alternative method for
 * a diskless system to obtain its subnet mask is the BOOTP/DHCP protocol. The identifier and
 * sequence number fields in the ICMP message can be set to anything the sender chooses, and these
 * values are returned in the reply. This allows the sender to match replies with requests.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Identifier          |        Sequence Number        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                      32-bit Subnet Mask                       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_ADDR_MASK_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Identifier;
    USHORT                          SeqNumber;
    ANSC_IPV4_ADDRESS               AddrMask;
}_struct_pack_
ICMPV4_ADDR_MASK_MESSAGE,  *PICMPV4_ADDR_MASK_MESSAGE;

#define  AnscIcmpv4AmGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4AmGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4AmGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4AmGetId(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier    )) )
#define  AnscIcmpv4AmGetSeqNumber(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber     )) )
#define  AnscIcmpv4AmGetMaskDot(p)                  ( p->AddrMask.Dot                   )
#define  AnscIcmpv4AmGetMaskValue(p)                ( AnscReadUlong(&p->AddrMask.Value) )

#define  AnscIcmpv4AmSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4AmSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4AmSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4AmSetId(p, id)                   AnscWriteUshort(&p->Identifier,    AnscUshortFromHToN(id ))
#define  AnscIcmpv4AmSetSeqNumber(p, num)           AnscWriteUshort(&p->SeqNumber,     AnscUshortFromHToN(num))
#define  AnscIcmpv4AmSetMaskDot(p, mask)            AnscWriteUlong(&p->AddrMask.Dot[0], *(PULONG)mask)
#define  AnscIcmpv4AmSetMaskValue(p, mask)          AnscWriteUlong(&p->AddrMask.Value,  mask         )


/***********************************************************
          FORMAT OF ICMP ROUTER DISCOVERY MESSAGES
***********************************************************/

/*
 * The ICMP router discovery messages are called "Router Advertisements" and "Router Solicitations".
 * Each router periodically multicasts a Router Advertisement from each of its multicast interfaces,
 * announcing the IP address(es) of that interface. Hosts discover the addresses of their
 * neighboring routers simply by listening for advertisements. When a host attached to a multicast
 * link starts up, it may multicast a Router Solicitation to ask for immediate advertisements,
 * rather than waiting for the next periodic ones to arrive; if (and only if) no advertisements are
 * forthcoming, the host may retransmit the solicitation a small number of times, but then must
 * desist from sending any more solicitations. Any routers that subsequently start up, or that were
 * not discovered because of packet loss or temporary link partitioning, are eventually discovered
 * by reception of their periodic (unsolicited) advertisements. (Links that suffer high packet loss
 * rates or frequent partitioning are accommodated by increasing the rate of advertisements, rather
 * than increasing the number of solicitations that hosts are permitted to send.)
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |           Checksum            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |   Num Addrs   |Addr Entry Size|           Lifetime            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Router Address[1]                       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                      Preference Level[1]                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Router Address[2]                       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                      Preference Level[2]                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                               .                               |
 *          |                               .                               |
 *          |                               .                               |
 *
 */
typedef  struct
_ICMPV4_ROUTER_ADVER_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    UCHAR                           NumberOfAddrs;
    UCHAR                           AddrEntrySize;
    USHORT                          LifeTime;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV4_ROUTER_ADVER_MESSAGE,  *PICMPV4_ROUTER_ADVER_MESSAGE;

#define  AnscIcmpv4RaGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4RaGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4RaGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4RaGetNumberOfAddrs(p)            ( p->NumberOfAddrs )
#define  AnscIcmpv4RaGetAddrEntrySize(p)            ( p->AddrEntrySize )
#define  AnscIcmpv4RaGetLifeTime(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->LifeTime)) )
#define  AnscIcmpv4RaGetAddrDot(p, i)               ( &p->Info[i * 8] )
#define  AnscIcmpv4RaGetAddrValue(p, i)             ( AnscReadUlong(&p->Info[i * 8]) )
#define  AnscIcmpv4RaGetPrefLevel(p, i)             ( AnscUlongFromNToH(AnscReadUlong(&p->Info[i * 8 + 4])) )

#define  AnscIcmpv4RaSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4RaSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4RaSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4RaSetNumberOfAddrs(p, n)         ( p->NumberOfAddrs = n )
#define  AnscIcmpv4RaSetAddrEntrySize(p, s)         ( p->AddrEntrySize = s )
#define  AnscIcmpv4RaSetLifeTime(p, t)              AnscWriteUshort(&p->LifeTime,        AnscUshortFromHToN(t))
#define  AnscIcmpv4RaSetAddrDot(p, i, addr)         AnscWriteUlong (&p->Info[i * 8    ], *(PULONG)addr        )
#define  AnscIcmpv4RaSetAddrValue(p, i, addr)       AnscWriteUlong (&p->Info[i * 8    ], addr                 )
#define  AnscIcmpv4RaSetPrefLevel(p, i, l)          AnscWriteUlong (&p->Info[i * 8 + 4], AnscUlongFromHToN(l) )

/*
 * A host is permitted (but not required) to transmit up to MAX_SOLICITATIONS Router Solicitation
 * messages from any of its multicast interfaces after any of the following events:
 *
 *      - The interface is initialized at system startup time.
 *      - The interface is reinitialized after a temporary interface failure or after
 *        being temporarily disabled by system management.
 *      - The system changes from being a router to being a host, by having its IP
 *        forwarding capability turned off by system management.
 *      - The PerformRouterDiscovery flag for the interface is changed from FALSE to
 *        TRUE by system management.
 *
 * The IP destination address of the solicitations is the configured SolicitationAddress for the
 * interface. The IP source address may contain zero if the host has not yet determined an address
 * for the interface; otherwise it contains one of the interface's addresses.
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |     Type      |     Code      |          Checksum             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             unused                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
typedef  struct
_ICMPV4_ROUTER_SOLIC_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;                               /* unused, must be zero */
}_struct_pack_
ICMPV4_ROUTER_SOLIC_MESSAGE,  *PICMPV4_ROUTER_SOLIC_MESSAGE;

#define  AnscIcmpv4RsGetType(p)                     AnscIcmpv4GetType(p)
#define  AnscIcmpv4RsGetCode(p)                     AnscIcmpv4GetCode(p)
#define  AnscIcmpv4RsGetChecksum(p)                 AnscIcmpv4GetChecksum(p)
#define  AnscIcmpv4RsGetReserved(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )

#define  AnscIcmpv4RsSetType(p, t)                  AnscIcmpv4SetType(p, t)
#define  AnscIcmpv4RsSetCode(p, c)                  AnscIcmpv4SetCode(p, c)
#define  AnscIcmpv4RsSetChecksum(p, c)              AnscIcmpv4SetChecksum(p, c)
#define  AnscIcmpv4RsSetReserved(p, r)              AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
