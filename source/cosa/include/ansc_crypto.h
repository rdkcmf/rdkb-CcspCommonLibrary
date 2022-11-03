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

    module:	ansc_crypto.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        data structures and basic types related to cryptography.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/21/01    initial revision.
        07/24/01    copied from the second revision of /bwsds/ansc
                    project.

**********************************************************************/


#ifndef  _ANSC_CRYPTO_
#define  _ANSC_CRYPTO_


/***********************************************************
     DEFINITION OF SOME GENERAL CRYPTO DATA STRUCTURES
***********************************************************/

/*
 * define some maximum sizes allowed during encryption/decryption/compression/hmac operations
 */
#define  ANSC_MAX_REPLAY_WINDOW_SIZE                4
/* CID 277388, 277392, 277440, 277487, 277615 fix */
#define  ANSC_MAX_KEY_LENGTH                        257
#define  ANSC_MAX_IV_LENGTH                         16
#define  ANSC_MAX_KEY_NUMBER                        5
#define  ANSC_MAX_HASH_RESULT                       32
#define  ANSC_MAX_RAW_BUFFER_SIZE                   8192

/*
 * the key length for HMAC
 * note: actually it's the block size of different hash function / bytes
 */
#define  ANSC_MD4_BLOCK_SIZE                        64
#define  ANSC_MD5_BLOCK_SIZE                        64
#define  ANSC_SHA1_BLOCK_SIZE                       64
#define  ANSC_SHA256_BLOCK_SIZE                     64

/*
 * the output length for hash functions / bytes
 */
#define  ANSC_MD2_OUTPUT_SIZE                       16
#define  ANSC_MD4_OUTPUT_SIZE                       16
#define  ANSC_MD5_OUTPUT_SIZE                       16
#define  ANSC_SHA1_OUTPUT_SIZE                      20
#define  ANSC_SHA256_OUTPUT_SIZE                    32

/*
 * I don't know why the draft guys chose these two values as the padding unit, there must be some
 * kind mathametics consideration involved.
 */
#define  ANSC_HMAC_INNER_PADDING_UNIT               0x36
#define  ANSC_HMAC_OUTER_PADDING_UNIT               0x5C

/*
 * size of the initialization vector and CRC block of encryption algorithm
 */
#define  ANSC_DES_IV_SIZE                           8
#define  ANSC_DES_BLOCK_SIZE                        8
#define  ANSC_RC2_BLOCK_SIZE                        8
#define  ANSC_RC5_BLOCK_SIZE                        8           /* it could be any block size, now we just hardcode it */
#define  ANSC_IDEA_BLOCK_SIZE                       8
#define  ANSC_AES_BLOCK_SIZE                        16

#define  ANSC_DES_KEY_SIZE                          8
#define  ANSC_RC2_KEY_SIZE                          8
#define  ANSC_IDEA_KEY_SIZE                         16

/*
 * Some encryption and decryption algorithm implementions share the same routine, so we have to
 * pass them different flag to indicate encryption or decryption.
 */
#define  ANSC_ENCRYPTION_MODE                       1
#define  ANSC_DECRYPTION_MODE                       0

/*
 * Some compression algorithms have different performance controled by different parameters.
 */
#define	 ANSC_LZS_PERFORMANCE_MODE_0                0x0000
#define  ANSC_LZS_PERFORMANCE_MODE_1                0x0008
#define  ANSC_LZS_PERFORMANCE_MODE_2                0x0010

/*
 * define the structure of keys that we will use in the encryption and authentication algorithms
 */
typedef  struct
_ANSC_CRYPTO_KEY
{
    ULONG                           KeyNumber;
    ULONG                           RoundNumber;
    ULONG                           Length;
    UCHAR                           Value[ANSC_MAX_KEY_NUMBER][ANSC_MAX_KEY_LENGTH];
}
ANSC_CRYPTO_KEY,  *PANSC_CRYPTO_KEY;

/*
 * define IV (initialization vector) structure
 */
typedef  struct
_ANSC_CRYPTO_IV
{
    ULONG                           Length;
    UCHAR                           Value[ANSC_MAX_IV_LENGTH];
}
ANSC_CRYPTO_IV,  *PANSC_CRYPTO_IV;

/*
 * hold the result of hash
 */
typedef  struct
_ANSC_CRYPTO_HASH
{
    ULONG                           Length;
    UCHAR                           Value[ANSC_MAX_HASH_RESULT];
}
ANSC_CRYPTO_HASH,  *PANSC_CRYPTO_HASH;


#endif
