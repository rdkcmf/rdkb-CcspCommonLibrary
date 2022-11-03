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

    module:	cw_vpn_sakm.h

        For Virtual Private Network Driver,
        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    functional description:

        This file defines the structures and prototypes used in
        IPSec AH/ESP engine.

    ---------------------------------------------------------------

    environment:

    ---------------------------------------------------------------

    author:

        Xuchen Yang

    ---------------------------------------------------------------

    revision history:

        *   01/29/98    initial revision
        *   05/03/01    vpn porting to vxWorks

**********************************************************************/


#ifndef  _CW_VPN_SAKM_
#define  _CW_VPN_SAKM_

#include "crypt_spi.h"

/***********************************************************
                COMMONSENSE OF CRYPTOGRAPHY
***********************************************************/

/*
 * some compression algorithms have different performance controled by different parameters
 */
#define	 LZS_PERFORMANCE_MODE_0                     0x0000
#define  LZS_PERFORMANCE_MODE_1                     0x0008
#define  LZS_PERFORMANCE_MODE_2                     0x0010

/*
 * the key length for HMAC
 * note: actually it's the block size of different hash function / bytes
 */
#define  MD5_BLOCK_SIZE                             64
#define  SHA1_BLOCK_SIZE                            64
#define  TIGER_BLOCK_SIZE                           64

/*
 * the output length for hash functions / bytes
 */
#define  MD5_OUTPUT_SIZE                            16
#define  SHA1_OUTPUT_SIZE                           20
#define  TIGER_OUTPUT_SIZE                          24
#define  DRAFT_OUTPUT_SIZE                          12          /* left-most 96 bits output */

/*
 * I don't know why the draft guys chose these two values as the padding unit, there must be some kind mathametics
 * consideration involved
 */
#define  HMAC_INNER_PADDING_UNIT                    0x36
#define  HMAC_OUTER_PADDING_UNIT                    0x5C

/*
 * size of the initialization vector and CRC block of encryption algorithm
 */
#define  DES_IV_SIZE                                8           /* should be the same as the block size */
#define  DES_BLOCK_SIZE                             8
#define  DES_ENCRYPTION_KEY_LENGTH                  8
#define  DES_ROUND_NUMBER                           16

#define  IDEA_IV_SIZE                               8
#define  IDEA_BLOCK_SIZE                            8
#define  IDEA_ENCRYPTION_KEY_LENGTH                 16

#define  BLOWFISH_IV_SIZE                           8
#define  BLOWFISH_BLOCK_SIZE                        8
#define  BLOWFISH_ENCRYPTION_KEY_LENGTH             56          /* default value is 16 bytes */

#define  RC5_IV_SIZE                                8
#define  RC5_BLOCK_SIZE                             8           /* RC5 has variable block size, 64 bits for RC5_R16_B64 */
#define  RC5_ENCRYPTION_KEY_LENGTH                  16          /* default value is 16 bytes */

#define  RC4_IV_SIZE                                0
#define  RC4_BLOCK_SIZE                             16

#define  CAST_IV_SIZE                               8
#define  CAST_BLOCK_SIZE                            8
#define  CAST_ENCRYPTION_KEY_LENGTH                 16          /* defalut value is 16 bytes */

#define  AES_IV_SIZE                                16          /* should be the same as the block size */
#define  AES_BLOCK_SIZE                             16
#define  AES_128_ENCRYPTION_KEY_LENGTH              16
#define  AES_192_ENCRYPTION_KEY_LENGTH              24
#define  AES_256_ENCRYPTION_KEY_LENGTH              32
#define  AES_128_ROUND_NUMBER                       10
#define  AES_192_ROUND_NUMBER                       12
#define  AES_256_ROUND_NUMBER                       14

/*
 * some encryption and decryption algorithm implementions share the same routine, so we have to pass them
 * different flag to indicate encryption or decryption
 */
#define  ENCRYPTION_MODE                            1
#define  DECRYPTION_MODE                            0

/*
 * these masks are used in the random number generator
 */
#define  GREATER                                    0x00000001
#define  LESS                                       0x00000002
#define  BETWEEN                                    0x00000004

/*
 * length of different signatures
 */
#define  DSS_SIGNATURE_LENGTH                       40          /* encoded as r followed by s */


/***********************************************************
   DEFINITION OF MESSAGES EXCHANGED BETWEEN SAKM AND CERT
***********************************************************/

/*
 * we define four SPD scales: small, medium, big and huge
 */
typedef  enum
_SPD_SCALE
{
    SMALL_SCALE     = 0,
    MEDIUM_SCALE,
    BIG_SCALE,
    HUGE_SCALE
}
SPD_SCALE,  *PSPD_SCALE;

/*
 * IPSEC DOI specified situation
 */
typedef  enum
_ISAKMP_DOI
{
    DOI_RESERVED = 0,
    IPSEC_DOI
}
ISAKMP_DOI,  *PISAKMP_DOI;

#define  SIT_IDENTITY_ONLY                          0x00000001L
#define  SIT_SECRECY                                0x00000002L
#define  SIT_INTEGRITY                              0x00000004L


/*
 * the following definitions list the Assigned Numbers for the IPSec DOI:
 * situation identifiers, protocol identifiers, transform identifiers, AH, ESP, and IPComp transform identifiers,
 * security association attribute type values, labled domain identifiers, ID payload type values and notify message
 * type values.
 */
#define  PROTO_ISAKMP                               1
#define  PROTO_IPSEC_AH                             2
#define  PROTO_IPSEC_ESP                            3
#define  PROTO_IPCOMP                               4

typedef  enum
_ISAKMP_TRANSFORM
{
    KEY_IKE     = 1
}
ISAKMP_TRANSFORM,  *PISAKMP_TRANSFORM;

typedef  enum
_AH_TRANSFORM
{
    AH_MD5 = 2,
    AH_SHA,
    AH_DES
}
AH_TRANSFORM,  *PAH_TRANSFORM;

typedef  enum
_ESP_TRANSFORM
{
    ESP_DES_IV64 = 1,
    ESP_DES,
    ESP_3DES,
    ESP_RC5,
    ESP_IDEA,
    ESP_CAST,
    ESP_BLOWFISH,
    ESP_3IDEA,
    ESP_DES_IV32,
    ESP_RC4,
    ESP_NULL,
    ESP_AES
}
ESP_TRANSFORM,  *PESP_TRANSFORM;

#define  ESP_MAX                                    ESP_AES

typedef  enum
_IPCOMP_TRANSFORM
{
    IPCOMP_RESERVED = 0,
    IPCOMP_OUT,
    IPCOMP_DEFLATE,
    IPCOMP_LZS,
    IPCOMP_V42BIS
}
IPCOMP_TRANSFORM,  *PIPCOMP_TRANSFORM;

typedef  enum
_LIFE_TYPE
{
    LIFE_RESERVED = 0,
    SECONDS,
    KILOBYTES
}
LIFE_TYPE,  *PLIFE_TYPE;

typedef  enum
_ENCAPSULATION_MODE
{
    MODE_RESERVED = 0,
    TUNNEL,
    TRANSPORT,
    ENCAP_MODE_UDP_TUNNEL,
    ENCAP_MODE_UDP_TRANSPORT,
    ENCAP_MODE_MAX_VALUE                            /* not in RFC, for programming convenience only */
}
ENCAPSULATION_MODE,  *PENCAPSULATION_MODE;

