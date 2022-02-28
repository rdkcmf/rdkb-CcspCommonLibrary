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

#include "ansc_global.h"
#include "user_base.h"
#include "user_memory.h"


PVOID
UserAllocateMemory(ULONG  ulMemorySize)
{
    PVOID p =  AnscAllocateMemory(ulMemorySize);
    if ( !p )
    {
        return  p;
    }
/*    printf("Alloc: %x\n", p); */
    return p;
}

VOID
UserFreeMemory(PVOID  pMemoryBlock)
{
/*    printf("Free: %x\n", pMemoryBlock); */
    AnscFreeMemory(pMemoryBlock);
	pMemoryBlock = NULL;
    return;
}

VOID
UserCopyMemory(PVOID  pDestination, PVOID  pSource, ULONG  ulMemorySize)
{
    memcpy(pDestination, pSource, ulMemorySize);
    return;
}

VOID
UserZeroMemory(PVOID  pMemory, ULONG  ulMemorySize)
{
    memset(pMemory, 0, ulMemorySize);
    return;
}

BOOLEAN
UserEqualMemory(PVOID  pMemory1, PVOID  pMemory2, ULONG  ulMemorySize)
{
    return  (memcmp(pMemory1, pMemory2, ulMemorySize) == 0);
}
