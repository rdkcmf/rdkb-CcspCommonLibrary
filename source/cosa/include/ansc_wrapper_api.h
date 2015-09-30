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

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

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
            FUNCTIONS IMPLEMENTED IN ANSC_TASK.C
***********************************************************/

void
AnscTaskRoutine1
    (
        void*                       runtime_record
    );

void
AnscTaskRoutine2
    (
        void*                       runtime_record
    );

void
AnscTaskJanitorRoutine
    (
        void*                       runtime_record
    );

#ifndef _ANSC_DEBUG_TASK_

ANSC_STATUS
AnscSpawnTask
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    );

ANSC_STATUS
AnscSpawnTask2
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority
    );

ANSC_STATUS
AnscSpawnTask3
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority,
        ULONG                       stack_size
    );

#else /* _ANSC_DEBUG_TASK_ */

ANSC_STATUS
AnscSpawnTask3Trace
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority,
        ULONG                       stack_size,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscSpawnTask(t, c, n)      \
            AnscSpawnTask3Trace(t, c, n, ANSC_TASK_PRIORITY_NORMAL, ANSC_DEFAULT_TASK_STACK_SIZE, __FILE__, __LINE__)

#define AnscSpawnTask2(t, c, n, p)  \
            AnscSpawnTask3Trace(t, c, n, p, ANSC_DEFAULT_TASK_STACK_SIZE, __FILE__, __LINE__)

#define AnscSpawnTask3(t, c, n, p, s)   AnscSpawnTask3Trace(t, c, n, p, s, __FILE__, __LINE__)

#endif /* _ANSC_DEBUG_TASK_ */

ANSC_STATUS
AnscEraseTask
    (
        ANSC_HANDLE                 hTaskRecord
    );

ANSC_STATUS
AnscInitializeTpm
    (
        ULONG                       ulPoolSize
    );

ANSC_STATUS
AnscInitializeTpj
    (
        ULONG                       ulPoolSize
    );

ANSC_HANDLE
AnscAcquireTask
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    );

ANSC_STATUS
AnscReleaseTask
    (
        ANSC_HANDLE                 hTaskRecord
    );

ANSC_STATUS
AnscInitializeTroTable
    (
        void
    );

ANSC_STATUS
AnscUnloadTroTable
    (
        void
    );

ANSC_HANDLE
AnscGetCurTaskRecord
    (
        void
    );

ANSC_HANDLE
AnscGetTaskRecord
    (
        ULONG                       ulTaskHandle
    );

#ifndef _ANSC_DEBUG_TASK_

ANSC_HANDLE
AnscAddTaskRecord
    (
        PANSC_TASK_RECORD           PTaskRecord
    );

#else

