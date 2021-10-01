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

    module:	tls_cpo_rsa.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced RSA-related crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoDecapRSAPublicKey
        *   TlsCpoEncapRSAPublicKey
        *   TlsCpoGenerateRSAKeys

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/03    initial revision.
        07/09/02    implemented all the apis by Bin Zhu

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoDecapRSAPublicKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_X509_KEY*              rsa_key,
                ANSC_ASN1_INT**             rsa_modulus,
                ANSC_ASN1_INT**             rsa_exponent
            );

    description:

        This function is called to break a RSA public key into modulus
        and exponent values.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              rsa_key
                Specifies the RSA public key to be dismantled.

                ANSC_ASN1_INT**             rsa_modulus
                Specifies the RSA modulus to be returned. This function
                is responsible for allocating memory for an
                ANSC_ASN1_INT structure and returning its pointer in
                this parameter.

                ANSC_ASN1_INT**             rsa_exponent
                Specifies the RSA exponent to be returned. This
                function is responsible for allocating memory for an
                ANSC_ASN1_INT structure and returning its pointer in
                this parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoDecapRSAPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              rsa_key,
        ANSC_ASN1_INT**             rsa_modulus,
        ANSC_ASN1_INT**             rsa_exponent
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)rsa_key->KeyData;

    if( pGenParams == NULL || rsa_key->KeyType != ANSC_CERT_keyType_rsa)
    {
        return ANSC_STATUS_FAILURE;
    }

    *rsa_modulus  = 
        (ANSC_ASN1_INT*)AnscAsn1AllocInt
            (
                pGenParams->PublicKey.RSA.Modulus.Data,
                pGenParams->PublicKey.RSA.Modulus.Length,
                FALSE
            );

    *rsa_exponent = 
        (ANSC_ASN1_INT*)AnscAsn1AllocInt
            (
                pGenParams->PublicKey.RSA.PublicExponent.Data.ucData,
                pGenParams->PublicKey.RSA.PublicExponent.Length,
                FALSE
            );

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsCpoEncapRSAPublicKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_ASN1_INT*              rsa_modulus,
                ANSC_ASN1_INT*              rsa_exponent
            );

    description:

        This function is called to assemble a RSA public key from the
        modulus and exponent values. This function is responsible for
        allocating memory for an ANSC_X509_KEY structure and return its
        pointer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_ASN1_INT*              rsa_modulus
                Specifies the RSA modulus of the public key to be
                assembled and returned.

                ANSC_ASN1_INT*              rsa_exponent
                Specifies the RSA exponent of the public key to be
                assembled and returned

    return:     rsa public key.

**********************************************************************/

ANSC_HANDLE
TlsCpoEncapRSAPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              rsa_modulus,
        ANSC_ASN1_INT*              rsa_exponent
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_X509_KEY*                  pRsaPubKey   = (ANSC_X509_KEY*             )NULL;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams   = NULL;

    if( rsa_modulus == NULL || rsa_exponent == NULL)
    {
        goto EXIT;
    }

    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    if( pGenParams == NULL)
    {
        goto EXIT;
    }

    pGenParams->PublicKey.RSA.Modulus.Length           = rsa_modulus->Length;
    pGenParams->PublicKey.RSA.PublicExponent.Length    = rsa_exponent->Length;

    AnscCopyMemory
        (
            pGenParams->PublicKey.RSA.Modulus.Data,
            rsa_modulus->Octets,
            rsa_modulus->Length
        );

    AnscCopyMemory
        (
            pGenParams->PublicKey.RSA.PublicExponent.Data.ucData,
            rsa_exponent->Octets,
            rsa_exponent->Length
        );


    /* create the PANSC_X509_KEY */
    pRsaPubKey = (PANSC_X509_KEY)AnscAllocateMemory(sizeof(ANSC_X509_KEY));

    if( pRsaPubKey == NULL)
    {
        goto EXIT;
    }

    pRsaPubKey->KeyType             = ANSC_CERT_keyType_rsa;
    pRsaPubKey->KeyBits             = rsa_modulus->Length * 8;   /* Bits value */
    pRsaPubKey->KeySize             = sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS);
    pRsaPubKey->KeyData             = (void*)pGenParams;

    /* reset pGenParams */
    pGenParams = NULL;

EXIT:

    if( pGenParams != NULL)
    {
        AnscFreeMemory(pGenParams);
    }

    return  (ANSC_HANDLE)pRsaPubKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGenerateRSAKeys
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       key_bits,
                ANSC_X509_KEY**             rsa_pub_key,
                ANSC_X509_KEY**             rsa_prv_key
            );

    description:

        This function is called to randomly generate a RSA key pair.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       key_bits
                Specifies the length of the RSA keys to be generated.

                ANSC_X509_KEY**             rsa_pub_key
                Specifies the RSA public key to be generated. This
                function is responsible for allocating memory for an
                ANSC_X509_KEY structure and returning its pointer in
                this parameter.

                ANSC_X509_KEY**             rsa_prv_key
                Specifies the RSA private key to be generated. This
                function is responsible for allocating memory for an
                ANSC_X509_KEY structure and returning its pointer in
                this parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoGenerateRSAKeys
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       key_bits,
        ANSC_X509_KEY**             rsa_pub_key,
        ANSC_X509_KEY**             rsa_prv_key
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_X509_KEY*                  pRsaPubKey   = (ANSC_X509_KEY*             )NULL;
    ANSC_X509_KEY*                  pRsaPrvKey   = (ANSC_X509_KEY*             )NULL;
    ANSC_CRYPTO_PUB_KEY_GEN_PARAMS  genParams;

    if( key_bits % 8 != 0)
    {
        AnscTrace("Invalid bits length '%lu'\n", key_bits);

        return ANSC_STATUS_FAILURE;
    }

    AnscZeroMemory(&genParams, sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    /* generate the key pair */
    genParams.PublicKey.RSA.Modulus.Length  = key_bits / 8;

    returnStatus = 
        pSSLCrypto->Request
            (
                ANSC_CRYPTO_PUB_ALGORITHM_RSA_SIGNATURE,
                ANSC_CRYPTO_PUB_OPERATION_GENERATE,
                &genParams,
                NULL,
                NULL
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* create the keys */
    /*RDKB-6310, CID-24239, CID-24345; re-arranging logic to avoid memory leaks*/
    pRsaPubKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pRsaPubKey == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pRsaPrvKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pRsaPrvKey == NULL)
    {
        AnscFreeMemory(pRsaPubKey);
        return ANSC_STATUS_FAILURE;
    }

    pRsaPubKey->KeyType             = ANSC_CERT_keyType_rsa;
    pRsaPubKey->KeyBits             = genParams.PublicKey.RSA.Modulus.Length * 8;
    pRsaPubKey->KeySize             = sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS);
    pRsaPubKey->KeyData             = (void*)AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    if( pRsaPubKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pRsaPubKey->KeyData,
                &genParams,
                sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
            );
    }

    pRsaPrvKey->KeyType             = ANSC_CERT_keyType_rsa;
    pRsaPrvKey->KeyBits             = genParams.PublicKey.RSA.Modulus.Length * 8;
    pRsaPrvKey->KeySize             = sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS);
    pRsaPrvKey->KeyData             = (void*)AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    if( pRsaPrvKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pRsaPrvKey->KeyData,
                &genParams,
                sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
            );
    }

    *rsa_pub_key = pRsaPubKey;
    *rsa_prv_key = pRsaPrvKey;

    return  returnStatus;
}
