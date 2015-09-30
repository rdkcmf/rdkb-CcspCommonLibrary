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

    module:	ansc_packet_binary_radius.h

        For Remote Authentication dial In User Srevice (RADIUS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of RADIUS protocols.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        01/03/02    Na Li           initial revision

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_RADIUS_
#define  _ANSC_PACKET_BINARY_RADIUS_

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
                 DATA STRUCTURES USED BY RADS MESSAGE
***********************************************************/

                

#define  RADS_MSG_AUTHENTICATOR_SIZE                 16
#define  RADS_MSG_HEADER_SIZE                        20

/*
 * define Radius header. 
 */
typedef  struct
_RADS_MSG_HEADER
{
    UCHAR                           Code;
    UCHAR                           Identifier;
    USHORT                          Length;
    UCHAR                           Authenticator[RADS_MSG_AUTHENTICATOR_SIZE];
    UCHAR                           Attributes[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RADS_MSG_HEADER,  *PRADS_MSG_HEADER;


/* define macro to access header fields. for optional fields, the caller has to check if the
 * field exist before getting or setting the value
 */
#define  AnscRadsGetCode(p)                         ((p)->Code)
#define  AnscRadsSetCode(p, v)                      ((p)->Code = (UCHAR)v )
#define  AnscRadsGetIdentifier(p)                   ((p)->Identifier)
#define  AnscRadsSetIdentifier(p, v)                ((p)->Identifier = (UCHAR)v )
#define  AnscRadsGetLength(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscRadsSetLength(p, v)                    AnscWriteUshort(&p->Length, AnscUshortFromHToN((USHORT)v))
#define  AnscRadsSetAuthenticator(p, v)             AnscCopyMemory((p)->Authenticator, (PUCHAR)v, RADS_MSG_AUTHENTICATOR_SIZE)
#define  AnscRadsZeroAuthenticator(p)               AnscZeroMemory((p)->Authenticator, RADS_MSG_AUTHENTICATOR_SIZE)

/***********************************************************
                 DATA STRUCTURES USED BY RADS ATTRIBUTE
***********************************************************/

/*
 * define the Attribute header
 */
typedef  struct
_RADS_ATTR_HEADER
{
    UCHAR                           Type;
    UCHAR                           Length;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
RADS_ATTR_HEADER,  *PRADS_ATTR_HEADER;

/*
 * the fixed fields of the RADS AVP occupy 6 bytes
 */ 
#define  RADS_ATTR_MAXIMUM_SIZE                      255
#define  RADS_ATTR_MINIMUM_SIZE                      2
#define  RADS_ATTR_VALUE_MAXIMUM_SIZE                253

/* 
 * excluse Vendor ID + Verndor type + Vendor length 
 */
#define  RADS_ATTR_VALUE_VENDOR_MAXIMUM_SIZE         247 

/*
 * define the macro to access header fields
 */

#define  AnscRadsAttrGetType(p)                     ((p)->Type)
#define  AnscRadsAttrSetType(p, value)              ((p)->Type   = (UCHAR)value)
#define  AnscRadsAttrGetLength(p)                   ((p)->Length)
#define  AnscRadsAttrSetLength(p, value)            ((p)->Length = (UCHAR)value)
#define  AnscRadsAttrGetValueLength(p)              ((p)->Length - RADS_ATTR_MINIMUM_SIZE)
#define  AnscRadsAttrSetValueLength(p, value)       ((p)->Length = (UCHAR)(value + RADS_ATTR_MINIMUM_SIZE))
#define  AnscRadsAttrGetUlongValue(p)               ( (AnscUlongFromNToH(AnscReadUlong(p->Value))) )
#define  AnscRadsAttrSetUlongValue(p, v)                        \
        {                                                       \
            AnscWriteUlong(p->Value, AnscUlongFromHToN(v));     \
            AnscRadsAttrSetValueLength(p, sizeof(ULONG));       \
        }
#define  AnscRadsAttrGetStrValue(p)                  ((p)->Value)

#define  AnscRadsAttrSetStrValue(p, v, l)                       \
        {                                                       \
            AnscCopyMemory(p->Value, v, l);                     \
            AnscRadsAttrSetValueLength(p, l);                   \
        }
#define  AnscRadsAttrZeroStrValue(p, l)                         \
        {                                                       \
            AnscZeroMemory(p->Value, l);                        \
            AnscRadsAttrSetValueLength(p, l);                   \
        }
/*
 * define the macro to set the ulong data in tunnel attributes.
 * the value filed in the tunnel attribute is tag(1 byte) and value (3 bytes)
 */
#define  AnscRadsAttrTunnelGetTag(p)                ((p)->Value[0])
#define  AnscRadsAttrTunnelSetTag(p, t)             ((p)->Value[0] = (UCHAR)t)
#define  AnscRadsAttrTunnelGetUlongValue(p)         ( (AnscUlongFromNToH(AnscReadUlong(p->Value))) & 0x00ffffff )
#define  AnscRadsAttrTunnelSetUlongValue(p, v)                                                      \
        {                                                                                           \
            AnscWriteUlong(p->Value, AnscUlongFromHToN((v & 0x00ffffff) | (p->Value[0] << 24)));    \
            AnscRadsAttrSetValueLength(p, 1 + sizeof(ULONG));                                       \
        }

#define  AnscRadsAttrTunnelGetStrValueLength(p)      ((p)->Length - RADS_ATTR_MINIMUM_SIZE - 1)
#define  AnscRadsAttrTunnelGetStrValue(p)            (&(p)->Value[1])
#define  AnscRadsAttrTunnelSetStrValue(p, v, l)      \
        {                                            \
            AnscCopyMemory(&p->Value[1], v, l);      \
            AnscRadsAttrSetValueLength(p, 1 + l);    \
        }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
