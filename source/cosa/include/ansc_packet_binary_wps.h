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

    module:	ansc_packet_binary_wps.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Wi-Fi Protected Setup (WPS), as
        defined in Wi-Fi Simple Config v1.0.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/08/07    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_WPS_
#define  _ANSC_PACKET_BINARY_WPS_


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
      DATA STRUCTURES USED BY WPS-RELATED ENCAPSULATION
***********************************************************/

/*
 * The Simple Config EAP method uses EAP as specified in RFC 3748 and EAPOL as specified in IEEE
 * 802.1X-2001, but does not represent a network authentication protocol. Rather Simple Config
 * utilizes the 802.1X data connection for acquiring settings necessary for connecting to the
 * network & the resulting EAP exchange must always terminate with EAP-Fail.
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Code      |  Identifier   |            Length             |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Type      |               Vendor-Id                       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                          Vendor-Type                          |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |  Op-Code      |    Flags      |      Message Length           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |  Message data...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-
 */
#define  WPS_EAP_VendorID                           0x372A
#define  WPS_EAP_VendorType                         1

#define  WPS_EAP_OpCode_WSC_Start                   0x01
#define  WPS_EAP_OpCode_WSC_ACK                     0x02
#define  WPS_EAP_OpCode_WSC_NACK                    0x03
#define  WPS_EAP_OpCode_WSC_MSG                     0x04
#define  WPS_EAP_OpCode_WSC_Done                    0x05
#define  WPS_EAP_OpCode_WSC_FRAG_ACK                0x06

#define  WPS_EAP_Flags_MoreFragment                 0x01
#define  WPS_EAP_Flags_LengthField                  0x02

typedef  struct
_WPS_EAP_PACKET
{
    UCHAR                           OpCode;
    UCHAR                           Flags;
    USHORT                          MessageLength;
    UCHAR                           MessageData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
WPS_EAP_PACKET,  *PWPS_EAP_PACKET;

#define  WpsEapIsMoreFragment(p)                    ( p->Flags & WPS_EAP_Flags_MoreFragment )
#define  WpsEapIsLengthField(p)                     ( p->Flags & WPS_EAP_Flags_LengthField  )

#define  WpsEapGetOpCode(p)                         ( p->OpCode )
#define  WpsEapGetFlags(p)                          ( p->Flags  )
#define  WpsEapGetMessageLength(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->MessageLength)) )
#define  WpsEapGetMessageData(p)                    ( WpsEapIsLengthField(p)? p->MessageData : (UCHAR*)&p->MessageLength )

#define  WpsEapSetOpCode(p, c)                      ( p->OpCode = c )
#define  WpsEapSetFlags(p, f)                       ( p->Flags  = f )
#define  WpsEapSetMessageLength(p, l)               AnscWriteUshort(&p->MessageLength, AnscUshortFromHToN(l))
#define  WpsEapSetMessageData(p, d, l)              AnscCopyMemory(WpsEapGetMessageData(p), d, l)


/***********************************************************
   DATA STRUCTURES USED BY WPS IE ATTRIBUTE ENCAPSULATION
***********************************************************/

/*
 * The Simple Config Information Element complies with the IEEE 802.11 Information Element format
 * and indicates specific data necessary for network information, capabilities and modes, to
 * configure an Enrollee for the wireless network and to report problems with the Enrollee associat-
 * ing with a specified wireless network with the supplied settings
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Element ID  |    Length     |               OUI             |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |              OUI              |  Data ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef  struct
_WPS_IE_HEADER
{
    UCHAR                           ElementID;
    UCHAR                           Length;
    ULONG                           OUI;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
WPS_IE_HEADER,  *PWPS_IE_HEADER;

#define  AnscWpsGetElementID(p)                     ( p->ElementID )
#define  AnscWpsGetLength(p)                        ( p->Length    )
#define  AnscWpsGetOUI(p)                           ( AnscUlongFromNToH(AnscReadUlong(&p->OUI)) )
#define  AnscWpsGetData(p)                          ( p->Data       )
#define  AnscWpsGetDataSize(p)                      ( AnscWpsGetLength(p) - 4 )

#define  AnscWpsSetElementID(p, id)                 ( p->ElementID = id )
#define  AnscWpsSetLength(p, l)                     ( p->Length    = l  )
#define  AnscWpsSetOUI(p, oui)                      AnscWriteUlong(&p->OUI, AnscUlongFromHToN(oui))
#define  AnscWpsSetData(p, d, l)                    AnscCopyMemory(p->Data, d, l)


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
