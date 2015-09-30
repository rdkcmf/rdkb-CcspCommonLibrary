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

    module:	ansc_crypto_random.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoGetRandomUlong
        *   AnscCryptoGetRandomUshort
        *   AnscCryptoGetRandomOctets

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/27/02    initial revision.

**********************************************************************/


#include "ansc_crypto_global.h"


static  ULONG  g_ulAdaptiveSeed = 0xd1310ba6;   /* first word in the ks0 of the blowfish cipher */


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoGetRandomUlong
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       rand_seed
            );

    description:

        This function performs cryptography computation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       rand_seed
                Specifies a psudo-random number (which is usually a
                memory pointer) to be used as one of seeds.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoGetRandomUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject    = (PANSC_CRYPTO_OBJECT)hThisObject;
    char                            hashString[16];
    ANSC_CRYPTO_HASH                hashResult;

    *(PULONG)&hashString[ 0] = AnscGetTickInMilliSeconds();
    *(PULONG)&hashString[ 4] = rand_seed;
    *(PULONG)&hashString[ 8] = g_ulAdaptiveSeed;
    *(PULONG)&hashString[12] = (ULONG)hashString;

    AnscCryptoMd5Digest(hashString, 16, &hashResult);

    g_ulAdaptiveSeed += *(PULONG)&hashResult.Value[0];

    return  *(PULONG)&hashResult.Value[4];
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        USHORT
        AnscCryptoGetRandomUshort
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       rand_seed
            );

    description:

        This function performs cryptography computation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       rand_seed
                Specifies a psudo-random number (which is usually a
                memory pointer) to be used as one of seeds.

    return:     result.

**********************************************************************/

USHORT
AnscCryptoGetRandomUshort
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject    = (PANSC_CRYPTO_OBJECT)hThisObject;
    char                            hashString[16];
    ANSC_CRYPTO_HASH                hashResult;

    *(PULONG)&hashString[ 0] = AnscGetTickInMilliSeconds();
    *(PULONG)&hashString[ 4] = rand_seed;
    *(PULONG)&hashString[ 8] = g_ulAdaptiveSeed;
    *(PULONG)&hashString[12] = (ULONG)hashString;

    AnscCryptoMd5Digest(hashString, 16, &hashResult);

    g_ulAdaptiveSeed += *(PULONG)&hashResult.Value[0];

    return  *(PUSHORT)&hashResult.Value[4];
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoGetRandomOctets
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       rand_seed,
                PUCHAR                      pOctetStream,
                ULONG                       ulSize
            );

    description:

        This function performs cryptography computation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       rand_seed
                Specifies a psudo-random number (which is usually a
                memory pointer) to be used as one of seeds.

                PUCHAR                      pOctetStream
                Specifies the octet stream to be retrieved.

                ULONG                       ulSize
                Specifies the size of the octet stream.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoGetRandomOctets
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed,
        PUCHAR                      pOctetStream,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject    = (PANSC_CRYPTO_OBJECT)hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < ulSize / sizeof(ULONG); i++ )
    {
        *(PULONG)&pOctetStream[i * sizeof(ULONG)] =
            AnscCryptoGetRandomUlong
                (
                    (ANSC_HANDLE)pMyObject,
                    (ULONG      )&pOctetStream[i * sizeof(ULONG)] + rand_seed
                );
    }

    for ( i = 0; i < ulSize % sizeof(ULONG); i++ )
    {
        pOctetStream[ulSize - 1 - i] =
            (UCHAR)AnscCryptoGetRandomUshort
                (
                    (ANSC_HANDLE)pMyObject,
                    (ULONG      )&pOctetStream[ulSize - 1 - i] + rand_seed
                );
    }

    return  ANSC_STATUS_SUCCESS;
}
