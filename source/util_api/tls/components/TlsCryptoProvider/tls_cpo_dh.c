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

    module:	tls_cpo_dh.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced DH-related crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoDecapDHPublicKey
        *   TlsCpoEncapDHPublicKey
        *   TlsCpoGenerateDHKeys1
        *   TlsCpoGenerateDHKeys2
        *   TlsCpoAgreeDHSecret

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

/*
 *  The pki header files should be included
 *
 *  please get them from "\bisga\isc\asn.1\include" 
 */
#include "ansc_asn1_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoDecapDHPublicKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_X509_KEY*              dh_key,
                ANSC_ASN1_INT**             dh_prime,
                ANSC_ASN1_INT**             dh_generator,
                ANSC_ASN1_INT**             dh_public
            );

    description:

        This function is called to break a DH public key into various
        integer values.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              dh_key
                Specifies the DH public key to be dismantled.

                ANSC_ASN1_INT**             dh_prime
                Specifies the Diffie-Hellman Prime value to be returned.
                This function is responsible for allocating memory for
                an ANSC_ASN1_INT structure and returning its pointer in
                this parameter.

                ANSC_ASN1_INT**             dh_generator
                Specifies the Diffie-Hellman Generator value to be
                returned. This function is responsible for allocating
                memory for an ANSC_ASN1_INT structure and returning its
                pointer in this parameter.

                ANSC_ASN1_INT**             dh_public
                Specifies the Diffie-Hellman Public value to be
                returned. This function is responsible for allocating
                memory for an ANSC_ASN1_INT structure and returning its
                pointer in this parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoDecapDHPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_key,
        ANSC_ASN1_INT**             dh_prime,
        ANSC_ASN1_INT**             dh_generator,
        ANSC_ASN1_INT**             dh_public
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_ASN1_INT*                  pDhPrime     = (ANSC_ASN1_INT*             )NULL;
    ANSC_ASN1_INT*                  pDhGenerator = (ANSC_ASN1_INT*             )NULL;
    ANSC_ASN1_INT*                  pDhPublic    = (ANSC_ASN1_INT*             )NULL;
    PANSC_CRYPTO_DH_PARAMETERS      pDHParam     = NULL;

    pDHParam = (PANSC_CRYPTO_DH_PARAMETERS)dh_key->KeyData;

    if( pDHParam == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( dh_key->KeyType != ANSC_CERT_keyType_diffieHellman)
    {
        AnscTrace("Invalid key type in TlsCpoDecapDHPublicKey().\n");

        return ANSC_STATUS_FAILURE;
    }

    /* create all the PANSC_ASN1_INTs */
    pDhPrime = 
        (PANSC_ASN1_INT)AnscAsn1AllocInt
            (
                pDHParam->GroupPrime.Data,
                pDHParam->GroupPrime.Length,
                FALSE
            );

    pDhGenerator = 
        (PANSC_ASN1_INT)AnscAsn1AllocInt
            (
                pDHParam->GroupGenerator.Data.ucData,
                pDHParam->GroupGenerator.Length,
                FALSE
            );

    pDhPublic   =
        (PANSC_ASN1_INT)AnscAsn1AllocInt
            (
                pDHParam->PublicKey.Data.ucData,
                pDHParam->PublicKey.Length,
                FALSE
            );

    *dh_prime     = pDhPrime;
    *dh_generator = pDhGenerator;
    *dh_public    = pDhPublic;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsCpoEncapDHPublicKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_ASN1_INT*              dh_prime,
                ANSC_ASN1_INT*              dh_generator,
                ANSC_ASN1_INT*              dh_public
            );

    description:

        This function is called to assemble a DH public key from the
        prime, generator and public values. This function is
        responsible for allocating memory for an ANSC_X509_KEY
        structure and return its pointer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_ASN1_INT*              dh_prime
                Specifies the DH prime value of the public key to be
                assembled and returned.

                ANSC_ASN1_INT*              dh_generator
                Specifies the DH generator value of the public key to
                be assembled and returned.

                ANSC_ASN1_INT*              dh_public
                Specifies the DH public value of the public key to be
                assembled and returned.

    return:     dh public key.

**********************************************************************/

