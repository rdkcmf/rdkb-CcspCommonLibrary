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

    module: ansc_asn1_bitstring.c

        For ASN.1 BITSTRING object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ANSC_ASN1_BITSTRING"

        *   AnscAsn1BitStringCreate
        *   AnscAsn1BitStringInitialize
        *   AnscAsn1BitStringClone
        *   AnscAsn1BitStringCopyTo
        *   AnscAsn1BitStringEqualsTo
        *   AnscAsn1BitStringGetSizeOfEncoded
        *   AnscAsn1BitStringDecodingData
        *   AnscAsn1BitStringEncodingData
        *   AnscAsn1BitStringDumpObject
        *   AnscAsn1BitStringReadyToEncode

        *   AnscAsn1BitStringGetUnusedBit
        *   AnscAsn1BitStringSetUnusedBit
        *   AnscAsn1BitStringSetBitStringValue

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1BitStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs an ASN.1 object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscAsn1BitStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_BITSTRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_BITSTRING)AnscAllocateMemory(sizeof(ANSC_ASN1_BITSTRING));
    }

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_BITSTRING_ID;
    pMyObject->uType               = ASN1_BITSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1BitStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1BitStringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_BITSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_BITSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BitStringInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1BitStringInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1StringInitialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1BitStringClone;
    pMyObject->CopyTo               = AnscAsn1BitStringCopyTo;
    pMyObject->EqualsTo             = AnscAsn1BitStringEqualsTo;
    pMyObject->DecodingData         = AnscAsn1BitStringDecodingData;
    pMyObject->EncodingData         = AnscAsn1BitStringEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1BitStringGetSizeOfEncoded;
    pMyObject->ReadyToEncode        = AnscAsn1BitStringReadyToEncode;
    pMyObject->DumpObject           = AnscAsn1BitStringDumpObject;

    pMyObject->GetUnusedBit         = AnscAsn1BitStringGetUnusedBit;
    pMyObject->SetUnusedBit         = AnscAsn1BitStringSetUnusedBit;
    pMyObject->SetBitStringValue    = AnscAsn1BitStringSetBitStringValue; 

    pMyObject->uUnusedBit           = 0;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1BitStringClone
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function clones a same ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The cloned container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1BitStringClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    PANSC_ASN1_BITSTRING            pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_BITSTRING)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1BitStringClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
    pNewObject->SetName(pNewObject, pMyObject->Name);

    if( !pMyObject->CopyTo(pMyObject, pNewObject))
    {
        pNewObject->AsnFree(pNewObject);

        return NULL;
    }

    return pNewObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1BitStringCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object.

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1BitStringCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    PANSC_ASN1_BITSTRING            pNewObject   = (PANSC_ASN1_BITSTRING)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }

    /*
     * Copy the optional state;
     */
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;

    /*
     *  Copy the values;
     */
    pNewObject->uUnusedBit          = pMyObject->uUnusedBit;
    pNewObject->bIsDynamic          = pMyObject->bIsDynamic;
    pNewObject->uLength             = pMyObject->uLength;

    if( pNewObject->bIsDynamic)
    {
        pNewObject->pStringBuffer   = (PVOID)AnscAllocateMemory(pMyObject->uLength);

        if( pNewObject->pStringBuffer == NULL)
        {
            AnscTrace("Failed to create memory in AnscAsn1StringCopyTo().\n");

            return FALSE;
        }

        AnscCopyMemory
            ( 
                pNewObject->pStringBuffer, 
                pMyObject->pStringBuffer, 
                pMyObject->uLength
            );
    }
    else
    {
        if( pMyObject->uLength > 0)
        {
            AnscCopyMemory
                ( 
                    pNewObject->pString, 
                    pMyObject->pString,
                    pMyObject->uLength
                );
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1BitStringEqualsTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOtherObject,
                BOOLEAN                     bValueOnly
            );

    description:

        This function compares the value of the 2 ASN.1 objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOtherObject
                This handle is the other object will be compared to;

                BOOLEAN                     bValueOnly
                Check value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1BitStringEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_BITSTRING            pMyObject  = (PANSC_ASN1_BITSTRING)hThisObject;
    PANSC_ASN1_BITSTRING            pNewObject = (PANSC_ASN1_BITSTRING)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1BitStringEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1BitStringEqualsTo().\n");

        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    /* check the attribute list */
    if( !bValueOnly)
    {
        if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
        {
            return FALSE;
        }
    }

    /* check the value */
    if(!pMyObject->bOptional)
    {
        if( pMyObject->bIsDynamic != pNewObject->bIsDynamic ||
            pMyObject->uLength    != pNewObject->uLength    ||
            pMyObject->uUnusedBit != pNewObject->uUnusedBit
          )
        {
            return FALSE;
        }

        if( pMyObject->bIsDynamic)
        {
            return AnscEqualMemory
                        (
                            pMyObject->pStringBuffer,
                            pNewObject->pStringBuffer,
                            pMyObject->uLength
                        );            
        }
        else
        {
            /* case insensitive in printable strings */
            if( pMyObject->uType == ASN1_PRINTABLESTRING_TYPE)
            {
                return ( strcmp((PCHAR)pMyObject->pString,(PCHAR)pNewObject->pString) == 0 );            
            }
            else
            {
                return AnscEqualMemory
                            (
                                pMyObject->pString,
                                pNewObject->pString,
                                pMyObject->uLength
                            );            
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1BitStringGetSizeOfEncoded
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function returns the encoding size of the 
        ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The encoding size of the ASN.1 object.
                If the object is not ready-to-encode, -1 will be 
                returned.

**********************************************************************/
LONG
AnscAsn1BitStringGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    ULONG                           ulSize       = 0;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the bitstring object is not ready to encode.\n");

        return -1;
    }

    /*
     * The data is one byte unusedbit and string values;
     */
    ulSize              = pMyObject->uLength + 1;

    /*
     *  check the attribute list, from the end of the list;
     *  the tag length value is 1.
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BitStringDecodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the input binary data

    return:     status of decoding operation

**********************************************************************/

static ANSC_STATUS
decodingConstructedBitString
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding,
        LONG                        length
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_BITSTRING            pNewObject;
    LONG                            actualLength;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEndBuffer   = NULL;
    PUCHAR                          pEncodingUnit, pBackBuffer;

    pEncodingUnit                   = (PUCHAR)*ppEncoding;

    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
        pMyObject->uLength          = 0;
        pMyObject->bIsDynamic       = FALSE;
        pMyObject->uUnusedBit       = 0;
    }

    if( length == 0)
    {
        AnscTrace("The length of bitstring value is 0, unacceptable.\n");

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
    }

    actualLength    = length;

    /* check the infinite mode first */
    if( length < 0 ) /* infinite mode */
    {
        pEndBuffer  =
            LookingForInfiniteEnd
                (
                    pEncodingUnit,
                    4096
                );

        if( pEndBuffer == NULL)
        {
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1StringDecodingWithoutTag().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        actualLength    =  pEndBuffer - pEncodingUnit;           
    }

    /*
     *  allocate memory first, it's no more than actualLength;
     */
    pMyObject->pStringBuffer = AnscAllocateMemory( actualLength);

    if( pMyObject->pStringBuffer == NULL)
    {
        AnscTrace("Failed to allocate memory in constructed decoding\n");

        return ANSC_STATUS_RESOURCES;
    }

    pMyObject->bIsDynamic   = TRUE;
    pBackBuffer             = pEncodingUnit;

    /*
     * Sample constructed encoding of BITSTRING :
     *  23 09        constructed encoding: "0110111001011101" + "11"
     *  03 03 00 6e 5d
     *  03 02 06 c0
     */

    while( actualLength > 0)
    {
        pNewObject = (PANSC_ASN1_BITSTRING)
                        pMyObject->Create
                            (
                                pMyObject->hContainerContext
                            );

        if( pNewObject == NULL)
        {
            AnscTrace("Failed to create a BITSTRING in constructed decoding\n");

            return ANSC_STATUS_RESOURCES;
        }

        returnStatus = 
            pNewObject->DecodingData
                (
                   pNewObject,
                   (PVOID*)&pEncodingUnit
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTrace("Failed to decode the BITSTRING in constructed decoding\n");
            pNewObject->AsnFree(pNewObject);

            return returnStatus;
        }

        /*
         *  recalculate the length;
         */
        actualLength     -=  pEncodingUnit - pBackBuffer;
        pBackBuffer      =   pEncodingUnit;

        /*
         *  sometimes the non-zero unusedbit is in the first sub-bitstring, 
         *  sometimes it's in the last one. Anyway,we just get the 
         *  non-zero one here.
         */
        if( pNewObject->uUnusedBit != 0)
        {
            pMyObject->uUnusedBit = pNewObject->uUnusedBit;
        }

        /*
         *  Copy the string value;
         */
        AnscCopyMemory
            (
                (PVOID)((PUCHAR)pMyObject->pStringBuffer + pMyObject->uLength),
                pNewObject->pStringBuffer,
                pNewObject->uLength
            );

        pMyObject->uLength          += pNewObject->uLength;

        /*
         * don't forget to remove the sub object.
         */
        pNewObject->AsnFree(pNewObject);
    }

    /* 
     * don't forget to add the last 2 '0x00' in
     * infinite mode
     */
    if( length < 0 ) /* infinite mode */
    {
        pEncodingUnit               += 2;
    }

    /*
     *  check after decoding;
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject, (PVOID*)&pEncodingUnit);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }
    }

    *ppEncoding                     = (PVOID)pEncodingUnit;

    /*
     *  Set the optional state;
     */
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
AnscAsn1BitStringDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    PUCHAR                          pEndBuffer   = NULL;
    ULONG                           uValueLength;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit;

    /* check the parameters */
    if( hThisObject == NULL  || ppEncoding == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    /*
     *  check before decoding;
     */
    if( pMyObject->BeforeDecoding != NULL)
    {
        returnStatus = pMyObject->BeforeDecoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    /* parsing the data */
    pEncodingUnit   = (PUCHAR)*ppEncoding;

    /*
     * Decode the attrlist first;
     */
    returnStatus = 
        AttrListDecodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pEncodingUnit,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize,
                &uInfiniteTags
            );
    
    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        return returnStatus;
    }

    /* if tag included, decode the tag */
    if( bTagIncluded)
    {
        pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

        if( pTagInfo == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);
        
        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return returnStatus;
        }

        /* check the tag matched or not */
        if( pTagInfo->FirstOctetOfTag != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject))
        {
            if( pTagInfo->FirstOctetOfTag == 
                (pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject) | CONSTRUCTED_FORM))
            {
                AnscTrace("**** Constructed Encoding was used in BITSTRING encoding, try to decode it.\n");
     
                pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);

                returnStatus    = 
                    decodingConstructedBitString
                        (
                          hThisObject,
                          (PVOID*)&pEncodingUnit,
                          pTagInfo->ulContentLength
                        );

                pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

                AnscTrace("**** The result of decoding constructed BITSTRING is ");
                TraceErrorMessage(returnStatus);

                goto EXIT;
            }

            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_BITSTRING - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->GetSizeOfTag(pTagInfo) != 1 )
        {
            AnscTrace("The length of BITSTRING tag is unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

        /* check the tag */
        if( pTagInfo->bIsInfinite)
        {
            uLeftSize       = 0;
            uInfiniteTags   ++;
        }
        else
        {
            uLeftSize       = pTagInfo->ulContentLength;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);

        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
        pMyObject->uLength          = 0;
        pMyObject->bIsDynamic       = FALSE;
        pMyObject->uUnusedBit       = 0;
    }

    uValueLength    = uLeftSize;

    if( uValueLength == 0)
    {
        /* infinite mode */
        pEndBuffer  =
            LookingForInfiniteEnd
                (
                    pEncodingUnit,
                    4096
                );

        if( pEndBuffer == NULL)
        {
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1StringDecoding().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        uValueLength = pEndBuffer - pEncodingUnit;
    }

    /* 
     * zhubin updated here on 09/16/2004 
     *
     * To protect from ASN.1 attack
     */
    if( uValueLength > 20000)
    {
        AnscTrace("The length of bit string is unreasonable '%lu'\n", uValueLength);

        pMyObject->uLength = 0;

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
    }

    pMyObject->uLength          = uValueLength - 1;
    pMyObject->uUnusedBit       = (ULONG)pEncodingUnit[0];
    pEncodingUnit               ++;

    if( pMyObject->uLength <= MAXIMUM_OCTETSTRING_OCTETS)
    {
        pMyObject->bIsDynamic   = FALSE;
        AnscCopyMemory( pMyObject->pString, pEncodingUnit, pMyObject->uLength);
    }
    else
    {   
        pMyObject->pStringBuffer = (PUCHAR)AnscAllocateMemory(pMyObject->uLength);

        if( pMyObject->pStringBuffer == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->bIsDynamic   = TRUE;
        AnscCopyMemory( pMyObject->pStringBuffer, pEncodingUnit, pMyObject->uLength);
    }

    pEncodingUnit               += pMyObject->uLength;

EXIT:

    /*
     *  if succeeded, set the new location.
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        /* Don't forget adding the end of those infinite forms */
        while( uInfiniteTags > 0)
        {
            if( pEncodingUnit[0] != 0x00 || pEncodingUnit[1] != 0x00)
            {
                /*
                AnscTrace
                    (
                        "Unexpected End Of Indefinite Content : [%.2X %.2X %.2X %.2X] !\n",
                        pEncodingUnit[0],
                        pEncodingUnit[1],
                        pEncodingUnit[2],
                        pEncodingUnit[3] 
                    );
                */
                returnStatus = ANSC_ASN1_INFINITE_END_MISSED;

                break;                
            }

            pEncodingUnit += 2;
            uInfiniteTags --;
        }
    }

    /*
     *  check after decoding;
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject, (PVOID*)&pEncodingUnit);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }

        *ppEncoding             = (PVOID)pEncodingUnit;
   }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BitStringEncodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the output binary data

    return:     status of encoding operation

**********************************************************************/
ANSC_STATUS
AnscAsn1BitStringEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_BITSTRING            pMyObject       = (PANSC_ASN1_BITSTRING)hThisObject;
    BOOLEAN                         bTagIncluded    = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData       = (PUCHAR)*ppEncoding;
    PUCHAR                          pBackData       = pCharData;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /*
     *  check before encoding;
     */
    if( pMyObject->BeforeEncoding != NULL)
    {
        returnStatus = pMyObject->BeforeEncoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    uSizeOfEncoded = pMyObject->GetSizeOfEncoded(pMyObject);

    if( uSizeOfEncoded < 0)
    {
        return ANSC_ASN1_NOT_READY_TO_ENCODE;
    }

    /*
     * Encode all attrlist first; 
     */
    returnStatus = 
        AttrListEncodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pCharData,
                (ULONG)uSizeOfEncoded,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize
            );

    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        return returnStatus;
    }

    /*
     *  Encode the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData                  = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        EncodeLength((PVOID*)&pCharData, pMyObject->uLength + 1);
    }

    /*
     *  Encode the unused bit first;
     */
    *pCharData                      = (UCHAR)pMyObject->uUnusedBit;
    pCharData++;

   /*
    *   Encode the value;
    */
    if( pMyObject->uLength > 0)
    {
        if( pMyObject->bIsDynamic)
        {
            AnscCopyMemory
                (
                    pCharData,
                    pMyObject->pStringBuffer,
                    pMyObject->uLength
                );
        }
        else
        {
            AnscCopyMemory
                (
                    pCharData,
                    pMyObject->pString,
                    pMyObject->uLength
                );
        }
    }

    pCharData    += pMyObject->uLength;

    /*
     *  Set the buffer before exit;
     */
    if( ANSC_STATUS_SUCCESS == returnStatus)
    { 
         if( pCharData - pBackData != uSizeOfEncoded)
        {
            /*
            AnscTrace
                (
                    "Warning in Encoding BITSTRING, the size of encoding is expected to be %d, but it's %d.\n",
                    uSizeOfEncoded,
                    (pCharData - pBackData)
                );
            */
        }

        *ppEncoding                     =  (PVOID)pCharData;

       /*
         *  check after encoding;
         */
        if( pMyObject->AfterEncoding != NULL)
        {
            returnStatus = pMyObject->AfterEncoding(pMyObject, ppEncoding);
        }
        
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1BitStringDumpObject
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pBuffer,
                PULONG                      pLength,
                BOOLEAN                     bIsRoot,
                BOOLEAN                     bShowValue
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pBuffer,
                The output buffer;

                PULONG                      pLength,
                The pointer of buffer size;

                BOOLEAN                     bIsRoot
                It's a root object or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/

#ifndef _PKI_KERNEL

static void
writeValueToBuffer
    (
        PUCHAR                      pEncoding,
        ULONG                       length,
        PCHAR                       pOutput   
    )
{
    PCHAR                           pBuffer;
    
    if( length <= 4)
    {
        AnscFromHexToString
            (
                pEncoding,
                pOutput,
                length,
                FALSE
            );
    }
    else
    {
        AnscFromHexToString
            (
                pEncoding,
                pOutput,
                4,
                FALSE
            );


        pBuffer = (PCHAR)(pOutput + 4*2);

        pBuffer[0] = '.';
        pBuffer[1] = '.';
        pBuffer[2] = '.';
        pBuffer[3] = 0;
    }
}

#endif

BOOLEAN
AnscAsn1BitStringDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

#ifndef _PKI_KERNEL

    PANSC_ASN1_BITSTRING            pMyObject       = (PANSC_ASN1_BITSTRING)hThisObject;
    CHAR                            pAttrBuffer[512]= { 0 };
    ULONG                           attrLength      = 512;
    PCHAR                           pName;
    CHAR                            pValueBuf[128]  = { 0 };
    errno_t                         rc              = -1;

    if( pBuffer == NULL || pLength == NULL)
    {
        return FALSE;
    }

    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

    if( bIsRoot)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if( pName == NULL)
    {
        return FALSE;
    }

    if( !bShowValue)
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else if( pMyObject->bOptional)
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (Optional)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else if( pMyObject->uLength == 0)
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (%d,L0)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    (int)pMyObject->uUnusedBit
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else
    {
        writeValueToBuffer( pMyObject->GetValueBuffer(pMyObject), pMyObject->uLength, pValueBuf);

        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (%d,L%d,0x%s)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    (int)pMyObject->uUnusedBit,
                    (int)pMyObject->uLength,
                    pValueBuf
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }

#endif

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1BitStringReadyToEncode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function checked the object is ready to encode or not

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     ready or not.

**********************************************************************/
BOOLEAN
AnscAsn1BitStringReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BITSTRING                 pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;

    if( pMyObject->bOptional)
    {
        return TRUE;
    }

    /*
     * Check the validity of the value
     */
    if( pMyObject->uUnusedBit  > 7)
    {
        AnscTrace("Unacceptable unused bit: %lu\n", pMyObject->uUnusedBit);

        return FALSE;
    }

    return TRUE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAsn1BitStringGetUnusedBit
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the unused bit of bitstring.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the unused bit

**********************************************************************/
ULONG
AnscAsn1BitStringGetUnusedBit
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;

    return pMyObject->uUnusedBit;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BitStringSetUnusedBit
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uBits
            );

    description:

        This function sets the unused bit of bitstring.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uBits
                The input unusedbits;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1BitStringSetUnusedBit
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uBits
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;

    if( uBits > 7)
    {
        return ANSC_ASN1_INVALID_UNUSED_BIT;
    }

    pMyObject->uUnusedBit           = uBits;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BitStringSetBitStringValue
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uBits,
                PUCHAR                      pData,
                ULONG                       uSize
            );

    description:

        This function set the bitstring value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uBits,
                The unused bit

                PUCHAR                      pData,
                The input binary data value;

                ULONG                       uSize
                The size of the buffer;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1BitStringSetBitStringValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uBits,
        PUCHAR                      pData,
        ULONG                       uSize
    )
{
    PANSC_ASN1_BITSTRING            pMyObject    = (PANSC_ASN1_BITSTRING)hThisObject;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    returnStatus                    = pMyObject->SetUnusedBit(hThisObject, uBits);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
    }
    pMyObject->uLength          = 0;
    pMyObject->bIsDynamic       = FALSE;

    if( pData == NULL || uSize == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }

    pMyObject->uLength              = uSize;

    if( pMyObject->uLength <= MAXIMUM_OCTETSTRING_OCTETS)
    {
        pMyObject->bIsDynamic       = FALSE;
        AnscCopyMemory( pMyObject->pString, pData, pMyObject->uLength);
    }
    else
    {   
        pMyObject->pStringBuffer = (PUCHAR)AnscAllocateMemory(pMyObject->uLength);

        if( pMyObject->pStringBuffer == NULL)
        {
            pMyObject->uLength      = 0;

            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->bIsDynamic       = TRUE;
        AnscCopyMemory( pMyObject->pStringBuffer, pData, pMyObject->uLength);
    }

    /*
     *  set as no-optional
     */
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}


