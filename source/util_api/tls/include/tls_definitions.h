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
RFC 2246 The TLS Protocol Version 1.0 January 1999

Full Copyright Statement

Copyright (C) The Internet Society (1999). All Rights Reserved.

This document and translations of it may be copied and furnished to
others, and derivative works that comment on or otherwise explain it
or assist in its implementation may be prepared, copied, published
and distributed, in whole or in part, without restriction of any
kind, provided that the above copyright notice and this paragraph are
included on all such copies and derivative works. However, this
document itself may not be modified in any way, such as by removing
the copyright notice or references to the Internet Society or other
Internet organizations, except as needed for the purpose of
developing Internet standards in which case the procedures for
copyrights defined in the Internet Standards process must be
followed, or as required to translate it into languages other than
English.

The limited permissions granted above are perpetual and will not be
revoked by the Internet Society or its successors or assigns.

This document and the information contained herein is provided on an
"AS IS" basis and THE INTERNET SOCIETY AND THE INTERNET ENGINEERING
TASK FORCE DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION
HEREIN WILL NOT INFRINGE ANY RIGHTS OR ANY IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
**********************************************************************/




/**********************************************************************

    module:	tls_definitions.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the TLS element parameters that can be
        applied to the TLS Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_DEFINITIONS_
#define  _TLS_DEFINITIONS_


/***********************************************************
             TLS CIPHER SUITE SELECTOR DEFINITIONS
***********************************************************/

/*
 * The CipherSuite list, passed from the client to the server in the client hello message, contains
 * the combinations of cryptographic algorithms supported by the client in order of the client's
 * preference (favorite choice first). Each CipherSuite defines a key exchange algorithm, a bulk
 * encryption algorithm (including secret key length) and a MAC algorithm. The server will select a
 * cipher suite or, if no acceptable choices are presented, return a handshake failure alert and
 * close the connection.
 *
 *      uint8  CipherSuite[2];
 */

/*
 * TLS_NULL_WITH_NULL_NULL is specified and is the initial state of a TLS connection during the
 * first handshake on that channel, but must not be negotiated, as it provides no more protection
 * than an unsecured connection.
 */
enum {
    TLS_NULL_WITH_NULL_NULL                   = 0x0000,

    TLS_RSA_WITH_NULL_MD5                     = 0x0001,
    TLS_RSA_WITH_NULL_SHA                     = 0x0002,
    TLS_RSA_EXPORT_WITH_RC4_40_MD5            = 0x0003,
    TLS_RSA_WITH_RC4_128_MD5                  = 0x0004,
    TLS_RSA_WITH_RC4_128_SHA                  = 0x0005,
    TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5        = 0x0006,
    TLS_RSA_WITH_IDEA_CBC_SHA                 = 0x0007,
    TLS_RSA_EXPORT_WITH_DES40_CBC_SHA         = 0x0008,
    TLS_RSA_WITH_DES_CBC_SHA                  = 0x0009,
    TLS_RSA_WITH_3DES_EDE_CBC_SHA             = 0x000A,

    TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA      = 0x000B,
    TLS_DH_DSS_WITH_DES_CBC_SHA               = 0x000C,
    TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA          = 0x000D,
    TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA      = 0x000E,
    TLS_DH_RSA_WITH_DES_CBC_SHA               = 0x000F,
    TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA          = 0x0010,
    TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA     = 0x0011,
    TLS_DHE_DSS_WITH_DES_CBC_SHA              = 0x0012,
    TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA         = 0x0013,
    TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA     = 0x0014,
    TLS_DHE_RSA_WITH_DES_CBC_SHA              = 0x0015,
    TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA         = 0x0016,

    TLS_DH_anon_EXPORT_WITH_RC4_40_MD5        = 0x0017,
    TLS_DH_anon_WITH_RC4_128_MD5              = 0x0018,
    TLS_DH_anon_EXPORT_WITH_DES40_CBC_SHA     = 0x0019,
    TLS_DH_anon_WITH_DES_CBC_SHA              = 0x001A,
    TLS_DH_anon_WITH_3DES_EDE_CBC_SHA         = 0x001B,

    /* since the algorithm is not continuous, we defined a gap here */
    TLS_ALGORITHM_GAP                         = 0x0013,

