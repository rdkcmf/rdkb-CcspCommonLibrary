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

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/


#ifndef  _KERNEL_PROTECTION_
#define  _KERNEL_PROTECTION_


/***********************************************************
       TEMPORARILY PUT SEMEPHORE DEFINITIONS HERE
***********************************************************/

typedef  tASSemaphore               KERNEL_SEMAPHORE;


/***********************************************************
            DEFINITIONS FOR LOCKING MECHANISM
***********************************************************/

/*
 *  switches
 */
#define  KERNEL_LOCK_USE_SEMAPHORE
/*#define  KERNEL_LOCK_TRACKING*/


typedef struct
_KERNEL_FREE_LOCK
{
    struct _KERNEL_FREE_LOCK    *Next;
    tASSemaphore                 semaLock;
}
KERNEL_FREE_LOCK, *PKERNEL_FREE_LOCK;

typedef     PKERNEL_FREE_LOCK     KERNEL_LOCK, *PKERNEL_LOCK;


#if defined(KERNEL_LOCK_USE_SEMAPHORE)

    BOOLEAN
    KernelRealInitializeLock
        (
            PKERNEL_LOCK                pLock,
            const char*                 pFileName,
            ULONG                       ulLineNumber
        );

    VOID
    KernelRealAcquireLock
        (
            PKERNEL_LOCK                pLock,
            const char*                 pFileName,
            ULONG                       ulLineNumber
        );

    VOID
    KernelRealReleaseLock
        (
            PKERNEL_LOCK                pLock,
            const char*                 pFileName,
            ULONG                       ulLineNumber
        );

    VOID
    KernelRealFreeLock
        (
            PKERNEL_LOCK                pLock,
            const char*                 pFileName,
            ULONG                       ulLineNumber
        );

    ULONG
    KernelGetLockCount
        (
            VOID
        );

    #ifdef   KERNEL_LOCK_TRACKING

        #define  KernelInitializeLock(pLock)            KernelRealInitializeLock(pLock, __FILE__, __LINE__)
        #define  KernelAcquireLock(pLock)               KernelRealAcquireLock(pLock, __FILE__, __LINE__)
        #define  KernelReleaseLock(pLock)               KernelRealReleaseLock(pLock, __FILE__, __LINE__)
        #define  KernelFreeLock(pLock)                  KernelRealFreeLock(pLock, __FILE__, __LINE__)

    #else

        #define  KernelInitializeLock(pLock)            KernelRealInitializeLock(pLock, NULL, 0)
        #define  KernelAcquireLock(pLock)               KernelRealAcquireLock(pLock, NULL, 0)
        #define  KernelReleaseLock(pLock)               KernelRealReleaseLock(pLock, NULL, 0)
        #define  KernelFreeLock(pLock)                  KernelRealFreeLock(pLock, NULL, 0)

    #endif

#else

    #define  KernelInitializeLock(pLock)                ((*pLock = 0) == 0)

    #define  KernelAcquireLock(pLock)                   *pLock = atmos_startcritical()

    #define  KernelReleaseLock(pLock)                   atmos_endcritical(*pLock)

    #define  KernelFreeLock(pLock)                      (*pLock = 0)

#endif


/***********************************************************
            DEFINITIONS FOR EVENT MECHANISM
***********************************************************/

typedef struct
_KERNEL_EVENT
{
    ULONG                           NumberSignaled;
    UCHAR                           Data[12];
}
KERNEL_EVENT, *PKERNEL_EVENT;

BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    );

VOID
KernelSetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    );

VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    );

VOID
KernelPulseEvent
    (
        PKERNEL_EVENT               pBaseEvent
    );

void
KernelWaitEvent
    (
        PKERNEL_EVENT               pBaseEvent,
        ULONG                       ulMilliSeconds
    );

VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    );


#endif
