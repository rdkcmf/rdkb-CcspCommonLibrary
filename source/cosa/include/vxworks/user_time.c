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

    module:	user_time.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   UserSleepInMilliSeconds
        *   UserGetTickInSeconds
        *   UserGetTickInMilliSeconds
        *   UserGetSystemTime
    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#include "user_base.h"
#include "user_time.h"


VOID
UserSleepInMilliSeconds
    (
        ULONG                       ulMLseconds
    )
{
    struct  timespec                sleepTime = {0};

    sleepTime.tv_sec    = ulMLseconds / 1000;
    sleepTime.tv_nsec   = (ulMLseconds % 1000) * (1000000000L / 1000);

    nanosleep(&sleepTime, NULL);
}


ULONG
UserGetTickInSeconds
    (
        VOID
    )
{
    ULONG                       TicksInOneSecond    = sysClkRateGet();
    ULONG                       Ticks               = tickGet();

    return  Ticks / TicksInOneSecond;
}


ULONG
UserGetTickInMilliSeconds
    (
        VOID
    )
{
    ULONG                       TicksInOneSecond    = sysClkRateGet();
    ULONG                       Ticks               = tickGet();

    return  (Ticks / TicksInOneSecond) * 1000 + ((Ticks % TicksInOneSecond) * 1000) / TicksInOneSecond;
}

ULONG
UserGetTickInMicroSeconds
    (
        VOID
    )
{
    ULONG                       TicksInOneSecond    = sysClkRateGet();
    ULONG                       Ticks               = tickGet();

    return  (Ticks / TicksInOneSecond) * 1000000 + ((Ticks % TicksInOneSecond) * 1000000) / TicksInOneSecond;
}

void
UserGetTickInMicroSeconds64
    (
        ULONG*                      hi_part, 
        ULONG*                      lo_part
    )
{
    ULONG                       TicksInOneSecond    = sysClkRateGet();
    ULONG                       Ticks               = tickGet();

    *hi_part = 0;
    *lo_part = (Ticks / TicksInOneSecond) * 1000000 + ((Ticks % TicksInOneSecond) * 1000000) / TicksInOneSecond;
}

void
UserGetSystemTime
    (
        PUSER_SYSTEM_TIME           pSystemTime
    )
{
    time_t                          currSeconds;
    struct tm                       currTime;

    struct  timespec                elapseTime = {0};

    time(&currSeconds);

    localtime_r(&currSeconds, &currTime);

    if ( currTime.tm_isdst < 0 )
    {
        pSystemTime->bDayLightSaving    = FALSE;
    }
    else if ( currTime.tm_isdst == 0 )
    {
        pSystemTime->bDayLightSaving    = FALSE;
    }
    else
    {
        pSystemTime->bDayLightSaving    = TRUE;
    }

    pSystemTime->Year                   = currTime.tm_year + 1900;
    pSystemTime->Month                  = currTime.tm_mon  + 1;
    pSystemTime->DayOfMonth             = currTime.tm_mday;
    pSystemTime->DayOfWeek              = currTime.tm_wday;
    pSystemTime->Hour                   = currTime.tm_hour;
    pSystemTime->Minute                 = currTime.tm_min;
    pSystemTime->Second                 = currTime.tm_sec;

    /*
     *  get the milli-second part
     */
    clock_gettime(CLOCK_REALTIME, &elapseTime);

    pSystemTime->MilliSecond            = (elapseTime.tv_nsec / (1000000000L / 1000)) % 1000;

    return;
}

void
UserSetSystemTime
    (
        USER_SYSTEM_TIME*  user_system_time
    )
{
    printf("UserSetSystemTime is not implemented yet!\n");
    assert(FALSE);
    return;
}
