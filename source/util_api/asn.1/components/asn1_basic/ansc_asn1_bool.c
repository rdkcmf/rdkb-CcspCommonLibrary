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

    module: ansc_asn1_bool.c

        For ASN.1 BOOL object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_BOOL"

        *   AnscAsn1BOOLCreate
        *   AnscAsn1BOOLInitialize
        *   AnscAsn1BOOLClone
        *   AnscAsn1BOOLCopyTo
        *   AnscAsn1BOOLEqualsTo
        *   AnscAsn1BOOLGetSizeOfEncoded
        *   AnscAsn1BOOLDecodingData
        *   AnscAsn1BOOLEncodingData
        *   AnscAsn1BOOLDumpObject
        *   AnscAsn1BOOLTraceObject

        *   AnscAsn1BOOLGetBooleanValue
        *   AnscAsn1BOOLSetBooleanValue

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1BOOLCreate
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
AnscAsn1BOOLCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_BOOL                 pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_BOOL)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_BOOL)AnscAllocateMemory(sizeof(ANSC_ASN1_BOOL));
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
    pMyObject->Oid                 = ANSC_ASN1_BOOL_ID;
    pMyObject->uType               = ASN1_BOOLEAN_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1BOOLCreate;
    pMyObject->AsnFree             = AnscAsn1Free;
    
    AnscAsn1BOOLInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_BOOL_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_BOOL_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BOOLInitialize
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
AnscAsn1BOOLInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1BOOLClone;
    pMyObject->CopyTo               = AnscAsn1BOOLCopyTo;
    pMyObject->EqualsTo             = AnscAsn1BOOLEqualsTo;
    pMyObject->DecodingData         = AnscAsn1BOOLDecodingData;
    pMyObject->EncodingData         = AnscAsn1BOOLEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1BOOLGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1BOOLDumpObject;

    pMyObject->GetBooleanValue      = AnscAsn1BOOLGetBooleanValue;
    pMyObject->SetBooleanValue      = AnscAsn1BOOLSetBooleanValue;

    pMyObject->bValue               = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1BOOLClone
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
AnscAsn1BOOLClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;
    PANSC_ASN1_BOOL                 pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_BOOL)
                 pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1BOOLClone()\n");

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
        AnscAsn1BOOLCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function clones a same ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1BOOLCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;
    PANSC_ASN1_BOOL                 pNewObject   = (PANSC_ASN1_BOOL)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }
  
    pNewObject->bValue              =  pMyObject->bValue;

    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;
   
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1BOOLEqualsTo
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
                Check the value only or not

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1BOOLEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_BOOL                 pMyObject  = (PANSC_ASN1_BOOL)hThisObject;
    PANSC_ASN1_BOOL                 pNewObject = (PANSC_ASN1_BOOL)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1BOOLEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1BOOLEqualsTo().\n");

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
        return (pMyObject->bValue == pNewObject->bValue);
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1BOOLGetSizeOfEncoded
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
AnscAsn1BOOLGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;
    
    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    /*
     *  check the attribute list, from the end of the list;
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, 1, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BOOLDecodingData
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
AnscAsn1BOOLDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;
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
        AnscTrace("Failed to decode the attrlist of BOOL.\n");

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
        if( pTagInfo->FirstOctetOfTag
                != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject))
        {
            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_BOOL - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->ulContentLength != 1 ||
            pTagInfo->GetSizeOfTag(pTagInfo)     != 1 )
        {
            AnscTrace("The length of BOOL tag or Bool value is not == 1,unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;        
        }
    
         /* check the tag */
        if( pTagInfo->bIsInfinite)
        {
            AnscTrace("Infinite form is unacceptable for BOOL object.\n");
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);
        uLeftSize       =  pTagInfo->ulContentLength;

        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    if( uInfiniteTags > 0)
    {
        AnscTrace("Infinite form is unacceptable for BOOL object.\n");

        return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    if( uLeftSize != 1)
    {
        AnscTrace("The length of BOOL value must be 1 , not '%lu\n", uLeftSize);

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;        
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    if( pEncodingUnit[0] != 0)
    {
        pMyObject->bValue           = TRUE;
    }
    else
    {
        pMyObject->bValue           = FALSE;
    }

    pEncodingUnit ++;

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
        AnscAsn1BOOLEncodingData
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
AnscAsn1BOOLEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_BOOL                 pMyObject       = (PANSC_ASN1_BOOL)hThisObject;
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
     *  Encode the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData              = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        *pCharData              = 0x01;
        pCharData++;
    }

   /*
    *   Encode the value;
    */
    if( pMyObject->bValue)
    {
        *pCharData              = 0xFF;
    }
    else
    {
        *pCharData              = 0x00;
    }

    pCharData++;

    /*
     *  Set the buffer before exit;
     */
    if( pCharData - pBackData != uSizeOfEncoded)
    {
        /*
        AnscTrace
            (
                "Warning in Encoding BOOL, the size of encoding is expected to be %d, but it's %d.\n",
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
        AnscAsn1BOOLDumpObject
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
                The object is root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1BOOLDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{

#ifndef _PKI_KERNEL

    PANSC_ASN1_BOOL                 pMyObject       = (PANSC_ASN1_BOOL)hThisObject;
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

    if( !bShowValue)
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
    else if( pMyObject->bOptional)
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
    else if( pMyObject->bValue)
    {
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (TRUE)",
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
                    "%s ::=%s %s (FALSE)",
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
        AnscAsn1BOOLTraceObject
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
AnscAsn1BOOLTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

#ifndef _PKI_KERNEL
    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_BOOL                 pMyObject    = (PANSC_ASN1_BOOL)hThisObject;
    CHAR                            pAttrBuf[512]= { 0 };
    ULONG                           attrLength   = 512;
    PCHAR                           pName;

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

    if( !bShowValue)
    {
        AnscTrace
            (
                "%s ::=%s %s\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else if( pMyObject->bOptional)
    {
        AnscTrace
            (
                "%s ::=%s %s (Optional)\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else if( pMyObject->bValue)
    {
        AnscTrace
            (
                "%s ::=%s %s (TRUE)\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
             );
    }
    else
    {
        AnscTrace
            (
                "%s ::=%s %s (FALSE)\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
             );
    }

#endif

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1BOOLGetBooleanValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the boolean value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the boolean value.

**********************************************************************/
BOOLEAN
AnscAsn1BOOLGetBooleanValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_BOOL                 pMyObject  = (PANSC_ASN1_BOOL)hThisObject;

    return pMyObject->bValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1BOOLSetBooleanValue
            (
                ANSC_HANDLE                 hThisObject,
                BOOLEAN                     bValue
            );

    description:

        This function set the boolean value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOLEAN                     bValue
                The input boolean value;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1BOOLSetBooleanValue
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bValue
    )
{
    PANSC_ASN1_BOOL                 pMyObject  = (PANSC_ASN1_BOOL)hThisObject;

    pMyObject->bValue               = bValue;
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}


