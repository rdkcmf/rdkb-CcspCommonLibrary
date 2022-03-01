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

    module:	ansc_protection.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines locking mechanism used in the
        ANSC and objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/02/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions

**********************************************************************/


#ifndef  _ANSC_PROTECTION_
#define  _ANSC_PROTECTION_

#include "user_protection.h"
/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */

    #define  AnscInitializeLock                     UserInitializeLock
    #define  AnscFreeLock                           UserFreeLock
    #define  AnscAcquireLock                        UserAcquireLock
    #define  AnscReleaseLock                        UserReleaseLock

    #define  AnscInitializeSpinLockWrapper          UserInitializeSpinLock
    #define  AnscFreeSpinLockWrapper                UserFreeSpinLock
    #define  AnscAcquireSpinLockWrapper             UserAcquireSpinLock
    #define  AnscReleaseSpinLockWrapper             UserReleaseSpinLock

    #define  AnscInitializeSemaphore                UserInitializeSemaphore
    #define  AnscFreeSemaphore                      UserFreeSemaphore
    #define  AnscAcquireSemaphore                   UserAcquireSemaphore
    #define  AnscAcquireSemaphore2                  UserAcquireSemaphore2
    #define  AnscReleaseSemaphore                   UserReleaseSemaphore

    #ifdef  _ANSC_SIMULATE_EVENT_

        #define  AnscInitializeEvent                AnscInitializeSimEvent
        #define  AnscFreeEvent                      AnscFreeSimEvent
        #define  AnscSetEvent                       AnscSetSimEvent
        #define  AnscResetEvent                     AnscResetSimEvent
        #define  AnscWaitEvent                      AnscWaitSimEvent
        #define  AnscPulseEvent                     AnscPulseSimEvent

    #else

        #define  AnscInitializeEvent                UserInitializeSemEvent
        #define  AnscFreeEvent                      UserFreeSemEvent
        #define  AnscSetEvent                       UserSetSemEvent
        #define  AnscResetEvent                     UserResetSemEvent
        #define  AnscWaitEvent                      UserWaitSemEvent
        #define  AnscPulseEvent                     UserPulseSemEvent

    #endif



#ifdef   _ANSC_SPINLOCK_PROFILING_

    #define  ANSC_SPINLOCK_PROFILING_CAP            0x1000000

    typedef  struct
    _ANSC_SPINLOCK_STRUCT
    {
        DOUBLE_LINK_ENTRY       Linkage;
        _ANSC_SPINLOCK_DEF      SpinLock;
        ULONG                   Flags;
        ULONG                   CurrTimestamp;
        ULONG                   CurrElapsedTime;
        ULONG                   LongestTime;
        ULONG                   TotalHits;
        ULONG                   TotalHitTime;
        char                    Name[32];
        int                     Line;
        char                    MaxHoldName[32];
        int                     MaxHoldLine;
    }
    ANSC_SPINLOCK,   *PANSC_SPINLOCK;

    #define  ACCESS_ANSC_SPINLOCK(p)                ACCESS_CONTAINER(p, ANSC_SPINLOCK, Linkage)

    #ifndef _ANSC_ENABLE_PROFILING_
        #error  _ANSC_ENABLE_PROFILING_ has to be defined, if _ANSC_SPINLOCK_PROFILING_ is defined.
    #endif

    ANSC_STATUS
    AnscInitializeSpinLock1
        (
            PANSC_SPINLOCK              pSpinLock,
            char*                       pFileName,
            ULONG                       ulLine
        );

    ANSC_STATUS
    AnscFreeSpinLock1
        (
            PANSC_SPINLOCK              pSpinLock
        );

    ANSC_STATUS
    AnscAcquireSpinLock1
        (
            PANSC_SPINLOCK              pSpinLock,
            char*                       pFileName,
            int                         lineno
        );

    ANSC_STATUS
    AnscReleaseSpinLock1
        (
            PANSC_SPINLOCK              pSpinLock,
            char*                       pFileName,
            int                         lineno
        );

    #define  AnscInitializeSpinLock(p)              AnscInitializeSpinLock1(p, __FILE__, __LINE__)
    #define  AnscFreeSpinLock(p)                    AnscFreeSpinLock1(p)
    #define  AnscAcquireSpinLock(p)                 AnscAcquireSpinLock1(p, __FILE__, __LINE__)
    #define  AnscReleaseSpinLock(p)                 AnscReleaseSpinLock1(p, __FILE__, __LINE__)

    ANSC_STATUS
    AnscSpinLockProfilingEnable
        (
            BOOLEAN                     bEnable
        );

    ANSC_STATUS
    AnscSpinLockProfilingPrint
        (
            void
        );

    void
    AnscSpinLockProfilingCleanup
        (
            void
        );

