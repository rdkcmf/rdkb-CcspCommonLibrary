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

    module:	kernel_memory.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelAllocateMemory
        *   KernelResizeMemory

    ---------------------------------------------------------------

    environment:

        eCos 1.3.1

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        05/01/06    initial revision.

**********************************************************************/


#include "kernel_base.h"
#include "kernel_memory.h"
#include "kernel_debug.h"
#include "kernel_protection.h"


/**********************************************************************
    WHAT A PAIN TO HAVE TO HANDLE THIS WITHOUT BASE DEFINITIONS!!!
**********************************************************************/

#define  CONTAINING_RECORD(address, type, field)    \
            ((type*)((PCHAR)(address) - (ULONG)(&((type*)0)->field)))

/* DOUBLY LINKED LIST */
typedef struct
_LIST_ENTRY
{
   struct _LIST_ENTRY               *Flink;
   struct _LIST_ENTRY               *Blink;
}
LIST_ENTRY, *PLIST_ENTRY;

/***********************************************************
                    DOUBLY LINKED LIST
***********************************************************/

/*
VOID
ALInitializeListHead
    (
        PLIST_ENTRY                 ListHead
    )
 */
#define  ALInitializeListHead(ListHead)             ((ListHead)->Flink = (ListHead)->Blink = (ListHead))

/*
BOOLEAN
ALIsListEmpty
    (
        PLIST_ENTRY                 ListHead
    )
 */
#define  ALIsListEmpty(ListHead)                    ((ListHead)->Flink == (ListHead))

/*
PLIST_ENTRY
ALGetFirstEntryList
    (
        PLIST_ENTRY                 ListHead
    )
 */
#define  ALGetFirstEntryList(ListHead)              (ListHead)->Flink

/*
VOID
ALInitializeListEntry
    (
        PLIST_ENTRY                 pListEntry
    )
 */
#define  ALInitializeListEntry(pListEntry)          ((pListEntry)->Flink = (pListEntry)->Blink = NULL)

/*
BOOLEAN
ALIsListEntryValid
    (
        PLIST_ENTRY                 pListEntry
    )
 */
#define  ALIsListEntryValid(pListEntry)             ( ((pListEntry)->Flink != NULL) && ((pListEntry)->Blink != NULL) )

/*
BOOLEAN
ALListIsRealEntry
    (
        PLIST_ENTRY                 ListHead,
        PLIST_ENTRY                 pListEntry
    )
 */
#define  ALListIsRealEntry(ListHead, pListEntry)    ((pListEntry != NULL) && (pListEntry != ListHead))

/*
PLIST_ENTRY
ALGetNextEntryList
    (
        PLIST_ENTRY                 pListEntry
    )
 */
#define  ALGetNextEntryList(Entry)                  (Entry)->Flink

/*
PLIST_ENTRY
ALRemoveHeadList
    (
        PLIST_ENTRY                 ListHead
    )
 */
#define  ALRemoveHeadList(ListHead)                 (ListHead)->Flink;{ALRemoveEntryList((ListHead)->Flink);}

/*
PLIST_ENTRY
ALRemoveTailList
    (
        PLIST_ENTRY                 ListHead
    )
 */
#define  ALRemoveTailList(ListHead)                 (ListHead)->Blink;{ALRemoveEntryList((ListHead)->Blink);}

/*
VOID
ALRemoveEntryList
    (
        PLIST_ENTRY                 Entry
    )
 */
#define  ALRemoveEntryList(Entry)                               \
            {                                                   \
                PLIST_ENTRY _EX_Blink;                          \
                PLIST_ENTRY _EX_Flink;                          \
                _EX_Flink           = (Entry)->Flink;           \
                _EX_Blink           = (Entry)->Blink;           \
                _EX_Blink->Flink    = _EX_Flink;                \
                _EX_Flink->Blink    = _EX_Blink;                \
            }

/*
VOID
ALInsertHeadList
    (
        PLIST_ENTRY                 ListHead,
        PLIST_ENTRY                 Entry
    )
 */
