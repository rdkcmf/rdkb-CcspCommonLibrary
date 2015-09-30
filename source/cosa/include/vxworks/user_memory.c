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

    module:	user_memory.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   UserAllocateMemory
        *   UserResizeMemory

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#include "user_base.h"
#include "user_memory.h"
#include "user_debug.h"
#include "user_protection.h"


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

#ifdef   _USER_USE_MEMORY_TRACKING_

/**********************************************************************
                        GLOBAL VARIABLES
**********************************************************************/

static  BOOLEAN                     gMemAllocMonitorInitialized = FALSE;
static  USER_LOCK                   gMemAllocMonitorLock;
static  ULONG                       gMemAllocTotalSize          = 0;
static  ULONG                       gNumberMemAllocated         = 0;

#if     defined(_USER_MEMORY_TRACKING_METHOD_1_)

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
_UserDumpMemoryLeak
    (
        void
    );

/**********************************************************************
                        ROUTINE TRUNKS
**********************************************************************/

VOID
__UserInitializeMemAllocMonitor(void)
{
    UserInitializeLock(&gMemAllocMonitorLock);
    ALInitializeListHead(&gMemAllocMonitorList);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    UserTrace
        (
            "__UserInitializeMemAllocMonitor -- AL_MEM_PREHEADER size = %u ...\n",
            sizeof(AL_MEM_PREHEADER)
        );

    return;
}


PVOID
__UserAllocateMemory
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
        __UserInitializeMemAllocMonitor();
    }

    UserAssert(ulSize != 0);

    /*DH  DEBUG*/
    /*
    UserTrace
        (
            "__UserAllocateMemory -- size = %lu, File: %s, Line: %lu\n",
            ulSize,
            pFileName,
            LineNumber
        );
     */

    pHeader =  (PAL_MEM_PREHEADER)RealUserAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER));

    if (pHeader)
    {
        UserZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER));

        pHeader->MemTrueSize = ulSize;

        pMem = ACCESS_MEM_FROM_PREHEADER(pHeader);

        strcpy(pHeader->Description, pFileName);
        strcat(pHeader->Description, ":");
        sprintf(&pHeader->Description[strlen(pHeader->Description)], "%u", LineNumber);

        UserAcquireLock(&gMemAllocMonitorLock);

        /* put it into monitoring list */
        ALInsertTailList(&gMemAllocMonitorList, &pHeader->Linkage);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        UserReleaseLock(&gMemAllocMonitorLock);

        /*DH  DEBUG*/
        /*
        UserTrace
            (
                "__UserAllocateMemory -- allocated memory = 0x%X/0x%X\n",
                (unsigned int)pHeader,
                (unsigned int)pMem
            );
         */

        return pMem;
    }
    else
    {
        UserTrace
            (
                "__UserAllocateMemory failed, size = %lu, File: %s, Line: %lu\n",
                ulSize,
                pFileName,
                LineNumber
            );

        /*DH  DEBUG*/
        _UserDumpMemoryLeak();
        UserAssert(FALSE);

        return NULL;
    }
}


VOID
__UserFreeMemory
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

    UserAssert(gMemAllocMonitorInitialized);

    /*DH  DEBUG*/
    /*
    UserTrace
        (
            "__UserFreeMemory -- pMem = 0x%X, File: %s, Line: %lu\n",
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
    UserAcquireLock(&gMemAllocMonitorLock);

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
            UserTrace
                (
                    "__UserFreeMemory -- the memory block 0x%X is not currently in the memory pool, File: %s, Line: %lu!\n",
                    pMem,
                    pFileName,
                    LineNumber
                );

            goto ErrorExit;
        }
    }
    else
    {
        UserTrace
            (
                "__UserFreeMemory -- the memory monitoring list is empty! File: %s, Line: %lu!\n",
                pFileName,
                LineNumber
            );

        goto ErrorExit;
    }

    if (!ALIsListEntryValid(&pHeader->Linkage))
    {
        UserTrace
            (
                "__UserFreeMemory -- memory 0x%X is invalid, File: %s, Line: %lu!\n",
                pMem,
                pFileName,
                LineNumber
            );

        goto ErrorExit;
    }

    ulSize = pHeader->MemTrueSize;

    /*DH  DEBUG*/
    /*
    UserTrace
        (
            "__UserFreeMemory -- size = %lu\n",
            ulSize
        );
     */

    /* remove it from monitoring list */
    ALRemoveEntryList(&pHeader->Linkage);

    /* reset everything in the memory block */
    UserZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER));

    RealUserFreeMemory(pHeader);

    gMemAllocTotalSize  -= ulSize;
    gNumberMemAllocated --;


