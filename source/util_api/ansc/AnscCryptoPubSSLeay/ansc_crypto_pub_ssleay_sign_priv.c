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

    Module:	ansc_crypto_pub_ssleay_sign.c

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
        *   07/09/03    update the RSA sign function by Bin Zhu

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"



/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayDSASign
        (
            PVOID                       pDataToSign,
            ULONG                       ulSizeOfData,
            PVOID                       pSignature,
            PULONG                      pulSizeOfSignature,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to generate a private key
    signature by applying DSA signature algorithm.

    argument:   PVOID                   pDataToSign
                Pointer to the content of data we're going to sign.

                ULONG                   ulSizeOfData
                Specify the size of the data we're going to sign.

                PVOID                   pSignature
                Output buffer which is holding the signature we're
                going to generate, caller is responsible for allocating
                the buffer.

                PULONG                  pulSizeOfSignature
                Return the size of the signature.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the private key data structure we
                are going to use to generate the signature.

      return:   status of operation.

**********************************************************************/


ANSC_STATUS
AnscCryptoPubSSLeayDSASign
    (
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToSign);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pSignature);
    UNREFERENCED_PARAMETER(pulSizeOfSignature);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
AnscCryptoPubSSLeayRSASignWithType
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

