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

    module:	ansc_crypto_utility.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the utilities for cryptography
        functions for the Crypto Object.

        *   AnscCryptoIsDesWeakKey  
        *   AnscCryptoFixParity     
        *   AnscCryptoCheckParity   
        *   AnscCryptoCorrectDesKey 
        *   AnscCryptoReverseByte
        *   AnscCryptoXorByte
        *   AnscCryptoMitDesStrToKey
        *   AnscCryptoNFold
        *   AnscCryptoRandomToKey
        *   AnscCryptoMapEncTypeToKeySize
        *   AnscCryptoDesRandomToKey
        *   AnscCryptoDes3RandomToKey
        *   AnscCryptoDeriveKey
        *   AnscCryptoDs3StringToKey
        *   AnscCryptoGenerateUuid
        *   AnscCryptoGenerateUuid2


    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        08/09/02    initial revision.
        03/09/05    zhubin added uuid generation apis

**********************************************************************/

#include "ansc_crypto_global.h"
#include "safec_lib_common.h"

#define AL_UUID_STRING_FORMAT                        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x"

/*
 * The following are the weak DES keys:
 */
#define     ANSC_CRYPTO_DES_WEAK_KEY_NUM             16

static const UCHAR Ansc_DES_Weak_Key[ANSC_CRYPTO_DES_WEAK_KEY_NUM][ANSC_DES_BLOCK_SIZE] = 
{
    /* weak keys */
    {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
    {0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe},
    {0x1f,0x1f,0x1f,0x1f,0x0e,0x0e,0x0e,0x0e},
    {0xe0,0xe0,0xe0,0xe0,0xf1,0xf1,0xf1,0xf1},

    /* semi-weak */
    {0x01,0xfe,0x01,0xfe,0x01,0xfe,0x01,0xfe},
    {0xfe,0x01,0xfe,0x01,0xfe,0x01,0xfe,0x01},

    {0x1f,0xe0,0x1f,0xe0,0x0e,0xf1,0x0e,0xf1},
    {0xe0,0x1f,0xe0,0x1f,0xf1,0x0e,0xf1,0x0e},

    {0x01,0xe0,0x01,0xe0,0x01,0xf1,0x01,0xf1},
    {0xe0,0x01,0xe0,0x01,0xf1,0x01,0xf1,0x01},

    {0x1f,0xfe,0x1f,0xfe,0x0e,0xfe,0x0e,0xfe},
    {0xfe,0x1f,0xfe,0x1f,0xfe,0x0e,0xfe,0x0e},

    {0x01,0x1f,0x01,0x1f,0x01,0x0e,0x01,0x0e},
    {0x1f,0x01,0x1f,0x01,0x0e,0x01,0x0e,0x01},

    {0xe0,0xfe,0xe0,0xfe,0xf1,0xfe,0xf1,0xfe},
    {0xfe,0xe0,0xfe,0xe0,0xfe,0xf1,0xfe,0xf1}
};

#define     ANSC_CRYPTO_DES_KEY_BYTES_NO_PARITY              7
#define     ANSC_CRYPTO_DES_KEY_BYTES_WITH_PARITY            ANSC_DES_BLOCK_SIZE

#define     ANSC_CRYPTO_DES3_KEY_BYTES_NO_PARITY             21


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscCryptoIsDesWeakKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hKey
            );

    description:

        This function is called to check if the given key is a DES key,
        and if so, if it's in the weak key list, Return TRUE; othewise,
        reutrn FALSE.

        The key should have correct Odd parity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hKey
                Specifies the key to check.


    return:     TRUE or FALSE.

**********************************************************************/

BOOL
AnscCryptoIsDesWeakKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_CRYPTO_KEY                pKey         = (PANSC_CRYPTO_KEY   )hKey;
    ULONG                           i            = 0;
    PUCHAR                          pWeakKey     = NULL;
    BOOL                            bResult      = FALSE;
    
    if ( (pKey->Length != ANSC_DES_BLOCK_SIZE) ||
         (pKey->KeyNumber != 1               )     )
    {
        /*
         * not a DES key
         */
        return FALSE;
    }

    for ( i = 0; i < ANSC_CRYPTO_DES_WEAK_KEY_NUM; i++ ) 
    {
        pWeakKey = (PUCHAR)Ansc_DES_Weak_Key[i];

        bResult = 
            AnscEqualMemory
                (
                    (PVOID)pWeakKey,
                    (PVOID)pKey->Value[0],
                    ANSC_DES_BLOCK_SIZE
                );

        if( bResult)
        {

            return TRUE;
        }
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoFixParity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hKey,
                BOOL                        bOddParity
            );

    description:

        This function is called to set the righ perity to the key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hKey
                Specifies the handle of the key to correct.

                BOOL                        bOddParity
                Specifies if the parity is Odd.


    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoFixParity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_KEY                pKey         = (PANSC_CRYPTO_KEY)hKey;
    PUCHAR                          pKeyValue    = NULL;
    ULONG                           ulKeyLength  = pKey->Length;
    ULONG                           i            = 0;
    ULONG                           j            = 0;
    UCHAR                           ucLastBit    = 0;

    for ( j = 0; j < pKey->KeyNumber; j ++ )
    {
        pKeyValue = pKey->Value[j];

        for ( i = 0; i < ulKeyLength; i++ ) 
        {
            /*
             * set the lowest bit 0
             */
            pKeyValue[i] &= 0xfe;
            pKeyValue[i] |= 1^AnscParityChar(pKeyValue[i]);
        }

        if ( bOddParity != TRUE )
        {
            for ( i = 0; i < ulKeyLength; i++ ) 
            {
                /*
                 * set the lowest bit to the oppisite value
                 */
                ucLastBit = pKeyValue[i] & 0x1;

                pKeyValue[i] &= 0xfe;

                if ( ucLastBit == 0 )
                {
                    pKeyValue[i] |= 0x1;
                }
            }
        }
    }
 

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscCryptoCheckParity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hKey,
                BOOL                        bOddParity
            );

    description:

        This function is called to check if the key has righ perity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hKey
                Specifies the handle of the key to correct.

                BOOL                        bOddParity
                Specifies if the parity is Odd.


    return:     TRUE if it has right perity.

