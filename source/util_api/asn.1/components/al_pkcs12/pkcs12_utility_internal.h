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

    MODULE: pkcs12_utility_internal.h

        The PKCS12 Utility functions for PKCS12 Handling

    ---------------------------------------------------------------

    DESCRIPTION:

        This interface is for the entity of PKI implementation.

        The entity is responsible for pkcs12 encode/decode implementation.

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   04/25/2003  initial revision

 **********************************************************************/


#ifndef  _PKCS12_UTILITY_INTERNAL_H
#define  _PKCS12_UTILITY_INTERNAL_H


/****************************************************************
 *
 * Memory free function
 *
 ****************************************************************/
ANSC_STATUS
PKCS12UtilityRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/****************************************************************
 *
 * Utility functions
 *
 ****************************************************************/
/*
 *  Change password string to BMP strings.
 *
 *  e.g. change "input" to "0x00 i 0x00 n 0x00 p 0x00 u 0x00 t"
 */
BOOLEAN  
PKCS12UtilityPasswordToBMPString
    ( 
        ANSC_HANDLE                 hThisObject,
        PCHAR                       password,     /*(INPUT)*/
        PUCHAR                      pString,      /*(OUT)*/  
        PULONG                      pLength       /*(OUT)*/  
     );

/*
 * Concatenate String, the caller is responsible to release the
 * returned buffer.
 */
PUCHAR
PKCS12UtilityConcatenateString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBBB,
        ULONG                       lenOfB,
        PUCHAR                      pCCC,
        ULONG                       lenOfC,
        PULONG                      pLenOfA
    );

/*
 *  Hash String;
 */
ANSC_STATUS
PKCS12UtilityHashString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pMessage,
        ULONG                       lenOfMessage,
        ULONG                       iteration,
        HASH_TYPE                   hashType,      /* HASH_MD5 or HASH_SHA1 */
        ANSC_HANDLE                 pHash
    );

/*
 * Concatenate String and hash
 */
ANSC_STATUS
PKCS12UtilityConcatenateAndHash
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pMessageA,
        ULONG                       lenOfMessageA,
        PUCHAR                      pMessageB,
        ULONG                       lenOfMessageB,
        ULONG                       hashIteration,
        HASH_TYPE                   hashType,      /* HASH_MD5 or HASH_SHA1 */
        ANSC_HANDLE                 pHash
    );

/*
 *  Get PKCS12 Derived key
 */
ANSC_STATUS
PKCS12UtilityGetDerivedKey
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       KeyID,              /* 1, 2 or 3 */
        PCHAR                       pPasswordString,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        HASH_TYPE                   hashType,           /* HASH_MD5 or HASH_SHA1 */
        PUCHAR                      pKey,
        PULONG                      pLenOfKey
    );

/*
 * Get PKCS5 Derived Key
 */
ANSC_STATUS
PKCS12UtilityGetPKCS5DerivedKey
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pPasswordString,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        HASH_TYPE                   hashType,           /* HASH_MD5 or HASH_SHA1 */
        PUCHAR                      pKeyAndIV,
        PULONG                      pLenOfKeyAndIV
    );

/*
 * Decrypt Message
 */
ANSC_STATUS
PKCS12UtilityDecrypt
    (
        ANSC_HANDLE                 hThisObject,
        PKCS12_CRYPTO_ALGOR         cryptType,
        PCHAR                       pPassword,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        PUCHAR                      pEncryptedData,
        ULONG                       lenOfEncryptedData,
        PUCHAR                      pDecryptedData,
        PULONG                      pLenOfDecryptedData
    );


/*
 * Encrypt Message
 */
ANSC_STATUS
PKCS12UtilityEncrypt
    (
        ANSC_HANDLE                 hThisObject,
        PKCS12_CRYPTO_ALGOR         cryptType,
        PCHAR                       pPassword,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        PUCHAR                      pClearData,
        ULONG                       lenOfClearData,
        PUCHAR                      pCipherData,
        PULONG                      pLenOfCipherData
    );

/*
 *  Mac Auth
 */
ANSC_STATUS
PKCS12UtilityGetHMAC
    (
        ANSC_HANDLE                 hThisObject,
        HASH_TYPE                   type,
        PCHAR                       pPassword,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        PUCHAR                      pMessage,
        ULONG                       lenOfMessage,
        ANSC_HANDLE                 pHash
    );

/*
 *  RC2-CBC Encrypt/Decrypt
 */
ANSC_STATUS
PKCS12UtilityRC2CBC
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     isEncrypt,
        PUCHAR                      pKey,
        ULONG                       lenOfKey,
        ULONG                       bits,
        PUCHAR                      pIVString,
        ULONG                       lenOfIV,
        PUCHAR                      pInputString,
        PUCHAR                      pOutputString,
        ULONG                       lenOfString
    );

/*
 *  RC2 Encrypt/Decrypt
 */
ANSC_STATUS
PKCS12UtilityRC2
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     isEncrypt,
        PUCHAR                      pKey,
        ULONG                       lenOfKey,
        ULONG                       bits,
        PUCHAR                      pInputString,
        PUCHAR                      pOutputString,
        ULONG                       lenOfString
    );



#endif  /*_PKCS12_UTILITY_INTERNAL_H*/


