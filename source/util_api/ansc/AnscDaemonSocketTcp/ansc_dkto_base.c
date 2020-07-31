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

    module:	ansc_dkto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Daemon Socket Tcp Object.

        *   AnscDktoCreate
        *   AnscDktoRemove
        *   AnscDktoEnrollObjects
        *   AnscDktoInitialize

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


#include "ansc_dkto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscDktoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Socket Tcp Object and
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
AnscDktoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_DAEMON_SOCKET_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_DAEMON_SOCKET_TCP_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_DAEMON_SOCKET_TCP_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_DAEMON_SOCKET_TCP_OID;
    pBaseObject->Create            = AnscDktoCreate;
    pBaseObject->Remove            = AnscDktoRemove;
    pBaseObject->EnrollObjects     = AnscDktoEnrollObjects;
    pBaseObject->Initialize        = AnscDktoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoRemove
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
AnscDktoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->Close((ANSC_HANDLE)pMyObject);
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pMyObject->RecvBuffer )
    {
        AnscFreeMemory(pMyObject->RecvBuffer);

        pMyObject->RecvBuffer = NULL;
    }

#ifndef _ANSC_USE_OPENSSL_
    if ( TRUE )
    {
	    PTLS_TSA_INTERFACE              pTlsTsaIf    = (PTLS_TSA_INTERFACE            )pMyObject->hTlsTsaIf;
	    if ( pTlsTsaIf )
	    {
	        AnscFreeMemory(pTlsTsaIf);
	    }

	    AnscSetEvent (&pMyObject->TlsConnEvent);
	    AnscFreeEvent(&pMyObject->TlsConnEvent);
	    AnscFreeLock (&pMyObject->TlsConnLock );
    }
#endif

    AnscCoRemove ((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoEnrollObjects
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
AnscDktoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

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

        pTlsTsaIf->RecvAppMessage = AnscDktoTsaRecvAppMessage;
        pTlsTsaIf->SendTlsMessage = AnscDktoTsaSendTlsMessage;
        pTlsTsaIf->NotifyEvent    = AnscDktoTsaNotifyEvent;
    }
#endif

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoInitialize
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
AnscDktoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

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
    pMyObject->Oid               = ANSC_DAEMON_SOCKET_TCP_OID;
    pMyObject->Create            = AnscDktoCreate;
    pMyObject->Remove            = AnscDktoRemove;
    pMyObject->EnrollObjects     = AnscDktoEnrollObjects;
    pMyObject->Initialize        = AnscDktoInitialize;

    pMyObject->Socket            = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->HashIndex         = 0;

    pMyObject->hDaemonServer     = (ANSC_HANDLE)NULL;
    pMyObject->hDaemonEngine     = (ANSC_HANDLE)NULL;
    pMyObject->hClientContext    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount    = 0;
    pMyObject->SendBytesCount    = 0;
    pMyObject->LastRecvAt        = 0;
    pMyObject->LastSendAt        = 0;
    pMyObject->StartTime         = 0;
    pMyObject->bClosed           = TRUE;
    pMyObject->bBroken           = FALSE;
    pMyObject->bToBeCleaned      = FALSE;
    pMyObject->bLocked           = FALSE;

    pMyObject->hRecvBufferTag    = (ANSC_HANDLE)NULL;
    pMyObject->RecvBuffer        = NULL;
    pMyObject->RecvBufferSize    = ANSC_DKTO_MAX_BUFFER_SIZE;
    pMyObject->RecvPacketSize    = 0;
    pMyObject->RecvOffset        = 0;

    pMyObject->hTlsScsIf         = (ANSC_HANDLE)NULL;
    pMyObject->hTlsConnection    = (ANSC_HANDLE)NULL;
    pMyObject->bTlsEnabled       = FALSE;
    pMyObject->bTlsInitializing  = FALSE;
    pMyObject->bTlsConnected     = FALSE;

    pMyObject->GetPeerAddress    = AnscDktoGetPeerAddress;
    pMyObject->SetPeerAddress    = AnscDktoSetPeerAddress;
    pMyObject->GetPeerPort       = AnscDktoGetPeerPort;
    pMyObject->SetPeerPort       = AnscDktoSetPeerPort;

    pMyObject->GetDaemonServer   = AnscDktoGetDaemonServer;
    pMyObject->SetDaemonServer   = AnscDktoSetDaemonServer;
    pMyObject->GetDaemonEngine   = AnscDktoGetDaemonEngine;
    pMyObject->SetDaemonEngine   = AnscDktoSetDaemonEngine;
    pMyObject->GetClientContext  = AnscDktoGetClientContext;
    pMyObject->SetClientContext  = AnscDktoSetClientContext;
    pMyObject->GetBufferSize     = AnscDktoGetBufferSize;
    pMyObject->SetBufferSize     = AnscDktoSetBufferSize;
    pMyObject->GetBufferContext  = AnscDktoGetBufferContext;
    pMyObject->SetBufferContext  = AnscDktoSetBufferContext;
    pMyObject->GetTlsScsIf       = AnscDktoGetTlsScsIf;
    pMyObject->SetTlsScsIf       = AnscDktoSetTlsScsIf;
    pMyObject->IsToBeCleaned     = AnscDktoIsToBeCleaned;
    pMyObject->ToClean           = AnscDktoToClean;
    pMyObject->Return            = AnscDktoReturn;
    pMyObject->Reset             = AnscDktoReset;

    pMyObject->Finish            = AnscDktoFinish;
    pMyObject->Open              = AnscDktoOpen;
    pMyObject->Close             = AnscDktoClose;
    pMyObject->InitTlsServer     = AnscDktoInitTlsServer;
    pMyObject->OpenTlsServer     = AnscDktoOpenTlsServer;
    pMyObject->Lock              = AnscDktoLock;
    pMyObject->Unlock            = AnscDktoUnlock;
    pMyObject->ResetTimestamp    = AnscDktoResetTimestamp;

    pMyObject->GetRecvBuffer     = AnscDktoGetRecvBuffer;
    pMyObject->Recv              = AnscDktoRecv;
    pMyObject->Send              = AnscDktoSend;

#ifndef _ANSC_USE_OPENSSL_
    pMyObject->TsaRecvAppMessage = AnscDktoTsaRecvAppMessage;
    pMyObject->TsaSendTlsMessage = AnscDktoTsaSendTlsMessage;
    pMyObject->TsaNotifyEvent    = AnscDktoTsaNotifyEvent;

    AnscInitializeEvent(&pMyObject->TlsConnEvent);
    AnscResetEvent     (&pMyObject->TlsConnEvent);
    AnscInitializeLock (&pMyObject->TlsConnLock );
#endif

    return  ANSC_STATUS_SUCCESS;
}
