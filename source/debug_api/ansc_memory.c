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

    module: ansc_memory.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements all the CCSP memory related
        functions.

        *   AnscAllocateMemoryOrig
        *   AnscFreeMemoryOrig
        *   AnscAllocateMemoryCountSize
        *   AnscReAllocateMemoryCountSize
        *   AnscFreeMemoryCountSize
        *   AnscGetMemorySizeCountSize
        *   AnscAllocateMemoryRecordDetail
        *   AnscReAllocateMemoryRecordDetail
        *   AnscFreeMemoryRecordDetail
        *   AnscGetMemorySizeRecordDetail

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Hui Ma

    ---------------------------------------------------------------

    revision:

        04/25/01    initial revision.

**********************************************************************/


#include "ansc_global.h"
#include "ccsp_memory.h"

/**********************************************************************
                    CONSTANT DEFINITIONS
**********************************************************************/

#define  ANSC_MEMORY_ALLOC_HEAD_SIZE                8
#define  ANSC_MEMORY_ALLOC_TAIL_SIZE                32

/**********************************************************************
                PROTOTYPES OF INTERNAL FUNCTIONS
**********************************************************************/

ANSC_HANDLE
AnscGetPrevMemoryAllocItem
    (
        void*                       pointer
    );

/**********************************************************************
                    GLOBAL VARIABLES FOR PROCESS
**********************************************************************/
/*static  ANSC_SPINLOCK   g_tMumSpinLock;
static  BOOL            g_bMumInitialized     = FALSE;
*/
ULONG           g_ulAllocatedSizeCurr = 0;
ULONG           g_ulAllocatedSizePeak = 0;
ULONG           g_ulAllocatedItemCurr = 0;
ULONG           g_ulAllocatedItemPeak = 0;
static  ULONG           g_ulAllocIdentifier   = 0;
static  ULONG           g_ulTimeMarker        = 0;

/**********************************************************************
                    VARIABLES FOR COUNTING SIZE
**********************************************************************/
static  BOOL            g_bCountSizeInitialized     = FALSE;
static  SLIST_HEADER    g_sAllocSizeList;
static  ANSC_SPINLOCK   g_tCountSizeSpinLock;

/**********************************************************************
                    VARIABLES FOR RECORDING DETAIL
**********************************************************************/
static  BOOL            g_bRecordDetailInitialized  = FALSE;
static  SLIST_HEADER    g_tAllocTable[ANSC_MEMORY_ALLOC_TABLE_SIZE];
static  ANSC_SPINLOCK   g_tRecordDetailSpinLock;

char *                  pComponentName = "ComponentName"; 

/*static  ANSC_SPINLOCK   g_tMpmSpinLock;
static  BOOL            g_bMpmInitialized     = FALSE;
static  QUEUE_HEADER    g_tMemPoolTable[ANSC_MEMORY_POOL_TABLE_SIZE];
static  ANSC_SPINLOCK   g_tMemPoolTableSpinLock;
*/


/**********************************************************************
    Functions with no size counting and detail recording.
**********************************************************************/
PVOID
AnscAllocateMemoryOrig(ULONG  ulMemorySize)
{
    PVOID p =  malloc(ulMemorySize);
    if ( !p )
    {
        return  p;
    }
    memset(p, 0, ulMemorySize);
    return p;
}

VOID
AnscFreeMemoryOrig(PVOID  pMemoryBlock)
{
    if (pMemoryBlock)
    {
        free(pMemoryBlock);
        pMemoryBlock = NULL;
    }
    return;
}

PVOID
AnscReallocMemory
    (
        PVOID pMemory,
        ULONG ulOldMemorySize,
        ULONG ulNewMemorySize
    )
{
    PVOID p = AnscReAllocateMemory(pMemory, ulNewMemorySize);

    if (p != NULL)
    {
        /*clear the new allocated mem*/
        ULONG minLen = AnscGetMin2(ulOldMemorySize, ulNewMemorySize);
        AnscZeroMemory(p + minLen, ulNewMemorySize - minLen);
    }

    return p;
}

ULONG 
AnscGetProcessMaxMemorySize                                                                 
    (               
    )
{
    return g_ulAllocatedSizePeak;
}
/**********************************************************************
    Functions with size counting but no detail recording.
    Count allocated size for each component.
    Do not introduce any extra burden.
**********************************************************************/
char
AnscMemoryGetComponentID
    (
        char*                       pComponentName
    )
{
    PANSC_MEMORY_COMPONENT_ITEM     pMemoryComponentItem  = NULL; 
    PSINGLE_LINK_ENTRY              pSLinkEntry           = NULL;
    BOOLEAN                         bFoundEntry           = FALSE;
    char                            cCandidateComponentID = 0;
    char                            cComponentID          = 0;

    AnscAcquireSpinLock(&g_tCountSizeSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_sAllocSizeList);

    while ( pSLinkEntry )
    {
        pMemoryComponentItem = (ANSC_MEMORY_COMPONENT_ITEM*)ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry          = AnscSListGetNextEntry(pSLinkEntry);
        if ( AnscEqualString1(pMemoryComponentItem->ComponentName, pComponentName, TRUE) )
        {
            cComponentID = pMemoryComponentItem->ComponentID;
            bFoundEntry = TRUE;
            break;
        }
        else 
        {
            if(pMemoryComponentItem->ComponentID >= cCandidateComponentID) 
                cCandidateComponentID = pMemoryComponentItem->ComponentID + 1;
        }
    }
    if(!bFoundEntry)
    {
        pMemoryComponentItem = (PANSC_MEMORY_COMPONENT_ITEM)AnscAllocateMemoryOrig(sizeof(ANSC_MEMORY_COMPONENT_ITEM));
        pMemoryComponentItem->MemorySize = 0;
        pMemoryComponentItem->MaxMemorySize = pMemoryComponentItem->MemorySize;
        pMemoryComponentItem->ComponentID = cCandidateComponentID;
        cComponentID = pMemoryComponentItem->ComponentID;
        AnscZeroMemory(pMemoryComponentItem->ComponentName, ANSC_COMPONENT_NAME_LENGTH);
        AnscCopyMemory(pMemoryComponentItem->ComponentName, pComponentName, ANSC_COMPONENT_NAME_LENGTH);
        AnscSListPushEntry (&g_sAllocSizeList, &pMemoryComponentItem->Linkage);
    }
    AnscReleaseSpinLock(&g_tCountSizeSpinLock);
    return cComponentID;
}

