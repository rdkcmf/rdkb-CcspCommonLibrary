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

    module:	ansc_task.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to task-management.

        *   AnscTaskRoutine1
        *   AnscTaskRoutine2
        *   AnscSpawnTask
        *   AnscSpawnTask2
        *   AnscEraseTask
        *   AnscInitializeTpm
        *   AnscInitializeTpj
        *   AnscAcquireTask
        *   AnscReleaseTask
        *   AnscInitializeTroTable
        *   AnscUnloadTroTable
        *   AnscGetCurTaskRecord
        *   AnscGetTaskRecord
        *   AnscAddTaskRecord
        *   AnscDelTaskRecord
        *   AnscGetActiveTaskCount
        *   AnscWaitAllTasks
        *   AnscKillAllTasks
        *   AnscGetChildTaskCount
        *   AnscWaitAllChildTasks
        *   AnscGetParentTaskHandle
        *   AnscGetTaskStorageUnit
        *   AnscAddTaskStorageUnit
        *   AnscDelTaskStorageUnit

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.

**********************************************************************/


#include "ansc_global.h"
#include "safec_lib_common.h"


static  BOOL                        g_bSafeParenting  = FALSE;
static  BOOL                        g_bTraceEnabled   = ANSC_TRACE_PLATFORM_TASK;
static  BOOL                        g_bTpmInitialized = FALSE;
static  BOOL                        g_bTpjInitialized = FALSE;
static  BOOL                        g_bTpmEnabled     = ANSC_USE_TASK_POOL_MANAGER;
static  BOOL                        g_bTpjEnabled     = ANSC_USE_TASK_POOL_JANITOR;
static  ULONG                       g_ulTpmPoolType   = ANSC_CUR_TASK_POOL_TYPE;
static  ULONG                       g_ulTpmPoolSize   = ANSC_MAX_TASK_POOL_SIZE;
static  ULONG                       g_ulTaskCount     = 0;

static  SLIST_HEADER                g_TpmTaskSList;
static  ANSC_SPINLOCK               g_TpmTaskSListSpinLock;
static  SLIST_HEADER                g_TpjTaskSList;
static  ANSC_SPINLOCK               g_TpjTaskSListSpinLock;

static  BOOL                        g_bTrmInitialized = FALSE;
static  SLIST_HEADER                g_TroTable[ANSC_TASK_RECORD_TABLE_SIZE];
static  ANSC_SPINLOCK               g_TroTableSpinLock;


#ifdef _ANSC_DEBUG_TASK_
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
#endif /* _ANSC_DEBUG_TASK_ */


PANSC_TASK_RECORD
AnscAllocateTaskRecord1
    (
        PCSTR                       pcszName
    )
{
    ULONG                           ii, len;
    PANSC_TASK_RECORD               pTaskRecord;


    pTaskRecord = (PANSC_TASK_RECORD)AnscAllocateMemory(sizeof(ANSC_TASK_RECORD));

    if( ! pTaskRecord )
    {
        return NULL;
    }

    pTaskRecord->RefCount       = 1;

#ifdef  _ANSC_DEBUG_TASK_
    pTaskRecord->bEnabled       = 1;
    pTaskRecord->bCaptured      = 0;
#endif /* _ANSC_DEBUG_TASK_ */

    len = pcszName ? _ansc_strlen(pcszName) : 0;
    if( len > ANSC_OBJECT_NAME_SIZE - 1 )
        len = ANSC_OBJECT_NAME_SIZE - 1;

    _ansc_memcpy(pTaskRecord->Name, pcszName, len);
    pTaskRecord->Name[len]      = '\0';

    pTaskRecord->bAnscOwned     = FALSE;
    pTaskRecord->bSuspended     = FALSE;
    pTaskRecord->Parent         = AnscGetCurTaskHandle();
    pTaskRecord->Handle         = 0; /* AnscGetCurTaskHandle();*/
    pTaskRecord->Routine        = NULL;
    pTaskRecord->hContext       = NULL;
    pTaskRecord->GeneralError   = 0;
    pTaskRecord->SsocketError   = 0;


    AnscInitializeSpinLock(&pTaskRecord->AccessSpinLock);

    for ( ii = 0; ii < ANSC_TASK_SUO_TABLE_SIZE; ii ++ )
    {
        AnscQueueInitializeHeader(&pTaskRecord->SuoTable[ii]);
    }

    return pTaskRecord;

}

