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

    module:	ansc_crypto_pub_ssleay_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Crypto Object.

        *   AnscCryptoPubSSLeayRequest

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/13/02    initial revision.
        09/23/03    zhubin added RSA private key encryption and public
                    key decryption.

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"

ANSC_STATUS
AnscCryptoPubSSLeayRequest
    (
        ANSC_CRYPTO_PUB_ALGORITHM   AlgorithmCode,
        ANSC_CRYPTO_PUB_OPERATION   OperationCode,
        ANSC_HANDLE                 hParams,
        ANSC_CRYPTO_FIN_CALLBACK    FinishCB,
        ANSC_HANDLE                 hFinishCBParam
    )
{
    UNREFERENCED_PARAMETER(FinishCB);
    UNREFERENCED_PARAMETER(hFinishCBParam);
    /* 
     * There is no implementation in this base object.
     * This object should not be used until someone
     * provides a real public key algorithm, no matter
     * it's software algorithm or hardware implementation.
     */
    ANSC_STATUS                     status;
    PANSC_CRYPTO_PUB_KEY_PARAMS     pParameters;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pKeyGenParams;
    PVOID                           pParams = (PVOID)hParams;

    pParameters     = (PANSC_CRYPTO_PUB_KEY_PARAMS)pParams;
    pKeyGenParams   = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)pParams;

    switch (AlgorithmCode)
    {
    default:
        return ANSC_STATUS_FAILURE;

    case ANSC_CRYPTO_PUB_ALGORITHM_DSA:
        {
            switch (OperationCode)
            {
            default:
                return ANSC_STATUS_FAILURE;

            case ANSC_CRYPTO_PUB_OPERATION_SIGN:
                {
                    status = AnscCryptoPubSSLeayDSASign
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }

            case ANSC_CRYPTO_PUB_OPERATION_VERIFY:
                {
                    status = AnscCryptoPubSSLeayDSAVerify
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }

            case ANSC_CRYPTO_PUB_OPERATION_GENERATE:
                {
                    ANSC_STATUS                 status;

                    status = AnscCryptoPubSSLeayGenKeyPairRandomly
                                (
                                    ANSC_CRYPTO_PUB_KEY_DSA,
                                    pKeyGenParams,
                                    pKeyGenParams->PublicKey.DSA.ParamP.Length
                                );

                    return status;
                }
            }
            break;
        }

    case ANSC_CRYPTO_PUB_ALGORITHM_DH:
        {
        switch (OperationCode)
        {
            default:
                return ANSC_STATUS_FAILURE;

            case ANSC_CRYPTO_PUB_OPERATION_DH_AGREE:
                return AnscCryptoPubSSLeayAgreeDHSharedSecret((PANSC_CRYPTO_DH_PARAMETERS)pParameters);
                break;

            case ANSC_CRYPTO_PUB_OPERATION_GENERATE:
                return AnscCryptoPubSSLeayGenDHKeyPairRandomly((PANSC_CRYPTO_DH_PARAMETERS)pParameters);
                break;
            }
            break;
        }

    case ANSC_CRYPTO_PUB_ALGORITHM_RSA_SIGNATURE:
        {
            switch (OperationCode)
            {
            default:
                return ANSC_STATUS_FAILURE;

            case ANSC_CRYPTO_PUB_OPERATION_SIGN:
                {
                    status = AnscCryptoPubSSLeayRSASign
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }  		
                break;

            case ANSC_CRYPTO_PUB_OPERATION_VERIFY:
                {
                    status = AnscCryptoPubSSLeayRSAVerify
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }
                break;

            case ANSC_CRYPTO_PUB_OPERATION_GENERATE:
                {
                    ANSC_STATUS                 status;

                    status = AnscCryptoPubSSLeayGenKeyPairRandomly
                                (
                                    ANSC_CRYPTO_PUB_KEY_RSA,
                                    pKeyGenParams,
                                    pKeyGenParams->PublicKey.RSA.Modulus.Length
                                );

                    return status;
                }
            }
            break;
        }

    case ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION:
        {
            switch (OperationCode)
            {
            default:
                return ANSC_STATUS_FAILURE;

            case ANSC_CRYPTO_PUB_OPERATION_ENCRYPT:
                {
                    status = AnscCryptoPubSSLeayRSAEncrypt
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }

            case ANSC_CRYPTO_PUB_OPERATION_DECRYPT:
                {
                    status = AnscCryptoPubSSLeayRSADecrypt
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }
                break;

            case ANSC_CRYPTO_PUB_OPERATION_PRI_ENCRYPT:
                {
                    status = AnscCryptoPubSSLeayRSAPriEncrypt
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }

            case ANSC_CRYPTO_PUB_OPERATION_PUB_DECRYPT:
                {
                    status = AnscCryptoPubSSLeayRSAPubDecrypt
                                (
                                    pParameters->pInput,
                                    pParameters->InputSize,
                                    pParameters->pOutput,
                                    &pParameters->OutputSize,
                                    pParameters
                                );

                    return status;
                }
                break;

            case ANSC_CRYPTO_PUB_OPERATION_GENERATE:
                {
                    ANSC_STATUS                 status;

                    status = AnscCryptoPubSSLeayGenKeyPairRandomly
                                (
                                    ANSC_CRYPTO_PUB_KEY_RSA,
                                    pKeyGenParams,
                                    pKeyGenParams->PublicKey.RSA.Modulus.Length
                                );

                    return status;
                }
            }
            break;
        }
        break;
    }

    return ANSC_STATUS_FAILURE;
}


