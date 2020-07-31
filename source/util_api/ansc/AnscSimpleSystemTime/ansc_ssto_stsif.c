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

        *   AnscSstoStsGetLocalTime
        *   AnscSstoStsGetCurSeconds
        *   AnscSstoStsGetUTCTime 
        *   AnscSstoStsAdjustClock1
        *   AnscSstoStsAdjustClock2
        *   AnscSstoStsSecondsToCalendar
        *   AnscSstoStsCalendarToSeconds
        *   AnscSstoStsSysTickToCalendar
        *   AnscSstoStsGetTimeOffset
        *   AnscSstoStsSetTimeOffset

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


/**********************************************************************

    caller:     owner of this object

    prototype:

    ANSC_STATUS
    AnscSstoStsGetLocalTime
        (
            ANSC_HANDLE                 hThisObject,
            ANSC_HANDLE                 hCalendar
        );

    description:

        This function is called to get the local current calendar time. It first get
        the current seconds since 01/01/1900, then consider the timezone offset,
        and finally convers it into the calendar. 


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCalendar
                Specifies the handle of the time structure to hold
                the current calendar time.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsGetLocalTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulTimeI           = 0;

    /*
    AnscTrace("AnscSstoStsGetLocalTime...!\n");
    */
    AnscZeroMemory(hCalendar, sizeof(ANSC_UNIVERSAL_TIME));


    /*
     * get current seconds, it starts from 00:00 1 Jan 1900 GMT 
     * instead of from 00:00  1 Jan 1970 GMT 
     * which corresponds to 2,208,988,800 seconds from 
     * midnight on January first 1900
     */
    ulTimeI = 
        pMyObject->GetCurrSecond( hThisObject);

    /*
     * take into account of the offset between the configured calendar time and the calculated
     * calendar time. 
     */
    ulTimeI += pMyObject->iTimeOffset;

    returnStatus =
        pMyObject->SecondToCalendar
            ( 
                hThisObject,
                ulTimeI,
                hCalendar
            );

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

    ULONG
    AnscSstoStsGetCurSeconds
        (
            ANSC_HANDLE                 hThisObject
        );

    description:

        This function is called to get the number of seconds since
        the midnight of 01/01/1900.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of seconds.

**********************************************************************/

ULONG
AnscSstoStsGetCurSeconds
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    return  pMyObject->GetCurrSecond((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

    ANSC_STATUS
    AnscSstoStsGetUTCTime
        (
            ANSC_HANDLE                 hThisObject,
            ANSC_HANDLE                 hCalendar
        );

    description:

        This function is called to get the UTC current calendar time. It first get
        the current seconds since 01/01/1900, and then convers it into the calendar. 
        Its value does not depend on the local timezone.



    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCalendar
                Specifies the handle of the time structure to hold
                the current calendar time.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsGetUTCTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulTimeI           = 0;

    /*
    AnscTrace("AnscSstoStsGetUTCTime...!\n");
    */
    AnscZeroMemory(hCalendar, sizeof(ANSC_UNIVERSAL_TIME));


    /*
     * get current seconds, it starts from 00:00 1 Jan 1900 GMT 
     * instead of from 00:00  1 Jan 1970 GMT 
     * which corresponds to 2,208,988,800 seconds from 
     * midnight on January first 1900
     */
    ulTimeI = 
        pMyObject->GetCurrSecond( hThisObject);

    returnStatus =
        pMyObject->SecondToCalendar
            ( 
                hThisObject,
                ulTimeI,
                hCalendar
            );

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsAdjustClock1
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds,
                int                         iTimeOffset
            );

    description:

        This function is called to set the current time. Later on, the
        object can derive the time by the ticks passing by.



    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the UTC seconds since 01/01/1900. It is 
                independent on the timezone and daylight saving.

                int                         iTimeOffset
                Specifies the timeoffset in seconds of this location
                from the GMT. It includes the effect of both timezone
                and daylight saving.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsAdjustClock1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds,
        int                         iTimeOffset
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    
    pMyObject->AcquireAccess(hThisObject);

    pMyObject->LastTick    = AnscGetTickInSeconds();
    pMyObject->LastSecond  = ulSeconds;
    pMyObject->iTimeOffset = iTimeOffset;

    pMyObject->ReleaseAccess(hThisObject);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsAdjustClock2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hUniversalTime,
                int                         iTimeOffset
            );

    description:

        This function is called to set the current time. Later on, the
        object can derive the time by the ticks passing by.



    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hUniversalTime
                Specifies the handle of the current UTC calendar time. It is 
                independent on the timezone and daylight saving.

                int                         iTimeOffset
                Specifies the timeoffset in seconds of this location
                from the GMT. It includes the effect of both timezone
                and daylight saving.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsAdjustClock2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUniversalTime,
        int                         iTimeOffset
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulSecond          = 0;
    ULONG                           ulTicks           = AnscGetTickInSeconds();

    returnStatus =
        pMyObject->CalendarToSecond
            (
                hThisObject,
                hUniversalTime,
                &ulSecond
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pMyObject->AcquireAccess(hThisObject);

        pMyObject->LastTick    = ulTicks;
        pMyObject->LastSecond  = ulSecond;
        pMyObject->iTimeOffset = iTimeOffset;

        pMyObject->ReleaseAccess(hThisObject);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsSecondsToCalendar
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
AnscSstoStsSecondsToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    return  pMyObject->SecondToCalendar
                (
                    (ANSC_HANDLE)pMyObject,
                    TimeStampI,
                    hCalendar
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsCalendarToSeconds
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
AnscSstoStsCalendarToSeconds
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    return  pMyObject->CalendarToSecond
                (
                    (ANSC_HANDLE)pMyObject,
                    hCalendar,
                    pTimeStampI
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsSysTickToCalendar
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTicks,
                ANSC_HANDLE                 hCalendar
            );

    description:

        This function is called to Converts the system tick count into
        calendar time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTicks
                This handle contains the 32-bit system tick.

                ANSC_HANDLE                 hCalendar
                The output time in calendar format. Its value is for
                GMT time

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsSysTickToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTicks,
        ANSC_HANDLE                 hCalendar
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulCurTick    = (ULONG                       )AnscGetTickInSeconds();
    ULONG                           ulCurSecs    = (ULONG                       )pMyObject->GetCurrSecond((ANSC_HANDLE)pMyObject);
    int                             iTbpSecs     = (int                         )0;

    iTbpSecs  = (int)ulCurSecs - ((int)ulCurTick - (int)ulTicks);
    iTbpSecs += pMyObject->iTimeOffset;

    return  pMyObject->SecondToCalendar
                (
                    (ANSC_HANDLE)pMyObject,
                    (ULONG      )iTbpSecs,
                    hCalendar
                );
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        INT
        AnscSstoStsGetTimeOffset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return the lastsecond, which is set 
        by adjust time functions or get time functions.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

INT
AnscSstoStsGetTimeOffset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    return  pMyObject->GetiTimeOffset(pMyObject);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoStsSetTimeOffset
            (
                ANSC_HANDLE                 hThisObject,
                INT                         value
            );

    description:

        This function is called to set the timeoffset in seconds
        of this location from the GMT.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


                INT                         value
                Specifies the value to set

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoStsSetTimeOffset
    (
        ANSC_HANDLE                 hThisObject,
        INT                         value
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    return  pMyObject->SetiTimeOffset(pMyObject, value);
}
