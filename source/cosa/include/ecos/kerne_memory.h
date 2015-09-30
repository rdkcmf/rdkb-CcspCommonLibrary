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

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

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


#ifndef  _KERNEL_MEMORY_
#define  _KERNEL_MEMORY_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/
/*#define  _KERNEL_USE_MEMORY_TRACKING_*/

#define  _KERNEL_MEMORY_TRACKING_METHOD_1_

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
 *  prototype
 *
PVOID
RealKernelAllocateMemory
    (
        ULONG                       ulSize
    );
 */
#define  RealKernelAllocateMemory(ulSize)             malloc(ulSize)

/*
 *  prototype
 *
VOID
RealKernelFreeMemory
    (
        PVOID                       pMem
    );
 */
#define  RealKernelFreeMemory(pMem)                   free(pMem)


/*
 *  prototype
 *
PVOID
RealKernelReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );
 */
#define  RealKernelReAllocMemory(pMem, ulSize)        realloc(pMem, ulSize)


/*
 *  not supported
 *
 *  if AnscAllocateMemory2/AnscFreeMemory2 are enabled, we do need to
 *  implement the mechanism to track memory usage
 *
 *  prototype
 *
ULONG
RealKernelGetMemorySize
    (
        PVOID                       pMem
    );
 */
#define  RealKernelGetMemorySize(pMem)                0


#ifdef   _KERNEL_USE_MEMORY_TRACKING_

    #if     defined(_KERNEL_MEMORY_TRACKING_METHOD_1_)

        PVOID
        __KernelAllocateMemory
            (
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        VOID
        __KernelFreeMemory
            (
                PVOID                       pMem,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        PVOID
        __KernelReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  KernelAllocateMemory(ulSize)             __KernelAllocateMemory(ulSize, __FILE__, __LINE__)
        #define  KernelFreeMemory(pMem)                   __KernelFreeMemory(pMem, __FILE__, __LINE__)
        #define  KernelReAllocMemory(pMem, ulSize)        __KernelReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        KernelGetMemorySize
            (
                PVOID                       pMem
            );

    #elif   defined(_KERNEL_MEMORY_TRACKING_METHOD_2_)

        PVOID
        KernelAllocateMemory
            (
                ULONG                       ulSize
            );

        VOID
        KernelFreeMemory
            (
                PVOID                       pMem
            );

        PVOID
        __KernelReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  KernelReAllocMemory(pMem, ulSize)        __KernelReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        KernelGetMemorySize
            (
                PVOID                       pMem
            );

    #elif   defined(_KERNEL_MEMORY_TRACKING_METHOD_3_)

        PVOID
        __KernelAllocateMemory
            (
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        VOID
        __KernelFreeMemory
            (
                PVOID                       pMem,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        PVOID
        __KernelReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  KernelAllocateMemory(ulSize)             __KernelAllocateMemory(ulSize, __FILE__, __LINE__)
        #define  KernelFreeMemory(pMem)                   __KernelFreeMemory(pMem, __FILE__, __LINE__)
        #define  KernelReAllocMemory(pMem, ulSize)        __KernelReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        KernelGetMemorySize
            (
                PVOID                       pMem
            );

    #endif

#else

    PVOID
    KernelAllocateMemory
        (
            ULONG                       ulSize
        );

    #define  KernelFreeMemory                         RealKernelFreeMemory
    #define  KernelReAllocMemory                      RealKernelReAllocMemory
    #define  KernelGetMemorySize                      RealKernelGetMemorySize

#endif

PVOID
KernelResizeMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );


/*
 *  prototype
 *
VOID
KernelCopyMemory
    (
        PVOID                       pDest,
        PVOID                       pSrc,
        ULONG                       ulSize
    );
 */
#define  KernelCopyMemory(pDest, pSrc, ulSize)        memcpy(pDest, pSrc, ulSize)


/*
 *  prototype
 *
VOID
KernelZeroMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );
 */
#define  KernelZeroMemory(pMem, ulSize)               memset(pMem, 0, ulSize)


/*
 *  prototype
 *
BOOLEAN
KernelEqualMemory
    (
        PVOID                       pMem1,
        PVOID                       pMem2,
        ULONG                       ulSize
    );
 */
#define  KernelEqualMemory(pMem1, pMem2, ulSize)      (memcmp(pMem1, pMem2, ulSize) == 0)

#define  KernelGetFreeMemory()                        0xFFFFFFFF

#endif
