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

    module:	ansc_packet_binary_ospf2.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Open Shortest Path First (OSPF) Version 2
        (OSPF2). Support RFC2328.


        The OSPF Version 2(OSPF2) is an Interior Gateway routing
        Protocol that is based on the link-state or SPF technology.
        It has explicit support for CIDR and the tagging of externally-derived
        routing information.


    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_OSPF2_
#define  _ANSC_PACKET_BINARY_OSPF2_


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


/*---------------------------------------------------------------------------------
                        CONSTANTS USED BY OSPF2

    OSPF2_IP_PROTOCOL_NUM                   OSPF is directly above IP 
    OSPF2_IP_TOS_BITS                       OSPF packets are sents using the normal service 
    OSPF2_IP_PRECEDENCE                     OSPF has precedence of internetwork control 
    OSPF2_IP_TOS_FIELD                      OSPF has set the IP header TOS field to be 0b11000000 
    OSPF2_LS_REFRESH_TIME                   30 minutes. The max time between distinct originations
                                            of any particular LSA. 
    OSPF2_MIN_LS_INTERVAL                   5 seconds. The min time between distinct
                                            orginations of any particular LSA 
    OSPF2_MIN_LS_ARRIVAL                    1 second. The min time that must elapse
                                            between reception of new LSA instances 
                                            during flooding 
    OSPF2_MAX_AGE                           60 minutes. The max age that an LSA can attain. 
    OSPF2_CHECK_AGE                         5 minutes. When the age of an LSA in the link state
                                            database hits a multiple of this value, the LSA's
                                            checksum is verified 
    OSPF2_MAX_AGE_DIFF                      15 minutes. The max time dispersion that can occur,
                                            as an LSA is flooded throughout the AS. 
    OSPF2_LS_INFINITY                       The metric value indicating that the destination
                                            described by an LSA is unreachable 
    OSPF2_DEFAULT_DESTINATION               The destination ID that indicates the default route 
    OSPF2_INIT_SEQ_NUM                      The value used for LS sequence number when 
                                            originating the first instance of any LSA 
    OSPF2_MAX_SEQ_NUM                       The max value that LS sequence num can attain 
    OSPF2_IP_TTL_MULTICAST                  Packets sent to the multicast addresses
                                            should never be forwarded; they are meant to travel
                                            a single hop only
    OSPF2_IP_TTL_UNICAST                    Packets sent to the unicast addresses
                                            could be forwarded if the link is a virtual link
    OSPF2_ROUTER_PRIORITY_NON_DR           

    OSPF2_AREA_ID_BACKBONE                     
    OSPF2_AREA_ID_UNDEFINED                    
 --------------------------------------------------------------------------------*/

#define  OSPF2_IP_PROTOCOL_NUM                      89     
#define  OSPF2_VERSION_NUM                          2     
#define  OSPF2_IP_TOS_BITS                          0           
#define  OSPF2_IP_PRECEDENCE                        6           
#define  OSPF2_IP_TOS_FIELD                         0xc0        
#define  OSPF2_LS_REFRESH_TIME                      1800        
#define  OSPF2_MIN_LS_INTERVAL                      5           
#define  OSPF2_MIN_LS_ARRIVAL                       1           
#define  OSPF2_MAX_AGE                              3600        
#define  OSPF2_CHECK_AGE                            300         
#define  OSPF2_MAX_AGE_DIFF                         900         
#define  OSPF2_LS_INFINITY                          0x00ffffff  
#define  OSPF2_DEFAULT_DESTINATION                  0x00000000  
#define  OSPF2_RESERVED_SEQ_NUM                     0x80000000  
#define  OSPF2_INIT_SEQ_NUM                         0x80000001  
#define  OSPF2_MAX_SEQ_NUM                          0x7fffffff  
#define  OSPF2_ROUTER_PRIORITY_NON_DR               0
#define  AnscOspf2IsDrEligible(pri)                 ( pri == OSPF2_ROUTER_PRIORITY_NON_DR? \
                                                      FALSE:TRUE)

#define  OSPF2_IP_TTL_MULTICAST                     1
#define  OSPF2_IP_TTL_UNICAST                       30           
           


#define  OSPF2_AREA_ID_BACKBONE                     0
#define  OSPF2_AREA_ID_UNDEFINED                    0xffffffff

/*---------------------------------------------------------------------------------
                        PACKET RELATED PARAMETERS

    OSPF2_PKT_SIZE_MAX_DEFAULT              576 bytes. To avoid IP fragmentation, the OSPF size
                                            over virtual links should be below this value unless
                                            path MTU Discovery is being performed. 

    OSPF2_MULTICAST_ALLSPFROUTERS           224.0.0.5, all routers running OSPF should be
                                            prepared to receive packets sent to this address 
    OSPF2_MULTICAST_ALLDROUTERS             224.0.0.6, both the designated router and backup
                                            designated router must be
                                            prepared to receive packets sent to this address 

 --------------------------------------------------------------------------------*/

#define  OSPF2_MULTICAST_ALLSPFROUTERS              AnscUlongFromHToN(0xe0000005)
#define  OSPF2_MULTICAST_ALLDROUTERS                AnscUlongFromHToN(0xe0000006)
#define  OSPF2_HOST_MASK                            AnscUlongFromHToN(0xffffffff)
                                                                


#define  OSPF2_AUTH_TYPE_NULL                       0           /* Null authentication */
#define  OSPF2_AUTH_TYPE_PASSWORD                   1           /* Simple password */
#define  OSPF2_AUTH_TYPE_CRYPT                      2           /* Cryptographic authentication */
#define  OSPF2_AUTH_KEY_SIZE_PASSWORD               8           /* The auth key is of 8 bytes */
#define  OSPF2_AUTH_KEY_SIZE_MD5                    16          /* The auth key is of 16 bytes */

#define  OSPF_PKT_TYPE_HELLO                        1           /* Hello packet */ 
#define  OSPF_PKT_TYPE_DATABASE_DESC                2           /* Database description packet */ 
#define  OSPF_PKT_TYPE_LINK_STATE_REQ               3           /* Link State request */ 
#define  OSPF_PKT_TYPE_LINK_STATE_UPDATE            4           /* Link State update */ 
#define  OSPF_PKT_TYPE_LINK_STATE_ACK               5           /* Link State acknowledgment */ 



/*---------------------------------------------------------------------------------
                        OSPF2_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_PKT_AUTH_SIZE                         8           /* The authentication at the OSPF
                                                                   header is of 8 bytes */ 
#define  OSPF_PKT_HEADER_SIZE                       24          

