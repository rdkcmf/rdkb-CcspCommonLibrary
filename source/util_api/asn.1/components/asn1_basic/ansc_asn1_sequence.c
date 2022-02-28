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

    module: ansc_asn1_sequence.c

        For ASN.1 SEQUENCE object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_SEQUENCE"

        *   AnscAsn1SequenceCreate
        *   AnscAsn1SequenceFree
        *   AnscAsn1SequenceInitialize
        *   AnscAsn1SequenceClone
        *   AnscAsn1SequenceCopyTo
        *   AnscAsn1SequenceDecodingData
        *   AnscAsn1SequenceEncodingData
        *   AnscAsn1SequenceReadyToEncode
        *   AnscAsn1SequenceGetSizeOfEncoded
        *   AnscAsn1SequenceTraceObject
        *   AnscAsn1SequenceDumpObject

        *   AnscAsn1SequenceGetExtraChild
        *   AnscAsn1SequenceSetExtraChild
        *   AnscAsn1SequenceGetFirstOIDChild
        *   AnscAsn1SequenceGetLastOIDChild
        *   AnscAsn1SequenceGetFirstIntegerChild
        *   AnscAsn1SequenceGetLastStringChild
        *   AnscAsn1SequenceGetExtraChildName
        *   AnscAsn1SequenceCreateExtraChild

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/25/02  initial revision.
        06/27/02  GetFirstIntegerChild was added for SEQUENCE object

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceCreate
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
AnscAsn1SequenceCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( (ULONG)hContainerContext > 0)
    {
        pMyObject = (PANSC_ASN1_SEQUENCE)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_SEQUENCE)AnscAllocateMemory(sizeof(ANSC_ASN1_SEQUENCE));
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
    pMyObject->Oid                 = ANSC_ASN1_SEQUENCE_ID;
    pMyObject->uType               = ASN1_SEQUENCE_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1SequenceCreate;
    pMyObject->AsnFree             = AnscAsn1SequenceFree;
    
    AnscAsn1SequenceInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    /*CID: 125331 Explicit null dereferenced*/
    if (pMyObject->SetName)
        pMyObject->SetName(pMyObject,ANSC_ASN1_SEQUENCE_NAME);
    if (pMyObject->SetClassName)
        pMyObject->SetClassName(pMyObject,ANSC_ASN1_SEQUENCE_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceInitialize
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
AnscAsn1SequenceInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1SetInitialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1SequenceClone;
    pMyObject->CopyTo               = AnscAsn1SequenceCopyTo;
    pMyObject->DecodingData         = AnscAsn1SequenceDecodingData;
    pMyObject->EncodingData         = AnscAsn1SequenceEncodingData;
    pMyObject->ReadyToEncode        = AnscAsn1SequenceReadyToEncode;
    pMyObject->GetSizeOfEncoded     = AnscAsn1SequenceGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1SequenceDumpObject;

    pMyObject->GetExtraChild        = AnscAsn1SequenceGetExtraChild;
    pMyObject->SetExtraChild        = AnscAsn1SequenceSetExtraChild;
    pMyObject->GetFirstOIDChild     = AnscAsn1SequenceGetFirstOIDChild;
    pMyObject->GetLastOIDChild      = AnscAsn1SequenceGetLastOIDChild;
    pMyObject->GetFirstIntegerChild = AnscAsn1SequenceGetFirstIntegerChild;
    pMyObject->GetLastStringChild   = AnscAsn1SequenceGetLastStringChild;
    pMyObject->GetExtraChildName    = AnscAsn1SequenceGetExtraChildName;
    pMyObject->CreateExtraChild     = AnscAsn1SequenceCreateExtraChild;

    pMyObject->BeforeDecodingChild  = NULL;
    pMyObject->BeforeEncodingChild  = NULL;
    pMyObject->AfterDecodingChild   = NULL;
    pMyObject->AfterEncodingChild   = NULL;
    pMyObject->bDecodingExtra       = TRUE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceFree
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
AnscAsn1SequenceFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;

    if( pMyObject != NULL)
    {
        /*
         *  Remove the children here, from the end;
         */
        pMyObject->RemoveAllChildren(pMyObject,TRUE);

        /*
         *  Remove the extra child;
         */
        pChild = pMyObject->pExtraChild;

        if( pChild != NULL)
        {
            pChild->AsnFree(pChild);
        }
    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceClone
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
AnscAsn1SequenceClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_SEQUENCE             pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_SEQUENCE)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1SequenceClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
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
        AnscAsn1SequenceCopyTo
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
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1SequenceCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_SEQUENCE             pNewObject   = (PANSC_ASN1_SEQUENCE)hDestObject;
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
            pNewChild   = (PANSC_ASN1_OBJECT)pNewObject->GetChildByIndex(pNewObject, i);

            if( pChild != NULL)
            {               
                if(!pChild->CopyTo(pChild, pNewChild))
                {
                    return FALSE;
                }
            }
        }
    }

    /*
     *  If there's an extra child, copy it.
     */
    pChild = pMyObject->GetExtraChild(pMyObject);

    if( pChild != NULL)
    {
        pNewChild = pChild->Clone(pChild);

        if( pNewChild != NULL)
        {
            pNewObject->SetExtraChild(pNewObject, pNewChild, TRUE);
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SequenceEqualsTo
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
AnscAsn1SequenceEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject  = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_SEQUENCE             pNewObject = (PANSC_ASN1_SEQUENCE)hOtherObject;
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
        AnscTrace("One of the parameter in AnscAsn1SequenceEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1SequenceEqualsTo().\n");

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

    /* we don't need comparing the extra child here. */

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1SequenceGetSizeOfEncoded
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
AnscAsn1SequenceGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
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
        AnscTrace
            (
                "The value of the SEQUENCE object '%s' is not ready to encode.\n",
                pMyObject->Name
            );

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
            "The encoded length of sequence '%s' is - %d.\n", 
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
        AnscAsn1SequenceEncodingData
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
AnscAsn1SequenceEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_SEQUENCE             pMyObject       = (PANSC_ASN1_SEQUENCE)hThisObject;
    BOOLEAN                         bTagIncluded    = TRUE;
    PANSC_ASN1_OBJECT               pChild;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;
    LONG                            i;
    int				    iEncodedLength = 0;

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

        /*CID: 69114 Improper use of negative value*/
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

        if( pChild == NULL)
        {
            return ANSC_ASN1_NULL_OBJCET;
        }

        /*
         *  check before encoding child
         */
        if( pMyObject->BeforeEncodingChild != NULL)
        {
            returnStatus =
                pMyObject->BeforeEncodingChild
                    (
                        pMyObject,
                        i,
                        (PVOID*)&pCharData
                    );

            if( returnStatus != ANSC_STATUS_SUCCESS)
            {
                return returnStatus;
            }
        }

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

        /*
         *  check after encoding child
         */
        if( pMyObject->AfterEncodingChild != NULL)
        {
            returnStatus = 
                pMyObject->AfterEncodingChild
                    (
                        pMyObject,
                        i,
                        (PVOID*)&pCharData
                    );
            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
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

        ANSC_STATUS
        AnscAsn1SequenceDecodingData
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
AnscAsn1SequenceDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ANSC_STATUS                     tempStatus;
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    PUCHAR                          pEndBuffer   = NULL;
    PUCHAR                          pBackBuffer  = NULL;
    LONG                            actLength,i;
    PANSC_ASN1_STRING               pStringChild;
    PANSC_ASN1_OBJECT               pChild, extraChild;
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
        pTagInfo = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

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
            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_SEQUENCE '%s' - '0x%.2X', expected - '0x%.2X'\n",
                    pMyObject->Name,
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->GetSizeOfTag(pTagInfo) != 1)
        {           
            AnscTrace("The length of SEQUENCE value is unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

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
     *  decode the child one by one.
     */
    for( i = 0; i < (LONG)pMyObject->uTotalChild; i ++)
    {
        pChild = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild == NULL)
        {
            AnscTrace("Failed to get the child - '%lu' of SEQUENCE object.\n", i+1 );

            return ANSC_ASN1_NULL_OBJCET;           
        }

        /* back up the position */
        pBackBuffer = pEncodingUnit;

        if( actLength > 0)
        {
            /*
             * before decoding child
             */
            if( pMyObject->BeforeDecodingChild != NULL)
            {
                returnStatus = 
                    pMyObject->BeforeDecodingChild
                        (
                            pMyObject,
                            i,
                            (PVOID*)&pEncodingUnit
                        );

                if( ANSC_STATUS_SUCCESS != returnStatus)
                {
                    return returnStatus;
                }
            }


            /*
             *  decode the child;
             */
            returnStatus = 
                pChild->DecodingData
                    (
                        pChild,
                        (PVOID*)&pEncodingUnit
                    );

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                if(!pChild->bCanBeOptional)
                {
                    return returnStatus;
                }

                /*
                AnscTrace("Set the object '%s' as optional.\n", pChild->GetName(pChild));
                */

                pChild->bOptional = TRUE;

                returnStatus = ANSC_STATUS_SUCCESS;
            }

            /*
             * check after decoding child;
             */
            if( pMyObject->AfterDecodingChild != NULL)
            {
                returnStatus = 
                    pMyObject->AfterDecodingChild
                        (
                            pMyObject,
                            i,
                            (PVOID*)&pEncodingUnit
                        );

                if( ANSC_STATUS_SUCCESS != returnStatus)
                {
                    return returnStatus;
                }
            }

            /*
             *  recalculate the size;
             */
            actLength -= (pEncodingUnit - pBackBuffer);

            if( actLength < 0)
            {
                AnscTrace
                    (
                        "Invalid length after decode child '%lu' of object '%s' -- %lu.\n", 
                        i+1, 
                        pMyObject->ClassName,
                        actLength
                    );

                return ANSC_ASN1_INVALID_ENCODE_LENGTH;
            }
        }
        else /* if no more data available, check it can be optional or not */
        {
            if( !pChild->bCanBeOptional)
            {
                AnscTrace
                    (
                        "No enough data to be decoded for child '%lu' of object '%s'.\n", 
                        i+1,
                        pMyObject->ClassName
                    );

                return ANSC_ASN1_UNEXPECTED_TAG;
            }

            pChild->bOptional = TRUE;
        }
    }

    /*
     *  check the length;
     */
    if( actLength != 0)
    {
        AnscTrace
            (
                "After decoding all the child objects of '%s', the length is not '0' but '%lu'\n", 
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
     *  If there's an extra child, try to decode it;
     */
    extraChild  = pMyObject->pExtraChild;

    if(pMyObject->bDecodingExtra)
    {
        if( extraChild == NULL)
        {
            pMyObject->pExtraChild = 
                pMyObject->CreateExtraChild(pMyObject);

            extraChild  = pMyObject->pExtraChild;
        }

        if( extraChild != NULL)
        {
            extraChild->SetName( extraChild, pMyObject->GetExtraChildName(pMyObject));
            extraChild->hOwnerContext = (ANSC_HANDLE)pMyObject;

            pStringChild = (PANSC_ASN1_STRING)pMyObject->GetLastStringChild(pMyObject);

            if( pStringChild != NULL && !pStringChild->bOptional && pStringChild->uLength > 0)
            {
                pBackBuffer  = (PUCHAR)pStringChild->GetValueBuffer(pStringChild);

                tempStatus = 
                    extraChild->DecodingData
                        (
                            extraChild,
                            (PVOID*)&pBackBuffer
                        );

                if( ANSC_STATUS_SUCCESS != tempStatus)
                {
                    if( extraChild->ClassName)
                    {
                        AnscTrace
                            (
                                "Failed to decode the extra child of sequence object - '%s'\n",
                                extraChild->ClassName
                            );

                        TraceErrorMessage(tempStatus);
                    }
                }
            }
            else
            {
                /* empty string, new extra child needed. */
                extraChild->AsnFree(extraChild);

                pMyObject->pExtraChild = NULL;
            }
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

        BOOLEAN
        AnscAsn1SequenceReadyToEncode
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
AnscAsn1SequenceReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    PANSC_ASN1_OBJECT               pExtra       = (PANSC_ASN1_OBJECT)pMyObject->pExtraChild;
    PANSC_ASN1_STRING               pStringChild;
    PUCHAR                          pBackEncoding;
    LONG                            lSize;
    ULONG                           i;

    /*
     *  If it's optional, forget it
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
                if( pChild->Name)
                {
                    AnscTrace("The child '%s' is not readyToEncode.\n", pChild->Name);

                    return FALSE;
                }
            }
        }
    }

    /*
     *  check the extra child;
     */
    if( pExtra == NULL)
    {
        return TRUE;
    }

    /*
     *  Either STRING or BITSTRING object is fine.
     */
    pStringChild = (PANSC_ASN1_STRING)pMyObject->GetLastStringChild(pMyObject);

    if( pStringChild == NULL)
    {
        return TRUE;
    }

    /*
     *  if no data in it, encoding the extra child and set the value to the string
     *  object.
     */
    if( pStringChild->uLength < 2)
    {
        lSize  = pExtra->GetSizeOfEncoded(pExtra);

        if( lSize < 0)
        {
            return FALSE;
        }

        if( pStringChild->pStringBuffer != NULL)
        {
            AnscFreeMemory(pStringChild->pStringBuffer);
            pStringChild->bIsDynamic = FALSE;
        }

        pStringChild->pStringBuffer = (PUCHAR)AnscAllocateMemory(lSize);

        if( pStringChild->pStringBuffer == NULL)
        {
            return FALSE;
        }

        pStringChild->bIsDynamic = TRUE;
        pStringChild->uLength    = lSize;
        pStringChild->bOptional  = FALSE;
        pBackEncoding            = pStringChild->pStringBuffer;

        pExtra->EncodingData
                 (
                    pExtra,
                    (PVOID*)&pBackEncoding
                 );
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceGetExtraChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the extra child of the sequence object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The extra child;

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceGetExtraChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;

    return pMyObject->pExtraChild;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceCreateExtraChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the extra child of the sequence object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The created extra child;

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceCreateExtraChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SequenceSetExtraChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSpecialChild,
                BOOLEAN                     bFreeOldOne
            );

    description:

        This function set the extra child to the sequence object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSpecialChild,
                The extra child object;

                BOOLEAN                     bFreeOldOne
                Delete the previous one or not;

    return:     the status

**********************************************************************/
ANSC_STATUS
AnscAsn1SequenceSetExtraChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSpecialChild,
        BOOLEAN                     bFreeOldOne
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pExtra       = (PANSC_ASN1_OBJECT)pMyObject->pExtraChild;

    if( pExtra == hSpecialChild)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /*
     *  Remove the old one;
     */
    if( bFreeOldOne && pExtra)
    {
        pExtra->AsnFree(pExtra);
    }

    /*
     *  Set the new one;
     */
    pMyObject->pExtraChild   = hSpecialChild;

    if( hSpecialChild != NULL)
    {
        pExtra       = (PANSC_ASN1_OBJECT)pMyObject->pExtraChild;

        pExtra->SetName( pExtra, pMyObject->GetExtraChildName(pMyObject));
        pExtra->hOwnerContext = (ANSC_HANDLE)pMyObject;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PCHAR
        AnscAsn1SequenceGetExtraChildName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function get the name of the extra child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the name of the extra object.

**********************************************************************/
PCHAR
AnscAsn1SequenceGetExtraChildName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_SEQUENCE_EXTRA_CHILD_NAME;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceGetLastStringChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function get the last string child.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the last string child object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceGetLastStringChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    PANSC_ASN1_OBJECT               pSelection;
    LONG                            i;

    for( i = pMyObject->uTotalChild - 1; i >= 0; i --)
    {
        pChild  = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL)
        {
            if(IsObjectStringType(pChild->uType))
            {
                return pChild;
            }
            if(IsObjectChoiceType(pChild->uType))
            {
                pSelection = (PANSC_ASN1_OBJECT)
                    ((PANSC_ASN1_CHOICE)pChild)->hSelection;

                if( pSelection != NULL && IsObjectStringType(pSelection->uType))
                {
                    return pSelection;
                }
            }
        }
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceGetLastOIDChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function get the last OID child.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the last OID object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceGetLastOIDChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    LONG                            i;

    for( i = pMyObject->uTotalChild - 1; i >= 0; i --)
    {
        pChild  = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL && pChild->uType == ASN1_OID_TYPE)
        {
            return pChild;
        }
    }

    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceGetFirstIntegerChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function get the last integer child.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the first integer object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceGetFirstIntegerChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    LONG                            i;

    for( i = 0 ; i < (LONG)pMyObject->uTotalChild ; i ++)
    {
        pChild  = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL && pChild->uType == ASN1_INTEGER_TYPE)
        {
            return pChild;
        }
    }

    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1SequenceGetFirstOIDChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function get the first OID child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the first OID child

**********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceGetFirstOIDChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_SEQUENCE             pMyObject    = (PANSC_ASN1_SEQUENCE)hThisObject;
    PANSC_ASN1_OBJECT               pChild;
    ANSC_HANDLE                     hOIDHandle;
    LONG                            i;

    for( i = 0 ; i < (LONG)pMyObject->uTotalChild ; i ++)
    {
        pChild  = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL && pChild->uType == ASN1_OID_TYPE)
        {
            return pChild;
        }
    }

    /* 
     *  If we cannot find the OID child, we need to find it recursive
     *
     *  the ASN.1 Object "SubjectPublicKeyInfo" will be this case;
     */
    for( i = 0 ; i < (LONG)pMyObject->uTotalChild ; i ++)
    {
        pChild  = pMyObject->GetChildByIndex(pMyObject, i);

        if( pChild != NULL && pChild->uType == ASN1_SEQUENCE_TYPE)
        {
            hOIDHandle = ((PANSC_ASN1_SEQUENCE)pChild)->GetFirstOIDChild(pChild);

            if( hOIDHandle != NULL)
            {
                return hOIDHandle;
            }
        }
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SequenceTraceObject
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
AnscAsn1SequenceTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SEQUENCE             pMyObject           = (PANSC_ASN1_SEQUENCE)hThisObject;
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
                /* pChild->TraceObject(pChild, layer + 1, bShowValue, TRUE);*/
            }
        }

        /* trace the extra child */
        /*
        if( pMyObject->pExtraChild != NULL)
        {
            ((PANSC_ASN1_OBJECT)pMyObject->pExtraChild)->TraceObject
                (
                    pMyObject->pExtraChild,
                    layer + 1,
                    bShowValue,
                    TRUE
                );
        }
        */
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1SequenceDumpObject
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
AnscAsn1SequenceDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

#ifndef _PKI_KERNEL
    UNREFERENCED_PARAMETER(bShowValue);
    PANSC_ASN1_SEQUENCE             pMyObject       = (PANSC_ASN1_SEQUENCE)hThisObject;
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
#endif

    return  TRUE;
}
