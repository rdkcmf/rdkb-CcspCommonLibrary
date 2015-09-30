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

    module:	scli_shell_process.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

        *   ScliShoAccept
        *   ScliShoServe
        *   ScliShoSignal
        *   ScliShoGetSession
        *   ScliShoCreateSession
        *   ScliShoRemoveSession
        *   ScliShoStartUserAuth
        *   ScliShoProcessCmdData
        *   ScliShoRunCmd
        *   ScliShoCacheCmd
        *   ScliShoRunBuiltInCmd
        *   ScliShoRunNonBuiltInCmd
        *   ScliShoValidateCmdArgs
        *   ScliShoGetCmdCode
        *   ScliShoGetPrevCmd
        *   ScliShoGetNextCmd
        *   ScliShoGetEscCmdCode
        *   ScliShoGetInputMode
        *   ScliShoRedrawTextBoxInput
        *   ScliShoMoveToTextBox
        *   ScliShoDoAuth
        *   ScliShoGetNumActiveSessions
        *   ScliShoRunShellCmd
             
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

        BOOL
        ScliShoAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClientInfo
            );

    description:

        This function is called to determine if the given connection
        should be accepted.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClientInfo
                Specifies the parameter for TSP object to process.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
ScliShoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoServe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv, 
                PVOID                       pCmd,
                ULONG                       ulCmdSize
            );

    description:

        This function is called to interpret the given telnet
        data.

        NOTICES:
          After the given command has been executed without any
          signal fired, TSP object needs to send out notification
          TELNET_CMD_EXEC_EVENT_End via Telnet Tsc interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Telnet session object.

                ANSC_HANDLE                 hExecEnv
                execution property.

                PVOID                       pCmd
                Buffer containing telnet data.

                ULONG                       ulCmdSize
                Telnet data size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoServe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv, 
        PVOID                       pCmd,
        ULONG                       ulCmdSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           SessionState;
    BOOL                            bComplete, bDM;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_RESOURCES;
    }

    if ( pProperty->bUserAuth )
    {
        pMyObject->StartTdo((ANSC_HANDLE)pMyObject);
    }

    SessionState = pSession->SessionState;

    AnscAcquireLock(&pSession->AccessLock);

    pSession->LastRecvTS    = AnscGetTickInSeconds();

    returnStatus =
        pMyObject->ProcessCmdData  
            (
                (ANSC_HANDLE)pMyObject,
                hSrvSession,
                hExecEnv,
                pCmd,
                ulCmdSize
            );

    SCLI_SHELL_BUFCMD_AVAILABLE
        (
            pSession->Command,
            pSession->CommandLen,
            bComplete,
            bDM
        );

    AnscReleaseLock(&pSession->AccessLock);

    if ( bComplete )
    {
        PUCHAR                      pCmd        = NULL;
        PUCHAR                      pCr;
        ULONG                       ulCmdLen    = 0;

        AnscAcquireLock(&pSession->AccessLock);

        pCmd    = NULL;
        pCr     = (PUCHAR)_ansc_memchr(pSession->Command, TELNET_CHAR_CR, pSession->CommandLen);

        ulCmdLen    = pCr - pSession->Command;

        if ( ulCmdLen != 0 )
        {
            pCmd    = (PUCHAR)AnscAllocateMemory(ulCmdLen + 1);

            if ( pCmd )
            {
                AnscCopyMemory(pCmd, pSession->Command, ulCmdLen);

                pCmd[ulCmdLen]  = 0;
            }
        }

        if ( TRUE )
        {
            int                     i;
            PUCHAR                  pNext       = pCr + 2;
            int                     nSizeLeft   = pSession->CommandLen - ulCmdLen - 2;

            if ( nSizeLeft > 0 )
            {
                for (i = 0; i < nSizeLeft; i ++)
                {
                    pSession->Command[i]    = pNext[i];
                }
            }

            pSession->CommandLen    = ( nSizeLeft > 0 ) ? nSizeLeft : 0;
            if ( pSession->CursorPos >= ulCmdLen + 2 )
            {
                pSession->CursorPos     -= (ulCmdLen + 2);
            }
            else
            {
                pSession->CursorPos     = pSession->CommandLen;
            }
        }

        AnscReleaseLock(&pSession->AccessLock);

        if ( pCmd )
        {
            AnscAcquireLock(&pSession->AccessLock);

            returnStatus =
                pMyObject->BufferCmd
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        SCLI_SHELL_BICODE_Unrecognized,
                        pCmd
                    );

            AnscReleaseLock(&pSession->AccessLock);

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                returnStatus = 
                    pTscIf->RingBell
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession
                        );
            }

            AnscFreeMemory(pCmd);
        }
    }

    if ( bComplete && !pSession->bServing )
    {
        returnStatus = 
            pMyObject->RunCmd
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoSignal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ULONG                       ulTspSignal
            );

    description:

        This function is called to signal execution engine to take
        the relevant action for the given signal. TSP object may
        optionally suppress this signal.

        NOTICES:
          In order to make telnet server work correctly when TSP
          object wants to suppress signals, it needs to let this 
          call return immediately and after execution is done, it
          needs to send out two execution events - firstly,
          TELNET_CMD_EXEC_EVENT_End and the secondly 
          TELNET_CMD_EXEC_EVENT_SignalAck_Break or
          TELNET_CMD_EXEC_EVENT_SignalAck_Abort according to what
          type of this signal is via Telnet Tsc interface.

          If TSP takes the corresponding action, it also needs to
          send out the two events in the same order aforementioned.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                ULONG                       ulTspSignal
                Signal ID.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSignal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       ulTspSignal
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    BOOL                            bNotRunning  = FALSE;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    BOOL                            bServing     = TRUE;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    if ( !pSession )
    {
        if ( ulTspSignal != TELNET_TSP_SIGNAL_SessionCreated )
        {
            return ANSC_STATUS_RESOURCES;
        }
        else
        {
            pSession    = 
                (PSCLI_SHELL_SESSION_EXEC)pMyObject->CreateSession
                    (
                        (ANSC_HANDLE)pMyObject, 
                        (ULONG)hSrvSession
                    );
        }
    }

    switch ( ulTspSignal )
    {
        case    TELNET_TSP_SIGNAL_Synch:
        case    TELNET_TSP_SIGNAL_Abort:

                if ( !pSession->bServing )
                {
                    returnStatus = 
                        pTscIf->SendDataMark
                            (
                                pTscIf->hOwnerContext,
                                hSrvSession
                            );

                    returnStatus = 
                        pTscIf->SendShellPrompt
                            (
                                pTscIf->hOwnerContext,
                                hSrvSession,
                                TRUE
                            );
                }
                else
                {
                    if ( pMyObject->hBmc2SccIf )
                    {
                        returnStatus = 
                            pBmc2SccIf->AbortExecution
                                (
                                    pBmc2SccIf->hOwnerContext,
                                    pSession->hBmc2Terminal
                                );
                    }
                }

                break;

        case    TELNET_TSP_SIGNAL_SessionCreated:

                returnStatus = 
                    pMyObject->StartUserAuth
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv
                        );

                break;

        case    TELNET_TSP_SIGNAL_SessionTerminated:

                /* clean up TSP session info */

                pMyObject->RemoveSession((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSession);

                break;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        ScliShoGetSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       SessionId
            )

    description:

        This function is called to get the session information and
        it will create a session for new one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       SessionId,
                Session Id.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
ScliShoGetSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PSCLI_SHELL_SESSION_EXEC        pSession     = NULL;
    BOOL                            bFound       = FALSE;

    AnscAcquireLock(&pMyObject->SessionListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SessionList);
    while ( pSLinkEntry )
    {
        pSession    = ACCESS_SCLI_SHELL_SESSION_EXEC_OBJECT(pSLinkEntry);

        if ( pSession->SessionId == SessionId )
        {
            bFound  = TRUE;

            break;
        }

        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
    }

    AnscReleaseLock(&pMyObject->SessionListLock);

    return bFound ? (ANSC_HANDLE)pSession : NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        ScliShoCreateSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       SessionId
            )

    description:

        This function is called to create a new shell session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       SessionId,
                Session Id.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
ScliShoCreateSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PSCLI_SHELL_SESSION_EXEC        pSession     = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, SessionId);

    if ( !pSession )
    {
        pSession    = (PSCLI_SHELL_SESSION_EXEC)AnscAllocateMemory(sizeof(SCLI_SHELL_SESSION_EXEC));

        if ( pSession )
        {
            AnscInitializeLock(&pSession->AccessLock);

            pSession->SessionId             = SessionId;
            pSession->bServing              = FALSE;
            pSession->CommandLen            = 0;
            pSession->ulFirstVisiblePos     = 0;
            pSession->InputMode             = SCLI_SHELL_INPUT_MODE_linemode;
            pSession->NumAuthFailure        = 0;

            if ( pProperty->bUserAuth )
            {
                pSession->bAuthenticated    = FALSE;
                pSession->SessionState      = SCLI_SHELL_STATE_INITIATED;
            }
            else
            {
                pSession->bAuthenticated    = TRUE;
                pSession->SessionState      = SCLI_SHELL_STATE_WAITING_PASSWORD;
            }

            pSession->hScliShellObj     = (ANSC_HANDLE)pMyObject;
            pSession->bGreeting         = TRUE;

            /* initialize commmand cache */
            if ( TRUE )
            {
                PSCLI_SHELL_SESSION_CMD_CACHE pCmdCache   = &pSession->CmdCache;

                pCmdCache->CacheSize    = pProperty->CmdCacheMaxCount;
                pCmdCache->Cursor       = 0;
                pCmdCache->NumOfCmds    = 0;
                pCmdCache->CacheHead    = 0;
                pCmdCache->pCmds        = (PUCHAR *)AnscAllocateMemory(pCmdCache->CacheSize * sizeof(PUCHAR));

                if ( !pCmdCache->pCmds )
                {
                    goto FAILURE;
                }
            }            

            /* initialize buffered command array */
            if ( TRUE )
            {
                PSCLI_SHELL_BUFFERED_CMD_LIST    pBufferedCmdArray   = &pSession->BufferedCmd;

                pBufferedCmdArray->ulSize   = SCLI_SHELL_BUFFERED_COMMAND_MAX_COUNT;
                pBufferedCmdArray->ulCount  = 0;
                pBufferedCmdArray->pCmds    = 
                    (PSCLI_SHELL_BUFFERED_CMD)AnscAllocateMemory(pBufferedCmdArray->ulSize * sizeof(SCLI_SHELL_BUFFERED_CMD));

                if ( !pBufferedCmdArray->pCmds )
                {
                    goto FAILURE;
                }
            }

            /* create ICE interface */
            if ( TRUE )
            {
                PBMC2_ICE_INTERFACE         pIceIf;

                pIceIf = (PBMC2_ICE_INTERFACE)AnscAllocateMemory(sizeof(BMC2_ICE_INTERFACE));

                if ( !pIceIf )
                {
                    goto FAILURE;
                }
                else
                {
                    pSession->hBmc2IceIf    = (ANSC_HANDLE)pIceIf;
                }

                AnscCopyString(pIceIf->Name, SCLI_SHELL_ICE_INTERFACE_NAME);

                pIceIf->InterfaceId         = SCLI_SHELL_ICE_INTERFACE_ID;
                pIceIf->hOwnerContext       = (ANSC_HANDLE)pSession;
                pIceIf->Size                = sizeof(BMC2_ICE_INTERFACE);

                pIceIf->Write               = ScliShoIceWrite;
                pIceIf->ReadInput           = ScliShoIceReadInput;
                pIceIf->ReadInput2          = ScliShoIceReadInput2;
                pIceIf->ClearScreen         = ScliShoIceClearScreen;
            }

            AnscInitializeEvent(&pSession->InputEvent);

            /* textbox */
            pSession->hActiveTextBox        = (ANSC_HANDLE)NULL;
            pSession->bWaitInput            = FALSE;

            pSession->bCursorFwdOutbox      = FALSE;

            AnscAcquireLock(&pMyObject->SessionListLock);
            AnscSListPushEntry(&pMyObject->SessionList, &pSession->Linkage);
            AnscReleaseLock(&pMyObject->SessionListLock);
        }
    }

    pSession->LastRecvTS    = AnscGetTickInSeconds();

    goto EXIT;

FAILURE:

    if ( pSession )
    {
        pMyObject->RemoveSession((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSession);

        pSession    = NULL;
    }

EXIT:

    return (ANSC_HANDLE)pSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRemoveSession
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            );

    description:

        This function is called to remove session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession,
                Session to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRemoveSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT      )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY    )&pMyObject->Property;
    PSCLI_SHELL_SESSION_EXEC        pSession     = (PSCLI_SHELL_SESSION_EXEC)hSession;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE     )pMyObject->hBmc2SccIf;
    PBMC2_ICE_INTERFACE             pIceIf       = pSession->hBmc2IceIf;

    AnscAcquireLock(&pMyObject->SessionListLock);
    AnscSListPopEntryByLink(&pMyObject->SessionList, &pSession->Linkage);
    AnscReleaseLock(&pMyObject->SessionListLock);

    if ( TRUE )
    {
        PSCLI_SHELL_SESSION_CMD_CACHE   pCmdCache;
        PSCLI_SHELL_BUFFERED_CMD_LIST   pBufferedCmdArray; 

        if ( pSession->hBmc2Terminal )
        {
            pBmc2SccIf->DelTerminal
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal
                );

            pSession->hBmc2Terminal = NULL;
        }

        pCmdCache   = &pSession->CmdCache;

        if ( pCmdCache->pCmds )
        {
            ULONG                   i;
            ULONG                   ulPos;
            
            for (i = 0; i < pCmdCache->NumOfCmds; i ++)
            {
                ulPos   = (pCmdCache->CacheHead + i) % pCmdCache->CacheSize;

                if ( pCmdCache->pCmds[ulPos] )
                {
                    AnscFreeMemory(pCmdCache->pCmds[ulPos]);
                }
            }

            AnscFreeMemory(pCmdCache->pCmds);
        }

        pBufferedCmdArray   = &pSession->BufferedCmd;

        if ( pBufferedCmdArray->pCmds )
        {
            pMyObject->ClearBufferCmd
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSession
                );

            AnscFreeMemory(pBufferedCmdArray->pCmds);
        }

        AnscFreeMemory(pIceIf);

        if ( pSession->hCeeExecEnv )
        {
            AnscFreeMemory(pSession->hCeeExecEnv);
        }

        AnscFreeEvent(&pSession->InputEvent);

        AnscFreeMemory(pSession);
    }

    /* remove idle timer if there's no session */
    if (pMyObject->GetNumActiveSessions((ANSC_HANDLE)pMyObject) == 0)
    {
        pMyObject->StopTdo((ANSC_HANDLE)pMyObject);
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoStartUserAuth
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv
            )

    description:

        This function is called to start user authentication.

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
ScliShoStartUserAuth
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    BOOL                            bNotRunning  = FALSE;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    if ( pSession->bGreeting )
    {
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                pProperty->Greeting,
                AnscSizeOfString(pProperty->Greeting)
            );

        pSession->bGreeting     = FALSE;
    }

    pTscIf->Output
        (
            pTscIf->hOwnerContext,
            hSrvSession,
            "\r\n",
            2
        );

    if ( pProperty->bUserAuth )
    {
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                SCLI_SHELL_LOGIN,
                AnscSizeOfString(SCLI_SHELL_LOGIN)
            );

        pSession->SessionState  = SCLI_SHELL_STATE_WAITING_USERNAME;

        pSession->CommandLen    = 0;
        pSession->Command[0]    = 0;
        pSession->CursorPos     = 0;
    }
    else
    {
        /*
        pTscIf->SendShellPrompt
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                TRUE
            );

        pSession->SessionState = SCLI_SHELL_STATE_LOGGED_IN; 
        */

        pMyObject->RunCmd((ANSC_HANDLE)pMyObject, hSrvSession, hExecEnv);
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoProcessCmdData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PVOID                       pCmd,
                ULONG                       ulCmdLen
            )

    description:

        This function is called to process data received from
        telnet client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                PVOID                       pCmd
                Data received.
                    
                ULONG                       ulCmdLen
                Data size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoProcessCmdData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PVOID                       pCmd,
        ULONG                       ulCmdLen
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           SessionState;
    BOOL                            bProcSpecial = FALSE;    
    ULONG                           ulLeftSize;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