static const UCHAR _simple_test_msg[]   = {
    'a', 'b', 'c', 'd', 'c', 'b', 'a','a', 'b', 'c', 'd', 'c', 'b', 'a','a', 'b', 'c', 'd', 'c', 'b', 'a',
    'a', 'b', 'c', 'd', 'c', 'b', 'a','a', 'b', 'c', 'd', 'c', 'b', 'a','a', 'b', 'c', 'd', 'c', 'b', 'a'
};

#define  _SIMPLE_TEST_SIG_SIZE                      512

BOOL
AnscCryptoPubSSLeayKeyPairMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_PUB_KEY_TYPE    KeyType,
        ANSC_HANDLE                 hPublicKey,
        ANSC_HANDLE                 hPrivateKey
    )
{
    PANSC_CRYPTO_PUB_SSLEAY_OBJECT  pCrypto;
    ANSC_STATUS                     status;
    PANSC_CRYPTO_DSA_PUBLIC_KEY     pPubDSA;
    PANSC_CRYPTO_DSA_PRIVATE_KEY    pPriDSA;
    PANSC_CRYPTO_RSA_PUBLIC_KEY     pPubRSA;
    PANSC_CRYPTO_RSA_PRIVATE_KEY    pPriRSA;
    UCHAR                           outData[_SIMPLE_TEST_SIG_SIZE];
    PANSC_CRYPTO_PUB_KEY_PARAMS     pParams;

    pCrypto = (PANSC_CRYPTO_PUB_SSLEAY_OBJECT)hThisObject;

    pPubDSA = (PANSC_CRYPTO_DSA_PUBLIC_KEY)hPublicKey;
    pPriDSA = (PANSC_CRYPTO_DSA_PRIVATE_KEY)hPrivateKey;

    pPubRSA = (PANSC_CRYPTO_RSA_PUBLIC_KEY)hPublicKey;
    pPriRSA = (PANSC_CRYPTO_RSA_PRIVATE_KEY)hPrivateKey;

    if (!pCrypto || !pPubDSA || !pPriDSA)
    {
        return FALSE;
    }

    pParams = AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_PARAMS));

    pParams->InputSize  = sizeof(_simple_test_msg)/sizeof(_simple_test_msg[0]);
    pParams->pInput     = (PVOID)_simple_test_msg;

    pParams->OutputSize = _SIMPLE_TEST_SIG_SIZE;
    pParams->pOutput    = (PVOID)outData;

    switch (KeyType)
    {
    case ANSC_CRYPTO_PUB_KEY_RSA:
        pParams->Key.RSA.RsaPrivateKey    = *pPriRSA;

        status = 
            pCrypto->RSAHashAndSign
                (
                    0,
                    (PVOID)_simple_test_msg,
                    sizeof(_simple_test_msg)/sizeof(_simple_test_msg[0]),
                    (PVOID)outData,
                    &pParams->OutputSize,
                    pParams
                );

        break;

    case ANSC_CRYPTO_PUB_KEY_DSA:
        pParams->Key.DSA.DsaPrivateKey    = *pPriDSA;

        status = 
            pCrypto->DSAHashAndSign
                (
                    0,
                    (PVOID)_simple_test_msg,
                    sizeof(_simple_test_msg)/sizeof(_simple_test_msg[0]),
                    (PVOID)outData,
                    &pParams->OutputSize,
                    pParams
                );

        break;

    default:
        AnscFreeMemory(pParams);

        return FALSE;
        break;
    }
    
    if (status == ANSC_STATUS_SUCCESS)
    {
        switch (KeyType)
        {
        case ANSC_CRYPTO_PUB_KEY_RSA:
            pParams->Key.RSA.RsaPublicKey    = *pPubRSA;

            status = 
                pCrypto->RSAHashAndVerify
                    (
                        0,
                        (PVOID)_simple_test_msg,
                        sizeof(_simple_test_msg)/sizeof(_simple_test_msg[0]),
                        (PVOID)outData,
                        pParams->OutputSize,
                        pParams
                    );

            break;

        case ANSC_CRYPTO_PUB_KEY_DSA:

            pParams->Key.DSA.DsaPublicKey    = *pPubDSA;

            status = 
                pCrypto->DSAHashAndVerify
                    (
                        0,
                        (PVOID)_simple_test_msg,
                        sizeof(_simple_test_msg)/sizeof(_simple_test_msg[0]),
                        (PVOID)outData,
                        pParams->OutputSize,
                        pParams
                    );

            break;
        default:
            break;
        }

    }
    
    AnscFreeMemory(pParams);

    return (status == ANSC_STATUS_SUCCESS);
}


