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

    MODULE: crypto_spi.h    (crypto service provider interface)

        For CyberWall IAD Project,
        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    FUNCTIONAL DESCRIPTION:

        This header file defines some data types/apis to hardware
        acceleration engine.

    ---------------------------------------------------------------

    ENVIRONMENT:

        Platform Independent.

    ---------------------------------------------------------------

    AUTHOR:

        Hua Ding

    ---------------------------------------------------------------

    TERMS:

    crypto service provider -- the module which provides cryptographic
        functionality, hardware acceleration engine is one

    crypto users -- the module which uses cryptographic functionality.

    crypto service moderator -- it holds all service provider information
        and provide it to crypto users.

    crypto service interface -- the interface among crypto service 
        moderator, service provider and users

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   01/05/01    initial revision

**********************************************************************/

#ifndef  _AL_CRYPTO_SERVICE_API_
#define  _AL_CRYPTO_SERVICE_API_


/**********************************************************************
    We are trying to unify the interfaces for different algorithms:
        Encryption
        Hash
        Authention
        Public Key
        Compression
        Randum Number Generation

    For each operation, the interface will include following elements:
        Context Handle
        Algorithm Code
        Operation Code
        Parameter Block
        Completion Callback Routine
        Completion Callback Context Handle

    Algorithm/Operation code will be organized in a unified way;
    Parameter Block is algorithm specific.
**********************************************************************/

/**********************************************************************
              ALGORITHM/OPERATION CODE DEFINITIONS
**********************************************************************/

typedef  enum
_AL_CRYPTO_OPERATION
{
    AL_CRYPTO_OPERATION_NULL                        = 0,

    /*
     *  Codes for Encryption -- public key encryption uses this too
     */
    AL_CRYPTO_OPERATION_ENCRYPT                     = 1,
    AL_CRYPTO_OPERATION_DECRYPT,

    /*
     *  Hash
     */
    AL_CRYPTO_OPERATION_HASH,

    /*
     *  Hash based Authentication
     */
    AL_CRYPTO_OPERATION_DIGEST,

    /*
     *  Encryption, then Hash
     */
    AL_CRYPTO_OPERATION_ENCRYPT_HASH,
    AL_CRYPTO_OPERATION_HASH_DECRYPT,

    /*
     *  Compression
     */
    AL_CRYPTO_OPERATION_COMPRESS,
    AL_CRYPTO_OPERATION_DECOMPRESS,

    /*
     *  Public Key sigature
     */
    AL_CRYPTO_OPERATION_SIGN,
    AL_CRYPTO_OPERATION_VERIFY,

    /*
     *  Generate key/key pair/random number
     */
    AL_CRYPTO_OPERATION_GENERATE,

    /*
     *  Public Key exchange -- generate diffie-hellman secret key
     */
    AL_CRYPTO_OPERATION_DH_AGREE,

    /*
     *  bind to a specific handle in crypto service provider
     *
     *  *** this feature requires no dynamic CSP deregistration right now ***
     */
    AL_CRYPTO_OPERATION_BIND,
    AL_CRYPTO_OPERATION_UNBIND
}
AL_CRYPTO_OPERATION, *PAL_CRYPTO_OPERATION;