#else
    #define  AnscInitializeSpinLock                 AnscInitializeSpinLockWrapper
    #define  AnscFreeSpinLock                       AnscFreeSpinLockWrapper
    #define  AnscAcquireSpinLock                    AnscAcquireSpinLockWrapper
    #define  AnscReleaseSpinLock                    AnscReleaseSpinLockWrapper

    #define  AnscSpinLockProfilingPrint()
#endif


/***********************************************************
       TASK SAFE SYNCHRONIZATION AND RESOURCE SHARING
***********************************************************/

/*
 * Although the basic locking mechanism enables a wide variety of multi-tasking resource sharing
 * scenarios, it's still not powerful enough to accommodate the flexibility required by certain
 * ANSC objects; the Ansc Time Descriptor Object is a perfect example: the Time Descriptor Object
 * encapsulates three basic member service functions:
 *
 *      $Start  - To register the timer to the scheduler; called within the runtime
 *                context of the owner object.
 *      $Stop   - To remove the timer from the internal queue maintained by the same
 *                scheduler; called within the runtime context of the owner object.
 *      $Invoke - Called within the runtime context of the scheduler object when the
 *                timer expires.
 *
 * Following rules are mandated to ensure the proper functioning of the Ansc Timer Descriptor
 * Object (notice the better flexibility than native timers provided by most operating systems):
 *
 *      1. Both start() and stop() can be called from inside the invoke().
 *      2. The timer object itself can be removed (memory being cleaned and released)
 *         from inside the invoke().
 *      3. After stop() returns, no more invoke() can be called.
 *
 * To enforce rule (3), we have to synchronize the stop() and cancel() call by using a shared lock.
 * But if we synchornize all the calls with the basic acquire()/release() locking action pair, the
 * start()/stop() call from inside the invoke() will cause deadlock. To gracefully solve such a
 * problem (which is deeply rooted in the history of computer/hardware/software evolution) is one
 * the primary goals of FAT-HEAD 2.0 architecture. For now, we use a task-safe locking mechanism.
 */
typedef  struct
_ANSC_TS_LOCK
{
    BOOL                            bAcquired;
    ULONG                           ActiveTaskHandle;
    ULONG                           RefCount;
    ANSC_LOCK                       ExternalLock;
    ANSC_LOCK                       InternalLock;
}
ANSC_TS_LOCK,  *PANSC_TS_LOCK;

#define  AnscInitializeTsLock(lock)                                                         \
         {                                                                                  \
            (lock)->bAcquired        = FALSE;                                               \
            (lock)->ActiveTaskHandle = 0;                                                   \
            (lock)->RefCount         = 0;                                                   \
                                                                                            \
            AnscInitializeLock(&(lock)->ExternalLock);                                      \
            AnscInitializeLock(&(lock)->InternalLock);                                      \
         }

#define  AnscFreeTsLock(lock)                                                               \
         {                                                                                  \
            AnscFreeLock(&(lock)->ExternalLock);                                            \
            AnscFreeLock(&(lock)->InternalLock);                                            \
         }