ANSC_HANDLE
AnscAddTaskRecordTrace
    (
        PANSC_TASK_RECORD           PTaskRecord,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscAddTaskRecord(n, p, t, s)   AnscAddTaskRecordTrace(n, p, t, s, __FILE__, __LINE__)

#endif /* _ANSC_DEBUG_TASK_ */

ANSC_HANDLE
AnscDelTaskRecord
    (
        ULONG                       ulTaskHandle
    );

ULONG
AnscGetActiveTaskCount
    (
        void
    );

ANSC_STATUS
AnscWaitAllTasks
    (
        void
    );

ANSC_STATUS
AnscKillAllTasks
    (
        void
    );

ULONG
AnscGetChildTaskCount
    (
        ULONG                       ulTaskParent
    );

ANSC_STATUS
AnscWaitAllChildTasks
    (
        ULONG                       ulTaskParent
    );

ULONG
AnscGetParentTaskHandle
    (
        void
    );

ANSC_HANDLE
AnscGetTaskStorageUnit
    (
        char*                       unit_name
    );

ANSC_STATUS
AnscAddTaskStorageUnit
    (
        char*                       unit_name,
        ANSC_HANDLE                 unit_context
    );

ANSC_STATUS
AnscDelTaskStorageUnit
    (
        char*                       unit_name
    );


#ifdef _ANSC_DEBUG_TASK_

ANSC_STATUS
AnscPrintTaskList
    (
        ULONG                       param
    );

ANSC_STATUS
AnscEnableTask
    (
        ULONG                       tid,
        int                         bEnable
    );

ANSC_STATUS
AnscCommitTask
    (
        void
    );

#define ANSC_COMMIT_TASK()          AnscCommitTask()

#else

#define ANSC_COMMIT_TASK()

#endif /* _ANSC_DEBUG_TASK_ */

/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_FILE_IO.C
***********************************************************/

ANSC_HANDLE
AnscCreateFile
    (
        char*                       name,
        ULONG                       mode,
        ULONG                       type
    );

ANSC_HANDLE
AnscOpenFile
    (
        char*                       name,
        ULONG                       mode,
        ULONG                       type
    );

ANSC_STATUS
AnscCloseFile
    (
        ANSC_HANDLE                 hFile
    );

ANSC_STATUS
AnscReadFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscWriteFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscSeekFile
    (
        ANSC_HANDLE                 hFile,
        ULONG                       ulOffset,
        int                         iPos
    );

ANSC_STATUS
AnscCopyFile
    (
        char*                       pSrcFile,
        char*                       pDstFile,
        BOOL                        bOverwriteDst
    );

ANSC_STATUS
AnscDeleteFile
    (
        char*                       file_name
    );

ANSC_STATUS
AnscRenameFile
    (
        char*                       old_file_name,
        char*                       new_file_name
    );

ULONG
AnscGetFileSize
    (
        ANSC_HANDLE                 hFile
    );

char*
AnscGetFileExt
    (
        ANSC_HANDLE                 hFile
    );

char*
AnscGetFileExt2
    (
        char*                       file_name
    );

ANSC_HANDLE
AnscGetFileProperty
    (
        char*                       file_name
    );

ANSC_STATUS
AnscGetMimeType
    (
        ANSC_HANDLE                 hFile,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    );

ANSC_STATUS
AnscCreateDirectory
    (
        char*                       dir_name
    );

ANSC_STATUS
AnscDeleteDirectory
    (
        char*                       dir_name
    );

ANSC_STATUS
AnscCopyDirectory
    (
        char*                       src_dir,
        char*                       dst_dir
    );

ANSC_STATUS
AnscMoveFile
    (
        char*                       src_file,
        char*                       dst_file
    );

#ifdef  _ANSC_FILE_GZIP_

ANSC_HANDLE
AnscCreateGzipFile
    (
        char*                       name,
        ULONG                       mode
    );

ANSC_HANDLE
AnscOpenGzipFile
    (
        char*                       name,
        ULONG                       mode
    );

ANSC_STATUS
AnscCloseGzipFile
    (
        ANSC_HANDLE                 hFile
    );

ANSC_STATUS
AnscReadGzipFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscWriteGzipFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscSeekGzipFile
    (
        ANSC_HANDLE                 hFile,
        ULONG                       ulOffset,
        int                         iPos
    );

ULONG
AnscGetGzipFileSize
    (
        ANSC_HANDLE                 hFile
    );

#endif


#ifdef  _ANSC_FILE_SEARCH_

ANSC_HANDLE
AnscSearchFirstFile
    (
        char*                       dir_name,
        char*                       target_file_name,   /* can be wildcard, cannot be NULL */
        PBOOL                       pbDirectory,
        char*                       first_file_name     /* buffer size must large enough - 260 bytes */
    );

BOOL
AnscSearchNextFile
    (
        ANSC_HANDLE                 hSearchContext,
        PBOOL                       pbDirectory,
        char*                       file_name
    );

void
AnscCloseSearch
    (
        ANSC_HANDLE                 hSearchContext
    );

#endif


/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_HASH.C
***********************************************************/

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
            FUNCTIONS IMPLEMENTED IN ANSC_QUEUE.C
***********************************************************/

ANSC_FORCEINLINE
PSINGLE_LINK_ENTRY
AnscQueuePopEntry
    (
        PQUEUE_HEADER               QueueHead
    )
{
    PSINGLE_LINK_ENTRY              FirstEntry;

    FirstEntry = (QueueHead)->Next.Next;
    if ( FirstEntry == NULL )
    {
        return NULL;
    }
    QueueHead->Next.Next = FirstEntry->Next;
    QueueHead->Depth--;
    if ( FirstEntry->Next == NULL)
    {
        QueueHead->Last.Next = NULL;
    }
    return  FirstEntry;
}


BOOLEAN
AnscQueuePopEntryByLink
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    );

