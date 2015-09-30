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

    module:	ansc_packet_binary_ike.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the constants and data structures
        that resemble the binary format of IPsec DOI of ISAKMP --
        RFC 2407, IKE -- RFC 2409 and other RFCs/drafts.

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


#ifndef  _ANSC_PACKET_BINARY_IKE_
#define  _ANSC_PACKET_BINARY_IKE_


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
             ASSIGNED NUMBERS FROM IPSEC DOI
***********************************************************/

/*
 * the following definitions list the Assigned Numbers for the IPSec DOI:
 * situation identifiers, protocol identifiers, transform identifiers, AH, ESP, and IPComp transform identifiers,
 * security association attribute type values, labled domain identifiers, ID payload type values and notify message
 * type values.
 */

/*
 *  ISAKMP situation identifiers
 */
#define  ISAKMP_SIT_IDENTITY_ONLY                   0x01
#define  ISAKMP_SIT_SECRECY                         0x02
#define  ISAKMP_SIT_INTEGRITY                       0x04

/*
 *  ISAKMP protocol identifiers
 */
#define  ISAKMP_PROTO_RESERVED                      0
#define  ISAKMP_PROTO_ISAKMP                        1
#define  ISAKMP_PROTO_IPSEC_AH                      2
#define  ISAKMP_PROTO_IPSEC_ESP                     3
#define  ISAKMP_PROTO_IPCOMP                        4

/*
 *  ISAKMP transform types
 */
#define  ISAKMP_TRANSFORM_RESERVED                  0
#define  ISAKMP_TRANSFORM_KEY_IKE                   1

/*
 *  AH transform definitions
 */
#define  IPSEC_AH_TRANSFORM_MD5                     2
#define  IPSEC_AH_TRANSFORM_SHA                     3
#define  IPSEC_AH_TRANSFORM_DES                     4

/*
 *  ESP transform definitions
 */
#define  IPSEC_ESP_TRANSFORM_DES_IV64               1
#define  IPSEC_ESP_TRANSFORM_DES                    2
#define  IPSEC_ESP_TRANSFORM_3DES                   3
#define  IPSEC_ESP_TRANSFORM_RC5                    4
#define  IPSEC_ESP_TRANSFORM_IDEA                   5
#define  IPSEC_ESP_TRANSFORM_CAST                   6
#define  IPSEC_ESP_TRANSFORM_BLOWFISH               7
#define  IPSEC_ESP_TRANSFORM_3IDEA                  8
#define  IPSEC_ESP_TRANSFORM_DES_IV32               9
#define  IPSEC_ESP_TRANSFORM_RC4                    10
#define  IPSEC_ESP_TRANSFORM_NULL                   11
#define  IPSEC_ESP_TRANSFORM_AES                    12

/*
 *  IPCOMP transform definitions
 */
#define  IPSEC_IPCOMP_TRANSFORM_OUT                 1
#define  IPSEC_IPCOMP_TRANSFORM_DEFLATE             2
#define  IPSEC_IPCOMP_TRANSFORM_LZS                 3
#define  IPSEC_IPCOMP_TRANSFORM_V42BIS              4

/*
 *  IPSec SA attribute types
 */
#define  IPSEC_ATTRIBUTE_LIFE_TYPE                  (1  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_LIFE_DURATION              2
#define  IPSEC_ATTRIBUTE_GROUP_DESCRIPTION          (3  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_ENCAPSULATION_MODE         (4  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_AUTHENTICATION_ALGORITHM   (5  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_KEY_LENGTH                 (6  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_KEY_ROUNDS                 (7  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_COMPRESS_DICTIONARY_SIZE   (8  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IPSEC_ATTRIBUTE_COMPRESS_PRIVATE_ALGORITHM 9
/*
 *  AL proprietary encapsulation mode attribute -- 0xF001
 */
#define  IPSEC_ATTRIBUTE_TUNNEL_TYPE                (28673 | ISAKMP_ATTRIBUTE_TV_FLAG)

/*
 *  SA lifetime types
 */
