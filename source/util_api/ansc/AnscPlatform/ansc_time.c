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

    module:	ansc_time.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to date/time operation.

        *   AnscGetTimeIntervalInSeconds
        *   AnscCalendarToSecond
        *   AnscSecondToCalendar
        *   AnscSecondToLocalCalendar
        *   AnscGetWholeTickInMicroSeconds

		*	AnscSetSimpleTimeZone
		*	AnscGetSimpleTimeZoneOffset
		*	AnscEnumSimpleTimeZones
		*	AnscGetSimpleTimeZone

        *   UserProfilingStartPcc
        *   UserProfilingStopPcc
        *   UserProfilingReadPcc

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        01/07/03    initial revision.

**********************************************************************/


#include "ansc_global.h"
#include "ansc_ssto_interface.h"
#include "ansc_ssto_external_api.h"
#include "safec_lib_common.h"


#ifdef	_ANSC_SIMPLE_TIMEZONE
#include "ansc_timezone.h"

static	char
gCurrTimeZoneName[ANSC_TZ_MAX_NAME_LENGTH] = "GMT";

static	USHORT
gCurrTimeZoneIndex = -1;

#endif


static  USHORT
g_usDaysOfYearInMonth[2][13] =
{
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

#define  ANSC_TIME_YEAR_1970                        1970
#define  ANSC_TIME_SECONDS_FROM_1900_TO_1970        2208988800LL
#define  ANSC_TIME_SECONDS_IN_DAY                   86400
#define  ANSC_TIME_SECONDS_IN_HOUR                  3600
#define	 ANSC_TIME_HOURS_IN_DAY	                    24
#define	 ANSC_TIME_MINUTES_IN_HOUR	                60
#define	 ANSC_TIME_SECONDS_IN_MINUTE                60
#define	 ANSC_TIME_DAYS_IN_WEEK                     7
#define	 ANSC_TIME_DAYS_IN_NORMAL_YEAR              365
#define  ANSC_TIME_WDAY_ON_JAN01_1970               4

#define	 AnscTimeIfLeapyear(year)		            \
         ( ( (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0) )? 1:0 )

#define	 AnscTimeDaysInYear(year) 	                (AnscTimeIfLeapyear(year) ? 366 : 365)

ULONG
AnscGetTimeIntervalInSeconds
    (
        ULONG                       ulOld,
        ULONG                       ulNew
    )
{
    if ( ulNew >= ulOld )
    {
        return ulNew - ulOld;
    }
    else
    {
        return 0xFFFFFFFFUL - ulOld + ulNew;
    }
}


ULONG
AnscCalendarToSecondFromEpoch
    (
        ANSC_HANDLE                 hCalendar
    )
{
    PANSC_UNIVERSAL_TIME            pCalendar      = (PANSC_UNIVERSAL_TIME)hCalendar;
    ULONG                           ulNewTime      = 0;
    ULONG                           ulLeapsToDate  = 0;
    ULONG                           ulYDay         = 0;
    ULONG                           ulLastYear     = pCalendar->Year - ANSC_TIME_YEAR_1970;
    ULONG                           ulRealLastYear = pCalendar->Year - 1;

    /*
     * Get the days since 01/01/1970
     * notice that DayOfMonth is using 1-31 instead of 0-30. We need to reduce the number of day
     * by one for the current day of year
     */
    ulLeapsToDate = ulRealLastYear / 4 - ulRealLastYear / 100 + ulRealLastYear / 400 -
                    (ANSC_TIME_YEAR_1970 / 4 - ANSC_TIME_YEAR_1970  / 100 + ANSC_TIME_YEAR_1970  / 400 );
    ulYDay        = 
        g_usDaysOfYearInMonth[AnscTimeIfLeapyear(pCalendar->Year)][pCalendar->Month-1] +
        pCalendar->DayOfMonth - 1;

    ulNewTime     = ulLastYear * 365 + ulLeapsToDate + ulYDay;

    /*
     * Overflow only occurs in the year 2106, so we don't handle the overflow here.
     */
    ulNewTime = ulNewTime * ANSC_TIME_HOURS_IN_DAY      + pCalendar->Hour;
    ulNewTime = ulNewTime * ANSC_TIME_MINUTES_IN_HOUR   + pCalendar->Minute;
    ulNewTime = ulNewTime * ANSC_TIME_SECONDS_IN_MINUTE + pCalendar->Second;

    return  ulNewTime;
}


ULONG
AnscCalendarToSecond
    (
        ANSC_HANDLE                 hCalendar
    )
{
    /*
     * return the second since the year 1900
     */
    return  AnscCalendarToSecondFromEpoch(hCalendar) + ANSC_TIME_SECONDS_FROM_1900_TO_1970;
}


ANSC_STATUS
AnscSecondToCalendar
    (
        ULONG                       ulSeconds,
        ANSC_HANDLE                 hCalendar
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PANSC_UNIVERSAL_TIME            pCalendar          = (PANSC_UNIVERSAL_TIME        )hCalendar;
/*
    PANSC_SIMPLE_SYS_TIME_OBJECT    pAnscSimpleSysTime = (PANSC_SIMPLE_SYS_TIME_OBJECT)NULL;

    pAnscSimpleSysTime =
        (PANSC_SIMPLE_SYS_TIME_OBJECT)AnscCreateSimpleSysTime
            (
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)NULL
            );

    if ( !pAnscSimpleSysTime )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        returnStatus =
            pAnscSimpleSysTime->StsSecondsToCalendar
                (
                    (ANSC_HANDLE)pAnscSimpleSysTime,
                    ulSeconds,
                    hCalendar
                );
    }

    pAnscSimpleSysTime->Remove((ANSC_HANDLE)pAnscSimpleSysTime);
*/

	struct tm*						gmTime;

	gmTime = gmtime((time_t*)&ulSeconds);

	if ( gmTime )
	{
		pCalendar->bDayLightSaving	= (gmTime->tm_isdst != 0);
		pCalendar->Year				= gmTime->tm_year + 1900;
		pCalendar->Month			= gmTime->tm_mon + 1;
		pCalendar->DayOfMonth		= gmTime->tm_mday;
		pCalendar->DayOfWeek		= gmTime->tm_wday;
		pCalendar->Hour				= gmTime->tm_hour;
		pCalendar->Minute			= gmTime->tm_min;
		pCalendar->Second			= gmTime->tm_sec;
		pCalendar->MilliSecond		= 0;
	}

    return  returnStatus;
}

ANSC_STATUS
AnscSecondToLocalCalendar
    (
        ULONG                       ulSeconds,
        ANSC_HANDLE                 hCalendar
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PANSC_UNIVERSAL_TIME            pCalendar          = (PANSC_UNIVERSAL_TIME        )hCalendar;

        struct tm*                                              gmTime;

        gmTime = localtime((time_t*)&ulSeconds);
        if ( gmTime )
        {
                pCalendar->bDayLightSaving      = (gmTime->tm_isdst != 0);
                pCalendar->Year                         = gmTime->tm_year + 1900;
                pCalendar->Month                        = gmTime->tm_mon + 1;
                pCalendar->DayOfMonth           = gmTime->tm_mday;
                pCalendar->DayOfWeek            = gmTime->tm_wday;
                pCalendar->Hour                         = gmTime->tm_hour;
                pCalendar->Minute                       = gmTime->tm_min;
                pCalendar->Second                       = gmTime->tm_sec;
                pCalendar->MilliSecond          = 0;
        }

    return  returnStatus;

}

#ifdef	_ANSC_SIMPLE_TIMEZONE

ANSC_STATUS 
AnscSetSimpleTimeZone
	(
		char*						pTzName
	)
{
	ANSC_STATUS			return_status = ANSC_STATUS_SUCCESS;
	USHORT				i;
	errno_t  rc = -1;

	if ( strcmp(gCurrTimeZoneName,  pTzName) == 0 )
	{
		return return_status;
	}

	rc = strcpy_s(gCurrTimeZoneName, sizeof(gCurrTimeZoneName), pTzName);
	ERR_CHK(rc);

	for (i = 0; i < al_tz_total_zones; i++)
	{
		if ( strcmp(al_tz_name_array[i].zone_name, gCurrTimeZoneName) == 0 )
		{
			break;
		}
	}

	if ( i < al_tz_total_zones )
	{
		gCurrTimeZoneIndex = al_tz_name_array[i].zone_info_idx;
	}
	else
	{
		return_status = ANSC_STATUS_FAILURE;
		gCurrTimeZoneIndex = gmtZoneIndex;
	}

	return	return_status;
}


ULONG
AnscGetSimpleTimeZoneOffset
	(
		LONG						utcSeconds,
		BOOL						bLocalTime
	)
{
	LONG			trans_sec, gmtoffset;
	USHORT			i, j, k;
	BOOL			bLocal   = bLocalTime,
		            bDoAgain = FALSE;

	if ( !utcSeconds )
	{
		trans_sec = utcSeconds;
	}
	else
	{
		trans_sec  = AnscGetUtcSeconds();
		bLocalTime = FALSE;
	}

	if ( gCurrTimeZoneIndex == (USHORT)-1 )
	{
		for (i = 0; i < al_tz_total_zones; i++)
		{
			if ( strcmp(al_tz_name_array[i].zone_name, gCurrTimeZoneName) == 0 )
			{
				break;
			}
		}

		if ( i < al_tz_total_zones )
		{
			gCurrTimeZoneIndex = i;
		}
		else
		{
			gCurrTimeZoneIndex = gmtZoneIndex;
		}
	}

	k = gCurrTimeZoneIndex;

	do 
	{
		if ( al_tz_info_array[k].trans_count )
		{
			for (j = 0; j < al_tz_info_array[k].trans_count; j++)
			{
				if ( trans_sec <= al_tz_info_array[k].trans_array[j].trans_sec )
				{
					break;
				}
			}

			if ( j != 0 )
			{
				j--;
			}
			gmtoffset = al_tz_info_array[k].types_array[al_tz_info_array[k].trans_array[j].types_idx].gmtoffs;
		}
		else
		{
			gmtoffset = al_tz_info_array[k].types_array[0].gmtoffs;
		}

		if ( bLocal && (al_tz_info_array[k].trans_count > 1))
		{
			trans_sec += gmtoffset;
			bDoAgain   = TRUE;
			bLocal     = FALSE;
		}
		else
		{
			bDoAgain   = FALSE;
		}
	} while (bDoAgain);

	return gmtoffset;
}


ANSC_STATUS
AnscEnumSimpleTimeZones
	(
		char***						ppTzArray, 
		PULONG						pulCount
	)
{
	USHORT			i;
	ANSC_STATUS		return_status = ANSC_STATUS_SUCCESS;

	*ppTzArray = (char **)AnscAllocateMemory(sizeof(char *) * al_tz_total_zones);

	if ( *ppTzArray != NULL )
	{
		for ( i = 0; i < al_tz_total_zones; i++ )
		{
			(*ppTzArray)[i] = al_tz_name_array[i].zone_name;
		}

		*pulCount     = al_tz_total_zones;
	}
	else
	{
		return_status = ANSC_STATUS_FAILURE;
		*pulCount     = 0;
	}

	return	return_status;
}


char* 
AnscGetTimeZone
	(
		void
	)
{
	return	gCurrTimeZoneName;
}


#endif


/**********************************************************************
                            PROFILING
**********************************************************************/


