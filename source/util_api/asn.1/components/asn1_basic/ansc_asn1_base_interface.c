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

    module: ansc_asn1_base_interface.c

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class

        *   AnscAsn1Create
        *   AnscAsn1Free
        *   AnscAsn1Initialize

        *   AnscAsn1SetName
        *   AnscAsn1SetClassName
        *   AnscAsn1AddAttribute
        *   AnscAsn1GetTagValue
        *   AnscAsn1ReadyToEncode
        *   AnscAsn1GetFirstOctet
        *   AnscAsn1IsConstructive

        *   AnscAsn1Clone
        *   AnscAsn1CopyTo
        *   AnscAsn1EqualsTo
        *   AnscAsn1GetSizeOfEncoded
        *   AnscAsn1DecodingData
        *   AnscAsn1EncodingData
        *   AnscAsn1GetEncodedData
        *   AnscAsn1GetBAOHandle

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.
        04/24/02    AnscAsn1GetTagValue() is added as an API.
        05/23/02    AnscAsn1GetEncodedData() is added as an API.
        05/28/02    AnscAsn1GetMD5FingerPrint() is added as an API.
                    AnscAsn1GetSHA1FingerPrint() is added as an API.
        07/22/02    AnscAsn1GetBAOHandle() is added as an API.
                    BAO APIs are defined by Lina for Kerberoes implementation.

**********************************************************************/

#include "ansc_asn1_local.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1Create
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
AnscAsn1Create
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pBaseObject = (PANSC_ASN1_OBJECT)AnscAllocateMemory(sizeof(ANSC_ASN1_OBJECT));

    if ( !pBaseObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->Name      = AnscCloneString(ANSC_ASN1_OBJECT_NAME);
    pBaseObject->ClassName = NULL;

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pBaseObject->sAttrList);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = NULL;
    pBaseObject->Oid               = ANSC_ASN1_OBJECT_ID;
    pBaseObject->uType             = ASN1_ANY_TYPE;
    pBaseObject->bOptional         = FALSE;
    pBaseObject->uBasicSize        = 0; 
    pBaseObject->Create            = AnscAsn1Create;
    pBaseObject->AsnFree           = AnscAsn1Free;

    AnscAsn1Initialize(pBaseObject);

    return  (ANSC_HANDLE)pBaseObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1Free
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
AnscAsn1Free
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = (PANSC_ASN1_OBJECT)hThisObject;

    if( pBaseObject != NULL)
    {
        if( pBaseObject->Name != NULL)
        {
            AnscFreeMemory(pBaseObject->Name);
        }

        if( pBaseObject->ClassName != NULL)
        {
            AnscFreeMemory(pBaseObject->ClassName);
        }

        AttrListRemoveAllAttributes(&pBaseObject->sAttrList);

        AnscFreeMemory(pBaseObject);
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1Initialize
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
AnscAsn1Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = (PANSC_ASN1_OBJECT)hThisObject;

    /* set other functions */
    pBaseObject->SetName            = AnscAsn1SetName;
    pBaseObject->SetClassName       = AnscAsn1SetClassName;
    pBaseObject->Clone              = AnscAsn1Clone;
    pBaseObject->CopyTo             = AnscAsn1CopyTo;
    pBaseObject->EqualsTo           = AnscAsn1EqualsTo;
    pBaseObject->DecodingData       = AnscAsn1DecodingData;
    pBaseObject->EncodingData       = AnscAsn1EncodingData;
    pBaseObject->GetEncodedData     = AnscAsn1GetEncodedData;
    pBaseObject->GetSizeOfEncoded   = AnscAsn1GetSizeOfEncoded;
    pBaseObject->GetFirstOctet      = AnscAsn1GetFirstOctet;
    pBaseObject->ReadyToEncode      = AnscAsn1ReadyToEncode;
    pBaseObject->IsConstructive     = AnscAsn1IsConstructive;
    pBaseObject->AddAttribute       = AnscAsn1AddAttribute;
    pBaseObject->GetEncodeTagValue  = AnscAsn1GetTagValue;
    pBaseObject->GetBAOHandle       = AnscAsn1GetBAOHandle;

    pBaseObject->BeforeDecoding     = NULL;
    pBaseObject->AfterDecoding      = NULL;
    pBaseObject->BeforeEncoding     = NULL;
    pBaseObject->AfterEncoding      = NULL;

    pBaseObject->bCanBeOptional     = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1Clone
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
AnscAsn1Clone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pMyObject    = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_ASN1_OBJECT               pNewObject   = NULL;

    pNewObject = 
        pMyObject->Create
            (
                pMyObject->hContainerContext
            );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1SetOfClone()\n");

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
        AnscAsn1CopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copy the value from the source object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The source object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1CopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hDestObject);
    /* We don't do anything at the base class here */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1EqualsTo
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
AnscAsn1EqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hOtherObject);
    UNREFERENCED_PARAMETER(bValueOnly);
    /* We don't do anything at the base class here */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetName
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pData
            );

    description:

        This function set the name of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pData
                The input name string of the object.

    return:     The status of the operation;

