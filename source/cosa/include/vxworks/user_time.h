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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/24/01    initial revision.

**********************************************************************/


#ifndef  _USER_TIME_
#define  _USER_TIME_


/***********************************************************
         DEFINITIONS SUPPOSEDLY TO BE IN MAKEFILE
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

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  AL_TIME_STAMP_MILI_NUMBER_FOR_SECOND       1000000

VOID
UserSleepInMilliSeconds
    (
        ULONG                       ulMLseconds
    );

ULONG
UserGetTickInSeconds
    (
        VOID
    );

ULONG
UserGetTickInMilliSeconds
    (
        VOID
    );

ULONG
UserGetTickInMicroSeconds
    (
        VOID
    );

void
UserGetTickInMicroSeconds64
    (
        ULONG*                      hi_part, 
        ULONG*                      lo_part
    );

void
UserGetSystemTime
    (
        PUSER_SYSTEM_TIME           pSystemTime
    );

void
UserSetSystemTime
    (
        USER_SYSTEM_TIME*           user_system_time
    );

#define  UserGetTickInSecondsAbs()          UserGetTickInSeconds()
#define  UserGetTickInMilliSecondsAbs()     UserGetTickInMilliSeconds()
#define  UserGetLocalTime                   UserGetSystemTime
#define  UserSetLocalTime                   UserSetSystemTime
#define  UserSetTzString                    0;assert(FALSE);
#define  UserGetTzOffset                    0;assert(FALSE);
#define  UserGetUtcSeconds                  0;assert(FALSE);

#endif
