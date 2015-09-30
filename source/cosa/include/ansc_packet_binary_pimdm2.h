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

    module:	ansc_packet_binary_pimdm2.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Protocol Independent Multicast -
        Dense Mode Version 2 (PIM-DM V2). 
        Based on draft-ietf-pim-dm-new-v2-03.txt. February 2003.
        Support RFC2932, RFC 2934, and draft-ietf-pim-mib-v2-01.txt.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        07/10/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_PIMDM2_
#define  _ANSC_PACKET_BINARY_PIMDM2_

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif


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



/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */

/***********************************************************
                 CONSTANTS USED BY PIMDM2 MSG
***********************************************************/

/*
 * The Routing Inforamtion Protocol Version 2(PIMDM2) is an Interior Gateway routing
 * Protocol that has very little overhead in terms of bandwidth used and 
 * configuration and management time. It is based on the Bellman-Ford algorithm
 * for calculating routs, and is suitable for small networks.
 */

#define  PIMDM2_MULTICAST_IPADDR                        AnscUlongFromHToN(0xe000000d) /* 224.0.0.13 */
#define  PIMDM2_ALL_PIM_ROUTERS_IPADDR                  PIMDM2_MULTICAST_IPADDR
#define  PIMDM2_INVALID_ROUTE_IPADDR                    0x00000000
#define  PIMDM2_SINGLE_IPADDR_MASK_LENGTH               32         /* bits */


#define  PIMDM_TRANSPORT                                103
#define  IP4_PROTOCOL_PIMDM                             103

#define  PIMDM2_VERSION_UNKNOWN                         0
#define  PIMDM2_VERSION_1                               1
#define  PIMDM2_VERSION_2                               2

#define  PIMDM2_MSG_TYPE_HELLO                          0
#define  PIMDM2_MSG_TYPE_REGISTER                       1
#define  PIMDM2_MSG_TYPE_REG_STOP                       2
#define  PIMDM2_MSG_TYPE_JOIN_PRUNE                     3
#define  PIMDM2_MSG_TYPE_BOOTSTRAP                      4
#define  PIMDM2_MSG_TYPE_ASSERT                         5
#define  PIMDM2_MSG_TYPE_GRAFT                          6
#define  PIMDM2_MSG_TYPE_GRAFT_ACK                      7
#define  PIMDM2_MSG_TYPE_CAND_RP_ADVS                   8
#define  PIMDM2_MSG_TYPE_STATE_REFRESH                  9

#define  PIMDM2_METRIC_DEFAULT                          1
#define  PIMDM2_METRIC_MINIMUM                          1
#define  PIMDM2_METRIC_INFINITY                         0xFFFFFFFF
#define  PIMDM2_PREFERENCE_INFINITY                     0x7FFFFFFF

#define  PIMDM2_ADDR_FAMILY_RESERVED                    0
#define  PIMDM2_ADDR_FAMILY_IPV4                        1
#define  PIMDM2_ADDR_FAMILY_IPV6                        2
#define  PIMDM2_AF_ENCODING_TYPE_RESERVED               0

/*
 * for infinite assert metric
 */
#define  PIMDM2_ASSERT_METRIC_NORMAL_RPT                0
#define  PIMDM2_ASSERT_METRIC_INFI_RPT                  1
#define  PIMDM2_ASSERT_METRIC_INFI_PREF                 PIMDM2_PREFERENCE_INFINITY
#define  PIMDM2_ASSERT_METRIC_INFI_METRIC               PIMDM2_METRIC_INFINITY
#define  PIMDM2_ASSERT_METRIC_INFI_IPADDR               0



/*
 * for Hello Message Options
 */
#define PIMDM2_HO_HOLD_TIME_TYPE                        1
#define PIMDM2_HO_LAN_PRUNE_DELAY_TYPE                  2
#define PIMDM2_HO_GENERATION_ID_TYPE                    20
#define PIMDM2_HO_STATE_REFRESH_CBL_TYPE                21

#define PIMDM2_HO_HOLD_TIME_LENGTH                      2
#define PIMDM2_HO_LAN_PRUNE_DELAY_LENGTH                4
#define PIMDM2_HO_GENERATION_ID_LENGTH                  4
#define PIMDM2_HO_STATE_REFRESH_CBL_LENGTH              4

#define PIMDM2_HO_HOLD_TIME_TIMEOUT_NOW                 0
#define PIMDM2_HO_HOLD_TIME_TIMEOUT_NEVER               0xFFFF

#define PIMDM2_HO_STATE_REFRESH_CBL_VERSION_1           1

/*
 * for Join/Prune message
 */
#define PIMDM2_JP_HOLD_TIME_TIMEOUT_NEVER               0xFFFF


