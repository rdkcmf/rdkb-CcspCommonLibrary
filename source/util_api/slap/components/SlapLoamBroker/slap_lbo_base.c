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

    module:	slap_lbo_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Loam Broker Object.

        *   SlapLboCreate
        *   SlapLboRemove
        *   SlapLboEnrollObjects
        *   SlapLboInitialize

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


#include "slap_lbo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapLboCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Loam Broker Object and
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
SlapLboCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_LOAM_BROKER_OBJECT)AnscAllocateMemory(sizeof(SLAP_LOAM_BROKER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_LOAM_BROKER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_LOAM_BROKER_OID;
    pBaseObject->Create            = SlapLboCreate;
    pBaseObject->Remove            = SlapLboRemove;
    pBaseObject->EnrollObjects     = SlapLboEnrollObjects;
    pBaseObject->Initialize        = SlapLboInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboRemove
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
SlapLboRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    #ifdef  _ANSC_SLAP_LPC_
    PANSC_CPC_INTERFACE             pAnscCpcIf        = (PANSC_CPC_INTERFACE       )pMyObject->hAnscCpcIf;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    #endif

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    #ifdef  _ANSC_SLAP_LPC_

    if ( pAnscLpcConnector )
    {
        pAnscLpcConnector->Remove((ANSC_HANDLE)pAnscLpcConnector);

        pMyObject->hAnscLpcConnector = (ANSC_HANDLE)NULL;
    }

    if ( pAnscCpcIf )
    {
        AnscFreeMemory(pAnscCpcIf);

        pMyObject->hAnscCpcIf = (ANSC_HANDLE)NULL;
    }

    #endif

    AnscFreeLock(&pMyObject->AccessLock  );
    AnscFreeLock(&pMyObject->OboTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboEnrollObjects
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
SlapLboEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;

    #ifdef  _ANSC_SLAP_LPC_

    PANSC_CPC_INTERFACE             pAnscCpcIf        = (PANSC_CPC_INTERFACE       )pMyObject->hAnscCpcIf;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    errno_t                         rc                = -1;

    if ( !pAnscCpcIf )
    {
        pAnscCpcIf = (PANSC_CPC_INTERFACE)AnscAllocateMemory(sizeof(ANSC_CPC_INTERFACE));

        if ( !pAnscCpcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscCpcIf = (ANSC_HANDLE)pAnscCpcIf;
        }

        rc = strcpy_s(pAnscCpcIf->Name, sizeof(pAnscCpcIf->Name), ANSC_CPC_INTERFACE_NAME);
        ERR_CHK(rc);

        pAnscCpcIf->InterfaceId      = ANSC_CPC_INTERFACE_ID;
        pAnscCpcIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pAnscCpcIf->Size             = sizeof(ANSC_CPC_INTERFACE);

        pAnscCpcIf->CallDispatch     = SlapLboCpcCallDispatch;
        pAnscCpcIf->FreeOutputBuffer = SlapLboCpcFreeOutputBuffer;
        pAnscCpcIf->NotifyEvent      = SlapLboCpcNotifyEvent;
    }

    if ( !pAnscLpcConnector )
    {
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
        pAnscLpcConnector =
            (PANSC_LPC_CONNECTOR_OBJECT)AnscCreateLpccoTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
#else
            pAnscLpcConnector =
                (PANSC_LPC_CONNECTOR_OBJECT)AnscCreateLpccoUdp
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
#endif


        if ( !pAnscLpcConnector )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscLpcConnector = (ANSC_HANDLE)pAnscLpcConnector;
        }
    }

    #endif

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboInitialize
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
SlapLboInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
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
    pMyObject->Oid                 = SLAP_LOAM_BROKER_OID;
    pMyObject->Create              = SlapLboCreate;
    pMyObject->Remove              = SlapLboRemove;
    pMyObject->EnrollObjects       = SlapLboEnrollObjects;
    pMyObject->Initialize          = SlapLboInitialize;

    pMyObject->bActive             = FALSE;

    pMyObject->GetAnscCpcIf        = SlapLboGetAnscCpcIf;
    pMyObject->GetAnscLpcConnector = SlapLboGetAnscLpcConnector;

    pMyObject->GetProperty         = SlapLboGetProperty;
    pMyObject->SetProperty         = SlapLboSetProperty;
    pMyObject->ResetProperty       = SlapLboResetProperty;
    pMyObject->Reset               = SlapLboReset;

    pMyObject->AcqAccess           = SlapLboAcqAccess;
    pMyObject->RelAccess           = SlapLboRelAccess;
    pMyObject->Engage              = SlapLboEngage;
    pMyObject->Cancel              = SlapLboCancel;

    pMyObject->GetObjBinding       = SlapLboGetObjBinding;
    pMyObject->AddObjBinding       = SlapLboAddObjBinding;
    pMyObject->DelObjBinding       = SlapLboDelObjBinding;
    pMyObject->DelObjBinding2      = SlapLboDelObjBinding2;
    pMyObject->DelAllObjBindings   = SlapLboDelAllObjBindings;

    pMyObject->AsyncCallTaskLoco   = SlapLboAsyncCallTaskLoco;
    pMyObject->ProcessCallAcqc     = SlapLboProcessCallAcqc;
    pMyObject->ProcessCallRelc     = SlapLboProcessCallRelc;
    pMyObject->ProcessCallLoco     = SlapLboProcessCallLoco;
    pMyObject->ProcessCallDelo     = SlapLboProcessCallDelo;
    pMyObject->ProcessCallInvo     = SlapLboProcessCallInvo;
    pMyObject->ProcessCallAcqo     = SlapLboProcessCallAcqo;
    pMyObject->ProcessCallRelo     = SlapLboProcessCallRelo;

    pMyObject->CpcCallDispatch     = SlapLboCpcCallDispatch;
    pMyObject->CpcFreeOutputBuffer = SlapLboCpcFreeOutputBuffer;
    pMyObject->CpcNotifyEvent      = SlapLboCpcNotifyEvent;

    AnscInitializeLock(&pMyObject->AccessLock);

    for ( i = 0; i < SLAP_LBO_OBO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->OboTable[i]);
    }
    AnscInitializeLock(&pMyObject->OboTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
