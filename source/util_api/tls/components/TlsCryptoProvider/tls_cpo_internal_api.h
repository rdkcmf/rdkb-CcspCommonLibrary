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

    module:	tls_cpo_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Crypto Provider
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/21/03    initial revision.

**********************************************************************/


#ifndef  _TLS_CPO_INTERNAL_API_
#define  _TLS_CPO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_STATES.C
***********************************************************/

/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_CIPHER.C
***********************************************************/

ANSC_STATUS
TlsCpoGetCipherInfo
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite,
        ANSC_HANDLE                 hSecurityParams
    );

ULONG
TlsCpoGetCertConfigBit
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetClientCertType
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetKeAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetKeyBlockSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetBulkEncryptionAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherType
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherKeyNumber
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherKeySize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherKeyMaterialSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherIVSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetCipherBlockSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetMacAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ULONG
TlsCpoGetMacHashSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ANSC_STATUS
TlsCpoGetExportable
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

ANSC_STATUS
TlsCpoGetStrongSecurity
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN TLS_CPO_CERT.C
***********************************************************/

BOOL
TlsCpoAuthenticateCertChain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 cert_chain
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN TLS_CPO_RSA.C
***********************************************************/

ANSC_STATUS
TlsCpoDecapRSAPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              rsa_key,
        ANSC_ASN1_INT**             rsa_modulus,
        ANSC_ASN1_INT**             rsa_exponent
    );

ANSC_HANDLE
TlsCpoEncapRSAPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              rsa_modulus,
        ANSC_ASN1_INT*              rsa_exponent
    );

ANSC_STATUS
TlsCpoGenerateRSAKeys
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_bits,
        ANSC_X509_KEY**             rsa_pub_key,
        ANSC_X509_KEY**             rsa_prv_key
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN TLS_CPO_DH.C
***********************************************************/

ANSC_STATUS
TlsCpoDecapDHPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_key,
        ANSC_ASN1_INT**             dh_prime,
        ANSC_ASN1_INT**             dh_generator,
        ANSC_ASN1_INT**             dh_public
    );

ANSC_HANDLE
TlsCpoEncapDHPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_ASN1_INT*              dh_public
    );

ANSC_STATUS
TlsCpoGenerateDHKeys1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    );

ANSC_STATUS
TlsCpoGenerateDHKeys2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    );

ANSC_STATUS
TlsCpoAgreeDHSecret
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_pub_key1,
        ANSC_X509_KEY*              dh_pub_key2,
        ANSC_X509_KEY*              dh_prv_key1,
        PUCHAR                      shared_secret,
        PULONG                      secret_size
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_RANDOM.C
***********************************************************/

ANSC_STATUS
TlsCpoGetRandomBytes
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       random_seed,
        PUCHAR                      random_bytes,
        ULONG                       random_len
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN TLS_CPO_PRF.C
***********************************************************/

ANSC_STATUS
TlsCpoCompute_PRF30
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_seed,
        ULONG                       i_seed_size,
        void*                       o_hash,
        ULONG                       o_hash_size
    );

ANSC_STATUS
TlsCpoCompute_PRF31
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        char*                       i_label,
        void*                       i_seed,
        ULONG                       i_seed_size,
        void*                       o_hash,
        ULONG                       o_hash_size
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_PUBLIC.C
***********************************************************/

ANSC_STATUS
TlsCpoEncrypt_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_pub_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_prv_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoSign
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_type,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    );

ANSC_STATUS
TlsCpoSign_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    );

ANSC_STATUS
TlsCpoSign_DSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    );

BOOL
TlsCpoVerify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_type,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    );

BOOL
TlsCpoVerify_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    );

BOOL
TlsCpoVerify_DSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN TLS_CPO_HASH.C
***********************************************************/

ANSC_STATUS
TlsCpoHash
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       hash_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

ANSC_STATUS
TlsCpoHash_MD5
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

ANSC_STATUS
TlsCpoHash_SHA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN TLS_CPO_HASH.C
***********************************************************/

ANSC_STATUS
TlsCpoHmac
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mac_algorithm,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

ANSC_STATUS
TlsCpoHmac_MD5
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

ANSC_STATUS
TlsCpoHmac_SHA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_ENCRYPT.C
***********************************************************/

ANSC_STATUS
TlsCpoEncrypt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cipher_algorithm,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_3DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_IDEA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_RC2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_RC4
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoEncrypt_AES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CPO_DECRYPT.C
***********************************************************/

ANSC_STATUS
TlsCpoDecrypt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cipher_algorithm,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_3DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_IDEA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_RC2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_RC4
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

ANSC_STATUS
TlsCpoDecrypt_AES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN TLS_CPO_COMPRESS.C
***********************************************************/

ANSC_STATUS
TlsCpoCompress
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       compress_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_comp,
        PULONG                      o_comp_size
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_CPO_DECOMPRESS.C
***********************************************************/

ANSC_STATUS
TlsCpoDecompress
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       compress_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_decomp,
        PULONG                      o_decomp_size
    );


#endif
