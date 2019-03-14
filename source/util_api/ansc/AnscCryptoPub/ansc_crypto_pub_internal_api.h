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

    module:	ansc_crypto_pub_internal_api.h

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

**********************************************************************/


#ifndef  _ANSC_CRYPTO_PUB_INTERNAL_API_
#define  _ANSC_CRYPTO_PUB_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_PUB_BASE.C
***********************************************************/

ANSC_HANDLE
AnscCryptoPubCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscCryptoPubRemove
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscCryptoPubInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCryptoPubRequest
    (
        ANSC_CRYPTO_PUB_ALGORITHM   Algorithm,
        ANSC_CRYPTO_PUB_OPERATION   Operation,
        ANSC_HANDLE                 hParams,
        ANSC_CRYPTO_FIN_CALLBACK    FinishCB,
        ANSC_HANDLE                 hFinishCBParam
    );

BOOL
AnscCryptoPubKeyPairMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_PUB_KEY_TYPE    KeyType,
        ANSC_HANDLE                 hPublicKey,
        ANSC_HANDLE                 hPrivateKey
    );

ANSC_STATUS
AnscCryptoPubKeyRSASign
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubKeyRSAVerify
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubKeyDSASign
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

ANSC_STATUS
AnscCryptoPubKeyDSAVerify
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

#endif
