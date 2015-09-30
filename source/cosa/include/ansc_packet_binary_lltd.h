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

    module:	ansc_packet_binary_lltd.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Link Layer Topology Discovery
        Protocol (LLTD).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/23/07    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_LLTD_
#define  _ANSC_PACKET_BINARY_LLTD_


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
                 DATA STRUCTURES USED BY LLTD
***********************************************************/

/*
 * As the protocol name suggests, the core functions of LLTD enable applications to discover the
 * topology of a network. In addition, LLTD has optional QoS extensions that application can use to
 * diagnose problems, especially those involving signal strength on wireless networks or bandwidth
 * constraints in home networks.
 */
#define  LLTD_CURRENT_VERSION                       1

#define  LLTD_TOS_TopologyDiscovery                 0x00
#define  LLTD_TOS_QuickDiscovery                    0x01
#define  LLTD_TOS_QosDiagnostics                    0x02

/* 
 * The Function field unambiguously differentiates the multiplex of messages for a given type of
 * service. The following functions are valid for service type 0x00:
 */
#define  LLTD_FUNCTION_Discover                     0x00
#define  LLTD_FUNCTION_Hello                        0x01
#define  LLTD_FUNCTION_Emit                         0x02
#define  LLTD_FUNCTION_Train                        0x03
#define  LLTD_FUNCTION_Probe                        0x04
#define  LLTD_FUNCTION_Ack                          0x05
#define  LLTD_FUNCTION_Query                        0x06
#define  LLTD_FUNCTION_QueryResp                    0x07
#define  LLTD_FUNCTION_Reset                        0x08
#define  LLTD_FUNCTION_Charge                       0x09
#define  LLTD_FUNCTION_Flat                         0x0A
#define  LLTD_FUNCTION_QueryLargeTlv                0x0B
#define  LLTD_FUNCTION_QueryLargeTlvResp            0x0C

#define  LLTD_FUNCTION_QosInitializeSink            0x00        /* following functions are valid for service type 0x02 */
#define  LLTD_FUNCTION_QosReady                     0x01
#define  LLTD_FUNCTION_QosProbe                     0x02
#define  LLTD_FUNCTION_QosQuery                     0x03
#define  LLTD_FUNCTION_QosQueryResp                 0x04
#define  LLTD_FUNCTION_QosReset                     0x05
#define  LLTD_FUNCTION_QosError                     0x06
#define  LLTD_FUNCTION_QosAck                       0x07
#define  LLTD_FUNCTION_QosCounterSnapshot           0x08
#define  LLTD_FUNCTION_QosCounterResult             0x09
#define  LLTD_FUNCTION_QosCounterLease              0x0A

/*
 * The following shows the position of each layer of header in the LLTD protocol.
 *
 *      +--------------------+
 *      |      Ethernet      |
 *      |       Header       |
 *      +--------------------+
 *      |    Demultiplex     |
 *      |       Header       |
 *      +--------------------+
 *      |        Base        |
 *      |       Header       |
 *      +--------------------+
 *      :     Upper-Level    :
 *      :       Header       :
 *
 * A following is a summary of the demultiplex header:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |    Version    |Type of Service|    Reserved   |    Function   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_DEMULTIPLEX_HEADER
{
    UCHAR                           Version;
    UCHAR                           TypeOfService;
    UCHAR                           Reserved;
    UCHAR                           Function;
}_struct_pack_
LLTD_DEMULTIPLEX_HEADER,  *PLLTD_DEMULTIPLEX_HEADER;

#define  AnscLltdDemulGetVersion(p)                 ( p->Version       )
#define  AnscLltdDemulGetTypeOfService(p)           ( p->TypeOfService )
#define  AnscLltdDemulGetReserved(p)                ( p->Reserved      )
#define  AnscLltdDemulGetFunction(p)                ( p->Function      )

#define  AnscLltdDemulSetVersion(p, v)              ( p->Version       = v )
#define  AnscLltdDemulSetTypeOfService(p, t)        ( p->TypeOfService = t )
#define  AnscLltdDemulSetReserved(p, r)             ( p->Reserved      = r )
#define  AnscLltdDemulSetFunction(p, f)             ( p->Reserved      = f )

/*
 * The base header format is as follows:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +       Network Address 1       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                               |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+       Network Address 2       +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |          Identifier           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_BASE_HEADER0
{
    UCHAR                           NetworkAddress1[ETHERNET_ADDRESS_SIZE];
    UCHAR                           NetworkAddress2[ETHERNET_ADDRESS_SIZE];
    USHORT                          Identifier;
}_struct_pack_
LLTD_BASE_HEADER0,  *PLLTD_BASE_HEADER0;

#define  AnscLltdAssignAddress(a, b)                AnscCopyMemory (a, b, ETHERNET_ADDRESS_SIZE)
#define  AnscLltdHashAddress(a, t)                  AnscHashString (a, ETHERNET_ADDRESS_SIZE, t)
#define  AnscLltdEqualAddress(a, b)                 AnscEqualMemory(a, b, ETHERNET_ADDRESS_SIZE)
#define  AnscLltdEqualAddress2(a, b)                ( (AnscReadUlong(&a[0]) == AnscReadUlong(&b[0])) && (AnscReadUshort(&a[4]) == AnscReadUshort(&b[4])) )

#define  AnscLltdBaseGetNetworkAddress1(p)          ( p->NetworkAddress1 )
#define  AnscLltdBaseGetNetworkAddress2(p)          ( p->NetworkAddress2 )
#define  AnscLltdBaseGetIdentifier(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->Identifier)) )

#define  AnscLltdBaseSetNetworkAddress1(p, addr)    ( AnscLltdAssignAddress(p->NetworkAddress1, addr) )
#define  AnscLltdBaseSetNetworkAddress2(p, addr)    ( AnscLltdAssignAddress(p->NetworkAddress2, addr) )
#define  AnscLltdBaseSetIdentifier(p, id)           AnscWriteUshort(&p->Identifier, AnscUshortFromHToN(id))


/***********************************************************
       DATA STRUCTURES USED BY LLTD DISCOVERY SERVICES
***********************************************************/

