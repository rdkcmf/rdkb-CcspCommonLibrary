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

    module:	ansc_crypto_key.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoDesPrepareKey
        *   AnscCryptoMsChap1LmHash
        *   AnscCryptoMsChap1DeriveKey1
        *   AnscCryptoMsChap1DeriveKey2
        *   AnscCryptoMsChap2GetMKey
        *   AnscCryptoMsChap2GetAKey
        *   AnscCryptoMsChap2GetNKey
        *   AnscCryptoMsChap2GetAuthResponse
        *   AnscCryptoCrypt_rc2_keyschedule

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/28/01    initial revision.
        06/02/04    zhubin added 3 rc2 related apis

**********************************************************************/


#include "ansc_crypto_global.h"


/*
 * The Microsoft Challenge-Handshake Authentication Protocol (MS-CHAP-1) is a Microsoft proprietary
 * PPP authentication protocol, providing the functionality to which LAN-based users are accustomed
 * while integrating the encryption and hashing algorithms used on Windows networks.
 */
UCHAR  MsChapShaPad1[40] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

UCHAR  MsChapShaPad2[40] =
{
    0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2,
    0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2,
    0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2,
    0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2
};

char  MsChapStdText[] = "KGS!@#$%";

UCHAR  MsChap2Magic1[27] =
{
    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74,
    0x68, 0x65, 0x20, 0x4d, 0x50, 0x50, 0x45, 0x20, 0x4d,
    0x61, 0x73, 0x74, 0x65, 0x72, 0x20, 0x4b, 0x65, 0x79
};

UCHAR  MsChap2Magic2[84] =
{
    0x4f, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6c, 0x69,
    0x65, 0x6e, 0x74, 0x20, 0x73, 0x69, 0x64, 0x65, 0x2c, 0x20,
    0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x20, 0x6b, 0x65, 0x79,
    0x3b, 0x20, 0x6f, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73,
    0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x73, 0x69, 0x64, 0x65,
    0x2c, 0x20, 0x69, 0x74, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x72, 0x65, 0x63, 0x65, 0x69, 0x76, 0x65, 0x20,
    0x6b, 0x65, 0x79, 0x2e
};

UCHAR  MsChap2Magic3[84] =
{
    0x4f, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6c, 0x69,
    0x65, 0x6e, 0x74, 0x20, 0x73, 0x69, 0x64, 0x65, 0x2c, 0x20,
    0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x72, 0x65, 0x63, 0x65, 0x69, 0x76, 0x65, 0x20,
    0x6b, 0x65, 0x79, 0x3b, 0x20, 0x6f, 0x6e, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x73,
    0x69, 0x64, 0x65, 0x2c, 0x20, 0x69, 0x74, 0x20, 0x69, 0x73,
    0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x65, 0x6e, 0x64, 0x20,
    0x6b, 0x65, 0x79, 0x2e
};

UCHAR  MsChap2Magic11[39] =
{
    0x4D, 0x61, 0x67, 0x69, 0x63, 0x20, 0x73, 0x65, 0x72, 0x76,
    0x65, 0x72, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x6C, 0x69, 0x65,
    0x6E, 0x74, 0x20, 0x73, 0x69, 0x67, 0x6E, 0x69, 0x6E, 0x67,
    0x20, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x74
};

