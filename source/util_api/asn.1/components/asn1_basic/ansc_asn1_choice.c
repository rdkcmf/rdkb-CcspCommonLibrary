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

    module: ansc_asn1_choice.c

        For ASN.1 CHOICE object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_CHOICE"

        *   AnscAsn1ChoiceCreate
        *   AnscAsn1ChoiceInitialize
        *   AnscAsn1ChoiceFree
        *   AnscAsn1ChoiceClone
        *   AnscAsn1ChoiceCopyTo
        *   AnscAsn1ChoiceEqualsTo
        *   AnscAsn1ChoiceGetSizeOfEncoded
        *   AnscAsn1ChoiceDecodingData
        *   AnscAsn1ChoiceEncodingData
        *   AnscAsn1ChoiceTraceObject
        *   AnscAsn1ChoiceDumpObject
        *   AnscAsn1ChoiceReadyToEncode

        *   AnscAsn1ChoiceGetSelection
        *   AnscAsn1ChoiceSetSelection
        *   AnscAsn1ChoiceSetAndCreateSelection
        *   AnscAsn1ChoiceSetSelectionByOID
        *   AnscAsn1ChoiceIsChoiceByOID
        *   AnscAsn1ChoiceIsChoiceByInt
        *   AnscAsn1ChoiceCreateSelectionByIndex
        *   AnscAsn1ChoiceCreateSelectionAttr
        *   AnscAsn1ChoiceGetChoiceByOID
        *   AnscAsn1ChoiceGetChildTagValue
        *   AnscAsn1ChoiceGetSelectionName
        *   AnscAsn1ChoiceGetOIDValueByMask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/19/2002  initial revision.
        05/07/2002  A new api was added for choice object.
        06/27/2002  IsChoiceByInt was added fro CHOICE object

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1ChoiceCreate
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
AnscAsn1ChoiceCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( (ULONG)hContainerContext > 0)
    {
        pMyObject = (PANSC_ASN1_CHOICE)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_CHOICE)AnscAllocateMemory(sizeof(ANSC_ASN1_CHOICE));
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
    pMyObject->Oid                 = ANSC_ASN1_CHOICE_ID;
    pMyObject->uType               = ASN1_CHOICE_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->bChoiceByInt        = FALSE;
    pMyObject->Create              = AnscAsn1ChoiceCreate;
    pMyObject->AsnFree             = AnscAsn1ChoiceFree;
    
    AnscAsn1ChoiceInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_CHOICE_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_CHOICE_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceFree
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
AnscAsn1ChoiceFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;

    if( pMyObject != NULL)
    {
        /*
         *  Free the selection;
         */
        if( pMyObject->hSelection != NULL)
        {
            ((PANSC_ASN1_OBJECT)pMyObject->hSelection)->AsnFree(pMyObject->hSelection);

            pMyObject->hSelection       = NULL;
        }
    }

    AnscAsn1Free(pMyObject);


    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceInitialize
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
AnscAsn1ChoiceInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1ChoiceClone;
    pMyObject->CopyTo               = AnscAsn1ChoiceCopyTo;
    pMyObject->EqualsTo             = AnscAsn1ChoiceEqualsTo;
    pMyObject->DecodingData         = AnscAsn1ChoiceDecodingData;
    pMyObject->EncodingData         = AnscAsn1ChoiceEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1ChoiceGetSizeOfEncoded;
    pMyObject->ReadyToEncode        = AnscAsn1ChoiceReadyToEncode;
    pMyObject->DumpObject           = AnscAsn1ChoiceDumpObject;

    pMyObject->GetSelection         = AnscAsn1ChoiceGetSelection;
    pMyObject->IsChoiceByOID        = AnscAsn1ChoiceIsChoiceByOID;
    pMyObject->IsChoiceByInt        = AnscAsn1ChoiceIsChoiceByInt;
    pMyObject->SetSelection         = AnscAsn1ChoiceSetSelection;
    pMyObject->SetAndCreateSelection= AnscAsn1ChoiceSetAndCreateSelection;
    pMyObject->CreateSelectionAttr  = AnscAsn1ChoiceCreateSelectionAttr;
    pMyObject->CreateSelection      = AnscAsn1ChoiceCreateSelectionByIndex;
    pMyObject->GetChoiceTagValue    = AnscAsn1ChoiceGetChildTagValue;
    pMyObject->GetChoiceFromOID     = AnscAsn1ChoiceGetChoiceByOID;
    pMyObject->GetSelectionName     = AnscAsn1ChoiceGetSelectionName;
    pMyObject->GetOIDValueByMask    = AnscAsn1ChoiceGetOIDValueByMask;
    pMyObject->SetSelectionByOID    = AnscAsn1ChoiceSetSelectionByOID;

    pMyObject->hSelection           = NULL;
    pMyObject->bChoiceByOID         = FALSE;
    pMyObject->lSelect              = -1;
    pMyObject->uTotalChoice         = 0;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1ChoiceClone
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function clones a same ASN.1 object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is actually the pointer of this object
                itself.

    return:     The cloned container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1ChoiceClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_CHOICE               pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_CHOICE)
                 pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1ChoiceClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList); */
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
        AnscAsn1ChoiceCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestHandle
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1ChoiceCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_CHOICE               pNewObject   = (PANSC_ASN1_CHOICE)hDestObject;
    PANSC_ASN1_OBJECT               pSelection;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }

    /*
     *  Set the value;
     */
    pNewObject->bChoiceByOID        = pMyObject->bChoiceByOID;
    pNewObject->lSelect             = pMyObject->lSelect;
    pNewObject->uTotalChoice        = pMyObject->uTotalChoice;

    if( pMyObject->hSelection != NULL)
    {
        pSelection
            = ((PANSC_ASN1_OBJECT)pMyObject->hSelection)->Clone(pMyObject->hSelection);

        if( pSelection != NULL)
        {
            pSelection->hOwnerContext = pNewObject;
        }

        pNewObject->SetSelection(pNewObject, pNewObject->lSelect, pSelection);
    }
    else
    {
        pNewObject->hSelection      = NULL;
    }

    /*
     *  Set the optional state;
     */
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;
   
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceEqualsTo
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
AnscAsn1ChoiceEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_CHOICE               pMyObject  = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_CHOICE               pNewObject = (PANSC_ASN1_CHOICE)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1ChoiceEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1ChoiceEqualsTo().\n");

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

    if(!pMyObject->bOptional)
    {
        if( 
            pMyObject->bChoiceByOID  != pNewObject->bChoiceByOID ||
            pMyObject->lSelect       != pNewObject->lSelect
          )
        {
            return FALSE;
        }

        if( pMyObject->hSelection == NULL)
        {
            return (pMyObject->hSelection == pNewObject->hSelection);
        }
        else
        {
            return ((PANSC_ASN1_OBJECT)pMyObject->hSelection)->
                        EqualsTo
                            (
                                pMyObject->hSelection, 
                                pNewObject->hSelection, 
                                FALSE
                            );
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceReadyToEncode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        Check the choice object is ready to encode or not

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     Ready or not

**********************************************************************/

BOOLEAN
AnscAsn1ChoiceReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_OBJECT               pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;
    LONG                            selType      = pMyObject->lSelect;

    /*
     *  if it's optional, return true;
     */
    if( pMyObject->bOptional)
    {
        return TRUE;
    }

    if( selType >= (LONG)pMyObject->uTotalChoice)
    {
        return FALSE;
    }

    if( pSelection == NULL)
    {
        return FALSE;
    }

    return pSelection->ReadyToEncode(pSelection);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1ChoiceGetSizeOfEncoded
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
AnscAsn1ChoiceGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    LONG                            lSize        = 0;
    
    /*
     *  BOOLEAN                         bMyInclude   = bTagIncluded;
     *
     *  Please be aware of that here is different from other objects.
     *  No matter the "bTagIncluded" is true or false, the selection
     *  tag should be encoded.
     */    

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the CHOICE object is not ready to encode.\n");

        return -1;
    }

    /*
     *  If there's no selection, not ready to encode;
     */
    if( pMyObject->hSelection == NULL || pMyObject->lSelect == -1)
    {

        return  -1;
    }


    lSize = 
        ((PANSC_ASN1_OBJECT)pMyObject->hSelection)->GetSizeOfEncoded(pMyObject->hSelection);

    if( lSize < 0)
    {
        AnscTrace("The selection of the CHOICE is not ready to be encoded.\n");
        return -1;
    }

    /*
     *  check the attribute list, from the end of the list;
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, (ULONG)lSize, 0);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceDecodingData
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
PANSC_ASN1_OBJECT
choiceCreateSelectionByAttr
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_ATTR_OBJECT           pAttrObject
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    LONG                            i            = -1;
    ASN_OBJECT_FORM_TYPE            formType;
    ULONG                           uTagValue;
    PANSC_ASN1_OBJECT               pTmpSelection= NULL;
    PANSC_ASN1_OBJECT               pChildSelection;


    /*
     * check the input;
     */
    if( pAttrObject == NULL || hThisObject == NULL)
    {
        return pTmpSelection;
    }

    /*
     * if it's only one choice, that's it;
     */
    if( pMyObject->uTotalChoice == 1)
    {
        pMyObject->lSelect = 0;

        pMyObject->hSelection = 
            pMyObject->CreateSelection
                (
                    pMyObject,
                    0
                );

        return (PANSC_ASN1_OBJECT)pMyObject->hSelection;
    }

    /*
     *  check the possible choices one by one;
     */
    for( i = 0; i < (LONG)pMyObject->uTotalChoice; i ++)
    {
        if(
            pMyObject->GetChoiceTagValue
            (
                pMyObject,
                i,
                &formType,
                &uTagValue
            )
          )
        {  
            if( formType  == pAttrObject->FormType &&
                uTagValue == pAttrObject->ulClassNumber)
            {
                pMyObject->lSelect = i;

                pMyObject->hSelection = 
                    pMyObject->CreateSelection
                        (
                            pMyObject,
                            i
                        );

                return (PANSC_ASN1_OBJECT)pMyObject->hSelection;
            }
        }
        else
        {
            pTmpSelection = pMyObject->CreateSelection(pMyObject,  i);

            if( pTmpSelection != NULL)
            {
                pTmpSelection->GetEncodeTagValue
                    (
                        pTmpSelection,
                        &formType,
                        &uTagValue
                    );
            
                if( formType    == pAttrObject->FormType    &&
                    uTagValue   == pAttrObject->ulClassNumber )
                {
                    pMyObject->hSelection = (ANSC_HANDLE)pTmpSelection;
                    pMyObject->lSelect    = i;

                    return pTmpSelection;
                }

                /* if it's a chice type and no further attribute */
                if( uTagValue == ASN1_CHOICE_TYPE && formType == UNIVERSAL_FORM)
                {
                    pChildSelection = (PANSC_ASN1_OBJECT)
                        choiceCreateSelectionByAttr
                            (
                                pTmpSelection,
                                pAttrObject
                            );

                    if( pChildSelection != NULL)
                    {
                        pMyObject->hSelection = (ANSC_HANDLE)pTmpSelection;
                        pMyObject->lSelect    = i;

                        return pTmpSelection;                    
                    }
                }
                /*
                 *  not this one, forget it;
                 */
                pTmpSelection->AsnFree(pTmpSelection);
            }
        }
    }

    return NULL;
}

