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

    module:	ansc_packet_binary_tcp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of Transmission Control Protocol (TCP).

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


#ifndef  _ANSC_PACKET_BINARY_TCP_
#define  _ANSC_PACKET_BINARY_TCP_


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
                 DATA STRUCTURES USED BY TCP
***********************************************************/

/*
 * TCP segments are sent as internet datagrams. The Internet Protocol header carries several
 * information fields, including the source and destination host addresses. A TCP header follows
 * the internet header, supplying information specific to the TCP protocol. This division allows
 * for the existence of host level protocols other than TCP.
 */
#define  TCP_HEADERLENGTH_MASK                      0xF0
#define  TCP_URG_FLAG                               0x20
#define  TCP_ACK_FLAG                               0x10
#define  TCP_PSH_FLAG                               0x08
#define  TCP_RST_FLAG                               0x04
#define  TCP_SYN_FLAG                               0x02
#define  TCP_FIN_FLAG                               0x01
#define  TCP_CHRISTMAS_FLAG                         0x3F

/*
 * TCP is a connection-oriented, end-to-end reliable protocol designed to fit into a layered
 * hierarchy of protocols which support multi-network applications. The TCP provides for reliable
 * inter-process communication between pairs of processes in host computers attached to distinct
 * but interconnected computer communication networks. Very few assumptions are made as to the
 * reliability of the communication protocols below the TCP layer. TCP assumes it can obtain a
 * simple, potentially unreliable datagram service from the lower level protocols. In principle,
 * the TCP should be able to operate above a wide spectrum of communication systems ranging from
 * hard-wired connections to packet-switched or circuit-switched networks.
 *
 *           0                   1                   2                   3   
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |          Source Port          |       Destination Port        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                        Sequence Number                        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                    Acknowledgment Number                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |  Data |           |U|A|P|R|S|F|                               |
 *          | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
 *          |       |           |G|K|H|T|N|N|                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Checksum            |         Urgent Pointer        |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                    Options                    |    Padding    |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             data                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define  MAX_TCP_PAYLOAD_SIZE                       1460

typedef  struct
_TCP_HEADER
{
    USHORT                          SrcPortNumber;
    USHORT                          DstPortNumber;
    ULONG                           SeqNumber;
    ULONG                           AckNumber;
    UCHAR                           HeaderLength;
    UCHAR                           Flags;
    USHORT                          WindowSize;
    USHORT                          Checksum;
    USHORT                          UrgentPointer;
    UCHAR                           Options[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TCP_HEADER,  *PTCP_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscTcpGetSrcPort(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->SrcPortNumber)) )
#define  AnscTcpGetDstPort(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->DstPortNumber)) )
#define  AnscTcpGetSeqNumber(p)                     ( AnscUlongFromNToH (AnscReadUlong (&p->SeqNumber    )) )
#define  AnscTcpGetAckNumber(p)                     ( AnscUlongFromNToH (AnscReadUlong (&p->AckNumber    )) )
#define  AnscTcpGetHeaderSize(p)                    ( ((p->HeaderLength & 0xF0) >> 4) * 4                   )
#define  AnscTcpGetFlags(p)                         ( p->Flags                                              )
#define  AnscTcpGetWindowSize(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->WindowSize   )) )
#define  AnscTcpGetChecksum(p)                      ( AnscReadUshort(&p->Checksum)                          )
#define  AnscTcpGetUrgentPointer(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->UrgentPointer)) )
#define  AnscTcpGetPayload(p)                       ( (PVOID)((ULONG)p + AnscTcpGetHeaderSize(p))           )

#define  AnscTcpSetSrcPort(p, n)                    AnscWriteUshort(&p->SrcPortNumber, AnscUshortFromHToN(n))
#define  AnscTcpSetDstPort(p, n)                    AnscWriteUshort(&p->DstPortNumber, AnscUshortFromHToN(n))
#define  AnscTcpSetSeqNumber(p, n)                  AnscWriteUlong (&p->SeqNumber,     AnscUlongFromHToN (n))
#define  AnscTcpSetAckNumber(p, n)                  AnscWriteUlong (&p->AckNumber,     AnscUlongFromHToN (n))
#define  AnscTcpSetHeaderSize(p, s)                 ( p->HeaderLength = ((((UCHAR)s) & 0x0F) << 4)          )
#define  AnscTcpSetFlags(p, f)                      ( p->Flags        = f                                   )
#define  AnscTcpSetWindowSize(p, w)                 AnscWriteUshort(&p->WindowSize,    AnscUshortFromHToN(w))
#define  AnscTcpSetChecksum(p, c)                   AnscWriteUshort(&p->Checksum,      c                    )
#define  AnscTcpSetUrgentPointer(p, u)              AnscWriteUshort(&p->UrgentPointer, AnscUshortFromHToN(u))

#define  AnscTcpIsUrg(p)                            ( (p->Flags  & TCP_URG_FLAG      ) != 0 )
#define  AnscTcpIsAck(p)                            ( (p->Flags  & TCP_ACK_FLAG      ) != 0 )
#define  AnscTcpIsPsh(p)                            ( (p->Flags  & TCP_PSH_FLAG      ) != 0 )
#define  AnscTcpIsRst(p)                            ( (p->Flags  & TCP_RST_FLAG      ) != 0 )
#define  AnscTcpIsSyn(p)                            ( (p->Flags  & TCP_SYN_FLAG      ) != 0 )
#define  AnscTcpIsFin(p)                            ( (p->Flags  & TCP_FIN_FLAG      ) != 0 )
#define  AnscTcpIsSynOnly(p)                        ( (p->Flags == TCP_SYN_FLAG      ) )
#define  AnscTcpIsChristmas(p)                      ( (p->Flags == TCP_CHRISTMAS_FLAG) )

#define  AnscTcpDoUrg(p)                            ( p->Flags |= TCP_URG_FLAG       )
#define  AnscTcpDoAck(p)                            ( p->Flags |= TCP_ACK_FLAG       )
#define  AnscTcpDoPsh(p)                            ( p->Flags |= TCP_PSH_FLAG       )
#define  AnscTcpDoRst(p)                            ( p->Flags |= TCP_RST_FLAG       )
#define  AnscTcpDoSyn(p)                            ( p->Flags |= TCP_SYN_FLAG       )
#define  AnscTcpDoFin(p)                            ( p->Flags |= TCP_FIN_FLAG       )
#define  AnscTcpDoSynOnly(p)                        ( p->Flags  = TCP_SYN_FLAG       )
#define  AnscTcpDoChristmas(p)                      ( p->Flags  = TCP_CHRISTMAS_FLAG )

#define  AnscTcpPack                                (ULONG)AnscGetPackingNumber

#define  AnscTcpReplaceAckNumber(tcp_header, new_ack_number)                                \
         {                                                                                  \
            USHORT                  old_tcp_checksum = AnscTcpGetChecksum(tcp_header);      \
            USHORT                  new_tcp_checksum = 0;                                   \
            ULONG                   old_tcp_ack_num  = AnscTcpGetAckNumber(tcp_header);     \
            ULONG                   new_tcp_ack_num  = new_ack_number;                      \
                                                                                            \
            old_tcp_ack_num  = AnscUlongFromHToN(old_tcp_ack_num);                          \
            new_tcp_ack_num  = AnscUlongFromHToN(new_tcp_ack_num);                          \
            new_tcp_checksum =                                                              \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &old_tcp_ack_num,                                                   \
                        sizeof(ULONG),                                                      \
                        &new_tcp_ack_num,                                                   \
                        sizeof(ULONG),                                                      \
                        old_tcp_checksum                                                    \
                    );                                                                      \
                                                                                            \
            AnscTcpSetAckNumber(tcp_header, new_ack_number  );                              \
            AnscTcpSetChecksum (tcp_header, new_tcp_checksum);                              \
         }

#define  AnscTcpReplaceSeqNumber(tcp_header, new_seq_number)                                \
         {                                                                                  \
            USHORT                  old_tcp_checksum = AnscTcpGetChecksum(tcp_header);      \
            USHORT                  new_tcp_checksum = 0;                                   \
            ULONG                   old_tcp_seq_num  = AnscTcpGetSeqNumber(tcp_header);     \
            ULONG                   new_tcp_seq_num  = new_seq_number;                      \
                                                                                            \
            old_tcp_seq_num  = AnscUlongFromHToN(old_tcp_seq_num);                          \
            new_tcp_seq_num  = AnscUlongFromHToN(new_tcp_seq_num);                          \
            new_tcp_checksum =                                                              \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &old_tcp_seq_num,                                                   \
                        sizeof(ULONG),                                                      \
                        &new_tcp_seq_num,                                                   \
                        sizeof(ULONG),                                                      \
                        old_tcp_checksum                                                    \
                    );                                                                      \
                                                                                            \
            AnscTcpSetSeqNumber(tcp_header, new_seq_number  );                              \
            AnscTcpSetChecksum (tcp_header, new_tcp_checksum);                              \
         }

#define  AnscTcpCalculateChecksum(ip4_header, tcp_header, tcp_body)                         \
         {                                                                                  \
            USHORT                  tcp_checksum   = 0;                                     \
            USHORT                  tcp_total_size = AnscIpv4GetBodySize(ip4_header);       \
            USHORT                  tcp_head_size  = AnscTcpGetHeaderSize(tcp_header);      \
            USHORT                  tcp_body_size  = tcp_total_size - tcp_head_size;        \
            ULONG                   dst_ip4_value  = AnscIpv4GetDstAddressValue(ip4_header);\
            ULONG                   src_ip4_value  = AnscIpv4GetSrcAddressValue(ip4_header);\
            ULONG                   tmp_checksum   = 0;                                     \
            IPV4_PSEUDO_HEADER      ip4_pseudo_header;                                      \
                                                                                            \
            AnscIpv4PseudoSetDstAddressValue((&ip4_pseudo_header), dst_ip4_value );         \
            AnscIpv4PseudoSetSrcAddressValue((&ip4_pseudo_header), src_ip4_value );         \
            AnscIpv4PseudoSetReserved       ((&ip4_pseudo_header), 0             );         \
            AnscIpv4PseudoSetType           ((&ip4_pseudo_header), TCP_TRANSPORT );         \
            AnscIpv4PseudoSetLength         ((&ip4_pseudo_header), tcp_total_size);         \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, 0);                                              \
                                                                                            \
            tmp_checksum += AnscTcpPack(&ip4_pseudo_header, sizeof(IPV4_PSEUDO_HEADER));    \
            tmp_checksum += AnscTcpPack(tcp_header,         tcp_head_size             );    \
            tmp_checksum += AnscTcpPack(tcp_body,           tcp_body_size             );    \
                                                                                            \
            while ( tmp_checksum >> 16 )                                                    \
            {                                                                               \
                tmp_checksum = (tmp_checksum & 0xFFFF) + (tmp_checksum >> 16);              \
            }                                                                               \
                                                                                            \
            tcp_checksum = ~((USHORT)tmp_checksum);                                         \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum);                                   \
         }

#define  AnscTcpCalculateChecksum2(ip4_header, tcp_header, tcp_bpn)                         \
         {                                                                                  \
            USHORT                  tcp_checksum   = 0;                                     \
            USHORT                  tcp_total_size = AnscIpv4GetBodySize(ip4_header);       \
            USHORT                  tcp_head_size  = AnscTcpGetHeaderSize(tcp_header);      \
            USHORT                  tcp_body_size  = tcp_total_size - tcp_head_size;        \
            ULONG                   dst_ip4_value  = AnscIpv4GetDstAddressValue(ip4_header);\
            ULONG                   src_ip4_value  = AnscIpv4GetSrcAddressValue(ip4_header);\
            ULONG                   tmp_checksum   = 0;                                     \
            IPV4_PSEUDO_HEADER      ip4_pseudo_header;                                      \
                                                                                            \
            AnscIpv4PseudoSetDstAddressValue((&ip4_pseudo_header), dst_ip4_value );         \
            AnscIpv4PseudoSetSrcAddressValue((&ip4_pseudo_header), src_ip4_value );         \
            AnscIpv4PseudoSetReserved       ((&ip4_pseudo_header), 0             );         \
            AnscIpv4PseudoSetType           ((&ip4_pseudo_header), TCP_TRANSPORT );         \
            AnscIpv4PseudoSetLength         ((&ip4_pseudo_header), tcp_total_size);         \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, 0);                                              \
                                                                                            \
            tmp_checksum += AnscTcpPack(&ip4_pseudo_header, sizeof(IPV4_PSEUDO_HEADER));    \
            tmp_checksum += AnscTcpPack(tcp_header,         tcp_head_size             );    \
            tmp_checksum += tcp_bpn;                                                        \
                                                                                            \
            while ( tmp_checksum >> 16 )                                                    \
            {                                                                               \
                tmp_checksum = (tmp_checksum & 0xFFFF) + (tmp_checksum >> 16);              \
            }                                                                               \
                                                                                            \
            tcp_checksum = ~((USHORT)tmp_checksum);                                         \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum);                                   \
         }

#define  AnscTcpVerifyChecksum(ip4_header, tcp_header, tcp_body, v_result)                  \
         {                                                                                  \
            USHORT                  tcp_checksum1 = AnscTcpGetChecksum(tcp_header);         \
            USHORT                  tcp_checksum2 = 0;                                      \
                                                                                            \
            AnscTcpCalculateChecksum(ip4_header, tcp_header, tcp_body);                     \
                                                                                            \
            tcp_checksum2 = AnscTcpGetChecksum(tcp_header);                                 \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum1);                                  \
                                                                                            \
            v_result = (tcp_checksum1 == tcp_checksum2)? TRUE : FALSE;                      \
         }

#define  AnscTcpVerifyChecksum2(ip4_header, tcp_header, tcp_bpn, v_result)                  \
         {                                                                                  \
            USHORT                  tcp_checksum1 = AnscTcpGetChecksum(tcp_header);         \
            USHORT                  tcp_checksum2 = 0;                                      \
                                                                                            \
            AnscTcpCalculateChecksum2(ip4_header, tcp_header, tcp_bpn);                     \
                                                                                            \
            tcp_checksum2 = AnscTcpGetChecksum(tcp_header);                                 \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum1);                                  \
                                                                                            \
            v_result = (tcp_checksum1 == tcp_checksum2)? TRUE : FALSE;                      \
         }

#define  AnscTcpUpdateChecksumUlong(tcp_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  tcp_checksum = AnscTcpGetChecksum(tcp_header);          \
            ULONG                   ul_value1    = old_value;                               \
            ULONG                   ul_value2    = new_value;                               \
                                                                                            \
            tcp_checksum =                                                                  \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &ul_value1,                                                         \
                        sizeof(ul_value1),                                                  \
                        &ul_value2,                                                         \
                        sizeof(ul_value2),                                                  \
                        tcp_checksum                                                        \
                    );                                                                      \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum);                                   \
         }

#define  AnscTcpUpdateChecksumUshort(tcp_header, old_value, new_value)                      \
         {                                                                                  \
            USHORT                  tcp_checksum = AnscTcpGetChecksum(tcp_header);          \
            USHORT                  us_value1    = old_value;                               \
            USHORT                  us_value2    = new_value;                               \
                                                                                            \
            tcp_checksum =                                                                  \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &us_value1,                                                         \
                        sizeof(us_value1),                                                  \
                        &us_value2,                                                         \
                        sizeof(us_value2),                                                  \
                        tcp_checksum                                                        \
                    );                                                                      \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum);                                   \
         }

#define  AnscTcpUpdateChecksumUchar(tcp_header, old_value, new_value)                       \
         {                                                                                  \
            USHORT                  tcp_checksum = AnscTcpGetChecksum(tcp_header);          \
            UCHAR                   uc_value1    = old_value;                               \
            UCHAR                   uc_value2    = new_value;                               \
                                                                                            \
            tcp_checksum =                                                                  \
                AnscReplaceChecksum                                                         \
                    (                                                                       \
                        &uc_value1,                                                         \
                        sizeof(uc_value1),                                                  \
                        &uc_value2,                                                         \
                        sizeof(uc_value2),                                                  \
                        tcp_checksum                                                        \
                    );                                                                      \
                                                                                            \
            AnscTcpSetChecksum(tcp_header, tcp_checksum);                                   \
         }


/***********************************************************
          DEFINITIONS FOR VARIOUS TCP HEADER OPTIONS
***********************************************************/