UCHAR  MsChap2Magic12[41] =
{
    0x50, 0x61, 0x64, 0x20, 0x74, 0x6F, 0x20, 0x6D, 0x61, 0x6B,
    0x65, 0x20, 0x69, 0x74, 0x20, 0x64, 0x6F, 0x20, 0x6D, 0x6F,
    0x72, 0x65, 0x20, 0x74, 0x68, 0x61, 0x6E, 0x20, 0x6F, 0x6E,
    0x65, 0x20, 0x69, 0x74, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6F,
    0x6E
};


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoDesPrepareKey
            (
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoDesPrepareKey
    (
        PANSC_CRYPTO_KEY            key
    )
{
    UNREFERENCED_PARAMETER(key);
	ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
#ifdef _ANSC_DES_USED_
    ULONG                           i            = 0;
    UCHAR                           keyByte[56];

    for ( i = 0; i < 7; i++ )
    {
        keyByte[i * 8 + 0] = (key->Value[0][i] & 0x80) >> 7;
        keyByte[i * 8 + 1] = (key->Value[0][i] & 0x40) >> 6;
        keyByte[i * 8 + 2] = (key->Value[0][i] & 0x20) >> 5;
        keyByte[i * 8 + 3] = (key->Value[0][i] & 0x10) >> 4;
        keyByte[i * 8 + 4] = (key->Value[0][i] & 0x08) >> 3;
        keyByte[i * 8 + 5] = (key->Value[0][i] & 0x04) >> 2;
        keyByte[i * 8 + 6] = (key->Value[0][i] & 0x02) >> 1;
        keyByte[i * 8 + 7] = (key->Value[0][i] & 0x01) >> 0;
    }

    for ( i = 0; i < 8; i++ )
    {
        key->Value[0][i]  = 0;
        key->Value[0][i] += keyByte[i * 7 + 0] * 0x80;
        key->Value[0][i] += keyByte[i * 7 + 1] * 0x40;
        key->Value[0][i] += keyByte[i * 7 + 2] * 0x20;
        key->Value[0][i] += keyByte[i * 7 + 3] * 0x10;
        key->Value[0][i] += keyByte[i * 7 + 4] * 0x08;
        key->Value[0][i] += keyByte[i * 7 + 5] * 0x04;
        key->Value[0][i] += keyByte[i * 7 + 6] * 0x02;
    }

    des_set_odd_parity((des_cblock*)key->Value[0]);

#else
    AnscTrace("WARNING: DES Prepare Key is disabled!!!\n");
#endif

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap1LmHash
            (
                char*                       password,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       password
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_HASH           hash
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap1LmHash
    (
        char*                       password,
        PANSC_CRYPTO_HASH           hash
    )
{
#ifdef _ANSC_DES_USED_
	ULONG                           ulPasswordSize = (AnscSizeOfString(password) > 14)? 14 : AnscSizeOfString(password);
    ANSC_CRYPTO_KEY                 desKey;
    char                            ucasePassword[14];

    AnscZeroMemory    (ucasePassword, 14);
    AnscCopyMemory    (ucasePassword, password,      ulPasswordSize);
    AnscStringFromLToU(password,      ucasePassword, ulPasswordSize);

    desKey.KeyNumber   = 1;
    desKey.RoundNumber = 1;
    desKey.Length      = ANSC_DES_BLOCK_SIZE;

    AnscZeroMemory         (desKey.Value[0], ANSC_MAX_KEY_LENGTH);
    AnscCopyMemory         (desKey.Value[0], &ucasePassword[0], 7);
    AnscCryptoDesPrepareKey(&desKey);

    AnscCryptoEncrypt
        (
            ANSC_CRYPTO_ENCRYPTION_DES,
            MsChapStdText,
            sizeof(MsChapStdText),
            &hash->Value[0],
            &desKey,
            NULL
        );

    AnscZeroMemory         (desKey.Value[0], ANSC_MAX_KEY_LENGTH);
    AnscCopyMemory         (desKey.Value[0], &ucasePassword[7], 7);
    AnscCryptoDesPrepareKey(&desKey);

    AnscCryptoEncrypt
        (
            ANSC_CRYPTO_ENCRYPTION_DES,
            MsChapStdText,
            sizeof(MsChapStdText),
            &hash->Value[8],
            &desKey,
            NULL
        );

#else
    UNREFERENCED_PARAMETER(password);
    UNREFERENCED_PARAMETER(hash);
    AnscTrace("WARNING: MS/CHAP I Hash is disabled!!!\n");
#endif

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap1DeriveKey1
            (
                char*                       initialKey,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       initialKey
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap1DeriveKey1
    (
        char*                       initialKey,
        PANSC_CRYPTO_KEY            key
    )
{
    SHA_CTX                         context;
    ANSC_CRYPTO_HASH                hash;

    AnscZeroMemory(hash.Value, ANSC_MAX_HASH_RESULT);

    SHA1_Init  (&context);
    SHA1_Update(&context,   initialKey,    key->Length);
    SHA1_Update(&context,   MsChapShaPad1, sizeof(MsChapShaPad1));
    SHA1_Update(&context,   key->Value[0], key->Length);
    SHA1_Update(&context,   MsChapShaPad2, sizeof(MsChapShaPad2));
    SHA1_Final (hash.Value, &context);

    AnscCopyMemory(key->Value[0], hash.Value, key->Length);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap1DeriveKey2
            (
                char*                       challenge,
                char*                       pwHash,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       challenge
                Specifies the cryptography parameter to be used.

                char*                       pwHash
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap1DeriveKey2
    (
        char*                       challenge,
        char*                       pwHash,
        PANSC_CRYPTO_KEY            key
    )
{
    SHA_CTX                         context;
    ANSC_CRYPTO_HASH                hash;

    AnscZeroMemory(hash.Value, ANSC_MAX_HASH_RESULT);

    SHA1_Init  (&context);
    SHA1_Update(&context,   pwHash,    16);
    SHA1_Update(&context,   pwHash,    16);
    SHA1_Update(&context,   challenge, 8);
    SHA1_Final (hash.Value, &context);

    AnscCopyMemory(key->Value[0], hash.Value, key->Length);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap2GetMKey
            (
                char*                       pw_hash_hash,
                char*                       nt_response,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       pw_hash_hash
                Specifies the cryptography parameter to be used.

                char*                       nt_response
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap2GetMKey
    (
        char*                       pw_hash_hash,
        char*                       nt_response,
        PANSC_CRYPTO_KEY            key
    )
{
    SHA_CTX                         context;
    ANSC_CRYPTO_HASH                hash;

    AnscZeroMemory(hash.Value, ANSC_MAX_HASH_RESULT);

    SHA1_Init  (&context);
    SHA1_Update(&context,   pw_hash_hash,  16);
    SHA1_Update(&context,   nt_response,   24);
    SHA1_Update(&context,   MsChap2Magic1, sizeof(MsChap2Magic1));
    SHA1_Final (hash.Value, &context);

    AnscCopyMemory(key->Value[0], hash.Value, key->Length);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap2GetAKey
            (
                char*                       master_key,
                PANSC_CRYPTO_KEY            key,
                BOOL                        bSend,
                BOOL                        bServer
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       master_key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

                BOOL                        bSend
                Specifies the cryptography parameter to be used.

                BOOL                        bServer
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap2GetAKey
    (
        char*                       master_key,
        PANSC_CRYPTO_KEY            key,
        BOOL                        bSend,
        BOOL                        bServer
    )
{
    SHA_CTX                         context;
    ANSC_CRYPTO_HASH                hash;
    PUCHAR                          magic = MsChap2Magic2;
    ULONG                           msize = sizeof(MsChap2Magic2);

    AnscZeroMemory(hash.Value, ANSC_MAX_HASH_RESULT);

    if ( bSend )
    {
        if ( bServer )
        {
            magic = MsChap2Magic3;
        }
        else
        {
            magic = MsChap2Magic2;
        }
    }
    else
    {
        if ( bServer )
        {
            magic = MsChap2Magic2;
        }
        else
        {
            magic = MsChap2Magic3;
        }
    }

    SHA1_Init  (&context);
    SHA1_Update(&context,   master_key,    16);
    SHA1_Update(&context,   MsChapShaPad1, sizeof(MsChapShaPad1));
    SHA1_Update(&context,   magic,         msize);
    SHA1_Update(&context,   MsChapShaPad2, sizeof(MsChapShaPad2));
    SHA1_Final (hash.Value, &context);

    AnscCopyMemory(key->Value[0], hash.Value, key->Length);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap2GetNKey
            (
                char*                       initial_key,
                PANSC_CRYPTO_KEY            key
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       initial_key
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_KEY            key
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap2GetNKey
    (
        char*                       initial_key,
        PANSC_CRYPTO_KEY            key
    )
{
    AnscCopyMemory(key->Value[0], initial_key, key->Length);

    return  AnscCryptoMsChap1DeriveKey1(initial_key, key);
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoMsChap2GetAuthResponse
            (
                char*                       pw_hash,
                char*                       nt_response,
                char*                       challenge,
                PANSC_CRYPTO_HASH           hash
            );

    description:

        This function performs cryptography computation.

    argument:   char*                       pw_hash
                Specifies the cryptography parameter to be used.

                char*                       nt_response
                Specifies the cryptography parameter to be used.

                char*                       challenge
                Specifies the cryptography parameter to be used.

                PANSC_CRYPTO_HASH           hash
                Specifies the cryptography parameter to be used.

    return:     result.

**********************************************************************/

ANSC_STATUS
AnscCryptoMsChap2GetAuthResponse
    (
        char*                       pw_hash,
        char*                       nt_response,
        char*                       challenge,
        PANSC_CRYPTO_HASH           hash
    )
{
    SHA_CTX                         context = {0};
    ANSC_CRYPTO_HASH                pw_hash_hash = {0}; /*RDKB-6147, CID-24680, initializing the variable*/

    AnscZeroMemory(pw_hash_hash.Value, ANSC_MAX_HASH_RESULT);

    AnscCryptoMd4Digest(pw_hash, 16, &pw_hash_hash);

    SHA1_Init  (&context);
    SHA1_Update(&context,    pw_hash_hash.Value, 16);
    SHA1_Update(&context,    nt_response,        24);
    SHA1_Update(&context,    MsChap2Magic11,     sizeof(MsChap2Magic11));
    SHA1_Final (hash->Value, &context);

    SHA1_Init  (&context);
    SHA1_Update(&context,    hash->Value,    20);
    SHA1_Update(&context,    challenge,      8);
    SHA1_Update(&context,    MsChap2Magic12, sizeof(MsChap2Magic12));
    SHA1_Final (hash->Value, &context);

    hash->Length = ANSC_SHA1_OUTPUT_SIZE;

    return  ANSC_STATUS_SUCCESS;
}

#ifndef _ANSC_CRYPTO_NO_RC2_
/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoCrypt_rc2_keyschedule
            (
                PUSHORT                     xkey,
                PUCHAR                      key,
                ULONG                       len,
                ULONG                       bits
            );

    description:

        This function generates the  rc2 key

    argument:   PUSHORT             xkey,
                the rc2 key

                PUCHAR              key
                the output key

                ULONG               len
                the size of the key

                ULONG               bits
                the bits

    return:     status of the operation

**********************************************************************/
ANSC_STATUS
AnscCryptoCrypt_rc2_keyschedule
    (
        PUSHORT                     xkey,
        PUCHAR                      key,
        ULONG                       len,
        ULONG                       bits
    )
{
#ifdef _ANSC_RC2_USED_
    rc2_keyschedule
        (
            xkey,
            key,
            len,
            bits
        );

    return ANSC_STATUS_SUCCESS;
#else
    UNREFERENCED_PARAMETER(xkey);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(len);
    UNREFERENCED_PARAMETER(bits);
    AnscTrace("WARNING: Key schedule RC2 is disabled!!!\n");
    return ANSC_STATUS_FAILURE;
#endif
}
#endif

