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

    module:	ansc_crypto_hash.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoHashMd5
        *   AnscCryptoHashSha1
        *   AnscCryptoHashMd4

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/27/01    initial revision.

**********************************************************************/


#include "ansc_crypto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHashMd5
            (
                PVOID                       buffer,
                ULONG                       size,
                ULONG                       modulus
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                ULONG                       modulus
                Specifies the size of the hash table.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHashMd5
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    )
{
    ULONG                           ulTemp1 = 0;
    ULONG                           ulTemp2 = 0;
    ANSC_CRYPTO_HASH                hashResult;

    AnscCryptoMd5Digest(buffer, size, &hashResult);

    ulTemp1 = (ULONG)*(PUSHORT)&hashResult.Value[0];
    ulTemp2 = (ULONG)*(PUSHORT)&hashResult.Value[4];

    return  (ulTemp1 * ulTemp2) % modulus;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHashSha1
            (
                PVOID                       buffer,
                ULONG                       size,
                ULONG                       modulus
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                ULONG                       modulus
                Specifies the size of the hash table.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHashSha1
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    )
{
    ULONG                           ulTemp1 = 0;
    ULONG                           ulTemp2 = 0;
    ANSC_CRYPTO_HASH                hashResult;

    AnscCryptoSha1Digest(buffer, size, &hashResult);

    ulTemp1 = (ULONG)*(PUSHORT)&hashResult.Value[0];
    ulTemp2 = (ULONG)*(PUSHORT)&hashResult.Value[4];

    return  (ulTemp1 * ulTemp2) % modulus;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHashMd4
            (
                PVOID                       buffer,
                ULONG                       size,
                ULONG                       modulus
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                ULONG                       modulus
                Specifies the size of the hash table.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHashMd4
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    )
{
    ULONG                           ulTemp1 = 0;
    ULONG                           ulTemp2 = 0;
    ANSC_CRYPTO_HASH                hashResult = {0}; /*RDKB-6145, CID-24798, initialize the variable*/

    AnscCryptoMd4Digest(buffer, size, &hashResult);

    ulTemp1 = (ULONG)*(PUSHORT)&hashResult.Value[0];
    ulTemp2 = (ULONG)*(PUSHORT)&hashResult.Value[4];

    return  (ulTemp1 * ulTemp2) % modulus;
}