/*
 * Options may occupy space at the end of the TCP header and are a multiple of 8 bits in lenght.
 * All options are included in the checksum. An option may begin on any octet boundary. Note that
 * the list of options may be shorter than the data offset field might imply. The content of the
 * header beyond the End-of-Option option must be header padding (i.e., zero).
 */
#define  TCP_OPTION_TYPE_EOL                        0           /* end of option list   */
#define  TCP_OPTION_TYPE_NOP                        1           /* no operation         */
#define  TCP_OPTION_TYPE_MSS                        2           /* maximum segment size */

#define  AnscTcpHasOptions(p)                       ( AnscTcpGetHeaderSize(p) > sizeof(TCP_HEADER) )

#define  AnscTcpCopyOption(dst_option, src_option)                                          \
         {                                                                                  \
            AnscCopyMemory(dst_option, src_option, (ULONG)src_option->Length);              \
         }

#define  AnscTcpGetFirstOption(tcp_header, tcp_option)                                      \
         {                                                                                  \
            if ( AnscTcpGetHeaderSize(tcp_header) <= sizeof(TCP_HEADER) )                   \
            {                                                                               \
                tcp_option = NULL;                                                          \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                tcp_option = (PTCP_OPTION)tcp_header->Options;                              \
            }                                                                               \
         }

