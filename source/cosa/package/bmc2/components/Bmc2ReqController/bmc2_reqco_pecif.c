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

    module:	bmc2_reqco_pecif.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoPecGetCommand
        *   Bmc2ReqcoPecGetInput
        *   Bmc2ReqcoPecGetOutput
        *   Bmc2ReqcoPecGetDomain
        *   Bmc2ReqcoPecGetTerminal
        *   Bmc2ReqcoPecGetApplication
        *   Bmc2ReqcoPecGetServer
        *   Bmc2ReqcoPecGetBmc2Object
        *   Bmc2ReqcoPecGetCookedPage
        *   Bmc2ReqcoPecRetCookedPage
        *   Bmc2ReqcoPecInvokeObject

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

        ANSC_HANDLE
        Bmc2ReqcoPecGetCommand
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetCommand
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hSlapBmc2Command;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetInput
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetInput
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hSlapBmc2Input;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetOutput
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetOutput
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hSlapBmc2Output;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetDomain
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetDomain
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hBmc2ComDomain;

    return  pBmc2ComDomain->hSlapBmc2Domain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetTerminal
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetTerminal
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pMyObject->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;

    return  pBmc2ComTerminal->hSlapBmc2Terminal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetApplication
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetApplication
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;

    return  pBmc2EnvController->hSlapBmc2App;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetServer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a BMC2 service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;

    return  pBmc2EnvController->hSlapBmc2Server;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetBmc2Object
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to retrieve a BMC2 service object by
        matching the object name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object name to be matched.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetBmc2Object
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject              = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController     = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf             = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    ANSC_HANDLE                     hSlapContainerServices = (ANSC_HANDLE                )pBmc2EnvController->hSlapContainerServices;
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference          = (PBMC2_REQCO_OBJ_REFERENCE  )NULL;
    ANSC_HANDLE                     hSlapObject            = (ANSC_HANDLE                )NULL;

    pObjReference =
        (PBMC2_REQCO_OBJ_REFERENCE)pMyObject->GetObjReference
            (
                (ANSC_HANDLE)pMyObject,
                obj_name
            );

    if ( pObjReference )
    {
        return  pObjReference->ObjHandle;
    }

    hSlapObject =
        pSlapGoaIf->AcqObjectAccess
            (
                pSlapGoaIf->hOwnerContext,
                hSlapContainerServices,
                obj_name
            );

    if ( hSlapObject )
    {
        pMyObject->AddObjReference2
            (
                (ANSC_HANDLE)pMyObject,
                obj_name,
                hSlapContainerServices,
                hSlapObject,
                1
            );
    }

    return  hSlapObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPecGetCookedPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to retrieve a BRWM Cooked Page Object
        by matching the page path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the web resource path to be matched.

    return:     page object.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pBmc2EnvController->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pBmc2EnvController->hBwrmEnvController;
    PBWRM_RAM_INTERFACE             pBwrmRamIf         = (PBWRM_RAM_INTERFACE        )pBwrmEnvController->GetBwrmRamIf((ANSC_HANDLE)pBwrmEnvController);
    char*                           pRootPath          = (char*                      )pBmc2EnvController->Property.RootPath;
    PBWRM_COOKED_PAGE_OBJECT        pBwrmCookedPage    = (PBWRM_COOKED_PAGE_OBJECT   )NULL;
    void*                           pPageBuffer        = (void*                      )NULL;
    ULONG                           ulPageSize         = (ULONG                      )0;

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
                pRootPath,
                page_path,
                (ANSC_HANDLE)&pBwrmCookedPage
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        returnStatus =
            pBwrmRamIf->GetRawPage
                (
                    pBwrmRamIf->hOwnerContext,
                    pRootPath,
                    page_path,
                    &pPageBuffer,
                    &ulPageSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pBwrmCookedPage =
                (PBWRM_COOKED_PAGE_OBJECT)pBmc2ComExecutor->PreparePage
                    (
                        (ANSC_HANDLE)pBmc2ComExecutor,
                        page_path,
                        pPageBuffer,
                        ulPageSize
                    );
        }

        if ( !pBwrmCookedPage )
        {
            AnscFreeMemory(pPageBuffer);

            return  (ANSC_HANDLE)NULL;
        }
        else if ( pBmc2EnvController->Property.bCacheScpPages || pBwrmRamIf->IsPageCacheForced(pBwrmRamIf->hOwnerContext, page_path) )
        {
            returnStatus =
                pBwrmRamIf->AddCookedPage
                    (
                        pBwrmRamIf->hOwnerContext,
                        pRootPath,
                        page_path,
                        (ANSC_HANDLE)pBwrmCookedPage
                    );
        }
        else
        {
            pBwrmCookedPage->SetRootPath ((ANSC_HANDLE)pBwrmCookedPage, pRootPath             );
            pBwrmCookedPage->SetPagePath ((ANSC_HANDLE)pBwrmCookedPage, page_path             );
            pBwrmCookedPage->SetTimestamp((ANSC_HANDLE)pBwrmCookedPage, AnscGetTickInSeconds());
        }
    }

    return  (ANSC_HANDLE)pBwrmCookedPage;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoPecRetCookedPage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCookedPage
            );

    description:

        This function is called to return a BRWM Cooked Page Object
        that was acquired previously by GetCookedPage().

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCookedPage
                Specifies the Cooked Page object to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoPecRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pBmc2EnvController->hBwrmEnvController;
    PBWRM_RAM_INTERFACE             pBwrmRamIf         = (PBWRM_RAM_INTERFACE        )pBwrmEnvController->GetBwrmRamIf((ANSC_HANDLE)pBwrmEnvController);
    PBWRM_COOKED_PAGE_OBJECT        pBwrmCookedPage    = (PBWRM_COOKED_PAGE_OBJECT   )hCookedPage;

    if ( !pBmc2EnvController->Property.bCacheScpPages && !pBwrmRamIf->IsPageCacheForced(pBwrmRamIf->hOwnerContext, pBwrmCookedPage->PagePath) )
    {
        pBwrmCookedPage->Remove((ANSC_HANDLE)pBwrmCookedPage);
    }
    else
    {
        pBwrmCookedPage->DecRefCount((ANSC_HANDLE)pBwrmCookedPage);
    }

    return   ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoPecInvokeObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmc2Object,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var,
                PULONG                      pulPecStatus
            );

    description:

        This function is called to invoke a method made available by
        the specified SLAP object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmc2Object
                Specifies the SLAP service object to be invoked.

                char*                       method_name
                Specifies the method name to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the input parameters to be passed into the
                method.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the output parameters to be returned by the
                method.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned by the method.

                PULONG                      pulPecStatus
                Specifies the control status to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoPecInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmc2Object,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulPecStatus
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);

    *pulPecStatus = BMC2_PEC_STATUS_continue;
    returnStatus  =
        pSlapGoaIf->InvokeObject
            (
                pSlapGoaIf->hOwnerContext,
                hBmc2Object,
                pMyObject->hSlapContainerReq,
                method_name,
                params_in,
                params_out,
                return_var
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        *pulPecStatus = BMC2_PEC_STATUS_abort;

        return  returnStatus;
    }

    returnStatus =
        pMyObject->DispatchCommands
            (
                (ANSC_HANDLE)pMyObject,
                pulPecStatus
            );

    return  returnStatus;
}
