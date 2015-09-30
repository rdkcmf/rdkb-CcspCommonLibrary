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

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

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


#ifndef  _KERNEL_TIME_
#define  _KERNEL_TIME_


/***********************************************************
         DEFINITIONS SUPPOSEDLY TO BE IN MAKEFILE
***********************************************************/

#define  _ANSC_SIMPLE_TIMEZONE

#define  KERNEL_TIME_SOURCE_SYS                       TRUE
/*#define  KERNEL_TIME_SOURCE_NTP                       TRUE*/

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

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  KernelSleepInMilliSeconds(ulMs)            cyg_thread_delay(ulMs / KERNEL_MILLISECONDS_PER_TICK)

#define  KernelGetTickInSeconds()                   cyg_current_time() / ( 1000 / KERNEL_MILLISECONDS_PER_TICK)

#define  KernelGetTickInMilliSeconds()              cyg_current_time() * KERNEL_MILLISECONDS_PER_TICK

#define  KernelGetTickInMicroSeconds()              cyg_current_time() * KERNEL_MILLISECONDS_PER_TICK * 1000

/*
 *
    void
    KernelGetTickInMicroSeconds64
        (
            ULONG*                      hi_part, 
            ULONG*                      lo_part
        );
 *
 */
#define  KernelGetTickInMicroSeconds64(pHiWord, pLowWord)       \
            {*pHiWord = 0; *pLowWord = KernelGetTickInMicroSeconds(); }

void
KernelGetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );

void
KernelSetSystemTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );

#define  KernelGetUtcTime                           KernelGetSystemTime
#define  KernelSetUtcTime                           KernelSetSystemTime

void
KernelGetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );

void
KernelSetLocalTime
    (
        PKERNEL_SYSTEM_TIME         pSystemTime
    );


#define  KernelGetTickInSecondsAbs()                KernelGetTickInSeconds()
#define  KernelGetTickInMilliSecondsAbs()           KernelGetTickInMilliSeconds()

#define  KernelSetTzString                          0;assert(FALSE);
#define  KernelGetTzOffset                          0;assert(FALSE);
#define  KernelGetUtcSeconds                        0;assert(FALSE);


#endif

