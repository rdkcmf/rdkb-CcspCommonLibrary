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

    module:	scli_shell_bic.c

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

        *   ScliShoRunBicExit
        *   ScliShoRunBicClear
        *   ScliShoRunBicShell
        *   ScliShoRunBicHistory
        *   ScliShoRunBicCursorBackward
        *   ScliShoRunBicCursorForward
        *   ScliShoRunBicCursorHome
        *   ScliShoRunBicCursorEnd
        *   ScliShoRunBicDel
        *   ScliShoRunBicBackspace
        *   ScliShoRunBicBreak
        *   ScliShoRunBicAutoCompletion
        *   ScliShoRunBicHelp
             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/20/05    initial revision.

**********************************************************************/


#include "scli_shell_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicExit
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicExit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED

    if ( pSession->bOsShellActive )
    {
        if ( pProperty->ShellType != SCLI_SHELL_TYPE_Local )
        {
            pSession->bOsShellActive = FALSE;

            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_ANSI_TTY_CODE_Crlf,
                        AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                    );

            /* reconstruct domain title */
            pTscIf->SetShellPrompt
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pMyObject->ShellPrompt
                );

            pTscIf->SendShellPrompt
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TRUE
                );
        }
    }
    else

#endif

    {
        pSession->SessionState  = SCLI_SHELL_STATE_INITIATED;

        if ( pSession->hBmc2Terminal )
        {
            pBmc2SccIf->DelTerminal
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal
                );

            pSession->hBmc2Terminal = NULL;
        }

        /* terminate this session */
        returnStatus = 
            pTscIf->Notify
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_TSC_EVENT_SessionEnd,
                    (ANSC_HANDLE)NULL
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicClear
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicClear
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    returnStatus =
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                TELNET_ANSI_TTY_CODE_ClearScreen,
                AnscSizeOfString(TELNET_ANSI_TTY_CODE_ClearScreen)
            );

    return returnStatus;
}


#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicShell
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
            )

    description:

        This function is called to start a system shell.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession,
                Session Id.

                ANSC_HANDLE                 hExecEnv
                Execution env.

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicShell
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    returnStatus =
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                SCLI_SYSTEM_SHELL_EXIT_PROMPT,
                AnscSizeOfString(SCLI_SYSTEM_SHELL_EXIT_PROMPT)
            );

    returnStatus = 
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                TELNET_ANSI_TTY_CODE_Crlf,
                AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
            );

    if ( pProperty->ShellType == SCLI_SHELL_TYPE_Local )
    {
        _ansc_system(SCLI_LOCAL_SHELL_CMD);
    }
    else
    {
        pSession->bOsShellActive = TRUE;

        pTscIf->SetShellPrompt
            (
                pTscIf->hOwnerContext,
                hSrvSession,
                SCLI_SYSTEM_SHELL_TITLE
            );
    }

    return returnStatus;
}

