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
		10/15/01	pSOS port

    ---------------------------------------------------------------

    note:

        maximum number of semaphores    -- KC_NSEMA4 in sys_conf.h
        maximum number of queues        -- KC_NQUEUE in sys_conf.h

**********************************************************************/


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"


static  ULONG                       semaphoreNo = 0;
static  ULONG                       eventNo     = 0;


BOOLEAN
KernelInitializeLock
    (
        PKERNEL_LOCK                pLock
    )
{
    ULONG                           returnStatus;
    char                            semaphoreName[5] = {'A', 0, 0, 0, 0};

    /* pSOS doesn't check if the name is duplicate */
    sprintf(&semaphoreName[1], "%03d", semaphoreNo);

    KernelTrace("KernelInitializeLock -- %s.\n", semaphoreName);

    returnStatus = sm_create(semaphoreName, 1, SM_LOCAL | SM_PRIOR | SM_BOUNDED, pLock);

    if (returnStatus == 0)
    {
        semaphoreNo++;

        return  TRUE;
    }
    else
    {
        KernelTrace("KernelInitializeLock -- cannot create semaphore, error code = 0x%X!\n", returnStatus);

        return  FALSE;
    }
}


BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pEvent
    )
{
    ULONG                           returnStatus;
    char                            eventName[5] = {'A', 0, 0, 0, 0};

    /* pSOS doesn't check if the name is duplicate */
    sprintf(&eventName[1], "%03d", eventNo);

    KernelTrace("KernelInitializeEvent -- %s.\n", eventName);

    returnStatus = q_create(eventName, 0, Q_LOCAL | Q_FIFO | Q_NOLIMIT | Q_SYSBUF, pEvent);

    if (returnStatus == 0)
    {
        eventNo++;

        return  TRUE;
    }
    else
    {
        KernelTrace("KernelInitializeEvent -- cannot create event, error code = 0x%X!\n", returnStatus);

        return  FALSE;
    }
}

VOID
KernelSetEvent(PKERNEL_EVENT  pEvent)
{
    ULONG                           msgBuf[4] = {0};

    msgBuf[0] = 'K' << 24 | 'Q' << 16 | 'U' << 8 | 'E';

    q_send(*pEvent, msgBuf);
}


VOID
KernelPulseEvent(PKERNEL_EVENT  pEvent)
{
    ULONG                           msgBuf[4] = {0};
    ULONG                           readiedCount;

    msgBuf[0] = 'K' << 24 | 'Q' << 16 | 'U' << 8 | 'E';

    q_broadcast(*pEvent, msgBuf, &readiedCount);
}


int
KernelWaitEvent(PKERNEL_EVENT  pEvent, ULONG  ulMilliSeconds)
{
    ULONG                           msgBuf[4] = {0};
    ULONG                           returnStatus;

    if ( ulMilliSeconds == 0 )
    {
        returnStatus = q_receive(*pEvent, Q_WAIT | Q_DEQUEUE, 0, msgBuf);
    }
    else
    {
        returnStatus = q_receive(*pEvent, Q_WAIT | Q_DEQUEUE, ulMilliSeconds * KC_TICKS2SEC / 1000, msgBuf);
    }

    /*
     *  need to revisit these return values
     */
    if (returnStatus == 0)
    {
        /*
         *  sanity check -- verify if the received is what we expected
         */
        if (msgBuf[0] != ('K' << 24 | 'Q' << 16 | 'U' << 8 | 'E'))
        {
        }

        return  0;
    }
    else if (returnStatus == ERR_TIMEOUT)
    {
        return  1;
    }
    else
    {
        return  -1;
    }
}
