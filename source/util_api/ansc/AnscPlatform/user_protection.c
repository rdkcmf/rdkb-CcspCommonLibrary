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

    module: user_protection.h

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

#define _XOPEN_SOURCE 600
#include "user_base.h"
#include "user_protection.h"
#include "ansc_platform.h"

#define  MAX_EVENT_INIT_TRY                         20
#define  MAX_EVENT_NUM                              1000

key_t pubEventKey = 10000;
/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/
#if 0
BOOLEAN
UserInitializeLock(PUSER_LOCK  pLock)
{
    *pLock = semget(IPC_PRIVATE, 1, SHM_R | SHM_W);
    if ( *pLock == -1 )
    {
        return FALSE;
    }
    else
    {
        union semun sunion;
        sunion.val = 1;
        semctl(*pLock, 0, SETVAL, sunion);
        return TRUE;
    }
}

VOID
UserAcquireLock(PUSER_LOCK  pLock)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op  = -1;
    sb.sem_flg = 0;

    semop(*pLock, &sb, 1);
}

VOID
UserReleaseLock(PUSER_LOCK  pLock)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op  = 1;
    sb.sem_flg = 0;

    semop(*pLock, &sb, 1);
}

VOID
UserFreeLock(PUSER_LOCK  pLock)
{
    union semun sunion;

    semctl(*pLock, 0, IPC_RMID, sunion);
}
#endif
VOID
UserInitializeEvent(PUSER_EVENT pEvent)
{
    union semun                     sunion;
    int                             i = 0;

    pEvent->eventId = -1;

    while (( pEvent->eventId == -1 ) && ( i < MAX_EVENT_INIT_TRY ))
    {
        if ( pubEventKey == MAX_EVENT_NUM )
        {
            pubEventKey = 0;
        }
        pubEventKey ++;
        pEvent->eventId = semget(pubEventKey, 1, IPC_CREAT | IPC_EXCL);
        i++;
    }

    if ( pEvent->eventId == -1 )
    {
        perror("Error when init Event!\n");
        return;
    }
    pEvent->keyId   = pubEventKey;
    sunion.val      = 0;

    semctl(pEvent->eventId, 0, SETVAL, sunion);

    return;
}

VOID
UserSetEvent(PUSER_EVENT pEvent)
{
    union semun sunion;

    /*
    sunion.val = 0;
    semctl(pEvent->eventId, 0, IPC_RMID, sunion);

    pEvent->eventId = semget(pEvent->keyId, 1, IPC_CREAT | IPC_EXCL);
    if (pEvent->eventId == -1)
    {
        perror("SetEvent error!\n");
        return;
    }
    else
    {
    }
    */
    sunion.val = 1;
    semctl(pEvent->eventId, 0, SETVAL, sunion);

    return;

}

VOID
UserResetEvent(PUSER_EVENT pEvent)
{
    union semun sunion;

    sunion.val = 0;
    semctl(pEvent->eventId, 0, SETVAL, sunion);
    return;
}

VOID
UserPulseEvent(PUSER_EVENT pEvent)
{
    union semun sunion;

    sunion.val = 0;
    semctl(pEvent->eventId, 0, IPC_RMID, sunion);

    pEvent->eventId = semget(pEvent->keyId, 1, IPC_CREAT | IPC_EXCL);
    if (pEvent->eventId == -1)
    {
        perror("SetEvent error!\n");
        return;
    }
    else
    {
        sunion.val = 0;
        semctl(pEvent->eventId, 0, SETVAL, sunion);
    }

    return;
}

VOID
UserWaitEvent(PUSER_EVENT pEvent, ULONG ulMilliSeconds)
{
    UNREFERENCED_PARAMETER(ulMilliSeconds);
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = IPC_NOWAIT;

    if (semop(pEvent->eventId, &sb, 1) == 0)
        return;

#if 0
    tm.tv_sec = ulMilliSeconds/1000;
    tm.tv_usec = (ulMilliSeconds%1000)*1000;

    select(0, NULL, NULL, NULL, &tm);

    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = IPC_NOWAIT;
#endif
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;

    semop(pEvent->eventId, &sb, 1);

    return;
}

VOID
UserFreeEvent(PUSER_EVENT pEvent)
{
    union semun sunion;

    sunion.val = 0;
    semctl(pEvent->eventId, 0, IPC_RMID, sunion);

    return;
}

//__inline  BOOLEAN
BOOLEAN
UserInitializeSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulInitialCount, ULONG  ulMaximumCount)
{
    UNREFERENCED_PARAMETER(ulMaximumCount);
    //ULONG ulInitialCount = 0;
    int rc = 0;
    rc = sem_init(pSemaphore, 0, ulInitialCount);

    if ( rc == -1 )
    {
        perror("Error when init Semaphore!\n");
        return  FALSE;
    }

    return  TRUE;
}

//__inline  void
void
UserAcquireSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    /* lack of timeout option */
    struct timespec  tm = {0};
    struct timeval                  tv = {0};
    int ret = 0;

    if ( (ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF) )
    {
        sem_wait(pSemaphore);
    }
    else
    {
        gettimeofday(&tv, NULL);

        tm.tv_sec = tv.tv_sec + ulMilliSeconds/1000;
        ulMilliSeconds -= (tm.tv_sec - tv.tv_sec)*1000;
        tm.tv_nsec = tv.tv_usec * 1000 + ulMilliSeconds*1000000;
        if (tm.tv_nsec >= 1000000000)
        {
            tm.tv_sec ++;
            tm.tv_nsec -= 1000000000;
        }
        /*CID: 66947 Unchecked return value*/
        ret = sem_timedwait(pSemaphore, &tm);
	if (ret < 0)
	{
	     CcspTraceInfo(("%s:sem_timedwait returns error %d - %s\n", __FUNCTION__, errno, strerror(errno)));
        }
    }
    return;
}