**********************************************************************/

ANSC_STATUS
AnscAsn1SetName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pData
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = (PANSC_ASN1_OBJECT)hThisObject;

    if( pData == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    if( pBaseObject->Name != NULL)
    {
        AnscFreeMemory(pBaseObject->Name);
    }

    pBaseObject->Name = AnscCloneString(pData);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1SetClassName
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pData
            );

    description:

        This function set the ClassName of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pData
                The input name string of the object.

    return:     The status of the operation;

**********************************************************************/

ANSC_STATUS
AnscAsn1SetClassName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pData
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = (PANSC_ASN1_OBJECT)hThisObject;

    if( pData == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    if( pBaseObject->ClassName != NULL)
    {
        AnscFreeMemory(pBaseObject->ClassName);
    }

    pBaseObject->ClassName = AnscCloneString(pData);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1IsConstructive
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the ASN.1 object is constructive or not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the optional state of this ASN.1 object.

**********************************************************************/
BOOLEAN
AnscAsn1IsConstructive
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pBaseObject  = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_ASN1_CHOICE               pChoice;
    PANSC_ATTR_OBJECT               pChoiceAttr;

    if(IsObjectContextType(pBaseObject->uType))
    {
        return TRUE;
    }
    else if( IsObjectPrimitiveType(pBaseObject->uType))
    {
        return FALSE;
    }
    else if( pBaseObject->uType  == ASN1_CHOICE_TYPE)
    {
        pChoice = (PANSC_ASN1_CHOICE)pBaseObject;

        if( pChoice->hSelection != NULL)
        {
            pChoiceAttr = pChoice->CreateSelectionAttr(pChoice, pChoice->lSelect);

            if( pChoiceAttr != NULL && !pChoiceAttr->bImplicit)
            {
                pChoiceAttr->Remove(pChoiceAttr);

                return TRUE;
            }

            if(pChoiceAttr != NULL)
            {
                pChoiceAttr->Remove(pChoiceAttr);
            }

            return ((PANSC_ASN1_OBJECT)pChoice->hSelection)->IsConstructive(pChoice->hSelection);
        }
    }

    return TRUE;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1ReadyToEncode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called before ASN.1 encoding to check the
        value is well-set or not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of ready to encode.

**********************************************************************/
BOOLEAN
AnscAsn1ReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1GetSizeOfEncoded
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
AnscAsn1GetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1DecodingData
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
AnscAsn1DecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ppEncoding);
    return ANSC_ASN1_FORBIDDEN_PLACE;
}

/**********************************************************************

    caller:     owner of this object
                All the ASN1 object can share the same function here, you
                don't need to rewrite it, but you have to free the memory 
                cause it's new created.

    prototype:

        PUCHAR
        AnscAsn1GetEncodedData
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pLength
            );

    description:

        This function encoded the object to the created binary data

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pLength
                The buffer of new created binary length

    return:     The new created and encoded binary data

**********************************************************************/
PUCHAR
AnscAsn1GetEncodedData
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pLength
    )
{
    PANSC_ASN1_OBJECT               pThisObject = (PANSC_ASN1_OBJECT)hThisObject;
    PUCHAR                          pNewBuffer, pBack;
    LONG                            length;

    if( pThisObject == NULL)
    {
        return NULL;
    }

    length = pThisObject->GetSizeOfEncoded(pThisObject);

    if( length <= 0)
    {
        return NULL;
    }


    pNewBuffer = (PUCHAR)AnscAllocateMemory(length + 16);

    if( pNewBuffer == NULL)
    {
        return NULL;
    }

    pBack = pNewBuffer;

    if( ANSC_STATUS_SUCCESS != pThisObject->EncodingData(pThisObject, (PVOID*)&pBack))
    {
        AnscFreeMemory(pNewBuffer);

        return NULL;
    }

    if( pLength)
    {
        *pLength = length;
    }

    return pNewBuffer;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EncodingData
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
AnscAsn1EncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ppEncoding);
    return ANSC_ASN1_FORBIDDEN_PLACE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        UCHAR
        AnscAsn1GetFirstOctet
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function returns the first octet when encoding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the first encoding octet;

