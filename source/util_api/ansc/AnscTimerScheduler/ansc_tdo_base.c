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

    module:	ansc_tdo_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Timer Descriptor Object.

        *   AnscTdoCreate
        *   AnscTdoRemove
        *   AnscTdoEnrollObjects
        *   AnscTdoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/01    initial revision.

**********************************************************************/


#include "ansc_tso_global.h"


#ifdef _ANSC_TDO_PROFILING_

/*
 * global data for profiling
 */

static   BOOLEAN                                gAnscTdoProfilingInit  = FALSE;
static   BOOLEAN                                gbAnscTdoProfiling     = FALSE;
static   ANSC_SPINLOCK                          gAnscTdoProfilingLock;
static   DLIST_HEADER                           gAnscTdoProfilingList;


/*
 * static funciton:
 */
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

#endif /* _ANSC_TDO_PROFILING_ */


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscTdoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Timer Descriptor Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

#if ! defined(_ANSC_SPINLOCK_PROFILING_) && ! defined(_ANSC_TDO_PROFILING_)
ANSC_HANDLE
AnscTdoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
#else /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
ANSC_HANDLE
AnscTdoCreate2
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved,
        char *                      pFileName,
        int                         iLineNo
    )
#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscAllocateMemory(sizeof(ANSC_TIMER_DESCRIPTOR_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, ANSC_TIMER_DESCRIPTOR_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_TIMER_DESCRIPTOR_OID;

#if ! defined(_ANSC_SPINLOCK_PROFILING_) && ! defined(_ANSC_TDO_PROFILING_)
    pBaseObject->Create            = AnscTdoCreate;
    pBaseObject->Initialize        = AnscTdoInitialize;
#else /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
    pBaseObject->Create            = NULL;
    pBaseObject->Initialize        = NULL;
#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */

    pBaseObject->Remove            = AnscTdoRemove;
    pBaseObject->EnrollObjects     = AnscTdoEnrollObjects;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);

#if ! defined(_ANSC_SPINLOCK_PROFILING_) && ! defined(_ANSC_TDO_PROFILING_)
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);
#else /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
    AnscTdoInitialize2(pBaseObject, pFileName, iLineNo);
#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;
    PANSC_TIMER_SCHEDULER_OBJECT    pScheduler   = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hScheduler;

    pMyObject->Stop((ANSC_HANDLE)pMyObject);

    if ( pScheduler && !pMyObject->hContainerContext )
    {
        pScheduler->Remove((ANSC_HANDLE)pScheduler);
    }

#ifdef _ANSC_TDO_PROFILING_
    AnscAcquireSpinLock(&gAnscTdoProfilingLock);
    AnscDListPopEntryByLink(&gAnscTdoProfilingList, &pMyObject->TdoProfileLink);
    AnscReleaseSpinLock(&gAnscTdoProfilingLock);
#endif /* _ANSC_TDO_PROFILING_ */

    AnscFreeSpinLock(&pMyObject->AccessSpinLock);
    AnscCoRemove    ((ANSC_HANDLE)pMyObject    );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

#if ! defined(_ANSC_SPINLOCK_PROFILING_) && ! defined(_ANSC_TDO_PROFILING_)
ANSC_STATUS
AnscTdoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
#else /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
ANSC_STATUS
AnscTdoInitialize2
    (
        ANSC_HANDLE                 hThisObject,
        char *                      pFileName,
        int                         iLineNo
    )
#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;
    PANSC_TIMER_SCHEDULER_OBJECT    pScheduler   = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hScheduler;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid            = ANSC_TIMER_DESCRIPTOR_OID;

#if ! defined(_ANSC_SPINLOCK_PROFILING_) && ! defined(_ANSC_TDO_PROFILING_)
    pMyObject->Create         = AnscTdoCreate;
    pMyObject->Initialize     = AnscTdoInitialize;
