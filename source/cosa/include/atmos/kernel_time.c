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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines:

        *   KernelGetTickInMilliSeconds
        *   KernelGetTickInMicroSeconds
        *   KernelGetTickInMicroSeconds64
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


ULONG
KernelGetTickInMicroSeconds
    (
        VOID
    )
{
    BITS                            critLevel;
    ULONG                           ulValue;

    critLevel = atmos_startcritical();

    ulValue = (ULONG)atmos_uptime() * 1000000L + (ULONG)atmos_timer_usec();

    atmos_endcritical(critLevel);

    return  ulValue;
}


static   const ULONG
g32BitLowMask[32] =
{
    0xFFFFFFFF, 0x7FFFFFFF, 0x3FFFFFFF, 0x1FFFFFFF, 0x0FFFFFFF, 0x07FFFFFF, 0x03FFFFFF, 0x01FFFFFF,
    0x00FFFFFF, 0x007FFFFF, 0x003FFFFF, 0x001FFFFF, 0x000FFFFF, 0x0007FFFF, 0x0003FFFF, 0x0001FFFF,
    0x0000FFFF, 0x00007FFF, 0x00003FFF, 0x00001FFF, 0x00000FFF, 0x000007FF, 0x000003FF, 0x000001FF,
    0x000000FF, 0x0000007F, 0x0000003F, 0x0000001F, 0x0000000F, 0x00000007, 0x00000003, 0x00000001
};

static   const ULONG
g32BitHighMask[32] =
{
    0x00000000, 0x80000000, 0xC0000000, 0xE0000000, 0xF0000000, 0xF8000000, 0xFC000000, 0xFE000000,
    0xFF000000, 0xFF800000, 0xFFC00000, 0xFFE00000, 0xFFF00000, 0xFFF80000, 0xFFFC0000, 0xFFFE0000,
    0xFFFF0000, 0xFFFF8000, 0xFFFFC000, 0xFFFFE000, 0xFFFFF000, 0xFFFFF800, 0xFFFFFC00, 0xFFFFFE00,
    0xFFFFFF00, 0xFFFFFF80, 0xFFFFFFC0, 0xFFFFFFE0, 0xFFFFFFF0, 0xFFFFFFF8, 0xFFFFFFFC, 0xFFFFFFFE
};

static   UCHAR                      gOpBit[]    = {6, 9, 14, 16, 17, 18, 19};

void
KernelGetTickInMicroSeconds64
    (
        ULONG*                      hi_part,
        ULONG*                      lo_part
    )
{
    BITS                            critLevel;
    ULONG                           ulSec;
    ULONG                           uluSec;

    ULONG                           ulHigh      = 0;
    ULONG                           ulLow       = 0;
    ULONG                           ulLowDelta  = 0;

#if 1

    static  ULONG                   ulPrevHigh  = 0;
    static  ULONG                   ulPrevLow   = 0;

    critLevel   = atmos_startcritical();

    /*
     *  the hardware timer frequency is known to be 8 MHz.
     */
    uluSec = as_TimerHardware();
    uluSec = uluSec >> 3;

    if ( uluSec >= (ulPrevLow & 0x1FFFFFFF) )
    {
        ulPrevLow  &= 0xE0000000;
        ulPrevLow  |= uluSec;
    }
    else
    {
        /* overflown */
        ulLow       = ulPrevLow;

        ulPrevLow  &= 0xE0000000;
        ulPrevLow  += 0x20000000 + uluSec;

        if ( ulPrevLow < ulLow )
        {
            ulPrevHigh ++;
        }
    }

    *hi_part    = ulPrevHigh;
    *lo_part    = ulPrevLow;

    atmos_endcritical(critLevel);

    return;

#else

    ULONG                           ulIndex     = 0;
    UCHAR                           ucCurrBit   = 0;

    critLevel   = atmos_startcritical();

    ulSec       = atmos_uptime();
    uluSec      = atmos_timer_usec();

    atmos_endcritical(critLevel);

    /*
     *  The maximum value for 32-bit is 0xFFFFFFFF = 4,294,967,295.
     *
     *  Time = ulSec * 1,000,000 + uluSec
     *
     *  We do not want to introduce LONGLONG type, which requires floating point support.
     *  So we have to do 32 Bit * 32 Bit = 64 Bit by ourselves:
     *
     *  1,000,000
     *      = 0xF4240
     *      = 1111, 0100, 0010, 0100, 0000
     *        9876  5432  1098  7654  3210
     *        1111  1111  11
     *        ^^^^   ^      ^    ^
     *
     *  Time = ulSec * Bit-1 + ulSec * Bit-2 + ... + ulSec * Bit-32 + uluSec
     *
     *  For each ulSec * Bit-n,
     *
     *      ulLow  += ulSec & (Bit-(32-n-1) | ... | Bit-0) << n
     *      ulHigh += ulSec & (Bit-31 | ... | Bit-(32-n))  >> (32-n) + carry from ulLow
     *
     */
    for ( ulIndex = 0; ulIndex < sizeof(gOpBit)/sizeof(gOpBit[0]); ulIndex++ )
    {
        ucCurrBit   = gOpBit[ulIndex];

        ulHigh     += (ulSec & g32BitHighMask[ucCurrBit]) >> (32-ucCurrBit);
        ulLowDelta  = (ulSec & g32BitLowMask[ucCurrBit])  << ucCurrBit;

        ulLow       += ulLowDelta;

        if ( ulLow < ulLowDelta )
        {
            ulHigh ++;
        }
    }

    /* last part -- add uluSec */
    ulLow += uluSec;

    if ( ulLow < uluSec )
    {
        ulHigh ++;
    }

    *hi_part    = ulHigh;
    *lo_part    = ulLow;

    return;

#endif

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

