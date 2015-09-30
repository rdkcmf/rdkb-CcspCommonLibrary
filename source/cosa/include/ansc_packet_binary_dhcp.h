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

    module:	ansc_packet_binary_dhcp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Dynamic Host Configuration
        Protocol (DHCP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/26/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_DHCP_
#define  _ANSC_PACKET_BINARY_DHCP_


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
                 DATA STRUCTURES USED BY DHCP
***********************************************************/

/*
 * The Dynamic Host Configuration Protocol (DHCP) provides a framework for passing configuration
 * information to hosts on a TCP/IP network. DHCP is based on the Bootstrap Protocol (BOOTP),
 * adding the capability of automatic allocation of reusable network addresses and additional
 * configuration options. DHCP captures the behavior of BOOTP relay agents, and DHCP participants
 * can interoperate with BOOTP participants.
 */
#define  DHCP_OPCODE_BOOTPREQ                       1
#define  DHCP_OPCODE_BOOTPREP                       2

#define  DHCP_BROADCAST_FLAG                        0x8000

#define  DHCP_MAX_HW_ADDRESS_SIZE                   16
#define  DHCP_MAX_SNAME_SIZE                        64
#define  DHCP_MAX_FNAME_SIZE                        128

#define  DHCP_MAGIC_COOKIE                          0x63825363

#define  DHCP_MESSAGE_TYPE_DISCOVER                 1
#define  DHCP_MESSAGE_TYPE_OFFER                    2
#define  DHCP_MESSAGE_TYPE_REQUEST                  3
#define  DHCP_MESSAGE_TYPE_DECLINE                  4
#define  DHCP_MESSAGE_TYPE_ACK                      5
#define  DHCP_MESSAGE_TYPE_NAK                      6
#define  DHCP_MESSAGE_TYPE_RELEASE                  7
#define  DHCP_MESSAGE_TYPE_INFORM                   8

typedef  struct
_DHCP_HEADER
{
    UCHAR                           OpCode;
    UCHAR                           HwType;
    UCHAR                           HwAddrLen;
    UCHAR                           Hops;
    ULONG                           Xid;
    USHORT                          Seconds;
    USHORT                          Flags;
    ANSC_IPV4_ADDRESS               CiAddr;
    ANSC_IPV4_ADDRESS               YiAddr;
    ANSC_IPV4_ADDRESS               SiAddr;
    ANSC_IPV4_ADDRESS               GiAddr;
    UCHAR                           HwAddr [DHCP_MAX_HW_ADDRESS_SIZE];
    char                            Sname  [DHCP_MAX_SNAME_SIZE     ];
    char                            Fname  [DHCP_MAX_FNAME_SIZE     ];
    UCHAR                           Options[ANSC_ZERO_ARRAY_SIZE    ];
}_struct_pack_
DHCP_HEADER,  *PDHCP_HEADER;

#define  AnscDhcpIsBootpRequest(p)                  ( AnscDhcpGetOpCode(p) == DHCP_OPCODE_BOOTPREQ    )
#define  AnscDhcpIsBootpReply(p)                    ( AnscDhcpGetOpCode(p) == DHCP_OPCODE_BOOTPREP    )
#define  AnscDhcpMustBroadcast(p)                   ( (AnscDhcpGetFlags(p) & DHCP_BROADCAST_FLAG) != 0)

#define  AnscDhcpGetOpCode(p)                       ( p->OpCode                                       )
#define  AnscDhcpGetHwType(p)                       ( p->HwType                                       )
#define  AnscDhcpGetHwAddrLen(p)                    ( p->HwAddrLen                                    )
#define  AnscDhcpGetHops(p)                         ( p->Hops                                         )
#define  AnscDhcpGetXid(p)                          ( AnscUlongFromNToH (AnscReadUlong (&p->Xid    )) )
#define  AnscDhcpGetSeconds(p)                      ( AnscUshortFromNToH(AnscReadUshort(&p->Seconds)) )
#define  AnscDhcpGetFlags(p)                        ( AnscUshortFromNToH(AnscReadUshort(&p->Flags  )) )
#define  AnscDhcpGetCiAddrDot(p)                    ( p->CiAddr.Dot                                   )
#define  AnscDhcpGetCiAddrValue(p)                  ( AnscReadUlong(&p->CiAddr.Value)                 )
#define  AnscDhcpGetYiAddrDot(p)                    ( p->YiAddr.Dot                                   )
#define  AnscDhcpGetYiAddrValue(p)                  ( AnscReadUlong(&p->YiAddr.Value)                 )
#define  AnscDhcpGetSiAddrDot(p)                    ( p->SiAddr.Dot                                   )
#define  AnscDhcpGetSiAddrValue(p)                  ( AnscReadUlong(&p->SiAddr.Value)                 )
#define  AnscDhcpGetGiAddrDot(p)                    ( p->GiAddr.Dot                                   )
#define  AnscDhcpGetGiAddrValue(p)                  ( AnscReadUlong(&p->GiAddr.Value)                 )
#define  AnscDhcpGetHwAddr(p)                       ( p->HwAddr                                       )
#define  AnscDhcpGetSname(p)                        ( p->Sname                                        )
#define  AnscDhcpGetFname(p)                        ( p->Fname                                        )
#define  AnscDhcpGetMsgSpecific(p)                  ( AnscUlongFromNToH (AnscReadUlong (p->Fname - 4)))
#define  AnscDhcpGetMagicCookie(p)                  ( AnscUlongFromNToH (AnscReadUlong (p->Options )) )
#define  AnscDhcpGetFirstOption(p)                  (PDHCP_OPTION)((ULONG)p->Options + sizeof(ULONG)  )

#define  AnscDhcpSetOpCode(p, c)                    ( p->OpCode    = c                                )
#define  AnscDhcpSetHwType(p, t)                    ( p->HwType    = t                                )
#define  AnscDhcpSetHwAddrLen(p, l)                 ( p->HwAddrLen = l                                )
#define  AnscDhcpSetHops(p, h)                      ( p->Hops      = h                                )
#define  AnscDhcpSetXid(p, x)                       AnscWriteUlong (&p->Xid,     AnscUlongFromHToN (x))
#define  AnscDhcpSetSeconds(p, s)                   AnscWriteUshort(&p->Seconds, AnscUshortFromHToN(s))
#define  AnscDhcpSetFlags(p, f)                     AnscWriteUshort(&p->Flags,   AnscUshortFromHToN(f))
#define  AnscDhcpSetCiAddrDot(p, addr)              AnscWriteUlong (&p->CiAddr,  *(PULONG)addr        )
#define  AnscDhcpSetCiAddrValue(p, addr)            AnscWriteUlong (&p->CiAddr,  addr                 )
#define  AnscDhcpSetYiAddrDot(p, addr)              AnscWriteUlong (&p->YiAddr,  *(PULONG)addr        )
#define  AnscDhcpSetYiAddrValue(p, addr)            AnscWriteUlong (&p->YiAddr,  addr                 )
#define  AnscDhcpSetSiAddrDot(p, addr)              AnscWriteUlong (&p->SiAddr,  *(PULONG)addr        )
#define  AnscDhcpSetSiAddrValue(p, addr)            AnscWriteUlong (&p->SiAddr,  addr                 )
#define  AnscDhcpSetGiAddrDot(p, addr)              AnscWriteUlong (&p->GiAddr,  *(PULONG)addr        )
#define  AnscDhcpSetGiAddrValue(p, addr)            AnscWriteUlong (&p->GiAddr,  addr                 )
#define  AnscDhcpSetHwAddr(p, addr)                 AnscCopyMemory (p->HwAddr, addr, p->HwAddrLen     )
#define  AnscDhcpSetSname(p, s)                     AnscCopyString (p->Sname,    s                    )
#define  AnscDhcpSetFname(p, s)                     AnscCopyString (p->Fname,    s                    )
#define  AnscDhcpSetMsgSpecific(p, m)               AnscWriteUlong ((p->Fname-4),AnscUlongFromHToN (m))
#define  AnscDhcpSetMagicCookie(p, c)               AnscWriteUlong (p->Options,  AnscUlongFromHToN (c))

#define  AnscDhcpClearSname(p)                      AnscZeroMemory(p->Sname, DHCP_MAX_SNAME_SIZE)
#define  AnscDhcpClearFname(p)                      AnscZeroMemory(p->Fname, DHCP_MAX_FNAME_SIZE)

/*
 * DHCP options have the same format as the BOOTP "vendor extensions" defined in RFC 1497. Options
 * may be fixed length or variable length. All options begin with a tag octet, which uniquely
 * identifies the option. Fixed-lenght options without data consist of only a tag octet. Only
 * options 0 and 255 are fixed length.
 */
#define  DHCP_OPTION_CODE_PAD                       0           /* RFC 1497 Vendor Extensions */
#define  DHCP_OPTION_CODE_END                       0xFF
#define  DHCP_OPTION_CODE_SUBNET_MASK               1
#define  DHCP_OPTION_CODE_TIME_OFFSET               2
#define  DHCP_OPTION_CODE_ROUTER                    3
#define  DHCP_OPTION_CODE_TIME_SERVER               4
#define  DHCP_OPTION_CODE_NAME_SERVER               5
#define  DHCP_OPTION_CODE_DNS_SERVER                6
#define  DHCP_OPTION_CODE_LOG_SERVER                7
#define  DHCP_OPTION_CODE_COOKIE_SERVER             8
#define  DHCP_OPTION_CODE_LPR_SERVER                9
#define  DHCP_OPTION_CODE_IMP_SERVER                10
#define  DHCP_OPTION_CODE_RL_SERVER                 11
#define  DHCP_OPTION_CODE_HOST_NAME                 12
#define  DHCP_OPTION_CODE_FILE_SIZE                 13
#define  DHCP_OPTION_CODE_MERIT_DUMP                14
#define  DHCP_OPTION_CODE_DOMAIN_NAME               15
#define  DHCP_OPTION_CODE_SWAP_SERVER               16
#define  DHCP_OPTION_CODE_ROOT_PATH                 17
#define  DHCP_OPTION_CODE_EXT_PATH                  18

#define  DHCP_OPTION_CODE_FW_ENABLED                19          /* IP Layer Parameters per Host      */
#define  DHCP_OPTION_CODE_NLSR_ENABLED              20          /* Non-local Source Routing          */
#define  DHCP_OPTION_CODE_POLICY_FILTER             21
#define  DHCP_OPTION_CODE_MDR                       22          /* Maximum Datagram Reassembly       */
#define  DHCP_OPTION_CODE_IP_TTL                    23
#define  DHCP_OPTION_CODE_MTU_AGING                 24
#define  DHCP_OPTION_CODE_MTU_PLATEAU               25

#define  DHCP_OPTION_CODE_IF_MTU                    26          /* IP Layer Parameters per Interface */
#define  DHCP_OPTION_CODE_UNI_MTU                   27
#define  DHCP_OPTION_CODE_BROADCAST_ADDR            28
#define  DHCP_OPTION_CODE_ICMP_SMD                  29          /* Perform ICMP Mask Discovery       */
#define  DHCP_OPTION_CODE_MASK_SUPPLIER             30
#define  DHCP_OPTION_CODE_ICMP_RD                   31          /* Perform ICMP Router Discovery     */
#define  DHCP_OPTION_CODE_RS_ADDR                   32          /* Router Solicitation Address       */
#define  DHCP_OPTION_CODE_STATIC_ROUTE              33

#define  DHCP_OPTION_CODE_TRAILER_ENCAP             34          /* Link Parameters per Interface     */
#define  DHCP_OPTION_CODE_ARP_TIMEOUT               35
#define  DHCP_OPTION_CODE_ETHER_ENCAP               36

#define  DHCP_OPTION_CODE_TCP_TTL                   37          /* TCP Parameters                    */
#define  DHCP_OPTION_CODE_TCP_KEEPALIVE             38
#define  DHCP_OPTION_CODE_TCP_KA_GARBAGE            39          /* TCP Keepalive Garbage             */

#define  DHCP_OPTION_CODE_NIS_DOMAIN                40          /* Application and Service Parameter */
#define  DHCP_OPTION_CODE_NIS_SERVER                41
#define  DHCP_OPTION_CODE_NTP_SERVER                42
#define  DHCP_OPTION_CODE_VENDOR_INFO               43
#define  DHCP_OPTION_CODE_NETBIOS_NS                44
#define  DHCP_OPTION_CODE_NETBIOS_DD                45
#define  DHCP_OPTION_CODE_NETBIOS_NT                46
#define  DHCP_OPTION_CODE_NETBIOS_SCOPE             47
#define  DHCP_OPTION_CODE_X_FONT_SERVER             48
#define  DHCP_OPTION_CODE_X_DM                      49
#define  DHCP_OPTION_CODE_NISP_DOMAIN               64
#define  DHCP_OPTION_CODE_NISP_SERVER               65
#define  DHCP_OPTION_CODE_MOBILE_IP_HA              68
#define  DHCP_OPTION_CODE_SMTP_SERVER               69
#define  DHCP_OPTION_CODE_POP3_SERVER               70
#define  DHCP_OPTION_CODE_NNTP_SERVER               71
#define  DHCP_OPTION_CODE_WWW_SERVER                72
#define  DHCP_OPTION_CODE_FINGER_SERVER             73
#define  DHCP_OPTION_CODE_IRC_SERVER                74
#define  DHCP_OPTION_CODE_ST_SERVER                 75
#define  DHCP_OPTION_CODE_STDA_SERVER               76
#define  DHCP_OPTION_CODE_USER_CLASS_ID             77
#define  DHCP_OPTION_CODE_CLASSLESS_STATIC_ROUTES   121
#define  DHCP_OPTION_CODE_TFTP_SERVER               128
#define  DHCP_OPTION_CODE_CALL_SERVER               129

#define  DHCP_OPTION_CODE_REQUESTED_IP              50          /* DHCP-specific Extensions          */
#define  DHCP_OPTION_CODE_LEASE_TIME                51
#define  DHCP_OPTION_CODE_OVERLOAD                  52
#define  DHCP_OPTION_CODE_TFTP_SNAME                66
#define  DHCP_OPTION_CODE_BOOT_FNAME                67
#define  DHCP_OPTION_CODE_MESSAGE_TYPE              53
#define  DHCP_OPTION_CODE_SERVER_ID                 54
#define  DHCP_OPTION_CODE_PARAM_LIST                55
#define  DHCP_OPTION_CODE_MESSAGE                   56
#define  DHCP_OPTION_CODE_MAX_MSG_SIZE              57
#define  DHCP_OPTION_CODE_T1                        58
#define  DHCP_OPTION_CODE_T2                        59
#define  DHCP_OPTION_CODE_VENDOR_ID                 60
#define  DHCP_OPTION_CODE_VENDOR_CLASS_ID           60
#define  DHCP_OPTION_CODE_CLIENT_ID                 61

typedef  struct
_DHCP_OPTION
{
    UCHAR                           Code;
    UCHAR                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
DHCP_OPTION,  *PDHCP_OPTION;

#define  AnscDhcpOptionGetNextOption(p)             (PDHCP_OPTION)((ULONG)p + AnscDhcpOptionGetSize(p))
#define  AnscDhcpOptionGetCode(p)                   ( p->Code   )
#define  AnscDhcpOptionGetLength(p)                 ( p->Length )
#define  AnscDhcpOptionGetSize(p)                   \
         ( ((p->Code == DHCP_OPTION_CODE_PAD) || (p->Code == DHCP_OPTION_CODE_END))? sizeof(UCHAR) : (ULONG)(p->Length) + sizeof(DHCP_OPTION) )
#define  AnscDhcpOptionGetData(p)                   ( p->Data   )

#define  AnscDhcpOptionSetCode(p, c)                ( p->Code   = c )
#define  AnscDhcpOptionSetLength(p, l)              ( p->Length = l )
#define  AnscDhcpOptionSetData(p, d)                AnscCopyMemory(p->Data, d, AnscDhcpOptionGetLength(p))

#define  AnscDhcpFindOption(h, s, c, o)                                                     \
         {                                                                                  \
            int                     iLeftSize = s - sizeof(DHCP_HEADER) - sizeof(ULONG);    \
            PDHCP_OPTION            pOption   = AnscDhcpGetFirstOption(h);                  \
                                                                                            \
            if ( s == sizeof(DHCP_HEADER) )                                                 \
            {                                                                               \
                iLeftSize = 0;                                                              \
            }                                                                               \
                                                                                            \
            while ( (AnscDhcpOptionGetCode(pOption) != c) && (iLeftSize > 0) )              \
            {                                                                               \
                iLeftSize -= (int)AnscDhcpOptionGetSize(pOption);                           \
                pOption    = AnscDhcpOptionGetNextOption(pOption);                          \
            }                                                                               \
                                                                                            \
            if ( iLeftSize <= 0 )                                                           \
            {                                                                               \
                o = NULL;                                                                   \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                o = pOption;                                                                \
            }                                                                               \
         }

#define  AnscDhcpOptionGetUchar(p, i)               ( p->Data[i]                                          )
#define  AnscDhcpOptionGetUshort(p, i)              ( AnscUshortFromNToH(AnscReadUshort(&p->Data[i * 2])) )
#define  AnscDhcpOptionGetUlong(p, i)               ( AnscUlongFromNToH (AnscReadUlong (&p->Data[i * 4])) )
#define  AnscDhcpOptionGetIpAddr(p, i)              ( AnscReadUlong(&p->Data[i * 4])                      )

#define  AnscDhcpOptionSetUchar(p, i, v)            ( p->Data[i] = v                                      )
#define  AnscDhcpOptionSetUshort(p, i, v)           AnscWriteUshort(&p->Data[i * 2], AnscUshortFromHToN(v))
#define  AnscDhcpOptionSetUlong(p, i, v)            AnscWriteUlong (&p->Data[i * 4], AnscUlongFromHToN (v))
#define  AnscDhcpOptionSetIpAddr(p, i, v)           AnscWriteUlong (&p->Data[i * 4], v                    )

/*
 * This form of DUID is assigned by the vendor to the device. It consists of the vendor's
 * registered Private Enterprise Number as maintained by IANA [6] followed by a unique identifier
 * assigned by the vendor. The following diagram summarizes the structure of a DUID-EN:
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |               2               |       enterprise-number       |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   enterprise-number (contd)   |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *      .                           identifier                          .
 *      .                       (variable length)                       .
 *      .                                                               .
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_DHCP_RFC3315_DUID_EN
{
    USHORT                          Type;
    ULONG                           EnterpriseNumber;
    UCHAR                           Identifier[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
DHCP_RFC3315_DUID_EN,  *PDHCP_RFC3315_DUID_EN;

#define  AnscDhcpRfc3315DuidEnGetType(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->Type)) )
#define  AnscDhcpRfc3315DuidEnGetEnterpriseNumber(p)                                        \
         ( AnscUlongFromNToH(AnscReadUlong(&p->EnterpriseNumber)) )
#define  AnscDhcpRfc3315DuidEnGetIdentifier(p)      ( p->Identifier )

#define  AnscDhcpRfc3315DuidEnSetType(p, t)         AnscWriteUshort(&p->Type, AnscUshortFromHToN(t))
#define  AnscDhcpRfc3315DuidEnSetEnterpriseNumber(p, en)                                    \
         AnscWriteUlong(&p->EnterpriseNumber, AnscUshortFromHToN(en))
#define  AnscDhcpRfc3315DuidEnSetIdentifier(p, id, size)                                    \
         AnscCopyMemory(p->Identifier, id, size)

/*
 * The format of the V-I Vendor-specific Information option is as follows:
 *
 *                           1 1 1 1 1 1
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |  option-code  |  option-len   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |      enterprise-number1       |
 *      |                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   data-len1   |               |
 *      +-+-+-+-+-+-+-+-+ option-data1  |
 *      /                               /
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ----
 *      |      enterprise-number2       |   ^
 *      |                               |   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   |
 *      |   data-len2   |               | optional
 *      +-+-+-+-+-+-+-+-+ option-data2  |   |
 *      /                               /   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   |
 *      ~            ...                ~   V
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ----
 *
 *      option-code         OPTION_V-I_VENDOR_OPTS (125)
 *      option-len          total length of all following option data in
 *                          octets
 *      enterprise-numberN  The vendor's registered 32-bit Enterprise Number
 *                          as registered with IANA [3]
 *      data-lenN           Length of option-data field
 *      option-dataN        Vendor-specific options, described below
 */
typedef  struct
_DHCP_RFC3925_VIVS
{
    ULONG                           EnterpriseNumber;
    UCHAR                           DataLen;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
DHCP_RFC3925_VIVS,  *PDHCP_RFC3925_VIVS;

#define  AnscDhcpRfc3925VivsGetNext(p)              (PDHCP_RFC3925_VIVS)((ULONG)p + AnscDhcpRfc3925VivsGetSize(p))

#define  AnscDhcpRfc3925VivsGetEnterpriseNumber(p)  ( AnscUlongFromNToH(AnscReadUlong(&p->EnterpriseNumber)) )
#define  AnscDhcpRfc3925VivsGetDataLen(p)           ( p->DataLen )
#define  AnscDhcpRfc3925VivsGetData(p)              ( p->Data    )
#define  AnscDhcpRfc3925VivsGetSize(p)              ( sizeof(DHCP_RFC3925_VIVS) + AnscDhcpRfc3925VivsGetDataLen(p) )

#define  AnscDhcpRfc3925VivsSetEnterpriseNumber(p, en)                                      \
         AnscWriteUlong(&p->EnterpriseNumber, AnscUshortFromHToN(en))
#define  AnscDhcpRfc3925VivsSetDataLen(p, l)        ( p->DataLen = l )
#define  AnscDhcpRfc3925VivsSetData(p, d, size)                                             \
         AnscCopyMemory(p->Data, d, size)


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
