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

    module:	kernel_time.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines:

        *   KernelGetTickInMilliSeconds
        *   KernelGetSystemTime
        *   KernelGetLocalTime

    ---------------------------------------------------------------

    environment:

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/24/01    initial revision.
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

#include "quantum/as_timer.h"


ULONG
KernelGetTickInMilliSeconds
    (
        VOID
    )
{
    BITS                            critLevel;
    ULONG                           ulValue;

    critLevel = atmos_startcritical();

    ulValue = (ULONG)atmos_uptime() * 1000 + (ULONG)atmos_timer_usec() / (1000000L / 1000);

    atmos_endcritical(critLevel);

    return  ulValue;
}

void
KernelGetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          currSeconds;
    ULONG                           milliSeconds;
    struct tm                       currTime;

    BITS                            crit;

    crit = atmos_startcritical();

    time(&currSeconds);
    milliSeconds = (atmos_timer_usec() / (1000000L / 1000)) % 1000;

    atmos_endcritical(crit);

    _gmtime_r((time_t *)&currSeconds, &currTime);

    pSystemTime->bDayLightSaving    = FALSE;
    pSystemTime->Year               = currTime.tm_year + 1900;
    pSystemTime->Month              = currTime.tm_mon  + 1;
    pSystemTime->DayOfMonth         = currTime.tm_mday;
    pSystemTime->DayOfWeek          = currTime.tm_wday;
    pSystemTime->Hour               = currTime.tm_hour;
    pSystemTime->Minute             = currTime.tm_min;
    pSystemTime->Second             = currTime.tm_sec;
    pSystemTime->MilliSecond        = milliSeconds;

    return;
}


void
KernelGetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          currSeconds;
    ULONG                           milliSeconds;
    struct tm                       currTime;

    BITS                            crit;

    crit = atmos_startcritical();

    time(&currSeconds);
    milliSeconds = (atmos_timer_usec() / (1000000L / 1000)) % 1000;

    atmos_endcritical(crit);

    _localtime_r((time_t *)&currSeconds, &currTime);

    pSystemTime->bDayLightSaving    = FALSE;
    pSystemTime->Year               = currTime.tm_year + 1900;
    pSystemTime->Month              = currTime.tm_mon  + 1;
    pSystemTime->DayOfMonth         = currTime.tm_mday;
    pSystemTime->DayOfWeek          = currTime.tm_wday;
    pSystemTime->Hour               = currTime.tm_hour;
    pSystemTime->Minute             = currTime.tm_min;
    pSystemTime->Second             = currTime.tm_sec;
    pSystemTime->MilliSecond        = milliSeconds;

    return;
}