ANSC_STATUS
AnscAsn1ChoiceDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_OBJECT               pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;
    PANSC_ASN1_OBJECT               pParent;
    PANSC_ASN1_OIDEN                pOIDObject;
    PANSC_ASN1_INTEGER              pInteger;
    PANSC_ATTR_OBJECT               pAttrObj;
    CHAR                            pOIDStringValue[128] = { 0 };
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit= (PUCHAR)*ppEncoding;

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

    /*
     *  If there's a selection, decode it directly;
     */
    if( pSelection == NULL)
    {
        /*
         * Create the selection and decode it;
         */
        if( pMyObject->IsChoiceByOID(pMyObject) || pMyObject->IsChoiceByInt(pMyObject))
        {
            pParent = (PANSC_ASN1_OBJECT)pMyObject->hOwnerContext;

            /*
             *   Sample object:
             *   AttributeTypeAndValues::= CASN1_SEQUENCE 
             *       {
             *                   type      AttributeType ,
             *                   values    AttributeValues
             *       }
             *
             *   AttributeValues ::= CASN1_SETOF AttributeValue  {}
             *
             *   AttributeValue ::= CASN1_CHOICE 
             *       {
             *                       x520Name X520name ,
             *                x520dnQualifier X520dnQualifier ,
             *                          email Pkcs9email ,
             *               unstructuredName UnstructuredName ,
             *                    contentType ContentType ,
             *                  messageDigest MessageDigest ,
             *                    signingTime SigningTime ,
             *               counterSignature SignerInfo ,
             *              challengePassword ChallengePassword ,
             *            unstructuredAddress UnstructuredAddress ,
             *              extendedCertAttri ExtendedCertificateAttributes 
             *                       regToken RegToken ,
             *                  authenticator Authenticator ,
             *             pkiPublicationInfo PKIPublicationInfo ,
             *              pkiArchiveOptions PKIArchiveOptions ,
             *                      oldCertID OldCertId ,
             *                protocolEncrKey ProtocolEncrKey,             
             *                            pfx PFX,
             *                      bmpString BMPString,
             *                       keyUsage KeyUsage,
             *               extensionRequest Extensions
             *           }
             *
             *  in this case , we have to turn to the owner of the owner of 
             *  the choice object (AttributeValue) and find out the OID value
             *  from the "AttributeType". After we get the OID value, we can 
             *  decide which is the selection of the choice object.
             *
             *  In most cases, the OID object is another child object of the same 
             *  owner.
             *
             *   And the last owner must be a SEQUENCE object.
             */

            /*
             *  Get the parent object;
             */
            if( pParent == NULL)
            {
                AnscTrace("Failed to find the parent object in a Choice object.\n");
                return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
            }    

            if(pMyObject->IsChoiceByInt(pMyObject))
            {
                /*
                 *  The parent must be a SEQUENCE object.
                 */
                if( pParent->uType != ASN1_SEQUENCE_TYPE)
                {
                    AnscTrace("It's not a sequence object, failed to find the OID object \
                            in Choice Decoding.\n");

                    return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                }

                /* Find the integer object */
                pInteger  = ((PANSC_ASN1_SEQUENCE)pParent)->GetFirstIntegerChild(pParent);

                if( pInteger == NULL)
                {
                    AnscTrace("Failed to find the previous Integer object in Choice decoding.\n");

                    return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                }

                /* get the integer string value */
                AnscGetUlongString
                    (
                        (int)pInteger->GetIntegerValue(pInteger),
                        pOIDStringValue
                    );
            }
            else /*Then it's chosed by OID*/
            {
                /*
                 *  If it's SETOF type, get the parent's parent;
                 */
                if( pParent->uType == ASN1_SET_TYPE)
                {
                    pParent = (PANSC_ASN1_OBJECT)pParent->hOwnerContext;

                    if( pParent == NULL)
                    {
                        AnscTrace("Failed to find the parent object in a Choice object.\n");
                        return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                    }    
                }

                /*
                 *  The parent must be a SEQUENCE object.
                 */
                if( pParent->uType != ASN1_SEQUENCE_TYPE)
                {
                    AnscTrace("It's not a sequence object, failed to find the OID object \
                            in Choice Decoding.\n");

                    return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                }

                /*
                 *  Find the OID object;
                 */
                pOIDObject  = (PANSC_ASN1_OIDEN)((PANSC_ASN1_SEQUENCE)pParent)->GetFirstOIDChild(pParent);

                if( pOIDObject == NULL)
                {
                    AnscTrace("Failed to find the previous OID object in Choice decoding.\n");

                    return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                }

                /*
                 *  get the oid value and get the selection;
                 */
                if( !pOIDObject->GetStringOIDValue( pOIDObject,pOIDStringValue))
                {
                    AnscTrace("Failed to get the OID value.\n");

                    return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
                }
            }


            /*
             *  get the selection type with OID value;
             */
            pMyObject->lSelect = 
                pMyObject->GetChoiceFromOID
                            ( 
                                pMyObject,
                                pOIDStringValue 
                            );

            if( pMyObject->lSelect < 0)
            {
                AnscTrace
                    (
                        "Unknown OID value \"%s\", failed to make the choice.\n",
                        pOIDStringValue
                    );
            }

            /*
             * Create the selection;
             */
            pMyObject->hSelection = 
                pMyObject->CreateSelection
                            (
                                pMyObject,
                                pMyObject->lSelect
                            );

            pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;

        }
        else /* if not choicebyOID, check the tag */
        {
            pAttrObj = 
                (PANSC_ATTR_OBJECT)AnscAsn1AttrParsingData(pEncodingUnit);

            if( pAttrObj == NULL)
            {
                /* ( AnscTrace("Failed to parse the binary data to a PANSC_ATTR_OBJECT object.\n");*/

                return ANSC_ASN1_UNEXPECTED_TAG;
            }

            /*
             *  get the selection type with Attribute value;
             */
            pSelection = 
                choiceCreateSelectionByAttr
                            ( 
                                pMyObject,
                                pAttrObj 
                            );

            /*
             *  Free the attribute object ;
             */
            pAttrObj->Remove(pAttrObj);
        }

        if( pSelection == NULL)
        {
            /* AnscTrace("Failed to decide the choice.\n"); */
            return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
        }
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
     returnStatus = pSelection->DecodingData(pSelection, (PVOID*)&pEncodingUnit);

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
                        "Unexpected End of Indefinite Content : [%.2X %.2X %.2X %.2X] !\n",
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

    if( ANSC_STATUS_SUCCESS == returnStatus)
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

        *ppEncoding          = (PVOID)pEncodingUnit;
        pMyObject->bOptional = FALSE;        
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceEncodingData
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
AnscAsn1ChoiceEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
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

    /*
     *  Encode the value;
     */
    if ( pMyObject->hSelection  == NULL)
    {
        AnscTrace("No selection in Choice object, failed to encode.\n");

        return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
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
        AnscTrace("Failed to encode the attrlist of CHOICE.\n");

        return returnStatus;
    }

   /*
    *   Encode the selection object;
    */
    returnStatus = 
        ((PANSC_ASN1_OBJECT)pMyObject->hSelection)
                ->EncodingData
                    (
                        pMyObject->hSelection,
                        (PVOID*)&pCharData
                    );

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
                    "Warning in Encoding CHOICE '%s', the size of encoding is expected to be %d, but it's %d.\n",
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

        *ppEncoding                     =  (PVOID)pCharData;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceDumpObject
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
                The object is  root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1ChoiceDumpObject
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
    PANSC_ASN1_CHOICE               pMyObject       = (PANSC_ASN1_CHOICE)hThisObject;
    CHAR                            pAttrBuf[512]   = { 0 };
    ULONG                           attrLength      = 512;
    PCHAR                           pName;
    errno_t                         rc              = -1;

    if( pBuffer == NULL || pLength == NULL)
    {
        return FALSE;
    }

    AttrListDump( &pMyObject->sAttrList, pAttrBuf, &attrLength);

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

    if( pMyObject->bOptional)
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (Optional)",
                    pName,
                    pAttrBuf,
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
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType)
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
        AnscAsn1ChoiceTraceObject
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
AnscAsn1ChoiceTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{
    UNREFERENCED_PARAMETER(bShowValue);
#ifndef _PKI_KERNEL
    
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_OBJECT               pSelection   = NULL;
    CHAR                            pAttrBuf[512]= { 0 };
    ULONG                           attrLength   = 512;
    PCHAR                           pName;

    if( hThisObject == NULL)
    {
        return FALSE;
    }

    /*
     *  If it's recursive and has selection, we only trace the selection here;
     */    
    pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;

    if( bRecursive && pSelection != NULL)
    {
        /* pSelection->TraceObject( pSelection, layer, bShowValue, bRecursive);*/
    }
    else 
    {
        TraceTabs(layer);
        AttrListDump( &pMyObject->sAttrList, pAttrBuf, &attrLength);

        if( layer  == 0)
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

        if( pMyObject->bOptional)
        {
            AnscTrace
                (
                    "%s ::=%s %s (Optional)\n",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType)
                );
        }
        else
        {
            AnscTrace
                (
                    "%s ::=%s %s\n",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType)
                );
        }
    }

