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

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        10/06/01    separated from the ansc platform itself, one
                    for each specific platform

**********************************************************************/


#ifndef  _KERNEL_TASK_
#define  _KERNEL_TASK_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/* nonsense value in linux kernel */
#define  KERNEL_USE_TASK_POOL_MANAGER               FALSE
#define  KERNEL_USE_TASK_POOL_JANITOR               FALSE
#define  KernelPoolableTaskRoutine                  AnscTaskRoutine2
#define  KernelJanitorTaskRoutine                   AnscTaskJanitorRoutine
#define  KERNEL_DEFAULT_TASK_STACK_SIZE             0
#define  KERNEL_DEFAULT_TASK_PRIORITY               0
#define  KERNEL_TASK_PRIORITY_NORMAL                0
#define  KERNEL_TASK_POOL_TYPE                      ANSC_TASK_POOL_TYPE_DYNAMIC
#define  KERNEL_MAX_TASK_POOL_SIZE                  0

#define  KernelLeaveTask()
#define  KernelDeleteTask(h)
#define  KernelKillTask(h)

#ifdef _ANSC_LINUX_2_6_

#define  KernelEnterTask(pTaskRecord)                                           \
{                                                                               \
    if(pTaskRecord && ((PANSC_TASK_RECORD)pTaskRecord)->Name &&                 \
    strlen(((PANSC_TASK_RECORD)pTaskRecord)->Name))                             \
    {                                                                           \
        daemonize(((PANSC_TASK_RECORD)pTaskRecord)->Name);                      \
    } else {                                                                    \
        daemonize("BISGA");                                                     \
    }                                                                           \
}

#else

#define  KernelEnterTask(pTaskRecord)                                           \
{                                                                               \
    daemonize();                                                                \
    reparent_to_init();                                                         \
                                                                                \
    if ( pTaskRecord && ((PANSC_TASK_RECORD)pTaskRecord)->Name                  \
         && strlen(((PANSC_TASK_RECORD)pTaskRecord)->Name) )                    \
    {                                                                           \
        strncpy(current->comm, ((PANSC_TASK_RECORD)pTaskRecord)->Name, sizeof(current->comm) - 1);   \
        current->comm[sizeof(current->comm) - 1] = '\0';                        \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        strcpy(current->comm, "BISGA");                                         \
    }                                                                           \
}

#endif

#ifdef _ANSC_LINUX_2_6_
#define  KernelGetProcessId()                       in_interrupt()?0xFFFFFFFE:current->parent->pid
#else
#define  KernelGetProcessId()                       in_interrupt()?0xFFFFFFFE:current->p_pptr->pid
#endif

#define  KernelGetCurTaskHandle()                   in_interrupt()?0xFFFFFFFE:(current->pid)
#define  KernelCreateTask(f, s, p, h, n)            kernel_thread((int (*)(void *))f, h, 0)
#define  KernelTaskRoutine                          AnscTaskRoutine1

#endif
