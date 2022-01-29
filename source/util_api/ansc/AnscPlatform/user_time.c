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

    module:	user_time.c

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
#include "user_base.h"
#include "user_time.h"
#include "safec_lib_common.h"


#define ZONE_NUM 38
char* zonemap[ZONE_NUM][3] = {
    {"(GMT-12:00) Kwajalein", "GMT12:00", "GMT12:00"},
    {"(GMT-11:00) Midway Island,Samoa", "GMT11:00", "GMT11:00"},
    {"(GMT-10:00) Hawaii", "GMT10:00", "GMT10:00"},
    {"(GMT-09:00) Alaska", "GMT09:00HDT", "GMT09:00"},
    {"(GMT-08:00) Pacific Time", "GMT08:00PDT", "GMT08:00"},
    {"(GMT-07:00) Arizona", "GMT07:00", "GMT07:00"},
    {"(GMT-07:00) Mountain Time", "GMT07:00MDT", "GMT07:00"},
    {"(GMT-06:00) Mexico", "GMT06:00CDT", "GMT06:00"},
    {"(GMT-06:00) Central Time", "GMT06:00CDT", "GMT06:00"},
    {"(GMT-05:00) Indiana East,Colombia,Panama", "GMT05:00", "GMT05:00"},
    {"(GMT-05:00) Eastern Time", "GMT05:00EDT",  "GMT05:00"},
    {"(GMT-04:00) Bolivia,Venezuela", "GMT04:00",  "GMT04:00"},
    {"(GMT-04:00) Atlantic Time,Brazil West", "GMT04:00ADT", "GMT04:00"},
    {"(GMT-03:00) Guyana", "GMT03:00", "GMT03:00"},
    {"(GMT-03:00) Brazil East,Greenland", "GMT03:00GDT", "GMT03:00"},
    {"(GMT-02:00) Mid-Atlantic", "GMT02:00MADT", "GMT02:00"},
    {"(GMT-01:00) Azores", "GMT01:00AZODT", "GMT01:00"},
    {"(GMT) Gambia,Liberia,Morocco", "GMT", "GMT"},
    {"(GMT) England", "GMT00:00BST", "GMT00:00"},
    {"(GMT+01:00) Tunisia", "GMT-01:00", "GMT-01:00"},
    {"(GMT+01:00) France,Germany,Italy", "GMT-01:00FST", "GMT-01:00"},
    {"(GMT+02:00) South Africa", "GMT-02:00", "GMT-02:00"},
    {"(GMT+02:00) Greece,Ukraine,Romania,Turkey", "GMT-02:00EETDST", "GMT-02:00"},
    {"(GMT+03:00) Iraq,Jordan,Kuwait", "GMT-03:00", "GMT-03:00"},
    {"(GMT+04:00) Armenia", "GMT-04:00", "GMT-04:00"},
    {"(GMT+05:00) Pakistan,Russia", "GMT-05:00", "GMT-05:00"},
    {"(GMT+05:30) India", "GMT-05:30", "GMT-05:30"},
    {"(GMT+06:00) Bangladesh,Russia", "GMT-06:00", "GMT-06:00"},
    {"(GMT+07:00) Thailand,Russia", "GMT-07:00", "GMT-07:00"},
    {"(GMT+08:00) China,Hong Kong,Australia Western", "GMT-08:00", "GMT-08:00"},
    {"(GMT+08:00) Singapore,Taiwan,Russia", "GMT-08:00", "GMT-08:00"},
    {"(GMT+09:00) Japan,Korea", "GMT-09:00", "GMT-09:00"},
    {"(GMT+10:00) Guam,Russia", "GMT-10:00", "GMT-10:00"},
    {"(GMT+10:00) Australia", "GMT-10:00", "GMT-10:00"},
    {"(GMT+11:00) Solomon Islands", "GMT-11:00", "GMT-11:00"},
    {"(GMT+12:00) Fiji", "GMT-12:00", "GMT-12:00"},
    {"(GMT+12:00) New Zealand", "GMT-12:00", "GMT-12:00"},
    {"", "",""}
};

void UserSetDelta();

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

VOID
UserSleepInMilliSeconds(ULONG milliSecond)
{
    struct timespec                 ts;

    ts.tv_sec  = milliSecond / 1000;
    ts.tv_nsec = (milliSecond - ts.tv_sec * 1000) * 1000000;

    nanosleep(&ts, NULL);

    /*
     * struct timeval tm;
     * tm.tv_sec = milliSecond/1000;
     * tm.tv_usec = (milliSecond%1000)*1000;
     * select(0, NULL, NULL, NULL, &tm);
     */
}

ULONG
UserGetTickInSeconds2()
{
    struct timeval                  tv = {0};

    gettimeofday(&tv, NULL);

    return tv.tv_sec;
}

