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

    Module:	ansc_crypto_pub_ssleay_verify.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    Functional Description:

        Implementation of public key request

    ---------------------------------------------------------------

    Environment:

        System Independent

    ---------------------------------------------------------------

    Author:

        Kang Quan
        Bin  Zhu

    ---------------------------------------------------------------

    Revision history:

        *   05/13/02    initial revision
        *   07/09/03    update the RSA verification function by Bin Zhu

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayDSAVerify
        (
            PVOID                       pDataToVerify,
            ULONG                       ulSizeOfData,
            PVOID                       pSignature,
            ULONG                       ulSizeOfSignature,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to verify a DSA signature.

    argument:   PVOID                   pDataToVerify
                Pointer to the content of data which has the signature
                stamped on.

                ULONG                   ulSizeOfData
                Specify the size of the data.

                PVOID                   pSignature
                Input buffer which is holding the signature we're
                going to verify.

                ULONG                   ulSizeOfSignature
                Specify the size of the signature.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal public key data structure we
                are going to use to verify the signature.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayDSAVerify
    (
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToVerify);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(ulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}



/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayRSAVerifyWithType
        (
            int                         nHashMethod,
            PVOID                       pDataToVerify,
            ULONG                       ulSizeOfData,
            PVOID                       pSignature,
            ULONG                       ulSizeOfSignature,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to verify a RSA signature.

    argument:
                int                     nHashMethod
                Specify the type of hash to encode the data to be
                signed to ASN.1 format (only SHA1 and MD5 supported)

                PVOID                   pDataToVerify
                Pointer to the content of data which has the signature
                stamped on.

                ULONG                   ulSizeOfData
                Specify the size of the data.

                PVOID                   pSignature
                Input buffer which is holding the signature we're
                going to verify.

                ULONG                   ulSizeOfSignature
                Specify the size of the signature.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal public key data structure we
                are going to use to verify the signature.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSAVerifyWithType
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
    return ANSC_STATUS_SUCCESS;
}

