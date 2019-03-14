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

    module:	tls_ciphersuite_list.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file encapsulates all the Cipher Suite Descriptors
        that are defined by TLS standards.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/03    initial revision.

**********************************************************************/


#ifndef  _TLS_CIPHERSUITE_LIST_
#define  _TLS_CIPHERSUITE_LIST_


#include "tls_definitions.h"


/***********************************************************
          TLS STANDARD CIPHER SUTIE LIST DEFINITION
***********************************************************/

/*
 * Unlike some other key exchange protocols, such as IKE, TLS handshake combines various security
 * settings into a single negotiable parameter: CipherSuite. While such mechanism simplies the
 * handshake process, we need to break the agreed CipherSuite value into various security settings.
 */
TLS_CIPHERSUITE_DESCRIPTOR  g_tlsCipherSuiteList[] =
{
    #if  1
    {
        TLS_NULL_WITH_NULL_NULL,                                        /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        0,                                                              /* KeAlgorithm             */
        0,                                                              /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_null,                                 /* BulkEncryptionAlgorithm */
        0,                                                              /* CipherType              */
        0,                                                              /* CipherKeyNumber         */
        0,                                                              /* CipherKeySize           */
        0,                                                              /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        0,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_null,                                         /* MacAlgorithm            */
        0,                                                              /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_NULL_MD5,                                          /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        32,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_null,                                 /* BulkEncryptionAlgorithm */
        0,                                                              /* CipherType              */
        0,                                                              /* CipherKeyNumber         */
        0,                                                              /* CipherKeySize           */
        0,                                                              /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        0,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_NULL_SHA,                                          /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        40,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_null,                                 /* BulkEncryptionAlgorithm */
        0,                                                              /* CipherType              */
        0,                                                              /* CipherKeyNumber         */
        0,                                                              /* CipherKeySize           */
        0,                                                              /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        0,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_EXPORT_WITH_RC4_40_MD5,                                 /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING | TLS_CERT_BIT_RSA_ENCRYPT_LE512,      /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA_EXPORT,                                    /* KeAlgorithm             */
        42,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc4,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_stream,                                         /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        1,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_RC4_128_MD5,                                       /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        64,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc4,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_stream,                                         /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        1,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_RC4_128_SHA,                                       /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc4,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_stream,                                         /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        1,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5,                             /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING | TLS_CERT_BIT_RSA_ENCRYPT_LE512,      /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA_EXPORT,                                    /* KeAlgorithm             */
        42,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc2,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_IDEA_CBC_SHA,                                      /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        88,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_idea,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_EXPORT_WITH_DES40_CBC_SHA,                              /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING | TLS_CERT_BIT_RSA_ENCRYPT_LE512,      /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA_EXPORT,                                    /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_DES_CBC_SHA,                                       /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_3DES_EDE_CBC_SHA,                                  /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA,                           /* CipherSuite             */
        TLS_CERT_BIT_DH_DSS,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_DSS,                                        /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_DSS_WITH_DES_CBC_SHA,                                    /* CipherSuite             */
        TLS_CERT_BIT_DH_DSS,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_DSS,                                        /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_DH_DSS,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_DSS,                                        /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA,                           /* CipherSuite             */
        TLS_CERT_BIT_DH_RSA,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_RSA,                                        /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_RSA_WITH_DES_CBC_SHA,                                    /* CipherSuite             */
        TLS_CERT_BIT_DH_RSA,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_RSA,                                        /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_DH_RSA,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_RSA,                                        /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA,                          /* CipherSuite             */
        TLS_CERT_BIT_DSS,                                               /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_DSS_EXPORT,                                /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_DSS_WITH_DES_CBC_SHA,                                   /* CipherSuite             */
        TLS_CERT_BIT_DSS,                                               /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_DSS,                                       /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA,                              /* CipherSuite             */
        TLS_CERT_BIT_DSS,                                               /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_DSS,                                       /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA,                          /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_RSA_EXPORT,                                /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_RSA_WITH_DES_CBC_SHA,                                   /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_RSA,                                       /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA,                              /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_RSA,                                       /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_EXPORT_WITH_RC4_40_MD5,                             /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        42,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc4,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_stream,                                         /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        1,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_WITH_RC4_128_MD5,                                   /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        64,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_rc4,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_stream,                                         /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        0,                                                              /* CipherIVSize            */
        1,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_md5,                                          /* MacAlgorithm            */
        16,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_EXPORT_WITH_DES40_CBC_SHA,                          /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        50,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des40,                                /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        5,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        TRUE,                                                           /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_WITH_DES_CBC_SHA,                                   /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        72,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_des,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        FALSE                                                           /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_WITH_3DES_EDE_CBC_SHA,                              /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_3des,                                 /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        3,                                                              /* CipherKeyNumber         */
        8,                                                              /* CipherKeySize           */
        8,                                                              /* CipherKeyMaterialSize   */
        8,                                                              /* CipherIVSize            */
        8,                                                              /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_AES_128_CBC_SHA,                                   /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        104,                                                             /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_DSS_WITH_AES_128_CBC_SHA,                                /* CipherSuite             */
        TLS_CERT_BIT_DH_DSS,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_DSS,                                        /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_RSA_WITH_AES_128_CBC_SHA,                                /* CipherSuite             */
        TLS_CERT_BIT_DH_RSA,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_RSA,                                        /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_DSS_WITH_AES_128_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_DSS,                                               /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_DSS,                                       /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_RSA_WITH_AES_128_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_RSA,                                       /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_WITH_AES_128_CBC_SHA,                               /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        104,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        16,                                                             /* CipherKeySize           */
        16,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_RSA_WITH_AES_256_CBC_SHA,                                   /* CipherSuite             */
        TLS_CERT_BIT_RSA_ENCRYPT,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_RSA,                                           /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_DSS_WITH_AES_256_CBC_SHA,                                /* CipherSuite             */
        TLS_CERT_BIT_DH_DSS,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_DSS,                                        /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_RSA_WITH_AES_256_CBC_SHA,                                /* CipherSuite             */
        TLS_CERT_BIT_DH_RSA,                                            /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaFixedDh,                                /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_RSA,                                        /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_DSS_WITH_AES_256_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_DSS,                                               /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_dssSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_DSS,                                       /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DHE_RSA_WITH_AES_256_CBC_SHA,                               /* CipherSuite             */
        TLS_CERT_BIT_RSA_SIGNING,                                       /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_rsaSign,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DHE_RSA,                                       /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif

    #if  1
    {
        TLS_DH_anon_WITH_AES_256_CBC_SHA,                               /* CipherSuite             */
        0,                                                              /* CertConfigBit           */
        TLS_CLIENT_CERT_TYPE_unknown,                                   /* ClientCertType          */
        TLS_KE_ALGORITHM_DH_ANONYMOUS,                                  /* KeAlgorithm             */
        136,                                                            /* KeyBlockSize            */
        TLS_BULK_CIPHER_ALGORITHM_aes,                                  /* BulkEncryptionAlgorithm */
        TLS_CIPHER_TYPE_block,                                          /* CipherType              */
        1,                                                              /* CipherKeyNumber         */
        32,                                                             /* CipherKeySize           */
        32,                                                             /* CipherKeyMaterialSize   */
        16,                                                             /* CipherIVSize            */
        16,                                                             /* CipherBlockSize         */
        TLS_MAC_ALGORITHM_sha,                                          /* MacAlgorithm            */
        20,                                                             /* MacHashSize             */
        TLS_COMPRESSION_METHOD_NULL,                                    /* CompressionAlgorithm    */
        FALSE,                                                          /* bExportable             */
        TRUE                                                            /* bStrongSecurity         */
    },
    #endif
};


#endif
