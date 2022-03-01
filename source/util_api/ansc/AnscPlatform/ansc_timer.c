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

    module:	ansc_timer.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the platform-dependent timer
        management and scheduling functions.

        *   AnscTimerScheduleTask
        *   AnscSchedulerInitialize
        *   AnscRegisterTimer
        *   AnscCancelTimer
        *   AnscScheduleTimer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/16/07    initial revision.

**********************************************************************/


#include "ansc_platform.h"
#include "ansc_timer.h"

static BOOL                                g_bSchedulerInitialized = FALSE;
static ULONG                               g_bScheduleTaskCount    = 0;
static ULONG                               g_SchedulingUnit        = ANSC_DEF_SCHEDULING_UNIT;
static ANSC_HANDLE                         g_hCurInvokedTimer      = (ANSC_HANDLE)NULL;

static ANSC_LOCK                           g_SyncLock;
static QUEUE_HEADER                        g_TimerQueue;
static ANSC_LOCK                           g_TimerQueueLock;


void
AnscTimerScheduleTask
    (
        void*                       dummy
    )
{
    UNREFERENCED_PARAMETER(dummy);
    PANSC_TIMER_DESCRIPTOR          pTimerDescriptor = NULL;
    PANSC_TIMER_DESCRIPTOR          pFreshTimer      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           ulCurTickInMs    = 0;
    BOOL                            bFireTimer       = FALSE;

    g_bScheduleTaskCount++;

    /*
     * Following is an endless loop until it's interrupted by an external object or the container.
     * We wake up every once a while and check the timer queue to see if anyone needs his fifteen
     * minutes of fame.
     */
    while ( AnscQueueQueryDepth(&g_TimerQueue) > 0 )
    {
        /*
         * When choosing the scheduling unit, we have to strike a balance between the performance
         * and responsiveness. The default interval is 10 milliseconds, which is the smallest
         * resolution for timer control on most operating systems.
         */
        AnscSleep(g_SchedulingUnit);

        /*
         * It's possible that multiple timers were scheduled to go off at the same time, which
         * could happen if the differece between their intervals is smaller than the scheduling
         * unit. We invoke them one by one.
         */
        AnscAcquireLock(&g_TimerQueueLock);

        pFreshTimer        = NULL;
        ulCurTickInMs      = AnscGetTickInMilliSeconds();
        g_hCurInvokedTimer = (ANSC_HANDLE)NULL;
        pSLinkEntry        = AnscQueueGetFirstEntry(&g_TimerQueue);

        while ( pSLinkEntry )
        {
            pTimerDescriptor = ACCESS_ANSC_TIMER_DESCRIPTOR(pSLinkEntry);
            ulCurTickInMs    = AnscGetTickInMilliSeconds();

            AnscAcquireLock(&pTimerDescriptor->AccessLock);

            if ( pTimerDescriptor == pFreshTimer )
            {
                AnscReleaseLock(&pTimerDescriptor->AccessLock);

                break;
            }

            if ( pTimerDescriptor->Timestamp < pTimerDescriptor->TimeToFire )
            {
                if ( (ulCurTickInMs < pTimerDescriptor->Timestamp ) &&
                     (ulCurTickInMs < pTimerDescriptor->TimeToFire) )
                {
                    bFireTimer = TRUE;
                }
                else if ( (ulCurTickInMs >= pTimerDescriptor->Timestamp ) &&
                          (ulCurTickInMs >= pTimerDescriptor->TimeToFire) )
                {
                    bFireTimer = TRUE;
                }
                else
                {
                    bFireTimer = FALSE;
                }
            }
            else if ( pTimerDescriptor->Timestamp == pTimerDescriptor->TimeToFire )
            {
                bFireTimer = TRUE;
            }
            else
            {
                if ( (ulCurTickInMs <  pTimerDescriptor->Timestamp ) &&
                     (ulCurTickInMs >= pTimerDescriptor->TimeToFire) )
                {
                    bFireTimer = TRUE;
                }
                else
                {
                    bFireTimer = FALSE;
                }
            }

            if ( !bFireTimer )
            {
                AnscReleaseLock(&pTimerDescriptor->AccessLock);

                break;
            }
            else
            {
                /*
                 * Unlink the timer descriptor from the queue before we invoke the callback member
                 * function. Note that both the register_timer and the cancel_timer() members
                 * may be called during the invocation ...
                 */
                AnscQueuePopEntryByLink(&g_TimerQueue, &pTimerDescriptor->Linkage);

                if ( pTimerDescriptor->TimerType == ANSC_TIMER_TYPE_PERIODIC )
                {
                    pTimerDescriptor->Timestamp  = ulCurTickInMs;
                    pTimerDescriptor->bScheduled = TRUE;
                    pTimerDescriptor->TimeToFire = ulCurTickInMs + pTimerDescriptor->Interval;

                        AnscScheduleTimer
                            (
                                (ANSC_HANDLE)pTimerDescriptor
                            );
                }
                else
                {
                    pTimerDescriptor->bScheduled = FALSE;
                }

                pFreshTimer        = pTimerDescriptor;
                g_hCurInvokedTimer = (ANSC_HANDLE)pTimerDescriptor;

                /*
                 * The right order of the following locking/unlocking sequence is crucial! If you
                 * cannot understand it completely at the first look, it's OK, doesn't mean you're
                 * stupid or have a lower IQ.
                 */

                AnscReleaseLock(&pTimerDescriptor->AccessLock);
                AnscReleaseLock(&g_TimerQueueLock            );

                /*
                 * We fire the timer from the current task context directly!? You might think that
                 * we should spawn another task for timer invocation. It's true that a separate
                 * task for firing the timer makes much better sense in terms of time management
                 * and accommodating multi-tasking programming practice. However, it would be
                 * harder to synchronize and less flexible.
                 */
                pTimerDescriptor->Invoke(pTimerDescriptor->hInvokeContext);

            }

            AnscAcquireLock(&g_TimerQueueLock);
            g_hCurInvokedTimer = (ANSC_HANDLE)NULL;
            pSLinkEntry        = AnscQueueGetFirstEntry(&g_TimerQueue);
        }

        AnscReleaseLock(&g_TimerQueueLock);
    }

    g_bScheduleTaskCount--;
}


