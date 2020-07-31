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

    module:	ansc_ssto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Simple System Time Object.

        *   AnscSstoCalendarToSecond
        *   AnscSstoLocalCalendarToSecond
        *   AnscSstoSecondToCalendar
        *   AnscSstoSecondToLocalCalendar
        *   AnscSstoGetWeekDay

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/22/01    Xuechen Yang    template
        11/25/02    Na Li           initial revision

**********************************************************************/


#include "ansc_ssto_global.h"

/*
 * define some data structure
 */
static USHORT usDaysOfYearInMonth[2][13] =
{
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoCalendarToSecond
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCalendar,
                PULONG                      pTimeStampI
            );

    description:

        This function is called to Converts Gregorian date to seconds
        since 1900-01-01 00:00:00. The year, month, DayOfMonth, Hour, 
        Minute, and Second fields of the Calendar have to be filled 
        before calling this function.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCalendar
                The input time in calendar format. Its value is for
                UTC time, which does not consist of the timezone adjustment
                and day saving time adjustment

                PULONG                      pTimeStampI
                This handle contains the 32-bit seconds since 
                1970-01-01 00:00:00, which is independent on the timeoffset

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoCalendarToSecond
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_UNIVERSAL_TIME            pCalendar         = (PANSC_UNIVERSAL_TIME)hCalendar;
    ULONG                           ulNewTime         = 0;
    ULONG                           ulLeapsToDate     = 0;
    ULONG                           ulYDay            = 0;
    ULONG                           ulLastYear        = pCalendar->Year - ANSC_SSTO_YEAR_1970;
    ULONG                           ulRealLastYear    = pCalendar->Year - 1;

    /*
    AnscTrace("AnscSstoCalendarToSecond...!\n");
    */

    /*
     * Get the days since 01/01/1970
     * notice that DayOfMonth is using 1-31 instead of 0-30. We need to reduce the number of day
     * by one for the current day of year
     */
    ulLeapsToDate   = ulRealLastYear / 4 - ulRealLastYear / 100 + ulRealLastYear / 400 -
                      (ANSC_SSTO_YEAR_1970 / 4 - ANSC_SSTO_YEAR_1970  / 100 + ANSC_SSTO_YEAR_1970  / 400 );
    ulYDay          = 
        usDaysOfYearInMonth[AnscSstoIfLeapyear(pCalendar->Year)][pCalendar->Month-1] +
        pCalendar->DayOfMonth - 1;

    ulNewTime       = ulLastYear * 365 + ulLeapsToDate + ulYDay;

    /*
     * overflow only occurs in the year 2106, so we don't handle the overflow here.
     */
    ulNewTime       = ulNewTime * ANSC_SSTO_HOURS_IN_DAY      + pCalendar->Hour;
    ulNewTime       = ulNewTime * ANSC_SSTO_MINUTES_IN_HOUR   + pCalendar->Minute;
    ulNewTime       = ulNewTime * ANSC_SSTO_SECONDS_IN_MINUTE + pCalendar->Second;


    *pTimeStampI = (ulNewTime + ANSC_SSTO_SECONDS_FROM_1900_TO_1970);

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoLocalCalendarToSecond
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCalendar,
                PULONG                      pTimeStampI
            );

    description:

        This function is called to Converts local Gregorian date to seconds
        since 1900-01-01 00:00:00. The year, month, DayOfMonth, Hour, 
        Minute, and Second fields of the Calendar have to be filled 
        before calling this function.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCalendar
                The input time in calendar format. Its value is for
                local time, which consists of the timezone adjustment
                and day saving time adjustment

                PULONG                      pTimeStampI
                This handle contains the 32-bit GMT seconds since 
                1900-01-01 00:00:00, which is independent on the timeoffset

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoLocalCalendarToSecond
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulNewTime         = 0;

    /*
    AnscTrace("AnscSstoLocalCalendarToSecond...!\n");
    */

    returnStatus =
        pMyObject->CalendarToSecond
            (
                (ANSC_HANDLE)pMyObject,
                hCalendar,
                &ulNewTime
            );


    /*
     * adjust the timeoffset
     */
     ulNewTime -= pMyObject->iTimeOffset;

    *pTimeStampI = ulNewTime;

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoSecondToCalendar
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       TimeStampI,
                ANSC_HANDLE                 hCalendar
            );

    description:

        This function is called to Converts the seconds since  
        1900-01-01 00:00:00 into Gregorian date . The calendar does
        not consist of the timezone adjustment and day saving time 
        adjustment. It is assumed that the calendar is later than
        1970-01-01 00:00:00.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       TimeStampI
                This handle contains the 32-bit seconds since 
                1900-01-01 00:00:00

                ANSC_HANDLE                 hCalendar
                The output time in calendar format. Its value is for
                GMT time

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoSecondToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    )
{
    /*
    AnscTrace("AnscSstoSecondToCalendar...!\n");
    */
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    PANSC_UNIVERSAL_TIME            pCalendar         = (PANSC_UNIVERSAL_TIME        )hCalendar;
    ULONG                           ulDay             = 0;
    ULONG                           ulSecLeft         = 0;
    ULONG                           ulLeapsToDate     = 0;
    ULONG                           ulLastYear        = 0;
    ULONG                           ulRealLastYear    = 0;
    ULONG                           ulMonth           = 0;
    USHORT*                         pusDaysInMonth    = NULL;
    USHORT                          usDayOfYear       = 0;

    /*
    AnscTrace("AnscSstoSecondToCalendar...!\n");
    */

    /*
     * adjust the second since 1900 to the second since 1970
     */
    TimeStampI -= ANSC_SSTO_SECONDS_FROM_1900_TO_1970;

    ulDay               = TimeStampI / ANSC_SSTO_SECONDS_IN_DAY;
    ulSecLeft           = TimeStampI % ANSC_SSTO_SECONDS_IN_DAY;

    pCalendar->Hour     = (UCHAR)(ulSecLeft / ANSC_SSTO_SECONDS_IN_HOUR);
    pCalendar->Minute   = (UCHAR)((ulSecLeft % ANSC_SSTO_SECONDS_IN_HOUR) / ANSC_SSTO_SECONDS_IN_MINUTE);
    pCalendar->Second   = (UCHAR)((ulSecLeft % ANSC_SSTO_SECONDS_IN_HOUR) % ANSC_SSTO_SECONDS_IN_MINUTE);

    ulLastYear          = ulDay / ANSC_SSTO_DAYS_IN_NORMAL_YEAR;
    ulRealLastYear      = ulLastYear + ANSC_SSTO_YEAR_1970 - 1;
    ulLeapsToDate       = ulRealLastYear / 4 - ulRealLastYear / 100 + ulRealLastYear / 400 -
                          (ANSC_SSTO_YEAR_1970 / 4 - ANSC_SSTO_YEAR_1970  / 100 + ANSC_SSTO_YEAR_1970  / 400 );

    
    if ( ulDay < (ulLastYear * ANSC_SSTO_DAYS_IN_NORMAL_YEAR + ulLeapsToDate) )
    {
        ulLastYear --;

        ulRealLastYear      = ulLastYear + ANSC_SSTO_YEAR_1970 - 1;
        ulLeapsToDate       = ulRealLastYear / 4 - ulRealLastYear / 100 + ulRealLastYear / 400 -
                              (ANSC_SSTO_YEAR_1970 / 4 - ANSC_SSTO_YEAR_1970  / 100 + ANSC_SSTO_YEAR_1970  / 400 );
    }
    
    usDayOfYear         = (USHORT)(ulDay - ulLastYear * ANSC_SSTO_DAYS_IN_NORMAL_YEAR - ulLeapsToDate);

    pCalendar->Year     = (USHORT)(ulLastYear + ANSC_SSTO_YEAR_1970);

    if( usDayOfYear     >= ANSC_SSTO_DAYS_IN_NORMAL_YEAR )
    {
        if( AnscSstoIfLeapyear(ulLastYear + ANSC_SSTO_YEAR_1970) && 
            (usDayOfYear > ANSC_SSTO_DAYS_IN_NORMAL_YEAR) )
        {
            pCalendar->Year ++;
            usDayOfYear = usDayOfYear - ANSC_SSTO_DAYS_IN_NORMAL_YEAR - 1;
        }
        else
        {
            pCalendar->Year ++;
            usDayOfYear = usDayOfYear - ANSC_SSTO_DAYS_IN_NORMAL_YEAR;
        }
    }

    ulDay               = usDayOfYear;
    pusDaysInMonth      = usDaysOfYearInMonth[AnscSstoIfLeapyear(pCalendar->Year)];

    for( ulMonth = 11; ulDay < (ULONG)pusDaysInMonth[ulMonth]; --ulMonth )
    {
        continue;
    }

    pCalendar->DayOfMonth   = (UCHAR)(ulDay - pusDaysInMonth[ulMonth] + 1);
    pCalendar->Month        = (UCHAR)ulMonth + 1;  /* month from 1-12 */

    /*
	 * Determine the day of week
	 */
	pMyObject->GetWeekDay( (ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCalendar );

    /* 
     * for leap time
     *
    if( TRUE )
    {
        if( (pCalendar->Second == (ANSC_SSTO_SECONDS_IN_MINUTE - 1)) &&
            (pCalendar->Minute == (ANSC_SSTO_MINUTES_IN_HOUR - 1)))
        {
            pCalendar->Second ++;
        }
    }
    */


    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoSecondToLocalCalendar
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       TimeStampI,
                ANSC_HANDLE                 hCalendar
            );

    description:

        This function is called to Convert seconds since 1970-01-01 00:00:00
        to local date. The calendar consists of the timezone adjustment
        and day saving time adjustment


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       TimeStampI
                This handle contains the 32-bit GMT seconds since 
                1900-01-01 00:00:00, which is independent on the timeoffset

                ANSC_HANDLE                 hCalendar
                The output time in calendar format. Its value is for
                local time, which consists of the timezone adjustment
                and day saving time adjustment


    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoSecondToLocalCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulNewTime         = 0;

    /*
    AnscTrace("AnscSstoSecondToLocalCalendar...!\n");
    */

    /*
     * adjust the GMT time to be the local seconds for the timeoffset
     */
    ulNewTime = TimeStampI + pMyObject->iTimeOffset;

    returnStatus =
        pMyObject->SecondToCalendar
            (
                hThisObject,
                ulNewTime,
                hCalendar
            );

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

    ANSC_STATUS
    AnscSstoGetWeekDay
        (
            ANSC_HANDLE                 hThisObject,
            ANSC_HANDLE                 hCalendar

        );

    description:

        This function is called to get the day of week according to
        other calender parameters. It sets the Wday and WNum parameters.
        Its year, month, Mday must always be filled before calling this
        function. This function can only handle years after 1970.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


                ANSC_HANDLE                 hCalendar
                This handle is time in calendar format. Its year, Yday
                should be filled; Or its year, month, Mday should be filled
                Otherwise, the error is returned

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
AnscSstoGetWeekDay
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar

    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_UNIVERSAL_TIME            pCalendar         = (PANSC_UNIVERSAL_TIME)hCalendar;
    LONG                            ulLastYear        = pCalendar->Year - ANSC_SSTO_YEAR_1970;
    LONG                            ulRealLastYear    = pCalendar->Year - 1;
    ULONG                           ulLeapsToDate     = 0;
    ULONG                           ulDayFrom1970     = 0;
    ULONG                           ulYDay            = 0;

    /*
    AnscTrace("AnscSstoGetWeekDay...!\n");
    */

    if( pCalendar->Year < ANSC_SSTO_YEAR_1970 )
    {
        return ANSC_STATUS_FAILURE;
    }

    ulLeapsToDate   = ulRealLastYear / 4 - ulRealLastYear / 100 + ulRealLastYear / 400 -
                      (ANSC_SSTO_YEAR_1970 / 4 - ANSC_SSTO_YEAR_1970 / 100 + ANSC_SSTO_YEAR_1970 / 400);

    ulYDay          = 
        usDaysOfYearInMonth[AnscSstoIfLeapyear(pCalendar->Year)][pCalendar->Month-1] +
        pCalendar->DayOfMonth - 1;

    ulDayFrom1970   = ulLastYear * 365 + ulLeapsToDate + ulYDay;


	pCalendar->DayOfWeek = (UCHAR)( (ulDayFrom1970 + ANSC_SSTO_WDAY_ON_JAN01_1970 ) 
                                    % ANSC_SSTO_DAYS_IN_WEEK);


    return returnStatus;
}


