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

    module:	ansc_ssto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Simple System Time Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        11/25/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SSTO_INTERFACE_
#define  _ANSC_SSTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_sts.h"
#include "ansc_co_interface.h"

/***********************************************************
     PLATFORM INDEPENDENT SYSTEM CLOCK OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SSTO_YEAR_1970                        1970
#define  ANSC_SSTO_SECONDS_FROM_1900_TO_1970        2208988800LL

#define  ANSC_SSTO_SECONDS_IN_DAY                   86400
#define  ANSC_SSTO_SECONDS_IN_HOUR                  3600
#define	 ANSC_SSTO_HOURS_IN_DAY	                    24
#define	 ANSC_SSTO_MINUTES_IN_HOUR	                60
#define	 ANSC_SSTO_SECONDS_IN_MINUTE                60
#define	 ANSC_SSTO_DAYS_IN_WEEK                     7
#define	 ANSC_SSTO_DAYS_IN_NORMAL_YEAR              365
#define  ANSC_SSTO_WDAY_ON_JAN01_1970               4


#define	AnscSstoIfLeapyear(year)		                \
            ( ( (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0) )? 1:0 )

#define	AnscSstoDaysInYear(year) 	                (AnscSstoIfLeapyear(year) ? 366 : 365)

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SSTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SSTO_OPERATION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_SSTO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSTO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       value
    );

typedef  INT
(*PFN_SSTO_GET_INT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSTO_SET_INT)
    (
        ANSC_HANDLE                 hThisObject,
        INT                         value
    );


typedef  ANSC_STATUS
(*PFN_SSTO_SEC_TO_CAL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Second,
        ANSC_HANDLE                 hUniversalTime
    );

typedef  ANSC_STATUS
(*PFN_SSTO_CAL_TO_SEC)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUniversalTime,
        PULONG                      pSecond
    );

typedef  ANSC_STATUS
(*PFN_SSTO_GET_WEEKDAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUniversalTime
    );

/*
 * this object provides the current calendar time once it is intialized by the current time.
 */
#define  ANSC_SIMPLE_SYS_TIME_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ULONG                           LastSecond;                                             \
    ULONG                           LastTick;                                               \
    INT                             iTimeOffset;                                            \
    ANSC_HANDLE                     hStsIf;                                                 \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_SSTO_GET_CONTEXT            GetStsIf;                                               \
    PFN_SSTO_OPERATION              Reset;                                                  \
    PFN_SSTO_GET_ULONG              GetLastSecond;                                          \
    PFN_SSTO_GET_ULONG              GetLastTick;                                            \
    PFN_SSTO_GET_INT                GetiTimeOffset;                                         \
    PFN_SSTO_SET_INT                SetiTimeOffset;                                         \
    PFN_SSTO_OPERATION              AcquireAccess;                                          \
    PFN_SSTO_OPERATION              ReleaseAccess;                                          \
    PFN_SSTO_GET_ULONG              GetCurrSecond;                                          \
                                                                                            \
    PFN_SSTO_SEC_TO_CAL             SecondToCalendar;                                       \
    PFN_SSTO_SEC_TO_CAL             SecondToLocalCalendar;                                  \
    PFN_SSTO_CAL_TO_SEC             CalendarToSecond;                                       \
    PFN_SSTO_GET_WEEKDAY            GetWeekDay;                                             \
    PFN_SSTO_CAL_TO_SEC             LocalCalendarToSecond;                                  \
                                                                                            \
    PFN_ANSCSTSIF_GET_TIME          StsGetLocalTime;                                        \
    PFN_ANSCSTSIF_GET_SEC           StsGetCurSeconds;                                       \
    PFN_ANSCSTSIF_ADJUST1           StsAdjustClock1;                                        \
    PFN_ANSCSTSIF_ADJUST2           StsAdjustClock2;                                        \
    PFN_ANSCSTSIF_SEC_TO_CAL        StsSecondsToCalendar;                                   \
    PFN_ANSCSTSIF_CAL_TO_SEC        StsCalendarToSeconds;                                   \
    PFN_ANSCSTSIF_TIC_TO_CAL        StsSysTickToCalendar;                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLE_SYS_TIME_OBJECT
{
    ANSC_SIMPLE_SYS_TIME_CLASS_CONTENT
}
ANSC_SIMPLE_SYS_TIME_OBJECT,  *PANSC_SIMPLE_SYS_TIME_OBJECT;

#define  ACCESS_ANSC_SIMPLE_SYS_TIME_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_SIMPLE_SYS_TIME_OBJECT, Linkage)


#endif
