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

    module:	bmc2_comdo_base.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bmc2 Com Domain Object.

        *   Bmc2ComdoCreate
        *   Bmc2ComdoRemove
        *   Bmc2ComdoEnrollObjects
        *   Bmc2ComdoInitialize

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


#include "bmc2_comdo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        Bmc2ComdoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bmc2 Com Domain Object and
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
Bmc2ComdoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = NULL;
    errno_t  rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBMC2_COM_DOMAIN_OBJECT)AnscAllocateMemory(sizeof(BMC2_COM_DOMAIN_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BMC2_COM_DOMAIN_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BMC2_COM_DOMAIN_OID;
    pBaseObject->Create            = Bmc2ComdoCreate;
    pBaseObject->Remove            = Bmc2ComdoRemove;
    pBaseObject->EnrollObjects     = Bmc2ComdoEnrollObjects;
    pBaseObject->Initialize        = Bmc2ComdoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoRemove
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
Bmc2ComdoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->CompoQueueLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject    );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoEnrollObjects
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
Bmc2ComdoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoInitialize
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
Bmc2ComdoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

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
    pMyObject->Oid                     = BMC2_COM_DOMAIN_OID;
    pMyObject->Create                  = Bmc2ComdoCreate;
    pMyObject->Remove                  = Bmc2ComdoRemove;
    pMyObject->EnrollObjects           = Bmc2ComdoEnrollObjects;
    pMyObject->Initialize              = Bmc2ComdoInitialize;

    pMyObject->hBmc2ComTerminal        = (ANSC_HANDLE)NULL;
    pMyObject->hBmc2EnvController      = (ANSC_HANDLE)NULL;
    pMyObject->hBmc2ReqController      = (ANSC_HANDLE)NULL;
    pMyObject->DomainTitle             = NULL;
    pMyObject->bActive                 = FALSE;

    pMyObject->hSlapContainerDomain    = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Domain         = (ANSC_HANDLE)NULL;

    pMyObject->GetBmc2ComTerminal      = Bmc2ComdoGetBmc2ComTerminal;
    pMyObject->SetBmc2ComTerminal      = Bmc2ComdoSetBmc2ComTerminal;
    pMyObject->GetBmc2EnvController    = Bmc2ComdoGetBmc2EnvController;
    pMyObject->SetBmc2EnvController    = Bmc2ComdoSetBmc2EnvController;
    pMyObject->GetBmc2ReqController    = Bmc2ComdoGetBmc2ReqController;
    pMyObject->SetBmc2ReqController    = Bmc2ComdoSetBmc2ReqController;
    pMyObject->GetDomainTitle          = Bmc2ComdoGetDomainTitle;
    pMyObject->SetDomainTitle          = Bmc2ComdoSetDomainTitle;
    pMyObject->Reset                   = Bmc2ComdoReset;

    pMyObject->Engage                  = Bmc2ComdoEngage;
    pMyObject->Cancel                  = Bmc2ComdoCancel;
    pMyObject->SetupEnv                = Bmc2ComdoSetupEnv;
    pMyObject->CloseEnv                = Bmc2ComdoCloseEnv;

    pMyObject->GetCommandsByPrefix     = Bmc2ComdoGetCommandsByPrefix;
    pMyObject->GetCommandProperty      = Bmc2ComdoGetCommandProperty;
    pMyObject->AddCommandProperty      = Bmc2ComdoAddCommandProperty;
    pMyObject->DelCommandProperty      = Bmc2ComdoDelCommandProperty;
    pMyObject->DelAllCommandProperties = Bmc2ComdoDelAllCommandProperties;
    pMyObject->SetCommandHelpSyntax    = Bmc2ComdoSetCommandHelpSyntax;

    AnscQueueInitializeHeader(&pMyObject->CompoQueue    );
    AnscInitializeLock       (&pMyObject->CompoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