#if 0
/* debug code */
if ( 1 )
{
    /* dump command line content */    
    ULONG   j;
    char*   pCmdStr = (char*)pCmd;

    if ( _ansc_strstr(pCmdStr, "\r") || _ansc_strstr(pCmdStr, "\n") )
    {
    char*   pLogBuf = (char*)AnscAllocateMemory(ulCmdLen * 4);
    CcspCliTrace(("\n[ProcessCmdData] Command line content length: %d -->\n", ulCmdLen));
    for ( j = 0; j < ulCmdLen; j ++ )
    {
        _ansc_sprintf(pLogBuf + AnscSizeOfString(pLogBuf), "%.2x ", pCmdStr[j]);
    }
    CcspCliTrace(("%s\n<--[ProcessCmdData]\n", pLogBuf));
    AnscFreeMemory(pLogBuf);
    }
}
#endif

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_RESOURCES;
    }

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    ulLeftSize  = SCLI_SHELL_MAX_COMMAND_LEN - pSession->CommandLen;

    SessionState = pSession->SessionState;

    bProcSpecial = (SessionState == SCLI_SHELL_STATE_LOGGED_IN);

    if ( TRUE )
    {
        ULONG                       i;
        PUCHAR                      pData       = (PUCHAR)pCmd;
        UCHAR                       uc;
        PUCHAR                      pEchoBuf    = NULL;
        ULONG                       ulEchoSize  = 0;

        if ( pExecEnv->bEcho )
        {
            pEchoBuf    = (PUCHAR)AnscAllocateMemory( ulCmdLen * 4 + 2 );

            if ( !pEchoBuf )
            {
                return ANSC_STATUS_RESOURCES;
            }
        }

        for (i = 0; i < ulCmdLen; i ++)
        {
            if ( pSession->CommandLen >= SCLI_SHELL_MAX_COMMAND_LEN )
            {
                uc  = TELNET_CHAR_BELL;

                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        &uc,
                        1
                    );

                returnStatus = ANSC_STATUS_BAD_SIZE;

                break;
            }

            uc  = pData[i];

            switch ( uc )
            {
                case    TELNET_CHAR_ESC:

                        if ( TRUE )
                        {
                            ULONG   ulCmdCode       = SCLI_SHELL_BICODE_Unrecognized;
                            ULONG   ulEscCodeLen    = 0;
                            BOOL    bRunCmd         = TRUE;

                            ulCmdCode   = 
                                pMyObject->GetEscCmdCode
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        pData,
                                        ulCmdLen - i,
                                        &ulEscCodeLen
                                    );

                            i += ulEscCodeLen - 1;

                            if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
                            {
                                switch ( ulCmdCode )
                                {
                                    case    SCLI_SHELL_BICODE_MoveToPrevTextBox:
                                    case    SCLI_SHELL_BICODE_MoveToNextTextBox:
                                    case    SCLI_SHELL_BICODE_PrevCmd:
                                    case    SCLI_SHELL_BICODE_NextCmd:
                                    case    SCLI_SHELL_BICODE_FirstCmd:
                                    case    SCLI_SHELL_BICODE_LastCmd:
                                    case    SCLI_SHELL_BICODE_AutoCompletion:
                                    case    SCLI_SHELL_BICODE_Help:

                                            /* suppress these commands */
                                            bRunCmd = FALSE;

                                            break;
                                }
                            }

                            if ( bRunCmd && ulCmdCode != SCLI_SHELL_BICODE_Unrecognized )
                            {
                                returnStatus = 
                                    pMyObject->RunBuiltInCmd
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            hExecEnv,
                                            ulCmdCode
                                        );
                            }
                        }

                        break;

                case    TELNET_CHAR_DEL:

                        if ( TRUE )
                        {
                            returnStatus =
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Del
                                    );

                            ulLeftSize  = SCLI_SHELL_MAX_COMMAND_LEN - pSession->CommandLen;
                        }

                        break;

                case    TELNET_CHAR_BACKSPACE:

                        if ( TRUE )
                        {
                            returnStatus =
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Backspace
                                    );

                            ulLeftSize  = SCLI_SHELL_MAX_COMMAND_LEN - pSession->CommandLen;
                        }

                        break;

                case    TELNET_CHAR_BREAK:

                        pMyObject->ClearBufferCmd((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSession);

                        if ( pSession->SessionState != SCLI_SHELL_STATE_LOGGED_IN )
                        {
                            returnStatus =
                                pMyObject->StartUserAuth
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv
                                    );

                            break;
                        }

                        if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
                        {
                            returnStatus =
                                pMyObject->BufferCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        SCLI_SHELL_BICODE_Break,
                                        NULL
                                    );

                            returnStatus = 
                                pBmc2SccIf->AbortExecution
                                    (
                                        pBmc2SccIf->hOwnerContext,
                                        pSession->hBmc2Terminal
                                    );
                        }
                        else
                        {
                            if ( !pSession->bServing )
                            {
                                returnStatus =
                                    pMyObject->RunBuiltInCmd
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            hExecEnv,
                                            SCLI_SHELL_BICODE_Break
                                        );

                                ulLeftSize  = SCLI_SHELL_MAX_COMMAND_LEN;
                            }
                            else
                            {
                                returnStatus = 
                                    pBmc2SccIf->AbortExecution
                                        (
                                            pBmc2SccIf->hOwnerContext,
                                            pSession->hBmc2Terminal
                                        );
                            }
                        }

                        break;

                case    TELNET_CHAR_CTRLD:

                        if ( !pSession->bServing )
                        {
                            returnStatus = 
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Exit
                                    );
                        }
                        else
                        {
                            pSession->bWillExit = TRUE;

                            if ( pSession->bWaitInput )
                            {
                                returnStatus =
                                    pMyObject->BufferCmd
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            SCLI_SHELL_BICODE_Break,
                                            NULL
                                        );
                            }

                            returnStatus = 
                                pBmc2SccIf->AbortExecution
                                    (
                                        pBmc2SccIf->hOwnerContext,
                                        pSession->hBmc2Terminal
                                    );

                            /*
                            returnStatus = 
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Exit
                                    );
                            */
                        }

                        break;

                case    TELNET_CHAR_CR:

                        if ( ulLeftSize >= 2 )
                        {
                            BOOL    bLfExist    = FALSE;

                            bLfExist = (i < ulCmdLen - 1) && ( pData[i+1] == TELNET_CHAR_LF || pData[i+1] == TELNET_CHAR_NULL );

                            if ( SCLI_SHELL_INPUT_IN_LINEMODE )
                            {
                                if ( !pSession->bServing )
                                {
                                    pSession->Command[pSession->CommandLen ++]  = TELNET_CHAR_CR;
                                    pSession->Command[pSession->CommandLen ++]  = TELNET_CHAR_LF;
                                }
                                ulLeftSize -=2;

                                if ( pEchoBuf )
                                {
                                    pEchoBuf[ulEchoSize ++] = TELNET_CHAR_CR;
                                    pEchoBuf[ulEchoSize ++] = TELNET_CHAR_LF;
                                }

                                if ( pSession->bServing )
                                {
                                    goto INPUT_DONE;
                                }
                            }
                            else
                            {
                                if ( pEchoBuf )
                                {
                                    pEchoBuf[ulEchoSize ++] = TELNET_CHAR_CR;
                                    pEchoBuf[ulEchoSize ++] = TELNET_CHAR_LF;
                                }

INPUT_DONE:
                                pSession->Command[pSession->CommandLen] = 0;

                                returnStatus =
                                    pMyObject->BufferCmd
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            SCLI_SHELL_BICODE_Unrecognized,
                                            pSession->Command
                                        );

                                pSession->CommandLen        = 0;
                                pSession->CursorPos         = 0;
                                pSession->ulFirstVisiblePos = 0;
                            }

                            if ( bLfExist )
                            {
                                i ++;
                            }
                        }
                        else
                        {
                            if ( pEchoBuf )
                            {
                                pEchoBuf[ulEchoSize ++] = TELNET_CHAR_BELL;
                            }
                        }

                        break;

                case    TELNET_CHAR_HTAB:

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
                        if ( !pSession->bOsShellActive && !pSession->bServing && pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN )
#else
                        if ( !pSession->bServing && pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN )
#endif
                        {
                            returnStatus = 
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_AutoCompletion
                                    );
                        }

                        break;

                case    SCLI_SHELL_HELP_CHAR:

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
                        if ( !pSession->bOsShellActive && !pSession->bServing && pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN )
#else
                        if ( !pSession->bServing && pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN )
#endif
                        {
                            returnStatus = 
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Help
                                    );
                        }

                        break;

                default:

                        returnStatus =
                            pMyObject->ProcessCmdChar
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    hSrvSession,
                                    hExecEnv,
                                    uc,
                                    pEchoBuf,
                                    &ulEchoSize
                                );

                        ulLeftSize --;

                        break;
            }
        }

        if ( ( !pSession->bServing || pSession->bWaitInput ) && pEchoBuf && ulEchoSize != 0 )
        {
            if ( SessionState != SCLI_SHELL_STATE_WAITING_PASSWORD )
            {
                returnStatus = 
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pEchoBuf,
                            ulEchoSize
                        );
            }
            else
            {
                PUCHAR              pCr;

                if ( pEchoBuf )
                {
                    pCr = (PUCHAR)_ansc_memchr( pEchoBuf, TELNET_CHAR_CR, ulEchoSize );

                    if ( pCr )
                    {
                        pEchoBuf[0]     = TELNET_CHAR_CR;
                        pEchoBuf[1]     = TELNET_CHAR_LF;
                        ulEchoSize      = 2;

                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    pEchoBuf,
                                    ulEchoSize
                                );
                    }
                    else
                    {
                        ULONG       i;

                        for (i = 0; i < ulEchoSize; i ++)
                        {
                            pEchoBuf[i]     = '*';
                        }

                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    pEchoBuf,
                                    ulEchoSize
                                );
                    }
                }
            }
        }

        if ( pEchoBuf )
        {
            AnscFreeMemory(pEchoBuf);
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoProcessCmdChar
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                UCHAR                       ucChar,
                PUCHAR                      pEchoBuf,
                PULONG                      pulEchoSize
            )

    description:

        This function is called to process data received from
        telnet client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                UCHAR                       ucChar
                Character to be processed which is not special 
                character.
                    
                PUCHAR                      pEchoBuf
                Echo buffer.

                PULONG                      pulEchoSize
                Echo buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoProcessCmdChar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        UCHAR                       ucChar,
        PUCHAR                      pEchoBuf,
        PULONG                      pulEchoSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    BOOL                            bProcSpecial = FALSE;    
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;
    int                             i;
    BOOL                            bCursorAtEnd = FALSE;
    ULONG                           ulCursorPos;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_RESOURCES;
    }

    ulCursorPos = pSession->CursorPos;

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CommandLen >= SCLI_SHELL_MAX_COMMAND_LEN ||
         ( SCLI_SHELL_INPUT_IN_TEXTBOX && pActiveTBox->bSizeFixed ) 
       )
    {
        if ( pActiveTBox && pActiveTBox->bSizeFixed )
        {
            if ( pSession->CommandLen != 0 && pSession->CursorPos < pSession->CommandLen )
            {
                pSession->CommandLen --;
            }

            goto INSERT_CHAR;
        }

        if ( pEchoBuf )
        {
            pEchoBuf[(*pulEchoSize) ++] = TELNET_CHAR_BELL;
        }
    }
    else
    if ( pSession->CursorPos >= pSession->CommandLen && !pSession->hActiveTextBox )
    {
        pSession->Command[pSession->CommandLen ++]  = ucChar;
        if ( pEchoBuf )
        {
            pEchoBuf[(*pulEchoSize) ++] = ucChar;
        }
        pSession->CursorPos ++;
    }
    else
    {
        for (i = pSession->CommandLen - 1; i >= (int)pSession->CursorPos; i --)
        {
            pSession->Command[i+1]  = pSession->Command[i];
        }

INSERT_CHAR:
        pSession->Command[pSession->CursorPos]   = ucChar;
        pSession->CommandLen ++;

        bCursorAtEnd    = ( SCLI_SHELL_INPUT_IN_TEXTBOX && pSession->CursorPos - pSession->ulFirstVisiblePos >= pActiveTBox->Width - 1 );

        if ( bCursorAtEnd )
        {
            ULONG                       ulWidth         = pActiveTBox->Width;
            ULONG                       ulCurTBoxPos    = pSession->CursorPos - pSession->ulFirstVisiblePos;

            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            if ( pActiveTBox && pActiveTBox->bSizeFixed )
            {
                pSession->CursorPos --;
            }
            else
            {
                pSession->ulFirstVisiblePos ++;
            }

            returnStatus =
                pMyObject->RedrawTextBoxInput
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv,
                        pActiveTBox->Width - 1
                    );
        }
        else
        {
            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            if ( pActiveTBox )
            {
                if ( ulCursorPos >= pSession->ulFirstVisiblePos )
                {
                    ulCursorPos -= pSession->ulFirstVisiblePos;
                    ulCursorPos ++;
                }
            }
            else
            {
                ulCursorPos ++;
            }

            returnStatus =
                pMyObject->RedrawTextBoxInput
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv,
                        ulCursorPos
                    );
        }

        pSession->CursorPos ++;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunCmd
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

