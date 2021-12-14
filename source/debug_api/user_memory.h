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

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang
        Ying Lou

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to linux kernel by Geng Yang
        04/03/02    adapted to linux user by Ying Loug

**********************************************************************/


#ifndef  _USER_MEMORY_
#define  _USER_MEMORY_

#include "user_debug.h"

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  UserReAllocMemory  
#define  UserGetMemorySize                          sizeof
#define  UserGetFreeMemory()                        0xFFFFFFFF

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

PVOID AnscAllocateMemory(ULONG  ulMemorySize);

VOID AnscFreeMemory(PVOID  pMemoryBlock);

PVOID
UserAllocateMemory(ULONG  ulMemorySize);

VOID
UserFreeMemory(PVOID  pMemoryBlock);

VOID
UserCopyMemory(PVOID  pDestination, PVOID  pSource, ULONG  ulMemorySize);

VOID
UserZeroMemory(PVOID  pMemory, ULONG  ulMemorySize);

BOOLEAN
UserEqualMemory(PVOID  pMemory1, PVOID  pMemory2, ULONG  ulMemorySize);


/***********************************************************
               MEMORY POOL BASED OPERATIONS
***********************************************************/

__static_inline PUSER_MEMORY_CACHE
UserMemoryCacheInit
    (
        char*                       pName,
        ULONG                       ulBlockSize,
        ULONG                       ulFlags
    )
{
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(ulFlags);	
    PUSER_MEMORY_CACHE              pMemPool;

    pMemPool = (USER_MEMORY_CACHE*)AnscAllocateMemory(sizeof(USER_MEMORY_CACHE));

    if ( pMemPool )
    {
        pMemPool->ulBlockSize   = ulBlockSize;

        return  pMemPool;
    }
    else
    {
        return  NULL;
    }
}

__static_inline PVOID
UserMemoryCacheAlloc
    (
        PUSER_MEMORY_CACHE          pMemPool,
        ULONG                       ulFlags
    )
{
    UNREFERENCED_PARAMETER(ulFlags);
    return  (VOID*)AnscAllocateMemory(pMemPool->ulBlockSize);
}


__static_inline void
UserMemoryCacheFree
    (
        PUSER_MEMORY_CACHE          pMemPool,
        PVOID                       pMem
    )
{
    UNREFERENCED_PARAMETER(pMemPool);
    AnscFreeMemory(pMem);
}


__static_inline void
UserMemoryCacheUninit
    (
        PUSER_MEMORY_CACHE          pMemPool
    )
{
    AnscFreeMemory(pMemPool);
}


#endif