#endif

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1ChoiceGetSelection
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the selected handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the selected handle

**********************************************************************/
ANSC_HANDLE
AnscAsn1ChoiceGetSelection
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE                 pMyObject  = (PANSC_ASN1_CHOICE)hThisObject;

    return pMyObject->hSelection;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceIsChoiceByOID
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This object is choosed by OID

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     yes or not

**********************************************************************/
BOOLEAN
AnscAsn1ChoiceIsChoiceByOID
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE                 pMyObject  = (PANSC_ASN1_CHOICE)hThisObject;

    return pMyObject->bChoiceByOID;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceIsChoiceByInt
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This object is choosed by Integer

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     yes or not

**********************************************************************/
BOOLEAN
AnscAsn1ChoiceIsChoiceByInt
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CHOICE                 pMyObject  = (PANSC_ASN1_CHOICE)hThisObject;

    return pMyObject->bChoiceByInt;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceSetSelection
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       selType,
                ANSC_HANDLE                 hSelection
            );

    description:

        Set the selected handle to Choice Object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       selType,
                The selection type;

                ANSC_HANDLE                 hSelection
                The selection object;

    return:     the selection type

**********************************************************************/
ANSC_STATUS
AnscAsn1ChoiceSetSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType,
        ANSC_HANDLE                 hSelection
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_OBJECT               pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;
    PANSC_ASN1_OBJECT               pNewSel      = (PANSC_ASN1_OBJECT)hSelection;

    /*
     * Check the select type;
     */
    if(selType >= pMyObject->uTotalChoice)
    {
        return ANSC_ASN1_INVALID_VALUE;
    }

    pMyObject->lSelect              = selType;

    if( hSelection == pSelection)
    {       
        return ANSC_ASN1_ALREADY_EXIST;
    }

    if( pSelection != NULL)
    {
        pSelection->AsnFree((ANSC_HANDLE)pSelection);
    }

    pMyObject->hSelection           = hSelection;

    if( pNewSel)
    {
        pNewSel->hOwnerContext = hThisObject;

        pNewSel->SetName
            (
                hSelection,
                pMyObject->GetSelectionName(pMyObject, selType)
            );

        pNewSel->AddAttribute
            (
                hSelection,
                pMyObject->CreateSelectionAttr(pMyObject, selType),
                FALSE
            );
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceSetAndCreateSelection
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       selType
            );

    description:

        Set the selected index and create the selection object;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       selType,
                The selection type;

    return:     the status