static
ANSC_STATUS
ScliPostUserAuth
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hShellSession,
        ULONG                       TermPermission
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession     = (PSCLI_SHELL_SESSION_EXEC )hShellSession;
    PSCLI_SHELL_EXEC_ENV            pCeeExecEnv  = (PSCLI_SHELL_EXEC_ENV     )pSession->hCeeExecEnv;
    ULONG                           SessionState;

    pCeeExecEnv = (PSCLI_SHELL_EXEC_ENV)pSession->hCeeExecEnv;

    pMyObject->ShellPromptChanged
        (
            (ANSC_HANDLE)pMyObject,
            hSrvSession,
            hExecEnv,
            NULL
        );

    if ( !pCeeExecEnv )
    {
        pCeeExecEnv = (PSCLI_SHELL_EXEC_ENV)AnscAllocateMemory(sizeof(SCLI_SHELL_EXEC_ENV));
    }

    if ( pCeeExecEnv )
    {
        pCeeExecEnv->hSession   = hSrvSession;
        pCeeExecEnv->hTelnetEnv = hExecEnv;
    }

    pSession->hCeeExecEnv   = (ANSC_HANDLE)pCeeExecEnv;

    if ( TRUE )
    {
        PUCHAR                      pUserIdentifier = pSession->Username;
        BMC2_TERMINAL_INFO          TermInfo;

        TermInfo.UserIdentifier = pUserIdentifier;
        TermInfo.UserPermission = TermPermission;
        TermInfo.bReadOnly      = FALSE;    /* how to set the value ? */
        TermInfo.MaxLineNumber  = pExecEnv->WindowHeight;
        TermInfo.MaxColumnNumber= pExecEnv->WindowWidth;

        pSession->hBmc2Terminal =
            pBmc2SccIf->AddTerminal
                (
                    pBmc2SccIf->hOwnerContext,
                    &TermInfo,
                    pSession->hBmc2IceIf
                );
    }

    if ( pSession->hBmc2Terminal )
    {
        returnStatus =
            pTscIf->SendShellPrompt
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TRUE
               );

        pSession->SessionState  = SCLI_SHELL_STATE_LOGGED_IN;
    }
    else
    {
        PUCHAR  pErrMsg = "Internal error occurred!";

        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pErrMsg,
                    AnscSizeOfString(pErrMsg)
                );

        returnStatus =
            pMyObject->RunBuiltInCmd
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv,
                    SCLI_SHELL_BICODE_Exit
                );
    }

    return  returnStatus;
}


ANSC_STATUS
ScliShoRunCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           SessionState;
    ULONG                           TermPermission;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_RESOURCES;
    }

    SessionState = pSession->SessionState;

    switch ( SessionState )
    {
        default:
        case    SCLI_SHELL_STATE_WAITING_USERNAME:

                if ( TRUE )
                {
                    PUCHAR          pCmd;
                                   
                    returnStatus = pMyObject->GetBufferedCmd((ANSC_HANDLE)pMyObject, hSrvSession, NULL, &pCmd);

                    if ( !pCmd )
                    {
                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    SCLI_SHELL_LOGIN,
                                    AnscSizeOfString(SCLI_SHELL_LOGIN)
                                );

                        break;
                    }

                    AnscCopyString(pSession->Username, pCmd);

                    AnscFreeMemory(pCmd);

                    returnStatus = 
                        pTscIf->Output
                            (
                                pTscIf->hOwnerContext,
                                hSrvSession,
                                SCLI_SHELL_PASSWORD,
                                AnscSizeOfString(SCLI_SHELL_PASSWORD)
                            );

                    pSession->SessionState = SCLI_SHELL_STATE_WAITING_PASSWORD;
                }

                break;

        case    SCLI_SHELL_STATE_WAITING_PASSWORD:

                if ( TRUE )
                {
                    PUCHAR          pPassword   = NULL;
                    BOOL            bAuthOK     = TRUE;
                    ULONG           ulPerm      = (ULONG)-1;
                    BOOL            bPrivileged = FALSE;
                   
                    if ( pProperty->bUserAuth )
                    { 
                        returnStatus    = pMyObject->GetBufferedCmd((ANSC_HANDLE)pMyObject, hSrvSession, NULL, &pPassword);

                        if ( !pPassword )
                        {
                            returnStatus = 
                                pTscIf->Output
                                    (
                                        pTscIf->hOwnerContext,
                                        hSrvSession,
                                        SCLI_SHELL_LOGIN,
                                        AnscSizeOfString(SCLI_SHELL_LOGIN)
                                    );
    
                            pSession->SessionState = SCLI_SHELL_STATE_WAITING_USERNAME;

                            break;
                        }
                    }

                    /* do user authentication */
                    returnStatus =
                        pMyObject->DoAuth
                            (
                                (ANSC_HANDLE)pMyObject,
                                (PUCHAR     )pSession->Username,
                                (PUCHAR     )pPassword,
                                &ulPerm,
                                &pSession->bPrivileged,
                                &TermPermission,
                                &pMyObject->InactivityTimeout,
                                &pMyObject->LockoutTimeout
                            );

                    AnscFreeMemory(pPassword);

                    bAuthOK = ( returnStatus == ANSC_STATUS_SUCCESS );

                    if ( !bAuthOK )
                    {
                        pSession->NumAuthFailure ++;

                        if ( pSession->NumAuthFailure > SCLI_SHELL_MAX_AUTH_FAILURES )
                        {
                            returnStatus =
                                pTscIf->Output
                                    (
                                        pTscIf->hOwnerContext,
                                        hSrvSession,
                                        SCLI_SHELL_LOCKOUT,
                                        AnscSizeOfString(SCLI_SHELL_LOCKOUT)
                                    );

                            /* lock this terminal out */
                            returnStatus = 
                                pTscIf->Notify
                                    (
                                        pTscIf->hOwnerContext,
                                        hSrvSession,
                                        TELNET_TSC_EVENT_Lockout,
                                        (ANSC_HANDLE)pMyObject->LockoutTimeout
                                    );

                            returnStatus =
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        SCLI_SHELL_BICODE_Exit
                                    );
                        }
                        else
                        {
                            returnStatus =
                                pTscIf->Output
                                    (
                                        pTscIf->hOwnerContext,
                                        hSrvSession,
                                        SCLI_SHELL_AUTH_FAILURE,
                                        AnscSizeOfString(SCLI_SHELL_AUTH_FAILURE)
                                    );
                        }
                    }

                    if ( bAuthOK )
                    {
                        returnStatus = 
                            ScliPostUserAuth
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    hSrvSession,
                                    hExecEnv,
                                    (ANSC_HANDLE)pSession,
                                    TermPermission
                                );
                    }
                    else
                    {
                        returnStatus = 
                            pMyObject->StartUserAuth
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    hSrvSession,
                                    hExecEnv
                                );
                    }
                }

                break;

        case    SCLI_SHELL_STATE_LOGGED_IN:

                /* execute command */
                if ( TRUE )
                {
                    ULONG           ulPos = pSession->CommandLen;
                    PUCHAR          pCmd  = pSession->Command;
                    ULONG           CmdCode;

                    returnStatus = 
                        pMyObject->GetBufferedCmd
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSrvSession,
                                NULL,
                                &pCmd
                            );

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
                    if ( pSession->bOsShellActive )
                    {
                        if ( AnscEqualString(pCmd, "exit", FALSE) )
                        {
                            returnStatus = 
                                pMyObject->RunBicExit
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        (ANSC_HANDLE)NULL
                                    );
                        }
                        else
                        {
                            returnStatus = 
                                pMyObject->RunShellCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        pCmd
                                    );
                        }
                    }
                    else
#endif
                    {
                        returnStatus = pMyObject->CacheCmd((ANSC_HANDLE)pMyObject, hSrvSession, pCmd);
                        CmdCode = pMyObject->GetCmdCode((ANSC_HANDLE)pMyObject, pCmd);

                        if ( CmdCode == SCLI_SHELL_BICODE_Unrecognized )
                        {
                            if ( pCmd && _ansc_strchr(pCmd, SCLI_SHELL_HELP_CHAR) )
                            {
                                returnStatus = 
                                    pMyObject->RunBicHelp
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            hExecEnv,
                                            (ANSC_HANDLE)NULL
                                        );
                            }
                            else
                            {
                                returnStatus = 
                                    pMyObject->RunNonBuiltInCmd
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            hSrvSession,
                                            hExecEnv,
                                            pCmd
                                        );
                            }
                        }
                        else
                        {
                            returnStatus = 
                                pMyObject->RunBuiltInCmd
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        hSrvSession,
                                        hExecEnv,
                                        CmdCode
                                    );

                            pTscIf->SendShellPrompt
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    TRUE
                                );
                        }
                    }

                    if ( pCmd )
                    {
                        AnscFreeMemory(pCmd);
                    }
                }

                break;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoCacheCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                PUCHAR                      pCmd
            )

    description:

        This function is called to cache the given command.

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
ScliShoCacheCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT          )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY        )&pMyObject->Property;
    PSCLI_SHELL_SESSION_CMD_CACHE   pCmdCache;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulIndex;
    PUCHAR                          pOldCmd;
    ULONG                           ulPrevIndex;

    if ( !pCmd || AnscSizeOfString( pCmd ) == 0 )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pSession->bServing )
    {
        /* we are in command context */

        return ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pCmdCache->CacheSize == 0 )
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    ulIndex     = ( pCmdCache->CacheHead + pCmdCache->NumOfCmds ) % pCmdCache->CacheSize;
    ulPrevIndex = ( ulIndex != 0 ) ? ulIndex - 1 : pCmdCache->CacheSize - 1;

    if ( TRUE )
    {
        PUCHAR                      pPrevCmd    = pCmdCache->pCmds[ulPrevIndex];

        if ( pPrevCmd && AnscSizeOfString(pPrevCmd) == AnscSizeOfString(pCmd) )
        {
            if ( AnscEqualString2(pPrevCmd, pCmd, AnscSizeOfString(pCmd), FALSE) )
            {
                /* 
                 * don't cache this command since it's exactly the same as previous one,
                 * however we need to set the cursor to the end.
                 */
                pCmdCache->Cursor           = ulIndex;
                pCmdCache->bCursorAtTail    = TRUE;
                pCmdCache->bCursorAfterTail = TRUE;

                return ANSC_STATUS_DONT_WANT;
            }
        }
    }

    pOldCmd     = pCmdCache->pCmds[ulIndex];
    if ( pOldCmd )
    {
        AnscFreeMemory(pOldCmd);
    }

    pCmdCache->pCmds[ulIndex]   = AnscCloneString(pCmd);
    pCmdCache->Cursor           = (ulIndex + 1) % pCmdCache->CacheSize;

    if ( pCmdCache->NumOfCmds >= pCmdCache->CacheSize )
    {
        pCmdCache->CacheHead ++;

        if ( pCmdCache->CacheHead >= pCmdCache->CacheSize )
        {
            pCmdCache->CacheHead    = 0;
        }
    }
    else
    {
        pCmdCache->NumOfCmds ++;
    }

    pCmdCache->bCursorAtTail    = TRUE;
    pCmdCache->bCursorAfterTail = TRUE;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ScliShoIsCmdModified
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                PUCHAR                      pCmd
            )

    description:

        This function is called to determine if the given command
        should be added into cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                PUCHAR                      pCmd
                Command to be cached.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
