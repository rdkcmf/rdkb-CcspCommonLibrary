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

    module:	stun_sco_base.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Stun Simple Client Object.

        *   StunScoCreate
        *   StunScoRemove
        *   StunScoEnrollObjects
        *   StunScoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/30/08    initial revision.

**********************************************************************/


#include "stun_sco_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        StunScoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Stun Simple Client Object and
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
StunScoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSTUN_SIMPLE_CLIENT_OBJECT)AnscAllocateMemory(sizeof(STUN_SIMPLE_CLIENT_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, STUN_SIMPLE_CLIENT_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = STUN_SIMPLE_CLIENT_OID;
    pBaseObject->Create            = StunScoCreate;
    pBaseObject->Remove            = StunScoRemove;
    pBaseObject->EnrollObjects     = StunScoEnrollObjects;
    pBaseObject->Initialize        = StunScoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRemove
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
StunScoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStageTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hStageTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pStageTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hStageTimerIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pRetryTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hRetryTimerIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pProbeTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hProbeTimerIf;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorker1 = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorker1;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpA = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpA;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorkerA = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorkerA;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpB = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpB;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorkerB = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorkerB;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    if ( pStageTimerObj )
    {
        pStageTimerObj->Remove((ANSC_HANDLE)pStageTimerObj);
    }

    if ( pStageTimerIf )
    {
        AnscFreeMemory(pStageTimerIf);
    }

    if ( pRetryTimerObj )
    {
        pRetryTimerObj->Remove((ANSC_HANDLE)pRetryTimerObj);
    }

    if ( pRetryTimerIf )
    {
        AnscFreeMemory(pRetryTimerIf);
    }

    if ( pProbeTimerObj )
    {
        pProbeTimerObj->Remove((ANSC_HANDLE)pProbeTimerObj);
    }

    if ( pProbeTimerIf )
    {
        AnscFreeMemory(pProbeTimerIf);
    }

    if ( pSimpleClientUdp1 )
    {
        pSimpleClientUdp1->Remove((ANSC_HANDLE)pSimpleClientUdp1);

        pMyObject->hSimpleClientUdp1 = (ANSC_HANDLE)NULL;
    }

    if ( pClientUdpWorker1 )
    {
        AnscFreeMemory(pClientUdpWorker1);

        pMyObject->hClientUdpWorker1 = (ANSC_HANDLE)NULL;
    }

    if ( pSimpleClientUdpA )
    {
        pSimpleClientUdpA->Remove((ANSC_HANDLE)pSimpleClientUdpA);

        pMyObject->hSimpleClientUdpA = (ANSC_HANDLE)NULL;
    }

    if ( pClientUdpWorkerA )
    {
        AnscFreeMemory(pClientUdpWorkerA);

        pMyObject->hClientUdpWorkerA = (ANSC_HANDLE)NULL;
    }

    if ( pSimpleClientUdpB )
    {
        pSimpleClientUdpB->Remove((ANSC_HANDLE)pSimpleClientUdpB);

        pMyObject->hSimpleClientUdpB = (ANSC_HANDLE)NULL;
    }

    if ( pClientUdpWorkerB )
    {
        AnscFreeMemory(pClientUdpWorkerB);

        pMyObject->hClientUdpWorkerB = (ANSC_HANDLE)NULL;
    }

    AnscFreeEvent(&pMyObject->SocketEventA);
    AnscCoRemove ((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoEnrollObjects
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
StunScoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStageTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hStageTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pStageTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hStageTimerIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pRetryTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hRetryTimerIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pProbeTimerIf     = (PANSC_TDO_CLIENT_OBJECT       )pMyObject->hProbeTimerIf;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorker1 = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorker1;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpA = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpA;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorkerA = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorkerA;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpB = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpB;
    PANSC_SCUO_WORKER_OBJECT        pClientUdpWorkerB = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hClientUdpWorkerB;

    if ( !pStageTimerObj )
    {
        pStageTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pStageTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hStageTimerObj = (ANSC_HANDLE)pStageTimerObj;
        }

        pStageTimerObj->SetTimerType((ANSC_HANDLE)pStageTimerObj, ANSC_TIMER_TYPE_SPORADIC     );
        pStageTimerObj->SetInterval ((ANSC_HANDLE)pStageTimerObj, STUN_SCO_STAGE_TIMER_INTERVAL);
    }

    if ( !pStageTimerIf )
    {
        pStageTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pStageTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hStageTimerIf = (ANSC_HANDLE)pStageTimerIf;
        }

        pStageTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pStageTimerIf->Invoke         = StunScoStageTimerInvoke;

        pStageTimerObj->SetClient((ANSC_HANDLE)pStageTimerObj, (ANSC_HANDLE)pStageTimerIf);
    }

    if ( !pRetryTimerObj )
    {
        pRetryTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pRetryTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRetryTimerObj = (ANSC_HANDLE)pRetryTimerObj;
        }

        pRetryTimerObj->SetTimerType((ANSC_HANDLE)pRetryTimerObj, ANSC_TIMER_TYPE_SPORADIC        );
        pRetryTimerObj->SetInterval ((ANSC_HANDLE)pRetryTimerObj, STUN_MIN_RETRANSMISSION_INTERVAL);
    }

    if ( !pRetryTimerIf )
    {
        pRetryTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pRetryTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRetryTimerIf = (ANSC_HANDLE)pRetryTimerIf;
        }

        pRetryTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pRetryTimerIf->Invoke         = StunScoRetryTimerInvoke;

        pRetryTimerObj->SetClient((ANSC_HANDLE)pRetryTimerObj, (ANSC_HANDLE)pRetryTimerIf);
    }

    if ( !pProbeTimerObj )
    {
        pProbeTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pProbeTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hProbeTimerObj = (ANSC_HANDLE)pProbeTimerObj;
        }

        pProbeTimerObj->SetTimerType((ANSC_HANDLE)pProbeTimerObj, ANSC_TIMER_TYPE_PERIODIC           );
        pProbeTimerObj->SetInterval ((ANSC_HANDLE)pProbeTimerObj, STUN_MIN_KEEP_ALIVE_INTERVAL * 1000);
    }

    if ( !pProbeTimerIf )
    {
        pProbeTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pProbeTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hProbeTimerIf = (ANSC_HANDLE)pProbeTimerIf;
        }

        pProbeTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pProbeTimerIf->Invoke         = StunScoProbeTimerInvoke;

        pProbeTimerObj->SetClient((ANSC_HANDLE)pProbeTimerObj, (ANSC_HANDLE)pProbeTimerIf);
    }

    if ( !pSimpleClientUdp1 )
    {
        pSimpleClientUdp1 =
            (PANSC_SIMPLE_CLIENT_UDP_OBJECT)AnscCreateSimpleClientUdp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSimpleClientUdp1 )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSimpleClientUdp1 = (ANSC_HANDLE)pSimpleClientUdp1;
        }
    }

    if ( !pClientUdpWorker1 )
    {
        pClientUdpWorker1 = (PANSC_SCUO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_SCUO_WORKER_OBJECT));

        if ( !pClientUdpWorker1 )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hClientUdpWorker1 = (ANSC_HANDLE)pClientUdpWorker1;
        }

        pClientUdpWorker1->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pClientUdpWorker1->Query          = StunScoScuowo1Query;
        pClientUdpWorker1->Process        = StunScoScuowo1Process;
        pClientUdpWorker1->Notify         = StunScoScuowo1Notify;

        pSimpleClientUdp1->SetWorker
            (
                (ANSC_HANDLE)pSimpleClientUdp1,
                (ANSC_HANDLE)pClientUdpWorker1,
                sizeof(ANSC_SCUO_WORKER_OBJECT)
            );
    }

    if ( !pSimpleClientUdpA )
    {
        pSimpleClientUdpA =
            (PANSC_SIMPLE_CLIENT_UDP_OBJECT)AnscCreateSimpleClientUdp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSimpleClientUdpA )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSimpleClientUdpA = (ANSC_HANDLE)pSimpleClientUdpA;
        }
    }

    if ( !pClientUdpWorkerA )
    {
        pClientUdpWorkerA = (PANSC_SCUO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_SCUO_WORKER_OBJECT));

        if ( !pClientUdpWorkerA )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hClientUdpWorkerA = (ANSC_HANDLE)pClientUdpWorkerA;
        }

        pClientUdpWorkerA->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pClientUdpWorkerA->Query          = StunScoScuowoAQuery;
        pClientUdpWorkerA->Process        = StunScoScuowoAProcess;
        pClientUdpWorkerA->Notify         = StunScoScuowoANotify;

        pSimpleClientUdpA->SetWorker
            (
                (ANSC_HANDLE)pSimpleClientUdpA,
                (ANSC_HANDLE)pClientUdpWorkerA,
                sizeof(ANSC_SCUO_WORKER_OBJECT)
            );
    }

    if ( !pSimpleClientUdpB )
    {
        pSimpleClientUdpB =
            (PANSC_SIMPLE_CLIENT_UDP_OBJECT)AnscCreateSimpleClientUdp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSimpleClientUdpB )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSimpleClientUdpB = (ANSC_HANDLE)pSimpleClientUdpB;
        }
    }

    if ( !pClientUdpWorkerB )
    {
        pClientUdpWorkerB = (PANSC_SCUO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_SCUO_WORKER_OBJECT));

        if ( !pClientUdpWorkerB )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hClientUdpWorkerB = (ANSC_HANDLE)pClientUdpWorkerB;
        }

        pClientUdpWorkerB->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pClientUdpWorkerB->Query          = StunScoScuowoBQuery;
        pClientUdpWorkerB->Process        = StunScoScuowoBProcess;
        pClientUdpWorkerB->Notify         = StunScoScuowoBNotify;

        pSimpleClientUdpB->SetWorker
            (
                (ANSC_HANDLE)pSimpleClientUdpB,
                (ANSC_HANDLE)pClientUdpWorkerB,
                sizeof(ANSC_SCUO_WORKER_OBJECT)
            );
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoInitialize
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
StunScoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT)hThisObject;

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
    pMyObject->Oid                       = STUN_SIMPLE_CLIENT_OID;
    pMyObject->Create                    = StunScoCreate;
    pMyObject->Remove                    = StunScoRemove;
    pMyObject->EnrollObjects             = StunScoEnrollObjects;
    pMyObject->Initialize                = StunScoInitialize;

    pMyObject->hStunBsmIf                = (ANSC_HANDLE)NULL;
    pMyObject->AsyncTaskCount            = 0;
    pMyObject->bActive                   = FALSE;

    pMyObject->ClientStage               = STUN_SCO_CLIENT_STAGE_None;
    pMyObject->ClientState               = STUN_SCO_CLIENT_STATE_Idle;
    pMyObject->SocketState1              = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->SocketStateA              = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->SocketStateB              = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->RetransInterval1          = STUN_MIN_RETRANSMISSION_INTERVAL;
    pMyObject->RetransTime1              = 0;
    pMyObject->LastSendMsg1              = NULL;
    pMyObject->LastMsgSize1              = 0;
    pMyObject->bMsgIntegrity1            = FALSE;
    pMyObject->bMsgIntegrityA            = FALSE;
    pMyObject->bMsgIntegrityB            = FALSE;

    pMyObject->GetStunBsmIf              = StunScoGetStunBsmIf;
    pMyObject->SetStunBsmIf              = StunScoSetStunBsmIf;
    pMyObject->GetClientAddr             = StunScoGetClientAddr;
    pMyObject->SetClientAddr             = StunScoSetClientAddr;
    pMyObject->GetClientPort             = StunScoGetClientPort;
    pMyObject->SetClientPort             = StunScoSetClientPort;
    pMyObject->GetServerAddr             = StunScoGetServerAddr;
    pMyObject->SetServerAddr             = StunScoSetServerAddr;
    pMyObject->GetServerPort             = StunScoGetServerPort;
    pMyObject->SetServerPort             = StunScoSetServerPort;
    pMyObject->GetUsername               = StunScoGetUsername;
    pMyObject->SetUsername               = StunScoSetUsername;
    pMyObject->GetPassword               = StunScoGetPassword;
    pMyObject->SetPassword               = StunScoSetPassword;
    pMyObject->GetMinKeepAliveInterval   = StunScoGetMinKeepAliveInterval;
    pMyObject->SetMinKeepAliveInterval   = StunScoSetMinKeepAliveInterval;
    pMyObject->GetMaxKeepAliveInterval   = StunScoGetMaxKeepAliveInterval;
    pMyObject->SetMaxKeepAliveInterval   = StunScoSetMaxKeepAliveInterval;

    pMyObject->GetProperty               = StunScoGetProperty;
    pMyObject->SetProperty               = StunScoSetProperty;
    pMyObject->ResetProperty             = StunScoResetProperty;
    pMyObject->Reset                     = StunScoReset;

    pMyObject->Engage                    = StunScoEngage;
    pMyObject->Cancel                    = StunScoCancel;
    pMyObject->StageTimerInvoke          = StunScoStageTimerInvoke;
    pMyObject->RetryTimerInvoke          = StunScoRetryTimerInvoke;
    pMyObject->ProbeTimerInvoke          = StunScoProbeTimerInvoke;

    pMyObject->PlayRole1                 = StunScoPlayRole1;
    pMyObject->DiscoverBinding           = StunScoDiscoverBinding;
    pMyObject->DiscoverBindingTimeoutA   = StunScoDiscoverBindingTimeoutA;
    pMyObject->DiscoverBindingTimeoutB   = StunScoDiscoverBindingTimeoutB;
    pMyObject->MaintainBinding           = StunScoMaintainBinding;
    pMyObject->NotifyBindingChange       = StunScoNotifyBindingChange;
    pMyObject->AsyncDiscoverTask         = StunScoAsyncDiscoverTask;

    pMyObject->RecvBindingResponse1      = StunScoRecvBindingResponse1;
    pMyObject->RecvBindingResponseA      = StunScoRecvBindingResponseA;
    pMyObject->RecvBindingResponseB      = StunScoRecvBindingResponseB;
    pMyObject->RecvBindingErrorResponse1 = StunScoRecvBindingErrorResponse1;
    pMyObject->RecvBindingErrorResponseA = StunScoRecvBindingErrorResponseA;
    pMyObject->RecvBindingErrorResponseB = StunScoRecvBindingErrorResponseB;
    pMyObject->VerifyMsgIntegrity        = StunScoVerifyMsgIntegrity;

    pMyObject->SendBindingRequest1       = StunScoSendBindingRequest1;
    pMyObject->SendBindingRequestA       = StunScoSendBindingRequestA;
    pMyObject->SendBindingRequestB       = StunScoSendBindingRequestB;

    pMyObject->Scuowo1Query              = StunScoScuowo1Query;
    pMyObject->Scuowo1Process            = StunScoScuowo1Process;
    pMyObject->Scuowo1Notify             = StunScoScuowo1Notify;
    pMyObject->ScuowoAQuery              = StunScoScuowoAQuery;
    pMyObject->ScuowoAProcess            = StunScoScuowoAProcess;
    pMyObject->ScuowoANotify             = StunScoScuowoANotify;
    pMyObject->ScuowoBQuery              = StunScoScuowoBQuery;
    pMyObject->ScuowoBProcess            = StunScoScuowoBProcess;
    pMyObject->ScuowoBNotify             = StunScoScuowoBNotify;

    StunInitBindingInfo((&pMyObject->BindingInfo));

    AnscInitializeEvent(&pMyObject->SocketEventA);
    AnscResetEvent     (&pMyObject->SocketEventA);

    /*
     * We shall initialize the object properties to the default values, which may be changed later
     * via the exposed member functions. If any of the future extensions needs to change the object
     * property, the following code also needs to be changed.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
