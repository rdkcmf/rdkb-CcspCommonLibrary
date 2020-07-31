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

    module:	ansc_scuo_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Simple Client Udp Object.

        *   AnscScuoCreate
        *   AnscScuoRemove
        *   AnscScuoEnrollObjects
        *   AnscScuoInitialize
        *   AnscScuoShutdown

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/12/01    initial revision.
        04/02/02    upgrade the base object from AnscCo to
                    AnscCoVer3.

**********************************************************************/


#include "ansc_scuo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscScuoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Simple Client Udp Object and
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
AnscScuoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)AnscAllocateMemory(sizeof(ANSC_SIMPLE_CLIENT_UDP_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_SIMPLE_CLIENT_UDP_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_SIMPLE_CLIENT_UDP_OID;
    pBaseObject->Create            = AnscScuoCreate;
    pBaseObject->Remove            = AnscScuoRemove;
    pBaseObject->EnrollObjects     = AnscScuoEnrollObjects;
    pBaseObject->Initialize        = AnscScuoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoRemove
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
AnscScuoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    AnscCoVer3Remove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoEnrollObjects
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
AnscScuoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;

    AnscCoVer3EnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoInitialize
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
AnscScuoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;

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
    pMyObject->Oid               = ANSC_SIMPLE_CLIENT_UDP_OID;
    pMyObject->Create            = AnscScuoCreate;
    pMyObject->Remove            = AnscScuoRemove;
    pMyObject->EnrollObjects     = AnscScuoEnrollObjects;
    pMyObject->Initialize        = AnscScuoInitialize;
    pMyObject->Shutdown          = AnscScuoShutdown;

    pMyObject->Socket            = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->hWorker           = (ANSC_HANDLE)NULL;
    pMyObject->MaxMessageSize    = ANSC_SCUO_MAX_MESSAGE_SIZE;
    pMyObject->Mode              = 0;
    pMyObject->bActive           = FALSE;
    pMyObject->bClosed           = TRUE;

    pMyObject->hClientContext    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount    = 0;
    pMyObject->SendBytesCount    = 0;
    pMyObject->LastRecvAt        = 0;
    pMyObject->LastSendAt        = 0;

    pMyObject->RecvBuffer        = NULL;
    pMyObject->RecvBufferSize    = ANSC_SCUO_MAX_MESSAGE_SIZE;
    pMyObject->RecvPacketSize    = 0;
    pMyObject->RecvOffset        = 0;

    pMyObject->GetHostAddress    = AnscScuoGetHostAddress;
    pMyObject->SetHostAddress    = AnscScuoSetHostAddress;
    pMyObject->GetHostPort       = AnscScuoGetHostPort;
    pMyObject->SetHostPort       = AnscScuoSetHostPort;
    pMyObject->GetPeerAddress    = AnscScuoGetPeerAddress;
    pMyObject->SetPeerAddress    = AnscScuoSetPeerAddress;
    pMyObject->GetPeerPort       = AnscScuoGetPeerPort;
    pMyObject->SetPeerPort       = AnscScuoSetPeerPort;

    pMyObject->GetWorker         = AnscScuoGetWorker;
    pMyObject->SetWorker         = AnscScuoSetWorker;
    pMyObject->GetMaxMessageSize = AnscScuoGetMaxMessageSize;
    pMyObject->SetMaxMessageSize = AnscScuoSetMaxMessageSize;
    pMyObject->GetMode           = AnscScuoGetMode;
    pMyObject->SetMode           = AnscScuoSetMode;
    pMyObject->Reset             = AnscScuoReset;

    pMyObject->RecvTask          = AnscScuoRecvTask;

    pMyObject->Engage            = AnscScuoEngage;
    pMyObject->Cancel            = AnscScuoCancel;

    pMyObject->GetRecvBuffer     = AnscScuoGetRecvBuffer;
    pMyObject->Recv              = AnscScuoRecv;
    pMyObject->Send              = AnscScuoSend;
    pMyObject->Send2             = AnscScuoSend2;

    AnscInitializeLock(&pMyObject->OpLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscScuoShutdown
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
AnscScuoShutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)hThisObject;
    PANSC_SCUO_WORKER_OBJECT        pWorker      = (PANSC_SCUO_WORKER_OBJECT      )pMyObject->hWorker;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pWorker )
    {
        AnscFreeMemory(pWorker);

        pMyObject->hWorker = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock      (&pMyObject->OpLock    );
    AnscCoVer3Shutdown((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