#define  IKE_ENCAP_MODE_UDP_TUNNEL_PRIVATE          61443
#define  IKE_ENCAP_MODE_UDP_TRANSPORT_PRIVATE       61444


typedef  enum
_TUNNEL_TYPE
{
    TUNNEL_TYPE_RESERVED = 0,
    PUBLIC_TUNNEL,
    PRIVATE_TUNNEL
}
TUNNEL_TYPE,  *PTUNNEL_TYPE;

typedef  enum
_AUTHENTICATION_ALGORITHM
{
    AUTHENTICATION_RESERVED = 0,
    HMAC_MD5,
    HMAC_SHA,
    DES_MAC,
    KPDK
}
AUTHENTICATION_ALGORITHM,  *PAUTHENTICATION_ALGORITHM;

/*
 * we use this message to retrieve the displayable information from a certain certificate, which includes issuer's name,
 * subject name and expiration date; we might extend this message later to hold more information
 */
#define  MAXIMUM_COUNTRY_NAME_SIZE                  16
#define  MAXIMUM_ORGANIZATION_NAME_SIZE             64
#define  MAXIMUM_OU_NAME_SIZE                       64
#define  MAXIMUM_COMMON_NAME_SIZE                   32
#define  MAXIMUM_ISSUER_NAME_SIZE                   64
#define  MAXIMUM_SUBJECT_NAME_SIZE                  32
/* CID 277614 fix */
#define  MAXIMUM_SUBJECT_ALTNAME_SIZE               256
#define  MAXIMUM_SUBJECT_ALTNAME_IP_SIZE            16



typedef  enum
_AL_INTERNAL_PKI_KEY_TYPE
{
    KEY_RESERVED = 0,
    RSA_KEY,
    DSA_KEY,
    DH_KEY
}
AL_INTERNAL_PKI_KEY_TYPE,  *PAL_INTERNAL_PKI_KEY_TYPE;

typedef  struct
_AL_INTERNAL_PKI_INFO
{
    SINGLE_LINK_ENTRY           Linkage;
    BOOLEAN                     bRemoved;                                               /* specify if this entry has already been remove */
    AL_HANDLE                   hCertHandle;                                            /* must not be NULL if the content is NULL */
    PVOID                       CertEncodingContent;                                    /* must not be NULL if the handle is NULL */
    ULONG                       ulContentSize;                                          /* size of the encoding content buffer */
    PVOID                       PrivateKeyEncoding;                                     /* DER encoding of subject's private key */
    ULONG                       ulKeySize;                                              /* size of key encoding */
    BOOLEAN                     bSelfSigned;                                            /* is this root CA's certificate? */
    AL_INTERNAL_PKI_KEY_TYPE    KeyType;                                                /* specify the public key type */
    char                        IssuerCountryName[MAXIMUM_COUNTRY_NAME_SIZE];           /* holds issuer's country name */
    char                        IssuerOrganizationName[MAXIMUM_ORGANIZATION_NAME_SIZE]; /* holds issuer's organization name */
    char                        IssuerOUName[MAXIMUM_OU_NAME_SIZE];                     /* holds issuer's organization unit name */
    char                        IssuerCommonName[MAXIMUM_COMMON_NAME_SIZE];             /* holds issuer's common name */
    char                        SubjectCountryName[MAXIMUM_COUNTRY_NAME_SIZE];          /* holds subject's country name */
    char                        SubjectOrganizationName[MAXIMUM_ORGANIZATION_NAME_SIZE];/* holds subject's organization name */
    char                        SubjectOUName[MAXIMUM_OU_NAME_SIZE];                    /* holds subject's organization unit name */
    char                        SubjectCommonName[MAXIMUM_COMMON_NAME_SIZE];            /* holds subject's common name */
    char                        SubjectAltNameIP[MAXIMUM_SUBJECT_ALTNAME_IP_SIZE];      /* holds subject's Alternative name when IP is used as alternative*/
    char                        SubjectAltNameDNS[MAXIMUM_SUBJECT_ALTNAME_SIZE];        /* holds subject's Alternative name when Email is used as alternative*/
    char                        SubjectAltNameEmail[MAXIMUM_SUBJECT_ALTNAME_SIZE];      /* holds subject's Alternative name when DNS is used as alternative*/
    
    /*
     *  certificate valid from/expiration time
     */
    ANSC_UNIVERSAL_TIME         NotBeforeTime;
    ANSC_UNIVERSAL_TIME         ExpirationTime;
    /*
     *  the following two fields are used by IPsec code internally:
     *      holds the CRL (binary format) if this is CA certificate.
     */
    PUCHAR                      pCrl;
    ULONG                       ulCrlSize;
}
AL_INTERNAL_PKI_INFO,  *PAL_INTERNAL_PKI_INFO;

#define  ACCESS_INTERNAL_PKI_INFO(p)                \
         CONTAINING_RECORD(p, AL_INTERNAL_PKI_INFO, Linkage)


/***********************************************************
           ASHLEY LAURENT CERTIFICATE FILE FORMAT
***********************************************************/

/*
 * for root CA's certifcate, it must be self-signed and it's possible it doesn't include CA's private key; for subject's
 * certificate, it must not be self-signed and it must include subject's private key
 */
typedef  struct
_AL_CERT_FILE_HEADER
{
    USHORT          MajorVersion;                   /* for this version, it must be 0 */
    USHORT          MinorVersion;                   /* for this version, it must be 8 */
    char            Mark[2];                        /* must be "AL" */
    UCHAR           CertType;                       /* 0 for subject's certificate; 1 for CA's certificate */
    UCHAR           CAType;                         /* 0 for root CA; 1 for everything else */
    ULONG           CertEncodingOffset;             /* zero based offset address of the certificate */
    ULONG           CertEncodingSize;               /* size of the certificate encoding */
    ULONG           PrivateKeyEncodingOffset;       /* zero based offset address of the private key encoding, be 0 if no private key */
    ULONG           PrivateKeyEncodingSize;         /* size of the private key encoding */
}
AL_CERT_FILE_HEADER,  *PAL_CERT_FILE_HEADER;


/***********************************************************
        CERTIFICATE ENCODING TYPES DEFINED IN ISAKMP
***********************************************************/

#define  PKCS7_ENCODING                             0x01
#define  PGP_ENCODING                               0x02
#define  DNS_SK_ENCODING                            0x03
#define  X509_SIG_ENCODING                          0x04
#define  X509_KE_ENCODING                           0x05
#define  KERBEROS_TOKEN_ENCODING                    0x06
#define  CRL_ENCODING                               0x07
#define  ARL_ENCODING                               0x08
#define  SPKI_ENCODING                              0x09
#define  X509_ATTRIBUTE_ENCODING                    0x0A


/***********************************************************
                  IKE/ISAKMP SA ATTRIBUTES
***********************************************************/

#define  ATTRIBUTE_TV_FLAG                          0x8000

/*
 * IKE SA attribute types, 0x8XXX means fixed size of 2 octets.
 */
