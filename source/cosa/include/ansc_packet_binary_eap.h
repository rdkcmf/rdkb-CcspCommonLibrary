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

    module:	ansc_packet_binary_eap.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Extensible Authentication Protocol
        (EAP), as defined in RFC 3748.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        11/28/06    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_EAP_
#define  _ANSC_PACKET_BINARY_EAP_


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
    DATA STRUCTURES USED BY EAP-RELATED PACKET OPERATION
***********************************************************/

/*
 * The lower layer is responsible for transmitting and receiving EAP frames between the peer and
 * authenticator. The EAP layer receives and transmits EAP packets via the lower layer, implements
 * duplicate detection and retransmission, and delivers and receives EAP messages to and from the
 * EAP peer and authenticator layers.
 *
 * A summary of the EAP packet format is shown below. The fields are transmitted from left to right.
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Code      |  Identifier   |            Length             |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |    Data ...
 *      +-+-+-+-+
 */
#define  EAP_CODE_Request                           1
#define  EAP_CODE_Response                          2
#define  EAP_CODE_Success                           3
#define  EAP_CODE_Failure                           4

typedef  struct
_EAP_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
EAP_HEADER,  *PEAP_HEADER;

#define  AnscEapGetCode(p)                          ( p->Code       )
#define  AnscEapGetIdentifier(p)                    ( p->Identifier )
#define  AnscEapGetLength(p)                        ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscEapGetData(p)                          ( p->Data       )
#define  AnscEapGetDataSize(p)                      ( AnscEapGetLength(p) - sizeof(EAP_HEADER) )

#define  AnscEapSetCode(p, c)                       ( p->Code       = c  )
#define  AnscEapSetIdentifier(p, id)                ( p->Identifier = id )
#define  AnscEapSetLength(p, l)                     AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscEapSetData(p, d, l)                    AnscCopyMemory(p->Data, d, l)


#define  EAP_REQ_TYPE_Identity                      1
#define  EAP_REQ_TYPE_Notification                  2
#define  EAP_REQ_TYPE_Nak                           3
#define  EAP_REQ_TYPE_MD5                           4
#define  EAP_REQ_TYPE_OTP                           5
#define  EAP_REQ_TYPE_GTC                           6
#define  EAP_REQ_TYPE_EAP_TLS                       13
#define  EAP_REQ_TYPE_Expanded                      254
#define  EAP_REQ_TYPE_Experimental                  255

typedef  struct
_EAP_REQUEST_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Type;
    UCHAR                           TypeData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
EAP_REQUEST_HEADER,  *PEAP_REQUEST_HEADER;

typedef  struct  _EAP_REQUEST_HEADER  EAP_RESPONSE_HEADER,  *PEAP_RESPONSE_HEADER;

#define  AnscEapReqGetCode(p)                       ( p->Code       )
#define  AnscEapReqGetIdentifier(p)                 ( p->Identifier )
#define  AnscEapReqGetLength(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscEapReqGetType(p)                       ( p->Type       )
#define  AnscEapReqGetTypeData(p)                   ( p->TypeData   )
#define  AnscEapReqGetDataSize(p)                   ( AnscEapGetLength(p) - sizeof(EAP_HEADER) )

#define  AnscEapReqSetCode(p, c)                    ( p->Code       = c  )
#define  AnscEapReqSetIdentifier(p, id)             ( p->Identifier = id )
#define  AnscEapReqSetLength(p, l)                  AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscEapReqSetType(p, t)                    ( p->Type       = t  )
#define  AnscEapReqSetTypeData(p, d, l)             AnscCopyMemory(p->TypeData, d, l)

#define  AnscEapRepGetCode                          AnscEapReqGetCode
#define  AnscEapRepGetIdentifier                    AnscEapReqGetIdentifier
#define  AnscEapRepGetLength                        AnscEapReqGetLength
#define  AnscEapRepGetType                          AnscEapReqGetType
#define  AnscEapRepGetTypeData                      AnscEapReqGetTypeData
#define  AnscEapRepGetDataSize                      AnscEapReqGetDataSize

#define  AnscEapRepSetCode                          AnscEapReqSetCode
#define  AnscEapRepSetIdentifier                    AnscEapReqSetIdentifier
#define  AnscEapRepSetLength                        AnscEapReqSetLength
#define  AnscEapRepSetType                          AnscEapReqSetType
#define  AnscEapRepSetTypeData                      AnscEapReqSetTypeData


typedef  struct
_EAP_EXPANDED_TYPE
{
    union
    {
        UCHAR                       UcharArray[4];
        ULONG                       UlongValue;
    }VendorId;
    ULONG                           Type;
}_struct_pack_
EAP_EXPANDED_TYPE,  *PEAP_EXPANDED_TYPE;

