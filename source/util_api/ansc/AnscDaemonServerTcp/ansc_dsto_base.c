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

    module:	ansc_dsto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoCreate
        *   AnscDstoRemove
        *   AnscDstoEnrollObjects
        *   AnscDstoInitialize
        *   AnscDstoShutdown

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#include "ansc_dsto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscDstoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Server Tcp Object and
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
AnscDstoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_DAEMON_SERVER_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_DAEMON_SERVER_TCP_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_DAEMON_SERVER_TCP_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_DAEMON_SERVER_TCP_OID;
    pBaseObject->Create            = AnscDstoCreate;
    pBaseObject->Remove            = AnscDstoRemove;
    pBaseObject->EnrollObjects     = AnscDstoEnrollObjects;
    pBaseObject->Initialize        = AnscDstoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoRemove
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
AnscDstoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscCoVer3Remove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoEnrollObjects
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
AnscDstoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_SDTO_PATIENT_OBJECT       pSdtoPatient = (PANSC_SDTO_PATIENT_OBJECT     )pMyObject->hSdtoPatient;

    AnscCoVer3EnrollObjects((ANSC_HANDLE)pMyObject);

    if ( !pSdtoPatient )
    {
        pSdtoPatient = (PANSC_SDTO_PATIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_SDTO_PATIENT_OBJECT));

        if ( !pSdtoPatient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSdtoPatient = (ANSC_HANDLE)pSdtoPatient;
        }

        pSdtoPatient->hPatientContext  = (ANSC_HANDLE)pMyObject;
        pSdtoPatient->Overview         = AnscDstoPoOverview;
        pSdtoPatient->Snapshot         = AnscDstoPoSnapshot;
        pSdtoPatient->TraceTransaction = AnscDstoPoTraceTransaction;
        pSdtoPatient->EngineCloseUp    = AnscDstoPoEngineCloseUp;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoInitialize
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
AnscDstoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoVer3Initialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                   = ANSC_DAEMON_SERVER_TCP_OID;
    pMyObject->Create                = AnscDstoCreate;
    pMyObject->Remove                = AnscDstoRemove;
    pMyObject->EnrollObjects         = AnscDstoEnrollObjects;
    pMyObject->Initialize            = AnscDstoInitialize;
    pMyObject->Shutdown              = AnscDstoShutdown;

    pMyObject->Socket                = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->hWorker               = (ANSC_HANDLE)NULL;
    pMyObject->MaxMessageSize        = ANSC_DSTO_MAX_MESSAGE_SIZE;
    pMyObject->EngineCount           = ANSC_DSTO_DEF_ENGINE_COUNT;
    pMyObject->hNextEngineToUse      = (ANSC_HANDLE)NULL;
    pMyObject->CurSocketCount        = 0;
    pMyObject->MinSocketCount        = ANSC_DSTO_MIN_SOCKET_COUNT;
    pMyObject->MaxSocketCount        = ANSC_DSTO_MAX_SOCKET_COUNT;
    pMyObject->Mode                  = ANSC_DSTO_MODE_COMPACT;
    pMyObject->StartTime             = 0;
    pMyObject->bActive               = FALSE;

    pMyObject->hTlsScsIf             = (ANSC_HANDLE)NULL;
    pMyObject->bTlsEnabled           = FALSE;
    pMyObject->bTlsReqCert           = FALSE;

    pMyObject->TccCount              = 0;
    pMyObject->MccCount              = 0;
    pMyObject->TrcCount              = 0;
    pMyObject->MrcCount              = 0;
    pMyObject->TscCount              = 0;
    pMyObject->MscCount              = 0;

    pMyObject->GetHostAddress        = AnscDstoGetHostAddress;
    pMyObject->SetHostAddress        = AnscDstoSetHostAddress;
    pMyObject->GetHostPort           = AnscDstoGetHostPort;
    pMyObject->SetHostPort           = AnscDstoSetHostPort;

    pMyObject->GetSdtoPatient        = AnscDstoGetSdtoPatient;
    pMyObject->GetWorker             = AnscDstoGetWorker;
    pMyObject->SetWorker             = AnscDstoSetWorker;
    pMyObject->GetMaxMessageSize     = AnscDstoGetMaxMessageSize;
    pMyObject->SetMaxMessageSize     = AnscDstoSetMaxMessageSize;
    pMyObject->GetEngineCount        = AnscDstoGetEngineCount;
    pMyObject->SetEngineCount        = AnscDstoSetEngineCount;
    pMyObject->GetMinSocketCount     = AnscDstoGetMinSocketCount;
    pMyObject->SetMinSocketCount     = AnscDstoSetMinSocketCount;
    pMyObject->GetMaxSocketCount     = AnscDstoGetMaxSocketCount;
    pMyObject->SetMaxSocketCount     = AnscDstoSetMaxSocketCount;
    pMyObject->GetMode               = AnscDstoGetMode;
    pMyObject->SetMode               = AnscDstoSetMode;
    pMyObject->GetTlsScsIf           = AnscDstoGetTlsScsIf;
    pMyObject->SetTlsScsIf           = AnscDstoSetTlsScsIf;
    pMyObject->Reset                 = AnscDstoReset;

    pMyObject->AcceptTask            = AnscDstoAcceptTask;
    pMyObject->WorkerTask            = AnscDstoWorkerTask;
    pMyObject->AsyncJobTask          = AnscDstoAsyncJobTask;

    pMyObject->Engage                = AnscDstoEngage;
    pMyObject->Cancel                = AnscDstoCancel;
    pMyObject->StartEngines          = AnscDstoStartEngines;
    pMyObject->StopEngines           = AnscDstoStopEngines;

    pMyObject->GetSocketByAddress    = AnscDstoGetSocketByAddress;
    pMyObject->GetSocketByOsocket    = AnscDstoGetSocketByOsocket;
    pMyObject->AcquireSocket         = AnscDstoAcquireSocket;
    pMyObject->ReleaseSocket         = AnscDstoReleaseSocket;
    pMyObject->ManufactureSocketPool = AnscDstoManufactureSocketPool;
    pMyObject->DestroySocketPool     = AnscDstoDestroySocketPool;

    pMyObject->AssignEngine          = AnscDstoAssignEngine;
    pMyObject->ManufactureEnginePool = AnscDstoManufactureEnginePool;
    pMyObject->DestroyEnginePool     = AnscDstoDestroyEnginePool;

    pMyObject->Query                 = AnscDstoQuery;
    pMyObject->ProcessSync           = AnscDstoProcessSync;
    pMyObject->ProcessAsync          = AnscDstoProcessAsync;

    pMyObject->PoOverview            = AnscDstoPoOverview;
    pMyObject->PoSnapshot            = AnscDstoPoSnapshot;
    pMyObject->PoTraceTransaction    = AnscDstoPoTraceTransaction;
    pMyObject->PoEngineCloseUp       = AnscDstoPoEngineCloseUp;

    AnscInitializeLock       (&pMyObject->OpLock     );
    AnscInitializeEvent      (&pMyObject->AcceptEvent);
    AnscSetEvent             (&pMyObject->AcceptEvent);

    AnscQueueInitializeHeader(&pMyObject->EngineQueue    );
    AnscInitializeLock       (&pMyObject->EngineQueueLock);
    AnscSListInitializeHeader(&pMyObject->SocketSList    );
    AnscInitializeLock       (&pMyObject->SocketSListLock);
    
    AnscZeroMemory(pMyObject->HostName, sizeof(pMyObject->HostName));

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoShutdown
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function really destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoShutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_SDTO_PATIENT_OBJECT       pSdtoPatient = (PANSC_SDTO_PATIENT_OBJECT     )pMyObject->hSdtoPatient;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;

    if ( pSdtoPatient )
    {
        AnscFreeMemory(pSdtoPatient);

        pMyObject->hSdtoPatient = (ANSC_HANDLE)NULL;
    }

    AnscFreeEvent(&pMyObject->AcceptEvent    );
    AnscFreeLock (&pMyObject->OpLock         );
    AnscFreeLock (&pMyObject->EngineQueueLock);
    AnscFreeLock (&pMyObject->SocketSListLock);

    if ( pWorker )
    {
        AnscFreeMemory(pWorker);

        pMyObject->hWorker = (ANSC_HANDLE)NULL;
    }

    AnscCoVer3Shutdown((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