void
AnscMemoryCountSize
    (
        char*                       pComponentName,
        ULONG                       ulMemorySize,
        BOOL                        isAllocation
    )
{
    PANSC_MEMORY_COMPONENT_ITEM     pMemoryComponentItem  = NULL; 
    PSINGLE_LINK_ENTRY              pSLinkEntry           = NULL;
    BOOLEAN                         bFoundEntry           = FALSE;
    char                            cCandidateComponentID = 0;

    AnscAcquireSpinLock(&g_tCountSizeSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_sAllocSizeList);

    while ( pSLinkEntry )
    {
        pMemoryComponentItem = (ANSC_MEMORY_COMPONENT_ITEM*)ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry          = AnscSListGetNextEntry(pSLinkEntry);
        if ( AnscEqualString1(pMemoryComponentItem->ComponentName, pComponentName, TRUE) )
        {
            if(isAllocation) 
            {
                pMemoryComponentItem->MemorySize += ulMemorySize;
                if(pMemoryComponentItem->MemorySize > pMemoryComponentItem->MaxMemorySize)
                    pMemoryComponentItem->MaxMemorySize = pMemoryComponentItem->MemorySize;
            }
            else pMemoryComponentItem->MemorySize -= ulMemorySize;
            bFoundEntry = TRUE;
            break;
        }
        else 
        {
            if(pMemoryComponentItem->ComponentID >= cCandidateComponentID) 
                cCandidateComponentID = pMemoryComponentItem->ComponentID + 1;
        }
    }
    if(!bFoundEntry)
    {
        pMemoryComponentItem = (PANSC_MEMORY_COMPONENT_ITEM)AnscAllocateMemoryOrig(sizeof(ANSC_MEMORY_COMPONENT_ITEM));
        if(isAllocation) pMemoryComponentItem->MemorySize = ulMemorySize;
        else pMemoryComponentItem->MemorySize = 0;
        pMemoryComponentItem->MaxMemorySize = pMemoryComponentItem->MemorySize;
        pMemoryComponentItem->ComponentID = cCandidateComponentID;
        AnscZeroMemory(pMemoryComponentItem->ComponentName, ANSC_COMPONENT_NAME_LENGTH);
        AnscCopyMemory(pMemoryComponentItem->ComponentName, pComponentName, ANSC_COMPONENT_NAME_LENGTH);
        AnscSListPushEntry (&g_sAllocSizeList, &pMemoryComponentItem->Linkage);
    }

    if(isAllocation)
    {
        g_ulAllocatedSizeCurr += ulMemorySize;
        if(g_ulAllocatedSizePeak < g_ulAllocatedSizeCurr) g_ulAllocatedSizePeak = g_ulAllocatedSizeCurr;
    }
    else
    {
        g_ulAllocatedSizeCurr -= ulMemorySize;
    }

    AnscReleaseSpinLock(&g_tCountSizeSpinLock);
}

static ULONG AnscGetMemorySizeCountSize
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock
    )
{
    UNREFERENCED_PARAMETER(pComponentName);
    if ( !pMemoryBlock )
    {
        return 0;
    }
    else
    {
        pMemoryBlock = (PVOID)((ULONG)pMemoryBlock - sizeof(ULONG));
        return *(PULONG)pMemoryBlock;
    }
}

void*
AnscAllocateMemoryCountSize
    (
        char*                       pComponentName,
        ULONG                       ulMemorySize
    )
{
    void*                           pMemoryPointer = NULL;

    if ( !g_bCountSizeInitialized )
    {
        AnscInitializeSpinLock(&g_tCountSizeSpinLock);

        AnscAcquireSpinLock(&g_tCountSizeSpinLock);
        g_bCountSizeInitialized     = TRUE;
        AnscReleaseSpinLock(&g_tCountSizeSpinLock);
    }

    pMemoryPointer = AnscAllocateMemoryOrig(ulMemorySize + sizeof(ULONG));

    if ( !pMemoryPointer )
    {
        CcspTraceWarning2(pComponentName, ("AnscAllocateMemory5 -- failed to allocate %lu bytes memory for %s!!!", ulMemorySize, pComponentName));

        return  NULL;
    }
    else
    {
        *(PULONG)pMemoryPointer = ulMemorySize;
        pMemoryPointer = (PVOID )((ULONG)pMemoryPointer + sizeof(ULONG));

        AnscMemoryCountSize(pComponentName, ulMemorySize, TRUE);
    }

    return  pMemoryPointer;
}


