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

    module:	ansc_tso_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Timer Scheduler Object.

        *   AnscTsoRegisterTimer
        *   AnscTsoCancelTimer
        *   AnscTsoScheduleTimer

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
        AnscTsoRegisterTimer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTimerDescriptor
            );

    description:

        This function is called by external objects to register a new
        timer descriptor object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTimerDescriptor
                Specifies the timer descriptor object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoRegisterTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject        = (PANSC_TIMER_SCHEDULER_OBJECT )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTimerDescriptor = (PANSC_TIMER_DESCRIPTOR_OBJECT)hTimerDescriptor;
    ULONG                           ulCurTickInMs    = AnscGetTickInMilliSeconds();

    if ( !pMyObject->bStarted )
    {
        pMyObject->Start((ANSC_HANDLE)pMyObject);
    }

    AnscAcquireSpinLock(&pMyObject->TdoQueueSpinLock);
    pTimerDescriptor->AcquireAccess((ANSC_HANDLE)pTimerDescriptor);

    if ( pTimerDescriptor->GetState((ANSC_HANDLE)pTimerDescriptor) )
    {
        pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
        AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTimerDescriptor->SetStartTime ((ANSC_HANDLE)pTimerDescriptor, AnscGetTickInSeconds()                    );
        pTimerDescriptor->SetTimestamp ((ANSC_HANDLE)pTimerDescriptor, ulCurTickInMs                             );
        pTimerDescriptor->SetState     ((ANSC_HANDLE)pTimerDescriptor, TRUE                                      );
        pTimerDescriptor->SetScheduler ((ANSC_HANDLE)pTimerDescriptor, (ANSC_HANDLE)pMyObject                    );
        pTimerDescriptor->SetTimeToFire((ANSC_HANDLE)pTimerDescriptor, ulCurTickInMs + pTimerDescriptor->Interval);
    }

    returnStatus =
        pMyObject->ScheduleTimer
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pTimerDescriptor
            );

    pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
    AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoCancelTimer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTimerDescriptor
            );

    description:

        This function is called by external objects to cancel an active
        timer descriptor object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTimerDescriptor
                Specifies the timer descriptor object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoCancelTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject        = (PANSC_TIMER_SCHEDULER_OBJECT )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTimerDescriptor = (PANSC_TIMER_DESCRIPTOR_OBJECT)hTimerDescriptor;
    BOOL                            bWaitForInvoke   = FALSE;

    AnscAcquireSpinLock(&pMyObject->TdoQueueSpinLock);
    pTimerDescriptor->AcquireAccess((ANSC_HANDLE)pTimerDescriptor);

    if ( !pTimerDescriptor->GetState((ANSC_HANDLE)pTimerDescriptor) )
    {
        pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
        AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pTimerDescriptor->GetScheduler((ANSC_HANDLE)pTimerDescriptor) != (ANSC_HANDLE)pMyObject )
    {
        pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
        AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

        return  ANSC_STATUS_SUCCESS;
    }

    AnscQueuePopEntryByLink(&pMyObject->TdoQueue, &pTimerDescriptor->Linkage);

    pTimerDescriptor->SetState((ANSC_HANDLE)pTimerDescriptor, FALSE);

    if ( pMyObject->hCurInvokedTdo == (ANSC_HANDLE)pTimerDescriptor )
    {
        bWaitForInvoke = TRUE;
    }

    pTimerDescriptor->ReleaseAccess((ANSC_HANDLE)pTimerDescriptor);
    AnscReleaseSpinLock(&pMyObject->TdoQueueSpinLock);

    if ( bWaitForInvoke && !(pTimerDescriptor->TimerMode & ANSC_TIMER_MODE_asyncStop) )
    {
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoScheduleTimer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTimerDescriptor
            );

    description:

        This function is called by external objects to schedule a
        timer descriptor object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTimerDescriptor
                Specifies the timer descriptor object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTsoScheduleTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject           = (PANSC_TIMER_SCHEDULER_OBJECT )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTbpTimerDescriptor = (PANSC_TIMER_DESCRIPTOR_OBJECT)hTimerDescriptor;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCurTimerDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = NULL;
    BOOL                            bScheduled          = FALSE;
    ULONG                           ulTimeSlice         = 0;

    if ( pTbpTimerDescriptor->Interval > 0 )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->TdoQueue);

        while ( pSLinkEntry )
        {
            pCurTimerDescriptor = ACCESS_ANSC_TIMER_DESCRIPTOR_OBJECT(pSLinkEntry);
            pSLinkEntry         = AnscQueueGetNextEntry(pSLinkEntry);

            /*
             * Since the internal of an ANSC timer is specified in the number of milli-seconds, it will
             * wrap around in 49.7 days. We need to consider the wrap around when we determine the
             * relative firing order of different timers.
             */
            if ( pCurTimerDescriptor->Timestamp <= pCurTimerDescriptor->TimeToFire )
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
                        &pMyObject->TdoQueue,
                        &pTbpTimerDescriptor->Linkage,
                        &pCurTimerDescriptor->Linkage
                    );

                break;
            }
        }

        if ( !bScheduled )
        {
            bScheduled = TRUE;

            AnscQueuePushEntry(&pMyObject->TdoQueue, &pTbpTimerDescriptor->Linkage);
        }
    }
    else
    {
        bScheduled = TRUE;

        AnscQueuePushEntryAtFront(&pMyObject->TdoQueue, &pTbpTimerDescriptor->Linkage);
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

    if ( ulTimeSlice < pMyObject->SchedulingUnit )
    {
        pMyObject->SchedulingUnit = AnscGetMax2(ANSC_TSO_MIN_SCHEDULING_UNIT, ulTimeSlice);
    }

    return  ANSC_STATUS_SUCCESS;
}