typedef  struct
_OSPF2_HEADER
{
    UCHAR                           Version;
    UCHAR                           Type;
    USHORT                          PktLength;
    ANSC_IPV4_ADDRESS               RouterID;
    ULONG                           AreaID;
    USHORT                          Checksum;
    USHORT                          AuthType;
    UCHAR                           Authentication[OSPF_PKT_AUTH_SIZE];
    UCHAR                           Payload[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_HEADER,  *POSPF2_HEADER;

#define  AnscOspf2GetVersion(p)                     ( p->Version                                      )
#define  AnscOspf2GetType(p)                        ( p->Type                                         )
#define  AnscOspf2GetPktLength(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->PktLength)) )
#define  AnscOspf2GetRouterIDValue(p)               AnscReadUlong(&p->RouterID.Value                  )
#define  AnscOspf2GetRouterIDDot(p)                 (p->RouterID.Dot                                  )
#define  AnscOspf2GetAreaID(p)                      ( AnscUlongFromNToH(AnscReadUlong(&p->AreaID))    )
#define  AnscOspf2GetChecksum(p)                    ( AnscReadUshort(&p->Checksum)                    )
#define  AnscOspf2GetAuthType(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->AuthType))  )
#define  AnscOspf2GetAuthentication(p)              ( p->Authentication                               )
#define  AnscOspf2GetPayload(p)                     ( p->Payload                                      )

#define  AnscOspf2SetVersion(p, v)                  ( p->Version = v                                  )
#define  AnscOspf2SetType(p, t)                     ( p->Type = t                                     )
#define  AnscOspf2SetPktLength(p, l)                AnscWriteUshort(&p->PktLength,       AnscUshortFromHToN((USHORT)l))
#define  AnscOspf2SetRouterIDValue(p, r)            AnscWriteUlong (&p->RouterID.Value,  r                    )
#define  AnscOspf2SetRouterIDDot(p, r)              AnscWriteUlong (&p->RouterID.Dot[0], *(PULONG)r           )
#define  AnscOspf2SetAreaID(p, a)                   AnscWriteUlong (&p->AreaID,          AnscUlongFromHToN(a) )
#define  AnscOspf2SetChecksum(p, c)                 AnscWriteUshort(&p->Checksum,        (c)                  )
#define  AnscOspf2SetAuthType(p, a)                 AnscWriteUshort(&p->AuthType,        AnscUshortFromHToN(a))
#define  AnscOspf2SetAuthentication(p, a, l)        AnscCopyMemory (p->Authentication, a, l                   )
#define  AnscOspf2ZeroAuthentication(p)             AnscZeroMemory (p->Authentication, OSPF_PKT_AUTH_SIZE     )

/*---------------------------------------------------------------------------------
                        OSPF2_CYPTO_AUTH

              Authentication Field in the OSPF packet header
              when Cryptographic authentication is employed
 --------------------------------------------------------------------------------*/
typedef  struct
_OSPF2_CYPTO_AUTH
{
    USHORT                          Reserved;
    UCHAR                           KeyID;
    UCHAR                           AuDataLength;
    ULONG                           CrptySeqNum;

}_struct_pack_
OSPF2_CYPTO_AUTH,  *POSPF2_CYPTO_AUTH;

#define  AnscOspf2CryptoGetReserved(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved))  )
#define  AnscOspf2CryptoGetKeyID(p)                 ( p->KeyID                                          )
#define  AnscOspf2CryptoGetAuDataLength(p)          ( p->AuDataLength                                   )
#define  AnscOspf2CryptoGetCrptySeqNum(p)           ( AnscUlongFromNToH(AnscReadUlong(&p->CrptySeqNum)) )

#define  AnscOspf2CryptoSetReserved(p, r)           AnscWriteUshort(&p->Reserved,       AnscUshortFromHToN(r)   )
#define  AnscOspf2CryptoSetKeyID(p, id)             ( p->KeyID = id                                             )
#define  AnscOspf2CryptoSetAuDataLength(p, len)     ( p->AuDataLength = len                                     )
#define  AnscOspf2CryptoSetCrptySeqNum(p, seq)      AnscWriteUlong(&p->CrptySeqNum,    AnscUlongFromHToN(seq)   )

/*---------------------------------------------------------------------------------
                        OSPF2_HELLO_HEADER

    The options field in the hello packets, database description packets and all LSAs
    is defined as

        +----------------------------------------+
        | *  | *  | DC | EA | N/P | MC | E  | *  |
        +----------------------------------------+
    
        DC:     describes the router's handling of demand circuits, as specified in RFC 1793 
        EA:     describes the router's willingness to receive and forward external-attributes-LSAs, 
                as specified in "The OSPF External Attributes LSA" 
        N/P:    describes the Handling of TYPE 7 LSAs, as specified in RFC 1587 
        MC:     describes whether IP multicast datagrams are forwarded according to the
                specifications in RFC 1584 
        E:      describes the way AS-external-LSAs are flooded. All AS-external-LSAs 
                have this bit set. rfc2328 section 12.1.2 

 --------------------------------------------------------------------------------*/
#define  OSPF_PKT_HELLO_HEADER_SIZE                 20          
#define  OSPF_PKT_HELLO_NEIGHBOR_SIZE               4          

#define  OSPF_OPTION_DC_SHIFT                       0x5         
#define  OSPF_OPTION_DC_MASK                        (0x1 << OSPF_OPTION_DC_SHIFT)
#define  OSPF_OPTION_EA_SHIFT                       0x4         
#define  OSPF_OPTION_EA_MASK                        (0x1 << OSPF_OPTION_EA_SHIFT)
#define  OSPF_OPTION_NP_SHIFT                       0x3         
#define  OSPF_OPTION_NP_MASK                        (0x1 << OSPF_OPTION_NP_SHIFT)
#define  OSPF_OPTION_MC_SHIFT                       0x2         
#define  OSPF_OPTION_MC_MASK                        (0x1 << OSPF_OPTION_MC_SHIFT)
#define  OSPF_OPTION_E_SHIFT                        0x1         
#define  OSPF_OPTION_E_MASK                         (0x1 << OSPF_OPTION_E_SHIFT)

#define  AnscOspf2OptionGetBitDC(option)            ( (option & OSPF_OPTION_DC_MASK) >> OSPF_OPTION_DC_SHIFT    )         
#define  AnscOspf2OptionGetBitEA(option)            ( (option & OSPF_OPTION_EA_MASK) >> OSPF_OPTION_EA_SHIFT    )         
#define  AnscOspf2OptionGetBitNP(option)            ( (option & OSPF_OPTION_NP_MASK) >> OSPF_OPTION_NP_SHIFT    )         
#define  AnscOspf2OptionGetBitMC(option)            ( (option & OSPF_OPTION_MC_MASK) >> OSPF_OPTION_MC_SHIFT    )         
#define  AnscOspf2OptionGetBitE(option)             ( (option & OSPF_OPTION_E_MASK)  >> OSPF_OPTION_E_SHIFT     )         


#define  AnscOspf2OptionSetBitDC(option)            ( option = (UCHAR)(option | OSPF_OPTION_DC_MASK)                   )         
#define  AnscOspf2OptionReSetBitDC(option)          ( option = (UCHAR)(option & ~OSPF_OPTION_DC_MASK)                  ) 
#define  AnscOspf2OptionSetBitEA(option)            ( option = (UCHAR)(option | OSPF_OPTION_EA_MASK)                   )         
#define  AnscOspf2OptionReSetBitEA(option)          ( option = (UCHAR)(option & ~OSPF_OPTION_EA_MASK)                  ) 
#define  AnscOspf2OptionSetBitNP(option)            ( option = (UCHAR)(option | OSPF_OPTION_NP_MASK)                   )         
#define  AnscOspf2OptionReSetBitNP(option)          ( option = (UCHAR)(option & ~OSPF_OPTION_NP_MASK)                  ) 
#define  AnscOspf2OptionSetBitMC(option)            ( option = (UCHAR)(option | OSPF_OPTION_MC_MASK)                   )         
#define  AnscOspf2OptionReSetBitMC(option)          ( option = (UCHAR)(option & ~OSPF_OPTION_MC_MASK)                  ) 
#define  AnscOspf2OptionSetBitE(option)             ( option = (UCHAR)(option | OSPF_OPTION_E_MASK)                    )         
#define  AnscOspf2OptionReSetBitE(option)           ( option = (UCHAR)(option & ~OSPF_OPTION_E_MASK)                   ) 


typedef  struct
_OSPF2_HELLO_HEADER
{
    ULONG                           NetworkMask;
    USHORT                          HelloInterval;
    UCHAR                           Options;
    UCHAR                           RtrPri;
    ULONG                           RtrDeadInterval;
    ULONG                           DRouter;
    ULONG                           BackupDRouter;
    ULONG                           Neighbor[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_HELLO_HEADER,  *POSPF2_HELLO_HEADER;

#define  AnscOspf2HelloGetNetworkMask(p)            ( AnscReadUlong(&p->NetworkMask)                            )
#define  AnscOspf2HelloGetHelloInterval(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->HelloInterval))     )
#define  AnscOspf2HelloGetOptions(p)                ( p->Options                                                )
#define  AnscOspf2HelloGetRtrPri(p)                 ( p->RtrPri                                                 )
#define  AnscOspf2HelloGetRtrDeadInterval(p)        ( AnscUlongFromNToH(AnscReadUlong(&p->RtrDeadInterval))     )
#define  AnscOspf2HelloGetDRouterValue(p)           ( AnscReadUlong(&p->DRouter)                                )
#define  AnscOspf2HelloGetDRouterDot(p)             ( (PUCHAR)(&p->DRouter)                                     )
#define  AnscOspf2HelloGetBackupValue(p)            ( AnscReadUlong(&p->BackupDRouter)                          )
#define  AnscOspf2HelloGetBackupDot(p)              ( (PUCHAR)(&p->BackupDRouter)                               )

#define  AnscOspf2HelloSetNetworkMask(p, n)         AnscWriteUlong (&p->NetworkMask,        (n) )
#define  AnscOspf2HelloSetHelloInterval(p, h)       AnscWriteUshort(&p->HelloInterval,      AnscUshortFromHToN(h)   )
#define  AnscOspf2HelloSetOptions(p, o)             ( p->Options = o                                                )
#define  AnscOspf2HelloSetRtrPri(p, r)              ( p->RtrPri = r                                                 )
#define  AnscOspf2HelloSetRtrDeadInterval(p, r)     AnscWriteUlong (&p->RtrDeadInterval,    AnscUlongFromHToN(r)    )
#define  AnscOspf2HelloSetDRouterValue(p, d)        AnscWriteUlong (&p->DRouter,            (d)                     )
#define  AnscOspf2HelloSetDRouterDot(p, d)          AnscWriteUlong (&p->DRouter,            *(PULONG)(d)            )
#define  AnscOspf2HelloSetBackupValue(p, b)         AnscWriteUlong (&p->BackupDRouter,      (b)                     )
#define  AnscOspf2HelloSetBackupDot(p, b)           AnscWriteUlong (&p->BackupDRouter,      *(PULONG)(b)            )

#define  AnscOspf2HelloGetFirstNeighbor(p)          ( p->Neighbor                                       )
#define  AnscOspf2HelloGetNextNeighbor(p)           ( (PULONG)((ULONG)p + OSPF_PKT_HELLO_NEIGHBOR_SIZE) )
#define  AnscOspf2HelloGetNeighborValue(p)          ( AnscReadUlong(p)                                  )
#define  AnscOspf2HelloGetNeighborDot(p)            ( (PUCHAR)p                                         )
#define  AnscOspf2HelloSetNeighborValue(p, n)       AnscWriteUlong (p,      n                           )
#define  AnscOspf2HelloSetNeighborDot(p, d)         AnscWriteUlong (p,      *(PULONG)d                  )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_HEADER
 --------------------------------------------------------------------------------*/

#define  OSPF2_LS_TYPE_ROUTER                       1
#define  OSPF2_LS_TYPE_NETWORK                      2
#define  OSPF2_LS_TYPE_SUM_NETWORK                  3
#define  OSPF2_LS_TYPE_SUM_ASBR                     4
#define  OSPF2_LS_TYPE_AS_EX                        5

#define  OSPF2_LSA_HEADER_SIZE                      20


typedef  struct
_OSPF2_LSA_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    LONG                            LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
}_struct_pack_
OSPF2_LSA_HEADER,  *POSPF2_LSA_HEADER;

#define  AnscOspf2LsaGetLSAge(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->LSAge))         )
#define  AnscOspf2LsaGetOptions(p)                  ( p->Options                                            )
#define  AnscOspf2LsaGetLSType(p)                   ( p->LSType                                             )
#define  AnscOspf2LsaGetLinkStateIDValue(p)         ( AnscReadUlong(&p->LinkStateID)                        )
#define  AnscOspf2LsaGetLinkStateIDDot(p)           ( (PUCHAR)(&p->LinkStateID)                             )
#define  AnscOspf2LsaGetAdvRouterValue(p)           ( AnscReadUlong(&p->AdvRouter)                          )
#define  AnscOspf2LsaGetAdvRouterDot(p)             ( (PUCHAR)(&p->AdvRouter)                               )
#define  AnscOspf2LsaGetLSSeqNum(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->LSSeqNum))        )
#define  AnscOspf2LsaGetLSChecksum(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->LSChecksum))    )
#define  AnscOspf2LsaGetLength(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->Length))        )
#define  AnscOspf2LsaGetPayLoad(p)                  ( (ANSC_HANDLE)((ULONG)p + OSPF2_LSA_HEADER_SIZE)       )
#define  AnscOspf2LsaGetPayLoadLen(p)               ( AnscOspf2LsaGetLength(p) - OSPF2_LSA_HEADER_SIZE      )

#define  AnscOspf2LsaSetLSAge(p, l)                 AnscWriteUshort (&p->LSAge,             AnscUshortFromHToN(l) )
#define  AnscOspf2LsaSetOptions(p, o)               ( p->Options = o                                  )
#define  AnscOspf2LsaSetLSType(p, l)                ( p->LSType = l                                   )
#define  AnscOspf2LsaSetLinkStateIDValue(p, l)      AnscWriteUlong (&p->LinkStateID,        (l) )
#define  AnscOspf2LsaSetLinkStateIDDot(p, l)        AnscWriteUlong (&p->LinkStateID,        *(PULONG)(l)  )
#define  AnscOspf2LsaSetAdvRouterValue(p, a)        AnscWriteUlong (&p->AdvRouter,          (a) )
#define  AnscOspf2LsaSetAdvRouterDot(p, a)          AnscWriteUlong (&p->AdvRouter,          *(PULONG)(a)  )
#define  AnscOspf2LsaSetLSSeqNum(p, l)              AnscWriteUlong (&p->LSSeqNum,           AnscUlongFromHToN(l) )
#define  AnscOspf2LsaSetLSChecksum(p, l)            AnscWriteUshort (&p->LSChecksum,        AnscUshortFromHToN(l) )
#define  AnscOspf2LsaSetLength(p, l)                AnscWriteUshort (&p->Length,            AnscUshortFromHToN(l) )

#define  AnscOspf2LsaIfTypeUnknown(p)               ( ( (p->LSType <  OSPF2_LS_TYPE_ROUTER) ||      \
                                                        (p->LSType >  OSPF2_LS_TYPE_AS_EX)     )?   \
                                                      TRUE:FALSE    )

#define  AnscOspf2LsaCreateFromLsa(pNewLsa, pOldLsa)                                        \
         {                                                                                  \
                                                                                            \
            pNewLsa       =                                                                 \
                (POSPF2_LSA_HEADER)AnscAllocateMemory                                       \
                    (AnscOspf2LsaGetLength(pOldLsa));                                       \
                                                                                            \
            AnscCopyMemory(pNewLsa,  pOldLsa, AnscOspf2LsaGetLength(pOldLsa));              \
         }

#define  AnscOspf2LsaDelete(pLsa)                   AnscFreeMemory(pLsa)                    

/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ROUTER_TOS
 --------------------------------------------------------------------------------*/
#define  OSPF2_LSA_ROUTER_TOS_SIZE                      4         


typedef  struct
_OSPF2_LSA_ROUTER_TOS
{
    UCHAR                           TOS;
    UCHAR                           Reserved;
    USHORT                          TOSMetric;

}_struct_pack_
OSPF2_LSA_ROUTER_TOS,  *POSPF2_LSA_ROUTER_TOS;

#define  AnscOspf2LsaRouterTosGetTOS(p)                ( p->TOS                                                  )
#define  AnscOspf2LsaRouterTosGetReserved(p)           ( p->Reserved                                             )
#define  AnscOspf2LsaRouterTosGetTOSMetric(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->TOSMetric))       )

#define  AnscOspf2LsaRouterTosSetTOS(p, t)             ( p->TOS = t                                              )
#define  AnscOspf2LsaRouterTosSetReserved(p, r)        ( p->Reserved = r                                         )
#define  AnscOspf2LsaRouterTosSetTOSMetric(p, m)       AnscWriteUshort (&p->TOSMetric,        AnscUshortFromHToN(m) )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ROUTER_LINK
 --------------------------------------------------------------------------------*/
#define  OSPF_LINK_TYPE_PTP                         1           /* Point to point connection to another router */ 
#define  OSPF_LINK_TYPE_TRANSIT                     2           /* Connection to a transit network */ 
#define  OSPF_LINK_TYPE_STUB                        3           /* Connection to a stub network */ 
#define  OSPF_LINK_TYPE_VIRTUAL                     4           /* Virtual link */ 


#define  OSPF2_LSA_ROUTER_LINK_HEADER_MIN_SIZE      12         
#define  OSPF2_LSA_ROUTER_LINK_COST_MAX             0xffff        


typedef  struct
_OSPF2_LSA_ROUTER_LINK
{
    ULONG                           LinkID;
    ULONG                           LinkData;
    UCHAR                           Type;
    UCHAR                           TOSNum;
    USHORT                          Metric;
    OSPF2_LSA_ROUTER_TOS            TOS[1];

}_struct_pack_
OSPF2_LSA_ROUTER_LINK,  *POSPF2_LSA_ROUTER_LINK;

#define  AnscOspf2LsaRouterLinkGetSize(p)                                                   \
        (                                                                                   \
            OSPF2_LSA_ROUTER_LINK_HEADER_MIN_SIZE +                                         \
            AnscOspf2LsaRouterLinkGetTOSNum(p) * OSPF2_LSA_ROUTER_TOS_SIZE                  \
        )

#define  AnscOspf2LsaRouterLinkGetLinkIDValue(p)         ( AnscReadUlong(&p->LinkID)                             )
#define  AnscOspf2LsaRouterLinkGetLinkIDDot(p)           ( (PUCHAR)(&p->LinkID)                                  )
#define  AnscOspf2LsaRouterLinkGetLinkDataValue(p)       ( AnscReadUlong(&p->LinkData)                           )
#define  AnscOspf2LsaRouterLinkGetLinkDataDot(p)         ( (PUCHAR)(&p->LinkData)                                )
#define  AnscOspf2LsaRouterLinkGetType(p)                ( p->Type                                               )
#define  AnscOspf2LsaRouterLinkGetTOSNum(p)              ( p->TOSNum                                             )
#define  AnscOspf2LsaRouterLinkGetMetric(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Metric))        )
#define  AnscOspf2LsaRouterLinkGetFirstTOS(p)            ( p->TOS                                                )
#define  AnscOspf2LsaRouterLinkGetNextTOS(t)             ( (POSPF2_LSA_ROUTER_TOS)((ULONG)t + sizeof(OSPF2_LSA_ROUTER_TOS)) )                                               )

#define  AnscOspf2LsaRouterLinkSetLinkIDValue(p, l)      AnscWriteUlong (&p->LinkID,        (l) )
#define  AnscOspf2LsaRouterLinkSetLinkIDDot(p, l)        AnscWriteUlong (&p->LinkID,        *(PULONG)(l)         )
#define  AnscOspf2LsaRouterLinkSetLinkDataValue(p, l)    AnscWriteUlong (&p->LinkData,      (l) )
#define  AnscOspf2LsaRouterLinkSetLinkDataDot(p, l)      AnscWriteUlong (&p->LinkData,      *(PULONG)(l)         )
#define  AnscOspf2LsaRouterLinkSetType(p, t)             ( p->Type = t                                           )
#define  AnscOspf2LsaRouterLinkSetTOSNum(p, t)           ( p->TOSNum = t                                         )
#define  AnscOspf2LsaRouterLinkSetMetric(p, m)           AnscWriteUshort (&p->Metric,       AnscUshortFromHToN((USHORT)m) )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ROUTER_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_LSA_ROUTER_V_SHIFT                    0x2         /* When set, the router is an endpoint 
                                                                   of one or more fully adjacent virtual
                                                                   links having the described area as 
                                                                   transit area */
#define  OSPF_LSA_ROUTER_V_MASK                     (UCHAR)(0x1 << OSPF_LSA_ROUTER_V_SHIFT)

#define  OSPF_LSA_ROUTER_E_SHIFT                    0x1         /* When set, the router is an AS boundary router */
#define  OSPF_LSA_ROUTER_E_MASK                     (UCHAR)(0x1 << OSPF_LSA_ROUTER_E_SHIFT)

#define  OSPF_LSA_ROUTER_B_SHIFT                    0x0         /* When set, the router is an area border router */
#define  OSPF_LSA_ROUTER_B_MASK                     (UCHAR)(0x1 << OSPF_LSA_ROUTER_B_SHIFT)

#define  OSPF_LSA_ROUTER_OWN_MIN_SIZE               4

typedef  struct
_OSPF2_LSA_ROUTER_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    ULONG                           LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
    UCHAR                           Flags;
    UCHAR                           Reserved;
    USHORT                          LinkNum;
    OSPF2_LSA_ROUTER_LINK           Link[ANSC_ZERO_ARRAY_SIZE];

}_struct_pack_
OSPF2_LSA_ROUTER_HEADER,  *POSPF2_LSA_ROUTER_HEADER;

