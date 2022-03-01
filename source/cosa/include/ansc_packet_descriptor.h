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

    module:	ansc_packet_descriptor.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Packet Descriptor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/25/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_DESCRIPTOR_
#define  _ANSC_PACKET_DESCRIPTOR_


/***********************************************************
     ANSC PACKET DESCRIPTOR COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */

    #define  ANSC_ENABLE_PDO_POOLING                USER_ENABLE_PDO_POOLING
    #define  ANSC_ENABLE_BDO_POOLING                USER_ENABLE_BDO_POOLING

    #define  ANSC_MAX_PDO_POOL_SIZE                 USER_MAX_PDO_POOL_SIZE
    #define  ANSC_MAX_BDO_POOL_SIZE                 USER_MAX_BDO_POOL_SIZE


/*
 * The Internet Protocol is designed for use in interconnected systems of packet-switched computer
 * communication networks. Such a system has been called a "catenet". The internet protocol
 * provides for transmitting blocks of data called datagrams from sources to destinations, where
 * sources and destinations are hosts identified by fixed length addresses. The internet protocol
 * also provides for fragmentation and reassembly of long datagrams, if necessary, for transmission
 * through "small packet" networks.
 */
#define  ANSC_PDO_MAX_HEAD_ROOM                     16
#define  ANSC_PDO_MAX_TAIL_ROOM                     16
#define  ANSC_PDO_INFO_PAD_SIZE                     80
#define  ANSC_PDO_INFO_PAD_SIZE2                    164

#define  ANSC_PDO_FLAG_queued                       0x00000001  /* packet is queued if TRUE */

typedef  ANSC_STATUS
(*PFN_PDO_FREE_ME)
    (
        ANSC_HANDLE                 hOwnerObject,
        ANSC_HANDLE                 hPdo
    );

typedef  struct
_ANSC_PACKET_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_STATUS                     FinalStatus;
    ULONG                           Flags;
    BOOL                            bSyncOnly;
    BOOL                            bStealthy;
    BOOL                            bQueuable;
    QUEUE_HEADER                    BdoQueue;

    ANSC_HANDLE                     CurHeaderBdo;
    PVOID                           CurHeaderBuffer;
    ULONG                           CurHeaderOffset;
    char                            OsSpecificPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            T2SpecificPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            Layer2InfoPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            Layer3InfoPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            Layer4InfoPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            ErrorsInfoPad[ANSC_PDO_INFO_PAD_SIZE ];
    char                            ModelsInfoPad[ANSC_PDO_INFO_PAD_SIZE2];
    ULONG                           ErrorCode;

    ANSC_HANDLE                     hPktPassport;
    ANSC_HANDLE                     hPktSnpuPass;
    ANSC_HANDLE                     hOwnerObject;
    PFN_PDO_FREE_ME                 pfnFreeMe;
}
ANSC_PACKET_DESCRIPTOR,  *PANSC_PACKET_DESCRIPTOR;

#define  ACCESS_ANSC_PACKET_DESCRIPTOR(p)           \
         ACCESS_CONTAINER(p, ANSC_PACKET_DESCRIPTOR, Linkage)

/*
 * It's probably not a very good idea to encapsulate the Packet Descriptor as an ansc-style object
 * since the object-creation and invocation of member functions will certainly slow things down. So
 * instead, we use following macros to do the job.
 */
#define  AnscPdoGetFinalStatus(p)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->FinalStatus     )
#define  AnscPdoGetFlags(p)                         ( ((PANSC_PACKET_DESCRIPTOR)p)->Flags           )
#define  AnscPdoGetSyncOnly(p)                      ( ((PANSC_PACKET_DESCRIPTOR)p)->bSyncOnly       )
#define  AnscPdoGetStealthy(p)                      ( ((PANSC_PACKET_DESCRIPTOR)p)->bStealthy       )
#define  AnscPdoGetQueuable(p)                      ( ((PANSC_PACKET_DESCRIPTOR)p)->bQueuable       )
#define  AnscPdoGetCurHeaderBdo(p)                  ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderBdo    )
#define  AnscPdoGetCurHeaderBuffer(p)               ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderBuffer )
#define  AnscPdoGetCurHeaderOffset(p)               ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderOffset )
#define  AnscPdoGetOsSpecific(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->OsSpecificPad   )
#define  AnscPdoGetT2Specific(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->T2SpecificPad   )
#define  AnscPdoGetLayer2Info(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->Layer2InfoPad   )
#define  AnscPdoGetLayer3Info(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->Layer3InfoPad   )
#define  AnscPdoGetLayer4Info(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->Layer4InfoPad   )
#define  AnscPdoGetErrorsInfo(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->ErrorsInfoPad   )
#define  AnscPdoGetModelsInfo(p)                    ( ((PANSC_PACKET_DESCRIPTOR)p)->ModelsInfoPad   )
#define  AnscPdoGetErrorCode(p)                     ( ((PANSC_PACKET_DESCRIPTOR)p)->ErrorCode       )
#define  AnscPdoGetPktPassport(p)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->hPktPassport    )
#define  AnscPdoGetPktSnpuPass(p)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->hPktSnpuPass    )
#define  AnscPdoGetOwnerObject(p)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->hOwnerObject    )
#define  AnscPdoGetFreeFunc(p)                      ( ((PANSC_PACKET_DESCRIPTOR)p)->pfnFreeMe       )

