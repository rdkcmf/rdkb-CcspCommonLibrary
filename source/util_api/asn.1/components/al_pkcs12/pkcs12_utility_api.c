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

    MODULE: pkcs12_utility_api.c

        The PKCS12 Utility functions for PKCS12 Handling

    ---------------------------------------------------------------

    DESCRIPTION:

        This interface is for the pkcs12 utility functions.

        Exported:
            
           * AnscCreatePKCS12Utility

        Internal:

           * PKCS12UtilityRemove
           * PKCS12UtilityPasswordToBMPString
           * PKCS12UtilityHashString
           * PKCS12UtilityConcatenateString
           * PKCS12UtilityConcatenateAndHash
           * PKCS12UtilityGetDerivedKey
           * PKCS12UtilityGetPKCS5DerivedKey
           * PKCS12UtilityDecrypt
           * PKCS12UtilityEncrypt
           * PKCS12UtilityGetHMAC
           * PKCS12UtilityRC2
           * PKCS12UtilityRC2CBC

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   04/24/2003  initial revision

 **********************************************************************/

#include "al_pkcs12_local.h"

ANSC_HANDLE
AnscCreatePKCS12Utility
    (
        ANSC_HANDLE                 hContainer,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PPKCS12_UTILITY_INTERFACE       pThisObject = NULL;
    PANSC_CRYPTO_OBJECT             pCrypto;

    /* create the object */
    pThisObject = (PPKCS12_UTILITY_INTERFACE)
        AnscAllocateMemory(sizeof(PKCS12_UTILITY_INTERFACE));

    if( pThisObject == NULL)
    {
        return NULL;
    }

    /* create the crypto obj */
    pCrypto =
        (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if( pCrypto == NULL)
    {
        AnscFreeMemory(pThisObject);

        return NULL;
    }

    pThisObject->pCryptAPI          = pCrypto;
    pThisObject->hContainerContext  = hContainer;
    pThisObject->Remove             = PKCS12UtilityRemove;
    pThisObject->PassToBMPString    = PKCS12UtilityPasswordToBMPString;
    pThisObject->HashMessage        = PKCS12UtilityHashString;
    pThisObject->ConcatenateString  = PKCS12UtilityConcatenateString;
    pThisObject->ConcatenateAndHash = PKCS12UtilityConcatenateAndHash;
    pThisObject->GetDerivedKey      = PKCS12UtilityGetDerivedKey;
    pThisObject->GetPKCS5Key        = PKCS12UtilityGetPKCS5DerivedKey;
    pThisObject->PKCS12Decrypt      = PKCS12UtilityDecrypt;
    pThisObject->PKCS12Encrypt      = PKCS12UtilityEncrypt;
    pThisObject->PKCS12GetHMac      = PKCS12UtilityGetHMAC;
    pThisObject->RC2                = PKCS12UtilityRC2;
    pThisObject->RC2CBC             = PKCS12UtilityRC2CBC;

    return pThisObject;
}

/****************************************************************
 *
 * Memory free function
 *
 ****************************************************************/
ANSC_STATUS
PKCS12UtilityRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;

    if( pThisObject == NULL)
    {
        return status;
    }

    if( pThisObject->pCryptAPI != NULL)
    {
        pThisObject->pCryptAPI->Remove
            (
                pThisObject->pCryptAPI
            );
    }

    AnscFreeMemory(hThisObject);

    return status;
}

/*********************************************************************
    BOOLEAN  
    PKCS12UtilityPasswordToBMPString
        ( 
            ANSC_HANDLE                 hThisObject,
            PCHAR                       password,     (INPUT)
            PUCHAR                      pString,      (OUT)
            PULONG                      pLength       (OUT)
         )

    Discription:

        Change visible Password to BMPString.
        e.g.  
        "Beavis" 
        to 14 bytes strings
        "0x00 0x42 0x00 0x65 0x00 0x61 0x00 0x76 0x00 0x69 0x73 0x00 0x00"

    Parameter:
        CString         password  (INPUT)
        The given password

        PUCHAR          pString   (OUTPUT)
        The buffer of BMPString, 

        PULONG          pLength
        The length of BMPString buffer.

    Return:
        If success, return true;
        else return false;            

**********************************************************************/  
BOOLEAN  
PKCS12UtilityPasswordToBMPString
    ( 
        ANSC_HANDLE                 hThisObject,
        PCHAR                       password,     /*(INPUT)*/
        PUCHAR                      pString,      /*(OUT)*/  
        PULONG                      pLength       /*(OUT)*/  
     )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           length;
    ULONG                           i;

    if( pString == NULL)
    {
        return FALSE;
    }

    if(password == NULL || AnscSizeOfString(password) == 0)
    {
        return FALSE;
    }

    length = AnscSizeOfString(password) * 2 + 2;

    if(pLength)
    {
        if( *pLength < length)
        {
            return FALSE;
        }

        *pLength = length;
    }

    AnscZeroMemory(pString, length);

    for( i = 0 ; i < length - 2; i ++)
    {
        if( i % 2 != 0)
        {
            pString[i] = (UCHAR)password[ i /2 ];
        }
    }

    return TRUE;
}

/**************************************************************
    PUCHAR
    PKCS12UtilityConcatenateString
        (
            ANSC_HANDLE                 hThisObject,
            PUCHAR                      pBBB,
            ULONG                       lenOfB,
            PUCHAR                      pCCC,
            ULONG                       lenOfC,
            PULONG                      pLenOfA
        )

    Descriptions:

        Concatenate string B and C, return A = B || C

    Parameter:

        PUCHAR   pBBB,
        ULONG    lenOfB,
        PUCHAR   pCCC,
        ULONG    lenOfC,    (Input)
        The string B , C and their length;

        PULONG   pLenOfA    (Output)
        The length of returned string;

    Return:

        If success, return buffer A = B || C, the caller is responsible to 
        release the buffer;others return NULL;

 **************************************************************/   