void
AnscFreeTaskRecord
    (
        PANSC_TASK_RECORD           pTaskRecord
    )
{
    if( pTaskRecord->RefCount == 1 )
    {
        pTaskRecord->RefCount = 0;
        pTaskRecord->Handle = 0;
        AnscFreeMemory(pTaskRecord);
    }
    else {
    	AnscTrace("WARNING - AnscFreeTaskRecord, task record %lu-%p reference count is %lu!\n",
    			pTaskRecord->Handle,pTaskRecord, pTaskRecord->RefCount);
    }

}


void
AnscTaskRoutine1
    (
        void*                       runtime_record
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)runtime_record;

    AnscEnterTask(pTaskRecord);

    pTaskRecord->Handle = AnscGetCurTaskHandle();

#ifndef  _ANSC_DEBUG_TASK_
        AnscAddTaskRecord
#else
        AnscAddTaskRecordTrace
#endif
            (
                pTaskRecord
#ifdef  _ANSC_DEBUG_TASK_
                ,
                pTaskRecord->CreateFile,
                pTaskRecord->CreateLine
#endif
            );


    if (pTaskRecord->Routine)
            pTaskRecord->Routine(pTaskRecord->hContext);

    if ( g_bTraceEnabled )
    {
        AnscTrace
            (
                "Task %s is exiting, context = 0x%lx!\n",
                pTaskRecord->Name,
                (ULONG)pTaskRecord->hContext
            );
    }

    if ( g_bSafeParenting )
    {
        AnscWaitAllChildTasks(pTaskRecord->Handle);
    }

    AnscEraseTask(pTaskRecord);

    AnscLeaveTask();
}


void
AnscTaskRoutine2
    (
        void*                       runtime_record
    )
{
    UNREFERENCED_PARAMETER(runtime_record);
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    BOOL                            bSuspended   = TRUE;

    AnscEnterTask(NULL);

    /*
     * We let the task create its own task context instead of the initialization function since
     * the static Task Pool Manager will not have a chance to do so. If memory allocation fails
     * for any reason, we drop out the task silently (not even trying to delete ourselves).
     */
    AnscAllocateTaskRecord("poolable_task", pTaskRecord);

    if ( !pTaskRecord )
    {
        AnscLeaveTask();

        return;
    }
    else
    {
        pTaskRecord->bAnscOwned   = TRUE;
        pTaskRecord->bSuspended   = TRUE;
        pTaskRecord->Parent       = AnscGetCurTaskHandle();
        pTaskRecord->Handle       = 0;
        pTaskRecord->Routine      = NULL;
        pTaskRecord->hContext     = (ANSC_HANDLE)NULL;
        pTaskRecord->GeneralError = 0;
        pTaskRecord->SsocketError = 0;
    }

    /*
     * Dangerous: following operation is not protected or synchronized between multiple tasks. We
     * presume that in RTOS a task will not be suspended until it gives up its control. It's true
     * regardless among all the poolable tasks. Since the Janitor Task will try to access the
     * global erased_task list without any validation, the Static Task Manager must ensure that
     * the Janitor Task is created after all poolable tasks.
     */
    if ( g_ulTaskCount == 0 )
    {
        AnscSListInitializeHeader(&g_TpmTaskSList        );
        AnscInitializeSpinLock   (&g_TpmTaskSListSpinLock);
    }

    AnscReleaseTask((ANSC_HANDLE)pTaskRecord);

        AnscAddTaskRecord
            (
                pTaskRecord
            );

    /*
     * Here we enter the infinite loop (note that poolable task is never explicitly killed) and
     * wait to be scheduled by the Task Pool Manager. While you're reading this comment, just for
     * fun, try to test your I.Q. with this question: what's the difference between this approach
     * and a user-mode Task-Scheduling simulation?
     */
    while ( TRUE )
    {
        while ( bSuspended )
        {
            AnscSleep(ANSC_TASK_SLEEP_INTERVAL);

            AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);
            bSuspended = pTaskRecord->bSuspended;
            AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);
        }
        /*CID:162747 Explicit null dereferenced*/
        if (pTaskRecord->Routine)
            pTaskRecord->Routine(pTaskRecord->hContext);
        bSuspended   = TRUE;

        if ( g_bTraceEnabled )
        {
            AnscTrace
                (
                    "Task %s is exiting, context = 0x%lx!\n",
                    pTaskRecord->Name,
                    (ULONG)pTaskRecord->hContext
                );
        }

        AnscReleaseTask((ANSC_HANDLE)pTaskRecord);
    }

    if ( g_bSafeParenting )
    {
        AnscWaitAllChildTasks(pTaskRecord->Handle);
    }

    AnscEraseTask(pTaskRecord);

    AnscLeaveTask();
}


