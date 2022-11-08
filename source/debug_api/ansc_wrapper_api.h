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

    module:	ansc_wrapper_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the container.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/01/2001    initial revision.
        08/09/2010    Bin added ipv6 related apis.

**********************************************************************/


#ifndef  _ANSC_WRAPPER_API_
#define  _ANSC_WRAPPER_API_


/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_SLIST.C
***********************************************************/

ANSC_FORCEINLINE
PSINGLE_LINK_ENTRY
AnscSListPopEntry
    (
        PSLIST_HEADER               ListHead
    )
{
    PSINGLE_LINK_ENTRY              FirstEntry;

    FirstEntry = ListHead->Next.Next;
    if ( FirstEntry == NULL )
    {
        return NULL;
    }
    ListHead->Next.Next = FirstEntry->Next;
    ListHead->Depth--;
    return  FirstEntry;
}

BOOLEAN
AnscSListPopEntryByLink
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry
    );

VOID
AnscSListReplaceEntry
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          NewEntry,
        PSINGLE_LINK_ENTRY          OldEntry
    );

PSINGLE_LINK_ENTRY
AnscSListGetLastEntry
    (
        PSLIST_HEADER               ListHead
    );

PSINGLE_LINK_ENTRY
AnscSListPushEntryFixNumber
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Max
    );

PSINGLE_LINK_ENTRY
AnscSListSearchEntryByIndex
    (
        PSLIST_HEADER               ListHead,
        ULONG                       Index
    );

PSINGLE_LINK_ENTRY
AnscSListPushEntryByIndex
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Index
    );

/***********************************************************
 *             FUNCTIONS IMPLEMENTED IN ANSC_HASH.C
 ************************************************************/

ULONG
AnscHashAddress
    (
        PUCHAR                      address,
        ULONG                       ulTableSize
    );

ANSC_FORCEINLINE
ULONG
AnscHashUshort
    (
        USHORT                      usValue,
        ULONG                       ulTableSize
    )
{
    return  (ULONG)usValue % ulTableSize;
}

ANSC_FORCEINLINE
ULONG
AnscHashUlong   
    (
        ULONG                       ulValue,
        ULONG                       ulTableSize
    )
{
    return  ulValue % ulTableSize;
}

ULONG
AnscHashString
    (
        char*                       string,
        ULONG                       ulStringSize,
        ULONG                       ulTableSize
    );

