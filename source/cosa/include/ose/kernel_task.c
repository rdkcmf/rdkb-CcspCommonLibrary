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

        This wrapper file implements the platform dependent routines.

        *   KernelTaskRoutineEntry

        *   KernelCreateTask

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


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"


/**********************************************************************
                     DEFINITIONS FOR THIS FILE
**********************************************************************/

typedef  struct
_KERNEL_TASK_CONTEXT
{
    KERNEL_TASK_START_ENTRY         StartEntry;
    KERNEL_HANDLE                   hContext;
}
KERNEL_TASK_CONTEXT, *PKERNEL_TASK_CONTEXT;


/**********************************************************************
                        FUNCITON PROTOTYPES
**********************************************************************/


/**********************************************************************
                         GLOBAL VARIABLES
**********************************************************************/
static   SEMAPHORE                  gSerializeSem   = {1, NULL, NULL};
static   SEMAPHORE                  gCompletionSem  = {0, NULL, NULL};
static   PKERNEL_TASK_CONTEXT       gpTaskContext   = NULL;

/**********************************************************************
                          ROUTINE TRUNKS
**********************************************************************/

OS_PROCESS(KernelTaskRoutineEntry)
{
    PKERNEL_TASK_CONTEXT            pTaskContext    = gpTaskContext;
    ANSC_SOCKET                     InitSock;

    /*
     *  clear our the global pointer to acknowledge that we recevied it
     */
    gpTaskContext = NULL;

    KernelAssert(pTaskContext != NULL);
    KernelAssert(pTaskContext->StartEntry != NULL);

    /*
     *  nofity the starter that we've got it
     */
    signal_sem(&gCompletionSem);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelTaskRoutineEntry -- task context = 0x%X ...\n",
            pTaskContext
        );

    /*
     *  patch for ose: we need to issue a socket call to get inet
     *  library initialized for the process
     */
    InitSock = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_DGRAM, 0);

    KernelAssert(InitSock != SOCKET_ERROR);

    _ansc_closesocket(InitSock);

    /*
     *  call the real task routine
     */
    pTaskContext->StartEntry(pTaskContext->hContext);

    KernelFreeMemory(pTaskContext);

    /*
     *  clean up everything about this process
     */
    stop(current_process());

    return;
}


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
    static  ULONG                   taskNo          = 0;

    PKERNEL_TASK_CONTEXT            pTaskContext    = NULL;
    PROCESS                         pProc;

    wait_sem(&gSerializeSem);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelCreateTask -- taskName = %s, startEntry = 0x%X, stackSize = %d, priority = %d, context = 0x%X.\n",
            taskName,
            startEntry,
            stackSize,
            priority,
            hContext
        );

    pTaskContext = KernelAllocateMemory(sizeof(KERNEL_TASK_CONTEXT));

    if (pTaskContext == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_GENERIC,
                "KernelCreateTask -- cannot allocate memory for the task context, size = %d.\n",
                sizeof(KERNEL_TASK_CONTEXT)
            );

        signal_sem(&gSerializeSem);

        return  KERNEL_HANDLE_NULL;
    }

    pTaskContext->StartEntry    = startEntry;
    pTaskContext->hContext      = hContext;

    KernelAssert(gpTaskContext == NULL);

    gpTaskContext = pTaskContext;

    pProc =
        create_process
            (
                priority - (priority % 64),
                taskName,
                KernelTaskRoutineEntry,
                stackSize,
                priority % 64,
                0,
                (PROCESS)0,
                (struct OS_redir_entry *)0,
                0x0000,
                (OSUSER)0
            );

    if (!pProc)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_GENERIC,
                "KernelCreateTask -- failed to create the process %s!\n",
                taskName
            );

        /*
         *  capture this exception
         */
        KernelAssert(FALSE);

        signal_sem(&gSerializeSem);

        return  KERNEL_HANDLE_NULL;
    }

    /*
     *  start the process
     */
    start(pProc);

    /*
     *  wait for the process complete access to the global task context
     */
    wait_sem(&gCompletionSem);

    KernelAssert(gpTaskContext == NULL);

    signal_sem(&gSerializeSem);

    return  (KERNEL_HANDLE)pProc;
}
