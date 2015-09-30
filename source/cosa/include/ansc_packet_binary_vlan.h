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

    module:	ansc_packet_binary_vlan.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the VLAN Tag as defined in IEEE 802.1Q.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/30/07    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_VLAN_
#define  _ANSC_PACKET_BINARY_VLAN_


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
 * Define some const values used in the VLAN tag format definition.
 */

/*
 * The E-RIF that can appear in a VLAN tag may be used to encode the rif_information parameter of
 * the corresponding EISS M_UNITDATA.request and is a modification of the RIF as defined in C3.3.2
 * of IEEE Std 802.1D.
 */
#define  VLAN_ERIF_ROUTE_TYPE_MASK                  0xE0
#define  VLAN_ERIF_LENGTH_MASK                      0x1F

typedef  struct
_VLAN_EMBEDDED_RIF
{
    UCHAR                           TypeAndLength;
    UCHAR                           DirectionAndLf;
}_struct_pack_
VLAN_EMBEDDED_RIF,  *PVLAN_EMBEDDED_RIF;

#define  AnscVlanErifGetRouteType(erif)             ( (erif->TypeAndLength & VLAN_ERIF_ROUTE_TYPE_MASK) >> 5 )
#define  AnscVlanErifGetLength(erif)                ( (erif->TypeAndLength & VLAN_ERIF_LENGTH_MASK    ) )

/*
 * Define the VLAN tag format. The VLAN TAG TCI field is two octets in length and encodes the
 * vlan_identifier and priority parameters of the corresponding EISS M_UNITDATA.request as unsigned
 * binary numbers and a Canonical Format Indicator (CFI) as a single bit.
 */
#define  VLAN_TAG_PCP_MASK                          0xE000
#define  VLAN_TAG_CFI_MASK                          0x1000
#define  VLAN_TAG_VID_MASK                          0x0FFF

typedef  struct
_VLAN_TAG
{
    union
    {
        UCHAR                       Bytes[2];
        USHORT                      Word;
    }TCI;
    USHORT                          FrameType;
    VLAN_EMBEDDED_RIF               Erif[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
VLAN_TAG,  *PVLAN_TAG;

#define  AnscVlanTagGetPriority(vt)                 ( (vt->TCI.Bytes[0] & 0xE0) >> 5 )
#define  AnscVlanTagGetCanonicalFormat(vt)          ( (vt->TCI.Bytes[0] & 0x10) != 0 )
#define  AnscVlanTagGetVlanId(vt)                   ( (AnscUshortFromNToH(vt->TCI.Word) & VLAN_TAG_VID_MASK) )
#define  AnscVlanTagGetLength(vt)                   ( AnscVlanTagGetCanonicalFormat(vt)? sizeof(VLAN_TAG) + AnscVlanErifGetLength((&vt->Erif[0])) : sizeof(VLAN_TAG) )
#define  AnscVlanTagGetFrameType(vt)                ( AnscUshortFromNToH(vt->FrameType) )

#define  AnscVlanTagSetPriority(vt, pcp)            ( vt->TCI.Bytes[0] = (vt->TCI.Bytes[0] & ~0xE0) | (pcp << 5) )
#define  AnscVlanTagSetCanonicalFormat(vt, cfi)                                             \
         {                                                                                  \
            if ( cfi )                                                                      \
            {                                                                               \
                vt->TCI.Bytes[0] |= 0x10;                                                   \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                vt->TCI.Bytes[0] &= ~0x10;                                                  \
            }                                                                               \
         }
#define  AnscVlanTagSetVlanId(vt, vid)              ( vt->TCI.Word  = AnscUshortFromHToN((USHORT)((AnscUshortFromNToH(vt->TCI.Word) & ~(USHORT)VLAN_TAG_VID_MASK) | vid)) )
#define  AnscVlanTagSetFrameType(vt, ft)            ( vt->FrameType = AnscUshortFromHToN(ft) )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
