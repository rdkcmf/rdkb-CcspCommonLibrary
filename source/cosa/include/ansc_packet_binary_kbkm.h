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

    module:	ansc_packet_binary_kbkm.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of Kerberized Key Management protocols defined
        by PacketCable.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        10/02/02    Na Li           initial revision

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_KBKM_
#define  _ANSC_PACKET_BINARY_KBKM_

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
                 DATA STRUCTURES USED BY KBKM MESSAGE
***********************************************************/

/*
 * KBKM defines a set of messages sent and received as UDP data at port 1293. 
 * The high order nibble of the version number is 1, and lower order nibble is 0.
 */
#define  KBKM_TRANSPORT_PORT_NUM                    1293
#define  KBKM_PROTOCOL_VER                          0x10

/* 
 * define the message ID
 */
#define  KBKM_MSGID_WAKEUP                          0x01 
#define  KBKM_MSGID_APREQ                           0x02                         
#define  KBKM_MSGID_APREP                           0x03                         
#define  KBKM_MSGID_SECPARA_RECOV                   0x04                         
#define  KBKM_MSGID_REKEY                           0x05                         
#define  KBKM_MSGID_ERROR                           0x06                         

/* 
 * define the domain of interpretation. The application specific data
 * is different according to this value
 */
#define  KBKM_DOI_IPSEC                             0x01 
#define  KBKM_DOI_SNMPV3                            0x02                         
                   
#define  KBKM_MSG_HMAC_SIZE                         20
#define  KBKM_MSG_REKEY_AUTHKEY_SIZE                20

/* 
 * define ciphersuite entry
 */
typedef  struct
_KBKM_CIPHERSUITE_ENTRY
{
    UCHAR                           AuthenAlgorithm;
    UCHAR                           EncrytTransformId;
}_struct_pack_
KBKM_CIPHERSUITE_ENTRY,  *PKBKM_CIPHERSUITE_ENTRY;

/******************************************************************************************
                          KBKM_BASIC_MSG

    define Basic Message Structure for Kerberized Key Management. 

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte

 ******************************************************************************************/

#define  KBKM_BASIC_MSG_CONTENT                                                             \
    UCHAR                           KmMessageId;                                            \
    UCHAR                           DomainOfInterp;                                         \
    UCHAR                           ProtocolVersion;                                        \

typedef  struct
_KBKM_BASIC_MSG
{
    KBKM_BASIC_MSG_CONTENT
}_struct_pack_
KBKM_BASIC_MSG,  *PKBKM_BASIC_MSG;

#define  KBKM_BASIC_MSG_SIZE                   3


#define  AnscKbkmBasicMsgGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmBasicMsgSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmBasicMsgGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmBasicMsgSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmBasicMsgGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmBasicMsgSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )

/******************************************************************************************
                          KBKM_MSG_WAKEUP

    define Wakeup Message Structure for Kerberized Key Management. 

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    ServerNonce                     4 bytes
    ServerKbPrinName                variable

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_WAKEUP
{
    KBKM_BASIC_MSG_CONTENT

    ULONG                           ServerNonce;
    UCHAR                           ServerKbPrinName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_MSG_WAKEUP,  *PKBKM_MSG_WAKEUP;

#define  KBKM_MSG_WAKEUP_SIZE_MIN                   7

/* 
 * define macro to access header fields. When setting ServerKbPrinName, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmWakeupCalMsgLength(strSize)              ( KBKM_MSG_WAKEUP_SIZE_MIN + strSize )

#define  AnscKbkmWakeupGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmWakeupSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmWakeupGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmWakeupSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmWakeupGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmWakeupSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmWakeupGetServerNonce(p)                  ( AnscUlongFromNToH(AnscReadUlong(&(p)->ServerNonce)) )
#define  AnscKbkmWakeupSetServerNonce(p, v)               AnscWriteUlong(&(p)->ServerNonce, AnscUlongFromHToN(v))
#define  AnscKbkmWakeupGetServerKbPrinNameHandle(p)       ( (p)->ServerKbPrinName )
#define  AnscKbkmWakeupGetServerKbPrinNameLength(p)       ( AnscSizeOfString((p)->ServerKbPrinName) )
#define  AnscKbkmWakeupSetServerKbPrinName(p, str)        AnscCopyString((p)->ServerKbPrinName, str)

/******************************************************************************************
                          KBKM_ASD_SNMPV3

    define Application specific data structure for SNMPV3. Since some fields are of variable 
    length, the fields have to be filled in order

    EngineIdlength                  1 byte
    EngineId                        variable
    EngineBoots                     4 bytes
    EngineTime                      4 bytes
    UserNameLength                  1 byte
    UserName                        variable

 ******************************************************************************************/