typedef	enum
_AL_CRYPTO_ALGORITHM
{
    /*
     *  for identifying invalid algorithm
     */
    AL_CRYPTO_ALGORITHM_NULL                        = 0,

    /* Symmetric Encryption */
    AL_CRYPTO_ALGORITHM_DES                         = 1,
    AL_CRYPTO_ALGORITHM_3DES,
    AL_CRYPTO_ALGORITHM_RC5,
    AL_CRYPTO_ALGORITHM_IDEA,
    AL_CRYPTO_ALGORITHM_CAST,
    AL_CRYPTO_ALGORITHM_AES,
    AL_CRYPTO_ALGORITHM_BLOWFISH,
    AL_CRYPTO_ALGORITHM_3IDEA,
    AL_CRYPTO_ALGORITHM_RC4,

    /* Hash */
    AL_CRYPTO_ALGORITHM_MD5,
    AL_CRYPTO_ALGORITHM_SHA1,
    AL_CRYPTO_ALGORITHM_TIGER,

    /* HMAC */
    AL_CRYPTO_ALGORITHM_HMAC_MD5,
    AL_CRYPTO_ALGORITHM_HMAC_SHA1,
    AL_CRYPTO_ALGORITHM_DES_MAC,

    /* Public Key */
    AL_CRYPTO_ALGORITHM_DSA,
    AL_CRYPTO_ALGORITHM_RSA_SIGNATURE,
    AL_CRYPTO_ALGORITHM_RSA_ENCRYPTION,
    AL_CRYPTO_ALGORITHM_DH,

    /* Compression -- put here for uniformity */
    AL_CRYPTO_ALGORITHM_OUT,
    AL_CRYPTO_ALGORITHM_DEFLATE,
    AL_CRYPTO_ALGORITHM_LZS,
    AL_CRYPTO_ALGORITHM_V42BIS,

    /* Random Number -- put here for uniformity */
    AL_CRYPTO_ALGORITHM_RANDOM_NUMBER,

    /*
     *  total number of algorithms
     */
    AL_CRYPTO_ALGORITHM_MAX_NUMBER
}
AL_CRYPTO_ALGORITHM, *PAL_CRYPTO_ALGORITHM;


/**********************************************************************

                DEFINITIONS OF PARAMETER BLOCK (PB)

    Note: all sizes are in bytes
**********************************************************************/

typedef  struct
_AL_CRYPTO_BASE_PB
{
    /*
     *  [IN] what kind of crypto function? bind/unbind does not go here.
     */
    AL_CRYPTO_OPERATION             OperationCode;
    /*
     *  [IN] only used for binding request
     */
    AL_CRYPTO_OPERATION             OperationCode2;
    /*
     *  only meaningful for BIND/UNBIND operations
     *  [OUT] unbind -- receives the handle to the csp
     *  [IN]  bind   -- specificies the handle to the csp
     */
    AL_HANDLE                       hProvider;

    /*
     *  optional, for AL_CRYPTO_OPERATION_BIND/AL_CRYPTO_OPERATION_UNBIND
     *  [IN] user's binding handle -- typically a SA entry.
     *
     *  Once User binds to a CSP context, its user binding context can not
     *  be released unless it successfully close down the binding. If the 
     *  binding cannot be closed down because of pending requests, it has
     *  to wait until its completion callback routine gets called with NULL
     *  hSpBindingContext -- see below too.
     */
    AL_HANDLE                       hUserBindingContext;
    /*
     *  optional
     *  [OUT]handle to the bound context set up in the service provider
     *       with AL_CRYPTO_OPERATION_BIND
     *  [IN] handle to the context already set up in the service provider
     *
     *  In the completion callback, this field should always be NON-NULL
     *  value. If it is NULL, it marks the close down of the handle in CSP,
     *  then
     */
    AL_HANDLE                       hSpBindingContext;

    /*
     *  [IN] which algorithm?
     */
    AL_CRYPTO_ALGORITHM             AlgorithmCode;
    /*
     *  optional, for AL_CRYPTO_OPERATION_ENCRYPT_HASH
     *  [IN] this is needed by Encryption + Hash
     */
    AL_CRYPTO_ALGORITHM             AlgorithmCode2;

    /*
     *  optional, usually only for packet engine processing.
     *  [IN] caller already provides the approriate data buffer for CSP
     *  [OUT]the data buffer CSP returned when the operation is done, if necessary
     */
    AL_HANDLE                       hCspDataBuffer;

    /*
     *  usually TRUE for non-packet engine processing; FALSE for packet engine processing
     *  [IN] if caller wants CSP to copy result back to the output buffer
     *  if CSP is allocating its own buffer in the operation.
     */
    BOOLEAN                         bFillOutputBuffer;

    /*
     *  usually only meaningful for packet engine processing.
     *  [OUT]when returns, states if caller owns back its data buffer
     */
    BOOLEAN                         bDataBufferHeld;
}
AL_CRYPTO_BASE_PB, *PAL_CRYPTO_BASE_PB;

/**********************************************
                ENCRYPTION/HASH
**********************************************/

/*
 *  We seperate the keys and supprt overall 64 bytes in single key format.
 */