    TLS_RSA_WITH_AES_128_CBC_SHA              = 0x002F,
    TLS_DH_DSS_WITH_AES_128_CBC_SHA           = 0x0030,
    TLS_DH_RSA_WITH_AES_128_CBC_SHA           = 0x0031,
    TLS_DHE_DSS_WITH_AES_128_CBC_SHA          = 0x0032,
    TLS_DHE_RSA_WITH_AES_128_CBC_SHA          = 0x0033,
    TLS_DH_anon_WITH_AES_128_CBC_SHA          = 0x0034,
    TLS_RSA_WITH_AES_256_CBC_SHA              = 0x0035,
    TLS_DH_DSS_WITH_AES_256_CBC_SHA           = 0x0036,
    TLS_DH_RSA_WITH_AES_256_CBC_SHA           = 0x0037,
    TLS_DHE_DSS_WITH_AES_256_CBC_SHA          = 0x0038,
    TLS_DHE_RSA_WITH_AES_256_CBC_SHA          = 0x0039,
    TLS_DH_anon_WITH_AES_256_CBC_SHA          = 0x003A,

    /* The max algorithm number increase the value if new Cypher group added other than defined gap*/
    TLS_MAX_ALGORITHM_NUMBER                  = 0x003A,

    TLS_MAX_CIPHER_SUITE_LIST_LENGTH          /*RDKB-6308, CID-24314, number of Cypher List avaialable*/
};

#define  TLS_COMPRESSION_METHOD_NULL                0

/*
 * Unlike some other key exchange protocols, such as IKE, TLS handshake combines various security
 * settings into a single negotiable parameter: CipherSuite. While such mechanism simplies the
 * handshake process, we need to break the agreed CipherSuite value into various security settings.
 */
typedef  struct
_TLS_CIPHERSUITE_DESCRIPTOR
{
    USHORT                          CipherSuite;

    ULONG                           CertConfigBit;              /* certification configuration required      */
    ULONG                           ClientCertType;             /* client certificate type may be requested  */
    ULONG                           KeAlgorithm;                /* key exchange algorithm used in handshake  */
    ULONG                           KeyBlockSize;               /* total number of bytes needed for keys     */
    ULONG                           BulkEncryptionAlgorithm;    /* algorithm used for bulk encryption        */
    ULONG                           CipherType;                 /* whether it's 'stream' or 'block' cipher   */
    ULONG                           CipherKeyNumber;            /* number of bulk cipher keys                */
    ULONG                           CipherKeySize;              /* size of a single bulk cipher key          */
    ULONG                           CipherKeyMaterialSize;      /* material size of a single cipher key      */
    ULONG                           CipherIVSize;               /* IV size of the block cipher algorithm     */
    ULONG                           CipherBlockSize;            /* block size of the block cipher algorithm  */
    ULONG                           MacAlgorithm;               /* algorithm used for message authentication */
    ULONG                           MacHashSize;                /* size of the hash result of the mac        */
    ULONG                           CompressionAlgorithm;       /* algorithm used for data compression       */
    BOOL                            bExportable;                /* whether the cipher is exportable          */
    BOOL                            bStrongSecurity;            /* whether the cipher is deemed strong       */
}
TLS_CIPHERSUITE_DESCRIPTOR,  *PTLS_CIPHERSUITE_DESCRIPTOR;


/***********************************************************
 TLS MODULE/CONNECTION CONFIGURATION PARAMETERS DEFINITIONS
***********************************************************/

/*
 * One advantage of TLS is that it is application protocol independent. Higher level protocols can
 * layer on top of the TLS Protocol transparently. The TLS standard, however, does not specify how
 * protocols add security with TLS; the decisions on how to initiate TLS handshaking and how to
 * interpret the authentication certificates exchanged are left up to the judgment of the designers
 * and implementors of protocols which run on top of TLS.
 */
#define  TLS_MAX_PROFILE_NUMBER                     32
#define  TLS_MAX_CERT_CHAIN_NUMBER                  32
#define  TLS_MAX_CA_CERT_NUMBER                     32
/*#define  TLS_MAX_CIPHER_SUITE_LIST_LENGTH           32  RDKB-6308, CID-24314, Nuber of Cypher suits are more*/
#define  TLS_DEF_SESSION_TIMEOUT                    3600            /* one hour, in seconds      */

#define  TLS_CERT_BIT_RSA_SIGNING_GT512             0x00000001      /* RSA public key >  512bits */
#define  TLS_CERT_BIT_RSA_ENCRYPT_GT512             0x00000002      /* RSA public key >  512bits */
#define  TLS_CERT_BIT_RSA_SIGNING_LE512             0x00000004      /* RSA public key <= 512bits */
#define  TLS_CERT_BIT_RSA_ENCRYPT_LE512             0x00000008      /* RSA public key <= 512bits */
#define  TLS_CERT_BIT_DSS                           0x00000010      /* DSS signing key           */
#define  TLS_CERT_BIT_DH_DSS                        0x00000020      /* Diffie-Hellman key        */
#define  TLS_CERT_BIT_DH_RSA                        0x00000040      /* Diffie-Hellman key        */

