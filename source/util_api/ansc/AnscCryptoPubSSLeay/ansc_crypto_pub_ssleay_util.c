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

    Module:	ansc_crypto_pub_ssleay_util.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    Functional Description:

        Utilities for using SSLeay

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

/*
 * This function will create an instance of BIGNUM, the caller must
 * free the instance by calling "BN_free" itself if the big number 
 * is no longer in use.
 */
BIGNUM *
AnscCryptoPubPrime2BigNum
    (
        PANSC_CRYPTO_PRIME          pLargePrime
    )
{
    BIGNUM                          *pBigNum;
    ANSC_CRYPTO_PRIME               *pPrime;
    ULONG                           PrimeLength;

#ifdef _ANSC_CRYPTO_LITTLE_ENDIAN
    ANSC_CRYPTO_PRIME               BigEndianPrime;
    int                             i;
#endif

    if (!pLargePrime)
        return NULL;

    PrimeLength = pLargePrime->Length;

#ifdef _ANSC_CRYPTO_LITTLE_ENDIAN
    /*
     * convert the large prime from "Little Endian" to "Big Endian"
     */
    BigEndianPrime.Length = PrimeLength;

    for (i = 0; i < PrimeLength; i ++)
        BigEndianPrime.Data[i] = pLargePrime->Data[PrimeLength-i-1];

    pPrime = &BigEndianPrime;
#else
    pPrime = pLargePrime;
#endif

    /*
     * convert "Big Endian" large prime to SSLeay's BIGNUM
     */
    pBigNum = BN_bin2bn((unsigned char *)pPrime->Data, (int)PrimeLength, NULL);

    return pBigNum;
}

/*
 * This function will create an instance of BIGNUM, the caller must
 * free the instance by calling "BN_free" itself if the big number 
 * is no longer in use.
 */
BIGNUM *
AnscCryptoPubInteger2BigNum
    (
        PANSC_CRYPTO_INTEGER        pLargeInteger
    )
{
    BIGNUM                          *pBigNum;
    ANSC_CRYPTO_INTEGER             *pInteger;
    ULONG                           IntegerLength;

#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    int                             i;
    ANSC_CRYPTO_INTEGER             BigEndianInteger;
#endif

    if (!pLargeInteger)
        return NULL;

    IntegerLength = pLargeInteger->Length;

#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    /*
     * convert the large integer from "Little Endian" to "Big Endian"
     */
    BigEndianInteger.Length = IntegerLength;

    for (i = 0; i < IntegerLength; i ++)
        BigEndianInteger.Data.ucData[i] = pLargeInteger->Data.ucData[IntegerLength-i-1];

    pInteger = &BigEndianInteger;
#else
    pInteger = pLargeInteger;
#endif

    /* 
     * convert the "Big Endian" large integer to SSLeay's BIGNUM
     */
    pBigNum = BN_bin2bn((unsigned char *)pInteger->Data.ucData, IntegerLength, NULL);

    return pBigNum;
}

void
AnscCryptoPubBigEndianInteger2Integer
    (
        PANSC_CRYPTO_INTEGER        pInteger,
        PANSC_CRYPTO_INTEGER        pBigEndianInteger
    )
{
#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    int         i;
#endif

    if (!pInteger || !pBigEndianInteger)
        return;

#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    pInteger->Length = pBigEndianInteger->Length;

    for (i = 0; i < pBigEndianInteger->Length; i ++)
        pInteger->Data.ucData[i] = pBigEndianInteger->Data.ucData[pBigEndianInteger->Length-i-1];
#else
    *pInteger = *pBigEndianInteger;
#endif
}

void
AnscCryptoPubBigEndianPrime2Prime
    (
        PANSC_CRYPTO_PRIME          pPrime,
        PANSC_CRYPTO_PRIME          pBigEndianPrime
    )
{
#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    int         i;
#endif

    if (!pPrime || !pBigEndianPrime)
        return;

#ifdef  _ANSC_CRYPTO_LITTLE_ENDIAN
    pPrime->Length = pBigEndianPrime->Length;

    for (i = 0; i < pBigEndianPrime->Length; i ++)
        pPrime->Data[i] = pBigEndianPrime->Data[pBigEndianPrime->Length-i-1];
#else
    *pPrime = *pBigEndianPrime;
#endif
}

void
AnscCryptoPubSetInteger
    (
        PANSC_CRYPTO_INTEGER        pInteger,
        PUCHAR                      pData,
        ULONG                       ulData
    )
{
    if (pInteger)
    {
        pInteger->Length = (ulData > ANSC_CRYPTO_PUB_MAX_INTEGER_SIZE)?ANSC_CRYPTO_PUB_MAX_INTEGER_SIZE:ulData;

        /*copy data in ANSC_CRYPTO_INTEGER*/
        AnscCopyMemory(pInteger->Data.ucData, pData, sizeof(UCHAR) * pInteger->Length);
    }
}

void
AnscCryptoPubSetPrime
    (
        PANSC_CRYPTO_PRIME          pPrime,
        PUCHAR                      pData,
        ULONG                       ulData
    )
{
    if (pPrime)
    {
        pPrime->Length = (ulData > ANSC_CRYPTO_PUB_MAX_PRIME_SIZE)?ANSC_CRYPTO_PUB_MAX_PRIME_SIZE:ulData;

        /*copy data in ANSC_CRYPTO_PRIME*/
        AnscCopyMemory(pPrime->Data, pData, pPrime->Length * sizeof(UCHAR));
    }
}