void
AnscQueueMoveEntryToStart
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    );

void
AnscQueueMoveEntryToEnd
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    );

PSINGLE_LINK_ENTRY
AnscQueueSearchEntryByIndex
    (
        PQUEUE_HEADER               QueueHead,
        ULONG                       Index
    );

PSINGLE_LINK_ENTRY
AnscQueuePopEntryByIndex
    (
        PQUEUE_HEADER               QueueHead,
        ULONG                       Index
    );

PSINGLE_LINK_ENTRY
AnscQueuePushEntryFixNumber
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Max
    );

BOOLEAN
AnscQueuePushEntryBefore
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        PSINGLE_LINK_ENTRY          beforeEntry
    );

VOID
AnscQueuePushEntryAfter
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        PSINGLE_LINK_ENTRY          afterEntry
    );


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
            FUNCTIONS IMPLEMENTED IN ANSC_DLIST.C
***********************************************************/

PDOUBLE_LINK_ENTRY
AnscDListGetEntryByIndex
    (
        PDLIST_HEADER               pDListHeader,
        ULONG                       ulIndex
    );

void
AnscDListPushEntryLeft
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    );

void
AnscDListPushEntryRight
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    );

void
AnscDListPushEntryAtIndex
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry,
        ULONG                       ulIndex
    );

PDOUBLE_LINK_ENTRY
AnscDListPushEntryLeftFixNumber
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry,
        ULONG                       Max
    );

PDOUBLE_LINK_ENTRY
AnscDListPopEntryLeft
    (
        PDLIST_HEADER               pDListHeader
    );

PDOUBLE_LINK_ENTRY
AnscDListPopEntryRight
    (
        PDLIST_HEADER               pDListHeader
    );

PDOUBLE_LINK_ENTRY
AnscDListPopEntryByIndex
    (
        PDLIST_HEADER               pDListHeader,
        ULONG                       ulIndex
    );

BOOLEAN
AnscDListPopEntryByLink
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    );

/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_STREE.C
***********************************************************/

ANSC_HANDLE
AnscStreeAllocateNode
    (
        PSTREE_NODE_INFO            pNodeInfo
    );

ANSC_STATUS
AnscStreeFreeNode
    (
        PSTREE_LINK_ENTRY           pStreeNode
    );

ANSC_STATUS
AnscStreeCleanUpNode
    (
        PSTREE_LINK_ENTRY           pStreeNode,
        BOOL                        bTableToo,
        BOOL                        bValueToo
    );

ULONG
AnscStreeGetSonNodeCount
    (
        PSTREE_LINK_ENTRY           pStreeNode
    );

ANSC_HANDLE
AnscStreeGenSearchString
    (
        PSTREE_LINK_ENTRY           pStreeNode
    );

ANSC_STATUS
AnscStreeCreateNode
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_NODE_PATH            pNodePath
    );

ANSC_STATUS
AnscStreeRemoveNode
    (
        PSTREE_LINK_ENTRY           pStreeNode
    );

ANSC_STATUS
AnscStreeRemovePath
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_STATUS
AnscStreeAddNode
    (
        PSTREE_LINK_ENTRY           pMomNode,
        PSTREE_LINK_ENTRY           pSonNode
    );

ANSC_STATUS
AnscStreeDelNode
    (
        PSTREE_LINK_ENTRY           pMomNode,
        PSTREE_LINK_ENTRY           pSonNode
    );

ANSC_HANDLE
AnscStreePingUserContext
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreePingNode
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreeQueryUserContext
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreeQueryNode
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreeMatchUserContext
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreeMatchNode
    (
        PSTREE_ROOT_NODE            pRootNode,
        PSTREE_SEARCH_STRING        pSearchString
    );

ANSC_HANDLE
AnscStreeGetNode
    (
        PSTREE_LINK_ENTRY           pStreeNode,
        PSTREE_TLV_TUPLE            pTlvTuple
    );

ANSC_STATUS
AnscStreeEnumerate
    (
        PSTREE_LINK_ENTRY           pStreeNode,
        ANSC_HANDLE                 hEnumContext
    );

ANSC_HANDLE
AnscStreeGetBMNode
    (
        PSTREE_LINK_ENTRY           pStreeNode,
        PSTREE_TLV_TUPLE            pTlvTuple,
        ULONG                       matchMode
    );