**********************************************************************/

BOOL
AnscCryptoCheckParity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey,
        BOOL                        bOddParity
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_CRYPTO_KEY                pKey         = (PANSC_CRYPTO_KEY)hKey;
    PUCHAR                          pKeyValue    = NULL;
    ULONG                           ulKeyLength  = pKey->Length;
    ULONG                           i            = 0;
    ULONG                           j            = 0;
    UCHAR                           ucLastBit    = 0;
    BOOL                            bResult      = TRUE;

    for ( j = 0; j < pKey->KeyNumber; j ++ )
    {
        if ( bOddParity )
        {
            for ( i = 0; i < ulKeyLength; i++ ) 
            {
              /*TODO CID: 59007 Explicit null dereferenced 
           * pKeyValue always NULL, need to update*/
                ucLastBit = pKeyValue[i] & 0x1;

                if ( ucLastBit != AnscParityChar((pKeyValue[i] & 0xfe)) )
                {
                    if(pKeyValue)
                    {
                            ucLastBit = pKeyValue[i] & 0x1;
        
                        if ( ucLastBit != AnscParityChar((pKeyValue[i] & 0xfe)) )
                        {
                            bResult = FALSE;
                            break;
                        }
                    }
                }
            }
        }
        else
        {

            for ( i = 0; i < ulKeyLength; i++ ) 
            {
              /*CID: 59007 Explicit null dereferenced
           * pKeyValue always NULL, need to update*/
                ucLastBit = pKeyValue[i] & 0x1;

                if ( ucLastBit == AnscParityChar((pKeyValue[i] & 0xfe)) )
                {
                    if(pKeyValue)
                    {
                        ucLastBit = pKeyValue[i] & 0x1;
        
                            if ( ucLastBit == AnscParityChar((pKeyValue[i] & 0xfe)) )
                            {
                                bResult = FALSE;
                                break;
                            }
                    }
                }
            }
        }
    }
    return  bResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoCorrectDesKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hKey
            );

    description:

        This function is called to make sure that the DES key has
        right parity and not a weak key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hKey
                Specifies the handle of the key to correct.


    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoCorrectDesKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject    = (PANSC_CRYPTO_OBJECT)hThisObject;
    PANSC_CRYPTO_KEY                pKey         = (PANSC_CRYPTO_KEY)hKey;
    PUCHAR                          pKeyValue    = pKey->Value[0];
    BOOL                            bWeakKey     = FALSE;

    returnStatus =
        pMyObject->FixParity
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pKey,
                TRUE
            );

    bWeakKey =
        pMyObject->IsDesWeakKey
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pKey
            );

    if ( bWeakKey )
    {
        pKeyValue[ANSC_DES_BLOCK_SIZE-1] ^= 0xf0;
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        UCHAR
        AnscCryptoReverseByte
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       ucValue
            );

    description:

        This function is called to invert the bit order of the byte.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       ucValue
                Specifies the value to reverse.


    return:     reversed byte.

**********************************************************************/

UCHAR
AnscCryptoReverseByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           i            = 0;
    UCHAR                           ucNewValue   = 0;
    UCHAR                           ucMask       = 0;
    UCHAR                           ucBitSize    = 8;

    for ( i = 0; i < ucBitSize; i ++ )
    {
        ucMask = 1 << i;
        ucNewValue |= ((ucValue & ucMask) >> i) << (ucBitSize - 1 - i);
    }

    return  ucNewValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        UCHAR
        AnscCryptoXorByte
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       ucValue1,
                UCHAR                       ucValue2
            );

    description:

        This function is called to XOR the bits of the input bytes.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       ucValue1
                Specifies the value to reverse.

                UCHAR                       ucValue2
                Specifies the value to reverse.

    return:     xored byte.

**********************************************************************/