#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */

    pMyObject->Remove         = AnscTdoRemove;
    pMyObject->EnrollObjects  = AnscTdoEnrollObjects;

    pMyObject->hScheduler     = (ANSC_HANDLE)NULL;
    pMyObject->hClient        = (ANSC_HANDLE)NULL;
    pMyObject->StartTime      = AnscGetTickInSeconds();
    pMyObject->Timestamp      = AnscGetTickInMilliSeconds();
    pMyObject->TimerType      = ANSC_TIMER_TYPE_PERIODIC;
    pMyObject->TimerMode      = 0;
    pMyObject->Interval       = 10;
    pMyObject->TimeToFire     = AnscGetTickInMilliSeconds();
    pMyObject->CallCounter    = 0;
    pMyObject->bScheduled     = FALSE;

    pMyObject->GetScheduler   = AnscTdoGetScheduler;
    pMyObject->SetScheduler   = AnscTdoSetScheduler;
    pMyObject->GetClient      = AnscTdoGetClient;
    pMyObject->SetClient      = AnscTdoSetClient;
    pMyObject->GetStartTime   = AnscTdoGetStartTime;
    pMyObject->SetStartTime   = AnscTdoSetStartTime;
    pMyObject->GetTimestamp   = AnscTdoGetTimestamp;
    pMyObject->SetTimestamp   = AnscTdoSetTimestamp;
    pMyObject->GetTimerType   = AnscTdoGetTimerType;
    pMyObject->SetTimerType   = AnscTdoSetTimerType;
    pMyObject->GetTimerMode   = AnscTdoGetTimerMode;
    pMyObject->SetTimerMode   = AnscTdoSetTimerMode;
    pMyObject->GetInterval    = AnscTdoGetInterval;
    pMyObject->SetInterval    = AnscTdoSetInterval;
    pMyObject->GetTimeToFire  = AnscTdoGetTimeToFire;
    pMyObject->SetTimeToFire  = AnscTdoSetTimeToFire;
    pMyObject->GetCallCounter = AnscTdoGetCallCounter;
    pMyObject->SetCallCounter = AnscTdoSetCallCounter;
    pMyObject->GetState       = AnscTdoGetState;
    pMyObject->SetState       = AnscTdoSetState;

    pMyObject->AcquireAccess  = AnscTdoAcquireAccess;
    pMyObject->ReleaseAccess  = AnscTdoReleaseAccess;
    pMyObject->Start          = AnscTdoStart;
    pMyObject->Stop           = AnscTdoStop;
    pMyObject->Invoke         = AnscTdoInvoke;

#ifndef _ANSC_SPINLOCK_PROFILING_
    AnscInitializeSpinLock(&pMyObject->AccessSpinLock);
#else
    AnscInitializeSpinLock1(&pMyObject->AccessSpinLock, pFileName, iLineNo);
#endif

#ifdef _ANSC_TDO_PROFILING_

    _ansc_strncpy(pMyObject->TdoInitFile, GetFileName(pFileName), sizeof(pMyObject->TdoInitFile));
    pMyObject->TdoInitFile[sizeof(pMyObject->TdoInitFile)-1] = '\0';
    pMyObject->TdoInitLine          = iLineNo;
    pMyObject->TdoFlags             = 1; /* enable it */
    pMyObject->CurrTimestamp        = 0;
    pMyObject->CurrElapsedTime      = 0;
    pMyObject->LongestTime          = 0;
    pMyObject->TotalHits            = 0;
    pMyObject->TotalHitTime         = 0;

    if( ! gAnscTdoProfilingInit )
    {
        AnscInitializeSpinLock(&gAnscTdoProfilingLock);
        AnscDListInitializeHeader(&gAnscTdoProfilingList);
        gAnscTdoProfilingInit = TRUE;
    }

    AnscAcquireSpinLock(&gAnscTdoProfilingLock);
    AnscDListPushEntryRight(&gAnscTdoProfilingList, &pMyObject->TdoProfileLink);
    AnscReleaseSpinLock(&gAnscTdoProfilingLock);

#endif /* _ANSC_TDO_PROFILING_ */

    /*
     * By default, the Timer Descriptor Object will register itself to the current Container
     * Object instead of directly to a Timer Scheduler Object. The mechanism makes it possible
     * to collapse multiple Time Scheduler instances into the one provided by the outmost Object
     * Container Object. However, it mandates the presence of a Container Object. There're cases
     * where Ansc Objects may need to use Timers with no container around.
     */
    if ( !pMyObject->hContainerContext )
    {
        pScheduler =
            (PANSC_TIMER_SCHEDULER_OBJECT)AnscCreateTimerScheduler
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)NULL,
                    (ANSC_HANDLE)NULL
                );

        if ( !pScheduler )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hScheduler = (ANSC_HANDLE)pScheduler;
        }

        pScheduler->Start((ANSC_HANDLE)pScheduler);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    description:

        print tdo info:

