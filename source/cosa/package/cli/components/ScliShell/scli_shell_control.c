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

    module:	scli_shell_control.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

        *   ScliShoInitTdo
        *   ScliShoKillTdo
        *   ScliShoStartTdo
        *   ScliShoStopTdo
        *   ScliShoIdleTask
             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/25/05    initial revision.

**********************************************************************/

#include "scli_shell_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoInitTdo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to init TDO.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoInitTdo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject   = (PSCLI_SHELL_OBJECT)hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo        = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hTdo;
    PANSC_TDO_CLIENT_OBJECT         pTdoClient  = (PANSC_TDO_CLIENT_OBJECT)pMyObject->hTdoClient;

    if (!pTdo)
    {
        pTdo = (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor(pMyObject->hContainerContext, hThisObject, NULL);

        if (!pTdo)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->hTdo = (ANSC_HANDLE)pTdo;

        pTdo->SetTimerType((ANSC_HANDLE)pTdo, ANSC_TIMER_TYPE_PERIODIC);
        pTdo->SetInterval ((ANSC_HANDLE)pTdo, SCLI_SHELL_IDLE_TIMER_INTERVAL);
    }

    if (!pTdoClient)
    {
        pTdoClient = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if (!pTdoClient)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->hTdoClient   = (ANSC_HANDLE)pTdoClient;

        pTdoClient->hClientContext  = (ANSC_HANDLE)pMyObject;
        pTdoClient->Invoke          = ScliShoIdleTask;

        if (pTdo)
        {
            pTdo->SetClient((ANSC_HANDLE)pTdo, (ANSC_HANDLE)pTdoClient);
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoKillTdo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to kill TDO.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoKillTdo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT   pMyObject   = (PSCLI_SHELL_OBJECT)hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo        = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hTdo;
    PANSC_TDO_CLIENT_OBJECT         pTdoClient  = (PANSC_TDO_CLIENT_OBJECT)pMyObject->hTdoClient;

    if (pTdo)
    {
        pTdo->Stop((ANSC_HANDLE)pTdo);
        pTdo->Remove((ANSC_HANDLE)pTdo);
    }

    if (pTdoClient)
    {
        AnscFreeMemory(pTdoClient);
    }

    pMyObject->hTdo         = (ANSC_HANDLE)NULL;
    pMyObject->hTdoClient   = (ANSC_HANDLE)NULL;

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoStartTdo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start TDO.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoStartTdo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT   pMyObject   = (PSCLI_SHELL_OBJECT)hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo        = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hTdo;

    if (!pTdo)
    {
        pMyObject->InitTdo((ANSC_HANDLE)pMyObject);
        
        pTdo = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hTdo;
    }

    if (pTdo)
    {
        pTdo->Start((ANSC_HANDLE)pTdo);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoStopTdo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to stop TDO.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoStopTdo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT   pMyObject   = (PSCLI_SHELL_OBJECT)hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pTdo        = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hTdo;

    if (pTdo)
    {
        pTdo->Stop((ANSC_HANDLE)pTdo);

        pMyObject->KillTdo((ANSC_HANDLE)pMyObject);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoTimerTask
            (
                ANSC_HANDLE                 hContext
            );

    description:

        This function is the timer task function.

    argument:   ANSC_HANDLE                 hContext
                Handle to timer task context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIdleTask
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject       = (PSCLI_SHELL_OBJECT        )hContext;
    PBMC2_SCC_INTERFACE             pBmc2SccIf      = (PBMC2_SCC_INTERFACE       )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_AUTH_INTERFACE      pAuthIf         = (PSCLI_SHELL_AUTH_INTERFACE)pMyObject->hAuthIf;
    PTELNET_TSC_INTERFACE           pTscIf          = (PTELNET_TSC_INTERFACE     )pMyObject->hTscIf;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulTimeNow       = AnscGetTickInSeconds();
    PSCLI_SHELL_SESSION_EXEC        pSession;
    BOOL                            bExpired;

    if ( pAuthIf )
    {
        /* synchronizing settings for authentication */
        status = 
            pAuthIf->GetTimeouts
                (
                    pAuthIf->hOwnerContext,
                    &pMyObject->InactivityTimeout,
                    &pMyObject->LockoutTimeout
                );
    }

    AnscAcquireLock(&pMyObject->SessionListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SessionList);
    while ( pSLinkEntry )
    {
        pSession    = ACCESS_SCLI_SHELL_SESSION_EXEC_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pMyObject->InactivityTimeout == 0 )
        {
            continue;
        }

        ulTimeNow   = AnscGetTickInSeconds();

        bExpired    = 
              SCLI_SHELL_INACTIVITY_TIMEOUT
                ( 
                    pSession->LastRecvTS, 
                    ulTimeNow, 
                    pMyObject->InactivityTimeout
                );

        if ( pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN && bExpired )
        {
            AnscReleaseLock(&pMyObject->SessionListLock);

            if ( !pSession->bServing )
            {
                TELNET_CMD_EXECUTION_ENV    env = { 0 };

                env.hTscIf  = (ANSC_HANDLE)pTscIf;

                status = 
                    pMyObject->RunBuiltInCmd
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSession->SessionId,
                            (ANSC_HANDLE)&env,
                            SCLI_SHELL_BICODE_Exit
                        );

                status = 
                    pTscIf->Notify
                        (
                            pTscIf->hOwnerContext,
                            (ANSC_HANDLE)pSession->SessionId,
                            TELNET_TSC_EVENT_IdleSession,
                            (ANSC_HANDLE)NULL
                        );
            }
            else
            {
                pSession->bWillExit = TRUE;

                if ( pSession->bWaitInput )
                {
                    status =
                        pMyObject->BufferCmd
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pSession->SessionId,
                                SCLI_SHELL_BICODE_Break,
                                NULL
                            );
                }

                status = 
                    pBmc2SccIf->AbortExecution
                        (
                            pBmc2SccIf->hOwnerContext,
                            pSession->hBmc2Terminal
                        );
            }
        
            return status;
        }
    }

    AnscReleaseLock(&pMyObject->SessionListLock);

    return status;
}