PUCHAR
PKCS12UtilityConcatenateString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBBB,
        ULONG                       lenOfB,
        PUCHAR                      pCCC,
        ULONG                       lenOfC,
        PULONG                      pLenOfA
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PUCHAR                          pTemp       = NULL;    
    ULONG                           lenOfTemp   = lenOfB + lenOfC;

    if(lenOfTemp == 0)
    {
        return pTemp;
    }

    pTemp = (PUCHAR)AnscAllocateMemory( lenOfTemp);

    if(pTemp == NULL)
    {
        return NULL;
    }

    if( pBBB != NULL)
    {
        AnscCopyMemory( pTemp, pBBB, lenOfB);
    }

    if( pCCC != NULL)
    {
        AnscCopyMemory( pTemp + lenOfB, pCCC, lenOfC);
    }

    if(pLenOfA)
    {
        *pLenOfA = lenOfTemp ;
    }

    return pTemp;
}

/*********************************************************************

    ANSC_STATUS
    PKCS12UtilityHashString
        (
            ANSC_HANDLE                 hThisObject,
            PUCHAR                      pMessage,
            ULONG                       lenOfMessage,
            ULONG                       iteration,
            HASH_TYPE                   hashType,      
            ANSC_HANDLE                 pHash
        )

    Discription:

        Hash the given message with given Hash Type in "iteration" times.

    Parameter:

        PUCHAR          pMessage,
        The message will be hashed;

        ULONG           lenOfMessage,
        The length of message will be hashed;

        LONG            iteration       = 1,
        The Hash iterations;

        HASH_TYPE       type            = HASH_SHA1
        The Hash Method;

        ANSC_HANDLE                 pHash
        The Hash Handle;

    Return:

        If success , return ANSC_STATUS_SUCCESS;
        others, return the error code;

  *********************************************************************/
ANSC_STATUS
PKCS12UtilityHashString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pMessage,
        ULONG                       lenOfMessage,
        ULONG                       iteration,
        HASH_TYPE                   hashType,      /* HASH_MD5 or HASH_SHA1 */
        ANSC_HANDLE                 pHash
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_HASH               pHashResult = (PANSC_CRYPTO_HASH)pHash;
    PANSC_CRYPTO_OBJECT             pCryptoAPI;
    ULONG                           i;

	if( hashType != HASH_MD5 && hashType != HASH_SHA1)
	{
		return ANSC_STATUS_FAILURE;
	}

    if( pHash == NULL || pThisObject == NULL || pThisObject->pCryptAPI == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pCryptoAPI     = pThisObject->pCryptAPI;


    /*
     * hold the output result of one-way hash function
     */

    if( hashType == HASH_MD5)
    {
        pHashResult->Length = ANSC_MD5_OUTPUT_SIZE;

        pCryptoAPI->Md5Digest
            (
                pMessage,
                lenOfMessage,
                pHashResult
            );
    }
    else
    {
        pHashResult->Length = ANSC_SHA1_OUTPUT_SIZE;

        pCryptoAPI->Sha1Digest
            (
                pMessage,
                lenOfMessage,
                pHashResult
            );
    }

    for( i = 0; i < iteration - 1; i ++)
    {
        if( hashType == HASH_MD5)
        {
            pCryptoAPI->Md5Digest
                (
                    pHashResult->Value,
                    pHashResult->Length,
                    pHashResult
                );
        }
        else
        {
            pCryptoAPI->Sha1Digest
                (
                    pHashResult->Value,
                    pHashResult->Length,
                    pHashResult
                );
        }
    }

    return status;
}

/*********************************************************************

    ANSC_STATUS
    PKCS12UtilityConcatenateAndHash
        (
            ANSC_HANDLE                 hThisObject,
            PUCHAR                      pMessageA,
            ULONG                       lenOfMessageA,
            PUCHAR                      pMessageB,
            ULONG                       lenOfMessageB,
            ULONG                       hashIteration,
            HASH_TYPE                   hashType,      
            ANSC_HANDLE                 pHash
        );

    Discription:

        Concatenate MessageA || MessageB , and then Hash them with given Hash Type in "iteration" times.

    Parameter:

        PUCHAR          pMessageA,
        ULONG           lenOfMessageA,
        PUCHAR          pMessageB,
        ULONG           lenOfMessageB,   (In)

        LONG            iteration       = 1,
        The Hash iterations;

        HASH_TYPE       type            = HASH_SHA1
        The Hash Method;

        ANSC_HANDLE                 pHash
        Hash handle;

    Return:

        If success , return ANSC_STATUS_SUCCESS;
        others, return the error code;

  *********************************************************************/
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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;
    PUCHAR                          pCombined   = NULL;
    ULONG                           wholeLen;
    ANSC_STATUS                     status;

    /* Concatenate first */
    pCombined = 
        pThisObject->ConcatenateString
                    ( 
                        pThisObject,
                        pMessageA, 
                        lenOfMessageA, 
                        pMessageB, 
                        lenOfMessageB,
                        &wholeLen
                    );

    if(!pCombined)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Then Hash it */
    status = 
        pThisObject->HashMessage
                   ( 
                      pThisObject,
                      pCombined, 
                      wholeLen, 
                      hashIteration, 
                      hashType,
                      pHash
                    );

    /* Don't forget to free memory */
    AnscFreeMemory(pCombined);

    return status;
}