#define  IKE_ENCRYPTION_ALGORITHM_ATTRIBUTE         0x8001
#define  IKE_HASH_ALGORITHM_ATTRIBUTE               0x8002
#define  IKE_AUTHENTICATION_METHOD_ATTRIBUTE        0x8003
#define  IKE_GROUP_DESCRIPTION_ATTRIBUTE            0x8004
#define  IKE_GROUP_TYPE_ATTRIBUTE                   0x8005
#define  IKE_GROUP_PRIME_ATTRIBUTE                  0x0006
#define  IKE_GROUP_GENERATOR1_ATTRIBUTE             0x0007
#define  IKE_GROUP_GENERATOR2_ATTRIBUTE             0x0008
#define  IKE_GROUP_CURVEA_ATTRIBUTE                 0x0009
#define  IKE_GROUP_CURVEB_ATTRIBUTE                 0x000A
#define  IKE_LIFE_TYPE_ATTRIBUTE                    0x800B
#define  IKE_LIFE_DURATION_ATTRIBUTE                0x000C
#define  IKE_PRF_ATTRIBUTE                          0x800D
#define  IKE_KEY_LENGTH_ATTRIBUTE                   0x800E
#define  IKE_FIELD_SIZE_ATTRIBUTE                   0x800F
#define  IKE_GROUP_ORDER_ATTRIBUTE                  0x0010

/*
 *  this defines the negotiation mode of phase 1
 */
typedef  enum
_IKE_P1_NEG_MODE
{
    IKE_P1_NEG_MODE_AUTO            = 0,            /* IKE determines the mode automatically */
    IKE_P1_NEG_MODE_MAIN,
    IKE_P1_NEG_MODE_AGGRESSIVE
}
IKE_P1_NEG_MODE, *PIKE_P1_NEG_MODE;

/*
 * the following data structures are the prototype implementation of the IKE SA attributes addressed in the ISAKMP
 * IKE protocol draft, most of them has fixed size - 2 octets, but for convience we use regular ENUM type to define
 * them (which should be 4 octets), conversion must be applied when constructing payload.
 */
typedef  enum
_IKE_ENCRYPTION_ALGORITHM
{
    DES_CBC             = 1,
    IDEA_CBC,
    BLOWFISH_CBC,
    RC5_R16_B64_CBC,
    TRIPLEDES_CBC,
    CAST_CBC,
    AES_CBC
}
IKE_ENCRYPTION_ALGORITHM,  *PIKE_ENCRYPTION_ALGORITHM;

typedef  enum
_IKE_HASH_ALGORITHM
{
    MD5_HASH            = 1,
    SHA_HASH,
    TIGER_HASH,
    SHA2_256,
    SHA2_384,
    SHA2_512
}
IKE_HASH_ALGORITHM,  *PIKE_HASH_ALGORITHM;

typedef  enum
_IKE_AUTHENTICATION_METHOD
{
    PRESHARED_KEY       = 1,
    DSS_SIGNATURE,
    RSA_SIGNATURE,
    RSA_ENCRYPTION,
    REVISED_RSA_ENCRYPTION
}
IKE_AUTHENTICATION_METHOD,  *PIKE_AUTHENTICATION_METHOD;

#define  MODP_768                                   1
#define  MODP_1024                                  2
#define  EC2N_155                                   3
#define  EC2N_185                                   4
#define  MODP_1536                                  5
#define  MODP_2048                                  14
#define  MODP_3072                                  15
#define  MODP_4096                                  16
#define  MODP_6144                                  17
#define  MODP_8192                                  18
#define  IKE_MAX_GROUP_DESCRIPTION                  MODP_1536

typedef  enum
_IKE_GROUP_TYPE
{
    MODP                = 1,
    ECP,
    EC2N
}
IKE_GROUP_TYPE,  *PIKE_GROUP_TYPE;

typedef  enum
_IKE_PRF
{
    PRF_RESERVED    = 0
}
IKE_PRF,  *PIKE_PRF;

#define  PRIVATE_GROUP_DESCRIPTION_START            0x8000
#define  PRIVATE_GROUP_DESCRIPTION_END              0xFFFF

#define  MY_ID                                      0x00000000
#define  ENDPOINT_ID                                0x00000001
#define  CLIENT_ID                                  0x00000002
#define  SOURCE_ID                                  0x00000003

/*
 * variable length data type used in IKE
 *
#define  MAXIMUM_PRIME_SIZE                         512
#define  MAXIMUM_INTEGER_SIZE                       256

typedef  struct
_IKE_PRIME
{
    ULONG       Length;
    UCHAR       Data[MAXIMUM_PRIME_SIZE];
}
IKE_PRIME,  *PIKE_PRIME;
 */
typedef  AL_CRYPTO_PRIME            IKE_PRIME,  *PIKE_PRIME;

/*
typedef  struct
_IKE_INTEGER
{
    ULONG       Length;
    union
    {
        ULONG   ulData;
        UCHAR   ucData[MAXIMUM_INTEGER_SIZE];
    }Data;
}
IKE_INTEGER,  *PIKE_INTEGER;
 */
typedef  AL_CRYPTO_INTEGER          IKE_INTEGER,  *PIKE_INTEGER;

/*
 * predefined ID types that could be used in IKE/ISAKMP authentication
 */
typedef  enum
_IKE_ID_TYPE
{
    ID_RESERVED         = 0,
    ID_IPV4_ADDR,
    ID_FQDN,
    ID_USER_FQDN,
    ID_IPV4_ADDR_SUBNET,
    ID_IPV6_ADDR,
    ID_IPV6_ADDR_SUBNET,
    ID_IPV4_ADDR_RANGE,
    ID_IPV6_ADDR_RANGE,
    ID_DER_ASN1_DN,
    ID_DER_ASN1_GN,
    ID_KEY_ID
}
IKE_ID_TYPE,  *PIKE_ID_TYPE;


/***********************************************************
             DEFINITION OF INTERNAL STRUCTURES
***********************************************************/
/*
 * define the structure of keys that we will use in the encryption and authentication algorithms
 */
/*
typedef  struct
_KEY
{
    ULONG                           KeyNumber;
    ULONG                           RoundNumber;
    ULONG                           KeyLength;
    UCHAR                           KeyValue[MAXIMUM_KEY_NUMBER][MAXIMUM_KEY_LENGTH];
}
KEY,  *PKEY;
 */
typedef  AL_CRYPTO_KEY              KEY,  *PKEY;

/*
 * define IV (initialization vector) structure
 */
/*
typedef  struct
_IV
{
    ULONG                           IVLength;
    UCHAR                           IVValue[MAXIMUM_IV_LENGTH];
}
IV,  *PIV;
 */
typedef  AL_CRYPTO_IV               IV,  *PIV;

/*
 * hold the result of hash
 */
typedef  struct
_HASH_RESULT
{
    ULONG                           HashLength;
    UCHAR                           HashValue[MAXIMUM_HASH_RESULT];
}
HASH_RESULT,  *PHASH_RESULT;


/*
 * we define our own public and private syntax here in C definition convention, we will provide some kind of conversion
 * mechanism to convert the ASN.1 syntax to our syntax... the definition of following structures is based on RSA PKCS
 * standards
 */
#if      0
typedef  struct
_AL_INTERNAL_RSA_PUBLIC_KEY
{
    IKE_PRIME       Modulus;                        /* n = pq */
    IKE_INTEGER     PublicExponent;                 /* e */
}
AL_INTERNAL_RSA_PUBLIC_KEY,  *PAL_INTERNAL_RSA_PUBLIC_KEY;
#endif

typedef  AL_CRYPTO_RSA_PUBLIC_KEY   AL_INTERNAL_RSA_PUBLIC_KEY,  *PAL_INTERNAL_RSA_PUBLIC_KEY;