/* number of bytes in the longest key */
#ifndef  MAXIMUM_KEY_LENGTH
#define  MAXIMUM_KEY_LENGTH                         16
#endif

/* the maximum number of keys in a single encryption or authentication algorithm */
#ifndef  MAXIMUM_KEY_NUMBER
#define  MAXIMUM_KEY_NUMBER                         4
#endif

/*
 *  define key structure
 */
typedef  struct
_AL_CRYPTO_KEY
{
    ULONG                           KeyNumber;
    ULONG                           RoundNumber;
    ULONG                           KeyLength;
    UCHAR                           KeyValue[MAXIMUM_KEY_NUMBER][MAXIMUM_KEY_LENGTH];
}
AL_CRYPTO_KEY, *PAL_CRYPTO_KEY;

/* number of bytes in the longest initialization vector */
#ifndef  MAXIMUM_IV_LENGTH
#define  MAXIMUM_IV_LENGTH                          16
#endif

/*
 * define IV (initialization vector) structure
 */
typedef  struct
_AL_CRYPTO_IV
{
    ULONG                           IVLength;
    UCHAR                           IVValue[MAXIMUM_IV_LENGTH];
}
AL_CRYPTO_IV,  *PAL_CRYPTO_IV;

/*
 *  for symmetric encryption algorithms
 */
typedef  struct
_AL_CRYPTO_ENCRYPT_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    PVOID                           pInput;
    ULONG                           InputSize;
    ULONG                           EncryptionStartOffset;
    ULONG                           EncryptionLength;

    /*
     *  start point to received output data -- encrypted data starts from the encryption offset
     */
    PVOID                           pOutput;
    /*
     *  IN  -- size of the output buffer
     *  OUT -- output size
     */
    ULONG                           OutputSize;

    /*
     *  the key
     */
    AL_CRYPTO_KEY                   Key;
    /*
     *  IV if necessary; otherwise NULL
     */
    AL_CRYPTO_IV                    IV;
    /*
     *  if IV needs to be updated with the last encrypted block
     */
    BOOLEAN                         IfUpdateIV;
}
AL_CRYPTO_ENCRYPT_PB, *PAL_CRYPTO_ENCRYPT_PB;

typedef  AL_CRYPTO_ENCRYPT_PB       AL_CRYPTO_ENCRYPTION_PARAMETERS, *PAL_CRYPTO_ENCRYPTION_PARAMETERS;

/*
 *  for hash algorithms
 */
typedef  struct
_AL_CRYPTO_HASH_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    PVOID                           pInput;
    ULONG                           InputSize;
    ULONG                           HashStartOffset;
    ULONG                           HashLength;

    /*
     *  start point to receive the hash result
     */
    PVOID                           pOutput;
    /*
     *  IN  -- size of the output buffer
     *  OUT -- output size
     */
    ULONG                           OutputSize;

    /*
     *  supplies the key if HMAC/MAC is desired; otherwise key number = 0
     */
    AL_CRYPTO_KEY                   Key;
}
AL_CRYPTO_HASH_PB, *PAL_CRYPTO_HASH_PB;

typedef  AL_CRYPTO_HASH_PB          AL_CRYPTO_HASH_PARAMETERS, *PAL_CRYPTO_HASH_PARAMETERS;


/*
 *  for encryption + hash 
 */
