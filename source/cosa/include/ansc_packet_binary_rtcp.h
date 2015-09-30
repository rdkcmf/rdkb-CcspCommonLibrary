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

    module:	ansc_packet_binary_rtcp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the RTCP Control Protocol (RTCP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/31/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_RTCP_
#define  _ANSC_PACKET_BINARY_RTCP_


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
          DATA STRUCTURES USED BY GENERIC RTCP PACKET
***********************************************************/

/*
 * The RTP control protocol (RTCP) is based on the periodic transmission of control packets to all
 * participants in the session, using the same distribution mechanism as the data packets. The
 * underlying protocol MUST provide multiplexing of the data and control packets, for example using
 * separate port numbers with UDP. RTCP performs four functions:
 *
 *      - The primary function is to provide feedback on the quality of the data
 *        distribution.
 *      - RTCP carries a persistent transport-level identifier for an RTP source
 *        called the canonical name or CNAME.
 *      - The first two functions require that all participants send RTCP packets,
 *        therefore the rate must be controlled in order for RTP to scale up to a
 *        large number of participants.
 *      - RTCP may also convey minimal session control information, for example
 *        participant identification to be displayed in the user interface.
 */
#define  RTCP_CURRENT_VERSION                       2

#define  RTCP_MASK_VERSION                          0xC0
#define  RTCP_MASK_PADDING                          0x20
#define  RTCP_MASK_RC_COUNT                         0x1F
#define  RTCP_MASK_SC_COUNT                         0x1F

#define  RTCP_PAYLOAD_TYPE_SR                       200
#define  RTCP_PAYLOAD_TYPE_RR                       201
#define  RTCP_PAYLOAD_TYPE_SDES                     202
#define  RTCP_PAYLOAD_TYPE_BYE                      203
#define  RTCP_PAYLOAD_TYPE_APP                      204

typedef  struct
_RTCP_HEADER
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
}_struct_pack_
RTCP_HEADER,  *PRTCP_HEADER;

#define  AnscRtcpHasPadding(p)                      ( ((p->Vprc & RTCP_MASK_PADDING  ) >> 5) != 0     )
#define  AnscRtcpGetVersion(p)                      (  (p->Vprc & RTCP_MASK_VERSION  ) >> 6           )
#define  AnscRtcpGetRcCount(p)                      (   p->Vprc & RTCP_MASK_RC_COUNT                  )
#define  AnscRtcpGetScCount(p)                      (   p->Vprc & RTCP_MASK_SC_COUNT                  )
#define  AnscRtcpGetPayloadType(p)                  (   p->PayloadType                                )
#define  AnscRtcpGetLength(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Length))  )
#define  AnscRtcpGetPacketSize(p)                   ( (AnscRtcpGetLength(p) + 1) * 4                  )
#define  AnscRtcpGetNextPacket(p)                   (PRTCP_HEADER)((ULONG)p + AnscRtcpGetPacketSize(p))

#define  AnscRtcpSetVersion(p, v)                   ( p->Vprc = (p->Vprc & ~RTCP_MASK_VERSION  ) | (UCHAR)(v << 6) )
#define  AnscRtcpSetPadding(p, d)                   ( p->Vprc = (p->Vprc & ~RTCP_MASK_PADDING  ) | (UCHAR)(d << 5) )
#define  AnscRtcpSetRcCount(p, c)                   ( p->Vprc = (p->Vprc & ~RTCP_MASK_RC_COUNT ) | (UCHAR)(c     ) )
#define  AnscRtcpSetScCount(p, c)                   ( p->Vprc = (p->Vprc & ~RTCP_MASK_SC_COUNT ) | (UCHAR)(c     ) )
#define  AnscRtcpSetPayloadType(p, t)               ( p->PayloadType = t                                           )
#define  AnscRtcpSetLength(p, l)                    AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscRtcpSetPacketSize(p, s)                AnscRtcpSetLength(p, (USHORT)((s / 4) - 1)       )

/*
 * RTP receivers provide reception quality feedback using RTCP report packets which may take one of
 * forms depending upon whether or not the receiver is also a sender. The only difference between
 * the sender report (SR) and receiver report (RR) forms, besides the packet type code, is that the
 * sender report includes a 20-byte sender information section for use by active senders. The SR is
 * issued if a site has sent any data packet during the interval since issuing the last report or
 * the previous one, otherwise the RR is issued.
 */
