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

    module:	user_time.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/24/01    initial revision.

**********************************************************************/


#ifndef  _USER_TIME_
#define  _USER_TIME_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 * Following data structure is used by objects to require time/date information from either the
 * underlying operationg system, or from the utility objects.
 */
typedef  struct
_USER_SYSTEM_TIME
{
    BOOL                            bDayLightSaving;
    USHORT                          Year;
    USHORT                          Month;          /* January = 1, February = 2... */
    USHORT                          DayOfMonth;
    USHORT                          DayOfWeek;      /* Sunday  = 0, Monday   = 1... */
    USHORT                          Hour;
    USHORT                          Minute;
    USHORT                          Second;
    USHORT                          MilliSecond;
}
USER_SYSTEM_TIME,  *PUSER_SYSTEM_TIME;

#define  USER_TIME_SOURCE_SYS                       TRUE
#define  USER_TIME_SOURCE_NTP                       TRUE


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  UserSleepInMilliSeconds                    Sleep
#define  UserGetTickInSeconds()                     (ULONG)(GetTickCount() / 1000)
#define  UserGetTickInSeconds2                      UserGetTickInSeconds
#define  UserGetTickInSecondsAbs()                  UserGetTickInSeconds()
#define  UserGetTickInMilliSeconds()                GetTickCount()
#define  UserGetTickInMilliSeconds2                 UserGetTickInMilliSeconds
#define  UserGetTickInMilliSecondsAbs()             UserGetTickInMilliSeconds()
#define  UserGetTickInMicroSeconds()                (UserGetTickInMilliSeconds() * 1000)

#define  UserGetUtcTime                             UserGetSystemTime
#define  UserSetUtcTime                             UserSetSystemTime

time_t time(time_t * t);

__inline  void
UserGetNtpTime(time_t*  ltime)
{
    time(ltime);
}


__inline  void
UserGetSystemTime(USER_SYSTEM_TIME*  user_system_time)
{
    SYSTEMTIME                      windows_system_time;

    GetSystemTime(&windows_system_time);

    user_system_time->bDayLightSaving = FALSE;
    user_system_time->Year            = (USHORT)windows_system_time.wYear;
    user_system_time->Month           = (USHORT)windows_system_time.wMonth;
    user_system_time->DayOfMonth      = (USHORT)windows_system_time.wDay;
    user_system_time->DayOfWeek       = (USHORT)windows_system_time.wDayOfWeek;
    user_system_time->Hour            = (USHORT)windows_system_time.wHour;
    user_system_time->Minute          = (USHORT)windows_system_time.wMinute;
    user_system_time->Second          = (USHORT)windows_system_time.wSecond;
    user_system_time->MilliSecond     = (USHORT)windows_system_time.wMilliseconds;
}


__inline  void
UserSetSystemTime(USER_SYSTEM_TIME*  user_system_time)
{
    SYSTEMTIME                      windows_system_time;

    windows_system_time.wYear         = (WORD)user_system_time->Year;
    windows_system_time.wMonth        = (WORD)user_system_time->Month;
    windows_system_time.wDay          = (WORD)user_system_time->DayOfMonth;
    windows_system_time.wDayOfWeek    = (WORD)user_system_time->DayOfWeek;
    windows_system_time.wHour         = (WORD)user_system_time->Hour;
    windows_system_time.wMinute       = (WORD)user_system_time->Minute;
    windows_system_time.wSecond       = (WORD)user_system_time->Second;
    windows_system_time.wMilliseconds = (WORD)user_system_time->MilliSecond;

    SetSystemTime(&windows_system_time);
}


__inline  void
UserGetLocalTime(USER_SYSTEM_TIME*  user_local_time)
{
    SYSTEMTIME                      windows_local_time;

    GetLocalTime(&windows_local_time);

    user_local_time->bDayLightSaving = FALSE;
    user_local_time->Year            = (USHORT)windows_local_time.wYear;
    user_local_time->Month           = (USHORT)windows_local_time.wMonth;
    user_local_time->DayOfMonth      = (USHORT)windows_local_time.wDay;
    user_local_time->DayOfWeek       = (USHORT)windows_local_time.wDayOfWeek;
    user_local_time->Hour            = (USHORT)windows_local_time.wHour;
    user_local_time->Minute          = (USHORT)windows_local_time.wMinute;
    user_local_time->Second          = (USHORT)windows_local_time.wSecond;
    user_local_time->MilliSecond     = (USHORT)windows_local_time.wMilliseconds;
}


__inline  void
UserSetLocalTime(USER_SYSTEM_TIME*  user_local_time)
{
    SYSTEMTIME                      windows_local_time;

    windows_local_time.wYear         = (WORD)user_local_time->Year;
    windows_local_time.wMonth        = (WORD)user_local_time->Month;
    windows_local_time.wDay          = (WORD)user_local_time->DayOfMonth;
    windows_local_time.wDayOfWeek    = (WORD)user_local_time->DayOfWeek;
    windows_local_time.wHour         = (WORD)user_local_time->Hour;
    windows_local_time.wMinute       = (WORD)user_local_time->Minute;
    windows_local_time.wSecond       = (WORD)user_local_time->Second;
    windows_local_time.wMilliseconds = (WORD)user_local_time->MilliSecond;

    SetLocalTime(&windows_local_time);
}


__inline  void
UserGetTickInMicroSeconds64(ULONG*  hi_part, ULONG*  lo_part)
{
    ULONG                           cur_ms    = GetTickCount();
    ULONG                           max_block = 0xFFFFFFFF / 1000;
    ULONG                           cur_block = 0;
    ULONG                           block_cnt = cur_ms / max_block + 1;
    ULONG                           k         = 0;

    *hi_part = 0;
    *lo_part = 0;

    for ( k = 0; k < block_cnt; k++ )
    {
        if ( cur_ms >= max_block )
        {
            cur_block = max_block;
        }
        else
        {
            cur_block = cur_ms;
        }

        cur_ms -= cur_block;

        cur_block *= 1000;
        *lo_part  += cur_block;

        if ( *lo_part < cur_block )
        {
            *hi_part += 1;
        }
    }
}


__inline  void
UserSetTzString(char*  tz_string)
{
    UNREFERENCED_PARAMETER(tz_string);
    return;
}


__inline  int
UserGetTzOffset()
{
    return  0;
}


__inline  ULONG
UserGetUtcSeconds()
{
    return  0;
}


/***********************************************************
                        PROFILING
***********************************************************/

/*
 *  We are not really going to rely on this profiling mechanism
 *  as Visual Studio has its profiling tool. It is here for the
 *  sake of completeness.
 */
#define  UserProfilingPccFactor                     1

void
UserProfilingStartPcc
    (
        void
    );

void
UserProfilingStopPcc
    (
        void
    );

ULONG
UserProfilingReadPcc
    (
        void
    );


#endif

