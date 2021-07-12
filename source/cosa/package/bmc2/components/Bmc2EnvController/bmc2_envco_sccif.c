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

    module:	bmc2_envco_sccif.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Bmc2 Env Controller Object.

        *   Bmc2EnvcoSccSetRootInfo
        *   Bmc2EnvcoSccSetMultiUserCtrl
        *   Bmc2EnvcoSccAddCommand
        *   Bmc2EnvcoSccAddTerminal
        *   Bmc2EnvcoSccDelTerminal
        *   Bmc2EnvcoSccGetCommands
        *   Bmc2EnvcoSccIsCommandValid
        *   Bmc2EnvcoSccExecuteCommand
        *   Bmc2EnvcoSccAbortExecution

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#include "bmc2_envco_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccSetRootInfo
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRootPath,
                char*                       pRootDomainCommand
            );

    description:

        This function is called to configure the root path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRootPath
                Specifies the root path to be configured.

                char*                       pRootDomainCommand
                Specifies the root domain name to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccSetRootInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRootPath,
        char*                       pRootDomainCommand
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty    = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;
    errno_t rc = -1;

    AnscZeroMemory(pProperty->RootPath,          ANSC_MAX_FILE_NAME_SIZE   );
    rc = STRCPY_S_NOCLOBBER(pProperty->RootPath, sizeof(pProperty->RootPath), pRootPath );
    ERR_CHK(rc);
    AnscZeroMemory(pProperty->RootDomainCommand, BMC2_MAX_COMMAND_NAME_SIZE);
    rc = STRCPY_S_NOCLOBBER(pProperty->RootDomainCommand, sizeof(pProperty->RootDomainCommand), pRootDomainCommand        );
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccSetMultiUserCtrl
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to configure the multiple user control.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies whether user control should be enabled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccSetMultiUserCtrl
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty    = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    pProperty->bMultiUserCtrl = bEnabled;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccAddCommand
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pDomainNamePattern,
                char*                       pCommand,
                char*                       pScpPage,
                BOOL                        bReadOnly,
                ULONG                       ulPermissions
            );

    description:

        This function is called to add a command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pDomainNamePattern
                Specifies the domain name pattern.

                char*                       pCommand
                Specifies the command name to be added.

                char*                       pScpPage
                Specifies the corresonding SCP page name.

                BOOL                        bReadOnly
                Specifies whether this is a read-only command.

                ULONG                       ulPermissions
                Specifies the associated user permissions.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccAddCommand
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainNamePattern,
        char*                       pCommand,
        char*                       pScpPage,
        BOOL                        bReadOnly,
        ULONG                       ulPermissions
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;
    ANSC_HANDLE                     hBmc2CommandRef;

    hBmc2CommandRef =
        pMyObject->DelCommandProperty
            (
                (ANSC_HANDLE)pMyObject,
                pCommand,
                pDomainNamePattern
            );

    pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY)AnscAllocateMemory(sizeof(BMC2_COMMAND_PROPERTY));

    if ( !pBmc2CommandProperty )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        if ( pDomainNamePattern )
        {
            pBmc2CommandProperty->DomainNamePattern = AnscCloneString(pDomainNamePattern);
        }
        pBmc2CommandProperty->CommandName = AnscCloneString(pCommand);
        pBmc2CommandProperty->ScpPageName = AnscCloneString(pScpPage);
        pBmc2CommandProperty->bReadOnly   = bReadOnly;
        pBmc2CommandProperty->bAuthorized = TRUE;
        pBmc2CommandProperty->Permissions = ulPermissions;
    }

    returnStatus =
        pMyObject->AddCommandProperty
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmc2CommandProperty,
                hBmc2CommandRef
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoSccAddTerminal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminalInfo,
                ANSC_HANDLE                 hBmc2IceIf
            );

    description:

        This function is called to add a com terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminalInfo
                Specifies the information needed to create the terminal.

                ANSC_HANDLE                 hBmc2IceIf
                Specifies the ICE interface assigned for the terminal.

    return:     terminal handle.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoSccAddTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminalInfo,
        ANSC_HANDLE                 hBmc2IceIf
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty            = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;
    PBMC2_TERMINAL_INFO             pBmc2TerminalInfo    = (PBMC2_TERMINAL_INFO          )hTerminalInfo;
    PBMC2_ICE_INTERFACE             pBmc2IceIf           = (PBMC2_ICE_INTERFACE          )hBmc2IceIf;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal     = (PBMC2_COM_TERMINAL_OBJECT    )NULL;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain       = (PBMC2_COM_DOMAIN_OBJECT      )NULL;
    PBMC2_COMMAND_REQUEST           pBmc2CommandReq      = (PBMC2_COMMAND_REQUEST        )NULL;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep      = (PBMC2_COMMAND_REPLY          )NULL;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pBmc2ComTerminal =
        (PBMC2_COM_TERMINAL_OBJECT)Bmc2CreateComTerminal
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pBmc2ComTerminal )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pMyObject->pPseudoTermDevName )
        {
            pBmc2ComTerminal->SetPseudoTermDevName((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pMyObject->pPseudoTermDevName);
        }

        pBmc2ComTerminal->SetUserIdentifier   ((ANSC_HANDLE)pBmc2ComTerminal, pBmc2TerminalInfo->UserIdentifier );
        pBmc2ComTerminal->SetUserPermission   ((ANSC_HANDLE)pBmc2ComTerminal, pBmc2TerminalInfo->UserPermission );
        pBmc2ComTerminal->SetReadOnly         ((ANSC_HANDLE)pBmc2ComTerminal, pBmc2TerminalInfo->bReadOnly      );
        pBmc2ComTerminal->SetMaxLineNumber    ((ANSC_HANDLE)pBmc2ComTerminal, pBmc2TerminalInfo->MaxLineNumber  );
        pBmc2ComTerminal->SetMaxColumnNumber  ((ANSC_HANDLE)pBmc2ComTerminal, pBmc2TerminalInfo->MaxColumnNumber);

        pBmc2ComTerminal->SetBmc2IceIf        ((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pBmc2IceIf           );
        pBmc2ComTerminal->SetBmc2EnvController((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pMyObject            );
        pBmc2ComTerminal->Engage              ((ANSC_HANDLE)pBmc2ComTerminal);
    }

    /*
     * When user first logs into the terminal, he/she is not in any domain (or at the root level).
     * It's better to manage the root level commands as other domain-bound commands. To achieve
     * such ubiquitous domain-oriented console management, we must execute a "root_domain" command
     * internally.
     */
    pBmc2CommandReq = (PBMC2_COMMAND_REQUEST)AnscAllocateMemory(sizeof(BMC2_COMMAND_REQUEST));
    pBmc2CommandRep = (PBMC2_COMMAND_REPLY  )AnscAllocateMemory(sizeof(BMC2_COMMAND_REPLY  ));

    if ( !pBmc2CommandReq || !pBmc2CommandRep )
    {
        goto  EXIT1;
    }
    else
    {
        pBmc2CommandReq->CommandLine   = AnscCloneString(pProperty->RootDomainCommand);
        pBmc2CommandReq->CurLineNumber = 1;
    }

    /*
     * Since the ExecuteCommand() API always assumes a Domain object is already present, now we
     * find ourselves in a catch-22 situation: we want to enter the root domain but we need to have
     * domain object to be able to execute this command. To solve this problem, we create a temp-
     * orary domain object before the execution and remove it afterwards.
     */
    pBmc2ComDomain =
        (PBMC2_COM_DOMAIN_OBJECT)Bmc2CreateComDomain
            (
                pBmc2ComTerminal->hContainerContext,
                (ANSC_HANDLE)pBmc2ComTerminal,
                (ANSC_HANDLE)NULL
            );

    if ( !pBmc2ComDomain )
    {
        goto  EXIT1;
    }
    else
    {
        pBmc2ComDomain->SetBmc2ComTerminal  ((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2ComTerminal);
        pBmc2ComDomain->SetBmc2EnvController((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pMyObject       );
        pBmc2ComDomain->SetDomainTitle      ((ANSC_HANDLE)pBmc2ComDomain, "m_domain"                   );
        pBmc2ComDomain->Engage              ((ANSC_HANDLE)pBmc2ComDomain);
    }

    pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY)AnscAllocateMemory(sizeof(BMC2_COMMAND_PROPERTY));

    if ( !pBmc2CommandProperty )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT2;
    }
    else
    {
        pBmc2CommandProperty->CommandName = AnscCloneString(pProperty->RootDomainCommand);
        pBmc2CommandProperty->ScpPageName = (char*)AnscAllocateMemory(AnscSizeOfString(pProperty->RootDomainCommand) + 16);
        pBmc2CommandProperty->bReadOnly   = FALSE;
        pBmc2CommandProperty->bAuthorized = TRUE;
        pBmc2CommandProperty->Permissions = 0xFFFFFFFF;

        if ( !pBmc2CommandProperty->CommandName || !pBmc2CommandProperty->ScpPageName )
        {
            Bmc2FreeCommandProperty(pBmc2CommandProperty);

            goto  EXIT2;
        }
        else
        {
            _ansc_sprintf
                (
                    pBmc2CommandProperty->ScpPageName,
                    "%s.scp",
                    pBmc2CommandProperty->CommandName
                );
        }

        pBmc2ComDomain->AddCommandProperty((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2CommandProperty);
    }

    pBmc2ComTerminal->AddComDomain((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pBmc2ComDomain);

    /*
     * Finally, we can execute the "root_domain" command...
     */
    returnStatus =
        pMyObject->SccExecuteCommand
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmc2ComTerminal,
                (ANSC_HANDLE)pBmc2CommandReq,
                (ANSC_HANDLE)pBmc2CommandRep
            );

    pBmc2ComTerminal->DelComDomain((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pBmc2ComDomain);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        Bmc2FreeCommandRequest(pBmc2CommandReq);
        Bmc2FreeCommandReply  (pBmc2CommandRep);
    }

    returnStatus =
        pMyObject->AddComTerminal
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmc2ComTerminal
            );

    return  (ANSC_HANDLE)pBmc2ComTerminal;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pBmc2ComDomain )
    {
        pBmc2ComDomain->Cancel((ANSC_HANDLE)pBmc2ComDomain);
        pBmc2ComDomain->Remove((ANSC_HANDLE)pBmc2ComDomain);
    }

EXIT1:

    if ( pBmc2CommandReq )
    {
        Bmc2FreeCommandRequest(pBmc2CommandReq);
    }

    if ( pBmc2CommandRep )
    {
        Bmc2FreeCommandReply(pBmc2CommandRep);
    }

    pBmc2ComTerminal->Cancel((ANSC_HANDLE)pBmc2ComTerminal);
    pBmc2ComTerminal->Remove((ANSC_HANDLE)pBmc2ComTerminal);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccDelTerminal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminal
            );

    description:

        This function is called to delete a com terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminal
                Specifies the terminal handle to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccDelTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject        = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;

    returnStatus =
        pMyObject->DelComTerminal
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmc2ComTerminal
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccGetCommands
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminal,
                char*                       pCommandPrefix,
                SLAP_STRING_ARRAY**         ppStringArray
            );

    description:

        This function is called to retrieve the list of commands that
        match the specified 'pCommandPrefix'.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminal
                Specifies the current terminal environment.

                char*                       pCommandPrefix
                Specifies the command prefix to be matched.

                SLAP_STRING_ARRAY**         ppStringArray
                Specifies the command list to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccGetCommands
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommandPrefix,
        SLAP_STRING_ARRAY**         ppStringArray
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain   = (PBMC2_COM_DOMAIN_OBJECT      )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);

    if ( !pBmc2ComDomain )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    *ppStringArray =
        pBmc2ComDomain->GetCommandsByPrefix
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommandPrefix
            );

    return  *ppStringArray? ANSC_STATUS_SUCCESS : ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Bmc2EnvcoSccIsCommandValid
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminal,
                char*                       pCommand,
                PBOOL                       pbAuthorized
            );

    description:

        This function is called to check whether the command is valid
        and authorized.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminal
                Specifies the current terminal environment.

                char*                       pCommand
                Specifies the command name to be matched.

                PBOOL                       pbAuthorized
                Specifies the authorization status to be returned.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
Bmc2EnvcoSccIsCommandValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand,
        PBOOL                       pbAuthorized
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal     = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain       = (PBMC2_COM_DOMAIN_OBJECT      )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;

    *pbAuthorized = FALSE;

    if ( !pBmc2ComDomain )
    {
        return  FALSE;
    }

    pBmc2CommandProperty =
        (PBMC2_COMMAND_PROPERTY)pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand
            );

    if ( !pBmc2CommandProperty )
    {
        return  FALSE;
    }
    else
    {
        *pbAuthorized = pBmc2CommandProperty->bAuthorized;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccExecuteCommand
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminal,
                ANSC_HANDLE                 hCommandReq,
                ANSC_HANDLE                 hCommandRep
            );

    description:

        This function is called to execute a command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminal
                Specifies the current terminal environment.

                ANSC_HANDLE                 hCommandReq
                Specifies the command request to be executed.

                ANSC_HANDLE                 hCommandRep
                Specifies the command reply to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccExecuteCommand
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        ANSC_HANDLE                 hCommandReq,
        ANSC_HANDLE                 hCommandRep
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject            = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty            = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor     = (PBMC2_COM_EXECUTOR_OBJECT    )pMyObject->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController   = (PBWRM_ENV_CONTROLLER_OBJECT  )pMyObject->hBwrmEnvController;
    PBWRM_RAM_INTERFACE             pBwrmRamIf           = (PBWRM_RAM_INTERFACE          )pBwrmEnvController->GetBwrmRamIf((ANSC_HANDLE)pBwrmEnvController);
    PBMC2_PEC_INTERFACE             pBmc2PecIf           = (PBMC2_PEC_INTERFACE          )NULL;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal     = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain       = (PBMC2_COM_DOMAIN_OBJECT      )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_COMMAND_REQUEST           pBmc2CommandReq      = (PBMC2_COMMAND_REQUEST        )hCommandReq;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep      = (PBMC2_COMMAND_REPLY          )hCommandRep;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY       )NULL;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT  )NULL;
    PANSC_TOKEN_CHAIN               pCliTokenChain       = (PANSC_TOKEN_CHAIN            )NULL;
    PANSC_STRING_TOKEN              pCommandToken        = (PANSC_STRING_TOKEN           )NULL;
    PANSC_STRING_TOKEN              pArgumentToken       = (PANSC_STRING_TOKEN           )NULL;
    PBWRM_COOKED_PAGE_OBJECT        pBwrmCookedPage      = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    void*                           pPageBuffer          = (void*                        )NULL;
    ULONG                           ulPageSize           = (ULONG                        )0;
    BOOL                            bCacheCookedPage     = (BOOL                         )TRUE;

    if ( !pBmc2ComDomain )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_internalError;

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else if ( !pBmc2CommandReq->CommandLine )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_invalidCommand;

        return  ANSC_STATUS_BAD_PARAMETER;
    }

    pBmc2ComTerminal->AcqAccess((ANSC_HANDLE)pBmc2ComTerminal);

    pCliTokenChain =
        AnscTcAllocate3
            (
                pBmc2CommandReq->CommandLine,
                BMC2_DEF_CLI_SEPARATORS
            );

    if ( !pCliTokenChain )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_invalidCommand;

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pCliTokenChain) == 0 )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_invalidCommand;

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else
    {
        /* scan the token list to see if there's quoted string and merge them accordingly */
        PSINGLE_LINK_ENTRY          pSLinkEntry = NULL, pSLinkEntryNext;
        PANSC_STRING_TOKEN          pStrToken, pStrTokenNext;

        pSLinkEntry = AnscQueueGetFirstEntry(&pCliTokenChain->TokensQueue);
        while ( pSLinkEntry )
        {
            pStrToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pStrToken->Name[0] == BMC2_CMD_ARG_VALUE_QSTRING_SEP )
            {
                int                 nStrLen     = AnscSizeOfString(pStrToken->Name);
                BOOL                bDone       = FALSE;
                ULONG               ulCopySize  = 0;

                _ansc_memmove(pStrToken->Name, pStrToken->Name+1, nStrLen-1);
                pStrToken->Name[nStrLen-1] = 0;

                pSLinkEntryNext = pSLinkEntry;

                while ( !bDone && pSLinkEntryNext )
                {
                    pSLinkEntry     = pSLinkEntryNext;
                    pStrTokenNext   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntryNext);
                    pSLinkEntryNext = AnscQueueGetNextEntry(pSLinkEntryNext);

                    AnscQueuePopEntryByLink(&pCliTokenChain->TokensQueue, pSLinkEntry);

                    nStrLen = AnscSizeOfString(pStrTokenNext->Name);

                    if ( pStrTokenNext->Name[nStrLen-1] == BMC2_CMD_ARG_VALUE_QSTRING_SEP )
                    {
                        bDone = TRUE;
                    }

                    ulCopySize = AnscSizeOfString(pStrTokenNext->Name);

                    if ( ulCopySize + AnscSizeOfString(pStrToken->Name) >= ANSC_URI_PATH_SIZE )
                    {
                        ulCopySize = ANSC_URI_PATH_SIZE - AnscSizeOfString(pStrToken->Name);
                    }

                    if ( ulCopySize != 0 )
                    {
                        AnscCopyMemory(pStrToken->Name + AnscSizeOfString(pStrToken->Name), pStrTokenNext->Name, ulCopySize);
                        pStrToken->Name[AnscSizeOfString(pStrToken->Name) + ulCopySize] = 0;
                    }
                }

                pSLinkEntry = pSLinkEntryNext;
            }
        }

        pCommandToken = AnscTcUnlinkToken(pCliTokenChain);
    }

    pBmc2CommandProperty =
        (PBMC2_COMMAND_PROPERTY)pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommandToken->Name
            );

    AnscFreeMemory(pCommandToken);

    if ( !pBmc2CommandProperty )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_invalidCommand;

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( !pBmc2CommandProperty->bAuthorized )
    {
        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_unauthorized;

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }

    /*
     * The script parsing can take fairly long time, which mandates some sort of page caching
     * mechanism to be used. The BWRM (BroadWay Web Resource Manager) module is responsible for
     * file-based page access and page caching. We first try to retrieve the requested page from
     * the in-memory cache, and only load the page from the underlying storage system if the
     * requested page is not in the cache.
     */
    returnStatus =
        pBwrmRamIf->GetCookedPage
            (
                pBwrmRamIf->hOwnerContext,
                pProperty->RootPath,
                pBmc2CommandProperty->ScpPageName,
                (ANSC_HANDLE)&pBwrmCookedPage
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        returnStatus =
            pBwrmRamIf->GetRawPage
                (
                    pBwrmRamIf->hOwnerContext,
                    pProperty->RootPath,
                    pBmc2CommandProperty->ScpPageName,
                    &pPageBuffer,
                    &ulPageSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_pageNotFound;

            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

            goto  EXIT1;
        }
        else
        {
            pBwrmCookedPage =
                (PBWRM_COOKED_PAGE_OBJECT)pBmc2ComExecutor->PreparePage
                    (
                        (ANSC_HANDLE)pBmc2ComExecutor,
                        pBmc2CommandProperty->ScpPageName,
                        pPageBuffer,
                        ulPageSize
                    );
        }

        if ( !pBwrmCookedPage )
        {
            pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_internalError;

            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

            goto  EXIT2;
        }
        else if ( pProperty->bCacheScpPages || pBwrmRamIf->IsPageCacheForced(pBwrmRamIf->hOwnerContext, pBmc2CommandProperty->ScpPageName) )
        {
            bCacheCookedPage = TRUE;
            returnStatus     =
                pBwrmRamIf->AddCookedPage
                    (
                        pBwrmRamIf->hOwnerContext,
                        pProperty->RootPath,
                        pBmc2CommandProperty->ScpPageName,
                        (ANSC_HANDLE)pBwrmCookedPage
                    );
        }
        else
        {
            bCacheCookedPage = FALSE;

            pBwrmCookedPage->SetRootPath ((ANSC_HANDLE)pBwrmCookedPage, pProperty->RootPath              );
            pBwrmCookedPage->SetPagePath ((ANSC_HANDLE)pBwrmCookedPage, pBmc2CommandProperty->ScpPageName);
            pBwrmCookedPage->SetTimestamp((ANSC_HANDLE)pBwrmCookedPage, AnscGetTickInSeconds()           );
        }
    }

    /*
     * The Req Controller Object builds a sandbox for the page execution engine. It provides the
     * direct access to the Command, Input, and Output Objects, and indirect access to the App-
     * lication, Server, Terminal and Domain Objects. The Req Controller Object MUST be instan-
     * tiated for every CLI command request. However, it may be pooled.
     */
    if ( TRUE )
    {
        pBmc2ReqController =
            (PBMC2_REQ_CONTROLLER_OBJECT)Bmc2CreateReqController
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBmc2ReqController )
        {
            pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_resources;

            returnStatus = ANSC_STATUS_RESOURCES;

            if ( bCacheCookedPage && pBwrmCookedPage )
            {
                pBwrmCookedPage->DecRefCount((ANSC_HANDLE)pBwrmCookedPage);
            }
            else if ( !bCacheCookedPage && pBwrmCookedPage )
            {
                pBwrmCookedPage->Remove((ANSC_HANDLE)pBwrmCookedPage);
            }

            goto  EXIT1;
        }
        else
        {
            pBmc2ReqController->SetBmc2ComDomain    ((ANSC_HANDLE)pBmc2ReqController, (ANSC_HANDLE)pBmc2ComDomain      );
            pBmc2ReqController->SetBmc2EnvController((ANSC_HANDLE)pBmc2ReqController, (ANSC_HANDLE)pMyObject           );
            pBmc2ReqController->SetCommandRequest   ((ANSC_HANDLE)pBmc2ReqController, (ANSC_HANDLE)pBmc2CommandReq     );
            pBmc2ReqController->SetCommandReply     ((ANSC_HANDLE)pBmc2ReqController, (ANSC_HANDLE)pBmc2CommandRep     );
            pBmc2ReqController->SetCommandProperty  ((ANSC_HANDLE)pBmc2ReqController, (ANSC_HANDLE)pBmc2CommandProperty);

            while (( pArgumentToken = AnscTcUnlinkToken(pCliTokenChain) ))
            {
                pBmc2ReqController->AddCliArgument((ANSC_HANDLE)pBmc2ReqController, pArgumentToken->Name);

                AnscFreeMemory(pArgumentToken);
            }

            pBmc2ReqController->Engage((ANSC_HANDLE)pBmc2ReqController);
        }

        pBmc2PecIf = (PBMC2_PEC_INTERFACE)pBmc2ReqController->GetBmc2PecIf((ANSC_HANDLE)pBmc2ReqController);
    }

    if ( TRUE )
    {
        pBmc2CommandRep->bStateChanged = FALSE;
        pBmc2CommandRep->MenuTitle     = NULL;
        pBmc2CommandRep->StateCode     = BMC2_TERMINAL_STATE_noChange;
        pBmc2CommandRep->ErrorCode     = BMC2_TERMINAL_ERROR_noError;
    }

    pBmc2ComDomain->SetBmc2ReqController((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2ReqController);
    pBmc2ComTerminal->RelAccess((ANSC_HANDLE)pBmc2ComTerminal);

    returnStatus =
        pBmc2ComExecutor->ExecutePage
            (
                (ANSC_HANDLE)pBmc2ComExecutor,
                (ANSC_HANDLE)pBwrmCookedPage,
                (ANSC_HANDLE)pBmc2PecIf
            );

    pBmc2ComTerminal->AcqAccess((ANSC_HANDLE)pBmc2ComTerminal);
    pBmc2ComDomain->SetBmc2ReqController((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)NULL);

    /*
     * During command execution new domain may have been created and old domain may have been exit.
     * Instead of adding/removing the domain object when executing the command, we do it afterwards
     * to avoid any synchronization issues.
     */
    if ( pBmc2CommandRep->bStateChanged )
    {
        switch ( pBmc2CommandRep->StateCode )
        {
            case    BMC2_TERMINAL_STATE_enterSubMenu :

                    if ( pBmc2CommandRep->hReserved )
                    {
                        pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT)pBmc2CommandRep->hReserved;

                        /*
                        pBmc2ComDomain->Engage((ANSC_HANDLE)pBmc2ComDomain);

                        pBmc2ComTerminal->AddComDomain((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pBmc2ComDomain);
                        */
                    }

                    break;

            case    BMC2_TERMINAL_STATE_leaveSubMenu :

                    pBmc2ComTerminal->DelCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);

                    /*
                     * We need to restore the Menu Title of the parent domain...
                     */
                    pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT)pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);

                    if ( pBmc2ComDomain )
                    {
                        if ( pBmc2ComTerminal->GetDomainCount((ANSC_HANDLE)pBmc2ComTerminal) == 1 )
                        {
                            /*
                             * The Menu Title for the root domain should be hidden...
                             */
                            pBmc2CommandRep->MenuTitle = NULL;
                        }
                        else
                        {
                            pBmc2CommandRep->MenuTitle = AnscCloneString(pBmc2ComDomain->DomainTitle);
                        }
                    }

                    break;

            case    BMC2_TERMINAL_STATE_exitToRoot :

                    while ( pBmc2ComTerminal->GetDomainCount((ANSC_HANDLE)pBmc2ComTerminal) > 1 )
                    {
                        pBmc2ComTerminal->DelCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
                    }

                    /*
                     * We need to restore the Menu Title of the parent domain...
                     */
                    pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT)pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);

                    if ( pBmc2ComDomain )
                    {
                        if ( pBmc2ComTerminal->GetDomainCount((ANSC_HANDLE)pBmc2ComTerminal) == 1 )
                        {
                            /*
                             * The Menu Title for the root domain should be hidden...
                             */
                            pBmc2CommandRep->MenuTitle = NULL;
                        }
                        else
                        {
                            pBmc2CommandRep->MenuTitle = AnscCloneString(pBmc2ComDomain->DomainTitle);
                        }
                    }

                    break;

            case    BMC2_TERMINAL_STATE_exitTerminal :

                    pBmc2ComTerminal->DelCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);

                    break;

            default :

                    break;
        }

        if ( pBmc2ComTerminal->GetDomainCount((ANSC_HANDLE)pBmc2ComTerminal) == 0 )
        {
            pBmc2CommandRep->StateCode = BMC2_TERMINAL_STATE_exitTerminal;
        }
    }

    if ( TRUE )
    {
        pBmc2ReqController->Cancel((ANSC_HANDLE)pBmc2ReqController);
        pBmc2ReqController->Remove((ANSC_HANDLE)pBmc2ReqController);
    }

    if ( bCacheCookedPage && pBwrmCookedPage )
    {
        pBwrmCookedPage->DecRefCount((ANSC_HANDLE)pBwrmCookedPage);
    }
    else if ( !bCacheCookedPage && pBwrmCookedPage )
    {
        pBwrmCookedPage->Remove((ANSC_HANDLE)pBwrmCookedPage);
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pPageBuffer )
    {
        AnscFreeMemory(pPageBuffer);
    }