#define  AnscOspf2LsaRouterGetLSAge(p)                    AnscOspf2LsaGetLSAge(p)
#define  AnscOspf2LsaRouterGetOptions(p)                  AnscOspf2LsaGetOptions(p)         
#define  AnscOspf2LsaRouterGetLSType(p)                   AnscOspf2LsaGetLSType(p)          
#define  AnscOspf2LsaRouterGetLinkStateIDValue(p)         AnscOspf2LsaGetLinkStateIDValue(p)
#define  AnscOspf2LsaRouterGetLinkStateIDDot(p)           AnscOspf2LsaGetLinkStateIDDot(p)  
#define  AnscOspf2LsaRouterGetAdvRouterValue(p)           AnscOspf2LsaGetAdvRouterValue(p)  
#define  AnscOspf2LsaRouterGetAdvRouterDot(p)             AnscOspf2LsaGetAdvRouterDot(p)    
#define  AnscOspf2LsaRouterGetLSSeqNum(p)                 AnscOspf2LsaGetLSSeqNum(p)        
#define  AnscOspf2LsaRouterGetLSChecksum(p)               AnscOspf2LsaGetLSChecksum(p)       
#define  AnscOspf2LsaRouterGetLength(p)                   AnscOspf2LsaGetLength(p)          
#define  AnscOspf2LsaRouterGetBitV(p)                     ( (p->Flags & OSPF_LSA_ROUTER_V_MASK) >> OSPF_LSA_ROUTER_V_SHIFT )         
#define  AnscOspf2LsaRouterGetBitE(p)                     ( (p->Flags & OSPF_LSA_ROUTER_E_MASK) >> OSPF_LSA_ROUTER_E_SHIFT )         
#define  AnscOspf2LsaRouterGetBitB(p)                     ( (p->Flags & OSPF_LSA_ROUTER_B_MASK) >> OSPF_LSA_ROUTER_B_SHIFT )         
#define  AnscOspf2LsaRouterGetReserved(p)                 ( p->Reserved )          
#define  AnscOspf2LsaRouterGetLinkNum(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->LinkNum))                )
#define  AnscOspf2LsaRouterGetFirstLink(p)                ( (POSPF2_LSA_ROUTER_LINK)p->Link                                )