/*
 * Topology discovery is a superset of quick discovery. Henceforth, the term enumerator is used to
 * identify any station that is issuing either a quick discovery request or the enumeration portion
 * of a topology discover request. Responders must be able to perform both activities simul-
 * taneously. In topology discovery, an initial round of responder enumeration must be performed,
 * similar to what is done with quick discovery.
 *
 * The base header format is as follows:
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +   Real Destination Address    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                               |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+      Real Source Address      +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |        Sequence Number        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_BASE_HEADER
{
    UCHAR                           RealDstAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           RealSrcAddress[ETHERNET_ADDRESS_SIZE];
    USHORT                          SequenceNumber;
}_struct_pack_
LLTD_BASE_HEADER,  *PLLTD_BASE_HEADER;

#define  AnscLltdBaseGetRealDstAddress(p)           ( p->RealDstAddress )
#define  AnscLltdBaseGetRealSrcAddress(p)           ( p->RealSrcAddress )
#define  AnscLltdBaseGetSequenceNumber(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->SequenceNumber)) )

#define  AnscLltdBaseSetRealDstAddress(p, addr)     ( AnscLltdAssignAddress(p->RealDstAddress, addr) )
#define  AnscLltdBaseSetRealSrcAddress(p, addr)     ( AnscLltdAssignAddress(p->RealSrcAddress, addr) )
#define  AnscLltdBaseSetSequenceNumber(p, n)        AnscWriteUshort(&p->SequenceNumber, AnscUshortFromHToN(n))

/*
 * The discover header immediately follows the base header:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |       Generation Number       |      Number of Stations       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      : Station List (variable) ...                                   :
 *
 * Generation Number: 16 bits
 *      This field allows the mapper to negotiate a generation number with the responders that
 *      respond to the Discover frame. Ultimately, this number allows the mapper to generate a
 *      unique range of MAC addresses from the reserved topology discovery address pool that does
 *      not conflict with those from a recent topology discovery process.
 *
 * Number of Stations: 16 bits
 *      This field indicates the number of station addresses that are present in the following
 *      station list.
 *
 * Station List: variable
 *      This field is a sequence of 6 octet MAC addresses. The length of the sequence is given by
 *      the Number of Stations field. For example, a station list that contains two addresses
 *      a1:b1:c1:d1:e1:f1 and a2:b2:c2:d2:e2:f2 is encoded as follows:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |      a1       |      b1       |      c1       |      d1       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |      e1       |      f1       |      a2       |      b2       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |      c2       |      d2       |      e2       |      f2       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_DISCOVER_HEADER
{
    USHORT                          GenerationNumber;
    USHORT                          NumberOfStations;
    UCHAR                           StationList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_DISCOVER_HEADER,  *PLLTD_DISCOVER_HEADER;

#define  AnscLltdDiscoverGetGenerationNumber(p)     ( AnscUshortFromNToH(AnscReadUshort(&p->GenerationNumber)) )
#define  AnscLltdDiscoverGetNumberOfStations(p)     ( AnscUshortFromNToH(AnscReadUshort(&p->NumberOfStations)) )
#define  AnscLltdDiscoverGetStationList(p)          ( p->StationList )
#define  AnscLltdDiscoverGetStationAddress(p, n)    ( &p->StationList[n * 6] )

#define  AnscLltdDiscoverSetGenerationNumber(p, n)  AnscWriteUshort(&p->GenerationNumber, AnscUshortFromHToN(n))
#define  AnscLltdDiscoverSetNumberOfStations(p, n)  AnscWriteUshort(&p->NumberOfStations, AnscUshortFromHToN(n))
#define  AnscLltdDiscoverSetStationAddress(p, addr, n)                                      \
         ( AnscLltdAssignAddress((&p->StationList[n * 6]), addr) )

#define  AnscLltdDiscoverAckResponder(p, addr, b_result)                                    \
         {                                                                                  \
            ULONG                   ii          = 0;                                        \
            ULONG                   station_num = AnscLltdDiscoverGetNumberOfStations(p);   \
                                                                                            \
            b_result = FALSE;                                                               \
                                                                                            \
            for ( ii = 0; ii < station_num; ii++ )                                          \
            {                                                                               \
                if ( AnscEthernetEqualAddress(addr, &p->StationList[ii * 6]) )              \
                {                                                                           \
                    b_result = TRUE;                                                        \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
         }

/*
 * Hello frames must be broadcasted so that all switches are made aware of the location of all
 * responders. The hello header following a base header is:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |       Generation Number       |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+    Current Mapper Address     +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +    Apparent Mapper Address    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                               | TLV List (variable) ...       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *      : TLV List (variable) ...                                       :
 */
