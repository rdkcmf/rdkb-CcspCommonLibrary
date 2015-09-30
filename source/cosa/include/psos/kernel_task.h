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

        This wrapper file defines some basic data types and
        structures on a particular platform.

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


#ifndef  _KERNEL_TASK_
#define  _KERNEL_TASK_

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

typedef  VOID
(*KERNEL_TASK_START_ENTRY)
    (
        void*                       context
    );

/*
 *  this is not supported yet.
 */
#define  KernelGetCurTaskHandle()                   0L

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
#define  KernelDeleteTask(h)

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
#define  KernelLeaveTask()                          t_suspend(0L); t_delete(0L)


#define  KernelTaskRoutine                          AnscTaskRoutine1
#define  KernelPoolableTaskRoutine                  AnscTaskRoutine2
#define  KernelJanitorTaskRoutine                   AnscTaskJanitorRoutine

#define  KERNEL_DEFAULT_TASK_STACK_SIZE             8192
#define  KERNEL_DEFAULT_TASK_PRIORITY               211

#define  KERNEL_USE_TASK_POOL_MANAGER               FALSE
#define  KERNEL_USE_TASK_POOL_JANITOR               FALSE
#define  KERNEL_TASK_POOL_TYPE                      ANSC_TASK_POOL_TYPE_DYNAMIC
#define  KERNEL_MAX_TASK_POOL_SIZE                  32

#define  KERNEL_TASK_PRIORITY_HIGHEST               225
#define  KERNEL_TASK_PRIORITY_HIGH                  213
#define  KERNEL_TASK_PRIORITY_NORMAL                211
#define  KERNEL_TASK_PRIORITY_LOW                   200
#define  KERNEL_TASK_PRIORITY_LOWEST                255

#define  KERNEL_DEFAULT_TASK_PRIORITY               KERNEL_TASK_PRIORITY_NORMAL


#endif