#define  IPSEC_SA_LIFETIME_TYPE_RESERVED            0
#define  IPSEC_SA_LIFETIME_SECONDS                  1
#define  IPSEC_SA_LIFETIME_KILOBYTES                2

/*
 *  Oakley group description, use IKE's
 */

/*
 *  encapsulation mode definitions
 */
#define  IPSEC_ENCAP_MODE_RESERVED                  0
#define  IPSEC_ENCAP_MODE_TUNNEL                    1
#define  IPSEC_ENCAP_MODE_TRANSPORT                 2
#define  IPSEC_ENCAP_MODE_UDP_TUNNEL                3           /* NAT-Traversal */
#define  IPSEC_ENCAP_MODE_UDP_TRANSPORT             4           /* NAT-Traversal */

/*
 *  AL proprietary encapsulation mode defintions
 */
#define  IPSEC_TUNNEL_TYPE_PUBLIC                   1
#define  IPSEC_TUNNEL_TYPE_PRIVATE                  2

/*
 *  authentication algorithm definitions for ESP ICV and AH
 */
#define  IPSEC_AUTH_ALG_NULL                        0
#define  IPSEC_AUTH_ALG_HMAC_MD5                    1
#define  IPSEC_AUTH_ALG_HMAC_SHA                    2
#define  IPSEC_AUTH_ALG_DES_MAC                     3
#define  IPSEC_AUTH_ALG_KPDK                        4


/*
 *  predefined ID types that could be used in IKE/ISAKMP authentication
 */
#define  ISAKMP_ID_RESERVED                         0
#define  ISAKMP_ID_IPV4_ADDR                        1
#define  ISAKMP_ID_FQDN                             2
#define  ISAKMP_ID_USER_FQDN                        3
#define  ISAKMP_ID_IPV4_ADDR_SUBNET                 4
#define  ISAKMP_ID_IPV6_ADDR                        5
#define  ISAKMP_ID_IPV6_ADDR_SUBNET                 6
#define  ISAKMP_ID_IPV4_ADDR_RANGE                  7
#define  ISAKMP_ID_IPV6_ADDR_RANGE                  8
#define  ISAKMP_ID_DER_ASN1_DN                      9
#define  ISAKMP_ID_DER_ASN1_GN                      10
#define  ISAKMP_ID_KEY_ID                           11


/***********************************************************
              ASSIGNED NUMBERS FROM IKE
***********************************************************/

/*
 * define the default transport and port for IKE
 */
#define  IKE_DEFAULT_TRANSPORT                      UDP_TRANSPORT
#define  IKE_DEFAULT_PORT                           500
#define  IKE_DEFAULT_PORT_NAT_TRAVERSAL             4500

/*
 *  IKE SA attribute types
 */
#define  IKE_ATTRIBUTE_ENCRYPTION_ALGORITHM         (1  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_HASH_ALGORITHM               (2  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_AUTHENTICATION_METHOD        (3  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_GROUP_DESCRIPTION            (4  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_GROUP_TYPE                   (5  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_GROUP_PRIME                  6
#define  IKE_ATTRIBUTE_GROUP_GENERATOR1             7
#define  IKE_ATTRIBUTE_GROUP_GENERATOR2             8
#define  IKE_ATTRIBUTE_GROUP_CURVEA                 9
#define  IKE_ATTRIBUTE_GROUP_CURVEB                 10
#define  IKE_ATTRIBUTE_LIFE_TYPE                    (11 | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_LIFE_DURATION                12
#define  IKE_ATTRIBUTE_PRF                          (13 | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_KEY_LENGTH                   (14 | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_FIELD_SIZE                   (15 | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_ATTRIBUTE_GROUP_ORDER                  16

#define  IKE_ATTRIBUTE_PRIVATE_START                16384
#define  IKE_ATTRIBUTE_PRIVATE_END                  32767

/*
 *  encryption algorithm definitions
 */
#define  IKE_ENCRYPTION_DES_CBC                     1
#define  IKE_ENCRYPTION_IDEA_CBC                    2
#define  IKE_ENCRYPTION_BLOWFISH_CBC                3
#define  IKE_ENCRYPTION_RC5_R16_B64_CBC             4
#define  IKE_ENCRYPTION_3DES_CBC                    5
#define  IKE_ENCRYPTION_CAST_CBC                    6
#define  IKE_ENCRYPTION_AES_CBC                     7