ULONG
UserGetTickInMilliSeconds2()
{
    struct timeval                  tv = {0};

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/*
ULONG
UserGetTickInSecondsAbs()
{
    return UserGetTickInMilliSecondsAbs()/1000;
}

ULONG
UserGetTickInMilliSecondsAbs()
{
    int fd,rd;
    unsigned char buf[64] = {0};
    unsigned long ulSecond, ulHundredth;
    char * pbuf = buf;
    char * pbuf1;

    fd = open("/proc/uptime",O_RDONLY);

    if ( fd < 0 )
    {
        return 0;
    }

    rd = read(fd, buf, sizeof(buf));

    while ( *pbuf )
    {
        if (*pbuf == '.')
        {
            *pbuf++ = '\0';
            pbuf1 = pbuf;
            continue;
        }
        else if ( *pbuf == ' ' )
        {
            *pbuf = '\0';
            break;
        }
        pbuf++;
    }

    ulSecond    = atol(buf);
    ulHundredth = atol(pbuf1);

    close(fd);

	return ulSecond * 1000 + ulHundredth * 10;
}
*/

void
UserGetSystemTime(USER_SYSTEM_TIME*  pSystemTime)
{
    time_t          timeNow;
	struct tm       Tm = {0};
    struct tm       *ptm = NULL;

	UserGetNtpTime(&timeNow);

	ptm = gmtime_r(&timeNow,&Tm);

    pSystemTime->Year           = ptm->tm_year + 1900;
    pSystemTime->Month          = ptm->tm_mon + 1;
    pSystemTime->DayOfMonth     = ptm->tm_mday;
    pSystemTime->DayOfWeek      = ptm->tm_wday;
    pSystemTime->Hour           = ptm->tm_hour;
    pSystemTime->Minute         = ptm->tm_min;
    pSystemTime->Second         = ptm->tm_sec;
    pSystemTime->MilliSecond    = 0;
    pSystemTime->bDayLightSaving= ptm->tm_isdst;
}


void
UserGetLocalTime(USER_SYSTEM_TIME*  pSystemTime)
{
    time_t          timeNow;
    struct tm       Tm = {0};
    struct tm       *ptm = NULL;

	UserGetNtpTime(&timeNow);

/*   printf("Linux - GetLocalTime: 1: %x %x %x %x\n", pSystemTime, timeNow, &timeNow, &Tm);
     localtime_r crash on android2.2 sometime  */

	ptm = UserGetTimeNow(&timeNow, &Tm);

    pSystemTime->Year           = ptm->tm_year + 1900;
    pSystemTime->Month          = ptm->tm_mon + 1;
    pSystemTime->DayOfMonth     = ptm->tm_mday;
    pSystemTime->DayOfWeek      = ptm->tm_wday;
    pSystemTime->Hour           = ptm->tm_hour;
    pSystemTime->Minute         = ptm->tm_min;
    pSystemTime->Second         = ptm->tm_sec;
    pSystemTime->MilliSecond    = 0;
    pSystemTime->bDayLightSaving= ptm->tm_isdst;

/*    printf("Linux - GetLocalTime: H: %d M: %d S: %d isdst: %d \n", pSystemTime->Hour, pSystemTime->Minute, pSystemTime->Second, pSystemTime->bDayLightSaving ); */
    return;
}

/*
 * get the TZ env from the zone string.
 */
char* UserGetZone(char* pTimeZone, BOOL bDaylightSavingsUsed)
{
	int i=0;
	if(pTimeZone == 0)
		return 0;
	for (i=0; i<ZONE_NUM; i++) {
		if(strcmp(pTimeZone, zonemap[i][0]) == 0) {
			if(bDaylightSavingsUsed)
				return zonemap[i][1];
			else
				return zonemap[i][2];
		}
	}
	/* printf("No zone found\n"); */
	return 0;
}

/*
 * this set the /config/localtime symbolic link to the file
 * /usr/share/zoneinfo/$pTimeZone
 */
void
UserSetTzString(char *pTimeZone, ...)
{
#if defined(_ANSC_LINUX_USE_ZONEINFO_)
    char    *tzlink    = "/config/localtime";
    char    *zoneinfo  = "/usr/share/zoneinfo/";
    char    zonefile[128];
    errno_t rc = -1;

    rc = sprintf_s( zonefile, sizeof(zonefile),"%s%s", zoneinfo,pTimeZone);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    unlink( tzlink);
    symlink( zonefile, tzlink);
    tzset();
#else
    char*                           zoneinfo    = "/etc/TZ";
    int                             TzFile      = 0;
    int                             iSize       = 0;
    errno_t                         rc          = -1;

    //Get the GMT format from the pTimeZone, such as "GMT+12:00"
    char                            zone[30]    = {0};

    va_list arg_p;
    int param;
    BOOL bDaylightSavingsUsed;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    va_start( arg_p, pTimeZone);
    //get the second parameter
    param = va_arg( arg_p, int);
    va_end( arg_p );

    bDaylightSavingsUsed = param;
    char *z = UserGetZone(pTimeZone, bDaylightSavingsUsed);
    rc = sprintf_s(zone, sizeof(zone), "%s\n", (z ? z : ""));
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    TzFile = open(zoneinfo, O_CREAT | O_WRONLY | O_TRUNC, mode);

    if ( TzFile < 0 )
    {
        /* printf("UserSetTzString -- failed to open file %s!\n", zoneinfo); */
    }
    else
    {
        iSize = write(TzFile, zone, strlen(zone));

        if ( iSize < 0 )
        {
            /* printf("UserSetTzString -- failed to write to the file, error = %d!\n", iSize); */
        }

        close(TzFile);
    }

    tzset();
#endif

    UserSetDelta();
    system("/sbin/hwclock --systohc --utc");

    //printf("SetTzString: pTimeZone: %s TzOffset = %ld, bDaylightSavingsUsed=%d \n", pTimeZone, UserGetTzOffset(), bDaylightSavingsUsed);

    return;
}

/*
 * this returns the current value set by tzset() in
 * extern long timezone;
 * it ignores the input timezone string parameter.
 */
LONG
UserGetTzOffset( void )
{
    time_t          timeNow;
	struct tm       Tm = {0};
    struct tm       *ptm = NULL;

    time(&timeNow);
    ptm = localtime_r(&timeNow,&Tm);

    /*
    printf("GetTzOffset = %ld \n",  ptm->tm_gmtoff);
    */

#if !defined(_ANSC_LINUX_NO_TM_GMT)
    return ptm->tm_gmtoff;
#endif

    /*return -timezone;*/
}

unsigned long
UserGetUtcSeconds( void )
{
    time_t          timeNow;

	UserGetNtpTime(&timeNow);

    return timeNow;
}


