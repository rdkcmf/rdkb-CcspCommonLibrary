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

    module:	ccsp_memory.h

        For CCSP memory related APIs

    ---------------------------------------------------------------

    description:

        This API header file defines all the CCSP memory related
        functions. 

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Hui Ma

    ---------------------------------------------------------------

    revision:

        04/25/11    initial revision.

**********************************************************************/


#ifndef  _CCSP_MEMORY_
#define  _CCSP_MEMORY_

#ifdef  __cplusplus
extern "C" {
#endif

#include "user_base.h"

extern ULONG           g_ulAllocatedSizePeak;

/* each component need to reset this variable to itself name as soon as early possibly .*/
extern    char *                  pComponentName;


/*#define  ANSC_MEMORY_USE_COUNTSIZE*/
#define ANSC_MEMORY_USE_NATIVE
#if defined ANSC_MEMORY_USE_NATIVE

#define AnscAllocateMemory(size) \
    calloc(1,(size))

#define AnscAllocateMemoryNoInit(size) \
    malloc((size))

#define AnscReAllocateMemory(pMemory, size)   \
    realloc((pMemory), (size))

#define AnscFreeMemory(pMemory) \
    AnscFreeMemoryOrig(pMemory)

#elif defined ANSC_MEMORY_USE_COUNTSIZE

#define AnscAllocateMemory(size) \
            AnscAllocateMemoryCountSize(pComponentName, size)
            
#define AnscReAllocateMemory(pMemory, size)   \
            AnscReAllocateMemoryCountSize(pComponentName, pMemory, size)

#define AnscFreeMemory(pMemory) \
            AnscFreeMemoryCountSize(pComponentName, pMemory)

#else 

#define AnscAllocateMemory(size) \
            AnscAllocateMemoryRecordDetail(pComponentName, size, __FILE__, __LINE__ )

#define AnscReAllocateMemory(pMemory, size)   \
            AnscReAllocateMemoryRecordDetail(pComponentName, pMemory, size, __FILE__, __LINE__ )

#define AnscFreeMemory(pMemory) \
            AnscFreeMemoryRecordDetail(pComponentName, pMemory )

#endif

#define AnscZeroMemory(pMemory, size) \
            (void) memset((pMemory), 0, (size))

VOID
AnscFreeMemoryOrig
(
     PVOID  pMemoryBlock
);

#define AnscCopyMemory(pDestination, pSource, size) \
            (void) memcpy((pDestination), (pSource), (size))

PVOID
AnscReallocMemory
    (
        PVOID pMemory,
        ULONG ulOldMemorySize,
        ULONG ulNewMemorySize
    );

VOID
AnscFreeMemoryOrig
    (
        PVOID  pMemoryBlock
    );
/**********************************************************************
    Only count size, no detail recording.
**********************************************************************/

PVOID AnscAllocateMemoryCountSize
(
    char*  pComponentName,
    ULONG  ulMemorySize
);

PVOID AnscReAllocateMemoryCountSize
(   
    char*  pComponentName,
    PVOID  pMemoryBlock,
    ULONG  ulMemorySize
);

VOID
AnscFreeMemoryCountSize
(
    char*  pComponentName,
    PVOID  pMemoryBlock
);

LONG AnscGetComponentMemorySize                                                                 
(               
    char*  pComponentName
);    

LONG AnscGetComponentMaxMemorySize                                                                 
(               
    char*  pComponentName
);    

ULONG AnscGetProcessMaxMemorySize                                                                 
(               
);    

static inline int                                                     
AnscEqualMemory(PVOID p1, PVOID p2, ULONG ulSize)                                   
{                                                                                   
    return  (memcmp(p1, p2, ulSize) == 0);
}


/**********************************************************************
    Count size and record detail.
**********************************************************************/

PVOID AnscAllocateMemoryRecordDetail
(
    char*  pComponentName,
    ULONG  ulMemorySize,
    char*  pFileName,
    ULONG  LineNumber
);

PVOID AnscReAllocateMemoryRecordDetail
(   
    char*  pComponentName,
    PVOID  pMemoryBlock,
    ULONG  ulMemorySize,
    char*  pFileName,
    ULONG  LineNumber
);

VOID
AnscFreeMemoryRecordDetail
(
    char*  pComponentName,
    PVOID  pMemoryBlock
);

void AnscPrintComponentMemoryTable
    (
        char* pComponentName
    );

void AnscPrintFileLineMemoryTable
(
    char*  pFileName,
    ULONG  LineNumber
);

/**********************************************************************
    Used for D_bus memory callback
**********************************************************************/

void*
Ansc_AllocateMemory_Callback
    (
        ULONG                       ulMemorySize
    );

void
Ansc_FreeMemory_Callback
    (
        void*                       pMemory
    );

#ifdef  __cplusplus
}
#endif
#endif
