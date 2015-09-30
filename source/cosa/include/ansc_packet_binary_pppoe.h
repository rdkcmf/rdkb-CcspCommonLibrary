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

    module:	ansc_packet_binary_pppoe.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the PPP over Ethernet (PPPoE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/17/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_PPPOE_
#define  _ANSC_PACKET_BINARY_PPPOE_


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
                DATA STRUCTURES USED BY PPPOE
***********************************************************/

/*
 * PPP over Ethernet (PPPoE) provides the ability to connect a network of hosts over a simple
 * bridging access device to a remote Access Concentrator. With this model, each host utilizes
 * it's own PPP stack and the user is presented with a familiar user interface. Access control,
 * billing and type of service can be done a per-user, rather than a per-site, basis.
 */
#define  PPPOE_PADI_CODE                            0x0009      /* Active Discovery Initiation           */
#define  PPPOE_PADO_CODE                            0x0007      /* Active Discovery Offer                */
#define  PPPOE_PADR_CODE                            0x0019      /* Active Discovery Request              */
#define  PPPOE_PADS_CODE                            0x0065      /* Active Discovery Session-Confirmation */
#define  PPPOE_PADT_CODE                            0x00A7      /* Active Discovery Terminate            */

#define  PPPOE_CURRENT_VERSION                      1
#define  PPPOE_CURRENT_TYPE                         1

typedef  struct
_PPPOE_HEADER
{
    UCHAR                           VerAndType;
    UCHAR                           Code;
    USHORT                          SessionId;
    USHORT                          Length;
    UCHAR                           Payload[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
PPPOE_HEADER,  *PPPPOE_HEADER;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscPppoeGetVersion(p)                     ( (p->VerAndType & 0xF0) >> 4 )
#define  AnscPppoeGetType(p)                        ( (p->VerAndType & 0x0F) *  4 )
#define  AnscPppoeGetCode(p)                        ( p->Code )
#define  AnscPppoeGetSessionId(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->SessionId)) )
#define  AnscPppoeGetLength(p)                      ( AnscUshortFromNToH(AnscReadUshort(&p->Length   )) )
#define  AnscPppoeGetPayload(p)                     ( p->Payload )

#define  AnscPppoeSetVersion(p, v)                  ( p->VerAndType = (p->VerAndType & 0x0F) | (UCHAR)(v << 4) )
#define  AnscPppoeSetType(p, t)                     ( p->VerAndType = (p->VerAndType & 0xF0) | (UCHAR)t        )
#define  AnscPppoeSetCode(p, c)                     ( p->Code       = (UCHAR)c )
#define  AnscPppoeSetSessionId(p, id)               AnscWriteUshort(&p->SessionId, AnscUshortFromHToN((USHORT)id))
#define  AnscPppoeSetLength(p, l)                   AnscWriteUshort(&p->Length,    AnscUshortFromHToN((USHORT)l ))

#define  AnscPppoeGetTagByType(p, size, type, tag)                                          \
         {                                                                                  \
            PPPPOE_TAG              temp_tag = (PPPPOE_TAG)AnscPppoeGetPayload(p);          \
                                                                                            \
            tag = NULL;                                                                     \
                                                                                            \
            while ( size > 0 )                                                              \
            {                                                                               \
                if ( AnscPppoeTagGetType(temp_tag) == type )                                \
                {                                                                           \
                    tag = temp_tag;                                                         \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                size    -= AnscPppoeTagGetLength(temp_tag) + sizeof(PPPOE_TAG);             \
                temp_tag = (PPPPOE_TAG)AnscPppoeTagGetNextTag(temp_tag);                    \
            }                                                                               \
         }

/*
 * The PPPoE payload contains zero or more TAGs. A TAG is a TLV (type-length-value) tuple construct
 * and is defined in RFC 2516.
 */
#define  PPPOE_TAG_TYPE_EOFL                        0x0000      /* End of List                */
#define  PPPOE_TAG_TYPE_SN                          0x0101      /* Service Name               */
#define  PPPOE_TAG_TYPE_ACN                         0x0102      /* Access Concentrator Name   */
#define  PPPOE_TAG_TYPE_HS_COOKIE                   0x0103      /* Host Specified Unique Id   */
#define  PPPOE_TAG_TYPE_AC_COOKIE                   0x0104      /* Access Concentrator Cookie */
#define  PPPOE_TAG_TYPE_VS                          0x0105      /* Vendor Specific            */
#define  PPPOE_TAG_TYPE_RS_ID                       0x0110      /* Relay Session Id           */
#define  PPPOE_TAG_TYPE_SN_ERROR                    0x0201      /* Service Name Error         */
#define  PPPOE_TAG_TYPE_AC_ERROR                    0x0202      /* Access Concentrator Error  */
#define  PPPOE_TAG_TYPE_GE_ERROR                    0x0203      /* Generic Error              */

typedef  struct
_PPPOE_TAG
{
    USHORT                          Type;
    USHORT                          Length;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}
PPPOE_TAG,  *PPPPOE_TAG;

/*
 * Define some macros that can help us access the header fields. The "get" and "set" property-based
 * naming schema is pretty straightforward. The only place needs some explanation is the read/write
 * access to the IP address fields: no byte-order rearranging is applied. So caller needs to bear
 * this in mind when accessing the source/destination IP addresses. Note that checksum is another
 * exception.
 */
#define  AnscPppoeTagGetType(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Type  )) )
#define  AnscPppoeTagGetLength(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscPppoeTagGetValue(p)                    ( p->Value )

#define  AnscPppoeTagSetType(p, t)                  AnscWriteUshort(&p->Type,   AnscUshortFromHToN((USHORT)t))
#define  AnscPppoeTagSetLength(p, l)                AnscWriteUshort(&p->Length, AnscUshortFromHToN((USHORT)l))
#define  AnscPppoeTagSetValue(p, v)                 AnscCopyMemory(p->Value, v, AnscPppoeTagGetLength(p))

#define  AnscPppoeTagGetNextTag(p)                  ( (PPPPOE_TAG)((ULONG)p->Value + AnscPppoeTagGetLength(p)) )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