/*
 *  hash algorithm definitions
 */
#define  IKE_HASH_ALG_MD5                           1
#define  IKE_HASH_ALG_SHA                           2
#define  IKE_HASH_ALG_TIGER                         3

/*
 *  authentication method definitions
 */
#define  IKE_AUTH_PRESHARED_KEY                     1
#define  IKE_AUTH_DSS_SIGNATURE                     2
#define  IKE_AUTH_RSA_SIGNATURE                     3
#define  IKE_AUTH_RSA_ENCRYPTION                    4
#define  IKE_AUTH_REVISED_RSA_ENCRYPTION            5

/*
 *  Diffie-Hellman Oakley groups
 */
#define  IKE_DH_GROUP_NULL                          0           /* not defined in RFC, for programming convenience */
#define  IKE_DH_GROUP_1                             1           /* MODP_768  */
#define  IKE_DH_GROUP_2                             2           /* MODP_1024 */
#define  IKE_DH_GROUP_3                             3           /* EC2N_155  */
#define  IKE_DH_GROUP_4                             4           /* EC2N_185  */
#define  IKE_DH_GROUP_5                             5           /* MODP_1536 */
#define  IKE_DH_GROUP_MAX                           IKE_DH_GROUP_5

#define  IKE_DH_PRIVATE_GROUP_START                 32768
#define  IKE_DH_PRIVATE_GROUP_END                   65535

/*
 *  group type
 */
#define  IKE_DH_GROUP_TYPE_MODP                     1
#define  IKE_DH_GROUP_TYPE_ECP                      2
#define  IKE_DH_GROUP_TYPE_EC2N                     3

#define  IKE_DH_PRIVATE_GROUP_TYPE_START            65001
#define  IKE_DH_PRIVATE_GROUP_TYPE_END              65535

/*
 *  SA lifetime type
 */
#define  IKE_SA_LIFETIME_SECONDS                    1
#define  IKE_SA_LIFETIME_KILOBYTES                  2

/*
 *  psuedo-random function
 */
#define  IKE_PRF_RESERVED                           0


/***********************************************************
           ASSIGNED NUMBERS FROM IKE XAUTH
***********************************************************/

/*
 *  authentication types defined by XAUTH
 */
#define  IKE_XAUTH_TYPE_GENERIC                     0
#define  IKE_XAUTH_TYPE_RADIUS_CHAP                 1
#define  IKE_XAUTH_TYPE_OTP                         2           /* One time password       */
#define  IKE_XAUTH_TYPE_SKEY                        3           /* S/KEY one time password */
                                                                /*4-32767       Reserved for future use */
                                                                /*32768-65535   Reserved for private use*/
#define  IKE_XAUTH_TYPE_MAX_VALUE                   3

/*
 *  XAUTH status codes
 */
#define  IKE_XAUTH_STATUS_SUCCESS                   1
#define  IKE_XAUTH_STATUS_FAILURE                   0

/*
 *  XAUTH attributes
 */
#define  IKE_XAUTH_ATTRIBUTE_TYPE                   (16520  | ISAKMP_ATTRIBUTE_TV_FLAG)
#define  IKE_XAUTH_ATTRIBUTE_USER_NAME              16521
#define  IKE_XAUTH_ATTRIBUTE_USER_PASSWORD          16522
#define  IKE_XAUTH_ATTRIBUTE_PASSCODE               16523
#define  IKE_XAUTH_ATTRIBUTE_MESSAGE                16524
#define  IKE_XAUTH_ATTRIBUTE_CHALLENGE              16525
#define  IKE_XAUTH_ATTRIBUTE_DOMAIN                 16526
#define  IKE_XAUTH_ATTRIBUTE_STATUS                 (16527  | ISAKMP_ATTRIBUTE_TV_FLAG)


/***********************************************************
                      DATA STRUCTURES
***********************************************************/


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
