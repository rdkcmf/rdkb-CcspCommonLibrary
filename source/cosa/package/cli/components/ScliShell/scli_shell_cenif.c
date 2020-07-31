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

    module:	scli_shell_cenif.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/15/05    initial revision.

**********************************************************************/


#include "scli_shell_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoCenNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hExecEnv,
                ULONG                       CenEvent
            );

    description:

        This function is called to notify Telnet simple server
        what Shell object has done.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hExecEnv
                Telnet srv session object.

                ULONG                       CenEvent
                Notification event id.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoCenNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       CenEvent
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv = (PSCLI_SHELL_EXEC_ENV     )hExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pTelnetEnv   = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pTelnetEnv->hTscIf;
    ANSC_HANDLE                     hSrvSession  = pScliExecEnv->hSession;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    switch ( CenEvent )
    {
        case    SCLI_SHELL_CEN_EVENT_CmdBegin:

                pSession->bServing  = TRUE;

                pTscIf->Notify
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_TSC_EVENT_CmdBegin,
                        (ANSC_HANDLE)NULL
                    );

                break;

        case    SCLI_SHELL_CEN_EVENT_CmdEnd:

                if ( pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN && !pSession->bWillExit )
                {
                    pTscIf->SendShellPrompt
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            TRUE
                        );
                }

                pTscIf->Notify
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_TSC_EVENT_CmdEnd,
                        (ANSC_HANDLE)NULL
                    );

                pSession->bServing  = FALSE;

                if ( pSession->bWillExit )
                {
                    returnStatus =
                        pMyObject->RunBuiltInCmd
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSrvSession,
                                (ANSC_HANDLE)pTelnetEnv,
                                SCLI_SHELL_BICODE_Exit
                            );
                }
                else
                {
                    returnStatus =
                        pMyObject->DoBufferedCommands
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSrvSession,
                                hExecEnv
                            );
                }

                break;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoCenSetShellPrompt
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pPrompt
            )

    description:

        This function is called to set shell prompt as specified.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hExecEnv
                Scli terminal info.

                PUCHAR                      pPrompt
                Prompt to be set.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoCenSetShellPrompt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pPrompt
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv    = (PSCLI_SHELL_EXEC_ENV     )hExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pTelnetEnv   = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pTelnetEnv->hTscIf;
    ANSC_HANDLE                     hSrvSession  = pScliExecEnv->hSession;

    return 
        pTscIf->SetShellPrompt
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                pPrompt
            );
}

