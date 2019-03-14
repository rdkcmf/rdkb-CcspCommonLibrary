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

    module:	ansc_crypto_pub.h

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

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/13/02    initial revision.
        09/23/03    zhubin added RSA private key encryption and public
                    key decryption.

**********************************************************************/


#ifndef  _ANSC_CRYPTO_PUB_
#define  _ANSC_CRYPTO_PUB_


/*
 *  variable length data type
 */
#define  ANSC_CRYPTO_PUB_MAX_PRIME_SIZE             512
#define  ANSC_CRYPTO_PUB_MAX_INTEGER_SIZE           512


/*
 * variable length prime
 */
typedef  struct
_ANSC_CRYPTO_PRIME
{
    ULONG                           Length;
    UCHAR                           Data[ANSC_CRYPTO_PUB_MAX_PRIME_SIZE];
}
ANSC_CRYPTO_PRIME,  *PANSC_CRYPTO_PRIME;


/*
 * variable length integer
 */
typedef  struct
_ANSC_CRYPTO_INTEGER
{
    ULONG                           Length;
    union
    {
        ULONG                       ulData;
        UCHAR                       ucData[ANSC_CRYPTO_PUB_MAX_INTEGER_SIZE];
    } Data;
}
ANSC_CRYPTO_INTEGER,  *PANSC_CRYPTO_INTEGER;

/*
 *  for public key based algorithms
 */
typedef struct
_ANSC_CRYPTO_RSA_PUBLIC_KEY
{
    ANSC_CRYPTO_PRIME               Modulus;                /* n = pq */
    ANSC_CRYPTO_INTEGER             PublicExponent;         /* e */
}
ANSC_CRYPTO_RSA_PUBLIC_KEY,  *PANSC_CRYPTO_RSA_PUBLIC_KEY;


/*
 * RSA private key definition
 */
typedef struct
_ANSC_CRYPTO_RSA_PRIVATE_KEY
{
    /*
     * shall be 0 for the current version of PKCS #1
     */
    ULONG                           Version;
    ANSC_CRYPTO_PRIME               Modulus;                /* n = pq */
    ANSC_CRYPTO_INTEGER             PublicExponent;         /* e */
    ANSC_CRYPTO_INTEGER             PrivateExponent;        /* d */
    ANSC_CRYPTO_PRIME               Prime1;                 /* p */
    ANSC_CRYPTO_PRIME               Prime2;                 /* q */
    ANSC_CRYPTO_INTEGER             Exponent1;              /* d mod (p - 1) */
    ANSC_CRYPTO_INTEGER             Exponent2;              /* d mod (q - 1) */
    ANSC_CRYPTO_INTEGER             CoEfficient;            /* (inverse of q) mod p */
}
ANSC_CRYPTO_RSA_PRIVATE_KEY,  *PANSC_CRYPTO_RSA_PRIVATE_KEY;


/*
 * DSA public key definition
 */
typedef struct
_ANSC_CRYPTO_DSA_PUBLIC_KEY
{
    ANSC_CRYPTO_INTEGER             ParamP;                 /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    ANSC_CRYPTO_INTEGER             ParamQ;                 /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    ANSC_CRYPTO_INTEGER             ParamG;                 /* h^(p - 1) mod p */
    ANSC_CRYPTO_INTEGER             ParamY;                 /* g^x mod p */
}
ANSC_CRYPTO_DSA_PUBLIC_KEY,  *PANSC_CRYPTO_DSA_PUBLIC_KEY;


/*
 * DSA private key definition
 */
typedef struct
_ANSC_CRYPTO_DSA_PRIVATE_KEY
{
    ANSC_CRYPTO_INTEGER             ParamP;                 /* a prime modulus, where 2^(L -1) < p < 2^L, and L a multiple of 64 */
    ANSC_CRYPTO_INTEGER             ParamQ;                 /* a prime divisor of p - 1, where 2^159 < q < 2^160 */
    ANSC_CRYPTO_INTEGER             ParamG;                 /* h^(p - 1) mod p */
    ANSC_CRYPTO_INTEGER             ParamY;                 /* g^x mod p */
    ANSC_CRYPTO_INTEGER             ParamX;                 /* a randomly or pseudorandomly generated integer with 0 < x < q */
}
ANSC_CRYPTO_DSA_PRIVATE_KEY,  *PANSC_CRYPTO_DSA_PRIVATE_KEY;


/*
 *  for RSA/DSA -- both use this structure to pass back/forth parameters
 */
typedef struct
_ANSC_CRYPTO_PUB_KEY_PARAMS
{
    PVOID                           pInput;
    ULONG                           InputSize;
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
            ANSC_CRYPTO_RSA_PUBLIC_KEY    RsaPublicKey;
            ANSC_CRYPTO_RSA_PRIVATE_KEY   RsaPrivateKey;
        } RSA;
        union
        {
            ANSC_CRYPTO_DSA_PUBLIC_KEY    DsaPublicKey;
            ANSC_CRYPTO_DSA_PRIVATE_KEY   DsaPrivateKey;
        } DSA;
    } Key;
}
ANSC_CRYPTO_PUB_KEY_PARAMS, *PANSC_CRYPTO_PUB_KEY_PARAMS;


/*
 *  for RSA/DSA key pair generation
 */
