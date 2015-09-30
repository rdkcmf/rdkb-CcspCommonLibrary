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

    module:	kernel_protection.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelInitializeEvent
        *   KernelSetEvent
        *   KernelResetEvent
        *   KernelPulseEvent
        *   KernelWaitEvent
        *   KernelFreeEvent

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


BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    KernelInitializeLock(&pEvent->AccessLock);

    pEvent->NumberSignaled  = 0;
    pEvent->NumberWaiting   = 0;

    return  TRUE;
}


VOID
KernelSetEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    KernelAcquireLock(&pEvent->AccessLock);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelSetEvent -- signaled = %d, waiting = %d.\n",
            pEvent->NumberSignaled,
            pEvent->NumberWaiting
        );

    if (pEvent->NumberWaiting == 0)
    {
        pEvent->NumberSignaled = 1;
    }
    else
    {
        pEvent->NumberSignaled ++;
    }

    KernelReleaseLock(&pEvent->AccessLock);

    return;
}


VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    KernelAcquireLock(&pEvent->AccessLock);

    pEvent->NumberSignaled = 0;

    KernelReleaseLock(&pEvent->AccessLock);

    return;
}


VOID
KernelPulseEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    KernelAcquireLock(&pEvent->AccessLock);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelPulseEvent -- signaled = %d, waiting = %d.\n",
            pEvent->NumberSignaled,
            pEvent->NumberWaiting
        );

    pEvent->NumberSignaled = pEvent->NumberWaiting;

    KernelReleaseLock(&pEvent->AccessLock);

    return;
}


int
KernelWaitEvent
    (
        PKERNEL_EVENT               pEvent,
        ULONG                       ulMilliSeconds
    )
{
    ULONG                           startTimeStamp  = 0;

    startTimeStamp = KernelGetTickInMilliSeconds();

    KernelAcquireLock(&pEvent->AccessLock);

    if (pEvent->NumberSignaled > 0)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TRACE_FLOW,
                KERNEL_DBG_MASK_GENERIC,
                "KernelWaitEvent -- signaled = %d, waiting = %d.\n",
                pEvent->NumberSignaled,
                pEvent->NumberWaiting
            );

        pEvent->NumberSignaled --;

        KernelReleaseLock(&pEvent->AccessLock);

        return  KERNEL_STATUS_SUCCESS;
    }

    pEvent->NumberWaiting ++;

    KernelReleaseLock(&pEvent->AccessLock);

    /*
     *  let others take the time slice
     */
    KernelSleepInMilliSeconds(1000);

    /*
     *  wait until time-out
     */
    while ( (KernelGetTickInMilliSeconds() - startTimeStamp) < ulMilliSeconds )
    {
        ULONG                       ulPrevTs    = 0;

        KernelAcquireLock(&pEvent->AccessLock);

        if (pEvent->NumberSignaled > 0)
        {
            pEvent->NumberSignaled --;
            pEvent->NumberWaiting  --;

            KernelReleaseLock(&pEvent->AccessLock);

            return  KERNEL_STATUS_SUCCESS;
        }

        KernelReleaseLock(&pEvent->AccessLock);

        /*
         *  let others take the time slice
         */
        KernelSleepInMilliSeconds(1000);
    }

    KernelAcquireLock(&pEvent->AccessLock);

    pEvent->NumberWaiting  --;

    KernelReleaseLock(&pEvent->AccessLock);

    return  KERNEL_STATUS_TIMEOUT;
}


VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    KernelAcquireLock(&pEvent->AccessLock);

    pEvent->NumberSignaled  = 0;
    pEvent->NumberWaiting   = 0;

    KernelReleaseLock(&pEvent->AccessLock);

    KernelFreeLock(&pEvent->AccessLock);

    return;
}