#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicHistory
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ULONG                       CmdCode
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
                History command code.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicHistory
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
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
    {
        return ANSC_STATUS_SUCCESS;
    }

    if ( !pSession->bServing )
    {
        PUCHAR          pCmd    = NULL;

        if ( TRUE )
        {
            ULONG       ulPos = pSession->CommandLen;

            if ( pSession->CommandLen >= SCLI_SHELL_MAX_COMMAND_LEN - 1 )
            {
                ulPos = SCLI_SHELL_MAX_COMMAND_LEN - 1;
            }
            pSession->Command[ulPos]     = 0;

            if ( pMyObject->IsCmdModified((ANSC_HANDLE)pMyObject, hSrvSession, pSession->Command) )
            {
                returnStatus    = pMyObject->CacheCmd((ANSC_HANDLE)pMyObject, hSrvSession, pSession->Command);

                if ( CmdCode == SCLI_SHELL_BICODE_PrevCmd )
                {
                    pMyObject->GetLastCmd((ANSC_HANDLE)pMyObject, hSrvSession);
                }
            }
        }

        switch ( CmdCode )
        {
            case    SCLI_SHELL_BICODE_PrevCmd:
                    
                    pCmd    = pMyObject->GetPrevCmd((ANSC_HANDLE)pMyObject, hSrvSession);

                    break;

            case    SCLI_SHELL_BICODE_NextCmd:

                    pCmd    = pMyObject->GetNextCmd((ANSC_HANDLE)pMyObject, hSrvSession);

                    break;

            case    SCLI_SHELL_BICODE_FirstCmd:

                    pCmd    = pMyObject->GetFirstCmd((ANSC_HANDLE)pMyObject, hSrvSession);

                    break;

            case    SCLI_SHELL_BICODE_LastCmd:

                    pCmd    = pMyObject->GetLastCmd((ANSC_HANDLE)pMyObject, hSrvSession);

                    break;
        }

        if ( !pCmd && CmdCode != SCLI_SHELL_BICODE_NextCmd )
        {
            returnStatus =
                pTscIf->RingBell
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession
                    );
        }
        else
        {
            ULONG       ulCmdLen        = pCmd ? AnscSizeOfString(pCmd) : 0;
            PUCHAR      pReply;
            ULONG       ulReplySize     = 0, i, ulCount = 0;
            ULONG       ulCurBackSize   = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward);
            ULONG       ulEraseToEnd    = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Erase_ToEnd);
            ULONG       ulCurBackCount  = pSession->CursorPos;

            if ( ulCurBackCount != 0 )
            {
                ulReplySize = ulCurBackSize * ulCurBackCount + ulEraseToEnd;
            }
            ulReplySize += ulCmdLen;

            if ( ulReplySize != 0 )
            {
                pReply  = (PUCHAR)AnscAllocateMemory( ulReplySize );

                if ( pReply )
                {
                    for ( i = 0; i < ulCurBackCount; i ++ )
                    {
                        AnscCopyMemory
                            (
                                pReply + ulCount, 
                                TELNET_ANSI_TTY_CODE_Cursor_Backward,
                                ulCurBackSize
                            );

                        ulCount += ulCurBackSize;
                    }

                    if ( ulCurBackCount != 0 )
                    {
                        AnscCopyMemory
                            (
                                pReply + ulCount, 
                                TELNET_ANSI_TTY_CODE_Erase_ToEnd,
                                ulEraseToEnd
                            );

                        ulCount += ulEraseToEnd;
                    }

                    if ( ulCmdLen != 0 )
                    {
                        AnscCopyMemory
                            (
                                pReply + ulCount,
                                pCmd,
                                ulCmdLen
                            );
                    }

                    returnStatus = 
                        pTscIf->Output
                            (
                                pTscIf->hOwnerContext,
                                hSrvSession,
                                pReply,
                                ulReplySize
                            );

                    AnscFreeMemory(pReply);
                }
            }

            /* 
             * copy this command to current command buffer - we don't 
             * modify commands in cache and we always add a new one at
             * the end of cache if it's committed.
             */
            AnscCopyMemory(pSession->Command, pCmd, ulCmdLen);

            pSession->CommandLen    = ulCmdLen;
            pSession->CursorPos     = ulCmdLen;
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicCursorBackward
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicCursorBackward
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CursorPos == 0 )
    {
        returnStatus = 
            pTscIf->RingBell
                (
                    pTscIf->hOwnerContext, 
                    hSrvSession
                );
    }
    else
    {
        BOOL                        bNeedShift      = FALSE;

        bNeedShift   = 
            ( 
                SCLI_SHELL_INPUT_IN_TEXTBOX && 
                pActiveTBox && 
                pSession->ulFirstVisiblePos >= pSession->CursorPos 
            );

        if ( bNeedShift )
        {
            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            pSession->ulFirstVisiblePos --;

            returnStatus =
                pMyObject->RedrawTextBoxInput
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv,
                        0
                    );
        }
        else
        {
            returnStatus =
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_ANSI_TTY_CODE_Cursor_Backward,
                        AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward)
                    );
        }

        pSession->CursorPos --;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicCursorForward
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicCursorForward
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CursorPos >= pSession->CommandLen )
    {
        returnStatus = 
            pTscIf->RingBell
                (
                    pTscIf->hOwnerContext, 
                    hSrvSession
                );
    }
    else
    {
        BOOL                        bTBoxOverflow       = FALSE;
        BOOL                        bMoveCursorOutBox   = TRUE;

        bTBoxOverflow   = 
            ( 
                SCLI_SHELL_INPUT_IN_TEXTBOX && 
                pActiveTBox && 
                pSession->CursorPos >= pSession->ulFirstVisiblePos + pActiveTBox->Width - 1
            );

        if ( bTBoxOverflow )
        {
            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            if ( !pSession->bCursorFwdOutbox )
            {
                bMoveCursorOutBox = FALSE;
            }

            if ( pActiveTBox->bSizeFixed )
            {
                if ( !pSession->bCursorFwdOutbox )
                {
                    pSession->CursorPos --;
                }
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
        
        if ( bMoveCursorOutBox )
        {
            returnStatus =
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_ANSI_TTY_CODE_Cursor_Forward,
                        AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Forward)
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
        ScliShoRunBicCursorHome
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicCursorHome
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CursorPos == 0 )
    {
        returnStatus = 
            pTscIf->RingBell
                (
                    pTscIf->hOwnerContext, 
                    hSrvSession
                );
    }
    else
    {
        ULONG                       ulBackwardCount = pSession->CursorPos;
        PUCHAR                      pBuf;
        ULONG                       i, ulBufSize    = 0;
        ULONG                       ulPos           = 0;
        ULONG                       ulCurBwdSize    = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward);
        BOOL                        bNeedShift      = FALSE;

        bNeedShift   = 
            ( 
                SCLI_SHELL_INPUT_IN_TEXTBOX && 
                pActiveTBox && 
                pSession->ulFirstVisiblePos > 0 
            );

        if ( bNeedShift )
        {
            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            pSession->ulFirstVisiblePos = 0;

            returnStatus =
                pMyObject->RedrawTextBoxInput
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv,
                        0
                    );
        }
        else
        {
            ulBufSize   = ulBackwardCount * ulCurBwdSize;

            pBuf = (PUCHAR)AnscAllocateMemory(ulBufSize);

            if ( pBuf )
            {
                for (i = 0; i < ulBackwardCount; i ++)
                {
                    AnscCopyMemory
                        (
                            pBuf + ulPos,
                            TELNET_ANSI_TTY_CODE_Cursor_Backward,
                            ulCurBwdSize
                        );

                    ulPos += ulCurBwdSize;
                }

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pBuf,
                            ulBufSize
                        );

                AnscFreeMemory(pBuf);
            }
        }

        pSession->CursorPos = 0;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicCursorEnd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicCursorEnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CursorPos >= pSession->CommandLen )
    {
        returnStatus = 
            pTscIf->RingBell
                (
                    pTscIf->hOwnerContext, 
                    hSrvSession
                );
    }
    else
    {
        ULONG                       ulForwardCount  = pSession->CommandLen - pSession->CursorPos;
        PUCHAR                      pBuf;
        ULONG                       i, ulBufSize    = 0;
        ULONG                       ulPos           = 0;
        ULONG                       ulCurFwdSize    = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Forward);
        BOOL                        bTBoxOverflow   = FALSE;

        bTBoxOverflow   = 
            ( 
                SCLI_SHELL_INPUT_IN_TEXTBOX && 
                pActiveTBox && 
                pSession->CommandLen >= pSession->ulFirstVisiblePos + pActiveTBox->Width - 1
            );

        if ( bTBoxOverflow )
        {
            returnStatus =
                pMyObject->MoveToTextBox
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSrvSession,
                        hExecEnv
                    );

            pSession->ulFirstVisiblePos = pSession->CommandLen - pActiveTBox->Width + 1;

            if ( pActiveTBox && pActiveTBox->bSizeFixed )
            {
                pSession->ulFirstVisiblePos --;
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
            ulBufSize   = ulForwardCount * ulCurFwdSize;

            pBuf = (PUCHAR)AnscAllocateMemory(ulBufSize);

            if ( pBuf )
            {
                for (i = 0; i < ulForwardCount; i ++)
                {
                    AnscCopyMemory
                        (
                            pBuf + ulPos,
                            TELNET_ANSI_TTY_CODE_Cursor_Forward,
                            ulCurFwdSize
                        );

                    ulPos += ulCurFwdSize;
                }

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pBuf,
                            ulBufSize
                        );

                AnscFreeMemory(pBuf);
            }
        }

        pSession->CursorPos = pSession->CommandLen;

        if ( pActiveTBox && pActiveTBox->bSizeFixed )
        {
            if ( pSession->CursorPos >= pActiveTBox->Width )
            {
                pSession->CursorPos --;
            }
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicDel
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicDel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    if ( pSession->CommandLen == 0 || pSession->CursorPos >= pSession->CommandLen )
    {
        if ( pExecEnv->bEcho )
        {
            returnStatus = 
                pTscIf->RingBell
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession
                    );
        }
        else
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        pSession->bCursorFwdOutbox = TRUE;

        returnStatus =
            pMyObject->RunBicCursorForward
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv,
                    (ANSC_HANDLE)NULL
                );

        pSession->bCursorFwdOutbox = FALSE;

        returnStatus = 
            pMyObject->RunBicBackspace
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv,
                    (ANSC_HANDLE)NULL
                );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicBackspace
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicBackspace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( pSession->CommandLen == 0 || pSession->CursorPos == 0 )
    {
        if ( pExecEnv->bEcho )
        {
            returnStatus = 
                pTscIf->RingBell
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession
                    );
        }
        else
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
    else if ( pSession->CursorPos >= pSession->CommandLen && !pSession->hActiveTextBox )
    {
        UCHAR                       buf[16];
        ULONG                       ulBufSize   = 0;

        /* cursor is at the end of current command */
        pSession->CommandLen --;
        pSession->CursorPos --;

        buf[ulBufSize ++] = TELNET_CHAR_BACKSPACE;

        if ( pExecEnv->TerminalType != TELNET_SERVER_TTY_NVT )
        {
            AnscCopyMemory
                (
                    buf + ulBufSize,
                    TELNET_ANSI_TTY_CODE_Erase_ToEnd,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Erase_ToEnd)
                );

            ulBufSize   += AnscSizeOfString(TELNET_ANSI_TTY_CODE_Erase_ToEnd);
        }

        if ( pExecEnv->bEcho )
        {
            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        buf,
                        ulBufSize
                    );
        }
    }
    else if ( !SCLI_SHELL_INPUT_IN_TEXTBOX )
    {
        if ( TRUE )
        {
            ULONG                   ulRightCharCount    = pSession->CommandLen - pSession->CursorPos;
            PUCHAR                  pBackBuf;
            ULONG                   ulBackBufSize       = 0;
            ULONG                   ulBackBufPos        = 0;
            ULONG                   ulCurBackCodeSize   = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Cursor_Backward);
            ULONG                   ulErase2EndSize     = AnscSizeOfString(TELNET_ANSI_TTY_CODE_Erase_ToEnd);
            ULONG                   i;

            if ( pExecEnv->bEcho )
            {
                ulBackBufSize   = ulCurBackCodeSize + ulErase2EndSize + ulRightCharCount;
                ulBackBufSize  += ulRightCharCount * ulCurBackCodeSize;

                pBackBuf    = (PUCHAR)AnscAllocateMemory(ulBackBufSize);

                if ( !pBackBuf )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    AnscCopyMemory
                        (
                            pBackBuf + ulBackBufPos,
                            TELNET_ANSI_TTY_CODE_Cursor_Backward,
                            ulCurBackCodeSize
                        );

                    ulBackBufPos += ulCurBackCodeSize;

                    AnscCopyMemory
                        (
                            pBackBuf + ulBackBufPos,
                            TELNET_ANSI_TTY_CODE_Erase_ToEnd,
                            ulErase2EndSize
                        );

                    ulBackBufPos += ulErase2EndSize;

                    AnscCopyMemory
                        (
                            pBackBuf + ulBackBufPos,
                            pSession->Command + pSession->CursorPos,
                            ulRightCharCount
                        );

                    ulBackBufPos += ulRightCharCount;

                    for (i = 0; i < ulRightCharCount; i ++)
                    {
                        AnscCopyMemory
                            (
                                pBackBuf + ulBackBufPos,
                                TELNET_ANSI_TTY_CODE_Cursor_Backward,
                                ulCurBackCodeSize
                            );

                        ulBackBufPos += ulCurBackCodeSize;
                    }

                    returnStatus =
                        pTscIf->Output
                            (
                                pTscIf->hOwnerContext,
                                hSrvSession,
                                pBackBuf,
                                ulBackBufSize
                            );

                    AnscFreeMemory(pBackBuf);
                }
            }

            for (i = pSession->CursorPos; i < pSession->CommandLen; i ++)
            {
                pSession->Command[i-1]  = pSession->Command[i];
            }

            pSession->CommandLen --;
            pSession->CursorPos --;
        }
    }
    else /* textbox mode */
    {
        ULONG                       i;

        for (i = pSession->CursorPos; i < pSession->CommandLen; i ++)
        {
            pSession->Command[i-1]  = pSession->Command[i];
        }

        pSession->CommandLen --;

        if ( pSession->ulFirstVisiblePos >= pSession->CursorPos )
        {
            /* doesn't affect display */
            pSession->ulFirstVisiblePos --;
        }
        else
        {
            if ( pSession->ulFirstVisiblePos != 0 )
            {
                ULONG               ulCursorTBoxPos = pSession->CursorPos - pSession->ulFirstVisiblePos;

                returnStatus =
                    pMyObject->MoveToTextBox
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv
                        );

                pSession->ulFirstVisiblePos --;

                returnStatus =
                    pMyObject->RedrawTextBoxInput
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            ulCursorTBoxPos
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

                returnStatus =
                    pMyObject->RedrawTextBoxInput
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSrvSession,
                            hExecEnv,
                            pSession->CursorPos - 1
                        );
            }
        }

        pSession->CursorPos --;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicBreak
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicBreak
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    pTscIf->SendDataMark
        (
            pTscIf->hOwnerContext,
            hSrvSession
        );

    pTscIf->SendShellPrompt
        (
            pTscIf->hOwnerContext,
            hSrvSession,
            TRUE
        );

    pSession->CommandLen        = 0;
    pSession->ulFirstVisiblePos = 0;

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicAutoCompletion
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicAutoCompletion
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PUCHAR                          pCmds        = NULL;
    BOOL                            bMultiCmds   = FALSE;
    PUCHAR                          pCmd         = NULL;
    PSLAP_STRING_ARRAY              pSlapStrArray= NULL;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;
    PSCLI_SHELL_CMD_TLIST           pCmdTokenList= NULL;
    PSCLI_CMD_ARG_MATCH_RESULT      pReqArgM     = NULL;
    ULONG                           ulReqArgMLen = 0;
    PSCLI_CMD_ARG_MATCH_RESULT      pOptArgM     = NULL;
    ULONG                           ulOptArgMLen = 0;
    BOOL                            bMatched     = FALSE;
    int                             nErrorPos    = 0;
    BOOL                            bShowOptOnly = (BOOL)hReserved;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
    {
        return ANSC_STATUS_SUCCESS;
    }

    pCmd                        = pSession->Command;
    pCmd[pSession->CommandLen]  = 0;

    if ( pSession->CursorPos < pSession->CommandLen )
    {
        returnStatus =
            pMyObject->RunBicCursorEnd
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( *pCmd != 0 )
    {
        SCLI_SHELL_SKIP_SPACE(pCmd);

        pCmdTokenList = ScliShoTokenizeCommand(pCmd, SCLI_SHELL_CMD_TOKEN_SEPARATOR);
        if ( !pCmdTokenList )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    if ( *pCmd == 0 || !pCmdTokenList || (pCmdTokenList->ulTokenCount == 0) || (pCmdTokenList->ulTokenCount == 1 && !pCmdTokenList->bEndWithSpace) )
    {
        /* trying to auto complete command */
        returnStatus =
            pBmc2SccIf->GetCommands
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal,
                    pCmd,
                    &pSlapStrArray
                );
    }
    else
    {
        PBMC2_CMD_HELP_SYNTAX       pCmdHelp;
        char*                       pCommand;
        ULONG                       i;

        pCommand    = pCmdTokenList->pTokens[0].pValue;

        pCmdHelp = 
            (PBMC2_CMD_HELP_SYNTAX)pBmc2SccIf->GetCommandSyntax
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal,
                    pCommand
                );

        if ( !pCmdHelp )
        {
            goto EXIT;
        }

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

        /* trying to auto complete argument name or value */
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
            returnStatus = ANSC_STATUS_UNMATCHED;
        }
        else
        {
            returnStatus =
                ScliShoAutoCompleteCommandArg
                    (
                        hThisObject,
                        hSrvSession,
                        pCmdTokenList->bEndWithSpace ? NULL : pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].pValue,
                        pCmdHelp,
                        pCmdTokenList,
                        pReqArgM,
                        pOptArgM,
                        &pSlapStrArray
                    );
        }
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto EXIT;
    }

    if ( !pSlapStrArray )
    {
        returnStatus =
            pTscIf->RingBell
                (
                    pTscIf->hOwnerContext,
                    hSrvSession
                );
    }
    else
    {
        bMultiCmds  = ( bShowOptOnly || (pSlapStrArray->VarCount > 1) );

        if ( pSlapStrArray->VarCount == 0 )
        {
            /* nothing to display, either command or argument can not be recognized */
        }
        else if ( !bMultiCmds )
        {
            PUCHAR                  pCommand        = pSlapStrArray->Array.arrayString[0];
            ULONG                   ulCommandLen    = AnscSizeOfString(pCommand);
            ULONG                   ulDiff          = ulCommandLen;
            ULONG                   ulOrgTokenLen   = 0;
            PUCHAR                  pPrefix         = pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount - 1].pValue;
            
            if ( !pCmdTokenList->bEndWithSpace && pPrefix )
            {
                if ( _ansc_strchr(pCommand, ',') == NULL && _ansc_strchr(pPrefix, ',') )
                {
                    int             nPrefixLen = AnscSizeOfString(pPrefix);
                    int             i;

                    for ( i = nPrefixLen - 1; i >= 0; i -- )
                    {
                        if ( pPrefix[i] == ',' )
                        {
                            break;
                        }
                    }

                    if ( i >= nPrefixLen - 1 )
                    {
                        pPrefix = NULL;
                    }
                    else if ( i >= 0 )
                    {
                        pPrefix += i + 1;
                    }
                }

                ulOrgTokenLen = pPrefix ? AnscSizeOfString(pPrefix) : 0;
                ulDiff       -= ulOrgTokenLen;
            }

            if ( ulDiff != 0 )
            {
                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pCommand + ulOrgTokenLen,
                            ulDiff
                        );

                AnscCopyString
                    (
                        pSession->Command + AnscSizeOfString(pSession->Command), 
                        pCommand + ulOrgTokenLen
                    );

                pSession->CommandLen    = AnscSizeOfString(pSession->Command);
                pSession->CursorPos     = pSession->CommandLen;
            }
        }
        else
        {
            ULONG                   ulMaxLen        = 0;
            ULONG                   i;
            ULONG                   ulWidth         = pExecEnv->WindowWidth;
            char*                   pPrefix         = NULL;
            ULONG                   ulPrefixLen     = 0;
            int                     nMaxOutStart    = (int)ulPrefixLen;
            int                     nMaxOutEnd      = nMaxOutStart - 1;
            BOOL                    bMaxingOut      = TRUE;
            ULONG                   ulMinStringLen  = (ULONG)-1;

            if ( pCmdTokenList && pCmdTokenList->ulTokenCount >= 1 && !pCmdTokenList->bEndWithSpace )
            {
                pPrefix = pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].pValue;
         
                if ( _ansc_strchr(pPrefix, ',') )
                {
                    int             nPrefixLen = AnscSizeOfString(pPrefix);
                    int             i;

                    for ( i = nPrefixLen - 1; i >= 0; i -- )
                    {
                        if ( pPrefix[i] == ',' )
                        {
                            break;
                        }
                    }

                    if ( i >= nPrefixLen - 1 )
                    {
                        pPrefix = NULL;
                    }
                    else if ( i >= 0 )
                    {
                        pPrefix += i + 1;
                    }
                }
            }

            ulPrefixLen     = pPrefix ? AnscSizeOfString(pPrefix) : 0;
            nMaxOutStart    = (int)ulPrefixLen;
            nMaxOutEnd      = nMaxOutStart - 1;

            for (i = 0; i < pSlapStrArray->VarCount; i ++)
            {
                ULONG               ulStrLen;
                
                if ( !pSlapStrArray->Array.arrayString[i] )
                {
                    break;
                }
                ulStrLen = AnscSizeOfString(pSlapStrArray->Array.arrayString[i]);

                if ( ulStrLen > ulMaxLen )
                {
                    ulMaxLen    = ulStrLen;
                }
            }

            if ( ulMaxLen > ulWidth )
            {
                /* console width is too small to even hold the longest command */
                pTscIf->RingBell
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession
                    );
            }
            else
            {
                /* display commands */
                ULONG               ulCmdsPerLine   = ulWidth % ulMaxLen;
                PUCHAR              pBuf            = NULL;
                ULONG               ulCount         = 0;

                if ( ulCmdsPerLine > 1 )
                {
                    ulMaxLen        = ulMaxLen / 4 * 4 + 4;
                    ulCmdsPerLine   = ulWidth % ulMaxLen;
                }

                pBuf    = (PUCHAR)AnscAllocateMemory(ulMaxLen);

                returnStatus = 
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            TELNET_ANSI_TTY_CODE_Crlf,
                            AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                        );

                for (i = 0; i < pSlapStrArray->VarCount; i ++)
                {
                    PUCHAR              pStr        = pSlapStrArray->Array.arrayString[i];
                    ULONG               ulStrLen    = pStr ? AnscSizeOfString(pStr) : 0;

                    if ( ulStrLen < ulMinStringLen )
                    {
                        ulMinStringLen = ulStrLen;
                    }

                    if ( pStr )
                    {
                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    SCLI_SHELL_OUTPUT_INDENT,
                                    AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                                );

                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    pStr,
                                    ulStrLen
                                );

                        returnStatus = 
                            pTscIf->Output
                                (
                                    pTscIf->hOwnerContext,
                                    hSrvSession,
                                    TELNET_ANSI_TTY_CODE_Crlf,
                                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                                );
                    }
                }

                /* find maximum common part and automatically put it into command buffer */
                for ( i = (ULONG)nMaxOutStart; i < ulMinStringLen; i ++ )
                {
                    PUCHAR              pStr = pSlapStrArray->Array.arrayString[i];
                    ULONG               j;
                    char                ch   = *(pSlapStrArray->Array.arrayString[0] + i);
                    
                    for ( j = 1; bMaxingOut && j < pSlapStrArray->VarCount; j ++ )
                    {
                        if ( *(pSlapStrArray->Array.arrayString[j] + i) != ch )
                        {
                            bMaxingOut = FALSE;
                        }
                    }

                    if ( bMaxingOut )
                    {
                        nMaxOutEnd = (int)i;
                    }
                    else
                    {
                        break;
                    }
                }

                if ( nMaxOutEnd >= nMaxOutStart )
                {
                    AnscCopyMemory
                        (
                            pCmd + pSession->CommandLen, 
                            pSlapStrArray->Array.arrayString[0] + nMaxOutStart, 
                            nMaxOutEnd - nMaxOutStart + 1
                        );
                    pSession->CommandLen += nMaxOutEnd - nMaxOutStart + 1;
                    pSession->CursorPos  += nMaxOutEnd - nMaxOutStart + 1;
                    pCmd[pSession->CommandLen]  = 0;
                }

                if ( pBuf )
                {
                    AnscFreeMemory(pBuf);
                }

                returnStatus = 
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            TELNET_ANSI_TTY_CODE_Crlf,
                            AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                        );

                returnStatus = 
                    pTscIf->SendShellPrompt
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            TRUE
                        );

                returnStatus = 
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            pCmd,
                            AnscSizeOfString(pCmd)
                        );
            }
        }

        SlapFreeVarArray(pSlapStrArray);
    }