ANSC_STATUS
AnscCryptoPubSSLeayRSASignAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    PANSC_CRYPTO_OBJECT             pCrypto;
    ANSC_CRYPTO_HASH                hash;
    ULONG                           ulHashSize;

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( nHashMethod == RSA_SIGN_WITH_MD5)
    {
        ulHashSize  = pCrypto->Md5Digest(pDataToSign, ulSizeOfData, &hash);
    }
    else
    {
        ulHashSize  = pCrypto->Sha1Digest(pDataToSign, ulSizeOfData, &hash);
    }

    if (ulHashSize == 0)
    {
        AnscCryptoRemove((ANSC_HANDLE)pCrypto);/*RDKB-6149, CID-33079, free unused mem on failure*/
        return ANSC_STATUS_FAILURE;
    }

    pCrypto->Remove(pCrypto);

    return AnscCryptoPubSSLeayRSASignWithType
               (
                   nHashMethod,
                   hash.Value,
                   ulHashSize,
                   pSignature,
                   pulSizeOfSignature,
                   pParams
               );
}


ANSC_STATUS
AnscCryptoPubSSLeayRSAVerifyAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    PANSC_CRYPTO_OBJECT             pCrypto;
    ANSC_CRYPTO_HASH                hash;
    ULONG                           ulHashSize;

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( nHashMethod == RSA_VERIFY_WITH_MD5)
    {
        ulHashSize  = pCrypto->Md5Digest(pDataToVerify, ulSizeOfData, &hash);
    }
    else
    {
        ulHashSize  = pCrypto->Sha1Digest(pDataToVerify, ulSizeOfData, &hash);
    }

    if (ulHashSize == 0)
    {
        AnscCryptoRemove((ANSC_HANDLE)pCrypto);/*RDKB-6149, CID-33363, free unused mem on failure*/
        return ANSC_STATUS_FAILURE;
    }

    pCrypto->Remove(pCrypto);

    return AnscCryptoPubSSLeayRSAVerifyWithType
               (
                   nHashMethod,
                   hash.Value,
                   ulHashSize,
                   pSignature,
                   ulSizeOfSignature,
                   pParams
               );
}

ANSC_STATUS
AnscCryptoPubSSLeayDSASignAPI
    (
        int                         nHashMethod,   /* it doesn't matter here, only SHA1 */
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    PANSC_CRYPTO_OBJECT             pCrypto;
    ANSC_CRYPTO_HASH                hash;
    ULONG                           ulHashSize;

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    ulHashSize  = pCrypto->Sha1Digest(pDataToSign, ulSizeOfData, &hash);

    if (ulHashSize == 0)
    {
        AnscCryptoRemove((ANSC_HANDLE)pCrypto);/*RDKB-6149, CID-32948, free unused mem on failure*/
        return ANSC_STATUS_FAILURE;
    }

    pCrypto->Remove(pCrypto);

    return
        AnscCryptoPubSSLeayDSASign
                (
                    hash.Value,
                    ulHashSize,
                    pSignature,
                    pulSizeOfSignature,
                    pParams
                );
}


ANSC_STATUS
AnscCryptoPubSSLeayDSAVerifyAPI
    (
        int                         nHashMethod,   /* it doesn't matter here, only SHA1 */
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    PANSC_CRYPTO_OBJECT             pCrypto;
    ANSC_CRYPTO_HASH                hash;
    ULONG                           ulHashSize;

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    ulHashSize  = pCrypto->Sha1Digest(pDataToVerify, ulSizeOfData, &hash);

    if (ulHashSize == 0)
    {
        AnscCryptoRemove((ANSC_HANDLE)pCrypto);/*RDKB-6149, CID-33070, free unused mem on failure*/
        return ANSC_STATUS_FAILURE;
    }

    pCrypto->Remove(pCrypto);

    return
        AnscCryptoPubSSLeayDSAVerify
                (
                    hash.Value,
                    ulHashSize,
                    pSignature,
                    ulSizeOfSignature,
                    pParams
                );
}