#define  AnscTcpGetNextOption(tcp_header, this_option, next_option)                         \
         {                                                                                  \
            ULONG                   tcp_header_size = AnscTcpGetHeaderSize(tcp_header);     \
                                                                                            \
            if ( !this_option )                                                             \
            {                                                                               \
                if ( AnscTcpGetHeaderSize(tcp_header) <= sizeof(TCP_HEADER) )               \
                {                                                                           \
                    next_option = NULL;                                                     \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    next_option = (PTCP_OPTION)tcp_header->Options;                         \
                }                                                                           \
            }                                                                               \
            else if ( (this_option->Type == TCP_OPTION_TYPE_EOL) ||                         \
                      (this_option->Type == TCP_OPTION_TYPE_NOP) )                          \
            {                                                                               \
                next_option = (PTCP_OPTION)((ULONG)this_option + 1);                        \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                next_option = (PTCP_OPTION)((ULONG)this_option + this_option->Length);      \
            }                                                                               \
                                                                                            \
            if ( ((ULONG)next_option - (ULONG)tcp_header) >= tcp_header_size )              \
            {                                                                               \
                next_option = NULL;                                                         \
            }                                                                               \
         }

#define  AnscTcpGetOptionByType(tcp_header1, option_type1, tcp_option1)                     \
         {                                                                                  \
            PTCP_OPTION             cur_option1 = NULL;                                     \
            PTCP_OPTION             cur_option2 = NULL;                                     \
                                                                                            \
            AnscTcpGetFirstOption(tcp_header1, cur_option1);                                \
                                                                                            \
            while ( cur_option1 )                                                           \
            {                                                                               \
                if ( AnscTcpOptionGetType(cur_option1) == option_type1 )                    \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                AnscTcpGetNextOption(tcp_header1, cur_option1, cur_option2);                \
                                                                                            \
                cur_option1 = cur_option2;                                                  \
            }                                                                               \
                                                                                            \
            tcp_option1 = cur_option1;                                                      \
         }

