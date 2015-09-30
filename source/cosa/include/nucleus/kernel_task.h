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
#define  NU_TIME_SLICE                              5

#define  KernelGetCurTaskHandle()                   NU_Current_Task_Pointer()

VOID
KernelTaskRoutine
	(
		ULONG		arc,
		PVOID		argv
	);
typedef
int
task_func
    (
	    void * handle
    );


#if 0
__inline  void *
KernelCreateTask
	(
		/*(int (*)(void *))	func,*/
		task_func			func,
		ULONG				stackSize,
		int					priority,
		PVOID				arg
	)
{
	PVOID		pointer		= NULL;
	STATUS		status		= NU_SUCCESS;
	NU_TASK *	pTask		= NULL;

	char 		name [NU_MAX_NAME_SIZE];

	pTask	= (NU_TASK *)OS_ADPT_Allocate_Memory(ghBisga_Memory, sizeof(NU_TASK));
	
	pointer	= OS_ADPT_Allocate_Memory(ghBisga_Memory, stackSize);

	status = 
		NU_Create_Task
			(
				pTask,
				name,
				func,
				1,
				arg,
				pointer,
				stackSize,
				priority,
				NU_TIME_SLICE,
				NU_PREEMPT,
				NU_START
			);

	if ( status != NU_SUCCESS )
	{
		OS_ADPT_Deallocate_Memory((PVOID)pTask);
		OS_ADPT_Deallocate_Memory(pointer);
		return  NULL;
	}
	else
	{
		return  pTask;
	}
}

#endif

#define  KernelDeleteTask(h)						NU_Delete_Task((NU_TASK *)(h))
#define  KernelEnterTask()
#define  KernelLeaveTask()
#define  KernelKillTask(h)							NU_Terminate_Task((NU_TASK *)(h))


#define  KernelPoolableTaskRoutine                  AnscTaskRoutine2
#define  KernelJanitorTaskRoutine                   AnscTaskJanitorRoutine
#define  KERNEL_DEFAULT_TASK_STACK_SIZE             32*1024
#define  KERNEL_DEFAULT_TASK_PRIORITY               60
#define  KERNEL_USE_TASK_POOL_MANAGER               FALSE
#define  KERNEL_USE_TASK_POOL_JANITOR               FALSE
#define  KERNEL_TASK_POOL_TYPE                      ANSC_TASK_POOL_TYPE_DYNAMIC
#define  KERNEL_MAX_TASK_POOL_SIZE                  32

#define  KERNEL_TASK_PRIORITY_HIGHEST               20
#define  KERNEL_TASK_PRIORITY_HIGH                  50
#define  KERNEL_TASK_PRIORITY_NORMAL                60
#define  KERNEL_TASK_PRIORITY_LOW                   100
#define  KERNEL_TASK_PRIORITY_LOWEST                255


#endif







