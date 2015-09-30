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

        *   messageHandler
        *   KernelTaskRoutineEntry

        *   KernelCreateTask2
        *   KernelDeleteTask
        *   KernelKillTask

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


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"
#include "kernel_runtime.h"

/*
 *  for socket_register_message_handler()
 */
#include "netinet/in.h"
#include "arpa/inet.h"


/**********************************************************************
                     DEFINITIONS FOR THIS FILE
**********************************************************************/

typedef  struct
_KERNEL_TASK_CONTEXT
{
    char                            TaskName[128];
    KERNEL_TASK_START_ENTRY         StartEntry;
    KERNEL_HANDLE                   hContext;
}
KERNEL_TASK_CONTEXT, *PKERNEL_TASK_CONTEXT;


/**********************************************************************
                        FUNCITON PROTOTYPES
**********************************************************************/

static   void
messageHandler
    (
        ATMOS_MESSAGE*              msg
    );

int
KernelTaskRoutineEntry
    (
        int                         argc,
        char*                       argv[]
    );

BOOLEAN
KernelTaskSaveContext
    (
        PKERNEL_TASK_CONTEXT        pContext
    );

PKERNEL_TASK_CONTEXT
KernelTaskClaimContext
    (
        const char*                 pTaskName
    );



/**********************************************************************
                         GLOBAL VARIABLES
**********************************************************************/

/*
 *  we don't need many since we only need to handle the backlogs between
 *  the task creation and actual task start.
 */
#define  KERNEL_TASK_MAX_TASK_CONTEXT_COUNT         64

static   PKERNEL_TASK_CONTEXT       gTaskContextArray[KERNEL_TASK_MAX_TASK_CONTEXT_COUNT] = {0};

/**********************************************************************
                          ROUTINE TRUNKS
**********************************************************************/

BOOLEAN
KernelTaskSaveContext
    (
        PKERNEL_TASK_CONTEXT        pContext
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < KERNEL_TASK_MAX_TASK_CONTEXT_COUNT; ulIndex++ )
    {
        if ( gTaskContextArray[ulIndex] == NULL )
        {
            gTaskContextArray[ulIndex] = pContext;

            return  TRUE;
        }
    }

    KernelAssert(FALSE);

    return  FALSE;
}


PKERNEL_TASK_CONTEXT
KernelTaskClaimContext
    (
        const char*                 pTaskName
    )
{
    PKERNEL_TASK_CONTEXT            pContext    = NULL;
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < KERNEL_TASK_MAX_TASK_CONTEXT_COUNT; ulIndex++ )
    {
        if ( _ansc_strcmp(gTaskContextArray[ulIndex]->TaskName, pTaskName) == 0 )
        {
            pContext                    = gTaskContextArray[ulIndex];
            gTaskContextArray[ulIndex]  = NULL;

            return  pContext;
        }
    }

    KernelAssert(FALSE);

    return  NULL;
}


static   void
messageHandler
    (
        ATMOS_MESSAGE*              msg
    )
{
    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelTask: messageHandler ...\n"
        );

    switch ( msg->code )
    {
        case    MSG_N_TELL:

                sendreply(msg);

                break;

        default:

                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_ERROR,
                        KERNEL_DBG_MASK_GENERIC,
                        "KernelTask: Received unknown message, code = 0x%X!\n",
                        msg->code
                    );

                KernelAssert(FALSE);

                break;
    }

    return;
}


/*
 *  provide a faked main() for llibc -- the function _main() in source/llibc/lmain.c is referencing main().
 */
int main(int argc, char *argv[])
{
    kprintf("This main() routine should never be called!!!\n");

    KernelAssert(FALSE);

    return  0;
}

int
KernelTaskRoutineEntry
    (
        int                         argc,
        char*                       argv[]
    )
{
    PKERNEL_TASK_CONTEXT            pTaskContext    = NULL;

    pTaskContext = KernelTaskClaimContext(atmos_pcb_current_get_name());

    if ( pTaskContext == NULL )
    {
        KernelTrace2
            (
                ANSC_DBG_LEVEL_CRITICAL,
                KERNEL_DBG_MASK_GENERIC,
                "KernelTaskRoutineEntry -- cannot find the context!\n"
            );

        return  -1;
    }

    KernelAssert(pTaskContext->StartEntry != NULL);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelTaskRoutineEntry -- task: %s, handle = 0x%lX, context = 0x%lX, start entry = 0x%lX, argc = 0x%lX, argv = 0x%lX.\n",
            atmos_pcb_current_get_name(),
            KernelGetCurTaskHandle(),
            pTaskContext,
            pTaskContext->StartEntry,
            argc,
            argv
        );

    /*
     *  initialize llibc to enable tasks to call time related system apis.
     */
    _initllibc();
    /*
     *  initialize i/o, which initialize llibc as well, to enable printf for tasks (also time related system apis).
     */
    /*_initio();*/

    /*
     *  always register the socket message handler
     */
    socket_register_message_handler(messageHandler);

    /*
     *  open foreground output
     */
    /*foreground_output_begin();*/

    /*
     *  call the real task routine
     */
    pTaskContext->StartEntry(pTaskContext->hContext);

    KernelFreeMemory(pTaskContext);

    /*
     *  stop foreground output
     */
    /*foreground_output_end();*/

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelTaskRoutineEntry -- task %s exits, handle = 0x%lX.\n",
            atmos_pcb_current_get_name(),
            KernelGetCurTaskHandle()
        );

    return 0;
}


