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

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/


#ifndef  _KERNEL_TASK_
#define  _KERNEL_TASK_


/**********************************************************************
            DEFINITION OF BASIC DATA TYPE AND STRUCTURES
**********************************************************************/

typedef  VOID
(*KERNEL_TASK_START_ENTRY)
    (
        void*                       context
    );

/*
KERNEL_HANDLE
KernelGetCurTaskHandle
    (
        void
    );
 */
#define  KernelGetCurTaskHandle()                   ((KERNEL_HANDLE)atmos_pcb_get_current())
#define  KernelGetTaskNameByHandle(hTask)           (atmos_pcb_get_name((ATMOS_PCBID)(hTask)))

KERNEL_HANDLE
KernelCreateTask2
    (
        KERNEL_TASK_START_ENTRY     startEntry,
        ULONG                       stackSize,
        ULONG                       priority,
        KERNEL_HANDLE               hContext,
        char*                       pName
    );

/*
#define  KernelCreateTask(startEntry, stackSize, priority, hContext)    \
            KernelCreateTask2(NULL, startEntry, stackSize, priority, hContext)
 */
#define  KernelCreateTask                           KernelCreateTask2

VOID
KernelDeleteTask
    (
        KERNEL_HANDLE               hTask
    );

/*
VOID
KernelEnterTask
    (
        PANSC_TASK_RECORD           pTaskRecord
    );
 */
#define  KernelEnterTask(pTaskRecord)

/*
VOID
KernelLeaveTask
    (
        VOID
    );
 */
#define  KernelLeaveTask()

VOID
KernelKillTask
    (
        KERNEL_HANDLE               hTask
    );


#define  KernelTaskRoutine                          AnscTaskRoutine1
#define  KernelPoolableTaskRoutine                  AnscTaskRoutine2
#define  KernelJanitorTaskRoutine                   AnscTaskJanitorRoutine

#define  KERNEL_DEFAULT_TASK_STACK_SIZE             32768
#define  KERNEL_DEFAULT_TASK_PRIORITY               KERNEL_TASK_PRIORITY_NORMAL

#define  KERNEL_USE_TASK_POOL_MANAGER               FALSE
#define  KERNEL_USE_TASK_POOL_JANITOR               TRUE

#define  KERNEL_TASK_POOL_TYPE                      ANSC_TASK_POOL_TYPE_DYNAMIC
#define  KERNEL_MAX_TASK_POOL_SIZE                  32

/*
 *  ISOS 8.2
 *
#define  KERNEL_TASK_PRIORITY_HIGHEST               ATMOS_PRILWB
#define  KERNEL_TASK_PRIORITY_HIGH                  ATMOS_PRILWB
#define  KERNEL_TASK_PRIORITY_NORMAL                ATMOS_PRILWB
#define  KERNEL_TASK_PRIORITY_LOW                   ATMOS_PRIUPB
#define  KERNEL_TASK_PRIORITY_LOWEST                ATMOS_PRIUPB
 */

/*
 *  ISOS 9.0
 */
#define  KERNEL_TASK_PRIORITY_HIGHEST               HS_PRI_DEFAULT + 1
#define  KERNEL_TASK_PRIORITY_HIGH                  HS_PRI_DEFAULT + 1
#define  KERNEL_TASK_PRIORITY_NORMAL                HS_PRI_DEFAULT
#define  KERNEL_TASK_PRIORITY_LOW                   HS_PRI_DEFAULT - 1
#define  KERNEL_TASK_PRIORITY_LOWEST                HS_RUN_LEVEL_LOWEST

#define  KERNEL_DEFAULT_TASK_PRIORITY               KERNEL_TASK_PRIORITY_NORMAL


/**********************************************************************
                             CONSTANTS
**********************************************************************/

#define  KERNEL_TASK_NAME_LENGTH                    128


#endif
