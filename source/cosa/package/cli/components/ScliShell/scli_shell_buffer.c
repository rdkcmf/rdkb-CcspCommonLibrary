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

    module:	scli_shell_buffer.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

        *   ScliShoDoBufferedCommands
        *   ScliShoGetBufferedCmd
        *   ScliShoBufferCmd
        *   ScliShoClearBufferCmd
             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/10/05    initial revision.

**********************************************************************/


#include "scli_shell_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoDoBufferedCommands
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv
            )

    description:

        This function is called to run current command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoDoBufferedCommands
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv = (PSCLI_SHELL_EXEC_ENV     )hExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pTelnetEnv   = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pTelnetEnv->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    BOOL                            bCmdAvailable;
    PSCLI_SHELL_BUFFERED_CMD_LIST   pBufferedCmdArray; 

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_RESOURCES;
    }

    pBufferedCmdArray   = &pSession->BufferedCmd;

    if ( TRUE )
    {
        AnscAcquireLock(&pSession->AccessLock);
        bCmdAvailable   = ( pBufferedCmdArray->ulCount != 0 );
        AnscReleaseLock(&pSession->AccessLock);

        if ( bCmdAvailable )
        {
                pMyObject->RunCmd
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        (ANSC_HANDLE)pTelnetEnv
                    );
        }
        else
        {
            AnscAcquireLock(&pSession->AccessLock);
            
            if ( pSession->CommandLen != 0 )
            {
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pSession->Command,
                            pSession->CommandLen
                        );
            }

            AnscReleaseLock(&pSession->AccessLock);
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoGetBufferedCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                PULONG                      pulCmdCode,
                PUCHAR                      *ppCmd
            )

    description:

        This function is called to get first available command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                PULONG                      pulCmdCode
                command code.

                PUCHAR                      *ppCmd
                command string.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoGetBufferedCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PULONG                      pulCmdCode,
        PUCHAR                      *ppCmd
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject           = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_BUFFERED_CMD_LIST   pBufferedCmdArray;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PSCLI_SHELL_BUFFERED_CMD        pCmd                = NULL;
    ULONG                           i;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    AnscAcquireLock(&pSession->AccessLock);

    pBufferedCmdArray   = &pSession->BufferedCmd;

    if ( pulCmdCode )
    {
        *pulCmdCode = pCmd ? pCmd->ulCmdCode : SCLI_SHELL_BICODE_Unrecognized;
    }

    if ( ppCmd )
    {
        *ppCmd  = NULL;
    }

    if ( pBufferedCmdArray->ulCount != 0 )
    {
        pCmd    = &pBufferedCmdArray->pCmds[0];

		if ( pulCmdCode )
		{
			*pulCmdCode = pCmd->ulCmdCode;
		}

		if ( ppCmd )
		{
			*ppCmd  = pCmd->pCmd;
		}

        for (i = 1; i < pBufferedCmdArray->ulCount; i ++)
        {
            pBufferedCmdArray->pCmds[i - 1] = pBufferedCmdArray->pCmds[i];
        }

        pBufferedCmdArray->ulCount --;
    }
    else
    {
        returnStatus = ANSC_STATUS_PENDING;
    }

    AnscReleaseLock(&pSession->AccessLock);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoBufferCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                PUCHAR                      pCmd
            )

    description:

        This function is called to buffer the given command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                PUCHAR                      pCmd
                Command to be cached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoBufferCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ULONG                       ulCmdCode,
        PUCHAR                      pCmd
    )
{
    PSCLI_SHELL_OBJECT              pMyObject           = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_BUFFERED_CMD_LIST   pBufferedCmdArray;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    /* we allow empty string to be cached to support cases like user just 
     * hits ENTER to accept default value and so on 
     */
    /*
    if ( !pCmd || AnscSizeOfString( pCmd ) == 0 )
    {
        return ANSC_STATUS_SUCCESS;
    }
    */

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    pBufferedCmdArray   = &pSession->BufferedCmd;

    if ( pBufferedCmdArray->ulCount >= pBufferedCmdArray->ulSize )
    {
        AnscReleaseLock(&pSession->AccessLock);
        return ANSC_STATUS_RESOURCES;
    }

    pBufferedCmdArray->pCmds[pBufferedCmdArray->ulCount].pCmd      = (PUCHAR)(pCmd ? AnscCloneString((PCHAR)pCmd) : NULL);
    pBufferedCmdArray->pCmds[pBufferedCmdArray->ulCount].ulCmdCode = ulCmdCode;

    pBufferedCmdArray->ulCount ++;

    if ( pSession->hActiveTextBox || pSession->bWaitInput )
    {
        AnscSetEvent((PSEM_EVENT *)&pSession->InputEvent);
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoClearBufferCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            )

    description:

        This function is called to clear buffered commands.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession,
                Shell session to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoClearBufferCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSCLI_SHELL_SESSION_EXEC        pSession     = (PSCLI_SHELL_SESSION_EXEC)hSession;

    if ( pSession )
    {
        PSCLI_SHELL_BUFFERED_CMD_LIST   pBufferedCmdArray; 

        pBufferedCmdArray   = &pSession->BufferedCmd;

        if ( pBufferedCmdArray->pCmds )
        {
            ULONG                   i;

            for (i = 0; i < pBufferedCmdArray->ulCount; i ++)
            {
                if ( pBufferedCmdArray->pCmds[i].pCmd )
                {
                    AnscFreeMemory(pBufferedCmdArray->pCmds[i].pCmd);
                }
            }
        }
    
        pBufferedCmdArray->ulCount  = 0;
    }

    return ANSC_STATUS_SUCCESS;
}


