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

    module:	ansc_packet_binary_isakmp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the constants and data structures
        that resemble the binary format of the ISAKMP -- RFC 2408.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        03/18/03    initial revision

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_ISAKMP_
#define  _ANSC_PACKET_BINARY_ISAKMP_


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
                ASSIGNED NUMBERS FROM ISAKMP
***********************************************************/

/*
 *  DOI definitions
 */
#define  ISAKMP_DOI_RESERVED                        0
#define  ISAKMP_IPSEC_DOI                           1

/*
 *  ISAKMP exchange type
 */
#define  ISAKMP_EXCHANGE_BASE                       1
#define  ISAKMP_EXCHANGE_ID_PROTECTION              2
#define  ISAKMP_EXCHANGE_AUTHENTICATION_ONLY        3
#define  ISAKMP_EXCHANGE_AGGRESSIVE                 4
#define  ISAKMP_EXCHANGE_INFORMATIONAL              5
#define  ISAKMP_EXCHANGE_TRANSACTION                6
/*
 *  DOI Specific Use:   32  - 239
 *  Private Use:        240 - 255
 */
#define  ISAKMP_EXCHANGE_QUICK_MODE                 32
#define  ISAKMP_EXCHANGE_NEW_GROUP_MODE             33

/*
 *  ISAKMP payload type
 */
#define  ISAKMP_PAYLOAD_TYPE_SA                     1
#define  ISAKMP_PAYLOAD_TYPE_PROPOSAL               2
#define  ISAKMP_PAYLOAD_TYPE_TRANSFORM              3 
#define  ISAKMP_PAYLOAD_TYPE_KEY_EXCHANGE           4
#define  ISAKMP_PAYLOAD_TYPE_ID                     5
#define  ISAKMP_PAYLOAD_TYPE_CERTIFICATE            6
#define  ISAKMP_PAYLOAD_TYPE_CERTIFICATE_REQUEST    7
#define  ISAKMP_PAYLOAD_TYPE_HASH                   8
#define  ISAKMP_PAYLOAD_TYPE_SIGNATURE              9
#define  ISAKMP_PAYLOAD_TYPE_NONCE                  10
#define  ISAKMP_PAYLOAD_TYPE_NOTIFICATION           11
#define  ISAKMP_PAYLOAD_TYPE_DELETE                 12
#define  ISAKMP_PAYLOAD_TYPE_VENDOR_ID              13
#define  ISAKMP_PAYLOAD_TYPE_ATTRIBUTE              14
#define  ISAKMP_PAYLOAD_TYPE_NAT_D                  15
#define  ISAKMP_PAYLOAD_TYPE_NAT_OA                 16

/*
 *  AL proprietary payload type
 */
#define  ISAKMP_PAYLOAD_TYPE_TIU                    0x80
#define  ISAKMP_PAYLOAD_TYPE_TIU_ACKNOWLEDGEMENT    0x81
#define  ISAKMP_PAYLOAD_TYPE_TIE_REQUEST            0x82
#define  ISAKMP_PAYLOAD_TYPE_TOPOLOGY_CHANGED       0x83
#define  ISAKMP_PAYLOAD_TYPE_ENDOF_TIE              0x84

/*
 *  certificate encoding types
 */
#define  ISAKMP_ENCODING_PKCS7                      1
#define  ISAKMP_ENCODING_PGP                        2
#define  ISAKMP_ENCODING_DNS_SK                     3
#define  ISAKMP_ENCODING_X509_SIG                   4
#define  ISAKMP_ENCODING_X509_KE                    5
#define  ISAKMP_ENCODING_KERBEROS_TOKEN             6
#define  ISAKMP_ENCODING_CRL                        7
#define  ISAKMP_ENCODING_ARL                        8
#define  ISAKMP_ENCODING_SPKI                       9
#define  ISAKMP_ENCODING_X509_ATTRIBUTE             10

/*
 *  ISAKMP SA ATTRIBUTES
 */
#define  ISAKMP_ATTRIBUTE_TV_FLAG                   0x8000


/***********************************************************
            ASSIGNED NUMBERS FROM ISAKMP MODE-CFG
***********************************************************/

/*
 *  type definitions for ISAKMP attribute payload
 */
#define  ISAKMP_CFG_TYPE_RESERVED                   0
#define  ISAKMP_CFG_TYPE_REQUEST                    1
#define  ISAKMP_CFG_TYPE_REPLY                      2
#define  ISAKMP_CFG_TYPE_SET                        3
#define  ISAKMP_CFG_TYPE_ACK                        4
                                                                /* Reserved for Future Use    5-127   */
                                                                /* Reserved for Private Use   128-255 */
#define  ISAKMP_CFG_TYPE_MAX_VALUE                  4

/*
 *  data attribute type definitions
 */
#define  ISAKMP_CFG_ATTR_RESERVED                   0
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_ADDRESS       1           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_NETMASK       2           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_DNS           3           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_NBNS          4           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_INTERNAL_ADDRESS_EXPIRY    5           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_DHCP          6           /* Variable, 0 or 4 octets                      */
#define  ISAKMP_CFG_ATTR_APPLICATION_VERSION        7           /* Variable, 0 or more                          */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_ADDRESS       8           /* Variable, 0 or 16 octets                     */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_NETMASK       9           /* Variable, 0 or 16 octets                     */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_DNS           10          /* Variable, 0 or 16 octets                     */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_NBNS          11          /* Variable, 0 or 16 octets                     */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_DHCP          12          /* Variable, 0 or 16 octets                     */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP4_SUBNET        13          /* Variable, 0 or 8 octets                      */
#define  ISAKMP_CFG_ATTR_SUPPORTED_ATTRIBUTES       14          /* Variable, 0 or multiples of 2                */
#define  ISAKMP_CFG_ATTR_INTERNAL_IP6_SUBNET        15          /* Variable, 0 or 17 octets                     */
                                                                /* Reserved for future use    16-16383          */
                                                                /* Reserved for private use   16384-32767       */
#define  ISAKMP_CFG_ATTR_MAX_VALUE                  15          /* mamximum value of current defined attributes */


/***********************************************************
                      DATA STRUCTURES
***********************************************************/



#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