ANSC_HANDLE
AnscStreeGetBTLeaf
    (
        PSTREE_LINK_ENTRY           pStreeNode
    );


/***********************************************************
             FUNCTIONS IMPLEMENTED IN ANSC_SWAP.C
***********************************************************/

void
AnscSwapMemory
    (
        char*                       memory,
        ULONG                       ulSize
    );

ULONG
AnscSwapUlong
    (
        ULONG                       ulValue
    );

USHORT
AnscSwapUshort
    (
        USHORT                      usValue
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_CHECKSUM.C
***********************************************************/

USHORT
AnscReplaceChecksum
    (
        PVOID                       oldData,
        ULONG                       oldLength,
        PVOID                       newData,
        ULONG                       newLength,
        USHORT                      oldChecksum
    );

USHORT
AnscCalculateChecksum
    (
        PVOID                       data,
        ULONG                       length
    );

USHORT
AnscCalculateTransportChecksum
    (
        PVOID                       pseudoHeader,
        ULONG                       headerLength,
        PVOID                       data,
        ULONG                       length
    );

USHORT
AnscGetPackingNumber
    (
        PVOID                       data,
        ULONG                       length
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_STRING.C
***********************************************************/

BOOL
AnscEqualString1
    (
        char*                       pString1,
        char*                       pString2,
        BOOL                        bCaseSensitive
    );

BOOL
AnscEqualString2
    (
        char*                       pString1,
        char*                       pString2,
        ULONG                       length,
        BOOL                        bCaseSensitive
    );

void
AnscFromHexToString
    (
        PUCHAR                      hex,
        char*                       string,
        ULONG                       ulSize,
        BOOL                        bLowerCase
    );

ULONG
AnscFromStringToHex
    (
        char*                       string,
        PUCHAR                      hex
    );

void
AnscStringFromAToU
    (
        char*                       string,
        ULONG                       ulSize
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

ULONG
AnscNumberOfCharsInString
    (
        char*                       string,
        char                        charToFind
    );

ULONG
AnscNumberOfTokens
    (
        char*                       string,
        char*                       separator
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

char*
AnscMoveToPreviousToken
    (
        char*                       string,
        char*                       separator,
        PULONG                      pulTokenLength
    );

ANSC_STATUS
AnscExtractToken
    (
        char*                       string,
        char*                       separator,
        ULONG                       index,
        char*                       output
    );

ANSC_STATUS
AnscExtractToken2
    (
        char*                       string,
        char*                       alphabet,
        ULONG                       index,
        char*                       output
    );

void
AnscConsumeToken
    (
        char*                       string,
        char*                       separator
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

#ifdef _ANSC_WINDOWSNT

char*
AnscCloneString2
    (
        char*                       src_string,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

#define AnscCloneString(s)          AnscCloneString2(s, __FILE__, __LINE__)

#else

char*
AnscCloneString
    (
        char*                       src_string
    );

#endif


ANSC_HANDLE
AnscCreateScanner
    (
        char**                      string_array,
        ULONG                       string_count
    );

ANSC_STATUS
AnscRemoveScanner
    (
        ANSC_HANDLE                 hStringScanner
    );

char*
AnscScanString
    (
        ANSC_HANDLE                 hStringScanner,
        char*                       tbs_string
    );

BOOL
AnscIsValidIpString
    (
        char*                       ip_addr_string
    );

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
            FUNCTIONS IMPLEMENTED IN ANSC_TOKEN.C
***********************************************************/

ANSC_HANDLE
AnscTcAllocate
    (
        char*                       string,
        char*                       separator
    );

ANSC_HANDLE
AnscTcAllocate2
    (
        char*                       string,
        char*                       separator,
        char*                       alphabet
    );

ANSC_HANDLE
AnscTcAllocate3
    (
        char*                       string,
        char*                       separator
    );

void
AnscTcFree
    (
        ANSC_HANDLE                 hTokenChain
    );

ULONG
AnscTcGetLength
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       separator
    );

BOOL
AnscTcEqualString
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator,
        BOOL                        bCaseSensitive,
        BOOL                        bStrict
    );

ANSC_HANDLE
AnscTcPopToken
    (
        ANSC_HANDLE                 hTokenChain
    );

ANSC_STATUS
AnscGetTokenChain
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator
    );

ANSC_STATUS
AnscSetTokenChain
    (
        ANSC_HANDLE                 hTokenChain,
        char*                       string,
        char*                       separator
    );

void
AnscDelTokenChain
    (
        ANSC_HANDLE                 hTokenChain
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_MATH.C
***********************************************************/

ULONG
AnscGetMinMultiplier
    (
        ULONG                       ulValue1,
        ULONG                       ulValue2
    );

ULONG
AnscGetIp4MaskBitsFromAddr
    (
        PUCHAR                      mask_addr
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
AnscCalendarToSecondFromEpoch
    (
        ANSC_HANDLE                 hCalendar
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

ANSC_STATUS
AnscSecondToLocalCalendar
    (
        ULONG                       ulSeconds,
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscGetWholeTickInMicroSeconds
    (
        PANSC_UINT64                pWholeTicks
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_TIMER.C
***********************************************************/

void
AnscSchedulerInitialize
    (
        void
    );

ANSC_STATUS
AnscRegisterTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
    );

ANSC_STATUS
AnscCancelTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
    );

ANSC_STATUS
AnscScheduleTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
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
AncResizeMemory2
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
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
AncResizeMemory3
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
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
AncResizeMemory4
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       LineNumber
    );

void*
AnscAllocateMemory5
    (
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    );

void*
AnscReAllocateMemory5
    (
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
        PVOID                       pMemoryBlock
    );

void*
AncResizeMemory5
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       LineNumber
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

void*
AncResizeMemory6
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    );

void
AnscTraceMemoryUsage
    (
        void
    );

ULONG
AnscTraceMemoryUsageCurr
    (
        void
    );

ULONG
AnscTraceMemoryUsagePeak
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


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_PROTECTION.C
***********************************************************/

BOOL
AnscInitializeSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    );

void
AnscFreeSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    );

int
AnscWaitSimEvent
    (
        ANSC_HANDLE*                phSimEvent,
        ULONG                       milli_seconds
    );

void
AnscSetSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    );

void
AnscResetSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    );

