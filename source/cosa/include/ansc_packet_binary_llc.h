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

    module:	ansc_packet_binary_llc.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Logical Link Control (LLC) defined in
        IEEE 802.2.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        09/19/05    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_LLC_
#define  _ANSC_PACKET_BINARY_LLC_


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
                 DATA STRUCTURES USED BY LLC
***********************************************************/

/*
 * The Logical Link Control (LLC) constitues the top sublay of the data link layer and are 
 * common for many medium access methods. The control field could be 8 bit if it does not
 * contain the sequence number, or 16 bits if it does. see 802.2 3.3.1.1.
 * The least significant bit in DSAP is IG flag. The least significant bit in SSAP is
 * CR bit
 *
 *           0               7 8              15 16
 *          +--------+--------+--------+--------+ 
 *          |     DSAP        |   SSAP          | 
 *          +--------+--------+--------+--------+ 
 *          |     control     |control or data  | 
 *          +--------+--------+--------+--------+ 
 *          |                                   |
 *          ~          data octets ...          ~
 *          |                                   |
 *          +-----------------------------------+
 *
 * For 802.3 3.1.1, the Ethernet header and LLC header should be like below.
 * the Length/Type field is interpreted as the Length is its value is less than
 * 1536 (0x600). It is the number of bytes in the subsequent field of the frame.
 *
 *                      Ethernet header                        LLC header
 *          +--------+--------+--------+--------+--------+--------+
 *          |     DstMac      |   SrcMac        | Length/Type | LLC ~
 *          |     6 bytes     |   6 bytes       | 2 bytes     |
 *          +--------+--------+--------+--------+--------+--------+ 
 */
#define  MAX_LLC_PAYLOAD_SIZE                       1472

#define  LLC_DSAP_IG_FLAG_MASK                      0x01    /* the least significent bit */
#define  LLC_SSAP_CR_FLAG_MASK                      0x01    /* the least significent bit */

#define  LLC_CONTROL_TYPE_INFO_TRAN_VALUE           0x00
#define  LLC_CONTROL_TYPE_INFO_TRAN_MASK            0x01
#define  LLC_CONTROL_TYPE_INFO_TRAN_SHIFT           0

#define  LLC_CONTROL_TYPE_SUPERVISORY_VALUE         0x02
#define  LLC_CONTROL_TYPE_SUPERVISORY_MASK          0x03
#define  LLC_CONTROL_TYPE_SUPERVISORY_SHIFT         0

#define  LLC_CONTROL_TYPE_UNNUMBERED_VALUE          0x03
#define  LLC_CONTROL_TYPE_UNNUMBERED_MASK           0x03
#define  LLC_CONTROL_TYPE_UNNUMBERED_SHIFT          0

#define  LLC_CTYPE_UNNUMBERED_PFBIT_MASK            0x10    /* poll/final if the bit is 1 */
#define  LLC_CTYPE_UNNUMBERED_PFBIT_SHIFT           4

/* 
 * 802.1d 7.12.3 table 7-8 bridge spanning tree 
 */
#define  LLC_ADDR_IP                                0x06    
#define  LLC_ADDR_STP                               0x42    
#define  LLC_ADDR_SNAP                              0xAA    


