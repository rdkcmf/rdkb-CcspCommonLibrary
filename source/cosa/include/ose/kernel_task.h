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

        Cisco System  , Inc., 1997 ~ 2002
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


#ifndef  _KERNEL_TASK_
#define  _KERNEL_TASK_

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 *  Since OSE process doesn't provide the mechanism to pass in the context,
 *  we have to achieve this by start the process with our own stub routine.
 *
 *  Therefore, KernelDeleteTask/KernelLeaveTask are emptied to ensure the
 *  actual AnscTaskRoutine returns the control to our stub routine, which
 *  will do all the clean-up. KernelKillTask is an exception since it kills
 *  the task brutally.
 */

typedef  VOID
(*KERNEL_TASK_START_ENTRY)
    (
        void*                       context
    );

/*
 *  prototype -- return type is ULONG, not KERNEL_HANDLE to accomodate ansc_task
 *
ULONG
KernelGetCurTaskHandle
    (
        void
    );
 */
#define  KernelGetCurTaskHandle()                   ((ULONG)current_process())

KERNEL_HANDLE
KernelCreateTask
    (
        KERNEL_TASK_START_ENTRY     startEntry,
        ULONG                       stackSize,
        ULONG                       priority,
        KERNEL_HANDLE               hContext,
        char*                       taskName
    );

/*
 *  prototype
 *
VOID
KernelDeleteTask
    (
        KERNEL_HANDLE               hTask
    );
 */
#define  KernelDeleteTask(hTask)

/*
 *  prototype
 *
VOID
KernelEnterTask
    (
        VOID
    );
 */
#define  KernelEnterTask()

/*
 *  prototype
 *
VOID
KernelLeaveTask
    (
        VOID
    );
 */
#define  KernelLeaveTask()

/*
 *  prototype
 *
VOID
KernelKillTask
    (
        KERNEL_HANDLE               hTask
    );
 */
#define  KernelKillTask(hTask)                      kill_proc((PROCESS)hTask)


#define  KernelTaskRoutine                          AnscTaskRoutine1
#define  KernelPoolableTaskRoutine                  AnscTaskRoutine2
#define  KernelJanitorTaskRoutine                   AnscTaskJanitorRoutine

#define  KERNEL_DEFAULT_TASK_STACK_SIZE             16384
#define  KERNEL_DEFAULT_TASK_PRIORITY               KERNEL_TASK_PRIORITY_NORMAL

#define  KERNEL_USE_TASK_POOL_MANAGER               FALSE
#define  KERNEL_USE_TASK_POOL_JANITOR               FALSE

#define  KERNEL_TASK_POOL_TYPE                      ANSC_TASK_POOL_TYPE_DYNAMIC
#define  KERNEL_MAX_TASK_POOL_SIZE                  32

#define  KERNEL_TASK_PRIORITY_HIGHEST               OS_PRI_PROC + 16
#define  KERNEL_TASK_PRIORITY_HIGH                  OS_PRI_PROC + 16
#define  KERNEL_TASK_PRIORITY_NORMAL                OS_PRI_PROC + 31
#define  KERNEL_TASK_PRIORITY_LOW                   OS_BG_PROC
#define  KERNEL_TASK_PRIORITY_LOWEST                OS_BG_PROC

#define  KERNEL_DEFAULT_TASK_PRIORITY               KERNEL_TASK_PRIORITY_NORMAL


#endif