UCHAR
AnscCryptoXorByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       ucValue1,
        UCHAR                       ucValue2
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           i            = 0;
    UCHAR                           ucNewValue   = 0;
    UCHAR                           ucMask       = 0;
    UCHAR                           ucBitSize    = 8;
    UINT                            uiBit1        = 0;
    UINT                            uiBit2        = 0;
    UINT                            uiBit3        = 0;


    for ( i = 0; i < ucBitSize; i ++ )
    {
        ucMask = 1 << i;

        uiBit1  = ((ucValue1 & ucMask) >> i);
        uiBit2  = ((ucValue2 & ucMask) >> i);
        uiBit3  = uiBit1^uiBit2;

        ucNewValue |= ( uiBit3 ) << i;
    }

    return  ucNewValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to convert the input string and salt to
        desired DES key format. The caller allocates the key memory.
        The function only fills its content.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hString
                Specifies the string to convert.

                ULONG                       ulStringLeng
                Specifies the length of the string.

                ANSC_HANDLE                 hSalt
                Specifies the handle of the the salt, which may be NULL.

                ULONG                       ulSaltLeng
                Specifies the length of the the salt, which may be zero.

                ANSC_HANDLE                 hNewKey
                Specifies the handle of the generated key.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoMitDesStrToKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString,
        ULONG                       ulStringLeng,
        ANSC_HANDLE                 hSalt,
        ULONG                       ulSaltLeng,
        ANSC_HANDLE                 hNewKey
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject       = (PANSC_CRYPTO_OBJECT)hThisObject;
    PUCHAR                          pSalt           = (PUCHAR)hSalt;
    PUCHAR                          pString         = (PUCHAR)hString;
    PANSC_CRYPTO_KEY                pNewKey         = (PANSC_CRYPTO_KEY)hNewKey;
    PUCHAR                          pNewString      = NULL;
    PUCHAR                          pCurrStrHead    = NULL;
    ULONG                           ulNewStringLeng = 0;
    LONG                            i               = 0;
    LONG                            j               = 0;
    BOOL                            bOdd            = TRUE;
    UCHAR                           ucByte          = 0;
    PUCHAR                          pCurrTemp       = NULL;
    PUCHAR                          pCurrTempKey    = NULL;
    ANSC_CRYPTO_HASH                HashResult;
    UCHAR                           temp[64];
    UCHAR                           tempKey[64];
    ANSC_CRYPTO_IV                  iv;

    /*
    ** RDKB-6148, CID-24328, must pass a valid handle, else will lead to crash.
    */
    if(!pNewKey)
    {
        return ANSC_STATUS_FAILURE;
    }

    AnscZeroMemory(temp,    64);
    AnscZeroMemory(tempKey, 64);

    HashResult.Length       = ANSC_DES_BLOCK_SIZE;
    pNewKey->KeyNumber      = 1;
    pNewKey->RoundNumber    = 1;
    pNewKey->Length         = ANSC_DES_BLOCK_SIZE;

    bOdd                    = TRUE;
    ulNewStringLeng         = ulStringLeng;


    if ( pSalt && ulSaltLeng )
    {
        ulNewStringLeng += ulSaltLeng;
    }

    /*
     * make sure the new string length conforms to 8 bytes boundary
     */
    if ( ulNewStringLeng % ANSC_DES_BLOCK_SIZE )
    {
        ulNewStringLeng = (ulNewStringLeng / ANSC_DES_BLOCK_SIZE + 1) * ANSC_DES_BLOCK_SIZE;
    }

    pNewString = AnscAllocateMemory(ulNewStringLeng);

    if ( !pNewString )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto EXIT1;
    }

    pCurrStrHead = pNewString;
    
    AnscCopyMemory
        (
            (PVOID)pNewString,
            (PVOID)pString,
            ulStringLeng
        );

    if ( pSalt && ulSaltLeng )
    {
        AnscCopyMemory
            (
                (PVOID)(pNewString + ulStringLeng),
                (PVOID)pSalt,
                ulSaltLeng
            );
    }

    pCurrTemp = temp;

    /* get next 8 bytes, pCurrStrHead parity, xor */
    for (i = 1; i <= (LONG)ulNewStringLeng; i++) 
    {
	    /* get next input key byte */
	    ucByte = (UCHAR) *pCurrStrHead++;

	    /* loop through bits within byte, ignore parity */
	    for (j = 0; j <= 6; j++) 
        {
	        if (bOdd)
            {
		        *pCurrTemp++ ^= (int) ucByte & 01;
            }
	        else
            {
		        *--pCurrTemp ^= (int) ucByte & 01;
            }

	        ucByte = ucByte >> 1;
	    }

	    /* check and flip direction */
	    if ((i%8) == 0)
	        bOdd = !bOdd;
    }

    /* now stuff into the key mit_des_cblock, and force odd parity */
    pCurrTemp        = temp;
    pCurrTempKey     = (UCHAR *) tempKey;

    for (i = 0; i <= 7; i++) 
    {
	    ucByte = 0;
	    for (j = 0; j <= 6; j++)
        {
	        ucByte |= *pCurrTemp++ << (1+j);
        }

	    *pCurrTempKey++ = (UCHAR) ucByte;
    }

    AnscCopyMemory(pNewKey->Value[0], tempKey, ANSC_DES_BLOCK_SIZE);

    returnStatus =
        pMyObject->CorrectDesKey
            (
                pMyObject,
                pNewKey
            );

    /* create the IV */
    iv.Length = ANSC_DES_BLOCK_SIZE;
    AnscCopyMemory
        ( 
            iv.Value, 
            pNewKey->Value[0],
            iv.Length
        );

    /*
     * get the DES-CBC alternative checksum
     */
    HashResult.Length = ANSC_DES_BLOCK_SIZE;

    pMyObject->DesMacKDigest
        (
            pNewString,
            ulNewStringLeng,
            &HashResult,
            pNewKey,
            &iv
        );

    AnscCopyMemory(pNewKey->Value[0], HashResult.Value, ANSC_DES_BLOCK_SIZE);

      
    returnStatus =
        pMyObject->CorrectDesKey
            (
                pMyObject,
                pNewKey
            );