typedef  struct
_AL_CRYPTO_CRYPT_HASH_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    /*
     *  ESP:
     *
     *              |<-------- ESP HEADER ------>|<--------------- ENCRYPTED PAYLOAD ------------------>|
     *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
     *  * IP HEADER * SPI | SEQUENCE NUMBER | IV * IP DATAGRAM * PADDING * PADDING LENGTH | NEXT HEADER * ICV *
     *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
     *                                                                   |<---------- ESP TAIL -------->|
     *              |<----------------------------- AUTHENTICATED BY ICV ------------------------------>|
     *
     *
     *  Encryption + Hash:
     *
     *  |<------------------------------------ Input Buffer (InputSize) ------------------------------------->|
     *  *-----------*----------------------------*------------------------------------------------------*-----*
     *  *CLEAR TEXT *       HASH START           *       ENCRYPTION START                               *     *
     *  *-----------*----------------------------*------------------------------------------------------*-----*
     *                                           |<---------------------------- Encryption ------------>|
     *              |<-------------------------------------- Hash ------------------------------------->|
     *
     *                                           |<------------------------- real Output -------------------->|
     *  |<--------------------------------- Output Buffer (OutputSize) -------------------------------------->|
     *
     *
     *  Hash + Decryption:
     *
     *                                                                                           HashOutputSize
     *                                                                                                     |
     *                                                                                         pHashResult |
     *                                                                                                  v  v
     *  |<---------------------------------- Input Buffer (InputSize) --------------------------------->|< | >|
     *  *-----------*----------------------------*------------------------------------------------------*-----*
     *  *CLEAR TEXT *       HASH START           *       ENCRYPTION START                               *     *
     *  *-----------*----------------------------*------------------------------------------------------*-----*
     *                                           |<---------------------------- Encryption ------------>|
     *              |<-------------------------------------- Hash ------------------------------------->|
     *
     *                                           |<------------------- real Output -------------------->|
     *  |<--------------------------------- Output Buffer (OutputSize) -------------------------------->|
     *
     */
    PVOID                           pInput;
    ULONG                           InputSize;
    ULONG                           HashStartOffset;
    ULONG                           HashLength;
    ULONG                           EncryptionStartOffset;
    ULONG                           EncryptionLength;

    /*
     *  start point to received the output -- encrypted/decrypted data starts from its offset
     */
    PVOID                           pOutput;
    /*
     *  start point of hash result for verification
     */
    PVOID                           pHashResult;
    /*
     *  IN  -- output buffer size
     *  OUT -- encryption + hash output size
     */
    ULONG                           OutputSize;
    /*
     *  IN  -- hash output buffer size, or the size of hash result to be verified.
     *         Note: the size of result to be verified will not be the standard hash output size
     *  OUT -- hash output size
     */
    ULONG                           HashOutputSize;

    /*
     *  the encryption key
     */
    AL_CRYPTO_KEY                   Key;
    /*
     *  IV if necessary; otherwise NULL
     */
    AL_CRYPTO_IV                    IV;
    /*
     *  if IV needs to be updated with the last encrypted block
     */
    BOOLEAN                         IfUpdateIV;
    /*
     *  Key for HMAC/MAC
     */
    AL_CRYPTO_KEY                   HashKey;
}
AL_CRYPTO_CRYPT_HASH_PB, *PAL_CRYPTO_CRYPT_HASH_PB;

typedef  AL_CRYPTO_CRYPT_HASH_PB    AL_CRYPTO_ENCRYPT_HASH_PB, *PAL_CRYPTO_ENCRYPT_HASH_PB;

/**********************************************
            PUBLIC KEY ALGORITHMS
**********************************************/

/*
 *  variable length data type -- we can only support 2048 bits because of the following definitions
 */
#define  MAXIMUM_PRIME_SIZE                         272
#define  MAXIMUM_INTEGER_SIZE                       272

typedef  struct
_AL_CRYPTO_PRIME
{
    ULONG                           Length;
    UCHAR                           Data[MAXIMUM_PRIME_SIZE];
}
AL_CRYPTO_PRIME,  *PAL_CRYPTO_PRIME;

typedef  struct
_AL_CRYPTO_INTEGER
{
    ULONG                           Length;
    union
    {
        ULONG                       ulData;
        UCHAR                       ucData[MAXIMUM_INTEGER_SIZE];
    } Data;
}
AL_CRYPTO_INTEGER,  *PAL_CRYPTO_INTEGER;

/*
 *  for public key based algorithms
 */
typedef struct
_AL_CRYPTO_RSA_PUBLIC_KEY
{
    AL_CRYPTO_PRIME                 Modulus;                /* n = pq */
    AL_CRYPTO_INTEGER               PublicExponent;         /* e */
}
AL_CRYPTO_RSA_PUBLIC_KEY,  *PAL_CRYPTO_RSA_PUBLIC_KEY;