**********************************************************************/
UCHAR
AnscAsn1GetFirstOctet
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pMyObject    = (PANSC_ASN1_OBJECT)hThisObject;
    ULONG                           uMyType      = pMyObject->uType;

    if( IsObjectPrimitiveType(uMyType))
    {
        return (PRIMITIVE_FORM & FORM_MASK) | (UCHAR)(pMyObject->uType & TYPE_MASK);
    }
    else if( IsObjectContextType(uMyType))
    {
        return (CONSTRUCTED_FORM & FORM_MASK) | (UCHAR)(pMyObject->uType & TYPE_MASK);
    }
    else
    {
        return 0x00;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscAsn1AddAttribute
            (
                ANSC_HANDLE                 hThisObject,
                PANSC_ATTR_OBJECT           pAttrObject,
                BOOLEAN                     bAppend
            );

    description:

        This function add a new attribute object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PANSC_ATTR_OBJECT           pAttrObject,
                The new attribute object will be added;

                BOOLEAN                     bAppend
                Add to the first or append at the last

    return:     the first encoding octet;

**********************************************************************/
void
AnscAsn1AddAttribute
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_ATTR_OBJECT           pAttrObject,
        BOOLEAN                     bAppend
    )
{
    PANSC_ASN1_OBJECT               pMyObject    = (PANSC_ASN1_OBJECT)hThisObject;

#if 0
    PANSC_ATTR_OBJECT               pAttr;
    PSINGLE_LINK_ENTRY              pSListEntry;
#endif

    if( hThisObject == NULL || pAttrObject == NULL)
    {
        return;
    }

    if( bAppend)
    {
        AnscSListPushEntryAtBack(&pMyObject->sAttrList, &pAttrObject->Linkage);
    }
    else
    {
#if 0
        /*
         *  Check if this attribute already exists, ignore the new one.
         */
        pSListEntry =   AnscSListGetFirstEntry(&pMyObject->sAttrList);

        if( pSListEntry != NULL)
        {
            pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);

            if( pAttrObject->EqualsTo(pAttrObject, pAttr))
            {
                return;
            }
        }
#endif
        AnscSListPushEntry(&pMyObject->sAttrList, &pAttrObject->Linkage);
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscAsn1GetTagValue
            (
                ANSC_HANDLE                 hThisObject,
                PASN_OBJECT_FORM_TYPE       pForm,
                PULONG                      pTagValue
            );

    description:

        Get the first encoding tag value;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PASN_OBJECT_FORM_TYPE       pForm,
                The form type buffer;

                PULONG                      pTagValue
                The tag value buffer;

    return:     the first encoding octet;

**********************************************************************/
void
AnscAsn1GetTagValue
    (
        ANSC_HANDLE                 hThisObject,
        PASN_OBJECT_FORM_TYPE       pForm,
        PULONG                      pTagValue
    )
{
    PANSC_ASN1_OBJECT               pMyObject    = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_ATTR_OBJECT               pAttr        = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry;

    if( hThisObject == NULL || pTagValue == NULL)
    {
        return;
    }

    pSListEntry =   AnscSListGetFirstEntry(&pMyObject->sAttrList);

    while( pSListEntry != NULL)
    {
        pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        if( pAttr != NULL && pAttr->FormType != EMPTY_FORM)
        {
            break;
        }              
    }

    if( pAttr != NULL)
    {
        if( pForm != NULL)
        {
            *pForm      = pAttr->FormType;
        }

        *pTagValue  = pAttr->ulClassNumber;
    }
    else
    {
        if( pForm != NULL)
        {
            *pForm      = UNIVERSAL_FORM;
        }

        *pTagValue  = pMyObject->uType;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAsn1GetBAOHandle
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        Get the BAO handle for Kerberoes object;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The handle of kerberoes BAO handle;

**********************************************************************/
ANSC_HANDLE
AnscAsn1GetBAOHandle
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OBJECT               pMyObject    = (PANSC_ASN1_OBJECT)hThisObject;

    if( hThisObject == NULL || pMyObject->uBasicSize == 0)
    {
        return NULL;
    }

    return (ANSC_HANDLE)(pMyObject->uBasicSize + (ULONG)hThisObject);

}

