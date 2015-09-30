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

    module:	ansc_packet_binary_rip2.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Routing Information Protocol Version 2
        (RIP2). Support RFC2453, RFC 2082, and RFC 1722.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        12/31/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_RIP2_
#define  _ANSC_PACKET_BINARY_RIP2_

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
                 DATA STRUCTURES USED BY RIP2
***********************************************************/

/*
 * The Routing Inforamtion Protocol Version 2(RIP2) is an Interior Gateway routing
 * Protocol that has very little overhead in terms of bandwidth used and 
 * configuration and management time. It is based on the Bellman-Ford algorithm
 * for calculating routs, and is suitable for small networks.
 */

#define  RIP2_MULTICAST_IPADDR                      AnscUlongFromHToN(0xe0000009)
#define  RIP2_DEFAULT_ROUTE_IPADDR                  0x00000000


#define  RIP2_UDP_PORT                              520
#define  RIP2_DEFAULT_ROUTE                         0x00000000

#define  RIP2_COMMAND_REQ                           1
#define  RIP2_COMMAND_RSP                           2

#define  RIP2_VERSION_UNKNOWN                       0
#define  RIP2_VERSION_1                             1
#define  RIP2_VERSION_2                             2

#define  RIP2_AF_ZERO                               0
#define  RIP2_AF_INET                               2
#define  RIP2_AF_AUTHENTICATION                     (0xffff)

#define  RIP2_REQ_WHOLE_TABLE                       0
#define  RIP2_REQ_SPECIFIC_ENTRIES                  1

#define  RIP2_METRIC_DEFAULT                        1
#define  RIP2_METRIC_MINIMUM                        1
#define  RIP2_METRIC_INFINITY                       16

#define  RIP2_AUTH_TYPE_DISABLE                     0
#define  RIP2_AUTH_TYPE_IP_ROUTE                    1
#define  RIP2_AUTH_TYPE_PASSWARD                    2
#define  RIP2_AUTH_TYPE_KEYED_MSG_DIGEST            3

#define  RIP2_MD5_KEY_SIZE                          16
#define  RIP2_MD5_DIGEST_SIZE                       16
#define  RIP2_PASSWARD_AUTH_SIZE                    16

#define  RIP2_ENTRY_NUM_NO_AUTH                     25
#define  RIP2_ENTRY_NUM_WITH_AUTH                   24

#define  RIP2_ENTRY_AUTH_MD5_RSV_SIZE               8