ErrorExit:

    UserReleaseLock(&gMemAllocMonitorLock);

    return;
}


PVOID
__UserReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = UserGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    UserTrace("__UserReAllocMemory -- %s:%lu ...\n", pFileName, LineNumber);

    pNewMemory = __UserAllocateMemory(ulNewSize, pFileName, LineNumber);

    if ( !pNewMemory )
    {
        return  NULL;
    }
    else
    {
        UserCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    __UserFreeMemory(pOldMemory, pFileName, LineNumber);

    return  pNewMemory;
}


ULONG
UserGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    UserAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


VOID
_UserDumpMemoryLeak
    (
        void
    )
{
    PLIST_ENTRY                     pListEntry  = NULL;
    PAL_MEM_PREHEADER               pHeader     = NULL;

    UserAcquireLock(&gMemAllocMonitorLock);

    if ( gNumberMemAllocated != 0 )
    {
        UserTrace
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

            UserTrace
                (
                    "0x%08X, size = %06lu, %s\n", 
                    (unsigned int)ACCESS_MEM_FROM_PREHEADER(pHeader),
                    pHeader->MemTrueSize, 
                    pHeader->Description
                );

            pListEntry = ALGetNextEntryList(pListEntry);
        }
    }

    UserReleaseLock(&gMemAllocMonitorLock);

    return;
}

#elif   defined(_USER_MEMORY_TRACKING_METHOD_2_)

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
__UserInitializeMemAllocMonitor(void)
{
    UserTrace
        (
            "__UserInitializeMemAllocMonitor ...\n"
        );

    UserInitializeLock(&gMemAllocMonitorLock);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    return;
}


