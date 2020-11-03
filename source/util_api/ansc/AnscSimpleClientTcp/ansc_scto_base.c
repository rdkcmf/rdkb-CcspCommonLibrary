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

    module:	ansc_scto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Simple Client Tcp Object.

        *   AnscSctoCreate
        *   AnscSctoRemove
        *   AnscSctoEnrollObjects
        *   AnscSctoInitialize
        *   AnscSctoShutdown

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/11/01    initial revision.
        04/02/02    upgrade the base object from AnscCo to
                    AnscCoVer3.

**********************************************************************/


#include "ansc_scto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscSctoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Simple Client Tcp Object and
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
AnscSctoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_SIMPLE_CLIENT_TCP_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_SIMPLE_CLIENT_TCP_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_SIMPLE_CLIENT_TCP_OID;
    pBaseObject->Create            = AnscSctoCreate;
    pBaseObject->Remove            = AnscSctoRemove;
    pBaseObject->EnrollObjects     = AnscSctoEnrollObjects;
    pBaseObject->Initialize        = AnscSctoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoRemove
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
AnscSctoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    AnscCoVer3Remove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoEnrollObjects
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
AnscSctoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

#ifndef _ANSC_USE_OPENSSL_
    PTLS_TSA_INTERFACE              pTlsTsaIf    = (PTLS_TSA_INTERFACE            )pMyObject->hTlsTsaIf;

    if ( !pTlsTsaIf )
    {
        pTlsTsaIf = (PTLS_TSA_INTERFACE)AnscAllocateMemory(sizeof(TLS_TSA_INTERFACE));

        if ( !pTlsTsaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsTsaIf = (ANSC_HANDLE)pTlsTsaIf;
        }

        pTlsTsaIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pTlsTsaIf->Size           = sizeof(TLS_TSA_INTERFACE);

        pTlsTsaIf->RecvAppMessage = AnscSctoTsaRecvAppMessage;
        pTlsTsaIf->SendTlsMessage = AnscSctoTsaSendTlsMessage;
        pTlsTsaIf->NotifyEvent    = AnscSctoTsaNotifyEvent;
    }
