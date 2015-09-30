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

    module:	ansc_packet_binary_dns.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Domain Name System messages (DNS).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_DNS_
#define  _ANSC_PACKET_BINARY_DNS_


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
                 DATA STRUCTURES USED BY DNS
***********************************************************/

/*
 * The domain system is a mixture of functions and data types which are an official protocol and
 * functions and data types which are still experimental. Since the domain system is intentionally
 * extensible, new data types and experimental behavior should always be expected in parts of the
 * system beyond the official protocol.
 */
#define  DNS_MESSAGE_TYPE_MASK                      0x8000
#define  DNS_MESSAGE_TYPE_OFFSET                    15
#define  DNS_MESSAGE_TYPE_QUERY                     0
#define  DNS_MESSAGE_TYPE_REPLY                     1

#define  DNS_OPCODE_MASK                            0x7800
#define  DNS_OPCODE_OFFSET                          11
#define  DNS_OPCODE_QUERY                           0
#define  DNS_OPCODE_IQUERY                          1
#define  DNS_OPCODE_STATUS                          2

#define  DNS_FLAG_MASK_AA                           0x0400      /* Authoritative Answer */
#define  DNS_FLAG_MASK_TC                           0x0200      /* Truncation           */
#define  DNS_FLAG_MASK_RD                           0x0100      /* Recursion Desired    */
#define  DNS_FLAG_MASK_RA                           0x0080      /* Recursion Available  */

#define  DNS_FLAG_OFFSET_AA                         10
#define  DNS_FLAG_OFFSET_TC                         9
#define  DNS_FLAG_OFFSET_RD                         8
#define  DNS_FLAG_OFFSET_RA                         7

#define  DNS_Z_MASK                                 0x0070
#define  DNS_Z_OFFSET                               4

#define  DNS_RCODE_MASK                             0x000F
#define  DNS_RCODE_OFFSET                           0
#define  DNS_RCODE_NO_ERROR                         0
#define  DNS_RCODE_FORMAT_ERROR                     1
#define  DNS_RCODE_SERVER_FAILURE                   2
#define  DNS_RCODE_NAME_ERROR                       3
#define  DNS_RCODE_NOT_IMPLEMENTED                  4
#define  DNS_RCODE_REFUSED                          5

/*
 * As defined in section 4.1.1 of the standard track IETF RFC 1035 (Domain Implementation and
 * Specification), the DNS header contains the following fields:
 *
 *                                          1  1  1  1  1  1
 *            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                      ID                       |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    QDCOUNT                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    ANCOUNT                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    NSCOUNT                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    ARCOUNT                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 */