__inline  BOOLEAN
UserAcquireSemaphore2(PUSER_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    /* lack of timeout option */
    struct timespec  tm = {0};
    struct timeval                  tv = {0};

    if ( (ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF) )
    {
        sem_wait(pSemaphore);

        return TRUE;
    }
    else
    {
        gettimeofday(&tv, NULL);

        tm.tv_sec = tv.tv_sec + ulMilliSeconds/1000;
        ulMilliSeconds -= (tm.tv_sec - tv.tv_sec)*1000;
        tm.tv_nsec = tv.tv_usec * 1000 + ulMilliSeconds*1000000;
        if (tm.tv_nsec >= 1000000000)
        {
            tm.tv_sec ++;
            tm.tv_nsec -= 1000000000;
        }

        return ( 0 == sem_timedwait(pSemaphore, &tm));
    }
}

__inline  void
UserReleaseSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulCount)
{
    while(ulCount--)
    {
        sem_post(pSemaphore);
    }
}

__inline  void
UserFreeSemaphore(PUSER_SEMAPHORE  pSemaphore)
{
    sem_destroy(pSemaphore);
}


VOID
UserInitializeSemEvent(PSEM_EVENT *pEvent)
{
	PSEM_EVENT event = NULL;
    event = (PSEM_EVENT)AnscAllocateMemory(sizeof(SEM_EVENT));

    if ( !event )
    {
        AnscTrace("Error creating SemEvent!\n");
        return;
    }

    if (sem_init(&event->sem, 0, 0) == -1)
    {
        perror("Error when initializing SemEvent!\n");
        event->inited = 0;
    }
    else
    {
    	event->inited = 1;
    }

    *pEvent = event;

    return;
}

VOID
UserSetSemEvent(PSEM_EVENT *pEvent)
{
	PSEM_EVENT event = *pEvent;
	if (!event || event->inited == 0)
	{
        AnscTrace("Set - SemEvent not inited!\n");
        return;
	}

	if (sem_post(&event->sem) == -1)
	{
        perror("Error when posting SemEvent!\n");
	}

	return;

}

VOID
UserResetSemEvent(PSEM_EVENT *pEvent)
{
	PSEM_EVENT event = *pEvent;
	int s, num = 0;

	if (!event || event->inited == 0)
	{
        AnscTrace("Reset - SemEvent not inited!\n");
        return;
	}

    s = sem_getvalue(&event->sem, &num);
	/* AnscTrace("Reset - get value of SemEvent %d, %d!\n", num, s); */
	if ( s != -1 && num < 0)
	{
        AnscTrace("Reset - posting SemEvent %d!\n", num);
		sem_post(&event->sem);
	}

	/*
	UserFreeSemEvent(pEvent);
	UserInitializeSemEvent(pEvent);
	*/
	return;
}

VOID
UserPulseSemEvent(PSEM_EVENT *pEvent)
{
    UNREFERENCED_PARAMETER(pEvent);
    AnscTrace("PulseSemEvent not supported!\n");

    return;
}

VOID
UserWaitSemEvent(PSEM_EVENT *pEvent, ULONG ulMilliSeconds)
{
	struct timespec ts;
	int s;
	struct timeval now;
	PSEM_EVENT event = *pEvent;

	if (!event || event->inited == 0)
	{
        AnscTrace("Wait - SemEvent not inited!\n");
        return;
	}

	/* gettimeofday(&now,NULL); */

    /* AnscTrace("Wait - SemEvent %p for %lu!\n", event, ulMilliSeconds); */
    /* AnscTrace("Wait - SemEvent %p start %lu.%lu!\n", event, now.tv_sec, now.tv_usec); */
	if ( ulMilliSeconds == 0xFFFFFFFF ) {
		while ((s = sem_wait(&event->sem)) == -1 && errno == EINTR)
	        continue;       /* Restart when interrupted by handler */
	}
	else {
		unsigned long ms = 0L;

		gettimeofday(&now,NULL);
		/* only to mill-second precision */
		ms = ulMilliSeconds + now.tv_usec / 1000;

		ts.tv_sec = now.tv_sec + ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000;

	    /* AnscTrace("Wait - SemEvent %p ts %lu.%lu!\n", event, ts.tv_sec, ts.tv_nsec); */

		while ((s = sem_timedwait(&event->sem, &ts)) == -1 && errno == EINTR)
		{
	        continue;       /* Restart when interrupted by handler */
		}

	}

	/* gettimeofday(&now,NULL); */
    /* AnscTrace("Wait - SemEvent %p end %lu.%lu!\n", event, now.tv_sec, now.tv_usec); */
    return;
}

VOID
UserFreeSemEvent(PSEM_EVENT *pEvent)
{
	PSEM_EVENT event = *pEvent;

	if (!event || event->inited == 0)
	{
        AnscTrace("Free - SemEvent not inited!\n");
        return;
	}

	sem_destroy(&event->sem);
	event->inited = 0;

	AnscFreeMemory(event);
	*pEvent = NULL;
    return;
}