#define  AnscAcquireTsLock(lock)                                                            \
         {                                                                                  \
            BOOL                    bToLock         = TRUE;                                 \
            ULONG                   ulCurTaskHandle = AnscGetCurTaskHandle();               \
                                                                                            \
            AnscAcquireLock(&(lock)->InternalLock);                                         \
                                                                                            \
            if ( (lock)->bAcquired )                                                        \
            {                                                                               \
                if ( (lock)->ActiveTaskHandle == ulCurTaskHandle )                          \
                {                                                                           \
                    bToLock = FALSE;                                                        \
                                                                                            \
                    (lock)->RefCount++;                                                     \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    bToLock = TRUE;                                                         \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                bToLock = TRUE;                                                             \
            }                                                                               \
                                                                                            \
            AnscReleaseLock(&(lock)->InternalLock);                                         \
                                                                                            \
            if ( bToLock )                                                                  \
            {                                                                               \
                AnscAcquireLock(&(lock)->ExternalLock);                                     \
                                                                                            \
                AnscAcquireLock(&(lock)->InternalLock);                                     \
                (lock)->bAcquired        = TRUE;                                            \
                (lock)->ActiveTaskHandle = ulCurTaskHandle;                                 \
                (lock)->RefCount         = 0;                                               \
                AnscReleaseLock(&(lock)->InternalLock);                                     \
            }                                                                               \
         }

#define  AnscReleaseTsLock(lock)                                                            \
         {                                                                                  \
            BOOL                    bToUnlock       = TRUE;                                 \
            ULONG                   ulCurTaskHandle = AnscGetCurTaskHandle();               \
                                                                                            \
            AnscAcquireLock(&(lock)->InternalLock);                                         \
                                                                                            \
            if ( (lock)->bAcquired )                                                        \
            {                                                                               \
                if ( (lock)->ActiveTaskHandle == ulCurTaskHandle )                          \
                {                                                                           \
                    if ( (lock)->RefCount > 0 )                                             \
                    {                                                                       \
                        (lock)->RefCount--;                                                 \
                                                                                            \
                        bToUnlock = FALSE;                                                  \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        bToUnlock = TRUE;                                                   \
                    }                                                                       \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    bToUnlock = FALSE;                                                      \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                bToUnlock = FALSE;                                                          \
            }                                                                               \
                                                                                            \
            AnscReleaseLock(&(lock)->InternalLock);                                         \
                                                                                            \
            if ( bToUnlock )                                                                \
            {                                                                               \
                AnscAcquireLock(&(lock)->InternalLock);                                     \
                (lock)->bAcquired        = FALSE;                                           \
                (lock)->ActiveTaskHandle = 0;                                               \
                (lock)->RefCount         = 0;                                               \
                AnscReleaseLock(&(lock)->InternalLock);                                     \
                                                                                            \
                AnscReleaseLock(&(lock)->ExternalLock);                                     \
            }                                                                               \
         }


/***********************************************************
     DEADLOCK SAFE SYNCHRONIZATION AND RESOURCE SHARING
***********************************************************/

/*
 * Although the basic locking mechanism enables a wide variety of multi-tasking resource sharing
 * scenarios, it's still not powerful enough to accommodate the flexibility required by certain
 * ANSC objects; the Tcp State Machine Object is a perfect example: the Tcp State Machine Object
 * employs four different timer objects:
 *
 *      $Retrainsmission Timer - responsible for retransmitting segments
 *      $Persist/Probing Timer - responsible for zero-window probing
 *      $Delayed/Waiting Timer - responsible for delayed-ack and serving wait queue
 *      $Keepalive Timer       - responsible for sending heartbeat signals
 *
 * To create a critical section for all the state-transition and segment processing, the Tcp State
 * Machine locks up this critical section whenever a segment is received from the network layer.
 * Different timer objects may be stopped/reset/restarted within the critical section, and such
 * operations are protected by timer's inherent lock. When timer fires (under the protection of
 * its inherent lock), it needs to update internal state information, hence requires the access
 * to the critical section. Bang, Deadlock!
 */
typedef  struct
_ANSC_DS_LOCK
{
    BOOL                            bAcquired;
    ANSC_LOCK                       ExternalLock;
    ANSC_LOCK                       InternalLock;
}
ANSC_DS_LOCK,  *PANSC_DS_LOCK;

#define  AnscInitializeDsLock(lock)                                                         \
         {                                                                                  \
            (lock)->bAcquired = FALSE;                                                      \
                                                                                            \
            AnscInitializeLock(&(lock)->ExternalLock);                                      \
            AnscInitializeLock(&(lock)->InternalLock);                                      \
         }

#define  AnscFreeDsLock(lock)                                                               \
         {                                                                                  \
            AnscFreeLock(&(lock)->ExternalLock);                                            \
            AnscFreeLock(&(lock)->InternalLock);                                            \
         }

#define  AnscAcquireDsLock(lock)                                                            \
         {                                                                                  \
            AnscAcquireLock(&(lock)->InternalLock);                                         \
            AnscAcquireLock(&(lock)->ExternalLock);                                         \
                                                                                            \
            (lock)->bAcquired = TRUE;                                                       \
            AnscReleaseLock(&(lock)->InternalLock);                                         \
         }

#define  AnscReleaseDsLock(lock)                                                            \
         {                                                                                  \
            (lock)->bAcquired = FALSE;                                                      \
            AnscReleaseLock(&(lock)->ExternalLock);                                         \
         }

#define  AnscTryLockDsLock(lock, result)                                                    \
         {                                                                                  \
            AnscAcquireLock(&(lock)->InternalLock);                                         \
                                                                                            \
            if ( (lock)->bAcquired )                                                        \
            {                                                                               \
                result = FALSE;                                                             \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                result = TRUE;                                                              \
            }                                                                               \
                                                                                            \
            if ( result )                                                                   \
            {                                                                               \
                AnscAcquireLock(&(lock)->ExternalLock);                                     \
                                                                                            \
                (lock)->bAcquired = TRUE;                                                   \
            }                                                                               \
                                                                                            \
            AnscReleaseLock(&(lock)->InternalLock);                                         \
         }


/***********************************************************
        SIMULATE EVENT BY USING LOCKS AND TASK QUEUE
***********************************************************/

/*
 * Most embedded operating systems don't support EVENT. Simply using semaphore/lock to simulate the
 * event mechanism is not enough. For example: the event-handling mechanism should allow multiple
 * tasks to wait on the same event. When the event is signalled, all waiting tasks MUST be waken up.
 * Here, we use ANSC_LOCKs and task queue to simulate the desired event-handling.
 */
typedef  struct
_ANSC_SIM_EVENT
{
    BOOL                            bSignalled;
    ANSC_SPINLOCK                   StateSpinLock;
    ANSC_SEMAPHORE                  StateSemaphore;
    ULONG                           StateCounter;
}
ANSC_SIM_EVENT,  *PANSC_SIM_EVENT;


#endif