#define  AnscEapExTypeValidate(p)                   ( p->VendorId.UcharArray[0] == EAP_REQ_TYPE_Expanded                     )
#define  AnscEapExTypeGetVendorId(p)                ( AnscUlongFromNToH(AnscReadUlong(&p->VendorId.UlongValue)) & 0x00FFFFFF )
#define  AnscEapExTypeGetType(p)                    ( AnscUlongFromNToH(AnscReadUlong(&p->Type               ))              )

#define  AnscEapExTypeSetVendorId(p, id)            AnscWriteUlong(&p->VendorId.UlongValue, AnscUlongFromHToN(id)); p->VendorId.UcharArray[0] = EAP_REQ_TYPE_Expanded
#define  AnscEapExTypeSetType(p, t)                 AnscWriteUlong(&p->Type,                AnscUlongFromHToN(t ))


/***********************************************************
     DATA STRUCTURES USED BY EAP MD5 CHALLENGE OPERATION
***********************************************************/


typedef  struct
_EAP_MD5_CHALLENGE
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Type;
    UCHAR                           ValueSize;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
EAP_MD5_CHALLENGE,  *PEAP_MD5_CHALLENGE;

#define  AnscEapMd5CrGetCode(p)                     AnscEapReqGetCode(p)
#define  AnscEapMd5CrGetIdentifier(p)               AnscEapReqGetIdentifier(p)
#define  AnscEapMd5CrGetLength(p)                   AnscEapReqGetLength(p)
#define  AnscEapMd5CrGetType(p)                     AnscEapReqGetType(p)

#define  AnscEapMd5CrSetCode(p, c)                  AnscEapReqSetCode(p, c)
#define  AnscEapMd5CrSetIdentifier(p, id)           AnscEapReqSetIdentifier(p, id)
#define  AnscEapMd5CrSetLength(p, l)                AnscEapReqSetLength(p, l)
#define  AnscEapMd5CrSetType(p, t)                  AnscEapReqSetType(p, t)

#define  AnscEapMd5CrGetValueSize(p)                ( p->ValueSize     )
#define  AnscEapMd5CrSetValueSize(p, s)             ( p->ValueSize = s )

#define  AnscEapMd5CrGetValue(p)                    ( (PVOID)((ULONG)p + sizeof(EAP_MD5_CHALLENGE)) )
#define  AnscEapMd5CrGetName(p)                     ( (PVOID)((ULONG)AnscEapMd5CrGetValue(p) + AnscEapMd5CrGetValueSize(p)) )
#define  AnscEapMd5CrGetNameSize(p)                 ( (ULONG)AnscEapMd5CrGetLength(p) - sizeof(EAP_MD5_CHALLENGE) - AnscEapMd5CrGetValueSize(p) )

#define  AnscEapMd5CrSetValue(p, v)                 AnscCopyMemory(p->Value, v, AnscEapMd5CrGetValueSize(p))
#define  AnscEapMd5CrSetName(p, s)                  AnscCopyString((char*)AnscEapMd5CrGetName(p), s)


/***********************************************************
            DATA STRUCTURES USED BY EAPOL OPERATION
***********************************************************/

/*
 * The 802.1X (EAPOL) protocol provides effective authentication regardless of whether you imple-
 * ment 802.11 WEP keys or no encryption at all. If configured to implement dynamic key exchange,
 * the 802.1X authentication server can return session keys to the access point along with the
 * accept message. The access point uses the session keys to build, sign and encrypt an EAP key
 * message that is sent to the client immediately after sending the success message. The client can
 * then use contents of the key message to define applicable encryption keys.
 */
#define  EAPOL_DEF_VERSION                          2

#define  EAPOL_PACKET_TYPE_EAP_Packet               0
#define  EAPOL_PACKET_TYPE_EAPOL_Start              1
#define  EAPOL_PACKET_TYPE_EAPOL_Logoff             2
#define  EAPOL_PACKET_TYPE_EAPOL_Key                3
#define  EAPOL_PACKET_TYPE_EAPOL_ASF_Alert          4

typedef  struct
_EAPOL_HEADER
{
    UCHAR                           ProtocolVersion;
    UCHAR                           PacketType;
    USHORT                          PacketBodyLength;
    UCHAR                           PacketBody[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
EAPOL_HEADER,  *PEAPOL_HEADER;

#define  AnscEapolGetProtocolVersion(p)             ( p->ProtocolVersion )
#define  AnscEapolGetPacketType(p)                  ( p->PacketType      )
#define  AnscEapolGetPacketBodyLength(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->PacketBodyLength)) )
#define  AnscEapolGetPacketBody(p)                  ( p->PacketBody      )

#define  AnscEapolSetProtocolVersion(p, v)          ( p->ProtocolVersion = v )
#define  AnscEapolSetPacketType(p, t)               ( p->PacketType      = t )
#define  AnscEapolSetPacketBodyLength(p, l)         AnscWriteUshort(&p->PacketBodyLength, AnscUshortFromHToN(l))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