EXIT1:

    if ( pCliTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pCliTokenChain);
    }

    pBmc2ComTerminal->RelAccess((ANSC_HANDLE)pBmc2ComTerminal);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccAbortExecution
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTerminal
            );

    description:

        This function is called to abort the execution.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTerminal
                Specifies the current terminal environment.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSccAbortExecution
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT      )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT  )NULL;

    pBmc2ComTerminal->AcqAccess((ANSC_HANDLE)pBmc2ComTerminal);

    if ( pBmc2ComDomain )
    {
        pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pBmc2ComDomain->GetBmc2ReqController((ANSC_HANDLE)pBmc2ComDomain);

        if ( pBmc2ReqController )
        {
            pBmc2ReqController->Abort((ANSC_HANDLE)pBmc2ReqController);
        }
    }

    pBmc2ComTerminal->RelAccess((ANSC_HANDLE)pBmc2ComTerminal);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSccGetCommandSyntax
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand
            );

    description:

        This function is called to get help syntax of the given
        command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command.

    return:     help syntax of the specified command, NULL
                if the command is not valid.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoSccGetCommandSyntax
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT    )hTerminal;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT      )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_COMMAND_PROPERTY          pCommandProperty   = (PBMC2_COMMAND_PROPERTY       )NULL;

    if ( !pBmc2ComDomain )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pCommandProperty =
        (PBMC2_COMMAND_PROPERTY)pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand
            );

    return  (ANSC_HANDLE)(pCommandProperty ? pCommandProperty->pHelpSyntax : NULL);
}