#define  ALInsertHeadList(ListHead,Entry)                       \
            {                                                   \
                PLIST_ENTRY _EX_Flink;                          \
                PLIST_ENTRY _EX_ListHead;                       \
                _EX_ListHead        = (ListHead);               \
                _EX_Flink           = _EX_ListHead->Flink;      \
                (Entry)->Flink      = _EX_Flink;                \
                (Entry)->Blink      = _EX_ListHead;             \
                _EX_Flink->Blink    = (Entry);                  \
                _EX_ListHead->Flink = (Entry);                  \
            }

/*
VOID
ALInsertTailList
    (
        PLIST_ENTRY                 ListHead,
        PLIST_ENTRY                 Entry
    )
 */
#define  ALInsertTailList(ListHead,Entry)                       \
            {                                                   \
                PLIST_ENTRY _EX_Blink;                          \
                PLIST_ENTRY _EX_ListHead;                       \
                _EX_ListHead        = (ListHead);               \
                _EX_Blink           = _EX_ListHead->Blink;      \
                (Entry)->Flink      = _EX_ListHead;             \
                (Entry)->Blink      = _EX_Blink;                \
                _EX_Blink->Flink    = (Entry);                  \
                _EX_ListHead->Blink = (Entry);                  \
            }

/*
VOID
ALInsertFrontList
    (
        PLIST_ENTRY                 NextEntry,
        PLIST_ENTRY                 Entry
    )
 *
 *  Insert the entry before NextEntry which is in the list
 */
#define  ALInsertFrontList(NextEntry,Entry)                     \
            {                                                   \
                PLIST_ENTRY _EX_Blink;                          \
                PLIST_ENTRY _EX_NextEntry;                      \
                _EX_NextEntry       = (NextEntry);              \
                _EX_Blink           = _EX_NextEntry->Blink;     \
                (Entry)->Flink      = _EX_NextEntry;            \
                (Entry)->Blink      = _EX_Blink;                \
                _EX_Blink->Flink    = (Entry);                  \
                _EX_NextEntry->Blink= (Entry);                  \
            }



/**********************************************************************
                            CODE SECTION
**********************************************************************/

#ifdef   _KERNEL_USE_MEMORY_TRACKING_

/**********************************************************************
                        GLOBAL VARIABLES
**********************************************************************/

static  BOOLEAN                     gMemAllocMonitorInitialized = FALSE;
static  KERNEL_LOCK                   gMemAllocMonitorLock;
static  ULONG                       gMemAllocTotalSize          = 0;
static  ULONG                       gNumberMemAllocated         = 0;

#if     defined(_KERNEL_MEMORY_TRACKING_METHOD_1_)

/**********************************************************************
                     STRUCTURE DEFINITIONS
**********************************************************************/

typedef struct _AL_MEM_PREHEADER
{
    LIST_ENTRY          Linkage;

    /* true size of memory allocation we were requested */
    ULONG               MemTrueSize;

    /*
     *  GNU compiler put full-path file name for __FILE__
     */
    char                Description[48];

    /*
     *  eliminate usage of zero-sized array
     *
    UCHAR               MemData[0];
     */
}
AL_MEM_PREHEADER, *PAL_MEM_PREHEADER;

#define ACCESS_MEM_FROM_PREHEADER(pHeader)          (PVOID)( ((PUCHAR)pHeader) + sizeof(AL_MEM_PREHEADER))
#define ACCESS_PREHEADER_FROM_MEM(pMem)             (PAL_MEM_PREHEADER)( ((PUCHAR)pMem) - sizeof(AL_MEM_PREHEADER))
#define ACCESS_PREHEADER_FROM_LINKAGE(pListEntry)   (PAL_MEM_PREHEADER)CONTAINING_RECORD(pListEntry, AL_MEM_PREHEADER, Linkage)

/**********************************************************************
                        GLOBAL VARIABLES
**********************************************************************/

