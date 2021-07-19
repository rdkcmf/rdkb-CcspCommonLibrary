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

    module:	ansc_dkuo_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Daemon Socket Udp Object.

        *   AnscDkuoCreate
        *   AnscDkuoRemove
        *   AnscDkuoEnrollObjects
        *   AnscDkuoInitialize

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


#include "ansc_dkuo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscDkuoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Socket Udp Object and
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
AnscDkuoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_DAEMON_SOCKET_UDP_OBJECT)AnscAllocateMemory(sizeof(ANSC_DAEMON_SOCKET_UDP_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), ANSC_DAEMON_SOCKET_UDP_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_DAEMON_SOCKET_UDP_OID;
    pBaseObject->Create            = AnscDkuoCreate;
    pBaseObject->Remove            = AnscDkuoRemove;
    pBaseObject->EnrollObjects     = AnscDkuoEnrollObjects;
    pBaseObject->Initialize        = AnscDkuoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoRemove
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
AnscDkuoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

    pMyObject->Close((ANSC_HANDLE)pMyObject);
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->OpLock    );
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoEnrollObjects
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
AnscDkuoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoInitialize
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
AnscDkuoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

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
    pMyObject->Oid               = ANSC_DAEMON_SOCKET_UDP_OID;
    pMyObject->Create            = AnscDkuoCreate;
    pMyObject->Remove            = AnscDkuoRemove;
    pMyObject->EnrollObjects     = AnscDkuoEnrollObjects;
    pMyObject->Initialize        = AnscDkuoInitialize;

    pMyObject->Socket            = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->HashIndex         = 0;

    pMyObject->hDaemonServer     = (ANSC_HANDLE)NULL;
    pMyObject->hDaemonEngine     = (ANSC_HANDLE)NULL;
    pMyObject->hClientContext    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount    = 0;
    pMyObject->SendBytesCount    = 0;
    pMyObject->LastRecvAt        = 0;
    pMyObject->LastSendAt        = 0;
    pMyObject->bClosed           = TRUE;
    pMyObject->bRecvEnabled      = TRUE;
    pMyObject->bSendEnabled      = TRUE;

    pMyObject->GetPeerAddress    = AnscDkuoGetPeerAddress;
    pMyObject->SetPeerAddress    = AnscDkuoSetPeerAddress;
    pMyObject->GetPeerPort       = AnscDkuoGetPeerPort;
    pMyObject->SetPeerPort       = AnscDkuoSetPeerPort;

    pMyObject->GetDaemonServer   = AnscDkuoGetDaemonServer;
    pMyObject->SetDaemonServer   = AnscDkuoSetDaemonServer;
    pMyObject->GetDaemonEngine   = AnscDkuoGetDaemonEngine;
    pMyObject->SetDaemonEngine   = AnscDkuoSetDaemonEngine;
    pMyObject->GetClientContext  = AnscDkuoGetClientContext;
    pMyObject->SetClientContext  = AnscDkuoSetClientContext;
    pMyObject->GetPacket         = AnscDkuoGetPacket;
    pMyObject->SetPacket         = AnscDkuoSetPacket;
    pMyObject->Return            = AnscDkuoReturn;
    pMyObject->Reset             = AnscDkuoReset;

    pMyObject->Finish            = AnscDkuoFinish;
    pMyObject->Open              = AnscDkuoOpen;
    pMyObject->Close             = AnscDkuoClose;
    pMyObject->EnableRecv        = AnscDkuoEnableRecv;
    pMyObject->EnableSend        = AnscDkuoEnableSend;

    pMyObject->Recv              = AnscDkuoRecv;
    pMyObject->Send              = AnscDkuoSend;

    AnscInitializeLock(&pMyObject->OpLock);

    return  ANSC_STATUS_SUCCESS;
}