ScliShoIsCmdModified
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT          )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY        )&pMyObject->Property;
    PSCLI_SHELL_SESSION_CMD_CACHE   pCmdCache;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulHeadIndex, ulTailIndex;
    ULONG                           ulCurIndex;

    if ( !pCmd || AnscSizeOfString(pCmd) == 0 )
    {
        return FALSE;
    }

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pCmdCache->NumOfCmds == 0 )
    {
        return TRUE;
    }

    ulHeadIndex = pCmdCache->CacheHead;
    ulTailIndex = pCmdCache->CacheHead + pCmdCache->NumOfCmds - 1;
    ulCurIndex  = pCmdCache->Cursor;

    if ( ulCurIndex < ulHeadIndex )
    {
        ulCurIndex  += pCmdCache->CacheSize;
    }

    if ( ulCurIndex >= ulTailIndex )
    {
        ulCurIndex  = ulTailIndex % pCmdCache->CacheSize;
    }

    if ( ulCurIndex >= pCmdCache->CacheSize )
    {
        ulCurIndex %= pCmdCache->CacheSize;
    }

    if ( !pCmdCache->pCmds[ulCurIndex] )
    {
        return TRUE;
    }

    if ( AnscSizeOfString(pCmd) != AnscSizeOfString(pCmdCache->pCmds[ulCurIndex]) )
    {
        return TRUE;
    }

    if ( AnscEqualString2(pCmdCache->pCmds[ulCurIndex], pCmd, AnscSizeOfString(pCmd), TRUE) )
    {
        return FALSE;
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBuiltInCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pCmd
            )

    description:

        This function is called to run built-in command if the given
        command is one of them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                ULONG                       CmdCode
                Command to be run.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBuiltInCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       CmdCode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    switch ( CmdCode )
    {
        case    SCLI_SHELL_BICODE_Exit:
        
                returnStatus =
                    pMyObject->RunBicExit
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_Clear:

                returnStatus =
                    pMyObject->RunBicClear
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
        case    SCLI_SHELL_BICODE_Shell:

                returnStatus =
                    pMyObject->RunBicShell
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;
#endif

        case    SCLI_SHELL_BICODE_PrevCmd:
        case    SCLI_SHELL_BICODE_NextCmd:
        case    SCLI_SHELL_BICODE_FirstCmd:
        case    SCLI_SHELL_BICODE_LastCmd:

                returnStatus =
                    pMyObject->RunBicHistory
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            CmdCode
                        );

                break;

        case    SCLI_SHELL_BICODE_CursorBackward:

                returnStatus =
                    pMyObject->RunBicCursorBackward
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_CursorForward:

                returnStatus =
                    pMyObject->RunBicCursorForward
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_CursorHome:

                returnStatus =
                    pMyObject->RunBicCursorHome
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_CursorEnd:

                returnStatus =
                    pMyObject->RunBicCursorEnd
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_Del:

                returnStatus =
                    pMyObject->RunBicDel
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                    break;

        case    SCLI_SHELL_BICODE_Backspace:

                returnStatus =
                    pMyObject->RunBicBackspace
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                    break;

        case    SCLI_SHELL_BICODE_Break:

                returnStatus =
                    pMyObject->RunBicBreak
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    SCLI_SHELL_BICODE_AutoCompletion:

                returnStatus =
                    pMyObject->RunBicAutoCompletion
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                    break;

        case    SCLI_SHELL_BICODE_Help:

                returnStatus =
                    pMyObject->RunBicHelp
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            (ANSC_HANDLE)NULL
                        );

                    break;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunNonBuiltInCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pCmd
            )

    description:

        This function is called to run NON built-in command if the given
        command is one of them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                PUCHAR                      pCmd
                Command to be run.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunNonBuiltInCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PSCLI_SHELL_CEN_INTERFACE       pCenIf       = (PSCLI_SHELL_CEN_INTERFACE)pMyObject->hCenIf;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_ASYNC_TASK_CONTEXT  pContext;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    pCenIf->Notify
        (
            pCenIf->hOwnerContext,
            pSession->hCeeExecEnv,
            SCLI_SHELL_CEN_EVENT_CmdBegin
        );

    if ( !pCmd || AnscSizeOfString(pCmd) == 0 )
    {
        pCenIf->Notify
            (
                pCenIf->hOwnerContext,
                pSession->hCeeExecEnv,
                SCLI_SHELL_CEN_EVENT_CmdEnd
            );

        return ANSC_STATUS_SUCCESS;
    }

    /* check validity of this command */
    if ( pMyObject->hBmc2SccIf )
    {
        BOOL                    bValidCmd, bAuthorized  = FALSE;
        BOOL                    bCanExecCmd     = FALSE;
        PUCHAR                  pCmdString      = AnscCloneString(pCmd);
        PUCHAR                  pCmdStringOrg   = pCmdString;
        ULONG                   i;

        if ( !pCmdString )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        SCLI_SHELL_SKIP_SPACE(pCmdString);

        for (i = 0; i < AnscSizeOfString(pCmdString); i ++)
        {
            if ( pCmdString[i] == TELNET_CHAR_SPACE || pCmdString[i] == TELNET_CHAR_HTAB )
            {
                pCmdString[i]   = 0;
                break;
            }
        }
        
        bValidCmd   = 
            pBmc2SccIf->IsCommandValid
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal,
                    pCmdString,
                    &bAuthorized
                );

        AnscFreeMemory(pCmdStringOrg);

        if ( !bValidCmd )
        {
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    SCLI_SHELL_INVALID_CMD,
                    AnscSizeOfString(SCLI_SHELL_INVALID_CMD)
                );

            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );
        }
        else if ( !bAuthorized )
        {
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    SCLI_SHELL_NO_PERMISSION,
                    AnscSizeOfString(SCLI_SHELL_NO_PERMISSION)
                );
        }
        else
        {
            returnStatus = pMyObject->ValidateCmdArgs((ANSC_HANDLE)pMyObject, hSrvSession, hExecEnv, pCmd);

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                bCanExecCmd = TRUE;
            }
        }

        if ( !bCanExecCmd )
        {
            pCenIf->Notify
                (
                    pCenIf->hOwnerContext,
                    pSession->hCeeExecEnv,
                    SCLI_SHELL_CEN_EVENT_CmdEnd
                );

            return ANSC_STATUS_SUCCESS;
        }
    }

    pContext    = (PSCLI_SHELL_ASYNC_TASK_CONTEXT)AnscAllocateMemory(sizeof(SCLI_SHELL_ASYNC_TASK_CONTEXT));

    if ( !pContext )
    {
        pCenIf->Notify
            (
                pCenIf->hOwnerContext,
                pSession->hCeeExecEnv,
                SCLI_SHELL_CEN_EVENT_CmdEnd
            );

        return ANSC_STATUS_RESOURCES;
    }

    pContext->pCmd              = AnscCloneString(pCmd);
    pContext->hScliShell        = (ANSC_HANDLE)pMyObject;
    pContext->hSrvSession       = hSrvSession;
    pContext->ExecEnv           = *pExecEnv;

    if ( pProperty->Mode & SCLI_SHELL_RUN_MODE_ASYNC )
    {
        returnStatus =
            AnscSpawnTask
                (
                    (void*      )pMyObject->AsyncRunCmd,
                    (ANSC_HANDLE)pContext,
                    "ScliShoAsyncTask"
                );
    }
    else
    {
        returnStatus =
            pMyObject->AsyncRunCmd
                (
                    (ANSC_HANDLE)pContext
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoValidateCmdArgs
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pCmd
            )

    description:

        This function is called to do basic command validation
        against command syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                PUCHAR                      pCmd
                Command to be run.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoValidateCmdArgs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PSCLI_SHELL_CEN_INTERFACE       pCenIf       = (PSCLI_SHELL_CEN_INTERFACE)pMyObject->hCenIf;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;
    PSCLI_SHELL_CMD_TLIST           pCmdTokenList= NULL;
    PSCLI_CMD_ARG_MATCH_RESULT      pReqArgM     = NULL;
    ULONG                           ulReqArgMLen = 0;
    PSCLI_CMD_ARG_MATCH_RESULT      pOptArgM     = NULL;
    ULONG                           ulOptArgMLen = 0;
    BOOL                            bMatched     = FALSE;
    int                             nErrorPos    = 0;
    char*                           pCommand;
    PBMC2_CMD_HELP_SYNTAX           pCmdHelp;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
    {
        return ANSC_STATUS_SUCCESS;
    }

    SCLI_SHELL_SKIP_SPACE(pCmd);

    if ( *pCmd == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pCmdTokenList = ScliShoTokenizeCommand(pCmd, SCLI_SHELL_CMD_TOKEN_SEPARATOR);

    if ( !pCmdTokenList )
    {
        return  returnStatus;   
    }

    pCommand    = pCmdTokenList->pTokens[0].pValue;
    nErrorPos   = (int)pCmdTokenList->pTokens[0].ulStartPos;

    pCmdHelp = 
        (PBMC2_CMD_HELP_SYNTAX)pBmc2SccIf->GetCommandSyntax
            (
                pBmc2SccIf->hOwnerContext,
                pSession->hBmc2Terminal,
                pCommand
            );

    if ( pCmdHelp )
    {
        ULONG                       i;

        ulReqArgMLen = pCmdHelp->ulReqArgCount;
        if ( ulReqArgMLen != 0 )
        {
            pReqArgM = (PSCLI_CMD_ARG_MATCH_RESULT)AnscAllocateMemory(sizeof(SCLI_CMD_ARG_MATCH_RESULT) * ulReqArgMLen);

            if ( !pReqArgM )
            {
                goto EXIT;
            }

            AnscZeroMemory(pReqArgM, sizeof(BOOL) * ulReqArgMLen);

            for ( i = 0; i < ulReqArgMLen; i ++ )
            {
                ScliShoInitArgMatchResult(&pReqArgM[i]);
            }
        }

        ulOptArgMLen = pCmdHelp->ulOptArgCount;
        if ( ulOptArgMLen != 0 )
        {
            pOptArgM = (PSCLI_CMD_ARG_MATCH_RESULT)AnscAllocateMemory(sizeof(SCLI_CMD_ARG_MATCH_RESULT) * ulOptArgMLen);

            if ( !pOptArgM )
            {
                goto EXIT;
            }

            AnscZeroMemory(pOptArgM, sizeof(BOOL) * ulOptArgMLen);

            for ( i = 0; i < ulOptArgMLen; i ++ )
            {
                ScliShoInitArgMatchResult(&pOptArgM[i]);
            }
        }

        bMatched = 
            ScliShoMatchCommand
                (
                    hThisObject,
                    pCmdHelp,
                    pCmdTokenList,
                    pReqArgM,
                    pOptArgM,
                    &nErrorPos
                );

        if ( !bMatched )
        {
            returnStatus =
                ScliShoIndicateError
                    (
                        hThisObject,
                        hSrvSession,
                        SCLI_SHELL_INVALID_ARG,
                        pCmd,
                        nErrorPos
                    );

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            ULONG                   i;
            int                     nErrorToken;
            BOOL                    bValueMissing = FALSE;

            /* make sure all required parameters are specified */
            for ( i = 0; i < ulReqArgMLen; i ++ )
            {
                if ( !pReqArgM[i].bMatched || !pReqArgM[i].bValueMatched )
                {
                    if ( pCmdTokenList->ulCurToken + 1 >= pCmdTokenList->ulTokenCount )
                    {
                        if ( ScliShoCanReqArgsSkipped
                                (
                                    hThisObject,
                                    pCmdTokenList,
                                    pCmdHelp->pReqArgs,
                                    pCmdHelp->ulReqArgCount,
                                    pReqArgM,
                                    i
                                ) )
                        {
                            /* the command is okay - the remaining required arguments are dependent args and conditions not met */
                            break;
                        }
                    }

                    nErrorToken = pReqArgM[i].nMatchTokenStart;
                    if ( nErrorToken >= 0 && nErrorToken < (int)pCmdTokenList->ulTokenCount )
                    {
                        if ( pReqArgM[i].bNamedArg && pReqArgM[i].bNameMatched )
                        {
                            if ( nErrorToken + 1 < (int)pCmdTokenList->ulTokenCount )
                            {
                                nErrorToken ++;
                            }
                            else
                            {
                                bValueMissing = TRUE;
                            }
                        }

                        nErrorPos = (int)pCmdTokenList->pTokens[nErrorToken].ulStartPos;

                        if ( bValueMissing )
                        {
                            nErrorPos += AnscSizeOfString(pCmdTokenList->pTokens[nErrorToken].pValue);
                        }
                    }
                    else
                    {
                        if ( i > 0 )
                        {
                            nErrorToken = pReqArgM[i-1].nMatchTokenEnd;
                        }
                        else
                        {
                            nErrorToken = 0;
                        }
                        nErrorPos = (int)(pCmdTokenList->pTokens[nErrorToken].ulStartPos + AnscSizeOfString(pCmdTokenList->pTokens[nErrorToken].pValue));
                    }

                    returnStatus =
                        ScliShoIndicateError
                            (
                                hThisObject,
                                hSrvSession,
                                SCLI_SHELL_INVALID_ARG,
                                pCmd,
                                nErrorPos
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto EXIT;
                }
            }

            /* make sure all optional parameters are specified correctly */
            for ( i = 0; i < ulOptArgMLen; i ++ )
            {
                PBMC2_CMD_ARGUMENT  pArg = &pCmdHelp->pOptArgs[i];

                if ( pOptArgM[i].bMatched && !pOptArgM[i].bValueMatched && 
                     (pArg->Type != BMC2_CMD_ARG_TYPE_dependent || pOptArgM[i].bNameMatched || pOptArgM[i].bNamePartialMatched || pOptArgM[i].bPartialValue) )
                {
                    nErrorToken = pOptArgM[i].nMatchTokenStart;
                    if ( nErrorToken >= 0 && nErrorToken < (int)pCmdTokenList->ulTokenCount )
                    {
                        nErrorPos = (int)pCmdTokenList->pTokens[nErrorToken].ulStartPos;
                    }
                    else
                    {
                        nErrorPos = 0;
                    }

                    returnStatus =
                        ScliShoIndicateError
                            (
                                hThisObject,
                                hSrvSession,
                                SCLI_SHELL_INVALID_ARG,
                                pCmd,
                                nErrorPos
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto EXIT;
                }
                else if ( !pOptArgM[i].bMatched )
                {
                    PBMC2_CMD_DEP_ARG   pDepArg = &pArg->Arg.Dependent;

                    if ( pArg->Type == BMC2_CMD_ARG_TYPE_dependent && pDepArg->ulArgBranchCount == 1 )
                    {
                        PBMC2_CMD_DEP_ARG_BRANCH    pBr = &pDepArg->pArgBranches[0];

                        if ( pBr->ulArgDepOn == BMC2_CMD_DEP_ARG_DEP_opt_arg_name )
                        {
                            int     nOptArgIndex;

                            nOptArgIndex = 
                                ScliShoGetOptNamedArgMatched
                                    (
                                        pCmdTokenList,
                                        pCmdHelp->pOptArgs,
                                        pCmdHelp->ulOptArgCount,
                                        pOptArgM,
                                        pBr->pArgValue
                                    );

                            if ( nOptArgIndex >= 0 )
                            {
                                int nErrorToken = pOptArgM[nOptArgIndex].nMatchTokenStart + 1;

                                if ( nErrorToken >= 0 && nErrorToken < (int)pCmdTokenList->ulTokenCount )
                                {
                                    nErrorPos = 
                                        (int)(pCmdTokenList->pTokens[nErrorToken].ulStartPos + 
                                              AnscSizeOfString(pCmdTokenList->pTokens[nErrorToken].pValue));
                                }
                                else
                                {
                                    nErrorPos = 0;
                                }

                                returnStatus =
                                    ScliShoIndicateError
                                        (
                                            hThisObject,
                                            hSrvSession,
                                            SCLI_SHELL_INVALID_ARG,
                                            pCmd,
                                            nErrorPos
                                        );

                                returnStatus = ANSC_STATUS_FAILURE;
                                goto EXIT;
                            }
                        }
                    }
                }
            }
        }
    }

EXIT:

    if ( pReqArgM )
    {
        AnscFreeMemory(pReqArgM);
    }

    if ( pOptArgM )
    {
        AnscFreeMemory(pOptArgM);
    }

    if ( pCmdTokenList )
    {
        ScliShoFreeCmdTokenList(pCmdTokenList);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ScliShoGetCmdCode
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pCmd
            )

    description:

        This function is called to get command code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pCmd
                Command to be run.

    return:     command code.

**********************************************************************/

ULONG
ScliShoGetCmdCode
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;
    ULONG                           CmdCode      = SCLI_SHELL_BICODE_Unrecognized;
    ULONG                           ulCmdLen     = pCmd ? AnscSizeOfString(pCmd) : 0;

    if ( !pCmd || ulCmdLen == 0 )
    {
        CmdCode = SCLI_SHELL_BICODE_Unrecognized;
    }
    else
    /*
    if ( ulCmdLen == AnscSizeOfString(SCLI_SHELL_BICMD_Exit) && 
         AnscEqualString2(pCmd, SCLI_SHELL_BICMD_Exit, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_Exit;
    }
    else 
    */
#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    if ( ulCmdLen == AnscSizeOfString(SCLI_SHELL_BICMD_Shell) && 
         AnscEqualString2(pCmd, SCLI_SHELL_BICMD_Shell, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_Shell;
    }
    else
#endif
    if ( ulCmdLen == AnscSizeOfString(SCLI_SHELL_BICMD_Clear) && 
         AnscEqualString2(pCmd, SCLI_SHELL_BICMD_Clear, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_Clear;
    }
    else
    if ( ulCmdLen == AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Up) && 
         AnscEqualString2(pCmd, TELNET_ANSI_TTY_CODE_Cursor_Up, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_PrevCmd;
    }
    else
    if ( ulCmdLen == AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Down) && 
         AnscEqualString2(pCmd, TELNET_ANSI_TTY_CODE_Cursor_Down, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_NextCmd;
    }
    else
    if ( ulCmdLen == AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward) && 
         AnscEqualString2(pCmd, TELNET_ANSI_TTY_CODE_Cursor_Backward, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_CursorBackward;
    }
    else
    if ( ulCmdLen == AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Forward) && 
         AnscEqualString2(pCmd, TELNET_ANSI_TTY_CODE_Cursor_Forward, ulCmdLen, FALSE)
       )
    {
        CmdCode = SCLI_SHELL_BICODE_CursorForward;
    }

    return CmdCode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        ScliShoGetPrevCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession
            )

    description:

        This function is called to get prev command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Srv session object.

    return:     command.

**********************************************************************/

PUCHAR
ScliShoGetPrevCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PSCLI_SHELL_SESSION_CMD_CACHE   pCmdCache;
    ULONG                           ulHeadIndex  = 0;
    ULONG                           ulPrevCmdIndex;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pCmdCache->NumOfCmds == 0 || ( pCmdCache->Cursor == pCmdCache->CacheHead && !pCmdCache->bCursorAtTail ) )
    {
        return NULL;
    }

    if ( pCmdCache->bCursorAfterTail )
    {
        ulPrevCmdIndex      = ( pCmdCache->CacheHead + pCmdCache->NumOfCmds - 1 ) % pCmdCache->CacheSize;
        pCmdCache->Cursor   = ulPrevCmdIndex;
    }
    else
    {
        ulHeadIndex = pCmdCache->CacheHead;

        if ( pCmdCache->Cursor != 0 )
        {
            pCmdCache->Cursor --;
            ulPrevCmdIndex  = pCmdCache->Cursor;
        }
        else
        {
            pCmdCache->Cursor   = pCmdCache->CacheSize - 1;
            ulPrevCmdIndex      = pCmdCache->Cursor;
        }
    }

    pCmdCache->bCursorAtTail    = FALSE;
    pCmdCache->bCursorAfterTail = FALSE;
    
    return pCmdCache->pCmds[ulPrevCmdIndex];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        ScliShoGetNextCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession
            )

    description:

        This function is called to get next command in cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Session object.

    return:     command.

**********************************************************************/

PUCHAR
ScliShoGetNextCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PSCLI_SHELL_SESSION_CMD_CACHE     pCmdCache;
    ULONG                           ulNextCmdIndex;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pCmdCache->NumOfCmds == 0 || pCmdCache->bCursorAtTail )
    {
        pCmdCache->bCursorAfterTail = TRUE;
    
        return NULL;
    }
    else if ( pCmdCache->NumOfCmds < pCmdCache->CacheSize && pCmdCache->Cursor >= pCmdCache->NumOfCmds - 1 )
    {
        pCmdCache->bCursorAfterTail = TRUE;

        return NULL;
    }

    if ( pCmdCache->Cursor < pCmdCache->CacheSize - 1 )
    {
        pCmdCache->Cursor ++;
        ulNextCmdIndex  = pCmdCache->Cursor;
    }
    else
    {
        pCmdCache->Cursor   = 0;
        ulNextCmdIndex      = pCmdCache->Cursor;
    }

    if ( pCmdCache->NumOfCmds >= pCmdCache->CacheSize )
    {
        pCmdCache->bCursorAtTail    = pCmdCache->Cursor == pCmdCache->CacheHead;
    }
    else
    {
        pCmdCache->bCursorAtTail    = ( pCmdCache->Cursor >= pCmdCache->NumOfCmds - 1 );
    }
    
    if ( pCmdCache->bCursorAtTail )
    {
        return NULL;
    }

    return pCmdCache->pCmds[ulNextCmdIndex];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        ScliShoGetFirstCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession
            )

    description:

        This function is called to get first command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Srv session object.

    return:     command.