#if      0
typedef  struct
_AL_INTERNAL_RSA_PRIVATE_KEY
{
    ULONG           Version;                        /* shall be 0 for the current version of PKCS #1 */
    IKE_PRIME       Modulus;                        /* n = pq */
    IKE_INTEGER     PublicExponent;                 /* e */
    IKE_INTEGER     PrivateExponent;                /* d */
    IKE_PRIME       Prime1;                         /* p */
    IKE_PRIME       Prime2;                         /* q */
    IKE_INTEGER     Exponent1;                      /* d mod (p - 1) */
    IKE_INTEGER     Exponent2;                      /* d mod (q - 1) */
    IKE_INTEGER     CoEfficient;                    /* (inverse of q) mod p */
}
AL_INTERNAL_RSA_PRIVATE_KEY,  *PAL_INTERNAL_RSA_PRIVATE_KEY;
#endif

typedef  AL_CRYPTO_RSA_PRIVATE_KEY  AL_INTERNAL_RSA_PRIVATE_KEY,  *PAL_INTERNAL_RSA_PRIVATE_KEY;


#if      0
typedef  struct
_AL_INTERNAL_DSA_PUBLIC_KEY
{
    IKE_INTEGER     ParamP;                         /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    IKE_INTEGER     ParamQ;                         /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    IKE_INTEGER     ParamG;                         /* h^(p - 1) mod p */
    IKE_INTEGER     ParamY;                         /* g^x mod p */
}
AL_INTERNAL_DSA_PUBLIC_KEY,  *PAL_INTERNAL_DSA_PUBLIC_KEY;
#endif

typedef  AL_CRYPTO_DSA_PUBLIC_KEY   AL_INTERNAL_DSA_PUBLIC_KEY,  *PAL_INTERNAL_DSA_PUBLIC_KEY;


#if      0
typedef  struct
_AL_INTERNAL_DSA_PRIVATE_KEY
{
    IKE_INTEGER     ParamP;                         /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    IKE_INTEGER     ParamQ;                         /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    IKE_INTEGER     ParamG;                         /* h^(p - 1) mod p */
    IKE_INTEGER     ParamY;                         /* g^x mod p */
    IKE_INTEGER     ParamX;                         /* a randomly or pseudorandomly generated integer with 0 < x < q */
}
AL_INTERNAL_DSA_PRIVATE_KEY,  *PAL_INTERNAL_DSA_PRIVATE_KEY;
#endif

typedef  AL_CRYPTO_DSA_PRIVATE_KEY  AL_INTERNAL_DSA_PRIVATE_KEY,  *PAL_INTERNAL_DSA_PRIVATE_KEY;


typedef  enum
_AL_INTERNAL_SIGNATURE_ALGORITHM
{
    PKI_RESERVED = 0,
    PKI_MD2_RSA,
    PKI_MD5_RSA,
    PKI_SHA1_RSA,
    PKI_SHA1_DSA
}
AL_INTERNAL_SIGNATURE_ALGORITHM,  *PAL_INTERNAL_SIGNATURE_ALGORITHM;

typedef  struct
_AL_INTERNAL_PUBLIC_KEY
{
    AL_INTERNAL_PKI_KEY_TYPE            KeyType;
    union
    {
        AL_INTERNAL_RSA_PUBLIC_KEY      RSAPublicKey;
        AL_INTERNAL_DSA_PUBLIC_KEY      DSAPublicKey;
    }Key;
}
AL_INTERNAL_PUBLIC_KEY,  *PAL_INTERNAL_PUBLIC_KEY;

typedef  struct
_AL_INTERNAL_PRIVATE_KEY
{
    AL_INTERNAL_PKI_KEY_TYPE            KeyType;
    union
    {
        AL_INTERNAL_RSA_PRIVATE_KEY     RSAPrivateKey;
        AL_INTERNAL_DSA_PRIVATE_KEY     DSAPrivateKey;
    }Key;
}
AL_INTERNAL_PRIVATE_KEY,  *PAL_INTERNAL_PRIVATE_KEY;


/***********************************************************
    INFORMATIONE EXCHANGED BETWEEN PKI DLL AND CERT DLL
***********************************************************/

/*
 * whenever PKI DLL needs to specify a new CA, it has to fill out the following structure and send it down to CERT DLL,
 * CERT DLL will generate a new self-signed certificate based on the information provided by PKI DLL
 */
#define  DIGITALSIGNATURE_KEY_USAGE                 0x00000001
#define  NONREPUDIATION_KEY_USAGE                   0x00000002
#define  KEYENCIPHERMENT_KEY_USAGE                  0x00000004
#define  DATAENCIPHERMENT_KEY_USAGE                 0x00000008
#define  KEYAGREEMENT_KEY_USAGE                     0x00000010
#define  KEYCERTSIGN_KEY_USAGE                      0x00000020
#define  CRLSIGN_KEY_USAGE                          0x00000040
#define  ENCIPHERONLY_KEY_USAGE                     0x00000080
#define  DECIPHERONLY_KEY_USAGE                     0x00000100

typedef  struct
_AL_PKI_SERVER_INFO
{
    AL_INTERNAL_SIGNATURE_ALGORITHM     AlgorithmType;              /* must be compatiable with key type */
    AL_INTERNAL_PKI_KEY_TYPE            KeyType;
    ULONG                               KeyLength;
    ULONG                               SerialNumber;
    ULONG                               PathLenConstraint;
    ULONG                               RequireExplicitPolicy;
    ULONG                               InhibitPolicyMapping;
    char*                               CountryName;
    char*                               OrganizationName;
    char*                               OUName;
    char*                               CommonName;
    char*                               StartTime;                  /* format as "199x0101000000" */
    char*                               EndTime;                    /* format as "199x0101000000" */
}
AL_PKI_SERVER_INFO,  *PAL_PKI_SERVER_INFO;

typedef  struct
_AL_PKI_CLIENT_INFO
{
    AL_INTERNAL_PKI_KEY_TYPE            KeyType;
    ULONG                               KeyLength;
    BOOLEAN                             bCA;
    ULONG                               KeyUsageMask;
    char*                               EmailAddress;               /* something like "xuechen@osgroup.com" */
    char*                               DNSName;                    /* something like "www.osgroup.com" */
    char*                               CountryName;
    char*                               OrganizationName;
    char*                               OUName;
    char*                               CommonName;
    char*                               StartTime;                  /* format as "199x0101000000" */
    char*                               EndTime;                    /* format as "199x0101000000" */
}
AL_PKI_CLIENT_INFO,  *PAL_PKI_CLIENT_INFO;


/***********************************************************
                      IPSEC SA ATTRIBUTES
***********************************************************/

/*
 * IPSec SA attribute types, 0x8XXX means fixed size of 2 octets.
 */
#define  IPSEC_LIFE_TYPE_ATTRIBUTE                  0x8001
#define  IPSEC_LIFE_DURATION_ATTRIBUTE              0x0002
#define  IPSEC_GROUP_DESCRIPTION_ATTRIBUTE          0x8003
#define  IPSEC_ENCAPSULATION_MODE_ATTRIBUTE         0x8004
#define  IPSEC_AUTHENTICATION_ALGORITHM_ATTRIBUTE   0x8005
#define  IPSEC_KEY_LENGTH_ATTRIBUTE                 0x8006
#define  IPSEC_KEY_ROUNDS_ATTRIBUTE                 0x8007
#define  IPSEC_COMPRESS_DICTIONARY_SIZE_ATTRIBUTE   0x8008
#define  IPSEC_COMPRESS_PRIVATE_ALGORITHM_ATTRIBUTE 0x0009