#define  AnscOspf2LsaRouterGetNextLink(t)                                                   \
        (                                                                                   \
            (POSPF2_LSA_ROUTER_LINK)( (ULONG)t + AnscOspf2LsaRouterLinkGetSize(t) )         \
        )              
                                 

#define  AnscOspf2LsaRouterSetLSAge(p, l)                 AnscOspf2LsaSetLSAge(p, l)           
#define  AnscOspf2LsaRouterSetOptions(p, o)               AnscOspf2LsaSetOptions(p, o)         
#define  AnscOspf2LsaRouterSetLSType(p, l)                AnscOspf2LsaSetLSType(p, l)          
#define  AnscOspf2LsaRouterSetLinkStateIDValue(p, l)      AnscOspf2LsaSetLinkStateIDValue(p, l)
#define  AnscOspf2LsaRouterSetLinkStateIDDot(p, l)        AnscOspf2LsaSetLinkStateIDDot(p, l)  
#define  AnscOspf2LsaRouterSetAdvRouterValue(p, a)        AnscOspf2LsaSetAdvRouterValue(p, a)  
#define  AnscOspf2LsaRouterSetAdvRouterDot(p, a)          AnscOspf2LsaSetAdvRouterDot(p, a)    
#define  AnscOspf2LsaRouterSetLSSeqNum(p, l)              AnscOspf2LsaSetLSSeqNum(p, l)        
#define  AnscOspf2LsaRouterSetLSChecksum(p, l)            AnscOspf2LsaSetLSChecksum(p, l)      
#define  AnscOspf2LsaRouterSetLength(p, l)                AnscOspf2LsaSetLength(p, l) 
  
