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

    module:	ansc_bkto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Broker Socket Tcp Object.

        *   AnscBktoCreate
        *   AnscBktoRemove
        *   AnscBktoEnrollObjects
        *   AnscBktoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#include "ansc_bkto_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscBktoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Broker Socket Tcp Object and
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
AnscBktoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = NULL;
    errno_t                         rc           = -1;
    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_BROKER_SOCKET_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_BROKER_SOCKET_TCP_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), ANSC_BROKER_SOCKET_TCP_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_BROKER_SOCKET_TCP_OID;
    pBaseObject->Create            = AnscBktoCreate;
    pBaseObject->Remove            = AnscBktoRemove;
    pBaseObject->EnrollObjects     = AnscBktoEnrollObjects;
    pBaseObject->Initialize        = AnscBktoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoRemove
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
AnscBktoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->Close((ANSC_HANDLE)pMyObject);
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pMyObject->RecvBuffer )
    {
        AnscFreeMemory(pMyObject->RecvBuffer);

        pMyObject->RecvBuffer = NULL;
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoEnrollObjects
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
AnscBktoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoInitialize
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
AnscBktoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

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
    pMyObject->Oid               = ANSC_BROKER_SOCKET_TCP_OID;
    pMyObject->Create            = AnscBktoCreate;
    pMyObject->Remove            = AnscBktoRemove;
    pMyObject->EnrollObjects     = AnscBktoEnrollObjects;
    pMyObject->Initialize        = AnscBktoInitialize;

    pMyObject->Socket            = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->HashIndex         = 0;

    pMyObject->hBrokerServer     = (ANSC_HANDLE)NULL;
    pMyObject->hBrokerEngine     = (ANSC_HANDLE)NULL;
    pMyObject->hClientContext    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount    = 0;
    pMyObject->SendBytesCount    = 0;
    pMyObject->LastRecvAt        = 0;
    pMyObject->LastSendAt        = 0;
    pMyObject->StartTime         = 0;
    pMyObject->bClosed           = TRUE;
    pMyObject->bBroken           = FALSE;
    pMyObject->bToBeCleaned      = FALSE;

    pMyObject->hRecvBufferTag    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBuffer        = NULL;
    pMyObject->RecvBufferSize    = ANSC_BKTO_MAX_BUFFER_SIZE;
    pMyObject->RecvPacketSize    = 0;
    pMyObject->RecvOffset        = 0;

    pMyObject->GetHostAddress    = AnscBktoGetHostAddress;
    pMyObject->SetHostAddress    = AnscBktoSetHostAddress;
    pMyObject->GetHostPort       = AnscBktoGetHostPort;
    pMyObject->SetHostPort       = AnscBktoSetHostPort;
    pMyObject->GetPeerAddress    = AnscBktoGetPeerAddress;
    pMyObject->SetPeerAddress    = AnscBktoSetPeerAddress;
    pMyObject->GetPeerPort       = AnscBktoGetPeerPort;
    pMyObject->SetPeerPort       = AnscBktoSetPeerPort;

    pMyObject->GetBrokerServer   = AnscBktoGetBrokerServer;
    pMyObject->SetBrokerServer   = AnscBktoSetBrokerServer;
    pMyObject->GetBrokerEngine   = AnscBktoGetBrokerEngine;
    pMyObject->SetBrokerEngine   = AnscBktoSetBrokerEngine;
    pMyObject->GetClientContext  = AnscBktoGetClientContext;
    pMyObject->SetClientContext  = AnscBktoSetClientContext;
    pMyObject->GetBufferSize     = AnscBktoGetBufferSize;
    pMyObject->SetBufferSize     = AnscBktoSetBufferSize;
    pMyObject->GetBufferContext  = AnscBktoGetBufferContext;
    pMyObject->SetBufferContext  = AnscBktoSetBufferContext;
    pMyObject->IsToBeCleaned     = AnscBktoIsToBeCleaned;
    pMyObject->ToClean           = AnscBktoToClean;
    pMyObject->Return            = AnscBktoReturn;
    pMyObject->Reset             = AnscBktoReset;

    pMyObject->Finish            = AnscBktoFinish;
    pMyObject->Open              = AnscBktoOpen;
    pMyObject->Close             = AnscBktoClose;

    pMyObject->GetRecvBuffer     = AnscBktoGetRecvBuffer;
    pMyObject->Recv              = AnscBktoRecv;
    pMyObject->Send              = AnscBktoSend;

    return  ANSC_STATUS_SUCCESS;
}
