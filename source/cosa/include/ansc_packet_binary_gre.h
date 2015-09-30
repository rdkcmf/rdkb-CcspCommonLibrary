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

    module:	ansc_packet_binary_gre.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the General Routing Encapsulation
        Protocol (GRE).

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


#ifndef  _ANSC_PACKET_BINARY_GRE_
#define  _ANSC_PACKET_BINARY_GRE_


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
                 DATA STRUCTURES USED BY GRE
***********************************************************/

/*
 * We shall maintain a mapping list between the port number and the popular application protocols, and
 * make this list upgradable from the management server. But we use the following static definition
 * temporarily.
 */
#define  GRE_PROTOCOL_IP                            0x0800
#define  GRE_PROTOCOL_PPTP                          0x880B

/*
 * Define some const values used in the GRE header definition
 */
#define  GRE_CTRL_CHECKSUM                          0x80
#define  GRE_CTRL_ROUTING                           0x40
#define  GRE_CTRL_KEY                               0x20
#define  GRE_CTRL_SEQ                               0x10
#define  GRE_CTRL_SSROUTE                           0x08

#define  GRE_FLAG_ACK                               0x80
#define  GRE_VER_MASK                               0x07

/*
 * In the most general case, a system has a packet that needs to be encapsulated and routed. We
 * will call this the payload packet. The payload is first encapsulated in a GRE packet, which
 * possibly also includes a route.  The resulting GRE packet can then be encapsulated in some
 * other protocol and then forwarded.
 */
typedef  struct
_GRE_HEADER
{
    UCHAR                           ControlBits;
    UCHAR                           Flags;
    USHORT                          ProtocolType;
}_struct_pack_
GRE_HEADER,  *PGRE_HEADER;

#define  GRE_PPTP_CONTROL_BITS                      0x20
#define  GRE_PPTP_FLAGS                             0x01

typedef  struct
_GRE_PPTP_HEADER
{
    UCHAR                           ControlBits;
    UCHAR                           Flags;
    USHORT                          ProtocolType;
    USHORT                          PayloadLength;
    USHORT                          CallId;
    ULONG                           SeqAck[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
GRE_PPTP_HEADER,  *PGRE_PPTP_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscGreGetControlBits(p)                   ( p->ControlBits )
#define  AnscGreGetFlags(p)                         ( p->Flags )
#define  AnscGreGetProtocolType(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->ProtocolType)) )
#define  AnscGreIsChecksum(p)                       ( (AnscGreGetControlBits(p) & GRE_CTRL_CHECKSUM) != 0 )
#define  AnscGreIsRouting(p)                        ( (AnscGreGetControlBits(p) & GRE_CTRL_ROUTING)  != 0 )
#define  AnscGreIsKey(p)                            ( (AnscGreGetControlBits(p) & GRE_CTRL_KEY)      != 0 )
#define  AnscGreIsSeq(p)                            ( (AnscGreGetControlBits(p) & GRE_CTRL_SEQ)      != 0 )
#define  AnscGreIsSsroute(p)                        ( (AnscGreGetControlBits(p) & GRE_CTRL_SSROUTE)  != 0 )
#define  AnscGreIsEnhanced(p)                       ( (AnscGreGetFlags(p)       & GRE_VER_MASK)      == 1 )

#define  AnscGreSetControlBits(p, b)                ( p->ControlBits = b )
#define  AnscGreSetFlags(p, f)                      ( p->Flags       = f )
#define  AnscGreSetProtocolType(p, type)            AnscWriteUshort(&p->ProtocolType, AnscUshortFromHToN(type))

#define  AnscGreIsPptp(p)                           \
         ( (AnscGreGetProtocolType(p) == GRE_PROTOCOL_PPTP) && !AnscGreIsChecksum(p) && !AnscGreIsRouting(p) && AnscGreIsKey(p) && !AnscGreIsSsroute(p) && AnscGreIsEnhanced(p) )
#define  AnscGrePptpGetControlBits(p)               AnscGreGetControlBits(p)
#define  AnscGrePptpGetFlags(p)                     AnscGreGetFlags(p)
#define  AnscGrePptpGetProtocolType(p)              AnscGreGetProtocolType(p)
#define  AnscGrePptpIsSeq(p)                        ( (AnscGreGetControlBits(p) & GRE_CTRL_SEQ) != 0 )
#define  AnscGrePptpIsAck(p)                        ( (AnscGreGetFlags(p)       & GRE_FLAG_ACK) != 0 )
#define  AnscGrePptpGetHeaderSize(p)                \
         ( sizeof(GRE_PPTP_HEADER) + (ULONG)AnscGrePptpIsSeq(p) * sizeof(ULONG) + (ULONG)AnscGrePptpIsAck(p) * sizeof(ULONG) )
#define  AnscGrePptpGetPayload(p)                   ( (PVOID)((ULONG)p + AnscGrePptpGetHeaderSize(p)) )
#define  AnscGrePptpGetPayloadLength(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->PayloadLength)) )
#define  AnscGrePptpGetCallId(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->CallId       )) )
#define  AnscGrePptpGetSeqAck0(p)                   ( AnscUlongFromNToH (AnscReadUlong (&p->SeqAck[0]    )) )
#define  AnscGrePptpGetSeqAck1(p)                   ( AnscUlongFromNToH (AnscReadUlong (&p->SeqAck[1]    )) )
#define  AnscGrePptpGetSeq(p)                       ( AnscGrePptpGetSeqAck0(p) )
#define  AnscGrePptpGetAck(p)                       ( AnscGrePptpIsSeq(p)? AnscGrePptpGetSeqAck1(p) : AnscGrePptpGetSeqAck0(p) )

#define  AnscGrePptpSetControlBits(p, b)            AnscGreSetControlBits(p, b)
#define  AnscGrePptpSetFlags(p, f)                  AnscGreSetFlags(p, f)
#define  AnscGrePptpSetProtocolType(p, type)        AnscGreSetProtocolType(p, type)
#define  AnscGrePptpSetPayloadLength(p, l)          AnscWriteUshort(&p->PayloadLength, AnscUshortFromHToN(l ))
#define  AnscGrePptpSetCallId(p, id)                AnscWriteUshort(&p->CallId,        AnscUshortFromHToN(id))
#define  AnscGrePptpSetSeqAck0(p, n)                AnscWriteUlong (&p->SeqAck[0],     AnscUlongFromHToN (n ))
#define  AnscGrePptpSetSeqAck1(p, n)                AnscWriteUlong (&p->SeqAck[1],     AnscUlongFromHToN (n ))
#define  AnscGrePptpSetSeq(p, n)                    AnscGrePptpSetSeqAck0(p, n)
#define  AnscGrePptpSetAck(p, n)                    \
         {                                          \
            if ( AnscGrePptpIsSeq(p) )              \
            {                                       \
                AnscGrePptpSetSeqAck1(p, n);        \
            }                                       \
            else                                    \
            {                                       \
                AnscGrePptpSetSeqAck0(p, n);        \
            }                                       \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
