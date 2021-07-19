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

    module:	ansc_lpccotcp_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpCreate
        *   AnscLpccoTcpRemove
        *   AnscLpccoTcpEnrollObjects
        *   AnscLpccoTcpInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscLpccoTcpCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Lpcco Tcp Object and
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
AnscLpccoTcpCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_LPCCO_TCP_OBJECT)AnscAllocateMemory(sizeof(ANSC_LPCCO_TCP_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), ANSC_LPCCO_TCP_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_LPCCO_TCP_OID;
    pBaseObject->Create            = AnscLpccoTcpCreate;
    pBaseObject->Remove            = AnscLpccoTcpRemove;
    pBaseObject->EnrollObjects     = AnscLpccoTcpEnrollObjects;
    pBaseObject->Initialize        = AnscLpccoTcpInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRemove
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
AnscLpccoTcpRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_DSTO_WORKER_OBJECT        pDaemonWorker = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hDaemonWorker;
    PANSC_BSTO_WORKER_OBJECT        pBrokerWorker = (PANSC_BSTO_WORKER_OBJECT      )pMyObject->hBrokerWorker;

    if ( pDaemonServer )
    {
        pDaemonServer->Remove((ANSC_HANDLE)pDaemonServer);

        pMyObject->hDaemonServer = (ANSC_HANDLE)NULL;
    }

    if ( pDaemonWorker )
    {
        AnscFreeMemory(pDaemonWorker);

        pMyObject->hDaemonWorker = (ANSC_HANDLE)NULL;
    }

    if ( pBrokerServer )
    {
        pBrokerServer->Remove((ANSC_HANDLE)pBrokerServer);

        pMyObject->hBrokerServer = (ANSC_HANDLE)NULL;
    }

    if ( pBrokerWorker )
    {
        AnscFreeMemory(pBrokerWorker);

        pMyObject->hBrokerWorker = (ANSC_HANDLE)NULL;
    }

    AnscLpccoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpEnrollObjects
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
AnscLpccoTcpEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_IMC_INTERFACE             pAnscImcIf    = (PANSC_IMC_INTERFACE           )pMyObject->hAnscImcIf;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_DSTO_WORKER_OBJECT        pDaemonWorker = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hDaemonWorker;
    PANSC_BSTO_WORKER_OBJECT        pBrokerWorker = (PANSC_BSTO_WORKER_OBJECT      )pMyObject->hBrokerWorker;

    if ( !pDaemonServer )
    {
        pDaemonServer =
            (PANSC_DAEMON_SERVER_TCP_OBJECT)AnscCreateDaemonServerTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pDaemonServer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDaemonServer = (ANSC_HANDLE)pDaemonServer;
        }
    }

    if ( !pBrokerServer )
    {
        pBrokerServer =
            (PANSC_BROKER_SERVER_TCP_OBJECT)AnscCreateBrokerServerTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBrokerServer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBrokerServer = (ANSC_HANDLE)pBrokerServer;
        }
    }

    if ( !pDaemonWorker )
    {
        pDaemonWorker = (PANSC_DSTO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_DSTO_WORKER_OBJECT));

        if ( !pDaemonWorker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDaemonWorker = (ANSC_HANDLE)pDaemonWorker;
        }

        pDaemonWorker->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pDaemonWorker->Init           = AnscLpccoTcpDwoInit;
        pDaemonWorker->Unload         = AnscLpccoTcpDwoUnload;
        pDaemonWorker->Accept         = AnscLpccoTcpDwoAccept;
        pDaemonWorker->SetOut         = AnscLpccoTcpDwoSetOut;
        pDaemonWorker->Remove         = AnscLpccoTcpDwoRemove;
        pDaemonWorker->Query          = AnscLpccoTcpDwoQuery;
        pDaemonWorker->ProcessSync    = AnscLpccoTcpDwoProcessSync;
        pDaemonWorker->ProcessAsync   = AnscLpccoTcpDwoProcessAsync;
        pDaemonWorker->SendComplete   = AnscLpccoTcpDwoSendComplete;
        pDaemonWorker->Notify         = AnscLpccoTcpDwoNotify;

        pDaemonServer->SetWorker
            (
                (ANSC_HANDLE)pDaemonServer,
                (ANSC_HANDLE)pDaemonWorker,
                sizeof(ANSC_DSTO_WORKER_OBJECT)
            );
    }

    if ( !pBrokerWorker )
    {
        pBrokerWorker = (PANSC_BSTO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_BSTO_WORKER_OBJECT));

        if ( !pBrokerWorker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBrokerWorker = (ANSC_HANDLE)pBrokerWorker;
        }

        pBrokerWorker->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pBrokerWorker->Init           = AnscLpccoTcpBwoInit;
        pBrokerWorker->Unload         = AnscLpccoTcpBwoUnload;
        pBrokerWorker->Accept         = AnscLpccoTcpBwoAccept;
        pBrokerWorker->Remove         = AnscLpccoTcpBwoRemove;
        pBrokerWorker->Query          = AnscLpccoTcpBwoQuery;
        pBrokerWorker->ProcessSync    = AnscLpccoTcpBwoProcessSync;
        pBrokerWorker->ProcessAsync   = AnscLpccoTcpBwoProcessAsync;
        pBrokerWorker->SendComplete   = AnscLpccoTcpBwoSendComplete;
        pBrokerWorker->Notify         = AnscLpccoTcpBwoNotify;

        pBrokerServer->SetWorker
            (
                (ANSC_HANDLE)pBrokerServer,
                (ANSC_HANDLE)pBrokerWorker,
                sizeof(ANSC_BSTO_WORKER_OBJECT)
            );
    }

    AnscLpccoEnrollObjects((ANSC_HANDLE)pMyObject);

    if ( TRUE )
    {
        pAnscImcIf = (PANSC_IMC_INTERFACE)pMyObject->hAnscImcIf;

        if ( pAnscImcIf )
        {
            pAnscImcIf->CallRequest  = AnscLpccoTcpImcCallRequest;
            pAnscImcIf->CallRequest2 = AnscLpccoTcpImcCallRequest2;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpInitialize
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
AnscLpccoTcpInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscLpccoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                  = ANSC_LPCCO_TCP_OID;
    pMyObject->Create               = AnscLpccoTcpCreate;
    pMyObject->Remove               = AnscLpccoTcpRemove;
    pMyObject->EnrollObjects        = AnscLpccoTcpEnrollObjects;
    pMyObject->Initialize           = AnscLpccoTcpInitialize;

    pMyObject->MyAddress.Value      = IPV4_LOOPBACK_ADDRESS;
    pMyObject->MyPort               = 0;
    pMyObject->ManagerAddress.Value = IPV4_LOOPBACK_ADDRESS;
    pMyObject->ManagerPort          = ANSC_LPC_MANAGER_PORT_NUMBER;
    pMyObject->MaxMessageSize       = 2048;
    pMyObject->EngineCount          = 1;
    pMyObject->MinSocketCount       = 4;
    pMyObject->MaxSocketCount       = 32;
    pMyObject->SocketTimeOut        = 90;

    pMyObject->GetMyAddress         = AnscLpccoTcpGetMyAddress;
    pMyObject->SetMyAddress         = AnscLpccoTcpSetMyAddress;
    pMyObject->GetMyPort            = AnscLpccoTcpGetMyPort;
    pMyObject->SetMyPort            = AnscLpccoTcpSetMyPort;
    pMyObject->GetManagerAddress    = AnscLpccoTcpGetManagerAddress;
    pMyObject->SetManagerAddress    = AnscLpccoTcpSetManagerAddress;
    pMyObject->GetManagerPort       = AnscLpccoTcpGetManagerPort;
    pMyObject->SetManagerPort       = AnscLpccoTcpSetManagerPort;
    pMyObject->Reset                = AnscLpccoTcpReset;

    pMyObject->Enroll               = AnscLpccoTcpEnroll;
    pMyObject->Unbind               = AnscLpccoTcpUnbind;
    pMyObject->ConnectToManager     = AnscLpccoTcpConnectToManager;

    pMyObject->ImcCallRequest       = AnscLpccoTcpImcCallRequest;
    pMyObject->ImcCallRequest2      = AnscLpccoTcpImcCallRequest2;

    pMyObject->SendHello            = AnscLpccoTcpSendHello;
    pMyObject->SendAck              = AnscLpccoTcpSendAck;
    pMyObject->SendRequest          = AnscLpccoTcpSendRequest;
    pMyObject->SendReply            = AnscLpccoTcpSendReply;
    pMyObject->SendBye              = AnscLpccoTcpSendBye;

    pMyObject->Recv                 = AnscLpccoTcpRecv;
    pMyObject->RecvHello            = AnscLpccoTcpRecvHello;
    pMyObject->RecvAck              = AnscLpccoTcpRecvAck;
    pMyObject->RecvRequest          = AnscLpccoTcpRecvRequest;
    pMyObject->RecvReply            = AnscLpccoTcpRecvReply;
    pMyObject->RecvBye              = AnscLpccoTcpRecvBye;

    pMyObject->DwoInit              = AnscLpccoTcpDwoInit;
    pMyObject->DwoUnload            = AnscLpccoTcpDwoUnload;
    pMyObject->DwoAccept            = AnscLpccoTcpDwoAccept;
    pMyObject->DwoSetOut            = AnscLpccoTcpDwoSetOut;
    pMyObject->DwoRemove            = AnscLpccoTcpDwoRemove;
    pMyObject->DwoQuery             = AnscLpccoTcpDwoQuery;
    pMyObject->DwoProcessSync       = AnscLpccoTcpDwoProcessSync;
    pMyObject->DwoProcessAsync      = AnscLpccoTcpDwoProcessAsync;
    pMyObject->DwoSendComplete      = AnscLpccoTcpDwoSendComplete;
    pMyObject->DwoNotify            = AnscLpccoTcpDwoNotify;

    pMyObject->BwoInit              = AnscLpccoTcpBwoInit;
    pMyObject->BwoUnload            = AnscLpccoTcpBwoUnload;
    pMyObject->BwoAccept            = AnscLpccoTcpBwoAccept;
    pMyObject->BwoRemove            = AnscLpccoTcpBwoRemove;
    pMyObject->BwoQuery             = AnscLpccoTcpBwoQuery;
    pMyObject->BwoProcessSync       = AnscLpccoTcpBwoProcessSync;
    pMyObject->BwoProcessAsync      = AnscLpccoTcpBwoProcessAsync;
    pMyObject->BwoSendComplete      = AnscLpccoTcpBwoSendComplete;
    pMyObject->BwoNotify            = AnscLpccoTcpBwoNotify;

    return  ANSC_STATUS_SUCCESS;
}
