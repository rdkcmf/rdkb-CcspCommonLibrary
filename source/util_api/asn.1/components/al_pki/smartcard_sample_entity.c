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

    MODULE: smartcard_sample_entity.c

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    COPYRIGHT:

        Cisco Systems, Inc., 1999 ~ 2003

        All Rights Reserved.

    ---------------------------------------------------------------

    DESCRIPTION:

        This interface is for the entity of PKI implementation.

        The entity is responsible for generating key pair, pkcs10
        certificate request, self-signed certificate.

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   08/19/2002  initial revision

 **********************************************************************/

#include "ansc_pki_local.h"

#ifndef _ANSC_KERNEL
/***********************************************************************
 *
 *  static functions for SampleSmartcardEntity
 *
 ***********************************************************************/
PKI_KEY_TYPE
SmartCardGetKeyType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSAMPLE_SMARTCARD_ENTITY       pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;

    return pThisObject->KeyType;
}    

LONG
SmartCardGetKeyLength
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSAMPLE_SMARTCARD_ENTITY       pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;

    return pThisObject->KeyLength;
}


ANSC_STATUS
SmartCardGetPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        PPKI_KEY_TYPE               pKeyType,
        ANSC_HANDLE                 pKeyParam
    )
{
    PSAMPLE_SMARTCARD_ENTITY       pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;

    if( pKeyType != NULL)
    {
        *pKeyType = pThisObject->KeyType;
    }

    AnscCopyMemory
        (
            pKeyParam,
            pThisObject->hKeyPairHandle,
            sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
        );

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
SmartCardSignData
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDataWillBeSigned,
        ULONG                       lengthOfData,
        SIGNATURE_TYPE              SignatureType,
        PUCHAR                      pDataSigned,
        PULONG                      pLength
    )
{
    PSAMPLE_SMARTCARD_ENTITY        pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;
    PANSC_CRYPTO_PUB_SSLEAY_OBJECT  pSSLCrypto   = NULL;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    PKI_KEY_TYPE                    keyType;
    int                             nHashMethod;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_STATUS                     retStatus;

    pSSLCrypto = (PANSC_CRYPTO_PUB_SSLEAY_OBJECT)pThisObject->hCryptoSSL;

    if( pSSLCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
        pThisObject->hKeyPairHandle;

    keyType = pThisObject->KeyType;

    if( keyType == PKI_RSA_KEY)
    {
        if( SignatureType == RSA_SHA1_SIGNATURE)
        {
            nHashMethod = RSA_SIGN_WITH_SHA1;
        }
        else if(SignatureType == RSA_MD5_SIGNATURE)
        {
            nHashMethod = RSA_SIGN_WITH_MD5;
        }
        else
        {
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( keyType == PKI_DSA_KEY)
    {
        /* for DSA, it doesn't matter here, only SHA1 is supported */
        nHashMethod = 0;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    /* init the params */
    if( keyType == PKI_RSA_KEY)
    {
        params.Key.RSA.RsaPrivateKey = pGenParams->PrivateKey.RSA;

        retStatus = 
            pSSLCrypto->RSAHashAndSign
                (
                    nHashMethod,
                    (PVOID)pDataWillBeSigned,
                    lengthOfData,
                    (PVOID)pDataSigned,
                    pLength,
                    &params
                );
    }
    else if( keyType == PKI_DSA_KEY)
    {
        params.Key.DSA.DsaPrivateKey = pGenParams->PrivateKey.DSA;

        retStatus = 
            pSSLCrypto->DSAHashAndSign
                (
                    nHashMethod,
                    (PVOID)pDataWillBeSigned,
                    lengthOfData,
                    (PVOID)pDataSigned,
                    pLength,
                    &params
                );
    }


    return retStatus;

}
 
ANSC_STATUS
SmartCardDecryptData
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDataWillBeDecrypted,
        ULONG                       lengthOfData,
        PUCHAR                      pDataDecrypted,
        PULONG                      pLength
    )
{
    PSAMPLE_SMARTCARD_ENTITY        pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;
    PANSC_CRYPTO_PUB_SSLEAY_OBJECT  pSSLCrypto   = NULL;
    ANSC_CRYPTO_PUB_KEY_PARAMS      params;
    ANSC_STATUS                     retStatus;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;

    if( pThisObject->KeyType != PKI_RSA_KEY)
    {
        return ANSC_STATUS_FAILURE;
    }

    pSSLCrypto = (PANSC_CRYPTO_PUB_SSLEAY_OBJECT)pThisObject->hCryptoSSL;

    if( pSSLCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
        pThisObject->hKeyPairHandle;

    /* init the key */
    params.Key.RSA.RsaPrivateKey = pGenParams->PrivateKey.RSA;

    /* init the decryption */
    Op  = ANSC_CRYPTO_PUB_OPERATION_DECRYPT;
    Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION;

    params.InputSize  = lengthOfData;
    params.pInput     = (PVOID)pDataWillBeDecrypted;

    params.OutputSize = *pLength;
    params.pOutput    = (PVOID)pDataDecrypted;

    retStatus = 
        pSSLCrypto->Request
            (
                Alg, 
                Op, 
                (ANSC_HANDLE)&params, 
                NULL, 
                NULL
            );

    if(pLength)
    {
        *pLength = params.OutputSize;
    }

    return retStatus;
}

ANSC_STATUS
SmartCardRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSAMPLE_SMARTCARD_ENTITY       pThisObject = (PSAMPLE_SMARTCARD_ENTITY)hThisObject;
    PANSC_CRYPTO_PUB_SSLEAY_OBJECT pSSLCrypto;

    if( pThisObject == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /* remove the hCryptoSSL */
    if( pThisObject->hCryptoSSL != NULL)
    {
        pSSLCrypto = (PANSC_CRYPTO_PUB_SSLEAY_OBJECT)pThisObject->hCryptoSSL;

        pSSLCrypto->Remove(pSSLCrypto);
    }

    /* remove the key pair */
    if( pThisObject->hKeyPairHandle != NULL)
    {
        AnscFreeMemory(pThisObject->hKeyPairHandle);
    }

    AnscFreeMemory(pThisObject);

    return ANSC_STATUS_SUCCESS;
}

/***********************************************************************
 *
 *  Exported API
 *
 ***********************************************************************/
ANSC_HANDLE
AnscPKICreateSampleSmartCardEntity
    (
        PKI_KEY_TYPE                type,
        ULONG                       length  /* in Bytes */
    )
{
    PSAMPLE_SMARTCARD_ENTITY        pThisObject = NULL; 
    PANSC_CRYPTO_PUB_SSLEAY_OBJECT  pSSLCrypto   = NULL;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;
    ANSC_CRYPTO_PUB_ALGORITHM       Alg;
    ANSC_CRYPTO_PUB_OPERATION       Op;

    if( type != PKI_RSA_KEY && type != PKI_DSA_KEY)
    {
        return NULL;
    }

    if( length < 64 || length % 8 != 0)
    {
        length = 128;
    }

    /* create the object */
    pThisObject = (PSAMPLE_SMARTCARD_ENTITY)
        AnscAllocateMemory(sizeof(SAMPLE_SMARTCARD_ENTITY));

    if( pThisObject == NULL)
    {
        return NULL;
    }

    /* create the CryptoSSL */
    pSSLCrypto = (PANSC_CRYPTO_PUB_SSLEAY_OBJECT)AnscCreateCryptoPubSSLeay(NULL);    

    if( pSSLCrypto == NULL)
    {
        goto EXIT;
    }

    /*
     *  init the paramters;
     */
    pThisObject->KeyLength      = length;
    pThisObject->KeyType        = type;
    pThisObject->hCryptoSSL     = pSSLCrypto;

    /*
     *  Generate the key pair;
     */
    pThisObject->hKeyPairHandle  = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
        AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    if( pThisObject->hKeyPairHandle == NULL)
    {
        AnscTrace("Failed to generate the key pair.\n");

        goto EXIT;
    }

    AnscZeroMemory( pThisObject->hKeyPairHandle, sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));
    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)pThisObject->hKeyPairHandle;

    if( pThisObject->KeyType == PKI_RSA_KEY)
    {
        Alg = ANSC_CRYPTO_PUB_ALGORITHM_RSA_SIGNATURE;
        pGenParams->PublicKey.RSA.Modulus.Length  = pThisObject->KeyLength;
    }
    else
    {
        Alg = ANSC_CRYPTO_PUB_ALGORITHM_DSA;
        pGenParams->PublicKey.DSA.ParamQ.Length   = pThisObject->KeyLength;
        pGenParams->PublicKey.DSA.ParamP.Length   = pThisObject->KeyLength;
    }

    Op = ANSC_CRYPTO_PUB_OPERATION_GENERATE;

    pSSLCrypto->Request(Alg, Op, pGenParams, NULL, NULL);

    /*
     *  init the apis;
     */
    pThisObject->pGetKeyType    = SmartCardGetKeyType;
    pThisObject->pGetKeyLength  = SmartCardGetKeyLength;
    pThisObject->pGetPublicKey  = SmartCardGetPublicKey;
    pThisObject->pSignData      = SmartCardSignData;
    pThisObject->pDecryptData   = SmartCardDecryptData;
    pThisObject->Remove         = SmartCardRemove;

    return pThisObject;

EXIT:

    if( pThisObject != NULL)
    {
        pThisObject->Remove(pThisObject);
    }

    return (ANSC_HANDLE)NULL;
}

#endif