void
AnscTaskJanitorRoutine
    (
        void*                       runtime_record
    )
{
    UNREFERENCED_PARAMETER(runtime_record);
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscEnterTask((PANSC_TASK_RECORD)AnscGetCurTaskRecord());

    while ( TRUE )
    {
        AnscSleep(ANSC_TASK_CLEAN_INTERVAL);

        AnscAcquireSpinLock(&g_TpjTaskSListSpinLock);

        while ( (pSLinkEntry = AnscSListPopEntry(&g_TpjTaskSList) ))
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);

            AnscDelTaskRecord    (pTaskRecord->Handle);
            AnscFreeTaskRecord(pTaskRecord);
        }

        AnscReleaseSpinLock(&g_TpjTaskSListSpinLock);
    }

    AnscLeaveTask();
}

#ifndef _ANSC_DEBUG_TASK_

ANSC_STATUS
AnscSpawnTask
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    returnStatus =
        AnscSpawnTask2
            (
                pTaskEntry,
                hTaskContext,
                name,
                ANSC_TASK_PRIORITY_NORMAL
            );

    return  returnStatus;
}


ANSC_STATUS
AnscSpawnTask2
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    returnStatus =
        AnscSpawnTask3
            (
                pTaskEntry,
                hTaskContext,
                name,
                priority,
                ANSC_DEFAULT_TASK_STACK_SIZE
            );

    return  returnStatus;
}

#endif /* _ANSC_DEBUG_TASK_ */


ANSC_STATUS
#ifndef _ANSC_DEBUG_TASK_
AnscSpawnTask3
#else
AnscSpawnTask3Trace
#endif
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority,
        ULONG                       stack_size
#ifdef _ANSC_DEBUG_TASK_
        ,
        const char *                pFileName,
        int                         iLineNo
#endif
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PFN_ANSC_TASK_ROUTINE           pTaskRoutine = (PFN_ANSC_TASK_ROUTINE)pTaskEntry;

    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    void                            *taskHandle; 

    if ( g_bTraceEnabled )
    {
        AnscTrace("AnscSpawnTask creates task %s!\n", name);
    }

    if ( !g_bTpmInitialized )
    {
        returnStatus = AnscInitializeTpm(g_ulTpmPoolSize);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
        else
        {
            g_bTpmInitialized = TRUE;
        }
    }

    if ( g_bTpmEnabled )
    {
        pTaskRecord = (PANSC_TASK_RECORD)AnscAcquireTask(pTaskEntry, hTaskContext, name);

        if ( !pTaskRecord )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }
    else
    {
        AnscAllocateTaskRecord(name, pTaskRecord);

        if ( !pTaskRecord )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        pTaskRecord->bAnscOwned   = TRUE;
        pTaskRecord->bSuspended   = FALSE;
        pTaskRecord->Parent       = AnscGetCurTaskHandle();
        pTaskRecord->Handle       = 0;
        pTaskRecord->Routine      = pTaskRoutine;
        pTaskRecord->hContext     = hTaskContext;
        pTaskRecord->GeneralError = 0;
        pTaskRecord->SsocketError = 0;

#ifdef _ANSC_DEBUG_TASK_
        _ansc_strncpy(pTaskRecord->CreateFile, GetFileName(pFileName), sizeof(pTaskRecord->CreateFile));
        pTaskRecord->CreateFile[sizeof(pTaskRecord->CreateFile)-1] = '\0';
        pTaskRecord->CreateLine   = iLineNo;
#endif

    if (
            (taskHandle = AnscCreateTask
            (
                AnscActiveTaskRoutine,
                stack_size,
                priority,
                (void *)pTaskRecord,
                (void *)name
            )) == NULL
        ) {
    	returnStatus = ANSC_STATUS_FAILURE;
    }
    else
    {
        pthread_detach((pthread_t)taskHandle);
    }
}

    return  returnStatus;
}