#define  TLS_CERT_BIT_RSA_SIGNING                   (TLS_CERT_BIT_RSA_SIGNING_GT512 | TLS_CERT_BIT_RSA_SIGNING_LE512)
#define  TLS_CERT_BIT_RSA_ENCRYPT                   (TLS_CERT_BIT_RSA_ENCRYPT_GT512 | TLS_CERT_BIT_RSA_ENCRYPT_LE512)

#define  TLS_CLIENT_CERT_TYPE_unknown               0
#define  TLS_CLIENT_CERT_TYPE_rsaSign               1
#define  TLS_CLIENT_CERT_TYPE_dssSign               2
#define  TLS_CLIENT_CERT_TYPE_rsaFixedDh            3
#define  TLS_CLIENT_CERT_TYPE_dssFixedDh            4

#define  TlsGetCertBits(x509_cert, cert_bits)                                               \
         {                                                                                  \
            cert_bits = 0;                                                                  \
                                                                                            \
            switch ( x509_cert->KeyType )                                                   \
            {                                                                               \
                case    ANSC_CERT_keyType_rsa :                                             \
                                                                                            \
                        if ( x509_cert->KeyBits > 512 )                                     \
                        {                                                                   \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature )\
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_RSA_SIGNING_GT512;                \
                            }                                                               \
                                                                                            \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment ) \
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_RSA_ENCRYPT_GT512;                \
                            }                                                               \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature )\
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_RSA_SIGNING_LE512;                \
                            }                                                               \
                                                                                            \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment ) \
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_RSA_ENCRYPT_LE512;                \
                            }                                                               \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_CERT_keyType_dsa :                                             \
                                                                                            \
                        if ( TRUE )                                                         \
                        {                                                                   \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature )\
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_DSS;                              \
                            }                                                               \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_CERT_keyType_diffieHellman :                                   \
                                                                                            \
                        if ( x509_cert->Signing == ANSC_CERT_signing_rsa )                  \
                        {                                                                   \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement )    \
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_DH_RSA;                           \
                            }                                                               \
                        }                                                                   \
                        else if ( x509_cert->Signing == ANSC_CERT_signing_dsa )             \
                        {                                                                   \
                            if ( x509_cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement )    \
                            {                                                               \
                                cert_bits |= TLS_CERT_BIT_DH_DSS;                           \
                            }                                                               \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
         }

typedef  struct
_TLS_MODULE_PARAMS
{
    BOOL                            bSessionPooling;
    BOOL                            bStrongSecurity;
    BOOL                            bExportableOnly;
    ULONG                           CertConfigBits;
    ULONG                           SessionTimeout;

    ULONG                           ProfileCount;
    ANSC_X509_PROFILE*              ProfileArray  [TLS_MAX_PROFILE_NUMBER   ];
    ULONG                           CertChainCount;
    ANSC_X509_CERTIFICATE_CHAIN*    CertChainArray[TLS_MAX_CERT_CHAIN_NUMBER];
    ULONG                           CACertCount;
    ANSC_X509_CERTIFICATE*          CACertArray   [TLS_MAX_CA_CERT_NUMBER   ];

    ULONG                           CipherSuiteCount;
    USHORT                          CipherSuiteArray[TLS_MAX_CIPHER_SUITE_LIST_LENGTH];
    ULONG                           CompressionCount;
    UCHAR                           CompressionArray[TLS_MAX_CIPHER_SUITE_LIST_LENGTH];
}
TLS_MODULE_PARAMS,  *PTLS_MODULE_PARAMS;