#define  AnscPdoSetFinalStatus(p, s)                ( ((PANSC_PACKET_DESCRIPTOR)p)->FinalStatus     = s )
#define  AnscPdoSetFlags(p, f)                      ( ((PANSC_PACKET_DESCRIPTOR)p)->Flags           = f )
#define  AnscPdoSetSyncOnly(p, s)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->bSyncOnly       = s )
#define  AnscPdoSetStealthy(p, s)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->bStealthy       = s )
#define  AnscPdoSetQueuable(p, s)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->bQueuable       = s )
#define  AnscPdoSetCurHeaderBdo(p, h)               ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderBdo    = h )
#define  AnscPdoSetCurHeaderBuffer(p, h)            ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderBuffer = h )
#define  AnscPdoSetCurHeaderOffset(p, h)            ( ((PANSC_PACKET_DESCRIPTOR)p)->CurHeaderOffset = h )
#define  AnscPdoSetErrorCode(p, c)                  ( ((PANSC_PACKET_DESCRIPTOR)p)->ErrorCode       = c )
#define  AnscPdoSetPktPassport(p, o)                ( ((PANSC_PACKET_DESCRIPTOR)p)->hPktPassport    = o )
#define  AnscPdoSetPktSnpuPass(p, o)                ( ((PANSC_PACKET_DESCRIPTOR)p)->hPktSnpuPass    = o )
#define  AnscPdoSetOwnerObject(p, o)                ( ((PANSC_PACKET_DESCRIPTOR)p)->hOwnerObject    = o )
#define  AnscPdoSetFreeFunc(p, f)                   ( ((PANSC_PACKET_DESCRIPTOR)p)->pfnFreeMe       = f )

#define  AnscPdoGetBdoCount(p)                      \
         AnscQueueQueryDepth(&((PANSC_PACKET_DESCRIPTOR)p)->BdoQueue)
#define  AnscPdoGetHeadBdo(p)                       \
         ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetFirstEntry(&p->BdoQueue))
#define  AnscPdoGetTailBdo(p)                       \
         ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetLastEntry(&p->BdoQueue))
#define  AnscPdoGetNextBdo(p, b)                    \
         (b != NULL)? ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetNextEntry(&b->Linkage)) : AnscPdoGetHeadBdo(p)

#define  AnscPdoAttachBdoAtFront(p, b)              \
         AnscQueuePushEntryAtFront(&p->BdoQueue, &b->Linkage)
#define  AnscPdoAttachBdoAtBack(p, b)               \
         AnscQueuePushEntry(&p->BdoQueue, &b->Linkage)
#define  AnscPdoInsertBdoAfter(p, b, after)         \
         AnscQueuePushEntryAfter(&p->BdoQueue, &b->Linkage, &after->Linkage);
#define  AnscPdoInsertBdoBefore(p, b, before)       \
         AnscQueuePushEntryBefore(&p->BdoQueue, &b->Linkage, &before->Linkage);
#define  AnscPdoDetachBdo(p, b)                     \
         AnscQueuePopEntryByLink(&p->BdoQueue, &b->Linkage)
#define  AnscPdoDeleteBdo(p, b)                     \
         AnscPdoDetachBdo(p, b);AnscFreeBdo((ANSC_HANDLE)b)
#define  AnscPdoUnlinkBdo(p)                        \
         ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueuePopEntry(&p->BdoQueue))
#define  AnscPdoResetBdoQueue(p)                    \
         AnscQueueInitializeHeader(&p->BdoQueue)