/*
 * The following general data structure can represent most TCP header options except the "end-of-
 * option-list" and the "no-operation" options.
 */
typedef  struct
_TCP_OPTION
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
TCP_OPTION,  *PTCP_OPTION;

#define  AnscTcpOptionGetType(p)                    ( p->Type                        )
#define  AnscTcpOptionGetLength(p)                  ( p->Length                      )
#define  AnscTcpOptionGetData(p)                    ( p->Data                        )
#define  AnscTcpOptionGetDataSize(p)                ( p->Length - sizeof(TCP_OPTION) )

#define  AnscTcpOptionSetType(p, t)                 ( p->Type   = (UCHAR)t           )
#define  AnscTcpOptionSetLength(p, l)               ( p->Length = (UCHAR)l           )
#define  AnscTcpOptionSetData(p, d)                 AnscCopyMemory(p->Data, d, p->Length - 2)


/***********************************************************
         DEFINITIONS FOR MAXIMUM SEGMENT SIZE OPTION
***********************************************************/

/*
 * If this option is present, then it communicates the maximum receive segment size at the TCP
 * which sends this segment. This field must only be sent in the initial connection request (i.e.,
 * in segments with the SYN control bit set). If this option is not used, any segment size is
 * allowed though a reasonable value based on the local MTU is recommended.
 *
 *          +--------+--------+--------+--------+
 *          |00000010|00000100|   max seg size  |
 *          +--------+--------+--------+--------+
 */
