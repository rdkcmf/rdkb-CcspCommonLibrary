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

    module:	scli_shell_global.h

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Simple CLI Shell
        Object.

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


#ifndef  _SCLI_SHELL_INTERNAL_API_
#define  _SCLI_SHELL_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_STATES.C
***********************************************************/

ANSC_HANDLE
ScliShoGetTspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoSetTspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
ScliShoGetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoSetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
ScliShoSetAuthIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
ScliShoSetTscIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
ScliShoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
ScliShoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
ScliShoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoShellPromptChanged
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pMenuTitle
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_PROCESS.C
***********************************************************/

BOOL
ScliShoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

ANSC_STATUS
ScliShoServe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTelnetSession,
        ANSC_HANDLE                 hExecutionProp,     /* _TELNET_CMD_EXECUTION_ENV */
        PVOID                       pCmd,
        ULONG                       ulCmdSize
    );

ANSC_STATUS
ScliShoSignal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTelnetSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       ulTspSignal
    );

ANSC_HANDLE
ScliShoGetSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    );

ANSC_HANDLE
ScliShoCreateSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    );

ANSC_STATUS
ScliShoRemoveSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
ScliShoStartUserAuth
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

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
    );

ANSC_STATUS
ScliShoProcessCmdData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PVOID                       pCmd,
        ULONG                       ulCmdLen
    );

ANSC_STATUS
ScliShoProcessCmdChar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        UCHAR                       ucChar,
        PUCHAR                      pEchoBuf,
        PULONG                      pulEchoSize
    );

ANSC_STATUS
ScliShoRunCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

ANSC_STATUS
ScliShoCacheCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    );

BOOL
ScliShoIsCmdModified
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    );

ANSC_STATUS
ScliShoRunBuiltInCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       CmdCode
    );

ANSC_STATUS
ScliShoRunNonBuiltInCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    );

ANSC_STATUS
ScliShoRunShellCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    );

ANSC_STATUS
ScliShoValidateCmdArgs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    );

ULONG
ScliShoGetCmdCode
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pCmd
    );

PUCHAR
ScliShoGetPrevCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

PUCHAR
ScliShoGetNextCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

PUCHAR
ScliShoGetFirstCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

PUCHAR
ScliShoGetLastCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

ULONG
ScliShoGetEscCmdCode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd,
        ULONG                       ulCmdLen,
        PULONG                      pulEscCodeLen
    );

ULONG
ScliShoGetInputMode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
ScliShoRedrawTextBoxInput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       ulCursorTBoxPos
    );

ANSC_STATUS
ScliShoMoveToTextBox
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

ULONG
ScliShoGetNumActiveSessions
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_EXEC.C
***********************************************************/

ANSC_STATUS
ScliShoAsyncRunCmdTask
    (
        ANSC_HANDLE                 hContext
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_CENIF.C
***********************************************************/

ANSC_STATUS
ScliShoCenNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ULONG                       CenEvent
    );

ANSC_STATUS
ScliShoCenSetShellPrompt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        PUCHAR                      pPrompt
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_BIC.C
***********************************************************/

ANSC_STATUS
ScliShoRunBicExit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicClear
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicShell
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicHistory
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       CmdCode
    );

ANSC_STATUS
ScliShoRunBicCursorBackward
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicCursorForward
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicCursorHome
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicCursorEnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicDel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicBackspace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicBreak
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicAutoCompletion
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoRunBicHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
ScliShoValidateCommand
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_BUFFER.C
***********************************************************/

ANSC_STATUS
ScliShoDoBufferedCommands
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

ANSC_STATUS
ScliShoGetBufferedCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PULONG                      pulCmdCode,
        PUCHAR                      *ppCmd
    );

ANSC_STATUS
ScliShoBufferCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ULONG                       ulCmdCode,
        PUCHAR                      pCmd
    );

ANSC_STATUS
ScliShoClearBufferCmd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_ICEIF.C
***********************************************************/

ANSC_STATUS
ScliShoIceWrite
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pContent        /* may or may not include "\r\n" */
    );

ANSC_STATUS
ScliShoIceReadInput
    (
        ANSC_HANDLE                 hThisObject,
        char**                      pTextInput
    );

ANSC_STATUS
ScliShoIceReadInput2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTextBoxInfo,
        char**                      pTextInput
    );

ANSC_STATUS
ScliShoIceClearScreen
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_CONTROL.C
***********************************************************/

