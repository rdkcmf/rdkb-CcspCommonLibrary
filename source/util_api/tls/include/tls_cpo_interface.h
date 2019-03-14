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

    module:	tls_cpo_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Crypto Provider Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#ifndef  _TLS_CPO_INTERFACE_
#define  _TLS_CPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"


/***********************************************************
   PLATFORM INDEPENDENT CRYPTO PROVIDER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSCPO_GET_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite,
        ANSC_HANDLE                 hSecurityParams
    );

typedef  ULONG
(*PFN_TLSCPO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

typedef  BOOL
(*PFN_TLSCPO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    );

typedef  ULONG
(*PFN_TLSCPO_GET_SIZE1)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite,
        PULONG                      hash_size,
        PULONG                      key_material_size,
        PULONG                      iv_size
    );

typedef  BOOL
(*PFN_TLSCPO_AUTHENTICATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 cert_chain
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GET_NUM_BYKEY_RSA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              rsa_key,
        ANSC_ASN1_INT**             rsa_modulus,
        ANSC_ASN1_INT**             rsa_exponent
    );

typedef  ANSC_HANDLE
(*PFN_TLSCPO_GET_KEY_BYNUM_RSA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              rsa_modulus,
        ANSC_ASN1_INT*              rsa_exponent
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GEN_KEY_RSA)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_bits,
        ANSC_X509_KEY**             rsa_pub_key,
        ANSC_X509_KEY**             rsa_prv_key
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GET_NUM_BYKEY_DH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_key,
        ANSC_ASN1_INT**             dh_prime,
        ANSC_ASN1_INT**             dh_generator,
        ANSC_ASN1_INT**             dh_public
    );

typedef  ANSC_HANDLE
(*PFN_TLSCPO_GET_KEY_BYNUM_DH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_ASN1_INT*              dh_public
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GEN_KEY_DH1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GEN_KEY_DH2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_AGREE_DH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_pub_key1,
        ANSC_X509_KEY*              dh_pub_key2,
        ANSC_X509_KEY*              dh_prv_key1,
        PUCHAR                      shared_secret,
        PULONG                      secret_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_GET_BYTES)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       random_seed,
        PUCHAR                      random_bytes,
        ULONG                       random_len
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_PRF30)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_seed,
        ULONG                       i_seed_size,
        void*                       o_hash,
        ULONG                       o_hash_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_PRF31)
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

typedef  ANSC_STATUS
(*PFN_TLSCPO_ENCRYPT_RSA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_pub_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_DECRYPT_RSA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_prv_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_SIGN1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_type,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_SIGN2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    );

typedef  BOOL
(*PFN_TLSCPO_VERIFY1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_type,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    );

typedef  BOOL
(*PFN_TLSCPO_VERIFY2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_HASH1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       hash_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_HASH2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_HMAC1)
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

typedef  ANSC_STATUS
(*PFN_TLSCPO_HMAC2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_ENCRYPT1)
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

typedef  ANSC_STATUS
(*PFN_TLSCPO_ENCRYPT2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_DECRYPT1)
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

typedef  ANSC_STATUS
(*PFN_TLSCPO_DECRYPT2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_COMPRESS1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       compress_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_comp,
        PULONG                      o_comp_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCPO_DECOMPRESS1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       compress_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_decomp,
        PULONG                      o_decomp_size
    );

/*
 * In order to begin connection protection, the TLS Record Protocol requires specification of a
 * suite of algorithms, a master secret, and the client and server random values. The authen-
 * tication, encryption, and MAC algorithms are determined by the cipher_suite selected by the
 * server and revealed in the server hello message. The compression algorithm is negotiated in the
 * hello messages, and the random values are exchanged in the hello messages. All that remains is
 * to calculate the master secret.
 */