typedef  struct
_TCP_OPTION_MSS
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          Value;
}
TCP_OPTION_MSS,  *PTCP_OPTION_MSS;

#define  AnscTcpOptionMssGetType                    AnscTcpOptionGetType
#define  AnscTcpOptionMssGetLength                  AnscTcpOptionGetLength
#define  AnscTcpOptionMssSetType                    AnscTcpOptionSetType
#define  AnscTcpOptionMssSetLength                  AnscTcpOptionSetLength

#define  AnscTcpGetMssOption(tcp_header2, mss_option)                                       \
         {                                                                                  \
            PTCP_OPTION             gen_tcp_option = NULL;                                  \
                                                                                            \
            AnscTcpGetOptionByType(tcp_header2, TCP_OPTION_TYPE_MSS, gen_tcp_option);       \
                                                                                            \
            mss_option = (PTCP_OPTION_MSS)gen_tcp_option;                                   \
         }

#define  AnscTcpOptionMssValidate(p, v_result)                                              \
         {                                                                                  \
            if ( p->Length != sizeof(TCP_OPTION_MSS) )                                      \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }

#define  AnscTcpOptionMssGetValue(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->Value)) )
#define  AnscTcpOptionMssSetValue(p, v)             AnscWriteUshort(&p->Value, AnscUshortFromHToN(v))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
