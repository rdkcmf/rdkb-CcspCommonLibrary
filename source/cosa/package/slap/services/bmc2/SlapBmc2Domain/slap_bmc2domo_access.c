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

    module:	slap_bmc2domo_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Domain Object.

        *   SlapBmc2DomoGetDomainName
        *   SlapBmc2DomoAddCommand
        *   SlapBmc2DomoSetCommandSyntax
        *   SlapBmc2DomoSetCommandSyntaxString
        *   SlapBmc2DomoDelCommand
        *   SlapBmc2DomoEnableCommand
        *   SlapBmc2DomoIsCommandValid
        *   SlapBmc2DomoIsCommandAuthorized
        *   SlapBmc2DomoExit
        *   SlapBmc2DomoExitToRoot

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/29/05    initial revision.

**********************************************************************/


#include "slap_bmc2domo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapBmc2DomoGetDomainName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the domain name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     domain name.

**********************************************************************/

char*
SlapBmc2DomoGetDomainName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;

    return  AnscCloneString(pBmc2ComDomain->DomainTitle);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoAddCommand
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand
            );

    description:

        This function is called to add a command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoAddCommand
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pBmc2ComDomain->hBmc2EnvController;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PBMC2_COMMAND_PROPERTY          pCommandPropertyE  = (PBMC2_COMMAND_PROPERTY     )NULL;
    PBMC2_COMMAND_PROPERTY          pCommandPropertyD  = (PBMC2_COMMAND_PROPERTY     )NULL;
    errno_t                         rc                 = -1;

    pCommandPropertyD = (PBMC2_COMMAND_PROPERTY)AnscAllocateMemory(sizeof(BMC2_COMMAND_PROPERTY));

    if ( !pCommandPropertyD )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pCommandPropertyD->CommandName = AnscCloneString(pCommand);
        pCommandPropertyD->ScpPageName = NULL;
        pCommandPropertyD->bReadOnly   = FALSE;
        pCommandPropertyD->bAuthorized = TRUE;
        pCommandPropertyD->Permissions = 0;
    }

    pCommandPropertyE =
        (PBMC2_COMMAND_PROPERTY)pBmc2EnvController->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2EnvController,
                pCommand,
                pBmc2ComDomain->DomainTitle
            );

    if ( pCommandPropertyE )
    {
        pCommandPropertyD->ScpPageName = AnscCloneString(pCommandPropertyE->ScpPageName);
        pCommandPropertyD->bReadOnly   = pCommandPropertyE->bReadOnly;
        pCommandPropertyD->bAuthorized = (pCommandPropertyE->Permissions & pBmc2ComTerminal->GetUserPermission((ANSC_HANDLE)pBmc2ComTerminal));
        pCommandPropertyD->Permissions = pCommandPropertyE->Permissions;
    }

    if ( !pCommandPropertyD->ScpPageName )
    {
        pCommandPropertyD->ScpPageName = (char*)AnscAllocateMemory(AnscSizeOfString(pCommandPropertyD->CommandName) + 16);

        if ( pCommandPropertyD->ScpPageName )
        {
            rc = sprintf_s
                (
                    pCommandPropertyD->ScpPageName,
                    (AnscSizeOfString(pCommandPropertyD->CommandName) + 16),
                    "%s.scp",
                    pCommandPropertyD->CommandName
                );
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }
    }

    returnStatus =
        pBmc2ComDomain->AddCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                (ANSC_HANDLE)pCommandPropertyD
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoSetCommandSyntax
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand,
                PSLAP_STRING_ARRAY          ReqArgs,  
                PSLAP_STRING_ARRAY          OptArgs,  
            );

    description:

        This function is called to set syntax of the given command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be added.

                PSLAP_STRING_ARRAY          ReqArgs
                Specifies the required arguments.

                PSLAP_STRING_ARRAY          OptArgs  
                Specifies the optional arguments.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoSetCommandSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        PSLAP_STRING_ARRAY          pSlapReqArgs,  
        PSLAP_STRING_ARRAY          pSlapOptArgs
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT)hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT )pMyObject->hInsContext;
    char**                          pReqArgs           = NULL;
    ULONG                           ulReqArgCount      = 0;
    char**                          pOptArgs           = NULL;
    ULONG                           ulOptArgCount      = 0;
    ULONG                           i;

    ulReqArgCount   = pSlapReqArgs->VarCount;
    ulOptArgCount   = pSlapOptArgs->VarCount;

    if ( ulReqArgCount != 0 )
    {
        pReqArgs = (char**)AnscAllocateMemory(sizeof(char*) * ulReqArgCount);

        if ( !pReqArgs )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        for ( i = 0; i < ulReqArgCount; i ++ )
        {
            pReqArgs[i] = pSlapReqArgs->Array.arrayString[i];
        }
    }

    if ( ulOptArgCount != 0 )
    {
        pOptArgs = (char**)AnscAllocateMemory(sizeof(char*) * ulOptArgCount);

        if ( !pOptArgs )
        {
            if ( pReqArgs )
            {
                AnscFreeMemory(pReqArgs);
            }

            return  ANSC_STATUS_RESOURCES;
        }

        for ( i = 0; i < ulOptArgCount; i ++ )
        {
            pOptArgs[i] = pSlapOptArgs->Array.arrayString[i];
        }
    }

    returnStatus = 
        pBmc2ComDomain->SetCommandHelpSyntax
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand,
                ulReqArgCount,
                pReqArgs,
                ulOptArgCount,
                pOptArgs
            );

    if ( pReqArgs )
    {
        AnscFreeMemory(pReqArgs);
    }

    if ( pOptArgs )
    {
        AnscFreeMemory(pOptArgs);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoDelCommand
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand
            );

    description:

        This function is called to remove a command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoDelCommand
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;

    returnStatus =
        pBmc2ComDomain->DelCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoEnableCommand
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand,
                BOOL                        bEnable
            );

    description:

        This function is called to add a command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be added.

                BOOL                        bEnable
                Enable or disable the given command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoEnableCommand
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_COMMAND_PROPERTY          pBmc2CmdProperty   = (PBMC2_COMMAND_PROPERTY )NULL;

    pBmc2CmdProperty =
        pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand
            );

    if ( pBmc2CmdProperty )
    {
        pBmc2CmdProperty->bDisabled = !bEnable;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoSetCommandSyntaxString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand,
                char*                       pReqArgsSyntax,
                char*                       pOptArgsSyntax
            );

    description:

        This function is called to set syntax of the given command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be added.

                char*                       pReqArgsSyntax
                Specifies the required arguments.

                char*                       pOptArgsSyntax
                Specifies the optional arguments.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoSetCommandSyntaxString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        char*                       pReqArgsSyntax,
        char*                       pOptArgsSyntax
    )
{
    ANSC_STATUS                     returnStatus                            = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject                               = (PSLAP_BMC2_DOMAIN_OBJECT)hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain                          = (PBMC2_COM_DOMAIN_OBJECT )pMyObject->hInsContext;
    char*                           pReqArgs[BMC2_CMD_REQ_ARG_MAX_COUNT]    = {0};
    ULONG                           ulReqArgCount                           = 0;
    char*                           pOptArgs[BMC2_CMD_OPT_ARG_MAX_COUNT]    = {0};
    ULONG                           ulOptArgCount                           = 0;
    char*                           pArgDesc;
    char*                           pNext;

    pArgDesc = pReqArgsSyntax;
    while ( pArgDesc && *pArgDesc != 0 )
    {
        pNext = _ansc_strchr(pArgDesc, BMC2_CMD_ARG_SYNTAX_STRING_SEPARATOR);

        if ( pNext ) 
        {
            *pNext = 0;
        }
        pReqArgs[ulReqArgCount ++] = pArgDesc;

        if ( pNext )
        {
            pArgDesc = pNext + 1;
        }
        else
        {
            pArgDesc = NULL;
        }

        if ( pArgDesc && ulReqArgCount >= BMC2_CMD_REQ_ARG_MAX_COUNT )
        {
            returnStatus = ANSC_STATUS_BAD_PARAMETER;
            goto EXIT;
        }
    }

    pArgDesc = pOptArgsSyntax;
    while ( pArgDesc && *pArgDesc != 0 )
    {
        pNext = _ansc_strchr(pArgDesc, BMC2_CMD_ARG_SYNTAX_STRING_SEPARATOR);

        if ( pNext ) 
        {
            *pNext = 0;
        }
        pOptArgs[ulOptArgCount ++] = pArgDesc;

        if ( pNext )
        {
            pArgDesc = pNext + 1;
        }
        else
        {
            pArgDesc = NULL;
        }

        if ( pArgDesc && ulOptArgCount >= BMC2_CMD_OPT_ARG_MAX_COUNT )
        {
            returnStatus = ANSC_STATUS_BAD_PARAMETER;
            goto EXIT;
        }
    }

    returnStatus = 
        pBmc2ComDomain->SetCommandHelpSyntax
            (
                (ANSC_HANDLE)pBmc2ComDomain,
                pCommand,
                ulReqArgCount,
                pReqArgs,
                ulOptArgCount,
                pOptArgs
            );

EXIT:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_BOOL
        SlapBmc2DomoIsCommandValid
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand
            );

    description:

        This function is called to check whether a command is valid.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be checked.

    return:     TRUE or FALSE.

