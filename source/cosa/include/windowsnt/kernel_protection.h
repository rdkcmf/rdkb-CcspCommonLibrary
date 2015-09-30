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

__inline  BOOLEAN
KernelInitializeLock(PKERNEL_LOCK  pLock)
{
    *pLock = CreateSemaphore(NULL, 1, 1, NULL);

    if ( *pLock == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  VOID
KernelAcquireLock(PKERNEL_LOCK  pLock)
{
    WaitForSingleObject(*pLock, INFINITE);
}

__inline  VOID
KernelReleaseLock(PKERNEL_LOCK  pLock)
{
    ReleaseSemaphore(*pLock, 1, NULL);
}

__inline  VOID
KernelFreeLock(PKERNEL_LOCK  pLock)
{
    CloseHandle(*pLock);
}

__inline  BOOLEAN
KernelInitializeEvent(PKERNEL_EVENT  pEvent)
{
    *pEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if ( *pEvent == NULL )
    {
        return  FALSE;
    }

    return  TRUE;
}

__inline  VOID
KernelSetEvent(PKERNEL_EVENT  pEvent)
{
    SetEvent(*pEvent);
}

__inline  VOID
KernelResetEvent(PKERNEL_EVENT  pEvent)
{
    ResetEvent(*pEvent);
}

__inline  VOID
KernelPulseEvent(PKERNEL_EVENT  pEvent)
{
    PulseEvent(*pEvent);
}

__inline  int
KernelWaitEvent(PKERNEL_EVENT  pEvent, ULONG  ulMilliSeconds)
{
    if ( ulMilliSeconds == 0 )
    {
        return  WaitForSingleObject(*pEvent, INFINITE);
    }
    else
    {
        return  WaitForSingleObject(*pEvent, ulMilliSeconds);
    }
}

__inline  VOID
KernelFreeEvent(PKERNEL_EVENT  pEvent)
{
    CloseHandle(*pEvent);
}


#endif
