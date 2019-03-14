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

    module: ansc_asn1_export_utils.h

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


#ifndef  _ANSC_ASN1_EXPORT_UTILS_
#define  _ANSC_ASN1_EXPORT_UTILS_


/**************************************************************

  ATTRIBUTE RELATED UTILITY FUNCTIONS

  IMPLEMENTED IN "ANSC_ASN1_EXPORT_UTILS.C"

  *************************************************************/
PSINGLE_LINK_ENTRY
GetSingleLinkEntryByIndex
    (
        PSLIST_HEADER               pListHeader,
        LONG                        index
    );

void
AttrListRemoveAllAttributes
    (
        PSLIST_HEADER               pListHeader
    );

void
AttrListCopyAllAttributes
    (
        PSLIST_HEADER               pDestListHeader,
        PSLIST_HEADER               pSourceListHeader
    );

BOOLEAN
AttrListCompareAttributeList
    (
        PSLIST_HEADER               pDestListHeader,
        PSLIST_HEADER               pSourceListHeader
    );

LONG
AttrListGetSizeOfEncoded
    (
        PSLIST_HEADER               pListHeader,
        ULONG                       uValueLength,
        ULONG                       uTags
    );

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

ANSC_STATUS
AttrListDecodingData
    (
        PSLIST_HEADER               pListHeader,
        PVOID*                      ppEncoding,
        BOOLEAN                     bIsConstructive,
        PBOOLEAN                    pTagInclued,
        PULONG                      pLeftSize,
        PULONG                      pInfiniteTag
    );

BOOLEAN
AttrListDump
    (
        PSLIST_HEADER               pListHeader,
        PCHAR                       pBuffer,
        PULONG                      pLength
    );

BOOLEAN
AnscAsn1GetCurrentTime
    (
        ANSC_HANDLE                 hTime
    );

BOOLEAN
AnscIsReadableString
    (
        PUCHAR                      pEncoding,
        ULONG                       length
    );

#endif /*_ANSC_ASN1_EXPORT_UTILS*/
