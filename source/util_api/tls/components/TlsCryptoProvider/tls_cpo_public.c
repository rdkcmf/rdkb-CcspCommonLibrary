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

    module:	tls_cpo_public.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced public key related
        crypto functions of the TLS Crypto Provider Object.

        *   TlsCpoEncrypt_RSA
        *   TlsCpoDecrypt_RSA
        *   TlsCpoSign
        *   TlsCpoSign_RSA
        *   TlsCpoSign_DSA
        *   TlsCpoVerify
        *   TlsCpoVerify_RSA
        *   TlsCpoVerify_DSA

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


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to perform RSA encryption by using
        the specifies public key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_pub_key
                Specifies the public key to be used for encryption.

                void*                       i_data
                Specifies the plain text data to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       o_data
                Specifies the output buffer of cipher text data.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_pub_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    if( i_pub_key == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( i_pub_key->KeyType != ANSC_CERT_keyType_rsa)
    {
        AnscTrace("TlsCpoEncrypt_RSA(): Invalid key type.");

        return ANSC_STATUS_FAILURE;
    }

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_pub_key->KeyData;

    params.Key.RSA.RsaPublicKey = pGenParams->PublicKey.RSA;

    Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION;
    Op  = ANSC_CRYPTO_PUB_OPERATION_ENCRYPT;

    params.InputSize  = i_data_size;
    params.pInput     = (PVOID)i_data;

    params.OutputSize = *o_data_size;
    params.pOutput    = (PVOID)o_data;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    *o_data_size = params.OutputSize;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to perform RSA decryption by using
        the specifies private key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_prv_key
                Specifies the private key to be used for decryption.

                void*                       i_data
                Specifies the cipher text data to be decrypted.

                ULONG                       i_data_size
                Specifies the size of the cihper text data.

                void*                       o_data
                Specifies the output buffer of plain text data.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoDecrypt_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_prv_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    if( i_prv_key == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( i_prv_key->KeyType != ANSC_CERT_keyType_rsa)
    {
        AnscTrace("TlsCpoEncrypt_RSA(): Invalid key type.");

        return ANSC_STATUS_FAILURE;
    }

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_prv_key->KeyData;

    params.Key.RSA.RsaPrivateKey = pGenParams->PrivateKey.RSA;

    Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION;
    Op  = ANSC_CRYPTO_PUB_OPERATION_DECRYPT;

    params.InputSize  = i_data_size;
    params.pInput     = (PVOID)i_data;

    params.OutputSize = *o_data_size;
    params.pOutput    = (PVOID)o_data;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    *o_data_size = params.OutputSize;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to generate a digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       key_type
                Specifie the private key type to be used.

                ANSC_X509_KEY*              i_key
                Specifies the private key to be used for signing.

                void*                       i_data
                Specifies the plain text data to be signed.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       o_sig
                Specifies the output buffer of signature data.

                PULONG                      o_sig_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

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
    )
{
    if( key_type == ANSC_CERT_keyType_rsa)
    {
        return
            TlsCpoSign_RSA
                (
                    hThisObject,
                    i_key,
                    i_data,
                    i_data_size,
                    o_sig,
                    o_sig_size
                );
    }
    else if( key_type == ANSC_CERT_keyType_dsa)
    {
        return
            TlsCpoSign_DSA
                (
                    hThisObject,
                    i_key,
                    i_data,
                    i_data_size,
                    o_sig,
                    o_sig_size
                );
    }

    AnscTrace("Unsupported sign key type : '%lu'\n", key_type);

    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to generate a RSA digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_key
                Specifies the private key to be used for signing.

                void*                       i_data
                Specifies the plain text data to be signed.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       o_sig
                Specifies the output buffer of signature data.

                PULONG                      o_sig_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoSign_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_key->KeyData;
    params.Key.RSA.RsaPrivateKey = pGenParams->PrivateKey.RSA;

    /* actually, here in TLS is encryption with RSA private key */
    Op  = ANSC_CRYPTO_PUB_OPERATION_PRI_ENCRYPT;
    Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION;

    params.InputSize  = i_data_size;
    params.pInput     = (PVOID)i_data;

    params.OutputSize = *o_sig_size;
    params.pOutput    = (PVOID)o_sig;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    *o_sig_size = params.OutputSize;

#if 0

    {
        char    pTemp[512]  = { 0 };

        AnscTrace("TlsCpoSign_RSA()\n");

        AnscFromHexToString( i_data, pTemp, i_data_size, FALSE);

        AnscTrace("The input hash value is (L%d):\n %s\n", i_data_size, pTemp);

        AnscFromHexToString( o_sig, pTemp, *o_sig_size, FALSE);

        AnscTrace("The input signature value is (L%d):\n %s\n", *o_sig_size, pTemp);
    }

#endif

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to generate a DSA digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_key
                Specifies the private key to be used for signing.

                void*                       i_data
                Specifies the plain text data to be signed.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       o_sig
                Specifies the output buffer of signature data.

                PULONG                      o_sig_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoSign_DSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_sig,
        PULONG                      o_sig_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_key->KeyData;
    params.Key.DSA.DsaPrivateKey = pGenParams->PrivateKey.DSA;

    /* init the operation method */
    Op  = ANSC_CRYPTO_PUB_OPERATION_SIGN;
    Alg = ANSC_CRYPTO_PUB_ALGORITHM_DSA;

    params.InputSize  = i_data_size;
    params.pInput     = (PVOID)i_data;

    params.OutputSize = *o_sig_size;
    params.pOutput    = (PVOID)o_sig;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    *o_sig_size = params.OutputSize;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to verify a digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       key_type
                Specifie the public key type to be used.

                ANSC_X509_KEY*              i_key
                Specifies the public key to be used for verifying.

                void*                       i_data
                Specifies the plain text data to be verified.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       i_sig
                Specifies the signature to be verified.

                ULONG                       i_data_size
                Specifies the size of the signature.

    return:     TRUE or FALSE.

**********************************************************************/

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
    )
{
    if( key_type == ANSC_CERT_keyType_rsa)
    {
        return
            TlsCpoVerify_RSA
                (
                    hThisObject,
                    i_key,
                    i_data,
                    i_data_size,
                    i_sig,
                    i_sig_size
                );
    }
    else if( key_type == ANSC_CERT_keyType_dsa)
    {
        return
            TlsCpoVerify_DSA
                (
                    hThisObject,
                    i_key,
                    i_data,
                    i_data_size,
                    i_sig,
                    i_sig_size
                );
    }

    AnscTrace("Unsupported sign key type : '%lu'\n", key_type);

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to verify a RSA digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_key
                Specifies the public key to be used for verifying.

                void*                       i_data
                Specifies the plain text data to be verified.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       i_sig
                Specifies the signature to be verified.

                ULONG                       i_data_size
                Specifies the size of the signature.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsCpoVerify_RSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;
    UCHAR                           pOutput[512] = { 0 };
    ULONG                           lenOfOut     = 512;

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_key->KeyData;
    params.Key.RSA.RsaPublicKey = pGenParams->PublicKey.RSA;

    /* actually, here in TLS is decryption with RSA public key */
    Op  = ANSC_CRYPTO_PUB_OPERATION_PUB_DECRYPT;
    Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION;

    params.InputSize  = i_sig_size;
    params.pInput     = (PVOID)i_sig;

    params.OutputSize = lenOfOut;
    params.pOutput    = (PVOID)pOutput;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to decrypt with RSA publickey.\n");

        return FALSE;
    }

#if 0

    {
        char    pTemp[512]  = { 0 };

        AnscTrace("TlsCpoVerify_RSA()\n");

        AnscFromHexToString( pOutput, pTemp, params.OutputSize, FALSE);

        AnscTrace("The decrypted data value is (L%d):\n %s\n", params.OutputSize, pTemp);
    }

#endif

    if( params.OutputSize != i_data_size)
    {
        return FALSE;
    }

    if(!AnscEqualMemory(params.pOutput, i_data, i_data_size))
    {
        return FALSE;
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to verify a DSA digital signature.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              i_key
                Specifies the public key to be used for verifying.

                void*                       i_data
                Specifies the plain text data to be verified.

                ULONG                       i_data_size
                Specifies the size of the plain text data.

                void*                       i_sig
                Specifies the signature to be verified.

                ULONG                       i_data_size
                Specifies the size of the signature.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsCpoVerify_DSA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              i_key,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       i_sig,
        ULONG                       i_sig_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)i_key->KeyData;
    params.Key.DSA.DsaPublicKey = pGenParams->PublicKey.DSA;

    /* init the operation method */
    Op  = ANSC_CRYPTO_PUB_OPERATION_VERIFY;
    Alg = ANSC_CRYPTO_PUB_ALGORITHM_DSA;

    params.InputSize  = i_data_size;
    params.pInput     = (PVOID)i_data;

    params.OutputSize = i_sig_size;
    params.pOutput    = (PVOID)i_sig;

    returnStatus =
        pSSLCrypto->Request
            (
                Alg,
                Op,
                (ANSC_HANDLE)&params,
                NULL,
                NULL
            );

    return  (returnStatus == ANSC_STATUS_SUCCESS);
}