typedef struct
_AL_CRYPTO_RSA_PRIVATE_KEY
{
    /*
     * shall be 0 for the current version of PKCS #1
     */
    ULONG                           Version;
    AL_CRYPTO_PRIME                 Modulus;                /* n = pq */
    AL_CRYPTO_INTEGER               PublicExponent;         /* e */
    AL_CRYPTO_INTEGER               PrivateExponent;        /* d */
    AL_CRYPTO_PRIME                 Prime1;                 /* p */
    AL_CRYPTO_PRIME                 Prime2;                 /* q */
    AL_CRYPTO_INTEGER               Exponent1;              /* d mod (p - 1) */
    AL_CRYPTO_INTEGER               Exponent2;              /* d mod (q - 1) */
    AL_CRYPTO_INTEGER               CoEfficient;            /* (inverse of q) mod p */
}
AL_CRYPTO_RSA_PRIVATE_KEY,  *PAL_CRYPTO_RSA_PRIVATE_KEY;

typedef struct
_AL_CRYPTO_DSA_PUBLIC_KEY
{
    AL_CRYPTO_INTEGER               ParamP;                 /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    AL_CRYPTO_INTEGER               ParamQ;                 /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    AL_CRYPTO_INTEGER               ParamG;                 /* h^(p - 1) mod p */
    AL_CRYPTO_INTEGER               ParamY;                 /* g^x mod p */
}
AL_CRYPTO_DSA_PUBLIC_KEY,  *PAL_CRYPTO_DSA_PUBLIC_KEY;

typedef struct
_AL_CRYPTO_DSA_PRIVATE_KEY
{
    AL_CRYPTO_INTEGER               ParamP;                 /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    AL_CRYPTO_INTEGER               ParamQ;                 /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    AL_CRYPTO_INTEGER               ParamG;                 /* h^(p - 1) mod p */
    AL_CRYPTO_INTEGER               ParamY;                 /* g^x mod p */
    AL_CRYPTO_INTEGER               ParamX;                 /* a randomly or pseudorandomly generated integer with 0 < x < q */
}
AL_CRYPTO_DSA_PRIVATE_KEY,  *PAL_CRYPTO_DSA_PRIVATE_KEY;

/*
 *  set:    user doesn't need to do digest(hash), csp needs to do it -- crypto user is not using this right now
 *  clear:  user is supplying digest(hash result), csp doesn't need to do hash
 */
#define  AL_CRYPTO_PUBLIC_KEY_MASK_RAW_DATA         0x00000001

/*
 *  if the digest supplied by user has been already encoded into pkcs1 format -- hash oid + digest.
 *  only for sign/verify
 */
#define  AL_CRYPTO_RSA_MASK_PKCS1_ENCODED           0x00010000

#define  AL_CRYPTO_RSA_MASK_SHA1                    0x00100000
#define  AL_CRYPTO_RSA_MASK_MD5                     0x00200000

/*
 *  for RSA/DSA -- both use this structure to pass back/forth parameters
 */
typedef struct
_AL_CRYPTO_PUBLIC_KEY_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    PVOID                           pInput;
    ULONG                           InputSize;
    /*
     *  sign/encryption/decryption  -- [OUT] result data
     *  vefify                      -- [IN]  signature to be verified
     */
    PVOID                           pOutput;
    /*
     *  IN  -- size of the output buffer
     *  OUT -- output size
     */
    ULONG                           OutputSize;

    union
    {
        union
        {
            AL_CRYPTO_RSA_PUBLIC_KEY    RsaPublicKey;
            AL_CRYPTO_RSA_PRIVATE_KEY   RsaPrivateKey;
        } RSA;
        union
        {
            AL_CRYPTO_DSA_PUBLIC_KEY    DsaPublicKey;
            AL_CRYPTO_DSA_PRIVATE_KEY   DsaPrivateKey;
        } DSA;
    } Key;

    ULONG                           Mask;
    AL_CRYPTO_ALGORITHM             HashAlgorithm;
}
AL_CRYPTO_PUBLIC_KEY_PB, *PAL_CRYPTO_PUBLIC_KEY_PB;

typedef  AL_CRYPTO_PUBLIC_KEY_PB    AL_CRYPTO_PUBLIC_KEY_PARAMETERS, *PAL_CRYPTO_PUBLIC_KEY_PARAMETERS;