#define  AnscOspf2LsaRouterReSetFlags(p)                  ( p->Flags = 0                                 ) 
#define  AnscOspf2LsaRouterSetBitV(p)                     ( p->Flags = (p->Flags | OSPF_LSA_ROUTER_V_MASK)                 )         
#define  AnscOspf2LsaRouterReSetBitV(p)                   ( p->Flags = (p->Flags & ~OSPF_LSA_ROUTER_V_MASK)                )         
#define  AnscOspf2LsaRouterSetBitE(p)                     ( p->Flags = (p->Flags | OSPF_LSA_ROUTER_E_MASK)                 )         
#define  AnscOspf2LsaRouterReSetBitE(p)                   ( p->Flags = (p->Flags & ~OSPF_LSA_ROUTER_E_MASK)                )         
#define  AnscOspf2LsaRouterSetBitB(p)                     ( p->Flags = (p->Flags | OSPF_LSA_ROUTER_B_MASK)                 )         
#define  AnscOspf2LsaRouterReSetBitB(p)                   ( p->Flags = (p->Flags & ~OSPF_LSA_ROUTER_B_MASK)                )         
#define  AnscOspf2LsaRouterSetReserved(p, r)              ( p->Reserved = r                                                )          
#define  AnscOspf2LsaRouterSetLinkNum(p, m)               AnscWriteUshort (&p->LinkNum,     AnscUshortFromHToN(m)          )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_NETWORK_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_LSA_NETWORK_HEADER_SIZE                     24

typedef  struct
_OSPF2_LSA_NETWORK_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    ULONG                           LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
    ULONG                           NetworkMask;
    ULONG                           AttachedRouter[ANSC_ZERO_ARRAY_SIZE];

}_struct_pack_
OSPF2_LSA_NETWORK_HEADER,  *POSPF2_LSA_NETWORK_HEADER;

#define  AnscOspf2LsaNetworkGetLSAge(p)                   AnscOspf2LsaGetLSAge(p)
#define  AnscOspf2LsaNetworkGetOptions(p)                 AnscOspf2LsaGetOptions(p)         
#define  AnscOspf2LsaNetworkGetLSType(p)                  AnscOspf2LsaGetLSType(p)          
#define  AnscOspf2LsaNetworkGetLinkStateIDValue(p)        AnscOspf2LsaGetLinkStateIDValue(p)
#define  AnscOspf2LsaNetworkGetLinkStateIDDot(p)          AnscOspf2LsaGetLinkStateIDDot(p)  
#define  AnscOspf2LsaNetworkGetAdvRouterValue(p)          AnscOspf2LsaGetAdvRouterValue(p)  
#define  AnscOspf2LsaNetworkGetAdvRouterDot(p)            AnscOspf2LsaGetAdvRouterDot(p)    
#define  AnscOspf2LsaNetworkGetLSSeqNum(p)                AnscOspf2LsaGetLSSeqNum(p)        
#define  AnscOspf2LsaNetworkGetLSChecksum(p)              AnscOspf2LsaGetLSChecksum(p)       
#define  AnscOspf2LsaNetworkGetLength(p)                  AnscOspf2LsaGetLength(p)          
#define  AnscOspf2LsaNetworkGetNetworkMaskValue(p)        ( AnscReadUlong(&p->NetworkMask)                          )     
#define  AnscOspf2LsaNetworkGetNetworkMaskDot(p)          ( (PUCHAR)(&p->NetworkMask)                               )
#define  AnscOspf2LsaNetworkGetAttachedRouterNum(p)       ( (AnscOspf2LsaNetworkGetLength(p) - OSPF_LSA_NETWORK_HEADER_SIZE) >> 2  )     
#define  AnscOspf2LsaNetworkGetAttachedRouterValue(p, i)  ( AnscReadUlong(&(p->AttachedRouter[i]))                  )     
#define  AnscOspf2LsaNetworkGetAttachedRouterDote(p, i)   ( (PUCHAR)(&(p->AttachedRouter[i]))                                  )     


#define  AnscOspf2LsaNetworkSetLSAge(p, l)                 AnscOspf2LsaSetLSAge(p, l)           
#define  AnscOspf2LsaNetworkSetOptions(p, o)               AnscOspf2LsaSetOptions(p, o)         
#define  AnscOspf2LsaNetworkSetLSType(p, l)                AnscOspf2LsaSetLSType(p, l)          
#define  AnscOspf2LsaNetworkSetLinkStateIDValue(p, l)      AnscOspf2LsaSetLinkStateIDValue(p, l)
#define  AnscOspf2LsaNetworkSetLinkStateIDDot(p, l)        AnscOspf2LsaSetLinkStateIDDot(p, l)  
#define  AnscOspf2LsaNetworkSetAdvRouterValue(p, a)        AnscOspf2LsaSetAdvRouterValue(p, a)  
#define  AnscOspf2LsaNetworkSetAdvRouterDot(p, a)          AnscOspf2LsaSetAdvRouterDot(p, a)    
#define  AnscOspf2LsaNetworkSetLSSeqNum(p, l)              AnscOspf2LsaSetLSSeqNum(p, l)        
#define  AnscOspf2LsaNetworkSetLSChecksum(p, l)            AnscOspf2LsaSetLSChecksum(p, l)      
#define  AnscOspf2LsaNetworkSetLength(p, l)                AnscOspf2LsaSetLength(p, l)          
#define  AnscOspf2LsaNetworkSetNetworkMaskValue(p, l)      AnscWriteUlong (&p->NetworkMask,        (l)        )
#define  AnscOspf2LsaNetworkSetNetworkMaskDot(p, l)        AnscWriteUlong (&p->NetworkMask,        *(PULONG)(l)                )
#define  AnscOspf2LsaNetworkSetAttachedRouterValue(p, i, l)                                                   \
                                                           AnscWriteUlong (&p->AttachedRouter[i],  (l)        )
#define  AnscOspf2LsaNetworkSetAttachedRouterDot(p, i, l)  AnscWriteUlong (&p->AttachedRouter[i],  *(PULONG)(l)                )