**********************************************************************/

PUCHAR
ScliShoGetFirstCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PSCLI_SHELL_SESSION_CMD_CACHE     pCmdCache;
    ULONG                           ulHeadIndex  = 0;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pCmdCache->NumOfCmds == 0 )
    {
        return NULL;
    }

    ulHeadIndex = pCmdCache->CacheHead;

    pCmdCache->Cursor   = ulHeadIndex;

    if ( pCmdCache->NumOfCmds >= pCmdCache->CacheSize )
    {
        pCmdCache->bCursorAtTail    = FALSE;
    }
    else
    {
        pCmdCache->bCursorAtTail    = ( pCmdCache->Cursor >= pCmdCache->NumOfCmds - 1 );
    }
    
    pCmdCache->bCursorAfterTail     = FALSE;

    return pCmdCache->pCmds[ulHeadIndex];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        ScliShoGetLastCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession
            )

    description:

        This function is called to get last command in cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Session object.

    return:     command.

**********************************************************************/

PUCHAR
ScliShoGetLastCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PSCLI_SHELL_SESSION_CMD_CACHE     pCmdCache;
    ULONG                           ulTailIndex;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    pCmdCache   = &pSession->CmdCache;

    if ( pCmdCache->NumOfCmds == 0 )
    {
        return NULL;
    }

    ulTailIndex = ( pCmdCache->CacheHead + pCmdCache->NumOfCmds - 1 ) % pCmdCache->CacheSize;

    pCmdCache->Cursor   = ulTailIndex;

    pCmdCache->bCursorAtTail    = TRUE;
    pCmdCache->bCursorAfterTail = FALSE;

    return pCmdCache->pCmds[ulTailIndex];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ScliShoProcessCmdData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pCmd,
                ULONG                       ulCmdLen,
                PULONG                      pulEscCodeLen
            )

    description:

        This function is called to process data received from
        telnet client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                PUCHAR                      pCmd
                Command buffer containing ESC command code.
                    
                ULONG                       ulCmdLen
                Command buffer size.

                PULONG                      pulEscCodeLen
                The length the ESC command.

    return:     ESC command code.