/***********************************************************
         MODEL-SPECIFIC PACKET DESCRIPTOR DEFINITIONS
***********************************************************/

/*
 * The 'ModelsInfoPad' field is used by a specific Model Implementation to perform efficient Data
 * Path Management. Each Model Implementation MUST derive its own PACKET_INFO structure from the
 * defintion below in order to ensure compatibility with existing network components.
 */
#define  ANSC_PKT_MODEL_RGWM                        1
#define  ANSC_PKT_MODEL_EGWM                        2
#define  ANSC_PKT_MODEL_GIAD                        ANSC_PKT_MODEL_EGWM
#define  ANSC_PKT_MODEL_CBHM                        3

#define  ANSC_PKT_PROCESS_HISTORY_forwarder         0x00000001  /* has been forwarded by fast forwarder    */
#define  ANSC_PKT_PROCESS_HISTORY_ids               0x00000002  /* survived intrusion detection            */
#define  ANSC_PKT_PROCESS_HISTORY_fwmarking         0x00000004  /* obsolete, should not be set by firewall */
#define  ANSC_PKT_PROCESS_HISTORY_fwingress         0x00000008  /* processed by firewall ingress hook      */
#define  ANSC_PKT_PROCESS_HISTORY_fwegress          0x00000010
#define  ANSC_PKT_PROCESS_HISTORY_natsrcip          0x00000020
#define  ANSC_PKT_PROCESS_HISTORY_natdstip          0x00000040
#define  ANSC_PKT_PROCESS_HISTORY_softnpu10         0x00000080
#define  ANSC_PKT_PROCESS_HISTORY_ipsece            0x00000100
#define  ANSC_PKT_PROCESS_HISTORY_ipsecd            0x00000200
#define  ANSC_PKT_PROCESS_HISTORY_pptpse            0x00000400
#define  ANSC_PKT_PROCESS_HISTORY_pptpsd            0x00000800
#define  ANSC_PKT_PROCESS_HISTORY_l2tpse            0x00001000
#define  ANSC_PKT_PROCESS_HISTORY_l2tpsd            0x00002000
#define  ANSC_PKT_PROCESS_HISTORY_pptpce            0x00004000
#define  ANSC_PKT_PROCESS_HISTORY_pptpcd            0x00008000
#define  ANSC_PKT_PROCESS_HISTORY_l2tpce            0x00010000
#define  ANSC_PKT_PROCESS_HISTORY_l2tpcd            0x00020000
#define  ANSC_PKT_PROCESS_HISTORY_webfilter         0x00040000
#define  ANSC_PKT_PROCESS_HISTORY_qosingress        0x00080000
#define  ANSC_PKT_PROCESS_HISTORY_firstpkt          0x00100000
#define  ANSC_PKT_PROCESS_HISTORY_trustedsrc        0x00200000
#define  ANSC_PKT_PROCESS_HISTORY_gree              0x00400000
#define  ANSC_PKT_PROCESS_HISTORY_gred              0x00800000
#define  ANSC_PKT_PROCESS_HISTORY_cgmmarking        0x01000000
#define  ANSC_PKT_PROCESS_HISTORY_smcswitchl1       0x02000000
#define  ANSC_PKT_PROCESS_HISTORY_bridged           0x04000000

