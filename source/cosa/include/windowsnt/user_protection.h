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

    module:	user_protection.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

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

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#ifndef  _USER_PROTECTION_
#define  _USER_PROTECTION_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
#define  UserInitializeSpinLock                     UserInitializeLock
#define  UserFreeSpinLock                           UserFreeLock
#define  UserAcquireSpinLock                        UserAcquireLock
#define  UserReleaseSpinLock                        UserReleaseLock
*/

#ifdef _ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_

__inline  BOOLEAN
UserInitializeSpinLock(PUSER_LOCK  pLock)
{
    *pLock = CreateMutex(NULL, FALSE, NULL);

    if ( *pLock == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  void
UserAcquireSpinLock(PUSER_LOCK  pLock)
{
    WaitForSingleObject(*pLock, INFINITE);
}

__inline  void
UserReleaseSpinLock(PUSER_LOCK  pLock)
{
    ReleaseMutex(*pLock);
}

__inline  void
UserFreeSpinLock(PUSER_LOCK  pLock)
{
    CloseHandle(*pLock);
}

#else /* _ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_ */

BOOLEAN
UserInitializeSpinLock(PUSER_SPINLOCK  pLock);

void
UserFreeSpinLock(PUSER_SPINLOCK  pLock);

void
UserAcquireSpinLock(PUSER_SPINLOCK  pLock, const char * file, int lineno);

void
UserReleaseSpinLock(PUSER_SPINLOCK  pLock);

#endif /* _ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_ */


__inline  BOOLEAN
UserInitializeLock(PUSER_LOCK  pLock)
{
    *pLock = CreateSemaphore(NULL, 1, 1, NULL);

    if ( *pLock == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  void
UserAcquireLock(PUSER_LOCK  pLock)
{
    WaitForSingleObject(*pLock, INFINITE);
}

__inline  void
UserReleaseLock(PUSER_LOCK  pLock)
{
    ReleaseSemaphore(*pLock, 1, NULL);
}

__inline  void
UserFreeLock(PUSER_LOCK  pLock)
{
    CloseHandle(*pLock);
}


__inline  BOOLEAN
UserInitializeEvent(PUSER_EVENT  pEvent)
{
    *pEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if ( *pEvent == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  void
UserSetEvent(PUSER_EVENT  pEvent)
{
    SetEvent(*pEvent);
}

__inline  void
UserResetEvent(PUSER_EVENT  pEvent)
{
    ResetEvent(*pEvent);
}

__inline  void
UserPulseEvent(PUSER_EVENT  pEvent)
{
    PulseEvent(*pEvent);
}

__inline  int
UserWaitEvent(PUSER_EVENT  pEvent, ULONG  ulMilliSeconds)
{
    if ( (ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF) )
    {
        return  WaitForSingleObject(*pEvent, INFINITE);
    }
    else
    {
        return  WaitForSingleObject(*pEvent, ulMilliSeconds);
    }
}

__inline  void
UserFreeEvent(PUSER_EVENT  pEvent)
{
    CloseHandle(*pEvent);
}

__inline  BOOLEAN
UserInitializeSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulInitialCount, ULONG  ulMaximumCount)
{
    *pSemaphore = CreateSemaphore(NULL, ulInitialCount, ulMaximumCount, NULL);

    if ( *pSemaphore == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  void
UserAcquireSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    if ( (ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF) )
    {
        WaitForSingleObject(*pSemaphore, INFINITE);
    }
    else
    {
        WaitForSingleObject(*pSemaphore, ulMilliSeconds);
    }
}

__inline  BOOL
UserAcquireSemaphore2(PUSER_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    ULONG                           wait_result = 0;

    if ( (ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF) )
    {
        wait_result = WaitForSingleObject(*pSemaphore, INFINITE);
    }
    else
    {
        wait_result = WaitForSingleObject(*pSemaphore, ulMilliSeconds);
    }

    return  (wait_result == WAIT_OBJECT_0);
}

__inline  void
UserReleaseSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulCount)
{
    ReleaseSemaphore(*pSemaphore, ulCount, NULL);
}

__inline  void
UserFreeSemaphore(PUSER_SEMAPHORE  pSemaphore)
{
    CloseHandle(*pSemaphore);
}


#define  UserInitializeSemEvent                     UserInitializeEvent
#define  UserFreeSemEvent                           UserFreeEvent
#define  UserSetSemEvent                            UserSetEvent
#define  UserResetSemEvent                          UserResetEvent
#define  UserWaitSemEvent                           UserWaitEvent
#define  UserPulseSemEvent                          UserPulseEvent


#endif
