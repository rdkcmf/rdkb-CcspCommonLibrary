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

    module:	ansc_lpcco_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoCreate
        *   AnscLpccoRemove
        *   AnscLpccoEnrollObjects
        *   AnscLpccoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/23/03    initial revision.

**********************************************************************/


#include "ansc_lpcco_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscLpccoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Lpc Connector Object and
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
AnscLpccoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_LPC_CONNECTOR_OBJECT)AnscAllocateMemory(sizeof(ANSC_LPC_CONNECTOR_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), ANSC_LPC_CONNECTOR_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_LPC_CONNECTOR_OID;
    pBaseObject->Create            = AnscLpccoCreate;
    pBaseObject->Remove            = AnscLpccoRemove;
    pBaseObject->EnrollObjects     = AnscLpccoEnrollObjects;
    pBaseObject->Initialize        = AnscLpccoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoRemove
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
AnscLpccoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject     = (PANSC_LPC_CONNECTOR_OBJECT   )hThisObject;
    PANSC_IMC_INTERFACE             pAnscImcIf    = (PANSC_IMC_INTERFACE          )pMyObject->hAnscImcIf;
    PANSC_CPC_INTERFACE             pAnscCpcIf    = (PANSC_CPC_INTERFACE          )pMyObject->hAnscCpcIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hConnTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pConnTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hConnTimerIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pAnscImcIf )
    {
        AnscFreeMemory(pAnscImcIf);

        pMyObject->hAnscImcIf = (ANSC_HANDLE)NULL;
    }

    if ( pAnscCpcIf )
    {
        AnscFreeMemory(pAnscCpcIf);

        pMyObject->hAnscCpcIf = (ANSC_HANDLE)NULL;
    }

    if ( pConnTimerObj )
    {
        pConnTimerObj->Remove((ANSC_HANDLE)pConnTimerObj);
    }

    if ( pConnTimerIf )
    {
        AnscFreeMemory(pConnTimerIf);
    }

    AnscFreeLock(&pMyObject->AccessLock    );
    AnscFreeLock(&pMyObject->PartyTableLock);
    AnscFreeLock(&pMyObject->PcallQueueLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject    );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoEnrollObjects
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
AnscLpccoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject     = (PANSC_LPC_CONNECTOR_OBJECT   )hThisObject;
    PANSC_IMC_INTERFACE             pAnscImcIf    = (PANSC_IMC_INTERFACE          )pMyObject->hAnscImcIf;
    PANSC_CPC_INTERFACE             pAnscCpcIf    = (PANSC_CPC_INTERFACE          )pMyObject->hAnscCpcIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hConnTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pConnTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hConnTimerIf;
    errno_t                         rc            = -1;

    if ( !pAnscImcIf )
    {
        pAnscImcIf = (PANSC_IMC_INTERFACE)AnscAllocateMemory(sizeof(ANSC_IMC_INTERFACE));

        if ( !pAnscImcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscImcIf = (ANSC_HANDLE)pAnscImcIf;
        }

        rc = strcpy_s(pAnscImcIf->Name, sizeof(pAnscImcIf->Name), ANSC_IMC_INTERFACE_NAME);
        ERR_CHK(rc);

        pAnscImcIf->InterfaceId      = ANSC_IMC_INTERFACE_ID;
        pAnscImcIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pAnscImcIf->Size             = sizeof(ANSC_IMC_INTERFACE);

        pAnscImcIf->GenPartyAddr     = AnscLpccoImcGenPartyAddr;
        pAnscImcIf->GetLpcOpmode     = AnscLpccoImcGetLpcOpmode;
        pAnscImcIf->SetLpcOpmode     = AnscLpccoImcSetLpcOpmode;
        pAnscImcIf->GetPartyName     = AnscLpccoImcGetPartyName;
        pAnscImcIf->SetPartyName     = AnscLpccoImcSetPartyName;
        pAnscImcIf->IsPartyConnected = AnscLpccoImcIsPartyConnected;

        pAnscImcIf->Engage           = AnscLpccoImcEngage;
        pAnscImcIf->Cancel           = AnscLpccoImcCancel;
        pAnscImcIf->CallRequest      = AnscLpccoImcCallRequest;
        pAnscImcIf->CallRequest2     = AnscLpccoImcCallRequest2;
    }

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

        pAnscCpcIf->CallDispatch     = AnscLpccoCpcCallDispatch;
        pAnscCpcIf->FreeOutputBuffer = AnscLpccoCpcFreeOutputBuffer;
        pAnscCpcIf->NotifyEvent      = AnscLpccoCpcNotifyEvent;
    }

    if ( !pConnTimerObj )
    {
        pConnTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pConnTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hConnTimerObj = (ANSC_HANDLE)pConnTimerObj;
        }

        pConnTimerObj->SetTimerType((ANSC_HANDLE)pConnTimerObj, ANSC_TIMER_TYPE_SPORADIC   );
        pConnTimerObj->SetInterval ((ANSC_HANDLE)pConnTimerObj, ANSC_LPCCO_DEF_CONN_TIMEOUT);
    }

    if ( !pConnTimerIf )
    {
        pConnTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pConnTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hConnTimerIf = (ANSC_HANDLE)pConnTimerIf;
        }

        pConnTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pConnTimerIf->Invoke         = AnscLpccoConnTimerInvoke;

        pConnTimerObj->SetClient((ANSC_HANDLE)pConnTimerObj, (ANSC_HANDLE)pConnTimerIf);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoInitialize
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
AnscLpccoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
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
    pMyObject->Oid                 = ANSC_LPC_CONNECTOR_OID;
    pMyObject->Create              = AnscLpccoCreate;
    pMyObject->Remove              = AnscLpccoRemove;
    pMyObject->EnrollObjects       = AnscLpccoEnrollObjects;
    pMyObject->Initialize          = AnscLpccoInitialize;

    pMyObject->LpcOpmode           = ANSC_LPC_OPMODE_CLIENT;
    pMyObject->PartyName           = NULL;
    pMyObject->SeqNumber           = 0;
    pMyObject->CallTimeout         = ANSC_LPC_DEF_CALL_TIMEOUT;
    pMyObject->bConnected          = FALSE;
    pMyObject->bActive             = FALSE;

    pMyObject->GetAnscImcIf        = AnscLpccoGetAnscImcIf;
    pMyObject->GetAnscCpcIf        = AnscLpccoGetAnscCpcIf;
    pMyObject->SetAnscCpcIf        = AnscLpccoSetAnscCpcIf;
    pMyObject->GetPartyAddrSize    = AnscLpccoGetPartyAddrSize;
    pMyObject->GetCallTimeout      = AnscLpccoGetCallTimeout;
    pMyObject->SetCallTimeout      = AnscLpccoSetCallTimeout;
    pMyObject->Reset               = AnscLpccoReset;

    pMyObject->GetPartyCount       = AnscLpccoGetPartyCount;
    pMyObject->GetPartyArray       = AnscLpccoGetPartyArray;
    pMyObject->GetPartyAddr        = AnscLpccoGetPartyAddr;
    pMyObject->GetPartyAddr2       = AnscLpccoGetPartyAddr2;
    pMyObject->AddPartyAddr        = AnscLpccoAddPartyAddr;
    pMyObject->DelPartyAddr        = AnscLpccoDelPartyAddr;
    pMyObject->DelAllPartyAddrs    = AnscLpccoDelAllPartyAddrs;

    pMyObject->GetPendingCall      = AnscLpccoGetPendingCall;
    pMyObject->PopPendingCall      = AnscLpccoPopPendingCall;
    pMyObject->AddPendingCall      = AnscLpccoAddPendingCall;
    pMyObject->DelPendingCall      = AnscLpccoDelPendingCall;
    pMyObject->PopAllPendingCalls  = AnscLpccoPopAllPendingCalls;
    pMyObject->DelAllPendingCalls  = AnscLpccoDelAllPendingCalls;

    pMyObject->AcqAccess           = AnscLpccoAcqAccess;
    pMyObject->RelAccess           = AnscLpccoRelAccess;
    pMyObject->Engage              = AnscLpccoEngage;
    pMyObject->Cancel              = AnscLpccoCancel;
    pMyObject->Enroll              = AnscLpccoEnroll;
    pMyObject->Unbind              = AnscLpccoUnbind;
    pMyObject->ConnectToManager    = AnscLpccoConnectToManager;
    pMyObject->ConnTimerInvoke     = AnscLpccoConnTimerInvoke;

    pMyObject->ImcGenPartyAddr     = AnscLpccoImcGenPartyAddr;
    pMyObject->ImcGetLpcOpmode     = AnscLpccoImcGetLpcOpmode;
    pMyObject->ImcSetLpcOpmode     = AnscLpccoImcSetLpcOpmode;
    pMyObject->ImcGetPartyName     = AnscLpccoImcGetPartyName;
    pMyObject->ImcSetPartyName     = AnscLpccoImcSetPartyName;
    pMyObject->ImcIsPartyConnected = AnscLpccoImcIsPartyConnected;

    pMyObject->ImcEngage           = AnscLpccoImcEngage;
    pMyObject->ImcCancel           = AnscLpccoImcCancel;
    pMyObject->ImcCallRequest      = AnscLpccoImcCallRequest;
    pMyObject->ImcCallRequest2     = AnscLpccoImcCallRequest2;

    pMyObject->CpcCallDispatch     = AnscLpccoCpcCallDispatch;
    pMyObject->CpcFreeOutputBuffer = AnscLpccoCpcFreeOutputBuffer;
    pMyObject->CpcNotifyEvent      = AnscLpccoCpcNotifyEvent;

    AnscInitializeLock(&pMyObject->AccessLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->PartyTable[i]);
    }
    AnscInitializeLock(&pMyObject->PartyTableLock);

    AnscQueueInitializeHeader(&pMyObject->PcallQueue    );
    AnscInitializeLock       (&pMyObject->PcallQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
