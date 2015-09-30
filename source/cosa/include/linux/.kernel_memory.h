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

    module:	kernel_memory.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Geng Yang

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
    	10/17/01	ported to linux by Geng Yang

**********************************************************************/


#ifndef  _KERNEL_MEMORY_
#define  _KERNEL_MEMORY_


#include "kernel_debug.h"


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  _ansc_memmove      memmove
extern   BOOLEAN    bAlEngaged;
__static_inline  PVOID
KernelAllocateMemory(ULONG  ulMemorySize)
{
    PVOID p =  NULL;
	p =  ( bAlEngaged || in_interrupt()) ? kmalloc(ulMemorySize, GFP_ATOMIC):kmalloc(ulMemorySize, GFP_KERNEL);

#ifdef _DEBUG
    if (! p)
    {
        printk("NULL from kmalloc %d bytes\n", ulMemorySize);
        //BUG();
		return p;
    }
#endif

    memset(p, 0, ulMemorySize);
    return p;
}

__static_inline  VOID
KernelFreeMemory(PVOID  pMemoryBlock)
{
    kfree(pMemoryBlock);
	pMemoryBlock = NULL;
}

__static_inline  VOID
KernelCopyMemory(PVOID  pDestination, PVOID  pSource, ULONG  ulMemorySize)
{
    memcpy(pDestination, pSource, ulMemorySize);
}

__static_inline  VOID
KernelZeroMemory(PVOID  pMemory, ULONG  ulMemorySize)
{
    memset(pMemory, 0, ulMemorySize);
}

__static_inline  BOOLEAN
KernelEqualMemory(PVOID  pMemory1, PVOID  pMemory2, ULONG  ulMemorySize)
{
    return  (memcmp(pMemory1, pMemory2, ulMemorySize) == 0);
}

__static_inline PVOID
KernelResizeMemory(PVOID  pMemory, ULONG  ulMemorySize)
{
	kfree(pMemory);
	pMemory = KernelAllocateMemory(ulMemorySize);
	return pMemory;
}

#define KernelGetMemorySize(p)           0
#define KernelReAllocMemory(p,s)         0
#define KernelGetFreeMemory()            0xFFFFFFFF

/***********************************************************
               MEMORY POOL BASED OPERATIONS
***********************************************************/

__static_inline PKERNEL_MEMORY_CACHE
KernelMemoryCacheInit
    (
        char*                       pName,
        ULONG                       ulBlockSize,
        ULONG                       ulFlags
    )
{
    PKERNEL_MEMORY_CACHE            pMemPool;

    pMemPool =
        kmem_cache_create
            (
                pName,
                ulBlockSize,
                0,
                SLAB_HWCACHE_ALIGN,
                NULL,
                NULL
            );

    if ( pMemPool )
    {
        return  pMemPool;
    }
    else
    {
        KernelTrace("KernelMemoryCacheInit -- failed to create memory cache %s!!!\n", pName);

        return  NULL;
    }
}

__static_inline PVOID
KernelMemoryCacheAlloc
    (
        PKERNEL_MEMORY_CACHE        pMemPool,
        ULONG                       ulFlags
    )
{
    return  kmem_cache_alloc(pMemPool, GFP_ATOMIC);
}


__static_inline  void
KernelMemoryCacheFree
    (
        PKERNEL_MEMORY_CACHE        pMemPool,
        PVOID                       pMem
    )
{
    kmem_cache_free(pMemPool, pMem);
}


__static_inline  void
KernelMemoryCacheUninit
    (
        PKERNEL_MEMORY_CACHE        pMemPool
    )
{
    kmem_cache_destroy(pMemPool);
}


#endif