void*
AnscReAllocateMemoryCountSize
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    )
{
    void*                           pOldMemory = (void*)pMemoryBlock;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = AnscGetMemorySizeCountSize(pComponentName, pOldMemory);
    ULONG                           ulNewSize  = ulMemorySize;
    ULONG                           ulCpySize  = AnscGetMin2(ulOldSize, ulNewSize);

    pNewMemory = AnscAllocateMemoryCountSize(pComponentName, ulNewSize);

    if ( !pNewMemory )
    {
        CcspTraceWarning2(pComponentName, ("AnscReAllocateMemory5 -- failed to allocate %lu bytes memory for %s!!!", ulMemorySize, pComponentName));

        return  NULL;
    }
    else
    {
        AnscCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    AnscFreeMemoryCountSize(pComponentName, pOldMemory);

    return  pNewMemory;
}


void
AnscFreeMemoryCountSize
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock
    )
{
    ULONG ulMemorySize   = 0;
    PVOID pMemoryPointer = NULL;
    if ( !pMemoryBlock )
    {
        return;
    }
    else
    {
        pMemoryPointer = (PVOID)((ULONG)pMemoryBlock - sizeof(ULONG));
        ulMemorySize   = *(PULONG)pMemoryPointer;
        *(PULONG)pMemoryPointer = 0;
        //memset(pMemoryPointer, 0, sizeof(ULONG) + ulMemorySize);
        AnscMemoryCountSize(pComponentName, ulMemorySize, FALSE);
    }

    //AnscFlushMemory(pMemoryBlock);
    AnscFreeMemoryOrig(pMemoryPointer);
}

// Return -1 if no size counting.
LONG AnscGetComponentMemorySize                                                                 
    (               
        char*  pComponentName
    )
{
    PANSC_MEMORY_COMPONENT_ITEM     pMemoryComponentItem = NULL; 
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    BOOLEAN                         bFoundEntry      = FALSE;
    LONG                            lMemorySize      = 0;

    AnscAcquireSpinLock(&g_tCountSizeSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_sAllocSizeList);

    while ( pSLinkEntry )
    {
        pMemoryComponentItem = (ANSC_MEMORY_COMPONENT_ITEM*)ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry          = AnscSListGetNextEntry(pSLinkEntry);
        if ( AnscEqualString1(pMemoryComponentItem->ComponentName, pComponentName, TRUE) )
        {
            lMemorySize = pMemoryComponentItem->MemorySize;
            bFoundEntry = TRUE;
            break;
        }
    }
    if(!bFoundEntry)
    {
        lMemorySize = -1;
    }
    AnscReleaseSpinLock(&g_tCountSizeSpinLock);

    return lMemorySize;
}

// Return -1 if no size counting.
LONG AnscGetComponentMaxMemorySize                                                                 
    (               
        char*  pComponentName
    )
{
    PANSC_MEMORY_COMPONENT_ITEM     pMemoryComponentItem = NULL; 
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    BOOLEAN                         bFoundEntry      = FALSE;
    LONG                            lMemorySize      = 0;

    AnscAcquireSpinLock(&g_tCountSizeSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_sAllocSizeList);

    while ( pSLinkEntry )
    {
        pMemoryComponentItem = (ANSC_MEMORY_COMPONENT_ITEM*)ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry          = AnscSListGetNextEntry(pSLinkEntry);
        if ( AnscEqualString1(pMemoryComponentItem->ComponentName, pComponentName, TRUE) )
        {
            lMemorySize = pMemoryComponentItem->MaxMemorySize;
            bFoundEntry = TRUE;
            break;
        }
    }
    if(!bFoundEntry)
    {
        lMemorySize = -1;
    }
    AnscReleaseSpinLock(&g_tCountSizeSpinLock);

    return lMemorySize;
}

/**********************************************************************
    Functions with size counting and detail recording.
**********************************************************************/
void
AnscInitializeMemoryAllocTable
    (
        void
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&g_tAllocTable[i]);
    }
    AnscInitializeSpinLock(&g_tRecordDetailSpinLock);

    return;
}


void
AnscAddMemoryAllocItem
    (
        char*                       pComponentName,
        void*                       pointer,
        ULONG                       size,
        ULONG                       alloc_id,
        char*                       owner_desp
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = (PANSC_MEMORY_ALLOC_ITEM)AnscAllocateMemoryOrig(sizeof(ANSC_MEMORY_ALLOC_ITEM));
    ULONG                           ulHashIndex      = AnscHashUlong((ULONG)pointer, ANSC_MEMORY_ALLOC_TABLE_SIZE);

    if ( g_ulAllocatedSizeCurr > g_ulAllocatedSizePeak )
    {
        g_ulAllocatedSizePeak = g_ulAllocatedSizeCurr;
        g_ulAllocatedItemPeak = g_ulAllocatedItemCurr;
    }

    if ( !pMemoryAllocItem )
    {
        return;
    }
    else
    {
        pMemoryAllocItem->HashIndex     = ulHashIndex;
        pMemoryAllocItem->AllocId       = alloc_id;
        pMemoryAllocItem->MemoryPointer = pointer;
        pMemoryAllocItem->MemorySize    = size;
        pMemoryAllocItem->Timestamp     = AnscGetTickInSeconds();
        pMemoryAllocItem->ComponentID   = AnscMemoryGetComponentID(pComponentName);

        if ( owner_desp )
        {
            AnscCopyString(pMemoryAllocItem->OwnerDesp, owner_desp);
        }
    }

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);
    AnscSListPushEntry (&g_tAllocTable[ulHashIndex], &pMemoryAllocItem->Linkage);
    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}


