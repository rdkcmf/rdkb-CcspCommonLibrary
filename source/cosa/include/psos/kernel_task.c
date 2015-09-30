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

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

    ---------------------------------------------------------------

    environment:

        pSOS 2.5

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        10/06/01    separated from the ansc platform itself, one
                    for each specific platform

		10/15/01	pSOS port

**********************************************************************/


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"


static  ULONG                       taskNo  = 0;

KERNEL_HANDLE
KernelCreateTask
    (
        KERNEL_TASK_START_ENTRY     startEntry,
        ULONG                       stackSize,
        ULONG                       priority,
        KERNEL_HANDLE               hContext,
        char*                       taskName
    )
{
    ULONG                           returnStatus;
    ULONG                           taskId;
    ULONG                           taskArgs[4];

    /* pSOS doesn't check if the task name is duplicate */
    sprintf(&taskName[1], "%03d", taskNo);

    KernelTrace("KernelCreateTask -- %s.\n", taskName);

    returnStatus = t_create(taskName, priority, stackSize, 0, 0x00, &taskId);
    
    if (returnStatus != 0)
    {
        KernelTrace("KernelCreateTask -- cannot create task, error code = 0x%X!\n", returnStatus);
        
        return  (KERNEL_HANDLE)NULL;
    }

    /* then start it */
    taskArgs[0]  = (ULONG)hContext;
    returnStatus = t_start(taskId, T_SUPV | T_PREEMPT | T_TSLICE | T_ISR, startEntry, taskArgs);
    
    if (returnStatus != 0)
    {
        KernelTrace("KernelCreateTask -- cannot start task, error code = 0x%X!\n", returnStatus);
        
        returnStatus = t_delete(taskId);
        
        return  (KERNEL_HANDLE)NULL;
    }

    taskNo++;

    return  (KERNEL_HANDLE)taskId;
}