ANSC_STATUS
ScliShoInitTdo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoKillTdo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoStartTdo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoStopTdo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
ScliShoIdleTask
    (
        ANSC_HANDLE                 hContext
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SCLI_SHELL_HELP.C
***********************************************************/

PSCLI_SHELL_CMD_TLIST
ScliShoTokenizeCommand
    (
        char*                       pCmd,
        char                        pSeparator
    );

void
ScliShoFreeCmdTokenList
    (
        PSCLI_SHELL_CMD_TLIST       pTokenList
    );

ANSC_STATUS
ScliShoIndicateError
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pErrorCode,
        char*                       pCmd,
        int                         nErrorPos
    );

ANSC_STATUS
ScliShoIndicateHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int*                        pErrorPos
    );

BOOL
ScliShoMatchSimpleArg
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulArgType,
        PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        int*                        pErrorPos
    );

BOOL
ScliShoMatchDepArg
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_DEP_ARG           pDepArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        int*                        pErrorPos
    );

BOOL
ScliShoMatchOptArgs
    (
        ANSC_HANDLE                 hThisObject,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        PBOOL                       pbPartialMatched,
        PULONG                      pulTokenMatched,
        int*                        pErrorPos
    );

BOOL
ScliShoMatchArg
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_ARGUMENT          pArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        int*                        pErrorPos
    );

BOOL
ScliShoMatchCommand
    (
        ANSC_HANDLE                 hThisObject,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int*                        pErrorPos
    );

int
ScliShoGetLastMatchedOptArg
    (
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount
    );

int
ScliShoFindDepArgOnName
    (
        PBMC2_CMD_ARGUMENT          pOptArgs,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        char*                       pOptArgName,
        PBOOL                       pbOptional
    );

int
ScliShoGetOptNamedArgMatched
    (
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        char*                       pArgName
    );

BOOL
ScliShoIsArgValueValid
    (
        ULONG                       ulDataType,
        ULONG                       ulMinOccur,
        ULONG                       ulMaxOccur,
        ULONG                       ulMinLength,
        ULONG                       ulMaxLength,
		BOOL						bStrDupAllowed,
        char*                       pValue,
        PBMC2_CMD_ARG_VRANGE        pRange,
        int*                        pErrorPos,
        PBOOL                       pbMultiMathced
    );

void
ScliShoInitArgMatchResult
    (
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult
    );

ANSC_STATUS
ScliShoShowArgHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pCmdArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        BOOL                        bReqArg,
        ULONG                       ulCurArg,
        int*                        pErrorPos
    );

ANSC_STATUS
ScliShoShowOptArgHelp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int                         nCurArg,
        int*                        pErrorPos
    );

ANSC_STATUS
ScliShoShowArgHelpList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PSCLI_SHELL_ARG_HELP_LIST   pHelpList
    );

int
ScliShoDepArgFindMatchedBranch
    (
        PBMC2_CMD_DEP_ARG           pDepArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount
    );

ANSC_STATUS
ScliShoGetSimpleArgHelp
    (
        ULONG                       ulArgType,
        PBMC2_CMD_SIMPLE_ARG        pSimpleArg,
        char*                       pPrefix,
        PSCLI_SHELL_ARG_HELP_LIST   pHelpList,
        ULONG                       ulListStartFrom,
        PULONG                      pulMatchedHelpCount,
        int*                        pErrorPos
    );

ANSC_STATUS
ScliShoAutoCompleteCommandArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_HELP_SYNTAX       pCmdSyntax,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        SLAP_STRING_ARRAY**         ppStringArray
    );

ANSC_STATUS
ScliShoAutoCompleteArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pCmdArg,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        ULONG                       ulOptArgCount,
        BOOL                        bReqArg,
        ULONG                       ulCurArg,
        SLAP_STRING_ARRAY**         ppStringArray
    );

ANSC_STATUS
ScliShoAutoCompleteOptArg
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        char*                       pPrefix,
        PBMC2_CMD_ARGUMENT          pOptArgs,
        ULONG                       ulOptArgCount,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PSCLI_CMD_ARG_MATCH_RESULT  pReqArgMatched,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pOptArgMatched,
        int                         nCurArg,
        SLAP_STRING_ARRAY**         ppStringArray
    );

BOOL
ScliShoCanReqArgsSkipped
    (
        ANSC_HANDLE                 hThisObject,
        PSCLI_SHELL_CMD_TLIST       pCmdTokenList,
        PBMC2_CMD_ARGUMENT          pReqArgs,
        ULONG                       ulReqArgCount,
        PSCLI_CMD_ARG_MATCH_RESULT  pArgMatchResult,
        ULONG                       ulReqArgFrom
    );

#endif

