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

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelAllocateMemory
        *   KernelReAllocMemory
        *   KernelFreeMemory
        *   KernelGetMemorySize
        *   KernelResizeMemory

    ---------------------------------------------------------------

    environment:

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/

#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"


#ifdef   KERNEL_CHECKED_BUILD
#define  KERNEL_DYN_MEMORY_POOL_SIZE          20000000
#else
#define  KERNEL_DYN_MEMORY_POOL_SIZE          20000000
#endif

/**********************************************************************
                          GLOBAL VARIABLES
**********************************************************************/

POOL_ID                             gAnscDynMemoryPoolId    = 0;


PVOID
KernelAllocateMemory
    (
        ULONG                       ulMemorySize
    )
{
    BITS                            critLevel;
    PVOID                           p;

    if (gAnscDynMemoryPoolId == 0)
    {
        gAnscDynMemoryPoolId = pool_create_new(KERNEL_DYN_MEMORY_POOL_SIZE);

        KernelAssert(gAnscDynMemoryPoolId != 0);

        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_MEMORY,
                "===== Bisga memory pool = 0x%lX. =====\n",
                gAnscDynMemoryPoolId
            );
    }

    critLevel = atmos_startcritical();

    p = pool_alloc(gAnscDynMemoryPoolId, ulMemorySize);

    atmos_endcritical(critLevel);

    if (p != NULL)
    {
        memset(p, 0, ulMemorySize);

        KernelTrace2
            (
                KERNEL_DBG_LEVEL_DUMPING,
                KERNEL_DBG_MASK_MEMORY,
                "KernelAllocateMemory -- allocated the memory block 0x%X, size = %d, header = 0x%X.\n",
                p,
                ulMemorySize,
                (((POOL_MEMHANDLE *)p) - 1)
            );

        {
            POOL_MEMHANDLE*                 pMemoryHeader;

            pMemoryHeader = ((POOL_MEMHANDLE *)p) - 1;

            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_VERBOSE,
                    KERNEL_DBG_MASK_MEMORY,
                    "KernelAllocateMemory -- allocated the memory block 0x%X for size = %d, header = 0x%X, size = %d, pad = %d .\n",
                    p,
                    ulMemorySize,
                    pMemoryHeader,
                    pMemoryHeader->size,
                    pMemoryHeader->pad
                );
        }
    }
    else
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_MEMORY,
                "KernelAllocateMemory -- failed to allocate a memory block, for size = %d.\n",
                ulMemorySize
            );
    }

    return p;
}


PVOID
KernelReAllocMemory
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    )
{
    BITS                            critLevel;
    void*                           pOldMemory = (void*)pMemoryBlock;
    void*                           pNewMemory = NULL;
    ULONG                           ulOldSize  = 0;
    ULONG                           ulNewSize  = 0;
    ULONG                           ulCpySize  = 0;

    critLevel  = atmos_startcritical();

    ulOldSize  = KernelGetMemorySize(pOldMemory);
    ulNewSize  = ulMemorySize;
    ulCpySize  = (ulOldSize >= ulNewSize) ? ulNewSize : ulOldSize;

    pNewMemory = KernelAllocateMemory(ulNewSize);

    atmos_endcritical(critLevel);

    if ( !pNewMemory )
    {
        return  NULL;
    }
    else
    {
        KernelCopyMemory(pNewMemory, pOldMemory, ulCpySize);
    }

    critLevel = atmos_startcritical();

    KernelFreeMemory(pOldMemory);

    atmos_endcritical(critLevel);

    return  pNewMemory;
}


VOID
KernelFreeMemory
    (
        PVOID                       pMemoryBlock
    )
{
    BITS                            critLevel;
    BOOLEAN                         bSucceeded      = TRUE;

    /*DH  Why this could be called with NULL pointer!*/
    /*KernelAssert(pMemoryBlock != NULL);*/
    if ( pMemoryBlock == NULL )
    {
        return;
    }

    critLevel = atmos_startcritical();

    bSucceeded = pool_free(gAnscDynMemoryPoolId, pMemoryBlock);

    atmos_endcritical(critLevel);

    if ( !bSucceeded )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_MEMORY,
                "KernelFreeMemory -- failed to free memory block 0x%X.\n",
                pMemoryBlock
            );
    }

    return;
}


ULONG
KernelGetMemorySize
    (
        PVOID                       pMemoryBlock
    )
{
    POOL_MEMHANDLE*                 pMemoryHeader;

    pMemoryHeader = ((POOL_MEMHANDLE *)pMemoryBlock) - 1;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_MEMORY,
            "KernelGetMemorySize -- memory block header = 0x%X, size = %d, block = 0x%X.\n",
            pMemoryHeader,
            pMemoryBlock,
            pMemoryHeader->size
        );

    return  pMemoryHeader->size;
}


PVOID
KernelResizeMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    )
{
    BITS                            critLevel;
    PVOID                           pNewMem     = NULL;

    critLevel = atmos_startcritical();

    KernelFreeMemory(pMem);

    pNewMem = KernelAllocateMemory(ulSize);

    atmos_endcritical(critLevel);

    return  pNewMem;
}

PVOID
KernelGetMemoryInfo
    (
        VOID
    )
{
    size_t total;
    size_t pfree;
    size_t max;
    size_t frag;

    pool_stat(gAnscDynMemoryPoolId, &total, &pfree, &max, &frag);

    printf("Stats of Bisga Mem Pool:\n");
    printf("   Mem Pool ID:  0x%x\n", gAnscDynMemoryPoolId);
    printf("   Total Alloc:  %d\n", total);
    printf("   Total Free :  %d\n", pfree);
    printf("   Max free bk:  %d\n", max);
    printf("   Mean Frag  :  %d\n", frag);

} 
