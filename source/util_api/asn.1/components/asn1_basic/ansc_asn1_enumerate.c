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

    module: ANSC_ASN1_ENUMERATE.c

        For ASN.1 ENUMERATE object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class  "ASN1_ENUMERATE"

        *   AnscAsn1EnumerateCreate
        *   AnscAsn1EnumerateFree
        *   AnscAsn1EnumerateInitialize
        *   AnscAsn1EnumerateClone
        *   AnscAsn1EnumerateCopyTo
        *   AnscAsn1EnumerateEqualsTo
        *   AnscAsn1EnumerateGetSizeOfEncoded
        *   AnscAsn1EnumerateDecodingData
        *   AnscAsn1EnumerateEncodingData
        *   AnscAsn1EnumerateDumpObject
        *   AnscAsn1EnumerateTraceObject

        *   AnscAsn1EnumerateGetIntegerValue
        *   AnscAsn1EnumerateSetIntegerValue

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1EnumerateCreate
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
AnscAsn1EnumerateCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_ENUMERATE)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_ENUMERATE)AnscAllocateMemory(sizeof(ANSC_ASN1_ENUMERATE));
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
    pMyObject->Oid                 = ANSC_ASN1_ENUMERATE_ID;
    pMyObject->uType               = ASN1_ENUMERATE_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->bFirstZero          = FALSE;
    pMyObject->Create              = AnscAsn1EnumerateCreate;
    pMyObject->AsnFree             = AnscAsn1EnumerateFree;
    
    AnscAsn1EnumerateInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_ENUMERATE_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_ENUMERATE_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EnumerateFree
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
AnscAsn1EnumerateFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    AnscAsn1Free(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EnumerateInitialize
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
AnscAsn1EnumerateInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1EnumerateClone;
    pMyObject->CopyTo               = AnscAsn1EnumerateCopyTo;
    pMyObject->EqualsTo             = AnscAsn1EnumerateEqualsTo;
    pMyObject->DecodingData         = AnscAsn1EnumerateDecodingData;
    pMyObject->EncodingData         = AnscAsn1EnumerateEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1EnumerateGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1EnumerateDumpObject;

    pMyObject->GetIntegerValue      = AnscAsn1EnumerateGetIntegerValue;
    pMyObject->SetIntegerValue      = AnscAsn1EnumerateSetIntegerValue;

    pMyObject->uValue               = 0;
    pMyObject->uLength              = 1;
    pMyObject->bFirstZero          = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1EnumerateClone
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
AnscAsn1EnumerateClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    PANSC_ASN1_ENUMERATE            pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_ENUMERATE)
                 pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1EnumerateClone()\n");

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
        AnscAsn1EnumerateClone
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies value to the other one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1EnumerateCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    PANSC_ASN1_ENUMERATE            pNewObject   = (PANSC_ASN1_ENUMERATE)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;        
    }

    /*
     * Copy the value;
     */
    pNewObject->uValue              = pMyObject->uValue;
    pNewObject->uLength             = pMyObject->uLength;
    pNewObject->bFirstZero          = pMyObject->bFirstZero;

    pNewObject->bOptional           = pMyObject->bOptional;
    pNewObject->bCanBeOptional      = pMyObject->bCanBeOptional;

    pNewObject->SetName(pNewObject, pMyObject->Name);

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1EnumerateEqualsTo
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
AnscAsn1EnumerateEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    UNREFERENCED_PARAMETER(bValueOnly);
    PANSC_ASN1_ENUMERATE            pMyObject  = (PANSC_ASN1_ENUMERATE)hThisObject;
    PANSC_ASN1_ENUMERATE            pNewObject = (PANSC_ASN1_ENUMERATE)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1EnumerateEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1EnumerateEqualsTo().\n");

        return FALSE;
    }

    /* check the attribute list */
    if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
    {
        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    if(!pMyObject->bOptional)
    {
        return (pMyObject->uValue == pNewObject->uValue);
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1EnumerateGetSizeOfEncoded
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
AnscAsn1EnumerateGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
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
        AnscTrace("The value of the ENUMERATE object is not ready to encode.\n");

        return -1;
    }

    ulSize              = pMyObject->uLength;

    /*
     *  check the attribute list, from the end of the list;
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EnumerateDecodingData
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
AnscAsn1EnumerateDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;

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
                    "Unexpected tag in ASN1_ENUMERATE - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->ulContentLength == 0)
        {
            AnscTrace("The length of ENUMERATE value is 0, unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

        if( pTagInfo->ulContentLength < 0)
        {
            AnscTrace("Infinite form is unacceptable for ENUMERATE object.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

        pEncodingUnit   += pTagInfo->GetSizeOfLength(pTagInfo) + 1;
        uLeftSize       =  pTagInfo->ulContentLength;


        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    if( uInfiniteTags > 0)
    {
        AnscTrace("Infinite form is unacceptable for NUMERATE object.\n");

        return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    if( uLeftSize > sizeof(ULONG))
    {
        AnscTrace("The length of ENUMERATE value is '%lu', unacceptable.\n", uLeftSize);

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    
    pMyObject->uLength              = uLeftSize;
    pMyObject->uValue               = GetLengthFromBuffer( pEncodingUnit, uLeftSize);

    if( pEncodingUnit[0] == 0x00 && uLeftSize != 1)
    {
        pMyObject->bFirstZero  = TRUE;
    }
    else
    {
        pMyObject->bFirstZero  = FALSE;
    }


    pEncodingUnit                   += uLeftSize;

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

    *ppEncoding                     = (PVOID)pEncodingUnit;
    pMyObject->bOptional            = FALSE;

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EnumerateEncodingData
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
AnscAsn1EnumerateEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    BOOLEAN                         bTagIncluded = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;

    /*
     * hold the computation value
     */
    LONG                            ulResult     = pMyObject->uValue;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData    = (PUCHAR)*ppEncoding;
    PUCHAR                          pBackData    = pCharData;

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
        *pCharData              = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        EncodeLength((PVOID*)&pCharData, pMyObject->uLength);
    }

    /*
     * we don't have problem if the current encoding is dynamic encoding; 
     * if the current encoding is static, we have to 
     * examine the long value and find out how many octets are needed
     */
    if( pMyObject->bFirstZero)
    {
        *pCharData               = 0x00;
        pCharData++;
    }

    if( ulResult <= 0xFF)
    {       
        *pCharData              = (UCHAR)(ulResult & 0xFF);
        pCharData++;
    }
    else
    {
        pCharData               += ASN1WriteUlong( pCharData, ulResult);
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
                    "Warning in Encoding ENUMERATE, the size of encoding is expected to be %d, but it's %d.\n",
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
        AnscAsn1EnumerateDumpObject
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
                The object is root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1EnumerateDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

#ifndef _PKI_KERNEL

    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
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
    else
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (%d)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    (int)pMyObject->uValue
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
        AnscAsn1EnumerateTraceObject
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
AnscAsn1EnumerateTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    CHAR                            pAttrBuffer[512] = { 0 };
    ULONG                           attrLength       = 512;
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

    if( pName == NULL)
    {
        return FALSE;
    }


    if( !bShowValue)
    {
        AnscTrace
            (
                "%s ::=%s %s\n",
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else if( pMyObject->bOptional)
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
                "%s ::=%s %s (%lu)\n",
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType),
                pMyObject->uValue
            );
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1EnumerateGetIntegerValue
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
AnscAsn1EnumerateGetIntegerValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject  = (PANSC_ASN1_ENUMERATE)hThisObject;


    return  (LONG)pMyObject->uValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1EnumerateSetIntegerValue
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
AnscAsn1EnumerateSetIntegerValue
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lValue
    )
{
    PANSC_ASN1_ENUMERATE            pMyObject    = (PANSC_ASN1_ENUMERATE)hThisObject;
    ULONG                           ulSize       = 0;
    ULONG                           uTemp        = lValue;
    ULONG                           uLast;

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
    if( uLast >= (ULONG)0x80 && ulSize < sizeof(ULONG))
    {
        pMyObject->bFirstZero       = TRUE;
        ulSize ++;
    }
    else
    {
        pMyObject->bFirstZero       = FALSE;
    }

    pMyObject->uLength              = ulSize;
    pMyObject->uValue               = lValue;

    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}