typedef  struct
_RIP2_HEADER
{
    UCHAR                           Command;
    UCHAR                           Version;
    USHORT                          Unused;
    UCHAR                           Entrys[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RIP2_HEADER,  *PRIP2_HEADER;

typedef  struct
_RIP2_ENTRY_ROUTE
{
    USHORT                          AddrFamilyID;
    USHORT                          RoutTag;
    ANSC_IPV4_ADDRESS               IPAddr;
    ANSC_IPV4_ADDRESS               SubnetMask;
    ANSC_IPV4_ADDRESS               NextHop;
    ULONG                           Metric;
}_struct_pack_
RIP2_ENTRY_ROUTE,  *PRIP2_ENTRY_ROUTE;

typedef  struct
_RIP2_ENTRY_AUTH
{
    USHORT                          AddrFamilyID;
    USHORT                          AuthType;
    UCHAR                           Authentication[RIP2_PASSWARD_AUTH_SIZE];
}_struct_pack_
RIP2_ENTRY_AUTH,  *PRIP2_ENTRY_AUTH;

typedef  struct
_RIP2_ENTRY_AUTH_MD5
{
    USHORT                          AddrFamilyID;
    USHORT                          AuthType;
    USHORT                          PktLength;
    UCHAR                           KeyID;
    UCHAR                           AuthLength;
    ULONG                           SequenceNum;
    UCHAR                           Reserved[RIP2_ENTRY_AUTH_MD5_RSV_SIZE];
}_struct_pack_
RIP2_ENTRY_AUTH_MD5,  *PRIP2_ENTRY_AUTH_MD5;


#define  RIP2_HEADER_SIZE                           4
#define  RIP2_ENTRY_SIZE                            20


#define  AnscRip2GetCommand(p)                      ( p->Command                                      )
#define  AnscRip2GetVersion(p)                      ( p->Version                                      )
#define  AnscRip2GetUnused(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Unused))  )
#define  AnscRip2GetFirstEntry(p)                   ( p->Entrys                                       )
#define  AnscRip2GetNextEntry(p)                    ( (PRIP2_ENTRY_ROUTE)((ULONG)p + RIP2_ENTRY_SIZE) )

#define  AnscRip2EntryRouteGetAF(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->AddrFamilyID)) )
#define  AnscRip2EntryRouteGetRoutTag(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->RoutTag)) )
#define  AnscRip2EntryRouteGetIPAddrValue(p)        ( AnscReadUlong(&p->IPAddr.Value)    )
#define  AnscRip2EntryRouteGetIPAddrDot(p)          ( p->IPAddr.Dot                      )
#define  AnscRip2EntryRouteGetMaskValue(p)          ( AnscReadUlong(&p->SubnetMask.Value))
#define  AnscRip2EntryRouteGetMaskDot(p)            ( p->SubnetMask.Dot                  )
#define  AnscRip2EntryRouteGetNextHopValue(p)       ( AnscReadUlong(&p->NextHop.Value)   )
#define  AnscRip2EntryRouteGetNextHopDot(p)         ( p->NextHop.Dot                     )
#define  AnscRip2EntryRouteGetMetric(p)             ( AnscUlongFromNToH(AnscReadUlong(&p->Metric))    )

#define  AnscRip2EntryAuthGetAF(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->AddrFamilyID)) )
#define  AnscRip2EntryAuthGetAuthType(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->AuthType)))
#define  AnscRip2EntryAuthGetAuthentication(p)      ( p->Authentication                               )

#define  AnscRip2EntryAuthMD5GetAF(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->AddrFamilyID)) )
#define  AnscRip2EntryAuthMD5GetAuthType(p)         ( AnscUshortFromNToH(AnscReadUshort(&p->AuthType)))
#define  AnscRip2EntryAuthMD5GetPktLength(p)        ( AnscUshortFromNToH(AnscReadUshort(&p->PktLength)))
#define  AnscRip2EntryAuthMD5GetKeyID(p)            ( p->KeyID                                        )
#define  AnscRip2EntryAuthMD5GetAuthLength(p)       ( p->AuthLength                                   )
#define  AnscRip2EntryAuthMD5GetSequenceNum(p)      ( AnscUlongFromNToH(AnscReadUlong(&p->SequenceNum)))
#define  AnscRip2EntryAuthMD5GetReserved(p)         ( p->Reserved                                     )


#define  AnscRip2SetCommand(p, c)                   ( p->Command = c                                  )
#define  AnscRip2SetVersion(p, v)                   ( p->Version = v                                  )
#define  AnscRip2SetUnused(p, u)                    AnscWriteUshort(&p->Unused,       AnscUshortFromHToN(u))

#define  AnscRip2EntryRouteSetAF(p, a)              AnscWriteUshort(&p->AddrFamilyID, AnscUshortFromHToN(a)) 
#define  AnscRip2EntryRouteSetRoutTag(p, t)         AnscWriteUshort(&p->RoutTag,      AnscUshortFromHToN(t))
#define  AnscRip2EntryRouteSetIPAddrValue(p, addr)  AnscWriteUlong (&p->IPAddr,       addr                 )
#define  AnscRip2EntryRouteSetIPAddrDot(p, addr)    AnscWriteUlong (&p->IPAddr,       *(PULONG)addr        )
#define  AnscRip2EntryRouteSetMaskValue(p, s)       AnscWriteUlong (&p->SubnetMask,   s                    )
#define  AnscRip2EntryRouteSetMaskDot(p, s)         AnscWriteUlong (&p->SubnetMask,   *(PULONG)s           )
#define  AnscRip2EntryRouteSetNextHopValue(p, n)    AnscWriteUlong (&p->NextHop,      n                    )
#define  AnscRip2EntryRouteSetNextHopDot(p, n)      AnscWriteUlong (&p->NextHop,      *(PULONG)n           )
#define  AnscRip2EntryRouteSetMetric(p, m)          AnscWriteUlong (&p->Metric,       AnscUlongFromHToN(m) )

#define  AnscRip2EntryAuthSetAF(p, a)               AnscWriteUshort(&p->AddrFamilyID, AnscUshortFromHToN(a)) 
#define  AnscRip2EntryAuthSetAuthType(p, auth)      AnscWriteUshort(&p->AuthType,     AnscUshortFromHToN(auth))
#define  AnscRip2EntryAuthSetAuthen(p, pd, l)       AnscCopyMemory (p->Authentication, pd, l               )

#define  AnscRip2EntryAuthMD5SetAF(p, a)            AnscWriteUshort(&p->AddrFamilyID, AnscUshortFromHToN(a))
#define  AnscRip2EntryAuthMD5SetAuthType(p, auth)   AnscWriteUshort(&p->AuthType,     AnscUshortFromHToN(auth))
#define  AnscRip2EntryAuthMD5SetPktLength(p, l)     AnscWriteUshort(&p->PktLength,    AnscUshortFromHToN(l))
#define  AnscRip2EntryAuthMD5SetKeyID(p, k)         ( p->KeyID     = k                                     )
#define  AnscRip2EntryAuthMD5SetAuthLength(p, a)    ( p->AuthLength= a                                     )
#define  AnscRip2EntryAuthMD5SetSequenceNum(p, s)   AnscWriteUlong (&p->SequenceNum,  AnscUlongFromHToN(s) )
#define  AnscRip2EntryAuthMD5SetReserved(p, pr)     AnscCopyMemory (p->Reserved, pr, RIP2_ENTRY_AUTH_MD5_RSV_SIZE)

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
