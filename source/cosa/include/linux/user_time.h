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

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang
        Ying Lou

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to linux kernel by Geng Yang
        04/03/02    adapted to linux user by Ying Lou

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
    USHORT                          Month;
    USHORT                          DayOfMonth;
    USHORT                          DayOfWeek;
    USHORT                          Hour;
    USHORT                          Minute;
    USHORT                          Second;
    USHORT                          MilliSecond;
}
USER_SYSTEM_TIME,  *PUSER_SYSTEM_TIME;

#define  USER_TIME_SOURCE_SYS                       TRUE
#define  USER_TIME_SOURCE_NTP                       FALSE

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

VOID
UserSleepInMilliSeconds(ULONG milliSecond);
ULONG
UserGetTickInSeconds();

ULONG
UserGetTickInSeconds2();

#define UserGetTickInSecondsAbs UserGetTickInSeconds2

ULONG
UserGetTickInMilliSeconds();

ULONG
UserGetTickInMilliSeconds2();

#define UserGetTickInMilliSecondsAbs UserGetTickInMilliSeconds2

void
UserGetNtpTime(time_t*  ltime);
void
UserGetSystemTime(USER_SYSTEM_TIME*  pSystemTime);
#define UserGetUtcTime UserGetSystemTime
void
UserSetSystemTime(USER_SYSTEM_TIME*  pSystemTime);
#define UserSetUtcTime UserSetSystemTime
void
UserGetLocalTime(USER_SYSTEM_TIME*  pSystemTime);
struct tm *
UserGetTimeNow(time_t *timeNow, struct tm *Tm);
void
UserSetLocalTime(USER_SYSTEM_TIME*  pSystemTime);
void
UserSetTzString(char *pTzString, ...);
LONG
UserGetTzOffset(void);
unsigned long
UserGetUtcSeconds( void );

#endif