/*********************************************************************

    ANSC_STATUS
    PKCS12UtilityGetDerivedKey
        (
            ANSC_HANDLE                 hThisObject,
            ULONG                       KeyID,              1, 2 or 3 
            PCHAR                       pPasswordString,
            PUCHAR                      pSaltString,
            ULONG                       lenOfSalt,
            ULONG                       iteration,
            HASH_TYPE                   hashType,           HASH_MD5 or HASH_SHA1
            PUCHAR                      pKey,
            PULONG                      pLenOfKey
        )

    Description:

        Get derived Key with given parameter in PKCS12 Algorithms.

        I = (I + B + 1) mod 2^v 
     
    Parameter:

        ULONG           KeyID,              
        KeyID  == 1 --> Encrypt and Decrypt key;
        KeyID  == 2 --> IV value;
        KeyID  == 3 --> Mac key;
        Others      --> The Same as KeyID == 1;

        PCHAR          pPasswordString,
        The given password;             (Input)
    
        PUCHAR          pSaltString,
        The given Salt String;          (Input)

        ULONG           lenOfSalt,
        The length of Salt String;      (Input)

        LONG            iteration,
        The iterations;

        PUCHAR          pKey,
        The output key buffer;          (Output)

        PULONG          pLenOfKey,
        The output key length;          (Output)

        HASH_TYPE       type            = HASH_SHA1
        The Hash method,  Default is HASH_SHA1;
 
    Return:  

        If success, return AL_STATUS_SUCCESS;
        Otherwise, return AL_STATUS_FAILURE;

*********************************************************************/
BOOLEAN 
UpdateStringIII
    (
        PUCHAR                          pIII,
        PUCHAR                          pBBB,
        ULONG                           length
    ) /* process pIII + pBBB */
{
    int                                 bCarry  = 1;
    LONG                                i;
    UCHAR                               temp;

    if(!pIII || !pBBB || length <= 0)
    {
        return FALSE;
    }

    for( i = length - 1; i >= 0 ; i --)
    {
        temp = pIII[i] + pBBB[i] + bCarry;

        if( temp < pIII[i] && temp < pBBB[i])
        {
            bCarry = 1;
        }
        else if( pIII[i] == 0xFF && pBBB[i] == 0xFF && bCarry)
        {
            bCarry = 1;
        }
        else 
        {
            bCarry = 0;
        }

        pIII[i] =  temp;
    }

    return TRUE;
}


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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    ULONG                           v           = 64;
    PUCHAR                          pDiver      = NULL;
    PUCHAR                          pBMPPassword= NULL; 
    ULONG                           u;
    ULONG                           i, j, n, cycle;
    ULONG                           passLength;
    ULONG                           lenOfBMPPass;
    ULONG                           lenOfPass;
    PUCHAR                          pPass;
    PUCHAR                          pS          = NULL;
    ULONG                           lenOfpS     = 0;
    ULONG                           lenOfIII;
    PUCHAR                          pIII,pAAA,pBBB;
    ANSC_CRYPTO_HASH                hashResult;


    if(hashType != HASH_MD5 && hashType != HASH_SHA1)        
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pPasswordString == NULL || pThisObject == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if(KeyID <= 0 || KeyID > 3)
    {
        KeyID = 1;
    }

    if( hashType == HASH_MD5)
    {
        u = 16 ; 
        hashResult.Length = 16;
    }
    else
    {
        u = 20;
        hashResult.Length = 20;
    }

    switch (KeyID)
    {
        case 1:   n = 24 ;              break;
        case 2:   n = 8  ;              break; 
        case 3:   n = u  ;              break;
    }

    cycle   = (n <= u ) ? 1 : 2;

    /* Construct a String  D (the "Diversifier") with KeyID */
    pDiver  = (PUCHAR)AnscAllocateMemory(v);

    if( pDiver == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Set all the value to KeyID */
    for( i = 0; i < v; i ++)
    {
        pDiver[i] = (UCHAR)KeyID;
    }
    
    /* Change password to BMPString first */
    passLength  = AnscSizeOfString(pPasswordString);
    lenOfBMPPass= passLength * 2 + 8;
    pBMPPassword    = (PUCHAR)AnscAllocateMemory(lenOfBMPPass);

    if(pBMPPassword == NULL)
    {
        AnscFreeMemory(pDiver);

        return   ANSC_STATUS_FAILURE;
    }

    pThisObject->PassToBMPString
        ( 
            pThisObject,
            pPasswordString, 
            pBMPPassword, 
            &lenOfBMPPass
        );


    /* Concatenate copies of password to create P of length v.[p/v] */
    lenOfPass       = v;

    if(lenOfBMPPass > (ULONG)v)
    {
        if(lenOfBMPPass % v == 0)
        {
            lenOfPass = lenOfBMPPass;
        }
        else
        {
            lenOfPass = (lenOfBMPPass/v +1 ) * v;
        }
    }

    pPass    =  (PUCHAR)AnscAllocateMemory(lenOfPass);

    if( pPass == NULL)
    {
        AnscFreeMemory(pDiver);
        AnscFreeMemory(pBMPPassword);
    
        return ANSC_STATUS_FAILURE;
    }

    for( i = 0; i < (ULONG)(lenOfPass/lenOfBMPPass); i ++)
    {
      AnscCopyMemory( pPass + i * lenOfBMPPass, pBMPPassword, lenOfBMPPass);
    }
    
    if( lenOfPass % lenOfBMPPass != 0)
    {
      AnscCopyMemory( pPass + i * lenOfBMPPass, pBMPPassword, lenOfPass % lenOfBMPPass);  
    }
    

    AnscFreeMemory(pBMPPassword);

    /* Concatenate salt together to Create String S */
    pS          = NULL;
    lenOfpS     = 0;
    
    if( lenOfSalt > 0 && pSaltString) /* If Salt exists, create pS */
    {
        /* make  string S of length v.[s/v] */
        if(lenOfSalt > (ULONG)v)
        {
            if(lenOfSalt % v == 0)
            {
                lenOfpS  = lenOfSalt;
            }
            else
            {
                lenOfpS  = (lenOfSalt/v + 1) * v;
            }
        }
        else
        {
            lenOfpS      = v;
        }
        
        pS  = (PUCHAR)AnscAllocateMemory(lenOfpS);

        if( pS == NULL)
        {
            AnscFreeMemory(pDiver);
            /*RDKB-6192 , CID-24189, free resources on return*/
            AnscFreeMemory(pPass);

            return ANSC_STATUS_FAILURE;
        }

        for( i = 0; i < (ULONG)(lenOfpS/lenOfSalt); i ++)
        {
            AnscCopyMemory( pS + i * lenOfSalt, pSaltString, lenOfSalt);
        }
        
        if( lenOfpS % lenOfSalt != 0)
        {
            AnscCopyMemory( pS + i * lenOfSalt, pSaltString, lenOfpS % lenOfSalt);  
        }
    }

    /* Concatenate S and P as :  I = S || P */   
    pIII = 
        pThisObject->ConcatenateString
            ( 
                pThisObject,
                pS, 
                lenOfpS, 
                pPass, 
                lenOfPass, 
                &lenOfIII
            );

    AnscFreeMemory(pPass);

    if( pS != NULL)
    {
        AnscFreeMemory(pS);
    }


    if( pIII == NULL)
    {
        AnscFreeMemory(pDiver);

        return ANSC_STATUS_FAILURE;
    }

    /* Allocate memory for output keys */
    pAAA = (PUCHAR)AnscAllocateMemory(cycle * u);

    if( pAAA == NULL)
    {
        AnscFreeMemory(pIII);
        AnscFreeMemory(pDiver);

        return ANSC_STATUS_FAILURE;
    }

    for( i = 0; i < cycle ; i ++)
    {       
        /* Hash (D || I) r times */
        status = 
            pThisObject->ConcatenateAndHash
                 ( 
                    pThisObject,
                    pDiver, 
                    v, 
                    pIII, 
                    lenOfIII, 
                    iteration, 
                    hashType,
                    &hashResult
                  );

        if(status != ANSC_STATUS_SUCCESS)
        {
            AnscFreeMemory(pDiver);
            AnscFreeMemory(pIII);
            AnscFreeMemory(pAAA);

            return status;
        }

        /* Create B of length v bits with copies of Hash */
        if( i < cycle -1 )
        {
            pBBB = (PUCHAR)AnscAllocateMemory(v);

            for( j = 0; j < (ULONG)(v/hashResult.Length) ; j ++ )
            {
                AnscCopyMemory( pBBB + j * hashResult.Length, hashResult.Value, hashResult.Length);
            }

            if( v % hashResult.Length != 0)
            {
                AnscCopyMemory( pBBB + (v/hashResult.Length)* hashResult.Length, hashResult.Value, v % hashResult.Length);
            }

            /* Treat every I0,I1,...I(k-1) as  I = (I + B + 1) % 2^v; */
            for ( j = 0; j < (int)lenOfIII / v ; j ++ )
            {
                UpdateStringIII( pIII + v * j, pBBB, v );
            }
            
            AnscFreeMemory(pBBB);
        }

        /* Copy Ai to pAAA */
        AnscCopyMemory( pAAA + i * u, hashResult.Value, hashResult.Length );
    }

    AnscFreeMemory(pIII);
    AnscFreeMemory(pDiver);

    if(pLenOfKey)
    {
        *pLenOfKey  = n;
    }

    if(pKey)
    {
        AnscCopyMemory( pKey, pAAA, n );
    }

    AnscFreeMemory(pAAA);

    return ANSC_STATUS_SUCCESS;
}