#define  TlsCloneModuleParams(src_params, dst_params)                                       \
         {                                                                                  \
            ULONG                   ii = 0;                                                 \
                                                                                            \
            if ( !dst_params )                                                              \
            {                                                                               \
                dst_params =                                                                \
                    (PTLS_MODULE_PARAMS)AnscAllocateMemory                                  \
                        (                                                                   \
                            sizeof(TLS_MODULE_PARAMS)                                       \
                        );                                                                  \
                                                                                            \
                if ( dst_params )                                                           \
                {                                                                           \
                    TlsInitModuleParams(dst_params);                                        \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( dst_params )                                                               \
            {                                                                               \
                TlsCleanModuleParams(dst_params);                                           \
                                                                                            \
                dst_params->bSessionPooling  = src_params->bSessionPooling;                 \
                dst_params->bStrongSecurity  = src_params->bStrongSecurity;                 \
                dst_params->bExportableOnly  = src_params->bExportableOnly;                 \
                dst_params->CertConfigBits   = src_params->CertConfigBits;                  \
                dst_params->SessionTimeout   = src_params->SessionTimeout;                  \
                                                                                            \
                dst_params->ProfileCount     = src_params->ProfileCount;                    \
                dst_params->CertChainCount   = src_params->CertChainCount;                  \
                dst_params->CACertCount      = src_params->CACertCount;                     \
                                                                                            \
                dst_params->CipherSuiteCount = src_params->CipherSuiteCount;                \
                dst_params->CompressionCount = src_params->CompressionCount;                \
                                                                                            \
                for ( ii = 0; ii < src_params->ProfileCount; ii++ )                         \
                {                                                                           \
                    if ( src_params->ProfileArray[ii] )                                     \
                    {                                                                       \
                        AnscCloneX509Profile                                                \
                            (                                                               \
                                src_params->ProfileArray[ii],                               \
                                dst_params->ProfileArray[ii]                                \
                            );                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                for ( ii = 0; ii < src_params->CertChainCount; ii++ )                       \
                {                                                                           \
                    if ( src_params->CertChainArray[ii] )                                   \
                    {                                                                       \
                        AnscCloneX509CertChain                                              \
                            (                                                               \
                                src_params->CertChainArray[ii],                             \
                                dst_params->CertChainArray[ii]                              \
                            );                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                for ( ii = 0; ii < src_params->CACertCount; ii++ )                          \
                {                                                                           \
                    if ( src_params->CACertArray[ii] )                                      \
                    {                                                                       \
                        AnscCloneX509Certificate                                            \
                            (                                                               \
                                src_params->CACertArray[ii],                                \
                                dst_params->CACertArray[ii]                                 \
                            );                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                for ( ii = 0; ii < src_params->CipherSuiteCount; ii++ )                     \
                {                                                                           \
                    dst_params->CipherSuiteArray[ii] = src_params->CipherSuiteArray[ii];    \
                }                                                                           \
                                                                                            \
                for ( ii = 0; ii < src_params->CompressionCount; ii++ )                     \
                {                                                                           \
                    dst_params->CompressionArray[ii] = src_params->CompressionArray[ii];    \
                }                                                                           \
            }                                                                               \
         }

#define  TlsInitModuleParams(tbi_params)                                                    \
         {                                                                                  \
            tbi_params->bSessionPooling  = TRUE;                                            \
            tbi_params->bStrongSecurity  = TRUE;                                            \
            tbi_params->bExportableOnly  = FALSE;                                           \
            tbi_params->CertConfigBits   = 0;                                               \
            tbi_params->SessionTimeout   = TLS_DEF_SESSION_TIMEOUT;                         \
            tbi_params->ProfileCount     = 0;                                               \
            tbi_params->CertChainCount   = 0;                                               \
            tbi_params->CACertCount      = 0;                                               \
            tbi_params->CipherSuiteCount = 0;                                               \
            tbi_params->CompressionCount = 0;                                               \
         }

#define  TlsCleanModuleParams(tbc_params)                                                   \
         {                                                                                  \
            ULONG                   ii = 0;                                                 \
                                                                                            \
            for ( ii = 0; ii < tbc_params->ProfileCount; ii++ )                             \
            {                                                                               \
                if ( tbc_params->ProfileArray[ii] )                                         \
                {                                                                           \
                    AnscFreeX509Profile(tbc_params->ProfileArray[ii]);                      \
                                                                                            \
                    tbc_params->ProfileArray[ii] = NULL;                                    \
                }                                                                           \
            }                                                                               \
                                                                                            \
            for ( ii = 0; ii < tbc_params->CertChainCount; ii++ )                           \
            {                                                                               \
                if ( tbc_params->CertChainArray[ii] )                                       \
                {                                                                           \
                    AnscFreeX509CertChain(tbc_params->CertChainArray[ii]);                  \
                                                                                            \
                    tbc_params->CertChainArray[ii] = NULL;                                  \
                }                                                                           \
            }                                                                               \
                                                                                            \
            for ( ii = 0; ii < tbc_params->CACertCount; ii++ )                              \
            {                                                                               \
                if ( tbc_params->CACertArray[ii] )                                          \
                {                                                                           \
                    AnscFreeX509Certificate(tbc_params->CACertArray[ii]);                   \
                                                                                            \
                    tbc_params->CACertArray[ii] = NULL;                                     \
                }                                                                           \
            }                                                                               \
         }

#define  TlsFreeModuleParams(tbf_params)                                                    \
         {                                                                                  \
            TlsCleanModuleParams(tbf_params);                                               \
            AnscFreeMemory      (tbf_params);                                               \
         }

#define  TLS_MAX_SESSION_ID_SIZE                    32

typedef  struct
_TLS_CONNECTION_PARAMS
{
    BOOL                            bSessionSharing;
    BOOL                            bQuickHandshake;
    BOOL                            bReqCertificate;
    BOOL                            bTlsClient;

    ULONG                           HostID;
    ULONG                           PeerID;
    ULONG                           SessionIDSize;
    UCHAR                           SessionID[TLS_MAX_SESSION_ID_SIZE];

    ULONG                           CipherSuiteCount;
    USHORT                          CipherSuiteArray[TLS_MAX_CIPHER_SUITE_LIST_LENGTH];
    ULONG                           CompressionCount;
    UCHAR                           CompressionArray[TLS_MAX_CIPHER_SUITE_LIST_LENGTH];
}
TLS_CONNECTION_PARAMS,  *PTLS_CONNECTION_PARAMS;

#define  TlsInitConnParams(tbi_params)                                                      \
         {                                                                                  \
            AnscZeroMemory(tbi_params, sizeof(TLS_CONNECTION_PARAMS));                      \
                                                                                            \
            tbi_params->bSessionSharing  = TRUE;                                            \
            tbi_params->bQuickHandshake  = FALSE;                                           \
            tbi_params->bReqCertificate  = FALSE;                                           \
            tbi_params->bTlsClient       = TRUE;                                            \
            tbi_params->HostID           = 0;                                               \
            tbi_params->PeerID           = 0;                                               \
            tbi_params->SessionIDSize    = 0;                                               \
            tbi_params->CipherSuiteCount = 0;                                               \
            tbi_params->CompressionCount = 0;                                               \
         }


/***********************************************************
      TLS RECORD CONNECTION STATE PARAMETER DEFINITIONS
***********************************************************/

/*
 * A TLS connection state is the operating environment of the TLS Record Protocol. It specifies a 
 * compression algorithm, encryption algorithm, and MAC algorithm. In addition, the parameters for
 * these algorithms are known: the MAC secret and the bulk encryption keys and IVs for the
 * connection in both the read and the write directions. Logically, there are always four
 * connection states outstanding: the current read and write states, and the pending read and write
 * states. All records are processed under the current read and write states. The security
 * parameters for the pending states can be set by the TLS Handshake Protocol, and the Handshake
 * Protocol can selectively make either of the pending states current, in which case the appro-
 * priate current state is disposed of and replaced with the pending state; the pending state is
 * then reinitialized to an empty state. It is illegal to make a state which has not been initia-
 * lized with security parameters a current state. The initial current state always specifies that
 * no encryption, compression, or MAC will be used.
 */
#define  TLS_MAX_RECORD_FRAGMENT_SIZE               16384
#define  TLS_MAX_RECORD_CHUNK_SIZE                  16384 + 2048

#define  TLS_KE_ALGORITHM_RSA                       1
#define  TLS_KE_ALGORITHM_RSA_EXPORT                2
#define  TLS_KE_ALGORITHM_DHE_DSS                   3
#define  TLS_KE_ALGORITHM_DHE_DSS_EXPORT            4
#define  TLS_KE_ALGORITHM_DHE_RSA                   5
#define  TLS_KE_ALGORITHM_DHE_RSA_EXPORT            6
#define  TLS_KE_ALGORITHM_DH_DSS                    7
#define  TLS_KE_ALGORITHM_DH_RSA                    8
#define  TLS_KE_ALGORITHM_DH_ANONYMOUS              9

#define  TLS_CONNECTION_END_server                  1
#define  TLS_CONNECTION_END_client                  2

#define  TLS_BULK_CIPHER_ALGORITHM_null             0
#define  TLS_BULK_CIPHER_ALGORITHM_rc4              1
#define  TLS_BULK_CIPHER_ALGORITHM_rc2              2
#define  TLS_BULK_CIPHER_ALGORITHM_des              3
#define  TLS_BULK_CIPHER_ALGORITHM_3des             4
#define  TLS_BULK_CIPHER_ALGORITHM_des40            5
#define  TLS_BULK_CIPHER_ALGORITHM_idea             6
#define  TLS_BULK_CIPHER_ALGORITHM_aes              7

#define  TLS_CIPHER_TYPE_stream                     1
#define  TLS_CIPHER_TYPE_block                      2

#define  TLS_MAC_ALGORITHM_null                     0
#define  TLS_MAC_ALGORITHM_md5                      1
#define  TLS_MAC_ALGORITHM_sha                      2

#define  TLS_COMPRESSION_ALGORITHM_null             0

#define  TLS_PRE_MASTER_SECRET_SIZE                 256
#define  TLS_MASTER_SECRET_SIZE                     48
#define  TLS_OPAQUE_RANDOM_SIZE                     32

typedef  struct
_TLS_SECURITY_PARAMS
{
    UCHAR                           MajorVersion;               /* major version of the tls protocol         */
    UCHAR                           MinorVersion;               /* minor version of the tls protocol         */

    ULONG                           ConnectionEnd;              /* whether it's 'client' or 'server'         */
    ULONG                           KeyBlockSize;               /* total number of bytes needed for keys     */
    ULONG                           BulkEncryptionAlgorithm;    /* algorithm used for bulk encryption        */
    ULONG                           CipherType;                 /* whether it's 'stream' or 'block' cipher   */
    ULONG                           CipherKeyNumber;            /* number of bulk cipher keys                */
    ULONG                           CipherKeySize;              /* size of a single bulk cipher key          */
    ULONG                           CipherKeyMaterialSize;      /* material size of a single cipher key      */
    ULONG                           CipherIVSize;               /* IV size of the block cipher algorithm     */
    ULONG                           CipherBlockSize;            /* block size of the block cipher algorithm  */
    ULONG                           MacAlgorithm;               /* algorithm used for message authentication */
    ULONG                           MacHashSize;                /* size of the hash result of the mac        */
    ULONG                           CompressionAlgorithm;       /* algorithm used for data compression       */
    BOOL                            bExportable;                /* whether the cipher is exportable          */

    ANSC_X509_PROFILE*              MyProfile;                  /* the certificate profile used locally      */
    ANSC_X509_CERTIFICATE_CHAIN*    PeerCertChain;              /* the certificate chain sent by the peer    */
    ANSC_X509_KEY*                  RsaKeyPub;                  /* the public RSA key used in handshake      */
    ANSC_X509_KEY*                  RsaKeyPrv;                  /* the private RSA key used in handshake     */
    ANSC_X509_KEY*                  DHKeyPubS;                  /* the public DH key used in handshake       */
    ANSC_X509_KEY*                  DHKeyPrvS;                  /* the private DH key used in handshake      */
    ANSC_X509_KEY*                  DHKeyPubC;                  /* the public DH key used in handshake       */
    ANSC_X509_KEY*                  DHKeyPrvC;                  /* the private DH key used in handshake      */

    ANSC_ASN1_INT*                  RsaModulus;                 /* the modulus of the temporary RSA key      */
    ANSC_ASN1_INT*                  RsaExponent;                /* the exponent of the temporary RSA key     */
    ANSC_ASN1_INT*                  DHPrime;                    /* the prime modulus of the DH key exchange  */
    ANSC_ASN1_INT*                  DHGenerator;                /* the generator of the DH key exchange      */
    ANSC_ASN1_INT*                  DHPublicS;                  /* the server's Diffie-Hellman public value  */
    ANSC_ASN1_INT*                  DHPublicC;                  /* the client's Diffie-Hellman public value  */
    ANSC_ASN1_INT*                  DHPrivate;                  /* the Diffie-Hellman private key value      */

    ULONG                           PreMasterSecretSize;
    UCHAR                           PreMasterSecret[TLS_PRE_MASTER_SECRET_SIZE];
    UCHAR                           MasterSecret   [TLS_MASTER_SECRET_SIZE    ];
    UCHAR                           ClientRandom   [TLS_OPAQUE_RANDOM_SIZE    ];
    UCHAR                           ServerRandom   [TLS_OPAQUE_RANDOM_SIZE    ];
}
TLS_SECURITY_PARAMS,  *PTLS_SECURITY_PARAMS;

#define  TlsInitSecurityParams(tbi_params)                                                  \
         {                                                                                  \
            tbi_params->MajorVersion            = 3;                                        \
            tbi_params->MinorVersion            = 1;                                        \
                                                                                            \
            tbi_params->ConnectionEnd           = TLS_CONNECTION_END_client;                \
            tbi_params->KeyBlockSize            = 0;                                        \
            tbi_params->BulkEncryptionAlgorithm = TLS_BULK_CIPHER_ALGORITHM_null;           \
            tbi_params->CipherType              = TLS_CIPHER_TYPE_stream;                   \
            tbi_params->CipherKeyNumber         = 0;                                        \
            tbi_params->CipherKeySize           = 0;                                        \
            tbi_params->CipherKeyMaterialSize   = 0;                                        \
            tbi_params->CipherIVSize            = 0;                                        \
            tbi_params->CipherBlockSize         = 0;                                        \
            tbi_params->MacAlgorithm            = TLS_MAC_ALGORITHM_null;                   \
            tbi_params->MacHashSize             = 0;                                        \
            tbi_params->CompressionAlgorithm    = TLS_COMPRESSION_ALGORITHM_null;           \
            tbi_params->bExportable             = TRUE;                                     \
                                                                                            \
            tbi_params->MyProfile               = NULL;                                     \
            tbi_params->PeerCertChain           = NULL;                                     \
            tbi_params->RsaKeyPub               = NULL;                                     \
            tbi_params->RsaKeyPrv               = NULL;                                     \
            tbi_params->DHKeyPubS               = NULL;                                     \
            tbi_params->DHKeyPrvS               = NULL;                                     \
            tbi_params->DHKeyPubC               = NULL;                                     \
            tbi_params->DHKeyPrvC               = NULL;                                     \
                                                                                            \
            tbi_params->RsaModulus              = NULL;                                     \
            tbi_params->RsaExponent             = NULL;                                     \
            tbi_params->DHPrime                 = NULL;                                     \
            tbi_params->DHGenerator             = NULL;                                     \
            tbi_params->DHPublicS               = NULL;                                     \
            tbi_params->DHPublicC               = NULL;                                     \
            tbi_params->DHPrivate               = NULL;                                     \
            tbi_params->PreMasterSecretSize     = TLS_PRE_MASTER_SECRET_SIZE;               \
         }

#define  TlsCleanSecurityParams(tbc_params)                                                 \
         {                                                                                  \
            if ( tbc_params->PeerCertChain )                                                \
            {                                                                               \
                AnscFreeX509CertChain(tbc_params->PeerCertChain);                           \
                                                                                            \
                tbc_params->PeerCertChain = NULL;                                           \
            }                                                                               \
                                                                                            \
            if ( tbc_params->RsaKeyPub )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->RsaKeyPub);                                     \
                                                                                            \
                tbc_params->RsaKeyPub = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->RsaKeyPrv )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->RsaKeyPrv);                                     \
                                                                                            \
                tbc_params->RsaKeyPrv = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHKeyPubS )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->DHKeyPubS);                                     \
                                                                                            \
                tbc_params->DHKeyPubS = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHKeyPrvS )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->DHKeyPrvS);                                     \
                                                                                            \
                tbc_params->DHKeyPrvS = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHKeyPubC )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->DHKeyPubC);                                     \
                                                                                            \
                tbc_params->DHKeyPubC = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHKeyPrvC )                                                    \
            {                                                                               \
                AnscFreeX509Key(tbc_params->DHKeyPrvC);                                     \
                                                                                            \
                tbc_params->DHKeyPrvC = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->RsaModulus )                                                   \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->RsaModulus);                                    \
                                                                                            \
                tbc_params->RsaModulus = NULL;                                              \
            }                                                                               \
                                                                                            \
            if ( tbc_params->RsaExponent )                                                  \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->RsaExponent);                                   \
                                                                                            \
                tbc_params->RsaExponent = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHPrime )                                                      \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->DHPrime);                                       \
                                                                                            \
                tbc_params->DHPrime = NULL;                                                 \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHGenerator )                                                  \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->DHGenerator);                                   \
                                                                                            \
                tbc_params->DHGenerator = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHPublicS )                                                    \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->DHPublicS);                                     \
                                                                                            \
                tbc_params->DHPublicS = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHPublicC )                                                    \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->DHPublicC);                                     \
                                                                                            \
                tbc_params->DHPublicC = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( tbc_params->DHPrivate )                                                    \
            {                                                                               \
                AnscAsn1FreeInt(tbc_params->DHPrivate);                                     \
                                                                                            \
                tbc_params->DHPrivate = NULL;                                               \
            }                                                                               \
         }

