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

    module:	ansc_packet_binary_rtp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Real-time Transport Protocol (RTP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/30/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_RTP_
#define  _ANSC_PACKET_BINARY_RTP_


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
                 DATA STRUCTURES USED BY RTP
***********************************************************/

/*
 * Real-time Transport Protocol (RTP) provides end-to-end delivery services for data with real-time
 * characteristics, such as interactive audio and video. Thoese services include payload type
 * identification, sequence numbering, timestamping and delivery monitoring. Applications typically
 * run RTP on top of UDP to make use of its multiplexing and checksum services; both protocols
 * contribute parts of the transport functionality. However, RTP may be used with other suitable
 * underlying network or transport protocols. RTP supports data transfer to multiple destination
 * using multicast distribution if provided by the underlying network.
 */
#define  RTP_CURRENT_VERSION                        2

#define  RTP_MASK_VERSION                           0xC0
#define  RTP_MASK_PADDING                           0x20
#define  RTP_MASK_EXTENSION                         0x10
#define  RTP_MASK_CSRC_COUNT                        0x0F
#define  RTP_MASK_MARKER                            0x80
#define  RTP_MASK_PAYLOAD_TYPE                      0x7F

typedef  struct
_RTP_HEADER
{
    UCHAR                           Vpxcc;
    UCHAR                           PayloadType;
    USHORT                          SeqNumber;
    ULONG                           TimeStamp;
    ULONG                           Ssrc;
    ULONG                           Csrc[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RTP_HEADER,  *PRTP_HEADER;

#define  AnscRtpHasPadding(p)                       ( ((p->Vpxcc       & RTP_MASK_PADDING  ) >> 5) != 0 )
#define  AnscRtpHasExtension(p)                     ( ((p->Vpxcc       & RTP_MASK_EXTENSION) >> 4) != 0 )
#define  AnscRtpHasMarker(p)                        ( ((p->PayloadType & RTP_MASK_MARKER   ) >> 7) != 0 )

#define  AnscRtpGetVersion(p)                       (  (p->Vpxcc       & RTP_MASK_VERSION  ) >> 6       )
#define  AnscRtpGetCsrcCount(p)                     (   p->Vpxcc       & RTP_MASK_CSRC_COUNT            )
#define  AnscRtpGetPayloadType(p)                   (   p->PayloadType & RTP_MASK_PAYLOAD_TYPE          )
#define  AnscRtpGetSeqNumber(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->SeqNumber)) )
#define  AnscRtpGetTimeStamp(p)                     ( AnscUlongFromNToH (AnscReadUlong (&p->TimeStamp)) )
#define  AnscRtpGetSsrc(p)                          ( AnscUlongFromNToH (AnscReadUlong (&p->Ssrc     )) )
#define  AnscRtpGetCsrc(p, i)                       ( AnscUlongFromNToH (AnscReadUlong (&p->Csrc[i]  )) )

#define  AnscRtpSetVersion(p, v)                    ( p->Vpxcc       = (p->Vpxcc       & ~RTP_MASK_VERSION     ) | (UCHAR)(v << 6) )
#define  AnscRtpSetPadding(p, d)                    ( p->Vpxcc       = (p->Vpxcc       & ~RTP_MASK_PADDING     ) | (UCHAR)(d << 5) )
#define  AnscRtpSetExtension(p, x)                  ( p->Vpxcc       = (p->Vpxcc       & ~RTP_MASK_EXTENSION   ) | (UCHAR)(x << 4) )
#define  AnscRtpSetCsrcCount(p, c)                  ( p->Vpxcc       = (p->Vpxcc       & ~RTP_MASK_CSRC_COUNT  ) | (UCHAR)(c     ) )
#define  AnscRtpSetMarker(p, m)                     ( p->PayloadType = (p->PayloadType & ~RTP_MASK_MARKER      ) | (UCHAR)(m << 7) )
#define  AnscRtpSetPayloadType(p, t)                ( p->PayloadType = (p->PayloadType & ~RTP_MASK_PAYLOAD_TYPE) | (UCHAR)(t     ) )
#define  AnscRtpSetSeqNumber(p, n)                  AnscWriteUshort(&p->SeqNumber, AnscUshortFromHToN(n))
#define  AnscRtpSetTimeStamp(p, t)                  AnscWriteUlong (&p->TimeStamp, AnscUlongFromHToN (t))
#define  AnscRtpSetSsrc(p, s)                       AnscWriteUlong (&p->Ssrc,      AnscUlongFromHToN (s))
#define  AnscRtpSetCsrc(p, s, i)                    AnscWriteUlong (&p->Csrc[i],   AnscUlongFromHToN (s))

#define  AnscRtpGetFixedHeaderSize(p)               ( sizeof(RTP_HEADER) + AnscRtpGetCsrcCount(p) * 4                                   )
#define  AnscRtpGetHeaderExtension(p)               ( (PRTP_HEADER_EXTENSION)((ULONG)p + AnscRtpGetFixedHeaderSize(p))                  )
#define  AnscRtpGetExtensionSize(p)                 ( AnscRtpHasExtension(p)? AnscRtpExtensionGetSize(AnscRtpGetHeaderExtension(p)) : 0 )
#define  AnscRtpGetHeaderSize(p)                    ( AnscRtpGetFixedHeaderSize(p) + AnscRtpGetExtensionSize(p)                         )
#define  AnscRtpGetPayload(p)                       ( (PVOID)((ULONG)p + AnscRtpGetHeaderSize(p))                                       )

#define  AnscRtpGetPaddingSize(p, s)                ( AnscRtpHasPadding(p)? (ULONG)*(PUCHAR*)((ULONG)p + s - 1) : 0 )
#define  AnscRtpGetPayloadSize(p, s)                ( s - AnscRtpGetHeaderSize(p) - AnscRtpGetPaddingSize(p, s)     )

/*
 * An extension mechanism is provided to allow individual implementations to experiment with new
 * payload-format-independent functions that require additional information to be carried in the
 * RTP data packet header. This mechanism is designed so that the header extension may be ignored
 * by other interoperating implementations that have not been extended.
 */
typedef  struct
_RTP_HEADER_EXTENSION
{
    USHORT                          Dbp;                        /* defined by profile                      */
    USHORT                          Length;                     /* number of 32-bits word in the extension */
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RTP_HEADER_EXTENSION,  *PRTP_HEADER_EXTENSION;

#define  AnscRtpExtensionGetDbp(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->Dbp   ))                  )
#define  AnscRtpExtensionGetLength(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->Length))                  )
#define  AnscRtpExtensionGetData(p)                 ( p->Data                                                         )
#define  AnscRtpExtensionGetSize(p)                 ( sizeof(RTP_HEADER_EXTENSION) + AnscRtpExtensionGetLength(p) * 4 )

#define  AnscRtpExtensionSetDbp(p, d)               AnscWriteUshort(&p->Dbp,    AnscUshortFromHToN(d)                 )
#define  AnscRtpExtensionSetLength(p, l)            AnscWriteUshort(&p->Length, AnscUshortFromHToN(l)                 )
#define  AnscRtpExtensionSetData(p, d)              AnscCopyMemory(p->Data, d,  AnscRtpExtensionGetLength(p) * 4      )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