KERNEL_HANDLE
KernelCreateTask2
    (
        KERNEL_TASK_START_ENTRY     startEntry,
        ULONG                       stackSize,
        ULONG                       priority,
        KERNEL_HANDLE               hContext,
        char*                       pName
    )
{
    /*
     *  ISOS is thread safe
     */
    static  ULONG                   taskNo          = 0;

    PKERNEL_TASK_CONTEXT            pTaskContext    = NULL;

    tASTask                         hTask;
    tASErr                          taskErr;
    char                            pTaskFullName[128];
    char                            pTaskProperties[128];

    pTaskContext = KernelAllocateMemory(sizeof(KERNEL_TASK_CONTEXT));

    if ( pTaskContext == NULL )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_GENERIC,
                "KernelCreateTask -- cannot allocate memory for the task context, size = %d.\n",
                sizeof(KERNEL_TASK_CONTEXT)
            );

        return  KERNEL_HANDLE_NULL;
    }

    if ( pName == NULL )
    {
        _ansc_sprintf(pTaskContext->TaskName, "Ansc%06u", taskNo++);
        pName = pTaskContext->TaskName;
    }
    else
    {
        _ansc_sprintf(pTaskContext->TaskName, "%s%06u", pName, taskNo++);
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelCreateTask -- current task = %s, new taskName = %s, startEntry = 0x%X, stackSize = %d, priority = %d, context = 0x%X, created context = 0x%lX.\n",
            atmos_pcb_current_get_name(),
            pTaskContext->TaskName,
            startEntry,
            stackSize,
            priority,
            hContext,
            pTaskContext
        );

    pTaskContext->StartEntry    = startEntry;
    pTaskContext->hContext      = hContext;

    KernelTaskSaveContext(pTaskContext);

    /*
     *  create and start the task -- use pure quantum APIs
     *
    return  (KERNEL_HANDLE)
        createCprocess
            (
                priority,
                (VFP)KernelTaskRoutineEntry,
                (VFP)atmos_exitprocess,
                pTaskContext->TaskName,
                stackSize / sizeof(ULONG),
                (int)pTaskContext
            );
     */
    _ansc_sprintf
        (
            pTaskFullName,
            "/task/%s",
            pTaskContext->TaskName
        );
    _ansc_sprintf
        (
            pTaskProperties,
            "stack=%d/service=%d/main=%p",
            stackSize,
            priority,
            KernelTaskRoutineEntry
        );

    taskErr = hs_TaskNew(&hTask, pTaskFullName, pTaskProperties, kASTaskInvalid);

    if ( kASEOK != taskErr )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_ALL,
                "KernelCreateTask -- cannot create the task %s, properties %s, error code = %d!\n",
                pTaskFullName,
                pTaskProperties,
                taskErr
            );

        hTask = (tASTask)0;

        goto  ERROR_EXIT;
    }

    taskErr = as_TaskStart(hTask);

    if ( kASEOK == taskErr )
    {
        return  hTask;
    }
    else
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_ALL,
                "KernelCreateTask -- failed to start the task %s, error code = %d!\n",
                pTaskFullName,
                taskErr
            );

        as_TaskDelete(hTask);

        hTask = (tASTask)0;
    }

ERROR_EXIT:

    KernelTaskClaimContext(pTaskContext->TaskName);

    KernelFreeMemory(pTaskContext);

    return  hTask;
}


VOID
KernelDeleteTask
    (
        KERNEL_HANDLE               hTask
    )
{
    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelDeleteTask...\n"
        );

    if ( TRUE )
    {
        tASErr                          asErr;

        asErr = as_TaskDelete((tASTask)hTask);

        if ( asErr != kASESUCCESS )
        {
            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_WARNING,
                    KERNEL_DBG_MASK_GENERIC,
                    "KernelDeleteTask -- failed to delete the task, error code = %d, handle = 0x%lX...\n",
                    asErr,
                    hTask
                );
        }
    }
    else
    {
        atmos_controlprocess((ATMOS_PCBID)hTask, ATMOS_PCB_OP_KILL);
    }

    return;
}


VOID
KernelKillTask
    (
        KERNEL_HANDLE               hTask
    )
{
    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelKillTask...\n"
        );

    if ( TRUE )
    {
        tASErr                          asErr;

        asErr = as_TaskDelete((tASTask)hTask);

        if ( asErr != kASESUCCESS )
        {
            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_WARNING,
                    KERNEL_DBG_MASK_GENERIC,
                    "KernelKillTask -- failed to delete the task, error code = %d, handle = 0x%lX...\n",
                    asErr,
                    hTask
                );
        }
    }
    else
    {
        atmos_controlprocess((ATMOS_PCBID)hTask, ATMOS_PCB_OP_KILL);
    }

    return;
}

