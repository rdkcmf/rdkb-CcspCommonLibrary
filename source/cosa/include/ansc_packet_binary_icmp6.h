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

    module:	ansc_packet_binary_icmp6.h

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
        (ICMPv6).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Tom Chnag

    ---------------------------------------------------------------

    revision:

        09/14/11    initial revision.


**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_ICMP6_
#define  _ANSC_PACKET_BINARY_ICMP6_


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

/*
 * The ICMPv6 messages have the following general format:
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |     Type      |     Code      |          Checksum             |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                                                               |
 *     +                         Message Body                          +
 *     |                                                               |
 *
 */

/***********************************************************
                DATA STRUCTURES USED BY ICMP
***********************************************************/

/*
 * Define some const values used in the ICMP header definition
 */

#define  ICMP6_TYPE_DESTINATION_UNREACHABLE          1
#define  ICMP6_TYPE_PACKET_TOO_BIG                   2
#define  ICMP6_TYPE_TIME_EXCEEDED                    3
#define  ICMP6_TYPE_PARAMETER_PROBLEM                4

#define  ICMP6_TYPE_ECHO_REQUEST                     128
#define  ICMP6_TYPE_ECHO_REPLY                       129

/*
 * Following is a list of codes may appear under icmp_destination_unreachable_type.
 */
#define  ICMP6_DUCODE_NO_ROUTE_TO_DESTINATION        0
#define  ICMP6_DUCODE_PROHIBITED                     1
#define  ICMP6_DUCODE_ADDRESS_UNREACHABLE            3
#define  ICMP6_DUCODE_PORT_UNREACHABLE               4



typedef  struct
_ICMPV6_HEADER
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;
    UCHAR                           MessageBody[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV6_HEADER,  *PICMPV6_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIcmpv6GetType(p)                       ( p->MessageType )
#define  AnscIcmpv6GetCode(p)                       ( p->MessageCode )
#define  AnscIcmpv6GetChecksum(p)                   ( AnscReadUshort(&p->Checksum) )
#define  AnscIcmpv6GetReserved(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv6GetMessageBody(p)                ( p->MessageBody )

#define  AnscIcmpv6SetType(p, type)                 ( p->MessageType = type )
#define  AnscIcmpv6SetCode(p, code)                 ( p->MessageCode = code )
#define  AnscIcmpv6SetChecksum(p, c)                AnscWriteUshort(&p->Checksum, c)
#define  AnscIcmpv6SetReserved(p, r)                AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv6SetMessageBody(p, ip, s)         AnscCopyMemory(p->MessageBody, ip, s)

#define  AnscIcmpv6Pack                             (ULONG)AnscGetPackingNumber


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
_ICMPV6_ECHO_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    USHORT                          Identifier;
    USHORT                          SeqNumber;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV6_ECHO_MESSAGE,  *PICMPV6_ECHO_MESSAGE;

#define  AnscIcmpv6EchoGetType(p)                   AnscIcmpv6GetType(p)
#define  AnscIcmpv6EchoGetCode(p)                   AnscIcmpv6GetCode(p)
#define  AnscIcmpv6EchoGetChecksum(p)               AnscIcmpv6GetChecksum(p)
#define  AnscIcmpv6EchoGetId(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier)) )
#define  AnscIcmpv6EchoGetSeqNumber(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber )) )
#define  AnscIcmpv6EchoGetData(p)                   ( p->Data )

#define  AnscIcmpv6EchoSetType(p, t)                AnscIcmpv6SetType(p, t)
#define  AnscIcmpv6EchoSetCode(p, c)                AnscIcmpv6SetCode(p, c)
#define  AnscIcmpv6EchoSetChecksum(p, c)            AnscIcmpv6SetChecksum(p, c)
#define  AnscIcmpv6EchoSetId(p, id)                 AnscWriteUshort(&p->Identifier, AnscUshortFromHToN(id ))
#define  AnscIcmpv6EchoSetSeqNumber(p, num)         AnscWriteUshort(&p->SeqNumber,  AnscUshortFromHToN(num))
#define  AnscIcmpv6EchoSetData(p, d, s)             AnscCopyMemory(p->Data, d, s)


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
 *          |                    As much of invoking packet                 |
 *          +                as will fit without the ICMPv6 packet          +
 *          |                exceeding the minimum IPv6 MTU [IPv6]          |
 *
 */
typedef  struct
_ICMPV6_TIME_EXCEEDED_MESSAGE
{
    UCHAR                           MessageType;
    UCHAR                           MessageCode;
    USHORT                          Checksum;
    ULONG                           Reserved;                               /* unused, must be zero */
    UCHAR                           OrgIpData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ICMPV6_TIME_EXCEEDED_MESSAGE,  *PICMPV6_TIME_EXCEEDED_MESSAGE;

#define  AnscIcmpv6TeGetType(p)                     AnscIcmpv6GetType(p)
#define  AnscIcmpv6TeGetCode(p)                     AnscIcmpv6GetCode(p)
#define  AnscIcmpv6TeGetChecksum(p)                 AnscIcmpv6GetChecksum(p)
#define  AnscIcmpv6TeGetReserved(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  AnscIcmpv6TeGetOrgIp(p)                    ( p->OrgIpData )

#define  AnscIcmpv6TeSetType(p, t)                  AnscIcmpv6SetType(p, t)
#define  AnscIcmpv6TeSetCode(p, c)                  AnscIcmpv6SetCode(p, c)
#define  AnscIcmpv6TeSetChecksum(p, c)              AnscIcmpv6SetChecksum(p, c)
#define  AnscIcmpv6TeSetReserved(p, r)              AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  AnscIcmpv6TeSetOrgIp(p, ip, s)             AnscCopyMemory(p->OrgIpData, ip, s)


#endif
