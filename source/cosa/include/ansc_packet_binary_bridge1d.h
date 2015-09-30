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

    module:	ansc_packet_binary_bridge1d.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the control packets used by the bridge 
        defined in IEEE 802.1d and IEEE 802.1w.

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


#ifndef  _ANSC_PACKET_BINARY_BRIDGE1D_
#define  _ANSC_PACKET_BINARY_BRIDGE1D_


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


/************************************************************************
                 DATA STRUCTURES USED BY SPANNING TREE PROTOCOL
*************************************************************************/

/*
 * 802.1d 7.12.3 table 7-9
 */
#define  BRIDGE1D_GROUP_ADDR_MAC                    0x0180C2000000
#define  BRIDGE1D_GROUP_ADDR_MAC_LOW4               0xC2000000
#define  BRIDGE1D_GROUP_ADDR_MAC_HIGH2              0x0180

/*
 * according to 802.1t 9.2.5, the most significent first 4 bits are
 * used for the settable priority, and the next most significent 12 bits 
 * comprises a locally assigned system ID extension for VLAN support.
 * the siz least significent octets are derived from the bridge MAC address.
 */
typedef  struct
_BRIDGE1D_STP_BRIDGEID
{
    USHORT                          BridgePriority;
    UCHAR                           BridgeMacAddress[ANSC_MAC_ADDRESS_SIZE];
}_struct_pack_
BRIDGE1D_STP_BRIDGEID,  *PBRIDGE1D_STP_BRIDGEID;

#define  Bridge1dStpBridgeIdGetPriority(p)            ( AnscUshortFromNToH((USHORT)(AnscReadUshort((p)->BridgePriority) & 0xF000)))
#define  Bridge1dStpBridgeIdGetSystemId(p)            ( AnscUshortFromNToH((USHORT)(AnscReadUshort((p)->BridgePriority) & 0x0FFF)))
#define  Bridge1dStpBridgeIdGetGetBridgeMacAddress(p) ( (p)->BridgeMacAddress )

#define  Bridge1dStpBridgeIdSetPriority(p, v)         AnscWriteUshort(&(p)->BridgePriority, AnscUshortFromHToN((USHORT)(((v) & 0xF000) + Bridge1dStpBridgeIdGetSystemId(p))))
#define  Bridge1dStpBridgeIdSetSystemId(p, s)         AnscWriteUshort(&(p)->BridgePriority, AnscUshortFromHToN((USHORT)(((s) & 0x0FFF) + Bridge1dStpBridgeIdGetPriority(p))))
#define  Bridge1dStpBridgeIdSetBridgeMacAddress(p, a) ( AnscCopyMemory(p->BridgeMacAddress, a, ANSC_MAC_ADDRESS_SIZE) )

#define  BRIDGE1D_STP_BRIDGEID_A_BETTERTHAN_B         0x01
#define  BRIDGE1D_STP_BRIDGEID_A_WORSETHAN_B          0x02
#define  BRIDGE1D_STP_BRIDGEID_A_EQUALTO_B            0x03

/*
 * this macro checks which bridge ID has better priority. In 802.1d 8.2, the lower
 * numerical value indicating better priority.
 */
#define  Bridge1dStpBridgeIdCompare(bid_a, bid_b, result)                                   \
        {                                                                                   \
            ULONG bid_a_word0   = AnscUlongFromNToH(AnscReadUlong(bid_a));                  \
            ULONG bid_a_word1   = AnscUlongFromNToH(AnscReadUlong(&(((PUCHAR)bid_a)[4])));  \
            ULONG bid_b_word0   = AnscUlongFromNToH(AnscReadUlong(bid_b));                  \
            ULONG bid_b_word1   = AnscUlongFromNToH(AnscReadUlong(&(((PUCHAR)bid_b)[4])));  \
                                                                                            \
            if ( bid_a_word0 > bid_b_word0 )                                                \
            {                                                                               \
                result = BRIDGE1D_STP_BRIDGEID_A_BETTERTHAN_B;                              \
            }                                                                               \
            else if ( bid_a_word0 < bid_b_word0 )                                           \
            {                                                                               \
                result = BRIDGE1D_STP_BRIDGEID_A_WORSETHAN_B;                               \
            }                                                                               \
            else if ( bid_a_word1 < bid_b_word1 )                                           \
            {                                                                               \
                result = BRIDGE1D_STP_BRIDGEID_A_BETTERTHAN_B;                              \
            }                                                                               \
            else if ( bid_a_word1 > bid_b_word1 )                                           \
            {                                                                               \
                result = BRIDGE1D_STP_BRIDGEID_A_WORSETHAN_B;                               \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                result = BRIDGE1D_STP_BRIDGEID_A_EQUALTO_B;                                 \
            }                                                                               \
        }