**********************************************************************/

SLAP_BOOL
SlapBmc2DomoIsCommandValid
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand
    )
{
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pBmc2ComDomain->hBmc2EnvController;
    PBMC2_COMMAND_PROPERTY          pCommandPropertyD  = (PBMC2_COMMAND_PROPERTY     )NULL;

    pCommandPropertyD =
        (PBMC2_COMMAND_PROPERTY)pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2EnvController,
                pCommand
            );

    if ( !pCommandPropertyD )
    {
        return  SLAP_FALSE;
    }

    return  SLAP_TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_BOOL
        SlapBmc2DomoIsCommandAuthorized
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCommand
            );

    description:

        This function is called to check whether a command is
        authorized.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCommand
                Specifies the command name to be checked.

    return:     TRUE or FALSE.

**********************************************************************/

SLAP_BOOL
SlapBmc2DomoIsCommandAuthorized
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand
    )
{
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pBmc2ComDomain->hBmc2EnvController;
    PBMC2_COMMAND_PROPERTY          pCommandPropertyD  = (PBMC2_COMMAND_PROPERTY     )NULL;

    pCommandPropertyD =
        (PBMC2_COMMAND_PROPERTY)pBmc2ComDomain->GetCommandProperty
            (
                (ANSC_HANDLE)pBmc2EnvController,
                pCommand
            );

    if ( !pCommandPropertyD )
    {
        return  SLAP_FALSE;
    }

    return  pCommandPropertyD->bAuthorized? SLAP_TRUE : SLAP_FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoExit
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to exit this domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoExit
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pBmc2ComDomain->hBmc2ReqController;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep    = (PBMC2_COMMAND_REPLY        )pBmc2ReqController->hCommandReply;

    if ( !pBmc2CommandRep )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2CommandRep->bStateChanged = TRUE;
        pBmc2CommandRep->StateCode     = BMC2_TERMINAL_STATE_leaveSubMenu;
        pBmc2CommandRep->hReserved     = (ANSC_HANDLE)pBmc2ComDomain;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2DomoExitToRoot
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to exit this domain and all the way
        back to the root.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2DomoExitToRoot
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_DOMAIN_OBJECT        pMyObject          = (PSLAP_BMC2_DOMAIN_OBJECT   )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hInsContext;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pBmc2ComDomain->hBmc2ReqController;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep    = (PBMC2_COMMAND_REPLY        )pBmc2ReqController->hCommandReply;

    if ( !pBmc2CommandRep )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2CommandRep->bStateChanged = TRUE;
        pBmc2CommandRep->StateCode     = BMC2_TERMINAL_STATE_exitToRoot;
        pBmc2CommandRep->hReserved     = (ANSC_HANDLE)pBmc2ComDomain;
    }

    return  ANSC_STATUS_SUCCESS;
}