void
AnscPulseSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_PACKET.C
***********************************************************/

/*
ANSC_HANDLE
AnscAcquirePdo
    (
        void
    );

ANSC_STATUS
AnscReleasePdo
    (
        ANSC_HANDLE                 hPdo
    );

ANSC_STATUS
AnscFreePdo2
    (
        ANSC_HANDLE                 hPdo
    );


ANSC_HANDLE
AnscAllocatePdo2
    (
        void
    );
*/

ANSC_HANDLE
AnscAllocatePdo
    (
        void
    );

#ifndef _ANSC_TRACE_PACKET_

ANSC_STATUS
AnscFreePdo
    (
        ANSC_HANDLE                 hPdo
    );

void
AnscPdoClean
    (
        ANSC_HANDLE                 hPdo
    );

void
AnscPdoCleanBdoQueue
    (
        ANSC_HANDLE                 hPdo
    );

#else

ANSC_STATUS
AnscFreePdoTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    );

void
AnscPdoCleanTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    );

void
AnscPdoCleanBdoQueueTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscFreePdo(hPdo)   \
            AnscFreePdoTrace(hPdo, __FILE__, __LINE__)

#define AnscPdoClean(hPdo)  \
            AnscPdoCleanTrace(hPdo, __FILE__, __LINE__)

#define AnscPdoCleanBdoQueue(hPdo)  \
            AnscPdoCleanBdoQueueTrace(hPdo, __FILE__, __LINE__)

#endif /* _ANSC_TRACE_PACKET_ */

ANSC_HANDLE
AnscCopyPdo
    (
        ANSC_HANDLE                 hPdo
    );

ANSC_HANDLE
AnscTransferPdo
    (
        ANSC_HANDLE                 hPdo
    );

ANSC_HANDLE
AnscClonePdo
    (
        ANSC_HANDLE                 hPdo
    );

ULONG
AnscPdoGetPacketSize
    (
        ANSC_HANDLE                 hPdo
    );

ANSC_HANDLE
AnscPdoGetBdoByOffset
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    );

PVOID
AnscPdoGetBufferByOffset
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    );

ANSC_STATUS
AnscPdoCopyFrom
    (
        ANSC_HANDLE                 hPdo,
        PVOID                       pDstBuffer,
        PULONG                      pulCopySize,
        ULONG                       ulOffset
    );

