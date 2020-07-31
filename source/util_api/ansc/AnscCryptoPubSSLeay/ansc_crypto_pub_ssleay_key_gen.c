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

    Module:	ansc_crypto_pub_ssleay_key_gen.c

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

    ---------------------------------------------------------------

    Revision history:

        *   05/13/02    initial revision

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayGenKeyPairRandomly
        (
            PAL_INTERNAL_PUBLIC_KEY     pPublicKey,
            PAL_INTERNAL_PRIVATE_KEY    pPrivateKey,
            AL_INTERNAL_PKI_KEY_TYPE    keyType,
            ULONG                       ulKeyOctets
        )

    This routine is called to generate public/private key pair randomly.

    argument:   PAL_INTERNAL_PUBLIC_KEY pPublicKey
                Receive the content of random public key.

                PAL_INTERNAL_PRIVATE_KEYpPrivateKey
                Receive the content of random private key.

                AL_INTERNAL_PKI_KEY_TYPEkeyType
                Specify the internal PKI key type.

                ULONG                   ulKeyOctets
                Specify how many octets should be included in the key.

      return:   status of operation.

**********************************************************************/

DSA *DSA_generate_parameters(int bits,
                             unsigned char *seed, int seed_len,
                             int *counter_ret, unsigned long *h_ret, void
                              (*callback) (int, int, void *), void *cb_arg);

static unsigned char DSA_key_seed[20]={
	0xd5,0x01,0x4e,0x4b,0x60,0xef,0x2b,0xa8,
    0xb6,0x21,0x1b,0x40,0x62,0xba,0x32,0x24,
    0xe0,0x42,0x7d,0xd3
	};

ANSC_STATUS
AnscCryptoPubSSLeayGenKeyPairRandomly
    (
        ANSC_CRYPTO_PUB_KEY_TYPE        keyType,
        PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pParams,
        ULONG                           ulKeyOctets
    )
{
    UNREFERENCED_PARAMETER(ulKeyOctets);
    UNREFERENCED_PARAMETER(pParams);
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/
    
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    /*
     * we don't consider DH key right now
     */
    switch ( keyType )
    {
        case    ANSC_CRYPTO_PUB_KEY_RSA :
        case    ANSC_CRYPTO_PUB_KEY_DSA :
                break;

        default :
                /*
                 * caller must specify the key-pair type, we now only support
                 * RSA_KEY and DSA_KEY
                 */
                returnStatus = ANSC_STATUS_FAILURE;

                break;
    }

    return returnStatus;
}

/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayGenRSAKeyPairRandomly
        (
            ANSC_CRYPTO_PRIME*		pPublicKey,
            ANSC_CRYPTO_PRIME*		pPrivateKey,
            ULONG                   ulKeyOctets
        )

    This routine is called to generate public/private key pair randomly.

    argument:   ANSC_CRYPTO_PRIME*		 pPublicKey
                Receive the content of random public key.

                ANSC_CRYPTO_PRIME*		pPrivateKey
                Receive the content of random private key.

                ULONG                   ulKeyOctets
                Specify how many octets should be included in the key.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayGenRSAKeyPairRandomly
    (
        ANSC_CRYPTO_PRIME*			pPublicKey,
        ANSC_CRYPTO_PRIME*			pPrivateKey,
        ULONG                       ulKeyOctets
    )
{
    UNREFERENCED_PARAMETER(pPublicKey);
    UNREFERENCED_PARAMETER(pPrivateKey);
    UNREFERENCED_PARAMETER(ulKeyOctets);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayGenDSAKeyPairRandomly
        (
            ANSC_CRYPTO_PRIME*					pPublicKey,
            ANSC_CRYPTO_PRIME*					pPrivateKey,
            ULONG                       ulKeyOctets
        )

    This routine is called to generate public/private key pair randomly.

    argument:   ANSC_CRYPTO_PRIME*		 pPublicKey
                Receive the content of random public key.

                ANSC_CRYPTO_PRIME*		pPrivateKey
                Receive the content of random private key.

                ULONG                   ulKeyOctets
                Specify how many octets should be included in the key.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayGenDSAKeyPairRandomly
    (
        ANSC_CRYPTO_PRIME*					pPublicKey,
        ANSC_CRYPTO_PRIME*					pPrivateKey,
        ULONG                       ulKeyOctets
    )
{
    UNREFERENCED_PARAMETER(pPublicKey);
    UNREFERENCED_PARAMETER(pPrivateKey);
    DSA                             *pDSA;
    int                             nCounter;
    unsigned long                   ulRet;

    pDSA = (DSA*)DSA_generate_parameters
              (
                  ulKeyOctets * 8, 
                  DSA_key_seed, 
                  sizeof(DSA_key_seed)/sizeof(DSA_key_seed[0]),
                  &nCounter, 
                  &ulRet, 
                  NULL, 
                  NULL
              );

    return (!pDSA) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}