/*********************************************************************
    In EncryptedPrivateKeyInfo, sometime use PKCS5 to  Derive Key

    ANSC_STATUS
    PKCS12UtilityGetPKCS5DerivedKey
        (
            ANSC_HANDLE                 hThisObject,
            PCHAR                       pPasswordString,
            PUCHAR                      pSaltString,
            ULONG                       lenOfSalt,
            ULONG                       iteration,
            HASH_TYPE                   hashType,   
            PUCHAR                      pKeyAndIV,
            PULONG                      pLenOfKeyAndIV
        )

    Description:

        Get derived Key with given parameter in PKCS5 Algorithms.
     
    Parameter:

        ANSC_HANDLE                 hThisObject,
        The interface handle;

        PCHAR          pPasswordString,
        The given password;             (Input)

        PUCHAR          pSaltString,
        The given Salt String;          (Input)

        ULONG           lenOfSalt,
        The length of Salt String;      (Input)

        LONG            iteration,
        The iterations;

        PUCHAR          pKeyAndIV,
        The output key buffer;          (Output)

        PULONG          pLenOfKeyAndIV,
        The output key length;          (Output)

        HASH_TYPE       type            = HASH_SHA1
        The Hash method,  Default is HASH_SHA1;
 
 Return:  

    If success, return ANSC_STATUS_SUCCESS;
    Otherwise, return the error code;

*********************************************************************/
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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    ULONG                           lenOfPassword;
    ULONG                           MsgLength;
    PUCHAR                          pCombineData;
    ANSC_CRYPTO_HASH                hashResult;

    if(hashType != HASH_MD5 && hashType != HASH_SHA1)        
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pPasswordString == NULL || pThisObject == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Cancatenation password and saltString :  P || S */

    lenOfPassword   = AnscSizeOfString(pPasswordString);
    MsgLength       = lenOfPassword + lenOfSalt;

    pCombineData    = (PUCHAR)AnscAllocateMemory(MsgLength);

    if(pCombineData == NULL)
    {
        AnscTrace("Cannot Allocate Memory in function : GetPKCS12DerivedKey\n");
        return ANSC_STATUS_FAILURE;
    }

    AnscCopyMemory( pCombineData, pPasswordString, lenOfPassword);
    AnscCopyMemory( pCombineData + lenOfPassword, pSaltString, lenOfSalt);

    /* Get Hash */
    if( hashType == HASH_MD5)
    {
        hashResult.Length = 16;
    }
    else
    {
        hashResult.Length = 20;
    }

    if(pLenOfKeyAndIV)
    {
        *pLenOfKeyAndIV =   hashResult.Length;
    }   

    if(pKeyAndIV)
    {
        /* Get Hash */
        pThisObject->HashMessage
            (
                pThisObject,
                pCombineData,
                MsgLength,
                iteration,
                hashType,
                &hashResult
            );

        AnscCopyMemory( pKeyAndIV, hashResult.Value, hashResult.Length);
    }

    AnscFreeMemory(pCombineData);

    return ANSC_STATUS_SUCCESS;

}

