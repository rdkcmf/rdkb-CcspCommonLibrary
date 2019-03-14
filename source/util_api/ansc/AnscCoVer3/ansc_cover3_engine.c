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

    module:	ansc_cover3_engine.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc CoVer3 Object.

        *   AnscCoVer3SpawnTask
        *   AnscCoVer3SpawnTask2
        *   AnscCoVer3SpawnTask3
        *   AnscCoVer3EngineTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/30/02    initial revision.

**********************************************************************/


#include "ansc_cover3_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3SpawnTask
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pTaskEntry,
                ANSC_HANDLE                 hTaskContext,
                char*                       name
            );

    description:

        This function is called to spawn a task.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pTaskEntry
                Specifies the function entry point to be invoked.

                ANSC_HANDLE                 hTaskContext
                Specifies the input parameter to be passed into the
                spawned task, which usually is the object itself.

                char*                       name
                Specifies the name of the spawned task.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoVer3SpawnTask
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT  )hThisObject;
    PANSC_COVER3_TASK_INFO          pTaskInfo    = (PANSC_COVER3_TASK_INFO)AnscAllocateMemory(sizeof(ANSC_COVER3_TASK_INFO));

    if ( !pTaskInfo )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTaskInfo->hTaskContext = hTaskContext;
        pTaskInfo->Routine      = (PFN_ANSC_COVER3_TASK_ROUTINE)pTaskEntry;
    }

	AnscAcquireLock   (&pMyObject->TaskInfoSListLock);
	AnscSListPushEntry(&pMyObject->TaskInfoSList, &pTaskInfo->Linkage);
	AnscReleaseLock   (&pMyObject->TaskInfoSListLock);

    returnStatus =
        AnscSpawnTask
            (
                pMyObject->EngineTask,
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
		AnscAcquireLock(&pMyObject->EtoAccessLock);
		pMyObject->EngineTaskCount++;
		AnscReleaseLock(&pMyObject->EtoAccessLock);
    }
    else
    {
        PANSC_COVER3_TASK_INFO          pTaskInfo    = NULL;
        PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

        AnscAcquireLock   (&pMyObject->TaskInfoSListLock);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->TaskInfoSList);
    	AnscReleaseLock   (&pMyObject->TaskInfoSListLock);

        if ( pSLinkEntry )
        {
			pTaskInfo = ACCESS_ANSC_COVER3_TASK_INFO(pSLinkEntry);
			AnscFreeMemory(pTaskInfo);
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3SpawnTask2
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pTaskEntry,
                ANSC_HANDLE                 hTaskContext,
                char*                       name,
                int                         priority
            );

    description:

        This function is called to spawn a task.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pTaskEntry
                Specifies the function entry point to be invoked.

                ANSC_HANDLE                 hTaskContext
                Specifies the input parameter to be passed into the
                spawned task, which usually is the object itself.

                char*                       name
                Specifies the name of the spawned task.

                int                         priority
                Specifies the priority of the task to be spawned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoVer3SpawnTask2
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT  )hThisObject;
    PANSC_COVER3_TASK_INFO          pTaskInfo    = (PANSC_COVER3_TASK_INFO)AnscAllocateMemory(sizeof(ANSC_COVER3_TASK_INFO));

    if ( !pTaskInfo )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTaskInfo->hTaskContext = hTaskContext;
        pTaskInfo->Routine      = (PFN_ANSC_COVER3_TASK_ROUTINE)pTaskEntry;
    }

    AnscAcquireLock   (&pMyObject->TaskInfoSListLock);
    AnscSListPushEntry(&pMyObject->TaskInfoSList, &pTaskInfo->Linkage);
    AnscReleaseLock   (&pMyObject->TaskInfoSListLock);

    AnscAcquireLock(&pMyObject->EtoAccessLock);
    pMyObject->EngineTaskCount++;
    AnscReleaseLock(&pMyObject->EtoAccessLock);

    returnStatus =
        AnscSpawnTask2
            (
                pMyObject->EngineTask,
                (ANSC_HANDLE)pMyObject,
                name,
                priority
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3SpawnTask3
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pTaskEntry,
                ANSC_HANDLE                 hTaskContext,
                char*                       name,
                int                         priority,
                ULONG                       stack_size
            );

    description:

        This function is called to spawn a task.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pTaskEntry
                Specifies the function entry point to be invoked.

                ANSC_HANDLE                 hTaskContext
                Specifies the input parameter to be passed into the
                spawned task, which usually is the object itself.

                char*                       name
                Specifies the name of the spawned task.

                int                         priority
                Specifies the priority of the task to be spawned.

                ULONG                       stack_size
                Specifies the stack size of the task.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoVer3SpawnTask3
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority,
        ULONG                       stack_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT  )hThisObject;
    PANSC_COVER3_TASK_INFO          pTaskInfo    = (PANSC_COVER3_TASK_INFO)AnscAllocateMemory(sizeof(ANSC_COVER3_TASK_INFO));

    if ( !pTaskInfo )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTaskInfo->hTaskContext = hTaskContext;
        pTaskInfo->Routine      = (PFN_ANSC_COVER3_TASK_ROUTINE)pTaskEntry;
    }

    AnscAcquireLock   (&pMyObject->TaskInfoSListLock);
    AnscSListPushEntry(&pMyObject->TaskInfoSList, &pTaskInfo->Linkage);
    AnscReleaseLock   (&pMyObject->TaskInfoSListLock);

    AnscAcquireLock(&pMyObject->EtoAccessLock);
    pMyObject->EngineTaskCount++;
    AnscReleaseLock(&pMyObject->EtoAccessLock);

    returnStatus =
        AnscSpawnTask3
            (
                pMyObject->EngineTask,
                (ANSC_HANDLE)pMyObject,
                name,
                priority,
                stack_size
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3EngineTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is the entry point for the spawned task.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
AnscCoVer3EngineTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT)hThisObject;
    PANSC_COVER3_TASK_INFO          pTaskInfo    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->TaskInfoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->TaskInfoSList);
    AnscReleaseLock(&pMyObject->TaskInfoSListLock);

    if ( pSLinkEntry )
    {
        pTaskInfo = ACCESS_ANSC_COVER3_TASK_INFO(pSLinkEntry);

        while ( !pMyObject->bShuttingDown )
        {
            if ( pTaskInfo->Routine(pTaskInfo->hTaskContext) == ANSC_STATUS_DO_IT_AGAIN )
            {
                continue;
            }
            else
            {
                break;
            }
        }

        AnscFreeMemory(pTaskInfo);

        AnscAcquireLock(&pMyObject->EtoAccessLock);
        pMyObject->EngineTaskCount--;
        AnscReleaseLock(&pMyObject->EtoAccessLock);

    }

    return  returnStatus;
}
