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

    module: ansc_asn1_integer.c

        For ASN.1 INTEGER object

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class  "ASN1_INTEGER"

        *   AnscAsn1IntegerCreate
        *   AnscAsn1IntegerFree
        *   AnscAsn1IntegerInitialize
        *   AnscAsn1IntegerClone
        *   AnscAsn1IntegerCopyTo
        *   AnscAsn1IntegerEqualsTo
        *   AnscAsn1IntegerGetSizeOfEncoded
        *   AnscAsn1IntegerDecodingData
        *   AnscAsn1IntegerEncodingData
        *   AnscAsn1IntegerDumpObject
        *   AnscAsn1IntegerTraceObject

        *   AnscAsn1IntegerGetIntegerValue
        *   AnscAsn1IntegerSetIntegerValue
        *   AnscAsn1IntegerSetUlongValue
        *   AnscAsn1IntegerIsBigInteger
        *   AnscAsn1IntegerSetStringValue
        *   AnscAsn1IntegerSetUnsignedStringValue
        *   AnscAsn1IntegerGetStringValue
        *   AnscAsn1IntegerGetValueBuffer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/13/2002    initial revision.
        06/14/2002    PFN_ASN1_GET_VALUE_BUFFER was added for Integer  
                      and STRING objects.
        11/14/2002  Add SetUlongValue for INTEGER object;

**********************************************************************/

