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

    module:	ansc_packet_binary_ppp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Point-to-Point Protocol (PPP).

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


#ifndef  _ANSC_PACKET_BINARY_PPP_
#define  _ANSC_PACKET_BINARY_PPP_


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
    DATA STRUCTURES USED BY PPP-RELATED PACKET OPERATION
***********************************************************/

/*
 * PPP uses the principles described in ISO 3309-1979 HDLC frame structure, most recently the
 * fourth edition 3309:1991 [2], which specifies modifications to allow HDLC use in asynchronous
 * environments.
 */
#define  HDLC_FRAME_ADDRESS                         0xFF
#define  HDLC_FRAME_CONTROL                         0x03

typedef  struct
_HDLC_FRAME_HEADER
{
    UCHAR                           Address;
    UCHAR                           Control;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
HDLC_FRAME_HEADER,  *PHDLC_FRAME_HEADER;

#define  AnscHdlcGetAddress(p)                      ( p->Address )
#define  AnscHdlcGetControl(p)                      ( p->Control )
#define  AnscHdlcGetInfo(p)                         ( p->Info )

#define  AnscHdlcSetAddress(p, addr)                ( p->Address = addr )
#define  AnscHdlcSetControl(p, c)                   ( p->Control = c    )
#define  AnscHdlcSetInfo(p, i, l)                   AnscCopyMemory(p->Info, i, l)

#define  AnscCreateHdlcFrame(p)                     \
         AnscHdlcSetAddress(p, HDLC_FRAME_ADDRESS); AnscHdlcSetControl(p, HDLC_FRAME_CONTROL)
#define  AnscRemoveHdlcFrame(p)                     ( (PVOID)((ULONG)p + sizeof(HDLC_FRAME_HEADER) )

/*
 * The PPP encapsulation is used to disambiguate multiprotocol datagrams. This encapsulation
 * requires framing to indicate the begining and end of the encapsulation.
 */
typedef  struct
_PPP_HEADER
{
    USHORT                          ProtocolField;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_HEADER,  *PPPP_HEADER;

#define  AnscPppGetProtocolField(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->ProtocolField)) )
#define  AnscPppGetInfo(p)                          ( p->Info )

#define  AnscPppSetProtocolField(p, f)              AnscWriteUshort(&p->ProtocolField, AnscUshortFromHToN(f))
#define  AnscPppSetInfo(p, i, l)                    AnscCopyMemory(p->Info, i, l)

#define  AnscPppGetProtocolHeader(p)                ( (PVOID)((ULONG)p + sizeof(PPP_HEADER)) )
#define  AnscPppIsNlpPacket(p)                      \
         ( (AnscPppGetProtocolField(p) > 0x0000) && (AnscPppGetProtocolField(p) < 0x4000) )
#define  AnscPppIsLvtPacket(p)                      \
         ( (AnscPppGetProtocolField(p) > 0x4000) && (AnscPppGetProtocolField(p) < 0x8000) )
#define  AnscPppIsNcpPacket(p)                      \
         ( (AnscPppGetProtocolField(p) > 0x8000) && (AnscPppGetProtocolField(p) < 0xC000) )
#define  AnscPppIsLcpPacket(p)                      \
         ( (AnscPppGetProtocolField(p) > 0xC000) && (AnscPppGetProtocolField(p) < 0xFFFF) )

/*
 * Define the data structure for Point-to-Point (PPP) Link Control protocol (LCP)
 */
typedef  struct
_PPP_LCP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_LCP_HEADER,  *PPPP_LCP_HEADER;

#define  AnscPppLcpGetCode(p)                       ( p->Code )
#define  AnscPppLcpGetIdentifier(p)                 ( p->Identifier )
#define  AnscPppLcpGetLength(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppLcpGetDataLength(p)                 ( AnscPppLcpGetLength(p) - sizeof(PPP_LCP_HEADER) )

#define  AnscPppLcpSetCode(p, c)                    ( p->Code       = c  )
#define  AnscPppLcpSetIdentifier(p, id)             ( p->Identifier = id )
#define  AnscPppLcpSetLength(p, l)                  AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscPppLcpSetInfo(p, i, l)                 AnscCopyMemory(p->Info, i, l)

typedef  struct  _PPP_LCP_HEADER  PPP_LCP_CFG_REQUEST,  *PPPP_LCP_CFG_REQUEST;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_CFG_ACK,      *PPPP_LCP_CFG_ACK;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_CFG_NAK,      *PPPP_LCP_CFG_NAK;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_CFG_REJECT,   *PPPP_LCP_CFG_REJECT;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_TMN_REQUEST,  *PPPP_LCP_TMN_REQUEST;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_TMN_ACK,      *PPPP_LCP_TMN_ACK;
typedef  struct  _PPP_LCP_HEADER  PPP_LCP_CODE_REJECT,  *PPPP_LCP_CODE_REJECT;

typedef  struct
_PPP_LCP_PROTOCOL_REJECT
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    USHORT                          Protocol;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_LCP_PROTOCOL_REJECT,  *PPPP_LCP_PROTOCOL_REJECT;

#define  AnscPppLcpProRejectGetProtocol(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->Protocol)) )
#define  AnscPppLcpProRejectSetProtocol(p, f)       AnscWriteUshort(&p->Protocol, AnscUshortFromHToN(f))

typedef  struct
_PPP_LCP_ECHO_REQUEST
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    ULONG                           MagicNumber;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_LCP_ECHO_REQUEST,  *PPPP_LCP_ECHO_REQUEST;

#define  AnscPppLcpEchoRequestGetMagicNumber(p)     ( AnscUlongFromNToH(AnscReadUlong(&p->MagicNumber)) )
#define  AnscPppLcpEchoRequestSetMagicNumber(p, n)  AnscWriteUlong(&p->MagicNumber, AnscUlongFromHToN(n))

#define  AnscPppLcpEchoRequestGetInfo(p)            ( p->Info )
#define  AnscPppLcpEchoRequestGetInfoSize(p)        ( AnscPppLcpGetLength(p) - sizeof(PPP_LCP_ECHO_REQUEST) )
#define  AnscPppLcpEchoRequestSetInfo(p, info)      AnscCopyMemory(p->Info, info, AnscPppLcpEchoRequestGetInfoSize(p))

typedef  struct  _PPP_LCP_ECHO_REQUEST  PPP_LCP_ECHO_REPLY,       *PPPP_LCP_ECHO_REPLY;
typedef  struct  _PPP_LCP_ECHO_REQUEST  PPP_LCP_DISCARD_REQUEST,  *PPPP_LCP_DISCARD_REQUEST;

#define  AnscPppLcpEchoReplyGetMagicNumber(p)       AnscPppLcpEchoRequestGetMagicNumber(p)
#define  AnscPppLcpEchoReplySetMagicNumber(p, n)    AnscPppLcpEchoRequestSetMagicNumber(p, n)

#define  AnscPppLcpEchoReplyGetInfo(p)              AnscPppLcpEchoRequestGetInfo(p)
#define  AnscPppLcpEchoReplyGetInfoSize(p)          AnscPppLcpEchoRequestGetInfoSize(p)
#define  AnscPppLcpEchoReplySetInfo(p, info)        AnscPppLcpEchoRequestSetInfo(p, info)

/*
 * LCP Configuration Options allow negotiation of modifications to the default characteristics of
 * a point-to-point link. If a Configuration Option is not included in a Configuration-Request
 * packet, the default value for that Configuration Option is assumed.
 */
typedef  struct
_PPP_LCP_CFG_OPTION
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_LCP_CFG_OPTION,  *PPPP_LCP_CFG_OPTION;

#define  AnscPppLcpCfgGetFirstOption(p)             ( (PVOID)((ULONG)p + sizeof(PPP_LCP_HEADER)) )

#define  AnscPppLcpCfgOptionGetType(p)              ( p->Type   )
#define  AnscPppLcpCfgOptionGetLength(p)            ( p->Length )
#define  AnscPppLcpCfgOptionGetData(p)              ( p->Data   )

#define  AnscPppLcpCfgOptionSetType(p, t)           ( p->Type   = t )
#define  AnscPppLcpCfgOptionSetLength(p, l)         ( p->Length = l )
#define  AnscPppLcpCfgOptionSetData(p, d, l)        AnscCopyMemory(p->Data, d, l)

#define  AnscPppLcpCfgOptionGetDataLength(p)        ( AnscPppLcpCfgOptionGetLength(p) - (UCHAR)sizeof(PPP_LCP_CFG_OPTION) )
#define  AnscPppLcpCfgOptionGetNextOption(p)        ( (PVOID)((ULONG)p + (ULONG)AnscPppLcpCfgOptionGetLength(p)) )

typedef  struct
_PPP_LCP_OPTION_MRU
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          MaxReceiveUnit;
}_struct_pack_
PPP_LCP_OPTION_MRU,  *PPPP_LCP_OPTION_MRU;

#define  AnscPppLcpOptionMruGetMru(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->MaxReceiveUnit)) )
#define  AnscPppLcpOptionMruSetMru(p, n)            AnscWriteUshort(&p->MaxReceiveUnit, AnscUshortFromHToN(n))

typedef  struct
_PPP_LCP_OPTION_AP
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          Protocol;
}_struct_pack_
PPP_LCP_OPTION_AP,  *PPPP_LCP_OPTION_AP;

