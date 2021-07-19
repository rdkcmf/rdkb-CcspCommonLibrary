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

    module:	slap_amo_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Access Manager Object.

        *   SlapAmoCreate
        *   SlapAmoRemove
        *   SlapAmoEnrollObjects
        *   SlapAmoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_amo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapAmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Access Manager Object and
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
SlapAmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_ACCESS_MANAGER_OBJECT)AnscAllocateMemory(sizeof(SLAP_ACCESS_MANAGER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_ACCESS_MANAGER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_ACCESS_MANAGER_OID;
    pBaseObject->Create            = SlapAmoCreate;
    pBaseObject->Remove            = SlapAmoRemove;
    pBaseObject->EnrollObjects     = SlapAmoEnrollObjects;
    pBaseObject->Initialize        = SlapAmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoRemove
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
SlapAmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pMyObject->hSlapGoaIf;
    PSLAP_POA_INTERFACE             pSlapPoaIf         = (PSLAP_POA_INTERFACE        )pMyObject->hSlapPoaIf;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pSlapGoaIf )
    {
        AnscFreeMemory(pSlapGoaIf);

        pMyObject->hSlapGoaIf = (ANSC_HANDLE)NULL;
    }

    if ( pSlapPoaIf )
    {
        AnscFreeMemory(pSlapPoaIf);

        pMyObject->hSlapPoaIf = (ANSC_HANDLE)NULL;
    }

    if ( pSlapEnvController )
    {
        pSlapEnvController->Remove((ANSC_HANDLE)pSlapEnvController);

        pMyObject->hSlapEnvController = (ANSC_HANDLE)NULL;
    }


    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoEnrollObjects
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
SlapAmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pMyObject->hSlapGoaIf;
    PSLAP_POA_INTERFACE             pSlapPoaIf         = (PSLAP_POA_INTERFACE        )pMyObject->hSlapPoaIf;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    errno_t                         rc                 = -1;

    if ( !pSlapGoaIf )
    {
        pSlapGoaIf = (PSLAP_GOA_INTERFACE)AnscAllocateMemory(sizeof(SLAP_GOA_INTERFACE));

        if ( !pSlapGoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapGoaIf = (ANSC_HANDLE)pSlapGoaIf;
        }

        rc = strcpy_s(pSlapGoaIf->Name, sizeof(pSlapGoaIf->Name), SLAP_GOA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapGoaIf->InterfaceId         = SLAP_GOA_INTERFACE_ID;
        pSlapGoaIf->hOwnerContext       = (ANSC_HANDLE)pMyObject;
        pSlapGoaIf->Size                = sizeof(SLAP_GOA_INTERFACE);

        pSlapGoaIf->GetAggregationMode  = SlapAmoGoaGetAggregationMode;
        pSlapGoaIf->SetAggregationMode  = SlapAmoGoaSetAggregationMode;
        pSlapGoaIf->GetLpcPartyPort     = SlapAmoGoaGetLpcPartyPort;
        pSlapGoaIf->SetLpcPartyPort     = SlapAmoGoaSetLpcPartyPort;
        pSlapGoaIf->GetLpcPartyAddr     = SlapAmoGoaGetLpcPartyAddr;
        pSlapGoaIf->SetLpcPartyAddr     = SlapAmoGoaSetLpcPartyAddr;
        pSlapGoaIf->GetLpcManagerAddr   = SlapAmoGoaGetLpcManagerAddr;
        pSlapGoaIf->SetLpcManagerAddr   = SlapAmoGoaSetLpcManagerAddr;

        pSlapGoaIf->CreateContainer     = SlapAmoGoaCreateContainer;
        pSlapGoaIf->DeleteContainer     = SlapAmoGoaDeleteContainer;

        pSlapGoaIf->CreateObject        = SlapAmoGoaCreateObject;
        pSlapGoaIf->DeleteObject        = SlapAmoGoaDeleteObject;
        pSlapGoaIf->InvokeObject        = SlapAmoGoaInvokeObject;
        pSlapGoaIf->InvokeObject2       = SlapAmoGoaInvokeObject2;
        pSlapGoaIf->AttachObject        = SlapAmoGoaAttachObject;
        pSlapGoaIf->DetachObject        = SlapAmoGoaDetachObject;
        pSlapGoaIf->UnlinkObject        = SlapAmoGoaUnlinkObject;
        pSlapGoaIf->AcqObjectAccess     = SlapAmoGoaAcqObjectAccess;
        pSlapGoaIf->RelObjectAccess     = SlapAmoGoaRelObjectAccess;
        pSlapGoaIf->FlushAllObjects     = SlapAmoGoaFlushAllObjects;

        pSlapGoaIf->EnrollObjDescriptor = SlapAmoGoaEnrollObjDescriptor;
        pSlapGoaIf->UnbindObjDescriptor = SlapAmoGoaUnbindObjDescriptor;
        pSlapGoaIf->VerifyObjDescriptor = SlapAmoGoaVerifyObjDescriptor;

        pSlapGoaIf->EnrollMobileObject  = SlapAmoGoaEnrollMobileObject;
        pSlapGoaIf->UnbindMobileObject  = SlapAmoGoaUnbindMobileObject;
        pSlapGoaIf->VerifyMobileObject  = SlapAmoGoaVerifyMobileObject;
    }

    if ( !pSlapPoaIf )
    {
        pSlapPoaIf = (PSLAP_POA_INTERFACE)AnscAllocateMemory(sizeof(SLAP_POA_INTERFACE));

        if ( !pSlapPoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapPoaIf = (ANSC_HANDLE)pSlapPoaIf;
        }

        rc = strcpy_s(pSlapPoaIf->Name, sizeof(pSlapPoaIf->Name), SLAP_POA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapPoaIf->InterfaceId       = SLAP_POA_INTERFACE_ID;
        pSlapPoaIf->hOwnerContext     = (ANSC_HANDLE)pMyObject;
        pSlapPoaIf->Size              = sizeof(SLAP_POA_INTERFACE);

        pSlapPoaIf->AcqObjectAccess   = SlapAmoPoaAcqObjectAccess;
        pSlapPoaIf->RelObjectAccess   = SlapAmoPoaRelObjectAccess;

        pSlapPoaIf->CreateProxyObject = SlapAmoPoaCreateProxyObject;
        pSlapPoaIf->EnrollProxyObject = SlapAmoPoaEnrollProxyObject;
        pSlapPoaIf->UnbindProxyObject = SlapAmoPoaUnbindProxyObject;
        pSlapPoaIf->VerifyProxyObject = SlapAmoPoaVerifyProxyObject;
    }

    if ( !pSlapEnvController )
    {
        pSlapEnvController =
            (PSLAP_ENV_CONTROLLER_OBJECT)SlapCreateEnvController
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapEnvController )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapEnvController = (ANSC_HANDLE)pSlapEnvController;
        }
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoInitialize
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
SlapAmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT)hThisObject;

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
    pMyObject->Oid                    = SLAP_ACCESS_MANAGER_OID;
    pMyObject->Create                 = SlapAmoCreate;
    pMyObject->Remove                 = SlapAmoRemove;
    pMyObject->EnrollObjects          = SlapAmoEnrollObjects;
    pMyObject->Initialize             = SlapAmoInitialize;

    pMyObject->bActive                = FALSE;

    pMyObject->GetSlapGoaIf           = SlapAmoGetSlapGoaIf;
    pMyObject->GetSlapPoaIf           = SlapAmoGetSlapPoaIf;
    pMyObject->GetSlapEnvController   = SlapAmoGetSlapEnvController;
    pMyObject->GetSlapLoamClient      = SlapAmoGetSlapLoamClient;
    pMyObject->GetSlapLoamServer      = SlapAmoGetSlapLoamServer;
    pMyObject->GetAnscLpcConnector    = SlapAmoGetAnscLpcConnector;

    pMyObject->GetProperty            = SlapAmoGetProperty;
    pMyObject->SetProperty            = SlapAmoSetProperty;
    pMyObject->ResetProperty          = SlapAmoResetProperty;
    pMyObject->Reset                  = SlapAmoReset;

    pMyObject->Engage                 = SlapAmoEngage;
    pMyObject->Cancel                 = SlapAmoCancel;

    pMyObject->GoaToUoaParamList      = SlapAmoGoaToUoaParamList;
    pMyObject->UoaToGoaParamList      = SlapAmoUoaToGoaParamList;
    pMyObject->GoaToUoaVariable       = SlapAmoGoaToUoaVariable;
    pMyObject->UoaToGoaVariable       = SlapAmoUoaToGoaVariable;
    pMyObject->GoaToLoaParamList      = SlapAmoGoaToLoaParamList;
    pMyObject->LoaToGoaParamList      = SlapAmoLoaToGoaParamList;
    pMyObject->GoaToLoaVariable       = SlapAmoGoaToLoaVariable;
    pMyObject->LoaToGoaVariable       = SlapAmoLoaToGoaVariable;

    pMyObject->GoaGetAggregationMode  = SlapAmoGoaGetAggregationMode;
    pMyObject->GoaSetAggregationMode  = SlapAmoGoaSetAggregationMode;
    pMyObject->GoaGetLpcPartyPort     = SlapAmoGoaGetLpcPartyPort;
    pMyObject->GoaSetLpcPartyPort     = SlapAmoGoaSetLpcPartyPort;

    pMyObject->GoaCreateContainer     = SlapAmoGoaCreateContainer;
    pMyObject->GoaDeleteContainer     = SlapAmoGoaDeleteContainer;

    pMyObject->GoaCreateObject        = SlapAmoGoaCreateObject;
    pMyObject->GoaDeleteObject        = SlapAmoGoaDeleteObject;
    pMyObject->GoaInvokeObject        = SlapAmoGoaInvokeObject;
    pMyObject->GoaInvokeObject2       = SlapAmoGoaInvokeObject2;
    pMyObject->GoaAttachObject        = SlapAmoGoaAttachObject;
    pMyObject->GoaDetachObject        = SlapAmoGoaDetachObject;
    pMyObject->GoaUnlinkObject        = SlapAmoGoaUnlinkObject;
    pMyObject->GoaAcqObjectAccess     = SlapAmoGoaAcqObjectAccess;
    pMyObject->GoaRelObjectAccess     = SlapAmoGoaRelObjectAccess;
    pMyObject->GoaFlushAllObjects     = SlapAmoGoaFlushAllObjects;

    pMyObject->GoaEnrollObjDescriptor = SlapAmoGoaEnrollObjDescriptor;
    pMyObject->GoaUnbindObjDescriptor = SlapAmoGoaUnbindObjDescriptor;
    pMyObject->GoaVerifyObjDescriptor = SlapAmoGoaVerifyObjDescriptor;

    pMyObject->GoaEnrollMobileObject  = SlapAmoGoaEnrollMobileObject;
    pMyObject->GoaUnbindMobileObject  = SlapAmoGoaUnbindMobileObject;
    pMyObject->GoaVerifyMobileObject  = SlapAmoGoaVerifyMobileObject;

    pMyObject->PoaAcqObjectAccess     = SlapAmoPoaAcqObjectAccess;
    pMyObject->PoaRelObjectAccess     = SlapAmoPoaRelObjectAccess;

    pMyObject->PoaCreateProxyObject   = SlapAmoPoaCreateProxyObject;
    pMyObject->PoaEnrollProxyObject   = SlapAmoPoaEnrollProxyObject;
    pMyObject->PoaUnbindProxyObject   = SlapAmoPoaUnbindProxyObject;
    pMyObject->PoaVerifyProxyObject   = SlapAmoPoaVerifyProxyObject;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