static  LIST_ENTRY                  gMemAllocMonitorList;


/**********************************************************************
                        FUNCTION PROTOTYPES
**********************************************************************/

VOID
_KernelDumpMemoryLeak
    (
        void
    );

/**********************************************************************
                        ROUTINE TRUNKS
**********************************************************************/

VOID
__KernelInitializeMemAllocMonitor(void)
{
    KernelInitializeLock(&gMemAllocMonitorLock);
    ALInitializeListHead(&gMemAllocMonitorList);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    KernelTrace
        (
            "__KernelInitializeMemAllocMonitor -- AL_MEM_PREHEADER size = %u ...\n",
            sizeof(AL_MEM_PREHEADER)
        );

    return;
}


PVOID
__KernelAllocateMemory
    (
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER               pHeader;
    PVOID                           pMem    = NULL;

    if ( !gMemAllocMonitorInitialized )
    {
        __KernelInitializeMemAllocMonitor();
    }

    KernelAssert(ulSize != 0);

    /*DH  DEBUG*/
    /*
    KernelTrace
        (
            "__KernelAllocateMemory -- size = %lu, File: %s, Line: %lu\n",
            ulSize,
            pFileName,
            LineNumber
        );
     */

    pHeader =  (PAL_MEM_PREHEADER)RealKernelAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER));

    if (pHeader)
    {
        KernelZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER));

        pHeader->MemTrueSize = ulSize;

        pMem = ACCESS_MEM_FROM_PREHEADER(pHeader);

        strcpy(pHeader->Description, pFileName);
        strcat(pHeader->Description, ":");
        sprintf(&pHeader->Description[strlen(pHeader->Description)], "%u", LineNumber);

        KernelAcquireLock(&gMemAllocMonitorLock);

        /* put it into monitoring list */
        ALInsertTailList(&gMemAllocMonitorList, &pHeader->Linkage);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        KernelReleaseLock(&gMemAllocMonitorLock);

        /*DH  DEBUG*/
        /*
        KernelTrace
            (
                "__KernelAllocateMemory -- allocated memory = 0x%X/0x%X\n",
                (unsigned int)pHeader,
                (unsigned int)pMem
            );
         */

        return pMem;
    }
    else
    {
        KernelTrace
            (
                "__KernelAllocateMemory failed, size = %lu, File: %s, Line: %lu\n",
                ulSize,
                pFileName,
                LineNumber
            );

        /*DH  DEBUG*/
        _KernelDumpMemoryLeak();
        KernelAssert(FALSE);

        return NULL;
    }
}


VOID
__KernelFreeMemory
    (
        PVOID                       pMem,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER               pHeader;
    PLIST_ENTRY                     pListEntryTobeFreed;
    BOOLEAN                         bFound                  = FALSE;
    PLIST_ENTRY                     pListEntry;
    ULONG                           ulSize;

    KernelAssert(gMemAllocMonitorInitialized);

    /*DH  DEBUG*/
    /*
    KernelTrace
        (
            "__KernelFreeMemory -- pMem = 0x%X, File: %s, Line: %lu\n",
            (unsigned int)pMem,
            pFileName,
            LineNumber
        );
     */

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);
    pListEntryTobeFreed = &pHeader->Linkage;

    /*
     *  check if this one is still in memory pool though it may be too late
     */
    KernelAcquireLock(&gMemAllocMonitorLock);

    if (!ALIsListEmpty(&gMemAllocMonitorList))
    {
        pListEntry = ALGetFirstEntryList(&gMemAllocMonitorList);

        while (pListEntry != NULL && pListEntry != &gMemAllocMonitorList)
        {
            if (pListEntryTobeFreed == pListEntry)
            {
                bFound = TRUE;
                break;
            }

            pListEntry = ALGetNextEntryList(pListEntry);
        }

        if (!bFound)
        {
            KernelTrace
                (
                    "__KernelFreeMemory -- the memory block 0x%X is not currently in the memory pool, File: %s, Line: %lu!\n",
                    pMem,
                    pFileName,
                    LineNumber
                );

            goto ErrorExit;
        }
    }
    else
    {
        KernelTrace
            (
                "__KernelFreeMemory -- the memory monitoring list is empty! File: %s, Line: %lu!\n",
                pFileName,
                LineNumber
            );

        goto ErrorExit;
    }

    if (!ALIsListEntryValid(&pHeader->Linkage))
    {
        KernelTrace
            (
                "__KernelFreeMemory -- memory 0x%X is invalid, File: %s, Line: %lu!\n",
                pMem,
                pFileName,
                LineNumber
            );

        goto ErrorExit;
    }

    ulSize = pHeader->MemTrueSize;

    /*DH  DEBUG*/
    /*
    KernelTrace
        (
            "__KernelFreeMemory -- size = %lu\n",
            ulSize
        );
     */

    /* remove it from monitoring list */
    ALRemoveEntryList(&pHeader->Linkage);

    /* reset everything in the memory block */
    KernelZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER));

    RealKernelFreeMemory(pHeader);

    gMemAllocTotalSize  -= ulSize;
    gNumberMemAllocated --;


