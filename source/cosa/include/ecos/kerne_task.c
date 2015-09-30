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

    module:	kernel_task.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelCreateTask
        *   KernelLeaveTask

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


#ifndef  _KERNEL_TASK_
#define  _KERNEL_TASK_


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"
#include "kernel_runtime.h"


/**********************************************************************
                          DEFINITIONS
**********************************************************************/

#define  ACCESS_CONTAINER(address, type, field)     \
         ((type*)((PCHAR)(address) - (ULONG)(&((type*)0)->field)))

typedef  struct
_KERNEL_TASK_MEM_BLOCK
{
    KERNEL_TASK_MEM_BLOCK*          Prev;
    KERNEL_TASK_MEM_BLOCK*          Next;
    cyg_thread                      ThreadInfo;
}
KERNEL_TASK_MEM_BLOCK, *PKERNEL_TASK_MEM_BLOCK;

#define  ACCESS_KERNEL_TASK_MEM_BLOCK1(p)       ACCESS_CONTAINER(p, KERNEL_TASK_MEM_BLOCK, Linkage)
#define  ACCESS_KERNEL_TASK_MEM_BLOCK2(p)       ACCESS_CONTAINER(p, KERNEL_TASK_MEM_BLOCK, ThreadInfo)


typedef  struct
_KERNEL_TASK_MEM_BLOCK_POOL
{
    KERNEL_TASK_MEM_BLOCK*          Prev;
    KERNEL_TASK_MEM_BLOCK*          Next;
}
KERNEL_TASK_MEM_BLOCK_POOL, *PKERNEL_TASK_MEM_BLOCK_POOL;


/**********************************************************************
                          DEFINITIONS
**********************************************************************/

KERNEL_SPINLOCK                     gMemBlockPoolLock       = 0;
KERNEL_TASK_MEM_BLOCK_POOL          gMemBlockPool           = {0};

/**********************************************************************
                          ROUTINE TRUNKS
**********************************************************************/

/*
 *  eCos doesn't clean the memory blocks of stack and thread info required to create
 *  a thread, if they are dynamically allocated. Here is how we clean them:
 *      1) when a thread is created, we allocate a memory block for both stack and thread info.
 *      2) The memory block pointer is stored into thread data, index 0
 *      3) When a thread exits, it retrieves the memory block pointer from thread data, index 0
 *          and put it into a pool
 *      4) Next time when another thread exits, it first cleans up the memory block pool.
 *      5) In step 3) and 4), a spin lock is used to protect the memory block pool.
 */
KERNEL_HANDLE
KernelCreateTask
    (
        cyg_thread_entry_t*         startEntry,
        ULONG                       stackSize,
        ULONG                       priority,
        KERNEL_HANDLE               hContext,
        char*                       pName
    )
{
    ULONG                           ulSize      = 0;
    PKERNEL_TASK_MEM_BLOCK          pMem        = NULL;
    PUCHAR                          pStack      = NULL;
    cyg_thread*                     pThreadInfo = NULL;
    cyg_handle_t                    hThread     = 0;

    ulSize  = (sizeof(KERNEL_TASK_MEM_BLOCK) + CYGNUM_HAL_STACK_FRAME_SIZE) / CYGNUM_HAL_STACK_FRAME_SIZE * CYGNUM_HAL_STACK_FRAME_SIZE;

    pMem    = (PKERNEL_TASK_MEM_BLOCK)KernelAllocateMemory(ulSize + stackSize);

    if ( pMem )
    {
        pStack      = pMem + ulSize;
        pThreadInfo = &pMem->ThreadInfo;

        cyg_thread_create
            (
                priority,
                startEntry,
                hContext,
                pName,
                pStack,
                stackSize,
                &hThread,
                pThreadInfo
            );

        if ( hThread )
        {
            KernelTrace
                (
                    "KernelCreateTask -- memory block pointer for task %s is 0x%lX.\n",
                    pName,
                    pMem
                );

            cyg_thread_set_data(0, pMem);

            cyg_thread_resume(hThread);

            return  hThread;
        }
    }

    if ( pMem )
    {
        KernelFreeMemory(pMem);
    }

    return  (KERNEL_HANDLE)NULL;
}


void
KernelLeaveTask
    (
        void
    )
{
    PKERNEL_TASK_MEM_BLOCK          pMem        = NULL;

    KernelAcquireSpinLock(&gMemBlockPoolLock);

    if ( TRUE )
    {
        pMem = gMemBlockPool.Next;

        while ( pMem )
        {
            gMemBlockPool.Next = pMem->Next;

            KernelFreeMemory(pMem);

            pMem = gMemBlockPool.Next;
        }

        gMemBlockPool.Prev = NULL;
        gMemBlockPool.Next = NULL;
    }

    if ( TRUE )
    {
        pMem = (PKERNEL_TASK_MEM_BLOCK)cyg_thread_get_data(0);

        if ( pMem )
        {
            gMemBlockPool.Prev = pMem;
            gMemBlockPool.Next = pMem;
        }
    }

    KernelReleaseSpinLock(&gMemBlockPoolLock);

    cyg_thread_exit();

    return;
}


#endif