#define  ANSC_PKT_PROCESS_FLAG_skipForwarder        0x00000001  /* bypass fast forwarder, should be indicated to layer3 */
#define  ANSC_PKT_PROCESS_FLAG_skipSnpu10           0x00000002  /* bypass softnpu 1.0, usually set for control packets  */
#define  ANSC_PKT_PROCESS_FLAG_skipFirewall         0x00000004  /* bypass firewall, usually set for web or ALG packets  */
#define  ANSC_PKT_PROCESS_FLAG_skipQosIngress       0x00000008  /* bypass QoS data path, usually set for control packet */
#define  ANSC_PKT_PROCESS_FLAG_skipQosEgress        0x00000010  /* bypass QoS data path, usually set for control packet */
#define  ANSC_PKT_PROCESS_FLAG_skipQosControl       0x00000020  /* bypass QoS ctrl path, usually set for control packet */
#define  ANSC_PKT_PROCESS_FLAG_skipWebFilter        0x00000040  /* bypass web filter, usually set for local HTTP packet */
#define  ANSC_PKT_PROCESS_FLAG_skipTscIngress       0x00000080  /* bypass TSC layer2 decapculation, set for VPN packets */
#define  ANSC_PKT_PROCESS_FLAG_skipTscEgress        0x00000100  /* bypass TSC layer2 encapculation, set for VPN packets */
#define  ANSC_PKT_PROCESS_FLAG_skipIds              0x00000200  /* bypass the Intrusion Detection System engine         */
#define  ANSC_PKT_PROCESS_FLAG_skipPktCounting      0x00000400  /* bypass statistics collection (pkt counting) process  */
#define  ANSC_PKT_PROCESS_FLAG_skipNatChecking      0x00000800  /* bypass the NAT address screening process             */
#define  ANSC_PKT_PROCESS_FLAG_reverseForwarding    0x00001000  /* fast-forward the packet based on source IP address   */
#define  ANSC_PKT_PROCESS_FLAG_natSrcIp             0x00002000  /* nat needs to translate the source IP address         */
#define  ANSC_PKT_PROCESS_FLAG_natDstIp             0x00004000  /* nat needs to translate the destination IP address    */
#define  ANSC_PKT_PROCESS_FLAG_applyWebFilter       0x00008000  /* run packet through the web content filter at layer3  */
#define  ANSC_PKT_PROCESS_FLAG_layer3Forwarding     0x00010000  /* relay packet to layer3 for forwarding purpose only   */
#define  ANSC_PKT_PROCESS_FLAG_surpressIcmp         0x00020000  /* surpress all the ICMP error messages if it is set    */
#define  ANSC_PKT_PROCESS_FLAG_restoreOrgTos        0x00040000  /* restore the original 'ToS' field if it is set        */
#define  ANSC_PKT_PROCESS_FLAG_mapVirtualServer     0x00080000  /* create a virtual server mapping for source IP/port   */
#define  ANSC_PKT_PROCESS_FLAG_natSrcPort           0x00100000  /* nat needs to translate the source port number        */
#define  ANSC_PKT_PROCESS_FLAG_natDstPort           0x00200000  /* nat needs to translate the destination port number   */
#define  ANSC_PKT_PROCESS_FLAG_logSession           0x00400000  /* event logging is enabled for the current session     */
#define  ANSC_PKT_PROCESS_FLAG_addStunBinding       0x00800000  /* Firewall needs to creates a STUN binding after NAT   */
#define  ANSC_PKT_PROCESS_FLAG_loopbackPkt          0x01000000  /* loopback packet on the same interface if it is set   */
#define  ANSC_PKT_PROCESS_FLAG_trailPkt             0x02000000  /* trail packet on the data path from receive to send   */
#define  ANSC_PKT_PROCESS_FLAG_mustBeTunneled       0x04000000  /* must be encrypted by one of the tunneling protocols  */
#define  ANSC_PKT_PROCESS_FLAG_profiling            0x08000000  /* indicate the profiling statistics is to be collected */
#define  ANSC_PKT_PROCESS_FLAG_followRoute          0x10000000  /* indicate routing decision has already been made      */
#define  ANSC_PKT_PROCESS_FLAG_forwardToOsStack     0x20000000  /* indicate the packet is intended for external stack   */
#define  ANSC_PKT_PROCESS_FLAG_snpu30AtLink         0x40000000  /* force softnpu 3.0 processing at layer-2 link driver  */
#define  ANSC_PKT_PROCESS_FLAG_issueSnpu30Passport  0x80000000  /* force softnpu 3.0 to issue a passport only           */

typedef  ANSC_STATUS
(*PFN_PMO_TRAIL_ME)
    (
        ANSC_HANDLE                 hTrailerObject,
        ANSC_HANDLE                 hTrailerRecord,
        ANSC_HANDLE                 hPdo,
        char*                       pTrailMessage,
        BOOL                        bEndOfTrail
    );

typedef  struct
_ANSC_PACKET_MODEL_INFO
{
    ULONG                           ModelId;            /* differentiate between different models   */
    ULONG                           ProcessHistory;     /* remeber what have been applied to packet */
    ULONG                           ProcessFlags;       /* bits cleared separately after excution   */
    ULONG                           DefRouteMetric;     /* the smallest def route metric acceptable */
    ANSC_HANDLE                     hTrailerObject;     /* the object that is trailing the packet   */
    ANSC_HANDLE                     hTrailerRecord;     /* the record that is trailing the packet   */
    PFN_PMO_TRAIL_ME                pfnTrailMe;         /* the callback that tracks the data path   */
}
ANSC_PACKET_MODEL_INFO,  *PANSC_PACKET_MODEL_INFO;


#endif