ANSC_STATUS
AnscEraseTask
    (
        ANSC_HANDLE                 hTaskRecord
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)hTaskRecord;
    ULONG                           ulTaskHandle = pTaskRecord->Handle;

    /* don't know why this would happen */
    if ( ulTaskHandle == 0 )
    	ulTaskHandle = AnscGetCurTaskHandle();

    if ( !g_bTpjInitialized )
    {
        returnStatus = AnscInitializeTpj(g_ulTpmPoolSize);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
        else
        {
            g_bTpjInitialized = TRUE;
        }
    }

    if ( g_bTpjEnabled )
    {
        AnscAcquireSpinLock(&g_TpjTaskSListSpinLock);
        AnscSListPushEntry (&g_TpjTaskSList, &pTaskRecord->Linkage);
        AnscReleaseSpinLock(&g_TpjTaskSListSpinLock);

        AnscDelTaskRecord(ulTaskHandle);
    }
    else
    {
    	AnscDelTaskRecord (ulTaskHandle);
       	AnscFreeTaskRecord(pTaskRecord        );

        /*
         * Note that the task execution may terminate in the following system call, so we need to
         * make sure it's the last function call we make.
         */
        AnscDeleteTask((void *)ulTaskHandle);
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscInitializeTpm
    (
        ULONG                       ulPoolSize
    )
{
    ULONG                           i            = 0;
    void                            *handle;

    if ( !g_bTpmEnabled )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( g_ulTpmPoolType == ANSC_TASK_POOL_TYPE_STATIC )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < ulPoolSize; i++ )
    {
        if(
            (handle = AnscCreateTask
            (
                AnscPoolableTaskRoutine,
                ANSC_DEFAULT_TASK_STACK_SIZE,
                ANSC_DEFAULT_TASK_PRIORITY,
                NULL,
                "anscPooledTask"
            )) == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
        else
        {
            pthread_detach((pthread_t)handle);
        }
    }

    AnscSleep(ANSC_TASK_BREAK_INTERVAL);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscInitializeTpj
    (
        ULONG                       ulPoolSize
    )
{
    UNREFERENCED_PARAMETER(ulPoolSize);
    void                            *handle; 

    if ( !g_bTpjEnabled )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( g_ulTpmPoolType == ANSC_TASK_POOL_TYPE_STATIC )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscSListInitializeHeader(&g_TpjTaskSList        );
        AnscInitializeSpinLock   (&g_TpjTaskSListSpinLock);
    }

    if(
        (handle = AnscCreateTask
        (
            AnscJanitorTaskRoutine,
            ANSC_DEFAULT_TASK_STACK_SIZE,
            ANSC_DEFAULT_TASK_PRIORITY,
            NULL,
            "anscJanitorTask"
        )) == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
    pthread_detach((pthread_t)handle);
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
AnscAcquireTask
    (
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    )
{
    PFN_ANSC_TASK_ROUTINE           pTaskRoutine = (PFN_ANSC_TASK_ROUTINE)pTaskEntry;
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    errno_t                         rc           = -1;

    AnscAcquireSpinLock(&g_TpmTaskSListSpinLock);
    pSLinkEntry = AnscSListPopEntry(&g_TpmTaskSList);
    AnscReleaseSpinLock(&g_TpmTaskSListSpinLock);

    if ( !pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
    }

    g_ulTaskCount--;

    AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);

    AnscZeroMemory(pTaskRecord->Name, ANSC_OBJECT_NAME_SIZE);
    rc = strcpy_s(pTaskRecord->Name, sizeof(pTaskRecord->Name), name                 );
    ERR_CHK(rc);

    pTaskRecord->bSuspended   = FALSE;
    pTaskRecord->Routine      = pTaskRoutine;
    pTaskRecord->hContext     = hTaskContext;
    pTaskRecord->GeneralError = 0;
    pTaskRecord->SsocketError = 0;

    AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);

    return  (ANSC_HANDLE)pTaskRecord;
}


ANSC_STATUS
AnscReleaseTask
    (
        ANSC_HANDLE                 hTaskRecord
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)hTaskRecord;

    AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);
    pTaskRecord->bSuspended = TRUE;
    AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);

    AnscAcquireSpinLock(&g_TpmTaskSListSpinLock);
    AnscSListPushEntry (&g_TpmTaskSList, &pTaskRecord->Linkage);
    AnscReleaseSpinLock(&g_TpmTaskSListSpinLock);

    g_ulTaskCount++;

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscInitializeTroTable
    (
        void
    )
{
    ULONG                           i            = 0;

    if ( !g_bTrmInitialized )
    {
        for ( i = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i++ )
        {
            AnscSListInitializeHeader(&g_TroTable[i]);
        }
        AnscInitializeSpinLock(&g_TroTableSpinLock);

        g_bTrmInitialized = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscUnloadTroTable
    (
        void
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    for ( i = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&g_TroTable[i]);

        while ( pSLinkEntry )
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&g_TroTable[i]);

            AnscFreeTaskRecord(pTaskRecord);
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);
    AnscFreeSpinLock   (&g_TroTableSpinLock);

    g_bTrmInitialized = FALSE;

    return  ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
AnscGetCurTaskRecord
    (
        void
    )
{
    ULONG                           ulTaskHandle = AnscGetCurTaskHandle();
    PANSC_TASK_RECORD               pTaskRecord  = NULL;

    if ( !g_bTrmInitialized )
    {
        AnscInitializeTroTable();
    }

    pTaskRecord = (PANSC_TASK_RECORD)AnscGetTaskRecord(ulTaskHandle);

    if ( pTaskRecord )
    {
        AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);
        -- pTaskRecord->RefCount;
        AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);

        return  (ANSC_HANDLE)pTaskRecord;
    }

    return NULL;
}