typedef  struct
_LLC_HEADER
{
    UCHAR                           Dsap;
    UCHAR                           Ssap;
    UCHAR                           Control0;
    UCHAR                           Control1OrPayload[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLC_HEADER,  *PLLC_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscLlcGetDsap(p)                          ( p->Dsap )
#define  AnscLlcGetSsap(p)                          ( p->Ssap )
#define  AnscLlcSetDsap(p, d)                       ( p->Dsap = d )
#define  AnscLlcSetSsap(p, s)                       ( p->Ssap = s )

#define  AnscLlcIsDsapIndividual(p)                 ( !(p->Dsap & LLC_DSAP_IG_FLAG_MASK) )
#define  AnscLlcIsDsapGroup(p)                      ( p->Dsap & LLC_DSAP_IG_FLAG_MASK    )
#define  AnscLlcIsSsapCommand(p)                    ( !(p->Ssap & LLC_SSAP_CR_FLAG_MASK) )
#define  AnscLlcIsSsapResponse(p)                   ( p->Ssap & LLC_SSAP_CR_FLAG_MASK    )
#define  AnscLlcIsControlTypeInfoTran(p)            ( ((p->Control0 & LLC_CONTROL_TYPE_INFO_TRAN_MASK  ) >> LLC_CONTROL_TYPE_INFO_TRAN_SHIFT  ) == LLC_CONTROL_TYPE_INFO_TRAN_VALUE   )
#define  AnscLlcIsControlTypeSupervisory(p)         ( ((p->Control0 & LLC_CONTROL_TYPE_SUPERVISORY_MASK) >> LLC_CONTROL_TYPE_SUPERVISORY_SHIFT) == LLC_CONTROL_TYPE_SUPERVISORY_VALUE )
#define  AnscLlcIsControlTypeUnnumbered(p)          ( ((p->Control0 & LLC_CONTROL_TYPE_UNNUMBERED_MASK ) >> LLC_CONTROL_TYPE_UNNUMBERED_SHIFT ) == LLC_CONTROL_TYPE_UNNUMBERED_VALUE  )

#define  AnscLlcSetDsapIndividual(p)                ( p->Dsap &= ~LLC_DSAP_IG_FLAG_MASK )
#define  AnscLlcResetDsapIndividual(p)              ( p->Dsap |= LLC_DSAP_IG_FLAG_MASK  )
#define  AnscLlcSetDsapGroup(p)                     ( p->Dsap |= LLC_DSAP_IG_FLAG_MASK  )
#define  AnscLlcResetDsapGroup(p)                   ( p->Dsap &= ~LLC_DSAP_IG_FLAG_MASK )
#define  AnscLlcSetSsapCommand(p)                   ( p->Ssap &= ~LLC_SSAP_CR_FLAG_MASK )
#define  AnscLlcResetSsapCommand(p)                 ( p->Ssap |= LLC_SSAP_CR_FLAG_MASK  )
#define  AnscLlcSetSsapResponse(p)                  ( p->Ssap |= LLC_SSAP_CR_FLAG_MASK  )
#define  AnscLlcResetSsapResponse(p)                ( p->Ssap &= ~LLC_SSAP_CR_FLAG_MASK )

#define  AnscLlcGetControl0(p)                      ( p->Control0     )
#define  AnscLlcSetControl0(p, v)                   ( p->Control0 = v )

#define  AnscLlcGetPayload(p)                       ( AnscLlcIsControlTypeUnnumbered(p) ?  p->Control1OrPayload : (&p->Control1OrPayload[1]) )

#define  AnscLlcUnnumberedPFbitValue(p)             ( (p->Control0 & LLC_CTYPE_UNNUMBERED_PFBIT_MASK ) >> LLC_CTYPE_UNNUMBERED_PFBIT_SHIFT )

/******************************************************************************
 *                          SNAP
 *
 * SNAP is defined in 802-2001. It stands for Subnetwork Access Point.

 * The SNAP is used with an Organization Code
 * indicating that the following 16 bits specify the EtherType code (as
 * listed in Assigned Numbers [7]).
 ******************************************************************************/

typedef  struct
_SNAP_HEADER
{
    UCHAR                           OUI[3];
    USHORT                          EtherType;      /* the same as standard ethernet type */
}_struct_pack_
SNAP_HEADER,  *PSNAP_HEADER;

#define  AnscSnapcGetOui(p)                          ( p->OUI )
#define  AnscSnapcSetOui(p, o)                       AnscCopyMemory(p->OUI, o, 3)
#define  AnscSnapcResetOui(p)                        AnscZeroMemory(p->OUI, 3   )

#define  AnscSnapcGetEtherType(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->EtherType)) )
#define  AnscSnapcSetEtherType(p)                    AnscWriteUshort(&p->EtherType, AnscUshortFromHToN(type))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