/*************************************************************************
 
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
        )

    Descriptions:
        
          Decrypt the Data in EncryptedData of ContentInfo or EncryptedPrivateKeyInfo,
          Please turn to PKCS12, PKCS8 and PKCS5 to get more information.

    Parameter:

        ANSC_HANDLE                 hThisObject,
        The utility interface handle;

        PKCS12_CRYPTO_ALGOR         cryptType,
        The Encrypt Algorithm, must be ALG_pbeWithXXX


        PCHAR                           pPassword,              (In)
        The Given Password

        PUCHAR                          pSaltString,            (In)
        The Salt Parameter of the Algorithm;

        ULONG                           lenOfSalt,              (In)
        The Length of Salt;

        ULONG                           iteration,              (In)
        The Iteration Parameter of the Algorithm;

        PUCHAR                          pEncryptedData,         (In)
        The Encrypted Data need to be Decrypt;

        ULONG                           lenOfEncryptedData,     (In)
        The length of Encrypted Data;

        PUCHAR                          pDecryptedData,         (Out)
        The output buffer with Decrypted Data;

        PULONG                          pLenOfDecryptedData     (Out)
        The length buffer of Decrypted Data;

  Return:

        If Success, return ANSC_STATUS_SUCCESS;
        Others return the error code;

  *************************************************************************/
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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    ULONG                           lenOfIV     = 8;
    UCHAR                           pIV[8]      = { 0 };
    ULONG                           lenOfKey    = 64;
    UCHAR                           pKey[64]    = { 0 };
    ULONG                           bEffectiveBit;      /* For RC2 Encryption; */
    ANSC_CRYPTO_IV                  EncryptIV;
    ANSC_CRYPTO_KEY                 EncryptKey;
    PANSC_CRYPTO_OBJECT             pCrypto;

    if( cryptType >= ALG_RESERVED || pThisObject == NULL )
    {
        AnscTrace("Unsupported PKCS12 algorithm.\n");

        return ANSC_STATUS_FAILURE;
    }

    /* get the crypto API */
    pCrypto = pThisObject->pCryptAPI;

    /* get the key */
    if( cryptType >= ALG_pbeWithSHAAnd128BitRC4 && cryptType <= ALG_pbeWithSHAAnd40BitRC2_CBC )
    {
        /* Get PKCS12 Derived key */
        pThisObject->GetDerivedKey
            (
                pThisObject,
                1, 
                pPassword, 
                pSaltString, 
                lenOfSalt, 
                iteration,
                HASH_SHA1,
                pKey, 
                &lenOfKey
            ); 


        /* Get IV String */
        if(cryptType == ALG_pbeWithSHAAnd3KeyTripleDES_CBC || cryptType == ALG_pbeWithSHAAnd2KeyTripleDES_CBC)
        {
            pThisObject->GetDerivedKey
                (
                    pThisObject,
                    2, 
                    pPassword, 
                    pSaltString, 
                    lenOfSalt, 
                    iteration, 
                    HASH_SHA1,
                    pIV, 
                    &lenOfIV
                ); 

            EncryptKey.RoundNumber = 16;
            EncryptKey.KeyNumber   = 1;
            EncryptIV.Length       = lenOfIV;

            AnscCopyMemory( EncryptIV.Value, pIV, lenOfIV);

            if( cryptType ==  ALG_pbeWithSHAAnd3KeyTripleDES_CBC)
            {
	            EncryptKey.KeyNumber   = 3;
	            EncryptKey.Length      = 8;
	            AnscCopyMemory((PVOID)EncryptKey.Value[0] , pKey,      8);
                AnscCopyMemory((PVOID)EncryptKey.Value[1] , pKey + 8,  8);
                AnscCopyMemory((PVOID)EncryptKey.Value[2] , pKey + 16, 8);

                status = 
                    pCrypto->TripleDesDecrypt
                        (
                            (PVOID)pEncryptedData,
                            lenOfEncryptedData,
                            (PVOID)pDecryptedData,
                            &EncryptKey,
                            &EncryptIV
                        ); 

                if(pLenOfDecryptedData)
                {
                    *pLenOfDecryptedData = lenOfEncryptedData;
                }

            }
        }
        else if( cryptType == ALG_pbeWithSHAAnd128BitRC2_CBC)
        {
            bEffectiveBit = 128;

            pThisObject->GetDerivedKey
                (
                    pThisObject,
                    2, 
                    pPassword, 
                    pSaltString, 
                    lenOfSalt, 
                    iteration, 
                    HASH_SHA1,
                    pIV, 
                    &lenOfIV
                ); 

            status = 
                pThisObject->RC2CBC
                    (
                        pThisObject,
                         false,
                         pKey,
                         128/8,       
                         bEffectiveBit,
                         pIV,
                         lenOfIV,
                         pEncryptedData,
                         pDecryptedData,
                         lenOfEncryptedData                             
                    );

            if(pLenOfDecryptedData)
            {
                *pLenOfDecryptedData = lenOfEncryptedData;
            }

        }
        else if( cryptType == ALG_pbeWithSHAAnd40BitRC2_CBC)
        {
            bEffectiveBit = 40;

            pThisObject->GetDerivedKey
                (
                    pThisObject,
                    2, 
                    pPassword, 
                    pSaltString, 
                    lenOfSalt, 
                    iteration, 
                    HASH_SHA1,
                    pIV, 
                    &lenOfIV
                ); 

            status = 
                pThisObject->RC2CBC
                    (
                        pThisObject,
                         false,
                         pKey,
                         40/8,       
                         bEffectiveBit,
                         pIV,
                         lenOfIV,
                         pEncryptedData,
                         pDecryptedData,
                         lenOfEncryptedData                             
                    );

            if(pLenOfDecryptedData)
            {
                *pLenOfDecryptedData = lenOfEncryptedData;
            }
        }
        else /* 128BitRC4, 40BitRC4; */
        {
            status = ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        /* Get PKCS5DerivedKey */
        if( cryptType == ALG_pbeWithSHA1AndRC2_CBC || cryptType == ALG_pbeWithSHA1AndDES_CBC)
        {
            /* HASH_SHA1 */
            pThisObject->GetPKCS5Key
                (
                    pThisObject,
                    pPassword, 
                    pSaltString, 
                    lenOfSalt, 
                    iteration, 
                    HASH_SHA1,
                    pKey, 
                    &lenOfKey
                );
        }
        else if(cryptType == ALG_pbeWithMD5AndRC2_CBC || cryptType == ALG_pbeWithMD5AndDES_CBC)
        {
            /* HASH_MD5 */
            pThisObject->GetPKCS5Key
                (
                    pThisObject,
                    pPassword, 
                    pSaltString, 
                    lenOfSalt, 
                    iteration, 
                    HASH_MD5,
                    pKey, 
                    &lenOfKey                    
                );
        }

        if( cryptType == ALG_pbeWithSHA1AndDES_CBC || cryptType == ALG_pbeWithMD5AndDES_CBC)
        {
            EncryptKey.RoundNumber = 16;
            EncryptIV.Length       = 8;

            AnscCopyMemory( EncryptIV.Value, pKey + 8, 8);

	        EncryptKey.KeyNumber   = 1;
	        EncryptKey.Length      = 8;
	        AnscCopyMemory((PVOID)EncryptKey.Value[0] , pKey,      8);

            status = 
                pCrypto->DesDecrypt
			        (
                        (PVOID)pEncryptedData,
                        lenOfEncryptedData,
                        (PVOID)pDecryptedData,
				        &EncryptKey,
				        &EncryptIV
			        );

            if(pLenOfDecryptedData)
            {
                *pLenOfDecryptedData = lenOfEncryptedData;
            }
        }
        else if(cryptType == ALG_pbeWithSHA1AndRC2_CBC || cryptType == ALG_pbeWithMD5AndRC2_CBC)
        {
            AnscCopyMemory(pIV, pKey + 8, 8);

            status = 
                pThisObject->RC2CBC
                    (
                        pThisObject,
                         false,
                         pKey,
                         8,       
                         0,
                         pIV,
                         8,
                         pEncryptedData,
                         pDecryptedData,
                         lenOfEncryptedData                             
                    );

            if(pLenOfDecryptedData)
            {
                *pLenOfDecryptedData = lenOfEncryptedData;
            }
        }
        else /* Others not support yet; */
        {
            return ANSC_STATUS_FAILURE;
        }
    }

   return status;
}