ULONG
AnscPdoGetPackingNumber
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    );

USHORT
AnscPdoCalChecksum
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscPdoConcatenate
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2
    );

#ifndef _ANSC_TRACE_PACKET_

ANSC_STATUS
AnscPdoShrinkLeft
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscPdoShrinkRight
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize
    );

/*ANSC_STATUS
AnscPdoShrinkMiddle
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    );
*/

ANSC_STATUS
AnscPdoBreakAt
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    );

ANSC_STATUS
AnscPdoBreakAt2
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    );

ANSC_STATUS
AnscPdoSplitAt
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2,
        ULONG                       ulOffset
    );

#else

ANSC_STATUS
AnscPdoShrinkLeftTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscPdoShrinkRightTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscPdoBreakAtTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscPdoBreakAt2Trace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscPdoSplitAtTrace
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscPdoShrinkLeft(hPdo, ulSize) \
            AnscPdoShrinkLeftTrace(hPdo, ulSize, __FILE__, __LINE__)

#define AnscPdoShrinkRight(hPdo, ulSize) \
            AnscPdoShrinkRightTrace(hPdo, ulSize, __FILE__, __LINE__)

#define AnscPdoBreakAt(hPdo, ulOffset)  \
            AnscPdoBreakAtTrace(hPdo, ulOffset, __FILE__, __LINE__)

#define AnscPdoBreakAt2(hPdo, ulOffset)  \
            AnscPdoBreakAt2Trace(hPdo, ulOffset, __FILE__, __LINE__)

#define AnscPdoSplitAt(hPdo1, hPdo2, ulOffset)  \
            AnscPdoSplitAtTrace(hPdo1, hPdo2, ulOffset, __FILE__, __LINE__)

#endif /* _ANSC_TRACE_PACKET_ */

/*
ANSC_HANDLE
AnscAcquireBdo
    (
        void
    );
*/

ANSC_STATUS
AnscReleaseBdo
    (
        ANSC_HANDLE                 hBdo
    );

#ifndef _ANSC_TRACE_PACKET_

ANSC_HANDLE
AnscAllocateBdo
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    );

ANSC_HANDLE
AnscAllocateBdoWithMmap
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    );

ANSC_STATUS
AnscFreeBdo
    (
        ANSC_HANDLE                 hBdo
    );

ANSC_HANDLE
AnscCopyBdo
    (
        ANSC_HANDLE                 hBdo
    );


ANSC_STATUS
AnscBdoBreakAt
    (
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        PANSC_HANDLE                phHeadBdo,
        PANSC_HANDLE                phTailBdo
    );


#else

ANSC_HANDLE
AnscAllocateBdoTrace
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscFreeBdoTrace
    (
        ANSC_HANDLE                 hBdo,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_HANDLE
AnscCopyBdoTrace
    (
        ANSC_HANDLE                 hBdo,
        const char *                pFileName,
        int                         iLineNo
    );

ANSC_STATUS
AnscBdoBreakAtTrace
    (
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        PANSC_HANDLE                phHeadBdo,
        PANSC_HANDLE                phTailBdo,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscAllocateBdo(ulBuf, ulBlkOffset, ulBlkSize)  \
            AnscAllocateBdoTrace(ulBuf, ulBlkOffset, ulBlkSize, __FILE__, __LINE__)

#define AnscFreeBdo(hBdo)   \
            AnscFreeBdoTrace(hBdo, __FILE__, __LINE__)

#define AnscCopyBdo(hBdo)   \
            AnscCopyBdoTrace(hBdo, __FILE__, __LINE__)

#define AnscBdoBreakAt(hBdo, ulOffset, phHeadBdo, phTailBdo) \
            AnscBdoBreakAtTrace(hBdo, ulOffset, phHeadBdo, phTailBdo, __FILE__, __LINE__)

void
AnscBdoTracePrint
    (
        int                         type
    );

#endif /* _ANSC_TRACE_PACKET_ */

/*
ANSC_HANDLE
AnscAllocateBdo2
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    );

ANSC_STATUS
AnscFreeBdo2
    (
        ANSC_HANDLE                 hBdo
    );
*/


ANSC_STATUS
AnscFreeSonBdo
    (
        ANSC_HANDLE                 hParentBdo,
        ANSC_HANDLE                 hSonBdo
    );

ANSC_STATUS
AnscBdoClean
    (
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
AnscBdoCopyFrom
    (
        ANSC_HANDLE                 hBdo,
        PVOID                       pDstBuffer,
        PULONG                      pulCopySize,
        ULONG                       ulOffset
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_SOCKET_LIB.C
***********************************************************/

void
AnscSocketLibInitialize
    (
        ANSC_HANDLE                 hBssBsdIf
    );

void
AnscSocketLibUnload
    (
        void
    );

ANSC_SOCKET
AnscSocketLibAccept
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           addr,
        int*                        addrlen
    );

int
AnscSocketLibBind
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int                         namelen
    );

int
AnscSocketLibCloseSocket
    (
        ANSC_SOCKET                 socket
    );

int
AnscSocketLibConnect
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int                         namelen
    );

ansc_hostent*
AnscSocketLibGetHostByAddr
    (
        char*                       addr,
        int                         len,
        int                         type
    );

ansc_hostent*
AnscSocketLibGetHostByName
    (
        char*                       name
    );

int
AnscSocketLibGetHostName
    (
        char*                       name,
        int                         namelen
    );

int
AnscSocketLibGetPeerName
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int*                        namelen
    );