ANSC_HANDLE
AnscGetMemoryAllocItem
    (
        void*                       pointer
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           ulHashIndex      = AnscHashUlong((ULONG)pointer, ANSC_MEMORY_ALLOC_TABLE_SIZE);

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

        if ( (void*)pMemoryAllocItem->MemoryPointer == pointer )
        {
            AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

            return  (ANSC_HANDLE)pMemoryAllocItem;
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return  (ANSC_HANDLE)NULL;
}


ANSC_HANDLE
AnscGetPrevMemoryAllocItem
    (
        void*                       pointer
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           ulHashIndex      = 0;
    PANSC_MEMORY_ALLOC_ITEM         pPrevAllocItem   = NULL;

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( ulHashIndex = 0; ulHashIndex < ANSC_MEMORY_ALLOC_TABLE_SIZE; ulHashIndex++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[ulHashIndex]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

            if ( pMemoryAllocItem->MemoryPointer < pointer )
            {
                if ( pPrevAllocItem == NULL )
                {
                    pPrevAllocItem = pMemoryAllocItem;
                }
                else if ( pMemoryAllocItem->MemoryPointer >= pPrevAllocItem->MemoryPointer )
                {
                    pPrevAllocItem = pMemoryAllocItem;
                }
            }
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return  (ANSC_HANDLE)pPrevAllocItem;
}


void
AnscDelMemoryAllocItem
    (
        void*                       pointer
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           ulHashIndex      = AnscHashUlong((ULONG)pointer, ANSC_MEMORY_ALLOC_TABLE_SIZE);

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
        pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

        if ( (void*)pMemoryAllocItem->MemoryPointer == pointer )
        {
            AnscSListPopEntryByLink(&g_tAllocTable[ulHashIndex], &pMemoryAllocItem->Linkage);

            //AnscFlushMemory(pMemoryAllocItem);
            AnscFreeMemoryOrig(pMemoryAllocItem);

            break;
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}

void
AnscTraceMemoryTable
    (
        void
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           i                = 0;
    ULONG                           TotalBlocks      = 0;

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

            CcspTraceError
                ((
                    " *** mem_block = 0x%8p, size = %7lu bytes, id = %8lu, %s ***\n",
                    pMemoryAllocItem->MemoryPointer,
                    pMemoryAllocItem->MemorySize,
                    pMemoryAllocItem->AllocId,
                    pMemoryAllocItem->OwnerDesp
                ));

            TotalBlocks ++;
        }
    }

    CcspTraceError(("=== Total memory blocks not freed %lu.\n\n", TotalBlocks));

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}

ULONG
AnscGetNextMemoryBlockId
    (
    )
{
    return  g_ulAllocIdentifier;
}


void
AnscTraceMemoryTableFromBlockId
    (
        ULONG                       ulBlockIdFrom
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           i                = 0;
    ULONG                           TotalBlocks      = 0;

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

            if ( pMemoryAllocItem->AllocId <= ulBlockIdFrom )
            {
                continue;
            }

            CcspTraceError
                ((
                    " *** mem_block = 0x%8p, size = %7lu bytes, id = %8lu, %s ***\n",
                    pMemoryAllocItem->MemoryPointer,
                    pMemoryAllocItem->MemorySize,
                    pMemoryAllocItem->AllocId,
                    pMemoryAllocItem->OwnerDesp
                ));

            TotalBlocks ++;
        }
    }

    CcspTraceError(("=== Total memory blocks not freed %lu.\n\n", TotalBlocks));

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}

void
AnscPrintComponentMemoryTable
    (
        char* pComponentName
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           i                = 0;
    char                            cComponentID     = AnscMemoryGetComponentID(pComponentName);

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);
            if(pMemoryAllocItem->ComponentID != cComponentID) continue;

            CcspTraceError
                ((
                    " *** mem_block = 0x%8p, size = %7lu bytes, id = %8lu, %s ***\n",
                    pMemoryAllocItem->MemoryPointer,
                    pMemoryAllocItem->MemorySize,
                    pMemoryAllocItem->AllocId,
                    pMemoryAllocItem->OwnerDesp
                ));
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}

void AnscPrintFileLineMemoryTable
(
    char*  pFileName,
    ULONG  ulLineNumber
)
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           i                = 0;
    char                            owner_desp[128] = {0}; /*RDKB-6143, CID-24780 ; initialize before use*/
    char*                           pFileNameChar  = pFileName;

    if ( pFileName )
    {
        /*
         * The file name passed into this function is the full path, we need to extract the file
         * name and store it in the memory block.
         */
        pFileNameChar  = pFileName;
        pFileNameChar += AnscSizeOfString(pFileName);

        while ( (*pFileNameChar != '/'      ) &&
                (*pFileNameChar != '\\'     ) &&
                ( pFileNameChar != pFileName) )
        {
            pFileNameChar--;
        }

        if ( (*pFileNameChar == '/') || (*pFileNameChar == '\\') )
        {
            pFileNameChar++;
        }

 
        if ( TRUE )
        {
            if ( AnscSizeOfString(pFileNameChar) < 110 )
            {
                AnscCopyString( owner_desp,                               pFileNameChar);
                AnscCopyString(&owner_desp[AnscSizeOfString(owner_desp)], ": "         );
                //_ansc_sprintf (&owner_desp[AnscSizeOfString(owner_desp)], "%d", ulLineNumber);
                sprintf (&owner_desp[AnscSizeOfString(owner_desp)], "%lu", ulLineNumber);
            }
            else
            {
                AnscCopyMemory(owner_desp, pFileNameChar, 110);
            }
        }
    }

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);
            if(!AnscEqualString2(owner_desp, pMemoryAllocItem->OwnerDesp, 128, TRUE)) continue;

            CcspTraceError
                ((
                    " *** mem_block = 0x%8p, size = %7lu bytes, id = %8lu, %s ***\n",
                    pMemoryAllocItem->MemoryPointer,
                    pMemoryAllocItem->MemorySize,
                    pMemoryAllocItem->AllocId,
                    pMemoryAllocItem->OwnerDesp
                ));
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    return;
}