/*************************************************************************
 
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
        )


    Descriptions:
        
          Encrypt the Data in EncryptedData of ContentInfo or EncryptedPrivateKeyInfo,
          Please turn to PKCS12, PKCS8 and PKCS5 to get more information.

    Parameter:

        ANSC_HANDLE                 hThisObject,
        The handle of pkcs12 utility interface;
    
        PKCS12_CRYPTO_ALGOR              type,                  (In)
        The Encrypt Algorithm, must be ALG_pbeWithXXX


        PCHAR                           pPassword,              (In)
        The Given Password

        PUCHAR                          pSaltString,            (In)
        The Salt Parameter of the Algorithm;

        ULONG                           lenOfSalt,              (In)
        The Length of Salt;

        ULONG                           iteration,              (In)
        The Iteration Parameter of the Algorithm;

        PUCHAR                          pClearData,             (In)
        The Data need to be Encrypted;

        ULONG                           lenOfClearData,         (In)
        The length of clear Data;

        PUCHAR                          pCipherData,            (Out)
        The output buffer with Encrypted Data;

        PULONG                          pLenOfCipherData        (Out)
        The length buffer of Encrypted Data;

  Return:

        If Success, return ANSC_STATUS_SUCCESS;
        Others return the error code;

  *************************************************************************/