**********************************************************************/
#ifdef _ANSC_TDO_PROFILING_

void
AnscTdoProfilingPrint
    (
        void
    )
{
    PDOUBLE_LINK_ENTRY              pEntry;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo;
    ULONG                           ulIndex = 0;

    AnscTraceError(("Total number of Tdo: %d.\n", AnscDListQueryDepth(&gAnscTdoProfilingList)));
    AnscTraceError(("  No  Enable   InitFile    InitLine    Type     Mode    Interval  Hits  Total  Avg   Cur  Max \n"));

    AnscAcquireSpinLock(&gAnscTdoProfilingLock);

    for( pEntry = AnscDListGetHeadEntryLeft(&gAnscTdoProfilingList); pEntry; ulIndex ++ )
    {
        pTdo    = ACCESS_CONTAINER(pEntry, ANSC_TIMER_DESCRIPTOR_OBJECT, TdoProfileLink);
        pEntry  = AnscDListGetEntryRight(pEntry);

        AnscTraceError
            ((
                "%4d  %1d  %24s  %6d  %1d  %08x  %8d  %8u  %10u  %6u  %8u  %8u\n",
                ulIndex,
                pTdo->TdoFlags,
                pTdo->TdoInitFile,
                pTdo->TdoInitLine,
                pTdo->TimerType,
                pTdo->TimerMode,
                pTdo->Interval,
                pTdo->TotalHits,
                pTdo->TotalHitTime,
                pTdo->TotalHits ? pTdo->TotalHitTime/pTdo->TotalHits : 0,
                pTdo->CurrElapsedTime,
                pTdo->LongestTime
            ));
    }

    AnscReleaseSpinLock(&gAnscTdoProfilingLock);
}

void
AnscTdoProfilingEnable
    (
        BOOLEAN                     bEnable,
        ULONG                       ulStart,
        ULONG                       ulStop
    )
{
    PDOUBLE_LINK_ENTRY              pEntry;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo;
    ULONG                           ulIndex = 0;

    AnscAcquireSpinLock(&gAnscTdoProfilingLock);
    for( pEntry = AnscDListGetHeadEntryLeft(&gAnscTdoProfilingList); pEntry; ulIndex ++ )
    {
        pTdo    = ACCESS_CONTAINER(pEntry, ANSC_TIMER_DESCRIPTOR_OBJECT, TdoProfileLink);
        pEntry  = AnscDListGetEntryRight(pEntry);

        if( ulIndex < ulStart )
            continue;

        if( ulIndex > ulStop )
            break;

        pTdo->TdoFlags = bEnable ? 1 : 0;
    }

    AnscReleaseSpinLock(&gAnscTdoProfilingLock);
}

void
AnscTdoProfilingCleanup
    (
        void
    )
{
    PDOUBLE_LINK_ENTRY              pEntry;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo;

    AnscAcquireSpinLock(&gAnscTdoProfilingLock);
    for( pEntry = AnscDListGetHeadEntryLeft(&gAnscTdoProfilingList); pEntry; )
    {
        pTdo    = ACCESS_CONTAINER(pEntry, ANSC_TIMER_DESCRIPTOR_OBJECT, TdoProfileLink);
        pEntry  = AnscDListGetEntryRight(pEntry);

        AnscAcquireSpinLock(&pTdo->AccessSpinLock);
        pTdo->CurrTimestamp        = 0;
        pTdo->CurrElapsedTime      = 0;
        pTdo->LongestTime          = 0;
        pTdo->TotalHits            = 0;
        pTdo->TotalHitTime         = 0;
        AnscReleaseSpinLock(&pTdo->AccessSpinLock);
    }
    AnscReleaseSpinLock(&gAnscTdoProfilingLock);
}

#endif /* _ANSC_TDO_PROFILING_ */

