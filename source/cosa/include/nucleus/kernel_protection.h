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
#if 0
__inline  BOOLEAN
KernelInitializeLock(PKERNEL_LOCK  pLock)
{
    char	sema_name[NU_MAX_NAME_SIZE];
	STATUS  status = NU_SUCCESS;

	status = NU_Create_Semaphore(pLock, (CHAR *)sema_name, 1, NU_FIFO);

    return  (status == NU_SUCCESS);
}

__inline  VOID
KernelAcquireLock(PKERNEL_LOCK  pLock)
{
    NU_Obtain_Semaphore(pLock, NU_SUSPEND);
}

__inline  VOID
KernelReleaseLock(PKERNEL_LOCK  pLock)
{
    NU_Release_Semaphore(pLock);
}

__inline  VOID
KernelFreeLock(PKERNEL_LOCK  pLock)
{
    NU_Delete_Semaphore(pLock);
}

__inline  BOOLEAN
KernelInitializeEvent(PKERNEL_EVENT  pEvent)
{
	CHAR	event_name[NU_MAX_NAME_SIZE];
	STATUS	status = NU_SUCCESS;

    status = NU_Create_Event_Group(pEvent, (CHAR *)event_name);

	return  ( status == NU_SUCCESS );
}

__inline  VOID
KernelSetEvent(PKERNEL_EVENT  pEvent)
{
    NU_Set_Events(pEvent, 0x00000001, NU_OR);
}

__inline  VOID
KernelResetEvent(PKERNEL_EVENT  pEvent)
{
    NU_Set_Events(pEvent, 0x00000000, NU_AND);
}

__inline  VOID
KernelPulseEvent(PKERNEL_EVENT  pEvent)
{
    NU_Set_Events(pEvent, 0x00000001, NU_OR);
	NU_Set_Events(pEvent, 0x00000000, NU_AND);
}

__inline  int
KernelWaitEvent(PKERNEL_EVENT  pEvent, ULONG  ulMilliSeconds)
{
	ULONG		retrieved;
    if ( ulMilliSeconds == 0 )
    {
        return  NU_Retrieve_Events(pEvent, 0x00000001, NU_AND, &retrieved, NU_SUSPEND);
    }
    else if ( ((ulMilliSeconds / SYSTEM_OS_TIME_PER_TICK) / 10) < 1 )
	{
		return  NU_Retrieve_Events(pEvent, 0x00000001, NU_AND, &retrieved, 1);
	}
	else
    {
        return  NU_Retrieve_Events(pEvent, 0x00000001, NU_AND, &retrieved, (ulMilliSeconds / SYSTEM_OS_TIME_PER_TICK) / 10 );
    }
}

__inline  VOID
KernelFreeEvent(PKERNEL_EVENT  pEvent)
{
    NU_Delete_Event_Group(pEvent);
}

#endif
#endif







