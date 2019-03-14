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

    module:	ansc_crypto_pub_ssleay_util.h

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

#ifndef  _ANSC_CRYPTO_PUB_SSLEAY_UTIL_
#define  _ANSC_CRYPTO_PUB_SSLEAY_UTIL_


BIGNUM *
AnscCryptoPubPrime2BigNum
    (
        PANSC_CRYPTO_PRIME          pLargePrime
    );


BIGNUM *
AnscCryptoPubInteger2BigNum
    (
        PANSC_CRYPTO_INTEGER        pLargeInteger
    );


void
AnscCryptoPubBigEndianInteger2Integer
    (
        PANSC_CRYPTO_INTEGER        pInteger,
        PANSC_CRYPTO_INTEGER        pBigEndianInteger
    );


void
AnscCryptoPubBigEndianPrime2Prime
    (
        PANSC_CRYPTO_PRIME          pPrime,
        PANSC_CRYPTO_PRIME          pBigEndianPrime
    );


void
AnscCryptoPubBigNum2Integer
    (
        BIGNUM                      *pBigNum,
        PANSC_CRYPTO_INTEGER        pInteger
    );


void
AnscCryptoPubBigNum2Prime
    (
        BIGNUM                      *pBigNum,
        PANSC_CRYPTO_PRIME          pPrime
    );


void
AnscCryptoPubSetInteger
    (
        PANSC_CRYPTO_INTEGER        pInteger,
        PUCHAR                      pData,
        ULONG                       ulData
    );


void
AnscCryptoPubSetPrime
    (
        PANSC_CRYPTO_PRIME          pPrime,
        PUCHAR                      pData,
        ULONG                       ulData
    );


#endif