/*---------------------------------------------------------------------------------
                        OSPF2_LSA_SUM3_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_LSA_SUM3_HEADER_SIZE                         28

typedef  struct
_OSPF2_LSA_SUM3_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    ULONG                           LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
    ULONG                           NetworkMask;
    UCHAR                           Reserved;
    UCHAR                           Metric[3];
    ULONG                           TOSInfo[ANSC_ZERO_ARRAY_SIZE]; 

}_struct_pack_
OSPF2_LSA_SUM3_HEADER,  *POSPF2_LSA_SUM3_HEADER;

#define  AnscOspf2LsaSum3GetLSAge(p)                      AnscOspf2LsaGetLSAge(p)
#define  AnscOspf2LsaSum3GetOptions(p)                    AnscOspf2LsaGetOptions(p)         
#define  AnscOspf2LsaSum3GetLSType(p)                     AnscOspf2LsaGetLSType(p)          
#define  AnscOspf2LsaSum3GetLinkStateIDValue(p)           AnscOspf2LsaGetLinkStateIDValue(p)
#define  AnscOspf2LsaSum3GetLinkStateIDDot(p)             AnscOspf2LsaGetLinkStateIDDot(p)  
#define  AnscOspf2LsaSum3GetAdvRouterValue(p)             AnscOspf2LsaGetAdvRouterValue(p)  
#define  AnscOspf2LsaSum3GetAdvRouterDot(p)               AnscOspf2LsaGetAdvRouterDot(p)    
#define  AnscOspf2LsaSum3GetLSSeqNum(p)                   AnscOspf2LsaGetLSSeqNum(p)        
#define  AnscOspf2LsaSum3GetLSChecksum(p)                 AnscOspf2LsaGetLSChecksum(p)       
#define  AnscOspf2LsaSum3GetLength(p)                     AnscOspf2LsaGetLength(p)          
#define  AnscOspf2LsaSum3GetNetworkMaskValue(p)           ( AnscReadUlong(&p->NetworkMask)                           )     
#define  AnscOspf2LsaSum3GetNetworkMaskDot(p)             ( (PUCHAR)(&p->NetworkMask)                                )
#define  AnscOspf2LsaSum3GetReserved(p)                   ( p->Reserved                                              )     
#define  AnscOspf2LsaSum3GetMetric(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) & 0xffffff)     
#define  AnscOspf2LsaSum3GetTOSInfoNum(p)                 ( (AnscOspf2LsaSum3GetLength(p) - OSPF_LSA_SUM3_HEADER_SIZE) >> 2  )     
#define  AnscOspf2LsaSum3GetTOS(p, i)                     ( (UCHAR)(p->TOSInfo[i])                                   )     
#define  AnscOspf2LsaSum3GetTosMetric(p, i)               ( AnscUlongFromNToH(AnscReadUlong(&p->TOSInfo[i])) & 0xffffff )     

#define  AnscOspf2LsaSum3SetLSAge(p, l)                    AnscOspf2LsaSetLSAge(p, l)           
#define  AnscOspf2LsaSum3SetOptions(p, o)                  AnscOspf2LsaSetOptions(p, o)         
#define  AnscOspf2LsaSum3SetLSType(p, l)                   AnscOspf2LsaSetLSType(p, l)          
#define  AnscOspf2LsaSum3SetLinkStateIDValue(p, l)         AnscOspf2LsaSetLinkStateIDValue(p, l)
#define  AnscOspf2LsaSum3SetLinkStateIDDot(p, l)           AnscOspf2LsaSetLinkStateIDDot(p, l)  
#define  AnscOspf2LsaSum3SetAdvRouterValue(p, a)           AnscOspf2LsaSetAdvRouterValue(p, a)  
#define  AnscOspf2LsaSum3SetAdvRouterDot(p, a)             AnscOspf2LsaSetAdvRouterDot(p, a)    
#define  AnscOspf2LsaSum3SetLSSeqNum(p, l)                 AnscOspf2LsaSetLSSeqNum(p, l)        
#define  AnscOspf2LsaSum3SetLSChecksum(p, l)               AnscOspf2LsaSetLSChecksum(p, l)      
#define  AnscOspf2LsaSum3SetLength(p, l)                   AnscOspf2LsaSetLength(p, l)          
#define  AnscOspf2LsaSum3SetNetworkMaskValue(p, l)         AnscWriteUlong (&p->NetworkMask,        (l)                          )
#define  AnscOspf2LsaSum3SetNetworkMaskDot(p, l)           AnscWriteUlong (&p->NetworkMask,        *(PULONG)(l)                 )
#define  AnscOspf2LsaSum3SetReserved(p, r)                 ( p->Reserved = r )
#define  AnscOspf2LsaSum3SetMetric(p, m)                   AnscWriteUlong (&p->Reserved,           AnscUlongFromHToN(m&0xffffff) )
#define  AnscOspf2LsaSum3SetTOS(p, i, r)                   ( *((PUCHAR)&p->TOSInfo[i]) = r )
#define  AnscOspf2LsaSum3SetTosMetric(p, i, m)             AnscWriteUlong (&p->TOSInfo[i],         ( AnscUlongFromHToN(m&0xffffff)) | ((UCHAR)(p->TOSInfo[i]) << 24) )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_SUM4_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_LSA_SUM4_HEADER_SIZE                         28

typedef  struct
_OSPF2_LSA_SUM4_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    ULONG                           LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
    ULONG                           NetworkMask;
    UCHAR                           Reserved;
    UCHAR                           Metric[3];
    ULONG                           TOSInfo[ANSC_ZERO_ARRAY_SIZE]; 

}_struct_pack_
OSPF2_LSA_SUM4_HEADER,  *POSPF2_LSA_SUM4_HEADER;

#define  AnscOspf2LsaSum4GetLSAge(p)                      AnscOspf2LsaGetLSAge(p)
#define  AnscOspf2LsaSum4GetOptions(p)                    AnscOspf2LsaGetOptions(p)         
#define  AnscOspf2LsaSum4GetLSType(p)                     AnscOspf2LsaGetLSType(p)          
#define  AnscOspf2LsaSum4GetLinkStateIDValue(p)           AnscOspf2LsaGetLinkStateIDValue(p)
#define  AnscOspf2LsaSum4GetLinkStateIDDot(p)             AnscOspf2LsaGetLinkStateIDDot(p)  
#define  AnscOspf2LsaSum4GetAdvRouterValue(p)             AnscOspf2LsaGetAdvRouterValue(p)  
#define  AnscOspf2LsaSum4GetAdvRouterDot(p)               AnscOspf2LsaGetAdvRouterDot(p)    
#define  AnscOspf2LsaSum4GetLSSeqNum(p)                   AnscOspf2LsaGetLSSeqNum(p)        
#define  AnscOspf2LsaSum4GetLSChecksum(p)                 AnscOspf2LsaGetLSChecksum(p)       
#define  AnscOspf2LsaSum4GetLength(p)                     AnscOspf2LsaGetLength(p)          
#define  AnscOspf2LsaSum4GetNetworkMaskValue(p)           ( AnscReadUlong(&p->NetworkMask)                           )     
#define  AnscOspf2LsaSum4GetNetworkMaskDot(p)             ( (PUCHAR)(&p->NetworkMask)                                )
#define  AnscOspf2LsaSum4GetReserved(p)                   ( p->Reserved                                              )     
#define  AnscOspf2LsaSum4GetMetric(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) & 0xffffff)     
#define  AnscOspf2LsaSum4GetTOSInfoNum(p)                 ( (AnscOspf2LsaSum3GetLength(p) - OSPF_LSA_SUM4_HEADER_SIZE) >> 2  )     
#define  AnscOspf2LsaSum4GetTOS(p, i)                     ( (UCHAR)(p->TOSInfo[i])                                   )     
#define  AnscOspf2LsaSum4GetTosMetric(p, i)               ( AnscUlongFromNToH(AnscReadUlong(&p->TOSInfo[i])) & 0xffffff )     

#define  AnscOspf2LsaSum4SetLSAge(p, l)                    AnscOspf2LsaSetLSAge(p, l)           
#define  AnscOspf2LsaSum4SetOptions(p, o)                  AnscOspf2LsaSetOptions(p, o)         
#define  AnscOspf2LsaSum4SetLSType(p, l)                   AnscOspf2LsaSetLSType(p, l)          
#define  AnscOspf2LsaSum4SetLinkStateIDValue(p, l)         AnscOspf2LsaSetLinkStateIDValue(p, l)
#define  AnscOspf2LsaSum4SetLinkStateIDDot(p, l)           AnscOspf2LsaSetLinkStateIDDot(p, l)  
#define  AnscOspf2LsaSum4SetAdvRouterValue(p, a)           AnscOspf2LsaSetAdvRouterValue(p, a)  
#define  AnscOspf2LsaSum4SetAdvRouterDot(p, a)             AnscOspf2LsaSetAdvRouterDot(p, a)    
#define  AnscOspf2LsaSum4SetLSSeqNum(p, l)                 AnscOspf2LsaSetLSSeqNum(p, l)        
#define  AnscOspf2LsaSum4SetLSChecksum(p, l)               AnscOspf2LsaSetLSChecksum(p, l)      
#define  AnscOspf2LsaSum4SetLength(p, l)                   AnscOspf2LsaSetLength(p, l)          
#define  AnscOspf2LsaSum4SetNetworkMaskValue(p, l)         AnscWriteUlong (&p->NetworkMask,        (l)                              )
#define  AnscOspf2LsaSum4SetNetworkMaskDot(p, l)           AnscWriteUlong (&p->NetworkMask,        *(PULONG)(l)                     )
#define  AnscOspf2LsaSum4SetReserved(p, r)                 ( p->Reserved = r )
#define  AnscOspf2LsaSum4SetMetric(p, m)                   AnscWriteUlong (&p->Reserved,           AnscUlongFromHToN(m&0xffffff)    )
#define  AnscOspf2LsaSum4SetTOS(p, i, r)                   ( *((PUCHAR)&p->TOSInfo[i]) = r )
#define  AnscOspf2LsaSum4SetTosMetric(p, i, m)             AnscWriteUlong (&p->TOSInfo[i],         ( AnscUlongFromHToN(m&0xffffff)) | ((UCHAR)(p->TOSInfo[i]) << 24) )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ASEX_TOS
 --------------------------------------------------------------------------------*/
typedef  struct
_OSPF2_LSA_ASEX_TOS
{
    UCHAR                           ETos;
    UCHAR                           TosMetric[3];
    ULONG                           ForwardAddr;
    ULONG                           ExRouteTag;

}_struct_pack_
OSPF2_LSA_ASEX_TOS,  *POSPF2_LSA_ASEX_TOS;


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ASEX_HEADER
 --------------------------------------------------------------------------------*/
#define  OSPF_LSA_ASEX_HEADER_SIZE                         36
#define  OSPF_LSA_ASEX_E_SHIFT                             7
#define  OSPF_LSA_ASEX_E_MASK                              ( 0x1 << OSPF_LSA_ASEX_E_SHIFT )