typedef struct
_ANSC_CRYPTO_PUB_KEY_GEN_PARAMS
{
    union
    {
        ANSC_CRYPTO_DSA_PRIVATE_KEY DSA;
        ANSC_CRYPTO_RSA_PRIVATE_KEY RSA;
    } PrivateKey;

    union
    {
        ANSC_CRYPTO_DSA_PUBLIC_KEY  DSA;
        ANSC_CRYPTO_RSA_PUBLIC_KEY  RSA;
    } PublicKey;
}
ANSC_CRYPTO_PUB_KEY_GEN_PARAMS, *PANSC_CRYPTO_PUB_KEY_GEN_PARAMS;


/*
 *  for Diffie-Hellman
 */
typedef struct
_ANSC_CRYPTO_DH_PARAMETERS
{
    ANSC_CRYPTO_PRIME               GroupPrime;             /* p */
    ANSC_CRYPTO_INTEGER             GroupGenerator;         /* g */
    ANSC_CRYPTO_INTEGER             PrivateKey;             /* x */
    ANSC_CRYPTO_INTEGER             PublicKey;              /* X */
    ANSC_CRYPTO_INTEGER             PeerPublicKey;          /* Y */
    ANSC_CRYPTO_INTEGER             SharedSecret;           /* k */
}
ANSC_CRYPTO_DH_PARAMETERS, *PANSC_CRYPTO_DH_PARAMETERS;


/*
 * public key type
 */
typedef  enum
_ANSC_CRYPTO_PUB_KEY_TYPE
{
    ANSC_CRYPTO_PUB_KEY_RESERVED = 0,
    ANSC_CRYPTO_PUB_KEY_RSA,
    ANSC_CRYPTO_PUB_KEY_DSA,
    ANSC_CRYPTO_PUB_KEY_DH
}
ANSC_CRYPTO_PUB_KEY_TYPE,  *PANSC_CRYPTO_PUB_KEY_TYPE;


#if 0
        /*
         * Internal public key definition
         */
        typedef  struct
        _ANSC_CRYPTO_INTERNAL_PUBLIC_KEY
        {
            ANSC_CRYPTO_PUB_KEY_TYPE            KeyType;

            union
            {
                ANSC_CRYPTO_RSA_PUBLIC_KEY      RSAPublicKey;
                ANSC_CRYPTO_DSA_PUBLIC_KEY      DSAPublicKey;
            }Key;
        }
        ANSC_CRYPTO_INTERNAL_PUBLIC_KEY,  *PANSC_CRYPTO_INTERNAL_PUBLIC_KEY;


        /*
         * Internal private key definition
         */
        typedef  struct
        _ANSC_CRYPTO_INTERNAL_PRIVATE_KEY
        {
            ANSC_CRYPTO_PUB_KEY_TYPE            KeyType;

            union
            {
                ANSC_CRYPTO_RSA_PRIVATE_KEY     RSAPrivateKey;
                ANSC_CRYPTO_DSA_PRIVATE_KEY     DSAPrivateKey;
            }Key;
        }
        ANSC_CRYPTO_INTERNAL_PRIVATE_KEY,  *PANSC_CRYPTO_INTERNAL_PRIVATE_KEY;
#endif


typedef	enum
_ANSC_CRYPTO_PUB_ALGORITHM
{
    ANSC_CRYPTO_PUB_ALGORITHM_DSA                         = 0,
    ANSC_CRYPTO_PUB_ALGORITHM_RSA_SIGNATURE,
    ANSC_CRYPTO_PUB_ALGORITHM_RSA_ENCRYPTION,
    ANSC_CRYPTO_PUB_ALGORITHM_DH,
}
ANSC_CRYPTO_PUB_ALGORITHM, *PANSC_CRYPTO_PUB_ALGORITHM;


typedef  enum
_ANSC_CRYPTO_PUB_OPERATION
{
    /* Codes for Encryption -- public key encryption uses this too */
    ANSC_CRYPTO_PUB_OPERATION_ENCRYPT                     = 0,
    ANSC_CRYPTO_PUB_OPERATION_DECRYPT,
    ANSC_CRYPTO_PUB_OPERATION_PRI_ENCRYPT,
    ANSC_CRYPTO_PUB_OPERATION_PUB_DECRYPT,

    /* Public Key sigature */
    ANSC_CRYPTO_PUB_OPERATION_SIGN,
    ANSC_CRYPTO_PUB_OPERATION_VERIFY,

    /* Generate key/key pair */
    ANSC_CRYPTO_PUB_OPERATION_GENERATE,

    /* Public Key exchange */
    ANSC_CRYPTO_PUB_OPERATION_DH_AGREE        /* generate diffie-hellman secret key */
}
ANSC_CRYPTO_PUB_OPERATION, *PANSC_CRYPTO_PUB_OPERATION;

#define ANSC_CRYPTO_PUB_OPERATION_PUB_ENCRYPT   ANSC_CRYPTO_PUB_OPERATION_ENCRYPT
#define ANSC_CRYPTO_PUB_OPERATION_PRI_DECRYPT   ANSC_CRYPTO_PUB_OPERATION_DECRYPT

/*
 * For future use (asynchronous)
 */
typedef VOID
(*ANSC_CRYPTO_FIN_CALLBACK)
    (
        ANSC_HANDLE                 hCallbackContext,
        ANSC_STATUS                 statusCode
    );


#endif
