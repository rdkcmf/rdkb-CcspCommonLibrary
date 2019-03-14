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

    module:	ansc_crypto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object interface for the
        Crypto Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        09/13/01    revised to work in BroadWay 2.0
        06/02/04    zhubin added 3 rc2 related apis
        03/09/05    zhubin added uuid generation apis
        08/03/05    zhubin added zlib and zip support

**********************************************************************/


#ifndef  _ANSC_CRYPTO_INTERFACE_
#define  _ANSC_CRYPTO_INTERFACE_


/***********************************************************
              CRYPTO CUSTOM DEFINITIONS
***********************************************************/
#include "ansc_crypto_custom.h"

/***********************************************************
              GENERAL CRYPTO OBJECT DEFINITION
***********************************************************/

/*
 * define some const values that will be used in the container object definition
 */
#define  ANSC_CRYPTO_ENCRYPTION_RESERVED            0
#define  ANSC_CRYPTO_ENCRYPTION_DES_IV64            1
#define  ANSC_CRYPTO_ENCRYPTION_DES                 2
#define  ANSC_CRYPTO_ENCRYPTION_3DES                3
#define  ANSC_CRYPTO_ENCRYPTION_RC5                 4
#define  ANSC_CRYPTO_ENCRYPTION_IDEA                5
#define  ANSC_CRYPTO_ENCRYPTION_CAST                6
#define  ANSC_CRYPTO_ENCRYPTION_BLOWFISH            7
#define  ANSC_CRYPTO_ENCRYPTION_3IDEA               8
#define  ANSC_CRYPTO_ENCRYPTION_DES_IV32            9
#define  ANSC_CRYPTO_ENCRYPTION_RC4                 10
#define  ANSC_CRYPTO_ENCRYPTION_RC2                 11
#define  ANSC_CRYPTO_ENCRYPTION_AES                 12
#define  ANSC_CRYPTO_ENCRYPTION_NULL                13

#define  ANSC_CRYPTO_COMPRESSION_RESERVED           0
#define  ANSC_CRYPTO_COMPRESSION_OUT                1
#define  ANSC_CRYPTO_COMPRESSION_DEFLATE            2
#define  ANSC_CRYPTO_COMPRESSION_LZS                3
#define  ANSC_CRYPTO_COMPRESSION_V42BIS             4
#define  ANSC_CRYPTO_COMPRESSION_ZLIB               5
#define  ANSC_CRYPTO_COMPRESSION_GZIP               6

#define  ANSC_CRYPTO_DIGESTION_RESERVED             0
#define  ANSC_CRYPTO_DIGESTION_HMAC_MD5             1
#define  ANSC_CRYPTO_DIGESTION_HMAC_SHA             2
#define  ANSC_CRYPTO_DIGESTION_DES_MAC              3
#define  ANSC_CRYPTO_DIGESTION_KPDK                 4
#define  ANSC_CRYPTO_DIGESTION_MD5                  5
#define  ANSC_CRYPTO_DIGESTION_SHA                  6
#define  ANSC_CRYPTO_DIGESTION_MD4                  7
#define  ANSC_CRYPTO_DIGESTION_MD2                  8

