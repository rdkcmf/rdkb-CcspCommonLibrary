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

    module:	ansc_external.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module encapsulates all the statically-linked data
        structures and arrays.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#include "ansc_global.h"


/***********************************************************
       CORRELATION FOR FILE EXTENSIONS AND MEDIA TYPES
***********************************************************/

/*
 * There're cases where we need to identify the media type of a particular disk file. For example:
 * in HTTP, many web server implementations simply map the request URI sent by client to a disk
 * file. In order to describe the requested entity, server needs to construct a "Content-Type" HTTP
 * header field along with the file content.
 */
char*  g_FileExtArray[] =
{
    "text",
    "tex",
    "txt",
    "html",
    "htm",
    "hml",
    "xml",
    "css",
    "bsp",
    "jpeg",
    "jpg",
    "jeg",
    "gif",
    "ief",
    "g3f",
    "tiff",
    "tif",
    "cgm",
    "png",
    "svg",
    "js",
    NULL
};

ULONG  g_MediaTypeArray[] =
{
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_TEXT,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_IMAGE,
    IANA_MEDIA_TYPE_CODE_APPLICATION,
    0
};

ULONG  g_SubTypeArray[] =
{
    IANA_MT_TX_STYPE_CODE_PLAIN,
    IANA_MT_TX_STYPE_CODE_PLAIN,
    IANA_MT_TX_STYPE_CODE_PLAIN,
    IANA_MT_TX_STYPE_CODE_HTML,
    IANA_MT_TX_STYPE_CODE_HTML,
    IANA_MT_TX_STYPE_CODE_HTML,
    IANA_MT_TX_STYPE_CODE_XML,
    IANA_MT_TX_STYPE_CODE_CSS,
    IANA_MT_TX_STYPE_CODE_HTML,
    IANA_MT_IM_STYPE_CODE_JPEG,
    IANA_MT_IM_STYPE_CODE_JPEG,
    IANA_MT_IM_STYPE_CODE_JPEG,
    IANA_MT_IM_STYPE_CODE_GIF,
    IANA_MT_IM_STYPE_CODE_IEF,
    IANA_MT_IM_STYPE_CODE_G3FAX,
    IANA_MT_IM_STYPE_CODE_TIFF,
    IANA_MT_IM_STYPE_CODE_TIFF,
    IANA_MT_IM_STYPE_CODE_CGM,
    IANA_MT_IM_STYPE_CODE_PNG,
    IANA_MT_IM_STYPE_CODE_SVG_XML,
    IANA_MT_AP_STYPE_CODE_X_JAVASCRIPT,
    0
};


/***********************************************************
       CORRELATION FOR PROTOCOL NAMES AND PORT NUMBERS
***********************************************************/

/*
 * Most people are more comfortable dealing with names instead of numbers. This observation also
 * holds true in the programming world. It's just plain nice to be able to convert a protocol name,
 * such as TCP or HTTP, into the corresonding protocol_type/port_number.
 */
char*  g_ProtoNameArray[] =
{
    "arp",
    "ip",
    "pppoe-ds",
    "pppoe-es",
    "icmp",
    "igmp",
    "tcp",
    "udp",
    "gre",
    "ah",
    "esp",
    "ipcomp",
    "ospf",
    "dhcp server",
    "dhcp client",
    "http",
    "pptp",
    "rtp",
    "rtcp",
    "dns",
    NULL
};

ULONG  g_ProtoTypeArray[] =
{
    ETH_FRAME_ARP,
    ETH_FRAME_IP,
    ETH_FRAME_PPPOE_DS,
    ETH_FRAME_PPPOE_SS,
    IP4_PROTOCOL_ICMP,
    IP4_PROTOCOL_IGMP,
    IP4_PROTOCOL_TCP,
    IP4_PROTOCOL_UDP,
    IP4_PROTOCOL_GRE,
    IP4_PROTOCOL_AH,
    IP4_PROTOCOL_ESP,
    IP4_PROTOCOL_IPCOMP,
    IP4_PROTOCOL_OSPF,
    APP_PORT_DHCP_SERVER,
    APP_PORT_DHCP_CLIENT,
    APP_PORT_HTTP,
    APP_PORT_PPTP,
    APP_PORT_RTP,
    APP_PORT_RTCP,
    APP_PORT_DNS,
    0
};


