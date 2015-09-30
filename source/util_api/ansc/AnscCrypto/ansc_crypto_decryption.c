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

    module: ansc_crypto_decryption.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoDecrypt
        *   AnscCryptoDesIv64Decrypt
        *   AnscCryptoDesDecrypt
        *   AnscCryptoTripleDesDecrypt
        *   AnscCryptoRc5Decrypt
        *   AnscCryptoIdeaDecrypt
        *   AnscCryptoCastDecrypt
        *   AnscCryptoBlowfishDecrypt
        *   AnscCryptoTripleIdeaDecrypt
        *   AnscCryptoDesIv32Decrypt
        *   AnscCryptoRc4Decrypt
        *   AnscCryptoRc2Decrypt
        *   AnscCryptoTLSRc4Decrypt
        *   AnscCryptoCrypt_rc2_decrypt
        *   AnscCryptoAesDecrypt

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        11/12/03    zhubin added RC4 decryption for TLS
        06/02/04    zhubin added 3 rc2 related apis

**********************************************************************/


#include "ansc_crypto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDecrypt
            (
                ULONG                       algorithm,
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   ULONG                       algorithm
                Specifies the cryptography algorithm to use.

                PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDecrypt
    (
        ULONG                       algorithm,
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
    ULONG                           ulResult = 0;

    switch ( algorithm )
    {
        case    ANSC_CRYPTO_ENCRYPTION_DES_IV64 :

                ulResult =
                    AnscCryptoDesIv64Decrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_DES :

                ulResult =
                    AnscCryptoDesDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_3DES :

                ulResult =
                    AnscCryptoTripleDesDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_RC5 :

                ulResult =
                    AnscCryptoRc5Decrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_IDEA :

                ulResult =
                    AnscCryptoIdeaDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_CAST :

                ulResult =
                    AnscCryptoCastDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_BLOWFISH :

                ulResult =
                    AnscCryptoBlowfishDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_3IDEA :

                ulResult =
                    AnscCryptoTripleIdeaDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_DES_IV32 :

                ulResult =
                    AnscCryptoDesDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

#ifndef _ANSC_CRYPTO_NO_RC4_

        case    ANSC_CRYPTO_ENCRYPTION_RC4 :
                ulResult =
                    AnscCryptoRc4Decrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );
                break;
#endif

#ifndef _ANSC_CRYPTO_NO_RC2_

        case    ANSC_CRYPTO_ENCRYPTION_RC2 :

                ulResult =
                    AnscCryptoRc2Decrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
                        );

                break;

#endif

        case    ANSC_CRYPTO_ENCRYPTION_AES :

                ulResult =
                    AnscCryptoAesDecrypt
                        (
                            cipher,
                            size,
                            plain,
                            key,
                            iv
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
        AnscCryptoDesIv64Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDesIv64Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_DES_USED_
	des_key_schedule                keySchedule;

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
            (des_cblock*)cipher,                    /* input stream of data blocks */
            (des_cblock*)plain,                     /* output stream of data blocks */
            size,                                   /* total length of data stream */
            keySchedule,                            /* key schedule */
            (des_cblock*)iv->Value,                 /* initialization vector */
            ANSC_DECRYPTION_MODE                    /* indicate it's an decryption operation */
        );

#else
    AnscTrace("WARNING: DES is disabled!!!\n");
#endif

    /*
     * DES decryption doesn't change the size of the input data
     */
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDesDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_DES_USED_
    des_key_schedule                keySchedule;
    UCHAR                           pTemp[ANSC_DES_BLOCK_SIZE + 1] = { 0 };

    /*
     * retrieve a key schedule?
     */
    des_key_sched
        (
            (des_cblock*)key->Value[0],
            keySchedule
        );

    /*
     * des-cbc/des-ecb
     */
    if ( iv )
    {
        /*
         *  Bin added here:
         *  Since the cipher and plain are possible the same buffer, we have
         *  to backup the last cipher block as the output iv..
         */
        AnscCopyMemory
            (
                pTemp,
                (PVOID)((ULONG)cipher +  size -  ANSC_DES_BLOCK_SIZE),
                ANSC_DES_BLOCK_SIZE
            );

        des_cbc_encrypt
            (
                (des_cblock*)cipher,                /* input stream of data blocks           */
                (des_cblock*)plain,                 /* output stream of data blocks          */
                size,                               /* total length of data stream           */
                keySchedule,                        /* key schedule                          */
                (des_cblock*)iv->Value,             /* initialization vector                 */
                ANSC_DECRYPTION_MODE                /* indicate it's an decryption operation */
            );

        AnscCopyMemory
            (
                iv->Value,
                pTemp,
                ANSC_DES_BLOCK_SIZE
            );
    }
    else
    {
        char*                           input  = cipher;
        char*                           output = plain;
        ULONG                           i      = 0;

        /* Comment it requested by DingHua
        char                            desBlock[ANSC_DES_BLOCK_SIZE];

        for ( i = 0; i < size / ANSC_DES_BLOCK_SIZE; i++ )
        {
            des_key_sched
                (
                    (des_cblock*)key->Value[0],
                    keySchedule
                );

            AnscCopyMemory(desBlock, input, ANSC_DES_BLOCK_SIZE);

            des_encrypt
                (
                    (DES_LONG*)desBlock,
                    keySchedule,
                    ANSC_DECRYPTION_MODE
                );

            AnscCopyMemory(output, desBlock, ANSC_DES_BLOCK_SIZE);

            input  += ANSC_DES_BLOCK_SIZE;
            output += ANSC_DES_BLOCK_SIZE;
        }
        */

        for ( i = 0; i < size / ANSC_DES_BLOCK_SIZE; i++ )
        {
            des_key_sched
                (
                    (des_cblock*)key->Value[0],
                    keySchedule
                );

            des_ecb_encrypt
                (
                    (des_cblock*)input,             /* input stream of data blocks */
                    (des_cblock*)output,            /* output stream of data blocks */
                    keySchedule,                    /* key schedule */
                    ANSC_DECRYPTION_MODE            /* indicate it's an decryption operation */
                );

            input  += ANSC_DES_BLOCK_SIZE;
            output += ANSC_DES_BLOCK_SIZE;
        }
    }
#else
    AnscTrace("WARNING: DES is disabled!!!\n");
#endif
    /*
     * DES decryption doesn't change the size of the input data
     */
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoTripleDesDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoTripleDesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_DES_USED_
	PUCHAR                          pInput                                   = (PUCHAR)cipher;
    PUCHAR                          pOutput                                  = (PUCHAR)plain;
    UCHAR                           previousCipherBlock[ANSC_DES_BLOCK_SIZE] = {0};
    ULONG                           i                                        = 0;
    ULONG                           j                                        = 0;
    des_key_schedule                keySchedule1;
    des_key_schedule                keySchedule2;
    des_key_schedule                keySchedule3;

    /*
     * retrieve a key schedule?
     */
    des_key_sched((des_cblock*)key->Value[0], keySchedule1);
    des_key_sched((des_cblock*)key->Value[1], keySchedule2);
    des_key_sched((des_cblock*)key->Value[2], keySchedule3);

    /*
     * des-cbc-decryption (k3) + des-cbc-encryption (k2) + des-cbc-decryption (k1)
     */
    for ( i = 0; i < size / ANSC_DES_BLOCK_SIZE; i++ )
    {
        AnscCopyMemory
            (
                previousCipherBlock,
                pInput,
                ANSC_DES_BLOCK_SIZE
            );

        des_ecb3_encrypt
            (
                (des_cblock*)pInput,                /* input stream of data blocks */
                (des_cblock*)pOutput,               /* output stream of data blocks */
                keySchedule1,                       /* key schedule */
                keySchedule2,                       /* key schedule */
                keySchedule3,                       /* key schedule */
                ANSC_DECRYPTION_MODE                /* indicate it's an decryption operation */
            );

        for ( j = 0; j < ANSC_DES_BLOCK_SIZE; j++ )
        {
            pOutput[j] ^= iv->Value[j];
        }

        AnscCopyMemory
            (
                iv->Value,
                (PVOID)previousCipherBlock,
                iv->Length
            );

        /*
         * update pointers
         */
        pInput  += ANSC_DES_BLOCK_SIZE;
        pOutput += ANSC_DES_BLOCK_SIZE;
    }

#else
    AnscTrace("WARNING: DES is disabled!!!\n");
#endif

    /*
     * DES encryption doesn't change the size of the input data
     */
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoRc5Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoRc5Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoIdeaDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoIdeaDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_IDEA_USED_
    IDEA_KEY_SCHEDULE               encryptionKeySchedule;
    IDEA_KEY_SCHEDULE               decryptionKeySchedule;
    UCHAR                           pTemp[ANSC_IDEA_BLOCK_SIZE + 1] = { 0 };

    /*
     * set key schedule
     */
    idea_set_encrypt_key(key->Value[0],          &encryptionKeySchedule);
    idea_set_decrypt_key(&encryptionKeySchedule, &decryptionKeySchedule);

    /*
     *  Bin added here:
     *  Since the cipher and plain are possible the same buffer, we have
     *  to backup the last cipher block as the output iv..
     */
    AnscCopyMemory
        (
            pTemp,
            (PVOID)((ULONG)cipher +  size -  ANSC_IDEA_BLOCK_SIZE),
            ANSC_IDEA_BLOCK_SIZE
        );

    /*
     * perform CBC mode encryption
     */
    idea_cbc_encrypt
        (
            (unsigned char*)cipher,                 /* input stream of data blocks           */
            (unsigned char*)plain,                  /* output stream of data blocks          */
            size,                                   /* total length of data stream           */
            &decryptionKeySchedule,                 /* key schedule                          */
            (unsigned char*)iv->Value,              /* initialization vector                 */
            ANSC_DECRYPTION_MODE                    /* indicate it's an decryption operation */
        );

    AnscCopyMemory
        (
            iv->Value,
            pTemp,
            ANSC_IDEA_BLOCK_SIZE
        );

    /*
     * IDEA doesn't change the payload size
     */
    return  size;
#else
    AnscTrace("WARNING: IDEA is disabled!!!\n");
    return 0;
#endif
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoCastDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoCastDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoBlowfishDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoBlowfishDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_BF_USED_
    BF_KEY                          blowfishKey;
    UCHAR                           pTemp[8 + 1] = { 0 };

    /*
     * set blowfish decryption key
     */
    BF_set_key(&blowfishKey, key->Length, key->Value[0]);

    /*
     *  Bin added here:
     *  Since the cipher and plain are possible the same buffer, we have
     *  to backup the last cipher block as the output iv..
     */
    AnscCopyMemory
        (
            pTemp,
            (PVOID)((ULONG)cipher +  size -  8),
            ANSC_DES_BLOCK_SIZE
        );

    /*
     * perform CBC mode encryption
     */
    BF_cbc_encrypt
        (
            (unsigned char*)cipher,                 /* input stream of data blocks */
            (unsigned char*)plain,                  /* output stream of data blocks */
            size,                                   /* total length of data stream */
            &blowfishKey,                           /* key schedule */
            (unsigned char*)iv->Value,              /* initialization vector */
            ANSC_DECRYPTION_MODE                    /* indicate it's an decryption operation */
        );

    AnscCopyMemory
        (
            iv->Value,
            (PVOID)pTemp,
            iv->Length
        );

    /*
     * BLOWFISH doesn't change the payload size
     */
    return  size;
#else
    AnscTrace("WARNING: BF is disabled!!!\n");
    return 0;
#endif

}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoTripleIdeaDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoTripleIdeaDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoDesIv32Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoDesIv32Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
    return  size;
}

