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

    module:	ansc_protection.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to protection and task
        synchronization.

        *   AnscInitializeSpinLock1
        *   AnscFreeSpinLock1
        *   AnscAcquireSpinLock1
        *   AnscReleaseSpinLock1
        *   AnscSpinLockProfilingEnable
        *   AnscSpinLockProfilingPrint

        *   AnscInitializeSimEvent
        *   AnscFreeSimEvent
        *   AnscWaitSimEvent
        *   AnscSetSimEvent
        *   AnscResetSimEvent
        *   AnscPulseSimEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/22/03    initial revision.

**********************************************************************/


#include "ansc_global.h"
#include "ansc_profiling.h"


/**********************************************************************
                        SPIN LOCK PROFILING
**********************************************************************/

#ifdef   _ANSC_SPINLOCK_PROFILING_

static   BOOLEAN                                gAnscSpinLockProfilingInit  = FALSE;
static   BOOLEAN                                gbAnscSpinLockProfiling     = FALSE;
static   _ANSC_SPINLOCK_DEF                     gAnscSpinLockProfilingLock;
static   DLIST_HEADER                           gAnscSpinLockProfilingList;
static   DLIST_HEADER                           gAnscFreedSpinLockProfilingList;

static
const char *
GetFileName
    (
        const char *                pPathName
    )
{
    const char *                    p;

    for( p = pPathName + AnscSizeOfString(pPathName); p > pPathName; p -- )
    {
        if( *(p-1) == '/' || *(p-1) == '\\' )
            break;
    }

    return p;
}


