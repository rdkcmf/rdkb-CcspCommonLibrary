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

    module:	ansc_packet_binary_eth.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Ethernet protocol.

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


#ifndef  _ANSC_PACKET_BINARY_ETH_
#define  _ANSC_PACKET_BINARY_ETH_


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
              DATA STRUCTURES USED BY ETHERNET
***********************************************************/

/*
 * Define some const values used in the ethernet definition.
 */
#define  ETHERNET_ADDRESS_SIZE                      6

#define  IP_FRAME                                   0x0800
#define  ARP_FRAME                                  0x0806
#define  PPPOE_DS_FRAME                             0x8863
#define  PPPOE_SS_FRAME                             0x8864
#define  IEEE_8021Q_TAG_FRAME                       0x8100
#define  OTHER_FRAME                                0x0000  /* for message like spanning tree, which has no defined frame type */

#define  ETH_FRAME_ARP                              0x0806
#define  ETH_FRAME_IP                               0x0800
#define  ETH_FRAME_PPPOE_DS                         0x8863
#define  ETH_FRAME_PPPOE_SS                         0x8864
#define  ETH_FRAME_EAPOL                            0x888E
#define  ETH_FRAME_LLTD                             0x88D9
#define  ETH_FRAME_LLD2                             ETH_FRAME_LLTD

/*
 * Define the ethernet header structure; note it's different from the IEEE 802.3
 */
#define  MAX_ETHERNET_PAYLOAD_SIZE                  1500

typedef  struct
_ETHERNET_HEADER
{
    UCHAR                           DstMacAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           SrcMacAddress[ETHERNET_ADDRESS_SIZE];
    USHORT                          FrameType;
    UCHAR                           Payload[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
ETHERNET_HEADER,  *PETHERNET_HEADER;

/*
 * Define some macros that can help us access the header fields.
 */
#define  AnscEthernetAssignAddress(a, b)            AnscCopyMemory (a, b, ETHERNET_ADDRESS_SIZE)
#define  AnscEthernetHashAddress(a, t)              AnscHashString (a, ETHERNET_ADDRESS_SIZE, t)
#define  AnscEthernetEqualAddress(a, b)             AnscEqualMemory(a, b, ETHERNET_ADDRESS_SIZE)
#define  AnscEthernetEqualAddress2(a, b)            ( (AnscReadUlong(&a[0]) == AnscReadUlong(&b[0])) && (AnscReadUshort(&a[4]) == AnscReadUshort(&b[4])) )
#define  AnscEthernetIsUnicast(p)                   ( (p->DstMacAddress[0] & 0x01) == 0 )
#define  AnscEthernetIsMulticast(p)                 ( (p->DstMacAddress[0] & 0x01) && !(AnscEthernetIsBroadcast(p)) )
#define  AnscEthernetIsBroadcast(p)                 \
         ( (AnscReadUshort(&p->DstMacAddress[0]) == 0xFFFF) && (AnscReadUlong(&p->DstMacAddress[2]) == 0xFFFFFFFF) )

#define  AnscEthernetGetDstMacAddress(p)            ( p->DstMacAddress )
#define  AnscEthernetGetSrcMacAddress(p)            ( p->SrcMacAddress )
#define  AnscEthernetGetFrameType(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->FrameType)) )

#define  AnscEthernetSetDstMacAddress(p, addr)      AnscEthernetAssignAddress((p->DstMacAddress), addr)
#define  AnscEthernetSetSrcMacAddress(p, addr)      AnscEthernetAssignAddress((p->SrcMacAddress), addr)
#define  AnscEthernetSetFrameType(p, type)          AnscWriteUshort(&p->FrameType, AnscUshortFromHToN(type))

#define  AnscEthernetGetPayload(p)                  ( p->Payload )

#define  AnscEthernetMapIpMcAddr(eth_addr, ip_addr)                                         \
         {                                                                                  \
            eth_addr[0] = 0x01;                                                             \
            eth_addr[1] = 0x00;                                                             \
            eth_addr[2] = 0x5e;                                                             \
            eth_addr[3] = ip_addr[1] & 0x7f;                                                \
            eth_addr[4] = ip_addr[2];                                                       \
            eth_addr[5] = ip_addr[3];                                                       \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