**********************************************************************/
ANSC_STATUS
AnscAsn1ChoiceSetAndCreateSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;
    PANSC_ASN1_OBJECT               pSelection   = (PANSC_ASN1_OBJECT)pMyObject->hSelection;
    PANSC_ASN1_OBJECT               pNewSel;

    /*
     * Check the select type;
     */
    if(selType >= pMyObject->uTotalChoice)
    {
        return ANSC_ASN1_INVALID_VALUE;
    }

    pMyObject->lSelect              = selType;

    if( pSelection != NULL)
    {
        pSelection->AsnFree((ANSC_HANDLE)pSelection);
    }

    pNewSel  = (PANSC_ASN1_OBJECT)
        pMyObject->CreateSelection(pMyObject, selType);

    pMyObject->hSelection  = (ANSC_HANDLE)pNewSel;

    if( pNewSel)
    {
        pNewSel->hOwnerContext = hThisObject;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    NOTE:       THE DERIVED CLASS MUST REWRITE THIS FUNCTION.

    caller:     owner of this object

    prototype:

        PANSC_ATTR_OBJECT
        AnscAsn1ChoiceCreateSelectionAttr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       selType
            );

    description:

        Create the attribute with the given select type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       selType
                The given seleted type;

    return:     the created attr object.

**********************************************************************/
PANSC_ATTR_OBJECT
AnscAsn1ChoiceCreateSelectionAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(selType);
    return NULL;
}