#ifndef _ANSC_CRYPTO_NO_RC4_
/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoRc4Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoRc4Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_RC4_USED_
    RC4_KEY                         rc4Key;

    /*
     * set RC4 decryption key
     */
    RC4_set_key(&rc4Key, key->Length, key->Value[0]);

    /*
     * perform RC4 decryption (no IV is needed)
     */
    RC4
        (
            &rc4Key,                                /* encryption key */
            size,                                   /* total length of data stream */
            (unsigned char*)cipher,                 /* input stream of data blocks */
            (unsigned char*)plain                   /* output stream of data blocks */
        );

    /*
     * RC4 doesn't change the payload size
     */
    return  size;
#else
    AnscTrace("WARNING: RC4 is disabled!!!\n");
    return 0;
#endif
}

#endif

/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoTLSRc4Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoTLSRc4Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_RC4_USED_
    RC4_KEY2                        rc4Key;

    /*
     * In TLS, RC4_set_key can be only set by once, the rc4Key changed after
     * every encrytion which mean after one encrtypion, the key is useless and
     * we have to remember the new states of rc4Key.
     */
     if( key->KeyNumber == 1)
     {
        /*
         * set RC4 encryption key
         */
        RC4_prepare_key(key->Value[0], key->Length, &rc4Key);
     }
     else
     {
         /* we borrow the "PANSC_CRYPTO_KEY" to save the 256 data of current rc4key */
         AnscCopyMemory
             (
                (PVOID)rc4Key.state,
                (PVOID)key->Value,
                256
             );

        rc4Key.x    =  key->Value[4][0];
        rc4Key.y    =  key->Value[4][1];
    }


    /*
     * perform RC4 decryption (no IV is needed)
     */
    RC4_2
        (
            (unsigned char*)cipher,                 /* input stream of data blocks */
            size,                                   /* total length of data stream */
            &rc4Key
        );

    if( cipher != plain)
    {
        AnscCopyMemory
            (
                plain,
                cipher,
                size
            );
    }

    /* save current RC4 states */
    key->KeyNumber           = 5;
    key->Length              = ANSC_MAX_KEY_LENGTH;

     AnscCopyMemory
         (
            (PVOID)key->Value,
            (PVOID)rc4Key.state,
            256
         );

    key->Value[4][0] = (UCHAR)rc4Key.x;
    key->Value[4][1] = (UCHAR)rc4Key.y;

    /*
     * RC4 doesn't change the payload size
     */
    return  size;