int
AnscSocketLibGetSockName
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int*                        namelen
    );

int
AnscSocketLibGetSockOpt
    (
        ANSC_SOCKET                 socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int*                        optlen
    );

int
AnscSocketLibIoctlSocket
    (
        ANSC_SOCKET                 socket,
        long                        cmd,
        unsigned long*              argp
    );

ULONG
AnscSocketLibInetAddr
    (
        char*                       cp
    );

char*
AnscSocketLibInetNtoa
    (
        ansc_in_addr                in
    );

int
AnscSocketLibListen
    (
        ANSC_SOCKET                 socket,
        int                         backlog
    );

int
AnscSocketLibRecv
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags
    );

int
AnscSocketLibRecvFrom
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags,
        ansc_socket_addr*           from,
        int*                        fromlen
    );

int
AnscSocketLibSelect
    (
        int                         nfds,
        ansc_fd_set*                readfds,
        ansc_fd_set*                writefds,
        ansc_fd_set*                exceptfds,
        ansc_timeval*               timeout
    );

int
AnscSocketLibSend
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags
    );

int
AnscSocketLibSendTo
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags,
        ansc_socket_addr*           to,
        int                         tolen
    );

int
AnscSocketLibSetSockOpt
    (
        ANSC_SOCKET                 socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int                         optlen
    );

int
AnscSocketLibShutdown
    (
        ANSC_SOCKET                 socket,
        int                         how
    );

ANSC_SOCKET
AnscSocketLibSocket
    (
        int                         af,
        int                         type,
        int                         protocol
    );

int
AnscSocketLibGetLastError
    (
        void
    );

int
AnscSocketLibFdsIsSet
    (
        ANSC_SOCKET                 socket,
        ansc_fd_set*                fds
    );

void
AnscSocketTlsInitialize
    (
        ANSC_HANDLE                 hTlsScsIf
    );

void
AnscSocketTlsUnload
    (
        void
    );

ANSC_HANDLE
AnscSocketTlsGetScsIf
    (
        void
    );


/***********************************************************
             FUNCTIONS IMPLEMENTED IN ANSC_ASN1.C
***********************************************************/

ANSC_HANDLE
AnscAsn1AllocOctetString
    (
        PUCHAR                      pSrcString,
        ULONG                       ulStrLen
    );

ANSC_HANDLE
AnscAsn1CloneOctetString
    (
        ANSC_HANDLE                 hSrcAsn1OctetString
    );

int
AnscAsn1CompOctetStrings
    (
        ANSC_HANDLE                 hAsn1OctetString1,
        ANSC_HANDLE                 hAsn1OctetString2
    );

ANSC_HANDLE
AnscAsn1AllocOid
    (
        PULONG                      pSrcSequence,
        ULONG                       ulSeqLen
    );

ANSC_HANDLE
AnscAsn1ConvOid
    (
        char*                       pOidString
    );

ANSC_HANDLE
AnscAsn1CloneOid
    (
        ANSC_HANDLE                 hSrcAsn1Oid
    );

