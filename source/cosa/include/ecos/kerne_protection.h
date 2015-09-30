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

    module:	kernel_protection.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        eCos 1.3.1

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        05/01/06    initial revision.

**********************************************************************/


#ifndef  _KERNEL_PROTECTION_
#define  _KERNEL_PROTECTION_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  KernelInitializeSpinLock(p)
#define  KernelAcquireSpinLock(p)                   cyg_scheduler_lock()
#define  KernelReleaseSpinLock(p)                   cyg_scheduler_unlock()
#define  KernelFreeSpinLock(p)


/*
 *  prototype
 *
BOOLEAN
KernelInitializeLock
    (
        PKERNEL_LOCK                  pLock
    );
 */
#define  KernelInitializeLock(pLock)                  cyg_semaphore_init(pLock, 1)

/*
 *  prototype
 *
void
KernelAcquireLock
    (
        PKERNEL_LOCK                  pLock
    );
 */
#define  KernelAcquireLock(pLock)                     cyg_semaphore_wait(pLock)

/*
 *  prototype
 *
void
KernelReleaseLock
    (
        PKERNEL_LOCK                  pLock
    );
 */
#define  KernelReleaseLock(pLock)                     cyg_semaphore_post(pLock)

/*
 *  prototype
 *
void
KernelFreeLock
    (
        PKERNEL_LOCK                  pLock
    );
 */
#define  KernelFreeLock(pLock)                        cyg_semaphore_destroy(pLock)

/*
 *  prototype
 *
BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT                 pEvent
    );
 */
#define  KernelInitializeEvent(pEvent)                #error must use simulated event!

/*
 *  prototype
 *
void
KernelSetEvent
    (
        PKERNEL_EVENT                 pEvent
    );
 *
 */
#define  KernelSetEvent(pEvent)                       #error must use simulated event!

/*
 *  prototype
 *
void
KernelResetEvent
    (
        PKERNEL_EVENT                 pEvent
    );
 */
#define  KernelResetEvent(pEvent)                     #error must use simulated event!

/*
 *  prototype
 *
void
KernelPulseEvent
    (
        PKERNEL_EVENT                 pEvent
    );
 */
#define  KernelPulseEvent(pEvent)                     #error must use simulated event!

/*
 *  prototype
 *
void
KernelWaitEvent
    (
        PKERNEL_EVENT                 pEvent,
        ULONG                       ulMilliSeconds
    );
 */
#define  KernelWaitEvent(pEvent, ulMilliSeconds)      #error must use simulated event!

/*
 *  prototype
 *
void
KernelFreeEvent
    (
        PKERNEL_EVENT                 pEvent
    );
 */
#define  KernelFreeEvent(pEvent)                      #error must use simulated event!


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  KernelInitializeSemaphore(pSemaphore, ulInitialCount, ulMaximumCount)      \
            cyg_semaphore_init(pSemaphore, ulInitialCount)

#define  KernelAcquireSemaphore(pSemaphore, ulMilliSeconds)                         \
            cyg_semaphore_timed_wait                                                \
                (                                                                   \
                    pSemaphore,                                                     \
                    ((ulMilliSeconds == 0) || (ulMilliSeconds == 0xFFFFFFFF))       \
                        ? WAIT_FOREVER                                              \
                        : (cyg_current_time() + (ulMilliSeconds / KERNEL_MILLISECONDS_PER_TICK))    \
                )

#define  KernelAcquireSemaphore2                        KernelAcquireSemaphore

#define  KernelReleaseSemaphore(pSemaphore, ulCount)                                \
         {                                                                          \
            ULONG           i   = 0;                                                \
                                                                                    \
            for ( i = 0; i < (ulCount); i ++ )                                      \
            {                                                                       \
                cyg_semaphore_post(pSemaphore);                                     \
            }                                                                       \
        }

#define  KernelFreeSemaphore(pSemaphore)                                            cyg_semaphore_destroy(pSemaphore)


#endif
