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

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelGetSystemTime
        *   KernelSetSystemTime
        *   KernelGetLocalTime
        *   KernelSetLocalTime

    ---------------------------------------------------------------

    environment:

        eCos 1.3.1

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        05/01/06    initial revision.

**********************************************************************/


#include "kernel_base.h"
#include "kernel_time.h"


void
KernelGetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          timeNow;
    struct tm                       Tm      = {0};
    struct tm*                      ptm     = NULL;

    time(&timeNow);
    ptm = gmtime_r(&timeNow,&Tm);

    pSystemTime->Year           = ptm->tm_year + 1900;
    pSystemTime->Month          = ptm->tm_mon + 1;
    pSystemTime->DayOfMonth     = ptm->tm_mday;
    pSystemTime->DayOfWeek      = ptm->tm_wday;
    pSystemTime->Hour           = ptm->tm_hour;
    pSystemTime->Minute         = ptm->tm_min;
    pSystemTime->Second         = ptm->tm_sec;
    pSystemTime->bDayLightSaving= ptm->tm_isdst;

    pSystemTime->MilliSecond    = 0;

    return;
}

void
KernelSetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          newTime     = 0;
    struct tm                       newtm;

    Cyg_libc_time_dst               DstState;
    time_t*                         StdOffset   = 0;
    time_t*                         DstOffset   = 0;

    newtm.tm_year   = pSystemTime->Year           - 1900;
    newtm.tm_mon    = pSystemTime->Month          - 1;
    newtm.tm_mday   = pSystemTime->DayOfMonth;
    newtm.tm_wday   = pSystemTime->DayOfWeek;
    newtm.tm_hour   = pSystemTime->Hour;
    newtm.tm_min    = pSystemTime->Minute;
    newtm.tm_sec    = pSystemTime->Second;

    DstState        = cyg_libc_time_getzoneoffsets(&StdOffset, &DstOffset);
    newtm.tm_isdst  = (int)DstState;

    newTime = mktime(&newtm);

    /*
     *  mktime is supposed to treat the input as local time, so we should offset it with the timezone.
     *  However, from eCos source code, mktime didn't anything to convert the input as local time, that
     *  is, mktime is effectively same as timegm!!!
     *
    if ( DstState == CYG_LIBC_TIME_DSTON )
    {
        newTime += StdOffset;
        newTime += DstOffset;
    }
    else
    {
        newTime += StdOffset;
    }
     */

    if ( !cyg_libc_time_settime(newTime) )
    {
        KernelTrace("KernelSetSystemTime -- failed to set system time!!!\n");
    }

    return;
}


void
KernelGetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          timeNow;
    struct tm                       Tm      = {0};
    struct tm*                      ptm     = NULL;

    time(&timeNow);
    ptm = localtime_r(&timeNow,&Tm);

    pSystemTime->Year           = ptm->tm_year + 1900;
    pSystemTime->Month          = ptm->tm_mon + 1;
    pSystemTime->DayOfMonth     = ptm->tm_mday;
    pSystemTime->DayOfWeek      = ptm->tm_wday;
    pSystemTime->Hour           = ptm->tm_hour;
    pSystemTime->Minute         = ptm->tm_min;
    pSystemTime->Second         = ptm->tm_sec;
    pSystemTime->bDayLightSaving= ptm->tm_isdst;

    pSystemTime->MilliSecond    = 0;

    return;
}


void
KernelSetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    )
{
    time_t                          newTime     = 0;
    struct tm                       newtm;

    newtm.tm_year   = pSystemTime->Year           - 1900;
    newtm.tm_mon    = pSystemTime->Month          - 1;
    newtm.tm_mday   = pSystemTime->DayOfMonth;
    newtm.tm_wday   = pSystemTime->DayOfWeek;
    newtm.tm_hour   = pSystemTime->Hour;
    newtm.tm_min    = pSystemTime->Minute;
    newtm.tm_sec    = pSystemTime->Second;
    newtm.tm_isdst  = -1;

    /*
     *  mktime is supposed to treat the input as local time, however, from eCos source code,
     *  mktime didn't anything to convert the input as local time!!! This will lead to
     *  inaccurate result!
     */
    newTime = mktime(&newtm);

    if ( !cyg_libc_time_settime(newTime) )
    {
        KernelTrace("KernelSetLocalTime -- failed to set system time!!!\n");
    }

    return;
}