PVOID
UserAllocateMemory
    (
        ULONG                       ulSize
    )
{
    PAL_MEM_PREHEADER               pHeader;

    if ( !gMemAllocMonitorInitialized )
    {
        __UserInitializeMemAllocMonitor();
    }

    pHeader =  RealUserAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

    if ( pHeader )
    {
        UserZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

        pHeader->MemTrueSize = ulSize;

        UserAcquireLock(&gMemAllocMonitorLock);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        UserReleaseLock(&gMemAllocMonitorLock);

        return ACCESS_MEM_FROM_PREHEADER(pHeader);
    }
    else
    {
        UserTrace("UserAllocateMemory -- insufficient resources for %lu bytes!!!\n", ulSize);
        UserTrace("UserAllocateMemory -- current number of allocated = %lu, size = %lu.\n", gNumberMemAllocated, gMemAllocTotalSize);

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
UserFreeMemory
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER           pHeader;

    pHeader = ACCESS_PREHEADER_FROM_MEM(pMem);

    UserAssert(gMemAllocMonitorInitialized);

    UserAcquireLock(&gMemAllocMonitorLock);

    gNumberMemAllocated --;
    gMemAllocTotalSize  -= pHeader->MemTrueSize;

    UserReleaseLock(&gMemAllocMonitorLock);

    RealUserFreeMemory(pHeader);

    return;
}


PVOID
__UserReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = UserGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    UserTrace
        (
            "__UserReAllocMemory -- %s:%lu, old size = %lu, new size = %lu, copy size = %lu ...\n",
            pFileName,
            LineNumber,
            ulOldSize,
            ulNewSize,
            ulCpySize
        );

    if ( ulNewSize <= ulOldSize )
    {
        UserFreeMemory(pOldMemory);

        return  pOldMemory;
    }

    pNewMemory = UserAllocateMemory(ulNewSize);

    if ( !pNewMemory )
    {
        return  NULL;
    }
    else
    {
        UserCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    UserFreeMemory(pOldMemory);

    return  pNewMemory;
}


ULONG
UserGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    UserAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


#elif   defined(_USER_MEMORY_TRACKING_METHOD_3_)

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
__UserInitializeMemAllocMonitor(void)
{
    UserTrace
        (
            "__UserInitializeMemAllocMonitor ...\n"
        );

    UserInitializeLock(&gMemAllocMonitorLock);

    gMemAllocTotalSize          = 0;
    gNumberMemAllocated         = 0;
    gMemAllocMonitorInitialized = TRUE;

    return;
}


extern   ULONG  GlobalDebugLevel;

PVOID
__UserAllocateMemory
    (
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER               pHeader;

    if ( !gMemAllocMonitorInitialized )
    {
        __UserInitializeMemAllocMonitor();
    }

    pHeader = RealUserAllocateMemory(ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

    if ( pHeader )
    {
        if ( GlobalDebugLevel >= 8 )
        {
            UserTrace("__UserAllocateMemory -- %s:%lu, allocated pMem = 0x%x ...\n", pFileName, LineNumber, ACCESS_MEM_FROM_PREHEADER(pHeader));
        }

        UserZeroMemory(pHeader, ulSize + sizeof(AL_MEM_PREHEADER) + AL_MEM_POST_PAD_SIZE);

        pHeader->MemTrueSize = ulSize;

        UserAcquireLock(&gMemAllocMonitorLock);

        gMemAllocTotalSize  += ulSize;
        gNumberMemAllocated ++;

        UserReleaseLock(&gMemAllocMonitorLock);

        return ACCESS_MEM_FROM_PREHEADER(pHeader);
    }
    else
    {
        UserTrace("UserAllocateMemory -- insufficient resources for %lu bytes -- %s:%lu!!!\n", ulSize, pFileName, LineNumber);
        UserTrace("UserAllocateMemory -- current number of allocated = %lu, size = %lu.\n", gNumberMemAllocated, gMemAllocTotalSize);

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
__UserFreeMemory
    (
        PVOID                       pMem,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    PAL_MEM_PREHEADER           pHeader;

    if ( GlobalDebugLevel >= 8 )
    {
        UserTrace("__UserAllocateMemory -- %s:%lu, to free pMem = 0x%x ...\n", pFileName, LineNumber, pMem);
    }

    pHeader = ACCESS_PREHEADER_FROM_MEM(pMem);

    UserAssert(gMemAllocMonitorInitialized);

    UserAcquireLock(&gMemAllocMonitorLock);

    gNumberMemAllocated --;
    gMemAllocTotalSize  -= pHeader->MemTrueSize;

    UserReleaseLock(&gMemAllocMonitorLock);

    RealUserFreeMemory(pHeader);

    return;
}


PVOID
__UserReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        PCSTR                       pFileName,
        ULONG                       LineNumber
    )
{
    void*                           pOldMemory = (void*)pMem;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = UserGetMemorySize(pOldMemory);
    ULONG                           ulNewSize  = ulSize;
    ULONG                           ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    /*DH  DEBUG*/
    UserTrace
        (
            "__UserReAllocMemory -- %s:%lu, old size = %lu, new size = %lu, copy size = %lu ...\n",
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

    pNewMemory = UserAllocateMemory(ulNewSize);

    if ( !pNewMemory )
    {
        UserFreeMemory(pOldMemory);

        return  NULL;
    }
    else
    {
        UserCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    UserFreeMemory(pOldMemory);

    return  pNewMemory;
}


ULONG
UserGetMemorySize
    (
        PVOID                       pMem
    )
{
    PAL_MEM_PREHEADER               pHeader;

    UserAssert(gMemAllocMonitorInitialized);

    /*
     *  no need to validate the memory, since we don't really touch the memory 
     *  before we do the check against the list
     */
    pHeader             = ACCESS_PREHEADER_FROM_MEM(pMem);

    return  pHeader->MemTrueSize;
}


#endif   /*_USER_MEMORY_TRACKING_METHOD_2_*/


#else    /*_USER_USE_MEMORY_TRACKING_*/

PVOID
UserAllocateMemory
    (
        ULONG                       ulSize
    )
{
    PVOID                           pMem;

    if ( ulSize == 0 )
    {
        return  NULL;
    }
    
    pMem = RealUserAllocateMemory(ulSize);

    if ( pMem == NULL )
    {
        UserTrace("UserAllocateMemory -- insufficient resources for %u bytes!!!\n", ulSize);

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
        UserZeroMemory(pMem, ulSize);
    }

    return  pMem;
}


#endif   /*_USER_USE_MEMORY_TRACKING_*/


PVOID
UserResizeMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    )
{
    /*DH  DEBUG*/
    /*UserTrace("UserResizeMemory ...\n");*/

    UserFreeMemory(pMem);

    return  UserAllocateMemory(ulSize);
}