typedef  struct
_OSPF2_LSA_ASEX_HEADER
{
    USHORT                          LSAge;
    UCHAR                           Options;
    UCHAR                           LSType;
    ULONG                           LinkStateID;
    ULONG                           AdvRouter;
    ULONG                           LSSeqNum;
    USHORT                          LSChecksum;
    USHORT                          Length;
    ULONG                           NetworkMask;
    UCHAR                           EFlag;
    UCHAR                           Metric[3];
    ULONG                           ForwardAddr;
    ULONG                           ExRouteTag;
    OSPF2_LSA_ASEX_TOS              TOSInfo[ANSC_ZERO_ARRAY_SIZE]; 

}_struct_pack_
OSPF2_LSA_ASEX_HEADER,  *POSPF2_LSA_ASEX_HEADER;

#define  AnscOspf2LsaAsexGetLSAge(p)                      AnscOspf2LsaGetLSAge(p)
#define  AnscOspf2LsaAsexGetOptions(p)                    AnscOspf2LsaGetOptions(p)         
#define  AnscOspf2LsaAsexGetLSType(p)                     AnscOspf2LsaGetLSType(p)          
#define  AnscOspf2LsaAsexGetLinkStateIDValue(p)           AnscOspf2LsaGetLinkStateIDValue(p)
#define  AnscOspf2LsaAsexGetLinkStateIDDot(p)             AnscOspf2LsaGetLinkStateIDDot(p)  
#define  AnscOspf2LsaAsexGetAdvRouterValue(p)             AnscOspf2LsaGetAdvRouterValue(p)  
#define  AnscOspf2LsaAsexGetAdvRouterDot(p)               AnscOspf2LsaGetAdvRouterDot(p)    
#define  AnscOspf2LsaAsexGetLSSeqNum(p)                   AnscOspf2LsaGetLSSeqNum(p)        
#define  AnscOspf2LsaAsexGetLSChecksum(p)                 AnscOspf2LsaGetLSChecksum(p)       
#define  AnscOspf2LsaAsexGetLength(p)                     AnscOspf2LsaGetLength(p)          
#define  AnscOspf2LsaAsexGetNetworkMaskValue(p)           ( AnscReadUlong(&p->NetworkMask)                           )     
#define  AnscOspf2LsaAsexGetNetworkMaskDot(p)             ( (PUCHAR)(&p->NetworkMask)                                )
#define  AnscOspf2LsaAsexGetEFlag(p)                      ( (p->EFlag & OSPF_LSA_ASEX_E_MASK) >> OSPF_LSA_ASEX_E_SHIFT )     
#define  AnscOspf2LsaAsexGetMetric(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->EFlag)) & 0xffffff   )     
#define  AnscOspf2LsaAsexGetForwardAddrValue(p)           ( AnscReadUlong(&p->ForwardAddr)                           )     
#define  AnscOspf2LsaAsexGetForwardAddrDot(p)             ( (PUCHAR)(&p->ForwardAddr)                                )
#define  AnscOspf2LsaAsexGetExRouteTag(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->ExRouteTag))         )     

#define  AnscOspf2LsaAsexGetTOSInfoNum(p)                 ( (AnscOspf2LsaAsexGetLength(p) - OSPF_LSA_ASEX_HEADER_SIZE) / sizeof(OSPF2_LSA_ASEX_TOS)  )     
#define  AnscOspf2LsaAsexGetTEBit(p, i)                   ( (p->TOSInfo[i].ETos & OSPF_LSA_ASEX_E_MASK) >> OSPF_LSA_ASEX_E_SHIFT )     
#define  AnscOspf2LsaAsexGetTTos(p, i)                    ( p->TOSInfo[i].ETos & ~OSPF_LSA_ASEX_E_MASK              )     
#define  AnscOspf2LsaAsexGetTTosMetric(p, i)              ( AnscUlongFromNToH(AnscReadUlong(&p->TOSInfo[i].ETos)) & 0xffffff )     
#define  AnscOspf2LsaAsexGetTForwardAddrValue(p, i)       ( AnscReadUlong(&p->TOSInfo[i].ForwardAddr)                           )     
#define  AnscOspf2LsaAsexGetTForwardAddrDot(p, i)         ( (PUCHAR)(&p->TOSInfo[i].ForwardAddr)                                )
#define  AnscOspf2LsaAsexGetTExRouteTag(p, i)             ( AnscUlongFromNToH(AnscReadUlong(&p->TOSInfo[i].ExRouteTag))         )     


#define  AnscOspf2LsaAsexSetLSAge(p, l)                    AnscOspf2LsaSetLSAge(p, l)           
#define  AnscOspf2LsaAsexSetOptions(p, o)                  AnscOspf2LsaSetOptions(p, o)         
#define  AnscOspf2LsaAsexSetLSType(p, l)                   AnscOspf2LsaSetLSType(p, l)          
#define  AnscOspf2LsaAsexSetLinkStateIDValue(p, l)         AnscOspf2LsaSetLinkStateIDValue(p, l)
#define  AnscOspf2LsaAsexSetLinkStateIDDot(p, l)           AnscOspf2LsaSetLinkStateIDDot(p, l)  
#define  AnscOspf2LsaAsexSetAdvRouterValue(p, a)           AnscOspf2LsaSetAdvRouterValue(p, a)  
#define  AnscOspf2LsaAsexSetAdvRouterDot(p, a)             AnscOspf2LsaSetAdvRouterDot(p, a)    
#define  AnscOspf2LsaAsexSetLSSeqNum(p, l)                 AnscOspf2LsaSetLSSeqNum(p, l)        
#define  AnscOspf2LsaAsexSetLSChecksum(p, l)               AnscOspf2LsaSetLSChecksum(p, l)      
#define  AnscOspf2LsaAsexSetLength(p, l)                   AnscOspf2LsaSetLength(p, l)          
#define  AnscOspf2LsaAsexSetNetworkMaskValue(p, l)         AnscWriteUlong (&p->NetworkMask,        (l)                         )
#define  AnscOspf2LsaAsexSetNetworkMaskDot(p, l)           AnscWriteUlong (&p->NetworkMask,        *(PULONG)(l)                )
#define  AnscOspf2LsaAsexSetEFlag(p, e)                    ( p->EFlag = (e <<OSPF_LSA_ASEX_E_SHIFT)                            )
#define  AnscOspf2LsaAsexSetMetric(p, m)                   AnscWriteUlong (&p->EFlag,              ( AnscUlongFromHToN(m&0xffffff) | ((UCHAR)(p->EFlag) << 24) ) )
#define  AnscOspf2LsaAsexSetForwardAddrValue(p, l)         AnscWriteUlong (&p->ForwardAddr,        (l)                         )
#define  AnscOspf2LsaAsexSetForwardAddrDot(p, l)           AnscWriteUlong (&p->ForwardAddr,        *(PULONG)(l)                )
#define  AnscOspf2LsaAsexSetExRouteTag(p, l)               AnscWriteUlong (&p->ExRouteTag,         AnscUlongFromHToN(l)        )


#define  AnscOspf2LsaAsexSetTEBit(p, i, e)                   ( p->TOSInfo[i].ETos = (p->TOSInfo[i].ETos & ~OSPF_LSA_ASEX_E_MASK) | (e << OSPF_LSA_ASEX_E_SHIFT) )     
#define  AnscOspf2LsaAsexSetTTos(p, i, t)                    ( p->TOSInfo[i].ETos = (p->TOSInfo[i].ETos & OSPF_LSA_ASEX_E_MASK)  | (t & ~OSPF_LSA_ASEX_E_MASK)  )     
#define  AnscOspf2LsaAsexSetTTosMetric(p, i, m)              AnscWriteUlong (&p->TOSInfo[i].ETos,         ( AnscUlongFromHToN(m&0xffffff) | ((UCHAR)(p->TOSInfo[i].ETos) << 24) ) )
#define  AnscOspf2LsaAsexSetTForwardAddrValue(p, i, f)       AnscWriteUlong (&p->TOSInfo[i].ForwardAddr,        (f)                         )     
#define  AnscOspf2LsaAsexSetTForwardAddrDot(p, i, f)         AnscWriteUlong (&p->TOSInfo[i].ForwardAddr,        *(PULONG)(l)                )
#define  AnscOspf2LsaAsexSetTExRouteTag(p, i, r)             AnscWriteUlong (&p->TOSInfo[i].ExRouteTag,         AnscUlongFromHToN(r)        )          