ANSC_STATUS
AnscInitializeSpinLock1
    (
        PANSC_SPINLOCK              pSpinLock,
        char*                       pFileName,
        ULONG                       ulLine
    )
{
    if ( !gAnscSpinLockProfilingInit )
    {
        AnscProfilingStartPcc();

        AnscInitializeSpinLockWrapper(&gAnscSpinLockProfilingLock);
        AnscDListInitializeHeader    (&gAnscSpinLockProfilingList);
        AnscDListInitializeHeader    (&gAnscFreedSpinLockProfilingList);
        gAnscSpinLockProfilingInit = TRUE;
    }

    AnscInitializeSpinLockWrapper(&pSpinLock->SpinLock);

    pSpinLock->Flags                = 0;
    _ansc_strncpy(pSpinLock->Name, GetFileName(pFileName), sizeof(pSpinLock->Name));
    pSpinLock->Name[sizeof(pSpinLock->Name)-1] = '\0';
    pSpinLock->Line                 = ulLine;
    pSpinLock->MaxHoldName[0]       = '\0';
    pSpinLock->MaxHoldLine          = 0;
    pSpinLock->CurrTimestamp        = 0;
    pSpinLock->CurrElapsedTime      = 0;
    pSpinLock->LongestTime          = 0;
    pSpinLock->TotalHits            = 0;
    pSpinLock->TotalHitTime         = 0;

    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);
    AnscDListPushEntryRight(&gAnscSpinLockProfilingList, &pSpinLock->Linkage);
    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscFreeSpinLock1
    (
        PANSC_SPINLOCK              pSpinLock
    )
{
    PANSC_SPINLOCK                  pFreedLock;

    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);
    AnscDListPopEntryByLink(&gAnscSpinLockProfilingList, &pSpinLock->Linkage);
    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);
    
    AnscFreeSpinLockWrapper(&pSpinLock->SpinLock);
    
    /* 
     * save each spin lock even it is freed 
     */
    pFreedLock = AnscAllocateMemory(sizeof(ANSC_SPINLOCK));
    if( ! pFreedLock )
    {
        AnscTraceWarning(("!!! Spin Lock Profiling: memory ran out.\n"));
        return ANSC_STATUS_SUCCESS;
    }

    _ansc_memcpy(pFreedLock, pSpinLock, sizeof(ANSC_SPINLOCK));
    pFreedLock->Flags = 1;

    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);
    AnscDListPushEntryRight(&gAnscFreedSpinLockProfilingList, &pFreedLock->Linkage);
    if( AnscDListQueryDepth(&gAnscFreedSpinLockProfilingList) > 1000 )
    {
        PDOUBLE_LINK_ENTRY      pLink;
        pLink = AnscDListPopEntryLeft(&gAnscFreedSpinLockProfilingList);
        pFreedLock = ACCESS_CONTAINER(pLink, ANSC_SPINLOCK, Linkage);
    }
    else
    {   
        pFreedLock = NULL;
    }
    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);

    if( pFreedLock )
    {
        AnscFreeMemory(pFreedLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscAcquireSpinLock1
    (
        PANSC_SPINLOCK              pSpinLock,
        char*                       pFileName,
        int                         iLineNo
    )
{
    AnscAcquireSpinLockWrapper(&pSpinLock->SpinLock);

    if ( gbAnscSpinLockProfiling )
    {
        pSpinLock->CurrTimestamp    = AnscProfilingReadPcc();
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscReleaseSpinLock1
    (
        PANSC_SPINLOCK              pSpinLock,
        char*                       pFileName,
        int                         iLineNo
    )
{
    BOOLEAN                         bMax = FALSE;

    if ( gbAnscSpinLockProfiling )
    {
        pSpinLock->CurrElapsedTime = AnscProfilingReadPcc() - pSpinLock->CurrTimestamp;

        if ( pSpinLock->CurrElapsedTime > pSpinLock->LongestTime )
        {
            pSpinLock->LongestTime  = pSpinLock->CurrElapsedTime;
            bMax = TRUE;
            pSpinLock->MaxHoldLine  = iLineNo;
        }

        pSpinLock->TotalHits    ++;
        if( pSpinLock->TotalHitTime + pSpinLock->CurrElapsedTime < pSpinLock->TotalHitTime )
        {
            AnscTraceError(("!!! Spin Lock total time overflow @ [%s]--<%d>\n", pFileName, iLineNo));
        }
        pSpinLock->TotalHitTime += pSpinLock->CurrElapsedTime;
    }

    AnscReleaseSpinLockWrapper(&pSpinLock->SpinLock);

    if ( gbAnscSpinLockProfiling && bMax )
    {
        _ansc_strncpy(pSpinLock->MaxHoldName, GetFileName(pFileName), sizeof(pSpinLock->MaxHoldName));
        pSpinLock->MaxHoldName[sizeof(pSpinLock->MaxHoldName)-1] = '\0';
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscSpinLockProfilingEnable
    (
        BOOLEAN                     bEnable
    )
{
    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);

    gbAnscSpinLockProfiling = bEnable;

    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);

    return  ANSC_STATUS_SUCCESS;
}

#define OUTPUT_SPINLOCK_PROFILE(pLock)                                          \
        AnscTraceError                                                          \
            ((                                                                  \
                "%06d  %1d  %8u  %10u  %6u  %8u  %8u  %24s  %4d  %24s  %4d\n",  \
                ulIndex ++ ,                                                    \
                (pLock)->Flags,                                                 \
                (pLock)->TotalHits,                                             \
                (pLock)->TotalHitTime,                                          \
                (pLock)->TotalHits ? (pLock)->TotalHitTime/(pLock)->TotalHits : 0,  \
                (pLock)->CurrElapsedTime,                                       \
                (pLock)->LongestTime,                                           \
                (pLock)->Name,                                                  \
                (pLock)->Line,                                                  \
                (pLock)->MaxHoldName[0] ? (pLock)->MaxHoldName : "NA",          \
                (pLock)->MaxHoldLine                                            \
            ))

ANSC_STATUS
AnscSpinLockProfilingPrint
    (
        void
    )
{
    PDOUBLE_LINK_ENTRY              pEntry      = NULL;
    PANSC_SPINLOCK                  pSpinLock   = NULL;
    ULONG                           ulIndex     = 1;

    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);
    AnscTraceError
        ((
            "Total number of spinlocks: %d\n", 
            AnscDListQueryDepth(&gAnscSpinLockProfilingList) + AnscDListQueryDepth(&gAnscFreedSpinLockProfilingList)
        ));
    AnscTraceError
        ((
            "Total number of spinlocks that were freed: %d\n", 
            AnscDListQueryDepth(&gAnscFreedSpinLockProfilingList)
        ));

    AnscTraceError(("  No  Flags  Hits  Total  Avg   Cur   Max  InitName  InitLine  MaxName  MaxLine \n"));

    pEntry = AnscDListGetHeadEntryLeft(&gAnscSpinLockProfilingList);
    while ( pEntry )
    {
        pSpinLock = ACCESS_ANSC_SPINLOCK(pEntry);
        OUTPUT_SPINLOCK_PROFILE(pSpinLock);
        pEntry    = AnscDListGetEntryRight(pEntry);
    }

    pEntry = AnscDListGetHeadEntryLeft(&gAnscFreedSpinLockProfilingList);
    while ( pEntry )
    {
        pSpinLock = ACCESS_ANSC_SPINLOCK(pEntry);
        OUTPUT_SPINLOCK_PROFILE(pSpinLock);
        pEntry    = AnscDListGetEntryRight(pEntry);
    }

    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);

    return  ANSC_STATUS_SUCCESS;
}

void 
AnscSpinLockProfilingCleanup
    (
        void
    )
{
    PDOUBLE_LINK_ENTRY              pEntry;
    PANSC_SPINLOCK                  pSpinLock;

    AnscAcquireSpinLockWrapper(&gAnscSpinLockProfilingLock);
    
    pEntry = AnscDListGetHeadEntryLeft(&gAnscSpinLockProfilingList);
    while ( pEntry )
    {
        pSpinLock = ACCESS_ANSC_SPINLOCK(pEntry);    
        pEntry    = AnscDListGetEntryRight(pEntry);

        AnscAcquireSpinLockWrapper(&pSpinLock->SpinLock);

        pSpinLock->MaxHoldName[0]       = '\0';
        pSpinLock->MaxHoldLine          = 0;
        pSpinLock->CurrTimestamp        = 0;
        pSpinLock->CurrElapsedTime      = 0;
        pSpinLock->LongestTime          = 0;
        pSpinLock->TotalHits            = 0;
        pSpinLock->TotalHitTime         = 0;

        AnscReleaseSpinLockWrapper(&pSpinLock->SpinLock);
    }

    pEntry = AnscDListGetHeadEntryLeft(&gAnscFreedSpinLockProfilingList);
    while ( pEntry )
    {
        pSpinLock = ACCESS_ANSC_SPINLOCK(pEntry);    
        pEntry    = AnscDListGetEntryRight(pEntry);

        AnscFreeMemory(pSpinLock);    
    }
    AnscDListInitializeHeader    (&gAnscFreedSpinLockProfilingList);
    
    AnscReleaseSpinLockWrapper(&gAnscSpinLockProfilingLock);
}

#endif /* _ANSC_SPINLOCK_PROFILING_ */

/**********************************************************************
                        EVENT SIMULATION
**********************************************************************/

BOOL
AnscInitializeSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    )
{
    PANSC_SIM_EVENT                 sim_event = (PANSC_SIM_EVENT)AnscAllocateMemory(sizeof(ANSC_SIM_EVENT));

    if ( !sim_event )
    {
        *phSimEvent = (ANSC_HANDLE)NULL;

        return  FALSE;
    }

    sim_event->bSignalled   = FALSE;
    sim_event->StateCounter = 0;

    AnscInitializeSpinLock (&sim_event->StateSpinLock          );
    AnscInitializeSemaphore(&sim_event->StateSemaphore, 0, 1024);

    *phSimEvent = (ANSC_HANDLE)sim_event;

    return  TRUE;
}


void
AnscFreeSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    )
{
    PANSC_SIM_EVENT                 sim_event = (PANSC_SIM_EVENT)*phSimEvent;

    if ( !sim_event )
    {
        return;
    }

    AnscSetSimEvent((ANSC_HANDLE*)&sim_event);

    AnscFreeSpinLock (&sim_event->StateSpinLock );
    AnscFreeSemaphore(&sim_event->StateSemaphore);

    AnscFreeMemory(sim_event);

    return;
}