BOOL
AnscAsn1ConcOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    );

int
AnscAsn1CompOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    );

ANSC_HANDLE
AnscAsn1CombOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    );

int
AnscAsn1FindOidInArray
    (
        ANSC_ASN1_OID*              asn1_oid,
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    );

BOOL
AnscAsn1MarkOidInArray
    (
        ANSC_ASN1_OID*              asn1_oid,
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size,
        PULONG                      oid_index
    );

ANSC_STATUS
AnscAsn1SortOidArray
    (
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    );

void
AnscAsn1FreeOidArray
    (
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    );

ANSC_HANDLE
AnscAsn1AllocInt
    (
        PUCHAR                      pSrcString,
        ULONG                       ulStrLen,
        BOOL                        bNegative
    );

ANSC_HANDLE
AnscAsn1CloneInt
    (
        ANSC_HANDLE                 hSrcAsn1Int
    );

int
AnscAsn1CompInts
    (
        ANSC_HANDLE                 hAsn1Int1,
        ANSC_HANDLE                 hAsn1Int2
    );


/***********************************************************
              FUNCTIONS IMPLEMENTED IN ANSC_URI.C
***********************************************************/

ANSC_HANDLE
AnscCloneUriHost
    (
        ANSC_HANDLE                 hSrcUriHost
    );

BOOL
AnscEqualUriHost
    (
        ANSC_HANDLE                 hUriHost1,
        ANSC_HANDLE                 hUriHost2
    );

ANSC_HANDLE
AnscCloneUriHostPort
    (
        ANSC_HANDLE                 hSrcUriHostPort
    );

BOOL
AnscEqualUriHostPort
    (
        ANSC_HANDLE                 hUriHostPort1,
        ANSC_HANDLE                 hUriHostPort2
    );

ANSC_HANDLE
AnscCloneUriServer
    (
        ANSC_HANDLE                 hSrcUriServer
    );

BOOL
AnscEqualUriServer
    (
        ANSC_HANDLE                 hUriServer1,
        ANSC_HANDLE                 hUriServer2
    );

ANSC_HANDLE
AnscCloneUriAbsolute
    (
        ANSC_HANDLE                 hSrcUriAbsolute
    );

BOOL
AnscEqualUriAbsolute
    (
        ANSC_HANDLE                 hUriAbsolute1,
        ANSC_HANDLE                 hUriAbsolute2
    );

ANSC_HANDLE
AnscCloneUriRelative
    (
        ANSC_HANDLE                 hSrcUriRelative
    );

BOOL
AnscEqualUriRelative
    (
        ANSC_HANDLE                 hUriRelative1,
        ANSC_HANDLE                 hUriRelative2
    );

ANSC_HANDLE
AnscCloneGenericUri
    (
        ANSC_HANDLE                 hSrcGenericUri
    );

BOOL
AnscEqualGenericUri
    (
        ANSC_HANDLE                 hGenericUri1,
        ANSC_HANDLE                 hGenericUri2
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN ANSC_BASE64.C
***********************************************************/

PUCHAR
AnscBase64DecodeLine
    (
        const PUCHAR                pString,
        PUCHAR                      pData,
        PULONG                      pulSize
    );

PUCHAR
AnscBase64Decode
    (
        PUCHAR                      pEncode,
        PULONG                      pulSize
    );

PUCHAR
AnscBase64Encode
    (
        const PUCHAR                pBuf,
        ULONG                       ulSize
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_X509_CERT.C
***********************************************************/

ANSC_X509_CERTIFICATE*
AnscX509CertGetCertWrap
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_X501_NAME*
AnscX509CertGetIssuer
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_X501_NAME*
AnscX509CertGetSubject
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_ASN1_OCTET_STRING*
AnscX509CertGetIssuerKeyID
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_ASN1_OCTET_STRING*
AnscX509CertGetSubjectKeyID
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_ASN1_INT*
AnscX509CertGetSerialNumber
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ANSC_X509_KEY*
AnscX509CertGetPublicKey
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

BOOL
AnscX509CertIsSelfSigned
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    );

ULONG
AnscX509CertGetKeyType
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size,
        PULONG                      cert_signing,
        PULONG                      cert_key_usage,
        PULONG                      cert_key_bits
    );


#endif