#endif

    AnscCoVer3EnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoInitialize
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
AnscSctoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

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
    pMyObject->Oid               = ANSC_SIMPLE_CLIENT_TCP_OID;
    pMyObject->Create            = AnscSctoCreate;
    pMyObject->Remove            = AnscSctoRemove;
    pMyObject->EnrollObjects     = AnscSctoEnrollObjects;
    pMyObject->Initialize        = AnscSctoInitialize;
    pMyObject->Shutdown          = AnscSctoShutdown;

    pMyObject->Socket            = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->hWorker           = (ANSC_HANDLE)NULL;
    pMyObject->MaxMessageSize    = ANSC_SCTO_MAX_MESSAGE_SIZE;
    pMyObject->Mode              = 0;
    pMyObject->bActive           = FALSE;
    pMyObject->bClosed           = TRUE;
    pMyObject->bBroken           = FALSE;
    pMyObject->LastEvent         = 0;
    pMyObject->LastError         = 0;

    pMyObject->hClientContext    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount    = 0;
    pMyObject->SendBytesCount    = 0;
    pMyObject->LastRecvAt        = 0;
    pMyObject->LastSendAt        = 0;

    pMyObject->RecvBuffer        = NULL;
    pMyObject->RecvBufferSize    = ANSC_SCTO_MAX_MESSAGE_SIZE;
    pMyObject->RecvPacketSize    = 0;
    pMyObject->RecvOffset        = 0;

    pMyObject->hTlsScsIf         = (ANSC_HANDLE)NULL;
    pMyObject->hTlsConnection    = (ANSC_HANDLE)NULL;
    pMyObject->bTlsEnabled       = FALSE;
    pMyObject->bTlsConnected     = FALSE;

    pMyObject->bSocketBindToDevice   = FALSE;
    pMyObject->GetSocketBindToDevice = AnscSctoGetSocketBindToDevice;
    pMyObject->SetSocketBindToDevice = AnscSctoSetSocketBindToDevice;
    pMyObject->GetSocketDeviceName   = AnscSctoGetSocketDeviceName;
    pMyObject->SetSocketDeviceName   = AnscSctoSetSocketDeviceName;
 
    pMyObject->GetHostAddress    = AnscSctoGetHostAddress;
    pMyObject->SetHostAddress    = AnscSctoSetHostAddress;
    pMyObject->GetHostPort       = AnscSctoGetHostPort;
    pMyObject->SetHostPort       = AnscSctoSetHostPort;
    pMyObject->GetPeerName       = AnscSctoGetPeerName;
    pMyObject->SetPeerName       = AnscSctoSetPeerName;
    pMyObject->GetPeerAddress    = AnscSctoGetPeerAddress;
    pMyObject->SetPeerAddress    = AnscSctoSetPeerAddress;
    pMyObject->GetPeerPort       = AnscSctoGetPeerPort;
    pMyObject->SetPeerPort       = AnscSctoSetPeerPort;

    pMyObject->GetHostNames      = AnscSctoGetHostNames;
    pMyObject->SetHostNames      = AnscSctoSetHostNames;

    pMyObject->GetWorker         = AnscSctoGetWorker;
    pMyObject->SetWorker         = AnscSctoSetWorker;
    pMyObject->GetMaxMessageSize = AnscSctoGetMaxMessageSize;
    pMyObject->SetMaxMessageSize = AnscSctoSetMaxMessageSize;
    pMyObject->GetBufferContext  = AnscSctoGetBufferContext;
    pMyObject->SetBufferContext  = AnscSctoSetBufferContext;
    pMyObject->GetMode           = AnscSctoGetMode;
    pMyObject->SetMode           = AnscSctoSetMode;
    pMyObject->GetTlsScsIf       = AnscSctoGetTlsScsIf;
    pMyObject->SetTlsScsIf       = AnscSctoSetTlsScsIf;
    pMyObject->Reset             = AnscSctoReset;

    pMyObject->RecvTask          = AnscSctoRecvTask;

    pMyObject->Engage            = AnscSctoEngage;
    pMyObject->Cancel            = AnscSctoCancel;

    pMyObject->GetRecvBuffer     = AnscSctoGetRecvBuffer;
    pMyObject->Send              = AnscSctoSend;
    pMyObject->Recv              = AnscSctoRecv;
    pMyObject->Send2             = AnscSctoSend2;
#ifndef _ANSC_USE_OPENSSL_
    pMyObject->InitTlsClient     = AnscSctoInitTlsClient;
    pMyObject->Recv2             = AnscSctoRecv2;

    pMyObject->TsaRecvAppMessage = AnscSctoTsaRecvAppMessage;
    pMyObject->TsaSendTlsMessage = AnscSctoTsaSendTlsMessage;
    pMyObject->TsaNotifyEvent    = AnscSctoTsaNotifyEvent;

    AnscInitializeEvent(&pMyObject->TlsConnEvent);
    AnscResetEvent     (&pMyObject->TlsConnEvent);
#endif

    AnscInitializeLock (&pMyObject->OpLock      );

    AnscZeroMemory(pMyObject->HostName, WEB_MAX_HOST_NAME_SIZE);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoShutdown
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
AnscSctoShutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pWorker )
    {
        AnscFreeMemory(pWorker);

        pMyObject->hWorker = (ANSC_HANDLE)NULL;
    }

#ifndef _ANSC_USE_OPENSSL_
    if ( pMyObject->hTlsTsaIf )
    {
        AnscFreeMemory(pMyObject->hTlsTsaIf);
    }

    AnscSetEvent      (&pMyObject->TlsConnEvent);
    AnscFreeEvent     (&pMyObject->TlsConnEvent);
#endif

    AnscFreeLock      (&pMyObject->OpLock      );
    AnscCoVer3Shutdown((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}