typedef  struct
_LLTD_HELLO_HEADER
{
    USHORT                          GenerationNumber;
    UCHAR                           CurrentMapperAddress [ETHERNET_ADDRESS_SIZE];
    UCHAR                           ApparentMapperAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           TlvList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_HELLO_HEADER,  *PLLTD_HELLO_HEADER;

#define  AnscLltdHelloGetGenerationNumber(p)        ( AnscUshortFromNToH(AnscReadUshort(&p->GenerationNumber)) )
#define  AnscLltdHelloGetCurrentMapperAddress(p)    ( p->CurrentMapperAddress  )
#define  AnscLltdHelloGetApparentMapperAddress(p)   ( p->ApparentMapperAddress )
#define  AnscLltdHelloGetTlvList(p)                 ( p->TlvList )
#define  AnscLltdHelloGetFirstTlv(p)                ( (PLLTD_TLV)p->TlvList )

#define  AnscLltdHelloSetGenerationNumber(p, n)     AnscWriteUshort(&p->GenerationNumber, AnscUshortFromHToN(n))
#define  AnscLltdHelloSetCurrentMapperAddress(p, addr)                                      \
         ( AnscLltdAssignAddress(p->CurrentMapperAddress, addr) )
#define  AnscLltdHelloSetApparentMapperAddress(p, addr)                                     \
         ( AnscLltdAssignAddress(p->ApparentMapperAddress, addr) )

/*
 * An Emit frame is a list of source and destination MAC addresses that is prefixed by number of
 * milliseconds to pause before sending a frame. The Emit frame following a base header is:
 *
 *       0                   1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *      |           Num Descs           | EmiteeDescs (variable) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *
 * Each EmiteeDesc item is a 14-octet structure:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Type      |     Pause     |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+        Source Address         +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +      Destination Address      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_EMIT_HEADER
{
    USHORT                          NumberOfDescs;
    UCHAR                           EmiteeDescs[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_EMIT_HEADER,  *PLLTD_EMIT_HEADER;

#define  AnscLltdEmitGetNumberOfDescs(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->NumberOfDescs)) )
#define  AnscLltdEmitGetEmiteeDescs(p)              ( p->EmiteeDescs )
#define  AnscLltdEmitGetEmiteeDesc(p, n)            ( &p->EmiteeDescs[sizeof(LLTD_EMITEE_DESC) * n] )

#define  AnscLltdEmitSetNumberOfDescs(p, n)         AnscWriteUshort(&p->NumberOfDescs, AnscUshortFromHToN(n))

#define  LLTD_EMIT_TYPE_Train                       0x00
#define  LLTD_EMIT_TYPE_Probe                       0x01

typedef  struct
_LLTD_EMITEE_DESC
{
    UCHAR                           Type;
    UCHAR                           Pause;
    UCHAR                           SrcAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           DstAddress[ETHERNET_ADDRESS_SIZE];
}_struct_pack_
LLTD_EMITEE_DESC,  *PLLTD_EMITEE_DESC;

#define  AnscLltdEmiteeDescGetType(p)               ( p->Type       )
#define  AnscLltdEmiteeDescGetPause(p)              ( p->Pause      )
#define  AnscLltdEmiteeDescGetSrcAddress(p)         ( p->SrcAddress )
#define  AnscLltdEmiteeDescGetDstAddress(p)         ( p->DstAddress )

#define  AnscLltdEmiteeDescSetType(p, t)            ( p->Type  = t )
#define  AnscLltdEmiteeDescSetPause(p, s)           ( p->Pause = s )
#define  AnscLltdEmiteeDescSetSrcAddress(p, addr)   ( AnscLltdAssignAddress(p->SrcAddress, addr) )
#define  AnscLltdEmiteeDescSetDstAddress(p, addr)   ( AnscLltdAssignAddress(p->DstAddress, addr) )

/*
 * QueryResp is the response to a query. It lists which recordable events (such as Ethernet source
 * and Ethernet destination addresses) have been observed on the wire since the previous Query
 * frame. (The Query frame removes reported events from the responder's topology mapping engine's
 * internal list.) QueryResp frames are not ACKed but must set the base header's sequence number
 * field to match the Query frame to which they are generated in response. Responders that are
 * sending this frame must not merge identical recordable events (RecveeDescs items) even if they
 * occur multiple times. The ordering of RecveeDesc items in this frame should represent arrival
 * time ordering. If there are more triples than will fit in one frame, "num descs" has its top (M)
 * bit set to indicate that further pairs follow. If the mapper receives a QueryResp frame with the
 * M bit set, it should issue a fresh Query frame (with new sequence number) to the responder to
 * collect additional RecveeDescs items from it.
 *
 * The QueryResp frame that follows a base header is:
 *
 *       0                   1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *      |M|E|  Reserved |   Num Descs   | RecveeDescs (variable) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *
 * Each RecveeDesc item is a 20-octet structure:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |             Type              |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+      Real Source Address      +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +    Ethernet Source Address    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                               |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Ethernet Destination Address  |
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define  LLTD_QUERY_RESP_M_FLAG                     0x80
#define  LLTD_QUERY_RESP_E_FLAG                     0x40

typedef  struct
_LLTD_QUERY_RESP_HEADER
{
    UCHAR                           Flags;
    UCHAR                           NumberOfDescs;
    UCHAR                           RecveeDescs[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_QUERY_RESP_HEADER,  *PLLTD_QUERY_RESP_HEADER;

#define  AnscLltdQueryRespIsMFlagSet(p)             ( (p->Flags & LLTD_QUERY_RESP_M_FLAG) != 0 )
#define  AnscLltdQueryRespIsEFlagSet(p)             ( (p->Flags & LLTD_QUERY_RESP_E_FLAG) != 0 )

#define  AnscLltdQueryRespGetFlags(p)               ( p->Flags         )
#define  AnscLltdQueryRespGetNumberOfDescs(p)       ( p->NumberOfDescs )
#define  AnscLltdQueryRespGetRecveeDescs(p)         ( p->RecveeDescs   )
#define  AnscLltdQueryRespGetRecveeDesc(p, n)       ( &p->RecveeDescs[sizeof(LLTD_RECVEE_DESC) * n] )

#define  AnscLltdQueryRespSetMFlag(p)               ( p->Flags |=  LLTD_QUERY_RESP_M_FLAG )
#define  AnscLltdQueryRespSetEFlag(p)               ( p->Flags |=  LLTD_QUERY_RESP_E_FLAG )
#define  AnscLltdQueryRespClearMFlag(p)             ( p->Flags &= ~LLTD_QUERY_RESP_M_FLAG )
#define  AnscLltdQueryRespClearEFlag(p)             ( p->Flags &= ~LLTD_QUERY_RESP_E_FLAG )

#define  AnscLltdQueryRespSetFlags(p, f)            ( p->Flags         = f )
#define  AnscLltdQueryRespSetNumberOfDescs(p, n)    ( p->NumberOfDescs = n )

#define  LLTD_RECV_PROTOCOL_TYPE_Probe              0
#define  LLTD_RECV_PROTOCOL_TYPE_ARP_ICMPv6         1

typedef  struct
_LLTD_RECVEE_DESC
{
    USHORT                          Type;
    UCHAR                           RealSrcAddress    [ETHERNET_ADDRESS_SIZE];
    UCHAR                           EthernetSrcAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           EthernetDstAddress[ETHERNET_ADDRESS_SIZE];
}_struct_pack_
LLTD_RECVEE_DESC,  *PLLTD_RECVEE_DESC;

#define  AnscLltdRecveeDescGetType(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->Type)) )
#define  AnscLltdRecveeDescGetRealSrcAddress(p)     ( p->RealSrcAddress     )
#define  AnscLltdRecveeDescGetEthernetSrcAddress(p) ( p->EthernetSrcAddress )
#define  AnscLltdRecveeDescGetEthernetDstAddress(p) ( p->EthernetDstAddress )

#define  AnscLltdRecveeDescSetType(p, t)            AnscWriteUshort(&p->Type, AnscUshortFromHToN(t))
#define  AnscLltdRecveeDescSetRealSrcAddress(p, addr)                                       \
         ( AnscLltdAssignAddress(p->RealSrcAddress, addr) )
#define  AnscLltdRecveeDescSetEthernetSrcAddress(p, addr)                                   \
         ( AnscLltdAssignAddress(p->EthernetSrcAddress, addr) )
#define  AnscLltdRecveeDescSetEthernetDstAddress(p, addr)                                   \
         ( AnscLltdAssignAddress(p->EthernetDstAddress, addr) )

/*
 * The Flat frame following a base header is:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Current Transmit Credit (CTC) in Bytes             |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |        CTC in Packets         |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_FLAT_HEADER
{
    ULONG                           CTCInBytes;
    USHORT                          CTCInPackets;
}_struct_pack_
LLTD_FLAT_HEADER,  *PLLTD_FLAT_HEADER;

#define  AnscLltdFlatGetCTCInBytes(p)               ( AnscUlongFromNToH (AnscReadUlong (&p->CTCInBytes  )) )
#define  AnscLltdFlatGetCTCInPackets(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->CTCInPackets)) )

#define  AnscLltdFlatSetCTCInBytes(p, c)            AnscWriteUlong (&p->CTCInBytes,   AnscUlongFromHToN (c))
#define  AnscLltdFlatSetCTCInPackets(p, c)          AnscWriteUshort(&p->CTCInPackets, AnscUshortFromHToN(c))

/*
 * The QueryLargeTlv frame allows the mapper to query a responder for TLVs that are too large to
 * fit into a single Hello frame. Each QueryLargeTlv request results in a maximum of one
 * QueryLargeTlvResp response. Repeated QueryLargeTlv requests would have to be made for
 * sufficiently large TLVs that do not fit in a single QueryLargeTlvResp response frame.
 *
 * The QueryLargeTlv frame that follows a base header is:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Type      |                    Offset                     |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_QUERY_LTLV_HEADER
{
    UCHAR                           Type;
    UCHAR                           Offset1;
    USHORT                          Offset2;
}_struct_pack_
LLTD_QUERY_LTLV_HEADER,  *PLLTD_QUERY_LTLV_HEADER;

#define  AnscLltdQueryLtlvGetType(p)                ( p->Type )
#define  AnscLltdQueryLtlvGetOffset(p)              ( (ULONG)(p->Offset1 * 256 * 256) + AnscUshortFromNToH(AnscReadUshort(&p->Offset2)) )

#define  AnscLltdQueryLtlvSetType(p, t)             ( p->Type   = t )
#define  AnscLltdQueryLtlvSetOffset(p, o)                                                   \
         {                                                                                  \
            p->Offset1 = (UCHAR )(o / (256 * 256));                                         \
            p->Offset2 = (USHORT)(o % (256 * 256));                                         \
                                                                                            \
            AnscWriteUshort(&p->Offset2, AnscUshortFromHToN(p->Offset2));                   \
         }

/*
 * The QueryLargeTlvResp frame is a response to a QueryLargeTlv request. It returns the maximum
 * relevant octets that would fit into a response frame over the Ethernet media from a requested
 * offset. If a QueryLargeTlv request is for an unsupported TLV type, a QueryLargeTlvResp frame
 * must be sent with the Length field zeroed.
 *
 * The QueryLargeTlvResp header immediately follows the base header:
 *
 *       0                   1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *      |M|R|         Length            | Data (variable) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 */
#define  LLTD_QUERY_LTLV_RESP_M_FLAG                0x80
#define  LLTD_QUERY_LTLV_RESP_R_FLAG                0x40

typedef  struct
_LLTD_QUERY_LTLV_RESP_HEADER
{
    UCHAR                           FlagsAndLength1;
    UCHAR                           Length2;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_QUERY_LTLV_RESP_HEADER,  *PLLTD_QUERY_LTLV_RESP_HEADER;

#define  AnscLltdQueryLtlvRespIsMFlagSet(p)         ( (p->FlagsAndLength1 & LLTD_QUERY_LTLV_RESP_M_FLAG) != 0 )
#define  AnscLltdQueryLtlvRespIsRFlagSet(p)         ( (p->FlagsAndLength1 & LLTD_QUERY_LTLV_RESP_R_FLAG) != 0 )

#define  AnscLltdQueryLtlvRespGetLength(p)          ( (USHORT)((p->FlagsAndLength1 & 0x3F) * 256) + (USHORT)p->Length2 )
#define  AnscLltdQueryLtlvRespGetData(p)            ( p->Data )

#define  AnscLltdQueryLtlvRespSetMFlag(p)           ( p->FlagsAndLength1 |=  LLTD_QUERY_LTLV_RESP_M_FLAG )
#define  AnscLltdQueryLtlvRespSetRFlag(p)           ( p->FlagsAndLength1 |=  LLTD_QUERY_LTLV_RESP_R_FLAG )
#define  AnscLltdQueryLtlvRespClearMFlag(p)         ( p->FlagsAndLength1 &= ~LLTD_QUERY_LTLV_RESP_M_FLAG )
#define  AnscLltdQueryLtlvRespClearRFlag(p)         ( p->FlagsAndLength1 &= ~LLTD_QUERY_LTLV_RESP_R_FLAG )

#define  AnscLltdQueryLtlvRespSetLength(p, l)                                               \
         {                                                                                  \
            p->Length2          = (UCHAR)(l / 256);                                         \
            p->FlagsAndLength1 |= (p->Length2 & 0x3F);                                      \
            p->Length2          = (UCHAR)(l % 256);                                         \
         }
#define  AnscLltdQueryLtlvRespSetData(p, d)         AnscCopy(p->Data, d, AnscLltdQueryLtlvRespGetLength(p))


/***********************************************************
        DATA STRUCTURES USED BY LLTD QOS NETWORK TEST
***********************************************************/

/*
 * To set up a network test session, a QosInitializeSink frame is sent to the sink. The
 * QosInitializeSink header that follows the base header is:
 *
 *      +-+-+-+-+-+-+-+-+
 *      | Interrupt Mod |
 *      +-+-+-+-+-+-+-+-+
 *
 * 'Interrupt Mod' (I) flag: 8 bits
 *
 *      This field is set to indicate the interrupt moderation requirement of a network test session:
 *          0x00	=	Disable interrupt moderation
 *          0x01 	= 	Enable interrupt moderation
 *          0xFF 	= 	Use existing interrupt moderation setting
 */
#define  LLTD_QOS_INTERRUPT_MOD_DisableInterrupt    0x00
#define  LLTD_QOS_INTERRUPT_MOD_EnableInterrupt     0x01
#define  LLTD_QOS_INTERRUPT_MOD_UseExistingSetting  0xFF

typedef  struct
_LLTD_QOS_INIT_SINK_HEADER
{
    UCHAR                           InterruptMod;
}_struct_pack_
LLTD_QOS_INIT_SINK_HEADER,  *PLLTD_QOS_INIT_SINK_HEADER;

#define  AnscLltdQosInitSinkGetInterruptMod(p)      ( p->InterruptMod     )
#define  AnscLltdQosInitSinkSetInterruptMod(p, m)   ( p->InterruptMod = m )

/*
 * A QosReady frame is sent in reply to a QosInitializeSink frame, to confirm the creation or
 * existence of a network test session. Note that a QosReady frame is sent even if the network test
 * session already exists. The QosReady header that follows a base header is:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                        Sink Link Speed                        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                 Performance Counter Frequency                 +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_QOS_READY_HEADER
{
    ULONG                           SinkLinkSpeed;
    ULONG                           PerformanceCounterFrequency1;
    ULONG                           PerformanceCounterFrequency2;
}_struct_pack_
LLTD_QOS_READY_HEADER,  *PLLTD_QOS_READY_HEADER;

#define  AnscLltdQosReadyGetSinkLinkSpeed(p)                                                \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkLikSpeed)) )
#define  AnscLltdQosReadyGetPerformanceCounterFrequency1(p)                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->PerformanceCounterFrequency1)) )
#define  AnscLltdQosReadyGetPerformanceCounterFrequency2(p)                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->PerformanceCounterFrequency2)) )

#define  AnscLltdQosReadySetSinkLinkSpeed(p, s)                                             \
         AnscWriteUlong(&p->SinkLikSpeed, AnscUlongFromHToN(s))
#define  AnscLltdQosReadySetPerformanceCounterFrequency1(p, f)                              \
         AnscWriteUlong(&p->PerformanceCounterFrequency1, AnscUlongFromHToN(f))
#define  AnscLltdQosReadySetPerformanceCounterFrequency2(p, f)                              \
         AnscWriteUlong(&p->PerformanceCounterFrequency2, AnscUlongFromHToN(f))

/*
 * The QosProbe frame should be time stamped upon transmission and again when it is received.
 * Responders receiving QosProbe frames should log to their event list the two timestamps, ready to
 * report them in a subsequent QosQueryResp frame.
 *
 * In probegap analysis, a QosProbe frame is transmitted by the controller, received by the sink,
 * and then returned by the sink to the Controller. The frame is time stamped by the controller and
 * time stamped by the sink when it is received and again when it is returned to the controller.
 * The controller makes a final timestamp when it receives the QosProbe packet from the sink.
 *
 * In timed probe analysis, the controller may send up to 82 consecutive QosProbe frames. This
 * represents the maximum number of records that may be returned in a single QosQueryResp frame.
 * Sequence numbering is used only for probegap test type.
 *
 * The QosProbe header that follows the base header is:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                 Controller Transmit Timestamp                 +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                    Sink Receive Timestamp                     +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                    Sink Transmit Timestamp                    +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Test Type   |   Packet ID   |T|802.1p Value | Payload ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 */
#define  LLTD_QOS_TEST_TYPE_TimedProbe              0x00
#define  LLTD_QOS_TEST_TYPE_ProbeGapFromController  0x01
#define  LLTD_QOS_TEST_TYPE_ProbeGapFromSink        0x02

#define  LLTD_QOS_PROBE_T_FLAG                      0x80

typedef  struct
_LLTD_QOS_PROBE_HEADER
{
    ULONG                           ControllerTransmitTimestamp1;
    ULONG                           ControllerTransmitTimestamp2;
    ULONG                           SinkReceiveTimestamp1;
    ULONG                           SinkReceiveTimestamp2;
    ULONG                           SinkTransmitTimestamp1;
    ULONG                           SinkTransmitTimestamp2;
    UCHAR                           TestType;
    UCHAR                           PacketID;
    UCHAR                           T_802_1p_Value;
}_struct_pack_
LLTD_QOS_PROBE_HEADER,  *PLLTD_QOS_PROBE_HEADER;

#define  AnscLltdQosProbeGetControllerTransmitTimestamp1(p)                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ControllerTransmitTimestamp1)) )
#define  AnscLltdQosProbeGetControllerTransmitTimestamp2(p)                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ControllerTransmitTimestamp2)) )
#define  AnscLltdQosProbeGetSinkReceiveTimestamp1(p)                                        \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkReceiveTimestamp1)) )
#define  AnscLltdQosProbeGetSinkReceiveTimestamp2(p)                                        \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkReceiveTimestamp2)) )
#define  AnscLltdQosProbeGetSinkTransmitTimestamp1(p)                                       \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkTransmitTimestamp1)) )
#define  AnscLltdQosProbeGetSinkTransmitTimestamp2(p)                                       \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkTransmitTimestamp2)) )

#define  AnscLltdQosProbeIsTFlagSet(p)              ( (p->T_802_1p_Value & LLTD_QOS_PROBE_T_FLAG) != 0 )
#define  AnscLltdQosProbeGetTestType(p)             ( p->TestType              )
#define  AnscLltdQosProbeGetPacketID(p)             ( p->PacketID              )
#define  AnscLltdQosProbeGet802_1pValue(p)          ( p->T_802_1p_Value & ~LLTD_QOS_PROBE_T_FLAG )

#define  AnscLltdQosProbeSetControllerTransmitTimestamp1(p, t)                              \
         AnscWriteUlong(&p->ControllerTransmitTimestamp1, AnscUlongFromHToN(t))
#define  AnscLltdQosProbeSetControllerTransmitTimestamp2(p, t)                              \
         AnscWriteUlong(&p->ControllerTransmitTimestamp2, AnscUlongFromHToN(t))
#define  AnscLltdQosProbeSetSinkReceiveTimestamp1(p, t)                                     \
         AnscWriteUlong(&p->SinkReceiveTimestamp1, AnscUlongFromHToN(t))
#define  AnscLltdQosProbeSetSinkReceiveTimestamp2(p, t)                                     \
         AnscWriteUlong(&p->SinkReceiveTimestamp2, AnscUlongFromHToN(t))
#define  AnscLltdQosProbeSetSinkTransmitTimestamp1(p, t)                                    \
         AnscWriteUlong(&p->SinkTransmitTimestamp1, AnscUlongFromHToN(t))
#define  AnscLltdQosProbeSetSinkTransmitTimestamp2(p, t)                                    \
         AnscWriteUlong(&p->SinkTransmitTimestamp2, AnscUlongFromHToN(t))

#define  AnscLltdQosProbeSetTFlag(p)                ( p->T_802_1p_Value |=  LLTD_QOS_PROBE_T_FLAG )
#define  AnscLltdQosProbeClearTFlag(p)              ( p->T_802_1p_Value &= ~LLTD_QOS_PROBE_T_FLAG )
#define  AnscLltdQosProbeSetTestType(p, t)          ( p->TestType = t  )
#define  AnscLltdQosProbeSetPacketID(p, id)         ( p->PacketID = id )
#define  AnscLltdQosProbeSet802_1pValue(p, v)       ( p->T_802_1p_Value = AnscLltdQosProbeIsTFlagSet(p)? (v | LLTD_QOS_PROBE_T_FLAG) : (v & ~LLTD_QOS_PROBE_T_FLAG) )

/*
 * The QosQueryResp frame is the response to a QosQuery frame. It lists QosProbe events (also known
 * as "QosEventDesc structures") that have been observed since the previous QosQuery frame.
 * QosQueryResp frames are not acknowledged, but must set the base header's Identifier field to
 * match the QosQuery frame that is generated in response to the QosQueryResp frame. The ordering
 * of QosEventDesc items in this frame should represent the ordering of the arrival time.
 *
 * The QosQueryResp header that follows the base header is:
 *
 *       0                   1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *      |R|E|        Num Events         | QosEventDesc list (variable) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- . . . .
 *
 * 'Reserved' (R) flag: 1 bit
 *      This field must be set to zero.
 *
 * 'Error' (E) flag: 1 bit
 *      This field indicates that the responder was unable to allocate enough memory for one or
 *      more QosEventDesc structures. The Num Events field should be zero, and no QosEventDesc
 *      structures should follow the field.
 *
 * Num Events: 14 bits
 *      This field identifies the count of QosEventDesc items that follow.
 *
 * Each QosEventDesc item is an 18-octet structure:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                 Controller Transmit Timestamp                 +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                    Sink Receive Timestamp                     +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Packet ID   |   Reserved    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define  LLTD_QOS_QUERY_RESP_R_FLAG                 0x80
#define  LLTD_QOS_QUERY_RESP_E_FLAG                 0x40

typedef  struct
_LLTD_QOS_QUERY_RESP_HEADER
{
    UCHAR                           FlagsAndNumberOfEvents1;
    UCHAR                           FlagsAndNumberOfEvents2;
    UCHAR                           QosEventDescs[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_QOS_QUERY_RESP_HEADER,  *PLLTD_QOS_QUERY_RESP_HEADER;

#define  AnscLltdQosQueryRespIsRFlagSet(p)          ( (p->FlagsAndNumberOfEvents1 & LLTD_QOS_QUERY_RESP_R_FLAG) != 0 )
#define  AnscLltdQosQueryRespIsEFlagSet(p)          ( (p->FlagsAndNumberOfEvents1 & LLTD_QOS_QUERY_RESP_E_FLAG) != 0 )

#define  AnscLltdQosQueryRespGetNumberOfEvents(p)   ( (USHORT)((p->FlagsAndNumberOfEvents1 & 0x3F) * 256) + (USHORT)p->FlagsAndNumberOfEvents2 )
#define  AnscLltdQosQueryRespGetQosEventDescs(p)    ( p->QosEventDescs )
#define  AnscLltdQosQueryRespGetQosEventDesc(p, n)  (&p->QosEventDescs[sizeof(LLTD_QOS_EVENT_DESC) * n] )

#define  AnscLltdQosQueryRespSetRFlag(p)            ( p->FlagsAndNumberOfEvents1 |=  LLTD_QOS_QUERY_RESP_R_FLAG )
#define  AnscLltdQosQueryRespSetEFlag(p)            ( p->FlagsAndNumberOfEvents1 |=  LLTD_QOS_QUERY_RESP_E_FLAG )
#define  AnscLltdQosQueryRespClearRFlag(p)          ( p->FlagsAndNumberOfEvents1 &= ~LLTD_QOS_QUERY_RESP_R_FLAG )
#define  AnscLltdQosQueryRespClearEFlag(p)          ( p->FlagsAndNumberOfEvents1 &= ~LLTD_QOS_QUERY_RESP_E_FLAG )

#define  AnscLltdQosQueryRespSetNumberOfEvents(p, n)                                        \
         {                                                                                  \
            p->FlagsAndNumberOfEvents2  = (UCHAR)(n / 256);                                 \
            p->FlagsAndNumberOfEvents1 &= LLTD_QOS_QUERY_RESP_R_FLAG;                       \
            p->FlagsAndNumberOfEvents1 &= LLTD_QOS_QUERY_RESP_E_FLAG;                       \
            p->FlagsAndNumberOfEvents1 |= p->FlagsAndNumberOfEvents2;                       \
            p->FlagsAndNumberOfEvents2  = (UCHAR)(n % 256);                                 \
         }

#define  LLTD_RECV_PROTOCOL_TYPE_Probe              0
#define  LLTD_RECV_PROTOCOL_TYPE_ARP_ICMPv6         1

typedef  struct
_LLTD_QOS_EVENT_DESC
{
    ULONG                           ControllerTransmitTimestamp1;
    ULONG                           ControllerTransmitTimestamp2;
    ULONG                           SinkReceiveTimestamp1;
    ULONG                           SinkReceiveTimestamp2;
    UCHAR                           PacketID;
    UCHAR                           Reserved;
}_struct_pack_
LLTD_QOS_EVENT_DESC,  *PLLTD_QOS_EVENT_DESC;

#define  AnscLltdQosEventDescrGetControllerTransmitTimestamp1(p)                            \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ControllerTransmitTimestamp1)) )
#define  AnscLltdQosEventDescrGetControllerTransmitTimestamp2(p)                            \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ControllerTransmitTimestamp2)) )
#define  AnscLltdQosEventDescrGetSinkReceiveTimestamp1(p)                                   \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkReceiveTimestamp1)) )
#define  AnscLltdQosEventDescrGetSinkReceiveTimestamp2(p)                                   \
         ( AnscUlongFromNToH(AnscReadUlong(&p->SinkReceiveTimestamp2)) )

#define  AnscLltdQosEventDescrGetPacketID(p)        ( p->PacketID )
#define  AnscLltdQosEventDescrGetReserved(p)        ( p->Reserved )

#define  AnscLltdQosEventDescrSetControllerTransmitTimestamp1(p, t)                         \
         AnscWriteUlong(&p->ControllerTransmitTimestamp1, AnscUlongFromHToN(t))
#define  AnscLltdQosEventDescrSetControllerTransmitTimestamp2(p, t)                         \
         AnscWriteUlong(&p->ControllerTransmitTimestamp2, AnscUlongFromHToN(t))
#define  AnscLltdQosEventDescrSetSinkReceiveTimestamp1(p, t)                                \
         AnscWriteUlong(&p->SinkReceiveTimestamp1, AnscUlongFromHToN(t))
#define  AnscLltdQosEventDescrSetSinkReceiveTimestamp2(p, t)                                \
         AnscWriteUlong(&p->SinkReceiveTimestamp2, AnscUlongFromHToN(t))

#define  AnscLltdQosEventDescrSetPacketID(p, id)    ( p->PacketID = id )
#define  AnscLltdQosEventDescrSetReserved(p, r)     ( p->Reserved = r  )

/*
 * The QosError header that follows the base header is:
 *
 *       0                   1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |          Error Code           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Error Code: 16 bits

 *      This field specifies the error code that identifies the reason that a request failed,
 *      resulting in this response. Valid error code values are:
 *          0x00 = Insufficient resources
 *          0x01 = Busy; try again later
 *          0x02 = Interrupt moderation not available
 */
#define  LLTD_QOS_ERROR_InsufficientResources       0x00
#define  LLTD_QOS_ERROR_Busy                        0x01
#define  LLTD_QOS_ERROR_InterruptModeUnavailable    0x02

typedef  struct
_LLTD_QOS_ERROR_HEADER
{
    USHORT                          ErrorCode;
}_struct_pack_
LLTD_QOS_ERROR_HEADER,  *PLLTD_QOS_ERROR_HEADER;

#define  AnscLltdQosErrorGetErrorCode(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->ErrorCode)) )
#define  AnscLltdQosErrorSetErrorCode(p, c)         AnscWriteUshort(&p->ErrorCode, AnscUshortFromHToN(c))


/***********************************************************
   DATA STRUCTURES USED BY LLTD QOS CROSS-TRAFFIC ANALYSIS
***********************************************************/

/*
 * The QosCounterSnapshot header immediately follows the base header:
 *
 *      +-+-+-+-+-+-+-+-+
 *      | History Size  |
 *      +-+-+-+-+-+-+-+-+
 *
 * History Size: 8 bits
 *
 *      This field indicates the maximum number of most recent full 4-tuples to return from the
 *      history.
 */
typedef  struct
_LLTD_QOS_CN_SNAPSHOT_HEADER
{
    UCHAR                           HistorySize;
}_struct_pack_
LLTD_QOS_CN_SNAPSHOT_HEADER,  *PLLTD_QOS_CN_SNAPSHOT_HEADER;

#define  AnscLltdQosCnSnapshotGetHistorySize(p)     ( p->HistorySize     )
#define  AnscLltdQosCnSnapshotSetHistorySize(p, s)  ( p->HistorySize = s )

/*
 * Each QosCounterResult frame reports as many full 4-tuples as are requested in the preceding
 * QosCounterSnapshot request. When the QosCounterSnapshot request is received, a snapshot of the
 * 4-tuples is also taken and the time span since the last sampling interval is recorded. This
 * subsecond sample is also returned in the QosCounterResult frame.
 *
 * The QosCounterResult header immediately follows the base header:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |Sub-second Span|  Byte Scale   | Packet Scale  | History Size  |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      : Snapshot List (variable) ...                                  :
 *
 * Each snapshot has the following format:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |        Bytes Received         |       Packets Received        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |          Bytes Sent           |         Packets Sent          |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_LLTD_QOS_CN_RESULT_HEADER
{
    UCHAR                           SubSecondSpan;
    UCHAR                           ByteScale;
    UCHAR                           PacketScale;
    UCHAR                           HistorySize;
    UCHAR                           SnapshotList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_QOS_CN_RESULT_HEADER,  *PLLTD_QOS_CN_RESULT_HEADER;

#define  AnscLltdQosCnResultGetSubSecondSpan(p)     ( p->SubSecondSpan )
#define  AnscLltdQosCnResultGetByteScale(p)         ( p->ByteScale     )
#define  AnscLltdQosCnResultGetPacketScale(p)       ( p->PacketScale   )
#define  AnscLltdQosCnResultGetHistorySize(p)       ( p->HistorySize   )
#define  AnscLltdQosCnResultGetSnapshotList(p)      ( p->SnapshotList  )
#define  AnscLltdQosCnResultGetSnapshot(p, n)       (&p->SnapshotList[sizeof(LLTD_QOS_SNAPSHOT) * n] )

#define  AnscLltdQosCnResultSetSubSecondSpan(p, s)  ( p->SubSecondSpan = s )
#define  AnscLltdQosCnResultSetByteScale(p, s)      ( p->ByteScale     = s )
#define  AnscLltdQosCnResultSetPacketScale(p, s)    ( p->PacketScale   = s )
#define  AnscLltdQosCnResultSetHistorySize(p, s)    ( p->HistorySize   = s )

typedef  struct
_LLTD_QOS_SNAPSHOT
{
    USHORT                          BytesReceived;
    USHORT                          PacketsReceived;
    USHORT                          BytesSent;
    USHORT                          PacketsSent;
}_struct_pack_
LLTD_QOS_SNAPSHOT,  *PLLTD_QOS_SNAPSHOT;

#define  AnscLltdQosSnapshotGetBytesReceived(p)     ( AnscUshortFromNToH(AnscReadUshort(&p->BytesReceived  )) )
#define  AnscLltdQosSnapshotGetPacketsReceived(p)   ( AnscUshortFromNToH(AnscReadUshort(&p->PacketsReceived)) )
#define  AnscLltdQosSnapshotGetBytesSent(p)         ( AnscUshortFromNToH(AnscReadUshort(&p->BytesSent      )) )
#define  AnscLltdQosSnapshotGetPacketsSent(p)       ( AnscUshortFromNToH(AnscReadUshort(&p->PacketsSent    )) )

#define  AnscLltdQosSnapshotSetBytesReceived(p, c)                                          \
         AnscWriteUshort(&p->BytesReceived, AnscUshortFromHToN(c))
#define  AnscLltdQosSnapshotSetPacketsReceived(p, c)                                        \
         AnscWriteUshort(&p->PacketsReceived, AnscUshortFromHToN(c))
#define  AnscLltdQosSnapshotSetBytesSent(p, c)                                              \
         AnscWriteUshort(&p->BytesSent, AnscUshortFromHToN(c))
#define  AnscLltdQosSnapshotSetPacketsSent(p, c)                                            \
         AnscWriteUshort(&p->PacketsSent, AnscUshortFromHToN(c))


/***********************************************************
        DATA STRUCTURES USED BY LLTD TLV DEFINITIONS
***********************************************************/

/*
 * The TLV list gives properties that the responder knows about the interface on which it is
 * running. Sometimes, a TLV may be too large to fit into a Hello frame, particularly in the
 * presence of other TLV properties that occupy their share of valuable space. The responder may
 * choose to declare certain TLVs as zero length. This tells the mapper to issue one or more
 * QueryLargeTlv requests later for each of these TLVs. Each valid QueryLargeTlv request is
 * followed by exactly one QueryLargeTlvResp response, so if the TLV is sufficiently large,
 * multiple QueryLargeTlv requests may have to be issued. Note that only specific TLVs are allowed
 * this behavior.
 */
#define  LLTD_TLV_TYPE_EndOfProperty                0x00
#define  LLTD_TLV_TYPE_HostID                       0x01
#define  LLTD_TLV_TYPE_Characteristics              0x02
#define  LLTD_TLV_TYPE_PhysicalMedium               0x03
#define  LLTD_TLV_TYPE_WirelessMode                 0x04
#define  LLTD_TLV_TYPE_802_11_BSSID                 0x05
#define  LLTD_TLV_TYPE_802_11_SSID                  0x06
#define  LLTD_TLV_TYPE_IPv4Address                  0x07
#define  LLTD_TLV_TYPE_IPv6Address                  0x08
#define  LLTD_TLV_TYPE_802_11_MaxOperationalRate    0x09
#define  LLTD_TLV_TYPE_PerformanceCounterFrequency  0x0A
#define  LLTD_TLV_TYPE_LinkSpeed                    0x0C
#define  LLTD_TLV_TYPE_802_11_RSSI                  0x0D
#define  LLTD_TLV_TYPE_IconImage                    0x0E
#define  LLTD_TLV_TYPE_MachineName                  0x0F
#define  LLTD_TLV_TYPE_SupportInformation           0x10
#define  LLTD_TLV_TYPE_FriendlyName                 0x11
#define  LLTD_TLV_TYPE_DeviceUUID                   0x12
#define  LLTD_TLV_TYPE_HardwareID                   0x13
#define  LLTD_TLV_TYPE_QoSCharacteristics           0x14
#define  LLTD_TLV_TYPE_802_11_PhysicalMedium        0x15
#define  LLTD_TLV_TYPE_APAssociationTable           0x16
#define  LLTD_TLV_TYPE_DetailedIconImage            0x18
#define  LLTD_TLV_TYPE_SeesListWorkingSet           0x19
#define  LLTD_TLV_TYPE_ComponentTable               0x1A
#define  LLTD_TLV_TYPE_RepeaterAPLineage            0x1B
#define  LLTD_TLV_TYPE_RepeaterAPTable              0x1C

typedef  struct
_LLTD_TLV
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_TLV,  *PLLTD_TLV;

#define  AnscLltdTlvGetType(p)                      ( p->Type   )
#define  AnscLltdTlvGetLength(p)                    ( p->Length )
#define  AnscLltdTlvGetSize(p)                      ( p->Length + sizeof(LLTD_TLV) )
#define  AnscLltdTlvGetValue(p)                     ( p->Value  )

#define  AnscLltdTlvSetType(p, t)                   ( p->Type   = t )
#define  AnscLltdTlvSetLength(p, l)                 ( p->Length = l )
#define  AnscLltdTlvSetValue(p, v)                  AnscCopyMemory(p->Value, AnscLltdTlvGetLength(p), v)

#define  AnscLltdTlvGetNextTlv(p)                   ( (PLLTD_TLV)((ULONG)p + AnscLltdTlvGetSize(p)) )

#define  AnscLltdTlvGetValue_Uchar(p)               (                                  ( p->Value[0    ] ) )
#define  AnscLltdTlvGetValue_Ushort(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Value[0    ])) )
#define  AnscLltdTlvGetValue_Ulong(p)               ( AnscUlongFromNToH (AnscReadUlong (&p->Value[0    ])) )
#define  AnscLltdTlvGetValue_Ulong2(p, n)           ( AnscUlongFromNToH (AnscReadUlong (&p->Value[4 * n])) )
#define  AnscLltdTlvGetValue_Ip4AddrValue(p)        (                   (AnscReadUlong (&p->Value[0    ])) )

#define  AnscLltdTlvSetValue_Uchar(p, v)            ( p->Value[0] = v )
#define  AnscLltdTlvSetValue_Ushort(p, v)           AnscWriteUshort(&p->Value[0    ], AnscUshortFromHToN(v))
#define  AnscLltdTlvSetValue_Ulong(p, v)            AnscWriteUlong (&p->Value[0    ], AnscUlongFromHToN (v))
#define  AnscLltdTlvSetValue_Ulong2(p, v, n)        AnscWriteUlong (&p->Value[4 * n], AnscUlongFromHToN (v))
#define  AnscLltdTlvSetValue_Ip4AddrValue(p, v)     AnscWriteUlong (&p->Value[0    ],                    v )

/*
 * Characteristics
 *
 *      +--------+--------+--------+--------+--------+--------+
 *      |00000010|00000100|          Characteristics          |
 *      +--------+--------+--------+--------+--------+--------+
 *      Type=0x02 Length=4
 *
 * This property allows a responder to report various simple characteristics of its host or the
 * network interface that it is using.
 *
 *      MSB
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |N|N|F|M|L| | | | | | | | | | | |
 *      |P|X|D|W|P|0|0|0|0|0|0|0|0|0|0|0|
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      | | | | | | | | | | | | | | | | |
 *      |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                    LSB
 *
 * Bits 0-28: Reserved, must be zero.
 * Bit 27: (LP) 1   =   Interface is looping back outbound packets.
 * Bit 28: (MW) 1   =   Device has management Web page accessible via HTTP protocol. The mapper
 *                      constructs a URL from the reported IPv6 address. If one is not available,
 *                      the IPv4 address is used instead. The URL is of the form:
 *                          http://<ip-address>/
 * Bit 29: (FD) 1   =   Interface is in full duplex mode.
 * Bit 30: (NX) 1   =   Interface is NAT-private side.
 * Bit 31: (NP) 1   =   Interface is NAT-public side.
 */
#define  LLTD_TLV_CHARACTERISTICS_NP                0x80000000      /* Interface is NAT-public side                       */
#define  LLTD_TLV_CHARACTERISTICS_NX                0x40000000      /* Interface is NAT-private side                      */
#define  LLTD_TLV_CHARACTERISTICS_FD                0x20000000      /* Interface is full duplex side                      */
#define  LLTD_TLV_CHARACTERISTICS_MW                0x10000000      /* Device has management web page accessible via HTTP */
#define  LLTD_TLV_CHARACTERISTICS_LP                0x08000000      /* Interface is looping outbound packets              */

/*
 * Wireless Mode
 *
 *      +--------+--------+--------+
 *      |00000100|00000001|  Mode  |
 *      +--------+--------+--------+
 *      Type=0x04 Length=1
 *
 * This property allows a responder to identify how its IEEE 802.11 interface connects to the
 * network. Valid values are:
 *
 *      0x00 = IBSS or ad hoc mode
 *      0x01 = Infrastructure mode
 *      0x02 = Automatic mode
 */
#define  LLTD_TLV_WIRELESS_MODE_AdHoc               0x00
#define  LLTD_TLV_WIRELESS_MODE_Infrastructure      0x01
#define  LLTD_TLV_WIRELESS_MODE_Automatic           0x02

/*
 * QoS Characteristics
 *
 *      +--------+--------+--------+--------+--------+--------+
 *      |00010100|00000100|        QoS Characteristics        |
 *      +--------+--------+--------+--------+--------+--------+
 *      Type=0x14 Length=4
 *
 * This property allows a responder to report various QoS-related characteristics of its host or
 * the network interface that it is using.
 *
 *      MSB
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |Q|8|8| | | | | | | | | | | | | |
 *      |W|Q|P|0|0|0|0|0|0|0|0|0|0|0|0|0|
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      | | | | | | | | | | | | | | | | |
 *      |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                    LSB
 *
 * Bits 0-28: Reserved, must be zero.
 * Bit 29: (8P) 1   =   Interface supports 802.1p priority tagging.
 * Bit 30: (8Q) 1   =   Interface supports 802.1q VLAN tagging.
 * Bit 31: (QW) 1   =   Interface is qWave-enabled.
 */
#define  LLTD_TLV_QOS_CHARACTERISTICS_QW            0x80000000      /* Interface is qWave-enabled                 */
#define  LLTD_TLV_QOS_CHARACTERISTICS_8Q            0x40000000      /* Interface supports 802.1q VLAN tagging     */
#define  LLTD_TLV_QOS_CHARACTERISTICS_8P            0x40000000      /* Interface supports 802.1p priority tagging */

/*
 * 802.11 Physical Medium
 *
 *      +--------+--------+--------+
 *      |00010101|00000001|PHY Type|
 *      +--------+--------+--------+
 *      Type=0x15 Length=1
 *
 * This property allows a responder to report the 802.11 physical medium in use. Valid values are:
 *
 *      0x00            Unknown
 *      0x01            FHSS 2.4 GHz
 *      0x02            DSSS 2.4 GHz
 *      0x03            IR Baseband
 *      0x04            OFDM 5 GHz
 *      0x05            HRDSSS
 *      0x06            ERP
 *      0x07-0xFF       Reserved for future use
 */
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_Unknown    0x00
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_FHSS_24    0x01
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_DSSS_24    0x02
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_IRBaseband 0x03
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_OFDM5      0x04
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_HRDSSS     0x05
#define  LLTD_TLV_802_11_PHYSICAL_MEDIUM_ERP        0x06

/*
 * AP Association Table
 *
 *      +--------+--------+
 *      |00010110|00000000|
 *      +--------+--------+
 *      Type=0x16
 *
 * This property allows an access point to report the wireless hosts that are associated with it.
 * This information is particularly useful for discovering legacy wireless devices that do not
 * implement the responder. Additionally, it allows the mapper to conclusively match wireless hosts
 * that are associated to the same access point via different BSSIDs (for example, one for each
 * supported band). This is a large TLV.
 *
 * Each table entry is 10 octets long and has the format:
 *
 *      +--------+--------+--------+--------+--------+--------+
 *      |            MAC address of wireless host             |
 *      +--------+--------+--------+--------+--------+--------+
 *      | Max Oper. Rate  |PHY type|Reserved|
 *      +--------+--------+--------+--------+
 */
typedef  struct
_LLTD_TLV_AP_ASSOC_ENTRY
{
    UCHAR                           WirelessHostMacAddress[ETHERNET_ADDRESS_SIZE];
    USHORT                          MaxOperationalRate;
    UCHAR                           PhysicalMediumType;
    UCHAR                           Reserved;
}_struct_pack_
LLTD_TLV_AP_ASSOC_ENTRY,  *PLLTD_TLV_AP_ASSOC_ENTRY;

#define  AnscLltdTlvApAssocEntryGetWirelessHostMacAddress(p)                                \
         ( p->WirelessHostMacAddress )
#define  AnscLltdTlvApAssocEntryGetMaxOperationalRate(p)                                    \
         ( AnscUshortFromNToH(AnscReadUshort(&p->Type)) )
#define  AnscLltdTlvApAssocEntryGetPhysicalMediumType(p)                                    \
         ( p->PhysicalMediumType )
#define  AnscLltdTlvApAssocEntryGetReserved(p)                                              \
         ( p->Reserved )

#define  AnscLltdTlvApAssocEntrySetWirelessHostMacAddress(p, addr)                          \
         ( AnscLltdAssignAddress(p->WirelessHostMacAddress, addr) )
#define  AnscLltdTlvApAssocEntrySetMaxOperationalRate(p, r)                                 \
         AnscWriteUshort(&p->MaxOperationalRate, AnscUshortFromHToN(r))
#define  AnscLltdTlvApAssocEntrySetPhysicalMediumType(p, t)                                 \
         ( p->PhysicalMediumType = t )
#define  AnscLltdTlvApAssocEntrySetReserved(p, r)                                           \
         ( p->Reserved = r )

/*
 * Component Table
 *
 *      +--------+--------+
 *      |00011000|00000000|
 *      +--------+--------+
 *      Type=0x1A
 *
 * This property identifies the components in a multifunction device. This is a large TLV. The
 * table begins with a 2-octet-long header:
 *
 *      +--------+--------+
 *      |Version |Reserved|
 *      +--------+--------+
 *
 * Version:
 *      Must be set to 1.
 * Reserved:
 *      Must be set to zero.
 *
 * This header is followed by an arbitrary number of component descriptors, each carrying a
 * mandatory header:
 *
 *      +--------+--------+
 *      |  Type  | Length |
 *      +--------+--------+
 *
 * Type:
 *      The component type. Valid values are:
 *          0x00    A bridge that interconnects all identified WLAN and/or LAN segments. It is
 *                  assumed that the responder reporting the component table TLV is connected
 *                  directly into this bridge.
 *          0x01    Wireless radio band.
 *          0x02    Built-in switch. If a bridge component (type 0x00) exists, it is assumed that
 *                  this switch connects directly into the bridge. If a bridge does not exist, the
 *                  switch is assumed to connect directly to the built-in responder.
 *
 * Components not defined through the type enumeration above do not have to be reported.
 *
 * Length:
 *      The length of the descriptor payload immediately following this header in octets.
 *
 * A bridge component descriptor with type value 0x00 has the format:
 *
 *      +--------+--------+--------+
 *      |00000000|00000001|Behavior|
 *      +--------+--------+--------+
 *      Type=0  Length=1
 *
 * Behavior:
 *      Identifies the behavior of the bridge. Valid values are:
 *          0x00    Hub: All packets transiting through the bridge are seen on the responder.
 *          0x01    Switch: Packets from LAN or WLAN are seen only on the responder if they are
 *                  broadcast or explicitly targeted at the responder.
 *          0x02    Internal hub-switch: Packets transitioning through the bridge are seen on the
 *                  responder; however, the bridge learns addresses like a switch, provided they
 *                  initiate on components other than the responder. Please check with Microsoft
 *                  before using this value.
 *
 * A wireless radio band component descriptor with type value 0x01 has the format:
 *
 *      +--------+--------+--------+--------+--------+--------+
 *      |00000001|00001010| Max Oper. Rate  |PHY type|  Mode  |
 *      +--------+--------+--------+--------+--------+--------+
 *      |                        BSSID                        |
 *      +--------+--------+--------+--------+--------+--------+
 */
#define  LLTD_TLV_COMPONENT_TYPE_Bridge             0x00
#define  LLTD_TLV_COMPONENT_TYPE_WirelessRadioBand  0x01
#define  LLTD_TLV_COMPONENT_TYPE_BuiltInSwitch      0x02

#define  LLTD_TLV_BRIDGE_BEHAVIOR_Hub               0x00
#define  LLTD_TLV_BRIDGE_BEHAVIOR_Switch            0x01
#define  LLTD_TLV_BRIDGE_BEHAVIOR_InternalHubSwitch 0x02

typedef  struct
_LLTD_TLV_COMPONTENT_TABLE
{
    UCHAR                           Version;
    UCHAR                           Reserved;
    UCHAR                           Content[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
LLTD_TLV_COMPONTENT_TABLE,  *PLLTD_TLV_COMPONTENT_TABLE;

#define  AnscLltdTlvComponentTableGetVersion(p)     ( p->Version  )
#define  AnscLltdTlvComponentTableGetReserved(p)    ( p->Reserved )
#define  AnscLltdTlvComponentTableGetContent(p)     ( p->Content  )

#define  AnscLltdTlvComponentTableSetVersion(p, v)  ( p->Version  = v )
#define  AnscLltdTlvComponentTableSetReserved(p, r) ( p->Reserved = r )

typedef  struct
_LLTD_TLV_WIRELESS_RADIO_BAND
{
    USHORT                          MaxOperationalRate;
    UCHAR                           PhysicalMediumType;
    UCHAR                           Mode;
    UCHAR                           BSSID[ETHERNET_ADDRESS_SIZE];
}_struct_pack_
LLTD_TLV_WIRELESS_RADIO_BAND,  *PLLTD_TLV_WIRELESS_RADIO_BAND;

#define  AnscLltdTlvWirelessRadioBandGetMaxOperationalRate(p)                               \
         ( AnscUshortFromNToH(AnscReadUshort(&p->Type)) )
#define  AnscLltdTlvWirelessRadioBandGetPhysicalMediumType(p)                               \
         ( p->PhysicalMediumType )
#define  AnscLltdTlvWirelessRadioBandGetMode(p)                                             \
         ( p->Mode )
#define  AnscLltdTlvWirelessRadioBandGetBSSID(p)                                            \
         ( p->BSSID )

#define  AnscLltdTlvWirelessRadioBandSetMaxOperationalRate(p, r)                            \
         AnscWriteUshort(&p->MaxOperationalRate, AnscUshortFromHToN(r))
#define  AnscLltdTlvWirelessRadioBandSetPhysicalMediumType(p, t)                            \
         ( p->PhysicalMediumType = t )
#define  AnscLltdTlvWirelessRadioBandSetMode(p, m)                                          \
         ( p->Mode = m )
#define  AnscLltdTlvWirelessRadioBandSetBSSID(p, addr)                                      \
         ( AnscLltdAssignAddress(p->BSSID, addr) )

/*
 * Repeater AP Table
 *
 *      +--------+--------+
 *      |00011100|00000000|
 *      +--------+--------+
 *      Type=0x1C
 *
 * This property allows an access point operating in Wireless Distribution System mode to report
 * the routing table it is using for packets to addresses that are not directly associated. Table
 * entries are not required for addresses routed to the parent, since this is assumed for all
 * unknown addresses. This is a large TLV.
 *
 * Each table entry is 12 octets long and has the format:
 *
 *      +--------+--------+--------+--------+--------+--------+
 *      |            MAC address of destination host          |
 *      +--------+--------+--------+--------+--------+--------+
 *      |            MAC address of next hop access point     |
 *      +--------+--------+--------+--------+--------+--------+
 *
 * MAC address of destination host:
 *      No further description needed
 * MAC address of next hop access point:
 *      This should be one of the addresses listed in the AP Association Table.
 */
typedef  struct
_LLTD_TLV_REPEATER_AP_ENTRY
{
    UCHAR                           DstHostMacAddress[ETHERNET_ADDRESS_SIZE];
    UCHAR                           NextHopMacAddress[ETHERNET_ADDRESS_SIZE];
}_struct_pack_
LLTD_TLV_REPEATER_AP_ENTRY,  *PLLTD_TLV_REPEATER_AP_ENTRY;

#define  AnscLltdTlvRepeaterApEntryGetDstHostMacAddress(p)                                  \
         ( p->DstHostMacAddress )
#define  AnscLltdTlvRepeaterApEntryGetNextHopMacAddress(p)                                  \
         ( p->NextHopMacAddress )

#define  AnscLltdTlvRepeaterApEntrySetDstHostMacAddress(p, addr)                            \
         ( AnscLltdAssignAddress(p->DstHostMacAddress, addr) )
#define  AnscLltdTlvRepeaterApEntrySetNextHopMacAddress(p, addr)                            \
         ( AnscLltdAssignAddress(p->NextHopMacAddress, addr) )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