EXIT1:

    if ( pNewString )
    {
        AnscFreeMemory(pNewString);
    }

    if ( (returnStatus != ANSC_STATUS_SUCCESS) &&
         pNewKey                                  )
    {
        AnscZeroMemory(pNewKey, sizeof(ANSC_CRYPTO_KEY));
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoNFold
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pInString,
                ULONG                       ulInBitNum,
                ULONG                       ulOutBitNum,
                PUCHAR                      pOutString
            );

    description:

        This function is called to N_Fold a string. N_Fold is an algorithm
        which takes m input bits and ``stretches'' them to form n output
        bits with equal contribution from each input bit to the output.

        Blumenthal, U., "A Better Key Schedule for DES-Like Ciphers",
        Proceedings of PRAGOCRYPT '96, 1996.

        Restriction: msb first, assume ulInBitNum and ulOutBitNum are 
        multiples of 8, and that ulInBitNum >= 16.  this is the case 
        of all the cryptosystems which are likely to be used.  
        this function can be replaced if that assumption ever fails.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pInString
                Specifies the input string to stretch.

                ULONG                       ulInBitNum
                Specifies the number of bits in the input string.

                ULONG                       ulOutBitNum
                Specifies the number of bits in the output string.

                PUCHAR                      pOutString
                Specifies the output string, which is allocated by the caller
                and is long enough to hold all output bits.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoNFold
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInBitNum,
        ULONG                       ulOutBitNum,
        PUCHAR                      pOutString
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    ULONG                           ulValue1        = 0;
    ULONG                           ulValue2        = 0;
    ULONG                           ulValue3        = 0;
    ULONG                           ulLeastComMulti = 0;
    ULONG                           ulInByteNum     = 0;
    ULONG                           ulOutByteNum    = 0;
    LONG                            lByte           = 0;
    LONG                            i               = 0;
    LONG                            MsBit           = 0;
    ULONG                           ulBitsInByte    = 8;


    /* 
     * get the lByte number of the input and output strings
     */
    if ( ulInBitNum % ulBitsInByte )
    {
        ulInByteNum = (ulInBitNum / ulBitsInByte + 1) * ulBitsInByte;
    }
    else
    {
        ulInByteNum = ulInBitNum / ulBitsInByte;
    }

    if ( ulOutBitNum % ulBitsInByte )
    {
        ulOutByteNum = (ulOutBitNum / ulBitsInByte + 1) * ulBitsInByte;
    }
    else
    {
        ulOutByteNum = ulOutBitNum / ulBitsInByte;
    }


    /* 
     * first compute Least Common Multiple of (ulInByteNum,ulOutByteNum) 
     */
    ulValue1 = ulOutByteNum;
    ulValue2 = ulInByteNum;

    while ( ulValue2 != 0 ) 
    {
	    ulValue3 = ulValue2;
	    ulValue2 = ulValue1 % ulValue2;
	    ulValue1 = ulValue3;
    }

    ulLeastComMulti = ulOutByteNum * ulInByteNum / ulValue1;

    memset(pOutString, 0, ulOutByteNum);
    lByte = 0;

    /* 
     * this will end up cycling through LeastComMulti(ulInByteNum,ulOutByteNum)/ulInByteNum 
     * times
     */
    for ( i = ulLeastComMulti-1; i >= 0; i-- ) 
    {
	    /* 
         * compute the MsBit in instring which gets added into this lByte 
         * first, start with the MsBit pInString the first, unrotated lByte 
         * then, for each lByte, shift to the right for each repetition 
         * last, pick pOutString the correct Byte within that shifted repetition 
         */
	    MsBit = 
            (
		         (( ulInBitNum)-1) +
		         (((ulInBitNum)+13) * (i/ulInByteNum)) +
		         ((ulInByteNum      - (i%ulInByteNum))<<3)
		     ) % ( ulInBitNum );

	    /* 
         * pull out the Byte value itself 
         */
	    lByte += 
            (((pInString[((ulInByteNum-1)-(MsBit>>3)) % ulInByteNum] << 8 )|
		      (pInString[((ulInByteNum  )-(MsBit>>3)) % ulInByteNum]))
		     >>((MsBit&7)+1)) & 0xff;

	    /* 
         * do the addition 
         */
	    lByte += pOutString[i%ulOutByteNum];
	    pOutString[i%ulOutByteNum] = lByte&0xff;

   
        /*
	    AnscTrace
            (
                "MsBit[%d] = %d\tbyte = %02x\tsum = %03x\n", i, MsBit,
	               (((pInString[((ulInByteNum-1)-(MsBit>>3))%ulInByteNum]<<8)|
		           (pInString[((ulInByteNum)-(MsBit>>3))%ulInByteNum]))
		        >>((MsBit&7)+1))&0xff, lByte
            );
        */

	    /* 
         * keep around the carry bit, if any 
         */
	    lByte >>= 8;

        /*
	    AnscTrace("carry=%d\n", lByte);
        */
    
    }

    /* 
     * if there's ulValue1 carry bit left over, add it back pInString 
     */
    if (lByte) 
    {
	    for ( i = ulOutByteNum-1; i >= 0; i-- ) 
        {
	        /* 
             * do the addition 
             */
	        lByte           += pOutString[i];
	        pOutString[i]   =  lByte & 0xff;

	        /* 
             * keep around the carry bit, if any 
             */
	        lByte >>= 8;
	    }
    }


    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoRandomToKey
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       algorithm,
                PUCHAR                      pInString,
                ULONG                       ulInLength,
                PANSC_CRYPTO_KEY            hOutKey
            );

    description:

        This function is called to generate a key from a random string.
        The random string size and the output key size are determined by the
        input encryption algorithm.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       algorithm
                Specifies the encryption algorithm that the key is generated for.

                PUCHAR                      pInString
                Specifies the input string to generate key.

                ULONG                       ulInLength
                Specifies the number of bytes in the input string.

                PANSC_CRYPTO_KEY            hOutKey
                Specifies the handle of the output key, which is allocated 
                by the caller

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoRandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       algorithm,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;


    switch ( algorithm )
    {
        case    ANSC_CRYPTO_ENCRYPTION_DES_IV64 :
        case    ANSC_CRYPTO_ENCRYPTION_DES :
        case    ANSC_CRYPTO_ENCRYPTION_DES_IV32 :


                returnStatus =
                    AnscCryptoDesRandomToKey
                        (
                            hThisObject,
                            pInString,
                            ulInLength,
                            hOutKey
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_3DES :

                returnStatus =
                    AnscCryptoDes3RandomToKey
                        (
                            hThisObject,
                            pInString,
                            ulInLength,
                            hOutKey
                        );

                break;

        case    ANSC_CRYPTO_ENCRYPTION_RC5 :
        case    ANSC_CRYPTO_ENCRYPTION_IDEA :
        case    ANSC_CRYPTO_ENCRYPTION_CAST :
        case    ANSC_CRYPTO_ENCRYPTION_BLOWFISH :
        case    ANSC_CRYPTO_ENCRYPTION_3IDEA :
        case    ANSC_CRYPTO_ENCRYPTION_RC4 :
        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to return the Key seed size and key length
        for a given encryption algorithm. the Key seed size is the key
        generation seed length needed for the encryption system.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       algorithm
                Specifies the encryption algorithm that the key is generated for.

                PULONG                      pulKeySeedSize
                Specifies the handle of the KeySeedSize.

                PULONG                      pulKeyLength
                Specifies the handle of the KeyLength.

                PULONG                      pulKeyNumber
                Specifies the handle of the KeyNumber.

                PULONG                      pulKeyRound
                Specifies the handle of the KeyRound.

                PULONG                      pulBlockSize
                Specifies the handle of the BlockSize of the algorithm.

                PULONG                      pIvLength
                Specifies the handle of the Initial Vector Length.

    return:     operation status.

**********************************************************************/

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
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;

    *pulKeySeedSize = 0;
    *pulKeyLength   = 0;
    *pulKeyNumber   = 0;
    *pulKeyRound    = 0;
    *pulBlockSize   = 0;
    *pIvLength      = 0;

    switch ( algorithm )
    {
        case    ANSC_CRYPTO_ENCRYPTION_DES :

                *pulKeySeedSize = ANSC_CRYPTO_DES_KEY_BYTES_NO_PARITY;
                *pulKeyLength   = ANSC_DES_BLOCK_SIZE;
                *pulKeyNumber   = 1;
                *pulKeyRound    = 1;
                *pulBlockSize   = ANSC_DES_BLOCK_SIZE;
                *pIvLength      = ANSC_DES_BLOCK_SIZE;

                break;

        case    ANSC_CRYPTO_ENCRYPTION_DES_IV64 :

                *pulKeySeedSize = ANSC_CRYPTO_DES_KEY_BYTES_NO_PARITY;
                *pulKeyLength   = ANSC_DES_BLOCK_SIZE;
                *pulKeyNumber   = 1;
                *pulKeyRound    = 1;
                *pulBlockSize   = ANSC_DES_BLOCK_SIZE;
                *pIvLength      = ANSC_DES_BLOCK_SIZE;

                break;

        case    ANSC_CRYPTO_ENCRYPTION_DES_IV32 :

                *pulKeySeedSize = ANSC_CRYPTO_DES_KEY_BYTES_NO_PARITY;
                *pulKeyLength   = ANSC_DES_BLOCK_SIZE;
                *pulKeyNumber   = 1;
                *pulKeyRound    = 1;
                *pulBlockSize   = ANSC_DES_BLOCK_SIZE;
                *pIvLength      = 32;

                break;


        case    ANSC_CRYPTO_ENCRYPTION_3DES :

                /* 
                 * the total key length is the *pulKeyLength x *pulKeyNumber
                 */
                *pulKeySeedSize = ANSC_CRYPTO_DES3_KEY_BYTES_NO_PARITY;
                *pulKeyLength   = ANSC_DES_BLOCK_SIZE;                  
                *pulKeyNumber   = 3;
                *pulKeyRound    = 1;
                *pulBlockSize   = ANSC_DES_BLOCK_SIZE;
                *pIvLength      = ANSC_DES_BLOCK_SIZE;



                break;

        case    ANSC_CRYPTO_ENCRYPTION_RC5 :
        case    ANSC_CRYPTO_ENCRYPTION_IDEA :
        case    ANSC_CRYPTO_ENCRYPTION_CAST :
        case    ANSC_CRYPTO_ENCRYPTION_BLOWFISH :
        case    ANSC_CRYPTO_ENCRYPTION_3IDEA :
        case    ANSC_CRYPTO_ENCRYPTION_RC4 :
        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoDesRandomToKey
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pInString,
                ULONG                       ulInLength,
                PANSC_CRYPTO_KEY            hOutKey
            );

    description:

        This function is called to generate a DES key from a random string.
        The random string size has to be 7 bytes. The output key is
        8 bytes.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pInString
                Specifies the input string to generate key.

                ULONG                       ulInLength
                Specifies the number of bytes in the input string.

                PANSC_CRYPTO_KEY            hOutKey
                Specifies the handle of the output key, which is allocated 
                by the caller

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoDesRandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject       = (PANSC_CRYPTO_OBJECT)hThisObject;
    PANSC_CRYPTO_KEY                pOutKey         = (PANSC_CRYPTO_KEY)hOutKey;
    LONG                            i               = 0;


    if (ulInLength != ANSC_CRYPTO_DES_KEY_BYTES_NO_PARITY )
    {
	    return ANSC_STATUS_UNAPPLICABLE;
    }

    pOutKey->KeyNumber      = 1;
    pOutKey->RoundNumber    = 1;
    pOutKey->Length         = ANSC_DES_BLOCK_SIZE;

    /* 
     * take the seven bytes, move them around into the top 7 bits of the
     * 8 pOutKey bytes, then compute the parity bits.  Do this three times. 
     */
    i = 0;
 
	AnscCopyMemory(pOutKey->Value[i], (pInString+i*7), 7);

	pOutKey->Value[i][7] = 
               (((pOutKey->Value[i][0]&1)<<1) |
				((pOutKey->Value[i][1]&1)<<2) |
				((pOutKey->Value[i][2]&1)<<3) |
				((pOutKey->Value[i][3]&1)<<4) |
				((pOutKey->Value[i][4]&1)<<5) |
				((pOutKey->Value[i][5]&1)<<6) |
				((pOutKey->Value[i][6]&1)<<7)   );

    returnStatus =
        pMyObject->FixParity
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pOutKey,
                TRUE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoDes3RandomToKey
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pInString,
                ULONG                       ulInLength,
                PANSC_CRYPTO_KEY            hOutKey
            );

    description:

        This function is called to generate a DES3 key from a random string.
        The random string size has to be 21 bytes. The output key is
        24 bytes.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pInString
                Specifies the input string to generate key.

                ULONG                       ulInLength
                Specifies the number of bytes in the input string.

                PANSC_CRYPTO_KEY            hOutKey
                Specifies the handle of the output key, which is allocated 
                by the caller

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoDes3RandomToKey
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pInString,
        ULONG                       ulInLength,
        PANSC_CRYPTO_KEY            hOutKey
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject       = (PANSC_CRYPTO_OBJECT)hThisObject;
    PANSC_CRYPTO_KEY                pOutKey         = (PANSC_CRYPTO_KEY)hOutKey;
    LONG                            i               = 0;

    if (ulInLength != ANSC_CRYPTO_DES3_KEY_BYTES_NO_PARITY )
    {
	    return ANSC_STATUS_UNAPPLICABLE;
    }

    pOutKey->KeyNumber      = 3;
    pOutKey->RoundNumber    = 1;
    pOutKey->Length         = ANSC_DES_BLOCK_SIZE;

    /* 
     * take the seven bytes, move them around into the top 7 bits of the
     * 8 pOutKey bytes, then compute the parity bits.  Do this three times. 
     */
    for ( i = 0; i < 3; i++ ) 
    {
	    AnscCopyMemory(pOutKey->Value[i], (pInString+i*7), 7);

	    pOutKey->Value[i][7] = 
                   (((pOutKey->Value[i][0]&1)<<1) |
				    ((pOutKey->Value[i][1]&1)<<2) |
				    ((pOutKey->Value[i][2]&1)<<3) |
				    ((pOutKey->Value[i][3]&1)<<4) |
				    ((pOutKey->Value[i][4]&1)<<5) |
				    ((pOutKey->Value[i][5]&1)<<6) |
				    ((pOutKey->Value[i][6]&1)<<7)   );

        returnStatus =
            pMyObject->FixParity
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pOutKey,
                    TRUE
                );
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to derive a key from a base key and
        input constant according to draft-ietf-krb-wg-crypto-01.txt
        May 2002.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBaseKey
                Specifies the handle of the base key.

                ULONG                       EncryptAlgorithm
                Specifies the encryption algorithm to use in the derivation.
                The algorithm also defines the key sizes used in the function.

                ANSC_HANDLE                 hConstant
                Specifies the handle of the constant string to use

                ULONG                       ulConstantLeng,
                Specifies the constant string length.

                ANSC_HANDLE                 hNewKey
                Specifies the handle of the derived key. The caller 
                allocates the memory

    return:     operation status.

**********************************************************************/

ANSC_STATUS
AnscCryptoDeriveKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBaseKey,
        ULONG                       EncryptAlgorithm,
        ANSC_HANDLE                 hConstant,
        ULONG                       ulConstantLeng,
        ANSC_HANDLE                 hNewKey
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject       = (PANSC_CRYPTO_OBJECT)hThisObject;
    PANSC_CRYPTO_KEY                pBaseKey        = (PANSC_CRYPTO_KEY   )hBaseKey;
    PANSC_CRYPTO_KEY                pNewKey         = (PANSC_CRYPTO_KEY   )hNewKey;
    PUCHAR                          pConstant       = (PUCHAR)hConstant;
    ULONG                           ulKeyBytes      = 0;
    ULONG                           ulCurrKeyBytes  = 0;
    ULONG                           ulKeyLength     = 0;
    ULONG                           ulBlockSize     = 0;
    PUCHAR                          pInBlockData    = NULL;
    PUCHAR                          pOutBlockData   = NULL;
    PUCHAR                          pTempKey        = NULL;
    ULONG                           ulBitsInByte    = 8;
    ANSC_CRYPTO_IV                  iv;


    AnscZeroMemory(pNewKey, sizeof(ANSC_CRYPTO_KEY));

    returnStatus =
        pMyObject->MapEncTypeToKeySize
            (
                pMyObject,
                EncryptAlgorithm,
                &ulKeyBytes,
                &ulKeyLength,
                &pNewKey->KeyNumber,
                &pNewKey->RoundNumber,
                &ulBlockSize,
                &iv.Length
            );

    AnscZeroMemory( iv.Value, ANSC_MAX_IV_LENGTH);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    if ( pBaseKey->Length != ulKeyLength ) 
    {
	    return ANSC_STATUS_UNAPPLICABLE;
    }

    pNewKey->Length = ulKeyLength;

    /* 
     * allocate buffers for next processing
     */
    pInBlockData = (PUCHAR)AnscAllocateMemory(ulBlockSize);

    if ( pInBlockData == NULL )
    {
	    return ANSC_STATUS_RESOURCES;
    }

    pOutBlockData = (PUCHAR)AnscAllocateMemory(ulBlockSize);

    if ( pOutBlockData == NULL ) 
    {
	    AnscFreeMemory(pInBlockData);
	    return ANSC_STATUS_RESOURCES;
    }

    pTempKey = (PUCHAR)AnscAllocateMemory(ulKeyBytes);

    if ( pTempKey == NULL ) 
    {
	    AnscFreeMemory(pOutBlockData);
	    AnscFreeMemory(pInBlockData);
	    return ANSC_STATUS_RESOURCES;
    }


    /* 
     * initialize the input block by the input constant 
     */
    if ( ulConstantLeng == ulBlockSize ) 
    {
	    AnscCopyMemory(pInBlockData, pConstant, ulBlockSize);
    } 
    else 
    {
        pMyObject->NFold
            (
                hThisObject,
                pConstant,
                ulConstantLeng * ulBitsInByte,
                ulBlockSize    * ulBitsInByte,
                pInBlockData
            );
    }

    /* 
     * loop encrypting the blocks until enough key bytes are generated 
     * i.e., k-truncate
     */
    ulCurrKeyBytes = 0;

    while ( ulCurrKeyBytes < ulKeyBytes ) 
    {
        AnscZeroMemory( iv.Value, ANSC_MAX_IV_LENGTH);

        pMyObject->Encrypt
            (
                EncryptAlgorithm,
                pInBlockData,
                ulBlockSize,
                pOutBlockData,
                pBaseKey,
                &iv
            );

	    if ( (ulKeyBytes - ulCurrKeyBytes) <= ulBlockSize ) 
        {
	        AnscCopyMemory
                (
                    pTempKey + ulCurrKeyBytes, 
                    pOutBlockData, 
                    (ulKeyBytes - ulCurrKeyBytes)
                );

	        break;
	    }

	    AnscCopyMemory(pTempKey+ulCurrKeyBytes, pOutBlockData, ulBlockSize);
	    AnscCopyMemory(pInBlockData,            pOutBlockData, ulBlockSize);

	    ulCurrKeyBytes += ulBlockSize;
    }

    /*
    AnscTrace("DR:     ");
    for ( j = 0; j < ulKeyBytes; j ++ )
    {
        AnscTrace("%02X ", pTempKey[j]);
    }
    AnscTrace("\n");
    */

    /* 
     * generate the final key 
     */
    returnStatus =
        pMyObject->RandomToKey
            (
                hThisObject,
                EncryptAlgorithm,
                pTempKey,
                ulKeyBytes,
                pNewKey
            );


    /* clean memory, free resources and exit */
    AnscFreeMemory(pTempKey);
    AnscFreeMemory(pOutBlockData);
    AnscFreeMemory(pInBlockData);

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to generate a DES3 key from input string
        salt, and constant. For kerberos, it is ASCII encoding of
        "kerberos"


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hString
                Specifies the input string to generate key.

                ULONG                       ulStringLeng
                Specifies the number of bytes in the input string.

                ANSC_HANDLE                 hSalt
                Specifies the input salt to generate key.

                ULONG                       ulSaltLeng
                Specifies the number of bytes in the input salt.

                ANSC_HANDLE                 hConstant
                Specifies the handle of the constant string to use

                ULONG                       ulConstantLeng,
                Specifies the constant string length.

                PANSC_CRYPTO_KEY            hNewKey
                Specifies the handle of the output key, which is allocated 
                by the caller

    return:     operation status.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_CRYPTO_OBJECT             pMyObject       = (PANSC_CRYPTO_OBJECT)hThisObject;
    PANSC_CRYPTO_KEY                pNewKey         = (PANSC_CRYPTO_KEY   )hNewKey;
    PUCHAR                          pSalt           = (PUCHAR)hSalt;
    PUCHAR                          pString         = (PUCHAR)hString;
    PUCHAR                          pNewString      = NULL;
    ULONG                           ulNewStringLeng = 0;
    ULONG                           ulKeyByteNoP    = ANSC_CRYPTO_DES3_KEY_BYTES_NO_PARITY;
    PUCHAR                          pFoldString     = NULL;
    ULONG                           ulBitsInByte    = 8;
    ANSC_CRYPTO_KEY                 TempKey;


    /*
     * concatinate string and salt
     */
    ulNewStringLeng = ulStringLeng;
    
    if ( pSalt && ulSaltLeng)
    { 
        ulNewStringLeng += ulSaltLeng;
    }

    pNewString = AnscAllocateMemory(ulNewStringLeng);

    if ( !pNewString )
    {
        return ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory
        (
            (PVOID)pNewString,
            (PVOID)pString,
            ulStringLeng
        );

    if ( pSalt && ulSaltLeng )
    {
        AnscCopyMemory
            (
                (PVOID)(pNewString + ulStringLeng),
                (PVOID)pSalt,
                ulSaltLeng
            );
    }

    pFoldString = (PUCHAR) AnscAllocateMemory(ulKeyByteNoP);

    if ( pFoldString == NULL ) 
    {
	    AnscFreeMemory(pNewString);

	    return ANSC_STATUS_RESOURCES;
    }

    /* 
     * fold the new string, make a temp key from it 
     */
     returnStatus =
        pMyObject->NFold
            (
                hThisObject,
                pNewString,
                ulNewStringLeng * ulBitsInByte,
                ulKeyByteNoP    * ulBitsInByte,
                pFoldString
            );

    returnStatus =
        pMyObject->RandomToKey
            (
                hThisObject,
                ANSC_CRYPTO_ENCRYPTION_3DES,
                pFoldString,
                ulKeyByteNoP,
                &TempKey
            );

     /* 
      * now derive the key from this temp key
      */
    returnStatus =
        pMyObject->DeriveKey
            (
                hThisObject,
                &TempKey,
                ANSC_CRYPTO_ENCRYPTION_3DES,
                hConstant,
                ulConstantLeng,
                pNewKey
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscZeroMemory(pNewKey, sizeof(ANSC_CRYPTO_KEY));
    }


    AnscFreeMemory(pFoldString);
    AnscFreeMemory(pNewString);

    return  returnStatus;
}

#ifndef _ANSC_CRYPTO_NO_UUID
/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscCryptoGenerateUuid
            (
                PUCHAR                      pMacAddress,    
                ULONG                       ulMacAddrSize     
            );

    description:

        This function is called to generate uuid based on the MAC address.

    argument:   PUCHAR                      pMacAddress,        
                unique MAC address

                ULONG                       ulMacAddrSize
                must be 6 bytes long.


    return:     the uuid string

**********************************************************************/
char*
AnscCryptoGenerateUuid
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize       /* must be 6 bytes long */
    )
{
    PCHAR                           pUuidValue   = (PCHAR)NULL;
    UCHAR                           hString[16]  = { 0 };
    ULONG                           uTicks       = AnscGetTickInSeconds();
    USHORT                          ClockSeq     = 0;
    errno_t                         rc           = -1;

    if( ulMacAddrSize != 6 || pMacAddress == NULL)
    {
        return pUuidValue;
    }

    /* time_low part */
    AnscWriteUlong(&hString[0], AnscUlongFromNToH(uTicks));

    uTicks     = AnscGetTickInMilliSeconds();

    /* tme_mid and time_hi_and_version */
    AnscWriteUlong(&hString[4], AnscUlongFromNToH(uTicks));

    /* version */
    hString[6] &= 0x0F;
    hString[6] += 0x10;

    /* clock sequence high and low */
    ClockSeq   = AnscCryptoGetRandomUshort(NULL, 0x7788FFEE);

    AnscWriteUshort(&hString[8], AnscUshortFromHToN(ClockSeq));
    hString[8] &= 0x3F;
    hString[8] += 0x80;

    /* the node */
    AnscCopyMemory
        (
            &hString[10],
            pMacAddress,
            ulMacAddrSize
        );

    /* create the uuid value */
    pUuidValue = (PCHAR)AnscAllocateMemory(40);

    if( pUuidValue == NULL)
    {
        return NULL;
    }

    rc = sprintf_s
        (
            pUuidValue,
            40,
            AL_UUID_STRING_FORMAT,
            hString[0],
            hString[1],
            hString[2],
            hString[3],
            hString[4],
            hString[5],
            hString[6],
            hString[7],
            hString[8],
            hString[9],
            hString[10],
            hString[11],
            hString[12],
            hString[13],
            hString[14],
            hString[15]
        );

    if(rc < EOK)
    {
        ERR_CHK(rc);
        return NULL;
    }

    return pUuidValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscCryptoGenerateUuid
            (
                PUCHAR                      pMacAddress,    
                ULONG                       ulMacAddrSize,
                ULONG                       ulUuidVersion
            );

    description:

        This function is called to generate uuid based on the MAC address and
        version.

    argument:   PUCHAR                      pMacAddress,        
                unique MAC address

                ULONG                       ulMacAddrSize
                must be 6 bytes long.

                ULONG                       ulUuidVersion
                the uuid version


    return:     the uuid string

**********************************************************************/
char*
AnscCryptoGenerateUuid2
    (
        PUCHAR                      pMacAddress,        /* unique MAC address   */
        ULONG                       ulMacAddrSize,      /* must be 6 bytes long */
        ULONG                       ulUuidVersion       /* as defined in http://www.ietf.org/internet-drafts/draft-mealling-uuid-urn-05.txt */
    )
{
    PCHAR                           pUuidValue    = (PCHAR)NULL;
    UCHAR                           hString[32]   = { 0 };
    ULONG                           uUuidSize     = 16;
    ANSC_CRYPTO_HASH                hashResult;
    errno_t                         rc            = -1;

    if( ulMacAddrSize != 6 || pMacAddress == NULL)
    {
        return pUuidValue;
    }

    switch ( ulUuidVersion )
    {
        case  3:

            AnscCryptoGetRandomOctets
                (
                    NULL,
                    0xFFEE9988,
                    hString,
                    uUuidSize
                );

            AnscCopyMemory
                (
                    &hString[uUuidSize],
                    pMacAddress,
                    ulMacAddrSize
                );

            AnscCryptoMd5Digest(hString, uUuidSize + ulMacAddrSize, &hashResult);

            AnscCopyMemory
                (
                    hString,
                    hashResult.Value,
                    uUuidSize
                );

            /* version */
            hString[6] &= 0x0F;
            hString[6] += 0x30;

            /* clock sequence high and low */
            hString[8] &= 0x3F;
            hString[8] += 0x80;

            break;

        case  4:

            AnscCryptoGetRandomOctets
                (
                    NULL,
                    0xFFEE1122,
                    hString,
                    uUuidSize
                );

            AnscCopyMemory
                (
                    &hString[10],
                    pMacAddress,
                    ulMacAddrSize
                );

            /* version */
            hString[6] &= 0x0F;
            hString[6] += 0x40;

            /* clock sequence high and low */
            hString[8] &= 0x3F;
            hString[8] += 0x80;

            break;

        case  5:

            AnscCryptoGetRandomOctets
                (
                    NULL,
                    0xFFEE7766,
                    hString,
                    uUuidSize
                );

            AnscCopyMemory
                (
                    &hString[uUuidSize],
                    pMacAddress,
                    ulMacAddrSize
                );

            AnscCryptoSha1Digest(hString, uUuidSize + ulMacAddrSize, &hashResult);

            AnscCopyMemory
                (
                    hString,
                    hashResult.Value,
                    uUuidSize
                );

            /* version */
            hString[6] &= 0x0F;
            hString[6] += 0x50;

            /* clock sequence high and low */
            hString[8] &= 0x3F;
            hString[8] += 0x80;

            break;

        default:
        
            return 
                AnscCryptoGenerateUuid
                    (
                        pMacAddress,
                        ulMacAddrSize
                    );
    }

    /* create the uuid value */
    pUuidValue = (PCHAR)AnscAllocateMemory(40);

    if( pUuidValue == NULL)
    {
        return NULL;
    }

    rc = sprintf_s
        (
            pUuidValue,
            40,
            AL_UUID_STRING_FORMAT,
            hString[0],
            hString[1],
            hString[2],
            hString[3],
            hString[4],
            hString[5],
            hString[6],
            hString[7],
            hString[8],
            hString[9],
            hString[10],
            hString[11],
            hString[12],
            hString[13],
            hString[14],
            hString[15]
        );

    if(rc < EOK)
    {
        ERR_CHK(rc);
        return NULL;
    }

    return pUuidValue;
}
#endif