#define  TLS_CRYPTO_PROVIDER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAnscCrypto;                                            \
    ANSC_HANDLE                     hAnscCryptoPub;                                         \
                                                                                            \
    PFN_TLSCPO_GET_INFO             GetCipherInfo;                                          \
    PFN_TLSCPO_GET_ULONG            GetCertConfigBit;                                       \
    PFN_TLSCPO_GET_ULONG            GetClientCertType;                                      \
    PFN_TLSCPO_GET_ULONG            GetKeAlgorithm;                                         \
    PFN_TLSCPO_GET_ULONG            GetKeyBlockSize;                                        \
    PFN_TLSCPO_GET_ULONG            GetBulkEncryptionAlgorithm;                             \
    PFN_TLSCPO_GET_ULONG            GetCipherType;                                          \
    PFN_TLSCPO_GET_ULONG            GetCipherKeyNumber;                                     \
    PFN_TLSCPO_GET_ULONG            GetCipherKeySize;                                       \
    PFN_TLSCPO_GET_ULONG            GetCipherKeyMaterialSize;                               \
    PFN_TLSCPO_GET_ULONG            GetCipherIVSize;                                        \
    PFN_TLSCPO_GET_ULONG            GetCipherBlockSize;                                     \
    PFN_TLSCPO_GET_ULONG            GetMacAlgorithm;                                        \
    PFN_TLSCPO_GET_ULONG            GetMacHashSize;                                         \
    PFN_TLSCPO_GET_BOOL             GetExportable;                                          \
    PFN_TLSCPO_GET_BOOL             GetStrongSecurity;                                      \
                                                                                            \
    PFN_TLSCPO_AUTHENTICATE         AuthenticateCertChain;                                  \
                                                                                            \
    PFN_TLSCPO_GET_NUM_BYKEY_RSA    DecapRSAPublicKey;                                      \
    PFN_TLSCPO_GET_KEY_BYNUM_RSA    EncapRSAPublicKey;                                      \
    PFN_TLSCPO_GEN_KEY_RSA          GenerateRSAKeys;                                        \
                                                                                            \
    PFN_TLSCPO_GET_NUM_BYKEY_DH     DecapDHPublicKey;                                       \
    PFN_TLSCPO_GET_KEY_BYNUM_DH     EncapDHPublicKey;                                       \
    PFN_TLSCPO_GEN_KEY_DH1          GenerateDHKeys1;                                        \
    PFN_TLSCPO_GEN_KEY_DH2          GenerateDHKeys2;                                        \
    PFN_TLSCPO_AGREE_DH             AgreeDHSecret;                                          \
                                                                                            \
    PFN_TLSCPO_GET_BYTES            GetRandomBytes;                                         \
    PFN_TLSCPO_PRF30                Compute_PRF30;                                          \
    PFN_TLSCPO_PRF31                Compute_PRF31;                                          \
                                                                                            \
    PFN_TLSCPO_ENCRYPT_RSA          Encrypt_RSA;                                            \
    PFN_TLSCPO_DECRYPT_RSA          Decrypt_RSA;                                            \
    PFN_TLSCPO_SIGN1                Sign;                                                   \
    PFN_TLSCPO_SIGN2                Sign_RSA;                                               \
    PFN_TLSCPO_SIGN2                Sign_DSA;                                               \
    PFN_TLSCPO_VERIFY1              Verify;                                                 \
    PFN_TLSCPO_VERIFY2              Verify_RSA;                                             \
    PFN_TLSCPO_VERIFY2              Verify_DSA;                                             \
                                                                                            \
    PFN_TLSCPO_HASH1                Hash;                                                   \
    PFN_TLSCPO_HASH2                Hash_MD5;                                               \
    PFN_TLSCPO_HASH2                Hash_SHA;                                               \
                                                                                            \
    PFN_TLSCPO_HMAC1                Hmac;                                                   \
    PFN_TLSCPO_HMAC2                Hmac_MD5;                                               \
    PFN_TLSCPO_HMAC2                Hmac_SHA;                                               \
                                                                                            \
    PFN_TLSCPO_ENCRYPT1             Encrypt;                                                \
    PFN_TLSCPO_ENCRYPT2             Encrypt_DES;                                            \
    PFN_TLSCPO_ENCRYPT2             Encrypt_3DES;                                           \
    PFN_TLSCPO_ENCRYPT2             Encrypt_IDEA;                                           \
    PFN_TLSCPO_ENCRYPT2             Encrypt_RC2;                                            \
    PFN_TLSCPO_ENCRYPT2             Encrypt_RC4;                                            \
    PFN_TLSCPO_ENCRYPT2             Encrypt_AES;                                            \
                                                                                            \
    PFN_TLSCPO_DECRYPT1             Decrypt;                                                \
    PFN_TLSCPO_DECRYPT2             Decrypt_DES;                                            \
    PFN_TLSCPO_DECRYPT2             Decrypt_3DES;                                           \
    PFN_TLSCPO_DECRYPT2             Decrypt_IDEA;                                           \
    PFN_TLSCPO_DECRYPT2             Decrypt_RC2;                                            \
    PFN_TLSCPO_DECRYPT2             Decrypt_RC4;                                            \
    PFN_TLSCPO_DECRYPT2             Decrypt_AES;                                            \
                                                                                            \
    PFN_TLSCPO_COMPRESS1            Compress;                                               \
                                                                                            \
    PFN_TLSCPO_DECOMPRESS1          Decompress;                                             \
    /* end of object class content */                                                       \

typedef  struct
_TLS_CRYPTO_PROVIDER_OBJECT
{
    TLS_CRYPTO_PROVIDER_CLASS_CONTENT
}
TLS_CRYPTO_PROVIDER_OBJECT,  *PTLS_CRYPTO_PROVIDER_OBJECT;

#define  ACCESS_TLS_CRYPTO_PROVIDER_OBJECT(p)       \
         ACCESS_CONTAINER(p, TLS_CRYPTO_PROVIDER_OBJECT, Linkage)


#endif
