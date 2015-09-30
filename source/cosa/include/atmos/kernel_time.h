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

        This wrapper file defines some basic data types and
        structures on a particular platform.

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


#ifndef  _KERNEL_TIME_
#define  _KERNEL_TIME_


/***********************************************************
              DEFINITIONS FOR CONFIGURATION
***********************************************************/

#define  USER_TIME_SOURCE_SYS                       TRUE
/*#define  USER_TIME_SOURCE_NTP                       TRUE*/

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 * Following data structure is used by objects to require time/date information from either the
 * underlying operationg system, or from the utility objects.
 */
typedef  struct
_KERNEL_SYSTEM_TIME
{
    BOOL                            bDayLightSaving;
    USHORT                          Year;
    USHORT                          Month;
    USHORT                          DayOfMonth;
    USHORT                          DayOfWeek;
    USHORT                          Hour;
    USHORT                          Minute;
    USHORT                          Second;
    USHORT                          MilliSecond;
}
KERNEL_SYSTEM_TIME,  *PKERNEL_SYSTEM_TIME;

#define  KERNEL_TIME_SOURCE_SYS                     TRUE
#define  KERNEL_TIME_SOURCE_NTP                     FALSE

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
void
KernelSleepInMilliSeconds
    (
        ULONG                       ulMilliSeconds
    );
 */
#define  KernelSleepInMilliSeconds(ms)              timer_ms_wait(ms)


/*
ULONG
KernelGetTickInSeconds
    (
        VOID
    );
 */
#define  KernelGetTickInSeconds()                   ((ULONG)atmos_uptime())

ULONG
KernelGetTickInMilliSeconds
    (
        VOID
    );

ULONG
KernelGetTickInMicroSeconds
    (
        VOID
    );

void
KernelGetTickInMicroSeconds64
    (
        ULONG*                      hi_part,
        ULONG*                      lo_part
    );

void
KernelGetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );

void
KernelGetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );

/*
 *  before ANSC provides GetLocalTime, we patch it here.
 */
#define  AnscGetLocalTime           KernelGetLocalTime


#endif