/*
 * private IPSec SA attribute
 */
#define  IPSEC_TUNNEL_TYPE_ATTRIBUTE                0xF001

/*
 * default life duration for phase 1 and phase 2
 */
#define  IKE_DEFAULT_LIFE_DURATION                  86400           /* 24 hours */
#define  IPSEC_DEFAULT_LIFE_DURATION                28800           /* 8 hours */

/*
 * vendor specific compression algorithm
 */
#define  MAXIMUM_VENDOR_DATA_SIZE                   16

typedef  struct
_COMPRESS_PRIVATE_ALGORITHM
{
    UCHAR   IEEECompanyID[3];
    UCHAR   CompressionSubtype;
    ULONG   VendorDataSize;
    UCHAR   VendorData[MAXIMUM_VENDOR_DATA_SIZE];
}
COMPRESS_PRIVATE_ALGORITHM,  *PCOMPRESS_PRIVATE_ALGORITHM;


/***********************************************************
         IKE/ISAKMP AND IPSEC DOI DEFINED ID TYPES
***********************************************************/

/*
 * the following structures are not defined in any drafts, they consist of our IKE context database for certain pair
 * of IKE endpoints. DLL is responsible for maintaining a context memory block for every IKE channel. There're a couple
 * things need to be addressed:
 *
 *  (1) Multiple active IKE channels can coexist between two peers at the same time. However, only one IKE SA can be
 *      established. In another word, there could be multiple active Quick Mode exchanges going on between two peers
 *      at the same time; but there must be only one active Main Mode / Aggressive Mode exchange at any time.
 *  (2) If a Quick Mode exchange has been started by whomever during the renegotiation of phase one with same guy, the
 *      Quick Mode exchange must abort since our implementation will be that marking the IKE SA as unestablished in the
 *      beginning of renegotiation.
 */
#define  MAXIMUM_ID_SIZE                            256         /* new GUI change */
#define  MAXIMUM_SHARED_KEY_SIZE                    64

#ifndef  IPV4_ADDRESS_SIZE
#define  IPV4_ADDRESS_SIZE                          4
#endif

#define  IPV6_ADDRESS_SIZE                          16

#ifndef  IP_ADDRESS_LENGTH
#define  IP_ADDRESS_LENGTH                          4
#endif

#define  PRESHARED_KEY_FLAG                         0x00000001
#define  CERTIFICATE_FLAG                           0x00000002

#define  MAXIMUM_ORGANIZATIONNAME_LENGTH            64
#define  MAXIMUM_ORGANIZATIONUNITNAME_LENGTH        64
#define  MAXIMUM_COMMONNAME_LENGTH                  32

typedef  struct
_AL_INTERNAL_CERTIFICATE
{
    SINGLE_LINK_ENTRY           Linkage;
    AL_HANDLE                   hCertContext;                       /* reserved by AL_CERT DLL */
    AL_INTERNAL_PKI_KEY_TYPE    KeyType;                            /* RSA / DSA / DH */
    AL_INTERNAL_PUBLIC_KEY      PublicKey;                          /* correponding public key extracted from the certificate */
    AL_INTERNAL_PRIVATE_KEY     PrivateKey;                         /* only exist in our own ID */
    AL_INTERNAL_PUBLIC_KEY      CAPublicKey;                        /* correspoind CA's public key used to verify the certificate */
}
AL_INTERNAL_CERTIFICATE,  *PAL_INTERNAL_CERTIFICATE;

typedef  struct
_IKE_ID_ATTRIBUTE
{
    ULONG                       AttributesMask;                     /* specify which attribute is valid */
    ULONG                       PreSharedKeySize;                   /* how many octets contained in the preshared key */
    UCHAR                       KeyData[MAXIMUM_SHARED_KEY_SIZE];   /* this field is endpoint dependent */
    AL_HANDLE                   hActiveInternalCert;                /* pointer to the current active certificate */
    AL_HANDLE                   hInternalCertificate;               /* single ID could have multiple certs */
}
IKE_ID_ATTRIBUTE,  *PIKE_ID_ATTRIBUTE;

typedef  struct
_IKE_COMMON_ID
{
    SINGLE_LINK_ENTRY       Linkage;
    IKE_ID_TYPE             IDType;                                 /* 1 octet */
    UCHAR                   ProtocolID;                             /* 1 octet */
    USHORT                  PortNumber;                             /* 2 octet */
    ULONG                   IDSize;                                 /* how many octets contained in this ID */
    UCHAR                   IDData[MAXIMUM_ID_SIZE];                /* the data content of ID */
    IKE_ID_ATTRIBUTE        Attribute;                              /* the corresponding certificate/preshared key */
}
IKE_COMMON_ID,  *PIKE_COMMON_ID;

#define  ACCESS_IKE_COMMON_ID(p)                    \
         CONTAINING_RECORD(p, IKE_COMMON_ID, Linkage)

#define  AL_SERVER_TYPE_MASK                        0x00000001
#define  AL_CLIENT_TYPE_MASK                        0x00000002

typedef  struct
_AL_ACCESS_ATTRIBUTE
{
    ULONG   HostTypeMask;
    ULONG   AssetDomainID;
    ULONG   AssetClassificationLevel;
    ULONG   AccessDomainID;
    ULONG   AccessPrivilegeLevel;
}
AL_ACCESS_ATTRIBUTE,  *PAL_ACCESS_ATTRIBUTE;


/***********************************************************
    IKE/ISAKMP NOTIFICATION MESSAGE BETWEEN IKE AND SAKM
***********************************************************/

/*
 * define the default transport and port for IKE and VCSP
 */
#define  DEFAULT_ISAKMP_TRANSPORT                   UDP_TRANSPORT
#define  DEFAULT_ISAKMP_PORT                        500

/*
 *  NAT-T
 */
#define  DEFAULT_IKE_NAT_T_PORT                     4500
#define  IPSEC_NAT_T_NON_ESP_MARKER                 0
#define  IPSEC_NAT_T_KEEP_ALIVE_MARKER              0xFF

/*
 * define the SPI size for different protocols
 */
#define  ISAKMP_COOKIE_SIZE                         8
#define  IPSEC_AH_SPI_SIZE                          4
#define  IPSEC_ESP_SPI_SIZE                         4
#define  IPCOMP_SPI_SIZE                            2

#define  MAXIMUM_SPI_SIZE                           16
#define  MAXIMUM_DATA_SIZE                          128

#define  PHASE1_NEGOTIATION_COMPLETED               0x00000001
#define  PHASE1_NEGOTIATION_ABORT                   0x00000002
#define  PHASE2_NEGOTIATION_COMPLETED               0x00000003
#define  PHASE2_NEGOTIATION_ABORT                   0x00000004
#define  PHASE1_CONTEXT_EXPIRED                     0x00000005
#define  PHASE2_CONTEXT_REMOVED                     0x00000006
#define  PHASE1_STARTED_BY_PEER                     0x00000007
#define  PHASE2_STARTED_BY_PEER                     0x00000008
#define  INITIAL_CONTACT_BY_PEER                    0x00000009
#define  IKE_ERROR_LOG                              0x0000000A
#define  RECOVER_CONTACT_BY_PEER                    0x0000000B
#define  PEER_DISCONNECT                            0x0000000C
/*
 *  RFC3706 -- TDPD
 */
