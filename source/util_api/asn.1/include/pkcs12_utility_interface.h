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

    MODULE: pkcs12_utility_interface.h

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

        *   01/15/2003  initial revision

 **********************************************************************/


#ifndef  _PKCS12_UTILITY_INTERFACE_H
#define  _PKCS12_UTILITY_INTERFACE_H

typedef  enum
_PKCS12_CRYPTO_ALGOR
{
    ALG_pbeWithSHAAnd128BitRC4          = 0,
    ALG_pbeWithSHAAnd40BitRC4,
    ALG_pbeWithSHAAnd3KeyTripleDES_CBC,
    ALG_pbeWithSHAAnd2KeyTripleDES_CBC,
    ALG_pbeWithSHAAnd128BitRC2_CBC,
    ALG_pbeWithSHAAnd40BitRC2_CBC,

    ALG_pbeWithMD2AndDES_CBC,
    ALG_pbeWithMD2AndRC2_CBC,
    ALG_pbeWithMD5AndDES_CBC,
    ALG_pbeWithMD5AndRC2_CBC,
    ALG_pbeWithSHA1AndDES_CBC,
    ALG_pbeWithSHA1AndRC2_CBC,
    ALG_RESERVED
}
PKCS12_CRYPTO_ALGOR,  *PPKCS12_CRYPTO_ALGOR;


/****************************************************************
 *
 * Memory free function
 *
 ****************************************************************/
typedef ANSC_STATUS
(*PKCS12_UTILITY_REMOVE)
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
 typedef BOOLEAN  
 (*PKCS12_UTILITY_PASS_2_BMP)
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
typedef PUCHAR
(*PKCS12_UTILITY_CONCATENATE)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_HASH)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_CONCATENATE_HASH)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_GET_DEVKEY)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_GET_PKCS5_KEY)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_DECRYPT)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_ENCRYPT)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_MAC_AUTH)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_RC2_CBC)
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
typedef ANSC_STATUS
(*PKCS12_UTILITY_RC2)
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


typedef  struct
_PKCS12_UTILITY_INTERFACE
{
    /* start of object class content */                               
    ANSC_HANDLE                     hContainerContext;                
    PANSC_CRYPTO_OBJECT             pCryptAPI;                        
                                                                      
    PKCS12_UTILITY_REMOVE           Remove;                           
    PKCS12_UTILITY_PASS_2_BMP       PassToBMPString;                  
    PKCS12_UTILITY_CONCATENATE      ConcatenateString;                
    PKCS12_UTILITY_HASH             HashMessage;                      
    PKCS12_UTILITY_CONCATENATE_HASH ConcatenateAndHash;               
    PKCS12_UTILITY_GET_DEVKEY       GetDerivedKey;                    
    PKCS12_UTILITY_GET_PKCS5_KEY    GetPKCS5Key;                      
    PKCS12_UTILITY_DECRYPT          PKCS12Decrypt;                    
    PKCS12_UTILITY_ENCRYPT          PKCS12Encrypt;                    
    PKCS12_UTILITY_MAC_AUTH         PKCS12GetHMac;                    
    PKCS12_UTILITY_RC2              RC2;                              
    PKCS12_UTILITY_RC2_CBC          RC2CBC;                           
     /* end of object class content */                                
}
PKCS12_UTILITY_INTERFACE,  *PPKCS12_UTILITY_INTERFACE;


#endif  /*_PKCS12_UTILITY_INTERFACE_H*/


