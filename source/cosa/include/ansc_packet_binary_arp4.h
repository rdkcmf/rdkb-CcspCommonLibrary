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

    module:	ansc_packet_binary_arp4.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Address Resolution Protocol (ARP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/18/01    modify the file name from ansc_protocol_binary
                    to ansc_packet_binary.
        01/19/01    restructure the container object, add and
                    remove a few things
        09/11/01    add ppp related packet structures
        10/03/01    break it up into several header files; one for
                    each specific protocol suite

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_ARP4_
#define  _ANSC_PACKET_BINARY_ARP4_


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
                DATA STRUCTURES USED BY ARP
***********************************************************/

/*
 * Define some const values used in the Arp message definition.
 */
#define  ARP_REQUEST_TYPE                           0x0001
#define  ARP_RESPONSE_TYPE                          0x0002

#define  HARDWARE_ETHERNET_TYPE                     0x0001
#define  HARDWARE_EXPERIMENTAL_ETHERNET_TYPE        0x0002
#define  HARDWARE_IEEE802_TYPE                      0x0006

/*
 * This structure is for parsing Arp message.
 */
typedef  struct
_ARPV4_MESSAGE
{
    USHORT                          HardwareAddressType;                /* type of hardware address, 0x0001 for Ethernet */
    USHORT                          ProtocolAddressType;                /* type of protocol address, 0x0800 for IP address */
    UCHAR                           HardwareAddressSize;                /* size in bytes of the hardware address, 0x06 for Ethernet address */
    UCHAR                           ProtocolAddressSize;                /* size in bytes of the protocol address, 0x04 for IP address */
    USHORT                          OperationType;                      /* operation type, 0x0001 - ARP request, 0x0002 - ARP response, 0x0003 - RARP request, 0x0004 - RARP response */
    UCHAR                           SrcMacAddress[ANSC_MAC_ADDRESS_SIZE];
    ANSC_IPV4_ADDRESS               SrcNetAddress;
    UCHAR                           DstMacAddress[ANSC_MAC_ADDRESS_SIZE];
    ANSC_IPV4_ADDRESS               DstNetAddress;
}_struct_pack_
ARPV4_MESSAGE,  *PARPV4_MESSAGE;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses.
 */
#define  AnscArpv4GetMacAddressType(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->HardwareAddressType)) )
#define  AnscArpv4GetNetAddressType(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->ProtocolAddressType)) )
#define  AnscArpv4GetMacAddressSize(p)              ( p->HardwareAddressSize )
#define  AnscArpv4GetNetAddressSize(p)              ( p->ProtocolAddressSize )
#define  AnscArpv4GetOperationType(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->OperationType)) )
#define  AnscArpv4GetSrcMacAddress(p)               ( p->SrcMacAddress )
#define  AnscArpv4GetDstMacAddress(p)               ( p->DstMacAddress )
#define  AnscArpv4GetSrcNetAddressDot(p)            ( p->SrcNetAddress.Dot )
#define  AnscArpv4GetSrcNetAddressValue(p)          ( AnscReadUlong(&p->SrcNetAddress.Value) )
#define  AnscArpv4GetDstNetAddressDot(p)            ( p->DstNetAddress.Dot )
#define  AnscArpv4GetDstNetAddressValue(p)          ( AnscReadUlong(&p->DstNetAddress.Value) )

#define  AnscArpv4SetMacAddressType(p, type)        AnscWriteUshort(&p->HardwareAddressType,  AnscUshortFromHToN(type))
#define  AnscArpv4SetNetAddressType(p, type)        AnscWriteUshort(&p->ProtocolAddressType,  AnscUshortFromHToN(type))
#define  AnscArpv4SetMacAddressSize(p, size)        ( p->HardwareAddressSize = size )
#define  AnscArpv4SetNetAddressSize(p, size)        ( p->ProtocolAddressSize = size )
#define  AnscArpv4SetOperationType(p, type)         AnscWriteUshort(&p->OperationType,        AnscUshortFromHToN(type))
#define  AnscArpv4SetSrcMacAddress(p, addr)         AnscCopyMemory (&p->SrcMacAddress[0],     addr, ANSC_MAC_ADDRESS_SIZE)
#define  AnscArpv4SetDstMacAddress(p, addr)         AnscCopyMemory (&p->DstMacAddress[0],     addr, ANSC_MAC_ADDRESS_SIZE)
#define  AnscArpv4SetSrcNetAddressDot(p, addr)      AnscWriteUlong (&p->SrcNetAddress.Dot[0], *(PULONG)addr)
#define  AnscArpv4SetSrcNetAddressValue(p, addr)    AnscWriteUlong (&p->SrcNetAddress.Value,  addr)
#define  AnscArpv4SetDstNetAddressDot(p, addr)      AnscWriteUlong (&p->DstNetAddress.Dot[0], *(PULONG)addr)
#define  AnscArpv4SetDstNetAddressValue(p, addr)    AnscWriteUlong (&p->DstNetAddress.Value,  addr)

#define  AnscArpv4IsArpRequest(p)                   ( AnscArpv4GetOperationType(p) == ARP_REQUEST_TYPE )

/*
 * Following are the macros with shorter names ...
 */
#define  AnscArpv4GetMacAddrType                    AnscArpv4GetMacAddressType
#define  AnscArpv4GetNetAddrType                    AnscArpv4GetNetAddressType
#define  AnscArpv4GetMacAddrSize                    AnscArpv4GetMacAddressSize
#define  AnscArpv4GetNetAddrSize                    AnscArpv4GetNetAddressSize
#define  AnscArpv4GetSrcMacAddr                     AnscArpv4GetSrcMacAddress
#define  AnscArpv4GetDstMacAddr                     AnscArpv4GetDstMacAddress
#define  AnscArpv4GetSrcNetAddrDot                  AnscArpv4GetSrcNetAddressDot
#define  AnscArpv4GetSrcNetAddrValue                AnscArpv4GetSrcNetAddressValue
#define  AnscArpv4GetDstNetAddrDot                  AnscArpv4GetDstNetAddressDot
#define  AnscArpv4GetDstNetAddrValue                AnscArpv4GetDstNetAddressValue

#define  AnscArpv4SetMacAddrType                    AnscArpv4SetMacAddressType
#define  AnscArpv4SetNetAddrType                    AnscArpv4SetNetAddressType
#define  AnscArpv4SetMacAddrSize                    AnscArpv4SetMacAddressSize
#define  AnscArpv4SetNetAddrSize                    AnscArpv4SetNetAddressSize
#define  AnscArpv4SetSrcMacAddr                     AnscArpv4SetSrcMacAddress
#define  AnscArpv4SetDstMacAddr                     AnscArpv4SetDstMacAddress
#define  AnscArpv4SetSrcNetAddrDot                  AnscArpv4SetSrcNetAddressDot
#define  AnscArpv4SetSrcNetAddrValue                AnscArpv4SetSrcNetAddressValue
#define  AnscArpv4SetDstNetAddrDot                  AnscArpv4SetDstNetAddressDot
#define  AnscArpv4SetDstNetAddrValue                AnscArpv4SetDstNetAddressValue


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