char**
AnscTraceMemoryTable2
    (
        ULONG                       ulTraceMask,
        char*                       pOwnerDesp,
        PULONG                      pulBlockCount
    )
{
    UNREFERENCED_PARAMETER(ulTraceMask);
    char**                          pTraceMsgArray   = NULL;
    char*                           pTraceMessage    = NULL;
    ULONG                           ulTotalBlocks    = 0;
    ULONG                           ulMaxBlockCount  = (*pulBlockCount > 0)? AnscGetMin2(*pulBlockCount, 1024) : 1024;
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           i                = 0;
    ULONG                           ulCurrentAllocId = g_ulAllocIdentifier;

    /*
     * We shall limit the number of trace messages to 1024.
     */
    pTraceMsgArray = (char**)AnscAllocateMemoryOrig(sizeof(char*) * ulMaxBlockCount);

    if ( !pTraceMsgArray )
    {
        *pulBlockCount = 0;

        return  pTraceMsgArray;
    }

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);

        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

            if ( pMemoryAllocItem->AllocId > ulCurrentAllocId )
            {
                continue;
            }
            else if ( /*(ulTraceMask & ANSC_MEMORY_TRACE_byMarker) &&*/ (pMemoryAllocItem->Timestamp <= g_ulTimeMarker) )
            {
                continue;
            }
            else if ( /*(ulTraceMask & ANSC_MEMORY_TRACE_byOwner) &&*/ pOwnerDesp &&
                      !AnscEqualString2
                        (
                            pOwnerDesp,
                            pMemoryAllocItem->OwnerDesp,
                            AnscGetMin2(AnscSizeOfString(pOwnerDesp), AnscSizeOfString(pMemoryAllocItem->OwnerDesp)),
                            FALSE
                        ) )
            {
                continue;
            }

            pTraceMessage = (char*)AnscAllocateMemoryOrig(128);

            if ( !pTraceMessage )
            {
                continue;
            }
            else
            {
                pTraceMsgArray[ulTotalBlocks++] = pTraceMessage;
            }

            //_ansc_sprintf
            sprintf
                (
                    pTraceMessage,
#ifdef _64BIT_ARCH_SUPPORT_
                    " *** mem_block = %p, size = %9d bytes, id = %6d, %s ***",
                    pMemoryAllocItem->MemoryPointer,
#else
                    " *** mem_block = 0x%8X, size = %9d bytes, id = %6d, %s ***",
                    (UINT)pMemoryAllocItem->MemoryPointer,
#endif
                    (int)pMemoryAllocItem->MemorySize,
                    (int)pMemoryAllocItem->AllocId,
                    pMemoryAllocItem->OwnerDesp
                );

            if ( ulTotalBlocks >= ulMaxBlockCount )
            {
                break;
            }
        }
    }

    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);

    *pulBlockCount = ulTotalBlocks;

    return  pTraceMsgArray;
}

static ULONG AnscGetMemorySizeRecordDetail
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock
    )
{
    UNREFERENCED_PARAMETER(pComponentName);
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = (PANSC_MEMORY_ALLOC_ITEM)AnscGetMemoryAllocItem(pMemoryBlock);

    if ( !pMemoryAllocItem )
    {
        return  0;
    }

    return  pMemoryAllocItem->MemorySize;
}

void*
AnscAllocateMemoryRecordDetail
    (
        char*                       pComponentName,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    )
{
    void*                           pMemoryPointer = NULL;
    PUCHAR                          pMemoryArray1  = NULL;
    PUCHAR                          pMemoryArray2  = NULL;
    ULONG                           ulCurAllocId   = g_ulAllocIdentifier ++;
    char*                           pFileNameChar  = pFileName;
    ULONG                           i              = 0;
    char                            owner_desp[128] = {'\0'}; /* CID 282068 fix */
                /*CcspTraceError
                ((
                    " AnscAllocateMemoryRecordDetail 1 \n"
                ));*/

    if ( !g_bCountSizeInitialized )
    {
        AnscInitializeSpinLock(&g_tCountSizeSpinLock);

        AnscAcquireSpinLock(&g_tCountSizeSpinLock);
        g_bCountSizeInitialized     = TRUE;
        AnscReleaseSpinLock(&g_tCountSizeSpinLock);
    }
    if ( !g_bRecordDetailInitialized )
    {
        AnscInitializeSpinLock(&g_tRecordDetailSpinLock);
        AnscInitializeMemoryAllocTable();

        AnscAcquireSpinLock(&g_tRecordDetailSpinLock);
        g_bRecordDetailInitialized     = TRUE;
        AnscReleaseSpinLock(&g_tRecordDetailSpinLock);
    }
                /*CcspTraceError
                ((
                    " AnscAllocateMemoryRecordDetail 2 \n"
                ));*/

    pMemoryPointer = AnscAllocateMemoryOrig(ulMemorySize + sizeof(ULONG) + ANSC_MEMORY_ALLOC_HEAD_SIZE + ANSC_MEMORY_ALLOC_TAIL_SIZE);

    if ( !pMemoryPointer )
    {
        CcspTraceWarning2(pComponentName, ("AnscAllocateMemory5 -- failed to allocate %lu bytes memory for %s:%lu!!!", ulMemorySize, pFileName, ulLineNumber));

        return  NULL;
    }
    else
    {
        *(PULONG)pMemoryPointer = ulMemorySize;

        pMemoryPointer = (PVOID )((ULONG)pMemoryPointer + sizeof(ULONG));
        pMemoryArray1  = (PUCHAR)pMemoryPointer;

        /*
        AnscWriteUlong(pMemoryArray1, 0x5C5C5C5C);
        pMemoryArray1 += 4;
        AnscWriteUlong(pMemoryArray1, 0x5C5C5C5C);
         */
        for ( i = 0; i < ANSC_MEMORY_ALLOC_HEAD_SIZE; i++ )
        {
            pMemoryArray1[i] = 0x5C;
        }

        pMemoryPointer = (PVOID )((ULONG)pMemoryPointer + ANSC_MEMORY_ALLOC_HEAD_SIZE);
        pMemoryArray2  = (PUCHAR)((ULONG)pMemoryPointer + ulMemorySize);

        /*
        AnscWriteUlong(pMemoryArray2, 0x7A7A7A7A);
        pMemoryArray2 += 4;
        AnscWriteUlong(pMemoryArray2, 0x7A7A7A7A);
         */
        for ( i = 0; i < ANSC_MEMORY_ALLOC_TAIL_SIZE; i++ )
        {
            pMemoryArray2[i] = 0x7A;
        }

        AnscMemoryCountSize(pComponentName, ulMemorySize, TRUE);
    }

    if ( pFileName )
    {
        /*
         * The file name passed into this function is the full path, we need to extract the file
         * name and store it in the memory block.
         */
        pFileNameChar  = pFileName;
        pFileNameChar += AnscSizeOfString(pFileName);

        while ( (*pFileNameChar != '/'      ) &&
                (*pFileNameChar != '\\'     ) &&
                ( pFileNameChar != pFileName) )
        {
            pFileNameChar--;
        }

        if ( (*pFileNameChar == '/') || (*pFileNameChar == '\\') )
        {
            pFileNameChar++;
        }

        AnscZeroMemory(owner_desp, 128);

        if ( TRUE )
        {
            if ( AnscSizeOfString(pFileNameChar) < 110 )
            {
                AnscCopyString( owner_desp,                               pFileNameChar);
                AnscCopyString(&owner_desp[AnscSizeOfString(owner_desp)], ": "         );
                //_ansc_sprintf (&owner_desp[AnscSizeOfString(owner_desp)], "%d", ulLineNumber);
                sprintf (&owner_desp[AnscSizeOfString(owner_desp)], "%lu", ulLineNumber);
            }
            else
            {
                AnscCopyMemory(owner_desp, pFileNameChar, 110);
            }
        }
    }

    AnscAddMemoryAllocItem
        (
            pComponentName,
            pMemoryPointer,
            ulMemorySize,
            ulCurAllocId,
            owner_desp
        );

    return  pMemoryPointer;
}