#define  AnscSmack(step)                            ((1<<step)-1)
#define  AnscPstep(x,step)                          (((x)&AnscSmack(step))^(((x)>>step)&AnscSmack(step)))
#define  AnscParityChar(x)                          AnscPstep(AnscPstep(AnscPstep((x),4),2),1)

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_CRYPTO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_CRYPTO_DIGEST)
    (
        ULONG                       algorithm,
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

typedef  ULONG
(*PFN_CRYPTO_HMAC_MD5_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

typedef  ULONG
(*PFN_CRYPTO_HMAC_SHA1_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

typedef  ULONG
(*PFN_CRYPTO_DES_MAC_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );


typedef  ULONG
(*PFN_CRYPTO_DES_MAC_K_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             pIv
    );

typedef  ULONG
(*PFN_CRYPTO_KPDK_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

typedef  ULONG
(*PFN_CRYPTO_MD5_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ULONG
(*PFN_CRYPTO_SHA1_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ULONG
(*PFN_CRYPTO_MD4_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ULONG
(*PFN_CRYPTO_MD2_DIGEST)
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ULONG
(*PFN_CRYPTO_ENCRYPT)
    (
        ULONG                       algorithm,
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_IV64_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_3DES_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC5_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_IDEA_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_CAST_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_BLOWFISH_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_3IDEA_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_IV32_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC4_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC2_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DECRYPT)
    (
        ULONG                       algorithm,
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_IV64_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_AES_ENCRYPT)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       cipher,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_3DES_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC5_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_IDEA_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_CAST_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_BLOWFISH_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_3IDEA_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_DES_IV32_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC4_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_RC2_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ULONG
(*PFN_CRYPTO_AES_DECRYPT)
    (
        PVOID                       cipher,
        ULONG                       size,
        PVOID                       plain,
        PANSC_CRYPTO_KEY            key,
        PANSC_CRYPTO_IV             iv
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_COMPRESS)
    (
        ULONG                       algorithm,
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_OUT_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_DEFLATE_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_LZS_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_V42BIS_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_ZLIB_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_GZIP_COMPRESS)
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_DECOMPRESS)
    (
        ULONG                       algorithm,
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_OUT_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_DEFLATE_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_LZS_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_V42BIS_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_ZLIB_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_GZIP_DECOMPRESS)
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_DES_PREPARE_KEY)
    (
        PANSC_CRYPTO_KEY            key
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP1_LM_HASH)
    (
        char*                       password,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP1_DERIVE_KEY1)
    (
        char*                       initialKey,
        PANSC_CRYPTO_KEY            key
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP1_DERIVE_KEY2)
    (
        char*                       challenge,
        char*                       pwHash,
        PANSC_CRYPTO_KEY            key
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP2_GET_MKEY)
    (
        char*                       pw_hash_hash,
        char*                       nt_response,
        PANSC_CRYPTO_KEY            key
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP2_GET_AKEY)
    (
        char*                       master_key,
        PANSC_CRYPTO_KEY            key,
        BOOL                        bSend,
        BOOL                        bServer
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP2_GET_NKEY)
    (
        char*                       initial_key,
        PANSC_CRYPTO_KEY            key
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_MSCHAP2_GET_AUTH)
    (
        char*                       pw_hash,
        char*                       nt_response,
        char*                       challenge,
        PANSC_CRYPTO_HASH           hash
    );

typedef  ULONG
(*PFN_CRYPTO_HASH_MD5)
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );

typedef  ULONG
(*PFN_CRYPTO_HASH_SHA1)
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );

typedef  ULONG
(*PFN_CRYPTO_HASH_MD4)
    (
        PVOID                       buffer,
        ULONG                       size,
        ULONG                       modulus
    );

typedef  ULONG
(*PFN_CRYPTO_GET_RANDOM1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    );

typedef  USHORT
(*PFN_CRYPTO_GET_RANDOM2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_GET_RANDOM3)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed,
        PUCHAR                      pOctetStream,
        ULONG                       ulSize
    );

typedef  BOOL
(*PFN_CRYPTO_IS_DES_WEAK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );

  
typedef  ANSC_STATUS
(*PFN_CRYPTO_FIX_PARITY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    );
 
 
typedef  BOOL
(*PFN_CRYPTO_CHECK_PARITY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    );

  
typedef  ANSC_STATUS
(*PFN_CRYPTO_CORRECT_DES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );

  
typedef  ANSC_STATUS
(*PFN_CRYPTO_DES_STR_TO_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString,
        ULONG                       ulStringLeng,
        ANSC_HANDLE                 hSalt,
        ULONG                       ulSaltLeng,
        ANSC_HANDLE                 hNewKey
    ); 

typedef UCHAR
(*PFN_CRYPTO_REVERSE_BYTE)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue
    );

typedef UCHAR
(*PFN_CRYPTO_XORE_BYTE)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue1,
        UCHAR                       ucValue2
    );


typedef ANSC_STATUS
(*PFN_CRYPTO_N_FOLD)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInBitNum,
        ULONG                       ulOutBitNum,
        PUCHAR                      pOutString
    );
      
  
typedef ANSC_STATUS
(*PFN_CRYPTO_RANDOM_TO_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       algorithm,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );
 
    
typedef ANSC_STATUS
(*PFN_CRYPTO_TYPE_TO_SIZE)
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

  
typedef ANSC_STATUS
(*PFN_CRYPTO_DES_RANDOM2KEY)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );

 
typedef ANSC_STATUS
(*PFN_CRYPTO_DES3_RANDOM2KEY)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    );

typedef ANSC_STATUS
(*PFN_CRYPTO_DERIVE_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBaseKey,
        ULONG                       EncryptAlgorithm,
        ANSC_HANDLE                 hConstant,
        ULONG                       ulConstantLeng,
        ANSC_HANDLE                 hNewKey
    );
     
 
typedef ANSC_STATUS
(*PFN_CRYPTO_DES3_STR_TO_KEY)
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


typedef  ANSC_STATUS
(*PFN_CRYPTO_RC2_KEY)
    ( 
        PUSHORT                     xkey,
        PUCHAR                      key,
        ULONG                       len,
        ULONG                       bits 
    );

