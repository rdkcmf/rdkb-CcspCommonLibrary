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

    module: ansc_asn1_set.c

        For ASN.1 SET object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_SET"

        *   AnscAsn1SetCreate
        *   AnscAsn1SetFree
        *   AnscAsn1SetInitialize
        *   AnscAsn1SetClone
        *   AnscAsn1SetCopyTo
        *   AnscAsn1SetEqualsTo
        *   AnscAsn1SetGetSizeOfEncoded
        *   AnscAsn1SetDecodingData
        *   AnscAsn1SetEncodingData
        *   AnscAsn1SetDumpObject
        *   AnscAsn1SetTraceObject
        *   AnscAsn1SetReadyToEncode

        *   AnscAsn1SetCreateChildObject
        *   AnscAsn1SetEnumChildObject
        *   AnscAsn1SetCreateChildAttr
        *   AnscAsn1SetGetChildName
        *   AnscAsn1SetGetChildByIndex
        *   AnscAsn1SetSetChildByIndex
        *   AnscAsn1SetGetChildCount

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/25/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1SetCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ULONG                       size
            );

    description:

        This function constructs an ASN.1 object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ULONG                       size
                The size of the structure;

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscAsn1SetCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_SET                  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     * RDKB-6199, CID-24280, do mem reset only for allocated memory.
     */
    if( (ULONG)hContainerContext > 0)
    {
        pMyObject = (PANSC_ASN1_SET)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_SET)AnscAllocateMemory(sizeof(ANSC_ASN1_SET));
    }

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    /*
     *  reset the child list;
     */
    AnscSListInitializeHeader(&pMyObject->sChildList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_SET_ID;
    pMyObject->uType               = ASN1_SET_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1SetCreate;
    pMyObject->AsnFree             = AnscAsn1SetFree;
    
    AnscAsn1SetInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    /*CID: 125313 Explicit null dereferenced*/
    if (pMyObject->SetName)
        pMyObject->SetName(pMyObject,ANSC_ASN1_SET_NAME);
    if (pMyObject->SetClassName)
        pMyObject->SetClassName(pMyObject,ANSC_ASN1_SET_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetInitialize
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
AnscAsn1SetInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1SetClone;
    pMyObject->CopyTo               = AnscAsn1SetCopyTo;
    pMyObject->EqualsTo             = AnscAsn1SetEqualsTo;
    pMyObject->DecodingData         = AnscAsn1SetDecodingData;
    pMyObject->EncodingData         = AnscAsn1SetEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1SetGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1SetDumpObject;
    pMyObject->ReadyToEncode        = AnscAsn1SetReadyToEncode;

    pMyObject->CreateChildAttr      = AnscAsn1SetCreateChildAttr;
    pMyObject->EnumChildObject      = AnscAsn1SetEnumChildObject;
    pMyObject->GetChildByIndex      = AnscAsn1SetGetChildByIndex;
    pMyObject->GetChildName         = AnscAsn1SetGetChildName;
    pMyObject->SetChildByIndex      = AnscAsn1SetSetChildByIndex;
    pMyObject->GetChildCount        = AnscAsn1SetGetChildCount;
    pMyObject->CreateChildObject    = AnscAsn1SetCreateChildObject;
    pMyObject->CreateAllChildren    = AnscAsn1SetCreateAllChildren;
    pMyObject->RemoveAllChildren    = AnscAsn1SetRemoveAllChildren;

    pMyObject->uTotalChild          = 0;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetFree
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
AnscAsn1SetFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;

    if( pMyObject != NULL)
    {
        /*
         *  Remove the children here;
         */
        pMyObject->RemoveAllChildren(pMyObject, TRUE);

    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1SetClone
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
AnscAsn1SetClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_SET                  pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_SET)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1SetClone()\n");

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
        AnscAsn1SetCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies value to the other one;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1SetCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_SET                  pNewObject   = (PANSC_ASN1_SET)hDestObject;
    PANSC_ASN1_OBJECT               pChild;
    PANSC_ASN1_OBJECT               pNewChild;
    ULONG                           i;

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
    pNewObject->uTotalChild         = pMyObject->uTotalChild;

    if( !pMyObject->bOptional)
    {
        for( i = 0; i < pMyObject->uTotalChild; i ++)
        {
            pChild      = (PANSC_ASN1_OBJECT)pMyObject->GetChildByIndex( pMyObject, i);
            pNewChild   = (PANSC_ASN1_OBJECT)pNewObject->GetChildByIndex( pNewObject, i);

            if( pChild != NULL)
            {
                if( !pChild->CopyTo(pChild, pNewChild))
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SetEqualsTo
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
                Check  the value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1SetEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_SET                  pMyObject  = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_SET                  pNewObject = (PANSC_ASN1_SET)hOtherObject;
    ULONG                           i;
    PANSC_ASN1_OBJECT               pChild;
    PANSC_ASN1_OBJECT               pNewChild;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1SetEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1SetEqualsTo().\n");

        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
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

    /* check the value */
    if(!pMyObject->bOptional)
    {
        if( pMyObject->uTotalChild  != pNewObject->uTotalChild)
        {
            return FALSE;
        }

        for( i = 0; i < pMyObject->uTotalChild; i ++)
        {
            pChild      = (PANSC_ASN1_OBJECT)pMyObject->GetChildByIndex( pMyObject, i);
            pNewChild   = (PANSC_ASN1_OBJECT)pNewObject->GetChildByIndex( pNewObject, i);

            if( pChild == NULL)
            {
                if( pChild != pNewChild)
                {                    
                    return FALSE;
                }
            }

            if( !pChild->EqualsTo(pChild, pNewChild, FALSE))
            {                
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1SetGetSizeOfEncoded
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
AnscAsn1SetGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    ULONG                           ulSize       = 0;
    ULONG                           i            = 0;
    LONG                            lChildLen    = 0;
    PANSC_ASN1_OBJECT               pChild;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the 'set' object is not ready to encode.\n");

        return -1;
    }

    /***********************************************************
         CALCULATE ALL THE CHILDREN
    ***********************************************************/
    for( i = 0; i < pMyObject->uTotalChild; i ++)
    {
        lChildLen       = 0;
        pChild          = pMyObject->GetChildByIndex(pMyObject, i);
     
        if( pChild == NULL)
        {
            AnscTrace
                (
                    "The '%s' child of the SEQUENCE is not well-configured, not ready to encode.\n", 
                    pMyObject->Name
                );

            return -1;
        }

        lChildLen   = pChild->GetSizeOfEncoded(pChild);

        if( lChildLen < 0)
        {
            AnscTrace("The child '%lu' is not ready to encode.\n", (i+1));

            return -1;
        }

        ulSize          += lChildLen;
    }

    /*
     *  check the attribute list, from the end of the list;
     *  the tag length value is 1.
     */ 
    ulSize = AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);

#if 0
    AnscTrace
        (
            "The encoded length of set '%s' is - %d.\n", 
            pMyObject->Name,
            ulSize
        );
#endif

    return ulSize;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetDecodingData
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
static
LONG
findComingChildEncoding
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEncoding
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ATTR_OBJECT               pComingAttr;
    PANSC_ASN1_OBJECT               pChild;
    ASN_OBJECT_FORM_TYPE            formType;
    ULONG                           uTagValue;
    ULONG                           i;

    if( hThisObject == NULL || pEncoding == NULL)
    {
        return -1;
    }

    /*
     * Create the coming attribute;
     */
    pComingAttr = (PANSC_ATTR_OBJECT)AnscAsn1AttrParsingData(pEncoding);

    /*RDKB-6199, CID-24299; null check and return*/
    if( pComingAttr == NULL)
    {
        AnscTrace("Failed to parse the binary data to a PANSC_ATTR_OBJECT object.\n");
        return -1;
    }
    /*
     *  get the child attribute;
     */
    for( i = 0; i < pMyObject->uTotalChild; i ++)
    {
        pChild = pMyObject->GetChildByIndex( pMyObject, i);

        pChild->GetEncodeTagValue
            (
                pChild,
                &formType,
                &uTagValue
            );
        
        if( formType  == pComingAttr->FormType &&
            uTagValue == pComingAttr->ulClassNumber)
        {
            pComingAttr->Remove(pComingAttr);

            return i;
        }
    }

    pComingAttr->Remove(pComingAttr);

    AnscTrace("Failed to find the correct child in SET to decode.\n");

    return -1;
}

ANSC_STATUS
AnscAsn1SetDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    PUCHAR                          pEndBuffer   = NULL;
    PUCHAR                          pBackBuffer  = NULL;
    BOOLEAN                         bLastInfinite= FALSE;
    LONG                            actLength,i, lChildIndex;
    PANSC_ASN1_OBJECT               pChild;

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

    if( bTagIncluded)
    {
        pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

        if( pTagInfo == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        /* parsing the data */
        pEncodingUnit   = (PUCHAR)*ppEncoding;
        returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);
        
        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return returnStatus;
        }

        /* check the tag matched or not */
        if( pTagInfo->FirstOctetOfTag
                != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject))
        {
            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_SET - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->GetSizeOfTag(pTagInfo) != 1)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
            AnscTrace("The length of SET value is unacceptable.\n");

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }
        else if( pTagInfo->bIsInfinite)
        {
            uLeftSize       = 0;
            uInfiniteTags   ++;
            bLastInfinite   = TRUE;
        }
        else
        {
            uLeftSize       = pTagInfo->ulContentLength;
            bLastInfinite   = FALSE;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);

        /*
         *  free the tag info;
         */
        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
     actLength    = uLeftSize;

    if( actLength == 0 && bLastInfinite > 0)
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
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1SequenceDecoding().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        actLength = pEndBuffer - pEncodingUnit;
    }

    /*
     *  first set all the children to optional;
     */
    for( i = 0; i < (LONG)pMyObject->uTotalChild; i ++)
    {
        pChild = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL)
        {
            pChild->bOptional = TRUE;
        }
    }

    /*
     *  decode the child one by one;
     */
    while( actLength > 0)
    {
        pBackBuffer = pEncodingUnit;

        lChildIndex = 
            findComingChildEncoding
                (
                    pMyObject,
                    pEncodingUnit
                );

        if( lChildIndex < 0)
        {
            /*
            AnscTrace
                (
                    "Unexpected tag in SET '%s' decoding: 0x%.2x %.2x... \n",
                    pMyObject->Name,
                    pEncodingUnit[0],
                    pEncodingUnit[1]
                );
            */    
            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        pChild = pMyObject->GetChildByIndex(pMyObject,lChildIndex);

        returnStatus = 
            pChild->DecodingData
                (
                    pChild,
                    (PVOID*)&pEncodingUnit
                );

        if( ANSC_STATUS_SUCCESS != returnStatus  )
        {
            return returnStatus;
        }

        /*
         *  recalculate the size;
         */
        actLength -= (pEncodingUnit - pBackBuffer);

        if( actLength < 0)
        {
            AnscTrace
                (
                    "Invalid length after decode child '%lu' of object '%s'\n", 
                    i+1,
                    pMyObject->ClassName
                );

            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }
    }

    /*
     *  check the length;
     */
    if( actLength != 0)
    {
        AnscTrace
            (
                "After decoding all the child objects of '%s', the length is not 0 but '%lu'\n", 
                pMyObject->ClassName,
                actLength
            );

        return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    /*
     *  check the children one by one
     */
    for( i = 0; i < (LONG)pMyObject->uTotalChild; i ++)
    {
        pChild = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL && pChild->bOptional)
        {
            if( !pChild->bCanBeOptional)
            {
                AnscTrace("The child '%lu' of SET object cannot be optional.\n", (i+1));

                return ANSC_STATUS_FAILURE;
            }
        }
    }

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
     *  check the status before exit;
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        /*
         *  check after decoding;
         */
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject, (PVOID*)&pEncodingUnit);

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
        AnscAsn1SetEncodingData
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
AnscAsn1SetEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_SET                  pMyObject       = (PANSC_ASN1_SET)hThisObject;
    BOOLEAN                         bTagIncluded    = TRUE;
    PANSC_ASN1_OBJECT               pChild;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;
    LONG                            i;
    int 			    iEncodedLength = 0;

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
     *  check before Enecoding;
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
        AnscTrace("Failed to encode the attrlist of BOOL objects.\n");

        return returnStatus;
    }

    /*
     *  Encode all the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData                  = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        /*CID: 68051 Improper use of negative value*/
	iEncodedLength = GetPureEncodedLength( uLeftSize - 1);
	if ( iEncodedLength < 0 )
	{
		return ANSC_ASN1_NOT_READY_TO_ENCODE;
	}
        returnStatus = 
            EncodeLength
                (
                    (PVOID*)&pCharData, 
                    (ULONG)iEncodedLength
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            return returnStatus;
        }
    }

    /*
     *  Encode all the children here;
     */
    for( i = 0; i < (LONG)pMyObject->uTotalChild; i ++)
    {
        pChild  = pMyObject->GetChildByIndex( pMyObject, i);

        /*
         *  Encode the child object;
         */
        returnStatus = 
            pChild->EncodingData
                (
                    pChild,
                    (PVOID*)&pCharData
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            return returnStatus;
        }
    }

    /*
     *  Set the buffer before exit;
     */
    if( pCharData - pBackData != uSizeOfEncoded)
    {
        /*
        AnscTrace
            (
                "Warning in Encoding SET/SEQUENCE '%s', the size of encoding is expected to be %d, but it's %d.\n",
                pMyObject->Name,
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
        returnStatus = pMyObject->AfterEncoding(pMyObject, (PVOID*)&pCharData);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    *ppEncoding                     = (PVOID)pCharData;

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SetDumpObject
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
                The  object is the root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1SetDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SET                  pMyObject       = (PANSC_ASN1_SET)hThisObject;
    CHAR                            pAttrBuffer[512]= { 0 };
    ULONG                           attrLength      = 512;
    PCHAR                           pName;
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

    rc =
        sprintf_s
            (
                pBuffer,
                *pLength,
                "%s ::=%s %s%s",
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType),
                ((pMyObject->bOptional) ? " (Optional)" : "")
            );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }
    *pLength = rc;

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SetTraceObject
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
AnscAsn1SetTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SET                  pMyObject           = (PANSC_ASN1_SET)hThisObject;
    CHAR                            pAttrBuffer[512]    = { 0 };
    ULONG                           attrLength          = 512;
    LONG                            i;
    PANSC_ASN1_OBJECT               pChild;
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

    if( bRecursive)
    {
        for( i = 0; i < (LONG)pMyObject->uTotalChild; i ++)
        {
            pChild  = pMyObject->GetChildByIndex( pMyObject, i);

            if( pChild != NULL)
            {
                /* pChild->TraceObject(pChild, layer + 1, bShowValue, TRUE); */
            }
        }
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SetReadyToEncode
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
AnscAsn1SetReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    ULONG                           i;

    /*
     *  if it's optional, return true;
     */
    if( pMyObject->bOptional)
    {
        return TRUE;
    }

    /*
     * Check the validity of the value
     */
    for( i = 0 ; i < pMyObject->uTotalChild; i ++)
    {
        pChild = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild == NULL)
        {
            return FALSE;
        }
        else
        {
            if(!pChild->ReadyToEncode(pChild))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SetEnumChildObject
            (
                ANSC_HANDLE                 hThisObject,
                PFN_ASN1_ENUM_CHILD_PROC    hProc,
                PVOID                       pContext
            );

    description:

        This function enumerate all the child objects in the SET object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PFN_ASN1_ENUM_CHILD_PROC    hProc,
                The enum child proc;

                PVOID                       pContext
                The parameter of above proc;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1SetEnumChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    )
{
    PANSC_ASN1_SET                  pThisObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;

    if( hProc == NULL || hThisObject == NULL)
    {
        return FALSE;
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pThisObject->sChildList);

    while ( pSLinkEntry )
    {
        pChild             = (PANSC_ASN1_OBJECT)ACCESS_ANSC_ASN1_OBJECT(pSLinkEntry);

        if( pChild != NULL)
        {
            if( !hProc( pChild, pContext))
            {
                return FALSE;
            }
        }

        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);
    }

    return TRUE;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PANSC_ATTR_OBJECT
        AnscAsn1SetCreateChildAttr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        This function create the attr object for the child
        object with given index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                The index of the child object.

    return:     the attr object.

