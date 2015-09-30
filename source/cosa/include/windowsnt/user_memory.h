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

    module:	user_memory.h

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

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#ifndef  _USER_MEMORY_
#define  _USER_MEMORY_

#include "user_debug.h"

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  UserGetFreeMemory()                        0xFFFFFFFF

__inline  PVOID
UserAllocateMemory(ULONG  ulMemorySize)
{
    return  HeapAlloc
                (
                    GetProcessHeap(),
                    HEAP_ZERO_MEMORY,
                    ulMemorySize
                );
}

__inline  PVOID
UserReAllocMemory(PVOID  pMemoryBlock, ULONG  ulMemorySize)
{
    return  HeapReAlloc
                (
                    GetProcessHeap(),
                    HEAP_ZERO_MEMORY,
                    pMemoryBlock,
                    ulMemorySize
                );
}

__inline  VOID
UserFreeMemory(PVOID  pMemoryBlock)
{
    HeapFree(GetProcessHeap(), 0, pMemoryBlock);
}

__inline  ULONG
UserGetMemorySize(PVOID  pMemoryBlock)
{
    return  HeapSize(GetProcessHeap(), 0, pMemoryBlock);
}

__inline  PVOID
UserResizeMemory(PVOID  pMemory, ULONG  ulMemorySize)
{
    UserFreeMemory(pMemory);

    return  UserAllocateMemory(ulMemorySize);
}

__inline  VOID
UserCopyMemory(PVOID  pDestination, PVOID  pSource, ULONG  ulMemorySize)
{
    memcpy(pDestination, pSource, ulMemorySize);
}

__inline  VOID
UserZeroMemory(PVOID  pMemory, ULONG  ulMemorySize)
{
    memset(pMemory, 0, ulMemorySize);
}

__inline  BOOLEAN
UserEqualMemory(PVOID  pMemory1, PVOID  pMemory2, ULONG  ulMemorySize)
{
    return  (BOOLEAN)(memcmp(pMemory1, pMemory2, ulMemorySize) == 0);
}


/***********************************************************
               MEMORY POOL BASED OPERATIONS
***********************************************************/

__inline  PUSER_MEMORY_CACHE
UserMemoryCacheInit
    (
        char*                       pName,
        ULONG                       ulBlockSize,
        ULONG                       ulFlags
    )
{
    PUSER_MEMORY_CACHE              pMemPool;

    pMemPool = 
        HeapAlloc
            (
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                sizeof(USER_MEMORY_CACHE)
            );

    if ( pMemPool )
    {
        pMemPool->ulBlockSize   = ulBlockSize;

        return  pMemPool;
    }
    else
    {
        UserTrace("UserMemoryCacheInit -- failed to create memory cache %s!!!\n", pName);

        return  NULL;
    }
}

__inline  PVOID
UserMemoryCacheAlloc
    (
        PUSER_MEMORY_CACHE          pMemPool,
        ULONG                       ulFlags
    )
{
    return  HeapAlloc
                (
                    GetProcessHeap(),
                    HEAP_ZERO_MEMORY,
                    pMemPool->ulBlockSize
                );
}


__inline  void
UserMemoryCacheFree
    (
        PUSER_MEMORY_CACHE          pMemPool,
        PVOID                       pMem
    )
{
    HeapFree(GetProcessHeap(), 0, pMem);
}


__inline  void
UserMemoryCacheUninit
    (
        PUSER_MEMORY_CACHE          pMemPool
    )
{
    HeapFree(GetProcessHeap(), 0, pMemPool);
}


#endif
