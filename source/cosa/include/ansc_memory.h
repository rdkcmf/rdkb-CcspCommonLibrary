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

    module:	ansc_memory.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to memory operation. Instead
        of defining everything in a single file, we simply route
        every function and macro to the corresponding platform-
        dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions

**********************************************************************/


#ifndef  _ANSC_MEMORY_
#define  _ANSC_MEMORY_


/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * according to the current platform definition, we route the definition to the corresponding header files
 */

    #define  AnscGetFreeMemory                      UserGetFreeMemory

    #if defined(_ANSC_ENABLE_PROFILING_)

        _ANSC_STATIC_ _ANSC_INLINE_ PVOID                                                   \
        AnscAllocMemory(ULONG   size)                                                       \
        {                                                                                   \
            PVOID                   pMem;                                                   \
            AnscProfilingStartCounter(ANSC_PROFILING_COUNTER_anscAllocMemory);              \
            pMem = UserAllocateMemory(size);                                                \
            AnscProfilingStopCounter (ANSC_PROFILING_COUNTER_anscAllocMemory);              \
            return  pMem;                                                                   \
        }

        #define  AnscFlushMemory(p)                                                         \
                    {                                                                       \
                        AnscProfilingStartCounter(ANSC_PROFILING_COUNTER_anscFlushMemory);  \
                        UserFreeMemory(p);                                                  \
                        AnscProfilingStopCounter (ANSC_PROFILING_COUNTER_anscFlushMemory);  \
                    }

        #define  AnscCopyMemory(pDst, pSrc, size)                                           \
                    {                                                                       \
                        AnscProfilingStartCounter(ANSC_PROFILING_COUNTER_anscCopyMemory);   \
                        UserCopyMemory(pDst, pSrc, size);                                   \
                        AnscProfilingStopCounter (ANSC_PROFILING_COUNTER_anscCopyMemory);   \
                    }

        #define  AnscZeroMemory(p, size)                                                    \
                    {                                                                       \
                        AnscProfilingStartCounter(ANSC_PROFILING_COUNTER_anscZeroMemory);   \
                        UserZeroMemory(p, size);                                            \
                        AnscProfilingStopCounter (ANSC_PROFILING_COUNTER_anscZeroMemory);   \
                    }

        _ANSC_STATIC_ _ANSC_INLINE_ int                                                     \
        AnscEqualMemory(PVOID p1, PVOID p2, ULONG ulSize)                                   \
        {                                                                                   \
            int                         iReturnValue;                                       \
            AnscProfilingStartCounter(ANSC_PROFILING_COUNTER_anscEqualMemory);              \
            iReturnValue = UserEqualMemory(p1, p2, ulSize);                                 \
            AnscProfilingStopCounter (ANSC_PROFILING_COUNTER_anscEqualMemory);              \
            return  iReturnValue;    
        }
    #else
        #define  AnscAllocMemory                        UserAllocateMemory
        #define  AnscFlushMemory                        UserFreeMemory

        #define  AnscCopyMemory                         UserCopyMemory
        #define  AnscZeroMemory                         UserZeroMemory
        #define  AnscEqualMemory                        UserEqualMemory
    #endif

    #ifdef  _ANSC_MALLOC_METHOD1

        #define  AnscReAllocMemory                  UserReAllocMemory
        #define  AnscAllocateMemory                 AnscAllocMemory
        #define  AnscFreeMemory                     AnscFlushMemory
        #define  AnscGetMemorySize                  UserGetMemorySize

    #endif

    #ifdef  _ANSC_MALLOC_METHOD2

        #define  AnscReAllocMemory                  AnscReAllocateMemory2
        #define  AnscAllocateMemory                 AnscAllocateMemory2
        #define  AnscFreeMemory                     AnscFreeMemory2
        #define  AnscGetMemorySize                  AnscGetMemorySize2

    #endif

    #ifdef  _ANSC_MALLOC_METHOD3

        #define  AnscReAllocMemory                  AnscReAllocateMemory3
        #define  AnscAllocateMemory                 AnscAllocateMemory3
        #define  AnscFreeMemory                     AnscFreeMemory3
        #define  AnscGetMemorySize                  AnscGetMemorySize3

    #endif

    #ifdef  _ANSC_MALLOC_METHOD4

        #define  AnscReAllocMemory(p, s)            AnscReAllocateMemory4(p, s, __FILE__, __LINE__)
        #define  AnscAllocateMemory(s)              AnscAllocateMemory4(s, __FILE__, __LINE__)
        #define  AnscFreeMemory                     AnscFreeMemory4
        #define  AnscGetMemorySize                  AnscGetMemorySize4

    #endif

    #ifdef  _ANSC_MALLOC_METHOD5

        #define  AnscReAllocMemory(p, s)            AnscReAllocateMemory5(p, s, __FILE__, __LINE__)
        #define  AnscAllocateMemory(s)              AnscAllocateMemory5(s, __FILE__, __LINE__)
        #define  AnscFreeMemory                     AnscFreeMemory5
        #define  AnscGetMemorySize                  AnscGetMemorySize5

    #endif

    #ifdef  _ANSC_MALLOC_METHOD6

        #define  AnscReAllocMemory                  AnscReAllocateMemory6
        #define  AnscAllocateMemory                 AnscAllocateMemory6
        #define  AnscFreeMemory                     AnscFreeMemory6
        #define  AnscGetMemorySize                  AnscGetMemorySize6

    #endif

    #define  AnscMemoryCacheInit                    UserMemoryCacheInit
    #define  AnscMemoryCacheAlloc                   UserMemoryCacheAlloc
    #define  AnscMemoryCacheFree                    UserMemoryCacheFree
    #define  AnscMemoryCacheUninit                  UserMemoryCacheUninit