#define  TDPD_R_U_THERE_BY_PEER                     0x0000000D
#define  TDPD_R_U_THERE_ACK_BY_PEER                 0x0000000E

#define  PHASE1_MM_MESSAGE1_PROCESSED               0x0000FF01
#define  PHASE1_MM_MESSAGE2_PROCESSED               0x0000FF02
#define  PHASE1_MM_MESSAGE3_PROCESSED               0x0000FF03
#define  PHASE1_MM_MESSAGE4_PROCESSED               0x0000FF04
#define  PHASE1_MM_MESSAGE5_PROCESSED               0x0000FF05
#define  PHASE1_MM_MESSAGE6_PROCESSED               0x0000FF06

#define  PHASE1_AM_MESSAGE1_PROCESSED               0x0000FF07
#define  PHASE1_AM_MESSAGE2_PROCESSED               0x0000FF08
#define  PHASE1_AM_MESSAGE3_PROCESSED               0x0000FF09

#define  PHASE2_QM_MESSAGE1_PROCESSED               0x0000FF0A
#define  PHASE2_QM_MESSAGE2_PROCESSED               0x0000FF0B
#define  PHASE2_QM_MESSAGE3_PROCESSED               0x0000FF0C

#define  REFRESH_TOPOLOGY_INFORMATION               0x00FF0001
#define  TOPOLOGY_INFORMATION_CHANGED               0x00FF0002

#define  MAXIMUM_IP_ADDRESS_SIZE                    16

typedef  struct
_IKE_LOG_CONTENT
{
    PIKE_COMMON_ID      MyID;
    PIKE_COMMON_ID      HerID;
    UCHAR               MyCookie[8];
    UCHAR               HerCookie[8];
}
IKE_LOG_CONTENT,  *PIKE_LOG_CONTENT;

typedef  struct
_IKE_NOTIFICATION
{
    UCHAR               ProtocolID;                         /* always be PROTO_ISAKMP */
    UCHAR               PhaseStage;                         /* phase 1 or phase 2 */
    ULONG               MessageType;                        /* as defined above */
    BOOLEAN             bInitiator;                         /* indicate if we're the initiator */
    AL_HANDLE           hUserContext;                       /* could be hUserPhase1Context / hUserPhase2Context */
    AL_HANDLE           hNotificationContext;               /* could be IKE endpoint context / IPSec endpoint context */
    UCHAR               ProposalNumber;                     /* only valid for negotiation completion notification */

    union                                                   /* notification specific fields */
    {
        ULONG           Spi;
        ULONG           SeqNo;
        struct
        {
            UCHAR       LocalCookie[ISAKMP_COOKIE_SIZE];
            UCHAR       PeerCookie[ISAKMP_COOKIE_SIZE];
        }
        P1;
    }
    NotifSpecifc;

    ULONG               TopologyUnitNumber;                 /* only valid for topology refresh notification */
    UCHAR               IPAddress[MAXIMUM_IP_ADDRESS_SIZE]; /* IP address of the remote negotiation peer */
    USHORT              ErrorLogType;                       /* only valid for IKE_ERROR_LOG message */
    IKE_LOG_CONTENT     LogContent;                         /* reserved for error log message only */
}
IKE_NOTIFICATION,  *PIKE_NOTIFICATION;

/*
 *  this structure encapsulates the properties of NAT-Traversal
 */
typedef  struct
_IKE_NAT_T_PROPERTY
{
    /*
     *  whether NAT-T is enabled?
     */
    BOOLEAN                         bUseNatT;
    /*
     *  the interval between sending NAT-keepalive packets, in seconds.
     */
    USHORT                          KeepAliveInterval;
}
IKE_NAT_T_PROPERTY, *PIKE_NAT_T_PROPERTY;

#define  IPSEC_NAT_T_DFT_KEEP_ALIVE_INTERVAL        10


typedef  struct
_IKE_NAT_T_STATE
{
    /*
     *  Port numbers are of concern since NAT-T could be in place.
     */
    UCHAR                           TransportType;
    USHORT                          LocalPortNumber;
    USHORT                          PeerPortNumber;

    /*
     *  Whether NAT-T is applied?
     */
    BOOLEAN                         bNatTApplied;
    /*
     *  Any side is behind a NAT gateway?
     */
    BOOLEAN                         bLocalOnNat;
    BOOLEAN                         bPeerOnNat;
}
IKE_NAT_T_STATE, *PIKE_NAT_T_STATE;


typedef  struct
_IKE_NAT_T_OA_STATE
{
    /*
     *  The peer's original IP address.
     */
    UCHAR                           PeerOrigAddr[MAXIMUM_IP_ADDRESS_SIZE];  /* 16 octects for IPv6, 4 octects for IPv4 */
    /*
     *  The public IP address of local end seen by the peer.
     */
    UCHAR                           LocalPubAddr[MAXIMUM_IP_ADDRESS_SIZE];  /* 16 octects for IPv6, 4 octects for IPv4 */
}
IKE_NAT_T_OA_STATE, *PIKE_NAT_T_OA_STATE;


/***********************************************************
        SECURITY POLICY REQUIREMENT IMPOSED ON AL_IKE
***********************************************************/

/*
 * following IKE requirement data structures are associated with certain SAKM policy entries
 */
typedef  struct
_SAKM_IPSEC_REQUIREMENT
{
    ULONG   RequirementMask;
    ULONG   AHTransformMask;
    ULONG   ESPTransformMask;
    ULONG   IPCOMPTransformMask;
    ULONG   AuthenticationAlgorithmMask;
    ULONG   LifeTypeMask;
    ULONG   DHGroupMask;
    ULONG   LifeDuration;
    ULONG   EncapsulationModeMask;
    ULONG   SANumber;                               /* not implemented */
    ULONG   TunnelTypeMask;
    ULONG   KeyLength;                              /* we decide the default key length */
    ULONG   Rounds;                                 /* we decide the default round number */
    ULONG   CompressDictionarySize;                 /* not implemented */
}
SAKM_IPSEC_REQUIREMENT,  *PSAKM_IPSEC_REQUIREMENT;

typedef  struct
_SAKM_IKE_REQUIREMENT
{
    ULONG   EncryptionAlgorithmMask;
    ULONG   HashAlgorithmMask;
    ULONG   AuthenticationMethodAllowedMask;
    ULONG   SignatureMethodMask;
    ULONG   PublicKeyMethodMask;
    ULONG   LifeTypeMask;
    ULONG   LifeDuration;
    ULONG   DHGroupMask;
    ULONG   GroupTypeMask;
}
SAKM_IKE_REQUIREMENT,  *PSAKM_IKE_REQUIREMENT;


/***********************************************************
DHCP TUNNEL INFORMATION EXCHANGED BETWEEN SERVER AND CLIENT
***********************************************************/

/*
 * this data structure includes the current tunnel type, private tunnel port number, dhcp address assignment mode ...
 */
#define  STANDARD_DTI_NOTIFICATION_FLAG             0x00000001

