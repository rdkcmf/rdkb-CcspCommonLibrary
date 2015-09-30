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

    module:	ansc_packet_binary_ipcomp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the IP Compression Protocol (IPCOMP).

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


#ifndef  _ANSC_PACKET_BINARY_IPCOMP_
#define  _ANSC_PACKET_BINARY_IPCOMP_


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
               DATA STRUCTURES USED BY IPCOMP
***********************************************************/

/*
 * Define the data structure for IP compression protocol
 */
#define  MAX_IPCOMP_PAYLOAD_SIZE                    1476

typedef  struct
_IPCOMP_HEADER
{
    UCHAR                           NextHeader;
    UCHAR                           Flags;
    USHORT                          Cpi;
}_struct_pack_
IPCOMP_HEADER,  *PIPCOMP_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscIpcompGetNextHeader(p)                 ( p->NextHeader )
#define  AnscIpcompGetFlags(p)                      ( p->Flags )
#define  AnscIpcompGetCpi(p)                        ( AnscUshortFromNToH(AnscReadUshort(&p->Cpi)) )
#define  AnscIpcompGetSpi(p)                        (ULONG)AnscIpcompGetCpi(p)

#define  AnscIpcompSetNextHeader(p, h)              ( p->NextHeader = h )
#define  AnscIpcompSetFlags(p, l)                   ( p->Flags      = l )
#define  AnscIpcompSetCpi(p, n)                     AnscWriteUshort(&p->Cpi, AnscUshortFromHToN(n))
#define  AnscIpcompSetSpi(p, n)                     AnscIpcompSetCpi(p, (USHORT)n)


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
