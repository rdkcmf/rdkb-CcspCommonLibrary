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

    module:	ansc_dsto_patient.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced diagnose functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoPoOverview
        *   AnscDstoPoSnapshot
        *   AnscDstoPoTraceTransaction
        *   AnscDstoPoEngineCloseUp

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/18/01    initial revision.

**********************************************************************/


#include "ansc_dsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoPoOverview
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hForm
            );

    description:

        This function is called to fill in the "overview form" for
        diagnostic purpose.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hForm
                Specifies the data structure to be filled in.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoPoOverview
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_OVERVIEW_FORM        pOverviewForm = (PANSC_DSTO_OVERVIEW_FORM      )hForm;

    pOverviewForm->StartTime      = pMyObject->StartTime;
    pOverviewForm->CurrentTime    = AnscGetTickInSecondsAbs();
    pOverviewForm->EngineCount    = pMyObject->EngineCount;
    pOverviewForm->MinSocketCount = pMyObject->MinSocketCount;
    pOverviewForm->MaxSocketCount = pMyObject->MaxSocketCount;
    pOverviewForm->CurSocketCount = pMyObject->CurSocketCount;
    pOverviewForm->TccCount       = pMyObject->TccCount;
    pOverviewForm->MccCount       = pMyObject->MccCount;
    pOverviewForm->TrcCount       = pMyObject->TrcCount;
    pOverviewForm->MrcCount       = pMyObject->MrcCount;
    pOverviewForm->TscCount       = pMyObject->TscCount;
    pOverviewForm->MscCount       = pMyObject->MscCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoPoSnapshot
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hForm,
                ULONG                       ulIndex
            );

    description:

        This function is called to fill in the "snapshot form" for
        diagnostic purpose.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hForm
                Specifies the data structure to be filled in.

                ULONG                       ulIndex
                Specifies which engine to take the snapshot.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoPoSnapshot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_SNAPSHOT_FORM        pSnapshotForm = (PANSC_DSTO_SNAPSHOT_FORM      )hForm;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine       = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);
    pSLinkEntry = AnscQueueSearchEntryByIndex(&pMyObject->EngineQueue, ulIndex);
    AnscReleaseLock(&pMyObject->EngineQueueLock);

    if ( !pSLinkEntry )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pEngine = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
    }

    returnStatus =
        pEngine->Snapshot
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pSnapshotForm
            );

    pSnapshotForm->EngineIndex = ulIndex;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoPoTraceTransaction
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to trace the next incoming transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoPoTraceTransaction
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoPoEngineCloseUp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hForm,
                ULONG                       ulIndex
            );

    description:

        This function is called to fill in the "close-up form" for
        diagnostic purpose.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hForm
                Specifies the data structure to be filled in.

                ULONG                       ulIndex
                Specifies which engine to take the close-up shot.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoPoEngineCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_CLOSE_UP_FORM        pCloseUpForm = (PANSC_DSTO_CLOSE_UP_FORM      )hForm;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);
    pSLinkEntry = AnscQueueSearchEntryByIndex(&pMyObject->EngineQueue, ulIndex);
    AnscReleaseLock(&pMyObject->EngineQueueLock);

    if ( !pSLinkEntry )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pEngine = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
    }

    returnStatus =
        pEngine->CloseUp
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pCloseUpForm
            );

    pCloseUpForm->EngineIndex = ulIndex;

    return  returnStatus;
}
