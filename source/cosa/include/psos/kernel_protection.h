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

        pSOS 2.5

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
		10/15/01	pSOS port

**********************************************************************/


#ifndef  _KERNEL_PROTECTION_
#define  _KERNEL_PROTECTION_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

BOOLEAN
KernelInitializeLock
    (
        PKERNEL_LOCK                pLock
    );


/*
 *  prototype
 *
VOID
KernelAcquireLock
    (
        PKERNEL_LOCK                pLock
    );
 *
 *  utilizing semaphore -- wait forever
 */
#define  KernelAcquireLock(pLock)                   sm_p(*pLock, SM_WAIT, 0)


/*
 *  prototype
 *
VOID
KernelReleaseLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelReleaseLock(pLock)                   sm_v(*pLock)


/*
 *  prototype
 *
VOID
KernelFreeLock
    (
        PKERNEL_LOCK                pLock
    );
 */
#define  KernelFreeLock(pLock)                      sm_delete(*pLock)



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


/*
 *  prototype
 *
VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pEvent
    );
 */
#define  KernelResetEvent(pEvent)


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


/*
 *  prototype
 *
VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pEvent
    );
 */
#define  KernelFreeEvent(pEvent)                    q_delete(*pEvent)


#endif
