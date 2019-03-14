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

    module:	ansc_wco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Wall Clock Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        11/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_WCO_INTERFACE_
#define  _ANSC_WCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
     PLATFORM INDEPENDENT WALL CLOCK OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_WALL_CLOCK_TYPE_SYSTEM                1
#define  ANSC_WALL_CLOCK_TYPE_NTP                   2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_WCO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

typedef  USHORT
(*PFN_WCO_GET_YEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_MONTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_DAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_HOUR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_MINUTE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_SECOND)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_WCO_GET_MS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_SET_DEFAULT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_GET_DST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDlsBeginTime,
        ANSC_HANDLE                 hDlsEndTime,
        PLONG                       plSeconds
    );

typedef  ANSC_STATUS
(*PFN_WCO_SET_DST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDlsBeginTime,
        ANSC_HANDLE                 hDlsEndTime,
        LONG                        seconds
    );

typedef  LONG
(*PFN_WCO_GET_TIME_ZONE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_SET_TIME_ZONE)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        time_zone
    );

typedef  ANSC_STATUS
(*PFN_WCO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WCO_SYNCHRONIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_WCO_GET_UPTIME)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * During the service development process, we constantly find ourselves in a situation where
 * absolute time/date has to be known in order to perform a operation. This Wall Clock Object
 * encapsulates most standard clock-related operations. This object SHOULD be considered as
 * a base object, and may be implemented differently on different operating systems. If the
 * underlying operating system already supports the time/date operation, we may inherit the
 * functions instead of implementing our own; otherwise, we have to implement NTP (Network Time
 * Protocol) based Wall Clock.
 */
#define  ANSC_WALL_CLOCK_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_UNIVERSAL_TIME             ClockTime;                                              \
    ANSC_UNIVERSAL_TIME             DlsBeginTime;                                           \
    ANSC_UNIVERSAL_TIME             DlsEndTime;                                             \
    LONG                            DlsInSeconds;                                           \
    LONG                            TimeZone;                                               \
    BOOL                            bStarted;                                               \
                                                                                            \
    PFN_WCO_GET_TIME                GetClockTime;                                           \
    PFN_WCO_GET_TIME                GetLocalTime;                                           \
    PFN_WCO_GET_TIME                GetUtcTime;                                             \
                                                                                            \
    PFN_WCO_GET_YEAR                GetCurrentYear;                                         \
    PFN_WCO_GET_MONTH               GetCurrentMonth;                                        \
    PFN_WCO_GET_DAY                 GetCurrentDay;                                          \
    PFN_WCO_GET_DAY                 GetCurrentDayOfMonth;                                   \
    PFN_WCO_GET_DAY                 GetCurrentDayOfWeek;                                    \
    PFN_WCO_GET_HOUR                GetCurrentHour;                                         \
    PFN_WCO_GET_MINUTE              GetCurrentMinute;                                       \
    PFN_WCO_GET_SECOND              GetCurrentSecond;                                       \
    PFN_WCO_GET_MS                  GetCurrentMilliSecond;                                  \
                                                                                            \
    PFN_WCO_SET_DEFAULT             SetDefaultParams;                                       \
    PFN_WCO_GET_DST                 GetDayLightSaving;                                      \
    PFN_WCO_SET_DST                 SetDayLightSaving;                                      \
    PFN_WCO_GET_TIME_ZONE           GetTimeZone;                                            \
    PFN_WCO_SET_TIME_ZONE           SetTimeZone;                                            \
                                                                                            \
    PFN_WCO_START                   Start;                                                  \
    PFN_WCO_STOP                    Stop;                                                   \
    PFN_WCO_SYNCHRONIZE             Synchronize;                                            \
                                                                                            \
    PFN_WCO_GET_UPTIME              GetUpTimeInMilliSeconds;                                \
    PFN_WCO_GET_UPTIME              GetUpTimeInSeconds;                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_WALL_CLOCK_OBJECT
{
    ANSC_WALL_CLOCK_CLASS_CONTENT
}
ANSC_WALL_CLOCK_OBJECT,  *PANSC_WALL_CLOCK_OBJECT;

#define  ACCESS_ANSC_WALL_CLOCK_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_WALL_CLOCK_OBJECT, Linkage)


#endif