ANSC_HANDLE
TlsCpoEncapDHPublicKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_ASN1_INT*              dh_public
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_X509_KEY*                  pDhPubKey    = (ANSC_X509_KEY*             )NULL;
    PANSC_CRYPTO_DH_PARAMETERS      pDHParam     = NULL;

    pDHParam = (PANSC_CRYPTO_DH_PARAMETERS)
        AnscAllocateMemory(sizeof(ANSC_CRYPTO_DH_PARAMETERS));

    if( pDHParam == NULL)
    {
        goto EXIT;
    }

    pDHParam->GroupPrime.Length         = dh_prime->Length;
    pDHParam->GroupGenerator.Length     = dh_generator->Length;
    pDHParam->PublicKey.Length          = dh_public->Length;

    AnscCopyMemory
        (
            pDHParam->GroupPrime.Data,
            dh_prime->Octets,
            dh_prime->Length
        );

    AnscCopyMemory
        (
            pDHParam->GroupGenerator.Data.ucData,
            dh_generator->Octets,
            dh_generator->Length
        );

    AnscCopyMemory
        (
            pDHParam->PublicKey.Data.ucData,
            dh_public->Octets,
            dh_public->Length
        );

    /* create the PANSC_X509_KEY */
    pDhPubKey = (PANSC_X509_KEY)AnscAllocateMemory(sizeof(ANSC_X509_KEY));

    if( pDhPubKey == NULL)
    {
        goto EXIT;
    }

    pDhPubKey->KeyType              = ANSC_CERT_keyType_diffieHellman;
    pDhPubKey->KeyBits              = dh_prime->Length * 8;   /* Bits value */
    pDhPubKey->KeySize              = sizeof(ANSC_CRYPTO_DH_PARAMETERS);
    pDhPubKey->KeyData              = (void*)pDHParam;

    /* reset params */
    pDHParam = NULL;

