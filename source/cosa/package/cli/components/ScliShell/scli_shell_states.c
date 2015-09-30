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

    module:	scli_shell_states.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the SCLI Shell Object.

        *   ScliShoGetTspIf
        *   ScliShoSetTspIf
        *   ScliShoGetBmc2SccIf
        *   ScliShoSetBmc2SccIf
        *   ScliShoSetSlapPoaIf
        *   ScliShoSetAuthIf
        *   ScliShoSetTscIf
        *   ScliShoResetProperty
        *   ScliShoReset
        *   ScliShoShellPromptChanged

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

        ANSC_HANDLE
        ScliShoGetTspIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
ScliShoGetTspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT                pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY              pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    return  pMyObject->hTspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoSetTspIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSetTspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT   )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY )&pMyObject->Property;
    PTELNET_TSP_INTERFACE           pTspIf       = (PTELNET_TSP_INTERFACE)pMyObject->hTspIf;

    *(PTELNET_TSP_INTERFACE)pTspIf = *(PTELNET_TSP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        ScliShoGetBmc2SccIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
ScliShoGetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    return  pMyObject->hBmc2SccIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoSetBmc2SccIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE )pMyObject->hBmc2SccIf;

    pMyObject->hBmc2SccIf   = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoSetAuthIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSetAuthIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE )pMyObject->hBmc2SccIf;

    pMyObject->hAuthIf      = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoSetTscIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSetTscIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;
    PBMC2_SCC_INTERFACE             pBmc2SccIf   = (PBMC2_SCC_INTERFACE )pMyObject->hBmc2SccIf;

    pMyObject->hTscIf   = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to get object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Property.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    *((PSCLI_SHELL_PROPERTY)hProperty)    = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Property.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    *pProperty  = *((PSCLI_SHELL_PROPERTY)hProperty);

    if ( pProperty->CmdCacheMaxCount == 0 )
    {
        pProperty->CmdCacheMaxCount = SCLI_SHELL_CACHED_CMD_DEF_COUNT;
    }
    else
    if ( pProperty->CmdCacheMaxCount < SCLI_SHELL_CACHED_CMD_MIN_COUNT )
    {
        pProperty->CmdCacheMaxCount = SCLI_SHELL_CACHED_CMD_MIN_COUNT;
    }
    else if ( pProperty->CmdCacheMaxCount > SCLI_SHELL_CACHED_CMD_MAX_COUNT )
    {
        pProperty->CmdCacheMaxCount = SCLI_SHELL_CACHED_CMD_MAX_COUNT;
    }

    /* always works in asynchronous mode */
    pProperty->Mode = SCLI_SHELL_RUN_MODE_ASYNC;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    pProperty->bUserAuth        = TRUE;
    pProperty->CmdCacheMaxCount = SCLI_SHELL_CACHED_CMD_DEF_COUNT;
    AnscCopyString(pProperty->Greeting, SCLI_SHELL_DEF_GREETING_MSG);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT  )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty    = (PSCLI_SHELL_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoShellPromptChanged
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvSession,
                ANSC_HANDLE                 hExecEnv,
                PUCHAR                      pMenuTitle
            )

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvSession
                The session which shell prompt will be changed.

                PUCHAR                      pMenuTitle
                Menu title.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoShellPromptChanged
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pMenuTitle
    )
{
    PSCLI_SHELL_OBJECT              pMyObject       = (PSCLI_SHELL_OBJECT       )hThisObject;
    PSCLI_SHELL_PROPERTY            pProperty       = (PSCLI_SHELL_PROPERTY     )&pMyObject->Property;
    PTELNET_CMD_EXECUTION_ENV       pExecEnv        = (PTELNET_CMD_EXECUTION_ENV)hExecEnv;
    PTELNET_TSC_INTERFACE           pTscIf          = (PTELNET_TSC_INTERFACE    )pMyObject->hTscIf;
    PSCLI_SHELL_SESSION_EXEC        pSession;

    pSession    = (PSCLI_SHELL_SESSION_EXEC)pMyObject->GetSession((ANSC_HANDLE)pMyObject, (ULONG)hSrvSession);

    if ( pSession )
    {
        ULONG           ulPromptLen = 0;
        PUCHAR          pPrompt     = NULL;
        ULONG           ulMaxMenuLen= TELNET_SSO_SHELL_PROMPT_LEN;

        if ( pMenuTitle )
        {
            ulPromptLen  = AnscSizeOfString(pMenuTitle);
        }

        ulPromptLen += AnscSizeOfString(pProperty->ShellPrompt) + 1;
        
        if ( ulPromptLen + 5 > TELNET_SSO_SHELL_PROMPT_LEN )
        {
            ulPromptLen = TELNET_SSO_SHELL_PROMPT_LEN - 5;

            ulMaxMenuLen = ulPromptLen - 1 - AnscSizeOfString(pProperty->ShellPrompt);

            _ansc_memmove(pMenuTitle, pMenuTitle + AnscSizeOfString(pMenuTitle) - ulMaxMenuLen, ulMaxMenuLen);
            pMenuTitle[ulMaxMenuLen]    = 0;
        }

        pPrompt = (PUCHAR)AnscAllocateMemory(ulPromptLen + 10);

        if ( pPrompt )
        {
            if ( pMenuTitle )
            {
                _ansc_sprintf
                    (
                        pPrompt,
                        "%s (%s) %c ",
                        pProperty->ShellPrompt,
                        pMenuTitle,
                        pSession->bPrivileged ? 
                            SCLI_SHELL_PRIVILEGED_SIGN : 
                            SCLI_SHELL_NORMAL_SIGN
                    );
            }
            else
            {
                _ansc_sprintf
                    (
                        pPrompt,
                        "%s %c ",
                        pProperty->ShellPrompt,
                        pSession->bPrivileged ? 
                            SCLI_SHELL_PRIVILEGED_SIGN : 
                            SCLI_SHELL_NORMAL_SIGN
                    );
            }

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
            if ( pMyObject->ShellPrompt )
            {
                AnscFreeMemory(pMyObject->ShellPrompt);
            }

            pMyObject->ShellPrompt = AnscCloneString(pPrompt);
#endif

            pTscIf->SetShellPrompt
                (
                    pTscIf->hOwnerContext,
                    hSrvSession,
                    pPrompt
                );

            AnscFreeMemory(pPrompt);
        }
    }

    return ANSC_STATUS_SUCCESS;
}



