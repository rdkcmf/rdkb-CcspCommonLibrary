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

    module:	user_time_priv.c

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

		Venka Gade

    ---------------------------------------------------------------

    revision:

        05/01/14    initial revision.

**********************************************************************/
#include "user_base.h"
#include "user_time.h"


extern __inline void UserSetDelta();

struct delta_t {
    unsigned long  sec;
    unsigned long  millisec;
};

struct seg {
    struct delta_t  delta;
    int             count;
};

struct seg * segptr = NULL;
int shmid = -1,sid = -1;

key_t shemkey= 100;
#define SEGSIZE 12
#define SEM_RESOURCE_MAX        1
key_t semKey = 2000;

void
UserGetNtpTime(time_t*  ltime)
{

	time(ltime);
}

ULONG
UserGetTickInSeconds()
{
    struct timeval                  tv = {0};
    struct timezone                 tz = {0};
    union semun semopts;
    struct sembuf sem_readerlock={ 0, -1, 0};
    struct sembuf sem_writerlock={ 1, -1, 0};
    struct sembuf sem_readerunlock={ 0, 1, 0};
    struct sembuf sem_writerunlock={ 1, 1, 0};
	int cntr;
	unsigned long  ret = 0;

    if ( !segptr )
    {
        UserSetDelta();
    }

	if ( sid == -1 )
    {
        if ( (sid = semget(semKey, 2, IPC_CREAT|IPC_EXCL|0666)) != -1)
        {
            semopts.val = SEM_RESOURCE_MAX;

            for(cntr=0; cntr<2; cntr++)
            {
                semctl(sid, cntr, SETVAL, semopts);
            }
        }
		else
        {
            sid = semget(semKey, 2, IPC_CREAT|0666);
        }
    }

	semop(sid, &sem_readerlock, 1);

    if ( ++segptr->count == 1 )
    {
        semop(sid, &sem_writerlock, 1);
    }

    semop(sid, &sem_readerunlock,1);

    gettimeofday(&tv, &tz);

	ret = tv.tv_sec - segptr->delta.sec;

	semop(sid, &sem_readerlock, 1);

	if ( !--segptr->count)
    {
        semop(sid,&sem_writerunlock,1);
    }

    semop(sid, &sem_readerunlock,1);

    return ret;
    /*return UserGetTickInMilliSeconds()/1000;*/
}

__inline void
UserSetDelta()
{
    struct timeval                  tv = {0};
    int fd;
    unsigned char buf[64] = {0};
    unsigned long ulSecond, ulHundredth = 0;
    char * pbuf = (char *)buf;
    char * pbuf1 = NULL; /*RDKB-6288, CID-24502, initilize before use*/

	if ( shmid == -1 )
    {
        shmid = shmget(shemkey, SEGSIZE, IPC_CREAT|0666);
        if((segptr = (struct seg *)shmat(shmid, 0, 0)) == (void*)-1)
        {
            fprintf(stderr,"can't get shared memory !\n");
			shmid = -1;
			segptr = NULL;
			return;
        }
    }

    fd = open("/proc/uptime",O_RDONLY);

    if ( fd < 0 )
    {
        return;
    }

    /*CID: 162543 Ignoring number of bytes read*/
    int valread = read(fd, buf, sizeof(buf));
    if ( valread < 0 )
    {
       fprintf(stderr,"Failure in read!\n");
       close(fd);
       return;
    }

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

    ulSecond    = atol((char *)buf);
    /*CID: 70450 Explicit null dereferenced*/
    if (pbuf1)
        ulHundredth = atol(pbuf1);

    close(fd);


    gettimeofday(&tv, NULL);

    if ( tv.tv_usec > (long)ulHundredth * 10000 )
    {
        segptr->delta.millisec = tv.tv_usec/1000 - ulHundredth * (10000/1000);
    }
    else
    {
        segptr->delta.millisec = (tv.tv_usec + 1000000)/1000  - ulHundredth * (10000/1000);
        tv.tv_sec--;
    }

    segptr->delta.sec = tv.tv_sec - ulSecond;

    return;
}

ULONG
UserGetTickInMilliSeconds()
{
    struct timeval                  tv = {0};
    struct timezone                 tz = {0};
    union semun semopts;
    struct sembuf sem_readerlock={ 0, -1, 0};
    struct sembuf sem_writerlock={ 1, -1, 0};
    struct sembuf sem_readerunlock={ 0, 1, 0};
    struct sembuf sem_writerunlock={ 1, 1, 0};
	int cntr;
	unsigned long  ret = 0;

    if ( !segptr )
    {
        UserSetDelta();
    }

	if ( sid == -1 )
    {
        if ( (sid = semget(semKey, 2, IPC_CREAT|IPC_EXCL|0666)) != -1)
        {
            semopts.val = SEM_RESOURCE_MAX;

            for(cntr=0; cntr<2; cntr++)
            {
                semctl(sid, cntr, SETVAL, semopts);
            }
        }
		else
        {
            sid = semget(semKey, 2, IPC_CREAT|0666);
        }
    }

	semop(sid, &sem_readerlock, 1);

    if ( ++segptr->count == 1 )
    {
        semop(sid, &sem_writerlock, 1);
    }

    semop(sid, &sem_readerunlock,1);

    gettimeofday(&tv, &tz);

	ret = (tv.tv_sec - segptr->delta.sec) * 1000 + tv.tv_usec/1000 - segptr->delta.millisec;

	semop(sid, &sem_readerlock, 1);

	if ( !--segptr->count)
    {
        semop(sid,&sem_writerunlock,1);
    }

    semop(sid, &sem_readerunlock,1);

    return ret;
}

