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

    module:	ansc_crypto_internal_api.h

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

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        06/02/04    zhubin added 3 rc2 related apis
        03/09/05    zhubin added uuid generation apis
        08/03/05    zhubin added zlib and zip support

**********************************************************************/


#ifndef  _ANSC_CRYPTO_INTERNAL_API_
#define  _ANSC_CRYPTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscCryptoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscCryptoRemove
    (
        ANSC_HANDLE                 hOwnerContext
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_DIGESTION.C
***********************************************************/

ULONG
AnscCryptoDigest
    (
        ULONG                       algorithm,
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoHmacMd5Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoHmacSha1Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoDesMacDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoKpdkDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoMd5Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

ULONG
AnscCryptoSha1Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

ULONG
AnscCryptoMd4Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

ULONG
AnscCryptoMd2Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

ULONG
AnscCryptoDesMacKDigest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             pIV
    );

/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_ENCRYPTION.C
***********************************************************/

ULONG
AnscCryptoEncrypt
    (
        ULONG                       algorithm,
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoDesIv64Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoDesEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTripleDesEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc5Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoIdeaEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoCastEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoBlowfishEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTripleIdeaEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoDesIv32Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc4Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTLSRc4Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc2Encrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ANSC_STATUS
AnscCryptoCrypt_rc2_encrypt
    (
        PUSHORT                     xkey,
        PUCHAR                      plain,
        PUCHAR                      cipher 
    );

ULONG
AnscCryptoAesEncrypt
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_DECRYPTION.C
***********************************************************/

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

ULONG
AnscCryptoDesIv64Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoDesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTripleDesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc5Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoIdeaDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoCastDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoBlowfishDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTripleIdeaDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoDesIv32Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc4Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoRc2Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ULONG
AnscCryptoTLSRc4Decrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

ANSC_STATUS
AnscCryptoCrypt_rc2_decrypt
    ( 
        PUSHORT                     xkey,
        PUCHAR                      plain,
        PUCHAR                      cipher 
    );

ULONG
AnscCryptoAesDecrypt
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

/***********************************************************
     FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_COMPRESSION.C
***********************************************************/

ANSC_STATUS
AnscCryptoCompress
    (
        ULONG                       algorithm,
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

ANSC_STATUS
AnscCryptoOutCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

ANSC_STATUS
AnscCryptoDeflateCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

ANSC_STATUS
AnscCryptoLzsCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

ANSC_STATUS
AnscCryptoV42bisCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

ANSC_STATUS
AnscCryptoZlibCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoGzipCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    );

/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_DECOMPRESSION.C
***********************************************************/

ANSC_STATUS
AnscCryptoDecompress
    (
        ULONG                       algorithm,
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoOutDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoDeflateDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoLzsDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoV42bisDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoZlibDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

ANSC_STATUS
AnscCryptoGzipDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_KEY.C
***********************************************************/

ANSC_STATUS
AnscCryptoDesPrepareKey
    (
        PANSC_CRYPTO_KEY            key
    );

ANSC_STATUS
AnscCryptoMsChap1LmHash
    (
        char*                       password,
        PANSC_CRYPTO_HASH           hash
    );

ANSC_STATUS
AnscCryptoMsChap1DeriveKey1
    (
        char*                       initialKey,
        PANSC_CRYPTO_KEY            key
    );

ANSC_STATUS
AnscCryptoMsChap1DeriveKey2
    (
        char*                       challenge,
        char*                       pwHash,
        PANSC_CRYPTO_KEY            key
    );

ANSC_STATUS
AnscCryptoMsChap2GetMKey
    (
        char*                       pw_hash_hash,
        char*                       nt_response,
        PANSC_CRYPTO_KEY            key
    );

ANSC_STATUS
AnscCryptoMsChap2GetAKey
    (
        char*                       master_key,
        PANSC_CRYPTO_KEY            key,
        BOOL                        bSend,
        BOOL                        bServer
    );

ANSC_STATUS
AnscCryptoMsChap2GetNKey
    (
        char*                       initial_key,
        PANSC_CRYPTO_KEY            key
    );

ANSC_STATUS
AnscCryptoMsChap2GetAuthResponse
    (
        char*                       pw_hash,
        char*                       nt_response,
        char*                       challenge,
        PANSC_CRYPTO_HASH           hash
    );


ANSC_STATUS
AnscCryptoCrypt_rc2_keyschedule
    ( 
        PUSHORT                     xkey,
        PUCHAR                      key,
        ULONG                       len,
        ULONG                       bits 
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_HASH.C
***********************************************************/

ULONG
AnscCryptoHashMd5
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );

ULONG
AnscCryptoHashSha1
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );

ULONG
AnscCryptoHashMd4
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_RANDOM.C
***********************************************************/

ULONG
AnscCryptoGetRandomUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    );

USHORT
AnscCryptoGetRandomUshort
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    );

ANSC_STATUS
AnscCryptoGetRandomOctets
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed,
        PUCHAR                      pOctetStream,
        ULONG                       ulSize
    );


/*********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_UTILITY.C
**********************************************************/

BOOL
AnscCryptoIsDesWeakKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );

ANSC_STATUS
AnscCryptoFixParity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    );

BOOL
AnscCryptoCheckParity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    );

ANSC_STATUS
AnscCryptoCorrectDesKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );

ANSC_STATUS
AnscCryptoMitDesStrToKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString,
        ULONG                       ulStringLeng,
        ANSC_HANDLE                 hSalt,
        ULONG                       ulSaltLeng,
        ANSC_HANDLE                 hNewKey
    );

UCHAR
AnscCryptoReverseByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue
    );

UCHAR
AnscCryptoXorByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue1,
        UCHAR                       ucValue2
    );

ANSC_STATUS
AnscCryptoNFold
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInBitNum,
        ULONG                       ulOutBitNum,
        PUCHAR                      pOutString
    );

ANSC_STATUS
AnscCryptoRandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       algorithm,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );

ANSC_STATUS
AnscCryptoMapEncTypeToKeySize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       algorithm,
        PULONG                      pulKeySeedSize,
        PULONG                      pulKeyLength,
        PULONG                      pulKeyNumber,
        PULONG                      pulKeyRound,
        PULONG                      pulBlockSize,
        PULONG                      pIvLength
    );

ANSC_STATUS
AnscCryptoDesRandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );

ANSC_STATUS
AnscCryptoDes3RandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );

ANSC_STATUS
AnscCryptoDeriveKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBaseKey,
        ULONG                       EncryptAlgorithm,
        ANSC_HANDLE                 hConstant,
        ULONG                       ulConstantLeng,
        ANSC_HANDLE                 hNewKey
    );

ANSC_STATUS
AnscCryptoDes3StringToKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString,
        ULONG                       ulStringLeng,
        ANSC_HANDLE                 hSalt,
        ULONG                       ulSaltLeng,
        ANSC_HANDLE                 hConstant,
        ULONG                       ulConstantLeng,
        ANSC_HANDLE                 hNewKey
    );

char*
AnscCryptoGenerateUuid
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize       /* must be 6 bytes long */
    );

char*
AnscCryptoGenerateUuid2
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize,      /* must be 6 bytes long */
        ULONG                       ulUuidVersion       /* as defined in http://www.ietf.org/internet-drafts/draft-mealling-uuid-urn-05.txt */
    );


#endif
