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

        01/24/01    initial revision.
		10/15/01	pSOS port

**********************************************************************/


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"


void
KernelSleepInMilliSeconds
    (
        ULONG                       ulMilliSeconds
    )
{
    ULONG                           ticks;

    ticks = ulMilliSeconds * KC_TICKS2SEC / 1000;

    tm_wkafter(ticks);

    return;
}


ULONG
KernelGetTickInSeconds
    (
        VOID
    )
{
    ULONG                           sysTicksHigh;
    ULONG                           sysTicksLow;
    unsigned long long int          sysTicks;
    ULONG                           seconds;

    tm_getticks(&sysTicksHigh, &sysTicksLow);

    sysTicks = ((unsigned long long int)sysTicksHigh) << 32 + sysTicksLow;

    seconds  = sysTicks / KC_TICKS2SEC;

    return  seconds;
}


ULONG
KernelGetTickInMillSeconds
    (
        VOID
    )
{
    ULONG                           sysTicksHigh;
    ULONG                           sysTicksLow;
    unsigned long long int          sysTicks;
    ULONG                           mSeconds;

    tm_getticks(&sysTicksHigh, &sysTicksLow);

    sysTicks = ((unsigned long long int)sysTicksHigh) << 32 + sysTicksLow;

    mSeconds  = sysTicks * 1000 / KC_TICKS2SEC;

    return  mSeconds;
}