/**********************************************************************\
* Encrypt an RC2_BLOCK_SIZE-byte block of ciphertext using the given key.          
\**********************************************************************/
typedef  ANSC_STATUS
(*PFN_CRYPTO_RC2_ENC)
    (
        PUSHORT                     xkey,
        PUCHAR                      plain,
        PUCHAR                      cipher 
    );

/**********************************************************************\
* Decrypt an RC2_BLOCK_SIZE-byte block of ciphertext using the given key.          
\**********************************************************************/

typedef  ANSC_STATUS
(*PFN_CRYPTO_RC2_DEC)
    ( 
        PUSHORT                     xkey,
        PUCHAR                      plain,
        PUCHAR                      cipher 
    );

typedef  char*
(*PFN_CRYPTO_GEN_UUID1)
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize       /* must be 6 bytes long */
    );

typedef  char*
(*PFN_CRYPTO_GEN_UUID2)
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize,      /* must be 6 bytes long */
        ULONG                       ulUuidVersion       /* as defined in http://www.ietf.org/internet-drafts/draft-mealling-uuid-urn-05.txt */
    );

/*
 * The Crypto Object provides a set of crypto wrapper functions for other container objects to
 * perform symmetric encryption/decryption, hmac-based authentication, one-way hash function and
 * asymmetric encryption/decryption, and signature verification.
 */
typedef  struct
_ANSC_CRYPTO_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hOwnerContext;

    PFN_CRYPTO_CREATE               Create;
    PFN_CRYPTO_REMOVE               Remove;

    PFN_CRYPTO_DIGEST               Digest;
    PFN_CRYPTO_HMAC_MD5_DIGEST      HmacMd5Digest;
    PFN_CRYPTO_HMAC_SHA1_DIGEST     HmacSha1Digest;
    PFN_CRYPTO_DES_MAC_DIGEST       DesMacDigest;
    PFN_CRYPTO_DES_MAC_K_DIGEST     DesMacKDigest;
    PFN_CRYPTO_KPDK_DIGEST          KpdkDigest;
    PFN_CRYPTO_MD5_DIGEST           Md5Digest;
    PFN_CRYPTO_SHA1_DIGEST          Sha1Digest;
    PFN_CRYPTO_MD4_DIGEST           Md4Digest;
    PFN_CRYPTO_MD2_DIGEST           Md2Digest;

    PFN_CRYPTO_ENCRYPT              Encrypt;
    PFN_CRYPTO_DES_IV64_ENCRYPT     DesIv64Encrypt;
    PFN_CRYPTO_DES_ENCRYPT          DesEncrypt;
    PFN_CRYPTO_3DES_ENCRYPT         TripleDesEncrypt;
    PFN_CRYPTO_RC5_ENCRYPT          Rc5Encrypt;
    PFN_CRYPTO_IDEA_ENCRYPT         IdeaEncrypt;
    PFN_CRYPTO_CAST_ENCRYPT         CastEncrypt;
    PFN_CRYPTO_BLOWFISH_ENCRYPT     BlowfishEncrypt;
    PFN_CRYPTO_3IDEA_ENCRYPT        TripleIdeaEncrypt;
    PFN_CRYPTO_DES_IV32_ENCRYPT     DesIv32Encrypt;
#ifndef _ANSC_CRYPTO_NO_RC2_
    PFN_CRYPTO_RC2_ENCRYPT          Rc2Encrypt;
#endif
    PFN_CRYPTO_AES_ENCRYPT          AesEncrypt;

#ifndef _ANSC_CRYPTO_NO_RC4_
    PFN_CRYPTO_RC4_ENCRYPT          Rc4Encrypt;
    PFN_CRYPTO_RC4_ENCRYPT          TLSRc4Encrypt;
#endif

    PFN_CRYPTO_DECRYPT              Decrypt;
    PFN_CRYPTO_DES_IV64_DECRYPT     DesIv64Decrypt;
    PFN_CRYPTO_DES_DECRYPT          DesDecrypt;
    PFN_CRYPTO_3DES_DECRYPT         TripleDesDecrypt;
    PFN_CRYPTO_RC5_DECRYPT          Rc5Decrypt;
    PFN_CRYPTO_IDEA_DECRYPT         IdeaDecrypt;
    PFN_CRYPTO_CAST_DECRYPT         CastDecrypt;
    PFN_CRYPTO_BLOWFISH_DECRYPT     BlowfishDecrypt;
    PFN_CRYPTO_3IDEA_DECRYPT        TripleIdeaDecrypt;
    PFN_CRYPTO_DES_IV32_DECRYPT     DesIv32Decrypt;
#ifndef _ANSC_CRYPTO_NO_RC2_
    PFN_CRYPTO_RC2_DECRYPT          Rc2Decrypt;
