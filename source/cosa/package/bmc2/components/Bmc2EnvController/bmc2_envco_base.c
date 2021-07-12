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

    module:	bmc2_envco_base.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bmc2 Env Controller Object.

        *   Bmc2EnvcoCreate
        *   Bmc2EnvcoRemove
        *   Bmc2EnvcoEnrollObjects
        *   Bmc2EnvcoInitialize

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

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bmc2 Env Controller Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = NULL;
    errno_t    rc   = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBMC2_ENV_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(BMC2_ENV_CONTROLLER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BMC2_ENV_CONTROLLER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BMC2_ENV_CONTROLLER_OID;
    pBaseObject->Create            = Bmc2EnvcoCreate;
    pBaseObject->Remove            = Bmc2EnvcoRemove;
    pBaseObject->EnrollObjects     = Bmc2EnvcoEnrollObjects;
    pBaseObject->Initialize        = Bmc2EnvcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject          = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PBMC2_SCC_INTERFACE             pBmc2SccIf         = (PBMC2_SCC_INTERFACE        )pMyObject->hBmc2SccIf;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pMyObject->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pMyObject->hBwrmEnvController;

    if ( pMyObject->pPseudoTermDevName )
    {
        AnscFreeMemory(pMyObject->pPseudoTermDevName);
        pMyObject->pPseudoTermDevName = NULL;
    }

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pBmc2SccIf )
    {
        AnscFreeMemory(pBmc2SccIf);

        pMyObject->hBmc2SccIf = (ANSC_HANDLE)NULL;
    }

    if ( pBmc2ComExecutor )
    {
        pBmc2ComExecutor->Remove((ANSC_HANDLE)pBmc2ComExecutor);

        pMyObject->hBmc2ComExecutor = (ANSC_HANDLE)NULL;
    }

    if ( pBwrmEnvController )
    {
        pBwrmEnvController->Remove((ANSC_HANDLE)pBwrmEnvController);

        pMyObject->hBwrmEnvController = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->ComtoQueueLock);
    AnscFreeLock(&pMyObject->CompoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject    );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject          = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PBMC2_SCC_INTERFACE             pBmc2SccIf         = (PBMC2_SCC_INTERFACE        )pMyObject->hBmc2SccIf;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pMyObject->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pMyObject->hBwrmEnvController;
    errno_t    rc = -1;

    if ( !pBmc2SccIf )
    {
        pBmc2SccIf = (PBMC2_SCC_INTERFACE)AnscAllocateMemory(sizeof(BMC2_SCC_INTERFACE));

        if ( !pBmc2SccIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmc2SccIf = (ANSC_HANDLE)pBmc2SccIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBmc2SccIf->Name, sizeof(pBmc2SccIf->Name), BMC2_SCC_INTERFACE_NAME);
        ERR_CHK(rc);

        pBmc2SccIf->InterfaceId      = BMC2_SCC_INTERFACE_ID;
        pBmc2SccIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pBmc2SccIf->Size             = sizeof(BMC2_SCC_INTERFACE);

        pBmc2SccIf->SetRootInfo      = Bmc2EnvcoSccSetRootInfo;
        pBmc2SccIf->SetMultiUserCtrl = Bmc2EnvcoSccSetMultiUserCtrl;
        pBmc2SccIf->AddCommand       = Bmc2EnvcoSccAddCommand;

        pBmc2SccIf->AddTerminal      = Bmc2EnvcoSccAddTerminal;
        pBmc2SccIf->DelTerminal      = Bmc2EnvcoSccDelTerminal;
        pBmc2SccIf->GetCommands      = Bmc2EnvcoSccGetCommands;
        pBmc2SccIf->IsCommandValid   = Bmc2EnvcoSccIsCommandValid;
        pBmc2SccIf->ExecuteCommand   = Bmc2EnvcoSccExecuteCommand;
        pBmc2SccIf->AbortExecution   = Bmc2EnvcoSccAbortExecution;
        pBmc2SccIf->GetCommandSyntax = Bmc2EnvcoSccGetCommandSyntax;
    }

    if ( !pBmc2ComExecutor )
    {
        pBmc2ComExecutor =
            (PBMC2_COM_EXECUTOR_OBJECT)Bmc2CreateComExecutor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBmc2ComExecutor )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmc2ComExecutor = (ANSC_HANDLE)pBmc2ComExecutor;
        }
    }

    if ( !pBwrmEnvController )
    {
        pBwrmEnvController =
            (PBWRM_ENV_CONTROLLER_OBJECT)BwrmCreateEnvController
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBwrmEnvController )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwrmEnvController = (ANSC_HANDLE)pBwrmEnvController;
        }
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    ULONG                           i            = 0;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                      = BMC2_ENV_CONTROLLER_OID;
    pMyObject->Create                   = Bmc2EnvcoCreate;
    pMyObject->Remove                   = Bmc2EnvcoRemove;
    pMyObject->EnrollObjects            = Bmc2EnvcoEnrollObjects;
    pMyObject->Initialize               = Bmc2EnvcoInitialize;

    pMyObject->hSlapGoaIf               = (ANSC_HANDLE)NULL;
    pMyObject->hSlapUoaIf               = (ANSC_HANDLE)NULL;
    pMyObject->bActive                  = FALSE;

    pMyObject->hSlapContainerApp        = (ANSC_HANDLE)NULL;
    pMyObject->hSlapContainerServices   = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2App             = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Server          = (ANSC_HANDLE)NULL;
    pMyObject->hSlapVarEntity           = (ANSC_HANDLE)NULL;
    pMyObject->hSlapVarConverter        = (ANSC_HANDLE)NULL;

    pMyObject->GetBmc2SccIf             = Bmc2EnvcoGetBmc2SccIf;
    pMyObject->GetBmc2ComExecutor       = Bmc2EnvcoGetBmc2ComExecutor;
    pMyObject->GetBwrmEnvController     = Bmc2EnvcoGetBwrmEnvController;
    pMyObject->GetBwrmRamIf             = Bmc2EnvcoGetBwrmRamIf;
    pMyObject->GetSlapGoaIf             = Bmc2EnvcoGetSlapGoaIf;
    pMyObject->SetSlapGoaIf             = Bmc2EnvcoSetSlapGoaIf;
    pMyObject->GetSlapUoaIf             = Bmc2EnvcoGetSlapUoaIf;
    pMyObject->SetSlapUoaIf             = Bmc2EnvcoSetSlapUoaIf;
    pMyObject->GetSlapContainerServices = Bmc2EnvcoGetSlapContainerServices;
    pMyObject->SetSlapContainerServices = Bmc2EnvcoSetSlapContainerServices;

    pMyObject->GetProperty              = Bmc2EnvcoGetProperty;
    pMyObject->SetProperty              = Bmc2EnvcoSetProperty;
    pMyObject->ResetProperty            = Bmc2EnvcoResetProperty;
    pMyObject->Reset                    = Bmc2EnvcoReset;

    pMyObject->Engage                   = Bmc2EnvcoEngage;
    pMyObject->Cancel                   = Bmc2EnvcoCancel;
    pMyObject->SetupEnv                 = Bmc2EnvcoSetupEnv;
    pMyObject->CloseEnv                 = Bmc2EnvcoCloseEnv;

    pMyObject->GetComTerminal           = Bmc2EnvcoGetComTerminal;
    pMyObject->AddComTerminal           = Bmc2EnvcoAddComTerminal;
    pMyObject->DelComTerminal           = Bmc2EnvcoDelComTerminal;
    pMyObject->DelAllComTerminals       = Bmc2EnvcoDelAllComTerminals;

    pMyObject->GetCommandProperty       = Bmc2EnvcoGetCommandProperty;
    pMyObject->AddCommandProperty       = Bmc2EnvcoAddCommandProperty;
    pMyObject->DelCommandProperty       = Bmc2EnvcoDelCommandProperty;
    pMyObject->DelAllCommandProperties  = Bmc2EnvcoDelAllCommandProperties;

    pMyObject->SccSetRootInfo           = Bmc2EnvcoSccSetRootInfo;
    pMyObject->SccSetMultiUserCtrl      = Bmc2EnvcoSccSetMultiUserCtrl;
    pMyObject->SccAddCommand            = Bmc2EnvcoSccAddCommand;

    pMyObject->SccAddTerminal           = Bmc2EnvcoSccAddTerminal;
    pMyObject->SccDelTerminal           = Bmc2EnvcoSccDelTerminal;
    pMyObject->SccGetCommands           = Bmc2EnvcoSccGetCommands;
    pMyObject->SccIsCommandValid        = Bmc2EnvcoSccIsCommandValid;
    pMyObject->SccExecuteCommand        = Bmc2EnvcoSccExecuteCommand;
    pMyObject->SccAbortExecution        = Bmc2EnvcoSccAbortExecution;
    pMyObject->SccGetCommandSyntax      = Bmc2EnvcoSccGetCommandSyntax;

    pMyObject->SetPseudoTermDevName     = Bmc2EnvcoSetPseudoTermDevName;

    AnscQueueInitializeHeader(&pMyObject->ComtoQueue    );
    AnscInitializeLock       (&pMyObject->ComtoQueueLock);

    for ( i = 0; i < BMC2_ENVCO_COMPO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->CompoTable[i]);
    }
    AnscInitializeLock(&pMyObject->CompoTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