#include "ansc_asn1_local.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1IntegerCreate
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
AnscAsn1IntegerCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_INTEGER              pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_INTEGER)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_INTEGER)AnscAllocateMemory(sizeof(ANSC_ASN1_INTEGER));
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
    pMyObject->Oid                 = ANSC_ASN1_INTEGER_ID;
    pMyObject->uType               = ASN1_INTEGER_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->bFirstZero          = FALSE;
    pMyObject->Create              = AnscAsn1IntegerCreate;
    pMyObject->AsnFree             = AnscAsn1IntegerFree;
    
    AnscAsn1IntegerInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_INTEGER_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_INTEGER_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerFree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1IntegerFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject     = (PANSC_ASN1_INTEGER)hThisObject;

    if( pMyObject != NULL)
    {
        if( pMyObject->pValueBuffer)
        {
            AnscFreeMemory( pMyObject->pValueBuffer);
        }
    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerInitialize
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
AnscAsn1IntegerInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject    = (PANSC_ASN1_INTEGER)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1IntegerClone;
    pMyObject->CopyTo               = AnscAsn1IntegerCopyTo;
    pMyObject->EqualsTo             = AnscAsn1IntegerEqualsTo;
    pMyObject->DecodingData         = AnscAsn1IntegerDecodingData;
    pMyObject->EncodingData         = AnscAsn1IntegerEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1IntegerGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1IntegerDumpObject;


    pMyObject->GetIntegerValue      = AnscAsn1IntegerGetIntegerValue;
    pMyObject->SetIntegerValue      = AnscAsn1IntegerSetIntegerValue;
    pMyObject->SetUlongValue        = AnscAsn1IntegerSetUlongValue;
    pMyObject->IsBigInteger         = AnscAsn1IntegerIsBigInteger;
    pMyObject->SetStringValue       = AnscAsn1IntegerSetStringValue;
    pMyObject->SetUnsignedStringValue
                                    = AnscAsn1IntegerSetUnsignedStringValue;
    pMyObject->GetStringValue       = AnscAsn1IntegerGetStringValue;
    pMyObject->GetValueBuffer       = AnscAsn1IntegerGetValueBuffer;

    pMyObject->bIsDynamic           = FALSE;
    pMyObject->lValue               = 0;
    pMyObject->pValueBuffer         = NULL;
    pMyObject->uLength              = 1;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1IntegerClone
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
AnscAsn1IntegerClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject    = (PANSC_ASN1_INTEGER)hThisObject;
    PANSC_ASN1_INTEGER              pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_INTEGER)
                 pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1IntegerClone()\n");

        return NULL;
    }

    /*
     * Copy all the attributes;
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
        AnscAsn1IntegerCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies value to the dest object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The destination object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1IntegerCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject    = (PANSC_ASN1_INTEGER)hThisObject;
    PANSC_ASN1_INTEGER              pNewObject   = (PANSC_ASN1_INTEGER)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }

    /*
     * Copy the values;
     */
    pNewObject->bIsDynamic          =  pMyObject->bIsDynamic;
    pNewObject->uLength             =  pMyObject->uLength;
    pNewObject->lValue              =  pMyObject->lValue;
    pNewObject->bFirstZero          =  pMyObject->bFirstZero;

    if( pMyObject->pValueBuffer)
    {
        pNewObject->pValueBuffer    =  (PUCHAR)AnscAllocateMemory(pNewObject->uLength);

        if( pNewObject->pValueBuffer)
        {
            AnscCopyMemory(pNewObject->pValueBuffer, pMyObject->pValueBuffer, pNewObject->uLength);
        }
    }
    
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;


    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1IntegerEqualsTo
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
                Check the value  only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1IntegerEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;
    PANSC_ASN1_INTEGER              pNewObject = (PANSC_ASN1_INTEGER)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1IntegerEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1IntegerEqualsTo().\n");

        return FALSE;
    }

    /* check the attribute list */
    if(!bValueOnly)
    {
        if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
        {
            return FALSE;
        }
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    if(!pMyObject->bOptional)
    {
        /* check the dynamic state */
        if( pMyObject->bIsDynamic != pNewObject->bIsDynamic)
        {
            AnscTrace("Different dynamic state in AnscAsn1IntegerEqualsTo().\n");

            return FALSE;
        }

        /* check the value */
        if( pMyObject->bIsDynamic)
        {
            if( pMyObject->uLength != pNewObject->uLength)
            {
                AnscTrace
                    (
                        "Different length in AnscAsn1IntegerEqualsTo() (%d) != (%d).\n",
                        pMyObject->uLength, 
                        pNewObject->uLength
                    );

                return FALSE;            
            }
            else if( !AnscEqualMemory(pMyObject->pValueBuffer, pNewObject->pValueBuffer, pMyObject->uLength))
            {
                return FALSE;
            }
        }
        else if( pMyObject->lValue != pNewObject->lValue)
        {
            AnscTrace
                (
                    "Different value in AnscAsn1IntegerEqualsTo() (%d) != (%d).\n",
                    pMyObject->lValue, 
                    pNewObject->lValue
                );

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1IntegerGetSizeOfEncoded
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
AnscAsn1IntegerGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject    = (PANSC_ASN1_INTEGER)hThisObject;
    ULONG                           ulSize       = 0;
    ULONG                           tagSize      = 0;    

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the ENUMERATE object is not ready to encode.\n");

        return -1;
    }

    ulSize              = pMyObject->uLength;

    /*
     *  The tag size is 1;
     */
    tagSize  = 1;

    /*
     *  check the attribute list, from the end of the list;
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerDecodingData
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
ANSC_STATUS
AnscAsn1IntegerDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_INTEGER              pMyObject    = (PANSC_ASN1_INTEGER)hThisObject;
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
        if( pTagInfo->FirstOctetOfTag != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject) ||
            pTagInfo->GetSizeOfTag(pTagInfo) != 1
          )
        {
            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_INTEGER - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->ulContentLength == 0)
        {       
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            AnscTrace("The length of integer value is 0, unacceptable.\n");
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
    if( pMyObject->pValueBuffer != NULL)
    {
        AnscFreeMemory(pMyObject->pValueBuffer);
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
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1IntegerDecodingData().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        uValueLength = pEndBuffer - pEncodingUnit;
    }

    /* 
     * zhubin updated here on 09/16/2004 
     *
     * To protect from ASN.1 attack
     */
    if( uValueLength > 2000)
    {
        AnscTrace("The length of integer is unreasonable '%d'\n", uValueLength);

        pMyObject->uLength = 0;

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
    }

    pMyObject->bIsDynamic   = TRUE;
    pMyObject->uLength      = uValueLength;
    pMyObject->pValueBuffer = (PUCHAR)AnscAllocateMemory(pMyObject->uLength);

    if( pMyObject->pValueBuffer == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory( pMyObject->pValueBuffer, pEncodingUnit, pMyObject->uLength);

    if( pMyObject->uLength <= sizeof(ULONG) + 1)
    {
        /* could be 5 bytes unsigned integer */
        if( pMyObject->uLength <= sizeof(ULONG) || pEncodingUnit[0] == 0x00)
        {
            pMyObject->lValue = 
                GetIntegerValueFromBuffer
                    ( 
                        pMyObject->pValueBuffer,
                        pMyObject->uLength
                    );

            if( pEncodingUnit[0] == 0x00 && pMyObject->uLength > 1)
            {
                pMyObject->bFirstZero = TRUE;
            }
            else
            {
                pMyObject->bFirstZero = FALSE;
            }
        }
    }

    pEncodingUnit   += uValueLength;

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

    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        /*
         *  check after decoding;
         */
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject,(PVOID*)&pEncodingUnit);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }

        *ppEncoding             = (PVOID)pEncodingUnit;
        pMyObject->bOptional    = FALSE;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerEncodingData
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
AnscAsn1IntegerEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_INTEGER              pMyObject      = (PANSC_ASN1_INTEGER)hThisObject;

    /*
     * hold the computation value
     */
    ULONG                           ulResult        = (ULONG)pMyObject->lValue;
    BOOLEAN                         bTagIncluded    = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData       = (PUCHAR)*ppEncoding;
    PUCHAR                          pBackData       = pCharData;
    CHAR                            pTmp[5]         = { 0 };

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
        returnStatus = pMyObject->BeforeEncoding(pMyObject,ppEncoding);

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
        *pCharData              = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        EncodeLength((PVOID*)&pCharData, pMyObject->uLength);
    }

    /*
     * we don't have problem if the current encoding is dynamic encoding; if the current encoding is static, we have to 
     * examine the long value and find out how many octets are needed
     */
    if ( pMyObject->bIsDynamic)
    {
        AnscCopyMemory((PVOID)pCharData, (PVOID)pMyObject->pValueBuffer, pMyObject->uLength);

        pCharData                += pMyObject->uLength;
    }
    else
    {
        if(pMyObject->lValue >= 0)
        {
            if( pMyObject->bFirstZero)
            {
                *pCharData               = 0x00;
                pCharData++;
            }

            if( ulResult <= (ULONG)0xFF)
            {       
                *pCharData               = (UCHAR)(ulResult & 0xFF);
                pCharData++;
            }
            else
            {
                pCharData += ASN1WriteUlong( pCharData, ulResult);
            }
        }
        else
        {
            /*
             *  write to ulong buffer;
             */
            AnscWriteUlong(pTmp, AnscUlongFromHToN(ulResult));

            if( pMyObject->bFirstZero)
            {
                *pCharData               = 0x00;
                pCharData++;

                AnscCopyMemory
                    (
                        pCharData,
                        pTmp + sizeof(ULONG) - pMyObject->uLength + 1,
                        pMyObject->uLength
                    );

                pCharData += pMyObject->uLength - 1;
            }
            else
            {
                AnscCopyMemory
                    (
                        pCharData,
                        pTmp + sizeof(ULONG) - pMyObject->uLength,
                        pMyObject->uLength
                    );


                pCharData += pMyObject->uLength;
            }
        }
    }

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
                    "Warning in Encoding INTEGER, the size of encoding is expected to be %d, but it's %d.\n",
                    uSizeOfEncoded,
                    (pCharData - pBackData)
                );
            */
        }

        /*
         *  check after encoding;
         */
        if( pMyObject->AfterEncoding != NULL)
        {
            returnStatus = pMyObject->AfterEncoding(pMyObject,(PVOID*)&pCharData);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }

        *ppEncoding  =  (PVOID)pCharData;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1IntegerDumpObject
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

                BOOLEAN                     bIsRoot,
                The  object is  the root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1IntegerDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_INTEGER              pMyObject       = (PANSC_ASN1_INTEGER)hThisObject;
    CHAR                            pAttrBuffer[512]= { 0 };
    ULONG                           attrLength      = 512;
    PCHAR                           pName;
    PUCHAR                          pValueBuffer;

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

    if( !bShowValue)
    {
        *pLength = 
            AnscSprintfString
                (
                    pBuffer,
                    "%s ::=%s %s",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
    }
    else if( pMyObject->bOptional)
    {
        *pLength = 
            AnscSprintfString
                (
                    pBuffer,
                    "%s ::=%s %s (Optional)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
    }
    else if( pMyObject->uLength <= sizeof(ULONG))
    {
        *pLength = 
            AnscSprintfString
                (
                    pBuffer,
                    "%s ::=%s %s (%d)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->lValue
                );
    }
    else
    {
        pValueBuffer = pMyObject->GetValueBuffer(pMyObject);

        *pLength = 
            AnscSprintfString
                (
                    pBuffer,
                    "%s ::=%s %s (L%d, 0x%.2X%.2X%.2X%.2X...)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->uLength,
                    pValueBuffer[0],
                    pValueBuffer[1],
                    pValueBuffer[2],
                    pValueBuffer[3]
                );
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1IntegerTraceObject
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        layer,
                BOOLEAN                     bShowValue,
                BOOLEAN                     bRecursive
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                LONG                        layer,
                Specifies the layer of current object.

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

                BOOLEAN                     bRecursive
                Trace recursive or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1IntegerTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_INTEGER              pMyObject        = (PANSC_ASN1_INTEGER)hThisObject;
    CHAR                            pAttrBuffer[128] = { 0 };
    ULONG                           attrLength       = 128;
    PCHAR                           pName;

    TraceTabs(layer);
    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

    if( layer  == 0)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if(bShowValue)
    {
        if( pMyObject->bOptional)
        {
            AnscTrace
                (
                    "%s ::=%s %s (Optional)\n", 
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        }
        else if( pMyObject->uLength <= sizeof(ULONG) && !pMyObject->bIsDynamic)
        {
           AnscTrace
               (
                    "%s ::=%s %s (%d)\n", 
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->lValue
               );
        }
        else if( pMyObject->uLength > sizeof(ULONG))
        {
           AnscTrace
               (
                    "%s ::=%s %s (L%d - ",     
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->uLength
               );
           
           TraceBinaryData(pMyObject->pValueBuffer, pMyObject->uLength,16);
 
           AnscTrace(")\n");
        }
        else
        {
           AnscTrace
               (
                    "%s ::=%s %s (%d - ",     
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->lValue
               );
           
           TraceBinaryData(pMyObject->pValueBuffer, pMyObject->uLength,pMyObject->uLength);

           AnscTrace(")\n");

        }
    }
    else
    {
        AnscTrace
            (
                "%s ::=%s %s \n", 
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType)
            );
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1IntegerGetIntegerValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the integer value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the integer value.

**********************************************************************/
LONG
AnscAsn1IntegerGetIntegerValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER               pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;

    return pMyObject->lValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerSetIntegerValue
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        lValue
            );

    description:

        This function set the integer value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                LONG                        lValue
                The input long value;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1IntegerSetIntegerValue
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lValue
    )
{
    PANSC_ASN1_INTEGER              pMyObject               = (PANSC_ASN1_INTEGER)hThisObject;
    ULONG                           ulSize                  = 0;
    ULONG                           uTemp                   = lValue;
    CHAR                            pTmp[sizeof(ULONG) + 1] = { 0 };
    UINT                            i;

    if( pMyObject->pValueBuffer)
    {
        AnscFreeMemory(pMyObject->pValueBuffer);
    }

    pMyObject->pValueBuffer         = NULL;
    pMyObject->lValue               = lValue;
    pMyObject->bIsDynamic           = FALSE;

    if( lValue >= 0)
    {
        return pMyObject->SetUlongValue(pMyObject,uTemp);
    }
    else
    {
        /*
         *  write to ulong buffer;
         */
        AnscWriteUlong(pTmp, AnscUlongFromHToN(uTemp));

        ulSize                          = 4;
        i                               = 0;

        while( ulSize > 1 && (UCHAR)pTmp[i] == (UCHAR)0xFF && (UCHAR)pTmp[i+1] >= (UCHAR)0x80)
        {
            ulSize --;
            i ++;
        }

        pMyObject->uLength              = ulSize;
        pMyObject->bFirstZero           = FALSE;
    }

    pMyObject->bOptional  = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerSetUlongValue
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uValue
            );

    description:

        This function set the integer value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uValue
                The input ULONG value;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1IntegerSetUlongValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uValue
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;
    ULONG                           ulSize       = 0;
    ULONG                           uTemp        = uValue;
    ULONG                           uLast;

    if( pMyObject->pValueBuffer)
    {
        AnscFreeMemory(pMyObject->pValueBuffer);
    }

    pMyObject->pValueBuffer         = NULL;
    pMyObject->lValue               = uValue;
    pMyObject->bIsDynamic           = FALSE;

    uLast   = uTemp;
    if( uTemp <= 0xFF)
    {
        ulSize                      = 1;
    }
    else
    {
        while( uTemp > 0)
        {
            uLast                   = uTemp;
            uTemp                   = uTemp / 256;
            ulSize  ++;
        }
    }

    /* make sure it's an unsigned integer */
    if( uLast >= (ULONG)0x80)
    {
        pMyObject->bFirstZero       = TRUE;
        ulSize ++;
    }
    else
    {
        pMyObject->bFirstZero       = FALSE;
    }

    pMyObject->uLength              = ulSize;

    pMyObject->bOptional  = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1IntegerIsBigInteger
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the dynamic state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the dynamic state.

**********************************************************************/
BOOLEAN
AnscAsn1IntegerIsBigInteger
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;

    return pMyObject->bIsDynamic;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerSetStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pData,
                ULONG                       length
            );

    description:

        This function set the long integer value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pData
                The input long integer value buffer;

                ULONG                       length
                The size of the buffer;

    return:     the result of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1IntegerSetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;

    /* Free the old value */
    if( pMyObject->pValueBuffer)
    {
        AnscFreeMemory(pMyObject->pValueBuffer);
    }

    pMyObject->bIsDynamic           =  TRUE;

    if( length >= 64 && pData[0] != 0x00)
    {
        pMyObject->uLength              =  length + 1;
    }
    else
    {
        pMyObject->uLength              =  length;
    }

    pMyObject->pValueBuffer         =  (PUCHAR)AnscAllocateMemory(pMyObject->uLength);
    
    if( !pMyObject->pValueBuffer)
    {
        AnscTrace("Short of memory in AnscAsn1IntegerSetStringValue().\n");

        return ANSC_STATUS_RESOURCES;
    }

    AnscZeroMemory(pMyObject->pValueBuffer, pMyObject->uLength);

    if( pMyObject->uLength > length)
    {
        AnscCopyMemory((PUCHAR)(pMyObject->pValueBuffer + 1), pData, length);
    }
    else
    {
        AnscCopyMemory(pMyObject->pValueBuffer, pData, length);
    }

    /* if not long integer, set the ulong value as well */
    if( length <= sizeof(ULONG))
    {
        pMyObject->lValue           = GetLengthFromBuffer( pMyObject->pValueBuffer, pMyObject->uLength);
    }

    pMyObject->bOptional  = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerSetUnsignedStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pData,
                ULONG                       length
            );

    description:

        This function set the long integer value and make sure it > 0.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pData
                The input long integer value buffer;

                ULONG                       length
                The size of the buffer;

    return:     the result of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1IntegerSetUnsignedStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;

    /* Free the old value */
    if( pMyObject->pValueBuffer)
    {
        AnscFreeMemory(pMyObject->pValueBuffer);
    }

    pMyObject->bIsDynamic           =  TRUE;
    pMyObject->uLength              =  length;

    /* check the string > 0 or not */
    if( pData[ 0 ] >= 0x80)
    {
        pMyObject->uLength ++;
    }

    pMyObject->pValueBuffer         =  (PUCHAR)AnscAllocateMemory(pMyObject->uLength);
    
    if( !pMyObject->pValueBuffer)
    {
        AnscTrace("Short of memory in AnscAsn1IntegerSetStringValue().\n");

        return ANSC_STATUS_RESOURCES;
    }

    AnscZeroMemory(pMyObject->pValueBuffer, pMyObject->uLength);
    AnscCopyMemory
        (
            (PUCHAR)(pMyObject->pValueBuffer + pMyObject->uLength - length), 
            pData, 
            length
        );

    pMyObject->bOptional  = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1IntegerGetStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PPUCHAR                     ppData,
                PULONG                      pLength
            );

    description:

        This function set the long integer value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PPUCHAR                     ppData
                The buffer of output long integer value buffer;

                PULONG                      pLength
                The buffer of size;

    return:     the result of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1IntegerGetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppData,
        PULONG                      pLength
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;
    PUCHAR                          pData      = (PUCHAR)*ppData;

    /*
     * hold the computation value
     */
    ULONG                           ulResult   = pMyObject->lValue;

    /*
     * temporary counters
     */
    ULONG                           counter    = 0;

    if( !ppData || !pLength )
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    /* check the memeory */
    if( pData == NULL)
    {
        pData = (PUCHAR)AnscAllocateMemory( pMyObject->uLength + 8);

        *ppData = pData;

        *pLength = pMyObject->uLength;
    }

    if( pData == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    if( pMyObject->bIsDynamic)
    {
        if( *pLength < pMyObject->uLength)
        {
            return ANSC_ASN1_BAD_PARAMETER;
        }

        *pLength                        = pMyObject->uLength;

        AnscCopyMemory( pData, pMyObject->pValueBuffer, pMyObject->uLength);
    }
    else
    {
        if( ulResult == 0)
        {
            counter                     = 1;
        }

        while ( ulResult )
        {
            ulResult = ulResult / 256;
            counter++;
        }
        
        ASN1WriteUlong( pData, pMyObject->lValue);
        *pLength                        = counter;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscAsn1IntegerGetValueBuffer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

         get the string value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the string value buffer;

**********************************************************************/
PUCHAR
AnscAsn1IntegerGetValueBuffer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_INTEGER              pMyObject  = (PANSC_ASN1_INTEGER)hThisObject;


    if( pMyObject == NULL)
    {
        return NULL;
    }

    return pMyObject->pValueBuffer;
}

