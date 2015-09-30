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

    module:	ansc_packet_binary_imcp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Inter-Module Communication
        Protocol (IMCP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_IMCP_
#define  _ANSC_PACKET_BINARY_IMCP_


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
                 DATA STRUCTURES USED BY IMCP
***********************************************************/

/*
 * The Inter-Module Communication Protocol (IMCP) is a proprietary protocol used in BISGA to faci-
 * litate the interaction between BISGA modules. This protocol provides a framework for different
 * IPC modules to communicate with each other either directly or indirectly via a central manager.
 */
#define  IMCP_MESSAGE_TYPE_HELLO                    1
#define  IMCP_MESSAGE_TYPE_ACK                      2
#define  IMCP_MESSAGE_TYPE_REQUEST                  3
#define  IMCP_MESSAGE_TYPE_REPLY                    4
#define  IMCP_MESSAGE_TYPE_BYE                      5

typedef  struct
_IMCP_HEADER
{
    ULONG                           MsgType;        /* one of the message types defined above       */
    ULONG                           MsgLength;      /* total_message_size - 'MsgType' - 'MsgLength' */
    UCHAR                           MsgData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_HEADER,  *PIMCP_HEADER;

#define  ImcpGetMsgType(p)                          ( AnscUlongFromNToH(AnscReadUlong(&p->MsgType  )) )
#define  ImcpGetMsgLength(p)                        ( AnscUlongFromNToH(AnscReadUlong(&p->MsgLength)) )
#define  ImcpGetMsgSize(p)                          ( sizeof(IMCP_HEADER) + ImcpGetMsgLength(p)   )
#define  ImcpGetMsgData(p)                          ( p->MsgData                                      )

#define  ImcpSetMsgType(p, t)                       AnscWriteUlong(&p->MsgType,   AnscUlongFromHToN(t))
#define  ImcpSetMsgLength(p, l)                     AnscWriteUlong(&p->MsgLength, AnscUlongFromHToN(l))
#define  ImcpSetMsgData(p, d)                       AnscCopyMemory(p->MsgData, d, ImcpGetMsgLength(p))

/*
 * The HELLO message is sent by a LPC Connector Client when it first starts up. This message serves
 * as the registration of the LPC party represented by the LPC Connector. Upon receiving this
 * message, the LPC Connector Server SHOULD establish a LPC connection back to the Client right
 * away which will be used to send REQUESTs initiated by Server back to the Client.
 */
typedef  struct
_IMCP_HELLO_MESSAGE
{
    ANSC_IPV4_ADDRESS               PartyAddr;
    USHORT                          PartyPort;
    char                            PartyName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_HELLO_MESSAGE,  *PIMCP_HELLO_MESSAGE;

#define  ImcpHelloGetPartyAddrDot(p)                ( p->PartyAddr.Dot                                  )
#define  ImcpHelloGetPartyAddrValue(p)              ( AnscReadUlong(&p->PartyAddr.Value)                )
#define  ImcpHelloGetPartyPort(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->PartyPort)) )
#define  ImcpHelloGetPartyName(p)                   ( p->PartyName                                      )

#define  ImcpHelloSetPartyAddrDot(p, addr)          AnscWriteUlong (&p->PartyAddr.Dot[0], *(PULONG)addr    )
#define  ImcpHelloSetPartyAddrValue(p, addr)        AnscWriteUlong (&p->PartyAddr.Value,  addr             )
#define  ImcpHelloSetPartyPort(p, port)             AnscWriteUshort(&p->PartyPort, AnscUshortFromHToN(port))
#define  ImcpHelloSetPartyName(p, name)             AnscCopyMemory(p->PartyName, name, AnscSizeOfString(name))

/*
 * The ACK message is sent by a LPC Connector Server upon receiving the HELLO message sent by the
 * Client. This message serves as the confirmation for the successful processing of the corres-
 * ponding HELLO message.
 */
typedef  struct
_IMCP_ACK_MESSAGE
{
    ULONG                           Reserved;
    char                            PartyName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_ACK_MESSAGE,  *PIMCP_ACK_MESSAGE;

#define  ImcpAckGetReserved(p)                      ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  ImcpAckGetPartyName(p)                     ( p->PartyName )

#define  ImcpAckSetReserved(p, r)                   AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  ImcpAckSetPartyName(p, name)               AnscCopyMemory(p->PartyName, name, AnscSizeOfString(name))

/*
 * The REQUEST message can be sent by either a LPC Connector Client or a Connector Server. This
 * message encapsulates a LPC/RPC call parameters. Upon receiving this message. the reciving party
 * should dispatch the call to the target module and encapulate the call result in a REPLY
 * message and send it back.
 */
#define  IMCP_REQUEST_TYPE_QUERY_PARTY              1
#define  IMCP_REQUEST_TYPE_LPC_CALL                 2

#define  IMCP_REQUEST_BUFFER_OVERHEAD               (sizeof(IMCP_HEADER) + sizeof(IMCP_REQUEST_MESSAGE) + 32)

typedef  struct
_IMCP_REQUEST_MESSAGE
{
    ULONG                           SeqNumber;
    ULONG                           ReqType;
    ULONG                           ReqSize;
    UCHAR                           ReqData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_REQUEST_MESSAGE,  *PIMCP_REQUEST_MESSAGE;

#define  ImcpRequestGetSeqNumber(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->SeqNumber)) )
#define  ImcpRequestGetReqType(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->ReqType  )) )
#define  ImcpRequestGetReqSize(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->ReqSize  )) )
#define  ImcpRequestGetReqData(p)                   ( p->ReqData                                      )

#define  ImcpRequestSetSeqNumber(p, s)              AnscWriteUlong(&p->SeqNumber, AnscUlongFromHToN(s))
#define  ImcpRequestSetReqType(p, t)                AnscWriteUlong(&p->ReqType,   AnscUlongFromHToN(t))
#define  ImcpRequestSetReqSize(p, s)                AnscWriteUlong(&p->ReqSize,   AnscUlongFromHToN(s))
#define  ImcpRequestSetReqData(p, d)                AnscCopyMemory(p->ReqData, d, ImcpRequestGetReqSize(p))

#define  IMCP_REPLY_BUFFER_OVERHEAD                 (sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE) + 32)

typedef  struct
_IMCP_REPLY_MESSAGE
{
    ULONG                           SeqNumber;
    ULONG                           ReqType;
    ULONG                           RepCode;
    ULONG                           RepSize;
    UCHAR                           RepData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_REPLY_MESSAGE,  *PIMCP_REPLY_MESSAGE;

#define  ImcpReplyGetSeqNumber(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->SeqNumber)) )
#define  ImcpReplyGetReqType(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->ReqType  )) )
#define  ImcpReplyGetRepCode(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->RepCode  )) )
#define  ImcpReplyGetRepSize(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->RepSize  )) )
#define  ImcpReplyGetRepData(p)                     ( p->RepData                                      )

#define  ImcpReplySetSeqNumber(p, s)                AnscWriteUlong(&p->SeqNumber, AnscUlongFromHToN(s))
#define  ImcpReplySetReqType(p, t)                  AnscWriteUlong(&p->ReqType,   AnscUlongFromHToN(t))
#define  ImcpReplySetRepCode(p, c)                  AnscWriteUlong(&p->RepCode,   AnscUlongFromHToN(c))
#define  ImcpReplySetRepSize(p, s)                  AnscWriteUlong(&p->RepSize,   AnscUlongFromHToN(s))
#define  ImcpReplySetRepData(p, d)                  AnscCopyMemory(p->RepData, d, ImcpReplyGetRepSize(p))

/*
 * The BYE message should be sent by a LPC Connector Client whenever it disconnects from the LPC
 * backbone. The LPC Connector Server may send this message as well, but it means that no central
 * control will be available. Hence LPC mechanism is broken.
 */
typedef  struct
_IMCP_BYE_MESSAGE
{
    ULONG                           Reserved;
    char                            PartyName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_BYE_MESSAGE,  *PIMCP_BYE_MESSAGE;

#define  ImcpByeGetReserved(p)                      ( AnscUlongFromNToH(AnscReadUlong(&p->Reserved)) )
#define  ImcpByeGetPartyName(p)                     ( p->PartyName )

#define  ImcpByeSetReserved(p, r)                   AnscWriteUlong(&p->Reserved, AnscUlongFromHToN(r))
#define  ImcpByeSetPartyName(p, name)               AnscCopyMemory(p->PartyName, name, AnscSizeOfString(name))


/***********************************************************
 DATA STRUCTURES USED BY VARIOUS IMCP REQUEST/REPLY MESSAGES
***********************************************************/

/*
 * IMCP client may send QUERY_PARTY request message to the IMCP server to resolve the name of a LPC
 * party. The 'ReqData' field of the REQUEST message should encapsulate the 'party_name' of the
 * target LPC party. If the IMCP server can resolve the 'party_name' successfully, it should send
 * back the address information of the target LPC party.
 */
typedef  struct
_IMCP_PARTY_INFO
{
    ANSC_IPV4_ADDRESS               PartyAddr;
    UCHAR                           PartyProtocol;
    USHORT                          PartyPort;
    char                            PartyName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_PARTY_INFO,  *PIMCP_PARTY_INFO;

#define  ImcpPartyInfoGetPartyAddrDot(p)            ( p->PartyAddr.Dot                                  )
#define  ImcpPartyInfoGetPartyAddrValue(p)          ( AnscReadUlong(&p->PartyAddr.Value)                )
#define  ImcpPartyInfoGetPartyProtocol(p)           ( p->PartyProtocol                                  )
#define  ImcpPartyInfoGetPartyPort(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->PartyPort)) )
#define  ImcpPartyInfoGetPartyName(p)               ( p->PartyName                                      )

#define  ImcpPartyInfoSetPartyAddrDot(p, a)         AnscWriteUlong (&p->PartyAddr.Dot[0], *(PULONG)a       )
#define  ImcpPartyInfoSetPartyAddrValue(p, a)       AnscWriteUlong (&p->PartyAddr.Value,  a                )
#define  ImcpPartyInfoSetPartyProtocol(p, prot)     ( p->PartyProtocol = prot                              )
#define  ImcpPartyInfoSetPartyPort(p, port)         AnscWriteUshort(&p->PartyPort, AnscUshortFromHToN(port))
#define  ImcpPartyInfoSetPartyName(p, name)         AnscCopyMemory(p->PartyName, name, AnscSizeOfString(name))


/***********************************************************
   DATA STRUCTURES USED BY ALL SLAP REQUEST/REPLY MESSAGES
***********************************************************/

/*
 * The parameter serialization in SLAP is primarily about encapsulating and decapsulating the
 * SLAP_VARIABLE and the SLAP_PARAMETER_LIST data structures. The formal way to do that is to use
 * the CTLV or TLV based data processing, but it introduces more overhead. Here, we define this
 * data structure to simplify the parsing and generation of the parameter list.
 */
#define  IMCP_SLAP_VAR_FLAG_objType                 0x00000001
#define  IMCP_SLAP_VAR_FLAG_objName                 0x00000002
#define  IMCP_SLAP_VAR_FLAG_objData                 0x00000004

typedef  struct
_IMCP_SLAP_VARIABLE
{
    ULONG                           ContentType;
    ULONG                           Syntax;
    ULONG                           Length;
    ULONG                           Flags;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_VARIABLE,  *PIMCP_SLAP_VARIABLE;

#define  ImcpSlapVarGetContentType(p)               ( AnscUlongFromNToH(AnscReadUlong(&p->ContentType))    )
#define  ImcpSlapVarGetSyntax(p)                    ( AnscUlongFromNToH(AnscReadUlong(&p->Syntax     ))    )
#define  ImcpSlapVarGetLength(p)                    ( AnscUlongFromNToH(AnscReadUlong(&p->Length     ))    )
#define  ImcpSlapVarGetSize(p)                      ( ImcpSlapVarGetLength(p) + sizeof(IMCP_SLAP_VARIABLE) )
#define  ImcpSlapVarGetFlags(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Flags      ))    )
#define  ImcpSlapVarGetData(p)                      ( p->Data                                              )
#define  ImcpSlapVarGetEndOfData(p)                 ( (PUCHAR)((ULONG)p->Data + ImcpSlapVarGetLength(p))   )
#define  ImcpSlapVarGetNextVar(p)                   (PIMCP_SLAP_VARIABLE)((ULONG)p + ImcpSlapVarGetSize(p) )

#define  ImcpSlapVarSetContentType(p, t)            AnscWriteUlong(&p->ContentType, AnscUlongFromHToN(t))
#define  ImcpSlapVarSetSyntax(p, s)                 AnscWriteUlong(&p->Syntax,      AnscUlongFromHToN(s))
#define  ImcpSlapVarSetLength(p, l)                 AnscWriteUlong(&p->Length,      AnscUlongFromHToN(l))
#define  ImcpSlapVarSetFlags(p, f)                  AnscWriteUlong(&p->Flags,       AnscUlongFromHToN(f))
#define  ImcpSlapVarSetData(p, d)                   AnscCopyMemory(p->Data, d, ImcpSlapVarGetLength(p))
#define  ImcpSlapVarAppendData(p, data, size)                                               \
         {                                                                                  \
            ULONG                   var_length = ImcpSlapVarGetLength(p);                   \
                                                                                            \
            AnscCopyMemory                                                                  \
                (                                                                           \
                    ImcpSlapVarGetEndOfData(p),                                             \
                    data,                                                                   \
                    size                                                                    \
                );                                                                          \
                                                                                            \
            var_length += size;                                                             \
                                                                                            \
            ImcpSlapVarSetLength(p, var_length);                                            \
         }
#define  ImcpSlapVarAppendUchar(p, tbp_uchar)                                               \
         {                                                                                  \
            UCHAR                   tmp_uchar = (UCHAR)tbp_uchar;                           \
                                                                                            \
            ImcpSlapVarAppendData(p, &tmp_uchar, sizeof(UCHAR));                            \
         }
#define  ImcpSlapVarAppendUshort(p, tbp_ushort)                                             \
         {                                                                                  \
            USHORT                  tmp_ushort = AnscUshortFromHToN((USHORT)tbp_ushort);    \
                                                                                            \
            ImcpSlapVarAppendData(p, &tmp_ushort, sizeof(USHORT));                          \
         }
#define  ImcpSlapVarAppendUlong(p, tbp_ulong)                                               \
         {                                                                                  \
            ULONG                   tmp_ulong = AnscUlongFromHToN((ULONG)tbp_ulong);        \
                                                                                            \
            ImcpSlapVarAppendData(p, &tmp_ulong, sizeof(ULONG));                            \
         }
#define  ImcpSlapVarAppendString(p, tbp_string)                                             \
         {                                                                                  \
            ImcpSlapVarAppendData(p, tbp_string, AnscSizeOfString(tbp_string) + 1);         \
         }

typedef  struct
_IMCP_SLAP_VAR_OBJECT
{
    ULONG                           Handle;
    ULONG                           Type;
    char                            Name[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_VAR_OBJECT,  *PIMCP_SLAP_VAR_OBJECT;

#define  ImcpSlapVarObjGetHandle(p)                 ( AnscUlongFromNToH(AnscReadUlong(&p->Handle)) )
#define  ImcpSlapVarObjGetType(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Type  )) )
#define  ImcpSlapVarObjGetName(p)                   ( p->Name                                      )

#define  ImcpSlapVarObjSetHandle(p, h)              AnscWriteUlong(&p->Handle, AnscUlongFromHToN(h))
#define  ImcpSlapVarObjSetType(p, t)                AnscWriteUlong(&p->Type,   AnscUlongFromHToN(t))
#define  ImcpSlapVarObjSetName(p, n)                AnscCopyMemory(p->Name, n, AnscSizeOfString(n) + 1)

/*
 * Apparently the 'invoke_obj' call is the primary call request satisfies most LPC communcation
 * needs. However we need to define some other calls to setup the LPC object access channel before
 * the SLAP parties can make remote object invocation.
 */
#define  IMCP_SLAP_CALL_TYPE_ACQC                   1
#define  IMCP_SLAP_CALL_TYPE_RELC                   2
#define  IMCP_SLAP_CALL_TYPE_LOCO                   3
#define  IMCP_SLAP_CALL_TYPE_DELO                   4
#define  IMCP_SLAP_CALL_TYPE_INVO                   5
#define  IMCP_SLAP_CALL_TYPE_ACQO                   6
#define  IMCP_SLAP_CALL_TYPE_RELO                   7

typedef  struct
_IMCP_SLAP_CALL
{
    ULONG                           Type;
    ULONG                           Size;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_CALL,  *PIMCP_SLAP_CALL;

#define  ImcpSlapCallGetType(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Type)) )
#define  ImcpSlapCallGetSize(p)                     ( AnscUlongFromNToH(AnscReadUlong(&p->Size)) )
#define  ImcpSlapCallGetData(p)                     ( p->Data                                    )

#define  ImcpSlapCallSetType(p, t)                  AnscWriteUlong(&p->Type, AnscUlongFromHToN(t))
#define  ImcpSlapCallSetSize(p, s)                  AnscWriteUlong(&p->Size, AnscUlongFromHToN(s))
#define  ImcpSlapCallSetData(p, d)                  AnscCopyMemory(p->Data, d, ImcpSlapCallGetSize(p))

typedef  struct
_IMCP_SLAP_ANSWER
{
    ULONG                           Size;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_ANSWER,  *PIMCP_SLAP_ANSWER;

#define  ImcpSlapAnswerGetSize(p)                   ( AnscUlongFromNToH(AnscReadUlong(&p->Size)) )
#define  ImcpSlapAnswerGetData(p)                   ( p->Data                                    )

#define  ImcpSlapAnswerSetSize(p, s)                AnscWriteUlong(&p->Size, AnscUlongFromHToN(s))
#define  ImcpSlapAnswerSetData(p, d)                AnscCopyMemory(p->Data, d, ImcpSlapAnswerGetSize(p))


/***********************************************************
   DATA STRUCTURES USED BY SLAP ACQC CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_ACQC_CALL request message to a LPC party to acquire the
 * access to the specified SLAP container. The 'CallData' field of the REQUEST message encapsulates
 * the 'ContainerName' of the target SLAP container. If the target LPC party is willing to grant
 * the access, it should send back the requested container handle, which is encoded as a four-bytes
 * unsigned integer.
 */
typedef  struct
_IMCP_SLAP_ACQC_CALL
{
    ULONG                           ContainerType;
    char                            ContainerName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_ACQC_CALL,  *PIMCP_SLAP_ACQC_CALL;

#define  ImcpSlapAcqcCallGetContainerType(p)                                                \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ContainerType)) )
#define  ImcpSlapAcqcCallGetContainerName(p)                                                \
         ( p->ContainerName )

#define  ImcpSlapAcqcCallSetContainerType(p, type)                                          \
         AnscWriteUlong(&p->ContainerType, AnscUlongFromHToN(type))
#define  ImcpSlapAcqcCallSetContainerName(p, name)                                          \
         AnscCopyMemory(p->ContainerName, name, AnscSizeOfString(name))

typedef  struct
_IMCP_SLAP_ACQC_ANSWER
{
    ULONG                           ContainerId;
}_struct_pack_
IMCP_SLAP_ACQC_ANSWER,  *PIMCP_SLAP_ACQC_ANSWER;

#define  ImcpSlapAcqcAnswerGetContainerId(p)                                                \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ContainerId)) )
#define  ImcpSlapAcqcAnswerSetContainerId(p, id)                                            \
         AnscWriteUlong(&p->ContainerId, AnscUlongFromHToN(id))


/***********************************************************
   DATA STRUCTURES USED BY SLAP RELC CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_RELC_CALL request message to a LPC party to release the
 * access to the specified SLAP container, whose 'ContainerId' was returned in a previous ACQC_CALL
 * request. The 'CallData' field of the REQUEST message encapsulates the 'ContainerId' of the
 * target SLAP container. If the target LPC party deems it as a valid request, it should send back
 * a 'StatusCode' which indicates whether the call succeeded.
 */
typedef  struct
_IMCP_SLAP_RELC_CALL
{
    ULONG                           ContainerId;
}_struct_pack_
IMCP_SLAP_RELC_CALL,  *PIMCP_SLAP_RELC_CALL;

#define  ImcpSlapRelcCallGetContainerId(p)                                                  \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ContainerId)) )
#define  ImcpSlapRelcCallSetContainerId(p, id)                                              \
         AnscWriteUlong(&p->ContainerId, AnscUlongFromHToN(id))

typedef  struct
_IMCP_SLAP_RELC_ANSWER
{
    ULONG                           StatusCode;
}_struct_pack_
IMCP_SLAP_RELC_ANSWER,  *PIMCP_SLAP_RELC_ANSWER;

#define  ImcpSlapRelcAnswerGetStatusCode(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->StatusCode)) )
#define  ImcpSlapRelcAnswerSetStatusCode(p, code)                                           \
         AnscWriteUlong(&p->StatusCode, AnscUlongFromHToN(code))


/***********************************************************
   DATA STRUCTURES USED BY SLAP LOCO CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_LOCO_CALL request message to a LPC party to locate a SLAP
 * object by matching the object name. The 'CallData' field of the REQUEST message encapsulates the
 * 'ObjectName' of the target SLAP object. If the specified object has been registered at the
 * target LPC party, a 'StatusCode Success' along with the party name should be returned; otherwise,
 * the 'StatusCode Failure' should be returned.
 */
typedef  struct
_IMCP_SLAP_LOCO_CALL
{
    ULONG                           ObjectType;
    char                            ObjectName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_LOCO_CALL,  *PIMCP_SLAP_LOCO_CALL;

#define  ImcpSlapLocoCallGetObjectType(p)                                                   \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ObjectType)) )
#define  ImcpSlapLocoCallGetObjectName(p)                                                   \
         ( p->ObjectName )

#define  ImcpSlapLocoCallSetObjectType(p, type)                                             \
         AnscWriteUlong(&p->ObjectType, AnscUlongFromHToN(type))
#define  ImcpSlapLocoCallSetObjectName(p, name)                                             \
         AnscCopyMemory(p->ObjectName, name, AnscSizeOfString(name))

typedef  struct
_IMCP_SLAP_LOCO_ANSWER
{
    ULONG                           StatusCode;
    UCHAR                           PartyInfo[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_LOCO_ANSWER,  *PIMCP_SLAP_LOCO_ANSWER;

#define  ImcpSlapLocoAnswerGetStatusCode(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->StatusCode)) )
#define  ImcpSlapLocoAnswerGetPartyInfo(p)                                                  \
         ( p->PartyInfo )

#define  ImcpSlapLocoAnswerSetStatusCode(p, code)                                           \
         AnscWriteUlong(&p->StatusCode, AnscUlongFromHToN(code))
#define  ImcpSlapLocoAnswerSetPartyInfo(p, info, size)                                      \
         AnscCopyMemory(p->PartyInfo, info, size)


/***********************************************************
   DATA STRUCTURES USED BY SLAP DELO CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_DELO_CALL request message to a LPC party to delete a SLAP
 * object, whose 'ObjectId' was returned in a previous ACQO_CALL request. The 'CallData' field of
 * the REQUEST message encapsulates the 'ObjectId' of the target SLAP object. If the target LPC
 * party deems it as a valid request, it should send back a 'StatusCode' which indicates whether
 * the call succeeded.
 */
typedef  struct
_IMCP_SLAP_DELO_CALL
{
    ULONG                           ObjectId;
}_struct_pack_
IMCP_SLAP_DELO_CALL,  *PIMCP_SLAP_DELO_CALL;

#define  ImcpSlapDeloCallGetObjectId(p)                                                     \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ObjectId)) )
#define  ImcpSlapDeloCallSetObjectId(p, id)                                                 \
         AnscWriteUlong(&p->ObjectId, AnscUlongFromHToN(id))

typedef  struct
_IMCP_SLAP_DELO_ANSWER
{
    ULONG                           StatusCode;
}_struct_pack_
IMCP_SLAP_DELO_ANSWER,  *PIMCP_SLAP_DELO_ANSWER;

#define  ImcpSlapDeloAnswerGetStatusCode(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->StatusCode)) )
#define  ImcpSlapDeloAnswerSetStatusCode(p, code)                                           \
         AnscWriteUlong(&p->StatusCode, AnscUlongFromHToN(code))


/***********************************************************
   DATA STRUCTURES USED BY SLAP INVO CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_INVO_CALL request message to a LPC party to invoke a method
 * of the specified SLAP object. The 'CallData' field of the REQUEST message encapsulates the
 * 'ObjectId' of the target container and the input parameter list passed in by the caller. If the
 * target LPC party can successfully process the  request, it should send back the output parameter
 * list along with the returned variable.
 */
typedef  struct
_IMCP_SLAP_INVO_CALL
{
    ULONG                           ObjectId;
    ULONG                           MethodNameSize;
    ULONG                           VarArraySize;
    char                            MethodName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_INVO_CALL,  *PIMCP_SLAP_INVO_CALL;

#define  ImcpSlapInvoCallValidate(invo_call, c_size, v_result)                              \
         {                                                                                  \
            ULONG                   var_array_size = 0;                                     \
            PIMCP_SLAP_VARIABLE     imcp_slap_var  = NULL;                                  \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( c_size < sizeof(IMCP_SLAP_INVO_CALL) )                                 \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( c_size != ImcpSlapInvoCallGetTotalSize(invo_call) )               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( ImcpSlapInvoCallGetMethodNameSize(invo_call) == 0 )               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    var_array_size = ImcpSlapInvoCallGetVarArraySize(invo_call);            \
                }                                                                           \
                                                                                            \
                while ( var_array_size > 0 )                                                \
                {                                                                           \
                    if ( imcp_slap_var )                                                    \
                    {                                                                       \
                        imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);               \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        imcp_slap_var = ImcpSlapInvoCallGetFirstVar(invo_call);             \
                    }                                                                       \
                                                                                            \
                    if ( var_array_size < sizeof(IMCP_SLAP_VARIABLE) )                      \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else if ( var_array_size < ImcpSlapVarGetSize(imcp_slap_var) )          \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        var_array_size -= ImcpSlapVarGetSize(imcp_slap_var);                \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  ImcpSlapInvoCallGetObjectId(p)                                                     \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ObjectId)) )
#define  ImcpSlapInvoCallGetMethodNameSize(p)                                               \
         ( AnscUlongFromNToH(AnscReadUlong(&p->MethodNameSize)) )
#define  ImcpSlapInvoCallGetVarArraySize(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->VarArraySize)) )
#define  ImcpSlapInvoCallGetTotalSize(p)                                                    \
         ( sizeof(IMCP_SLAP_INVO_CALL) + ImcpSlapInvoCallGetMethodNameSize(p) + ImcpSlapInvoCallGetVarArraySize(p) )
#define  ImcpSlapInvoCallGetMethodName(p)                                                   \
         ( p->MethodName )
#define  ImcpSlapInvoCallGetFirstVar(p)                                                     \
         ( (PIMCP_SLAP_VARIABLE)((ULONG)p->MethodName + ImcpSlapInvoCallGetMethodNameSize(p)) )
#define  ImcpSlapInvoCallGetEndOfVarArray(p)                                                \
         ( (PIMCP_SLAP_VARIABLE)((ULONG)ImcpSlapInvoCallGetFirstVar(p) + ImcpSlapInvoCallGetVarArraySize(p)) )
#define  ImcpSlapInvoCallGetVarCount(p, var_count)                                          \
         {                                                                                  \
            ULONG                   var_array_size = ImcpSlapInvoCallGetVarArraySize(p);    \
            PIMCP_SLAP_VARIABLE     imcp_slap_var  = NULL;                                  \
                                                                                            \
            var_count = 0;                                                                  \
                                                                                            \
            while ( var_array_size > 0 )                                                    \
            {                                                                               \
                if ( imcp_slap_var )                                                        \
                {                                                                           \
                    imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);                   \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    imcp_slap_var = ImcpSlapInvoCallGetFirstVar(p);                         \
                }                                                                           \
                                                                                            \
                if ( var_array_size < sizeof(IMCP_SLAP_VARIABLE) )                          \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else if ( var_array_size < ImcpSlapVarGetSize(imcp_slap_var) )              \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    var_count++;                                                            \
                    var_array_size -= ImcpSlapVarGetSize(imcp_slap_var);                    \
                }                                                                           \
            }                                                                               \
         }
#define  ImcpSlapInvoCallGetVarByIndex(p, imcp_slap_var, var_index)                         \
         {                                                                                  \
            ULONG                   tmp_index = 0;                                          \
                                                                                            \
            imcp_slap_var = ImcpSlapInvoCallGetFirstVar(p);                                 \
                                                                                            \
            while ( tmp_index < var_index )                                                 \
            {                                                                               \
                imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);                       \
                tmp_index++;                                                                \
            }                                                                               \
         }

#define  ImcpSlapInvoCallSetObjectId(p, id)                                                 \
         AnscWriteUlong(&p->ObjectId, AnscUlongFromHToN(id))
#define  ImcpSlapInvoCallSetMethodNameSize(p, s)                                            \
         AnscWriteUlong(&p->MethodNameSize, AnscUlongFromHToN(s))
#define  ImcpSlapInvoCallSetVarArraySize(p, s)                                              \
         AnscWriteUlong(&p->VarArraySize, AnscUlongFromHToN(s))
#define  ImcpSlapInvoCallSetMethodName(p, name)                                             \
         AnscCopyMemory(p->MethodName, name, AnscSizeOfString(name))
#define  ImcpSlapInvoCallAppendVar(p, var)                                                  \
         {                                                                                  \
            ULONG                   var_array_size = ImcpSlapInvoCallGetVarArraySize(p);    \
                                                                                            \
            AnscCopyMemory                                                                  \
                (                                                                           \
                    ImcpSlapInvoCallGetEndOfVarArray(p),                                    \
                    var,                                                                    \
                    AnscSlapVarGetSize(var)                                                 \
                );                                                                          \
                                                                                            \
            var_array_size += AnscSlapVarGetSize(var);                                      \
                                                                                            \
            ImcpSlapInvoCallSetVarArraySize(p, var_array_size);                             \
         }

#define  IMCP_SLAP_INVO_FLAG_returnedValue          0x00000001
#define  IMCP_SLAP_INVO_FLAG_outputParamList        0x00000002
#define  IMCP_SLAP_INVO_FLAG_returnedObjData        0x00000004

typedef  struct
_IMCP_SLAP_INVO_ANSWER
{
    ULONG                           StatusCode;
    ULONG                           ReturnFlag;
    ULONG                           VarArraySize;
    UCHAR                           VarArray[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_INVO_ANSWER,  *PIMCP_SLAP_INVO_ANSWER;

#define  ImcpSlapInvoAnswerValidate(invo_answer, a_size, v_result)                          \
         {                                                                                  \
            ULONG                   var_array_size = 0;                                     \
            PIMCP_SLAP_VARIABLE     imcp_slap_var  = NULL;                                  \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( a_size < sizeof(IMCP_SLAP_INVO_ANSWER) )                               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( a_size != ImcpSlapInvoAnswerGetTotalSize(invo_answer) )           \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    var_array_size = ImcpSlapInvoAnswerGetVarArraySize(invo_answer);        \
                }                                                                           \
                                                                                            \
                while ( var_array_size > 0 )                                                \
                {                                                                           \
                    if ( imcp_slap_var )                                                    \
                    {                                                                       \
                        imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);               \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        imcp_slap_var = ImcpSlapInvoAnswerGetFirstVar(invo_answer);         \
                    }                                                                       \
                                                                                            \
                    if ( var_array_size < sizeof(IMCP_SLAP_VARIABLE) )                      \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else if ( var_array_size < ImcpSlapVarGetSize(imcp_slap_var) )          \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        var_array_size -= ImcpSlapVarGetSize(imcp_slap_var);                \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  ImcpSlapInvoAnswerGetStatusCode(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->StatusCode)) )
#define  ImcpSlapInvoAnswerGetReturnFlag(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ReturnFlag)) )
#define  ImcpSlapInvoAnswerGetVarArraySize(p)                                               \
         ( AnscUlongFromNToH(AnscReadUlong(&p->VarArraySize)) )
#define  ImcpSlapInvoAnswerGetTotalSize(p)                                                  \
         ( sizeof(IMCP_SLAP_INVO_ANSWER) + ImcpSlapInvoAnswerGetVarArraySize(p) )
#define  ImcpSlapInvoAnswerGetVarArray(p)                                                   \
         ( p->VarArray )
#define  ImcpSlapInvoAnswerGetFirstVar(p)                                                   \
         ( (PIMCP_SLAP_VARIABLE)p->VarArray )
#define  ImcpSlapInvoAnswerGetEndOfVarArray(p)                                              \
         ( (PIMCP_SLAP_VARIABLE)((ULONG)ImcpSlapInvoAnswerGetFirstVar(p) + ImcpSlapInvoAnswerGetVarArraySize(p)) )
#define  ImcpSlapInvoAnswerGetVarCount(p, var_count)                                        \
         {                                                                                  \
            ULONG                   var_array_size = ImcpSlapInvoAnswerGetVarArraySize(p);  \
            PIMCP_SLAP_VARIABLE     imcp_slap_var  = NULL;                                  \
                                                                                            \
            var_count = 0;                                                                  \
                                                                                            \
            while ( var_array_size > 0 )                                                    \
            {                                                                               \
                if ( imcp_slap_var )                                                        \
                {                                                                           \
                    imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);                   \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    imcp_slap_var = ImcpSlapInvoAnswerGetFirstVar(p);                       \
                }                                                                           \
                                                                                            \
                if ( var_array_size < sizeof(IMCP_SLAP_VARIABLE) )                          \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else if ( var_array_size < ImcpSlapVarGetSize(imcp_slap_var) )              \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    var_count++;                                                            \
                    var_array_size -= ImcpSlapVarGetSize(imcp_slap_var);                    \
                }                                                                           \
            }                                                                               \
         }
#define  ImcpSlapInvoAnswerGetVarByIndex(p, imcp_slap_var, var_index)                       \
         {                                                                                  \
            ULONG                   tmp_index = 0;                                          \
                                                                                            \
            imcp_slap_var = ImcpSlapInvoAnswerGetFirstVar(p);                               \
                                                                                            \
            while ( tmp_index < var_index )                                                 \
            {                                                                               \
                imcp_slap_var = ImcpSlapVarGetNextVar(imcp_slap_var);                       \
                tmp_index++;                                                                \
            }                                                                               \
         }

#define  ImcpSlapInvoAnswerSetStatusCode(p, code)                                           \
         AnscWriteUlong(&p->StatusCode, AnscUlongFromHToN(code))
#define  ImcpSlapInvoAnswerSetReturnFlag(p, flag)                                           \
         AnscWriteUlong(&p->ReturnFlag, AnscUlongFromHToN(flag))
#define  ImcpSlapInvoAnswerSetVarArraySize(p, s)                                            \
         AnscWriteUlong(&p->VarArraySize, AnscUlongFromHToN(s))
#define  ImcpSlapInvoAnswerSetVarArray(p, array)                                            \
         AnscCopyMemory(p->VarArray, array, ImcpSlapInvoAnswerGetVarArraySize(p))
#define  ImcpSlapInvoAnswerAppendVar(p, var)                                                \
         {                                                                                  \
            ULONG                   var_array_size = ImcpSlapInvoAnswerGetVarArraySize(p);  \
                                                                                            \
            AnscCopyMemory                                                                  \
                (                                                                           \
                    ImcpSlapInvoAnswerGetEndOfVarArray(p),                                  \
                    var,                                                                    \
                    AnscSlapVarGetSize(var)                                                 \
                );                                                                          \
                                                                                            \
            var_array_size += AnscSlapVarGetSize(var);                                      \
                                                                                            \
            ImcpSlapInvoAnswerSetVarArraySize(p, var_array_size);                           \
         }


/***********************************************************
   DATA STRUCTURES USED BY SLAP ACQO CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_ACQO_CALL request message to a LPC party to acquire the
 * access to the specified SLAP object. The 'CallData' field of the REQUEST message encapsulates
 * the 'ContainerId' of the target container and the 'ObjectName' of the target SLAP object. If the
 * target LPC party is willing to grant the access, it should send back the requested object handle,
 * which is encoded as a four-bytes unsigned integer.
 */
typedef  struct
_IMCP_SLAP_ACQO_CALL
{
    ULONG                           ContainerId;
    char                            ObjectName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
IMCP_SLAP_ACQO_CALL,  *PIMCP_SLAP_ACQO_CALL;

#define  ImcpSlapAcqoCallGetContainerId(p)                                                  \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ContainerId)) )
#define  ImcpSlapAcqoCallGetObjectName(p)                                                   \
         ( p->ObjectName )

#define  ImcpSlapAcqoCallSetContainerId(p, id)                                              \
         AnscWriteUlong(&p->ContainerId, AnscUlongFromHToN(id))
#define  ImcpSlapAcqoCallSetObjectName(p, name)                                             \
         AnscCopyMemory(p->ObjectName, name, AnscSizeOfString(name))

typedef  struct
_IMCP_SLAP_ACQO_ANSWER
{
    ULONG                           ObjectId;
}_struct_pack_
IMCP_SLAP_ACQO_ANSWER,  *PIMCP_SLAP_ACQO_ANSWER;

#define  ImcpSlapAcqoAnswerGetObjectId(p)                                                   \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ObjectId)) )
#define  ImcpSlapAcqoAnswerSetObjectId(p, id)                                               \
         AnscWriteUlong(&p->ObjectId, AnscUlongFromHToN(id))


/***********************************************************
   DATA STRUCTURES USED BY SLAP RELO CALL/ANSWER MESSAGES
***********************************************************/

/*
 * The SLAP Loam Client may send SLAP_RELO_CALL request message to a LPC party to release the
 * access to the specified SLAP object, whose 'ObjectId' was returned in a previous ACQO_CALL
 * request. The 'CallData' field of the REQUEST message encapsulates the 'ObjectId' of the target
 * SLAP object. If the target LPC party deems it as a valid request, it should send back a
 * 'StatusCode' which indicates whether the call succeeded.
 */
typedef  struct
_IMCP_SLAP_RELO_CALL
{
    ULONG                           ObjectId;
}_struct_pack_
IMCP_SLAP_RELO_CALL,  *PIMCP_SLAP_RELO_CALL;

#define  ImcpSlapReloCallGetObjectId(p)                                                     \
         ( AnscUlongFromNToH(AnscReadUlong(&p->ObjectId)) )
#define  ImcpSlapReloCallSetObjectId(p, id)                                                 \
         AnscWriteUlong(&p->ObjectId, AnscUlongFromHToN(id))

typedef  struct
_IMCP_SLAP_RELO_ANSWER
{
    ULONG                           StatusCode;
}_struct_pack_
IMCP_SLAP_RELO_ANSWER,  *PIMCP_SLAP_RELO_ANSWER;

#define  ImcpSlapReloAnswerGetStatusCode(p)                                                 \
         ( AnscUlongFromNToH(AnscReadUlong(&p->StatusCode)) )
#define  ImcpSlapReloAnswerSetStatusCode(p, code)                                           \
         AnscWriteUlong(&p->StatusCode, AnscUlongFromHToN(code))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
