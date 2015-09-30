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


#ifndef  _KERNEL_PROTECTION_
#define  _KERNEL_PROTECTION_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

__static_inline  BOOLEAN
KernelInitializeLock(PKERNEL_LOCK  pLock)
{
	sema_init(pLock, 1);
    return  TRUE;
}

__static_inline  VOID
KernelAcquireLock(PKERNEL_LOCK  pLock)
{
    down(pLock);
}

__static_inline  VOID
KernelReleaseLock(PKERNEL_LOCK  pLock)
{
    up(pLock);
}

__static_inline  VOID
KernelFreeLock(PKERNEL_LOCK  pLock)
{
    /*
     * this only worked in 2.2
     * *pLock = MUTEX;
     */
    ;
}

#define  KernelInitializeSpinLock  spin_lock_init
#define  KernelAcquireSpinLock(x)     do{ spin_lock_bh(x); /**x = jiffies;*/ } while(0)
#define  KernelReleaseSpinLock(x)     do{ /*if (jiffies-*x > 100){printk("%s:%s Spinlocked for more than a second \n",__BASE_FILE__,__FUNCTION__);}*/ spin_unlock(x); __local_bh_enable();} while(0)
#define  KernelFreeSpinLock
#define  SIGNALED               1
#define  UNSIGNALED             0

#ifdef _ANSC_LINUX_2_6_
#define KernelInitializeEvent(pEvent)                                   \
    do {                                                                \
        (pEvent)->wq.lock           = SPIN_LOCK_UNLOCKED;               \
        (pEvent)->wq.task_list.next = &(pEvent)->wq.task_list;          \
        (pEvent)->wq.task_list.prev = &(pEvent)->wq.task_list;          \
        (pEvent)->status.counter    = UNSIGNALED;                       \
    } while (0)
#else
#define KernelInitializeEvent(pEvent)                                   \
    do {                                                                \
        (pEvent)->wq.lock           = WAITQUEUE_RW_LOCK_UNLOCKED;       \
        (pEvent)->wq.task_list.next = &(pEvent)->wq.task_list;          \
        (pEvent)->wq.task_list.prev = &(pEvent)->wq.task_list;          \
        (pEvent)->status.counter    = UNSIGNALED;                       \
    } while (0)
#endif

#define KernelSetEvent(pEvent)                                          \
	do {                                                                \
    	atomic_set(&(pEvent)->status, SIGNALED);                        \
    	wake_up_interruptible(&(pEvent)->wq);                           \
    } while (0)

#define KernelResetEvent(pEvent)                                        \
	atomic_set(&(pEvent)->status, UNSIGNALED);

#define KernelPulseEvent(pEvent)                                        \
	do {                                                                \
    	wake_up_interruptible(&(pEvent)->wq);                           \
    	atomic_set(&(pEvent)->status, UNSIGNALED);                      \
    } while (0)

#define KernelWaitEvent(pEvent, ulMilliSeconds)                         \
	do {                                                                \
    	if (atomic_read(&(pEvent)->status) == UNSIGNALED )              \
        {                                                               \
        	if( 0 == ulMilliSeconds)                                    \
            {                                                           \
            	interruptible_sleep_on(&(pEvent)->wq);                  \
            } else {                                                    \
            	interruptible_sleep_on_timeout(&(pEvent)->wq, ulMilliSeconds/10);   \
            }                                                           \
        }                                                               \
    } while (0)

#define KernelFreeEvent

__static_inline BOOLEAN
KernelInitializeSemaphore(PKERNEL_SEMAPHORE pSemaphore, ULONG  ulInitialCount, ULONG  ulMaximumCount)
{
	sema_init(pSemaphore, ulInitialCount);
    return TRUE;
}

__static_inline VOID
KernelAcquireSemaphore(PKERNEL_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    /*
     * no time-out support
     */
    down(pSemaphore);
}

__static_inline BOOLEAN
KernelAcquireSemaphore2(PKERNEL_SEMAPHORE  pSemaphore, ULONG  ulMilliSeconds)
{
    /*
     * no time-out support
     */
    down(pSemaphore);
    return TRUE;
}

__static_inline VOID
KernelReleaseSemaphore(PKERNEL_SEMAPHORE  pSemaphore, ULONG  ulCount)
{
    while(ulCount--)
    {
        up(pSemaphore);
    }
}
#define KernelFreeSemaphore
#endif