/*---------------------------------------------------------------------------------
                        OSPF2_DATA_DESC_HEADER
    The flags field in the database description packets is defined as

        +----------------------------------------+
        |  *  |  *  |  * | * |  * | I  | M  | MS |
        +----------------------------------------+
    
        I:      Init bit. When set to 1, this packet is the first in the sequence of database
                description packets.
        M:      The more bit. When set to 1, it indicates that more DD packets are to follow 
        MS:     The Master/slave bit. When set to 1, it indicates that the router is the 
                master during the database exchange process. Otherwise, it's the slave

 --------------------------------------------------------------------------------*/

#define  OSPF_DD_FLAG_I_SHIFT                       (UCHAR)0x2         
#define  OSPF_DD_FLAG_I_MASK                        (UCHAR)(0x1 << OSPF_DD_FLAG_I_SHIFT)
#define  OSPF_DD_FLAG_M_SHIFT                       (UCHAR)0x1         
#define  OSPF_DD_FLAG_M_MASK                        (UCHAR)(0x1 << OSPF_DD_FLAG_M_SHIFT)
#define  OSPF_DD_FLAG_MS_SHIFT                      (UCHAR)0x0         
#define  OSPF_DD_FLAG_MS_MASK                       (UCHAR)(0x1 << OSPF_DD_FLAG_MS_SHIFT)


#define  AnscOspf2DDFlagGetBitI(option)             ( (option & OSPF_DD_FLAG_I_MASK ) >> OSPF_DD_FLAG_I_SHIFT    )         
#define  AnscOspf2DDFlagGetBitM(option)             ( (option & OSPF_DD_FLAG_M_MASK ) >> OSPF_DD_FLAG_M_SHIFT    )         
#define  AnscOspf2DDFlagGetBitMS(option)            ( (option & OSPF_DD_FLAG_MS_MASK) >> OSPF_DD_FLAG_MS_SHIFT   )         


#define  AnscOspf2DDFlagSetBitI(option)             ( option = (UCHAR)(option  | OSPF_DD_FLAG_I_MASK)                   )         
#define  AnscOspf2DDFlagReSetBitI(option)           ( option = (UCHAR)(option  & ~OSPF_DD_FLAG_I_MASK)                  ) 
#define  AnscOspf2DDFlagSetBitM(option)             ( option = (UCHAR)(option  | OSPF_DD_FLAG_M_MASK)                   )         
#define  AnscOspf2DDFlagReSetBitM(option)           ( option = (UCHAR)(option  & ~OSPF_DD_FLAG_M_MASK)                  ) 
#define  AnscOspf2DDFlagSetBitMS(option)            ( option = (UCHAR)(option  | OSPF_DD_FLAG_MS_MASK)                  )         
#define  AnscOspf2DDFlagReSetBitMS(option)          ( option = (UCHAR)(option  & ~OSPF_DD_FLAG_MS_MASK)                 ) 

#define  OSPF2_DATA_DESC_HEADER_SIZE                8

typedef  struct
_OSPF2_DATA_DESC_HEADER
{
    USHORT                          IfMTU;
    UCHAR                           Options;
    UCHAR                           Flags;
    ULONG                           DDSeq;
    UCHAR                           Lsa[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_DATA_DESC_HEADER,  *POSPF2_DATA_DESC_HEADER;

#define  AnscOspf2DDGetIfMTU(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->IfMTU))             )
#define  AnscOspf2DDGetOptions(p)                   ( p->Options                                                )
#define  AnscOspf2DDGetFlags(p)                     ( p->Flags                                                 )
#define  AnscOspf2DDGetDDSeq(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->DDSeq))     )

#define  AnscOspf2DDSetIfMTU(p, h)                  AnscWriteUshort(&p->IfMTU,              AnscUshortFromHToN(h)   )
#define  AnscOspf2DDSetOptions(p, o)                ( p->Options = o                                                )
#define  AnscOspf2DDSetFlags(p, r)                  ( p->Flags = r                                                  )
#define  AnscOspf2DDSetDDSeq(p, r)                  AnscWriteUlong (&p->DDSeq,              AnscUlongFromHToN(r)    )

#define  AnscOspf2DDGetFirstLsaHeader(p)            ( (POSPF2_LSA_HEADER)p->Lsa                                     )
#define  AnscOspf2DDGetNextLsaHeader(p)             ( (POSPF2_LSA_HEADER)((ULONG)p + OSPF2_LSA_HEADER_SIZE)         )

/*---------------------------------------------------------------------------------
                        OSPF2_LSA_REQ_HEADER


 --------------------------------------------------------------------------------*/

#define  OSPF2_LSA_ID_SIZE                          12

typedef  struct
_OSPF2_LSA_ID_HEADER
{
    ULONG                           LsType;
    ULONG                           LsID;
    ULONG                           AdvRouter;
}_struct_pack_
OSPF2_LSA_ID_HEADER,  *POSPF2_LSA_ID_HEADER;
       

#define  AnscOspf2LsaIDGetLSType(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->LsType))                )
#define  AnscOspf2LsaIDGetLinkStateIDValue(p)         ( AnscReadUlong(&p->LsID)                                     )
#define  AnscOspf2LsaIDGetLinkStateIDDot(p)           ( (PUCHAR)(&p->LsID)                                          )
#define  AnscOspf2LsaIDGetAdvRouterValue(p)           ( AnscReadUlong(&p->AdvRouter)                                )
#define  AnscOspf2LsaIDGetAdvRouterDot(p)             ( (PUCHAR)(&p->AdvRouter)                                     )

#define  AnscOspf2LsaIDSetLSType(p, l)                AnscWriteUlong (&p->LsType,      AnscUlongFromHToN(l)         )
#define  AnscOspf2LsaIDSetLinkStateIDValue(p, l)      AnscWriteUlong (&p->LsID,        (l)                          )
#define  AnscOspf2LsaIDSetLinkStateIDDot(p, l)        AnscWriteUlong (&p->LsID,        *(PULONG)(l)                 )
#define  AnscOspf2LsaIDSetAdvRouterValue(p, a)        AnscWriteUlong (&p->AdvRouter,   (a)                          )
#define  AnscOspf2LsaIDSetAdvRouterDot(p, a)          AnscWriteUlong (&p->AdvRouter,   *(PULONG)(a)                 )
           

typedef  struct
_OSPF2_LSA_REQ_HEADER
{
    OSPF2_LSA_ID_HEADER             LsaID[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_LSA_REQ_HEADER,  *POSPF2_LSA_REQ_HEADER;

#define  AnscOspf2LSAReqGetFirstLsaID(p)            ( p->LsaID                               )
#define  AnscOspf2LSAReqGetNextLsaID(p)             ( (PULONG)((ULONG)p + OSPF2_LSA_ID_SIZE) )


/*---------------------------------------------------------------------------------
                        OSPF2_LSA_UPDATE_HEADER


 --------------------------------------------------------------------------------*/

#define  OSPF2_LSA_UPDATE_HEADER_SIZE               4

typedef  struct
_OSPF2_LSA_UPDATE_HEADER
{
    ULONG                        LsaNum;
    OSPF2_LSA_HEADER             Lsa[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_LSA_UPDATE_HEADER,  *POSPF2_LSA_UPDATE_HEADER;

#define  AnscOspf2LSAUpdateGetLsaNum(p)             ( AnscUlongFromNToH(AnscReadUlong(&p->LsaNum))     )
#define  AnscOspf2LSAUpdateSetLsaNum(p, l)          AnscWriteUlong (&p->LsaNum, AnscUlongFromHToN(l)   )

#define  AnscOspf2LSAUpdateGetFirstLsa(p)           ( p->Lsa                                           )
#define  AnscOspf2LSAUpdateGetNextLsa(p, currLen)   ( (PULONG)((ULONG)p + currLen)                     )

/*---------------------------------------------------------------------------------
                        OSPF2_LSA_ACK_HEADER


 --------------------------------------------------------------------------------*/

typedef  struct
_OSPF2_LSA_ACK_HEADER
{
    OSPF2_LSA_HEADER             Lsa[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
OSPF2_LSA_ACK_HEADER,  *POSPF2_LSA_ACK_HEADER;

#define  AnscOspf2LSAAckGetFirstLsa(p)              ( (POSPF2_LSA_HEADER)(p->Lsa)                             )
#define  AnscOspf2LSAAckGetNextLsa(p)               ( (POSPF2_LSA_HEADER)((ULONG)p + OSPF2_LSA_HEADER_SIZE)   )

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
