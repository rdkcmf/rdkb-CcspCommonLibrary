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

    module:	http_wcso_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Webc Session Object.

        *   HttpWcsoCreate
        *   HttpWcsoRemove
        *   HttpWcsoEnrollObjects
        *   HttpWcsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#include "http_wcso_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpWcsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Webc Session Object and
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
HttpWcsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_WEBC_SESSION_OBJECT)AnscAllocateMemory(sizeof(HTTP_WEBC_SESSION_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, HTTP_WEBC_SESSION_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_WEBC_SESSION_OID;
    pBaseObject->Create            = HttpWcsoCreate;
    pBaseObject->Remove            = HttpWcsoRemove;
    pBaseObject->EnrollObjects     = HttpWcsoEnrollObjects;
    pBaseObject->Initialize        = HttpWcsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoRemove
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
HttpWcsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PANSC_SCTO_WORKER_OBJECT        pSctoWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hSctoWorker;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pTcpSimpleClient )
    {
        pTcpSimpleClient->Remove((ANSC_HANDLE)pTcpSimpleClient);

        pMyObject->hTcpSimpleClient = (ANSC_HANDLE)NULL;
    }

    if ( pSctoWorker )
    {
        AnscFreeMemory(pSctoWorker);

        pMyObject->hSctoWorker = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->AccessLock   );
    AnscFreeLock(&pMyObject->WctoSListLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject   );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoEnrollObjects
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
HttpWcsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PANSC_SCTO_WORKER_OBJECT        pSctoWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hSctoWorker;

    if ( !pTcpSimpleClient )
    {
        pTcpSimpleClient =
            (PANSC_SIMPLE_CLIENT_TCP_OBJECT)AnscCreateSimpleClientTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTcpSimpleClient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTcpSimpleClient = (ANSC_HANDLE)pTcpSimpleClient;
        }
    }

    if ( !pSctoWorker )
    {
        pSctoWorker = (PANSC_SCTO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_SCTO_WORKER_OBJECT));

        if ( !pSctoWorker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSctoWorker = (ANSC_HANDLE)pSctoWorker;
        }

        pSctoWorker->hWorkerContext = (ANSC_HANDLE)pMyObject;

        pSctoWorker->Query          = HttpWcsoSctowoQuery;
        pSctoWorker->Process        = HttpWcsoSctowoProcess;
        pSctoWorker->Notify         = HttpWcsoSctowoNotify;

        pTcpSimpleClient->SetWorker
            (
                (ANSC_HANDLE)pTcpSimpleClient,
                (ANSC_HANDLE)pSctoWorker,
                sizeof(ANSC_SCTO_WORKER_OBJECT)
            );
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoInitialize
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
HttpWcsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

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
    pMyObject->Oid              = HTTP_WEBC_SESSION_OID;
    pMyObject->Create           = HttpWcsoCreate;
    pMyObject->Remove           = HttpWcsoRemove;
    pMyObject->EnrollObjects    = HttpWcsoEnrollObjects;
    pMyObject->Initialize       = HttpWcsoInitialize;

    pMyObject->hBspIf           = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf           = (ANSC_HANDLE)NULL;
    pMyObject->SessionState     = HTTP_WCSO_STATE_INITIALIZED;
    pMyObject->Timestamp        = AnscGetTickInSeconds();
    pMyObject->bRemoveMe        = FALSE;
    pMyObject->hReqUri          = NULL;

    pMyObject->GetPeerName      = HttpWcsoGetPeerName;
    pMyObject->SetPeerName      = HttpWcsoSetPeerName;
    pMyObject->GetPeerPort      = HttpWcsoGetPeerPort;
    pMyObject->SetPeerPort      = HttpWcsoSetPeerPort;
    pMyObject->GetHostAddr      = HttpWcsoGetHostAddr;
    pMyObject->SetHostAddr      = HttpWcsoSetHostAddr;
    pMyObject->GetHostPort      = HttpWcsoGetHostPort;
    pMyObject->SetHostPort      = HttpWcsoSetHostPort;
    pMyObject->GetSessionFlags  = HttpWcsoGetSessionFlags;
    pMyObject->SetSessionFlags  = HttpWcsoSetSessionFlags;

    pMyObject->GetBspIf         = HttpWcsoGetBspIf;
    pMyObject->SetBspIf         = HttpWcsoSetBspIf;
    pMyObject->GetHfpIf         = HttpWcsoGetHfpIf;
    pMyObject->SetHfpIf         = HttpWcsoSetHfpIf;
    pMyObject->GetSessionState  = HttpWcsoGetSessionState;
    pMyObject->SetSessionState  = HttpWcsoSetSessionState;

    pMyObject->GetProperty      = HttpWcsoGetProperty;
    pMyObject->SetProperty      = HttpWcsoSetProperty;
    pMyObject->ResetProperty    = HttpWcsoResetProperty;
    pMyObject->Reset            = HttpWcsoReset;
    pMyObject->Return           = HttpWcsoReturn;

    pMyObject->MatchServer      = HttpWcsoMatchServer;
    pMyObject->IsReadyToRequest = HttpWcsoIsReadyToRequest;
    pMyObject->Open             = HttpWcsoOpen;
    pMyObject->Close            = HttpWcsoClose;

    pMyObject->AcquireAccess    = HttpWcsoAcquireAccess;
    pMyObject->ReleaseAccess    = HttpWcsoReleaseAccess;

    pMyObject->GetEndWcto       = HttpWcsoGetEndWcto;
    pMyObject->GetCurWcto       = HttpWcsoGetCurWcto;
    pMyObject->AddNewWcto       = HttpWcsoAddNewWcto;
    pMyObject->DelAllWctos      = HttpWcsoDelAllWctos;

    pMyObject->Connect          = HttpWcsoConnect;
    pMyObject->Request          = HttpWcsoRequest;
    pMyObject->DoOptions        = HttpWcsoDoOptions;
    pMyObject->DoGet            = HttpWcsoDoGet;
    pMyObject->DoHead           = HttpWcsoDoHead;
    pMyObject->DoPost           = HttpWcsoDoPost;
    pMyObject->DoPut            = HttpWcsoDoPut;
    pMyObject->DoDelete         = HttpWcsoDoDelete;
    pMyObject->DoTrace          = HttpWcsoDoTrace;
    pMyObject->DoConnect        = HttpWcsoDoConnect;
    pMyObject->ConnectPeer      = HttpWcsoConnectPeer;
    pMyObject->ConnectionFailed = HttpWcsoConnectionFailed;

    pMyObject->SctowoQuery      = HttpWcsoSctowoQuery;
    pMyObject->SctowoProcess    = HttpWcsoSctowoProcess;
    pMyObject->SctowoNotify     = HttpWcsoSctowoNotify;

    AnscInitializeLock       (&pMyObject->AccessLock   );
    AnscSListInitializeHeader(&pMyObject->WctoSList    );
    AnscInitializeLock       (&pMyObject->WctoSListLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