void
AnscSchedulerInitialize
    (
        void
    )
{
    AnscInitializeLock       (&g_SyncLock      );
    AnscQueueInitializeHeader(&g_TimerQueue    );
    AnscInitializeLock       (&g_TimerQueueLock);

    g_bSchedulerInitialized = TRUE;
}


ANSC_STATUS
AnscRegisterTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_DESCRIPTOR          pTimerDescriptor = (PANSC_TIMER_DESCRIPTOR)hTimerDescriptor;
    ULONG                           ulCurTickInMs    = AnscGetTickInMilliSeconds();

    if ( !g_bSchedulerInitialized )
    {
        AnscSchedulerInitialize();
    }

    AnscAcquireLock(&g_TimerQueueLock            );
    AnscAcquireLock(&pTimerDescriptor->AccessLock);

    if ( pTimerDescriptor->bScheduled )
    {
        AnscReleaseLock(&pTimerDescriptor->AccessLock);
        AnscReleaseLock(&g_TimerQueueLock            );

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTimerDescriptor->StartTime  = AnscGetTickInSeconds();
        pTimerDescriptor->Timestamp  = ulCurTickInMs;
        pTimerDescriptor->bScheduled = TRUE;
        pTimerDescriptor->TimeToFire = ulCurTickInMs + pTimerDescriptor->Interval;
    }

    returnStatus = AnscScheduleTimer((ANSC_HANDLE)pTimerDescriptor);

    AnscReleaseLock(&pTimerDescriptor->AccessLock);
    AnscReleaseLock(&g_TimerQueueLock            );

    if ( g_bScheduleTaskCount == 0)
    {
        AnscCreateTask
            (
                AnscTimerScheduleTask,
                ANSC_DEFAULT_TASK_STACK_SIZE,
                ANSC_DEFAULT_TASK_PRIORITY,
                NULL,
                "timer_scheduler_task"
            );
    }

    return  returnStatus;
}


