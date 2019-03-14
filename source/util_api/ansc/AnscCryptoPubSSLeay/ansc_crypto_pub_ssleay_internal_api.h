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

    module:	ansc_crypto_pub_ssleay_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Crypto Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/13/02    initial revision.
        05/23/02    zhubin added DSA Sign/Verify function
        09/23/03    zhubin added RSA private key encryption and public
                    key decryption.

**********************************************************************/


#ifndef  _ANSC_CRYPTO_PUB_SSLEAY_INTERNAL_API_
#define  _ANSC_CRYPTO_PUB_SSLEAY_INTERNAL_API_


/***********************************************************
   FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_BASE.C
***********************************************************/

ANSC_HANDLE
AnscCryptoPubSSLeayCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );


ANSC_STATUS
AnscCryptoPubSSLeayRemove
    (
        ANSC_HANDLE                 hOwnerContext
    );


ANSC_STATUS
AnscCryptoPubSSLeayRequest
    (
        ANSC_CRYPTO_PUB_ALGORITHM   Algorithm,
        ANSC_CRYPTO_PUB_OPERATION   Operation,
        ANSC_HANDLE                 hParams,
        ANSC_CRYPTO_FIN_CALLBACK    FinishCB,
        ANSC_HANDLE                 hFinishCBParam
    );


BOOL
AnscCryptoPubSSLeayKeyPairMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_PUB_KEY_TYPE    KeyType,
        ANSC_HANDLE                 hPublicKey,
        ANSC_HANDLE                 hPrivateKey
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_KEY.C
***********************************************************/
PVOID
AnscCryptoCreateSSLeayDSA
    (
        PVOID                       pKey,
        BOOL                        bPublicKey
    );


PVOID
AnscCryptCreateSSLeayRSA
    (
        PVOID                       pKey,
        BOOL                        bPublicKey
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_SIGN.C
***********************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayDSASign
    (
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSASign
    (
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSASignWithType
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


/***********************************************************
  FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_VERIFY.C
***********************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayDSAVerify
    (
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSAVerifyWithType
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSAVerify
    (
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


/***********************************************************
 FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_ENCRYPT.C
***********************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSAEncrypt
    (
        PVOID                       pDataToEncrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pCipherData,
        PULONG                      pulSizeOfCipher,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSADecrypt
    (
        PVOID                       pDataToDecrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pClearData,
        PULONG                      pulSizeOfClear,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubSSLeayRSAPriEncrypt
    (
        PVOID                       pDataToEncrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pCipherData,
        PULONG                      pulSizeOfCipher,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


ANSC_STATUS
AnscCryptoPubSSLeayRSAPubDecrypt
    (
        PVOID                       pDataToDecrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pClearData,
        PULONG                      pulSizeOfClear,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

/***********************************************************
 FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_KEY_GEN.C
***********************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayGenKeyPairRandomly
    (
        ANSC_CRYPTO_PUB_KEY_TYPE        keyType,
        PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pParams,
        ULONG                           ulKeyOctets
    );


ANSC_STATUS
AnscCryptoPubSSLeayGenRSAKeyPairRandomly
    (
        ANSC_CRYPTO_PRIME*			pPublicKey,
        ANSC_CRYPTO_PRIME*			pPrivateKey,
        ULONG                       ulKeyOctets
    );


ANSC_STATUS
AnscCryptoPubSSLeayGenDSAKeyPairRandomly
    (
        ANSC_CRYPTO_PRIME*					pPublicKey,
        ANSC_CRYPTO_PRIME*					pPrivateKey,
        ULONG                       ulKeyOctets
    );


/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_SSLEAY_DH.C
***********************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayGenDHKeyPairRandomly
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    );


ANSC_STATUS
AnscCryptoPubSSLeayAgreeDHSharedSecret
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    );

ANSC_STATUS
AnscCryptoPubSSLeayRSASignAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubSSLeayRSAVerifyAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubSSLeayDSASignAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubSSLeayDSAVerifyAPI
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

#endif
