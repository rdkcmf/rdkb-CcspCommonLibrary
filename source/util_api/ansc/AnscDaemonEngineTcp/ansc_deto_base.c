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

    module:	ansc_deto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoCreate
        *   AnscDetoRemove
        *   AnscDetoEnrollObjects
        *   AnscDetoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/09/01    initial revision.

**********************************************************************/


#include "ansc_deto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscDetoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Engine Tcp Object and
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
AnscDetoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_DAEMON_ENGINE_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_DAEMON_ENGINE_TCP_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_DAEMON_ENGINE_TCP_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_DAEMON_ENGINE_TCP_OID;
    pBaseObject->Create            = AnscDetoCreate;
    pBaseObject->Remove            = AnscDetoRemove;
    pBaseObject->EnrollObjects     = AnscDetoEnrollObjects;
    pBaseObject->Initialize        = AnscDetoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoRemove
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
AnscDetoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    pMyObject->Stop ((ANSC_HANDLE)pMyObject);
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeEvent(&pMyObject->RecvEvent        );
    AnscFreeEvent(&pMyObject->SendEvent        );
    AnscFreeEvent(&pMyObject->NewSocketEvent   );
    AnscFreeLock (&pMyObject->RecvSocketSetLock);
    AnscFreeLock (&pMyObject->SendSocketSetLock);
    AnscFreeLock (&pMyObject->SocketTableLock  );
    AnscFreeLock (&pMyObject->PacketQueueLock  );

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoEnrollObjects
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
AnscDetoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoInitialize
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
AnscDetoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
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
    pMyObject->Oid                 = ANSC_DAEMON_ENGINE_TCP_OID;
    pMyObject->Create              = AnscDetoCreate;
    pMyObject->Remove              = AnscDetoRemove;
    pMyObject->EnrollObjects       = AnscDetoEnrollObjects;
    pMyObject->Initialize          = AnscDetoInitialize;

    pMyObject->hDaemonServer       = (ANSC_HANDLE)NULL;
    pMyObject->CurSocketCount      = 0;
    pMyObject->MaxSocketCount      = ANSC_DETO_MAX_SOCKET_COUNT;
    pMyObject->ControlFlags        = 0;
    pMyObject->StartTime           = 0;
    pMyObject->AvgServingTime      = 0;
    pMyObject->SocketTimeOut       = ANSC_DETO_SOCKET_TIMEOUT;
    pMyObject->bStarted            = FALSE;
    pMyObject->bCleaningDemanded   = FALSE;
    pMyObject->bBusy               = FALSE;

    pMyObject->TtcCount            = 0;
    pMyObject->MtcCount            = 0;
    pMyObject->TrcCount            = 0;
    pMyObject->MrcCount            = 0;
    pMyObject->TscCount            = 0;
    pMyObject->MscCount            = 0;

    pMyObject->GetDaemonServer     = AnscDetoGetDaemonServer;
    pMyObject->SetDaemonServer     = AnscDetoSetDaemonServer;
    pMyObject->GetMaxSocketCount   = AnscDetoGetMaxSocketCount;
    pMyObject->SetMaxSocketCount   = AnscDetoSetMaxSocketCount;
    pMyObject->GetControlFlags     = AnscDetoGetControlFlags;
    pMyObject->SetControlFlags     = AnscDetoSetControlFlags;
    pMyObject->Reset               = AnscDetoReset;

    pMyObject->GetSocketByAddress  = AnscDetoGetSocketByAddress;
    pMyObject->GetSocketByAddress2 = AnscDetoGetSocketByAddress2;
    pMyObject->GetSocketByOsocket  = AnscDetoGetSocketByOsocket;
    pMyObject->GetSocketByOsocket2 = AnscDetoGetSocketByOsocket2;
    pMyObject->AddSocket           = AnscDetoAddSocket;
    pMyObject->DelSocket           = AnscDetoDelSocket;
    pMyObject->DelAllSockets       = AnscDetoDelAllSockets;
    pMyObject->ExpAllSockets       = AnscDetoExpAllSockets;
    pMyObject->EnableRecv          = AnscDetoEnableRecv;
    pMyObject->EnableSend          = AnscDetoEnableSend;

    pMyObject->RecvTask            = AnscDetoRecvTask;
    pMyObject->SendTask            = AnscDetoSendTask;

    pMyObject->Start               = AnscDetoStart;
    pMyObject->Stop                = AnscDetoStop;
    pMyObject->Cancel              = AnscDetoCancel;
    pMyObject->Clean               = AnscDetoClean;
    pMyObject->Snapshot            = AnscDetoSnapshot;
    pMyObject->CloseUp             = AnscDetoCloseUp;

    pMyObject->Recv                = AnscDetoRecv;
    pMyObject->Recv2               = AnscDetoRecv2;
    pMyObject->Send                = AnscDetoSend;
    pMyObject->Send2               = AnscDetoSend2;

    AnscInitializeEvent(&pMyObject->RecvEvent     );
    AnscSetEvent       (&pMyObject->RecvEvent     );
    AnscInitializeEvent(&pMyObject->SendEvent     );
    AnscSetEvent       (&pMyObject->SendEvent     );
    AnscInitializeEvent(&pMyObject->NewSocketEvent);
    AnscResetEvent     (&pMyObject->NewSocketEvent);

    AnscInitializeLock(&pMyObject->RecvSocketSetLock);
    AnscInitializeLock(&pMyObject->SendSocketSetLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->SocketTable[i]);
    }
    AnscInitializeLock(&pMyObject->SocketTableLock);

    AnscQueueInitializeHeader(&pMyObject->PacketQueue    );
    AnscInitializeLock       (&pMyObject->PacketQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