/*
 * according to 802.1t 9.2.7, the most significent first 4 bits are
 * used for the port priority, and the rest 12 bits are for the link Id.
 * the link ID should be unique in the bridge to identify the port
 */
typedef  USHORT  BRIDGE1D_STP_PORTID,  *PBRIDGE1D_STP_PORTID;

#define  Bridge1dStpPortIdGetPriority(p)            ((USHORT)((AnscUshortFromNToH(AnscReadUshort(p)) & 0xF000) >> 8))
#define  Bridge1dStpPortIdGetLinkId(p)              ((USHORT)( AnscUshortFromNToH(AnscReadUshort(p)) & 0x0FFF) )

#define  Bridge1dStpPortIdSetPriority(p, v)         AnscWriteUshort(p, AnscUshortFromHToN((USHORT)(((v << 8) & 0xF000) + Bridge1dStpPortIdGetLinkId(p))))
#define  Bridge1dStpPortIdSetLinkId(p, l)           AnscWriteUshort(p, AnscUshortFromHToN((USHORT)((AnscUshortFromNToH(AnscReadUshort(p)) & 0xF000) + ((USHORT)l & 0x0FFF))))
#define  Bridge1dStpPortIdReset(p)                  (*(p) = 0)

#define  BRIDGE1D_STP_PORTID_A_BETTERTHAN_B         0x01
#define  BRIDGE1D_STP_PORTID_A_WORSETHAN_B          0x02
#define  BRIDGE1D_STP_PORTID_A_EQUALTO_B            0x03

/*
 * this macro checks which port ID has better priority. In 802.1d 8.2, the lower
 * numerical value indicating better priority.
 */
#define  Bridge1dStpPortIdCompare(pid_a, pid_b, result)                                     \
        {                                                                                   \
            USHORT pid_a_word0   = AnscUshortFromNToH(AnscReadUshort(pid_a));               \
            USHORT pid_b_word0   = AnscUshortFromNToH(AnscReadUshort(pid_b));               \
                                                                                            \
            if ( pid_a_word0 > pid_b_word0 )                                                \
            {                                                                               \
                result = BRIDGE1D_STP_PORTID_A_BETTERTHAN_B;                                \
            }                                                                               \
            else if ( pid_a_word0 < pid_b_word0 )                                           \
            {                                                                               \
                result = BRIDGE1D_STP_PORTID_A_WORSETHAN_B;                                 \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                result = BRIDGE1D_STP_PORTID_A_EQUALTO_B;                                   \
            }                                                                               \
        }

/*
 * defines constants like flags masks etc.
 * protocol ID are defined in 802.1d 9.3.1 and 12.11.2.1
 */
#define  BRIDGE1D_STP_BPDU_PROTOCOLID               0x0000
#define  BRIDGE1D_GARP_BPDU_PROTOCOLID              0x0001

#define  BRIDGE1D_STP_BPDU_PROTOCOLVER_0            0x00
#define  BRIDGE1D_STP_BPDU_PROTOCOLVER_1            0x01
#define  BRIDGE1D_STP_BPDU_PROTOCOLVER_2            0x02

#define  BRIDGE1D_STP_BPDU_BPDUTYPE_CONFIGBPDU      0x00
#define  BRIDGE1D_STP_BPDU_BPDUTYPE_TCN             0x80
#define  BRIDGE1D_STP_BPDU_BPDUTYPE_RSTP            0x02

#define  BRIDGE1D_STP_BPDU_FLAGS_TC_MASK            0x01
#define  BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK      0x02
#define  BRIDGE1D_STP_BPDU_FLAGS_PORTROLE_MASK      0x0C
#define  BRIDGE1D_STP_BPDU_FLAGS_LEARNING_MASK      0x10
#define  BRIDGE1D_STP_BPDU_FLAGS_FORWARDING_MASK    0x20
#define  BRIDGE1D_STP_BPDU_FLAGS_AGREEMENT_MASK     0x40
#define  BRIDGE1D_STP_BPDU_FLAGS_TCA_MASK           0x80

#define  BRIDGE1D_STP_BPDU_FLAGS_PORTROLE_SHIFT     2

#define  BRIDGE1D_RSTP_BPDU_PORTROLE_UNKNOWN        0x00
#define  BRIDGE1D_RSTP_BPDU_PORTROLE_ALT_BKP        0x01
#define  BRIDGE1D_RSTP_BPDU_PORTROLE_ROOT           0x02
#define  BRIDGE1D_RSTP_BPDU_PORTROLE_DESIGNATED     0x03

