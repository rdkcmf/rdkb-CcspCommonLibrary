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

    module: ansc_asn1_export_utils.c

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file implements the utility functions for 
        the general ASN.1 Component Objects.

        *   GetSingleLinkEntryByIndex
        *   AttrListRemoveAllAttributes
        *   AttrListCopyAllAttributes
        *   AttrListCompareAttributeList
        *   AttrListGetSizeOfEncoded
        *   AttrListEncodingData
        *   AttrListDecodingData
        *   AttrListTrace
        *   AttrListDump

        *   AnscAsn1GetCurrentTime
        *   AnscIsReadableString

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/09/02    initial revision.
        05/27/02    AnscAsn1GetCurrentTime was added;
        11/02/02    AnscIsReadableString was added;

**********************************************************************/

#include "ansc_asn1_local.h"

#include "ansc_ntpco_interface.h"
#include "ansc_ntpco_external_api.h"
#include "ansc_smtco_interface.h"
#include "ansc_smtco_external_api.h"
#include "safec_lib_common.h"

/**************************************************************
      ATTRIBUTE RELATED UTILITY FUNCTIONS
 *************************************************************/

/****************************************************************

    PSINGLE_LINK_ENTRY
    GetSingleLinkEntryByIndex
        (
            PSLIST_HEADER               pListHeader,
            LONG                        index
        )

    This routine returns the link entry by index;

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

                LONG                        index
                The index of the entry

      return:   the single list entry;

*****************************************************************/
PSINGLE_LINK_ENTRY
GetSingleLinkEntryByIndex
    (
        PSLIST_HEADER               pListHeader,
        LONG                        index
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry;
    ULONG                           depth;
    ULONG                           i       = 0;

    if( pListHeader == NULL)
    {
        return NULL;
    }

    depth = AnscSListQueryDepth(pListHeader);

    if( depth <= (ULONG)index || index < 0)
    {
        return NULL;
    }

    pSListEntry =   AnscSListGetFirstEntry(pListHeader);

    while( pSListEntry != NULL)
    {
        if( i == (ULONG)index)
        {
            break;
        }

        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        i ++;
    }

    return pSListEntry;
}

/****************************************************************

    void
    AttrListRemoveAllAttributes
        (
            PSLIST_HEADER               pListHeader
        )

    This routine removes all the attribute in the list

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

      return:   nothing

*****************************************************************/
void
AttrListRemoveAllAttributes
    (
        PSLIST_HEADER               pListHeader
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry;
    PANSC_ATTR_OBJECT               pAttr;

    if( pListHeader == NULL)
    {
        return;
    }

    pSListEntry =   AnscSListGetFirstEntry(pListHeader);

    while( pSListEntry != NULL)
    {
        pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        if( pAttr != NULL)
        {
            pAttr->Remove(pAttr);
        }              
    }

    /*
     *  reset the SList;
     */
    AnscSListInitializeHeader(pListHeader);      
}

/****************************************************************

    void
    AttrListCopyAllAttributes
        (
            PSLIST_HEADER               pDestListHeader,
            PSLIST_HEADER               pSourceListHeader
        )

    This routine copies all the attributes from the other list

    argument:   PSLIST_HEADER               pDestListHeader
                The header of the destination list;

                PSLIST_HEADER               pSourceListHeader
                The header of the source list;

      return:   nothing

*****************************************************************/
void
AttrListCopyAllAttributes
    (
        PSLIST_HEADER               pDestListHeader,
        PSLIST_HEADER               pSourceListHeader
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry;
    PANSC_ATTR_OBJECT               pAttr, pCloneAttr;


    if( pDestListHeader == NULL || pSourceListHeader == NULL)
    {
        return;
    }

    /*
     *  Remove all the exists  object;
     */
     AttrListRemoveAllAttributes(pDestListHeader);

    /*
     *  Copy from the source one by one;
     */
    pSListEntry =   AnscSListGetFirstEntry(pSourceListHeader);

    while( pSListEntry != NULL)
    {
        pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        if( pAttr != NULL)
        {
            pCloneAttr = pAttr->Clone(pAttr);

            if( pCloneAttr != NULL)
            {
                AnscSListPushEntryAtBack(pDestListHeader, &pCloneAttr->Linkage);
            }
        }              
    }
}