/*
 *  for RSA/DSA key pair generation
 */
typedef struct
_AL_CRYPTO_PUBLIC_KEY_GEN_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    union
    {
        AL_CRYPTO_DSA_PRIVATE_KEY   DSA;
        AL_CRYPTO_RSA_PRIVATE_KEY   RSA;
    } PrivateKey;

    union
    {
        AL_CRYPTO_DSA_PUBLIC_KEY    DSA;
        AL_CRYPTO_RSA_PUBLIC_KEY    RSA;
    } PublicKey;
}
AL_CRYPTO_PUBLIC_KEY_GEN_PB, *PAL_CRYPTO_PUBLIC_KEY_GEN_PB;

typedef  AL_CRYPTO_PUBLIC_KEY_GEN_PB
            AL_CRYPTO_PUBLIC_KEY_GENERATION_PARAMETERS, *PAL_CRYPTO_PUBLIC_KEY_GENERATION_PARAMETERS;


/*
 *  for Diffie-Hellman
 */
typedef struct
_AL_CRYPTO_DH_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    AL_CRYPTO_PRIME                 GroupPrime;                 /* p */
    AL_CRYPTO_INTEGER               GroupGenerator;             /* g */
    AL_CRYPTO_INTEGER               PrivateKey;                 /* x */
    AL_CRYPTO_INTEGER               PublicKey;                  /* X */
    AL_CRYPTO_INTEGER               PeerPublicKey;              /* Y */
    AL_CRYPTO_INTEGER               SharedSecret;               /* k */
}
AL_CRYPTO_DH_PB, *PAL_CRYPTO_DH_PB;

typedef  AL_CRYPTO_DH_PB            AL_CRYPTO_DH_PARAMETERS, *PAL_CRYPTO_DH_PARAMETERS;


/**********************************************
                COMPRESSION
**********************************************/

/*
 * some compression algorithms have different performance controled by different parameters
 */
#define	LZS_PERFORMANCE_MODE_0                      0x0000
#define LZS_PERFORMANCE_MODE_1                      0x0008
#define LZS_PERFORMANCE_MODE_2                      0x0010

#define LZS_DEFAULT_PERFORMANCE_VALUE               8

typedef struct
_AL_CRYPTO_COMPRESSION_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    PVOID                           pInput;
    ULONG                           InputSize;
    ULONG                           CompressStartOffset;
    ULONG                           CompressLength;

    PVOID                           pOutput;
    /*
     *  IN  -- size of the output buffer
     *  OUT -- output size
     */
    ULONG                           OutputSize;

    /*
     *  only for LZS -- see above definitions
     */
    ULONG                           PerformanceMode;
    ULONG                           PerformanceValue;
}
AL_CRYPTO_COMPRESSION_PB, *PAL_CRYPTO_COMPRESSION_PB;

typedef  AL_CRYPTO_COMPRESSION_PB   AL_CRYPTO_COMPRESSION_PARAMETERS, *PAL_CRYPTO_COMPRESSION_PARAMETERS;


/**********************************************
          RANDOM NUMBER GENERATION
**********************************************/

typedef  struct
_AL_CRYPTO_RANDOM_PB
{
    /*
     *  base parameters
     */
    AL_CRYPTO_BASE_PB               BasePb;

    /*
     *  How many random bytes are needed
     */
    ULONG                           ulSizeOfData;
    /*
     *  Receive the stream of random bytes.
     */
    PUCHAR                          pRandomData;
}
AL_CRYPTO_RANDOM_PB, *PAL_CRYPTO_RANDOM_PB;

typedef  AL_CRYPTO_RANDOM_PB        AL_CRYPTO_RANDOM_PARAMETERS, *PAL_CRYPTO_RANDOM_PARAMETERS;


/**********************************************************************
             CRYPTO SERVICE PROVIDER INTERFACE DEFINITIONS
**********************************************************************/

