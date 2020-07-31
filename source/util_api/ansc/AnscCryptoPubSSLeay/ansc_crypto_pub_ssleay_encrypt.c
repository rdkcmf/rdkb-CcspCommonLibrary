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

    Module:	ansc_crypto_pub_ssleay_encrypt.c

        For Virtual Private Network Driver
        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    Functional Description:

        Implementation of public key request

    ---------------------------------------------------------------

    Environment:

        System Independent

    ---------------------------------------------------------------

    Author:

        Kang Quan

    ---------------------------------------------------------------

    Revision history:

        *   05/13/02    initial revision
        *   09/23/03    zhubin added RSA private key encryption and public
                        key decryption.

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayRSAEncrypt
        (
            PVOID                       pDataToEncrypt,
            ULONG                       ulSizeOfData,
            PVOID                       pCipherData,
            PULONG                      pulSizeOfCipher,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to encrypt data by applying RSA
    public key encryption algorithm.

    argument:   PVOID                   pDataToEncrypt
                Pointer to the content of data we're going to encrypt.

                ULONG                   ulSizeOfData
                Specify the size of the data we're going to encrypt.

                PVOID                   pCipherData
                Output buffer which is holding the cipher data we're
                going to generate, caller is responsible for allocating
                the buffer.

                PULONG                  pulSizeOfCipher
                Return the size of the cipher.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal public key data structure we
                are going to use to encrypt the data.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSAEncrypt
    (
        PVOID                       pDataToEncrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pCipherData,
        PULONG                      pulSizeOfCipher,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToEncrypt);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pCipherData);
    UNREFERENCED_PARAMETER(pulSizeOfCipher);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayRSADecrypt
        (
            PVOID                       pDataToDecrypt,
            ULONG                       ulSizeOfData,
            PVOID                       pClearData,
            PULONG                      pulSizeOfClear,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to decrypt data by applying RSA
    public key encryption algorithm.

    argument:   PVOID                   pDataToDecrypt
                Pointer to the content of data we're going to decrypt.

                ULONG                   ulSizeOfData
                Specify the size of the data we're going to decrypt.

                PVOID                   pClearData
                Output buffer which is holding the clear text we're
                going to generate, caller is responsible for allocating
                the buffer.

                PULONG                  pulSizeOfClear
                Return the size of the clear text data.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal private key data structure we
                are going to use to decrypt the data.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSADecrypt
    (
        PVOID                       pDataToDecrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pClearData,
        PULONG                      pulSizeOfClear,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToDecrypt);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pClearData);
    UNREFERENCED_PARAMETER(pulSizeOfClear);
    UNREFERENCED_PARAMETER(pParams);
    return  ANSC_STATUS_FAILURE;
}


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayRSAPriEncrypt
        (
            PVOID                       pDataToEncrypt,
            ULONG                       ulSizeOfData,
            PVOID                       pCipherData,
            PULONG                      pulSizeOfCipher,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to encrypt data with RSA private key

    argument:   PVOID                   pDataToEncrypt
                Pointer to the content of data we're going to encrypt.

                ULONG                   ulSizeOfData
                Specify the size of the data we're going to encrypt.

                PVOID                   pCipherData
                Output buffer which is holding the cipher data we're
                going to generate, caller is responsible for allocating
                the buffer.

                PULONG                  pulSizeOfCipher
                Return the size of the cipher.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal public key data structure we
                are going to use to encrypt the data.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSAPriEncrypt
    (
        PVOID                       pDataToEncrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pCipherData,
        PULONG                      pulSizeOfCipher,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToEncrypt);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pCipherData);
    UNREFERENCED_PARAMETER(pulSizeOfCipher);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayRSAPubDecrypt
        (
            PVOID                       pDataToDecrypt,
            ULONG                       ulSizeOfData,
            PVOID                       pClearData,
            PULONG                      pulSizeOfClear,
            PANSC_CRYPTO_PUB_KEY_PARAMS pParams
        )

    This API function will be called to decrypt data with RSA public key.

    argument:   PVOID                   pDataToDecrypt
                Pointer to the content of data we're going to decrypt.

                ULONG                   ulSizeOfData
                Specify the size of the data we're going to decrypt.

                PVOID                   pClearData
                Output buffer which is holding the clear text we're
                going to generate, caller is responsible for allocating
                the buffer.

                PULONG                  pulSizeOfClear
                Return the size of the clear text data.

                PANSC_CRYPTO_PUB_KEY_PARAMS pParams
                Pointer to the internal private key data structure we
                are going to use to decrypt the data.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayRSAPubDecrypt
    (
        PVOID                       pDataToDecrypt,
        ULONG                       ulSizeOfData,
        PVOID                       pClearData,
        PULONG                      pulSizeOfClear,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    )
{
    UNREFERENCED_PARAMETER(pDataToDecrypt);
    UNREFERENCED_PARAMETER(ulSizeOfData);
    UNREFERENCED_PARAMETER(pClearData);
    UNREFERENCED_PARAMETER(pulSizeOfClear);
    UNREFERENCED_PARAMETER(pParams);
    return ANSC_STATUS_FAILURE;
}