ULONG
AnscHashString2
    (
        char*                       string,
        ULONG                       ulStringSize,
        ULONG                       ulTableSize
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_STRING.C
***********************************************************/

void
AnscFromHexToString
    (
        PUCHAR                      hex,
        char*                       string,
        ULONG                       ulSize,
        BOOL                        bLowerCase
    );

void
AnscStringFromLToU
    (
        char*                       lstring,
        char*                       ustring,
        ULONG                       ulSize
    );

void
AnscStringFromUToL
    (
        char*                       ustring,
        char*                       lstring,
        ULONG                       ulSize
    );

ULONG
AnscSizeOfToken
    (
        char*                       string,
        char*                       separator,
        ULONG                       range
    );

ULONG
AnscSizeOfToken2
    (
        char*                       string,
        char*                       alphabet,
        ULONG                       range
    );

ULONG
AnscSizeOfToken3
    (
        char*                       string,
        char*                       separator,
        ULONG                       range
    );

char*
AnscMoveToNextToken
    (
        char*                       string,
        char*                       separtor
    );

char*
AnscMoveToNextToken2
    (
        char*                       string,
        char*                       alphabet
    );

void
AnscGetUlongString
    (
        ULONG                       ulValue,
        char*                       ulong_str
    );

ULONG
AnscGetStringUlong
    (
        char*                       ulong_str
    );

ULONG
AnscGetStringUlongHex
    (
        char*                       ulong_str_hex
    );


char*
AnscCloneString
    (
        char*                       src_string
    );


#if 0
BOOL
AnscIsValidIpString
    (
        char*                       ip_addr_string
    );
#endif
BOOL
AnscIsValidIp6String
    (
        char*                       ip6_addr_string
    );

BOOL
AnscParseIp6Address
    (
        char*                       ip6_addr_string,
        unsigned char*              ipAddrArray
    );

char*
AnscIp6AddressToString
    (
        unsigned char*              ipAddrArray
    );

/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_TIME.C
***********************************************************/

ULONG
AnscGetTimeIntervalInSeconds
    (
        ULONG                       ulOld,
        ULONG                       ulNew
    );


ULONG
AnscCalendarToSecond
    (
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscSecondToCalendar
    (
        ULONG                       ulSeconds,
        ANSC_HANDLE                 hCalendar
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_MEMORY.C
***********************************************************/

void*
AnscAllocateMemory2
    (
        ULONG                       ulMemorySize
    );

void*
AnscReAllocateMemory2
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    );

void
AnscFreeMemory2
    (
        PVOID                       pMemoryBlock
    );

ULONG
AnscGetMemorySize2
    (
        PVOID                       pMemoryBlock
    );

void*
AnscAllocateMemory3
    (
        ULONG                       ulMemorySize
    );

void*
AnscReAllocateMemory3
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    );

void
AnscFreeMemory3
    (
        PVOID                       pMemoryBlock
    );

ULONG
AnscGetMemorySize3
    (
        PVOID                       pMemoryBlock
    );

void*
AnscAllocateMemory4
    (
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

void*
AnscReAllocateMemory4
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

void
AnscFreeMemory4
    (
        PVOID                       pMemoryBlock
    );

ULONG
AnscGetMemorySize4
    (
        PVOID                       pMemoryBlock
    );

void*
AnscAllocateMemory5
    (
        char*                       pComponentName,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

void*
AnscReAllocateMemory5
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

void
AnscFreeMemory5
    (
        PVOID                       pMemoryBlock
    );

ULONG
AnscGetMemorySize5
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock
    );

void*
AnscAllocateMemory6
    (
        ULONG                       ulMemorySize
    );

void*
AnscReAllocateMemory6
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    );

void
AnscFreeMemory6
    (
        PVOID                       pMemoryBlock
    );

ULONG
AnscGetMemorySize6
    (
        PVOID                       pMemoryBlock
    );

void
AnscTraceMemoryUsage
    (
        void
    );

char*
AnscTraceMemoryUsage2
    (
        ULONG                       ulTraceMask,
        char*                       pOwnerDesp
    );

void
AnscInitializeMemoryAllocTable
    (
        void
    );

void
AnscAddMemoryAllocItem
    (
	char*                       pComponentName,
        void*                       pointer,
        ULONG                       size,
        ULONG                       alloc_id,
        char*                       owner_desp
    );

ANSC_HANDLE
AnscGetMemoryAllocItem
    (
        void*                       pointer
    );

void
AnscDelMemoryAllocItem
    (
        void*                       pointer
    );

void
AnscTraceMemoryTable
    (
        void
    );

char**
AnscTraceMemoryTable2
    (
        ULONG                       ulTraceMask,
        char*                       pOwnerDesp,
        PULONG                      pulBlockCount
    );

ULONG
AnscGetNextMemoryBlockId
    (
    );

void
AnscTraceMemoryTableFromBlockId
    (
        ULONG                       ulBlockIdFrom
    );

void
AnscExpand2
    (
        PVOID                       i_buffer,
        PVOID                       o_buffer,
        int                         n_bytes,
        BOOL                        b_ljustified
    );

void
AnscMemorySetTimeMarker
    (
        void
    );

void
AnscInitializeMemoryPoolTable
    (
        void
    );

ANSC_HANDLE
AnscGetMemoryPool
    (
        ULONG                       ulBlockSize
    );

void
AnscAddMemoryPool
    (
        ULONG                       ulBlockSize,
        ULONG                       ulMinFreeMemorySize
    );

void
AnscDelMemoryPool
    (
        ULONG                       ulBlockSize
    );

void
AnscClrMemoryPool1
    (
        ULONG                       ulBlockSize,
        ULONG                       ulNumberOfBlocks
    );

void
AnscClrMemoryPool2
    (
        ANSC_HANDLE                 hMemoryPool,
        ULONG                       ulNumberOfBlocks
    );

void
AnscDelAllMemoryPools
    (
        void
    );

#endif