ANSC_STATUS
PKCS12UtilityEncrypt
    (
        ANSC_HANDLE                 hThisObject,
        PKCS12_CRYPTO_ALGOR         type,
        PCHAR                       pPassword,
        PUCHAR                      pSaltString,
        ULONG                       lenOfSalt,
        ULONG                       iteration,
        PUCHAR                      pClearData,
        ULONG                       lenOfClearData,
        PUCHAR                      pCipherData,
        PULONG                      pLenOfCipherData
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    ULONG                           lenOfIV     = 8;
    UCHAR                           pIV[8]      = { 0 };
    ULONG                           lenOfKey    = 64;
    UCHAR                           pKey[64]    = { 0 };
    ULONG                           bEffectiveBit;       /* for RC2 Encryption */

    if( pThisObject == NULL || pPassword == NULL || pClearData == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( type != ALG_pbeWithSHAAnd40BitRC2_CBC)
    {
        AnscTrace("Only 'ALG_pbeWithSHAAnd40BitRC2_CBC' was supported right now, '%d' is not supported yet.\n", type);

        return ANSC_STATUS_FAILURE;
    }

    /* Get PKCS12 Derived key */
    pThisObject->GetDerivedKey
        (
            pThisObject,
            1, 
            pPassword, 
            pSaltString, 
            lenOfSalt, 
            iteration, 
            HASH_SHA1,
            pKey, 
            &lenOfKey
        ); 

    if( type == ALG_pbeWithSHAAnd40BitRC2_CBC)
    {
        bEffectiveBit = 40;

        pThisObject->GetDerivedKey
            (
                pThisObject,
                2, pPassword, 
                pSaltString, 
                lenOfSalt, 
                iteration, 
                HASH_SHA1,
                pIV, 
                &lenOfIV
            ); 
        
        status = 
            pThisObject->RC2CBC
                (
                    pThisObject,
                    true,
                    pKey,
                    40/8,   
                    bEffectiveBit,
                    pIV,
                    lenOfIV,
                    pClearData,
                    pCipherData,
                    lenOfClearData
                 );

        if(pLenOfCipherData)
        {
            *pLenOfCipherData = lenOfClearData;
        }
    }

    return status;
}

/**************************************************************************

    PKCS12UtilityMacAuthenticate
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
        )

    Descriptions:
    
        MAC Authenticate in PKCS12 ;  

    Parameter:

        ANSC_HANDLE                 hThisObject,
        The handle of the pkcs12 utility;

        HASH_TYPE                       type,               (In)
        Hash Type;

        PCHAR                           pPassword,          (In)
        The Given Password;

        PUCHAR                          pSaltString,        (In)
        The Salt String of MAC;

        ULONG                           lenOfSalt,          (In)
        The length of Salt string;

        ULONG                           iteration,          (In)
        The iteration parameter of MAC

        PUCHAR                          pMessage,           (In)
        The message data will be signed;

        ULONG                           lenOfMessage,       (In)
        The length of message

        ANSC_HANDLE                 pHash                   (Out)
        The hash result handle;
  
  Return:

        If succeed, return ANSC_STATUS_SUCCESS;
        Others return the error code;

 **************************************************************************/
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
    )
{
     PPKCS12_UTILITY_INTERFACE      pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
     ANSC_STATUS                    status      = ANSC_STATUS_SUCCESS;
     PANSC_CRYPTO_HASH              pHashResult = (PANSC_CRYPTO_HASH)pHash;
     ULONG                          lenOfKey    = 64;
     UCHAR                          pKey[64]    = { 0 };
     ULONG                          result      = 0;
     PANSC_CRYPTO_OBJECT            pCrypto;
     ANSC_CRYPTO_KEY                key;

     if( hThisObject == NULL)
     {
         return ANSC_STATUS_FAILURE;
     }

    /* get the crypto API */
    pCrypto = pThisObject->pCryptAPI;

     /* Get PKCS12 Derived HMAC key */
     pThisObject->GetDerivedKey
         (
            pThisObject,
            3, 
            pPassword, 
            pSaltString, 
            lenOfSalt, 
            iteration, 
            type,
            pKey, 
            &lenOfKey 
         ); 

     key.KeyNumber      = 1;
     key.Length         = lenOfKey;
     AnscCopyMemory(key.Value, pKey, lenOfKey);

     if( type == HASH_MD5)
     {
          pHashResult->Length = ANSC_MD5_OUTPUT_SIZE;

          result = 
              pCrypto->HmacMd5Digest
                  (
                     pMessage,
                     lenOfMessage,
                     pHash,
                     &key
                  );  
     }
     else
     {
         pHashResult->Length = ANSC_SHA1_OUTPUT_SIZE;

          result = 
              pCrypto->HmacSha1Digest
                  (
                     pMessage,
                     lenOfMessage,
                     pHash,
                     &key
                  );  
     }

      if(result != 0)
      {
          return status;
      }

      return  ANSC_STATUS_FAILURE;
}

