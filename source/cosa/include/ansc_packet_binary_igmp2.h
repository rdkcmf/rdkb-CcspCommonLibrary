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

    module:	ansc_packet_binary_igmp2.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Internet Group Management Protocol
        (IGMP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/27/02    initial revision.
        12/24/06    add definitions for IGMPv3.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_IGMP2_
#define  _ANSC_PACKET_BINARY_IGMP2_


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
                DATA STRUCTURES USED BY IGMP
***********************************************************/

/*
 * Define some const values used in the IGMP header definition
 */
#define  IGMP_TYPE_MEMBERSHIP_QUERY                 0x11
#define  IGMP_TYPE_MEMBERSHIP_REPORT_V1             0x12
#define  IGMP_TYPE_MEMBERSHIP_REPORT_V2             0x16
#define  IGMP_TYPE_LEAVE_GROUP                      0x17
#define  IGMP_TYPE_MEMBERSHIP_REPORT_V3             0x22

/*
 * The Internet Group Management Protocol (IGMP) is used by IP hosts to report their multicast
 * group memberships to any immediately-neighboring multicast routers. Like ICMP, IGMP is an
 * integral part of IP. It is required to be implemented by all hosts wishing to receive IP multi-
 * casts. IGMP messages are encapsulated in IP datagrams, with an IP protocol number of 2. All
 * IGMP messages are usually sent with IP TTL 1, and contain the IP Router Alert Option in their
 * IP header. All IGMP messages of concern to hosts have the following format:
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |      Type     | Max Resp Time |           Checksum            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                         Group Address                         |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_IGMPV2_HEADER
{
    UCHAR                           Type;
    UCHAR                           MaxResponseTime;
    USHORT                          Checksum;
    ANSC_IPV4_ADDRESS               GroupAddress;
}_struct_pack_
IGMPV2_HEADER,  *PIGMPV2_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIgmpv2GetType(p)                       ( p->Type                               )
#define  AnscIgmpv2GetMaxResponseTime(p)            ( p->MaxResponseTime                    )
#define  AnscIgmpv2GetChecksum(p)                   ( AnscReadUshort(&p->Checksum)          )
#define  AnscIgmpv2GetGroupAddressDot(p)            ( p->GroupAddress.Dot                   )
#define  AnscIgmpv2GetGroupAddressValue(p)          ( AnscReadUlong(&p->GroupAddress.Value) )

#define  AnscIgmpv2SetType(p, type)                 ( p->Type            = type )
#define  AnscIgmpv2SetMaxResponseTime(p, time)      ( p->MaxResponseTime = time )
#define  AnscIgmpv2SetChecksum(p, c)                AnscWriteUshort(&p->Checksum, c)
#define  AnscIgmpv2SetGroupAddressDot(p, addr)      AnscWriteUlong (&p->GroupAddress.Dot[0], *(PULONG)addr)
#define  AnscIgmpv2SetGroupAddressValue(p, addr)    AnscWriteUlong (&p->GroupAddress.Value,  addr)


/***********************************************************
                DATA STRUCTURES USED BY IGMPV3
***********************************************************/

/*
 * IGMP messages are encapsulated in IPv4 datagrams, with an IP protocol number of 2. Every IGMP
 * message described in this document is sent with an IP Time-to-Live of 1, IP Precedence of Inter-
 * network Control (e.g., Type of Service 0xc0), and carries an IP Router Alert option [RFC-2113]
 * in its IP header. IGMP message types are registered by the IANA [IANA-REG] as described by
 * [RFC-3228].
 */
typedef  struct  _IGMPV2_HEADER  IGMPV3_HEADER,  *PIGMPV3_HEADER;

#define  AnscIgmpv3GetType(p)                       AnscIgmpv2GetType(p)
#define  AnscIgmpv3GetChecksum(p)                   AnscIgmpv2GetChecksum(p)

#define  AnscIgmpv3SetType(p, type)                 AnscIgmpv2SetType(p, type)
#define  AnscIgmpv3SetChecksum(p, c)                AnscIgmpv2SetChecksum(p, c)

/*
 * Membership Queries are sent by IP multicast routers to query the multicast reception state of
 * neighboring interfaces. Queries havethe following format:
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |  Type = 0x11  | Max Resp Code |           Checksum            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                         Group Address                         |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          | Resv  |S| QRV |     QQIC      |     Number of Sources (N)     |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Source Address [1]                      |
 *          +-                                                             -+
 *          |                       Source Address [2]                      |
 *          +-                              .                              -+
 *          .                               .                               .
 *          .                               .                               .
 *          +-                                                             -+
 *          |                       Source Address [N]                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_IGMPV3_QUERY
{
    UCHAR                           Type;
    UCHAR                           MaxRespCode;
    USHORT                          Checksum;
    ANSC_IPV4_ADDRESS               GroupAddress;
    UCHAR                           SflagAndQrv;
    UCHAR                           Qqic;
    USHORT                          NumberOfSources;
    ANSC_IPV4_ADDRESS               SourceAddresses[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IGMPV3_QUERY,  *PIGMPV3_QUERY;

#define  AnscIgmpv3QueryGetType(p)                  ( p->Type                               )
#define  AnscIgmpv3QueryGetMaxRespCode(p)           ( p->MaxRespCode                        )
#define  AnscIgmpv3QueryGetMaxResponseTime(p)       ( ((p->MaxRespCode & 0x0F) | 0x10 ) << ((p->MaxRespCode >> 4) & 0x07 + 3) )
#define  AnscIgmpv3QueryGetChecksum(p)              ( AnscReadUshort(&p->Checksum)          )
#define  AnscIgmpv3QueryGetGroupAddressDot(p)       ( p->GroupAddress.Dot                   )
#define  AnscIgmpv3QueryGetGroupAddressValue(p)     ( AnscReadUlong(&p->GroupAddress.Value) )
#define  AnscIgmpv3QueryIsSflagSet(p)               ( (p->SflagAndQrv & 0x08) != 0          )
#define  AnscIgmpv3QueryGetQrv(p)                   (  p->SflagAndQrv & 0x07                )
#define  AnscIgmpv3QueryGetQqic(p)                  (  p->Qqic                              )
#define  AnscIgmpv3QueryGetQqit(p)                  ( ((p->Qqic & 0x0F) | 0x10 ) << ((p->Qqic >> 4) & 0x07 + 3) )
#define  AnscIgmpv3QueryGetNumberOfSources(p)       ( AnscUshortFromNToH(AnscReadUshort(&p->NumberOfSources)) )
#define  AnscIgmpv3QueryGetSourceAddress(p, n)      ( AnscReadUlong(&p->SourceAddresses[n].Value) )

#define  AnscIgmpv3QuerySetType(p, type)            ( p->Type        = type )
#define  AnscIgmpv3QuerySetMaxRespCode(p, code)     ( p->MaxRespCode = code )
#define  AnscIgmpv3QuerySetMaxResponseTime(p, time)                                         \
         {                                                                                  \
            if ( time < 128 )                                                               \
            {                                                                               \
                p->MaxRespCode = time;                                                      \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                UCHAR               exp  = 0;                                               \
                UCHAR               mant = 0;                                               \
                                                                                            \
                exp   = time /  0x10 - 3;                                                   \
                mant  = time >> (exp + 3);                                                  \
                mant &= 0x0F;                                                               \
                                                                                            \
                p->MaxRespCode = 128 + (exp << 4) + mant;                                   \
            }                                                                               \
         }
#define  AnscIgmpv3QuerySetChecksum(p, c)           AnscWriteUshort(&p->Checksum, c)
#define  AnscIgmpv3QuerySetGroupAddressDot(p, addr)                                         \
         AnscWriteUlong (&p->GroupAddress.Dot[0], *(PULONG)addr)
#define  AnscIgmpv3QuerySetGroupAddressValue(p, addr)                                       \
         AnscWriteUlong (&p->GroupAddress.Value,  addr)
#define  AnscIgmpv3QuerySetSflag(p)                 ( p->SflagAndQrv |= 0x08 )
#define  AnscIgmpv3QueryClrSflag(p)                 ( p->SflagAndQrv &= 0xF7 )
#define  AnscIgmpv3QuerySetSflag2(p, s)                                                     \
         {                                                                                  \
            if ( s )                                                                        \
            {                                                                               \
                AnscIgmpv3QuerySetSflag(p);                                                 \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                AnscIgmpv3QueryClrSflag(p);                                                 \
            }                                                                               \
         }
#define  AnscIgmpv3QuerySetQrv(p, qrv)                                                      \
         {                                                                                  \
            p->SflagAndQrv &= 0x07;                                                         \
                                                                                            \
            if ( qrv < 7 )                                                                  \
            {                                                                               \
                p->SflagAndQrv |= qrv;                                                      \
            }                                                                               \
         }
#define  AnscIgmpv3QuerySetQqic(p, code)            ( p->Qqic = code )
#define  AnscIgmpv3QuerySetQqit(p, time)                                                    \
         {                                                                                  \
            if ( time < 128 )                                                               \
            {                                                                               \
                p->Qqic = time;                                                             \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                UCHAR               exp  = 0;                                               \
                UCHAR               mant = 0;                                               \
                                                                                            \
                exp   = time /  0x10 - 3;                                                   \
                mant  = time >> (exp + 3);                                                  \
                mant &= 0x0F;                                                               \
                                                                                            \
                p->Qqic = 128 + (exp << 4) + mant;                                          \
            }                                                                               \
         }
#define  AnscIgmpv3QuerySetNumberOfSources(p, n)    AnscWriteUshort(&p->NumberOfSources, AnscUshortFromHToN(n))
#define  AnscIgmpv3QuerySetSourceAddress(p, n, addr)                                        \
         AnscWriteUlong (&p->SourceAddresses[n].Value, addr)

#define  AnscIgmpv3QueryIsSuppressRouterSideProcessing(p)                                   \
         AnscIgmpv3QueryIsSflagSet(p)
#define  AnscIgmpv3QueryGetQuerierRobustnessVariable(p)                                     \
         AnscIgmpv3QueryGetQrv(p)
#define  AnscIgmpv3QueryGetQuerierQueryIntervalCode(p)                                      \
         AnscIgmpv3QueryGetQqic(p)
#define  AnscIgmpv3QueryGetQuerierQueryIntervalTime(p)                                      \
         AnscIgmpv3QueryGetQqit(p)

#define  AnscIgmpv3QuerySetQuerierRobustnessVariable(p)                                     \
         AnscIgmpv3QuerySetQrv(p)
#define  AnscIgmpv3QuerySetQuerierQueryIntervalCode(p)                                      \
         AnscIgmpv3QuerySetQqic(p)
#define  AnscIgmpv3QuerySetQuerierQueryIntervalTime(p)                                      \
         AnscIgmpv3QuerySetQqit(p)

/*
 * Version 3 Membership Reports are sent by IP systems to report (to neighboring routers) the
 * current multicast reception state, or changes in the multicast reception state, of their inter-
 * faces. Reports have the following format:
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |  Type = 0x22  |    Reserved   |           Checksum            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |           Reserved            |  Number of Group Records (M)  |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                                                               |
 *          .                                                               .
 *          .                        Group Record [1]                       .
 *          .                                                               .
 *          |                                                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                                                               |
 *          .                                                               .
 *          .                        Group Record [2]                       .
 *          .                                                               .
 *          |                                                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                               .                               |
 *          .                               .                               .
 *          |                               .                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                                                               |
 *          .                                                               .
 *          .                        Group Record [M]                       .
 *          .                                                               .
 *          |                                                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_IGMPV3_REPORT
{
    UCHAR                           Type;
    UCHAR                           Reserved1;
    USHORT                          Checksum;
    USHORT                          Reserved2;
    USHORT                          NumberOfGroupRecords;
    UCHAR                           GroupRecords[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IGMPV3_REPORT,  *PIGMPV3_REPORT;

#define  AnscIgmpv3ReportGetType(p)                 ( p->Type                               )
#define  AnscIgmpv3ReportGetChecksum(p)             ( AnscReadUshort(&p->Checksum)          )
#define  AnscIgmpv3ReportGetNumberOfGroupRecords(p) ( AnscUshortFromNToH(AnscReadUshort(&p->NumberOfGroupRecords)) )
#define  AnscIgmpv3ReportGetFirstGroupRecord(p)     ( p->GroupRecords )
#define  AnscIgmpv3ReportGetNextGroupRecord(this_gr)                                        \
         ( (void*)((ULONG)this_gr + AnscIgmpv3GrGetSize(this_gr)) )

#define  AnscIgmpv3ReportSetType(p, type)           ( p->Type        = type )
#define  AnscIgmpv3ReportSetChecksum(p, c)          AnscWriteUshort(&p->Checksum, c)
#define  AnscIgmpv3ReportSetNumberOfGroupRecords(p, n)                                      \
         AnscWriteUshort(&p->NumberOfGroupRecords, AnscUshortFromHToN(n))

/*
 * Where each Group Record has the following internal format:
 *
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |  Record Type  |  Aux Data Len |     Number of Sources (N)     |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Multicast Address                       |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                       Source Address [1]                      |
 *          +-                                                             -+
 *          |                       Source Address [2]                      |
 *          +-                                                             -+
 *          .                               .                               .
 *          .                               .                               .
 *          .                               .                               .
 *          +-                                                             -+
 *          |                       Source Address [N]                      |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                                                               |
 *          .                                                               .
 *          .                         Auxiliary Data                        .
 *          .                                                               .
 *          |                                                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define  IGMPV3_GROUP_RECORD_TYPE_IS_IN             1
#define  IGMPV3_GROUP_RECORD_TYPE_IS_EX             2
#define  IGMPV3_GROUP_RECORD_TYPE_TO_IN             3
#define  IGMPV3_GROUP_RECORD_TYPE_TO_EX             4
#define  IGMPV3_GROUP_RECORD_TYPE_ALLOW             5
#define  IGMPV3_GROUP_RECORD_TYPE_BLOCK             6

typedef  struct
_IGMPV3_GROUP_RECORD
{
    UCHAR                           RecordType;
    UCHAR                           AuxDataLen;
    USHORT                          NumberOfSources;
    ANSC_IPV4_ADDRESS               GroupAddress;
    ANSC_IPV4_ADDRESS               SourceAddresses[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IGMPV3_GROUP_RECORD,  *PIGMPV3_GROUP_RECORD;

#define  AnscIgmpv3GrGetRecordType(p)               ( p->RecordType )
#define  AnscIgmpv3GrGetAuxDataLen(p)               ( p->AuxDataLen )
#define  AnscIgmpv3GrGetNumberOfSources(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->NumberOfSources)) )
#define  AnscIgmpv3GrGetGroupAddressDot(p)          ( p->GroupAddress.Dot                   )
#define  AnscIgmpv3GrGetGroupAddressValue(p)        ( AnscReadUlong(&p->GroupAddress.Value) )
#define  AnscIgmpv3GrGetSourceAddress(p, n)         ( AnscReadUlong(&p->SourceAddresses[n].Value) )
#define  AnscIgmpv3GrGetSize(p)                     ( sizeof(IGMPV3_GROUP_RECORD) + AnscIgmpv3GrGetNumberOfSources(p) * 4 +  AnscIgmpv3GrGetAuxDataLen(p) )

#define  AnscIgmpv3GrSetRecordType(p, type)         ( p->RecordType = type )
#define  AnscIgmpv3GrSetAuxDataLen(p, len)          ( p->AuxDataLen = len  )
#define  AnscIgmpv3GrSetNumberOfSources(p, n)       AnscWriteUshort(&p->NumberOfSources, AnscUshortFromHToN(n))
#define  AnscIgmpv3GrSetGroupAddressDot(p, addr)    AnscWriteUlong (&p->GroupAddress.Dot[0], *(PULONG)addr)
#define  AnscIgmpv3GrSetGroupAddressValue(p, addr)  AnscWriteUlong (&p->GroupAddress.Value,  addr         )
#define  AnscIgmpv3GrSetSourceAddress(p, n, addr)   AnscWriteUlong (&p->SourceAddresses[n].Value,  addr)
#define  AnscIgmpv3GrSetSourceAddresses(p, addr_list)                                       \
         {                                                                                  \
            ULONG                   ii       = 0;                                           \
            ULONG                   num_addr = AnscIgmpv3GrGetNumberOfSources(p);           \
            ULONG                   src_addr = 0;                                           \
                                                                                            \
            for ( ii = 0; ii < num_addr; ii++ )                                             \
            {                                                                               \
                src_addr = AnscReadUlong((void*)((ULONG)addr_list + ii * 4));               \
                                                                                            \
                AnscIgmpv3GrSetSourceAddress(p, ii, src_addr);                              \
            }                                                                               \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