/****************************************************************

    BOOLEAN
    AttrListCompareAttributeList
        (
            PSLIST_HEADER               pListHeader1,
            PSLIST_HEADER               pListHeader2
        )

    This routine compares all the attributes in the 2 list.

    argument:   PSLIST_HEADER               pListHeader1
                The first listheader;

                PSLIST_HEADER               pListHeader2
                The second listheader;

      return:   nothing

*****************************************************************/
BOOLEAN
AttrListCompareAttributeList
    (
        PSLIST_HEADER               pListHeader1,
        PSLIST_HEADER               pListHeader2
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry1;
    PSINGLE_LINK_ENTRY              pSListEntry2;
    PANSC_ATTR_OBJECT               pAttr1;
    PANSC_ATTR_OBJECT               pAttr2;

    if( pListHeader1 == NULL || pListHeader2 == NULL)
    {
        return (pListHeader1 == pListHeader2);
    }

    if( AnscSListQueryDepth(pListHeader1) != AnscSListQueryDepth(pListHeader2))
    {
        return FALSE;
    }

    pSListEntry1 =   AnscSListGetFirstEntry(pListHeader1);
    pSListEntry2 =   AnscSListGetFirstEntry(pListHeader2);

    while( pSListEntry1 != NULL && pSListEntry2 != NULL)
    {
        pAttr1       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry1);
        pAttr2       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry2);

        pSListEntry1 = AnscSListGetNextEntry(pSListEntry1);
        pSListEntry2 = AnscSListGetNextEntry(pSListEntry2);

        if( !pAttr1->EqualsTo(pAttr1, pAttr2))
        {
            return FALSE;
        }              
    }

    if( pSListEntry1 != pSListEntry2)
    {
        return FALSE;
    }

    return TRUE;
}


/****************************************************************

    LONG
    AttrListGetSizeOfEncoded
        (
            PSLIST_HEADER               pListHeader,
            ULONG                       uValueLength,
            ULONG                       uTags
        );

    This routine returns the encoding length of the object.

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

                ULONG                       uValueLength,
                The value length of the object;

                ULONG                       uTags
                The tag length of the object;

      return:   The size of the encoded data;

*****************************************************************/
LONG
AttrListGetSizeOfEncoded
    (
        PSLIST_HEADER               pListHeader,
        ULONG                       uValueLength,
        ULONG                       uTags
    )
{
    LONG                            length, i;
    PANSC_ATTR_OBJECT               pAttr;
    PSINGLE_LINK_ENTRY              pSListEntry;
    ULONG                           tagSize      = uTags;    

    if( pListHeader == NULL)
    {

        return -1;
    }

    length = uValueLength;

    for( i = AnscSListQueryDepth(pListHeader) - 1; i >= 0; i --)
    {
        pSListEntry = 
            GetSingleLinkEntryByIndex
                (
                    pListHeader,
                    i
                );

        pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);

        if( pAttr == NULL)
        {
            break;
        }
        
        if( pAttr->FormType == EMPTY_FORM)
        {
            continue;
        }

        if( !pAttr->bImplicit && tagSize > 0)
        {
            length += GetSizeOfEncodedLength(length);
            length += tagSize;
        }

        tagSize = pAttr->GetSizeOfEncodedTag(pAttr);
    }

    if( tagSize > 0)
    {
        length += GetSizeOfEncodedLength(length);
        length += tagSize;
    }

    return length;
}

/****************************************************************

    ANSC_STATUS
    AttrListEncodingData
        (
            PSLIST_HEADER               pListHeader,
            PVOID*                      ppEncoding,
            ULONG                       uSizeOfEncoded,
            BOOLEAN                     bIsConstructive,
            PBOOLEAN                    pTagInclued,
            PULONG                      pLeftSize
        );

    This routine encoding the data of the attribute list

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

                PVOID*                      ppEncoding,
                The output buffer of the encoding data;

                ULONG                       uSizeOfEncoded,
                The size of the encoded data;

                BOOLEAN                     bIsConstructive,
                The object is constructive or not;

                PBOOLEAN                    pTagInclued
                The output buffer of the last attribute 
                (explicit or implicit);

                PULONG                      pLeftSize
                The output buffer of the left size;
   

      return:   The status of the operation;

*****************************************************************/
ANSC_STATUS
AttrListEncodingData
    (
        PSLIST_HEADER               pListHeader,
        PVOID*                      ppEncoding,
        ULONG                       uSizeOfEncoded,
        BOOLEAN                     bIsConstructive,
        PBOOLEAN                    pTagIncluded,
        PULONG                      pLeftSize
    )
{
    PANSC_ATTR_OBJECT               pAttrObject;
    PSINGLE_LINK_ENTRY              pSListEntry;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    BOOLEAN                         bTag         = TRUE;
    ULONG                           uleftSize    = uSizeOfEncoded;
    LONG                            pureLength;
    ULONG                           i;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData    = NULL;

    
    if( pListHeader  == NULL || ppEncoding == NULL || 
        pTagIncluded == NULL || pLeftSize  == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    /*RDKB-6197, CID-24466, assign after null check*/
    pCharData = (PUCHAR)*ppEncoding;

    /*
     *  encode all the attributes one by one;
     */
    for( i = 0; i < AnscSListQueryDepth(pListHeader); i ++)
    {
        pSListEntry = GetSingleLinkEntryByIndex(pListHeader, i);

        pAttrObject = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);

        if( pAttrObject == NULL)
        {
            break;
        }

        if( pAttrObject->FormType == EMPTY_FORM)
        {
            continue;
        }

        if( bTag == FALSE)
        {
            bTag = (BOOLEAN)!pAttrObject->bImplicit;

            continue;
        }

        /*
         * encode the attribute;
         */
        returnStatus = 
            pAttrObject->EncodingData
                    (
                        pAttrObject,    
                        (PVOID*)&pCharData,
                        bIsConstructive
                    );

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }

        uleftSize -= pAttrObject->GetSizeOfEncodedTag(pAttrObject);

        pureLength = GetPureEncodedLength(uleftSize);

        if( pureLength < 0)
        {
            AnscTrace("The encoding length is incorrect.\n");
            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }

        uleftSize = pureLength;

        returnStatus = EncodeLength((PVOID*)&pCharData, uleftSize);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }

        /*
         *  The next tag is needed or not;
         */
        bTag = (BOOLEAN)!pAttrObject->bImplicit;
    }

    /*
     *  set the output values;
     */
    *pTagIncluded   = bTag;
    *pLeftSize      = uleftSize;
    *ppEncoding     = (PVOID)pCharData;

    return returnStatus;
}