**********************************************************************/

ULONG
ScliShoGetEscCmdCode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd,
        ULONG                       ulCmdLen,
        PULONG                      pulEscCodeLen
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    BOOL                            bProcSpecial = FALSE;    
    ULONG                           ulCmdCode    = SCLI_SHELL_BICODE_Unrecognized;
    ULONG                           ulInputMode;

    *pulEscCodeLen  = 1;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ulCmdCode;
    }

    ulInputMode = pSession->InputMode;

    /* check HOME, END,  */
    if ( ulCmdLen >= 4 && ulCmdCode == SCLI_SHELL_BICODE_Unrecognized )
    {
        if ( pCmd[1] == TELNET_CHAR_SBRACKET )
        {
            if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_NVT )
            {
                switch ( pCmd[2] )
                {
                    case    '2':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorHome;

                            break;

                    case    '5':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorEnd;

                            break;

                    case    '3':

                            ulCmdCode   = SCLI_SHELL_BICODE_FirstCmd;

                            break;

                    case    '6':

                            ulCmdCode   = SCLI_SHELL_BICODE_LastCmd;

                            break;

                    case    '4':

                            ulCmdCode   = SCLI_SHELL_BICODE_Del;

                            break;
                }
            }
            else if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_Ansi )
            {
                switch ( pCmd[2] )
                {
                    case    '1':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorHome;

                            break;


                    case    '4':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorEnd;

                            break;

                    case    '3':
                    case    '5':

                            ulCmdCode   = SCLI_SHELL_BICODE_FirstCmd;

                            break;

                    case    '6':

                            ulCmdCode   = SCLI_SHELL_BICODE_LastCmd;

                            break;
                }
            }
            else if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_Vt100 )
            {
                switch ( pCmd[2] )
                {
                    case    '2':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorHome;

                            break;


                    case    '4':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorEnd;

                            break;

                    case    '5':

                            ulCmdCode   = SCLI_SHELL_BICODE_FirstCmd;

                            break;

                    case    '6':

                            ulCmdCode   = SCLI_SHELL_BICODE_LastCmd;

                            break;
                }
            }
            else if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_Vt102 )
            {
                switch ( pCmd[2] )
                {
                    case    '3':

                            ulCmdCode   = SCLI_SHELL_BICODE_Del;

                            break;

                    case    '5':

                            ulCmdCode   = SCLI_SHELL_BICODE_FirstCmd;

                            break;

                    case    '6':

                            ulCmdCode   = SCLI_SHELL_BICODE_LastCmd;

                            break;
                }
            }
            else if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_Xterm )
            {
                switch ( pCmd[2] )
                {
                    case    '1':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorHome;

                            break;


                    case    '4':

                            ulCmdCode   = SCLI_SHELL_BICODE_CursorEnd;

                            break;

                    case    '5':

                            ulCmdCode   = SCLI_SHELL_BICODE_FirstCmd;

                            break;

                    case    '6':

                            ulCmdCode   = SCLI_SHELL_BICODE_LastCmd;

                            break;

                    case    '3':

                            ulCmdCode   = SCLI_SHELL_BICODE_Del;

                            break;
                }
            }

            if ( ulCmdCode != SCLI_SHELL_BICODE_Unrecognized && pCmd[3] != TELNET_CHAR_TILDA )
            {
                ulCmdCode   = SCLI_SHELL_BICODE_Unrecognized;
            }
        }

        if ( ulCmdCode != SCLI_SHELL_BICODE_Unrecognized )
        {
            *pulEscCodeLen  = 4;
        }
    }

    if ( ulCmdLen >= 3 && ulCmdCode == SCLI_SHELL_BICODE_Unrecognized )
    {
        if ( pCmd[1] == TELNET_CHAR_SBRACKET )
        {
            switch ( pCmd[2] )
            {
                case    'A':

                        ulCmdCode   = SCLI_SHELL_BICODE_PrevCmd;

                        break;

                case    'B':

                        ulCmdCode   = SCLI_SHELL_BICODE_NextCmd;

                        break;

                case    'C':

                        ulCmdCode   = SCLI_SHELL_BICODE_CursorForward;

                        break;

                case    'D':

                        ulCmdCode   = SCLI_SHELL_BICODE_CursorBackward;

                        break;
            }
        }
        else if ( pExecEnv->TerminalType == TELNET_SERVER_TTY_Vt102 )
        {
            if ( pCmd[1] == 'O' && pCmd[2] == 'H' )
            {
                ulCmdCode = SCLI_SHELL_BICODE_CursorHome;
            }
            else
            if ( pCmd[1] == 'O' && pCmd[2] == 'F' )
            {
                ulCmdCode = SCLI_SHELL_BICODE_CursorEnd;
            }
        }

        if ( ulCmdCode != SCLI_SHELL_BICODE_Unrecognized )
        {
            *pulEscCodeLen  = 3;
        }
    }
    
    if ( ulCmdLen >= 2 && ulCmdCode == SCLI_SHELL_BICODE_Unrecognized && pExecEnv->TerminalType == TELNET_SERVER_TTY_Vt102 )
    {
        if ( pCmd[1] == 'H' )
        {
            ulCmdCode = SCLI_SHELL_BICODE_CursorHome;
        }

        if ( ulCmdCode != SCLI_SHELL_BICODE_Unrecognized )
        {
            *pulEscCodeLen  = 2;
        }
    }

    return ulCmdCode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ScliShoGetInputMode
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession
            )

    description:

        This function is called to get input mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Which session to query.

    return:     input mode.