ANSC_HANDLE
AnscGetTaskRecord
    (
        ULONG                       ulTaskHandle
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulTaskHandle, ANSC_TASK_RECORD_TABLE_SIZE);

    if ( !g_bTrmInitialized )
    {
        AnscInitializeTroTable();
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_TroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pTaskRecord->Handle == ulTaskHandle )
        {
            AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);
            pTaskRecord->RefCount ++;
            AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);

            AnscReleaseSpinLock(&g_TroTableSpinLock);

            return  (ANSC_HANDLE)pTaskRecord;
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return  (ANSC_HANDLE)NULL;
}


ANSC_HANDLE
#ifndef _ANSC_DEBUG_TASK_
AnscAddTaskRecord
#else
AnscAddTaskRecordTrace
#endif
    (
        PANSC_TASK_RECORD           pTaskRecord
#ifdef _ANSC_DEBUG_TASK_
        ,
        const char *                pFileName,
        int                         iLineNo
#endif
    )
{
    ULONG                           ulHashIndex  = AnscHashUlong(pTaskRecord->Handle, ANSC_TASK_RECORD_TABLE_SIZE);

    if ( !g_bTrmInitialized )
    {
        AnscInitializeTroTable();
    }

#ifdef _ANSC_DEBUG_TASK_
    _ansc_strncpy(pTaskRecord->CreateFile, GetFileName(pFileName), sizeof(pTaskRecord->CreateFile));
    pTaskRecord->CreateFile[sizeof(pTaskRecord->CreateFile)-1] = '\0';
    pTaskRecord->CreateLine   = iLineNo;
#endif

    AnscAcquireSpinLock(&g_TroTableSpinLock);
    AnscSListPushEntry (&g_TroTable[ulHashIndex], &pTaskRecord->Linkage);
    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return  (ANSC_HANDLE)pTaskRecord;
}


ANSC_HANDLE
AnscDelTaskRecord
    (
        ULONG                       ulTaskHandle
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulTaskHandle, ANSC_TASK_RECORD_TABLE_SIZE);

    if ( !g_bTrmInitialized )
    {
        AnscInitializeTroTable();
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&g_TroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pTaskRecord->Handle == ulTaskHandle )
        {
            if ( AnscSListPopEntryByLink(&g_TroTable[ulHashIndex], &pTaskRecord->Linkage) ) {
                AnscReleaseSpinLock(&g_TroTableSpinLock);
                return pTaskRecord;
            }
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return  NULL;
}