ErrorExit:

    KernelReleaseLock(&gMemAllocMonitorLock);

    return;
}


PVOID
__KernelReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = KernelGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    KernelTrace("__KernelReAllocMemory -- %s:%lu ...\n", pFileName, LineNumber);

    pNewMemory = __KernelAllocateMemory(ulNewSize, pFileName, LineNumber);

    if ( !pNewMemory )
    {
        return  NULL;
    }
    else
    {
        KernelCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    __KernelFreeMemory(pOldMemory, pFileName, LineNumber);

    return  pNewMemory;
}


ULONG
KernelGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    KernelAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


VOID
_KernelDumpMemoryLeak
    (
        void
    )
{
    PLIST_ENTRY                     pListEntry  = NULL;
    PAL_MEM_PREHEADER               pHeader     = NULL;

    KernelAcquireLock(&gMemAllocMonitorLock);

    if ( gNumberMemAllocated != 0 )
    {
        KernelTrace
            (
                "Memory usage: %lu memory blocks left, total size = %lu, dumped as following:\n",
                gNumberMemAllocated,
                gMemAllocTotalSize
            );
    }

    if (!ALIsListEmpty(&gMemAllocMonitorList))
    {
        pListEntry = ALGetFirstEntryList(&gMemAllocMonitorList);

        while (pListEntry != NULL && pListEntry != &gMemAllocMonitorList)
        {
            pHeader    = ACCESS_PREHEADER_FROM_LINKAGE(pListEntry);

            KernelTrace
                (
                    "0x%08X, size = %06lu, %s\n", 
                    (unsigned int)ACCESS_MEM_FROM_PREHEADER(pHeader),
                    pHeader->MemTrueSize, 
                    pHeader->Description
                );

            pListEntry = ALGetNextEntryList(pListEntry);
        }
    }

    KernelReleaseLock(&gMemAllocMonitorLock);

    return;
}

#elif   defined(_KERNEL_MEMORY_TRACKING_METHOD_2_)

typedef struct _AL_MEM_PREHEADER
{
    /* true size of memory allocation we were requested */
    ULONG               MemTrueSize;

    /*
    ULONG               PrePad[28];
     */
    /*
    UCHAR               MemData[0];
     */
}
AL_MEM_PREHEADER, *PAL_MEM_PREHEADER;

#define ACCESS_MEM_FROM_PREHEADER(pHeader)          (PVOID)(((PUCHAR)pHeader) + sizeof(AL_MEM_PREHEADER))
#define ACCESS_PREHEADER_FROM_MEM(pMem)             (PAL_MEM_PREHEADER)(((PUCHAR)pMem) - sizeof(AL_MEM_PREHEADER))

#define  AL_MEM_POST_PAD_SIZE                       0