void*
AnscReAllocateMemoryRecordDetail
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize,
        char*                       pFileName,
        ULONG                       ulLineNumber
    )
{
    void*                           pOldMemory = (void*)pMemoryBlock;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = AnscGetMemorySizeRecordDetail(pComponentName, pOldMemory);
    ULONG                           ulNewSize  = ulMemorySize;
    ULONG                           ulCpySize  = AnscGetMin2(ulOldSize, ulNewSize);

    pNewMemory = AnscAllocateMemoryRecordDetail(pComponentName, ulNewSize, pFileName, ulLineNumber);

    if ( !pNewMemory )
    {
        CcspTraceWarning2(pComponentName, ("AnscReAllocateMemory5 -- failed to allocate %lu bytes memory for %s:%lu!!!", ulMemorySize, pFileName, ulLineNumber));

        return  NULL;
    }
    else
    {
        AnscCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    AnscFreeMemoryRecordDetail(pComponentName, pOldMemory);

    return  pNewMemory;
}


void
AnscFreeMemoryRecordDetail
    (
        char*                       pComponentName,
        PVOID                       pMemoryBlock
    )
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PUCHAR                          pMemoryPointer   = NULL;
    PUCHAR                          pMemoryArray1    = NULL;
    PUCHAR                          pMemoryArray2    = NULL;
    ULONG                           ulMemorySize     = 0;
    ULONG                           i                = 0;

    PANSC_MEMORY_ALLOC_ITEM         pPrevAllocItem   = NULL;
    BOOLEAN                         bHeadCorrupted   = FALSE;
    BOOLEAN                         bTailCorrupted   = FALSE;

    if ( !pMemoryBlock )
    {
        return;
    }
    else
    {
        pMemoryPointer = (PVOID)((ULONG)pMemoryBlock - sizeof(ULONG) - ANSC_MEMORY_ALLOC_HEAD_SIZE);
        ulMemorySize   = *(PULONG)pMemoryPointer;
    }

    AnscMemoryCountSize(pComponentName, ulMemorySize, FALSE);

    /*
     * Before we actually free the memory, we need to compare the signatures before and after the
     * memory block. If either signature has been modified, we need to do something noticeable to
     * warn the developer.
     */
    if ( TRUE )
    {
        pMemoryAllocItem = (PANSC_MEMORY_ALLOC_ITEM)AnscGetMemoryAllocItem(pMemoryBlock);

        if ( !pMemoryAllocItem ) 
        {
            CcspTraceWarning2
                (pComponentName, (
                    "!! memory block 0x%p has either been freed already or never allocated !!\n",
                    pMemoryBlock
                ));
            return;
        }

        pMemoryArray1    = (PUCHAR                 )((ULONG)pMemoryBlock - ANSC_MEMORY_ALLOC_HEAD_SIZE);
        pMemoryArray2    = (PUCHAR                 )((ULONG)pMemoryBlock + ulMemorySize);

        for ( i = 0; i < ANSC_MEMORY_ALLOC_TAIL_SIZE; i++ )
        {
            if ( pMemoryArray2[i] != 0x7A )
            {
                bTailCorrupted = TRUE;

                CcspTraceWarning2
                    (pComponentName, (
                        "!!Sky is falling down, memory block 0x%p was written out of boundary in Tail!! owner = %s, size = %lu/%lu!\n",
                        pMemoryBlock,
                        pMemoryAllocItem->OwnerDesp,
                        ulMemorySize,
                        pMemoryAllocItem->MemorySize
                    ));

                break;
            }
        }

        for ( i = 0; i < ANSC_MEMORY_ALLOC_HEAD_SIZE; i++ )
        {
            if ( pMemoryArray1[i] != 0x5C )
            {
                bHeadCorrupted = TRUE;

                CcspTraceWarning2
                    (pComponentName, (
                        "!!Sky is falling down, memory block 0x%p was written out of boundary in Head!! owner = %s, size = %lu/%lu, ",
                        pMemoryBlock,
                        pMemoryAllocItem->OwnerDesp,
                        ulMemorySize,
                        pMemoryAllocItem->MemorySize
                    ));

                pPrevAllocItem = AnscGetPrevMemoryAllocItem(pMemoryBlock);

                if ( !pPrevAllocItem )
                {
                    CcspTraceWarning2(pComponentName, ("Cannot find the memory block before this one!!!\n"));
                }
                else
                {
                    pMemoryArray1    = (PUCHAR)((ULONG)pPrevAllocItem->MemoryPointer - ANSC_MEMORY_ALLOC_HEAD_SIZE);
                    pMemoryArray2    = (PUCHAR)((ULONG)pPrevAllocItem->MemoryPointer + pPrevAllocItem->MemorySize);

                    CcspTraceWarning2
                        (pComponentName, (
                            "Found the memory block before this one, memory pointer = 0x%p, owner = %s. Head/Tail content = 0x%lu 0x%lu/0x%lu 0x%lu.\n",
                            pPrevAllocItem->MemoryPointer,
                            pPrevAllocItem->OwnerDesp,
                            AnscReadUlong(pMemoryArray1),
                            AnscReadUlong((pMemoryArray1 + 4)),
                            AnscReadUlong(pMemoryArray2),
                            AnscReadUlong((pMemoryArray2 + 4))
                        ));
                }

                break;
            }
        }

        if ( bHeadCorrupted || bTailCorrupted )
        {
            ULONG                   ulIndex         = 0;
            ULONG                   ulFullMemSize   = sizeof(ULONG) + ANSC_MEMORY_ALLOC_HEAD_SIZE + ulMemorySize + ANSC_MEMORY_ALLOC_TAIL_SIZE;

            CcspTraceWarning2(pComponentName, ("Dump out the content of memory block 0x%p, size = %lu, full size = %lu:", pMemoryBlock, ulMemorySize, ulFullMemSize));

            for ( ulIndex = 0; ulIndex < (ulFullMemSize - (ulFullMemSize % 16)); ulIndex++ )
            {
                if ( (ulIndex % 16) == 0 )
                {
                    CcspTraceWarning(("\n%8p: %02X", pMemoryPointer + ulIndex, pMemoryPointer[ulIndex]));
                }
                else
                {
                    CcspTraceWarning((" %02X", pMemoryPointer[ulIndex]));
                }
            }

            for ( ; ulIndex < ulFullMemSize; ulIndex++ )
            {
                if ( (ulIndex % 16) == 0 )
                {
                    CcspTraceWarning(("\n%8p: %02X", pMemoryPointer + ulIndex, pMemoryPointer[ulIndex]));
                }
                else
                {
                    CcspTraceWarning((" %02X", pMemoryPointer[ulIndex]));
                }
            }

            CcspTraceWarning(("\n"));

        }
    }

    /*
     *  zero out the memory block, which helps on finding out whether it is being referenced by other parties
     */
    //_ansc_memset(pMemoryPointer, 0, sizeof(ULONG) + ANSC_MEMORY_ALLOC_HEAD_SIZE + ulMemorySize + ANSC_MEMORY_ALLOC_TAIL_SIZE);
    memset(pMemoryPointer, 0, sizeof(ULONG) + ANSC_MEMORY_ALLOC_HEAD_SIZE + ulMemorySize + ANSC_MEMORY_ALLOC_TAIL_SIZE);

    AnscDelMemoryAllocItem(pMemoryBlock);

    //AnscFlushMemory(pMemoryPointer);
    AnscFreeMemoryOrig(pMemoryPointer);
}