/****************************************************************


    ANSC_STATUS
    AttrListDecodingData
        (
            PSLIST_HEADER               pListHeader,
            PVOID*                      ppEncoding,
            BOOLEAN                     bIsConstructive,
            PBOOLEAN                    pTagInclued,
            PULONG                      pLeftSize,
            PULONG                      pInfiniteTag
        )

    This routine encoding the data of the attribute list

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

                PVOID*                      ppEncoding,
                The input buffer of the encoding data;

                BOOLEAN                     bIsConstructive,
                The object is constructive or not;

                PBOOLEAN                    pTagInclued
                The output buffer of the last attribute 
                (explicit or implicit);

                PULONG                      pLeftSize
                The output buffer of the left size;
   
                PULONG                      pInfiniteTag
                The sum buffer of infinite tags;

      return:   The status of the operation;

*****************************************************************/

ANSC_STATUS
AttrListDecodingData
    (
        PSLIST_HEADER               pListHeader,
        PVOID*                      ppEncoding,
        BOOLEAN                     bIsConstructive,
        PBOOLEAN                    pTagIncluded,
        PULONG                      pLeftSize,
        PULONG                      pInfiniteTag
    )
{
    UNREFERENCED_PARAMETER(bIsConstructive);
    PANSC_ATTR_OBJECT               pAttrObject, pAttrNew;
    PSINGLE_LINK_ENTRY              pSListEntry;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    BOOLEAN                         bTag         = TRUE;
    ULONG                           uleftSize    = 0;
    ULONG                           i, uInfinite = 0;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pEncodingUnit= NULL;

    if( ppEncoding   == NULL || pListHeader == NULL ||
        pTagIncluded == NULL || pLeftSize   == NULL ||
        pInfiniteTag == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    /*RDKB-6197, CID-24117, assign after NULL check*/
    pEncodingUnit= (PUCHAR)*ppEncoding;

    pTagInfo   = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

    if( pTagInfo == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    /*
     *  decode all the attributes one by one;
     */
    if( AnscSListQueryDepth(pListHeader) > 0)
    {
        for( i = 0; i < AnscSListQueryDepth(pListHeader); i ++)
        {
            pSListEntry = GetSingleLinkEntryByIndex(pListHeader, i);

            pAttrObject = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);

            if( pAttrObject == NULL)
            {
                break;
            }

            if( pAttrObject->FormType == EMPTY_FORM)
            {
                continue;
            }

            if( bTag == FALSE)
            {
                bTag = (BOOLEAN)!pAttrObject->bImplicit;

                continue;
            }

            /*
             * decode the attribute;
             */
            returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);

            if( returnStatus != ANSC_STATUS_SUCCESS)
            {
                pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

                return returnStatus;
            }
        
            /*
             *  check the tag;
             */
            pAttrNew = AnscAsn1AttrParsingData( pEncodingUnit);
        
            if( pAttrNew == NULL)
            {
                pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

                AnscTrace("Failed to parse the data in AttributeList decoding.\n");

                return ANSC_ASN1_UNEXPECTED_TAG;
            }

            if( pAttrNew->FormType      != pAttrObject->FormType ||
                pAttrNew->ulClassNumber != pAttrObject->ulClassNumber)
            {
                /*
                AnscTrace("WARNING: ");
                pAttrObject->Trace(pAttrObject);
                AnscTrace(" is expected, but it's ");
                pAttrNew->Trace(pAttrNew);
                AnscTrace("\n");
                */
            
                pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
                pAttrNew->Remove(pAttrNew);

                return ANSC_ASN1_UNEXPECTED_TAG;    
            }
        
            /* free the new attribute */
            pAttrNew->Remove(pAttrNew);

            pEncodingUnit += 
                pTagInfo->GetSizeOfTag(pTagInfo) +
                pTagInfo->GetSizeOfLength(pTagInfo);

            if( pTagInfo->bIsInfinite)
            {
                uInfinite ++;
                uleftSize = 0;
            }
            else if( uleftSize == 0)
            {
                uleftSize = pTagInfo->ulContentLength;
            }
            else 
            /* check the size correct or not */
            {
                uleftSize -= 
                    pTagInfo->GetSizeOfTag(pTagInfo) +
                    pTagInfo->GetSizeOfLength(pTagInfo);

                if( uleftSize != (ULONG)pTagInfo->ulContentLength)
                {
                    AnscTrace
                        (
                            "The expected length is %lu, but actually it's %lu\n",
                            uleftSize,
                            pTagInfo->ulContentLength
                        );

                    pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

                    return ANSC_ASN1_INVALID_ENCODE_LENGTH;
                }
            }

            /*
             *  The next tag is needed or not;
             */
            bTag = (BOOLEAN)!pAttrObject->bImplicit;
        }
    }
    else
    {
        /* do nothing here */
    }
    /*
     *  Free the tag info;
     */
    pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

    /*
     *  set the output values;
     */
    *pTagIncluded   = bTag;
    *pLeftSize      = uleftSize;
    *pInfiniteTag   = uInfinite;
    *ppEncoding     = (PVOID)pEncodingUnit;

    return returnStatus;
}