/**********************************************************************

    NOTE:       THE DERIVED CLASS MUST REWRITE THIS FUNCTION.

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1ChoiceCreateSelectionByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       type
            );

    description:

        Create selected with the given type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       type
                The given type

    return:     The created selection handle.

**********************************************************************/
ANSC_HANDLE
AnscAsn1ChoiceCreateSelectionByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(type);
    AnscTrace("Please implement this function in the derived class.\n");

    return NULL;
}

/**********************************************************************

    NOTE:       THE DERIVED CLASS MUST REWRITE THIS FUNCTION.

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ChoiceGetChildTagValue
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uIndex,
                PASN_OBJECT_FORM_TYPE       pForm,
                PULONG                      pTagValue
            );

    description:

        this function returns the form type and tag value of the
        object will be selected.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uIndex,
                The index of the selection;

                PASN_OBJECT_FORM_TYPE       pForm,
                The buffer of returned form type

                PULONG                      pTagValue
                The buffer of returned tag value

    return:     Succeeded or not

**********************************************************************/
BOOLEAN
AnscAsn1ChoiceGetChildTagValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PASN_OBJECT_FORM_TYPE       pForm,
        PULONG                      pTagValue
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(uIndex);
    UNREFERENCED_PARAMETER(pForm);
    UNREFERENCED_PARAMETER(pTagValue);
    return FALSE;
}

