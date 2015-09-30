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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

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

#define  UserInitializeSpinLock                     UserInitializeLock
#define  UserFreeSpinLock                           UserFreeLock
#define  UserAcquireSpinLock                        UserAcquireLock
#define  UserReleaseSpinLock                        UserReleaseLock

/*
 *  prototype
 *
BOOLEAN
UserInitializeLock
    (
        PUSER_LOCK                  pLock
    );
 */
#define  UserInitializeLock(pLock)                  ((*(PUSER_LOCK)(pLock) = semBCreate(SEM_Q_PRIORITY, SEM_FULL)) != NULL)

/*
 *  prototype
 *
void
UserAcquireLock
    (
        PUSER_LOCK                  pLock
    );
 */
#define  UserAcquireLock(pLock)                     semTake(*(PUSER_LOCK)(pLock), WAIT_FOREVER)

/*
 *  prototype
 *
void
UserReleaseLock
    (
        PUSER_LOCK                  pLock
    );
 */
#define  UserReleaseLock(pLock)                     semGive(*(PUSER_LOCK)(pLock))

/*
 *  prototype
 *
void
UserFreeLock
    (
        PUSER_LOCK                  pLock
    );
 */
#define  UserFreeLock(pLock)                        semDelete(*(PUSER_LOCK)(pLock))

/*
 *  prototype
 *
BOOLEAN
UserInitializeEvent
    (
        PUSER_EVENT                 pEvent
    );
 */
#define  UserInitializeEvent(pEvent)                ((*(pEvent) = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) != NULL)

/*
 *  prototype
 *
void
UserSetEvent
    (
        PUSER_EVENT                 pEvent
    );
 */
#define  UserSetEvent(pEvent)                       semGive(*(pEvent))

/*
 *  prototype
 *
void
UserResetEvent
    (
        PUSER_EVENT                 pEvent
    );
 */
#define  UserResetEvent(pEvent)                     semGive(*(pEvent)); semTake(*(pEvent), WAIT_FOREVER)

/*
 *  prototype
 *
void
UserPulseEvent
    (
        PUSER_EVENT                 pEvent
    );
 */
#define  UserPulseEvent(pEvent)                     semFlush(*(pEvent))

/*
 *  prototype
 *
void
UserWaitEvent
    (
        PUSER_EVENT                 pEvent,
        ULONG                       ulMilliSeconds
    );
 */
#define  UserWaitEvent(pEvent, ulMilliSeconds)      semTake(*(pEvent), ((ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF)) ? WAIT_FOREVER : (sysClkRateGet()*(ulMilliSeconds))/1000)

/*
 *  prototype
 *
void
UserFreeEvent
    (
        PUSER_EVENT                 pEvent
    );
 */
#define  UserFreeEvent(pEvent)                      semDelete(*(pEvent))



/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  UserInitializeSemaphore(pSemaphore, ulInitialCount, ulMaximumCount)        ((*(PUSER_SEMAPHORE)(pSemaphore) = semCCreate(SEM_Q_PRIORITY, ulInitialCount)) != NULL)

#define  UserAcquireSemaphore(pSemaphore, ulMilliSeconds)                           semTake(*(PUSER_SEMAPHORE)(pSemaphore), ((ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF)) ? WAIT_FOREVER : (sysClkRateGet()*(ulMilliSeconds))/1000)


#define  UserReleaseSemaphore(pSemaphore, ulCount)                                  \
         {                                                                          \
            ULONG           i   = 0;                                                \
                                                                                    \
            for ( i = 0; i < (ulCount); i ++ )                                      \
            {                                                                       \
                semGive(*(PUSER_SEMAPHORE)(pSemaphore));                            \
            }                                                                       \
        }

#define  UserFreeSemaphore(pSemaphore)                                              semDelete(*(PUSER_SEMAPHORE)(pSemaphore))


#endif