#define  ANSC_MEMORY_TRACE_byMarker                 0x00000001
#define  ANSC_MEMORY_TRACE_byOwner                  0x00000002


/*
 * Memory leak has always been the most common fight every software developer engages in on daily
 * basis. To detect memory leak and track the origination, we use a hash table to maintain the
 * history of memory allocation.
 */
#define  ANSC_MEMORY_ALLOC_TABLE_SIZE               2048

typedef  struct
_ANSC_MEMORY_ALLOC_ITEM
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ULONG                           AllocId;
    PVOID                           MemoryPointer;
    ULONG                           MemorySize;
    ULONG                           Timestamp;
    char                            OwnerDesp[128];
}
ANSC_MEMORY_ALLOC_ITEM,  *PANSC_MEMORY_ALLOC_ITEM;

#define  ACCESS_ANSC_MEMORY_ALLOC_ITEM(p)           \
         ACCESS_CONTAINER(p, ANSC_MEMORY_ALLOC_ITEM, Linkage)


/***********************************************************
     COMMON MEMORY POOLING AND MANAGEMENT CONFIGURATION
***********************************************************/

/*
 * Memory-pooling is a popular mechanism used in many areas to improve the software performance by
 * reducing the overhead of allocating/releasing memory. ANSC wrapper offers a generalized memory-
 * pooling implementation which can be utilized by developers to speed up development.
 */
typedef  struct
_ANSC_MEMORY_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           BlockSize;
    PUCHAR                          Data[ANSC_ZERO_ARRAY_SIZE];
}
ANSC_MEMORY_DESCRIPTOR,  *PANSC_MEMORY_DESCRIPTOR;

#define  ACCESS_ANSC_MEMORY_DESCRIPTOR(p)           \
         ACCESS_CONTAINER(p, ANSC_MEMORY_DESCRIPTOR, Linkage)
#define  ACCESS_ANSC_MEMORY_DESCRIPTOR_BYDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_MEMORY_DESCRIPTOR, Data)

#define  ANSC_MEMORY_POOL_TABLE_SIZE                16

typedef  struct
_ANSC_MEMORY_POOL
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           BlockSize;
    ULONG                           MinFreeMemorySize;  /* in KBytes */
    SLIST_HEADER                    MdoSList;
}
ANSC_MEMORY_POOL,  *PANSC_MEMORY_POOL;

#define  ACCESS_ANSC_MEMORY_POOL(p)                 \
         ACCESS_CONTAINER(p, ANSC_MEMORY_POOL, Linkage)


#endif