VOID
__KernelInitializeMemAllocMonitor(void)
{
    KernelTrace
        (
            "__KernelInitializeMemAllocMonitor ...\n"
        );

    KernelInitializeLock(&gMemAllocMonitorLock);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    return;
}


PVOID
KernelAllocateMemory
    (
        ULONG                       ulSize
    )
{
    PAL_MEM_PREHEADER               pHeader;

    if ( !gMemAllocMonitorInitialized )
    {
        __KernelInitializeMemAllocMonitor();
    }

    pHeader =  RealKernelAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

    if ( pHeader )
    {
        KernelZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

        pHeader->MemTrueSize = ulSize;

        KernelAcquireLock(&gMemAllocMonitorLock);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        KernelReleaseLock(&gMemAllocMonitorLock);

        return ACCESS_MEM_FROM_PREHEADER(pHeader);
    }
    else
    {
        KernelTrace("KernelAllocateMemory -- insufficient resources for %lu bytes!!!\n", ulSize);
        KernelTrace("KernelAllocateMemory -- current number of allocated = %lu, size = %lu.\n", gNumberMemAllocated, gMemAllocTotalSize);

        /*DH  DEBUG*/
        memShow(0);
        taskShow(taskIdSelf(), 1);
        checkStack(0);

        /*force an exception*/
        *(PULONG)0 = 0;

        return NULL;
    }
}

VOID
KernelFreeMemory
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER           pHeader;

    pHeader = ACCESS_PREHEADER_FROM_MEM(pMem);

    KernelAssert(gMemAllocMonitorInitialized);

    KernelAcquireLock(&gMemAllocMonitorLock);

    gNumberMemAllocated --;
    gMemAllocTotalSize  -= pHeader->MemTrueSize;

    KernelReleaseLock(&gMemAllocMonitorLock);

    RealKernelFreeMemory(pHeader);

    return;
}


PVOID
__KernelReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = KernelGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    KernelTrace
        (
            "__KernelReAllocMemory -- %s:%lu, old size = %lu, new size = %lu, copy size = %lu ...\n",
            pFileName,
            LineNumber,
            ulOldSize,
            ulNewSize,
            ulCpySize
        );

    if ( ulNewSize <= ulOldSize )
    {
        KernelFreeMemory(pOldMemory);

        return  pOldMemory;
    }

    pNewMemory = KernelAllocateMemory(ulNewSize);

    if ( !pNewMemory )
    {
        return  NULL;
    }
    else
    {
        KernelCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    KernelFreeMemory(pOldMemory);

    return  pNewMemory;
}


ULONG
KernelGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    KernelAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


#elif   defined(_KERNEL_MEMORY_TRACKING_METHOD_3_)

typedef struct _AL_MEM_PREHEADER
{
    /* true size of memory allocation we were requested */
    ULONG               MemTrueSize;

    /*
    ULONG               PrePad[28];
     */
    /*
    UCHAR               MemData[0];
     */
}
AL_MEM_PREHEADER, *PAL_MEM_PREHEADER;

#define ACCESS_MEM_FROM_PREHEADER(pHeader)          (PVOID)(((PUCHAR)pHeader) + sizeof(AL_MEM_PREHEADER))
#define ACCESS_PREHEADER_FROM_MEM(pMem)             (PAL_MEM_PREHEADER)(((PUCHAR)pMem) - sizeof(AL_MEM_PREHEADER))

#define  AL_MEM_POST_PAD_SIZE                       0

VOID
__KernelInitializeMemAllocMonitor(void)
{
    KernelTrace
        (
            "__KernelInitializeMemAllocMonitor ...\n"
        );

    KernelInitializeLock(&gMemAllocMonitorLock);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    return;
}


extern   ULONG  GlobalDebugLevel;

