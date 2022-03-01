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

    module:	ansc_tso_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Timer Scheduler Object.

        *   AnscTsoScheduleTask
        *   AnscTsoStart
        *   AnscTsoStop

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/01    initial revision.

**********************************************************************/


#include "ansc_tso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoScheduleTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function runs as a background task, which performs timer
        scheduling until it's interrupted.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoScheduleTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject        = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTimerDescriptor = NULL;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pFreshTdo        = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    ULONG                           ulCurTickInMs    = 0;
    BOOL                            bFireTimer       = FALSE;

    /*
     * Following is an endless loop until it's interrupted by an external object or the container.
     * We wake up every once a while and check the timer queue to see if anyone needs his fifteen
     * minutes of fame.
     */
    while ( pMyObject->bStarted )
    {
        ANSC_COMMIT_TASK();

        /*
         * When choosing the scheduling unit, we have to strike a balance between the performance
         * and responsiveness. The default interval is 10 milliseconds, which is the smallest
         * resolution for timer control on most operating systems.
         */
        AnscSleep(pMyObject->SchedulingUnit);

        /*
         * It's possible that multiple timers were scheduled to go off at the same time, which
         * could happen if the differece between their intervals is smaller than the scheduling
         * unit. We invoke them one by one.
         */
        AnscAcquireSpinLock(&pMyObject->TdoQueueSpinLock);

        pFreshTdo                 = NULL;
        ulCurTickInMs             = AnscGetTickInMilliSeconds();
        pMyObject->hCurInvokedTdo = (ANSC_HANDLE)NULL;
        pSLinkEntry               = AnscQueueGetFirstEntry(&pMyObject->TdoQueue);

        while ( pSLinkEntry )
        {
            pTimerDescriptor = ACCESS_ANSC_TIMER_DESCRIPTOR_OBJECT(pSLinkEntry);
            ulCurTickInMs    = AnscGetTickInMilliSeconds();

            pTimerDescriptor->AcquireAccess((ANSC_HANDLE)pTimerDescriptor);

            if ( pTimerDescriptor == pFreshTdo )
            {
                pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);

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
                pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);

                pMyObject->SchedulingUnit = ANSC_TSO_DEF_SCHEDULING_UNIT;

                break;
            }
            else
            {
                /*
                 * Unlink the timer descriptor from the queue before we invoke the callback member
                 * function. Note that both the register_timer and the cancel_timer() members
                 * may be called during the invocation ...
                 */
                AnscQueuePopEntryByLink(&pMyObject->TdoQueue, &pTimerDescriptor->Linkage);

                if ( pTimerDescriptor->GetTimerType((ANSC_HANDLE)pTimerDescriptor) == ANSC_TIMER_TYPE_PERIODIC )
                {
                    pTimerDescriptor->SetTimestamp ((ANSC_HANDLE)pTimerDescriptor, ulCurTickInMs                             );
                    pTimerDescriptor->SetState     ((ANSC_HANDLE)pTimerDescriptor, TRUE                                      );
                    pTimerDescriptor->SetTimeToFire((ANSC_HANDLE)pTimerDescriptor, ulCurTickInMs + pTimerDescriptor->Interval);

                    returnStatus =
                        pMyObject->ScheduleTimer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pTimerDescriptor
                            );
                }
                else
                {
                    pTimerDescriptor->SetState((ANSC_HANDLE)pTimerDescriptor, FALSE);
                }

                pFreshTdo                 = pTimerDescriptor;
                pMyObject->hCurInvokedTdo = (ANSC_HANDLE)pTimerDescriptor;

                /*
                 * The right order of the following locking/unlocking sequence is crucial! If you
                 * cannot understand it completely at the first look, it's OK, doesn't mean you're
                 * stupid or have a lower IQ.
                 */

                pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
                AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

                /*
                 * We fire the timer from the current task context directly!? You might think that
                 * we should spawn another task for timer invocation. It's true that a separate
                 * task for firing the timer makes much better sense in terms of time management
                 * and accommodating multi-tasking programming practice. However, it would be
                 * harder to synchronize and less flexible.
                 */
                returnStatus = pTimerDescriptor->Invoke((ANSC_HANDLE)pTimerDescriptor);

            }

            AnscAcquireSpinLock(&pMyObject->TdoQueueSpinLock);
            pMyObject->hCurInvokedTdo = (ANSC_HANDLE)NULL;
            pSLinkEntry               = AnscQueueGetFirstEntry(&pMyObject->TdoQueue);
        }

        AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);
    }

    if ( pMyObject->bStarted )
    {
        returnStatus = ANSC_STATUS_DO_IT_AGAIN;
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function spawns a background task to perform the desired
        timer scheduling.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    if ( pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = TRUE;
    }

    returnStatus =
        pMyObject->SpawnTask
            (
                (ANSC_HANDLE)pMyObject,
                (void*)pMyObject->ScheduleTask,
                (ANSC_HANDLE)pMyObject,
                "timer_scheduler_task"
             );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoStop
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function terminates the background scheduling task and
        wait for it to exit.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoStop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = FALSE;
    }

    return  ANSC_STATUS_SUCCESS;
}