#define  TlsFreeSecurityParams(tbf_params)                                                  \
         {                                                                                  \
            TlsCleanSecurityParams(tbc_params);                                             \
            AnscFreeMemory        (tbc_params);                                             \
         }


typedef  struct
_TLS_RECORD_STATE
{
    UCHAR                           MajorVersion;               /* major version of the tls protocol         */
    UCHAR                           MinorVersion;               /* minor version of the tls protocol         */

    ANSC_UINT64                     SeqNumber;                  /* sequence number for the current state     */
    ULONG                           KeyBlockSize;               /* total number of bytes needed for keys     */
    ULONG                           BulkEncryptionAlgorithm;    /* algorithm used for bulk encryption        */
    ULONG                           CipherType;                 /* whether it's 'stream' or 'block' cipher   */
    ULONG                           CipherKeyNumber;            /* number of bulk cipher keys                */
    ULONG                           CipherKeySize;              /* size of a single bulk cipher key          */
    ULONG                           CipherKeyMaterialSize;      /* material size of a single cipher key      */
    ULONG                           CipherIVSize;               /* IV size of the block cipher algorithm     */
    ULONG                           CipherBlockSize;            /* block size of the block cipher algorithm  */
    ULONG                           MacAlgorithm;               /* algorithm used for message authentication */
    ULONG                           MacHashSize;                /* size of the hash result of the mac        */
    ULONG                           CompressionAlgorithm;       /* algorithm used for data compression       */
    BOOL                            bExportable;                /* whether the cipher is exportable          */

    ANSC_CRYPTO_KEY                 MacSecret;
    ANSC_CRYPTO_KEY                 CipherKey;
    ANSC_CRYPTO_IV                  CipherIV;
}
TLS_RECORD_STATE,  *PTLS_RECORD_STATE;