ANSC_STATUS
AnscCancelTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    PANSC_TIMER_DESCRIPTOR          pTimerDescriptor = (PANSC_TIMER_DESCRIPTOR)hTimerDescriptor;
    BOOL                            bWaitForInvoke   = FALSE;

    AnscAcquireLock(&g_TimerQueueLock            );
    AnscAcquireLock(&pTimerDescriptor->AccessLock);

    if ( !pTimerDescriptor->bScheduled )
    {
        AnscReleaseLock(&pTimerDescriptor->AccessLock);
        AnscReleaseLock(&g_TimerQueueLock            );

        return  ANSC_STATUS_SUCCESS;
    }

    AnscQueuePopEntryByLink(&g_TimerQueue, &pTimerDescriptor->Linkage);

    pTimerDescriptor->bScheduled = FALSE;

    if ( g_hCurInvokedTimer == (ANSC_HANDLE)pTimerDescriptor )
    {
        bWaitForInvoke = TRUE;
    }

    AnscReleaseLock(&pTimerDescriptor->AccessLock);
    AnscReleaseLock(&g_TimerQueueLock            );

    if ( bWaitForInvoke )
    {
    }

    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
AnscScheduleTimer
    (
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    PANSC_TIMER_DESCRIPTOR          pTbpTimerDescriptor = (PANSC_TIMER_DESCRIPTOR)hTimerDescriptor;
    PANSC_TIMER_DESCRIPTOR          pCurTimerDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = NULL;
    BOOL                            bScheduled          = FALSE;
    ULONG                           ulTimeSlice         = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&g_TimerQueue);

    while ( pSLinkEntry )
    {
        pCurTimerDescriptor = ACCESS_ANSC_TIMER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry         = AnscQueueGetNextEntry(pSLinkEntry);

        /*
         * Since the internal of an ANSC timer is specified in the number of milli-seconds, it will
         * wrap around in 49.7 days. We need to consider the wrap around when we determine the
         * relative firing order of different timers.
         */
        if ( pCurTimerDescriptor->Timestamp < pCurTimerDescriptor->TimeToFire )
        {
            if ( (pTbpTimerDescriptor->TimeToFire < pCurTimerDescriptor->Timestamp ) &&
                 (pTbpTimerDescriptor->TimeToFire < pCurTimerDescriptor->TimeToFire) )
            {
                continue;
            }
            else if ( (pTbpTimerDescriptor->TimeToFire >= pCurTimerDescriptor->Timestamp ) &&
                      (pTbpTimerDescriptor->TimeToFire >= pCurTimerDescriptor->TimeToFire) )
            {
                continue;
            }
        }
        else
        {
            if ( (pTbpTimerDescriptor->TimeToFire <  pCurTimerDescriptor->Timestamp ) &&
                 (pTbpTimerDescriptor->TimeToFire >= pCurTimerDescriptor->TimeToFire) )
            {
                continue;
            }
        }

        if ( TRUE )
        {
            bScheduled = TRUE;

            AnscQueuePushEntryBefore
                (
                    &g_TimerQueue,
                    &pTbpTimerDescriptor->Linkage,
                    &pCurTimerDescriptor->Linkage
                );

            break;
        }
    }

    if ( !bScheduled )
    {
        bScheduled = TRUE;

        AnscQueuePushEntry(&g_TimerQueue, &pTbpTimerDescriptor->Linkage);
    }

    /*
     * Change the default scheduling unit if the interval of the specified timer descriptor demands
     * a smaller unit. We follow the steps highlighted below to calculate the new scheduling unit:
     *
     *      - divide the new interval by five;
     *      - if this value is bigger than the current unit, no change needs to be made;
     *      - otherwise, if the new value is bigger than 10ms, use the new value;
     *      - otherwise, use 10ms instead.
     */
    ulTimeSlice = pTbpTimerDescriptor->Interval / 5;

    if ( ulTimeSlice < g_SchedulingUnit )
    {
        g_SchedulingUnit = AnscGetMax2(ANSC_MIN_SCHEDULING_UNIT, ulTimeSlice);
    }

    return  ANSC_STATUS_SUCCESS;
}