#define  AnscPppLcpOptionApGetProtocol(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->Protocol)) )
#define  AnscPppLcpOptionApSetProtocol(p, f)        AnscWriteUshort(&p->Protocol, AnscUshortFromHToN(f))

typedef  struct  _PPP_LCP_OPTION_AP  PPP_LCP_OPTION_QP,  *PPPP_LCP_OPTION_QP;

#define  AnscPppLcpOptionQpGetProtocol(p)           AnscPppLcpOptionApGetProtocol(p)
#define  AnscPppLcpOptionQpSetProtocol(p, f)        AnscPppLcpOptionApSetProtocol(p, f)

typedef  struct
_PPP_LCP_OPTION_AP_CHAP
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          Protocol;
    UCHAR                           Algorithm;
}_struct_pack_
PPP_LCP_OPTION_AP_CHAP,  *PPPP_LCP_OPTION_AP_CHAP;

#define  AnscPppLcpOptionApChapGetProtocol(p)       ( AnscUshortFromNToH(AnscReadUshort(&p->Protocol )) )
#define  AnscPppLcpOptionApChapGetAlgorithm(p)      ( p->Algorithm )

#define  AnscPppLcpOptionApChapSetProtocol(p, f)    AnscWriteUshort(&p->Protocol, AnscUshortFromHToN(f))
#define  AnscPppLcpOptionApChapSetAlgorithm(p, a)   ( p->Algorithm = a )

typedef  struct
_PPP_LCP_OPTION_MN
{
    UCHAR                           Type;
    UCHAR                           Length;
    ULONG                           MagicNumber;
}_struct_pack_
PPP_LCP_OPTION_MN,  *PPPP_LCP_OPTION_MN;

#define  AnscPppLcpOptionMnGetMagicNumber(p)        ( AnscUlongFromNToH(AnscReadUlong(&p->MagicNumber)) )
#define  AnscPppLcpOptionMnSetMagicNumber(p, n)     AnscWriteUlong(&p->MagicNumber, AnscUlongFromHToN(n))

typedef  struct  _PPP_LCP_CFG_OPTION  PPP_LCP_OPTION_PFC,   *PPPP_LCP_OPTION_PFC;
typedef  struct  _PPP_LCP_CFG_OPTION  PPP_LCP_OPTION_ACFC,  *PPPP_LCP_OPTION_ACFC;


/***********************************************************
      DATA STRUCTURES USED BY PPP PAP PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Point-to-Point (PPP) Password Authentication Protocol
 */
typedef  struct
_PPP_PAP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_PAP_HEADER,  *PPPP_PAP_HEADER;

#define  AnscPppPapGetCode(p)                       ( p->Code )
#define  AnscPppPapGetIdentifier(p)                 ( p->Identifier )
#define  AnscPppPapGetLength(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppPapGetDataLength(p)                 ( (AnscPppPapGetLength(p)) - sizeof(PPP_PAP_HEADER) )

#define  AnscPppPapSetCode(p, c)                    ( p->Code       = c  )
#define  AnscPppPapSetIdentifier(p, id)             ( p->Identifier = id )
#define  AnscPppPapSetLength(p, l)                  AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))

/*
 * The Authenticate-Request packet is used to begin the Password Authentication Protocol. The link
 * peer MUST transmit a PAP packet with the Code field set to 1 (Authenticate-Request) during the
 * Authentication Phase. The Authenticate-Request MUST be repeated until a valid reply packet is
 * received, or an optional retry counter expires.
 */