#endif

#ifndef _ANSC_CRYPTO_NO_RC4_
    PFN_CRYPTO_RC4_DECRYPT          Rc4Decrypt;
    PFN_CRYPTO_RC4_DECRYPT          TLSRc4Decrypt;
#endif
    PFN_CRYPTO_AES_DECRYPT          AesDecrypt;

    PFN_CRYPTO_COMPRESS             Compress;
    PFN_CRYPTO_OUT_COMPRESS         OutCompress;
    PFN_CRYPTO_DEFLATE_COMPRESS     DeflateCompress;
    PFN_CRYPTO_LZS_COMPRESS         LzsCompress;
    PFN_CRYPTO_V42BIS_COMPRESS      V42bisCompress;
    PFN_CRYPTO_ZLIB_COMPRESS        ZlibCompress;
    PFN_CRYPTO_GZIP_COMPRESS        GzipCompress;

    PFN_CRYPTO_DECOMPRESS           Decompress;
    PFN_CRYPTO_OUT_DECOMPRESS       OutDecompress;
    PFN_CRYPTO_DEFLATE_DECOMPRESS   DeflateDecompress;
    PFN_CRYPTO_LZS_DECOMPRESS       LzsDecompress;
    PFN_CRYPTO_V42BIS_DECOMPRESS    V42bisDecompress;
    PFN_CRYPTO_ZLIB_DECOMPRESS      ZlibDecompress;
    PFN_CRYPTO_GZIP_DECOMPRESS      GzipDecompress;

    PFN_CRYPTO_DES_PREPARE_KEY      DesPrepareKey;
    PFN_CRYPTO_MSCHAP1_LM_HASH      MsChap1LmHash;
    PFN_CRYPTO_MSCHAP1_DERIVE_KEY1  MsChap1DeriveKey1;
    PFN_CRYPTO_MSCHAP1_DERIVE_KEY2  MsChap1DeriveKey2;
    PFN_CRYPTO_MSCHAP2_GET_MKEY     MsChap2GetMKey;
    PFN_CRYPTO_MSCHAP2_GET_AKEY     MsChap2GetAKey;
    PFN_CRYPTO_MSCHAP2_GET_NKEY     MsChap2GetNKey;
    PFN_CRYPTO_MSCHAP2_GET_AUTH     MsChap2GetAuthResponse;

    PFN_CRYPTO_HASH_MD5             HashMd5;
    PFN_CRYPTO_HASH_SHA1            HashSha1;
    PFN_CRYPTO_HASH_MD4             HashMd4;

    PFN_CRYPTO_GET_RANDOM1          GetRandomUlong;
    PFN_CRYPTO_GET_RANDOM2          GetRandomUshort;
    PFN_CRYPTO_GET_RANDOM3          GetRandomOctets;

    /*
     * Utility functions
     */
    PFN_CRYPTO_IS_DES_WEAK          IsDesWeakKey;  
    PFN_CRYPTO_FIX_PARITY           FixParity;     
    PFN_CRYPTO_CHECK_PARITY         CheckParity;   
    PFN_CRYPTO_CORRECT_DES          CorrectDesKey; 
    PFN_CRYPTO_REVERSE_BYTE         ReverseByte;
    PFN_CRYPTO_XORE_BYTE            XorByte;
    PFN_CRYPTO_DES_STR_TO_KEY       MitDesStrToKey;

    PFN_CRYPTO_N_FOLD               NFold;           
    PFN_CRYPTO_RANDOM_TO_KEY        RandomToKey;        
    PFN_CRYPTO_TYPE_TO_SIZE         MapEncTypeToKeySize;
    PFN_CRYPTO_DES_RANDOM2KEY       DesRandomToKey;     
    PFN_CRYPTO_DES3_RANDOM2KEY      Des3RandomToKey;    
    PFN_CRYPTO_DERIVE_KEY           DeriveKey;       
    PFN_CRYPTO_DES3_STR_TO_KEY      Des3StringToKey;     

#ifndef _ANSC_CRYPTO_NO_RC2_
    PFN_CRYPTO_RC2_KEY              Crypt_rc2_keyschedule;
    PFN_CRYPTO_RC2_ENC              Crypt_rc2_encrypt;
    PFN_CRYPTO_RC2_DEC              Crypt_rc2_decrypt;
#endif

#ifndef _ANSC_CRYPTO_NO_UUID
    PFN_CRYPTO_GEN_UUID1            GenerateUuid;   /* can be default to either verion 1 or 2 */
    PFN_CRYPTO_GEN_UUID2            GenerateUuid2;
#endif
}
ANSC_CRYPTO_OBJECT,  *PANSC_CRYPTO_OBJECT;


#endif