#define  BRIDGE1D_RSTP_TIMES_UNITS_IN_SECOND        256
#define  BRIDGE1D_RSTP_TIMES_UNITS_IN_SECOND_MASK   255
#define  BRIDGE1D_RSTP_TIMES_UNITS_IN_SECOND_SHIFT  8

#define  BRIDGE1D_STP_MAX_HEAD_ROOM                 24
#define  BRIDGE1D_STP_MAX_TAIL_ROOM                 4


/*
 * all time related fields are represented in 2 bytes, representing an unsigned 
 * binary number multiplied by a unit of time of 1/256 of a second, i.e., seconds
 * shifted by 8 bits. It can represnt time in the range 0 to, but not including,
 * 256 seconds.
 */
typedef  struct
_BRIDGE1D_STP_BPDU
{
    USHORT                          ProtocolId;
    UCHAR                           ProtocolVersion;
    UCHAR                           BpduType;
    UCHAR                           Flags;
    BRIDGE1D_STP_BRIDGEID           RootIdentifier;
    ULONG                           RootPathCost;
    BRIDGE1D_STP_BRIDGEID           BridgeIdentifier;
    BRIDGE1D_STP_PORTID             PortIdentifier;
    USHORT                          MessageAge;
    USHORT                          MaxAge;
    USHORT                          HelloTime;
    USHORT                          ForwardDelay;
}_struct_pack_
BRIDGE1D_STP_BPDU,  *PBRIDGE1D_STP_BPDU;

typedef  struct
_BRIDGE1D_RSTP_BPDU
{
    USHORT                          ProtocolId;
    UCHAR                           ProtocolVersion;
    UCHAR                           BpduType;
    UCHAR                           Flags;
    BRIDGE1D_STP_BRIDGEID           RootIdentifier;
    ULONG                           RootPathCost;
    BRIDGE1D_STP_BRIDGEID           BridgeIdentifier;
    BRIDGE1D_STP_PORTID             PortIdentifier;
    USHORT                          MessageAge;
    USHORT                          MaxAge;
    USHORT                          HelloTime;
    USHORT                          ForwardDelay;
    UCHAR                           Version1Length;
}_struct_pack_
BRIDGE1D_RSTP_BPDU,  *PBRIDGE1D_RSTP_BPDU;

typedef  struct
_BRIDGE1D_STP_TCN
{
    USHORT                          ProtocolId;
    UCHAR                           ProtocolVersion;
    UCHAR                           BpduType;
}_struct_pack_
BRIDGE1D_STP_TCN,  *PBRIDGE1D_STP_TCN;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  Bridge1dStpBpduGetProtocolId(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->ProtocolId)))
#define  Bridge1dStpBpduGetProtocolVersion(p)       p->ProtocolVersion
#define  Bridge1dStpBpduGetBpduType(p)              p->BpduType
#define  Bridge1dStpBpduGetFlags(p)                 p->Flags
#define  Bridge1dStpBpduGetRootIdentifier(p)        p->RootIdentifier
#define  Bridge1dStpBpduGetRootPathCost(p)          ( AnscUlongFromNToH(AnscReadUlong(&p->RootPathCost)))
#define  Bridge1dStpBpduGetBridgeIdentifier(p)      p->BridgeIdentifier
#define  Bridge1dStpBpduGetPortIdentifier(p)        p->PortIdentifier
#define  Bridge1dStpBpduGetMessageAge(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->MessageAge)))
#define  Bridge1dStpBpduGetMaxAge(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->MaxAge)))
#define  Bridge1dStpBpduGetHelloTime(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->HelloTime)))
#define  Bridge1dStpBpduGetForwardDelay(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->ForwardDelay)))
#define  Bridge1dRstpBpduGetVersion1Length(p)       p->Version1Length

