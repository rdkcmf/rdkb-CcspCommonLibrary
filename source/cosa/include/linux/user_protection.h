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
        09/11/03    add semaphore support

**********************************************************************/


#ifndef  _USER_PROTECTION_
#define  _USER_PROTECTION_

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/
#if 0
BOOLEAN
UserInitializeLock(PUSER_LOCK  pLock);

VOID
UserAcquireLock(PUSER_LOCK  pLock);

VOID
UserReleaseLock(PUSER_LOCK  pLock);

VOID
UserFreeLock(PUSER_LOCK  pLock);
#endif
#define UserInitializeLock(lock)            pthread_mutex_init(lock, NULL)
#define UserAcquireLock                     pthread_mutex_lock
#define UserReleaseLock                     pthread_mutex_unlock
#define UserFreeLock                        pthread_mutex_destroy

#define  UserInitializeSpinLock(lock)       UserInitializeLock(lock)
#define  UserFreeSpinLock                   UserFreeLock
#define  UserAcquireSpinLock                UserAcquireLock
#define  UserReleaseSpinLock                UserReleaseLock


VOID
UserInitializeEvent(PUSER_EVENT pEvent);

VOID
UserSetEvent(PUSER_EVENT pEvent);

VOID
UserResetEvent(PUSER_EVENT pEvent);

VOID
UserPulseEvent(PUSER_EVENT pEvent);

VOID
UserWaitEvent(PUSER_EVENT pEvent, ULONG ulMilliSeconds);

VOID
UserFreeEvent(PUSER_EVENT pEvent);

BOOLEAN UserInitializeSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulInitialCount, ULONG  ulMaximumCount);

VOID UserAcquireSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds);

VOID UserReleaseSemaphore(PUSER_SEMAPHORE  pSemaphore, ULONG  ulCount);

VOID UserFreeSemaphore(PUSER_SEMAPHORE  pSemaphore);

VOID
UserInitializeSemEvent(PSEM_EVENT *pEvent);

VOID
UserSetSemEvent(PSEM_EVENT *pEvent);

VOID
UserResetSemEvent(PSEM_EVENT *pEvent);

VOID
UserPulseSemEvent(PSEM_EVENT *pEvent);

VOID
UserWaitSemEvent(PSEM_EVENT *pEvent, ULONG ulMilliSeconds);

VOID
UserFreeSemEvent(PSEM_EVENT *pEvent);


#endif