typedef  struct
_DYNAMIC_TUNNEL_INFO
{
    ULONG                           OperationFlags;                         /* control the behavior of dynamic tunnel information exchange */
    ULONG                           DefaultTunnelTypeMask;
    ULONG                           PrivateTunnelPortNumber;
    ULONG                           AddressMode;
    union
    {
        UCHAR                       INSIPAddress[IPV4_ADDRESS_SIZE];
        ULONG                       INSIPValue;
    }INSAddress;
    union
    {
        UCHAR                       INSSubetMask[IPV4_ADDRESS_SIZE];
        ULONG                       INSMaskValue;
    }INSMask;
    union
    {
        UCHAR                       NextHopIPAddress[IPV4_ADDRESS_SIZE];
        ULONG                       NextHopIPValue;
    }NextHop;
    union
    {
        UCHAR                       DNSServerIPAddress[IPV4_ADDRESS_SIZE];
        ULONG                       DNSServerIPValue;
    }DnsServer;
    union
    {
        UCHAR                       WINSServerIPAddress[IPV4_ADDRESS_SIZE];
        ULONG                       WINSServerIPValue;
    }WinsServer;
    SAKM_IKE_REQUIREMENT            IKERequirement;
}
DYNAMIC_TUNNEL_INFO,  *PDYNAMIC_TUNNEL_INFO;


/***********************************************************
            USER CALLBACK ROUTINES REQUIRED BY IKE
***********************************************************/

typedef  AL_STATUS
(*USER_CHECK_PHASE1_ID_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        PUCHAR                      pEndpointAddress,
        AL_HANDLE                   hNegotiationContext,
        BOOLEAN                     bAggressiveMode,
        PAL_HANDLE                  phUserPhase1Context
    );

typedef  AL_STATUS
(*USER_CHECK_PHASE2_ID_CALLBACK)
    (
        AL_HANDLE                   hSAKMContext,
        AL_HANDLE                   pUserPhase1Context,
        AL_HANDLE                   hNegotiationContext,
        PAL_HANDLE                  phUserPhase2Context
    );

typedef  AL_STATUS
(*USER_CHECK_PHASE1_ADDRESS_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        PUCHAR                      pEndpointAddress,
        PIKE_COMMON_ID              pEndpointID,
        AL_HANDLE                   hIKENegotiationContext
    );

typedef  AL_STATUS
(*USER_VERIFY_ENDPOINT_CERTIFICATE_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hEndpointCertContext,
        ULONG                       ulCertNumber,
        AL_INTERNAL_PKI_KEY_TYPE    KeyType,
        PIKE_COMMON_ID              pEndpointID,
        PAL_HANDLE                  phUserPhase1Context
    );

typedef  AL_STATUS
(*USER_REQUEST_CERTIFICATE_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   pUserPhase1Context,
        UCHAR                       ucCertEncoding,
        PVOID                       pCertificateRequirement,
        ULONG                       ulRequirementSize,
        AL_INTERNAL_PKI_KEY_TYPE    KeyType,
        PULONG                      pulCertNumber,
        PULONG                      pulCertSizeArray,
        PVOID*                      pCertArray,
        PIKE_COMMON_ID              pUserID
    );

typedef  AL_STATUS
(*USER_ACQUIRE_CERTIFICATE_UNIT_BYHANDLE_CALLBACK)
    (
        AL_HANDLE                   pCertHandle,
        PUCHAR                      pucCertEncoding,
        PVOID                       pCertUnit,
        PULONG                      pulCertUnitSize
    );

typedef  AL_STATUS
(*USER_ACQUIRE_CERTIFICATE_UNIT_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hUserCertContext,
        ULONG                       ulIndex,
        PUCHAR                      pucCertEncoding,
        PVOID                       pCertUnit,
        PULONG                      pulCertUnitSize
    );

typedef  AL_STATUS
(*USER_SEND_PACKET_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        PUCHAR                      pHerAddress,
        PUCHAR                      pMyAddress,
        UCHAR                       TransportType,
        USHORT                      HerPortNumber,
        USHORT                      MyPortNumber,
        PVOID                       pMessage,
        ULONG                       MessageSize
    );

typedef  AL_STATUS
(*USER_RECEIVE_NOTIFICATION_CALLBACK)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hNotification
    );

typedef  AL_STATUS
(*USER_PRECREATE_IKE_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase1Context,
        PULONG                      pIKEProposalNumber
    );

typedef  AL_STATUS
(*USER_CONSTRUCT_IKE_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase1Context,
        AL_HANDLE                   hNegotiationContext
    );

typedef  AL_STATUS
(*USER_PRECREATE_IPSEC_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase2Context,
        PULONG                      pIPSecProposalNumber
    );

typedef  AL_STATUS
(*USER_CONSTRUCT_IPSEC_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase2Context,
        AL_HANDLE                   hNegotiationContext
    );

typedef  AL_STATUS
(*USER_PRECHECK_PEER_IKE_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase1Context
    );

typedef  AL_STATUS
(*USER_CHECK_PEER_IKE_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase1Context,
        AL_HANDLE                   hNegotiationContext,
        ULONG                       ProposalNumber
    );

typedef  AL_STATUS
(*USER_PRECHECK_PEER_IPSEC_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase2Context
    );

typedef  AL_STATUS
(*USER_CHECK_PEER_IPSEC_PROPOSAL_CALLBACK)
    (
        AL_HANDLE                   hUserPhase2Context,
        AL_HANDLE                   hNegotiationContext,
        ULONG                       ProposalNumber
    );

typedef  AL_STATUS
(*USER_ACQUIRE_PHASE1_CONTEXT)
    (
        AL_HANDLE                   hUserContext,
        PUCHAR                      pHerAddress,
        PUCHAR                      pMyAddress,
        PAL_HANDLE                  phUserPhase1Context
    );

typedef  AL_STATUS
(*USER_ACQUIRE_TOPOLOGY_INFORMATION)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hUserPhase1Context,
        PUCHAR                      pLocalAddress,
        PULONG                      pulUnitNumber
    );

typedef  AL_STATUS
(*USER_ACQUIRE_TOPOLOGY_UNIT)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hUserPhase1Context,
        ULONG                       ulUnitIndex,
        PCHAR                       pUnitDescription,
        PULONG                      pulDescriptionSize,
        PSAKM_IPSEC_REQUIREMENT     pIPSecRequirement,
        PUCHAR                      pAddress,
        PUCHAR                      pMask,
        PIKE_COMMON_ID              pUnitID
    );

typedef  AL_STATUS
(*USER_SET_TUNNEL_INFORMATION)
    (
        AL_HANDLE                   hUserContext,
        AL_HANDLE                   hUserPhase1Context,
        PDYNAMIC_TUNNEL_INFO        pDynamicTunnelInfo
    );

/*
 * define the IKE user registration structure, user has to construct a registration structure and submit it to IKE DLL
 * for reviewing
 */