void AnscPrintComponentMemoryDetail                                                                 
(               
    char*  pComponentName
);  

/*
    This function inspects current memory health.
    If there is voliation, just print warning information.
    added by Yan 
*/
void AnscLiveMemoryInspectRecordDetail
(               
    void
)
{
    PANSC_MEMORY_ALLOC_ITEM         pMemoryAllocItem = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    PUCHAR                          pMemoryPointer   = NULL;
    PUCHAR                          pMemoryArray1    = NULL;
    PUCHAR                          pMemoryArray2    = NULL;
    ULONG                           ulMemorySize     = 0;
    ULONG                           i                = 0;
    ULONG                           j                = 0;

    PANSC_MEMORY_ALLOC_ITEM         pPrevAllocItem   = NULL;
    BOOLEAN                         bHeadCorrupted   = FALSE;
    BOOLEAN                         bTailCorrupted   = FALSE;
    PVOID                           pMemoryBlock     = NULL;
    UCHAR                           uMaxCount        = 10;

    AnscAcquireSpinLock(&g_tRecordDetailSpinLock);
    
    CcspTraceWarning(("In AnscLiveMemoryInspectRecordDetail-- begin.\n"));

    for ( i = 0; i < ANSC_MEMORY_ALLOC_TABLE_SIZE && uMaxCount; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_tAllocTable[i]);
        /*
        CcspTraceWarning(( "\n\n\n!!!!!!!!!!!!!!!!!!!%dth:%d \n\n\n", i, g_tAllocTable[i].Depth ));
        */        
        while ( pSLinkEntry )
        {
            pMemoryAllocItem = ACCESS_ANSC_MEMORY_ALLOC_ITEM(pSLinkEntry);
            pSLinkEntry      = AnscSListGetNextEntry(pSLinkEntry);

            pMemoryBlock     = pMemoryAllocItem->MemoryPointer;
            pMemoryPointer = (PVOID)((ULONG)pMemoryBlock - sizeof(ULONG) - ANSC_MEMORY_ALLOC_HEAD_SIZE);
            ulMemorySize   = *(PULONG)pMemoryPointer;

            /*
            CcspTraceWarning
                ((
                    "pMemoryBlock = %x!!owner = %s, size = %lu/%lu!\n",
                    pMemoryBlock,
                    pMemoryAllocItem->OwnerDesp,
                    ulMemorySize,
                    pMemoryAllocItem->MemorySize
                ));*/

            /*
                       * We need to compare the signatures before and after the memory block. 
                       * If either signature has been modified, we need to do something noticeable to
                       * warn the developer.
                       */
            if ( TRUE )
            {
                
		/*CID:137611 Value not atomically updated*/
		/* pMemoryBlock might now be unreliable */
                pMemoryAllocItem = (PANSC_MEMORY_ALLOC_ITEM)AnscGetMemoryAllocItem(pMemoryBlock);                
                if ( !pMemoryAllocItem )
                {
                    CcspTraceWarning2(pComponentName, ("Cannot find the memory block before this one!!!\n"));
		    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);
                    return;
                }

                pMemoryArray1    = (PUCHAR                 )((ULONG)pMemoryBlock - ANSC_MEMORY_ALLOC_HEAD_SIZE);
                pMemoryArray2    = (PUCHAR                 )((ULONG)pMemoryBlock + ulMemorySize);
            
                for ( j = 0; j < ANSC_MEMORY_ALLOC_TAIL_SIZE; j++ )
                {
                    if ( pMemoryArray2[j] != 0x7A )
                    {
                        bTailCorrupted = TRUE;
            
                        CcspTraceWarning2
                            (pComponentName, (
                                "!!Sky is falling down, memory block 0x%p was written out of boundary in Tail!! owner = %s, size = %lu/%lu!\n",
                                pMemoryBlock,
                                pMemoryAllocItem->OwnerDesp,
                                ulMemorySize,
                                pMemoryAllocItem->MemorySize
                            ));
            
                        break;
                    }
                }
            
                for ( j = 0; j < ANSC_MEMORY_ALLOC_HEAD_SIZE; j++ )
                {
                    if ( pMemoryArray1[j] != 0x5C )
                    {
                        bHeadCorrupted = TRUE;
            
                        CcspTraceWarning2
                            (pComponentName, (
                                "!!Sky is falling down, memory block 0x%p was written out of boundary in Head!! owner = %s, size = %lu/%lu, ",
                                pMemoryBlock,
                                pMemoryAllocItem->OwnerDesp,
                                ulMemorySize,
                                pMemoryAllocItem->MemorySize
                            ));
            
                        pPrevAllocItem = AnscGetPrevMemoryAllocItem(pMemoryBlock);

#ifdef _64BIT_ARCH_SUPPORT_
                        printf("found %p\n",pPrevAllocItem);
#else
                        printf("found %x\n",(UINT)pPrevAllocItem);
#endif
                        
                        if ( !pPrevAllocItem )
                        {
                            CcspTraceWarning2(pComponentName, ("Cannot find the memory block before this one!!!\n"));
                        }
                        else
                        {
                            pMemoryArray1    = (PUCHAR)((ULONG)pPrevAllocItem->MemoryPointer - ANSC_MEMORY_ALLOC_HEAD_SIZE);
                            pMemoryArray2    = (PUCHAR)((ULONG)pPrevAllocItem->MemoryPointer + pPrevAllocItem->MemorySize);
            
                            CcspTraceWarning2
                                (pComponentName, (
                                    "Found the memory block before this one, memory pointer = 0x%p, owner = %s. Head/Tail content = 0x%lu 0x%lu/0x%lu 0x%lu.\n",
                                    pPrevAllocItem->MemoryPointer,
                                    pPrevAllocItem->OwnerDesp,
                                    AnscReadUlong(pMemoryArray1),
                                    AnscReadUlong((pMemoryArray1 + 4)),
                                    AnscReadUlong(pMemoryArray2),
                                    AnscReadUlong((pMemoryArray2 + 4))
                                ));
                        }

                        break;
                    }
                }

                if ( bHeadCorrupted || bTailCorrupted )
                {
                    ULONG                   ulIndex         = 0;
                    ULONG                   ulFullMemSize   = sizeof(ULONG) + ANSC_MEMORY_ALLOC_HEAD_SIZE + ulMemorySize + ANSC_MEMORY_ALLOC_TAIL_SIZE;
            
                    CcspTraceWarning2(pComponentName, ("Dump out the content of memory block 0x%p, size = %lu, full size = %lu:", pMemoryBlock, ulMemorySize, ulFullMemSize));
            
                    for ( ulIndex = 0; ulIndex < (ulFullMemSize - (ulFullMemSize % 16)); ulIndex++ )
                    {
                        if ( (ulIndex % 16) == 0 )
                        {
                            CcspTraceWarning(("\n%8p: %02X", pMemoryPointer + ulIndex, pMemoryPointer[ulIndex]));
                        }
                        else
                        {
                            CcspTraceWarning((" %02X", pMemoryPointer[ulIndex]));
                        }
                    }
            
                    for ( ; ulIndex < ulFullMemSize; ulIndex++ )
                    {
                        if ( (ulIndex % 16) == 0 )
                        {
                            CcspTraceWarning(("\n%8p: %02X", pMemoryPointer + ulIndex, pMemoryPointer[ulIndex]));
                        }
                        else
                        {
                            CcspTraceWarning((" %02X", pMemoryPointer[ulIndex]));
                        }
                    }
            
                    CcspTraceWarning(("\n"));

                    uMaxCount--;
                    
                    bHeadCorrupted = FALSE;
                    bTailCorrupted = FALSE;
                }

            }
            
        }
    }

    CcspTraceWarning(("In AnscLiveMemoryInspectRecordDetail-- end.\n"));
    
    AnscReleaseSpinLock(&g_tRecordDetailSpinLock);
    
    return;
}


void*
Ansc_AllocateMemory_Callback
    (
        ULONG                       ulMemorySize
    )
{
    return AnscAllocateMemory(ulMemorySize);
}

void
Ansc_FreeMemory_Callback
    (
        void*                       pMemory
    )
{
    return AnscFreeMemory((char *)pMemory);
}