#define  Bridge1dStpBpduSetProtocolId(p, v)         AnscWriteUshort(&p->ProtocolId, AnscUshortFromHToN(v))
#define  Bridge1dStpBpduSetProtocolVersion(p, v)    (p->ProtocolVersion = v)
#define  Bridge1dStpBpduSetBpduType(p, v)           (p->BpduType = v)
#define  Bridge1dStpBpduSetFlags(p, v)              (p->Flags = v)
#define  Bridge1dStpBpduSetRootIdentifier(p, pBid)  p->RootIdentifier = *pBid
#define  Bridge1dStpBpduSetRootPathCost(p, v)       AnscWriteUlong(&p->RootPathCost, AnscUlongFromHToN(v))
#define  Bridge1dStpBpduSetBridgeIdentifier(p, pId) p->BridgeIdentifier = *pId
#define  Bridge1dStpBpduSetPortIdentifier(p, pPid)  p->PortIdentifier = *pPid
#define  Bridge1dStpBpduSetMessageAge(p, v)         AnscWriteUshort(&p->MessageAge, AnscUshortFromHToN(v))
#define  Bridge1dStpBpduSetMaxAge(p, v)             AnscWriteUshort(&p->MaxAge, AnscUshortFromHToN(v))
#define  Bridge1dStpBpduSetHelloTime(p, v)          AnscWriteUshort(&p->HelloTime, AnscUshortFromHToN(v))
#define  Bridge1dStpBpduSetForwardDelay(p, v)       AnscWriteUshort(&p->ForwardDelay, AnscUshortFromHToN(v))
#define  Bridge1dRstpBpduSetVersion1Length(p, v)   (p->Version1Length = v)

#define  Bridge1dStpBpduIsFlagsTc(p)                ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_TC_MASK       ) != 0 )
#define  Bridge1dStpBpduIsFlagsProposal(p)          ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK ) != 0 )
#define  Bridge1dStpBpduGetFlagsPortRole(p)         ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK ) >> BRIDGE1D_STP_BPDU_FLAGS_PORTROLE_SHIFT )
#define  Bridge1dStpBpduIsFlagsLearning(p)          ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_LEARNING_MASK ) != 0 )
#define  Bridge1dStpBpduIsFlagsForwarding(p)        ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_FORWARDING_MASK ) != 0 )
#define  Bridge1dStpBpduIsFlagsAgreement(p)         ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_AGREEMENT_MASK ) != 0 )
#define  Bridge1dStpBpduIsFlagsTca(p)               ( (p->Flags & BRIDGE1D_STP_BPDU_FLAGS_TCA_MASK ) != 0 )

#define  Bridge1dStpBpduSetFlagsTc(p)               ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_TC_MASK          )
#define  Bridge1dStpBpduResetFlagsTc(p)             ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_TC_MASK      ) )
#define  Bridge1dStpBpduSetFlagsProposal(p)         ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK    )
#define  Bridge1dStpBpduResetFlagsProposal(p)       ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK) )
#define  Bridge1dStpBpduSetFlagsPortRole(p, v)      ( p->Flags = (p->Flags & (~BRIDGE1D_STP_BPDU_FLAGS_PROPOSAL_MASK)) | (v << BRIDGE1D_STP_BPDU_FLAGS_PORTROLE_SHIFT) )
#define  Bridge1dStpBpduSetFlagsLearning(p)         ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_LEARNING_MASK    )
#define  Bridge1dStpBpduResetFlagsLearning(p)       ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_LEARNING_MASK) )

#define  Bridge1dStpBpduSetFlagsForwarding(p)       ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_FORWARDING_MASK    )
#define  Bridge1dStpBpduResetFlagsForwarding(p)     ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_FORWARDING_MASK) )
#define  Bridge1dStpBpduSetFlagsAgreement(p)        ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_AGREEMENT_MASK    )
#define  Bridge1dStpBpduResetFlagsAgreement(p)      ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_AGREEMENT_MASK) )
#define  Bridge1dStpBpduSetFlagsTca(p)              ( p->Flags |= BRIDGE1D_STP_BPDU_FLAGS_TCA_MASK    )
#define  Bridge1dStpBpduResetFlagsTca(p)            ( p->Flags &= (~BRIDGE1D_STP_BPDU_FLAGS_TCA_MASK) )

#define  Bridge1dStpTcnGetProtocolId(p)            ( AnscUshortFromNToH(AnscReadUshort(p->ProtocolId)))
#define  Bridge1dStpTcnGetProtocolVersion(p)       p->ProtocolVersion
#define  Bridge1dStpTcnGetBpduType(p)              p->BpduType

#define  Bridge1dStpTcnSetProtocolId(p, v)         AnscWriteUshort(&p->ProtocolId, AnscUshortFromHToN(v))
#define  Bridge1dStpTcnSetProtocolVersion(p, v)    (p->ProtocolVersion = v)
#define  Bridge1dStpTcnSetBpduType(p, v)           (p->BpduType = v)

#define  Bridge1dStpGetAllocateMsgSize(msg_size, allo_msg_size)                             \
        {                                                                                   \
            allo_msg_size = ( ( msg_size + BRIDGE1D_STP_MAX_HEAD_ROOM +                     \
                                BRIDGE1D_STP_MAX_TAIL_ROOM              ) & (~0x3) ) + 0x4; \
        }

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