EXIT:

    if( pDHParam != NULL)
    {
        AnscFreeMemory(pDHParam);
    }

    return  (ANSC_HANDLE)pDhPubKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGenerateDHKeys1
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_X509_KEY**             dh_pub_key,
                ANSC_X509_KEY**             dh_prv_key
            );

    description:

        This function is called to randomly generate a DH key pair.
        This function SHOULD use OAKLEY group 2 by default.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY**             dh_pub_key
                Specifies the Diffie-Hellman public key to be generated.
                This function is responsible for allocating memory for
                an ANSC_X509_KEY structure and returning its pointer in
                this parameter.

                ANSC_X509_KEY**             dh_prv_key
                Specifies the Diffie-Hellman private key to be
                generated. This function is responsible for allocating
                memory for an ANSC_X509_KEY structure and returning its
                pointer in this parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoGenerateDHKeys1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_X509_KEY*                  pDhPubKey    = (ANSC_X509_KEY*             )NULL;
    ANSC_X509_KEY*                  pDhPrvKey    = (ANSC_X509_KEY*             )NULL;
    ANSC_CRYPTO_DH_PARAMETERS       dhParam;
    
    if( dh_pub_key == NULL || dh_prv_key == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* we use OAKLEY group 2 by default */
    AnscZeroMemory(&dhParam, sizeof(ANSC_CRYPTO_DH_PARAMETERS));

    PKIInitDHKeyParameter( &dhParam, 128);

    /* generate the key pair */
    returnStatus = 
        pSSLCrypto->Request
            (
                ANSC_CRYPTO_PUB_ALGORITHM_DH,
                ANSC_CRYPTO_PUB_OPERATION_GENERATE,
                &dhParam,
                NULL,
                NULL
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* create the keys */
    /*RDKB-6309, CID-24144, CID-24373; re-arranging logic to avoid memory leaks*/
    pDhPubKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pDhPubKey == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pDhPrvKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pDhPrvKey == NULL)
    {
        AnscFreeMemory(pDhPubKey);
        return ANSC_STATUS_FAILURE;
    }

    pDhPubKey->KeyType              = ANSC_CERT_keyType_diffieHellman;
    pDhPubKey->KeyBits              = 128 * 8;   /* Bits value */
    pDhPubKey->KeySize              = sizeof(ANSC_CRYPTO_DH_PARAMETERS);
    pDhPubKey->KeyData              = (void*)AnscAllocateMemory(pDhPubKey->KeySize);

    if(pDhPubKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pDhPubKey->KeyData,
                &dhParam,
                pDhPubKey->KeySize
            );
    }

    pDhPrvKey->KeyType              = ANSC_CERT_keyType_diffieHellman;
    pDhPrvKey->KeyBits              = 128 * 8;   /* Bits value */
    pDhPrvKey->KeySize              = sizeof(ANSC_CRYPTO_DH_PARAMETERS);
    pDhPrvKey->KeyData              = (void*)AnscAllocateMemory(pDhPrvKey->KeySize);

    if(pDhPrvKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pDhPrvKey->KeyData,
                &dhParam,
                pDhPrvKey->KeySize
            );
    }

    *dh_pub_key                     = pDhPubKey;
    *dh_prv_key                     = pDhPrvKey;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGenerateDHKeys2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_ASN1_INT*              dh_prime,
                ANSC_ASN1_INT*              dh_generator,
                ANSC_X509_KEY**             dh_pub_key,
                ANSC_X509_KEY**             dh_prv_key
            );

    description:

        This function is called to randomly generate a DH key pair.
        The difference between this function and GenerateDHKeys1() is
        that the prime and generator values are provided to this
        function.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_ASN1_INT*              dh_prime
                Specifies the DH prime value for which the key pair to
                be generated.

                ANSC_ASN1_INT*              dh_generator
                Specifies the DH generator value for which the key pair
                to be generated.

                ANSC_X509_KEY**             dh_pub_key
                Specifies the Diffie-Hellman public key to be generated.
                This function is responsible for allocating memory for
                an ANSC_X509_KEY structure and returning its pointer in
                this parameter.

                ANSC_X509_KEY**             dh_prv_key
                Specifies the Diffie-Hellman private key to be
                generated. This function is responsible for allocating
                memory for an ANSC_X509_KEY structure and returning its
                pointer in this parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoGenerateDHKeys2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_ASN1_INT*              dh_prime,
        ANSC_ASN1_INT*              dh_generator,
        ANSC_X509_KEY**             dh_pub_key,
        ANSC_X509_KEY**             dh_prv_key
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    ANSC_X509_KEY*                  pDhPubKey    = (ANSC_X509_KEY*             )NULL;
    ANSC_X509_KEY*                  pDhPrvKey    = (ANSC_X509_KEY*             )NULL;
    ANSC_CRYPTO_DH_PARAMETERS       dhParam;

    if( dh_pub_key == NULL || dh_prv_key == NULL || dh_prime == NULL || dh_generator == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Init the dh key parameters */
    {
        AnscZeroMemory(&dhParam, sizeof(ANSC_CRYPTO_DH_PARAMETERS));
        dhParam.GroupPrime.Length       = dh_prime->Length;
        AnscCopyMemory(dhParam.GroupPrime.Data, dh_prime->Octets, dh_prime->Length);
        dhParam.GroupGenerator.Length   = dh_generator->Length;
        AnscCopyMemory(dhParam.GroupGenerator.Data.ucData, dh_generator->Octets, dh_generator->Length);
    }

    if(!PKIIsDHParameterAcceptable(&dhParam))
    {
        AnscTrace("Warning::: The DHkey parameters are not in Oakley group 1 and 2.\n");

        {
            char    pTemp[512]  = { 0 };

            if( dh_prime->Length > 200)
            {
                AnscFromHexToString( dh_prime->Octets, pTemp, 200, FALSE);
                AnscTrace("The DHKey prime is(L%lu):\n %s...\n", dh_prime->Length,pTemp);
            }
            else
            {
                AnscFromHexToString( dh_prime->Octets, pTemp, dh_prime->Length, FALSE);
                AnscTrace("The DHKey prime is(L%lu):\n %s...\n", dh_prime->Length,pTemp);
            }

            AnscZeroMemory(pTemp, 512);
            AnscFromHexToString( dh_generator->Octets, pTemp, dh_generator->Length, FALSE);
            AnscTrace("The DHKey generator is(L%lu):\n %s...\n", dh_generator->Length,pTemp);

        }
    }

    /* generate the key pair */
    returnStatus = 
        pSSLCrypto->Request
            (
                ANSC_CRYPTO_PUB_ALGORITHM_DH,
                ANSC_CRYPTO_PUB_OPERATION_GENERATE,
                &dhParam,
                NULL,
                NULL
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* create the keys */
    /*RDKB-6309, CID-24132, CID-24240; re-arranging logic to avoid memory leaks*/
    pDhPubKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pDhPubKey == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pDhPrvKey = (ANSC_X509_KEY*)AnscAllocateMemory(sizeof(ANSC_X509_KEY));
    if(pDhPrvKey == NULL)
    {
        AnscFreeMemory(pDhPubKey);
        return ANSC_STATUS_FAILURE;
    }

    pDhPubKey->KeyType              = ANSC_CERT_keyType_diffieHellman;
    pDhPubKey->KeyBits              = dhParam.GroupPrime.Length * 8;   /* Bits value */
    pDhPubKey->KeySize              = sizeof(ANSC_CRYPTO_DH_PARAMETERS);
    pDhPubKey->KeyData              = (void*)AnscAllocateMemory(pDhPubKey->KeySize);

    if(pDhPubKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pDhPubKey->KeyData,
                &dhParam,
                pDhPubKey->KeySize
            );
    }

    pDhPrvKey->KeyType              = ANSC_CERT_keyType_diffieHellman;
    pDhPrvKey->KeyBits              = dhParam.GroupPrime.Length * 8;   /* Bits value */
    pDhPrvKey->KeySize              = sizeof(ANSC_CRYPTO_DH_PARAMETERS);
    pDhPrvKey->KeyData              = (void*)AnscAllocateMemory(pDhPrvKey->KeySize);

    if(pDhPrvKey->KeyData != NULL)
    {
        AnscCopyMemory
            (
                pDhPrvKey->KeyData,
                &dhParam,
                pDhPrvKey->KeySize
            );
    }

    *dh_pub_key                     = pDhPubKey;
    *dh_prv_key                     = pDhPrvKey;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to complete DH key negotiation by
        agreeing on a shared secret.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_X509_KEY*              dh_pub_key1
                Specifies the DH public key of party 1.

                ANSC_X509_KEY*              dh_pub_key2
                Specifies the DH public key of party 2.

                ANSC_X509_KEY*              dh_prv_key1
                Specifies the DH private key of party 1.
                PUCHAR                      shared_secret
                Specifies the shared secret to be returned.
                PULONG                      secret_size
                As an input parameter, it specifies the maximum
                acceptable size of the shared secret; as an output
                parameter, it returns the size of the final secret.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoAgreeDHSecret
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_X509_KEY*              dh_pub_key1,
        ANSC_X509_KEY*              dh_pub_key2,
        ANSC_X509_KEY*              dh_prv_key1,
        PUCHAR                      shared_secret,
        PULONG                      secret_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_PUB_OBJECT         pSSLCrypto   = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;
    PANSC_CRYPTO_DH_PARAMETERS      pDhParam;
    PANSC_CRYPTO_DH_PARAMETERS      pOtherParam;

    if( dh_pub_key1 == NULL || dh_pub_key2 == NULL || dh_prv_key1 == NULL || 
        secret_size == NULL || shared_secret == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pDhParam    = (PANSC_CRYPTO_DH_PARAMETERS)dh_pub_key1->KeyData;

    /* copy the other values */
    pOtherParam = (PANSC_CRYPTO_DH_PARAMETERS)dh_prv_key1->KeyData;

    pDhParam->PrivateKey.Length     = pOtherParam->PrivateKey.Length;

    AnscCopyMemory
        (
            pDhParam->PrivateKey.Data.ucData,
            pOtherParam->PrivateKey.Data.ucData,
            pDhParam->PrivateKey.Length
        );

    pOtherParam = (PANSC_CRYPTO_DH_PARAMETERS)dh_pub_key2->KeyData;

    pDhParam->PeerPublicKey.Length = pOtherParam->PublicKey.Length;

    AnscCopyMemory
        (
            pDhParam->PeerPublicKey.Data.ucData,
            pOtherParam->PublicKey.Data.ucData,
            pDhParam->PeerPublicKey.Length
        );

    /* generate the key agreement */
    returnStatus = 
        pSSLCrypto->Request
            (
                ANSC_CRYPTO_PUB_ALGORITHM_DH,
                ANSC_CRYPTO_PUB_OPERATION_DH_AGREE,
                pDhParam,
                NULL,
                NULL
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to get DH Agreement Key.\n");
        return returnStatus;
    }

    if( *secret_size > pDhParam->SharedSecret.Length)
    {
        *secret_size = pDhParam->SharedSecret.Length;
    }

    AnscCopyMemory
        (
            shared_secret,
            pDhParam->SharedSecret.Data.ucData,
            *secret_size
        );

    return  returnStatus;
}