typedef  struct
_RTCP_SENDER_INFO
{
    ULONG                           NtpTimeStamp1;
    ULONG                           NtpTimeStamp2;
    ULONG                           RtpTimeStamp;
    ULONG                           PacketCount;
    ULONG                           OctetCount;
}
RTCP_SENDER_INFO,  *PRTCP_SENDER_INFO;

#define  AnscRtcpSiGetNtpTimeStamp1(p)              ( AnscUlongFromNToH(AnscReadUlong(&p->NtpTimeStamp1)) )
#define  AnscRtcpSiGetNtpTimeStamp2(p)              ( AnscUlongFromNToH(AnscReadUlong(&p->NtpTimeStamp2)) )
#define  AnscRtcpSiGetRtpTimeStamp(p)               ( AnscUlongFromNToH(AnscReadUlong(&p->RtpTimeStamp )) )
#define  AnscRtcpSiGetPacketCount(p)                ( AnscUlongFromNToH(AnscReadUlong(&p->PacketCount  )) )
#define  AnscRtcpSiGetOctetCount(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->OctetCount   )) )

#define  AnscRtcpSiGetNtpTimeStamp32bits(p)         \
         ( ((AnscRtcpSiGetNtpTimeStamp1(p) & 0x0000FFFF) << 16) + ((AnscRtcpSiGetNtpTimeStamp2(p) & 0xFFFF0000) >> 16) )

#define  AnscRtcpSiSetNtpTimeStamp1(p, t)           AnscWriteUlong(&p->NtpTimeStamp1, AnscUlongFromHToN(t))
#define  AnscRtcpSiSetNtpTimeStamp2(p, t)           AnscWriteUlong(&p->NtpTimeStamp2, AnscUlongFromHToN(t))
#define  AnscRtcpSiSetRtpTimeStamp(p, t)            AnscWriteUlong(&p->RtpTimeStamp,  AnscUlongFromHToN(t))
#define  AnscRtcpSiSetPacketCount(p, c)             AnscWriteUlong(&p->PacketCount,   AnscUlongFromHToN(c))
#define  AnscRtcpSiSetOctetCount(p, c)              AnscWriteUlong(&p->OctetCount,    AnscUlongFromHToN(c))

/*
 * The third section contains zero or more reception report blocks depending on the number of
 * other sources heard by this sender since the last report. Each reception report block conveys
 * statistics on the reception of RTP packets from a single synchronization source. Receivers
 * SHOULD NOT carry over statistics when a source changes its SSRC identifier due to a collision.
 */
#define  RTCP_RB_MASK_FRACTION_LOST                 0xFF000000
#define  RTCP_RB_MASK_TOTAL_LOST                    0x00FFFFFF

typedef  struct
_RTCP_REPORT_BLOCK
{
    ULONG                           Ssrc;                       /* source identifier                                 */
    ULONG                           PacketLosts;                /* fraction lost + cumulative number of packets lost */
    ULONG                           HighestSeq;                 /* extended highest sequence number received         */
    ULONG                           Jitter;                     /* interarrival jitter                               */
    ULONG                           Lsr;                        /* last sender report (SR) timestamp                 */
    ULONG                           Dlsr;                       /* delay since last sender report (SR)               */
}
RTCP_REPORT_BLOCK,  *PRTCP_REPORT_BLOCK;

#define  AnscRtcpRbGetSsrc(p)                       ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc       )) )
#define  AnscRtcpRbGetPacketLosts(p)                ( AnscUlongFromNToH(AnscReadUlong(&p->PacketLosts)) )
#define  AnscRtcpRbGetHighestSeq(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->HighestSeq )) )
#define  AnscRtcpRbGetJitter(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Jitter     )) )
#define  AnscRtcpRbGetLsr(p)                        ( AnscUlongFromNToH(AnscReadUlong(&p->Lsr        )) )
#define  AnscRtcpRbGetDlsr(p)                       ( AnscUlongFromNToH(AnscReadUlong(&p->Dlsr       )) )

