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

    module:	ansc_crypto_digestion.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoDigest
        *   AnscCryptoHmacMd5Digest
        *   AnscCryptoHmacSha1Digest
        *   AnscCryptoHmacSha256Digest
        *   AnscCryptoDesMacDigest
        *   AnscCryptoKpdkDigest
        *   AnscCryptoMd5Digest
        *   AnscCryptoSha1Digest
        *   AnscCryptoSha256Digest
        *   AnscCryptoMd4Digest
        *   AnscCryptoMd2Digest
        *   AnscCryptoDesMacKDigest


    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.

**********************************************************************/


#include "ansc_crypto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDigest
            (
                ULONG                       algorithm,
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   ULONG                       algorithm
                Specifies the cryptography algorithm to use.

                PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDigest
    (
        ULONG                       algorithm,
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
    ULONG                           ulResult = 0;

    switch ( algorithm )
    {
        case    ANSC_CRYPTO_DIGESTION_HMAC_MD5 :

                ulResult =
                    AnscCryptoHmacMd5Digest
                        (
                            buffer,
                            size,
                            hash,
                            key
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_HMAC_SHA :

                ulResult =
                    AnscCryptoHmacSha1Digest
                        (
                            buffer,
                            size,
                            hash,
                            key
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_DES_MAC :

                ulResult =
                    AnscCryptoDesMacDigest
                        (
                            buffer,
                            size,
                            hash,
                            key
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_KPDK :

                ulResult =
                    AnscCryptoKpdkDigest
                        (
                            buffer,
                            size,
                            hash,
                            key
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_MD5 :

                ulResult =
                    AnscCryptoMd5Digest
                        (
                            buffer,
                            size,
                            hash
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_SHA :

                ulResult =
                    AnscCryptoSha1Digest
                        (
                            buffer,
                            size,
                            hash
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_MD4 :

                ulResult =
                    AnscCryptoMd4Digest
                        (
                            buffer,
                            size,
                            hash
                        );

                break;

        case    ANSC_CRYPTO_DIGESTION_MD2 :

                ulResult =
                    AnscCryptoMd2Digest
                        (
                            buffer,
                            size,
                            hash
                        );

                break;

        default :

                ulResult = 0;

                break;
    }

    return  ulResult;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHmacMd5Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHmacMd5Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
    ULONG                           i                                 = 0;
    UCHAR                           innerPadding[ANSC_MD5_BLOCK_SIZE] = {0};
    UCHAR                           outerPadding[ANSC_MD5_BLOCK_SIZE] = {0};
    UCHAR                           HMACKey[ANSC_MD5_BLOCK_SIZE]      = {0};
    MD5_CTX                         context;

    /*
     * if the key is longer than the block size of hash function, we have to reset it to the output size;
     * otherwise, we have to pad zeros to the end of the key
     */
    if ( key->Length > ANSC_MD5_BLOCK_SIZE )
    {
        MD5_Init  (&context);
        MD5_Update(&context, key->Value[0], key->Length);
        MD5_Final (HMACKey,  &context);
    }
    else
    {
        for ( i = 0; i < key->Length; i++ )
        {
            HMACKey[i] = key->Value[0][i];
        }
    }

    /*
     * perform the XOR operation between key and paddings
     */
    AnscCopyMemory(innerPadding, HMACKey, ANSC_MD5_BLOCK_SIZE);
    AnscCopyMemory(outerPadding, HMACKey, ANSC_MD5_BLOCK_SIZE);

    for ( i = 0; i < ANSC_MD5_BLOCK_SIZE; i++ )
    {
        innerPadding[i] ^= ANSC_HMAC_INNER_PADDING_UNIT;
        outerPadding[i] ^= ANSC_HMAC_OUTER_PADDING_UNIT;
    }

    /*
     * perform inner MD5
     */
    MD5_Init  (&context);
    MD5_Update(&context,    innerPadding,   ANSC_MD5_BLOCK_SIZE);
    MD5_Update(&context,    (PUCHAR)buffer, size);
    MD5_Final (hash->Value, &context);

    /*
     * perform outer MD5
     */
    MD5_Init  (&context);
    MD5_Update(&context,    outerPadding, ANSC_MD5_BLOCK_SIZE);
    MD5_Update(&context,    hash->Value,  ANSC_MD5_OUTPUT_SIZE);
    MD5_Final (hash->Value, &context);

    /*
     * let's make sure our output size is bigger than the requirement; otherwise, we should pad zeros to the end
     * of the hash result data
     */
    if ( ANSC_MD5_OUTPUT_SIZE < hash->Length )
    {
        AnscZeroMemory(&hash->Value[ANSC_MD5_OUTPUT_SIZE], hash->Length - ANSC_MD5_OUTPUT_SIZE);
    }

    /*
     * return control to caller
     */
    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHmacSha1Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHmacSha1Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
    ULONG                           i                                  = 0;
    UCHAR                           innerPadding[ANSC_SHA1_BLOCK_SIZE] = {0};
    UCHAR                           outerPadding[ANSC_SHA1_BLOCK_SIZE] = {0};
    UCHAR                           HMACKey[ANSC_SHA1_BLOCK_SIZE]      = {0};
    SHA_CTX                         context;

    /*
     * if the key is longer than the block size of hash function, we have to reset it to the output size;
     * otherwise, we have to pad zeros to the end of the key
     */
    if ( key->Length > ANSC_SHA1_BLOCK_SIZE )
    {
        SHA1_Init  (&context);
        SHA1_Update(&context, key->Value[0], key->Length);
        SHA1_Final (HMACKey,  &context);
    }
    else
    {
        for ( i = 0; i < key->Length; i++ )
        {
            HMACKey[i] = key->Value[0][i];
        }
    }

    /*
     * perform the XOR operation between key and paddings
     */
    AnscCopyMemory(innerPadding, HMACKey, ANSC_SHA1_BLOCK_SIZE);
    AnscCopyMemory(outerPadding, HMACKey, ANSC_SHA1_BLOCK_SIZE);

    for ( i = 0; i < ANSC_SHA1_BLOCK_SIZE; i++ )
    {
        innerPadding[i] ^= ANSC_HMAC_INNER_PADDING_UNIT;
        outerPadding[i] ^= ANSC_HMAC_OUTER_PADDING_UNIT;
    }

    /*
     * perform inner SHA1
     */
    SHA1_Init  (&context);
    SHA1_Update(&context,    innerPadding,   ANSC_SHA1_BLOCK_SIZE);
    SHA1_Update(&context,    (PUCHAR)buffer, size);
    SHA1_Final (hash->Value, &context);

    /*
     * perform outer SHA1
     */
    SHA1_Init  (&context);
    SHA1_Update(&context,    outerPadding, ANSC_SHA1_BLOCK_SIZE);
    SHA1_Update(&context,    hash->Value,  ANSC_SHA1_OUTPUT_SIZE);
    SHA1_Final (hash->Value, &context);

    /*
     * let's make sure our output size is bigger than the requirement; otherwise, we should pad zeros to the end
     * of the hash result data
     */
    if ( ANSC_SHA1_OUTPUT_SIZE < hash->Length )
    {
        AnscZeroMemory(&hash->Value[ANSC_SHA1_OUTPUT_SIZE], hash->Length - ANSC_SHA1_OUTPUT_SIZE);
    }

    /*
     * return control to caller
     */
    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoHmacSha256Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoHmacSha256Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
    ULONG                           i                                    = 0;
    UCHAR                           innerPadding[ANSC_SHA256_BLOCK_SIZE] = {0};
    UCHAR                           outerPadding[ANSC_SHA256_BLOCK_SIZE] = {0};
    UCHAR                           HMACKey[ANSC_SHA256_BLOCK_SIZE]      = {0};
    UCHAR                           tempBuffer[ANSC_SHA256_OUTPUT_SIZE]  = {0};
    SHA256_CTX                      context;

    AnscTraceFlow
        ((
            "%s -- size = %lu, buffer = %s, key = %02X.%02X.%02X.%02X.%02X.%02X..., hash length = %lu, key length = %lu, block size %d, output size %d\n", 
            __FUNCTION__,
            size,
            (char*)buffer,
            key->Value[0][0], key->Value[0][1], key->Value[0][2],
            key->Value[0][3], key->Value[0][4], key->Value[0][5],
            hash->Length,
            key->Length,
            ANSC_SHA256_BLOCK_SIZE,
            ANSC_SHA256_OUTPUT_SIZE
        ));

    /*
     * if the key is longer than the block size of hash function, we have to reset it to the output size;
     * otherwise, we have to pad zeros to the end of the key
     */
    if ( key->Length > ANSC_SHA256_BLOCK_SIZE )
    {
        SHA256_Init  (&context);
        SHA256_Update(&context, key->Value[0], key->Length);
        SHA256_Final (HMACKey,  &context);
    }
    else
    {
        for ( i = 0; i < key->Length; i++ )
        {
            HMACKey[i] = key->Value[0][i];
        }
    }

    /*
     * perform the XOR operation between key and paddings
     */
    AnscCopyMemory(innerPadding, HMACKey, ANSC_SHA256_BLOCK_SIZE);
    AnscCopyMemory(outerPadding, HMACKey, ANSC_SHA256_BLOCK_SIZE);

    for ( i = 0; i < ANSC_SHA256_BLOCK_SIZE; i++ )
    {
        innerPadding[i] ^= ANSC_HMAC_INNER_PADDING_UNIT;
        outerPadding[i] ^= ANSC_HMAC_OUTER_PADDING_UNIT;
    }

    /*
     * perform inner SHA
     */
    SHA256_Init  (&context);
    SHA256_Update(&context,    innerPadding,   ANSC_SHA256_BLOCK_SIZE);
    SHA256_Update(&context,    (PUCHAR)buffer, size);
    SHA256_Final (tempBuffer, &context);

    /*
     * perform outer SHA
     */
    SHA256_Init  (&context);
    SHA256_Update(&context,    outerPadding, ANSC_SHA256_BLOCK_SIZE);
    SHA256_Update(&context,    tempBuffer,  ANSC_SHA256_OUTPUT_SIZE);
    SHA256_Final (hash->Value, &context);

#if 0 /*RDKB-6145, CID-24478, outof bound access may currupt memset beyond "array boundary"*/
    /*
     * let's make sure our output size is bigger than the requirement; otherwise, we should pad zeros to the end
     * of the hash result data
     */
    if ( ANSC_SHA256_OUTPUT_SIZE < hash->Length )
    {
        AnscZeroMemory(&hash->Value[ANSC_SHA256_OUTPUT_SIZE-1], hash->Length - ANSC_SHA256_OUTPUT_SIZE);
    }
#endif
    /*
     * return control to caller
     */
    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDesMacDigest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDesMacDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
#ifdef _ANSC_DES_USED_
	ULONG                           i                   = 0;
    PUCHAR                          pAuthenticationData = NULL;
    PVOID                           pEncryptedData      = NULL;
    ANSC_CRYPTO_IV                  iv                  = {0};
    des_key_schedule                keySchedule;

    /*
     * allocate memory to hold the encrypted data
     */
    pEncryptedData = AnscAllocateMemory(size);

    if ( !pEncryptedData )
    {
        AnscZeroMemory(hash->Value, hash->Length);

        return  hash->Length;
    }

    /*
     * proceed des-cbc on the whole payload, but first we need to initialize the IV according to the draft,
     * IV should be filled by zeros.
     */
    iv.Length = ANSC_DES_IV_SIZE;

    /*
     * retrieve a key schedule?
     */
    des_key_sched
        (
            (des_cblock*)key->Value[0],
            keySchedule
        );

    /*
     * des-cbc
     */
    des_cbc_encrypt
        (
            (des_cblock*)buffer,                    /* input stream of data blocks */
            (des_cblock*)pEncryptedData,            /* output stream of data blocks */
            size,                                   /* total length of data stream */
            keySchedule,                            /* key schedule */
            (des_cblock*)iv.Value,                  /* initialization vector */
            1                                       /* indicate it's an encryption operation */
        );

    /*
     * use the last 8 bytes output as authentication data
     */
    AnscCopyMemory
        (
            hash->Value,
            (PVOID)((ULONG)pEncryptedData + size - ANSC_DES_BLOCK_SIZE),
            ANSC_DES_BLOCK_SIZE
        );

    /*
     * let's make sure our output size is bigger than the requirement; otherwise, we should pad zeros to the end
     * of the hash result data
     */
    if ( ANSC_DES_BLOCK_SIZE < hash->Length )
    {
        AnscZeroMemory(&hash->Value[ANSC_DES_BLOCK_SIZE], hash->Length - ANSC_DES_BLOCK_SIZE);
    }

    /*
     * free the allocated memory
     */
    AnscFreeMemory(pEncryptedData);

#else
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(key);
    AnscTrace("WARNING: DES digest is disabled!!!\n");
#endif
    /*
     * return control to caller
     */
    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoKpdkDigest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoKpdkDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(hash);
    UNREFERENCED_PARAMETER(key);
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoMd5Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoMd5Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    )
{
    MD5_CTX                         context;

    MD5_Init  (&context);
    MD5_Update(&context,    (PUCHAR)buffer, size);
    MD5_Final (hash->Value, &context);

    hash->Length = ANSC_MD5_OUTPUT_SIZE;

    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoSha1Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoSha1Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    )
{
    SHA_CTX                         context;

    SHA1_Init  (&context);
    SHA1_Update(&context,    (PUCHAR)buffer, size);
    SHA1_Final (hash->Value, &context);

    hash->Length = ANSC_SHA1_OUTPUT_SIZE;

    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoSha256Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoSha256Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    )
{
    SHA256_CTX                      context;

    SHA256_Init  (&context);
    SHA256_Update(&context,    (PUCHAR)buffer, size);
    SHA256_Final (hash->Value, &context);

    hash->Length = ANSC_SHA256_OUTPUT_SIZE;

    return  hash->Length;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoMd4Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoMd4Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    )
{
#ifdef _ANSC_MD4_USED_
	MD4_CTX                         context;

    MD4_Init  (&context);
    MD4_Update(&context,    (PUCHAR)buffer, size);
    MD4_Final (hash->Value, &context);

    hash->Length = ANSC_MD4_OUTPUT_SIZE;

#else
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(size);
    AnscTrace("WARNING: MD4 digest is disabled!!!\n");
#endif
    return  hash->Length;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoMd2Digest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoMd2Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    )
{
#ifdef _ANSC_MD2_USED_
    MD2_CTX                         context;

    MD2_Init  (&context);
    MD2_Update(&context,    (PUCHAR)buffer, size);
    MD2_Final (hash->Value, &context);

    hash->Length = ANSC_MD2_OUTPUT_SIZE;

    return  hash->Length;
#else
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(size);
    AnscTrace("WARNING: MD2 digest is disabled!!!\n");
    return hash->Length;
#endif
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDesMacKDigest
            (
                PVOID                       buffer,
                ULONG                       size,
                PANSC_CRYPTO_HASH           hash,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             pIv
            );

    description:

        This function performs cryptography computation with input IV.

    argument:   PVOID                       buffer
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PANSC_CRYPTO_HASH           hash
                Specifies the result of the computation.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             pIV
                Specifies the initial vector to use.

    return:     result.

**********************************************************************/
ULONG
AnscCryptoDesMacKDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             pIV
    )
{
#ifdef _ANSC_DES_USED_
	ULONG                           i                   = 0;
    PUCHAR                          pAuthenticationData = NULL;
    PVOID                           pEncryptedData      = NULL;
    ANSC_CRYPTO_IV                  iv                  = {0};
    des_key_schedule                keySchedule;

    /*
     * allocate memory to hold the encrypted data
     */
    pEncryptedData = AnscAllocateMemory(size);

    if ( !pEncryptedData )
    {
        AnscZeroMemory(hash->Value, hash->Length);

        return  hash->Length;
    }

    /*
     * proceed des-cbc on the whole payload, but first we need to initialize the IV according to the draft,
     * IV should be filled by zeros.
     */
    iv.Length = ANSC_DES_IV_SIZE;

    /*
     * retrieve a key schedule?
     */
    des_key_sched
        (
            (des_cblock*)key->Value[0],
            keySchedule
        );

    /*
     * des-cbc
     */
    if( pIV != NULL)
    {
        des_cbc_encrypt
            (
                (des_cblock*)buffer,                    /* input stream of data blocks */
                (des_cblock*)pEncryptedData,            /* output stream of data blocks */
                size,                                   /* total length of data stream */
                keySchedule,                            /* key schedule */
                (des_cblock*)pIV->Value,                  /* initialization vector */
                1                                       /* indicate it's an encryption operation */
            );
    }
    else
    {
        des_cbc_encrypt
            (
                (des_cblock*)buffer,                    /* input stream of data blocks */
                (des_cblock*)pEncryptedData,            /* output stream of data blocks */
                size,                                   /* total length of data stream */
                keySchedule,                            /* key schedule */
                (des_cblock*)iv.Value,                  /* initialization vector */
                1                                       /* indicate it's an encryption operation */
            );
    }

    /*
     * use the last 8 bytes output as authentication data
     */
    AnscCopyMemory
        (
            hash->Value,
            (PVOID)((ULONG)pEncryptedData + size - ANSC_DES_BLOCK_SIZE),
            ANSC_DES_BLOCK_SIZE
        );

    /*
     * let's make sure our output size is bigger than the requirement; otherwise, we should pad zeros to the end
     * of the hash result data
     */
    if ( ANSC_DES_BLOCK_SIZE < hash->Length )
    {
        AnscZeroMemory(&hash->Value[ANSC_DES_BLOCK_SIZE], hash->Length - ANSC_DES_BLOCK_SIZE);
    }

    /*
     * free the allocated memory
     */
    AnscFreeMemory(pEncryptedData);

#else
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(pIV);
    AnscTrace("WARNING: DES MAC digest is disabled!!!\n");
#endif
    /*
     * return control to caller
     */
    return  hash->Length;
}