int
AnscWaitSimEvent
    (
        ANSC_HANDLE*                phSimEvent,
        ULONG                       milli_seconds
    )
{
    PANSC_SIM_EVENT                 sim_event = (PANSC_SIM_EVENT)*phSimEvent;

    if ( !sim_event )
    {
        return  -1;
    }

    AnscAcquireSpinLock(&sim_event->StateSpinLock);

    if ( sim_event->bSignalled )
    {
        AnscReleaseSpinLock(&sim_event->StateSpinLock);

        return  1;
    }

    sim_event->StateCounter++;

    AnscReleaseSpinLock(&sim_event->StateSpinLock);

    AnscAcquireSemaphore(&sim_event->StateSemaphore, milli_seconds);

    {
        AnscAcquireSpinLock(&sim_event->StateSpinLock);
        if ( sim_event->StateCounter > 0 )
        {
            sim_event->StateCounter--;
        }
        AnscReleaseSpinLock(&sim_event->StateSpinLock);
    }

    return  0;
}


void
AnscSetSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    )
{
    PANSC_SIM_EVENT                 sim_event       = (PANSC_SIM_EVENT)*phSimEvent;
    ULONG                           ulWaitTaskCount = 0;

    if ( !sim_event )
    {
        return;
    }

    AnscAcquireSpinLock(&sim_event->StateSpinLock);
    ulWaitTaskCount         = sim_event->StateCounter;
    sim_event->bSignalled   = TRUE;
    sim_event->StateCounter = 0;
    AnscReleaseSpinLock(&sim_event->StateSpinLock);

    /*
     * On some OSes, ReleaseSemaphore doesn't take parameters...
     */
    if ( ulWaitTaskCount > 0 )
    {
        AnscReleaseSemaphore(&sim_event->StateSemaphore, AnscGetMin2(ulWaitTaskCount, 1024));
    }

    return;
}


void
AnscResetSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    )
{
    PANSC_SIM_EVENT                 sim_event = (PANSC_SIM_EVENT)*phSimEvent;

    if ( !sim_event )
    {
        return;
    }

    if ( !sim_event->bSignalled )
    {
        return;
    }

    AnscAcquireSpinLock(&sim_event->StateSpinLock);
    sim_event->bSignalled = FALSE;
    AnscReleaseSpinLock(&sim_event->StateSpinLock);

    return;
}


void
AnscPulseSimEvent
    (
        ANSC_HANDLE*                phSimEvent
    )
{
    PANSC_SIM_EVENT                 sim_event = (PANSC_SIM_EVENT)*phSimEvent;

    if ( !sim_event )
    {
        return;
    }

    AnscSetSimEvent  ((ANSC_HANDLE*)&sim_event);
    AnscResetSimEvent((ANSC_HANDLE*)&sim_event);

    return;
}



