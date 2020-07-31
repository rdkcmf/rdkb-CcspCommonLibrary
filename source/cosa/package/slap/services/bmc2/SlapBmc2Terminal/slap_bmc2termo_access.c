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

    module:	slap_bmc2termo_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Terminal Object.

        *   SlapBmc2TermoTerminate
        *   SlapBmc2TermoEnterDomain
        *   SlapBmc2TermoGetUsername
        *   SlapBmc2TermoSetUsername

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


#include "slap_bmc2termo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2TermoTerminate
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to terminate the terminal console.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2TermoTerminate
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pBmc2ComDomain? pBmc2ComDomain->hBmc2ReqController : NULL;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep    = (PBMC2_COMMAND_REPLY        )NULL;

    if ( !pBmc2ReqController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2CommandRep = (PBMC2_COMMAND_REPLY  )pBmc2ReqController->hCommandReply;
    }

    if ( !pBmc2CommandRep )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2CommandRep->bStateChanged = TRUE;
        pBmc2CommandRep->StateCode     = BMC2_TERMINAL_STATE_exitTerminal;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2TermoEnterDomain
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pDomainName
            );

    description:

        This function is called to enter a new domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pDomainName
                Specifies the name of the new domain to be entered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2TermoEnterDomain
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainName
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pBmc2ComTerminal->hBmc2EnvController;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ComTerminal->GetCurComDomain((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pBmc2ComDomain? pBmc2ComDomain->hBmc2ReqController : NULL;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep    = (PBMC2_COMMAND_REPLY        )NULL;

    if ( !pDomainName )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else if ( !pBmc2ReqController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2CommandRep = (PBMC2_COMMAND_REPLY  )pBmc2ReqController->hCommandReply;
    }

    if ( !pBmc2CommandRep )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pBmc2ComDomain =
            (PBMC2_COM_DOMAIN_OBJECT)Bmc2CreateComDomain
                (
                    pBmc2ComTerminal->hContainerContext,
                    (ANSC_HANDLE)pBmc2ComTerminal,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pBmc2ComDomain )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pBmc2ComDomain->SetBmc2ComTerminal  ((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2ComTerminal  );
        pBmc2ComDomain->SetBmc2EnvController((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2EnvController);
        pBmc2ComDomain->SetDomainTitle      ((ANSC_HANDLE)pBmc2ComDomain, pDomainName                    );
        pBmc2ComDomain->SetBmc2ReqController((ANSC_HANDLE)pBmc2ComDomain, (ANSC_HANDLE)pBmc2ReqController);
        pBmc2ComDomain->Engage              ((ANSC_HANDLE)pBmc2ComDomain);

        pBmc2ComTerminal->AddComDomain((ANSC_HANDLE)pBmc2ComTerminal, (ANSC_HANDLE)pBmc2ComDomain);
    }

    if ( TRUE )
    {
        pBmc2CommandRep->bStateChanged = TRUE;
        pBmc2CommandRep->MenuTitle     = AnscCloneString(pDomainName);
        pBmc2CommandRep->StateCode     = BMC2_TERMINAL_STATE_enterSubMenu;
        pBmc2CommandRep->hReserved     = (ANSC_HANDLE)pBmc2ComDomain;
    }

    if ( TRUE )
    {
        pBmc2ReqController->DelObjReference((ANSC_HANDLE)pBmc2ReqController, "Domain");

            pBmc2ReqController->AddObjReference2
                (
                    (ANSC_HANDLE)pBmc2ReqController,
                    "Domain",
                    pBmc2ComDomain->hSlapContainerDomain,
                    pBmc2ComDomain->hSlapBmc2Domain,
                    0
                );
    }

    if ( TRUE )
    {
        pBmc2ReqController->DelObjReference((ANSC_HANDLE)pBmc2ReqController, "Services.Bmc2.Domain");

            pBmc2ReqController->AddObjReference2
                (
                    (ANSC_HANDLE)pBmc2ReqController,
                    "Services.Bmc2.Domain",
                    pBmc2ComDomain->hSlapContainerDomain,
                    pBmc2ComDomain->hSlapBmc2Domain,
                    0
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapBmc2TermoGetUsername
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the username of this
        terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     user name.

**********************************************************************/

char*
SlapBmc2TermoGetUsername
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;

    return  AnscCloneString(pBmc2ComTerminal->GetUserIdentifier((ANSC_HANDLE)pBmc2ComTerminal));
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2TermoSetUsername
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pUsername
            );

    description:

        This function is called to get the username of this
        terminal.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pUsername
                Specify user name.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2TermoSetUsername
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUsername
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;

    return  pBmc2ComTerminal->SetUserIdentifier((ANSC_HANDLE)pBmc2ComTerminal, pUsername);
}

