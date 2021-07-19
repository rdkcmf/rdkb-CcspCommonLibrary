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

    module:	ansc_deuo_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoCreate
        *   AnscDeuoRemove
        *   AnscDeuoEnrollObjects
        *   AnscDeuoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/10/01    initial revision.

**********************************************************************/


#include "ansc_deuo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscDeuoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Engine Udp Object and
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
AnscDeuoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_DAEMON_ENGINE_UDP_OBJECT)AnscAllocateMemory(sizeof(ANSC_DAEMON_ENGINE_UDP_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), ANSC_DAEMON_ENGINE_UDP_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_DAEMON_ENGINE_UDP_OID;
    pBaseObject->Create            = AnscDeuoCreate;
    pBaseObject->Remove            = AnscDeuoRemove;
    pBaseObject->EnrollObjects     = AnscDeuoEnrollObjects;
    pBaseObject->Initialize        = AnscDeuoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoRemove
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
AnscDeuoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->Stop ((ANSC_HANDLE)pMyObject);
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeEvent(&pMyObject->RecvEvent      );
    AnscFreeEvent(&pMyObject->SendEvent      );
    AnscFreeEvent(&pMyObject->NewPacketEvent );
    AnscFreeLock (&pMyObject->PacketQueueLock);
    AnscFreeLock (&pMyObject->SocketTableLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoEnrollObjects
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
AnscDeuoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoInitialize
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
AnscDeuoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
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
    pMyObject->Oid               = ANSC_DAEMON_ENGINE_UDP_OID;
    pMyObject->Create            = AnscDeuoCreate;
    pMyObject->Remove            = AnscDeuoRemove;
    pMyObject->EnrollObjects     = AnscDeuoEnrollObjects;
    pMyObject->Initialize        = AnscDeuoInitialize;

    pMyObject->hDaemonServer     = (ANSC_HANDLE)NULL;
    pMyObject->CurSocketCount    = 0;
    pMyObject->MaxSocketCount    = ANSC_DEUO_MAX_SOCKET_COUNT;
    pMyObject->SocketTimeOut     = ANSC_DEUO_SOCKET_TIMEOUT;
    pMyObject->PacketTimeOut     = ANSC_DEUO_PACKET_TIMEOUT;
    pMyObject->ControlFlags      = 0;
    pMyObject->bStarted          = FALSE;

    pMyObject->GetDaemonServer   = AnscDeuoGetDaemonServer;
    pMyObject->SetDaemonServer   = AnscDeuoSetDaemonServer;
    pMyObject->GetMaxSocketCount = AnscDeuoGetMaxSocketCount;
    pMyObject->SetMaxSocketCount = AnscDeuoSetMaxSocketCount;
    pMyObject->GetSocketTimeOut  = AnscDeuoGetSocketTimeOut;
    pMyObject->SetSocketTimeOut  = AnscDeuoSetSocketTimeOut;
    pMyObject->GetPacketTimeOut  = AnscDeuoGetPacketTimeOut;
    pMyObject->SetPacketTimeOut  = AnscDeuoSetPacketTimeOut;
    pMyObject->GetControlFlags   = AnscDeuoGetControlFlags;
    pMyObject->SetControlFlags   = AnscDeuoSetControlFlags;
    pMyObject->Reset             = AnscDeuoReset;

    pMyObject->GetPacket         = AnscDeuoGetPacket;
    pMyObject->AddPacket         = AnscDeuoAddPacket;
    pMyObject->DelPacket         = AnscDeuoDelPacket;
    pMyObject->DelAllPackets     = AnscDeuoDelAllPackets;

    pMyObject->GetSocket         = AnscDeuoGetSocket;
    pMyObject->AddSocket         = AnscDeuoAddSocket;
    pMyObject->DelSocket         = AnscDeuoDelSocket;
    pMyObject->DelAllSockets     = AnscDeuoDelAllSockets;
    pMyObject->EnableRecv        = AnscDeuoEnableRecv;
    pMyObject->EnableSend        = AnscDeuoEnableSend;

    pMyObject->RecvTask          = AnscDeuoRecvTask;
    pMyObject->SendTask          = AnscDeuoSendTask;

    pMyObject->Start             = AnscDeuoStart;
    pMyObject->Stop              = AnscDeuoStop;
    pMyObject->Clean             = AnscDeuoClean;

    pMyObject->Recv              = AnscDeuoRecv;
    pMyObject->Send              = AnscDeuoSend;

    AnscInitializeEvent(&pMyObject->RecvEvent     );
    AnscSetEvent       (&pMyObject->RecvEvent     );
    AnscInitializeEvent(&pMyObject->SendEvent     );
    AnscSetEvent       (&pMyObject->SendEvent     );
    AnscInitializeEvent(&pMyObject->NewPacketEvent);
    AnscResetEvent     (&pMyObject->NewPacketEvent);

    AnscQueueInitializeHeader(&pMyObject->PacketQueue    );
    AnscInitializeLock       (&pMyObject->PacketQueueLock);

    for ( i = 0; i < ANSC_DEUO_SOCKET_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->SocketTable[i]);
    }
    AnscInitializeLock(&pMyObject->SocketTableLock);

    return  ANSC_STATUS_SUCCESS;
}
