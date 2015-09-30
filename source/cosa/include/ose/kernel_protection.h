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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        OSE 4.2

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        07/31/02    initial revision.

**********************************************************************/


#ifndef  _KERNEL_PROTECTION_
#define  _KERNEL_PROTECTION_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

typedef  SEMAPHORE                  *KERNEL_LOCK,   **PKERNEL_LOCK;

typedef  struct
_KERNEL_EVENT
{
    KERNEL_LOCK                     AccessLock;
    ULONG                           NumberSignaled;
    ULONG                           NumberWaiting;
}
KERNEL_EVENT,   *PKERNEL_EVENT;

#define  KERNEL_STATUS_SUCCESS                      0
#define  KERNEL_STATUS_TIMEOUT                      0x00000102L

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
BOOLEAN
KernelInitializeLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelInitializeLock(pLock)                ((*(pLock) = create_sem(1)) != NULL)

/*
VOID
KernelAcquireLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelAcquireLock(pLock)                   wait_sem(*(pLock))

/*
VOID
KernelReleaseLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelReleaseLock(pLock)                   signal_sem(*(pLock))

/*
VOID
KernelFreeLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelFreeLock(pLock)                      kill_sem(*(pLock))



BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pEvent
    );

VOID
KernelSetEvent
    (
        PKERNEL_EVENT               pEvent
    );

VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pEvent
    );

VOID
KernelPulseEvent
    (
        PKERNEL_EVENT               pEvent
    );

int
KernelWaitEvent
    (
        PKERNEL_EVENT               pEvent,
        ULONG                       ulMilliSeconds
    );

VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pEvent
    );

#endif