typedef  struct
_KBKM_ASD_SNMPV3
{
    UCHAR                           EngineIdLength;
    UCHAR                           EngineId[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_ASD_SNMPV3,  *PKBKM_ASD_SNMPV3;

#define  KBKM_ASD_SNMPV3_MIN_SIZE                           10


#define  AnscKbkmASDSnmpv3GetEngineIdLength(p)              ( (p)->EngineIdLength )
#define  AnscKbkmASDSnmpv3GetEngineId(p)                    ( (p)->EngineId )
#define  AnscKbkmASDSnmpv3SetEngineId(p, handle, size)      \
        {                                                   \
            AnscCopyMemory((p)->EngineId, handle, size);    \
            (p)->EngineIdLength = size;                     \
        }
#define  AnscKbkmASDSnmpv3GetEngineBootsHandle(p)           ( (PULONG)((ULONG)((p)->EngineId) + AnscKbkmASDSnmpv3GetEngineIdLength(p)) )
#define  AnscKbkmASDSnmpv3GetEngineBoots(p)                 AnscUlongFromNToH(AnscReadUlong(AnscKbkmASDSnmpv3GetEngineBootsHandle(p)) )
#define  AnscKbkmASDSnmpv3SetEngineBoots(p, v)              AnscWriteUlong(AnscKbkmASDSnmpv3GetEngineBootsHandle(p), AnscUlongFromHToN(v))
#define  AnscKbkmASDSnmpv3GetEngineTimeHandle(p)            ( (PULONG)((ULONG)AnscKbkmASDSnmpv3GetEngineBootsHandle(p) + 4) )
#define  AnscKbkmASDSnmpv3GetEngineTime(p)                  AnscUlongFromNToH(AnscReadUlong(AnscKbkmASDSnmpv3GetEngineTimeHandle(p)) )
#define  AnscKbkmASDSnmpv3SetEngineTime(p, v)               AnscWriteUlong(AnscKbkmASDSnmpv3GetEngineTimeHandle(p), AnscUlongFromHToN(v))
#define  AnscKbkmASDSnmpv3GetUserNameLengthHandle(p)        ( (PCHAR)((ULONG)AnscKbkmASDSnmpv3GetEngineTimeHandle(p) + 4) )
#define  AnscKbkmASDSnmpv3GetUserNameLength(p)              ( *(AnscKbkmASDSnmpv3GetUserNameLengthHandle(p)) )
#define  AnscKbkmASDSnmpv3SetUserNameLength(p, v)           ( *AnscKbkmASDSnmpv3GetUserNameLengthHandle(p) = v )
#define  AnscKbkmASDSnmpv3GetUserName(p)                    ( (PCHAR)((ULONG)AnscKbkmASDSnmpv3GetUserNameLengthHandle(p) + 1) )
#define  AnscKbkmASDSnmpv3SetUserName(p, handle, size)                          \
        {                                                                       \
            AnscCopyMemory(AnscKbkmASDSnmpv3GetUserName(p), handle, size);      \
            AnscKbkmASDSnmpv3SetUserNameLength(p, size);                        \
        }
#define  AnscKbkmASDSnmpv3GetTotalSize(p)                   ( (ULONG)AnscKbkmASDSnmpv3GetUserName(p) + \
                                                              AnscKbkmASDSnmpv3GetUserNameLength(p)  - (ULONG)(p) )

/******************************************************************************************
                          KBKM_ASD_IPSEC

    define Application specific data structure for IPSEC. 

    SecurityParaIndex               4 bytes

 ******************************************************************************************/
typedef  struct
_KBKM_ASD_IPSEC
{
    ULONG                           SecurityParaIndex;
}_struct_pack_
KBKM_ASD_IPSEC,  *PKBKM_ASD_IPSEC;


#define  AnscKbkmASDIpsecGetSecurityParaIndex(p)           AnscUlongFromNToH(AnscReadUlong(&(p)->SecurityParaIndex))
#define  AnscKbkmASDIpsecSetSecurityParaIndex(p, v)        AnscWriteUlong(&(p)->SecurityParaIndex, AnscUlongFromHToN(v))

#define  AnscKbkmASDIpsecGetTotalSize(p)                   ( 4 )


/******************************************************************************************
                          KBKM_MSG_APREQ

    define AP_REQ for Kerberized Key Management. Since some fields are of variable 
    length, the fields have to be filled in order

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    KrbApReq                        KrbApReq encoding, variable
    ServerNonce                     4 bytes
    ASD                             size depends on the DomainOfInterp
    CiphersuitNum                   1 byte
    Ciphersuit[i].AuthenAlgm        1 byte
    Ciphersuit[i].EncTrans          1 byte
    ReestablishFlag                 1 byte
    SHA1HMAC                        20 bytes

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_APREQ
{
    KBKM_BASIC_MSG_CONTENT

    UCHAR                          KrbApReq[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_MSG_APREQ,  *PKBKM_MSG_APREQ;

#define  KBKM_MSG_APREQ_SIZE_MIN                   29

/* 
 * define macro to access header fields. When setting KrbApReq, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmApreqCalMsgLength(kb5Size, asdSize, ciphNum)                       \
            ( KBKM_MSG_APREQ_SIZE_MIN + kb5Size + asdSize + ciphNum * 2 )

#define  AnscKbkmApreqGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmApreqSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmApreqGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmApreqSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmApreqGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmApreqSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmApreqGetKrbApReqHandle(p)               ( (p)->KrbApReq )
#define  AnscKbkmApreqGetKrbApReqSize(p)                 ( AnscGetTLVLength((p)->KrbApReq) )
#define  AnscKbkmApreqSetKrbApReq(p, str, size)          AnscCopyMemory((p)->KrbApReq, str, size)

#define  AnscKbkmApreqGetServerNonceHandle(p)            (PULONG)((p)->KrbApReq + AnscKbkmApreqGetKrbApReqSize(p))
#define  AnscKbkmApreqGetServerNonce(p)                  AnscUlongFromNToH(AnscReadUlong(AnscKbkmApreqGetServerNonceHandle(p)) )
#define  AnscKbkmApreqSetServerNonce(p, v)               AnscWriteUlong(AnscKbkmApreqGetServerNonceHandle(p), AnscUlongFromHToN(v))

#define  AnscKbkmApreqGetASDHandle(p)                    (PVOID)((ULONG)AnscKbkmApreqGetServerNonceHandle(p) + 4)
#define  AnscKbkmApreqGetASDTotalSize(p)                                            \
         (ULONG)(   AnscKbkmApreqGetDomainOfInterp(p) ==  KBKM_DOI_IPSEC ?          \
                    AnscKbkmASDIpsecGetTotalSize ((PKBKM_ASD_IPSEC )AnscKbkmApreqGetASDHandle(p)) :   \
                    AnscKbkmASDSnmpv3GetTotalSize((PKBKM_ASD_SNMPV3)AnscKbkmApreqGetASDHandle(p)) )


#define  AnscKbkmApreqGetCiphersuitNumHandle(p)          (PUCHAR)((ULONG)AnscKbkmApreqGetASDHandle(p) + AnscKbkmApreqGetASDTotalSize(p))
#define  AnscKbkmApreqGetCiphersuitNum(p)                (*(PUCHAR)AnscKbkmApreqGetCiphersuitNumHandle(p))
#define  AnscKbkmApreqSetCiphersuitNum(p, v)             (*(PUCHAR)AnscKbkmApreqGetCiphersuitNumHandle(p) = (UCHAR)v)

#define  AnscKbkmApreqGetCiphersuitHandle(p, i)          (PUCHAR)((ULONG)AnscKbkmApreqGetCiphersuitNumHandle(p) + 1 + i*2)
#define  AnscKbkmApreqGetCiphersuitAuthAlgm(p, i)        (*(PUCHAR)AnscKbkmApreqGetCiphersuitHandle(p, i)           )
#define  AnscKbkmApreqSetCiphersuitAuthAlgm(p, i, v)     (*(PUCHAR)AnscKbkmApreqGetCiphersuitHandle(p, i) = (UCHAR)v)
#define  AnscKbkmApreqGetCiphersuitEncTrans(p, i)        (*(PUCHAR)((ULONG)AnscKbkmApreqGetCiphersuitHandle(p, i) + 1)           )
#define  AnscKbkmApreqSetCiphersuitEncTrans(p, i, v)     (*(PUCHAR)((ULONG)AnscKbkmApreqGetCiphersuitHandle(p, i) + 1) = (UCHAR)v)

#define  AnscKbkmApreqGetReestablishFlagHandle(p)        (PUCHAR)(AnscKbkmApreqGetCiphersuitHandle(p, AnscKbkmApreqGetCiphersuitNum(p)))
#define  AnscKbkmApreqGetReestablishFlag(p)              (*AnscKbkmApreqGetReestablishFlagHandle(p)            )
#define  AnscKbkmApreqSetReestablishFlag(p, v)           (*AnscKbkmApreqGetReestablishFlagHandle(p) = (UCHAR)v )

#define  AnscKbkmApreqGetSHA1HMAC(p)                     (PUCHAR)((ULONG)AnscKbkmApreqGetReestablishFlagHandle(p) + 1)
#define  AnscKbkmApreqGetSHA1HMACSize(p)                 ANSC_SHA1_OUTPUT_SIZE
#define  AnscKbkmApreqSetSHA1HMAC(p, str)                AnscCopyMemory(AnscKbkmApreqGetSHA1HMAC(p), str, ANSC_SHA1_OUTPUT_SIZE)

#define  AnscKbkmApreqGetTotalSize(p)                    ( (ULONG)AnscKbkmApreqGetSHA1HMAC(p) + \
                                                           AnscKbkmApreqGetSHA1HMACSize(p)  - (ULONG)(p) )

/******************************************************************************************
                          KBKM_MSG_APREP

    define AP_REP for Kerberized Key Management. Since some fields are of variable 
    length, the fields have to be filled in order

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    KrbApRep                        KrbApRep encoding, variable
    ASD                             size depends on the DomainOfInterp
    CiphersuitNum                   1 byte, its value has to be 1
    Ciphersuit[i].AuthenAlgm        1 byte
    Ciphersuit[i].EncTrans          1 byte
    SecParaLifetime                 4 bytes
    GracePeriod                     4 bytes
    ReestablishFlag                 1 byte
    AckRequiredFlag                 1 byte
    SHA1HMAC                        20 bytes

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_APREP
{
    KBKM_BASIC_MSG_CONTENT

    UCHAR                          KrbApRep[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_MSG_APREP,  *PKBKM_MSG_APREP;

#define  KBKM_MSG_APREP_SIZE_MIN                   36

/* 
 * define macro to access header fields. When setting KrbApRep, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmAprepCalMsgLength(kb5Size, asdSize)     ( KBKM_MSG_APREP_SIZE_MIN + kb5Size + asdSize )

#define  AnscKbkmAprepGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmAprepSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmAprepGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmAprepSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmAprepGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmAprepSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmAprepGetKrbApRepHandle(p)               ( (p)->KrbApRep )
#define  AnscKbkmAprepGetKrbApRepSize(p)                 ( AnscGetTLVLength((p)->KrbApRep) )
#define  AnscKbkmAprepSetKrbApRep(p, handle, size)       AnscCopyMemory((p)->KrbApRep, handle, size)


#define  AnscKbkmAprepGetASDHandle(p)                    (PULONG)((ULONG)((p)->KrbApRep) + AnscKbkmAprepGetKrbApRepSize(p))
#define  AnscKbkmAprepGetASDTotalSize(p)                                            \
         (ULONG)(   AnscKbkmAprepGetDomainOfInterp(p) ==  KBKM_DOI_IPSEC ?          \
                    AnscKbkmASDIpsecGetTotalSize ((PKBKM_ASD_IPSEC )AnscKbkmAprepGetASDHandle(p)) :   \
                    AnscKbkmASDSnmpv3GetTotalSize((PKBKM_ASD_SNMPV3)AnscKbkmAprepGetASDHandle(p)) )


#define  AnscKbkmAprepGetCiphersuitNumHandle(p)          (PUCHAR)((ULONG)AnscKbkmAprepGetASDHandle(p) + AnscKbkmAprepGetASDTotalSize(p))
#define  AnscKbkmAprepGetCiphersuitNum(p)                (*(PUCHAR)AnscKbkmAprepGetCiphersuitNumHandle(p))
#define  AnscKbkmAprepSetCiphersuitNum(p, v)             (*(PUCHAR)AnscKbkmAprepGetCiphersuitNumHandle(p) = (UCHAR)v)

#define  AnscKbkmAprepGetCiphersuitHandle(p, i)          (PUCHAR)((ULONG)AnscKbkmAprepGetCiphersuitNumHandle(p) + 1 + i*2)
#define  AnscKbkmAprepGetCiphersuitAuthAlgm(p, i)        (*(PUCHAR)AnscKbkmAprepGetCiphersuitHandle(p, i)           )
#define  AnscKbkmAprepSetCiphersuitAuthAlgm(p, i, v)     (*(PUCHAR)AnscKbkmAprepGetCiphersuitHandle(p, i) = (UCHAR)v)
#define  AnscKbkmAprepGetCiphersuitEncTrans(p, i)        (*(PUCHAR)((ULONG)AnscKbkmAprepGetCiphersuitHandle(p, i) + 1)           )
#define  AnscKbkmAprepSetCiphersuitEncTrans(p, i, v)     (*(PUCHAR)((ULONG)AnscKbkmAprepGetCiphersuitHandle(p, i) + 1) = (UCHAR)v)

#define  AnscKbkmAprepGetSecParaLifetimeHandle(p)        (PULONG)(AnscKbkmAprepGetCiphersuitHandle(p, AnscKbkmAprepGetCiphersuitNum(p)))
#define  AnscKbkmAprepGetSecParaLifetime(p)              AnscUlongFromNToH(AnscReadUlong(AnscKbkmAprepGetSecParaLifetimeHandle(p)) )
#define  AnscKbkmAprepSetSecParaLifetime(p, v)           AnscWriteUlong(AnscKbkmAprepGetSecParaLifetimeHandle(p), AnscUlongFromHToN(v))

#define  AnscKbkmAprepGetGracePeriodHandle(p)            (PULONG)((ULONG)AnscKbkmAprepGetSecParaLifetimeHandle(p) + 4)
#define  AnscKbkmAprepGetGracePeriod(p)                  AnscUlongFromNToH(AnscReadUlong(AnscKbkmAprepGetGracePeriodHandle(p)) )
#define  AnscKbkmAprepSetGracePeriod(p, v)               AnscWriteUlong(AnscKbkmAprepGetGracePeriodHandle(p), AnscUlongFromHToN(v))

#define  AnscKbkmAprepGetReestablishFlagHandle(p)        (PUCHAR)((ULONG)AnscKbkmAprepGetGracePeriodHandle(p) + 4)
#define  AnscKbkmAprepGetReestablishFlag(p)              (*AnscKbkmAprepGetReestablishFlagHandle(p)            )
#define  AnscKbkmAprepSetReestablishFlag(p, v)           (*AnscKbkmAprepGetReestablishFlagHandle(p) = (UCHAR)v )

#define  AnscKbkmAprepGetAckRequiredFlagHandle(p)        (PUCHAR)((ULONG)AnscKbkmAprepGetReestablishFlagHandle(p) + 1)
#define  AnscKbkmAprepGetAckRequiredFlag(p)              (*AnscKbkmAprepGetAckRequiredFlagHandle(p)            )
#define  AnscKbkmAprepSetAckRequiredFlag(p, v)           (*AnscKbkmAprepGetAckRequiredFlagHandle(p) = (UCHAR)v )

#define  AnscKbkmAprepGetSHA1HMAC(p)                     (PUCHAR)((ULONG)AnscKbkmAprepGetAckRequiredFlagHandle(p) + 1)
#define  AnscKbkmAprepGetSHA1HMACSize(p)                 20
#define  AnscKbkmAprepSetSHA1HMAC(p, handle)             AnscCopyMemory(AnscKbkmAprepGetSHA1HMAC(p), handle, 20)

#define  AnscKbkmAprepGetTotalSize(p)                    ( (ULONG)AnscKbkmAprepGetSHA1HMAC(p) + \
                                                           AnscKbkmAprepGetSHA1HMACSize(p)  - (ULONG)(p) )


/******************************************************************************************
                          KBKM_MSG_SECPARA_RECOV

    define security parameter recovered message for Kerberized Key Management. 

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    SHA1HMAC                        20 bytes

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_SECPARA_RECOV
{
    KBKM_BASIC_MSG_CONTENT

    UCHAR                          SHA1HMAC[20];
}_struct_pack_
KBKM_MSG_SECPARA_RECOV,  *PKBKM_MSG_SECPARA_RECOV;

#define  KBKM_MSG_SECPARA_RECOV_SIZE_MIN                   23

/* 
 * define macro to access header fields. When setting KrbApRep, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmSPRecovCalMsgLength()                     ( KBKM_MSG_SECPARA_RECOV_SIZE_MIN )

#define  AnscKbkmSPRecovGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmSPRecovSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmSPRecovGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmSPRecovSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmSPRecovGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmSPRecovSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmSPRecovGetSHA1HMAC(p)                     ( (p)->SHA1HMAC )
#define  AnscKbkmSPRecovGetSHA1HMACSize(p)                 ( ANSC_SHA1_OUTPUT_SIZE )
#define  AnscKbkmSPRecovSetSHA1HMAC(p, handle)             AnscCopyMemory((p)->SHA1HMAC, handle, ANSC_SHA1_OUTPUT_SIZE) 

/******************************************************************************************
                          KBKM_MSG_ERROR

    define ERROR reply for Kerberized Key Management. Since some fields are of variable 
    length, the fields have to be filled in order

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    KrbError                        KrbError encoding, variable

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_ERROR
{
    KBKM_BASIC_MSG_CONTENT

    UCHAR                           KrbError[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_MSG_ERROR,  *PKBKM_MSG_ERROR;

#define  KBKM_MSG_ERROR_SIZE_MIN                   3

/* 
 * define macro to access header fields. When setting KrbApRep, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmErrorCalMsgLength(kb5Size)              ( KBKM_MSG_ERROR_SIZE_MIN + kb5Size )

#define  AnscKbkmErrorGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmErrorSetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmErrorGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmErrorSetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmErrorGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmErrorSetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmErrorGetKrbErrorHandle(p)               ( (p)->KrbError )
#define  AnscKbkmErrorGetKrbErrorSize(p)                 ( AnscGetTLVLength((p)->KrbError) )
#define  AnscKbkmErrorSetKrbError(p, handle, size)       AnscCopyMemory((p)->KrbError, handle, size)

#define  AnscKbkmErrorGetTotalSize(p)                    ( KBKM_MSG_ERROR_SIZE_MIN + \
                                                           AnscKbkmErrorGetKrbErrorSize(p) )


/******************************************************************************************
                          KBKM_MSG_REKEY

    define AP_REP for Kerberized Key Management. Since some fields are of variable 
    length, the fields have to be filled in order

    KmMessageId                     1 byte
    DomainOfInterp                  1 byte
    ProtocolVersion                 1 byte
    Server-nonce                    4 bytes
    ServerKbPrinName                variable
    Timestamp                       13 bytes
    ASD                             size depends on the DomainOfInterp
    CiphersuitNum                   1 byte
    Ciphersuit[i].AuthenAlgm        1 byte
    Ciphersuit[i].EncTrans          1 byte
    SecParaLifetime                 4 bytes
    GracePeriod                     4 bytes
    ReestablishFlag                 1 byte
    SHA1HMAC                        20 bytes

 ******************************************************************************************/
typedef  struct
_KBKM_MSG_REKEY
{
    KBKM_BASIC_MSG_CONTENT

    ULONG                           ServerNonce;
    UCHAR                           ServerKbPrinName[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
KBKM_MSG_REKEY,  *PKBKM_MSG_REKEY;

#define  KBKM_MSG_REKEY_SIZE_MIN                   50
#define  KBKM_MSG_REKEY_TIMESTAMP_SIZE             13

/* 
 * define macro to access header fields. When setting KrbApRep, the macro assumes that
 * enough memory is avaialble
 */
#define  AnscKbkmRekeyCalMsgLength(kb5Size, asdSize, ciphNum)     \
            ( KBKM_MSG_REKEY_SIZE_MIN + kb5Size + asdSize + ciphNum * 2)

#define  AnscKbkmRekeyGetKmMessageId(p)                  ( (p)->KmMessageId )
#define  AnscKbkmRekeySetKmMessageId(p, v)               ( (p)->KmMessageId = (UCHAR)v )
#define  AnscKbkmRekeyGetDomainOfInterp(p)               ( (p)->DomainOfInterp )
#define  AnscKbkmRekeySetDomainOfInterp(p, v)            ( (p)->DomainOfInterp = (UCHAR)v )
#define  AnscKbkmRekeyGetProtocolVersion(p)              ( (p)->ProtocolVersion )
#define  AnscKbkmRekeySetProtocolVersion(p, v)           ( (p)->ProtocolVersion = (UCHAR)v )
#define  AnscKbkmRekeyGetServerNonce(p)                  ( AnscUlongFromNToH(AnscReadUlong(&(p)->ServerNonce)) )
#define  AnscKbkmRekeySetServerNonce(p, v)               AnscWriteUlong(&(p)->ServerNonce, AnscUlongFromHToN(v))
#define  AnscKbkmRekeyGetServerKbPrinNameHandle(p)       (PUCHAR)( (p)->ServerKbPrinName )
#define  AnscKbkmRekeyGetServerKbPrinNameLength(p)       ( AnscSizeOfString((p)->ServerKbPrinName) + 1) /* 1 for null-termination */
#define  AnscKbkmRekeySetServerKbPrinName(p, str)        AnscCopyString((p)->ServerKbPrinName, str)

#define  AnscKbkmRekeyGetTimestampHandle(p)              (PUCHAR)((ULONG)((p)->ServerKbPrinName) + AnscKbkmRekeyGetServerKbPrinNameLength(p))
#define  AnscKbkmRekeyGetTimestampSize(p)                KBKM_MSG_REKEY_TIMESTAMP_SIZE
#define  AnscKbkmRekeySetTimestamp(p, str)               AnscCopyMemory(AnscKbkmRekeyGetTimestampHandle(p), str, KBKM_MSG_REKEY_TIMESTAMP_SIZE)


#define  AnscKbkmRekeyGetASDHandle(p)                    (PULONG)((ULONG)(AnscKbkmRekeyGetTimestampHandle(p)) + AnscKbkmRekeyGetTimestampSize(p))
#define  AnscKbkmRekeyGetASDTotalSize(p)                                            \
         (ULONG)(   AnscKbkmRekeyGetDomainOfInterp(p) ==  KBKM_DOI_IPSEC ?          \
                    AnscKbkmASDIpsecGetTotalSize ((PKBKM_ASD_IPSEC )AnscKbkmRekeyGetASDHandle(p)) :   \
                    AnscKbkmASDSnmpv3GetTotalSize((PKBKM_ASD_SNMPV3)AnscKbkmRekeyGetASDHandle(p)) )


#define  AnscKbkmRekeyGetCiphersuitNumHandle(p)          (PUCHAR)((ULONG)AnscKbkmRekeyGetASDHandle(p) + AnscKbkmRekeyGetASDTotalSize(p))
#define  AnscKbkmRekeyGetCiphersuitNum(p)                (*(PUCHAR)AnscKbkmRekeyGetCiphersuitNumHandle(p))
#define  AnscKbkmRekeySetCiphersuitNum(p, v)             (*(PUCHAR)AnscKbkmRekeyGetCiphersuitNumHandle(p) = (UCHAR)v)

#define  AnscKbkmRekeyGetCiphersuitHandle(p, i)          (PUCHAR)((ULONG)AnscKbkmRekeyGetCiphersuitNumHandle(p) + 1 + i*2)
#define  AnscKbkmRekeyGetCiphersuitAuthAlgm(p, i)        (*(PUCHAR)AnscKbkmRekeyGetCiphersuitHandle(p, i)           )
#define  AnscKbkmRekeySetCiphersuitAuthAlgm(p, i, v)     (*(PUCHAR)AnscKbkmRekeyGetCiphersuitHandle(p, i) = (UCHAR)v)
#define  AnscKbkmRekeyGetCiphersuitEncTrans(p, i)        (*(PUCHAR)((ULONG)AnscKbkmRekeyGetCiphersuitHandle(p, i) + 1)           )
#define  AnscKbkmRekeySetCiphersuitEncTrans(p, i, v)     (*(PUCHAR)((ULONG)AnscKbkmRekeyGetCiphersuitHandle(p, i) + 1) = (UCHAR)v)

#define  AnscKbkmRekeyGetSecParaLifetimeHandle(p)        (PULONG)(AnscKbkmRekeyGetCiphersuitHandle(p, AnscKbkmRekeyGetCiphersuitNum(p)))
#define  AnscKbkmRekeyGetSecParaLifetime(p)              AnscUlongFromNToH(AnscReadUlong(AnscKbkmRekeyGetSecParaLifetimeHandle(p)) )
#define  AnscKbkmRekeySetSecParaLifetime(p, v)           AnscWriteUlong(AnscKbkmRekeyGetSecParaLifetimeHandle(p), AnscUlongFromHToN(v))

#define  AnscKbkmRekeyGetGracePeriodHandle(p)            (PULONG)((ULONG)AnscKbkmRekeyGetSecParaLifetimeHandle(p) + 4)
#define  AnscKbkmRekeyGetGracePeriod(p)                  AnscUlongFromNToH(AnscReadUlong(AnscKbkmRekeyGetGracePeriodHandle(p)) )
#define  AnscKbkmRekeySetGracePeriod(p, v)               AnscWriteUlong(AnscKbkmRekeyGetGracePeriodHandle(p), AnscUlongFromHToN(v))

#define  AnscKbkmRekeyGetReestablishFlagHandle(p)        (PUCHAR)((ULONG)AnscKbkmRekeyGetGracePeriodHandle(p) + 4)
#define  AnscKbkmRekeyGetReestablishFlag(p)              (*AnscKbkmRekeyGetReestablishFlagHandle(p)            )
#define  AnscKbkmRekeySetReestablishFlag(p, v)           (*AnscKbkmRekeyGetReestablishFlagHandle(p) = (UCHAR)v )

#define  AnscKbkmRekeyGetSHA1HMAC(p)                     (PUCHAR)((ULONG)AnscKbkmRekeyGetReestablishFlagHandle(p) + 1)
#define  AnscKbkmRekeyGetSHA1HMACSize(p)                 20
#define  AnscKbkmRekeySetSHA1HMAC(p, handle)             AnscCopyMemory(AnscKbkmRekeyGetSHA1HMAC(p), handle, 20)

#define  AnscKbkmRekeyGetTotalSize(p)                    ( (ULONG)AnscKbkmRekeyGetSHA1HMAC(p) + \
                                                           AnscKbkmRekeyGetSHA1HMACSize(p)  - (ULONG)(p) )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