typedef  struct
_DNS_HEADER
{
    USHORT                          Id;
    USHORT                          FlagsAndCodes;
    USHORT                          QdCount;
    USHORT                          AnCount;
    USHORT                          NsCount;
    USHORT                          ArCount;
    UCHAR                           Sections[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
DNS_HEADER,  *PDNS_HEADER;

#define  AnscDnsIsQuery(p)                          ( AnscDnsGetMessageType(p) == DNS_MESSAGE_TYPE_QUERY )
#define  AnscDnsIsReply(p)                          ( AnscDnsGetMessageType(p) == DNS_MESSAGE_TYPE_REPLY )

#define  AnscDnsGetId(p)                            ( AnscUshortFromNToH(AnscReadUshort(&p->Id           )) )
#define  AnscDnsGetFlagsAndCodes(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->FlagsAndCodes)) )
#define  AnscDnsGetQdCount(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->QdCount      )) )
#define  AnscDnsGetAnCount(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->AnCount      )) )
#define  AnscDnsGetNsCount(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->NsCount      )) )
#define  AnscDnsGetArCount(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->ArCount      )) )

#define  AnscDnsGetMessageType(p)                   ( (AnscDnsGetFlagsAndCodes(p) & DNS_MESSAGE_TYPE_MASK) >> DNS_MESSAGE_TYPE_OFFSET )
#define  AnscDnsGetOpCode(p)                        ( (AnscDnsGetFlagsAndCodes(p) & DNS_OPCODE_MASK      ) >> DNS_OPCODE_OFFSET       )
#define  AnscDnsIsAuthoritativeAnswer(p)            ( (AnscDnsGetFlagsAndCodes(p) & DNS_FLAG_MASK_AA     ) != 0                       )
#define  AnscDnsIsTruncated(p)                      ( (AnscDnsGetFlagsAndCodes(p) & DNS_FLAG_MASK_TC     ) != 0                       )
#define  AnscDnsIsRecursionDesired(p)               ( (AnscDnsGetFlagsAndCodes(p) & DNS_FLAG_MASK_RD     ) != 0                       )
#define  AnscDnsIsRecursionAvailable(p)             ( (AnscDnsGetFlagsAndCodes(p) & DNS_FLAG_MASK_RA     ) != 0                       )
#define  AnscDnsGetZ(p)                             ( (AnscDnsGetFlagsAndCodes(p) & DNS_Z_MASK           ) >> DNS_Z_OFFSET            )
#define  AnscDnsGetRcode(p)                         ( (AnscDnsGetFlagsAndCodes(p) & DNS_RCODE_MASK       ) >> DNS_RCODE_OFFSET        )

#define  AnscDnsSetId(p, id)                        AnscWriteUshort(&p->Id,            AnscUshortFromHToN(id))
#define  AnscDnsSetFlagsAndCodes(p, fc)             AnscWriteUshort(&p->FlagsAndCodes, AnscUshortFromHToN(fc))
#define  AnscDnsSetQdCount(p, c)                    AnscWriteUshort(&p->QdCount,       AnscUshortFromHToN(c ))
#define  AnscDnsSetAnCount(p, c)                    AnscWriteUshort(&p->AnCount,       AnscUshortFromHToN(c ))
#define  AnscDnsSetNsCount(p, c)                    AnscWriteUshort(&p->NsCount,       AnscUshortFromHToN(c ))
#define  AnscDnsSetArCount(p, c)                    AnscWriteUshort(&p->ArCount,       AnscUshortFromHToN(c ))

#define  AnscDnsRstMessageType(p)                   (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_MESSAGE_TYPE_MASK )
#define  AnscDnsRstOpCode(p)                        (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_OPCODE_MASK       )
#define  AnscDnsRstAuthoritativeAnswer(p)           (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_FLAG_MASK_AA      )
#define  AnscDnsRstTruncated(p)                     (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_FLAG_MASK_TC      )
#define  AnscDnsRstRecursionDesired(p)              (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_FLAG_MASK_RD      )
#define  AnscDnsRstRecursionAvailable(p)            (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_FLAG_MASK_RA      )
#define  AnscDnsRstRcode(p)                         (USHORT)( AnscDnsGetFlagsAndCodes(p) & ~DNS_RCODE_MASK        )

#define  AnscDnsSetMessageType(p, t)                AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstMessageType(p)         | (t << DNS_MESSAGE_TYPE_OFFSET)) )
#define  AnscDnsSetOpCode(p, c)                     AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstOpCode(p)              | (c << DNS_OPCODE_OFFSET      )) )
#define  AnscDnsSetAuthoritativeAnswer(p, b)        AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstAuthoritativeAnswer(p) | (b << DNS_FLAG_OFFSET_AA     )) )
#define  AnscDnsSetTruncated(p, b)                  AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstTruncated(p)           | (b << DNS_FLAG_OFFSET_TC     )) )
#define  AnscDnsSetRecursionDesired(p, b)           AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstRecursionDesired(p)    | (b << DNS_FLAG_OFFSET_RD     )) )
#define  AnscDnsSetRecursionAvailable(p, b)         AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstRecursionAvailable(p)  | (b << DNS_FLAG_OFFSET_RA     )) )
#define  AnscDnsSetRcode(p, c)                      AnscDnsSetFlagsAndCodes(p, (USHORT)(AnscDnsRstRcode(p)               | (c << DNS_RCODE_OFFSET       )) )

/*
 * Domain names in messages are expressed in terms of a sequence of labels. Each label is
 * represented as a one octet length field followed by that number of octets. Since every domain
 * name ends with the null label of the root, a domain name is terminated by a length byte of zero.
 * The high order two bits of every length octet must be zero, and the remaining six bits of the
 * length field limit the label to 63 octets or less.
 */
#define  ANSC_DNS_LABEL_LENGTH_MASK                 0x3F
#define  ANSC_DNS_LABEL_TYPE_MASK                   0xC0
#define  ANSC_DNS_LABEL_TYPE_OFFSET                 6
#define  ANSC_DNS_LABEL_TYPE_POINTER                0x03
#define  ANSC_DNS_LABEL_TYPE_STRING                 0x00

#define  AnscDnsGetLabelSize(l)                     ( (ULONG)(AnscDnsGetLabelLength(l) + 1)                                           )
#define  AnscDnsGetLabelLength(l)                   ( ((*(PUCHAR)l) &  ANSC_DNS_LABEL_LENGTH_MASK)                                    )
#define  AnscDnsGetLabelType(l)                     ( ((*(PUCHAR)l) &  ANSC_DNS_LABEL_TYPE_MASK  ) >> ANSC_DNS_LABEL_TYPE_OFFSET      )

#define  AnscDnsSetLabelType(l, t)                  ( *(PUCHAR)l = ((*(PUCHAR)l) & ~ANSC_DNS_LABEL_TYPE_MASK) | ( t << ANSC_DNS_LABEL_TYPE_OFFSET) )

#define  AnscDnsIsLabelPointer(l)                   ( AnscDnsGetLabelType(l) == ANSC_DNS_LABEL_TYPE_POINTER )
#define  AnscDnsIsLabelString(l)                    ( AnscDnsGetLabelType(l) == ANSC_DNS_LABEL_TYPE_STRING  )

#define  ANSC_DNS_LABEL_POINTER_MASK                0x3FFF

#define  AnscDnsGetLabelPointer(l)                  ( AnscUshortFromNToH(AnscReadUshort(l))  & ANSC_DNS_LABEL_POINTER_MASK )
#define  AnscDnsSetLabelPointer(l, p)               AnscWriteUshort(l, AnscUshortFromHToN((USHORT)(p | 0xC000)) )

#define  AnscDnsLabelGetSize(label, size)                                                   \
         {                                                                                  \
            if ( AnscDnsIsLabelPointer(label) )                                             \
            {                                                                               \
                size = sizeof(USHORT);                                                      \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                size = AnscDnsGetLabelSize(label);                                          \
            }                                                                               \
         }

#define  AnscDnsNameGetSize(name, size)                                                     \
         {                                                                                  \
            char*                   domain_name5  = (char*)name;                            \
            char*                   domain_label5 = domain_name5;                           \
            ULONG                   name_size5    = 0;                                      \
            ULONG                   label_size5   = 0;                                      \
                                                                                            \
            while ( TRUE )                                                                  \
            {                                                                               \
                if ( AnscDnsIsLabelPointer(domain_label5) )                                 \
                {                                                                           \
                    label_size5  = sizeof(USHORT);                                          \
                    name_size5  += label_size5;                                             \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    label_size5 = AnscDnsGetLabelSize(domain_label5);                       \
                    name_size5 += label_size5;                                              \
                                                                                            \
                    if ( label_size5 == 1 )                                                 \
                    {                                                                       \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                domain_label5 += label_size5;                                               \
            }                                                                               \
                                                                                            \
            size = name_size5;                                                              \
         }

#define  AnscDnsNameRead2(header, name, buffer, size)                                       \
         {                                                                                  \
            char*                   domain_name2   = (char*)name;                           \
            char*                   domain_label2  = domain_name;                           \
            char*                   dst_buffer2    = (char*)buffer;                         \
            ULONG                   buffer_offset2 = 0;                                     \
            ULONG                   name_size2     = 0;                                     \
            ULONG                   label_size2    = 0;                                     \
                                                                                            \
            while ( TRUE )                                                                  \
            {                                                                               \
                if ( AnscDnsIsLabelPointer(domain_label2) )                                 \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    label_size2 = AnscDnsGetLabelLength(domain_label2);                     \
                                                                                            \
                    if ( label_size2 > 0 )                                                  \
                    {                                                                       \
                        AnscCopyMemory                                                      \
                            (                                                               \
                                (char*)((ULONG)dst_buffer2 + buffer_offset2),               \
                                domain_label2 + 1,                                          \
                                label_size2                                                 \
                            );                                                              \
                                                                                            \
                        name_size2     += label_size2;                                      \
                        buffer_offset2 += label_size2;                                      \
                                                                                            \
                        dst_buffer2[buffer_offset2] = '.';                                  \
                                                                                            \
                        name_size2    ++;                                                   \
                        buffer_offset2++;                                                   \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                domain_label2 += label_size2 + 1;                                           \
            }                                                                               \
                                                                                            \
            size = name_size2;                                                              \
         }

#define  AnscDnsNameRead(header, name, buffer, size)                                        \
         {                                                                                  \
            char*                   domain_name   = (char*)name;                            \
            char*                   domain_label  = domain_name;                            \
            char*                   dst_buffer    = (char*)buffer;                          \
            ULONG                   buffer_offset = 0;                                      \
            ULONG                   name_size     = 0;                                      \
            ULONG                   label_size    = 0;                                      \
                                                                                            \
            while ( TRUE )                                                                  \
            {                                                                               \
                if ( AnscDnsIsLabelPointer(domain_label) )                                  \
                {                                                                           \
                    AnscDnsNameRead2                                                        \
                        (                                                                   \
                            header,                                                         \
                            (char*)((ULONG)name + AnscDnsGetLabelPointer(domain_label)),    \
                            (char*)((ULONG)dst_buffer + buffer_offset),                     \
                            label_size                                                      \
                        );                                                                  \
                                                                                            \
                    name_size     += label_size;                                            \
                    buffer_offset += label_size;                                            \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    label_size = AnscDnsGetLabelLength(domain_label);                       \
                                                                                            \
                    if ( label_size > 0 )                                                   \
                    {                                                                       \
                        AnscCopyMemory                                                      \
                            (                                                               \
                                (char*)((ULONG)dst_buffer + buffer_offset),                 \
                                domain_label + 1,                                           \
                                label_size                                                  \
                            );                                                              \
                                                                                            \
                        name_size     += label_size;                                        \
                        buffer_offset += label_size;                                        \
                                                                                            \
                        dst_buffer[buffer_offset] = '.';                                    \
                                                                                            \
                        name_size    ++;                                                    \
                        buffer_offset++;                                                    \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                domain_label += label_size + 1;                                             \
            }                                                                               \
                                                                                            \
            size = name_size;                                                               \
         }

#define  AnscDnsNameWrite(name, buffer, size)                                               \
         {                                                                                  \
            char*                   domain_name   = (char*)name;                            \
            char*                   domain_label  = domain_name;                            \
            char*                   src_buffer    = (char*)buffer;                          \
            ULONG                   buffer_offset = 0;                                      \
            ULONG                   label_offset  = 0;                                      \
            ULONG                   left_size     = size;                                   \
            ULONG                   copy_size     = 0;                                      \
            ULONG                   name_size     = 0;                                      \
                                                                                            \
            while ( left_size > 0 )                                                         \
            {                                                                               \
                while ( src_buffer[label_offset] != '.' )                                   \
                {                                                                           \
                    domain_label[1 + label_offset] = src_buffer[label_offset];              \
                    label_offset++;                                                         \
                }                                                                           \
                                                                                            \
                domain_label[0] = (char)((UCHAR)label_offset & ANSC_DNS_LABEL_LENGTH_MASK); \
                label_offset   += 1;                                                        \
                domain_label   += label_offset;                                             \
                src_buffer     += label_offset;                                             \
                left_size      -= label_offset;                                             \
                copy_size      += label_offset;                                             \
                label_offset    = 0;                                                        \
                                                                                            \
                if ( (left_size == 0) && (*(src_buffer - 1) == '.') )                       \
                {                                                                           \
                    *domain_label = 0;                                                      \
                }                                                                           \
            }                                                                               \
                                                                                            \
            name_size = copy_size;                                                          \
         }

#define  AnscDnsNameWritePointer(name, pointer)     AnscDnsSetLabelPointer(name, ((USHORT)pointer))


/***********************************************************
        DATA STRUCTURES USED BY DNS MESSAGE SECTIONS
***********************************************************/

/*
 * The question section is used to carry the "question" in most queries, i.e., the parameters that
 * define what is being asked.  The section contains QDCOUNT (usually 1) entries, each of the
 * following format:
 *
 *                                          1  1  1  1  1  1
 *            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                                               |
 *          /                     QNAME                     /
 *          /                                               /
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                     QTYPE                     |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                     QCLASS                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 */
#define  DNS_RR_TYPE_A                              1
#define  DNS_RR_TYPE_NS                             2
#define  DNS_RR_TYPE_CNAME                          5
#define  DNS_RR_TYPE_SOA                            6
#define  DNS_RR_TYPE_PTR                            12
#define  DNS_RR_TYPE_HINFO                          13
#define  DNS_RR_TYPE_MX                             15
#define  DNS_RR_TYPE_AAAA                           28
#define  DNS_RR_TYPE_AXFR                           252
#define  DNS_RR_TYPE_WILDCARD                       255

#define  DNS_RR_CLASS_IN                            1
#define  DNS_RR_CLASS_CH                            3
#define  DNS_RR_CLASS_WILDCARD                      255

typedef  struct
_DNS_QD_ENTRY_FIX
{
    USHORT                          QType;
    USHORT                          QClass;
}_struct_pack_
DNS_QD_ENTRY_FIX,  *PDNS_QD_ENTRY_FIX;

#define  AnscDnsQdEntryFixGetQType(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->QType )) )
#define  AnscDnsQdEntryFixGetQClass(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->QClass)) )

#define  AnscDnsQdEntryFixSetQType(p, t)            AnscWriteUshort(&p->QType,  AnscUshortFromHToN(t))
#define  AnscDnsQdEntryFixSetQClass(p, c)           AnscWriteUshort(&p->QClass, AnscUshortFromHToN(c))

#define  AnscDnsQdSectionGetSize(header, qs, size)                                          \
         {                                                                                  \
            ULONG                   entry_number = AnscDnsGetQdCount(header);               \
            ULONG                   entry_offset = 0;                                       \
            ULONG                   entry_size   = 0;                                       \
            ULONG                   entry_index  = 0;                                       \
                                                                                            \
            for ( entry_index = 0; entry_index < entry_number; entry_index++ )              \
            {                                                                               \
                AnscDnsQdEntryGetSize                                                       \
                    (                                                                       \
                        header,                                                             \
                        (char*)((ULONG)qs + entry_offset),                                  \
                        entry_size                                                          \
                    );                                                                      \
                                                                                            \
                entry_offset += entry_size;                                                 \
            }                                                                               \
                                                                                            \
            size = entry_offset;                                                            \
         }

#define  AnscDnsQdSectionGetEntry(header, qs, i, qe)                                        \
         {                                                                                  \
            ULONG                   entry_offset = 0;                                       \
            ULONG                   entry_size   = 0;                                       \
            ULONG                   entry_index  = 0;                                       \
                                                                                            \
            for ( entry_index = 0; entry_index < i; entry_index++ )                         \
            {                                                                               \
                AnscDnsQdEntryGetSize                                                       \
                    (                                                                       \
                        header,                                                             \
                        (char*)((ULONG)qs + entry_offset),                                  \
                        entry_size                                                          \
                    );                                                                      \
                                                                                            \
                entry_offset += entry_size;                                                 \
            }                                                                               \
                                                                                            \
            qe = (char*)((ULONG)qs + entry_offset);                                         \
         }

#define  AnscDnsQdEntryGetNextEntry(header, qe, next_qe)                                    \
         {                                                                                  \
            ULONG                   entry_size = 0;                                         \
                                                                                            \
            AnscDnsQdEntryGetSize(header, qe, entry_size);                                  \
                                                                                            \
            next_qe = (char*)((ULONG)qe + entry_size);                                      \
         }

#define  AnscDnsQdEntryGetSize(header, qe, size)                                            \
         {                                                                                  \
            AnscDnsNameGetSize((char*)qe, size);                                            \
                                                                                            \
            size += sizeof(DNS_QD_ENTRY_FIX);                                               \
         }

#define  AnscDnsQdEntryGetQName(header, qe, buffer, size)                                   AnscDnsNameRead(header, (char*)qe, buffer, size)
#define  AnscDnsQdEntryGetQType(header, qe, q_type)                                         \
         {                                                                                  \
            ULONG                   entry_size  = 0;                                        \
            PDNS_QD_ENTRY_FIX       pQdEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsQdEntryGetSize(header, qe, entry_size);                                  \
                                                                                            \
            pQdEntryFix =                                                                   \
                (PDNS_QD_ENTRY_FIX)((ULONG)qe + entry_size - sizeof(DNS_QD_ENTRY_FIX));     \
                                                                                            \
            q_type = AnscDnsQdEntryFixGetQType(pQdEntryFix);                                \
         }
#define  AnscDnsQdEntryGetQClass(header, qe, q_class)                                       \
         {                                                                                  \
            ULONG                   entry_size  = 0;                                        \
            PDNS_QD_ENTRY_FIX       pQdEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsQdEntryGetSize(header, qe, entry_size);                                  \
                                                                                            \
            pQdEntryFix =                                                                   \
                (PDNS_QD_ENTRY_FIX)((ULONG)qe + entry_size - sizeof(DNS_QD_ENTRY_FIX));     \
                                                                                            \
            q_class = AnscDnsQdEntryFixGetQClass(pQdEntryFix);                              \
         }

#define  AnscDnsQdEntrySetQNameString(header, qe, buffer, size)                             AnscDnsNameWrite((char*)qe, buffer, size)
#define  AnscDnsQdEntrySetQNamePointer(header, qe, pointer)                                 AnscDnsNameWritePointer((char*)qe, pointer)
#define  AnscDnsQdEntrySetQType(header, qe, q_type)                                         \
         {                                                                                  \
            ULONG                   entry_size  = 0;                                        \
            PDNS_QD_ENTRY_FIX       pQdEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsQdEntryGetSize(header, qe, entry_size);                                  \
                                                                                            \
            pQdEntryFix =                                                                   \
                (PDNS_QD_ENTRY_FIX)((ULONG)qe + entry_size - sizeof(DNS_QD_ENTRY_FIX));     \
                                                                                            \
            AnscDnsQdEntryFixSetQType(pQdEntryFix, q_type);                                 \
         }
#define  AnscDnsQdEntrySetQClass(header, qe, q_class)                                       \
         {                                                                                  \
            ULONG                   entry_size  = 0;                                        \
            PDNS_QD_ENTRY_FIX       pQdEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsQdEntryGetSize(header, qe, entry_size);                                  \
                                                                                            \
            pQdEntryFix =                                                                   \
                (PDNS_QD_ENTRY_FIX)((ULONG)qe + entry_size - sizeof(DNS_QD_ENTRY_FIX));     \
                                                                                            \
            AnscDnsQdEntryFixSetQClass(pQdEntryFix, q_class);                               \
         }

/*
 * The answer, authority, and additional sections all share the same format: a variable number of
 * resource records, where the number of records is specified in the corresponding count field in
 * the header.
 */
#define  AnscDnsAnSectionGetSize(header, as, size)                                          AnscDnsRrSectionGetSize(header, AnscDnsGetAnCount(header), as, size)
#define  AnscDnsAnSectionGetEntry(header, as, i, ae)                                        AnscDnsRrSectionGetEntry(header, as, i, ae)

#define  AnscDnsNsSectionGetSize(header, as, size)                                          AnscDnsRrSectionGetSize(header, AnscDnsGetNsCount(header), as, size)
#define  AnscDnsNsSectionGetEntry(header, as, i, ae)                                        AnscDnsRrSectionGetEntry(header, as, i, ae)

#define  AnscDnsArSectionGetSize(header, as, size)                                          AnscDnsRrSectionGetSize(header, AnscDnsGetArCount(header), as, size)
#define  AnscDnsArSectionGetEntry(header, as, i, ae)                                        AnscDnsRrSectionGetEntry(header, as, i, ae)

#define  AnscDnsGetQdSection(header, qd_section)                                            \
         {                                                                                  \
            qd_section = (char*)header->Sections;                                           \
         }
#define  AnscDnsGetAnSection(header, an_section)                                            \
         {                                                                                  \
            char*                   qd_section = NULL;                                      \
            ULONG                   qd_size    = 0;                                         \
                                                                                            \
            AnscDnsGetQdSection    (header, qd_section);                                    \
            AnscDnsQdSectionGetSize(header, qd_section, qd_size);                           \
                                                                                            \
            an_section = (char*)((ULONG)qd_section + qd_size);                              \
         }
#define  AnscDnsGetNsSection(header, ns_section)                                            \
         {                                                                                  \
            char*                   an_section = NULL;                                      \
            ULONG                   an_size    = 0;                                         \
                                                                                            \
            AnscDnsGetAnSection    (header, an_section);                                    \
            AnscDnsAnSectionGetSize(header, an_section, an_size);                           \
                                                                                            \
            ns_section = (char*)((ULONG)an_section + an_size);                              \
         }
#define  AnscDnsGetArSection(header, ar_section)                                            \
         {                                                                                  \
            char*                   ns_section = NULL;                                      \
            ULONG                   ns_size    = 0;                                         \
                                                                                            \
            AnscDnsGetNsSection    (header, ns_section);                                    \
            AnscDnsNsSectionGetSize(header, ns_section, ns_size);                           \
                                                                                            \
            ar_section = (char*)((ULONG)ns_section + ns_size);                              \
         }


/***********************************************************
         DATA STRUCTURES USED BY DNS RESOURCE RECORD
***********************************************************/

/*
 * The answer, authority, and additional sections all share the same format: a variable number of
 * resource records, where the number of records is specified in the corresponding count field in
 * the header. Each resource record has the following format:
 *
 *                                          1  1  1  1  1  1
 *            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                                               |
 *          /                                               /
 *          /                      NAME                     /
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                      TYPE                     |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                     CLASS                     |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                      TTL                      |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                   RDLENGTH                    |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
 *          /                     RDATA                     /
 *          /                                               /
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 */

typedef  struct
_DNS_RR_ENTRY_FIX
{
    USHORT                          Type;
    USHORT                          Class;
    ULONG                           Ttl;
    USHORT                          RdLength;
    UCHAR                           Rdata[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
DNS_RR_ENTRY_FIX,  *PDNS_RR_ENTRY_FIX;

#define  AnscDnsRrEntryFixGetType(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->Type    )) )
#define  AnscDnsRrEntryFixGetClass(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->Class   )) )
#define  AnscDnsRrEntryFixGetTtl(p)                 ( AnscUlongFromNToH (AnscReadUlong (&p->Ttl     )) )
#define  AnscDnsRrEntryFixGetRdLength(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->RdLength)) )
#define  AnscDnsRrEntryFixGetRdata(p)               ( p->Rdata                                         )

#define  AnscDnsRrEntryFixGetRdAddrDot(p)           ( p->Rdata                                         )
#define  AnscDnsRrEntryFixGetRdAddrValue(p)         ( AnscReadUlong(p->Rdata)                          )

#define  AnscDnsRrEntryFixSetType(p, t)             AnscWriteUshort(&p->Type,     AnscUshortFromHToN(t))
#define  AnscDnsRrEntryFixSetClass(p, c)            AnscWriteUshort(&p->Class,    AnscUshortFromHToN(c))
#define  AnscDnsRrEntryFixSetTtl(p, t)              AnscWriteUlong (&p->Ttl,      AnscUlongFromHToN (t))
#define  AnscDnsRrEntryFixSetRdLength(p, l)         AnscWriteUshort(&p->RdLength, AnscUshortFromHToN(l))
#define  AnscDnsRrEntryFixSetRdata(p, d)            AnscCopyMemory(p->Rdata, d, AnscDnsRrEntryFixGetRdLength(p))

#define  AnscDnsRrEntryFixSetRdAddrDot(p, addr)     AnscWriteUlong(p->Rdata,  *(PULONG)addr)
#define  AnscDnsRrEntryFixSetRdAddrValue(p, addr)   AnscWriteUlong(p->Rdata,  addr         )

#define  AnscDnsRrSectionGetSize(header, count, rs, size)                                   \
         {                                                                                  \
            ULONG                   entry_number = count;                                   \
            ULONG                   entry_offset = 0;                                       \
            ULONG                   entry_size   = 0;                                       \
            ULONG                   entry_index  = 0;                                       \
                                                                                            \
            for ( entry_index = 0; entry_index < entry_number; entry_index++ )              \
            {                                                                               \
                AnscDnsRrEntryGetSize                                                       \
                    (                                                                       \
                        header,                                                             \
                        (char*)((ULONG)rs + entry_offset),                                  \
                        entry_size                                                          \
                    );                                                                      \
                                                                                            \
                entry_offset += entry_size;                                                 \
            }                                                                               \
                                                                                            \
            size = entry_offset;                                                            \
         }

#define  AnscDnsRrSectionGetEntry(header, rs, i, re)                                        \
         {                                                                                  \
            ULONG                   entry_offset = 0;                                       \
            ULONG                   entry_size   = 0;                                       \
            ULONG                   entry_index  = 0;                                       \
                                                                                            \
            for ( entry_index = 0; entry_index < i; entry_index++ )                         \
            {                                                                               \
                AnscDnsRrEntryGetSize                                                       \
                    (                                                                       \
                        header,                                                             \
                        (char*)((ULONG)rs + entry_offset),                                  \
                        entry_size                                                          \
                    );                                                                      \
                                                                                            \
                entry_offset += entry_size;                                                 \
            }                                                                               \
                                                                                            \
            re = (char*)((ULONG)rs + entry_offset);                                         \
         }

#define  AnscDnsRrEntryGetNextEntry(header, re, next_re)                                    \
         {                                                                                  \
            ULONG                   entry_size = 0;                                         \
                                                                                            \
            AnscDnsRrEntryGetSize(header, re, entry_size);                                  \
                                                                                            \
            next_re = (char*)((ULONG)re + entry_size);                                      \
         }

#define  AnscDnsRrEntryGetSize(header, re, size)                                            \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            size  = name_size + sizeof(DNS_RR_ENTRY_FIX);                                   \
            size += AnscDnsRrEntryFixGetRdLength(pRrEntryFix);                              \
         }

#define  AnscDnsRrEntryGetName(header, re, buffer, size)                                    AnscDnsNameRead(header, (char*)re, buffer, size)
#define  AnscDnsRrEntryGetType(header, re, q_type)                                          \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            q_type = AnscDnsRrEntryFixGetType(pRrEntryFix);                                 \
         }
#define  AnscDnsRrEntryGetClass(header, re, q_class)                                        \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            q_class = AnscDnsRrEntryFixGetClass(pRrEntryFix);                               \
         }
#define  AnscDnsRrEntryGetTtl(header, re, q_ttl)                                            \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            q_ttl = AnscDnsRrEntryFixGetTtl(pRrEntryFix);                                   \
         }
#define  AnscDnsRrEntryGetRdLength(header, re, q_length)                                    \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            q_length = AnscDnsRrEntryFixGetRdLength(pRrEntryFix);                           \
         }
#define  AnscDnsRrEntryGetRdata(header, re, q_data)                                         \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            q_data = AnscDnsRrEntryFixGetRdata(pRrEntryFix);                                \
         }
#define  AnscDnsRrEntryGetRdAddr(header, re, q_addr)                                        \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            *(PULONG)q_addr = AnscDnsRrEntryFixGetRdAddrValue(pRrEntryFix);                 \
         }
#define  AnscDnsRrEntryGetRdName(header, re, q_name, q_size)                                \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsNameRead(header, (char*)pRrEntryFix->Rdata, q_name, q_size)              \
         }

#define  AnscDnsRrEntrySetNameString(header, re, buffer, size)                              AnscDnsNameWrite((char*)re, buffer, size)
#define  AnscDnsRrEntrySetNamePointer(header, re, pointer)                                  AnscDnsNameWritePointer((char*)re, pointer)
#define  AnscDnsRrEntrySetType(header, re, q_type)                                          \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetType(pRrEntryFix, q_type);                                  \
         }