/********************************************************************************
                 DATA STRUCTURES USED BY PIMDM2
*********************************************************************************/
typedef  struct
_PIMDM2_HEADER
{
    UCHAR                           VersionType;
    UCHAR                           Reserved;
    USHORT                          Checksum;
    UCHAR                           Payload[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PIMDM2_HEADER,  *PPIMDM2_HEADER;

#define  PimDm2HeaderGetVersion(p)                      ( p->VersionType >> 4                                   )
#define  PimDm2HeaderSetVersion(p, v)                   ( p->VersionType = ((v) << 4) | (p->VersionType & 0x0F) )
#define  PimDm2HeaderGetMsgType(p)                      ( p->VersionType & 0x0F                                 )
#define  PimDm2HeaderSetMsgType(p, v)                   ( p->VersionType = (v )     | (p->VersionType & 0xF0)   )
#define  PimDm2HeaderGetReserved(p)                     ( p->Reserved                                           )
#define  PimDm2HeaderSetReserved(p, v)                  ( p->Reserved    = v                                    )
#define  PimDm2HeaderGetChecksum(p)                     ( AnscReadUshort(&p->Checksum)                          )
#define  PimDm2HeaderSetChecksum(p, c)                  AnscWriteUshort(&p->Checksum,          c                )
#define  PimDm2HeaderGetPayload(p)                      ( p->Payload                                            )

typedef  struct
_PIMDM2_ENCODED_UNI_ADDR
{
    UCHAR                           AddrFamily;
    UCHAR                           EncodingType;
    ANSC_IPV4_ADDRESS               UnicastAddr;
}_struct_pack_
PIMDM2_ENCODED_UNI_ADDR,  *PPIMDM2_ENCODED_UNI_ADDR;

#define  PIMDM2_ENCODED_UNI_ADDR_SIZE                   sizeof(PIMDM2_ENCODED_UNI_ADDR)

#define  PimDm2EncUniAddrGetAddrFamily(p)               ( p->AddrFamily                                         )
#define  PimDm2EncUniAddrSetAddrFamily(p, v)            ( p->AddrFamily = v                                     )
#define  PimDm2EncUniAddrGetEncodingType(p)             ( p->EncodingType                                       )
#define  PimDm2EncUniAddrSetEncodingType(p, v)          ( p->EncodingType = v                                   )
#define  PimDm2EncUniAddrGetUnicastAddrValue(p)         ( AnscReadUlong(&p->UnicastAddr.Value)                  )
#define  PimDm2EncUniAddrGetUnicastAddrDot(p)           ( p->UnicastAddr.Dot                                    )
#define  PimDm2EncUniAddrSetUnicastAddrValue(p, s)       AnscWriteUlong (&p->UnicastAddr,   s                   )
#define  PimDm2EncUniAddrSetUnicastAddrDot(p, s)         AnscWriteUlong (&p->UnicastAddr,   *(PULONG)s          )

typedef  struct
_PIMDM2_ENCODED_GROUP_ADDR
{
    UCHAR                           AddrFamily;
    UCHAR                           EncodingType;
    UCHAR                           BReservedZ;
    UCHAR                           MaskLength;
    ANSC_IPV4_ADDRESS               GroupMulAddr;
}_struct_pack_
PIMDM2_ENCODED_GROUP_ADDR,  *PPIMDM2_ENCODED_GROUP_ADDR;

#define  PIMDM2_ENCODED_GROUP_ADDR_SIZE                 sizeof(PIMDM2_ENCODED_GROUP_ADDR)

#define  PIMDM2_ENCGRPADDR_B_SHIFT                      ((UCHAR)7)
#define  PIMDM2_ENCGRPADDR_B_MASK                       ((UCHAR)(1 << PIMDM2_ENCGRPADDR_B_SHIFT))
#define  PIMDM2_ENCGRPADDR_Z_SHIFT                      ((UCHAR)0)
#define  PIMDM2_ENCGRPADDR_Z_MASK                       ((UCHAR)(1 << PIMDM2_ENCGRPADDR_Z_SHIFT))
#define  PIMDM2_ENCGRPADDR_RESERVED_SHIFT               ((UCHAR)1)
#define  PIMDM2_ENCGRPADDR_RESERVED_MASK                ((UCHAR)0x7E)


#define  PimDm2EncGrpAddrGetAddrFamily(p)               ( p->AddrFamily                                         )
#define  PimDm2EncGrpAddrSetAddrFamily(p, v)            ( p->AddrFamily = v                                     )
#define  PimDm2EncGrpAddrGetEncodingType(p)             ( p->EncodingType                                       )
#define  PimDm2EncGrpAddrSetEncodingType(p, v)          ( p->EncodingType = v                                   )
#define  PimDm2EncGrpAddrGetB(p)                        ( (p->BReservedZ & PIMDM2_ENCGRPADDR_B_MASK) >> PIMDM2_ENCGRPADDR_B_SHIFT                    )
#define  PimDm2EncGrpAddrSetB(p, v)                     ( p->BReservedZ = (UCHAR)((((UCHAR)v) << PIMDM2_ENCGRPADDR_B_SHIFT) & PIMDM2_ENCGRPADDR_B_MASK) | (p->BReservedZ & (~PIMDM2_ENCGRPADDR_B_MASK)) )
#define  PimDm2EncGrpAddrGetZ(p)                        ( (p->BReservedZ & PIMDM2_ENCGRPADDR_Z_MASK) >> PIMDM2_ENCGRPADDR_Z_SHIFT                    )
#define  PimDm2EncGrpAddrSetZ(p, v)                     ( p->BReservedZ = (UCHAR)((((UCHAR)v) << PIMDM2_ENCGRPADDR_Z_SHIFT) & PIMDM2_ENCGRPADDR_Z_MASK) | (p->BReservedZ & (~PIMDM2_ENCGRPADDR_Z_MASK)) )
#define  PimDm2EncGrpAddrGetReserved(p)                 ( (p->BReservedZ & PIMDM2_ENCGRPADDR_RESERVED_MASK) >> PIMDM2_ENCGRPADDR_RESERVED_SHIFT                    )
#define  PimDm2EncGrpAddrSetReserved(p, v)              ( p->BReservedZ = (UCHAR)((((UCHAR)v) << PIMDM2_ENCGRPADDR_RESERVED_SHIFT) & PIMDM2_ENCGRPADDR_RESERVED_MASK) | (p->BReservedZ & (~PIMDM2_ENCGRPADDR_RESERVED_MASK)) )
#define  PimDm2EncGrpAddrGetMaskLength(p)               ( p->MaskLength                                          )
#define  PimDm2EncGrpAddrSetMaskLength(p, v)            ( p->MaskLength = (UCHAR)v                               )
#define  PimDm2EncGrpAddrGetGroupMulAddrValue(p)        ( AnscReadUlong(&p->GroupMulAddr.Value)                  )
#define  PimDm2EncGrpAddrGetGroupMulAddrDot(p)          ( p->GroupMulAddr.Dot                                    )
#define  PimDm2EncGrpAddrSetGroupMulAddrValue(p, s)      AnscWriteUlong (&p->GroupMulAddr,   s                   )
#define  PimDm2EncGrpAddrSetGroupMulAddrDot(p, s)        AnscWriteUlong (&p->GroupMulAddr,   *(PULONG)s          )


typedef  struct
_PIMDM2_ENCODED_SOURCE_ADDR
{
    UCHAR                           AddrFamily;
    UCHAR                           EncodingType;
    UCHAR                           ReservedSWR;
    UCHAR                           MaskLength;
    ANSC_IPV4_ADDRESS               SourceAddr;
}_struct_pack_
PIMDM2_ENCODED_SOURCE_ADDR,  *PPIMDM2_ENCODED_SOURCE_ADDR;

#define  PIMDM2_ENCODED_SOURCE_ADDR_SIZE                sizeof(PIMDM2_ENCODED_SOURCE_ADDR)

#define  PIMDM2_ENCSRCADDR_S_SHIFT                      ((UCHAR)2)
#define  PIMDM2_ENCSRCADDR_S_MASK                       ((UCHAR)(1 << PIMDM2_ENCSRCADDR_S_SHIFT))
#define  PIMDM2_ENCSRCADDR_W_SHIFT                      ((UCHAR)1)
#define  PIMDM2_ENCSRCADDR_W_MASK                       ((UCHAR)(1 << PIMDM2_ENCSRCADDR_W_SHIFT))
#define  PIMDM2_ENCSRCADDR_R_SHIFT                      ((UCHAR)0)
#define  PIMDM2_ENCSRCADDR_R_MASK                       ((UCHAR)(1 << PIMDM2_ENCSRCADDR_R_SHIFT))
#define  PIMDM2_ENCSRCADDR_RESERVED_SHIFT               ((UCHAR)3)
#define  PIMDM2_ENCSRCADDR_RESERVED_MASK                ((UCHAR)(1 << PIMDM2_ENCSRCADDR_RESERVED_SHIFT))

#define  PimDm2EncSrcAddrGetAddrFamily(p)               ( p->AddrFamily                                         )
#define  PimDm2EncSrcAddrSetAddrFamily(p, v)            ( p->AddrFamily = v                                     )
#define  PimDm2EncSrcAddrGetEncodingType(p)             ( p->EncodingType                                       )
#define  PimDm2EncSrcAddrSetEncodingType(p, v)          ( p->EncodingType = v                                   )
#define  PimDm2EncSrcAddrGetS(p)                        ( (p->ReservedSWR & PIMDM2_ENCSRCADDR_S_MASK) >> PIMDM2_ENCSRCADDR_S_SHIFT                    )
#define  PimDm2EncSrcAddrSetS(p, v)                     ( p->ReservedSWR = (((v) << PIMDM2_ENCSRCADDR_S_SHIFT) & PIMDM2_ENCSRCADDR_S_MASK) | (p->ReservedSWR & (~PIMDM2_ENCSRCADDR_S_MASK)) )
#define  PimDm2EncSrcAddrGetW(p)                        ( (p->ReservedSWR & PIMDM2_ENCSRCADDR_W_MASK) >> PIMDM2_ENCSRCADDR_W_SHIFT                    )
#define  PimDm2EncSrcAddrSetW(p, v)                     ( p->ReservedSWR = (((v) << PIMDM2_ENCSRCADDR_W_SHIFT) & PIMDM2_ENCSRCADDR_W_MASK) | (p->ReservedSWR & (~PIMDM2_ENCSRCADDR_W_MASK)) )
#define  PimDm2EncSrcAddrGetR(p)                        ( (p->ReservedSWR & PIMDM2_ENCSRCADDR_R_MASK) >> PIMDM2_ENCSRCADDR_R_SHIFT                    )
#define  PimDm2EncSrcAddrSetR(p, v)                     ( p->ReservedSWR = (((v) << PIMDM2_ENCSRCADDR_R_SHIFT) & PIMDM2_ENCSRCADDR_R_MASK) | (p->ReservedSWR & (~PIMDM2_ENCSRCADDR_R_MASK)) )
#define  PimDm2EncSrcAddrGetReserved(p)                 ( (p->ReservedSWR & PIMDM2_ENCSRCADDR_RESERVED_MASK) >> PIMDM2_ENCSRCADDR_RESERVED_SHIFT                    )
#define  PimDm2EncSrcAddrSetReserved(p, v)              ( p->ReservedSWR = (((v) << PIMDM2_ENCSRCADDR_RESERVED_SHIFT) & PIMDM2_ENCSRCADDR_RESERVED_MASK) | (p->ReservedSWR & (~PIMDM2_ENCSRCADDR_RESERVED_MASK)) )
#define  PimDm2EncSrcAddrGetMaskLength(p)               ( p->MaskLength                                         )
#define  PimDm2EncSrcAddrSetMaskLength(p, v)            ( p->MaskLength = v                                     )
#define  PimDm2EncSrcAddrGetSourceAddrValue(p)          ( AnscReadUlong(&p->SourceAddr.Value)                  )
#define  PimDm2EncSrcAddrGetSourceAddrDot(p)            ( p->SourceAddr.Dot                                    )
#define  PimDm2EncSrcAddrSetSourceAddrValue(p, s)       AnscWriteUlong (&p->SourceAddr,   s                   )
#define  PimDm2EncSrcAddrSetSourceAddrDot(p, s)         AnscWriteUlong (&p->SourceAddr,   *(PULONG)s          )

/*
 * hello options 
 */
typedef  struct
_PIMDM2_HELLO_OPTION
{
    USHORT                          OptionType;
    USHORT                          OptionLength;
    UCHAR                           OptionValue[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PIMDM2_HELLO_OPTION,  *PPIMDM2_HELLO_OPTION;

#define  PimDm2HelloOptionGetOptionType(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->OptionType   )) )
#define  PimDm2HelloOptionSetOptionType(p, v)           AnscWriteUshort(&p->OptionType,       AnscUshortFromHToN(v ))
#define  PimDm2HelloOptionGetOptionLength(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->OptionLength   )) )
#define  PimDm2HelloOptionSetOptionLength(p, v)         AnscWriteUshort(&p->OptionLength,       AnscUshortFromHToN(v ))
#define  PimDm2HelloOptionGetOptionValue(p)             ( p->OptionValue                                            )
#define  PimDm2HelloOptionGetTotalLength(p)             ( 4 + PimDm2HelloOptionGetOptionLength(p)                )

typedef  struct
_PIMDM2_HO_HOLD_TIME
{
    USHORT                          OptionType;
    USHORT                          OptionLength;
    USHORT                          HoldTime;           /* in seconds */
}_struct_pack_
PIMDM2_HO_HOLD_TIME,  *PPIMDM2_HO_HOLD_TIME;



#define  PimDm2HoHoldTimeGetOptionType(p)               PimDm2HelloOptionGetOptionType(p)
#define  PimDm2HoHoldTimeSetOptionType(p)               PimDm2HelloOptionSetOptionType(p, PIMDM2_HO_HOLD_TIME_TYPE)
#define  PimDm2HoHoldTimeGetOptionLength(p)             PimDm2HelloOptionGetOptionLength(p)   
#define  PimDm2HoHoldTimeSetOptionLength(p)             PimDm2HelloOptionSetOptionLength(p, PIMDM2_HO_HOLD_TIME_LENGTH)
#define  PimDm2HoHoldTimeGetHoldTime(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->HoldTime   )) )
#define  PimDm2HoHoldTimeSetHoldTime(p, v)              AnscWriteUshort(&p->HoldTime,       AnscUshortFromHToN((USHORT)v ))

typedef  struct
_PIMDM2_HO_LAN_PRUNE_DELAY
{
    USHORT                          OptionType;
    USHORT                          OptionLength;
    USHORT                          TLanPruneDelay;           /* in milliseconds */
    USHORT                          OverrideInterval;         /* in milliseconds */

}_struct_pack_
PIMDM2_HO_LAN_PRUNE_DELAY,  *PPIMDM2_HO_LAN_PRUNE_DELAY;

#define  PIMDM2_HO_LPDELAY_T_SHIFT                      15
#define  PIMDM2_HO_LPDELAY_T_MASK                       (1 << PIMDM2_HO_LPDELAY_T_SHIFT)
#define  PIMDM2_HO_LPDELAY_LPD_SHIFT                    0
#define  PIMDM2_HO_LPDELAY_LPD_MASK                     0x7FFF


#define  PimDm2HoLPDelayGetOptionType(p)                PimDm2HelloOptionGetOptionType(p)
#define  PimDm2HoLPDelaySetOptionType(p)                PimDm2HelloOptionSetOptionType(p, PIMDM2_HO_LAN_PRUNE_DELAY_TYPE)
#define  PimDm2HoLPDelayGetOptionLength(p)              PimDm2HelloOptionGetOptionLength(p)   
#define  PimDm2HoLPDelaySetOptionLength(p)              PimDm2HelloOptionSetOptionLength(p, PIMDM2_HO_LAN_PRUNE_DELAY_LENGTH)
#define  PimDm2HoLPDelayGetTLanPruneDelay(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->TLanPruneDelay   )) )   
#define  PimDm2HoLPDelaySetTLanPruneDelay(p, v)         AnscWriteUshort(&p->TLanPruneDelay,       AnscUshortFromHToN((USHORT)v ))

#define  PimDm2HoLPDelayGetT(p)                         (UCHAR)( (PimDm2HoLPDelayGetTLanPruneDelay(p) & PIMDM2_HO_LPDELAY_T_MASK) >> PIMDM2_HO_LPDELAY_T_SHIFT)
#define  PimDm2HoLPDelaySetT(p, v)                      \
         PimDm2HoLPDelaySetTLanPruneDelay(p, (((v << PIMDM2_HO_LPDELAY_T_SHIFT) & PIMDM2_HO_LPDELAY_T_MASK) | (PimDm2HoLPDelayGetTLanPruneDelay(p) & (~PIMDM2_HO_LPDELAY_T_MASK))))
#define  PimDm2HoLPDelayGetLPDelay(p)                   ( PimDm2HoLPDelayGetTLanPruneDelay(p) & PIMDM2_HO_LPDELAY_LPD_MASK )
#define  PimDm2HoLPDelaySetLPDelay(p, v)                \
         PimDm2HoLPDelaySetTLanPruneDelay(p, ((((USHORT)v) & PIMDM2_HO_LPDELAY_LPD_MASK) | (PimDm2HoLPDelayGetTLanPruneDelay(p) & (~PIMDM2_HO_LPDELAY_LPD_MASK))))
#define  PimDm2HoLPDelayGetOverrideInterval(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->OverrideInterval   )) )   
#define  PimDm2HoLPDelaySetOverrideInterval(p, v)       AnscWriteUshort(&p->OverrideInterval,       AnscUshortFromHToN((USHORT)v ))


typedef  struct
_PIMDM2_HO_GENERATION_ID
{
    USHORT                          OptionType;
    USHORT                          OptionLength;
    ULONG                           GenerationId;           

}_struct_pack_
PIMDM2_HO_GENERATION_ID,  *PPIMDM2_HO_GENERATION_ID;

#define  PimDm2HoGenIdGetOptionType(p)                PimDm2HelloOptionGetOptionType(p)
#define  PimDm2HoGenIdSetOptionType(p)                PimDm2HelloOptionSetOptionType(p, PIMDM2_HO_GENERATION_ID_TYPE)
#define  PimDm2HoGenIdGetOptionLength(p)              PimDm2HelloOptionGetOptionLength(p)   
#define  PimDm2HoGenIdSetOptionLength(p)              PimDm2HelloOptionSetOptionLength(p, PIMDM2_HO_GENERATION_ID_LENGTH)
#define  PimDm2HoGenIdGetGenerationId(p)              ( AnscUlongFromNToH(AnscReadUlong(&p->GenerationId   )) )   
#define  PimDm2HoGenIdSetGenerationId(p, v)           AnscWriteUlong(&p->GenerationId,       AnscUlongFromHToN(v ))


typedef  struct
_PIMDM2_HO_STATE_REFRESH_CBL
{
    USHORT                          OptionType;
    USHORT                          OptionLength;
    UCHAR                           Version;
    UCHAR                           Interval;           /* in seconds */
    USHORT                          Reserved;           

}_struct_pack_
PIMDM2_HO_STATE_REFRESH_CBL,  *PPIMDM2_HO_STATE_REFRESH_CBL;

#define  PimDm2HoSTCblGetOptionType(p)                PimDm2HelloOptionGetOptionType(p)
#define  PimDm2HoSTCblSetOptionType(p)                PimDm2HelloOptionSetOptionType(p, PIMDM2_HO_STATE_REFRESH_CBL_TYPE)
#define  PimDm2HoSTCblGetOptionLength(p)              PimDm2HelloOptionGetOptionLength(p)   
#define  PimDm2HoSTCblSetOptionLength(p)              PimDm2HelloOptionSetOptionLength(p, PIMDM2_HO_STATE_REFRESH_CBL_LENGTH)
#define  PimDm2HoSTCblGetVersion(p)                   ( p->Version                                            )
#define  PimDm2HoSTCblSetVersion(p, v)                ( p->Version    = (UCHAR)v                              )
#define  PimDm2HoSTCblGetInterval(p)                  ( p->Interval                                           )
#define  PimDm2HoSTCblSetInterval(p, v)               ( p->Interval   = (UCHAR)v                              )
#define  PimDm2HoSTCblGetReserved(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved   )) )   
#define  PimDm2HoSTCblSetReserved(p, v)               AnscWriteUshort(&p->Reserved, AnscUshortFromHToN(v ))


/* 
 * hello message
 */
typedef  PIMDM2_HEADER                                PIMDM2_HELLO_MSG, *PPIMDM2_HELLO_MSG;

#define  PimDm2HelloMsgGetVersion(p)                  PimDm2HeaderGetVersion(p)    
#define  PimDm2HelloMsgSetVersion(p, v)               PimDm2HeaderSetVersion(p, v) 
#define  PimDm2HelloMsgGetMsgType(p)                  PimDm2HeaderGetMsgType(p)    
#define  PimDm2HelloMsgSetMsgType(p, v)               PimDm2HeaderSetMsgType(p, v) 
#define  PimDm2HelloMsgGetReserved(p)                 PimDm2HeaderGetReserved(p)   
#define  PimDm2HelloMsgSetReserved(p, v)              PimDm2HeaderSetReserved(p, v)
#define  PimDm2HelloMsgGetChecksum(p)                 PimDm2HeaderGetChecksum(p)   
#define  PimDm2HelloMsgSetChecksum(p, c)              PimDm2HeaderSetChecksum(p, c)
#define  PimDm2HelloMsgGetPayload(p)                  PimDm2HeaderGetPayload(p)    
#define  PimDm2HelloMsgGetFirstOption(p)              ( (PPIMDM2_HELLO_OPTION)PimDm2HeaderGetPayload(p) )    
#define  PimDm2HelloMsgGetNextOption(p)               ( (PPIMDM2_HELLO_OPTION)((ULONG)p + PimDm2HelloOptionGetTotalLength(p)) )  


/*
 * Join/Prune group block
 */
typedef  struct
_PIMDM2_JOIN_PRUNE_GROUP
{
    PIMDM2_ENCODED_GROUP_ADDR       GroupAddr;
    USHORT                          JoinedSrcNum;
    USHORT                          PrunedSrcNum;
    PIMDM2_ENCODED_SOURCE_ADDR      SrcAddrArray[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PIMDM2_JOIN_PRUNE_GROUP,  *PPIMDM2_JOIN_PRUNE_GROUP;

#define  PimDm2JPGroupGetGroupAddr(p)                   ( &p->GroupAddr                                         )

#define  PimDm2JPGroupGetJoinedSrcNum(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->JoinedSrcNum   )) )
#define  PimDm2JPGroupSetJoinedSrcNum(p, v)             AnscWriteUshort(&p->JoinedSrcNum, AnscUshortFromHToN((USHORT)v ))
#define  PimDm2JPGroupGetPrunedSrcNum(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->PrunedSrcNum   )) )
#define  PimDm2JPGroupSetPrunedSrcNum(p, v)              AnscWriteUshort(&p->PrunedSrcNum, AnscUshortFromHToN((USHORT)v ))
#define  PimDm2JPGroupGetTotalSize(p)                   \
         ( PIMDM2_ENCODED_GROUP_ADDR_SIZE + 4 +         \
           (PimDm2JPGroupGetJoinedSrcNum(p) + PimDm2JPGroupGetPrunedSrcNum(p)) * PIMDM2_ENCODED_SOURCE_ADDR_SIZE )

#define  PimDm2JPGroupGetFirstSrcAddr(p)                (p->SrcAddrArray)    
#define  PimDm2JPGroupGetSrcAddrByAbsIndex(p, i)        ( &p->SrcAddrArray[i] )
#define  PimDm2JPGroupGetJoinedSrcAddr(p, i)            ( &p->SrcAddrArray[i] )
#define  PimDm2JPGroupGetPrunedSrcAddr(p, i)            ( &p->SrcAddrArray[i + PimDm2JPGroupGetJoinedSrcNum(p)] )

/*
 * JOIN/PRUNE message
 */
typedef  struct
_PIMDM2_JOIN_PRUNE_MSG
{
    UCHAR                           VersionType;
    UCHAR                           Reserved;
    USHORT                          Checksum;

    PIMDM2_ENCODED_UNI_ADDR         UpNbrAddr;
    UCHAR                           JPReserved;
    UCHAR                           GroupNum;
    USHORT                          HoldTime;
    ANSC_HANDLE                     JPGroupArray[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PIMDM2_JOIN_PRUNE_MSG,  *PPIMDM2_JOIN_PRUNE_MSG;

#define  PimDm2JPMsgGetVersion(p)                    PimDm2HeaderGetVersion(p)    
#define  PimDm2JPMsgSetVersion(p, v)                 PimDm2HeaderSetVersion(p, v) 
#define  PimDm2JPMsgGetMsgType(p)                    PimDm2HeaderGetMsgType(p)    
#define  PimDm2JPMsgSetMsgType(p, v)                 PimDm2HeaderSetMsgType(p, v) 
#define  PimDm2JPMsgGetReserved(p)                   PimDm2HeaderGetReserved(p)   
#define  PimDm2JPMsgSetReserved(p, v)                PimDm2HeaderSetReserved(p, v)
#define  PimDm2JPMsgGetChecksum(p)                   PimDm2HeaderGetChecksum(p)   
#define  PimDm2JPMsgSetChecksum(p, c)                PimDm2HeaderSetChecksum(p, c)

#define  PimDm2JPMsgGetUpNbrAddr(p)                  ( &p->UpNbrAddr                                         )
#define  PimDm2JPMsgGetJPReserved(p)                 ( p->JPReserved)  
#define  PimDm2JPMsgSetJPReserved(p, v)              ( p->JPReserved = v)
#define  PimDm2JPMsgGetGroupNum(p)                   ( p->GroupNum)  
#define  PimDm2JPMsgSetGroupNum(p, v)                ( p->GroupNum = (UCHAR)v)
#define  PimDm2JPMsgGetHoldTime(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->HoldTime   )) )
#define  PimDm2JPMsgSetHoldTime(p, v)                AnscWriteUshort(&p->HoldTime, AnscUshortFromHToN((USHORT)v ))
#define  PimDm2JPMsgGetJPGroupArray(p)               ( p->JPGroupArray                                         )

#define  PimDm2JPMsgGetFirstJPGroup(p)               ( (PPIMDM2_JOIN_PRUNE_GROUP)PimDm2JPMsgGetJPGroupArray(p) )  
#define  PimDm2JPMsgGetNextJPGroup(p)                ( (PPIMDM2_JOIN_PRUNE_GROUP)((ULONG)p + PimDm2JPGroupGetTotalSize(p)) )  

/*
 * graft message
 */
typedef  PIMDM2_JOIN_PRUNE_MSG                   PIMDM2_GRAFT_MSG, *PPIMDM2_GRAFT_MSG;

#define  PimDm2GraftMsgGetVersion(p)                    PimDm2JPMsgGetVersion(p)    
#define  PimDm2GraftMsgSetVersion(p, v)                 PimDm2JPMsgSetVersion(p, v) 
#define  PimDm2GraftMsgGetMsgType(p)                    PimDm2JPMsgGetMsgType(p)    
#define  PimDm2GraftMsgSetMsgType(p, v)                 PimDm2JPMsgSetMsgType(p, v) 
#define  PimDm2GraftMsgGetReserved(p)                   PimDm2JPMsgGetReserved(p)   
#define  PimDm2GraftMsgSetReserved(p, v)                PimDm2JPMsgSetReserved(p, v)
#define  PimDm2GraftMsgGetChecksum(p)                   PimDm2JPMsgGetChecksum(p)   
#define  PimDm2GraftMsgSetChecksum(p, c)                PimDm2JPMsgSetChecksum(p, c)

#define  PimDm2GraftMsgGetUpNbrAddr(p)                  PimDm2JPMsgGetUpNbrAddr(p)     
#define  PimDm2GraftMsgGetJPReserved(p)                 PimDm2JPMsgGetJPReserved(p)    
#define  PimDm2GraftMsgSetJPReserved(p, v)              PimDm2JPMsgSetJPReserved(p, v) 
#define  PimDm2GraftMsgGetGroupNum(p)                   PimDm2JPMsgGetGroupNum(p)      
#define  PimDm2GraftMsgSetGroupNum(p, v)                PimDm2JPMsgSetGroupNum(p, v)   
#define  PimDm2GraftMsgGetHoldTime(p)                   PimDm2JPMsgGetHoldTime(p)      
#define  PimDm2GraftMsgSetHoldTime(p, v)                PimDm2JPMsgSetHoldTime(p, v)   
#define  PimDm2GraftMsgGetJPGroupArray(p)               PimDm2JPMsgGetJPGroupArray(p)  

#define  PimDm2GraftMsgGetFirstJPGroup(p)               PimDm2JPMsgGetFirstJPGroup(p)  
#define  PimDm2GraftMsgGetNextJPGroup(p)                PimDm2JPMsgGetNextJPGroup(p) 

/*
 * graft ack message
 */
typedef  PIMDM2_JOIN_PRUNE_MSG                   PIMDM2_GFT_ACK_MSG, *PPIMDM2_GFT_ACK_MSG;

#define  PimDm2GftAckMsgGetVersion(p)                    PimDm2JPMsgGetVersion(p)    
#define  PimDm2GftAckMsgSetVersion(p, v)                 PimDm2JPMsgSetVersion(p, v) 
#define  PimDm2GftAckMsgGetMsgType(p)                    PimDm2JPMsgGetMsgType(p)    
#define  PimDm2GftAckMsgSetMsgType(p, v)                 PimDm2JPMsgSetMsgType(p, v) 
#define  PimDm2GftAckMsgGetReserved(p)                   PimDm2JPMsgGetReserved(p)   
#define  PimDm2GftAckMsgSetReserved(p, v)                PimDm2JPMsgSetReserved(p, v)
#define  PimDm2GftAckMsgGetChecksum(p)                   PimDm2JPMsgGetChecksum(p)   
#define  PimDm2GftAckMsgSetChecksum(p, c)                PimDm2JPMsgSetChecksum(p, c)

#define  PimDm2GftAckMsgGetUpNbrAddr(p)                  PimDm2JPMsgGetUpNbrAddr(p)     
#define  PimDm2GftAckMsgGetJPReserved(p)                 PimDm2JPMsgGetJPReserved(p)    
#define  PimDm2GftAckMsgSetJPReserved(p, v)              PimDm2JPMsgSetJPReserved(p, v) 
#define  PimDm2GftAckMsgGetGroupNum(p)                   PimDm2JPMsgGetGroupNum(p)      
#define  PimDm2GftAckMsgSetGroupNum(p, v)                PimDm2JPMsgSetGroupNum(p, v)   
#define  PimDm2GftAckMsgGetHoldTime(p)                   PimDm2JPMsgGetHoldTime(p)      
#define  PimDm2GftAckMsgSetHoldTime(p, v)                PimDm2JPMsgSetHoldTime(p, v)   
#define  PimDm2GftAckMsgGetJPGroupArray(p)               PimDm2JPMsgGetJPGroupArray(p)  

#define  PimDm2GftAckMsgGetFirstJPGroup(p)               PimDm2JPMsgGetFirstJPGroup(p)  
#define  PimDm2GftAckMsgGetNextJPGroup(p)                PimDm2JPMsgGetNextJPGroup(p) 

/*
 * Assert message
 */
typedef  struct
_PIMDM2_ASSERT_MSG
{
    UCHAR                           VersionType;
    UCHAR                           Reserved;
    USHORT                          Checksum;

    PIMDM2_ENCODED_GROUP_ADDR       GroupAddr;
    PIMDM2_ENCODED_UNI_ADDR         SourceAddr;
    ULONG                           RMetricPref;
    ULONG                           Metric;
}_struct_pack_
PIMDM2_ASSERT_MSG,  *PPIMDM2_ASSERT_MSG;

#define  PimDm2AssertMsgGetVersion(p)                   PimDm2HeaderGetVersion(p)    
#define  PimDm2AssertMsgSetVersion(p, v)                PimDm2HeaderSetVersion(p, v) 
#define  PimDm2AssertMsgGetMsgType(p)                   PimDm2HeaderGetMsgType(p)    
#define  PimDm2AssertMsgSetMsgType(p, v)                PimDm2HeaderSetMsgType(p, v) 
#define  PimDm2AssertMsgGetReserved(p)                  PimDm2HeaderGetReserved(p)   
#define  PimDm2AssertMsgSetReserved(p, v)               PimDm2HeaderSetReserved(p, v)
#define  PimDm2AssertMsgGetChecksum(p)                  PimDm2HeaderGetChecksum(p)   
#define  PimDm2AssertMsgSetChecksum(p, c)               PimDm2HeaderSetChecksum(p, c)

#define  PimDm2AssertMsgGetGroupAddr(p)                 ( &p->GroupAddr                                         )
#define  PimDm2AssertMsgGetSourceAddr(p)                ( &p->SourceAddr                                         )
#define  PimDm2AssertMsgGetRMetricPref(p)               AnscUlongFromNToH( AnscReadUlong(&p->RMetricPref) ) 
#define  PimDm2AssertMsgSetRMetricPref(p, v)            AnscWriteUlong(&p->RMetricPref,       AnscUlongFromHToN(v ))
#define  PimDm2AssertMsgGetR(p)                         ( (PimDm2AssertMsgGetRMetricPref(p) & 0x80000000) >> 31 ) 
#define  PimDm2AssertMsgSetR(p, v)                      \
         PimDm2AssertMsgSetRMetricPref(p, (((v << 31) & 0x80000000) | (PimDm2AssertMsgGetRMetricPref(p) & 0x7FFFFFFF)))
#define  PimDm2AssertMsgGetMetricPref(p)                 ( PimDm2AssertMsgGetRMetricPref(p) & 0x7FFFFFFF )
#define  PimDm2AssertMsgSetMetricPref(p, v)             \
         PimDm2AssertMsgSetRMetricPref(p, (((v) & 0x7FFFFFFF) | (PimDm2AssertMsgGetRMetricPref(p) & 0x80000000)))
#define  PimDm2AssertMsgGetMetric(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Metric   )) )
#define  PimDm2AssertMsgSetMetric(p, v)                 AnscWriteUlong(&p->Metric,       AnscUlongFromHToN(v ))


/*
 * State Refresh message
 */
typedef  struct
_PIMDM2_STATE_REFRESH_MSG
{
    UCHAR                           VersionType;
    UCHAR                           Reserved;
    USHORT                          Checksum;

    PIMDM2_ENCODED_GROUP_ADDR       GroupAddr;
    PIMDM2_ENCODED_UNI_ADDR         SourceAddr;
    PIMDM2_ENCODED_UNI_ADDR         OrigAddr;
    ULONG                           RMetricPref;
    ULONG                           Metric;
    UCHAR                           MaskLength;
    UCHAR                           TTL;
    UCHAR                           PNOReserved;
    UCHAR                           Interval;
}_struct_pack_
PIMDM2_STATE_REFRESH_MSG,  *PPIMDM2_STATE_REFRESH_MSG;

#define  PIMDM2_STREF_MSG_P_SHIFT                       7
#define  PIMDM2_STREF_MSG_P_MASK                        ( 1 << PIMDM2_STREF_MSG_P_SHIFT )
#define  PIMDM2_STREF_MSG_N_SHIFT                       6
#define  PIMDM2_STREF_MSG_N_MASK                        0x40
#define  PIMDM2_STREF_MSG_O_SHIFT                       5
#define  PIMDM2_STREF_MSG_O_MASK                        0x20
#define  PIMDM2_STREF_MSG_RSV_SHIFT                     0
#define  PIMDM2_STREF_MSG_RSV_MASK                      0x10


#define  PimDm2StRefMsgGetVersion(p)                    PimDm2HeaderGetVersion(p)    
#define  PimDm2StRefMsgSetVersion(p, v)                 PimDm2HeaderSetVersion(p, v) 
#define  PimDm2StRefMsgGetMsgType(p)                    PimDm2HeaderGetMsgType(p)    
#define  PimDm2StRefMsgSetMsgType(p, v)                 PimDm2HeaderSetMsgType(p, v) 
#define  PimDm2StRefMsgGetReserved(p)                   PimDm2HeaderGetReserved(p)   
#define  PimDm2StRefMsgSetReserved(p, v)                PimDm2HeaderSetReserved(p, v)
#define  PimDm2StRefMsgGetChecksum(p)                   PimDm2HeaderGetChecksum(p)   
#define  PimDm2StRefMsgSetChecksum(p, c)                PimDm2HeaderSetChecksum(p, c)

#define  PimDm2StRefMsgGetGroupAddr(p)                  ( &p->GroupAddr                                         )
#define  PimDm2StRefMsgGetSourceAddr(p)                 ( &p->SourceAddr                                         )
#define  PimDm2StRefMsgGetOrigAddr(p)                   ( &p->OrigAddr                                         )
#define  PimDm2StRefMsgGetRMetricPref(p)                ( AnscUlongFromNToH(AnscReadUlong(&p->RMetricPref   )) )
#define  PimDm2StRefMsgSetRMetricPref(p, v)             AnscWriteUlong(&p->RMetricPref,       AnscUlongFromHToN(v ))
#define  PimDm2StRefMsgGetR(p)                          (( PimDm2StRefMsgGetRMetricPref(p) & 0x80000000) >> 31)
#define  PimDm2StRefMsgSetR(p, v)                       \
         PimDm2StRefMsgSetRMetricPref(p, (((v << 31) & 0x80000000) | (PimDm2StRefMsgGetRMetricPref(p) & 0x7FFFFFFF)))
#define  PimDm2StRefMsgGetMetricPref(p)                 ( PimDm2StRefMsgGetRMetricPref(p) & 0x7FFFFFFF )
#define  PimDm2StRefMsgSetMetricPref(p, v)              \
         PimDm2StRefMsgSetRMetricPref(p, (((v) & 0x7FFFFFFF) | (PimDm2StRefMsgGetRMetricPref(p) & 0x80000000)))
#define  PimDm2StRefMsgGetMetric(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Metric   )) )
#define  PimDm2StRefMsgSetMetric(p, v)                  AnscWriteUlong(&p->Metric,       AnscUlongFromHToN(v ))
#define  PimDm2StRefMsgGetMaskLength(p)                 ( p->MaskLength                                         )
#define  PimDm2StRefMsgSetMaskLength(p, v)              ( p->MaskLength = (UCHAR)v                              )
#define  PimDm2StRefMsgGetTTL(p)                        ( p->TTL                                         )
#define  PimDm2StRefMsgSetTTL(p, v)                     ( p->TTL = (UCHAR)v                                   )
#define  PimDm2StRefMsgGetInterval(p)                   ( p->Interval                                         )
#define  PimDm2StRefMsgSetInterval(p, v)                ( p->Interval = (UCHAR)v                              )

#define  PimDm2StRefMsgGetP(p)                          ( (p->PNOReserved  & PIMDM2_STREF_MSG_P_MASK) >> PIMDM2_STREF_MSG_P_SHIFT )
#define  PimDm2StRefMsgSetP(p, v)                       \
        ( p->PNOReserved = (UCHAR)(((v << PIMDM2_STREF_MSG_P_SHIFT) & PIMDM2_STREF_MSG_P_MASK)  | \
                                   (p->PNOReserved & (~PIMDM2_STREF_MSG_P_MASK)) ) )
#define  PimDm2StRefMsgGetN(p)                          ( (p->PNOReserved  & PIMDM2_STREF_MSG_N_MASK) >> PIMDM2_STREF_MSG_N_SHIFT )
#define  PimDm2StRefMsgSetN(p, v)                       \
        ( p->PNOReserved = ((v << PIMDM2_STREF_MSG_N_SHIFT) & PIMDM2_STREF_MSG_N_MASK)  | \
                           (p->PNOReserved & (~PIMDM2_STREF_MSG_N_MASK)) )
#define  PimDm2StRefMsgGetO(p)                          ( (p->PNOReserved  & PIMDM2_STREF_MSG_O_MASK) >> PIMDM2_STREF_MSG_O_SHIFT )
#define  PimDm2StRefMsgSetO(p, v)                       \
        ( p->PNOReserved = (UCHAR)(((v << PIMDM2_STREF_MSG_O_SHIFT) & PIMDM2_STREF_MSG_O_MASK)  | \
                                   (p->PNOReserved & (~PIMDM2_STREF_MSG_O_MASK)) ) )
#define  PimDm2StRefMsgGetRsv(p)                        ( (p->PNOReserved  & PIMDM2_STREF_MSG_RSV_MASK) >> PIMDM2_STREF_MSG_RSV_SHIFT )
#define  PimDm2StRefMsgSetRsv(p, v)                     \
        ( p->PNOReserved = (UCHAR)(((v << PIMDM2_STREF_MSG_RSV_SHIFT) & PIMDM2_STREF_MSG_RSV_MASK)  | \
                                   (p->PNOReserved & (~PIMDM2_STREF_MSG_RSV_MASK)) ))




#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