/**********************************************************************

    NOTE:       THE DERIVED CLASS MUST REWRITE THIS FUNCTION if it's 
                choice by OID.

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1ChoiceGetChoiceByOID
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pOIDString
            );

    description:

        With the previous OID value, find out what's the selection here.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pOIDString
                The oid value  string.

    return:     The selection type of the choice

**********************************************************************/
LONG
AnscAsn1ChoiceGetChoiceByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    )
{
    UNREFERENCED_PARAMETER(pOIDString);
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;

    if( pMyObject->bChoiceByOID)
    {
        AnscTrace("Please implement this function in the derived class.\n");
    }

    return  0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PCHAR
        AnscAsn1ChoiceGetSelectionName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       selType
            );

    description:

                return the name of the selected object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       selType
                The selected type;

    return:     The name of the selected object.

**********************************************************************/
PCHAR
AnscAsn1ChoiceGetSelectionName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;

    if(selType >= pMyObject->uTotalChoice)
    {
        return NULL;
    }

    return ANSC_CHOICE_SEL_NAME;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PCHAR
        AnscAsn1ChoiceGetOIDValueByMask
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       selType
            );

    description:

                return the OID value of the mask 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       selType
                The selected type;

    return:     The OID string value if have.

**********************************************************************/
PCHAR
AnscAsn1ChoiceGetOIDValueByMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(selType);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1ChoiceSetSelectionByOID
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pOIDString
            );

    description:

                Set the selection by OID

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pOIDString
                The input OID string value;

    return:     The status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1ChoiceSetSelectionByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    )
{
    PANSC_ASN1_CHOICE               pMyObject    = (PANSC_ASN1_CHOICE)hThisObject;

    if( pMyObject == NULL || pOIDString == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    if( !pMyObject->IsChoiceByOID(pMyObject) && !pMyObject->IsChoiceByInt(pMyObject))
    {
        return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
    }

    if( pMyObject->hSelection != NULL)
    {
        ((PANSC_ASN1_OBJECT)pMyObject->hSelection)->AsnFree(pMyObject->hSelection);
    }

    pMyObject->lSelect      = pMyObject->GetChoiceFromOID(pMyObject, pOIDString);
    pMyObject->hSelection   = 
        pMyObject->CreateSelection
        (
            pMyObject, 
            pMyObject->lSelect
        );

    if( pMyObject->hSelection == NULL)
    {
        return ANSC_ASN1_NO_SELECTION_IN_CHOICE;
    }

    return ANSC_STATUS_SUCCESS;
}