#define  AnscRtcpRbSetSsrc(p, s)                    AnscWriteUlong(&p->Ssrc,        AnscUlongFromHToN(s))
#define  AnscRtcpRbSetPacketLosts(p, l)             AnscWriteUlong(&p->PacketLosts, AnscUlongFromHToN(l))
#define  AnscRtcpRbSetHighestSeq(p, s)              AnscWriteUlong(&p->HighestSeq,  AnscUlongFromHToN(s))
#define  AnscRtcpRbSetJitter(p, j)                  AnscWriteUlong(&p->Jitter,      AnscUlongFromHToN(j))
#define  AnscRtcpRbSetLsr(p, l)                     AnscWriteUlong(&p->Lsr,         AnscUlongFromHToN(l))
#define  AnscRtcpRbSetDlsr(p, d)                    AnscWriteUlong(&p->Dlsr,        AnscUlongFromHToN(d))

#define  AnscRtcpRbGetFractionLost(p)               ( (AnscRtcpRbGetPacketLosts(p) & RTCP_RB_MASK_FRACTION_LOST) >> 24 )
#define  AnscRtcpRbGetTotalLost(p)                  (  AnscRtcpRbGetPacketLosts(p) & RTCP_RB_MASK_TOTAL_LOST           )

#define  AnscRtcpRbSetFractionLost(p, l)            AnscRtcpRbSetPacketLosts(p, (AnscRtcpRbGetTotalLost(p)           | (l << 24)))
#define  AnscRtcpRbSetTotalLost(p, l)               AnscRtcpRbSetPacketLosts(p, (AnscRtcpRbGetFractionLost(p) << 24) | l         )


/***********************************************************
     DATA STRUCTURES USED BY GENERIC RTCP SENDER REPORT
***********************************************************/

/*
 * The sender report packet consists of three sections, possibly followed by a fourth profile-
 * specific extension section if defined. The first section contains the generic RTCP header
 * plus the source identifier of the packet sender.
 */