/***********************************************************
         MORE COMPLETE SET OF NAME AND CODE MAPPINGS
***********************************************************/

/*
 * Most people are more comfortable dealing with names instead of numbers. This observation also
 * holds true in the programming world. It's just plain nice to be able to convert a protocol name,
 * such as TCP or HTTP, into the corresonding protocol_type/port_number.
 */
char*  g_AppPortNameArray[] =
{
    "dhcp server",
    "dhcp client",
    "http",
    "pptp",
    "rtp",
    "rtcp",
    "dns",
    NULL
};

ULONG  g_AppPortCodeArray[] =
{
    APP_PORT_DHCP_SERVER,
    APP_PORT_DHCP_CLIENT,
    APP_PORT_HTTP,
    APP_PORT_PPTP,
    APP_PORT_RTP,
    APP_PORT_RTCP,
    APP_PORT_DNS,
    0
};

char*  g_IpProtoNameArray[] =
{
    "icmp",
    "igmp",
    "tcp",
    "udp",
    "gre",
    "ah",
    "esp",
    "ipcomp",
    "ospf",
    NULL
};

ULONG  g_IpProtoCodeArray[] =
{
    IP4_PROTOCOL_ICMP,
    IP4_PROTOCOL_IGMP,
    IP4_PROTOCOL_TCP,
    IP4_PROTOCOL_UDP,
    IP4_PROTOCOL_GRE,
    IP4_PROTOCOL_AH,
    IP4_PROTOCOL_ESP,
    IP4_PROTOCOL_IPCOMP,
    IP4_PROTOCOL_OSPF,
    0
};

char*  g_PhyLinkNameArray[] =
{
    "LAN",
    "WAN",
    "DMZ",
    "loopback",
    "ethernet",
    "token ring",
    "fddi",
    "ppp",
    "ppp over ethernet",
    "x25",
    "isdn",
    "serial",
    "frame relay",
    "atm",
    "sonet",
    "irda",
    NULL
};

ULONG  g_PhyLinkCodeArray[] =
{
    ANSC_LINK_TYPE_LAN,
    ANSC_LINK_TYPE_WAN,
    ANSC_LINK_TYPE_DMZ,
    ANSC_LINK_TYPE_LOOPBACK,
    ANSC_LINK_TYPE_ETHERNET,
    ANSC_LINK_TYPE_TOKEN_RING,
    ANSC_LINK_TYPE_FDDI,
    ANSC_LINK_TYPE_PPP,
    ANSC_LINK_TYPE_PPPOE,
    ANSC_LINK_TYPE_X25,
    ANSC_LINK_TYPE_ISDN,
    ANSC_LINK_TYPE_SERIAL,
    ANSC_LINK_TYPE_FRAME_RELAY,
    ANSC_LINK_TYPE_ATM,
    ANSC_LINK_TYPE_SONET,
    ANSC_LINK_TYPE_IRDA,
    0
};


/***********************************************************
               PREDEFINED DEFAULT OAKLEY GROUPS
***********************************************************/

/*
 * FIRST OAKLEY DEFAULT GROUP - MODP
 */
UCHAR
g_OakleyGroup1_PrimeH[96] =     /* 2^768 - 2^704 - 1 + 2 ^64 * {[2^638 pi] + 149686} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x36, 0x3A, 0xA6, 0xE9, 0x42, 0x4C, 0xF4,
    0xC6, 0x7E, 0x5E, 0x62, 0x76, 0xB5, 0x85, 0xE4, 0x45, 0xC2, 0x51, 0x6D, 0x6D, 0x35, 0xE1, 0x4F,
    0x37, 0x14, 0x5F, 0xF2, 0x6D, 0x0A, 0x2B, 0x30, 0x1B, 0x43, 0x3A, 0xCD, 0xB3, 0x19, 0x95, 0xEF,
    0xDD, 0x04, 0x34, 0x8E, 0x79, 0x08, 0x4A, 0x51, 0x22, 0x9B, 0x13, 0x3B, 0xA6, 0xBE, 0x0B, 0x02,
    0x74, 0xCC, 0x67, 0x8A, 0x08, 0x4E, 0x02, 0x29, 0xD1, 0x1C, 0xDC, 0x80, 0x8B, 0x62, 0xC6, 0xC4,
    0x34, 0xC2, 0x68, 0x21, 0xA2, 0xDA, 0x0F, 0xC9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

UCHAR
g_OakleyGroup1_PrimeN[96] =     /* 2^768 - 2^704 - 1 + 2 ^64 * {[2^638 pi] + 149686} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x3A, 0x36, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

ULONG
g_OakleyGroup1_Generator = 2;

/*
 * SECOND OAKLEY DEFAULT GROUP - MODP
 */
