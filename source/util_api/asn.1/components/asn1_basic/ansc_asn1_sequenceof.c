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

    module: ANSC_ASN1_SEQUENCEOFof.c

        For ASN.1 SEQUENCEOF object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_SEQUENCEOF"

        *   AnscAsn1SequenceOfCreate
        *   AnscAsn1SequenceOfFree
        *   AnscAsn1SequenceOfInitialize
        *   AnscAsn1SequenceOfClone
        *   AnscAsn1SequenceOfCopyTo
        *   AnscAsn1SequenceOfEqualsTo
        *   AnscAsn1SequenceOfGetSizeOfEncoded
        *   AnscAsn1SequenceOfDecodingData
        *   AnscAsn1SequenceOfEncodingData
        *   AnscAsn1SequenceOfDumpObject
        *   AnscAsn1SequenceOfTraceObject
        *   AnscAsn1SequenceOfReadyToEncode

        *   AnscAsn1SequenceOfEnumChildObject
        *   AnscAsn1SequenceOfGetChildByIndex
        *   AnscAsn1SequenceOfGetChildCount
        *   AnscAsn1SequenceOfRemoveAllChildren
        *   AnscAsn1SequenceOfRemoveChildByIndex
        *   AnscAsn1SequenceOfAddChild
        *   AnscAsn1SequenceOfIsChildValid
        *   AnscAsn1SequenceOfCreateChild

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
        AnscAsn1SequenceOfCreate
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
AnscAsn1SequenceOfCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( (ULONG)hContainerContext > 0)
    {
        pMyObject = (PANSC_ASN1_SEQUENCEOF)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_SEQUENCEOF)AnscAllocateMemory(sizeof(ANSC_ASN1_SEQUENCEOF));
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
    pMyObject->Oid                 = ANSC_ASN1_SEQUENCEOF_ID;
    pMyObject->uType               = ASN1_SEQUENCE_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1SequenceOfCreate;
    pMyObject->AsnFree             = AnscAsn1SequenceOfFree;
    
    AnscAsn1SequenceOfInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_SEQUENCEOF_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_SEQUENCEOF_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfFree
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
AnscAsn1SequenceOfFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;

    if( hThisObject != NULL)
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

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfInitialize
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
AnscAsn1SequenceOfInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1SequenceOfClone;
    pMyObject->CopyTo               = AnscAsn1SequenceOfCopyTo;
    pMyObject->EqualsTo             = AnscAsn1SequenceOfEqualsTo;
    pMyObject->DecodingData         = AnscAsn1SequenceOfDecodingData;
    pMyObject->EncodingData         = AnscAsn1SequenceOfEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1SequenceOfGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1SequenceOfDumpObject;
    pMyObject->ReadyToEncode        = AnscAsn1SequenceOfReadyToEncode;

    pMyObject->EnumChildObject      = AnscAsn1SequenceOfEnumChildObject;
    pMyObject->GetChildByIndex      = AnscAsn1SequenceOfGetChildByIndex;
    pMyObject->GetChildCount        = AnscAsn1SequenceOfGetChildCount;
    pMyObject->RemoveAllChildren    = AnscAsn1SequenceOfRemoveAllChildren;
    pMyObject->RemoveChildByIndex   = AnscAsn1SequenceOfRemoveChildByIndex;
    pMyObject->AddChild             = AnscAsn1SequenceOfAddChild;
    pMyObject->CreateChild          = AnscAsn1SequenceOfCreateChild;
    pMyObject->IsChildValid         = AnscAsn1SequenceOfIsChildValid;

    AnscSListInitializeHeader(&pMyObject->sChildList);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceOfClone
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
AnscAsn1SequenceOfClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_ASN1_SEQUENCEOF           pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_SEQUENCEOF)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1SequenceOfClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList); */
    pNewObject->SetName(pNewObject, pMyObject->Name);

    if(!pMyObject->CopyTo(pMyObject, pNewObject))
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
        AnscAsn1SequenceOfCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1SequenceOfCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_ASN1_SEQUENCEOF           pNewObject   = (PANSC_ASN1_SEQUENCEOF)hDestObject;
    PANSC_ASN1_OBJECT               pChild;
    PANSC_HANDLE                    pNewChild;
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
    if( !pMyObject->bOptional)
    {
        for( i = 0; i < pMyObject->GetChildCount(pMyObject); i ++)
        {
            pChild      = (PANSC_ASN1_OBJECT)pMyObject->GetChildByIndex( pMyObject, i);

            if( pChild != NULL)
            {
                pNewChild   = pChild->Clone(pChild);

                if( pNewChild != NULL)
                {
                    if( ANSC_STATUS_SUCCESS != pNewObject->AddChild(pNewObject, pNewChild))
                    {
                        AnscTrace("The added child must be of the same type.\n");
                        
                        ((PANSC_ASN1_OBJECT)pNewChild)->AsnFree(pNewChild);
                    }
                }
                else
                {
                    AnscTrace("Failed to clone the child object in SETOF.\n");

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
        AnscAsn1SequenceOfEqualsTo
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
                Check the value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1SequenceOfEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject  = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_ASN1_SEQUENCEOF           pNewObject = (PANSC_ASN1_SEQUENCEOF)hOtherObject;
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
        AnscTrace("One of the parameter in AnscAsn1SequenceOfEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1SequenceOfEqualsTo().\n");

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
        if( pMyObject->GetChildCount(pMyObject) != pNewObject->GetChildCount(pNewObject))
        {
            return FALSE;
        }

        for( i = 0; i < pMyObject->GetChildCount(pMyObject); i ++)
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
        AnscAsn1SequenceOfGetSizeOfEncoded
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
AnscAsn1SequenceOfGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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
    for( i = 0; i < pMyObject->GetChildCount(pMyObject); i ++)
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
            "The encoded length of sequenceof '%s' is - %d.\n", 
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
        AnscAsn1SequenceOfDecodingData
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
AnscAsn1SequenceOfDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    PUCHAR                          pEndBuffer   = NULL;
    PUCHAR                          pBackBuffer  = NULL;
    LONG                            actLength;
    PANSC_ASN1_OBJECT               pChild;
    BOOLEAN                         bLastInfinite= FALSE;

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
            AnscTrace
                (
                    "Unexpected tag in ASN1_SETOF - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->GetSizeOfTag(pTagInfo) != 1)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
            AnscTrace("The length of SET value is unacceptable.\n");

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }
        if( pTagInfo->bIsInfinite)
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

    if( actLength == 0 && bLastInfinite)
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
     *  remove all the child first;
     */
    pMyObject->RemoveAllChildren(pMyObject, TRUE);

    /*
     *  decode the child one by one;
     */
    while( actLength > 0)
    {
        /* back up the position */
        pBackBuffer = pEncodingUnit;

        pChild = pMyObject->CreateChild(pMyObject, FALSE);

        if( pChild == NULL)
        {
            AnscTrace("Failed to create child object in '%s'.\n", pMyObject->ClassName);

            return ANSC_ASN1_NULL_OBJCET;
        }

        pChild->hOwnerContext = pMyObject;

        returnStatus = 
            pChild->DecodingData
                (
                    pChild,
                    (PVOID*)&pEncodingUnit
                );

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            pChild->AsnFree(pChild);

            AnscTrace("Failed to decode the child of '%s' object.\n",pMyObject->ClassName);

            return returnStatus;
        }

        /* add this child */
        if( pMyObject->AddChild(pMyObject, pChild) != ANSC_STATUS_SUCCESS)
        {
            pChild->AsnFree(pChild);

            AnscTrace
                (
                    "The added child is in unacceptable type in '%s' object.\n",
                    pMyObject->ClassName
                );

            return ANSC_ASN1_INVALID_TYPE_IN_SEQOF_OR_SETOF;
        }

        /*
         *  recalculate the size;
         */
        actLength -= (pEncodingUnit - pBackBuffer);

        if( actLength < 0)
        {
            AnscTrace
                (
                    "Invalid length after decoding child object of '%s' - %lu\n", 
                    pMyObject->ClassName,
                    actLength
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
        AnscAsn1SequenceOfEncodingData
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
AnscAsn1SequenceOfEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_SEQUENCEOF           pMyObject       = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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

        /*CID:70317 Improper use of negative value*/
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
    for( i = 0; i < (LONG)pMyObject->GetChildCount(pMyObject); i ++)
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
                "Warning in Encoding SETOF object, the size of encoding \
                is expected to be %d, but it's %d.\n",
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
        AnscAsn1SequenceOfDumpObject
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
                The object is the root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1SequenceOfDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SEQUENCEOF           pMyObject       = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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

    if( pMyObject->bOptional)
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
    else
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

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SequenceOfTraceObject
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
AnscAsn1SequenceOfTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    CHAR                            pAttrBuffer[512] = { 0 };
    ULONG                           attrLength       = 512;
    LONG                            i;
    PANSC_ASN1_OBJECT               pChild;
    PCHAR                           pName;

    TraceTabs(layer);
    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

    if( layer == 0)
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
        for( i = 0; i < (LONG)pMyObject->GetChildCount(pMyObject); i ++)
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
        AnscAsn1SequenceOfReadyToEncode
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
AnscAsn1SequenceOfReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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
     * check every child is ready or not;
     */
    for( i = 0; i < pMyObject->GetChildCount(pMyObject); i ++)
    {
        pChild = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL)
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
        AnscAsn1SequenceOfEnumChildObject
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
AnscAsn1SequenceOfEnumChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    )
{
    PANSC_ASN1_SEQUENCEOF           pThisObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

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

        ANSC_HANDLE
        AnscAsn1SequenceOfGetChildByIndex
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
AnscAsn1SequenceOfGetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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

        ULONG
        AnscAsn1SequenceOfGetChildCount
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
AnscAsn1SequenceOfGetChildCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;

    return AnscSListQueryDepth(&pMyObject->sChildList);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfRemoveAllChildren
            (
                ANSC_HANDLE                 hThisObject,
                BOOLEAN                     bFreeMemory
            );

    description:

        This function removes all the children objects

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOLEAN                     bFreeMemory
                Free the child objects or not;

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1SequenceOfRemoveAllChildren
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
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

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfAddChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChild
            );

    description:

        This function add a new child

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChild
                The child object.

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1SequenceOfAddChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_OBJECT               pChild       = (PANSC_ASN1_OBJECT)hChild;


    returnStatus = pMyObject->IsChildValid(pMyObject, hChild);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    pChild->hOwnerContext = hThisObject;
    
    /* make the chars to lower case */
    AnscStringFromUToL
        (
            pChild->Name,
            pChild->Name,
            AnscSizeOfString(pChild->Name)
        );

    AnscSListPushEntryAtBack(&pMyObject->sChildList, &pChild->Linkage);

    /* don't forget to set optional as FALSE */
    pMyObject->bOptional = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfIsChildValid
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChild
            );

    description:

        This function checks the given child is valid or not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChild
                The child object.

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1SequenceOfIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hChild);
    AnscTrace("Please implement it in your derived class.\n");

    return ANSC_ASN1_FORBIDDEN_PLACE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceOfRemoveChildByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index,
                BOOLEAN                     bFreeMemory
            )

    description:

        This function removes the child with the index;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                The index of the child object.

                BOOLEAN                     bFreeMemory
                Free the memory or not;

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1SequenceOfRemoveChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index,
        BOOLEAN                     bFreeMemory
    )
{
    PANSC_ASN1_SEQUENCEOF           pMyObject    = (PANSC_ASN1_SEQUENCEOF)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    PSINGLE_LINK_ENTRY              pSListEntry;

    pSListEntry = AnscSListSearchEntryByIndex(&pMyObject->sChildList, index);

    if( pSListEntry == NULL)
    {
        return ANSC_ASN1_NOT_EXIST;
    }

    pChild      = ACCESS_ANSC_ASN1_OBJECT(pSListEntry);

    AnscSListPopEntryByLink(&pMyObject->sChildList, pSListEntry);

    if( pChild != NULL && bFreeMemory)
    {
        pChild->AsnFree(pChild);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceCreateChild
            (
                ANSC_HANDLE                 hThisObject,
                BOOLEAN                     bAddItIn
            )

    description:

        This function creates a new child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOLEAN                     bAddItIn
                Add this child to the list or not.

    return:     the status of the operation.

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceOfCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(bAddItIn);
    AnscTrace("Please implement it in your derived class.\n");

    return NULL;
}