typedef  struct
_RTCP_SENDER_REPORT
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
    ULONG                           Ssrc;
    RTCP_SENDER_INFO                SenderInfo;
    RTCP_REPORT_BLOCK               ReportBlock[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RTCP_SENDER_REPORT,  *PRTCP_SENDER_REPORT;

#define  AnscRtcpSrGetSsrc(p)                       ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc)) )
#define  AnscRtcpSrSetSsrc(p, s)                    AnscWriteUlong(&p->Ssrc, AnscUlongFromHToN(s))

#define  AnscRtcpSrGetSenderInfo(p)                 ((PRTCP_SENDER_INFO )&p->SenderInfo    )
#define  AnscRtcpSrGetReportBlock(p, i)             ((PRTCP_REPORT_BLOCK)&p->ReportBlock[i])


/***********************************************************
    DATA STRUCTURES USED BY GENERIC RTCP RECEIVER REPORT
***********************************************************/

/*
 * The format of the receiver report (RR) packet is the same as that of the SR packet except that
 * the packet type field contains the constant 201 and the five words of sender information are
 * omitted (these are the NTP and RTP timestamps and sender's packet and octet counts). The
 * remaining fields have the same meaning as for the SR packet.
 */
typedef  struct
_RTCP_RECEIVER_REPORT
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
    ULONG                           Ssrc;
    RTCP_REPORT_BLOCK               ReportBlock[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RTCP_RECEIVER_REPORT,  *PRTCP_RECEIVER_REPORT;

#define  AnscRtcpRrGetSsrc(p)                       ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc)) )
#define  AnscRtcpRrSetSsrc(p, s)                    AnscWriteUlong(&p->Ssrc, AnscUlongFromHToN(s))

#define  AnscRtcpRrGetReportBlock(p, i)             ((PRTCP_REPORT_BLOCK)&p->ReportBlock[i])


/***********************************************************
       DATA STRUCTURES USED BY RTCP SOURCE DESCRIPTION
***********************************************************/

/*
 * The SDES packet is a three-level structure composed of a header and zero or more chunks, each
 * of which is composed of items describing the source identified in that chunk. Each chunk
 * consists of an SSRC/CSRC identifier followed by a list of zero or more items, which carry
 * information about the SSRC/CSRC. Each chunk starts on a 32-bit boundary.
 */
typedef  struct
_RTCP_SDES_PACKET
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
RTCP_SDES_PACKET,  *PRTCP_SDES_PACKET;

#define  AnscRtcpSdesGetFirstChunk(p)               (PRTCP_SDES_CHUNK)(p->Data)

/*
 * Each chunk consists of an SSRC/CSRC identifier followed by a list of zero or more items, which
 * carry information about the SSRC/CSRC. Each chunk starts on a 32-bit boundary. Each item
 * consists of an 8-bit type field, an 8-bit octet count describing the length of the text (thus,
 * not including this two-octet header), and the text itself.
 */
typedef  struct
_RTCP_SDES_CHUNK
{
    ULONG                           Ssrc;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
RTCP_SDES_CHUNK,  *PRTCP_SDES_CHUNK;

#define  AnscRtcpSdesChunkGetSsrc(p)                ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc)) )
#define  AnscRtcpSdesChunkSetSsrc(p, s)             AnscWriteUlong(&p->Ssrc, AnscUlongFromHToN(s))

#define  AnscRtcpSdesChunkGetFirstItem(p)           (PRTCP_SDES_ITEM)(p->Data)

#define  AnscRtcpSdesChunkGetNullItem(p, ni)                                                \
         {                                                                                  \
            PRTCP_SDES_ITEM         pTempSdesItem = AnscRtcpSdesChunkGetFirstItem(p);       \
                                                                                            \
            while ( AnscRtcpSdesItemGetType(pTempSdesItem) != RTCP_SDES_ITEM_TYPE_NULL )    \
            {                                                                               \
                pTempSdesItem = AnscRtcpSdesItemGetNextItem(pTempSdesItem);                 \
            }                                                                               \
                                                                                            \
            ni = pTempSdesItem;                                                             \
         }

#define  AnscRtcpSdesChunkGetSize(p, size)                                                  \
         {                                                                                  \
            PRTCP_SDES_ITEM         pNullSdesItem = NULL;                                   \
            ULONG                   ulTempOffset  = 0;                                      \
                                                                                            \
            AnscRtcpSdesChunkGetNullItem(p, pNullSdesItem);                                 \
                                                                                            \
            ulTempOffset = (ULONG)pNullSdesItem - (ULONG)p;                                 \
            size         = (ulTempOffset / 4 + 1) * 4;                                      \
         }

#define  AnscRtcpSdesChunkGetNextChunk(p, nc)                                               \
         {                                                                                  \
            ULONG                   ulChunkSize  = 0;                                       \
                                                                                            \
            AnscRtcpSdesChunkGetSize(p, ulChunkSize);                                       \
                                                                                            \
            nc = (PRTCP_SDES_CHUNK)((ULONG)p + ulChunkSize);                                \
         }


/*
 * Items are contiguous, i.e., items are not individually padded to a 32-bit boundary. Text is not
 * null terminated because some multi-octet encodings include null octets. The list items in each
 * chunk MUST be terminated by one or more null octets, the first of which is interpreted as
 * an item type of zero to denote the end of the list. No length octet follows the null item type
 * octet, but additional null octets MUST be included if needed to pad until the next 32-bit
 * boundary.
 */
#define  RTCP_SDES_ITEM_TYPE_NULL                   0
#define  RTCP_SDES_ITEM_TYPE_CNAME                  1
#define  RTCP_SDES_ITEM_TYPE_NAME                   2
#define  RTCP_SDES_ITEM_TYPE_EMAIL                  3
#define  RTCP_SDES_ITEM_TYPE_PHONE                  4
#define  RTCP_SDES_ITEM_TYPE_LOC                    5
#define  RTCP_SDES_ITEM_TYPE_TOOL                   6
#define  RTCP_SDES_ITEM_TYPE_NOTE                   7
#define  RTCP_SDES_ITEM_TYPE_PRIV                   8

typedef  struct
_RTCP_SDES_ITEM
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
RTCP_SDES_ITEM,  *PRTCP_SDES_ITEM;

#define  AnscRtcpSdesItemGetType(p)                 ( p->Type                                               )
#define  AnscRtcpSdesItemGetLength(p)               ( p->Length                                             )
#define  AnscRtcpSdesItemGetData(p)                 ( p->Data                                               )
#define  AnscRtcpSdesItemGetItemSize(p)             ( AnscRtcpSdesItemGetLength(p) + sizeof(RTCP_SDES_ITEM) )
#define  AnscRtcpSdesItemGetSize(p)                 ( (p->Type == 0)? 1 : AnscRtcpSdesItemGetItemSize(p)    )
#define  AnscRtcpSdesItemGetNextItem(p)             (PRTCP_SDES_ITEM)((ULONG)p + AnscRtcpSdesItemGetSize(p) )

#define  AnscRtcpSdesItemSetType(p, t)              ( p->Type   = t        )
#define  AnscRtcpSdesItemSetLength(p, l)            ( p->Length = (UCHAR)l )
#define  AnscRtcpSdesItemSetData(p, d)              AnscCopyMemory(p->Data, d, AnscRtcpSdesItemGetLength(p)         )
#define  AnscRtcpSdesItemSetSize(p, s)              AnscRtcpSdesItemSetLength(p, (UCHAR)(s - sizeof(RTCP_SDES_ITEM)))


/***********************************************************
         DATA STRUCTURES USED BY RTCP GOODBYE PACKET
***********************************************************/

/*
 * When a participant wishes to leave a session, a BYE packet is transmitted to inform the other
 * participants of the event. In order to avoid a flood of BYE packets when many participants leave
 * the system, a participant MUST execute the algorithm as defined in section 6.3.7 in RFC XXXX if
 * the number of members is more than 50 when the participant chooses to leave.
 */
typedef  struct
_RTCP_BYE_PACKET
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
    ULONG                           Ssrc[ANSC_ZERO_ARRAY_SIZE];
}
RTCP_BYE_PACKET,  *PRTCP_BYE_PACKET;

#define  AnscRtcpByeGetSsrc(p, i)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc[i]))                                         )
#define  AnscRtcpByeGetReason(p)                    (PVOID)((ULONG)p + sizeof(RTCP_BYE_PACKET) + sizeof(ULONG) * AnscRtcpHeaderGetScCount(p))
#define  AnscRtcpByeGetReasonLength(p)              ( *(PUCHAR)(AnscRtcpByeGetReason(p)                       )                             )
#define  AnscRtcpByeGetReasonString(p)              (  (char* )((ULONG)AnscRtcpByeGetReason(p) + sizeof(UCHAR))                             )

#define  AnscRtcpByeSetSsrc(p, s, i)                AnscWriteUlong(&p->Ssrc[i], AnscUlongFromHToN(s))
#define  AnscRtcpByeSetReasonLength(p, l)           ( *(PUCHAR)(AnscRtcpByeGetReason(p)) = l        )
#define  AnscRtcpByeSetReasonString(p, s)           AnscCopyString(AnscRtcpByeGetReasonString(p), s )


/***********************************************************
  DATA STRUCTURES USED BY RTCP APPLICATION-DEFINED PACKET
***********************************************************/

/*
 * The APP packet is intended for experimental use as new application and new features are
 * developed, without requiring packet type value registration. APP packets with unrecognized
 * names SHOULD be ignored. After testing and if wider use if justified, it is RECOMMENDED that
 * each APP packet be redefined without the subtype and name fields nad registered with IANA
 * using an RTCP packet type.
 */
#define  RTCP_APP_PACKET_NAME_SIZE                  4

typedef  struct
_RTCP_APP_PACKET
{
    UCHAR                           Vprc;
    UCHAR                           PayloadType;
    USHORT                          Length;
    ULONG                           Ssrc;
    char                            Name[RTCP_APP_PACKET_NAME_SIZE];
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
RTCP_APP_PACKET,  *PRTCP_APP_PACKET;

#define  AnscRtcpAppSubType(p)                      AnscRtcpGetRcCount(p)
#define  AnscRtcpAppGetSsrc(p)                      ( AnscUlongFromNToH(AnscReadUlong(&p->Ssrc)) )
#define  AnscRtcpAppGetName(p)                      ( p->Name                                    )
#define  AnscRtcpAppGetData(p)                      ( p->Data                                    )

#define  AnscRtcpAppSetSsrc(p, s)                   AnscWriteUlong(&p->Ssrc, AnscUlongFromHToN(s)       )
#define  AnscRtcpAppSetName(p, s)                   AnscCopyMemory(p->Name, s, RTCP_APP_PACKET_NAME_SIZE)
#define  AnscRtcpAppSetData(p, d, s)                AnscCopyMemory(p->Data, d, s                        )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