**********************************************************************/
PANSC_ATTR_OBJECT
AnscAsn1SetCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(index);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PCHAR
        AnscAsn1SetGetChildName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        This function get the name of the child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                The index of the child object.

    return:     the name of the object.

**********************************************************************/
PCHAR
AnscAsn1SetGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(index);
    AnscTrace("Please implement it in the derived object.\n");

    return "child";
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SetGetChildByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        This function get the child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                The index of the child object.

    return:     the child object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1SetGetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry;

    pSListEntry = 
        AnscSListGetEntryByIndex
            (
                &pMyObject->sChildList,
                index
            );

    if( pSListEntry == NULL)
    {
        return NULL;
    }

    return ACCESS_ANSC_ASN1_OBJECT(pSListEntry);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetSetChildByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChild,
                ULONG                       index,
                BOOLEAN                     bFreeOldOne
            );

    description:

        This function get the child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChild,
                The new input object.

                ULONG                       index
                The index of the child object.

                BOOLEAN                     bFreeOldOne
                Free the old one or not

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1SetSetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild,
        ULONG                       index,
        BOOLEAN                     bFreeOldOne
    )
{
    PANSC_ASN1_SET                  pMyObject    = NULL;
    PANSC_ASN1_OBJECT               pNewOne      = NULL;
    PSINGLE_LINK_ENTRY              pPrevEntry   = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PANSC_ASN1_OBJECT               pOldOne = NULL;
    PSLIST_HEADER                   pListHeader = NULL;

    if( hChild == NULL || hThisObject == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    pMyObject    = (PANSC_ASN1_SET)hThisObject;
    pNewOne = (PANSC_ASN1_OBJECT)hChild;

    pListHeader = &pMyObject->sChildList;
    pSListEntry = 
        AnscSListGetEntryByIndex
            (
                pListHeader,
                index
            );

    if( pSListEntry == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    if( index > 0)
    {
        pPrevEntry = 
            AnscSListGetEntryByIndex
                (
                    pListHeader,
                    index - 1
                );
    }

    pOldOne = (PANSC_ASN1_OBJECT)ACCESS_ANSC_ASN1_OBJECT(pSListEntry);

    if( pOldOne == pNewOne)
    {
        return ANSC_ASN1_ALREADY_EXIST;
    }

    /*
     *  set the new one;
     */
    /*RDKB-6199, CID-24270, null check before use*/
    if(pOldOne)
    {
        pNewOne->bCanBeOptional = pOldOne->bCanBeOptional;
    }
    pNewOne->hOwnerContext = (ANSC_HANDLE)pMyObject;
    pNewOne->SetName( pNewOne, pMyObject->GetChildName(pMyObject, index));
    pNewOne->AddAttribute
                ( 
                    pNewOne,
                    pMyObject->CreateChildAttr(pMyObject, index),
                    FALSE
                );

    if( pPrevEntry != NULL)
    {
        pPrevEntry->Next = &pNewOne->Linkage;
    }
    else
    {
        pListHeader->Next.Next = &pNewOne->Linkage;
    }

    pNewOne->Linkage.Next = pSListEntry->Next;

    /*
     *  If required, remove the old one;
     */
    if( pOldOne!= NULL && bFreeOldOne)
    {
        pOldOne->AsnFree(pOldOne);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAsn1SetGetChildCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function return the child amount of this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the amount of the child objects.

**********************************************************************/
ULONG
AnscAsn1SetGetChildCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;

    return pMyObject->uTotalChild;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscAsn1SetCreateAllChildren
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        this function creates all the child objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     nothing;

**********************************************************************/
void
AnscAsn1SetCreateAllChildren
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PANSC_ASN1_OBJECT               pChildObject;
    ULONG                           i;

    if( pMyObject == NULL)
    {
        return;
    }

    for( i = 0; i < pMyObject->uTotalChild; i ++)
    {
        pChildObject = pMyObject->CreateChildObject( pMyObject, i);

        if( pChildObject == NULL)
        {
            pChildObject = 
                AnscAsn1AnyCreate
                    ( 
                        pMyObject->hContainerContext
                    );
        }

        if( pChildObject != NULL)
        {
            pChildObject->hOwnerContext = (ANSC_HANDLE)pMyObject;
            AnscSListPushEntryAtBack(&pMyObject->sChildList, &pChildObject->Linkage);
        }
    }
}

/**********************************************************************

    NOTE:       THE DERIVED CLASS MUST REWRITE THIS FUNCTION.

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SetCreateChildObject
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        this function creates the child object with the given index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                The index of the child object.

    return:     the created child object;

**********************************************************************/
ANSC_HANDLE
AnscAsn1SetCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(index);
    AnscTrace("Please implement it in the derived object.\n");

    return NULL;
}


ANSC_STATUS
AnscAsn1SetRemoveAllChildren
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    )
{
    PANSC_ASN1_SET                  pMyObject    = (PANSC_ASN1_SET)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry;
    PANSC_ASN1_OBJECT               pChild;

    pSListEntry =   AnscSListGetFirstEntry(&pMyObject->sChildList);

    while( pSListEntry != NULL)
    {
        pChild      = ACCESS_ANSC_ASN1_OBJECT(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        if( pChild != NULL && bFreeMemory)
        {
            pChild->AsnFree(pChild);
        }              
    }

    /*
     *  reset the SList;
     */
    AnscSListInitializeHeader(&pMyObject->sChildList);

    return ANSC_STATUS_SUCCESS;
}
