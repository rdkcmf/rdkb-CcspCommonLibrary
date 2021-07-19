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

    module:	slap_eco_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Env Controller Object.

        *   SlapEcoCreate
        *   SlapEcoRemove
        *   SlapEcoEnrollObjects
        *   SlapEcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#include "slap_eco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapEcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Env Controller Object and
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
SlapEcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_ENV_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(SLAP_ENV_CONTROLLER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_ENV_CONTROLLER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_ENV_CONTROLLER_OID;
    pBaseObject->Create            = SlapEcoCreate;
    pBaseObject->Remove            = SlapEcoRemove;
    pBaseObject->EnrollObjects     = SlapEcoEnrollObjects;
    pBaseObject->Initialize        = SlapEcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoRemove
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
SlapEcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf        = (PSLAP_UOA_INTERFACE        )pMyObject->hSlapUoaIf;
    PSLAP_BSS_INTERFACE             pSlapBssIf        = (PSLAP_BSS_INTERFACE        )pMyObject->hSlapBssIf;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper    = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_VAR_CONVERTER_OBJECT      pSlapVarConverter = (PSLAP_VAR_CONVERTER_OBJECT )pMyObject->hSlapVarConverter;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper    = (PSLAP_VAR_MAPPER_OBJECT    )pMyObject->hSlapVarMapper;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pSlapUoaIf )
    {
        AnscFreeMemory(pSlapUoaIf);

        pMyObject->hSlapUoaIf = (ANSC_HANDLE)NULL;
    }

    if ( pSlapBssIf )
    {
        AnscFreeMemory(pSlapBssIf);

        pMyObject->hSlapBssIf = (ANSC_HANDLE)NULL;
    }

    if ( pSlapObjMapper )
    {
        pSlapObjMapper->Remove((ANSC_HANDLE)pSlapObjMapper);

        pMyObject->hSlapObjMapper = (ANSC_HANDLE)NULL;
    }

    if ( pSlapVarConverter )
    {
        pSlapVarConverter->Remove((ANSC_HANDLE)pSlapVarConverter);

        pMyObject->hSlapVarConverter = (ANSC_HANDLE)NULL;
    }

    if ( pSlapVarMapper )
    {
        pSlapVarMapper->Remove((ANSC_HANDLE)pSlapVarMapper);

        pMyObject->hSlapVarMapper = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->OcoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoEnrollObjects
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
SlapEcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf        = (PSLAP_UOA_INTERFACE        )pMyObject->hSlapUoaIf;
    PSLAP_BSS_INTERFACE             pSlapBssIf        = (PSLAP_BSS_INTERFACE        )pMyObject->hSlapBssIf;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper    = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_VAR_CONVERTER_OBJECT      pSlapVarConverter = (PSLAP_VAR_CONVERTER_OBJECT )pMyObject->hSlapVarConverter;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper    = (PSLAP_VAR_MAPPER_OBJECT    )pMyObject->hSlapVarMapper;
    errno_t                         rc                = -1;

    if ( !pSlapUoaIf )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)AnscAllocateMemory(sizeof(SLAP_UOA_INTERFACE));

        if ( !pSlapUoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapUoaIf = (ANSC_HANDLE)pSlapUoaIf;
        }

        rc = strcpy_s(pSlapUoaIf->Name, sizeof(pSlapUoaIf->Name), SLAP_UOA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapUoaIf->InterfaceId         = SLAP_UOA_INTERFACE_ID;
        pSlapUoaIf->hOwnerContext       = (ANSC_HANDLE)pMyObject;
        pSlapUoaIf->Size                = sizeof(SLAP_UOA_INTERFACE);

        pSlapUoaIf->CreateContainer     = SlapEcoUoaCreateContainer;
        pSlapUoaIf->DeleteContainer     = SlapEcoUoaDeleteContainer;
        pSlapUoaIf->AcqContainerAccess  = SlapEcoUoaAcqContainerAccess;
        pSlapUoaIf->RelContainerAccess  = SlapEcoUoaRelContainerAccess;

        pSlapUoaIf->CreateObject        = SlapEcoUoaCreateObject;
        pSlapUoaIf->DeleteObject        = SlapEcoUoaDeleteObject;
        pSlapUoaIf->DeleteObject2       = SlapEcoUoaDeleteObject2;
        pSlapUoaIf->InvokeObject        = SlapEcoUoaInvokeObject;
        pSlapUoaIf->GetObjInsContext    = SlapEcoUoaGetObjInsContext;
        pSlapUoaIf->SetObjInsContext    = SlapEcoUoaSetObjInsContext;
        pSlapUoaIf->GetObjCallOption    = SlapEcoUoaGetObjCallOption;
        pSlapUoaIf->AcqObjectAccess     = SlapEcoUoaAcqObjectAccess;
        pSlapUoaIf->RelObjectAccess     = SlapEcoUoaRelObjectAccess;

        pSlapUoaIf->EnrollObjDescriptor = SlapEcoUoaEnrollObjDescriptor;
        pSlapUoaIf->UnbindObjDescriptor = SlapEcoUoaUnbindObjDescriptor;
        pSlapUoaIf->VerifyObjDescriptor = SlapEcoUoaVerifyObjDescriptor;

        pSlapUoaIf->CreateProxyObject   = SlapEcoUoaCreateProxyObject;
        pSlapUoaIf->EnrollProxyObject   = SlapEcoUoaEnrollProxyObject;
        pSlapUoaIf->UnbindProxyObject   = SlapEcoUoaUnbindProxyObject;
        pSlapUoaIf->VerifyProxyObject   = SlapEcoUoaVerifyProxyObject;
    }

    if ( !pSlapBssIf )
    {
        pSlapBssIf = (PSLAP_BSS_INTERFACE)AnscAllocateMemory(sizeof(SLAP_BSS_INTERFACE));

        if ( !pSlapBssIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapBssIf = (ANSC_HANDLE)pSlapBssIf;
        }

        rc = strcpy_s(pSlapBssIf->Name, sizeof(pSlapBssIf->Name), SLAP_BSS_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapBssIf->InterfaceId     = SLAP_BSS_INTERFACE_ID;
        pSlapBssIf->hOwnerContext   = (ANSC_HANDLE)pMyObject;
        pSlapBssIf->Size            = sizeof(SLAP_BSS_INTERFACE);

        pSlapBssIf->GetPoolingFlag  = SlapEcoBssGetPoolingFlag;
        pSlapBssIf->SetPoolingFlag  = SlapEcoBssSetPoolingFlag;
        pSlapBssIf->GetDefPoolSize  = SlapEcoBssGetDefPoolSize;
        pSlapBssIf->SetDefPoolSize  = SlapEcoBssSetDefPoolSize;
        pSlapBssIf->GetVarConverter = SlapEcoBssGetVarConverter;
        pSlapBssIf->GetVarMapper    = SlapEcoBssGetVarMapper;
    }

    if ( !pSlapObjMapper )
    {
        pSlapObjMapper =
            (PSLAP_OBJ_MAPPER_OBJECT)SlapCreateObjMapper
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapObjMapper )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapObjMapper = (ANSC_HANDLE)pSlapObjMapper;
        }
    }

    if ( !pSlapVarConverter )
    {
        pSlapVarConverter =
            (PSLAP_VAR_CONVERTER_OBJECT)SlapCreateVarConverter
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapVarConverter )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapVarConverter = (ANSC_HANDLE)pSlapVarConverter;
        }
    }

    if ( !pSlapVarMapper )
    {
        pSlapVarMapper =
            (PSLAP_VAR_MAPPER_OBJECT)SlapCreateVarMapper
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapVarMapper )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapVarMapper = (ANSC_HANDLE)pSlapVarMapper;
        }
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoInitialize
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
SlapEcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
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
    pMyObject->Oid                     = SLAP_ENV_CONTROLLER_OID;
    pMyObject->Create                  = SlapEcoCreate;
    pMyObject->Remove                  = SlapEcoRemove;
    pMyObject->EnrollObjects           = SlapEcoEnrollObjects;
    pMyObject->Initialize              = SlapEcoInitialize;

    pMyObject->hSlapDefContainer       = (ANSC_HANDLE)NULL;
    pMyObject->bActive                 = FALSE;

    pMyObject->bCallPending            = FALSE;
    pMyObject->PendingCallTimestamp    = 0;

    pMyObject->GetSlapUoaIf            = SlapEcoGetSlapUoaIf;
    pMyObject->GetSlapBssIf            = SlapEcoGetSlapBssIf;
    pMyObject->GetSlapObjMapper        = SlapEcoGetSlapObjMapper;
    pMyObject->GetSlapVarConverter     = SlapEcoGetSlapVarConverter;
    pMyObject->GetSlapVarMapper        = SlapEcoGetSlapVarMapper;
    pMyObject->GetSlapDefContainer     = SlapEcoGetSlapDefContainer;
    pMyObject->GetPendingCallInfo      = SlapEcoGetPendingCallInfo;

    pMyObject->GetProperty             = SlapEcoGetProperty;
    pMyObject->SetProperty             = SlapEcoSetProperty;
    pMyObject->ResetProperty           = SlapEcoResetProperty;
    pMyObject->Reset                   = SlapEcoReset;

    pMyObject->Engage                  = SlapEcoEngage;
    pMyObject->Cancel                  = SlapEcoCancel;
    pMyObject->SetupEnv                = SlapEcoSetupEnv;

    pMyObject->GetSlapContainer        = SlapEcoGetSlapContainer;
    pMyObject->AddSlapContainer        = SlapEcoAddSlapContainer;
    pMyObject->DelSlapContainer1       = SlapEcoDelSlapContainer1;
    pMyObject->DelSlapContainer2       = SlapEcoDelSlapContainer2;
    pMyObject->DelAllSlapContainers    = SlapEcoDelAllSlapContainers;

    pMyObject->UoaCreateContainer      = SlapEcoUoaCreateContainer;
    pMyObject->UoaDeleteContainer      = SlapEcoUoaDeleteContainer;
    pMyObject->UoaAcqContainerAccess   = SlapEcoUoaAcqContainerAccess;
    pMyObject->UoaRelContainerAccess   = SlapEcoUoaRelContainerAccess;

    pMyObject->UoaCreateObject         = SlapEcoUoaCreateObject;
    pMyObject->UoaDeleteObject         = SlapEcoUoaDeleteObject;
    pMyObject->UoaDeleteObject2        = SlapEcoUoaDeleteObject2;
    pMyObject->UoaInvokeObject         = SlapEcoUoaInvokeObject;
    pMyObject->UoaGetObjInsContext     = SlapEcoUoaGetObjInsContext;
    pMyObject->UoaSetObjInsContext     = SlapEcoUoaSetObjInsContext;
    pMyObject->UoaGetObjCallOption     = SlapEcoUoaGetObjCallOption;
    pMyObject->UoaAcqObjectAccess      = SlapEcoUoaAcqObjectAccess;
    pMyObject->UoaRelObjectAccess      = SlapEcoUoaRelObjectAccess;

    pMyObject->UoaEnrollObjDescriptor  = SlapEcoUoaEnrollObjDescriptor;
    pMyObject->UoaUnbindObjDescriptor  = SlapEcoUoaUnbindObjDescriptor;
    pMyObject->UoaVerifyObjDescriptor  = SlapEcoUoaVerifyObjDescriptor;

    pMyObject->UoaCreateProxyObject    = SlapEcoUoaCreateProxyObject;
    pMyObject->UoaEnrollProxyObject    = SlapEcoUoaEnrollProxyObject;
    pMyObject->UoaUnbindProxyObject    = SlapEcoUoaUnbindProxyObject;
    pMyObject->UoaVerifyProxyObject    = SlapEcoUoaVerifyProxyObject;

    pMyObject->BssGetPoolingFlag       = SlapEcoBssGetPoolingFlag;
    pMyObject->BssSetPoolingFlag       = SlapEcoBssSetPoolingFlag;
    pMyObject->BssGetDefPoolSize       = SlapEcoBssGetDefPoolSize;
    pMyObject->BssSetDefPoolSize       = SlapEcoBssSetDefPoolSize;
    pMyObject->BssGetVarConverter      = SlapEcoBssGetVarConverter;
    pMyObject->BssGetVarMapper         = SlapEcoBssGetVarMapper;

    for ( i = 0; i < SLAP_ECO_OCO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->OcoTable[i]);
    }
    AnscInitializeLock(&pMyObject->OcoTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