/**********************************************************************

    function:   *AL_CRYPTO_GET_CAPACITY_CALLBACK

 description:   This API function will be called by crypto users to
                discover capacities this crypto service provider can 
                provide.

      return:   status of operation
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_INVALID_PARAMETER
                *   AL_STATUS_INSUFFICIENT_BUFFER

    argument:   AL_HANDLE                   hProviderContext
                the context handle Crypto Service Provider registered

                PAL_CRYPTO_ALGORITHM        pSupportedAlgorithms
                pointer to the buffer to receive supported algorithms

                PULONG                      pNumberOfAlgorithms
                [IN]pass in the number of algorithms the receive buffer
                can hold; [OUT]return the actual number of algorithms

**********************************************************************/

typedef AL_STATUS
(*AL_CRYPTO_GET_CAPACITY_CALLBACK)
    (
        AL_HANDLE                   hProviderContext,
        PAL_CRYPTO_ALGORITHM        pSupportedAlgorithms,
        PULONG                      pNumberOfAlgorithms
    );

/**********************************************************************

    function:   *AL_CRYPTO_COMPLETION_CB

 description:   This callback function will be called by crypto service
                provider to notify crypto user of the processing 
                completion.

      return:   VOID

    argument:   AL_HANDLE                   hCallbackContext
                the context handle crypto user provided when submitting
                request

                AL_STATUS                   statusCode
                indicates the result of processing
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_FAILURE

**********************************************************************/

typedef VOID
(*AL_CRYPTO_COMPLETION_CB)
    (
        AL_HANDLE                   hCallbackContext,
        AL_STATUS                   statusCode
    );


/**********************************************************************

    function:   *AL_CRYPTO_REQUEST_CALLBACK

 description:   This API funciton will be called by crypto users to
                submit request to crypto service provider.

      return:   status of operation
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_INVALID_PARAMETER
                *   AL_STATUS_INSUFFICIENT_BUFFER
                ...
                *   AL_STATUS_PENDING (completion callback will be called)

    argument:   AL_HANDLE                   hProviderContext
                the context handle Crypto Service Provider registered

                PVOID                       pParameters
                request parameters, one of following:
                    PAL_CRYPTO_ENCRYPT_PB
                    PAL_CRYPTO_HASH_PB
                    PAL_CRYPTO_ENCRYPT_HASH_PB
                    PAL_CRYPTO_PUBLIC_KEY_PB
                    PAL_CRYPTO_PUBLIC_KEY_GEN_PB
                    PAL_CRYPTO_DH_PB
                    PAL_CRYPTO_COMPRESSION_PB
                    PAL_CRYPTO_RANDOM_PB

                AL_CRYPTO_COMPLETION_CB     pCompletionCallback
                completion callback to be called if the request cannot
                be finished upon return.
                It is up to provider to use it or not, if this callback
                is supplied.

                AL_HANDLE                   hCompletionHandle
                the context handle to this request if to be completed
                later

**********************************************************************/

typedef AL_STATUS
(*AL_CRYPTO_REQUEST_CALLBACK)
    (
        AL_HANDLE                   hProviderContext,
        PVOID                       pParameters,
        AL_CRYPTO_COMPLETION_CB     pCompletionCallback,
        AL_HANDLE                   hCompletionHandle
    );


/**********************************************************************

    function:   *AL_CRYPTO_NOTIFY_USER_CALLBACK

 description:   This API function will be called by crypto moderator to
                notify user there is change of registered providers.

      return:   status of operation
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_INVALID_PARAMETER

    argument:   AL_HANDLE                   hUserContext
                the context handle Crypto User registered

**********************************************************************/

typedef AL_STATUS
(*AL_CRYPTO_NOTIFY_USER_CALLBACK)
    (
        AL_HANDLE                   hUserContext
    );


/**********************************************************************
                   CRYPTO SERVICE INTERFACE API
**********************************************************************/

/* It is up to Crypto User to make choices on priorities */
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE1      0x0001
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE2      0x0002
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE3      0x0003
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE4      0x0004
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE5      0x0005
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE6      0x0006
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE7      0x0007
#define  AL_CRYPTO_PROVIDER_PRIORITY_HARDWARE8      0x0008

#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE1      0x0011
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE2      0x0012
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE3      0x0013
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE4      0x0014
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE5      0x0015
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE6      0x0016
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE7      0x0017
#define  AL_CRYPTO_PROVIDER_PRIORITY_SOFTWARE8      0x0018


