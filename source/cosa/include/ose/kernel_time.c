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

    module:	kernel_time.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines:

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

#include "ansc_time.h"

BOOL
KernelGetUniversalTime
    (
        KERNEL_HANDLE               hTime
    )
{
    PANSC_UNIVERSAL_TIME            pUnivTime   = (PANSC_UNIVERSAL_TIME)hTime;
    time_t                          currSeconds;
    ULONG                           milliSeconds;
    struct tm                       currTime;

    time(&currSeconds);
    milliSeconds = KernelGetTickInMilliSeconds() % 1000;

    currTime = *localtime((time_t *)&currSeconds);

    pUnivTime->bDayLightSaving      = FALSE;
    pUnivTime->Year                 = currTime.tm_year + 1900;
    pUnivTime->Month                = currTime.tm_mon;
    pUnivTime->DayOfMonth           = currTime.tm_mday;
    pUnivTime->DayOfWeek            = currTime.tm_wday;
    pUnivTime->Hour                 = currTime.tm_hour;
    pUnivTime->Minute               = currTime.tm_min;
    pUnivTime->Second               = currTime.tm_sec;
    pUnivTime->MilliSecond          = milliSeconds;

    return  TRUE;
}