#define  AnscDnsRrEntrySetClass(header, re, q_class)                                        \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetClass(pRrEntryFix, q_class);                                \
         }
#define  AnscDnsRrEntrySetTtl(header, re, q_ttl)                                            \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetTtl(pRrEntryFix, q_ttl);                                    \
         }
#define  AnscDnsRrEntrySetRdLength(header, re, q_length)                                    \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetRdLength(pRrEntryFix, q_length);                            \
         }
#define  AnscDnsRrEntrySetRdata(header, re, q_data)                                         \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetRdata(pRrEntryFix, q_data);                                 \
         }
#define  AnscDnsRrEntrySetRdAddr(header, re, q_addr)                                        \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetRdAddrDot(pRrEntryFix, q_addr);                             \
         }
#define  AnscDnsRrEntrySetRdNameString(header, re, q_name, q_size)                          \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsNameWrite  ((char*)pRrEntryFix->Rdata, q_name, q_size);                  \
            AnscDnsNameGetSize((char*)pRrEntryFix->Rdata, name_size);                       \
                                                                                            \
            AnscDnsRrEntryFixSetRdLength(pRrEntryFix, (USHORT)name_size);                   \
         }
#define  AnscDnsRrEntrySetRdNameString2(header, re, q_name, q_size, a_data, a_size)         \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsNameWrite  ((char*)pRrEntryFix->Rdata, q_name, q_size);                  \
            AnscDnsNameGetSize((char*)pRrEntryFix->Rdata, name_size);                       \
                                                                                            \
            AnscCopyMemory((PVOID)((ULONG)pRrEntryFix->Rdata + name_size), a_data, a_size); \
                                                                                            \
            AnscDnsRrEntryFixSetRdLength(pRrEntryFix, (USHORT)(name_size + a_size));        \
         }