void
UserSetSystemTime(USER_SYSTEM_TIME*  pSystemTime)
{
    time_t          newTime;
    struct tm       newtm;
    struct timeval  newtv;
    union semun semopts;
    struct sembuf sem_writerlock={ 1, -1, 0};
    struct sembuf sem_writerunlock={ 1, 1, 0};
	int cntr;

    newtm.tm_year   = pSystemTime->Year           - 1900;
    newtm.tm_mon    = pSystemTime->Month          - 1;
    newtm.tm_mday   = pSystemTime->DayOfMonth;
    newtm.tm_wday   = pSystemTime->DayOfWeek;
    newtm.tm_hour   = pSystemTime->Hour;
    newtm.tm_min    = pSystemTime->Minute;
    newtm.tm_sec    = pSystemTime->Second;
    //newtm.tm_isdst  = -1;
    //newtm.tm_isdst = pSystemTime->bDayLightSaving;
    newtm.tm_isdst = 1;

#if !defined(_ANSC_LINUX_NO_TM_GMT)
    newTime = timegm(&newtm);
    /*CID: 62026 Argument cannot be negative*/
    if (newTime < 0) {
         printf("newTime cant be negative\n");
         return;
    }
#endif

    /*
    printf("Linux --- SetSystemTime: H: %d M: %d S: %d isdst: %d \n",  newtm.tm_hour,
	   newtm.tm_min, newtm.tm_sec, newtm.tm_isdst);
   */

    newtv.tv_sec  = (long)newTime;
    newtv.tv_usec = 0;

	if ( sid == -1 )
    {
        if ( (sid = semget(semKey, 2, IPC_CREAT|IPC_EXCL|0666)) != -1)
        {
            semopts.val = SEM_RESOURCE_MAX;

            for(cntr=0; cntr<2; cntr++)
            {
                semctl(sid, cntr, SETVAL, semopts);
            }
        }
		else
        {
            sid = semget(semKey, 2, IPC_CREAT|0666);
        }
    }

    semop(sid, &sem_writerlock, 1);
    settimeofday(&newtv, NULL);
	UserSetDelta();
    semop(sid, &sem_writerunlock, 1);
    system("/sbin/hwclock --systohc --utc");
}

struct tm *
UserGetTimeNow(time_t *timeNow, struct tm *Tm)
{
	UserGetNtpTime(timeNow);
	return localtime_r(timeNow,Tm);
}

void
UserSetLocalTime(USER_SYSTEM_TIME*  pSystemTime)
{
    time_t          newTime;
    struct tm       newtm;
    struct timeval  newtv;
    union semun semopts;
    struct sembuf sem_writerlock={ 1, -1, 0};
    struct sembuf sem_writerunlock={ 1, 1, 0};
    int cntr;

    newtm.tm_year   = pSystemTime->Year           - 1900;
    newtm.tm_mon    = pSystemTime->Month          - 1;
    newtm.tm_mday   = pSystemTime->DayOfMonth;
    newtm.tm_wday   = pSystemTime->DayOfWeek;
    newtm.tm_hour   = pSystemTime->Hour;
    newtm.tm_min    = pSystemTime->Minute;
    newtm.tm_sec    = pSystemTime->Second;
    //newtm.tm_isdst  = -1;
    newtm.tm_isdst  = pSystemTime->bDayLightSaving;


    printf("Linux --- SetLocalTime: H: %d M: %d S: %d isdst: %d \n",  newtm.tm_hour,
	   newtm.tm_min, newtm.tm_sec, newtm.tm_isdst);


    newTime = mktime(&newtm);
    /*CID:55942 Argument cannot be negative*/
    if (newTime < 0) {
         printf("newTime cant be negative\n");
         return;
    }

    newtv.tv_sec  = (long)newTime;
    newtv.tv_usec = 0;
	if ( sid == -1 )
    {
        if ( (sid = semget(semKey, 2, IPC_CREAT|IPC_EXCL|0666)) != -1)
        {
            semopts.val = SEM_RESOURCE_MAX;

            for(cntr=0; cntr<2; cntr++)
            {
                semctl(sid, cntr, SETVAL, semopts);
            }
        }
		else
        {
            sid = semget(semKey, 2, IPC_CREAT|0666);
        }
    }

    semop(sid, &sem_writerlock, 1);
    settimeofday(&newtv, NULL);
	UserSetDelta();
    semop(sid, &sem_writerunlock, 1);
    system("/sbin/hwclock --systohc --utc");
}