typedef  struct
_IKE_USER_INFO
{
    SPD_SCALE                                           Scale;                          /* mandatory */
    AL_HANDLE                                           hUserContext;                   /* passed by SAKM, opaque to IKE DLL */
    UCHAR                                               IPVersion;                      /* mandatory, can only be 0x4 or 0x6 */
    USER_CHECK_PHASE1_ID_CALLBACK                       CheckPhase1ID;                  /* mandatory */
    USER_CHECK_PHASE2_ID_CALLBACK                       CheckPhase2ID;                  /* mandatory */
    USER_CHECK_PHASE1_ADDRESS_CALLBACK                  CheckPhase1Address;             /* mandatory */
    USER_VERIFY_ENDPOINT_CERTIFICATE_CALLBACK           VerifyEndpointCertificate;      /* mandatory */
    USER_REQUEST_CERTIFICATE_CALLBACK                   RequestCertificate;             /* mandatory */
    USER_ACQUIRE_CERTIFICATE_UNIT_BYHANDLE_CALLBACK     AcquireCertificateUnitByHandle; /* mandatory */
    USER_ACQUIRE_CERTIFICATE_UNIT_CALLBACK              AcquireCertificateUnit;         /* mandatory */
    USER_SEND_PACKET_CALLBACK                           SendPacket;                     /* mandatory */
    USER_RECEIVE_NOTIFICATION_CALLBACK                  ReceiveNotification;            /* mandatory */
    USER_PRECREATE_IKE_PROPOSAL_CALLBACK                PreCreateIKEProposal;           /* optional, could be NULL */
    USER_CONSTRUCT_IKE_PROPOSAL_CALLBACK                ConstructIKEProposal;           /* mandatory */
    USER_PRECREATE_IPSEC_PROPOSAL_CALLBACK              PreCreateIPSecProposal;         /* optional, could be NULL */
    USER_CONSTRUCT_IPSEC_PROPOSAL_CALLBACK              ConstructIPSecProposal;         /* mandatory */
    USER_PRECHECK_PEER_IKE_PROPOSAL_CALLBACK            PreCheckPeerIKEProposal;        /* optional, could be NULL */
    USER_CHECK_PEER_IKE_PROPOSAL_CALLBACK               CheckPeerIKEProposal;           /* mandatory */
    USER_PRECHECK_PEER_IPSEC_PROPOSAL_CALLBACK          PreCheckPeerIPSecProposal;      /* optional, could be NULL */
    USER_CHECK_PEER_IPSEC_PROPOSAL_CALLBACK             CheckPeerIPSecProposal;         /* mandatory */
    USER_ACQUIRE_PHASE1_CONTEXT                         AcquirePhase1Context;           /* optional, could be NULL */
    USER_ACQUIRE_TOPOLOGY_INFORMATION                   AcquireTopologyInformation;     /* optional, could be NULL */
    USER_ACQUIRE_TOPOLOGY_UNIT                          AcquireTopologyUnit;            /* optional, could be NULL */
    USER_SET_TUNNEL_INFORMATION                         SetTunnelInformation;           /* optional, could be NULL */
}
IKE_USER_INFO,  *PIKE_USER_INFO;

/*
 * user interface will fill up this structure and pass it to IKE DLL during initialization
 */
typedef  struct
_IKE_USER_CHARACTERISTICS
{
    IKE_USER_INFO   UserInfo;                       /* hold all the user information */
    AL_HANDLE       hIKEContext;                    /* hold the context memory block for Cisco Systems IKE DLL */
}
IKE_USER_CHARACTERISTICS,  *PIKE_USER_CHARACTERISTICS;


/***********************************************************
               ERROR LOG TYPES DEFINED IN IKE
***********************************************************/

/*
 * ISAKMP notification error message type
 */
#define  INVALID_PAYLOAD_TYPE                       0x0001
#define  DOI_NOT_SUPPORTED                          0x0002
#define  SITUATION_NOT_SUPPORTED                    0x0003
#define  INVALID_COOKIE                             0x0004
#define  INVALID_MAJOR_VERSION                      0x0005
#define  INVALID_MINOR_VERSION                      0x0006
#define  INVALID_EXCHANGE_TYPE                      0x0007
#define  INVALID_FLAGS                              0x0008
#define  INVALID_MESSAGE_ID                         0x0009
#define  INVALID_PROTOCOL_ID                        0x000A
#define  INVALID_SPI                                0x000B
#define  INVALID_TRANSFORM_ID                       0x000C
#define  ATTRIBUTES_NOT_SUPPORTED                   0x000D
#define  NO_PROPOSAL_CHOSEN                         0x000E
#define  BAD_PROPOSAL_SYNTAX                        0x000F
#define  PAYLOAD_MALFORMED                          0x0010
#define  INVALID_KEY_INFORMATION                    0x0011
#define  INVALID_ID_INFORMATION                     0x0012
#define  INVALID_CERT_ENCODING                      0x0013
#define  INVALID_CERTIFICATE                        0x0014
#define  CERT_TYPE_UNSUPPORTED                      0x0015
#define  INVALID_CERT_AUTHORITY                     0x0016
#define  INVALID_HASH_INFORMATION                   0x0017
#define  AUTHENTICATION_FAILED                      0x0018
#define  INVALID_SIGNATURE                          0x0019
#define  ADDRESS_NOTIFICATION                       0x001A
#define  NOTIFY_SA_LIFETIME                         0x001B
#define  CERTIFICATE_UNAVAILABLE                    0x001C

#define  PROPOSAL_MODIFIED                          0x2001
#define  TOO_MANY_TRANSFORMS                        0x2002
#define  INVALID_LIFE_DURATION                      0x2003
#define  INVALID_PAYLOAD_LENGTH                     0x2004
#define  INVALID_NEXT_PAYLOAD                       0x2005
#define  INVALID_PROPOSAL_INDEX                     0x2006          /* she must keep the same proposal index as we sent her */
#define  CRITICAL_ATTRIBUTE_MISSING                 0x2007
#define  REDUNDANT_ATTRIBUTE                        0x2008
#define  INVALID_GROUP_DESCRIPTION                  0x2009
#define  INVALID_KEY_EXCHANGE_SIZE                  0x200A
#define  DECRYPTION_FAILED                          0x200B
#define  INVALID_CIPHER_LENGTH                      0x200C
#define  UNEXPECTED_PAYLOAD                         0x200D
#define  CRITICAL_PAYLOAD_MISSING                   0x200E
#define  PAYLOAD_MISORDERED                         0x200F
#define  INVALID_ENCAPSULATION_MODE                 0x2010
#define  INVALID_AUTH_ALGORITHM                     0x2011
#define  GROUP_DESCRIPTION_CONFLICT                 0x2012
#define  INVALID_AUTH_METHOD                        0x2013
#define  IKE_INTERNAL_ERROR                         0x2014
#define  INVALID_TUNNEL_TYPE                        0x2015

/*
 * ISAKMP notification status message type
 */
#define  CONNECTED                                  16384

/*
 * IKE notification status message type
 */
#define  RESPONDER_LIFETIME                         24576
#define  REPLAY_STATUS                              24577
#define  INITIAL_CONTACT                            24578

/*
 * our own extended ISAKMP notification message type
 */
#define  TOPOLOGY_REQUEST                           32768
#define  TOPOLOGY_CHANGED                           32769
#define  TOPOLOGY_INFORMATION                       32770
#define  TOPOLOGY_INFORMATION_ACKNOWLEDGEMENT       32771
#define  ENDOF_TIE                                  32772
#define  COME_AGAIN                                 32773

/*
 *  RFC3706 -- TDPD
 */
#define  TDPD_R_U_THERE                             36136
#define  TDPD_R_U_THERE_ACK                         36137

/*
 * default retransmission intervals and retry times
 */
#define  IKE_DEFAULT_RETRANSMISSION_TIMES           3
#define  IKE_DEFAULT_RETRANSMISSION_INTERVAL        10           /* seconds */


#endif