#define  AnscDnsRrEntrySetRdNamePointer(header, re, q_pointer)                              \
         {                                                                                  \
            ULONG                   name_size   = 0;                                        \
            PDNS_RR_ENTRY_FIX       pRrEntryFix = NULL;                                     \
                                                                                            \
            AnscDnsNameGetSize((char*)re, name_size);                                       \
                                                                                            \
            pRrEntryFix = (PDNS_RR_ENTRY_FIX)((ULONG)re + name_size);                       \
                                                                                            \
            AnscDnsNameWritePointer((char*)pRrEntryFix->Rdata, q_pointer);                  \
            AnscDnsNameGetSize     ((char*)pRrEntryFix->Rdata, name_size);                  \
                                                                                            \
            AnscDnsRrEntryFixSetRdLength(pRrEntryFix, (USHORT)name_size);                   \
         }


/***********************************************************
                 SOA RDATA RR FORMAT DEFINITION
***********************************************************/

/*
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          /                     MNAME                     /
 *          /                                               /
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          /                     RNAME                     /
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    SERIAL                     |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    REFRESH                    |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                     RETRY                     |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    EXPIRE                     |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          |                    MINIMUM                    |
 *          |                                               |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * where:
 *
 *      MNAME       The <domain-name> of the name server that was the original or primary source
 *                  of data for this zone.
 *      RNAME       A <domain-name> which specifies the mailbox of the person responsible for this
 *                  zone.
 *      SERIAL      The unsigned 32 bit version number of the original copy of the zone. Zone
 *                  transfers preserve this value. This value wraps and should be compared using
 *                  sequence space arithmetic.
 *      REFRESH     A 32 bit time interval before the zone should be refreshed.
 *      RETRY       A 32 bit time interval that should elapse before a failed refresh should be
 *                  retried.
 *      EXPIRE      A 32 bit time value that specifies the upper limit on the time interval that
 *                  can elapse before the zone is no longer authoritative.
 *      MINIMUM     The unsigned 32 bit minimum TTL field that should be exported with any RR from
 *                  this zone.
 */
typedef  struct
_DNS_SOA_RR_DATA_FIX
{
    ULONG                           Serial;
    ULONG                           Refresh;
    ULONG                           Retry;
    ULONG                           Expire;
    ULONG                           Minimum;
}_struct_pack_
DNS_SOA_RR_DATA_FIX,  *PDNS_SOA_RR_DATA_FIX;


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
