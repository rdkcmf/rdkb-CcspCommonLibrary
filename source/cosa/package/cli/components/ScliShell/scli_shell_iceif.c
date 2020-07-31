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

    module:	scli_shell_iceif.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the interface functions
        of the SCLI Shell Object.

        *   ScliShoIceWrite
        *   ScliShoIceReadInput
        *   ScliShoIceReadInput2
        *   ScliShoIceClearScreen
             
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/22/05    initial revision.

**********************************************************************/


#include "scli_shell_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIceWrite
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pContent
            )

    description:

        This function is called to output the content to client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pContent
                Content to output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIceWrite
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pContent        /* may or may not include "\r\n" */
    )
{
    PSCLI_SHELL_SESSION_EXEC        pSession        = (PSCLI_SHELL_SESSION_EXEC )hThisObject;
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv    = (PSCLI_SHELL_EXEC_ENV     )pSession->hCeeExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv        = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PTELNET_TSC_INTERFACE           pTscIf          = (PTELNET_TSC_INTERFACE    )pExecEnv->hTscIf;

    if ( !pContent || AnscSizeOfString(pContent) == 0 )
    {
        return ANSC_STATUS_SUCCESS;
    }

    return 
        pTscIf->Output
            (
                pTscIf->hOwnerContext,
                (ANSC_HANDLE)pSession->SessionId,
                pContent,
                AnscSizeOfString(pContent)
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIceReadInput
            (
                ANSC_HANDLE                 hThisObject,
                char**                      pTextInput
            )

    description:

        This function is called to read user input in the given
        text box.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char**                      pTextInput
                User input.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIceReadInput
    (
        ANSC_HANDLE                 hThisObject,
        char**                      pTextInput
    )
{

    return ScliShoIceReadInput2(hThisObject, (ANSC_HANDLE)NULL, pTextInput);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIceReadInput2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTextBoxInfo,
                char**                      pTextInput
            )

    description:

        This function is called to read user input in the given
        text box.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTextBoxInfo
                Text box info.

                char**                      pTextInput
                User input.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIceReadInput2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTextBoxInfo,
        char**                      pTextInput
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_SESSION_EXEC        pSession        = (PSCLI_SHELL_SESSION_EXEC )hThisObject;
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv    = (PSCLI_SHELL_EXEC_ENV     )pSession->hCeeExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv        = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PTELNET_TSC_INTERFACE           pTscIf          = (PTELNET_TSC_INTERFACE    )pExecEnv->hTscIf;
    PSCLI_SHELL_OBJECT              pScliShellObj   = (PSCLI_SHELL_OBJECT       )pSession->hScliShellObj;
    PUCHAR                          pCmd            = NULL;
    ULONG                           ulCmdCode;
    BOOL                            bCtrlCmd        = FALSE;
    PBMC2_ICE_TEXTBOX_INFO          pTextBoxInfo    = (PBMC2_ICE_TEXTBOX_INFO   )hTextBoxInfo;    

    pSession->hActiveTextBox    = (ANSC_HANDLE)pTextBoxInfo;
    pSession->InputMode         = SCLI_SHELL_INPUT_MODE_textbox;

    pScliShellObj->ClearBufferCmd
        (
            (ANSC_HANDLE)pScliShellObj,
            (ANSC_HANDLE)pSession
        );

    if ( pTextBoxInfo && pTextBoxInfo->Width != 0 )
    {
        PUCHAR                      pDefValue       = (PUCHAR)pTextBoxInfo->DefaultValue;
        ULONG                       ulDefValueLen   = pDefValue ? AnscSizeOfString((const char *)pDefValue) : 0;

        if ( ulDefValueLen > pTextBoxInfo->Width )
        {
            ulDefValueLen   = pTextBoxInfo->Width;
        }


        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    SCLI_SHELL_TEXTBOX_LEFT,
                    AnscSizeOfString(SCLI_SHELL_TEXTBOX_LEFT)
                );

        returnStatus =
            pTscIf->SendChar
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    ' ',
                    pTextBoxInfo->Width
                );

        returnStatus =
            pTscIf->Output
                (
                    pTscIf->hOwnerContext,
                    (ANSC_HANDLE)pSession->SessionId,
                    SCLI_SHELL_TEXTBOX_RIGHT,
                    AnscSizeOfString(SCLI_SHELL_TEXTBOX_RIGHT)
                );

        /* save the default value into input buffer */
        AnscAcquireLock(&pSession->AccessLock);

        pSession->ulFirstVisiblePos = 0;

        if ( pDefValue && ulDefValueLen != 0 )
        {
            if ( ulDefValueLen >= SCLI_SHELL_MAX_COMMAND_LEN )
            {
                ulDefValueLen   = SCLI_SHELL_MAX_COMMAND_LEN - 2;
            }

            AnscCopyMemory(pSession->Command, pDefValue, ulDefValueLen);
            pSession->CommandLen        = ulDefValueLen;
            pSession->CursorPos         = 0;
        }
        else
        {
            pSession->CommandLen        = 0;
            pSession->CursorPos         = 0;
            pSession->ulFirstVisiblePos = 0;
        }

        pSession->CursorPos    += pTextBoxInfo->Width + 1;

        returnStatus =
            pScliShellObj->MoveToTextBox
                (
                    (ANSC_HANDLE)pScliShellObj,
                    (ANSC_HANDLE)pSession->SessionId,
                    (ANSC_HANDLE)pExecEnv
                );

        pSession->CursorPos -= pTextBoxInfo->Width + 1;

        returnStatus =
            pScliShellObj->RedrawTextBoxInput
                (
                    (ANSC_HANDLE)pScliShellObj,
                    (ANSC_HANDLE)pSession->SessionId,
                    (ANSC_HANDLE)pExecEnv,
                    0
                );

        AnscReleaseLock(&pSession->AccessLock);
    }

    if ( !pSession->bWillExit )
    {
        returnStatus = 
            pScliShellObj->GetBufferedCmd
                (
                    (ANSC_HANDLE)pScliShellObj,
                    (ANSC_HANDLE)pSession->SessionId,
                    &ulCmdCode,
                    &pCmd
                );

        if ( returnStatus == ANSC_STATUS_PENDING )
        {
            pSession->bWaitInput    = TRUE;

            AnscWaitEvent((PSEM_EVENT *)&pSession->InputEvent, 0xFFFFFFFF);
            AnscResetEvent((PSEM_EVENT *)&pSession->InputEvent);

            returnStatus = 
                pScliShellObj->GetBufferedCmd
                    (
                        (ANSC_HANDLE)pScliShellObj,
                        (ANSC_HANDLE)pSession->SessionId,
                        &ulCmdCode,
                        &pCmd
                    );

            pSession->bWaitInput    = FALSE;
        }
    }

    if ( !bCtrlCmd && !pCmd )
    { 
        ULONG                       ulCmdLen;

        AnscAcquireLock(&pSession->AccessLock);

        ulCmdLen    = pSession->CommandLen;

        if ( ulCmdLen != 0 )
        {
            pCmd        = (PUCHAR)AnscAllocateMemory(ulCmdLen + 1);

            if ( pCmd )
            {
                AnscCopyMemory(pCmd, pSession->Command, ulCmdLen);
                pCmd[ulCmdLen]  = 0;
            }

            pSession->CommandLen        = 0;
            pSession->CursorPos         = 0;
            pSession->ulFirstVisiblePos = 0;
        }

        AnscReleaseLock(&pSession->AccessLock);
    }

    if ( pTextInput )
    {
        *pTextInput = (char *)pCmd;
    }
    else
    {
        if(pCmd)
        {
            AnscFreeMemory(pCmd);/*RDKB-5891, CID-24444, freeing allocated mem*/
        }
    }

    pSession->hActiveTextBox    = (ANSC_HANDLE)NULL;
    pSession->InputMode         = SCLI_SHELL_INPUT_MODE_linemode;

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoIceClearScreen
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear terminal screen.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoIceClearScreen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_SESSION_EXEC        pSession        = (PSCLI_SHELL_SESSION_EXEC )hThisObject;
    PSCLI_SHELL_EXEC_ENV            pScliExecEnv    = (PSCLI_SHELL_EXEC_ENV     )pSession->hCeeExecEnv;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv        = (PTELNET_CMD_EXECUTION_ENV)pScliExecEnv->hTelnetEnv;
    PSCLI_SHELL_OBJECT              pScliShellObj   = (PSCLI_SHELL_OBJECT       )pSession->hScliShellObj;

    returnStatus =
        pScliShellObj->RunBuiltInCmd
            (
                (ANSC_HANDLE)pScliShellObj,
                (ANSC_HANDLE)pSession->SessionId,
                (ANSC_HANDLE)pExecEnv,
                SCLI_SHELL_BICODE_Clear
            );

    return returnStatus;
}