typedef  struct
_PPP_PAP_REQUEST
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           PeerIdLength;
    UCHAR                           PeerId[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_PAP_REQUEST,  *PPPP_PAP_REQUEST;

#define  AnscPppPapArGetCode(p)                     AnscPppPapGetCode(p)
#define  AnscPppPapArGetIdentifier(p)               AnscPppPapGetIdentifier(p)
#define  AnscPppPapArGetLength(p)                   AnscPppPapGetLength(p)

#define  AnscPppPapArSetCode(p, c)                  AnscPppPapSetCode(p, c)
#define  AnscPppPapArSetIdentifier(p, id)           AnscPppPapSetIdentifier(p, id)
#define  AnscPppPapArSetLength(p, l)                AnscPppPapSetLength(p, l)

#define  AnscPppPapArGetPeerIdLength(p)             ( p->PeerIdLength                                   )
#define  AnscPppPapArGetPasswdLength(p)             ( *(PUCHAR)((ULONG)p->PeerId + p->PeerIdLength)     )
#define  AnscPppPapArSetPeerIdLength(p, l)          ( p->PeerIdLength                               = (UCHAR)l )
#define  AnscPppPapArSetPasswdLength(p, l)          ( *(PUCHAR)((ULONG)p->PeerId + p->PeerIdLength) = (UCHAR)l )

#define  AnscPppPapArGetPeerId(p)                   ( p->PeerId                                                   )
#define  AnscPppPapArGetPasswd(p)                   ( (PVOID)((ULONG)p->PeerId + p->PeerIdLength + sizeof(UCHAR)) )
#define  AnscPppPapArSetPeerId(p, id)               AnscCopyMemory(AnscPppPapArGetPeerId(p), id, AnscPppPapArGetPeerIdLength(p))
#define  AnscPppPapArSetPasswd(p, pw)               AnscCopyMemory(AnscPppPapArGetPasswd(p), pw, AnscPppPapArGetPasswdLength(p))

/*
 * If the Peer-ID/Password pair received in an Authenticate-Request is both recognizable and
 * acceptable, then the Authenticator MUST transmit a PAP packet with the Code field set to 2
 * (Authenticate-Ack). Otherwise, a Authenticate-Nak must be transmited, and the Auenticator
 * SHOULD take action to terminate the link.
 */
typedef  struct
_PPP_PAP_ACK
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           MessageLength;
    char                            Message[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_PAP_ACK,  *PPPP_PAP_ACK;

typedef  struct  _PPP_PAP_ACK  PPP_PAP_NAK,  *PPPP_PAP_NAK;

#define  AnscPppPapSfGetCode(p)                     AnscPppPapGetCode(p)
#define  AnscPppPapSfGetIdentifier(p)               AnscPppPapGetIdentifier(p)
#define  AnscPppPapSfGetLength(p)                   AnscPppPapGetLength(p)

#define  AnscPppPapSfSetCode(p, c)                  AnscPppPapSetCode(p, c)
#define  AnscPppPapSfSetIdentifier(p, id)           AnscPppPapSetIdentifier(p, id)
#define  AnscPppPapSfSetLength(p, l)                AnscPppPapSetLength(p, l)

#define  AnscPppPapSfGetMessageSize(p)              ( p->MessageLength )
#define  AnscPppPapSfGetMessage(p)                  ( p->Message       )

#define  AnscPppPapSfSetMessageSize(p, s)           ( p->MessageLength = (UCHAR)s )
#define  AnscPppPapSfSetMessage(p, s)               AnscCopyString((char*)AnscPppPapSfGetMessage(p), s)


/***********************************************************
      DATA STRUCTURES USED BY PPP CHAP PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Point-to-Point (PPP) Challenge Handshake Authentication Protocol
 */
typedef  struct
_PPP_CHAP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_CHAP_HEADER,  *PPPP_CHAP_HEADER;

#define  AnscPppChapGetCode(p)                      ( p->Code )
#define  AnscPppChapGetIdentifier(p)                ( p->Identifier )
#define  AnscPppChapGetLength(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppChapGetDataLength(p)                ( (AnscPppChapGetLength(p)) - sizeof(PPP_CHAP_HEADER) )

#define  AnscPppChapSetCode(p, c)                   ( p->Code       = c  )
#define  AnscPppChapSetIdentifier(p, id)            ( p->Identifier = id )
#define  AnscPppChapSetLength(p, l)                 AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))

/*
 * The Challenge packet is used to begin the Challenge-Handshake Authentication Protocol. The
 * authenticator MUST transmit a CHAP packet with the Code field set to 1 (Challenge). Additional
 * Challenge packets MUST be sent until a valid Response packet is received, or and  optional
 * retry counter expires.
 */
typedef  struct
_PPP_CHAP_CHALLENGE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           ValueSize;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_CHAP_CHALLENGE,  *PPPP_CHAP_CHALLENGE;

typedef  struct  _PPP_CHAP_CHALLENGE  PPP_CHAP_RESPONSE,  *PPPP_CHAP_RESPONSE;

#define  AnscPppChapCrGetCode(p)                    AnscPppChapGetCode(p)
#define  AnscPppChapCrGetIdentifier(p)              AnscPppChapGetIdentifier(p)
#define  AnscPppChapCrGetLength(p)                  AnscPppChapGetLength(p)

#define  AnscPppChapCrSetCode(p, c)                 AnscPppChapSetCode(p, c)
#define  AnscPppChapCrSetIdentifier(p, id)          AnscPppChapSetIdentifier(p, id)
#define  AnscPppChapCrSetLength(p, l)               AnscPppChapSetLength(p, l)

#define  AnscPppChapCrGetValueSize(p)               ( p->ValueSize     )
#define  AnscPppChapCrSetValueSize(p, s)            ( p->ValueSize = s )

#define  AnscPppChapCrGetChallengeSize(p)           ( (ULONG)AnscPppChapCrGetValueSize(p) )
#define  AnscPppChapCrGetResponseSize(p)            ( (ULONG)AnscPppChapCrGetValueSize(p) )
#define  AnscPppChapCrGetChallenge(p)               ( (PVOID)((ULONG)p + sizeof(PPP_CHAP_CHALLENGE)) )
#define  AnscPppChapCrGetResponse(p)                ( (PVOID)((ULONG)p + sizeof(PPP_CHAP_RESPONSE )) )
#define  AnscPppChapCrGetName(p)                    ( (PVOID)((ULONG)AnscPppChapCrGetChallenge(p) + AnscPppChapCrGetChallengeSize(p)) )
#define  AnscPppChapCrGetNameSize(p)                ( (ULONG)AnscPppChapCrGetLength(p) - sizeof(PPP_CHAP_CHALLENGE) - AnscPppChapCrGetChallengeSize(p) )

#define  AnscPppChapCrSetChallengeSize(p, s)        AnscPppChapCrSetValueSize(p, (UCHAR)s)
#define  AnscPppChapCrSetResponseSize(p, s)         AnscPppChapCrSetValueSize(p, (UCHAR)s)
#define  AnscPppChapCrSetChallenge(p, c)            AnscCopyMemory(p->Value, c, AnscPppChapCrGetChallengeSize(p))
#define  AnscPppChapCrSetResponse(p, r)             AnscCopyMemory(p->Value, r, AnscPppChapCrGetResponseSize (p))
#define  AnscPppChapCrSetName(p, s)                 AnscCopyString((char*)AnscPppChapCrGetName(p), s)

/*
 * If the Value received in a Response is equal to the expected value, then the implementation
 * MUST transmit a CHAP packet with the code field set to 3 (Success). If the Value received in
 * a Response is not equal to the expected value, then the implementation MUST transmit a CHAP
 * packet with the Code field set to 4 (field), and SHOULD take action to terminate the link.
 */
typedef  struct
_PPP_CHAP_SUCCESS
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    char                            Message[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_CHAP_SUCCESS,  *PPPP_CHAP_SUCCESS;

typedef  struct  _PPP_CHAP_SUCCESS  PPP_CHAP_FAILURE,  *PPPP_CHAP_FAILURE;

#define  AnscPppChapSfGetCode(p)                    AnscPppChapGetCode(p)
#define  AnscPppChapSfGetIdentifier(p)              AnscPppChapGetIdentifier(p)
#define  AnscPppChapSfGetLength(p)                  AnscPppChapGetLength(p)

#define  AnscPppChapSfSetCode(p, c)                 AnscPppChapSetCode(p, c)
#define  AnscPppChapSfSetIdentifier(p, id)          AnscPppChapSetIdentifier(p, id)
#define  AnscPppChapSfSetLength(p, l)               AnscPppChapSetLength(p, l)

#define  AnscPppChapSfGetMessage(p)                 ( p->Message )
#define  AnscPppChapSfGetMessageSize(p)             ( (ULONG)AnscPppChapSfGetLength(p) - sizeof(PPP_CHAP_SUCCESS) )

#define  AnscPppChapSfSetMessage(p, s)              AnscCopyString((char*)AnscPppChapSfGetMessage(p), s)


/***********************************************************
    DATA STRUCTURES USED BY PPP MS-CHAP1 PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Microsoft PPP CHAP Extensions.
 */
typedef  struct
_PPP_MSCHAP1_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP1_HEADER,  *PPPP_MSCHAP1_HEADER;

#define  AnscPppMsChap1GetCode(p)                   ( p->Code )
#define  AnscPppMsChap1GetIdentifier(p)             ( p->Identifier )
#define  AnscPppMsChap1GetLength(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppMsChap1GetDataLength(p)             ( (AnscPppMsChap1GetLength(p)) - sizeof(PPP_MSCHAP1_HEADER) )

#define  AnscPppMsChap1SetCode(p, c)                ( p->Code       = c  )
#define  AnscPppMsChap1SetIdentifier(p, id)         ( p->Identifier = id )
#define  AnscPppMsChap1SetLength(p, l)              AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))

/*
 * The Challenge packet is used to begin the Challenge-Handshake Authentication Protocol. The
 * authenticator MUST transmit a CHAP packet with the Code field set to 1 (Challenge). Additional
 * Challenge packets MUST be sent until a valid Response packet is received, or and  optional
 * retry counter expires. The MS-CHAP Challenge packet is identical in format to the standard CHAP
 * Challenge packet. MS-CHAP authenticators send an 8-octet challenge value field. Peers need not
 * duplicate Microsoft's algorithm for selecting the 8-octet value.
 */
#define  PPP_MSCHAP1_CHALLENGE_SIZE                 8

typedef  struct
_PPP_MSCHAP1_CHALLENGE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           ValueSize;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP1_CHALLENGE,  *PPPP_MSCHAP1_CHALLENGE;

#define  AnscPppMsChap1CrGetCode(p)                 AnscPppMsChap1GetCode(p)
#define  AnscPppMsChap1CrGetIdentifier(p)           AnscPppMsChap1GetIdentifier(p)
#define  AnscPppMsChap1CrGetLength(p)               AnscPppMsChap1GetLength(p)

#define  AnscPppMsChap1CrSetCode(p, c)              AnscPppMsChap1SetCode(p, c)
#define  AnscPppMsChap1CrSetIdentifier(p, id)       AnscPppMsChap1SetIdentifier(p, id)
#define  AnscPppMsChap1CrSetLength(p, l)            AnscPppMsChap1SetLength(p, l)

#define  AnscPppMsChap1CrGetValueSize(p)            ( p->ValueSize     )
#define  AnscPppMsChap1CrSetValueSize(p, s)         ( p->ValueSize = s )

#define  AnscPppMsChap1CrGetChallengeSize(p)        ( (ULONG)AnscPppMsChap1CrGetValueSize(p) )
#define  AnscPppMsChap1CrGetChallenge(p)            ( (PVOID)((ULONG)p + sizeof(PPP_MSCHAP1_CHALLENGE)) )
#define  AnscPppMsChap1CrGetName(p)                 ( (PVOID)((ULONG)AnscPppMsChap1CrGetChallenge(p) + AnscPppMsChap1CrGetChallengeSize(p)) )
#define  AnscPppMsChap1CrGetNameSize(p)             ( (ULONG)AnscPppMsChap1CrGetLength(p) - sizeof(PPP_MSCHAP1_CHALLENGE) - AnscPppMsChap1CrGetChallengeSize(p) )

#define  AnscPppMsChap1CrSetChallengeSize(p, s)     AnscPppMsChap1CrSetValueSize(p, (UCHAR)s)
#define  AnscPppMsChap1CrSetChallenge(p, c)         AnscCopyMemory(p->Value, c, AnscPppMsChap1CrGetChallengeSize(p))
#define  AnscPppMsChap1CrSetName(p, s)              AnscCopyString((char*)AnscPppMsChap1CrGetName(p), s)

/*
 * The MS-CHAP Response packet is identical in format to the standard CHAP response packet. However,
 * the Value field is sub-formatted differently as follows:
 *
 *      (1) 24 octets: LAN Manager compatible challenge response
 *      (2) 24 octets: Windows NT compatible challenge response
 *      (3)  1 octets: "Use Windows NT compatible challenge response" flag
 */
#define  PPP_MSCHAP1_LM_RESPONSE_SIZE               24
#define  PPP_MSCHAP1_NT_RESPONSE_SIZE               24
#define  PPP_MSCHAP1_RESPONSE_SIZE                  PPP_MSCHAP1_LM_RESPONSE_SIZE + PPP_MSCHAP1_NT_RESPONSE_SIZE + 1

typedef  struct
_PPP_MSCHAP1_RESPONSE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           ValueSize;
    UCHAR                           LmResponse[PPP_MSCHAP1_LM_RESPONSE_SIZE];
    UCHAR                           NtResponse[PPP_MSCHAP1_NT_RESPONSE_SIZE];
    UCHAR                           NtFlag;
    char                            Name[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP1_RESPONSE,  *PPPP_MSCHAP1_RESPONSE;

#define  AnscPppMsChap1CrGetNtFlag(p)               ( p->NtFlag )
#define  AnscPppMsChap1CrGetResponseSize(p)         \
         ( (PVOID)(p->NtFlag == 1)? PPP_MSCHAP1_NT_RESPONSE_SIZE : PPP_MSCHAP1_LM_RESPONSE_SIZE )
#define  AnscPppMsChap1CrGetLmResponse(p)           ( p->LmResponse )
#define  AnscPppMsChap1CrGetNtResponse(p)           ( p->NtResponse )
#define  AnscPppMsChap1CrGetResponse(p)             ( (PVOID)(p->NtFlag == 1)? p->NtResponse : p->LmResponse )

#define  AnscPppMsChap1CrSetNtFlag(p, f)            ( p->NtFlag = f )
#define  AnscPppMsChap1CrSetLmResponse(p, r)        AnscCopyMemory(p->LmResponse, r, PPP_MSCHAP1_LM_RESPONSE_SIZE)
#define  AnscPppMsChap1CrSetNtResponse(p, r)        AnscCopyMemory(p->NtResponse, r, PPP_MSCHAP1_NT_RESPONSE_SIZE)

/*
 * If the Value received in a Response is equal to the expected value, then the implementation
 * MUST transmit a CHAP packet with the code field set to 3 (Success). If the Value received in
 * a Response is not equal to the expected value, then the implementation MUST transmit a CHAP
 * packet with the Code field set to 4 (field), and SHOULD take action to terminate the link.
 */
typedef  struct
_PPP_MSCHAP1_SUCCESS
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    char                            Message[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP1_SUCCESS,  *PPPP_MSCHAP1_SUCCESS;

typedef  struct  _PPP_MSCHAP1_SUCCESS  PPP_MSCHAP1_FAILURE,  *PPPP_MSCHAP1_FAILURE;

#define  AnscPppMsChap1SfGetCode(p)                 AnscPppMsChap1GetCode(p)
#define  AnscPppMsChap1SfGetIdentifier(p)           AnscPppMsChap1GetIdentifier(p)
#define  AnscPppMsChap1SfGetLength(p)               AnscPppMsChap1GetLength(p)

#define  AnscPppMsChap1SfSetCode(p, c)              AnscPppMsChap1SetCode(p, c)
#define  AnscPppMsChap1SfSetIdentifier(p, id)       AnscPppMsChap1SetIdentifier(p, id)
#define  AnscPppMsChap1SfSetLength(p, l)            AnscPppMsChap1SetLength(p, l)

#define  AnscPppMsChap1SfGetMessage(p)              ( p->Message )
#define  AnscPppMsChap1SfGetMessageSize(p)          ( (ULONG)AnscPppMsChap1SfGetLength(p) - sizeof(PPP_MSCHAP1_SUCCESS) )

#define  AnscPppMsChap1SfSetMessage(p, s)           AnscCopyString((char*)AnscPppMsChap1SfGetMessage(p), s)


/***********************************************************
    DATA STRUCTURES USED BY PPP MS-CHAP2 PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Microsoft PPP CHAP Extensions v2.
 */
typedef  struct
_PPP_MSCHAP2_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP2_HEADER,  *PPPP_MSCHAP2_HEADER;

#define  AnscPppMsChap2GetCode(p)                   ( p->Code )
#define  AnscPppMsChap2GetIdentifier(p)             ( p->Identifier )
#define  AnscPppMsChap2GetLength(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppMsChap2GetDataLength(p)             ( (AnscPppMsChap2GetLength(p)) - sizeof(PPP_MSCHAP2_HEADER) )

#define  AnscPppMsChap2SetCode(p, c)                ( p->Code       = c  )
#define  AnscPppMsChap2SetIdentifier(p, id)         ( p->Identifier = id )
#define  AnscPppMsChap2SetLength(p, l)              AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))

/*
 * The Challenge packet is used to begin the Challenge-Handshake Authentication Protocol. The
 * authenticator MUST transmit a CHAP packet with the Code field set to 1 (Challenge). Additional
 * Challenge packets MUST be sent until a valid Response packet is received, or and  optional
 * retry counter expires. The MS-CHAP Challenge packet is identical in format to the standard CHAP
 * Challenge packet. MS-CHAP authenticators send an 16-octet challenge value field. Peers need not
 * duplicate Microsoft's algorithm for selecting the 16-octet value.
 */
#define  PPP_MSCHAP2_CHALLENGE_SIZE                 16

typedef  struct
_PPP_MSCHAP2_CHALLENGE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           ValueSize;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP2_CHALLENGE,  *PPPP_MSCHAP2_CHALLENGE;

#define  AnscPppMsChap2CrGetCode(p)                 AnscPppMsChap2GetCode(p)
#define  AnscPppMsChap2CrGetIdentifier(p)           AnscPppMsChap2GetIdentifier(p)
#define  AnscPppMsChap2CrGetLength(p)               AnscPppMsChap2GetLength(p)

#define  AnscPppMsChap2CrSetCode(p, c)              AnscPppMsChap2SetCode(p, c)
#define  AnscPppMsChap2CrSetIdentifier(p, id)       AnscPppMsChap2SetIdentifier(p, id)
#define  AnscPppMsChap2CrSetLength(p, l)            AnscPppMsChap2SetLength(p, l)

#define  AnscPppMsChap2CrGetValueSize(p)            ( p->ValueSize     )
#define  AnscPppMsChap2CrSetValueSize(p, s)         ( p->ValueSize = s )

#define  AnscPppMsChap2CrGetChallengeSize(p)        ( (ULONG)AnscPppMsChap2CrGetValueSize(p) )
#define  AnscPppMsChap2CrGetChallenge(p)            ( (PVOID)((ULONG)p + sizeof(PPP_MSCHAP2_CHALLENGE)) )
#define  AnscPppMsChap2CrGetName(p)                 ( (PVOID)((ULONG)AnscPppMsChap2CrGetChallenge(p) + AnscPppMsChap2CrGetChallengeSize(p)) )
#define  AnscPppMsChap2CrGetNameSize(p)             ( (ULONG)AnscPppMsChap2CrGetLength(p) - sizeof(PPP_MSCHAP2_CHALLENGE) - AnscPppMsChap2CrGetChallengeSize(p) )

#define  AnscPppMsChap2CrSetChallengeSize(p, s)     AnscPppMsChap2CrSetValueSize(p, (UCHAR)s)
#define  AnscPppMsChap2CrSetChallenge(p, c)         AnscCopyMemory(p->Value, c, AnscPppMsChap2CrGetChallengeSize(p))
#define  AnscPppMsChap2CrSetName(p, s)              AnscCopyString((char*)AnscPppMsChap2CrGetName(p), s)

/*
 * The MS-CHAP2 Response packet is identical in format to the standard CHAP response packet.
 * However, the Value field is sub-formatted differently as follows:
 *
 *      (1) 16 octets: Peer-Challenge
 *      (2)  8 octets: Reserved, must be zero
 *      (3) 24 octets: Windows NT compatible challenge response
 *      (4)  1 octets: Flags, reserved for future use and must be zero
 */
#define  PPP_MSCHAP2_PEER_CHALLENGE_SIZE            16
#define  PPP_MSCHAP2_RESERVED_SIZE                  8
#define  PPP_MSCHAP2_NT_RESPONSE_SIZE               24
#define  PPP_MSCHAP2_RESPONSE_SIZE                  PPP_MSCHAP2_PEER_CHALLENGE_SIZE + PPP_MSCHAP2_RESERVED_SIZE + PPP_MSCHAP2_NT_RESPONSE_SIZE + 1

typedef  struct
_PPP_MSCHAP2_RESPONSE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           ValueSize;
    UCHAR                           PeerChallenge[PPP_MSCHAP2_PEER_CHALLENGE_SIZE];
    UCHAR                           Reserved     [PPP_MSCHAP2_RESERVED_SIZE];
    UCHAR                           NtResponse   [PPP_MSCHAP2_NT_RESPONSE_SIZE];
    UCHAR                           Flag;
    char                            Name[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP2_RESPONSE,  *PPPP_MSCHAP2_RESPONSE;

#define  AnscPppMsChap2CrGetPeerChallenge(p)        ( (PVOID)p->PeerChallenge )
#define  AnscPppMsChap2CrGetResponse(p)             ( (PVOID)p->NtResponse )
#define  AnscPppMsChap2CrGetResponseSize(p)         PPP_MSCHAP2_NT_RESPONSE_SIZE
#define  AnscPppMsChap2CrGetFlag(p)                 ( p->Flag )

#define  AnscPppMsChap2CrSetPeerChallenge(p, c)     AnscCopyMemory(p->PeerChallenge, c, PPP_MSCHAP2_PEER_CHALLENGE_SIZE)
#define  AnscPppMsChap2CrSetResponse(p, r)          AnscCopyMemory(p->NtResponse,    r, PPP_MSCHAP2_NT_RESPONSE_SIZE   )
#define  AnscPppMsChap2CrSetFlag(p, f)              ( p->Flag = f )

/*
 * If the Value received in a Response is equal to the expected value, then the implementation
 * MUST transmit a CHAP packet with the code field set to 3 (Success). If the Value received in
 * a Response is not equal to the expected value, then the implementation MUST transmit a CHAP
 * packet with the Code field set to 4 (field), and SHOULD take action to terminate the link.
 */
typedef  struct
_PPP_MSCHAP2_SUCCESS
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    char                            Message[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_MSCHAP2_SUCCESS,  *PPPP_MSCHAP2_SUCCESS;

typedef  struct  _PPP_MSCHAP2_SUCCESS  PPP_MSCHAP2_FAILURE,  *PPPP_MSCHAP2_FAILURE;

#define  AnscPppMsChap2SfGetCode(p)                 AnscPppMsChap2GetCode(p)
#define  AnscPppMsChap2SfGetIdentifier(p)           AnscPppMsChap2GetIdentifier(p)
#define  AnscPppMsChap2SfGetLength(p)               AnscPppMsChap2GetLength(p)

#define  AnscPppMsChap2SfSetCode(p, c)              AnscPppMsChap2SetCode(p, c)
#define  AnscPppMsChap2SfSetIdentifier(p, id)       AnscPppMsChap2SetIdentifier(p, id)
#define  AnscPppMsChap2SfSetLength(p, l)            AnscPppMsChap2SetLength(p, l)

#define  AnscPppMsChap2SfGetMessage(p)              ( p->Message )
#define  AnscPppMsChap2SfGetMessageSize(p)          ( (ULONG)AnscPppMsChap2SfGetLength(p) - sizeof(PPP_MSCHAP2_SUCCESS) )

#define  AnscPppMsChap2SfSetMessage(p, s)           AnscCopyString((char*)AnscPppMsChap2SfGetMessage(p), s)


/***********************************************************
     DATA STRUCTURES USED BY PPP IPCP PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Point-to-Point (PPP) Internet Protocol Control protocol (IPCP)
 */
typedef  struct
_PPP_IPCP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_IPCP_HEADER,  *PPPP_IPCP_HEADER;

#define  AnscPppIpcpGetCode(p)                      ( p->Code )
#define  AnscPppIpcpGetIdentifier(p)                ( p->Identifier )
#define  AnscPppIpcpGetLength(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppIpcpGetDataLength(p)                ( AnscPppIpcpGetLength(p) - sizeof(PPP_IPCP_HEADER) )

#define  AnscPppIpcpSetCode(p, c)                   ( p->Code       = c  )
#define  AnscPppIpcpSetIdentifier(p, id)            ( p->Identifier = id )
#define  AnscPppIpcpSetLength(p, l)                 AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscPppIpcpSetInfo(p, i, l)                AnscCopyMemory(p->Info, i, l)

typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_CFG_REQUEST,  *PPPP_IPCP_CFG_REQUEST;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_CFG_ACK,      *PPPP_IPCP_CFG_ACK;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_CFG_NAK,      *PPPP_IPCP_CFG_NAK;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_CFG_REJECT,   *PPPP_IPCP_CFG_REJECT;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_TMN_REQUEST,  *PPPP_IPCP_TMN_REQUEST;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_TMN_ACK,      *PPPP_IPCP_TMN_ACK;
typedef  struct  _PPP_IPCP_HEADER  PPP_IPCP_CODE_REJECT,  *PPPP_IPCP_CODE_REJECT;

/*
 * IPCP Configuration Options allow negotiation of modifications to the default characteristics of
 * a point-to-point link. If a Configuration Option is not included in a Configuration-Request
 * packet, the default value for that Configuration Option is assumed.
 */
typedef  struct
_PPP_IPCP_CFG_OPTION
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_IPCP_CFG_OPTION,  *PPPP_IPCP_CFG_OPTION;

#define  AnscPppIpcpCfgGetFirstOption(p)            ( (PVOID)((ULONG)p + sizeof(PPP_IPCP_HEADER)) )

#define  AnscPppIpcpCfgOptionGetType(p)             ( p->Type   )
#define  AnscPppIpcpCfgOptionGetLength(p)           ( p->Length )
#define  AnscPppIpcpCfgOptionGetData(p)             ( p->Data   )

#define  AnscPppIpcpCfgOptionSetType(p, t)          ( p->Type   = t )
#define  AnscPppIpcpCfgOptionSetLength(p, l)        ( p->Length = l )
#define  AnscPppIpcpCfgOptionSetData(p, d, l)       AnscCopyMemory(p->Data, d, l)

#define  AnscPppIpcpCfgOptionGetDataLength(p)       ( AnscPppIpcpCfgOptionGetLength(p) - (UCHAR)sizeof(PPP_IPCP_CFG_OPTION) )
#define  AnscPppIpcpCfgOptionGetNextOption(p)       ( (PVOID)((ULONG)p + (ULONG)AnscPppIpcpCfgOptionGetLength(p)) )

typedef  struct
_PPP_IPCP_OPTION_COMP
{
    UCHAR                           Type;
    UCHAR                           Length;
    USHORT                          Protocol;
    UCHAR                           MaxSlotId;
    UCHAR                           CmpSlotId;
}_struct_pack_
PPP_IPCP_OPTION_COMP,  *PPPP_IPCP_OPTION_COMP;

typedef  struct
_PPP_IPCP_OPTION_IPADDR
{
    UCHAR                           Type;
    UCHAR                           Length;
    ANSC_IPV4_ADDRESS               Address;
}_struct_pack_
PPP_IPCP_OPTION_IPADDR,  *PPPP_IPCP_OPTION_IPADDR;

#define  AnscPppIpcpOptionIpAddrGetDot(p)           ( p->Address.Dot )
#define  AnscPppIpcpOptionIpAddrGetValue(p)         ( AnscReadUlong(&p->Address.Value) )

#define  AnscPppIpcpOptionIpAddrSetDot(p, addr)     AnscWriteUlong(&p->Address.Dot[0], *(PULONG)addr)
#define  AnscPppIpcpOptionIpAddrSetValue(p, addr)   AnscWriteUlong(&p->Address.Value,  addr         )

typedef  struct  _PPP_IPCP_OPTION_IPADDR  PPP_IPCP_OPTION_DNSADDR,   *PPPP_IPCP_OPTION_DNSADDR;
typedef  struct  _PPP_IPCP_OPTION_IPADDR  PPP_IPCP_OPTION_NBNSADDR,  *PPPP_IPCP_OPTION_NBNSADDR;


/***********************************************************
      DATA STRUCTURES USED BY PPP CCP PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Point-to-Point (PPP) Compression Control protocol (CCP)
 */
typedef  struct
_PPP_CCP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Info[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_CCP_HEADER,  *PPPP_CCP_HEADER;

#define  AnscPppCcpGetCode(p)                       ( p->Code )
#define  AnscPppCcpGetIdentifier(p)                 ( p->Identifier )
#define  AnscPppCcpGetLength(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppCcpGetDataLength(p)                 ( AnscPppCcpGetLength(p) - sizeof(PPP_CCP_HEADER) )

#define  AnscPppCcpSetCode(p, c)                    ( p->Code       = c  )
#define  AnscPppCcpSetIdentifier(p, id)             ( p->Identifier = id )
#define  AnscPppCcpSetLength(p, l)                  AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscPppCcpSetInfo(p, i, l)                 AnscCopyMemory(p->Info, i, l)

typedef  struct  _PPP_CCP_HEADER  PPP_CCP_CFG_REQUEST,  *PPPP_CCP_CFG_REQUEST;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_CFG_ACK,      *PPPP_CCP_CFG_ACK;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_CFG_NAK,      *PPPP_CCP_CFG_NAK;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_CFG_REJECT,   *PPPP_CCP_CFG_REJECT;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_TMN_REQUEST,  *PPPP_CCP_TMN_REQUEST;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_TMN_ACK,      *PPPP_CCP_TMN_ACK;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_CODE_REJECT,  *PPPP_CCP_CODE_REJECT;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_RST_REQUEST,  *PPPP_CCP_RST_REQUEST;
typedef  struct  _PPP_CCP_HEADER  PPP_CCP_RST_ACK,      *PPPP_CCP_RST_ACK;

/*
 * CCP Configuration Options allow negotiation of modifications to the default characteristics of
 * a point-to-point link. If a Configuration Option is not included in a Configuration-Request
 * packet, the default value for that Configuration Option is assumed.
 */
typedef  struct
_PPP_CCP_CFG_OPTION
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPP_CCP_CFG_OPTION,  *PPPP_CCP_CFG_OPTION;

#define  AnscPppCcpCfgGetFirstOption(p)             ( (PVOID)((ULONG)p + sizeof(PPP_CCP_HEADER)) )

#define  AnscPppCcpCfgOptionGetType(p)              ( p->Type   )
#define  AnscPppCcpCfgOptionGetLength(p)            ( p->Length )
#define  AnscPppCcpCfgOptionGetData(p)              ( p->Data   )

#define  AnscPppCcpCfgOptionSetType(p, t)           ( p->Type   = t )
#define  AnscPppCcpCfgOptionSetLength(p, l)         ( p->Length = l )
#define  AnscPppCcpCfgOptionSetData(p, d, l)        AnscCopyMemory(p->Data, d, l)

#define  AnscPppCcpCfgOptionGetDataLength(p)        ( AnscPppCcpCfgOptionGetLength(p) - (UCHAR)sizeof(PPP_CCP_CFG_OPTION) )
#define  AnscPppCcpCfgOptionGetNextOption(p)        ( (PVOID)((ULONG)p + (ULONG)AnscPppCcpCfgOptionGetLength(p)) )

/*
 * Microsoft doesn't use the standard PPP ECP (Encryption Control Protocol) to provide traffic
 * confidentiality over the PPP link. Instead, they used CCP to negotiate an encryption option
 * - Microsoft Point-to-Point Encryption (MPPE).
 */
#define  PPP_MSPPE_STATELESS_MODE                   0x01000000
#define  PPP_MSPPE_ENCRYPTION56                     0x00000080
#define  PPP_MSPPE_ENCRYPTION128                    0x00000040
#define  PPP_MSPPE_ENCRYPTION40                     0x00000020
#define  PPP_MSPPE_COMPRESSION                      0x00000001

typedef  struct
_PPP_CCP_OPTION_MSPPE
{
    UCHAR                           Type;
    UCHAR                           Length;
    ULONG                           SupportedBits;
}_struct_pack_
PPP_CCP_OPTION_MSPPE,  *PPPP_CCP_OPTION_MSPPE;

#define  AnscPppCcpOptionMsPpeGetBits(p)            ( AnscUlongFromNToH(AnscReadUlong(&p->SupportedBits)) )
#define  AnscPppCcpOptionMsPpeSetBits(p, b)         AnscWriteUlong(&p->SupportedBits, AnscUlongFromHToN(b))

#define  AnscPppCcpOptionMsPpeIsStateless(p)        ( (AnscPppCcpOptionMsPpeGetBits(p) & PPP_MSPPE_STATELESS_MODE) != 0 )
#define  AnscPppCcpOptionMsPpeIsE56(p)              ( (AnscPppCcpOptionMsPpeGetBits(p) & PPP_MSPPE_ENCRYPTION56  ) != 0 )
#define  AnscPppCcpOptionMsPpeIsE128(p)             ( (AnscPppCcpOptionMsPpeGetBits(p) & PPP_MSPPE_ENCRYPTION128 ) != 0 )
#define  AnscPppCcpOptionMsPpeIsE40(p)              ( (AnscPppCcpOptionMsPpeGetBits(p) & PPP_MSPPE_ENCRYPTION40  ) != 0 )
#define  AnscPppCcpOptionMsPpeIsComp(p)             ( (AnscPppCcpOptionMsPpeGetBits(p) & PPP_MSPPE_COMPRESSION   ) != 0 )


/***********************************************************
     DATA STRUCTURES USED BY PPP MSPPE PACKET OPERATION
***********************************************************/

/*
 * Define the data structure for Microsoft Point-to-Point (PPP) Encryption protocol (MSPPE)
 */
#define  PPP_MSPPE_BITA_MASK                        0x8000
#define  PPP_MSPPE_BITB_MASK                        0x4000
#define  PPP_MSPPE_BITC_MASK                        0x2000
#define  PPP_MSPPE_BITD_MASK                        0x1000
#define  PPP_MSPPE_FLAG_MASK                        0xF000
#define  PPP_MSPPE_COHERENCY_COUNT_MASK             0x0FFF
#define  PPP_MSPPE_FLAG_PACKET_MASK                 0x00FF
#define  PPP_MSPPE_FLUSHED_BIT_MASK                 PPP_MSPPE_BITA_MASK
#define  PPP_MSPPE_COMPRESSED_BIT_MASK              PPP_MSPPE_BITC_MASK
#define  PPP_MSPPE_ENCRYPTED_BIT_MASK               PPP_MSPPE_BITD_MASK

typedef  struct
_PPP_MSPPE_HEADER
{
    USHORT                          FlagAndCount;
}_struct_pack_
PPP_MSPPE_HEADER,  *PPPP_MSPPE_HEADER;

#define  AnscPppMsPpeGetFlagAndCount(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->FlagAndCount)) )
#define  AnscPppMsPpeSetFlagAndCount(p, fc)         AnscWriteUshort(&p->FlagAndCount, AnscUshortFromHToN(fc))

typedef  struct  _PPP_MSPPE_HEADER  PPP_MSPPC_HEADER,  *PPPP_MSPPC_HEADER;

#define  AnscPppMsPpeGetFlag(p)                     ( AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_FLAG_MASK            )
#define  AnscPppMsPpeGetCoherencyCount(p)           ( AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_COHERENCY_COUNT_MASK )

#define  AnscPppMsPpeSetFlag(p, f)                  AnscPppMsPpeSetFlagAndCount(p, (USHORT)AnscPppMsPpeGetCoherencyCount(p) | f )
#define  AnscPppMsPpeSetCoherencyCount(p, cc)       AnscPppMsPpeSetFlagAndCount(p, (USHORT)AnscPppMsPpeGetFlag(p)           | cc)

#define  AnscPppMsPpeIsBitA(p)                      ( (AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_BITA_MASK       ) != 0 )
#define  AnscPppMsPpeIsBitB(p)                      ( (AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_BITB_MASK       ) != 0 )
#define  AnscPppMsPpeIsBitC(p)                      ( (AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_BITC_MASK       ) != 0 )
#define  AnscPppMsPpeIsBitD(p)                      ( (AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_BITD_MASK       ) != 0 )
#define  AnscPppMsPpeIsFlagPacket(p)                ( (AnscPppMsPpeGetFlagAndCount(p) & PPP_MSPPE_FLAG_PACKET_MASK) == 0x00FF )
#define  AnscPppMsPpeIsFlushed                      AnscPppMsPpeIsBitA
#define  AnscPppMsPpeIsCompressed                   AnscPppMsPpeIsBitC
#define  AnscPppMsPpeIsEncrypted                    AnscPppMsPpeIsBitD

#define  AnscPppMsPpcGetCoherencyCount              AnscPppMsPpeGetCoherencyCount
#define  AnscPppMsPpcIsBitA                         AnscPppMsPpeIsBitA
#define  AnscPppMsPpcIsBitB                         AnscPppMsPpeIsBitB
#define  AnscPppMsPpcIsBitC                         AnscPppMsPpeIsBitC
#define  AnscPppMsPpcIsBitD                         AnscPppMsPpeIsBitD
#define  AnscPppMsPpcIsFlagPacket                   AnscPppMsPpeIsFlagPacket
#define  AnscPppMsPpcIsFlushed                      AnscPppMsPpeIsFlushed
#define  AnscPppMsPpcIsCompressed                   AnscPppMsPpeIsCompressed
#define  AnscPppMsPpcIsEncrypted                    AnscPppMsPpeIsEncrypted


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