#else
    AnscTrace("WARNING: RC4 is disabled!!!\n");
    return 0;
#endif

}

#ifndef _ANSC_CRYPTO_NO_RC2_
/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoRc2Decrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoRc2Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_RC2_USED_
    RC2_KEY                         rc2Key;
    UCHAR                           pTemp[ANSC_RC2_BLOCK_SIZE + 1] = { 0 };

    /*
     * set rc2 decryption key
     */
    RC2_set_key(&rc2Key, key->Length, key->Value[0], key->Length * 8);

    /*
     *  Bin added here:
     *  Since the cipher and plain are possible the same buffer, we have
     *  to backup the last cipher block as the output iv..
     */
    AnscCopyMemory
        (
            pTemp,
            (PVOID)((ULONG)cipher +  size -  ANSC_RC2_BLOCK_SIZE),
            ANSC_RC2_BLOCK_SIZE
        );

    /*
     * perform CBC mode encryption
     */
    RC2_cbc_encrypt
        (
            (unsigned char*)cipher,                 /* input stream of data blocks           */
            (unsigned char*)plain,                  /* output stream of data blocks          */
            size,                                   /* total length of data stream           */
            &rc2Key,                                /* key schedule                          */
            (unsigned char*)iv->Value,              /* initialization vector                 */
            ANSC_DECRYPTION_MODE                    /* indicate it's an decryption operation */
        );

    AnscCopyMemory
        (
            iv->Value,
            pTemp,
            ANSC_RC2_BLOCK_SIZE
        );

    /*
     * RC2 doesn't change the payload size
     */
    return  size;