UCHAR
g_OakleyGroup2_PrimeH[128] =    /* 2^1024 - 2^960 - 1 + 2 ^64 * {[2^894 pi] + 129093} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x53, 0xE6, 0xEC, 0x51, 0x66, 0x28, 0x49,
    0xE6, 0x1F, 0x4B, 0x7C, 0x11, 0x24, 0x9F, 0xAE, 0xA5, 0x9F, 0x89, 0x5A, 0xFB, 0x6B, 0x38, 0xEE,
    0xED, 0xB7, 0x06, 0xF4, 0xB6, 0x5C, 0xFF, 0x0B, 0x6B, 0xED, 0x37, 0xA6, 0xE9, 0x42, 0x4C, 0xF4,
    0xC6, 0x7E, 0x5E, 0x62, 0x76, 0xB5, 0x85, 0xE4, 0x45, 0xC2, 0x51, 0x6D, 0x6D, 0x35, 0xE1, 0x4F,
    0x37, 0x14, 0x5F, 0xF2, 0x6D, 0x0A, 0x2B, 0x30, 0x1B, 0x43, 0x3A, 0xCD, 0xB3, 0x19, 0x95, 0xEF,
    0xDD, 0x04, 0x34, 0x8E, 0x79, 0x08, 0x4A, 0x51, 0x22, 0x9B, 0x13, 0x3B, 0xA6, 0xBE, 0x0B, 0x02,
    0x74, 0xCC, 0x67, 0x8A, 0x08, 0x4E, 0x02, 0x29, 0xD1, 0x1C, 0xDC, 0x80, 0x8B, 0x62, 0xC6, 0xC4,
    0x34, 0xC2, 0x68, 0x21, 0xA2, 0xDA, 0x0F, 0xC9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

UCHAR
g_OakleyGroup2_PrimeN[128] =    /* 2^1024 - 2^960 - 1 + 2 ^64 * {[2^894 pi] + 129093} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
    0x49, 0x28, 0x66, 0x51, 0xEC, 0xE6, 0x53, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

ULONG
g_OakleyGroup2_Generator = 2;

/*
 * THIRD OAKLEY DEFAULT GROUP - EC2N
 */
ULONG
g_OakleyGroup3_FieldSize = 155;

UCHAR
g_OakleyGroup3_Prime[20] =    /* irreducible polynomial: u^155 + u^62 + 1; equation: y^2 + xy = x^3 + ax^2 + b */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08
};

ULONG
g_OakleyGroup3_Generator1 = 0x0000007B;

ULONG
g_OakleyGroup3_CurveA = 0x00000000;

ULONG
g_OakleyGroup3_CurveB = 0x0007338F;

UCHAR
g_OakleyGroup3_Order[20] =
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0x44, 0xF9, 0xAE, 0x93, 0x71, 0x53, 0x98, 0x56, 0xDB, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08
};

/*
 * FOURTH OAKLEY DEFAULT GROUP - EC2N
 */
ULONG
g_OakleyGroup4_FieldSize = 185;

UCHAR
g_OakleyGroup4_Prime[24] =    /* irreducible polynomial: u^185 + u^69 + 1; equation: y^2 + xy = x^3 + ax^2 + b */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

ULONG
g_OakleyGroup4_Generator1 = 0x00000018;

ULONG
g_OakleyGroup4_CurveA = 0x00000000;

ULONG
g_OakleyGroup4_CurveB = 0x00001EE9;

UCHAR
g_OakleyGroup4_Order[24] =
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xBC, 0x4E, 0xF9, 0x75, 0x41, 0x48, 0x3E, 0xB7, 0x89, 0xF8, 0xF2, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01
};
