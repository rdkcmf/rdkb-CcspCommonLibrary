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
#include "ccsp_memory.h"

/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/
/*#define  AnscMemoryCacheInit                    UserMemoryCacheInit
#define  AnscMemoryCacheAlloc                   UserMemoryCacheAlloc
#define  AnscMemoryCacheFree                    UserMemoryCacheFree
#define  AnscMemoryCacheUninit                  UserMemoryCacheUninit

#define  ANSC_MEMORY_TRACE_byMarker                 0x00000001
#define  ANSC_MEMORY_TRACE_byOwner                  0x00000002
*/

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
    char                            ComponentID;
}
ANSC_MEMORY_ALLOC_ITEM,  *PANSC_MEMORY_ALLOC_ITEM;

#define  ACCESS_ANSC_MEMORY_ALLOC_ITEM(p)           \
         ACCESS_CONTAINER(p, ANSC_MEMORY_ALLOC_ITEM, Linkage)

#define  ANSC_COMPONENT_NAME_LENGTH    256 

typedef struct
_ANSC_MEMORY_COMPONENT_ALLOC_SIZE
{
    SINGLE_LINK_ENTRY               Linkage;
    char                            ComponentName[ANSC_COMPONENT_NAME_LENGTH];
    ULONG                           MemorySize; 
    ULONG                           MaxMemorySize;
    char                            ComponentID;
}
ANSC_MEMORY_COMPONENT_ITEM, *PANSC_MEMORY_COMPONENT_ITEM;

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
