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

    module:	bmc2_reqco_operation.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoEngage
        *   Bmc2ReqcoCancel
        *   Bmc2ReqcoSetupEnv
        *   Bmc2ReqcoCloseEnv
        *   Bmc2ReqcoAsyncJobTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#include "bmc2_reqco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    pMyObject->bActive = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject              = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController     = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain         = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal       = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PSLAP_GOA_INTERFACE             pSlapGoaIf             = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor         = (PSLAP_OBJECT_DESCRIPTOR    )NULL;
    ANSC_HANDLE                     hSlapContainerApp      = (ANSC_HANDLE                )pBmc2EnvController->hSlapContainerApp;
    ANSC_HANDLE                     hSlapContainerServer   = (ANSC_HANDLE                )pBmc2EnvController->hSlapContainerApp;
    ANSC_HANDLE                     hSlapContainerTerminal = (ANSC_HANDLE                )pBmc2ComTerminal->hSlapContainerTerminal;
    ANSC_HANDLE                     hSlapContainerDomain   = (ANSC_HANDLE                )pBmc2ComDomain->hSlapContainerDomain;
    ANSC_HANDLE                     hSlapContainerReq      = (ANSC_HANDLE                )pMyObject->hSlapContainerReq;
    ANSC_HANDLE                     hSlapBmc2App           = (ANSC_HANDLE                )pBmc2EnvController->hSlapBmc2App;
    ANSC_HANDLE                     hSlapBmc2Server        = (ANSC_HANDLE                )pBmc2EnvController->hSlapBmc2Server;
    ANSC_HANDLE                     hSlapBmc2Terminal      = (ANSC_HANDLE                )pBmc2ComTerminal->hSlapBmc2Terminal;
    ANSC_HANDLE                     hSlapBmc2Domain        = (ANSC_HANDLE                )pBmc2ComDomain->hSlapBmc2Domain;
    ANSC_HANDLE                     hSlapBmc2Command       = (ANSC_HANDLE                )pMyObject->hSlapBmc2Command;
    ANSC_HANDLE                     hSlapBmc2Input         = (ANSC_HANDLE                )pMyObject->hSlapBmc2Input;
    ANSC_HANDLE                     hSlapBmc2Output        = (ANSC_HANDLE                )pMyObject->hSlapBmc2Output;

    if ( TRUE )
    {
        pMyObject->hSlapContainerReq =
            pSlapGoaIf->CreateContainer
                (
                    pSlapGoaIf->hOwnerContext,
                    BMC2_SLAP_CONTAINER_REQUEST,
                    SLAP_CONTAINER_TYPE_STANDARD
                );

        if ( !pMyObject->hSlapContainerReq )
        {
            return  ANSC_STATUS_FAILURE;
        }
        else
        {
            hSlapContainerReq = pMyObject->hSlapContainerReq;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor              = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2ComoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pMyObject->hSlapBmc2Command =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerReq,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );

        if ( !pMyObject->hSlapBmc2Command )
        {
            return  ANSC_STATUS_FAILURE;
        }
        else
        {
            hSlapBmc2Command = pMyObject->hSlapBmc2Command;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor            = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2InoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pMyObject->hSlapBmc2Input =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerReq,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );

        if ( !pMyObject->hSlapBmc2Input )
        {
            return  ANSC_STATUS_FAILURE;
        }
        else
        {
            hSlapBmc2Input = pMyObject->hSlapBmc2Input;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor             = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2OutoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pMyObject->hSlapBmc2Output =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerReq,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );

        if ( !pMyObject->hSlapBmc2Output )
        {
            return  ANSC_STATUS_FAILURE;
        }
        else
        {
            hSlapBmc2Output = pMyObject->hSlapBmc2Output;
        }
    }

    /*
     * The Bmc2 Req Controller serves as a transition layer between the script-based page execution
     * engines and the underlying SLAP environment. Req Controller captures every object-access
     * attempt and cache the object reference internally. This approach allows us to present a more
     * user-friendly object handling interface to the engines and facilitate garbage collection.
     */
    if ( TRUE )
    {
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Application", hSlapContainerApp,      hSlapBmc2App,      0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Server",      hSlapContainerServer,   hSlapBmc2Server,   0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Terminal",    hSlapContainerTerminal, hSlapBmc2Terminal, 0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Domain",      hSlapContainerDomain,   hSlapBmc2Domain,   0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Command",     hSlapContainerReq,      hSlapBmc2Command,  0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Input",       hSlapContainerReq,      hSlapBmc2Input,    0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Output",      hSlapContainerReq,      hSlapBmc2Output,   0);
    }

    if ( TRUE )
    {
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Application", hSlapContainerApp,      hSlapBmc2App,      0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Server",      hSlapContainerServer,   hSlapBmc2Server,   0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Terminal",    hSlapContainerTerminal, hSlapBmc2Terminal, 0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Domain",      hSlapContainerDomain,   hSlapBmc2Domain,   0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Command",     hSlapContainerReq,      hSlapBmc2Command,  0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Input",       hSlapContainerReq,      hSlapBmc2Input,    0);
        pMyObject->AddObjReference2((ANSC_HANDLE)pMyObject, "Services.Bmc2.Output",      hSlapContainerReq,      hSlapBmc2Output,   0);
    }

    if ( TRUE )
    {
        AnscAddTaskStorageUnit("Bmc2ReqController", (ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);

    pMyObject->ClearOroTable((ANSC_HANDLE)pMyObject);

    if ( TRUE )
    {
        pSlapGoaIf->DeleteContainer
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapContainerReq
            );
    }

    if ( TRUE )
    {
        AnscDelTaskStorageUnit("Bmc2ReqController");
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAsyncJobTask
            (
                ANSC_HANDLE                 hAsyncJob
            );

    description:

        This function is called to process an async job.

    argument:   ANSC_HANDLE                 hAsyncJob
                This handle is actually the pointer of the async job
                object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAsyncJobTask
    (
        ANSC_HANDLE                 hAsyncJob
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_REQCO_ASYNC_JOB           pAsyncJob          = (PBMC2_REQCO_ASYNC_JOB      )hAsyncJob;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)pAsyncJob->hThisObject;
    PANSC_EVENT                     pAsyncEvent        = (PANSC_EVENT                )pAsyncJob->hAsyncEvent;
    PBMC2_COMMAND_REPLY             pBmc2CommandRep    = (PBMC2_COMMAND_REPLY        )pMyObject->hCommandReply;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pBmc2EnvController->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pBmc2EnvController->hBwrmEnvController;
    PBWRM_RAM_INTERFACE             pBwrmRamIf         = (PBWRM_RAM_INTERFACE        )pBwrmEnvController->GetBwrmRamIf((ANSC_HANDLE)pBwrmEnvController);
    char*                           pRootPath          = (char*                      )pBmc2EnvController->Property.RootPath;
    char*                           pPagePath          = (char*                      )NULL;
    PBWRM_COOKED_PAGE_OBJECT        pBwrmCookedPage    = (PBWRM_COOKED_PAGE_OBJECT   )NULL;
    void*                           pPageBuffer        = (void*                      )NULL;
    ULONG                           ulPageSize         = (ULONG                      )0;
    BOOL                            bCacheCookedPage   = (BOOL                       )TRUE;

    if ( TRUE )
    {
        AnscAddTaskStorageUnit("Bmc2ReqController", (ANSC_HANDLE)pMyObject);
    }

    switch ( pAsyncJob->CommandId )
    {
        case    BMC2_REQCO_COMMAND_transfer :
        case    BMC2_REQCO_COMMAND_execute  :

                if ( !pAsyncJob->ParamList || (pAsyncJob->ParamList->ParamCount == 0) )
                {
                    returnStatus = ANSC_STATUS_UNAPPLICABLE;

                    break;
                }
                else if ( (pAsyncJob->ParamList->ParamArray[0].Syntax            != SLAP_VAR_SYNTAX_string) ||
                          (pAsyncJob->ParamList->ParamArray[0].Variant.varString == NULL                  ) )
                {
                    returnStatus = ANSC_STATUS_UNAPPLICABLE;

                    break;
                }
                else
                {
                    pPagePath = pAsyncJob->ParamList->ParamArray[0].Variant.varString;
                }

                returnStatus =
                    pBwrmRamIf->GetCookedPage
                        (
                            pBwrmRamIf->hOwnerContext,
                            pRootPath,
                            pPagePath,
                            (ANSC_HANDLE)&pBwrmCookedPage
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    returnStatus =
                        pBwrmRamIf->GetRawPage
                            (
                                pBwrmRamIf->hOwnerContext,
                                pRootPath,
                                pPagePath,
                                &pPageBuffer,
                                &ulPageSize
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_pageNotFound;

                        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                        break;
                    }
                    else
                    {
                        pBwrmCookedPage =
                            (PBWRM_COOKED_PAGE_OBJECT)pBmc2ComExecutor->PreparePage
                                (
                                    (ANSC_HANDLE)pBmc2ComExecutor,
                                    pPagePath,
                                    pPageBuffer,
                                    ulPageSize
                                );
                    }

                    if ( !pBwrmCookedPage )
                    {
                        pBmc2CommandRep->ErrorCode = BMC2_TERMINAL_ERROR_internalError;

                        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

                        if ( pPageBuffer )
                        {
                            AnscFreeMemory(pPageBuffer);
                        }

                        break;
                    }
                    else if ( pBmc2EnvController->Property.bCacheScpPages || pBwrmRamIf->IsPageCacheForced(pBwrmRamIf->hOwnerContext, pPagePath) )
                    {
                        bCacheCookedPage = TRUE;
                        returnStatus     =
                            pBwrmRamIf->AddCookedPage
                                (
                                    pBwrmRamIf->hOwnerContext,
                                    pRootPath,
                                    pPagePath,
                                    (ANSC_HANDLE)pBwrmCookedPage
                                );
                    }
                    else
                    {
                        bCacheCookedPage = FALSE;

                        pBwrmCookedPage->SetRootPath ((ANSC_HANDLE)pBwrmCookedPage, pRootPath             );
                        pBwrmCookedPage->SetPagePath ((ANSC_HANDLE)pBwrmCookedPage, pPagePath             );
                        pBwrmCookedPage->SetTimestamp((ANSC_HANDLE)pBwrmCookedPage, AnscGetTickInSeconds());
                    }
                }

                returnStatus =
                    pBmc2ComExecutor->ExecutePage
                        (
                            (ANSC_HANDLE)pBmc2ComExecutor,
                            (ANSC_HANDLE)pBwrmCookedPage,
                            pMyObject->hBmc2PecIf
                        );

                if ( bCacheCookedPage && pBwrmCookedPage )
                {
                    pBwrmCookedPage->DecRefCount((ANSC_HANDLE)pBwrmCookedPage);
                }
                else if ( !bCacheCookedPage && pBwrmCookedPage )
                {
                    pBwrmCookedPage->Remove((ANSC_HANDLE)pBwrmCookedPage);
                }

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    if ( pAsyncEvent )
    {
        AnscSetEvent(pAsyncEvent);
    }

    Bmc2ReqcoFreeAsyncJob(pAsyncJob);

    if ( TRUE )
    {
        AnscDelTaskStorageUnit("Bmc2ReqController");
    }

    return  returnStatus;
}