#define  TlsInitRecordState(tbi_state)                                                      \
         {                                                                                  \
            AnscInitUint64((&tbi_state->SeqNumber));                                        \
                                                                                            \
            tbi_state->MajorVersion            = 3;                                         \
            tbi_state->MinorVersion            = 1;                                         \
            tbi_state->KeyBlockSize            = 0;                                         \
            tbi_state->BulkEncryptionAlgorithm = TLS_BULK_CIPHER_ALGORITHM_null;            \
            tbi_state->CipherType              = TLS_CIPHER_TYPE_stream;                    \
            tbi_state->CipherKeyNumber         = 0;                                         \
            tbi_state->CipherKeySize           = 0;                                         \
            tbi_state->CipherKeyMaterialSize   = 0;                                         \
            tbi_state->CipherIVSize            = 0;                                         \
            tbi_state->CipherBlockSize         = 0;                                         \
            tbi_state->MacAlgorithm            = TLS_MAC_ALGORITHM_null;                    \
            tbi_state->MacHashSize             = 0;                                         \
            tbi_state->CompressionAlgorithm    = TLS_COMPRESSION_ALGORITHM_null;            \
            tbi_state->bExportable             = TRUE;                                      \
                                                                                            \
            tbi_state->MacSecret.KeyNumber     = 0;                                         \
            tbi_state->MacSecret.Length        = 0;                                         \
            tbi_state->CipherKey.KeyNumber     = 0;                                         \
            tbi_state->CipherKey.Length        = 0;                                         \
            tbi_state->CipherIV.Length         = 0;                                         \
         }