**********************************************************************/

ULONG
ScliShoGetInputMode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject       = (PSCLI_SHELL_OBJECT)hThisObject;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return SCLI_SHELL_INPUT_MODE_linemode;
    }

    return pSession->InputMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRedrawTextBoxInput
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ULONG                       ulCursorTBoxPos
            )

    description:

        This function is called to redraw textbox input string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Which session to query.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                ULONG                       ulCursorTBoxPos
                The offset to the left most textbox.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRedrawTextBoxInput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       ulCursorTBoxPos
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PBMC2_ICE_TEXTBOX_INFO          pTextBoxInfo = NULL;    
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    ULONG                           ulSpaceNum;
    ULONG                           ulBackCount;
    ULONG                           i;
    PUCHAR                          pLast;
    BOOL                            bMasked     = FALSE;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_FAILURE;
    }

    pLast        = pSession->Command + pSession->CommandLen - 1;
    pTextBoxInfo = (PBMC2_ICE_TEXTBOX_INFO   )pSession->hActiveTextBox;    
    pValue       = pSession->Command + pSession->ulFirstVisiblePos;
    ulValueLen   = pValue ? AnscSizeOfString(pValue) : 0;

    if ( pTextBoxInfo )
    {
        bMasked     = pTextBoxInfo->bMaskInput;

        ulBackCount = pTextBoxInfo->Width - ulCursorTBoxPos;

        if ( ulValueLen > pTextBoxInfo->Width )
        {
            ulValueLen   = pTextBoxInfo->Width;
        }
    }
    else
    {
        ulBackCount = pSession->CommandLen - ulCursorTBoxPos;
    }

    if ( pValue + ulValueLen > pLast )
    {
        ulValueLen  = pLast - pValue + 1;
    }

    if ( pTextBoxInfo )
    {
        ulSpaceNum  = pTextBoxInfo->Width - ulValueLen;
    }
    else
    {
        ulSpaceNum  = 0;
    }

    if ( pValue && ulValueLen != 0 )
    {
        if ( !bMasked )
        {
            returnStatus =
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        (ANSC_HANDLE)pSession->SessionId,
                        pValue,
                        ulValueLen
                    );
        }
        else
        {
            returnStatus =
                pTscIf->SendChar
                    (
                        pTscIf->hOwnerContext,
                        (ANSC_HANDLE)pSession->SessionId,
                        '*',
                        ulValueLen
                    );
        }
    }

    if ( ulSpaceNum != 0 )
    {
        returnStatus = 
            pTscIf->SendChar
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    ' ',
                    ulSpaceNum
                );
    }

    for (i = 0; i < ulBackCount; i ++)
    {
        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    TELNET_ANSI_TTY_CODE_Cursor_Backward,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward)
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoMoveToTextBox
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv
            );

    description:

        This function is called to move to the origin of the text
        box.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                Which session to query.

                ANSC_HANDLE                 hExecEnv
                Execution env.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoMoveToTextBox
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PBMC2_ICE_TEXTBOX_INFO          pTextBoxInfo = NULL;    
    ULONG                           ulBackCount  = 0;
    ULONG                           i;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);
    
    if ( !pSession )
    {
        return ANSC_STATUS_FAILURE;
    }

    pTextBoxInfo = (PBMC2_ICE_TEXTBOX_INFO   )pSession->hActiveTextBox;    

    if ( pTextBoxInfo )
    {
        if ( pSession->CursorPos > pSession->ulFirstVisiblePos )
        {
            ulBackCount = pSession->CursorPos - pSession->ulFirstVisiblePos;
        }
    }
    else
    {
        ulBackCount = pSession->CursorPos;
    }

    for (i = 0; i < ulBackCount; i ++)
    {
        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    TELNET_ANSI_TTY_CODE_Cursor_Backward,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward)
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoDoAuth
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pUserName,
                PUCHAR                      pPassword,
                PULONG                      pulPerm,
                PBOOL                       pbPrivileged,
                PULONG                      pulIdleTimeout,
                PULONG                      pulLockoutTimeout
            )

    description:

        This function is called to authenticate the given user.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pUserName
                User name.

                PUCHAR                      pPassword
                Password.

                PULONG                      pulPerm
                User permission on return if user credential has
                been verified successfully.

                PBOOL                       pbPrivileged
                If the user has privilege.

                PULONG                      pulIdleTimeout
                Inactivity timeout.

                PULONG                      pulLockoutTimeout
                Lockout timeout.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoDoAuth
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserName,
        PUCHAR                      pPassword,
        PULONG                      pulPerm,
        PBOOL                       pbPrivileged,
        PULONG                      pulTermPermission,
        PULONG                      pulIdleTimeout,
        PULONG                      pulLockoutTimeout
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT        )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY      )&pMyObject->Property;
    PSCLI_SHELL_AUTH_INTERFACE      pAuthIf      = (PSCLI_SHELL_AUTH_INTERFACE)pMyObject->hAuthIf;

    if ( !pAuthIf )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        returnStatus =
            pAuthIf->DoAuth
                (
                    pAuthIf->hOwnerContext,
                    pProperty->bUserAuth,
                    pUserName,
                    pPassword,
                    pulPerm,
                    pbPrivileged,
                    pulTermPermission,
                    pulIdleTimeout,
                    pulLockoutTimeout
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ScliShoGetNumActiveSessions
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the number of active
        sessions.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the number of active sessions at this moment.

**********************************************************************/

ULONG
ScliShoGetNumActiveSessions
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PSCLI_SHELL_SESSION_EXEC        pSession     = NULL;
    ULONG                           ulNumActive  = 0;
    BOOL                            bActive;

    AnscAcquireLock(&pMyObject->SessionListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SessionList);
    while ( pSLinkEntry )
    {
        pSession    = ACCESS_SCLI_SHELL_SESSION_EXEC_OBJECT(pSLinkEntry);

        bActive     = 
            ( pSession->SessionState != SCLI_SHELL_STATE_INITIATED && 
              pProperty->ShellType != SCLI_SHELL_TYPE_Local );

        if ( !bActive )
        {
            bActive = 
                ( pProperty->ShellType == SCLI_SHELL_TYPE_Local &&
                  pSession->SessionState == SCLI_SHELL_STATE_LOGGED_IN );
        }

        if ( bActive )
        {
            ulNumActive ++;
        }

        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
    }

    AnscReleaseLock(&pMyObject->SessionListLock);

    return ulNumActive;
}


#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunShellCmd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pCmd
            )

    description:

        This function is called to run system shell command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                PUCHAR                      pCmd
                Command to be run.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunShellCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PSCLI_SHELL_CEN_INTERFACE       pCenIf       = (PSCLI_SHELL_CEN_INTERFACE)pMyObject->hCenIf;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    char                            sys_cmd[256];
    char                            tmp_fname[64];
    int                             nRet;
    ULONG                           ulStartTime  = AnscGetTickInSeconds();
    ULONG                           ulTimeNow;
    ANSC_HANDLE                     hFile        = NULL;
    ULONG                           ulFileMode, ulFileType;

    ulFileMode  = ANSC_FILE_MODE_READ;
    ulFileType  = ANSC_FILE_TYPE_READ;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    pCenIf->Notify
        (
            pCenIf->hOwnerContext,
            pSession->hCeeExecEnv,
            SCLI_SHELL_CEN_EVENT_CmdBegin
        );

    if ( !pCmd || AnscSizeOfString(pCmd) == 0 )
    {
        pCenIf->Notify
            (
                pCenIf->hOwnerContext,
                pSession->hCeeExecEnv,
                SCLI_SHELL_CEN_EVENT_CmdEnd
            );

        return ANSC_STATUS_SUCCESS;
    }

    _ansc_sprintf(tmp_fname, SCLI_LOCAL_TEMP_FILE_TEMPLATE, pSession);
    _ansc_sprintf(sys_cmd, "%s > %s", pCmd, tmp_fname);

    AnscDeleteFile(tmp_fname);

    nRet = _ansc_system(sys_cmd);

    ulTimeNow = AnscGetTickInSeconds();

    while ( ulTimeNow - ulStartTime < 30 )
    {
        hFile = AnscOpenFile(tmp_fname, ulFileMode, ulFileType);

        if ( hFile )
        {
            break;
        }

        AnscSleep(1);
    }

    if ( (ANSC_HANDLE)NULL == hFile )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        UCHAR                       buf[513];
        ULONG                       size;
        ANSC_STATUS                 status;

        while ( TRUE )
        {
            size = 512;
            status = AnscReadFile(hFile, buf, &size);

            if ( status != ANSC_STATUS_SUCCESS || size == 0 )
            {
                break;
            }
            else
            {
                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            (ANSC_HANDLE)pSession->SessionId,
                            buf,
                            size
                        );
            }

            returnStatus =
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        (ANSC_HANDLE)pSession->SessionId,
                        buf,
                        size
                    );
        }

        /*
        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );
        */

        AnscCloseFile(hFile);
        AnscDeleteFile(tmp_fname);

        pCenIf->Notify
        (
            pCenIf->hOwnerContext,
            pSession->hCeeExecEnv,
            SCLI_SHELL_CEN_EVENT_CmdEnd
        );
    }

    return returnStatus;
}

#endif