/****************************************************************

    BOOLEAN
    AttrListDump
        (
            PSLIST_HEADER               pListHeader,
            PCHAR                       pBuffer,
            PULONG                      pLength
        );

    This routine dumps all the attribute to the buffer;

    argument:   PSLIST_HEADER               pListHeader
                The header of the list;

                PCHAR                       pBuffer
                The output buffer;

                PULONG                      pLength
                The buffer of the size;

      return:   If it's empty list, return false;
                if there's not enough buffer or bad parameters,
                return false; others return true;

*****************************************************************/
BOOLEAN
AttrListDump
    (
        PSLIST_HEADER               pListHeader,
        PCHAR                       pBuffer,
        PULONG                      pLength
    )
{

    PSINGLE_LINK_ENTRY              pSListEntry;
    PANSC_ATTR_OBJECT               pAttr;
    CHAR                            pTmpBuffer[512] = { 0 };
    ULONG                           lenOfBuffer     = 0;
    ULONG                           uSingleLen      = 512;
    errno_t                         rc              = -1;

    if( pListHeader == NULL || AnscSListQueryDepth(pListHeader) == 0 ||
        pBuffer     == NULL || pLength == NULL )
    {
        return FALSE;
    }

    pSListEntry =   AnscSListGetFirstEntry(pListHeader);

    while( pSListEntry != NULL)
    {
        pAttr       = ACCESS_ANSC_ATTR_OBJECT(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        if( pAttr != NULL)
        {
            pAttr->Dump
                (
                    pAttr,
                    pTmpBuffer + lenOfBuffer,
                    &uSingleLen    
                );

            lenOfBuffer += uSingleLen;

            uSingleLen  =  512 - lenOfBuffer;

            if( uSingleLen < 16)
            {
                break;
            }
        }              
    }

    if( *pLength < lenOfBuffer)
    {
        return FALSE;
    }

    *pLength = lenOfBuffer;

    rc = strcpy_s( pBuffer, *pLength, pTmpBuffer);
    ERR_CHK(rc);

    return TRUE;
}

BOOLEAN
AnscAsn1GetCurrentTime
    (
        ANSC_HANDLE                 hTime
    )
{

    AnscGetSystemTime(hTime);

   return TRUE;
}

BOOLEAN
AnscIsReadableString
    (
        PUCHAR                      pEncoding,
        ULONG                       length
    )
{
    ULONG                           i;
    UCHAR                           c;

    if( pEncoding == NULL || length == 0 || pEncoding[0] == 0x00)
    {
        return FALSE;
    }

    /* readable char is from 0x20 to 0x7E */
    for( i = 0; i < length; i ++)
    {
        c = pEncoding[i];

        if( c == 0x00)
        {
            continue;
        }

        if( c < 0x20 || c > 0x7E)
        {
            return FALSE;
        }
    }

    return TRUE;
}