#define  TlsCleanRecordState(tbc_state)                                                     \
         {                                                                                  \
         }


/***********************************************************
      TLS HANDSHAKE SESSION STATE PARAMETER DEFINITIONS
***********************************************************/

/*
 * Cryptographic operations tend to be highly CPU intensive, particularly public key operations.
 * For this reason, the TLS protocol has incorporated an optional session caching scheme to reduce
 * the number of connections that need to be established from scratch. Additionally, care has been
 * taken to reduce network activity.
 */
typedef  struct
_TLS_SESSION_STATE
{
    ULONG                           HostID;
    ULONG                           PeerID;
    ULONG                           SessionIDSize;
    UCHAR                           SessionID[TLS_MAX_SESSION_ID_SIZE];
    UCHAR                           CertID   [ANSC_X509_CERT_ID_SIZE ];
    BOOL                            bCertRequested;
    USHORT                          CipherSuite;
    UCHAR                           Compression;
    ULONG                           KeAlgorithm;
    TLS_SECURITY_PARAMS             SecurityParams;
}
TLS_SESSION_STATE,  *PTLS_SESSION_STATE;

#define  TlsInitSessionState(tbi_state)                                                     \
         {                                                                                  \
            tbi_state->HostID         = 0;                                                  \
            tbi_state->PeerID         = 0;                                                  \
            tbi_state->SessionIDSize  = 0;                                                  \
            tbi_state->bCertRequested = FALSE;                                              \
            tbi_state->CipherSuite    = TLS_RSA_WITH_3DES_EDE_CBC_SHA;                      \
            tbi_state->Compression    = TLS_COMPRESSION_METHOD_NULL;                        \
            tbi_state->KeAlgorithm    = TLS_KE_ALGORITHM_RSA;                               \
                                                                                            \
            TlsInitSecurityParams((&tbi_state->SecurityParams));                            \
         }

#define  TlsCleanSessionState(tbc_state)                                                    \
         {                                                                                  \
            TlsCleanSecurityParams((&tbc_state->SecurityParams));                           \
         }

#define  TlsFreeSessionState(tbf_state)                                                     \
         {                                                                                  \
            TlsCleanSessionState(tbc_state);                                                \
            AnscFreeMemory      (tbc_state);                                                \
         }


#endif
