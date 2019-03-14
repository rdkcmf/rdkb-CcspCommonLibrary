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

    module:	ansc_oco_timers.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced timers management
        functions of the Ansc Object Container Object.

        *   AnscOcoRegisterTimer
        *   AnscOcoCancelTimer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#include "ansc_oco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoRegisterTimer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTimer
            );

    description:

        This function is called to register a timer object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTimer
                Specifies the timer descriptor object to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoRegisterTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject        = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pParentContainer = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PANSC_TIMER_SCHEDULER_OBJECT    pTimerScheduler  = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hTimerScheduler;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTimer           = (PANSC_TIMER_DESCRIPTOR_OBJECT)hTimer;

    if ( pTimerScheduler )
    {
        returnStatus =
            pTimerScheduler->RegisterTimer
                (
                    (ANSC_HANDLE)pTimerScheduler,
                    (ANSC_HANDLE)pTimer
                );
    }
    else if ( pParentContainer )
    {
        returnStatus =
            pParentContainer->RegisterTimer
                (
                    (ANSC_HANDLE)pParentContainer,
                    (ANSC_HANDLE)pTimer
                );
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoCancelTimer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTimer
            );

    description:

        This function is called to cancel a timer object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTimer
                Specifies the timer descriptor object to be canceled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoCancelTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject        = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pParentContainer = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PANSC_TIMER_SCHEDULER_OBJECT    pTimerScheduler  = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hTimerScheduler;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTimer           = (PANSC_TIMER_DESCRIPTOR_OBJECT)hTimer;

    if ( pTimerScheduler )
    {
        returnStatus =
            pTimerScheduler->CancelTimer
                (
                    (ANSC_HANDLE)pTimerScheduler,
                    (ANSC_HANDLE)pTimer
                );
    }
    else if ( pParentContainer )
    {
        returnStatus =
            pParentContainer->CancelTimer
                (
                    (ANSC_HANDLE)pParentContainer,
                    (ANSC_HANDLE)pTimer
                );
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  returnStatus;
}