PVOID
__KernelAllocateMemory
    (
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER               pHeader;

    if ( !gMemAllocMonitorInitialized )
    {
        __KernelInitializeMemAllocMonitor();
    }

    pHeader = RealKernelAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

    if ( pHeader )
    {
        if ( GlobalDebugLevel >= 8 )
        {
            KernelTrace("__KernelAllocateMemory -- %s:%lu, allocated pMem = 0x%x ...\n", pFileName, LineNumber, ACCESS_MEM_FROM_PREHEADER(pHeader));
        }

        KernelZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

        pHeader->MemTrueSize = ulSize;

        KernelAcquireLock(&gMemAllocMonitorLock);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        KernelReleaseLock(&gMemAllocMonitorLock);

        return ACCESS_MEM_FROM_PREHEADER(pHeader);
    }
    else
    {
        KernelTrace("KernelAllocateMemory -- insufficient resources for %lu bytes -- %s:%lu!!!\n", ulSize, pFileName, LineNumber);
        KernelTrace("KernelAllocateMemory -- current number of allocated = %lu, size = %lu.\n", gNumberMemAllocated, gMemAllocTotalSize);

        /*DH  DEBUG*/
        memShow(0);
        taskShow(taskIdSelf(), 1);
        checkStack(0);

        /*force an exception*/
        *(PULONG)0 = 0;

        return NULL;
    }
}

VOID
__KernelFreeMemory
    (
        PVOID                       pMem,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER           pHeader;

    if ( GlobalDebugLevel >= 8 )
    {
        KernelTrace("__KernelAllocateMemory -- %s:%lu, to free pMem = 0x%x ...\n", pFileName, LineNumber, pMem);
    }

    pHeader = ACCESS_PREHEADER_FROM_MEM(pMem);

    KernelAssert(gMemAllocMonitorInitialized);

    KernelAcquireLock(&gMemAllocMonitorLock);

    gNumberMemAllocated --;
    gMemAllocTotalSize  -= pHeader->MemTrueSize;

    KernelReleaseLock(&gMemAllocMonitorLock);

    RealKernelFreeMemory(pHeader);

    return;
}


PVOID
__KernelReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = KernelGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    KernelTrace
        (
            "__KernelReAllocMemory -- %s:%lu, old size = %lu, new size = %lu, copy size = %lu ...\n",
            pFileName,
            LineNumber,
            ulOldSize,
            ulNewSize,
            ulCpySize
        );

    if ( ulNewSize <= ulOldSize )
    {
        return  pOldMemory;
    }

    pNewMemory = KernelAllocateMemory(ulNewSize);

    if ( !pNewMemory )
    {
        KernelFreeMemory(pOldMemory);

        return  NULL;
    }
    else
    {
        KernelCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    KernelFreeMemory(pOldMemory);

    return  pNewMemory;
}


ULONG
KernelGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    KernelAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


#endif   /*_KERNEL_MEMORY_TRACKING_METHOD_2_*/


#else    /*_KERNEL_USE_MEMORY_TRACKING_*/

PVOID
KernelAllocateMemory
    (
        ULONG                       ulSize
    )
{
    PVOID                           pMem;

    if ( ulSize == 0 )
    {
        return  NULL;
    }
    
    pMem = RealKernelAllocateMemory(ulSize);

    if ( pMem == NULL )
    {
        KernelTrace("KernelAllocateMemory -- insufficient resources for %u bytes!!!\n", ulSize);

        /*
         *  DH  DEBUG
         *
        memShow(0);
        taskShow(taskIdSelf(), 1);
        checkStack(0);
         */
        /*force an exception*/
        /**(PULONG)0 = 0;*/
    }
    else
    {
        KernelZeroMemory(pMem, ulSize);
    }

    return  pMem;
}


#endif   /*_KERNEL_USE_MEMORY_TRACKING_*/


PVOID
KernelResizeMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    )
{
    /*DH  DEBUG*/
    /*KernelTrace("KernelResizeMemory ...\n");*/

    KernelFreeMemory(pMem);

    return  KernelAllocateMemory(ulSize);
}