/*********************************************************
 *
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
        )

     Discription:

        RC2 Encryption and Decryption Algorithm;

     Parameter:

        ANSC_HANDLE                 hThisObject,
        The handle of pkcs12 utility interface.

        BOOL            isEncrypt,
        TRUE as encrypt, FALSE as Decrypt;

        PUCHAR          pKey,
        The RC2 Key buffer;

        ULONG           lenOfKey,
        The RC2 Key length;

        ULONG           bits,
        The Effective Key length of RC2 (In bits)

        PUCAHR          pIVString,
        IV string 
        ULONG           lenOfIV,

        PUCHAR          pInputString,
        The input string buffer;

        PUCHAR          pOutputString,
        The output string buffer;

        ULONG           lenOfString
        The length of string  (Supposed the input and output have the same length)

      Return:

        If success, return ANSC_STATUS_SUCCESS;
        else return the error code;            

 *
 *********************************************************/

#ifndef RC2_BLOCK_SIZE
#define RC2_BLOCK_SIZE              8
#endif

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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    PANSC_CRYPTO_OBJECT             pCrypto     = (PANSC_CRYPTO_OBJECT      )pThisObject->pCryptAPI;
    USHORT                          xkey[64];
    int                             i, j, total;
    UCHAR    previousCipherBlock[RC2_BLOCK_SIZE]= { 0 };

    if(    pKey             == NULL  ||  pInputString     == NULL           || 
           pOutputString    == NULL  ||  lenOfIV != RC2_BLOCK_SIZE          ||
           pCrypto          == NULL  ||  lenOfString % RC2_BLOCK_SIZE != 0 

      )
    {
        return ANSC_STATUS_FAILURE;
    }   

    AnscCopyMemory(previousCipherBlock, pIVString , lenOfIV);

    pCrypto->Crypt_rc2_keyschedule
        (
                xkey,
                pKey,
                lenOfKey,
                bits
        );

    total = lenOfString /RC2_BLOCK_SIZE ;

    for( i = 0; i < total; i ++)
    {
        if(isEncrypt)
        {   
            for( j = 0; j < RC2_BLOCK_SIZE; j ++)
            {
                previousCipherBlock[j] ^= pInputString[RC2_BLOCK_SIZE * i + j];
            }

            pCrypto->Crypt_rc2_encrypt
                (
                    xkey,
                    (PUCHAR)(previousCipherBlock),
                    (PUCHAR)(pOutputString + RC2_BLOCK_SIZE * i)
                );

            for( j = 0; j < RC2_BLOCK_SIZE; j ++)
            {
                previousCipherBlock[j] = pOutputString[RC2_BLOCK_SIZE * i + j];
            }
        }
        else
        {
            pCrypto->Crypt_rc2_decrypt
                (
                    xkey,
                    (PUCHAR)(pOutputString + RC2_BLOCK_SIZE * i),
                    (PUCHAR)(pInputString + RC2_BLOCK_SIZE * i)
                );

            for( j = 0; j < RC2_BLOCK_SIZE; j ++)
            {
                pOutputString[RC2_BLOCK_SIZE * i + j] ^= previousCipherBlock[j];
                previousCipherBlock[j]                 = pInputString[RC2_BLOCK_SIZE * i + j];
            }
        }
    }

    return ANSC_STATUS_SUCCESS;

}

/*********************************************************
 *
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
        )

     Discription:

        RC2 Encryption and Decryption Algorithm;

     Parameter:

        ANSC_HANDLE                 hThisObject,
        The handle of pkcs12 utility interface.

        BOOLEAN          isEncrypt,
        TRUE as encrypt, False as Decrypt;

        PUCHAR          pKey,
        The RC2 Key buffer;

        ULONG           lenOfKey,
        The RC2 Key length;

        ULONG           bits,
        The Effective Key length of RC2 (In bits)


        PUCHAR          pInputString,
        The input string buffer;

        PUCHAR          pOutputString,
        The output string buffer;

        ULONG           lenOfString
        The length of string  (Supposed the input and output have the same length)

      Return:

        If success, return ANSC_STATUS_SUCCESS;
        else return the error code;            

 *
 *********************************************************/
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
    )
{
    PPKCS12_UTILITY_INTERFACE       pThisObject = (PPKCS12_UTILITY_INTERFACE)hThisObject;    
    PANSC_CRYPTO_OBJECT             pCrypto     = (PANSC_CRYPTO_OBJECT      )pThisObject->pCryptAPI;
    USHORT                          xkey[64];
    int                             i,total;

    if(!pKey || lenOfKey <= 0  
             || !pInputString  || lenOfString % RC2_BLOCK_SIZE != 0
             || !pOutputString || pCrypto == NULL )
    {
        return ANSC_STATUS_FAILURE;
    }   

    pCrypto->Crypt_rc2_keyschedule
        (
                xkey,
                pKey,
                lenOfKey,
                bits
        );

    total = lenOfString /RC2_BLOCK_SIZE ;

    for( i = 0; i < total; i ++)
    {
        if(isEncrypt)
        {   
            pCrypto->Crypt_rc2_encrypt
                (
                    xkey,
                    (PUCHAR)(pInputString + RC2_BLOCK_SIZE * i),
                    (PUCHAR)(pOutputString + RC2_BLOCK_SIZE * i)
                );
        }
        else
        {
            pCrypto->Crypt_rc2_decrypt
                (
                    xkey,
                    (PUCHAR)(pOutputString + RC2_BLOCK_SIZE * i),
                    (PUCHAR)(pInputString + RC2_BLOCK_SIZE * i)
                );
        }
    }

    return ANSC_STATUS_SUCCESS;

}




