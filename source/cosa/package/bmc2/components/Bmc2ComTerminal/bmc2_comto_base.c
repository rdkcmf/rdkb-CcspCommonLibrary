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

    module:	bmc2_comto_base.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bmc2 Com Terminal Object.

        *   Bmc2ComtoCreate
        *   Bmc2ComtoRemove
        *   Bmc2ComtoEnrollObjects
        *   Bmc2ComtoInitialize

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


#include "bmc2_comto_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        Bmc2ComtoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bmc2 Com Terminal Object and
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
Bmc2ComtoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = NULL;
    errno_t   rc   = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBMC2_COM_TERMINAL_OBJECT)AnscAllocateMemory(sizeof(BMC2_COM_TERMINAL_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BMC2_COM_TERMINAL_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BMC2_COM_TERMINAL_OID;
    pBaseObject->Create            = Bmc2ComtoCreate;
    pBaseObject->Remove            = Bmc2ComtoRemove;
    pBaseObject->EnrollObjects     = Bmc2ComtoEnrollObjects;
    pBaseObject->Initialize        = Bmc2ComtoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoRemove
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
Bmc2ComtoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->AccessLock    );
    AnscFreeLock(&pMyObject->ComdoSListLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject    );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoEnrollObjects
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
Bmc2ComtoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoInitialize
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
Bmc2ComtoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT)hThisObject;

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
    pMyObject->Oid                    = BMC2_COM_TERMINAL_OID;
    pMyObject->Create                 = Bmc2ComtoCreate;
    pMyObject->Remove                 = Bmc2ComtoRemove;
    pMyObject->EnrollObjects          = Bmc2ComtoEnrollObjects;
    pMyObject->Initialize             = Bmc2ComtoInitialize;

    pMyObject->hBmc2IceIf             = (ANSC_HANDLE)NULL;
    pMyObject->hBmc2EnvController     = (ANSC_HANDLE)NULL;
    pMyObject->bActive                = FALSE;

    pMyObject->hSlapContainerTerminal = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBmc2Terminal      = (ANSC_HANDLE)NULL;

    pMyObject->GetBmc2IceIf           = Bmc2ComtoGetBmc2IceIf;
    pMyObject->SetBmc2IceIf           = Bmc2ComtoSetBmc2IceIf;
    pMyObject->GetBmc2EnvController   = Bmc2ComtoGetBmc2EnvController;
    pMyObject->SetBmc2EnvController   = Bmc2ComtoSetBmc2EnvController;
    pMyObject->GetUserIdentifier      = Bmc2ComtoGetUserIdentifier;
    pMyObject->SetUserIdentifier      = Bmc2ComtoSetUserIdentifier;
    pMyObject->GetUserPermission      = Bmc2ComtoGetUserPermission;
    pMyObject->SetUserPermission      = Bmc2ComtoSetUserPermission;
    pMyObject->GetMaxLineNumber       = Bmc2ComtoGetMaxLineNumber;
    pMyObject->SetMaxLineNumber       = Bmc2ComtoSetMaxLineNumber;
    pMyObject->GetMaxColumnNumber     = Bmc2ComtoGetMaxColumnNumber;
    pMyObject->SetMaxColumnNumber     = Bmc2ComtoSetMaxColumnNumber;
    pMyObject->GetReadOnly            = Bmc2ComtoGetReadOnly;
    pMyObject->SetReadOnly            = Bmc2ComtoSetReadOnly;

    pMyObject->SetPseudoTermDevName   = Bmc2ComtoSetPseudoTermDevName;

    pMyObject->GetProperty            = Bmc2ComtoGetProperty;
    pMyObject->SetProperty            = Bmc2ComtoSetProperty;
    pMyObject->ResetProperty          = Bmc2ComtoResetProperty;
    pMyObject->Reset                  = Bmc2ComtoReset;

    pMyObject->AcqAccess              = Bmc2ComtoAcqAccess;
    pMyObject->RelAccess              = Bmc2ComtoRelAccess;
    pMyObject->Engage                 = Bmc2ComtoEngage;
    pMyObject->Cancel                 = Bmc2ComtoCancel;
    pMyObject->SetupEnv               = Bmc2ComtoSetupEnv;
    pMyObject->CloseEnv               = Bmc2ComtoCloseEnv;

    pMyObject->GetDomainCount         = Bmc2ComtoGetDomainCount;
    pMyObject->GetCurComDomain        = Bmc2ComtoGetCurComDomain;
    pMyObject->AddComDomain           = Bmc2ComtoAddComDomain;
    pMyObject->DelCurComDomain        = Bmc2ComtoDelCurComDomain;
    pMyObject->DelComDomain           = Bmc2ComtoDelComDomain;
    pMyObject->DelAllComDomains       = Bmc2ComtoDelAllComDomains;

    AnscInitializeLock       (&pMyObject->AccessLock    );
    AnscSListInitializeHeader(&pMyObject->ComdoSList    );
    AnscInitializeLock       (&pMyObject->ComdoSListLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
