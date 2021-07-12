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

    module:	bmc2_reqco_base.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoCreate
        *   Bmc2ReqcoRemove
        *   Bmc2ReqcoEnrollObjects
        *   Bmc2ReqcoInitialize

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bmc2 Req Controller Object and
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
Bmc2ReqcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBMC2_REQ_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(BMC2_REQ_CONTROLLER_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BMC2_REQ_CONTROLLER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BMC2_REQ_CONTROLLER_OID;
    pBaseObject->Create            = Bmc2ReqcoCreate;
    pBaseObject->Remove            = Bmc2ReqcoRemove;
    pBaseObject->EnrollObjects     = Bmc2ReqcoEnrollObjects;
    pBaseObject->Initialize        = Bmc2ReqcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoRemove
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
Bmc2ReqcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_PEC_INTERFACE             pBmc2PecIf   = (PBMC2_PEC_INTERFACE        )pMyObject->hBmc2PecIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pBmc2PecIf )
    {
        AnscFreeMemory(pBmc2PecIf);

        pMyObject->hBmc2PecIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->CcoQueueLock);
    AnscFreeLock(&pMyObject->OroTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoEnrollObjects
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
Bmc2ReqcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_PEC_INTERFACE             pBmc2PecIf   = (PBMC2_PEC_INTERFACE        )pMyObject->hBmc2PecIf;
    errno_t  rc  = -1;

    if ( !pBmc2PecIf )
    {
        pBmc2PecIf = (PBMC2_PEC_INTERFACE)AnscAllocateMemory(sizeof(BMC2_PEC_INTERFACE));

        if ( !pBmc2PecIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmc2PecIf = (ANSC_HANDLE)pBmc2PecIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBmc2PecIf->Name, sizeof(pBmc2PecIf->Name), BMC2_PEC_INTERFACE_NAME);
        ERR_CHK(rc);

        pBmc2PecIf->InterfaceId    = BMC2_PEC_INTERFACE_ID;
        pBmc2PecIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pBmc2PecIf->Size           = sizeof(BMC2_PEC_INTERFACE);

        pBmc2PecIf->GetCommand     = Bmc2ReqcoPecGetCommand;
        pBmc2PecIf->GetInput       = Bmc2ReqcoPecGetInput;
        pBmc2PecIf->GetOutput      = Bmc2ReqcoPecGetOutput;
        pBmc2PecIf->GetDomain      = Bmc2ReqcoPecGetDomain;
        pBmc2PecIf->GetTerminal    = Bmc2ReqcoPecGetTerminal;
        pBmc2PecIf->GetApplication = Bmc2ReqcoPecGetApplication;
        pBmc2PecIf->GetServer      = Bmc2ReqcoPecGetServer;

        pBmc2PecIf->GetBmc2Object  = Bmc2ReqcoPecGetBmc2Object;
        pBmc2PecIf->GetCookedPage  = Bmc2ReqcoPecGetCookedPage;
        pBmc2PecIf->RetCookedPage  = Bmc2ReqcoPecRetCookedPage;
        pBmc2PecIf->InvokeObject   = Bmc2ReqcoPecInvokeObject;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoInitialize
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
Bmc2ReqcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
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
    pMyObject->Oid                  = BMC2_REQ_CONTROLLER_OID;
    pMyObject->Create               = Bmc2ReqcoCreate;
    pMyObject->Remove               = Bmc2ReqcoRemove;
    pMyObject->EnrollObjects        = Bmc2ReqcoEnrollObjects;
    pMyObject->Initialize           = Bmc2ReqcoInitialize;

    pMyObject->hBmc2ComDomain       = (ANSC_HANDLE)NULL;
    pMyObject->hBmc2EnvController   = (ANSC_HANDLE)NULL;
    pMyObject->hCommandRequest      = (ANSC_HANDLE)NULL;
    pMyObject->hCommandReply        = (ANSC_HANDLE)NULL;
    pMyObject->hCommandProperty     = (ANSC_HANDLE)NULL;
    pMyObject->CliArgumentCount     = 0;
    pMyObject->bAborted             = FALSE;
    pMyObject->bActive              = FALSE;

    pMyObject->hSlapContainerReq    = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Command     = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Input       = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Output      = (ANSC_HANDLE)NULL;

    pMyObject->GetBmc2PecIf         = Bmc2ReqcoGetBmc2PecIf;
    pMyObject->GetBmc2ComDomain     = Bmc2ReqcoGetBmc2ComDomain;
    pMyObject->SetBmc2ComDomain     = Bmc2ReqcoSetBmc2ComDomain;
    pMyObject->GetBmc2EnvController = Bmc2ReqcoGetBmc2EnvController;
    pMyObject->SetBmc2EnvController = Bmc2ReqcoSetBmc2EnvController;
    pMyObject->GetCommandRequest    = Bmc2ReqcoGetCommandRequest;
    pMyObject->SetCommandRequest    = Bmc2ReqcoSetCommandRequest;
    pMyObject->GetCommandReply      = Bmc2ReqcoGetCommandReply;
    pMyObject->SetCommandReply      = Bmc2ReqcoSetCommandReply;
    pMyObject->GetCommandProperty   = Bmc2ReqcoGetCommandProperty;
    pMyObject->SetCommandProperty   = Bmc2ReqcoSetCommandProperty;
    pMyObject->GetCliArgumentCount  = Bmc2ReqcoGetCliArgumentCount;
    pMyObject->GetCliArgument       = Bmc2ReqcoGetCliArgument;
    pMyObject->AddCliArgument       = Bmc2ReqcoAddCliArgument;
    pMyObject->RemoveCliArguments   = Bmc2ReqcoRemoveCliArguments;
    pMyObject->Abort                = Bmc2ReqcoAbort;
    pMyObject->Reset                = Bmc2ReqcoReset;

    pMyObject->Engage               = Bmc2ReqcoEngage;
    pMyObject->Cancel               = Bmc2ReqcoCancel;
    pMyObject->SetupEnv             = Bmc2ReqcoSetupEnv;
    pMyObject->CloseEnv             = Bmc2ReqcoCloseEnv;
    pMyObject->AsyncJobTask         = Bmc2ReqcoAsyncJobTask;

    pMyObject->AddControlCommand    = Bmc2ReqcoAddControlCommand;
    pMyObject->AddControlCommand2   = Bmc2ReqcoAddControlCommand2;
    pMyObject->PopControlCommand    = Bmc2ReqcoPopControlCommand;
    pMyObject->ClearCcoQueue        = Bmc2ReqcoClearCcoQueue;
    pMyObject->GetObjReference      = Bmc2ReqcoGetObjReference;
    pMyObject->AddObjReference      = Bmc2ReqcoAddObjReference;
    pMyObject->AddObjReference2     = Bmc2ReqcoAddObjReference2;
    pMyObject->DelObjReference      = Bmc2ReqcoDelObjReference;
    pMyObject->ClearOroTable        = Bmc2ReqcoClearOroTable;

    pMyObject->DispatchCommands     = Bmc2ReqcoDispatchCommands;
    pMyObject->DoReturn             = Bmc2ReqcoDoReturn;
    pMyObject->DoTransfer           = Bmc2ReqcoDoTransfer;
    pMyObject->DoExecute            = Bmc2ReqcoDoExecute;

    pMyObject->PecGetCommand        = Bmc2ReqcoPecGetCommand;
    pMyObject->PecGetInput          = Bmc2ReqcoPecGetInput;
    pMyObject->PecGetOutput         = Bmc2ReqcoPecGetOutput;
    pMyObject->PecGetDomain         = Bmc2ReqcoPecGetDomain;
    pMyObject->PecGetTerminal       = Bmc2ReqcoPecGetTerminal;
    pMyObject->PecGetApplication    = Bmc2ReqcoPecGetApplication;
    pMyObject->PecGetServer         = Bmc2ReqcoPecGetServer;

    pMyObject->PecGetBmc2Object     = Bmc2ReqcoPecGetBmc2Object;
    pMyObject->PecGetCookedPage     = Bmc2ReqcoPecGetCookedPage;
    pMyObject->PecRetCookedPage     = Bmc2ReqcoPecRetCookedPage;
    pMyObject->PecInvokeObject      = Bmc2ReqcoPecInvokeObject;

    for ( i = 0; i < BMC2_REQCO_MAX_ARGUMENT_NUMBER; i++ )
    {
        pMyObject->CliArgumentArray[i] = NULL;
    }

    AnscQueueInitializeHeader(&pMyObject->CcoQueue    );
    AnscInitializeLock       (&pMyObject->CcoQueueLock);

    for ( i = 0; i < BMC2_REQCO_ORO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->OroTable[i]);
    }
    AnscInitializeLock(&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}