EXIT:

    if ( pCmdTokenList )
    {
        ScliShoFreeCmdTokenList(pCmdTokenList);
    }

    if ( pReqArgM )
    {
        AnscFreeMemory(pReqArgM);
    }

    if ( pOptArgM )
    {
        AnscFreeMemory(pOptArgM);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRunBicHelp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Reserved argument, it could be used to specify
                command specific info used to execute the command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRunBicHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_NOT_SPECIFIED;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv     = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf       = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE      )pMyObject->hBmc2SccIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;
    PUCHAR                          pCmds        = NULL;
    PUCHAR                          pCmd         = NULL;
    ULONG                           ulInputMode;
    PBMC2_ICE_TEXTBOX_INFO          pActiveTBox  = NULL;
    PBMC2_CMD_HELP_SYNTAX           pCmdHelp     = NULL;
    char*                           pCommand     = NULL;
    ULONG                           ulLen        = 0;
    PSCLI_SHELL_CMD_TLIST           pCmdTokenList= NULL;
    PSCLI_CMD_ARG_MATCH_RESULT      pReqArgM     = NULL;
    ULONG                           ulReqArgMLen = 0;
    PSCLI_CMD_ARG_MATCH_RESULT      pOptArgM     = NULL;
    ULONG                           ulOptArgMLen = 0;
    BOOL                            bMatched     = FALSE;
    int                             nErrorPos    = 0;
    ULONG                           i;
    BOOL                            bNoPrompt    = FALSE;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    ulInputMode = pSession->InputMode;
    pActiveTBox = (PBMC2_ICE_TEXTBOX_INFO)pSession->hActiveTextBox;

    if ( SCLI_SHELL_INPUT_IN_TEXTBOX )
    {
        return ANSC_STATUS_SUCCESS;
    }

    pCmd                        = pSession->Command;
    pCmd[pSession->CommandLen]  = 0;

    if ( pSession->CursorPos < pSession->CommandLen )
    {
        returnStatus =
            pMyObject->RunBicCursorEnd
                (
                    (ANSC_HANDLE)pMyObject,
                    hSrvSession,
                    hExecEnv,
                    (ANSC_HANDLE)NULL
                );
    }

    /* output question mark */
    if ( TRUE )
    {
        char                        QMark[2] = {SCLI_SHELL_HELP_CHAR, 0};

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    QMark,
                    AnscSizeOfString(QMark)
                );
    }

    if ( *pCmd != 0 )
    {
        SCLI_SHELL_SKIP_SPACE(pCmd);

        pCmdTokenList = ScliShoTokenizeCommand(pCmd, SCLI_SHELL_CMD_TOKEN_SEPARATOR);

        if ( !pCmdTokenList )
        {
            return  returnStatus;
        }
    }

    if ( *pCmd == 0 || !pCmdTokenList || (pCmdTokenList->ulTokenCount == 0) || (pCmdTokenList->ulTokenCount == 1 && !pCmdTokenList->bEndWithSpace) )
    {
        /* list all commands */
        returnStatus = 
            pMyObject->RunBicAutoCompletion
                (
                    hThisObject,
                    hSrvSession,
                    hExecEnv,
                    (ANSC_HANDLE)TRUE
                );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            bNoPrompt = TRUE;
        }
        else
        {
            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_ANSI_TTY_CODE_Crlf,
                        AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                    );

            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        SCLI_SHELL_INVALID_CMD,
                        AnscSizeOfString(SCLI_SHELL_INVALID_CMD)
                    );

            returnStatus = 
                pTscIf->Output
                    (
                        pTscIf->hOwnerContext,
                        hSrvSession,
                        TELNET_ANSI_TTY_CODE_Crlf,
                        AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                    );
        }
    }
    else
    {
        pCommand    = pCmdTokenList->pTokens[0].pValue;
        nErrorPos   = (int)pCmdTokenList->pTokens[0].ulStartPos;

        pCmdHelp = 
            (PBMC2_CMD_HELP_SYNTAX)pBmc2SccIf->GetCommandSyntax
                (
                    pBmc2SccIf->hOwnerContext,
                    pSession->hBmc2Terminal,
                    pCommand
                );

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TELNET_ANSI_TTY_CODE_Crlf,
                    AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                );

        if ( !pCmdHelp )
        {
            returnStatus =
                ScliShoIndicateError
                    (
                        hThisObject,
                        hSrvSession,
                        SCLI_SHELL_NO_HELP_FOUND,
                        pCmd,
                        nErrorPos
                    );
        }
        else if ( pCmdHelp->ulReqArgCount == 0 && pCmdHelp->ulOptArgCount == 0 )
        {
            if ( pCmdTokenList->ulTokenCount > 1 )
            {
                nErrorPos = (int)pCmdTokenList->pTokens[1].ulStartPos;

                returnStatus =
                    ScliShoIndicateError
                        (
                            hThisObject,
                            hSrvSession,
                            SCLI_SHELL_INVALID_ARG,
                            pCmd,
                            nErrorPos
                        );
            }
            else
            {
                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            SCLI_SHELL_OUTPUT_INDENT,
                            AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                        );

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            SCLI_SHELL_OPTION_CR,
                            AnscSizeOfString(SCLI_SHELL_OPTION_CR)
                        );

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            SCLI_SHELL_OUTPUT_INDENT,
                            AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                        );

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            SCLI_SHELL_OUTPUT_INDENT,
                            AnscSizeOfString(SCLI_SHELL_OUTPUT_INDENT)
                        );

                returnStatus =
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            SCLI_SHELL_OPTION_CR_DESC,
                            AnscSizeOfString(SCLI_SHELL_OPTION_CR_DESC)
                        );

                returnStatus = 
                    pTscIf->Output
                        (
                            pTscIf->hOwnerContext,
                            hSrvSession,
                            TELNET_ANSI_TTY_CODE_Crlf,
                            AnscSizeOfString(TELNET_ANSI_TTY_CODE_Crlf)
                        );
            }
        }
        else
        {
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
            }
            else
            {
                returnStatus =
                    ScliShoIndicateHelp
                        (
                            hThisObject,
                            hSrvSession,
                            pCmdTokenList->bEndWithSpace ? NULL : pCmdTokenList->pTokens[pCmdTokenList->ulTokenCount-1].pValue,
                            pCmdHelp,
                            pCmdTokenList,
                            pReqArgM,
                            pOptArgM,
                            &nErrorPos
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
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
                }
            }
        }
    }

    /* output user's input without the question mark and let user continue */
    if ( !bNoPrompt )
    {
        returnStatus = 
            pTscIf->SendShellPrompt
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    TRUE
                );

        returnStatus = 
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pCmd,
                    AnscSizeOfString(pCmd)
                );
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:
    if ( pCmdTokenList )
    {
        ScliShoFreeCmdTokenList(pCmdTokenList);
    }

    if ( pReqArgM )
    {
        AnscFreeMemory(pReqArgM);
    }

    if ( pOptArgM )
    {
        AnscFreeMemory(pOptArgM);
    }

    return  returnStatus;
}


