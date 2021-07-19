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

    module:	http_sso_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Simple Server Object.

        *   HttpSsoCreate
        *   HttpSsoRemove
        *   HttpSsoEnrollObjects
        *   HttpSsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#include "http_sso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpSsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Simple Server Object and
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
HttpSsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_SIMPLE_SERVER_OBJECT)AnscAllocateMemory(sizeof(HTTP_SIMPLE_SERVER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_SIMPLE_SERVER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_SIMPLE_SERVER_OID;
    pBaseObject->Create            = HttpSsoCreate;
    pBaseObject->Remove            = HttpSsoRemove;
    pBaseObject->EnrollObjects     = HttpSsoEnrollObjects;
    pBaseObject->Initialize        = HttpSsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoRemove
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
HttpSsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pDstoWorker   = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hDstoWorker;
    PHTTP_WSP_INTERFACE             pWspIf        = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pDaemonServer )
    {
        pDaemonServer->Remove((ANSC_HANDLE)pDaemonServer);

        pMyObject->hDaemonServer = (ANSC_HANDLE)NULL;
    }

    if ( pDstoWorker )
    {
        AnscFreeMemory(pDstoWorker);

        pMyObject->hDstoWorker = (ANSC_HANDLE)NULL;
    }

    if ( pWspIf )
    {
        AnscFreeMemory(pWspIf);
    }

    if ( pHfpIf )
    {
        AnscFreeMemory(pHfpIf);
    }

    AnscFreeLock(&pMyObject->SyncLock       );
    AnscFreeLock(&pMyObject->WssoSListLock  );
    AnscFreeLock(&pMyObject->WstoSListLock  );
    AnscFreeLock(&pMyObject->BmoReqSListLock);
    AnscFreeLock(&pMyObject->BmoRepSListLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoEnrollObjects
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
HttpSsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pDstoWorker   = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hDstoWorker;
    PHTTP_WSP_INTERFACE             pWspIf        = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;

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

    if ( !pDstoWorker )
    {
        pDstoWorker = (PANSC_DSTO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_DSTO_WORKER_OBJECT));

        if ( !pDstoWorker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDstoWorker = (ANSC_HANDLE)pDstoWorker;
        }

        pDstoWorker->hWorkerContext = (ANSC_HANDLE)pMyObject;
        pDstoWorker->Init           = HttpSsoWorkerInit;
        pDstoWorker->Unload         = HttpSsoWorkerUnload;

        pDstoWorker->Accept         = HttpSsoDstowoAccept;
        pDstoWorker->SetOut         = HttpSsoDstowoSetOut;
        pDstoWorker->Remove         = HttpSsoDstowoRemove;
        pDstoWorker->Query          = HttpSsoDstowoQuery;
        pDstoWorker->ProcessSync    = HttpSsoDstowoProcessSync;
        pDstoWorker->ProcessAsync   = HttpSsoDstowoProcessAsync;
        pDstoWorker->SendComplete   = HttpSsoDstowoSendComplete;
        pDstoWorker->Notify         = HttpSsoDstowoNotify;

        pDaemonServer->SetWorker
            (
                (ANSC_HANDLE)pDaemonServer,
                (ANSC_HANDLE)pDstoWorker,
                sizeof(ANSC_DSTO_WORKER_OBJECT)
            );
    }

    if ( !pWspIf )
    {
        pWspIf = (PHTTP_WSP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_WSP_INTERFACE));

        if ( !pWspIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hWspIf = (ANSC_HANDLE)pWspIf;
        }

        pWspIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pWspIf->Size          = sizeof(HTTP_WSP_INTERFACE);
        pWspIf->Accept        = HttpSsoWspAccept;
        pWspIf->Serve         = HttpSsoWspServe;
    }

    if ( !pHfpIf )
    {
        pHfpIf = (PHTTP_HFP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_HFP_INTERFACE));

        if ( !pHfpIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hHfpIf = (ANSC_HANDLE)pHfpIf;
        }

        pHfpIf->hOwnerContext = (ANSC_HANDLE)NULL;
        pHfpIf->Size          = sizeof(HTTP_HFP_INTERFACE);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoInitialize
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
HttpSsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT)hThisObject;

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
    pMyObject->Oid                   = HTTP_SIMPLE_SERVER_OID;
    pMyObject->Create                = HttpSsoCreate;
    pMyObject->Remove                = HttpSsoRemove;
    pMyObject->EnrollObjects         = HttpSsoEnrollObjects;
    pMyObject->Initialize            = HttpSsoInitialize;

    pMyObject->ServerMode            = HTTP_SSO_MODE_COMPACT;
    pMyObject->bActive               = FALSE;

    pMyObject->GetWspIf              = HttpSsoGetWspIf;
    pMyObject->SetWspIf              = HttpSsoSetWspIf;
    pMyObject->GetHfpIf              = HttpSsoGetHfpIf;
    pMyObject->SetHfpIf              = HttpSsoSetHfpIf;
    pMyObject->GetFumIf              = HttpSsoGetFumIf;
    pMyObject->SetFumIf              = HttpSsoSetFumIf;
    pMyObject->GetServerMode         = HttpSsoGetServerMode;
    pMyObject->SetServerMode         = HttpSsoSetServerMode;

    pMyObject->GetProperty           = HttpSsoGetProperty;
    pMyObject->SetProperty           = HttpSsoSetProperty;
    pMyObject->ResetProperty         = HttpSsoResetProperty;
    pMyObject->Reset                 = HttpSsoReset;

    pMyObject->Engage                = HttpSsoEngage;
    pMyObject->Cancel                = HttpSsoCancel;

    pMyObject->WorkerInit            = HttpSsoWorkerInit;
    pMyObject->WorkerUnload          = HttpSsoWorkerUnload;

    pMyObject->AcquireWsso           = HttpSsoAcquireWsso;
    pMyObject->ReleaseWsso           = HttpSsoReleaseWsso;
    pMyObject->ManufactureWssoPool   = HttpSsoManufactureWssoPool;
    pMyObject->DestroyWssoPool       = HttpSsoDestroyWssoPool;

    pMyObject->AcquireWsto           = HttpSsoAcquireWsto;
    pMyObject->ReleaseWsto           = HttpSsoReleaseWsto;
    pMyObject->ManufactureWstoPool   = HttpSsoManufactureWstoPool;
    pMyObject->DestroyWstoPool       = HttpSsoDestroyWstoPool;

    pMyObject->AcquireBmoReq         = HttpSsoAcquireBmoReq;
    pMyObject->ReleaseBmoReq         = HttpSsoReleaseBmoReq;
    pMyObject->ManufactureBmoReqPool = HttpSsoManufactureBmoReqPool;
    pMyObject->DestroyBmoReqPool     = HttpSsoDestroyBmoReqPool;

    pMyObject->AcquireBmoRep         = HttpSsoAcquireBmoRep;
    pMyObject->ReleaseBmoRep         = HttpSsoReleaseBmoRep;
    pMyObject->ManufactureBmoRepPool = HttpSsoManufactureBmoRepPool;
    pMyObject->DestroyBmoRepPool     = HttpSsoDestroyBmoRepPool;

    pMyObject->WspAccept             = HttpSsoWspAccept;
    pMyObject->WspServe              = HttpSsoWspServe;

    pMyObject->DstowoAccept          = HttpSsoDstowoAccept;
    pMyObject->DstowoSetOut          = HttpSsoDstowoSetOut;
    pMyObject->DstowoRemove          = HttpSsoDstowoRemove;
    pMyObject->DstowoQuery           = HttpSsoDstowoQuery;
    pMyObject->DstowoProcessSync     = HttpSsoDstowoProcessSync;
    pMyObject->DstowoProcessAsync    = HttpSsoDstowoProcessAsync;
    pMyObject->DstowoSendComplete    = HttpSsoDstowoSendComplete;
    pMyObject->DstowoNotify          = HttpSsoDstowoNotify;

    AnscInitializeLock       (&pMyObject->SyncLock       );
    AnscSListInitializeHeader(&pMyObject->WssoSList      );
    AnscInitializeLock       (&pMyObject->WssoSListLock  );
    AnscSListInitializeHeader(&pMyObject->WstoSList      );
    AnscInitializeLock       (&pMyObject->WstoSListLock  );

    AnscSListInitializeHeader(&pMyObject->BmoReqSList    );
    AnscInitializeLock       (&pMyObject->BmoReqSListLock);
    AnscSListInitializeHeader(&pMyObject->BmoRepSList    );
    AnscInitializeLock       (&pMyObject->BmoRepSListLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
