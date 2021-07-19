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

    module:	slap_lco_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Loam Client Object.

        *   SlapLcoCreate
        *   SlapLcoRemove
        *   SlapLcoEnrollObjects
        *   SlapLcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/03    initial revision.

**********************************************************************/


#include "slap_lco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapLcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Loam Client Object and
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
SlapLcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_LOAM_CLIENT_OBJECT)AnscAllocateMemory(sizeof(SLAP_LOAM_CLIENT_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_LOAM_CLIENT_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_LOAM_CLIENT_OID;
    pBaseObject->Create            = SlapLcoCreate;
    pBaseObject->Remove            = SlapLcoRemove;
    pBaseObject->EnrollObjects     = SlapLcoEnrollObjects;
    pBaseObject->Initialize        = SlapLcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoRemove
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
SlapLcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject       = (PSLAP_LOAM_CLIENT_OBJECT     )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pPatrolTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hPatrolTimerIf;
    PSLAP_LOA_INTERFACE             pSlapLoaIf      = (PSLAP_LOA_INTERFACE          )pMyObject->hSlapLoaIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pPatrolTimerObj )
    {
        pPatrolTimerObj->Remove((ANSC_HANDLE)pPatrolTimerObj);
    }

    if ( pPatrolTimerIf )
    {
        AnscFreeMemory(pPatrolTimerIf);
    }

    if ( pSlapLoaIf )
    {
        AnscFreeMemory(pSlapLoaIf);

        pMyObject->hSlapLoaIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->OsoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoEnrollObjects
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
SlapLcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject       = (PSLAP_LOAM_CLIENT_OBJECT     )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pPatrolTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hPatrolTimerIf;
    PSLAP_LOA_INTERFACE             pSlapLoaIf      = (PSLAP_LOA_INTERFACE          )pMyObject->hSlapLoaIf;
    errno_t                         rc              = -1;

    if ( !pPatrolTimerObj )
    {
        pPatrolTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pPatrolTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hPatrolTimerObj = (ANSC_HANDLE)pPatrolTimerObj;
        }

        pPatrolTimerObj->SetTimerType((ANSC_HANDLE)pPatrolTimerObj, ANSC_TIMER_TYPE_PERIODIC      );
        pPatrolTimerObj->SetInterval ((ANSC_HANDLE)pPatrolTimerObj, SLAP_LCO_PATROL_TIMER_INTERVAL);
    }

    if ( !pPatrolTimerIf )
    {
        pPatrolTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pPatrolTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hPatrolTimerIf = (ANSC_HANDLE)pPatrolTimerIf;
        }

        pPatrolTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pPatrolTimerIf->Invoke         = SlapLcoPatrolTimerInvoke;

        pPatrolTimerObj->SetClient((ANSC_HANDLE)pPatrolTimerObj, (ANSC_HANDLE)pPatrolTimerIf);
    }

    if ( !pSlapLoaIf )
    {
        pSlapLoaIf = (PSLAP_LOA_INTERFACE)AnscAllocateMemory(sizeof(SLAP_LOA_INTERFACE));

        if ( !pSlapLoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapLoaIf = (ANSC_HANDLE)pSlapLoaIf;
        }

        rc = strcpy_s(pSlapLoaIf->Name, sizeof(pSlapLoaIf->Name), SLAP_LOA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapLoaIf->InterfaceId        = SLAP_LOA_INTERFACE_ID;
        pSlapLoaIf->hOwnerContext      = (ANSC_HANDLE)pMyObject;
        pSlapLoaIf->Size               = sizeof(SLAP_LOA_INTERFACE);

        pSlapLoaIf->AcqContainerAccess = SlapLcoLoaAcqContainerAccess;
        pSlapLoaIf->RelContainerAccess = SlapLcoLoaRelContainerAccess;

        pSlapLoaIf->LocateObject       = SlapLcoLoaLocateObject;
        pSlapLoaIf->DeleteObject       = SlapLcoLoaDeleteObject;
        pSlapLoaIf->InvokeObject       = SlapLcoLoaInvokeObject;
        pSlapLoaIf->AcqObjectAccess    = SlapLcoLoaAcqObjectAccess;
        pSlapLoaIf->RelObjectAccess    = SlapLcoLoaRelObjectAccess;
        pSlapLoaIf->RelObjectAccess2   = SlapLcoLoaRelObjectAccess2;
        pSlapLoaIf->FlushAllObjects    = SlapLcoLoaFlushAllObjects;

        pSlapLoaIf->EnrollMobileObject = SlapLcoLoaEnrollMobileObject;
        pSlapLoaIf->UnbindMobileObject = SlapLcoLoaUnbindMobileObject;
        pSlapLoaIf->VerifyMobileObject = SlapLcoLoaVerifyMobileObject;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoInitialize
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
SlapLcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT)hThisObject;
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
    pMyObject->Oid                    = SLAP_LOAM_CLIENT_OID;
    pMyObject->Create                 = SlapLcoCreate;
    pMyObject->Remove                 = SlapLcoRemove;
    pMyObject->EnrollObjects          = SlapLcoEnrollObjects;
    pMyObject->Initialize             = SlapLcoInitialize;

    pMyObject->hAnscLpcConnector      = (ANSC_HANDLE)NULL;
    pMyObject->hMobileContainer       = (ANSC_HANDLE)NULL;
    pMyObject->bActive                = FALSE;

    pMyObject->GetSlapLoaIf           = SlapLcoGetSlapLoaIf;
    pMyObject->GetSlapEnvController   = SlapLcoGetSlapEnvController;
    pMyObject->GetAnscLpcConnector    = SlapLcoGetAnscLpcConnector;
    pMyObject->SetAnscLpcConnector    = SlapLcoSetAnscLpcConnector;
    pMyObject->GetMobilityOn          = SlapLcoGetMobilityOn;
    pMyObject->SetMobilityOn          = SlapLcoSetMobilityOn;

    pMyObject->GetProperty            = SlapLcoGetProperty;
    pMyObject->SetProperty            = SlapLcoSetProperty;
    pMyObject->ResetProperty          = SlapLcoResetProperty;
    pMyObject->Reset                  = SlapLcoReset;

    pMyObject->Engage                 = SlapLcoEngage;
    pMyObject->Cancel                 = SlapLcoCancel;
    pMyObject->SetupEnv               = SlapLcoSetupEnv;
    pMyObject->CloseEnv               = SlapLcoCloseEnv;
    pMyObject->PatrolTimerInvoke      = SlapLcoPatrolTimerInvoke;

    pMyObject->GetObjectStubByName    = SlapLcoGetObjectStubByName;
    pMyObject->GetObjectStubByHandle  = SlapLcoGetObjectStubByHandle;
    pMyObject->AddObjectStub          = SlapLcoAddObjectStub;
    pMyObject->DelObjectStub          = SlapLcoDelObjectStub;
    pMyObject->DelAllObjectStubs      = SlapLcoDelAllObjectStubs;

    pMyObject->LpcToLoaImcpInvoAnswer = SlapLcoLpcToLoaImcpInvoAnswer;
    pMyObject->LpcToLoaSlapObject     = SlapLcoLpcToLoaSlapObject;
    pMyObject->LoaToLpcImcpInvoCall   = SlapLcoLoaToLpcImcpInvoCall;
    pMyObject->LoaToLpcSlapObject     = SlapLcoLoaToLpcSlapObject;

    pMyObject->LoaToUoaParamList      = SlapLcoLoaToUoaParamList;
    pMyObject->UoaToLoaParamList      = SlapLcoUoaToLoaParamList;
    pMyObject->LoaToUoaVariable       = SlapLcoLoaToUoaVariable;
    pMyObject->UoaToLoaVariable       = SlapLcoUoaToLoaVariable;

    pMyObject->LoaAcqContainerAccess  = SlapLcoLoaAcqContainerAccess;
    pMyObject->LoaRelContainerAccess  = SlapLcoLoaRelContainerAccess;

    pMyObject->LoaLocateObject        = SlapLcoLoaLocateObject;
    pMyObject->LoaDeleteObject        = SlapLcoLoaDeleteObject;
    pMyObject->LoaInvokeObject        = SlapLcoLoaInvokeObject;
    pMyObject->LoaAcqObjectAccess     = SlapLcoLoaAcqObjectAccess;
    pMyObject->LoaRelObjectAccess     = SlapLcoLoaRelObjectAccess;
    pMyObject->LoaRelObjectAccess2    = SlapLcoLoaRelObjectAccess2;
    pMyObject->LoaFlushAllObjects     = SlapLcoLoaFlushAllObjects;

    pMyObject->LoaEnrollMobileObject  = SlapLcoLoaEnrollMobileObject;
    pMyObject->LoaUnbindMobileObject  = SlapLcoLoaUnbindMobileObject;
    pMyObject->LoaVerifyMobileObject  = SlapLcoLoaVerifyMobileObject;

    for ( i = 0; i < SLAP_LCO_OSO_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->OsoTable[i]);
    }
    AnscInitializeLock(&pMyObject->OsoTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
