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

        OSE 4.2

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        07/31/02    initial revision.

**********************************************************************/


#ifndef  _KERNEL_MEMORY_
#define  _KERNEL_MEMORY_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

PVOID
KernelAllocateMemory
    (
        ULONG                       ulMemorySize
    );

/*
 *  prototype
 *
PVOID
KernelReAllocMemory
    (
        PVOID                       pMemoryBlock,
        ULONG                       ulMemorySize
    );
 */
#define  KernelReAllocMemory(pMem, ulSize)          realloc(pMem, ulSize)

/*
 *  prototype
 *
VOID
KernelFreeMemory
    (
        PVOID                       pMemoryBlock
    );
 */
#define  KernelFreeMemory(pMem)                     free(pMem)

/*
 *  if AnscAllocateMemory2/AnscFreeMemory2 are enabled, we do need to
 *  implement the mechanism to track memory usage
 *
ULONG
KernelGetMemorySize
    (
        PVOID                       pMemoryBlock
    );
 */
#define  KernelGetMemorySize(pMemoryBlock)          0

/*
 *  prototype
 *
VOID
KernelCopyMemory
    (
        PVOID                       pDestination,
        PVOID                       pSource,
        ULONG                       ulMemorySize
    );
 */
#define  KernelCopyMemory(pDest, pSrc, ulSize)      memcpy(pDest, pSrc, ulSize)

/*
 *  prototype
 *
VOID
KernelZeroMemory
    (
        PVOID                       pMemory,
        ULONG                       ulMemorySize
    );
 */
#define  KernelZeroMemory(pMem, ulSize)             memset(pMem, 0, ulSize)

/*
 *  prototype
 *
BOOLEAN
KernelEqualMemory
    (
        PVOID                       pMemory1,
        PVOID                       pMemory2,
        ULONG                       ulMemorySize
    );
 */
#define  KernelEqualMemory(pMem1, pMem2, ulSize)    (memcmp(pMem1, pMem2, ulSize) == 0)


#endif
