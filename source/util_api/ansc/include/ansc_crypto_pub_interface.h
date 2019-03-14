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

    module:	ansc_crypto_pub_interface.h

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

**********************************************************************/


#ifndef  _ANSC_CRYPTO_PUB_INTERFACE_
#define  _ANSC_CRYPTO_PUB_INTERFACE_

#include "ansc_crypto_pub.h"

/***********************************************************
              GENERAL CRYPTO OBJECT DEFINITION
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_CRYPTO_PUB_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );


typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );


typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_INITIALIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * We don't have Cryptographic Service Provider Interface yet, but
 * we still use the universal API.
 */
typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_REQUEST)
    (
        ANSC_CRYPTO_PUB_ALGORITHM   Algorithm,
        ANSC_CRYPTO_PUB_OPERATION   Operation,
        ANSC_HANDLE                 hParams,
        ANSC_CRYPTO_FIN_CALLBACK    FinishCB,
        ANSC_HANDLE                 hFinishCBParam
    );


typedef  BOOL
(*PFN_CRYPTO_PUB_KEY_PAIR_MATCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_PUB_KEY_TYPE    KeyType,
        ANSC_HANDLE                 hPublicKey,
        ANSC_HANDLE                 hPrivateKey
    );


typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_KEY_HASH_SIGN)
    (
        int                         nHashMethod,
        PVOID                       pDataToSign,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        PULONG                      pulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );


typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_KEY_HASH_VERIFY)
    (
        int                         nHashMethod,
        PVOID                       pDataToVerify,
        ULONG                       ulSizeOfData,
        PVOID                       pSignature,
        ULONG                       ulSizeOfSignature,
        PANSC_CRYPTO_PUB_KEY_PARAMS pParams
    );

typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_DH_KEY_GEN)
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    );


typedef  ANSC_STATUS
(*PFN_CRYPTO_PUB_AGREE_DH_SECRET)
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    );

#define  _ANSC_CRYPTO_PUB_BASE                                                  \
    SINGLE_LINK_ENTRY               Linkage;                                    \
    ANSC_HANDLE                     hOwnerContext;                              \
                                                                                \
    PFN_CRYPTO_PUB_CREATE           Create;                                     \
    PFN_CRYPTO_PUB_REMOVE           Remove;                                     \
    PFN_CRYPTO_PUB_INITIALIZE       Initialize;                                 \
                                                                                \
    PFN_CRYPTO_PUB_REQUEST          Request;                                    \
    PFN_CRYPTO_PUB_KEY_PAIR_MATCH   KeyPairMatch;                               \
    PFN_CRYPTO_PUB_KEY_HASH_SIGN    RSAHashAndSign;                             \
    PFN_CRYPTO_PUB_KEY_HASH_VERIFY  RSAHashAndVerify;                           \
    PFN_CRYPTO_PUB_KEY_HASH_SIGN    DSAHashAndSign;                             \
    PFN_CRYPTO_PUB_KEY_HASH_VERIFY  DSAHashAndVerify;                           \
    PFN_CRYPTO_PUB_DH_KEY_GEN       DHKeyGen;                                   \
    PFN_CRYPTO_PUB_AGREE_DH_SECRET  AgreeDHSecret;                              \



/*
 * The Crypto Object provides a set of crypto wrapper functions for other container objects to
 * perform symmetric encryption/decryption, hmac-based authentication, one-way hash function and
 * asymmetric encryption/decryption, and signature verification.
 */
typedef  struct
_ANSC_CRYPTO_PUB_OBJECT
{
    _ANSC_CRYPTO_PUB_BASE
}
ANSC_CRYPTO_PUB_OBJECT,  *PANSC_CRYPTO_PUB_OBJECT;


#endif
