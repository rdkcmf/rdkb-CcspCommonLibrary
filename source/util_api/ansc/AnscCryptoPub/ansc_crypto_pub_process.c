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

    module:	ansc_crypto_pub_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Crypto Object.

        *   AnscCryptoPubRequest

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

#include "ansc_crypto_pub_global.h"


ANSC_STATUS
AnscCryptoPubRequest
    (
        ANSC_CRYPTO_PUB_ALGORITHM   Algorithm,
        ANSC_CRYPTO_PUB_OPERATION   Operation,
        ANSC_HANDLE                 hParams,
        ANSC_CRYPTO_FIN_CALLBACK    FinishCB,
        ANSC_HANDLE                 hFinishCBParam
    )
{
    /* 
     * There is no implementation in this base object.
     * This object should not be used until someone
     * provides a real public key algorithm, no matter
     * it's software algorithm or hardware implementation.
     */
    UNREFERENCED_PARAMETER(Algorithm);
    UNREFERENCED_PARAMETER(Operation);
    UNREFERENCED_PARAMETER(hParams);
    UNREFERENCED_PARAMETER(FinishCB);
    UNREFERENCED_PARAMETER(hFinishCBParam);
    return ANSC_STATUS_FAILURE;
}


BOOL
AnscCryptoPubKeyPairMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_PUB_KEY_TYPE    KeyType,
        ANSC_HANDLE                 hPublicKey,
        ANSC_HANDLE                 hPrivateKey
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(KeyType);
    UNREFERENCED_PARAMETER(hPublicKey);
    UNREFERENCED_PARAMETER(hPrivateKey);
    return FALSE;
}


ANSC_STATUS
AnscCryptoPubKeyRSASign
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    UNREFERENCED_PARAMETER(pDataToSign);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(pulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
AnscCryptoPubKeyRSAVerify
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    UNREFERENCED_PARAMETER(pDataToVerify);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(ulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
AnscCryptoPubKeyDSASign
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    UNREFERENCED_PARAMETER(pDataToSign);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(pulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
AnscCryptoPubKeyDSAVerify
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(nHashMethod);
    UNREFERENCED_PARAMETER(pDataToVerify);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(ulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}