#else
    AnscTrace("WARNING: RC2 is disabled!!!\n");
    return 0;
#endif
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoCrypt_rc2_decrypt
            (
                PUSHORT             xkey,
                PUCHAR              plain,
                PUCHAR              cipher
            );

    description:

        This function performs cryptography computation.

    argument:   PUSHORT             xkey,
                the rc2 key

                PUCHAR              plain
                the input data

                PUCHAR              cipher
                the output data;

    return:     status of the operation

**********************************************************************/
ANSC_STATUS
AnscCryptoCrypt_rc2_decrypt
    (
        PUSHORT             xkey,
        PUCHAR              plain,
        PUCHAR              cipher
    )
{
#ifdef _ANSC_RC2_USED_
    rc2_decrypt
        (
            xkey,
            plain,
            cipher
        );

    return ANSC_STATUS_SUCCESS;
#else
    AnscTrace("WARNING: RC2 is disabled!!!\n");
    return ANSC_STATUS_FAILURE;
#endif
}

#endif

/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscCryptoAesDecrypt
            (
                PVOID                       cipher,
                ULONG                       size,
                PVOID                       plain,
                PANSC_CRYPTO_KEY            key,
                PANSC_CRYPTO_IV             iv
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       cipher
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_IV             iv
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ULONG
AnscCryptoAesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    )
{
#ifdef _ANSC_AES_USED_
	ULONG                           ulResult = 0;
    AES_KEY                         aesKey;
    int                             ret;

    if ( size % ANSC_AES_BLOCK_SIZE )
    {
        return 0;
    }

    ret = AES_set_decrypt_key((const char *)key->Value, key->Length * 8, &aesKey);

    if ( ret != 0 )
    {
        return 0;
    }

    AES_cbc_encrypt
        (
            (const unsigned char *)cipher,
            (unsigned char *      )plain,
            (const unsigned long  )size,
            (const AES_KEY *      )&aesKey,
            (unsigned char *      )iv->Value,
            AES_DECRYPT
        );

#else
    AnscTrace("WARNING: AES is disabled!!!\n");
#endif

    return size;
}

