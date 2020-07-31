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

    module: dslh_cpeco_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh Cpe Controller Object.

        *   DslhCpecoCreate
        *   DslhCpecoRemove
        *   DslhCpecoEnrollObjects
        *   DslhCpecoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.
        02/12/09    add more statistics apis

**********************************************************************/


#include "dslh_cpeco_global.h"

PDSLH_CPE_CONTROLLER_OBJECT    g_pDslhCpeController = NULL;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhCpecoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Cpe Controller Object and
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
DslhCpecoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_CPE_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(DSLH_CPE_CONTROLLER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, DSLH_CPE_CONTROLLER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = DSLH_CPE_CONTROLLER_OID;
    pBaseObject->Create            = DslhCpecoCreate;
    pBaseObject->Remove            = DslhCpecoRemove;
    pBaseObject->EnrollObjects     = DslhCpecoEnrollObjects;
    pBaseObject->Initialize        = DslhCpecoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    /* Assign the global CpeController */
    g_pDslhCpeController = pMyObject;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRemove
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
DslhCpecoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT)hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase  = (PDSLH_WMP_DATABASE_OBJECT  )pMyObject->hDslhWmpDatabase;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent= (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pDslhWmpDatabase )
    {
        pDslhWmpDatabase->Remove((ANSC_HANDLE)pDslhWmpDatabase);

        pMyObject->hDslhWmpDatabase = (ANSC_HANDLE)NULL;
    }

    if( pDslhDataModelAgent != NULL)
    {
        pDslhDataModelAgent->Remove((ANSC_HANDLE)pDslhDataModelAgent);

        pMyObject->hDslhDataModelAgent = NULL;
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoEnrollObjects
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
DslhCpecoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject           = (PDSLH_CPE_CONTROLLER_OBJECT)hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pMyObject->hDslhWmpDatabase;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;

    if ( !pDslhWmpDatabase )
    {
        pDslhWmpDatabase =
            (PDSLH_WMP_DATABASE_OBJECT)DslhCreateWmpDatabase
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pDslhWmpDatabase )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDslhWmpDatabase = (ANSC_HANDLE)pDslhWmpDatabase;
        }
    }

    if( !pDslhDataModelAgent)
    {
        pDslhDataModelAgent = 
             (PDSLH_DATAMODEL_AGENT_OBJECT)DslhCreateDataModelAgent
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        pMyObject->hDslhDataModelAgent = (ANSC_HANDLE)pDslhDataModelAgent;
    }
    
    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoInitialize
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
DslhCpecoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT)hThisObject;
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
    pMyObject->Oid                      = DSLH_CPE_CONTROLLER_OID;
    pMyObject->Create                   = DslhCpecoCreate;
    pMyObject->Remove                   = DslhCpecoRemove;
    pMyObject->EnrollObjects            = DslhCpecoEnrollObjects;
    pMyObject->Initialize               = DslhCpecoInitialize;

    pMyObject->bActive                  = FALSE;

    pMyObject->GetDslhWmpDatabase       = DslhCpecoGetDslhWmpDatabase;

    pMyObject->GetProperty              = DslhCpecoGetProperty;
    pMyObject->SetProperty              = DslhCpecoSetProperty;
    pMyObject->ResetProperty            = DslhCpecoResetProperty;
    pMyObject->Reset                    = DslhCpecoReset;

    pMyObject->GetDslhMpaIf             = DslhCpecoGetDslhMpaIf;
    pMyObject->GetDslhMprIf             = DslhCpecoGetDslhMprIf;
    pMyObject->SetDbusHandle            = DslhCpecoSetDbusHandle;

    pMyObject->hDbusHandle              = NULL;

    pMyObject->Engage                   = DslhCpecoEngage;
    pMyObject->Cancel                   = DslhCpecoCancel;
    pMyObject->SetupEnv                 = DslhCpecoSetupEnv;
    pMyObject->CloseEnv                 = DslhCpecoCloseEnv;

    pMyObject->InitDataModel            = DslhCpecoInitDataModel;
    pMyObject->RegisterAllParams        = DslhCpecoRegisterAllParams;
    pMyObject->RegisterObject           = DslhCpecoRegisterObject;
    pMyObject->RegisterObject2          = DslhCpecoRegisterObject2;
    pMyObject->RegisterHiddenObject     = DslhCpecoRegisterHiddenObject;
    pMyObject->RegisterHiddenObject2    = DslhCpecoRegisterHiddenObject2;
    pMyObject->LoadExternalDMLibFile    = DslhCpecoLoadExternalDMLibFile;
    pMyObject->LoadExternalDMLibFile2   = DslhCpecoLoadExternalDMLibFile2;
    pMyObject->LoadInternalDMLibFile    = DslhCpecoLoadInternalDMLibFile;
    pMyObject->UnloadDMLibFile          = DslhCpecoUnloadDMLibFile;
    pMyObject->RegisterCcspDataModel    = DslhCpecoRegisterDataModel;
    pMyObject->RegisterCcspDataModel2   = DslhCpecoRegisterDataModel2;


    pMyObject->SetParameterArray        = DslhCpecoSetParameterArray;

    pMyObject->AddInterface             = DslhCpecoAddInterface;
    pMyObject->GetInterfaceByName       = DslhCpecoGetInterfaceByName;

    pMyObject->uIfCount                 = 0;

    for( i = 0; i < CPECONTROLLER_MAX_INTERFACE_COUNT; i ++)
    {
        pMyObject->hIfArray[i] = NULL;
    }

    pMyObject->hParameterArray          = NULL;
    pMyObject->hParameterHolder         = NULL;
    pMyObject->uParameterCount          = 0;
    pMyObject->uParameterIndex          = 0;

    pMyObject->uObjCount            = 0;

    for( i = 0; i < CPECONTROLLER_MAX_REG_OBJECT_COUNT; i ++)
    {
        pMyObject->hObjDespArray[i] = NULL;
    }

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