ULONG
AnscGetActiveTaskCount
    (
        void
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulTaskCount  = 0;
    ULONG                           i            = 0;

    if ( !g_bTrmInitialized )
    {
        return  0;
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    for ( i = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_TroTable[i]);

        while ( pSLinkEntry )
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( pTaskRecord->bAnscOwned )
            {
                ulTaskCount++;
            }
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return  ulTaskCount;
}


ANSC_STATUS
AnscWaitAllTasks
    (
        void
    )
{
    while ( AnscGetActiveTaskCount() > 0 )
    {
        AnscSleep(ANSC_TASK_SLEEP_INTERVAL);
    }

    AnscUnloadTroTable();

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscKillAllTasks
    (
        void
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    if ( !g_bTrmInitialized )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    for ( i = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&g_TroTable[i]);

        while ( pSLinkEntry )
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&g_TroTable[i]);

            if ( pTaskRecord->bAnscOwned )
            {
                AnscKillTask((void *)pTaskRecord->Handle);
            }

            AnscFreeTaskRecord(pTaskRecord);
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    AnscUnloadTroTable();

    return  ANSC_STATUS_SUCCESS;
}


ULONG
AnscGetChildTaskCount
    (
        ULONG                       ulTaskParent
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulTaskCount  = 0;
    ULONG                           i            = 0;

    if ( !g_bTrmInitialized )
    {
        return  0;
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    for ( i = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_TroTable[i]);

        while ( pSLinkEntry )
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( (pTaskRecord->Parent == ulTaskParent) && pTaskRecord->bAnscOwned )
            {
                ulTaskCount++;
            }
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return  ulTaskCount;
}


#ifdef _ANSC_DEBUG_TASK_

ANSC_STATUS
AnscPrintTaskList
    (
        ULONG                       param
    )
{
    PANSC_TASK_RECORD               pTaskRecord;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    ULONG                           ulTaskCount;
    ULONG                           i;

    AnscTraceError(("Index   Task Name                 TID     Parent  Enabled Captured  File     Line  \n"));

    if ( !g_bTrmInitialized )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireSpinLock(&g_TroTableSpinLock);

    for ( i = 0, ulTaskCount = 0; i < ANSC_TASK_RECORD_TABLE_SIZE; i ++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&g_TroTable[i]);

        while ( pSLinkEntry )
        {
            pTaskRecord = ACCESS_ANSC_TASK_RECORD(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            AnscTraceError
                ((
                    "%6d  %24s  %6d  %6d  %6d  %6d  %24s  %6d\n",
                    ulTaskCount,
                    pTaskRecord->Name,
                    pTaskRecord->Handle,
                    pTaskRecord->Parent,
                    pTaskRecord->bEnabled,
                    pTaskRecord->bCaptured,
                    pTaskRecord->CreateFile,
                    pTaskRecord->CreateLine
                ));

            ulTaskCount ++;
        }
    }

    AnscReleaseSpinLock(&g_TroTableSpinLock);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscEnableTask
    (
        ULONG                       tid,
        int                         bEnable
    )
{
    PANSC_TASK_RECORD               pTaskRecord;

    pTaskRecord = (PANSC_TASK_RECORD)AnscGetTaskRecord(tid);

    if( ! pTaskRecord )
    {
        return ANSC_STATUS_CANT_FIND;
    }

    AnscAcquireSpinLock(&pTaskRecord->AccessSpinLock);
    pTaskRecord->bEnabled = bEnable;
    AnscReleaseSpinLock(&pTaskRecord->AccessSpinLock);

    AnscFreeTaskRecord(pTaskRecord);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
AnscCommitTask
    (
        void
    )
{
    PANSC_TASK_RECORD               pTaskRecord;

    pTaskRecord = (PANSC_TASK_RECORD)AnscGetCurTaskRecord();

    if( ! pTaskRecord )
    {
        return ANSC_STATUS_CANT_FIND;
    }

    if( ! pTaskRecord->bEnabled )
    {
        AnscTraceError(("Disabling thread %d :  %s\n", pTaskRecord->Handle, pTaskRecord->Name));

        pTaskRecord->bCaptured      = 1;

        do
        {
            /* sleep for a while */
            AnscSleep(500);
        }
        while( ! pTaskRecord->bEnabled );

        pTaskRecord->bCaptured      = 0;
    }

    return ANSC_STATUS_SUCCESS;
}

#endif /* _ANSC_DEBUG_TASK_ */


ANSC_STATUS
AnscWaitAllChildTasks
    (
        ULONG                       ulTaskParent
    )
{
    while ( AnscGetChildTaskCount(ulTaskParent) > 0 )
    {
        AnscSleep(ANSC_TASK_SLEEP_INTERVAL);
    }

    return  ANSC_STATUS_SUCCESS;
}


ULONG
AnscGetParentTaskHandle
    (
        void
    )
{
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)AnscGetCurTaskRecord();

    if ( !pTaskRecord )
    {
        return  0;
    }

    return  pTaskRecord->Parent;
}


ANSC_HANDLE
AnscGetTaskStorageUnit
    (
        char*                       unit_name
    )
{
    PANSC_TASK_RECORD               pTaskRecord      = (PANSC_TASK_RECORD      )AnscGetCurTaskRecord();
    PANSC_TASK_STORAGE_UNIT         pTaskStorageUnit = (PANSC_TASK_STORAGE_UNIT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex      = AnscHashString(unit_name, AnscSizeOfString(unit_name), ANSC_TASK_SUO_TABLE_SIZE);

    if ( !pTaskRecord )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pTaskRecord->SuoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTaskStorageUnit = ACCESS_ANSC_TASK_STORAGE_UNIT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pTaskStorageUnit->Name,unit_name) == 0 )
        {
            return  pTaskStorageUnit->hSuoContext;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


ANSC_STATUS
AnscAddTaskStorageUnit
    (
        char*                       unit_name,
        ANSC_HANDLE                 unit_context
    )
{
    PANSC_TASK_RECORD               pTaskRecord      = (PANSC_TASK_RECORD      )AnscGetCurTaskRecord();
    PANSC_TASK_STORAGE_UNIT         pTaskStorageUnit = (PANSC_TASK_STORAGE_UNIT)NULL;

    if ( !pTaskRecord )
    {
        return  ANSC_STATUS_FAILURE;
    }
    else if ( AnscGetTaskStorageUnit(unit_name) != (ANSC_HANDLE)NULL )
    {
        return  ANSC_STATUS_COLLISION;
    }
    else
    {
        pTaskStorageUnit = (PANSC_TASK_STORAGE_UNIT)AnscAllocateMemory(sizeof(ANSC_TASK_STORAGE_UNIT));

        if ( !pTaskStorageUnit )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pTaskStorageUnit->Name,
                    unit_name,
                    AnscGetMin2(ANSC_OBJECT_NAME_SIZE - 1, AnscSizeOfString(unit_name))
                );

            pTaskStorageUnit->HashIndex   = AnscHashString(unit_name, AnscSizeOfString(unit_name), ANSC_TASK_SUO_TABLE_SIZE);
            pTaskStorageUnit->hSuoContext = unit_context;
        }
    }

    AnscQueuePushEntry(&pTaskRecord->SuoTable[pTaskStorageUnit->HashIndex], &pTaskStorageUnit->Linkage);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscDelTaskStorageUnit
    (
        char*                       unit_name
    )
{
    PANSC_TASK_RECORD               pTaskRecord      = (PANSC_TASK_RECORD      )AnscGetCurTaskRecord();
    PANSC_TASK_STORAGE_UNIT         pTaskStorageUnit = (PANSC_TASK_STORAGE_UNIT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex      = AnscHashString(unit_name, AnscSizeOfString(unit_name), ANSC_TASK_SUO_TABLE_SIZE);

    if ( !pTaskRecord )
    {
        return  ANSC_STATUS_FAILURE;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pTaskRecord->SuoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTaskStorageUnit = ACCESS_ANSC_TASK_STORAGE_UNIT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pTaskStorageUnit->Name,unit_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pTaskRecord->SuoTable[ulHashIndex], &pTaskStorageUnit->Linkage);

            AnscFreeMemory(pTaskStorageUnit);

            break;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