/*
 *  service provider characteristics
 */
typedef struct
_AL_CRYPTO_PROVIDER_CHAR
{
    char                            ProviderName[32];
    ULONG                           ProviderPriority;

    /*
     *  if the CSP only supports requests after binding.
     */
    BOOLEAN                         bBoundReqOnly;

    AL_CRYPTO_GET_CAPACITY_CALLBACK GetCapacityCallback;
    AL_CRYPTO_REQUEST_CALLBACK      RequestCallback;

    /* internal use */
    AL_HANDLE                       hProviderContext;
}
AL_CRYPTO_PROVIDER_CHAR, *PAL_CRYPTO_PROVIDER_CHAR;


/*
 *  crypto user characteristics
 *  At this moment, user registeration is just to get notification when
 *  there is any provider change. If user doesn't care about this, it
 *  doesn't need to register itself.
 */
typedef struct
_AL_CRYPTO_USER_CHAR
{
    char                            UserName[32];
    AL_CRYPTO_NOTIFY_USER_CALLBACK  NotifyCallback;
}
AL_CRYPTO_USER_CHAR, *PAL_CRYPTO_USER_CHAR;


/**********************************************************************

    function:   ALCryptoRegisterServiceProvider

 description:   This API is called by service provider to register to
                crypto service moderator.

      return:   status of operation

    argument:   AL_HANDLE                           hProviderContext
                the context handle for future reference to this provider

                PAL_CRYPTO_PROVIDER_CHAR            pServiceChar
                this must be provided to let crypto service API interact
                with this provider

**********************************************************************/

AL_STATUS
ALCryptoRegisterServiceProvider
    (
        AL_HANDLE                   hProviderContext,
        PAL_CRYPTO_PROVIDER_CHAR    pServiceChar
    );

/**********************************************************************

    function:   ALCryptoDeregisterServiceProvider

 description:   This API is called by service provider to deregister
                itself from crypto service moderator.

      return:   status of operation
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_INVALID_PARAMETER

    argument:   AL_HANDLE                           hProviderContext
                [IN]the context handle this provider ever registered with.

**********************************************************************/

AL_STATUS
ALCryptoDeregisterServiceProvider
    (
        AL_HANDLE                   hProviderContext
    );

/**********************************************************************

    function:   ALCryptoRegisterUser

 description:   This API is called by crypto user to register itself
                crypto service moderator.

      return:   status of operation

    argument:   AL_HANDLE                   hUserContext
                the context handle for future reference to this user

                PAL_CRYPTO_USER_CHAR        pUserChar
                this must be provided to let crypto service API interact
                with this user

**********************************************************************/

AL_STATUS
ALCryptoRegisterUser
    (
        AL_HANDLE                   hUserContext,
        PAL_CRYPTO_USER_CHAR        pUserChar
    );

/**********************************************************************

    function:   ALCryptoDeregisterUser

 description:   This API is called by crypto user to deregister
                itself from crypto service moderator.

      return:   status of operation
                *   AL_STATUS_SUCCESS
                *   AL_STATUS_INVALID_PARAMETER

    argument:   AL_HANDLE                   hUserContext
                the context handle this user ever registered with.

**********************************************************************/

AL_STATUS
ALCryptoDeregisterUser
    (
        AL_HANDLE                   hUserContext
    );

/**********************************************************************

    function:   ALCryptoGetAllProvider

 description:   This API is called by crypto users to get info for
                registered crypto service providers.

      return:   status of operation

    argument:   PULONG                      pNumberProviders
                [IN] supplies the total number pProviders can hold
                [OUT]return number of providers registered

                PAL_CRYPTO_PROVIDER_CHAR    pProviders
                return the characteristics table of registered providers

**********************************************************************/

AL_STATUS
ALCryptoGetAllProvider
    (
        PULONG                      pNumberProviders,
        PAL_CRYPTO_PROVIDER_CHAR    pProviders
    );


AL_STATUS
ALCryptoRequest
    (
        PVOID                       pParameters,
        AL_CRYPTO_COMPLETION_CB     pCompletionCallback,
        AL_HANDLE                   hCompletionHandle
    );

#endif  /*_AL_CRYPTO_SERVICE_API_*/

