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

    module:	http_spo_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Simple Proxy Object.

        *   HttpSpoCreate
        *   HttpSpoRemove
        *   HttpSpoEnrollObjects
        *   HttpSpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/21/02    initial revision.

**********************************************************************/


#include "http_spo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpSpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Simple Proxy Object and
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
HttpSpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_SIMPLE_PROXY_OBJECT)AnscAllocateMemory(sizeof(HTTP_SIMPLE_PROXY_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_SIMPLE_PROXY_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_SIMPLE_PROXY_OID;
    pBaseObject->Create            = HttpSpoCreate;
    pBaseObject->Remove            = HttpSpoRemove;
    pBaseObject->EnrollObjects     = HttpSpoEnrollObjects;
    pBaseObject->Initialize        = HttpSpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoRemove
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
HttpSpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT    )hThisObject;
    PANSC_SIMPLE_PROXY_TCP_OBJECT   pSimpleProxy = (PANSC_SIMPLE_PROXY_TCP_OBJECT)pMyObject->hSimpleProxy;
    PANSC_SPTO_WORKER_OBJECT        pSptoWorker  = (PANSC_SPTO_WORKER_OBJECT     )pMyObject->hSptoWorker;
    PHTTP_WAM_INTERFACE             pWamIf       = (PHTTP_WAM_INTERFACE          )pMyObject->hWamIf;
    PHTTP_SBC_INTERFACE             pSbcIf       = (PHTTP_SBC_INTERFACE          )pMyObject->hSbcIf;
    PHTTP_CBC_INTERFACE             pCbcIf       = (PHTTP_CBC_INTERFACE          )pMyObject->hCbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf       = (PHTTP_PBC_INTERFACE          )pMyObject->hPbcIf;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pSimpleProxy )
    {
        pSimpleProxy->Remove((ANSC_HANDLE)pSimpleProxy);

        pMyObject->hSimpleProxy = (ANSC_HANDLE)NULL;
    }

    if ( pSptoWorker )
    {
        AnscFreeMemory(pSptoWorker);

        pMyObject->hSptoWorker = (ANSC_HANDLE)NULL;
    }

    if ( pWamIf )
    {
        AnscFreeMemory(pWamIf);
    }

    if ( pSbcIf )
    {
        AnscFreeMemory(pSbcIf);
    }

    if ( pCbcIf )
    {
        AnscFreeMemory(pCbcIf);
    }

    if ( pPbcIf )
    {
        AnscFreeMemory(pPbcIf);
    }

    if ( pHfpIf )
    {
        AnscFreeMemory(pHfpIf);
    }

    AnscFreeLock(&pMyObject->SyncLock       );
    AnscFreeLock(&pMyObject->PsoSListLock   );
    AnscFreeLock(&pMyObject->BmoReqSListLock);
    AnscFreeLock(&pMyObject->BmoRepSListLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoEnrollObjects
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
HttpSpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT    )hThisObject;
    PANSC_SIMPLE_PROXY_TCP_OBJECT   pSimpleProxy = (PANSC_SIMPLE_PROXY_TCP_OBJECT)pMyObject->hSimpleProxy;
    PANSC_SPTO_WORKER_OBJECT        pSptoWorker  = (PANSC_SPTO_WORKER_OBJECT     )pMyObject->hSptoWorker;
    PHTTP_WAM_INTERFACE             pWamIf       = (PHTTP_WAM_INTERFACE          )pMyObject->hWamIf;
    PHTTP_SBC_INTERFACE             pSbcIf       = (PHTTP_SBC_INTERFACE          )pMyObject->hSbcIf;
    PHTTP_CBC_INTERFACE             pCbcIf       = (PHTTP_CBC_INTERFACE          )pMyObject->hCbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf       = (PHTTP_PBC_INTERFACE          )pMyObject->hPbcIf;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;

    if ( !pSimpleProxy )
    {
        pSimpleProxy =
            (PANSC_SIMPLE_PROXY_TCP_OBJECT)AnscCreateSimpleProxyTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSimpleProxy )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSimpleProxy = (ANSC_HANDLE)pSimpleProxy;
        }
    }

    if ( !pSptoWorker )
    {
        pSptoWorker = (PANSC_SPTO_WORKER_OBJECT)AnscAllocateMemory(sizeof(ANSC_SPTO_WORKER_OBJECT));

        if ( !pSptoWorker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSptoWorker = (ANSC_HANDLE)pSptoWorker;
        }

        pSptoWorker->hWorkerContext  = (ANSC_HANDLE)pMyObject;
        pSptoWorker->Init            = HttpSpoWorkerInit;
        pSptoWorker->Unload          = HttpSpoWorkerUnload;

        pSptoWorker->PrvAccept       = HttpSpoPrvwoAccept;
        pSptoWorker->PrvSetOut       = HttpSpoPrvwoSetOut;
        pSptoWorker->PrvRemove       = HttpSpoPrvwoRemove;
        pSptoWorker->PrvQuery        = HttpSpoPrvwoQuery;
        pSptoWorker->PrvProcessSync  = HttpSpoPrvwoProcessSync;
        pSptoWorker->PrvProcessAsync = HttpSpoPrvwoProcessAsync;
        pSptoWorker->PrvSendComplete = HttpSpoPrvwoSendComplete;
        pSptoWorker->PrvNotify       = HttpSpoPrvwoNotify;

        pSptoWorker->PubAccept       = HttpSpoPubwoAccept;
        pSptoWorker->PubRemove       = HttpSpoPubwoRemove;
        pSptoWorker->PubQuery        = HttpSpoPubwoQuery;
        pSptoWorker->PubProcessSync  = HttpSpoPubwoProcessSync;
        pSptoWorker->PubProcessAsync = HttpSpoPubwoProcessAsync;
        pSptoWorker->PubSendComplete = HttpSpoPubwoSendComplete;
        pSptoWorker->PubNotify       = HttpSpoPubwoNotify;

        pSimpleProxy->SetProxyWorker
            (
                (ANSC_HANDLE)pSimpleProxy,
                (ANSC_HANDLE)pSptoWorker,
                sizeof(ANSC_SPTO_WORKER_OBJECT)
            );
    }

    if ( !pWamIf )
    {
        pWamIf = (PHTTP_WAM_INTERFACE)AnscAllocateMemory(sizeof(HTTP_WAM_INTERFACE));

        if ( !pWamIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hWamIf = (ANSC_HANDLE)pWamIf;
        }

        pWamIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pWamIf->Size          = sizeof(HTTP_WAM_INTERFACE);
        pWamIf->Accept        = HttpSpoWamAccept;
        pWamIf->Connect       = HttpSpoWamConnect;
        pWamIf->Close         = HttpSpoWamClose;
    }

    if ( !pSbcIf )
    {
        pSbcIf = (PHTTP_SBC_INTERFACE)AnscAllocateMemory(sizeof(HTTP_SBC_INTERFACE));

        if ( !pSbcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSbcIf = (ANSC_HANDLE)pSbcIf;
        }

        pSbcIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pSbcIf->Size           = sizeof(HTTP_SBC_INTERFACE);
        pSbcIf->RequestHeaders = HttpSpoSbcRequestHeaders;
        pSbcIf->RequestBody    = HttpSpoSbcRequestBody;
    }

    if ( !pCbcIf )
    {
        pCbcIf = (PHTTP_CBC_INTERFACE)AnscAllocateMemory(sizeof(HTTP_CBC_INTERFACE));

        if ( !pCbcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCbcIf = (ANSC_HANDLE)pCbcIf;
        }

        pCbcIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pCbcIf->Size          = sizeof(HTTP_CBC_INTERFACE);
        pCbcIf->ReplyHeaders  = HttpSpoCbcReplyHeaders;
        pCbcIf->ReplyBody     = HttpSpoCbcReplyBody;
    }

    if ( !pPbcIf )
    {
        pPbcIf = (PHTTP_PBC_INTERFACE)AnscAllocateMemory(sizeof(HTTP_PBC_INTERFACE));

        if ( !pPbcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hPbcIf = (ANSC_HANDLE)pPbcIf;
        }

        pPbcIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pPbcIf->Size           = sizeof(HTTP_PBC_INTERFACE);

        pPbcIf->RecvReqHeaders = HttpSpoPbcRecvReqHeaders;
        pPbcIf->RecvReqBody    = HttpSpoPbcRecvReqBody;
        pPbcIf->ReqComplete    = HttpSpoPbcReqComplete;

        pPbcIf->RecvRepHeaders = HttpSpoPbcRecvRepHeaders;
        pPbcIf->RecvRepBody    = HttpSpoPbcRecvRepBody;
        pPbcIf->RepComplete    = HttpSpoPbcRepComplete;
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
        HttpSpoInitialize
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
HttpSpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT)hThisObject;
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
    pMyObject->Oid                   = HTTP_SIMPLE_PROXY_OID;
    pMyObject->Create                = HttpSpoCreate;
    pMyObject->Remove                = HttpSpoRemove;
    pMyObject->EnrollObjects         = HttpSpoEnrollObjects;
    pMyObject->Initialize            = HttpSpoInitialize;

    pMyObject->ProxyMode             = HTTP_SPO_MODE_COMPACT     |
                                       HTTP_SPO_MODE_WAM_ENABLED | HTTP_SPO_MODE_SBC_ENABLED |
                                       HTTP_SPO_MODE_CBC_ENABLED | HTTP_SPO_MODE_PBC_ENABLED;
    pMyObject->bActive               = FALSE;

    pMyObject->GetWamIf              = HttpSpoGetWamIf;
    pMyObject->SetWamIf              = HttpSpoSetWamIf;
    pMyObject->GetSbcIf              = HttpSpoGetSbcIf;
    pMyObject->SetSbcIf              = HttpSpoSetSbcIf;
    pMyObject->GetCbcIf              = HttpSpoGetCbcIf;
    pMyObject->SetCbcIf              = HttpSpoSetCbcIf;
    pMyObject->GetPbcIf              = HttpSpoGetPbcIf;
    pMyObject->SetPbcIf              = HttpSpoSetPbcIf;
    pMyObject->GetHfpIf              = HttpSpoGetHfpIf;
    pMyObject->SetHfpIf              = HttpSpoSetHfpIf;
    pMyObject->GetProxyMode          = HttpSpoGetProxyMode;
    pMyObject->SetProxyMode          = HttpSpoSetProxyMode;

    pMyObject->GetProperty           = HttpSpoGetProperty;
    pMyObject->SetProperty           = HttpSpoSetProperty;
    pMyObject->ResetProperty         = HttpSpoResetProperty;
    pMyObject->Reset                 = HttpSpoReset;

    pMyObject->Engage                = HttpSpoEngage;
    pMyObject->Cancel                = HttpSpoCancel;

    pMyObject->WorkerInit            = HttpSpoWorkerInit;
    pMyObject->WorkerUnload          = HttpSpoWorkerUnload;

    pMyObject->AcquirePso            = HttpSpoAcquirePso;
    pMyObject->ReleasePso            = HttpSpoReleasePso;
    pMyObject->ManufacturePsoPool    = HttpSpoManufacturePsoPool;
    pMyObject->DestroyPsoPool        = HttpSpoDestroyPsoPool;

    pMyObject->AcquireBmoReq         = HttpSpoAcquireBmoReq;
    pMyObject->ReleaseBmoReq         = HttpSpoReleaseBmoReq;
    pMyObject->ManufactureBmoReqPool = HttpSpoManufactureBmoReqPool;
    pMyObject->DestroyBmoReqPool     = HttpSpoDestroyBmoReqPool;

    pMyObject->AcquireBmoRep         = HttpSpoAcquireBmoRep;
    pMyObject->ReleaseBmoRep         = HttpSpoReleaseBmoRep;
    pMyObject->ManufactureBmoRepPool = HttpSpoManufactureBmoRepPool;
    pMyObject->DestroyBmoRepPool     = HttpSpoDestroyBmoRepPool;

    pMyObject->WamAccept             = HttpSpoWamAccept;
    pMyObject->WamConnect            = HttpSpoWamConnect;
    pMyObject->WamClose              = HttpSpoWamClose;

    pMyObject->SbcRequestHeaders     = HttpSpoSbcRequestHeaders;
    pMyObject->SbcRequestBody        = HttpSpoSbcRequestBody;

    pMyObject->CbcReplyHeaders       = HttpSpoCbcReplyHeaders;
    pMyObject->CbcReplyBody          = HttpSpoCbcReplyBody;

    pMyObject->PbcRecvReqHeaders     = HttpSpoPbcRecvReqHeaders;
    pMyObject->PbcRecvReqBody        = HttpSpoPbcRecvReqBody;
    pMyObject->PbcReqComplete        = HttpSpoPbcReqComplete;
    pMyObject->PbcRecvRepHeaders     = HttpSpoPbcRecvRepHeaders;
    pMyObject->PbcRecvRepBody        = HttpSpoPbcRecvRepBody;
    pMyObject->PbcRepComplete        = HttpSpoPbcRepComplete;

    pMyObject->PrvwoAccept           = HttpSpoPrvwoAccept;
    pMyObject->PrvwoSetOut           = HttpSpoPrvwoSetOut;
    pMyObject->PrvwoRemove           = HttpSpoPrvwoRemove;
    pMyObject->PrvwoQuery            = HttpSpoPrvwoQuery;
    pMyObject->PrvwoProcessSync      = HttpSpoPrvwoProcessSync;
    pMyObject->PrvwoProcessAsync     = HttpSpoPrvwoProcessAsync;
    pMyObject->PrvwoSendComplete     = HttpSpoPrvwoSendComplete;
    pMyObject->PrvwoNotify           = HttpSpoPrvwoNotify;

    pMyObject->PubwoAccept           = HttpSpoPubwoAccept;
    pMyObject->PubwoRemove           = HttpSpoPubwoRemove;
    pMyObject->PubwoQuery            = HttpSpoPubwoQuery;
    pMyObject->PubwoProcessSync      = HttpSpoPubwoProcessSync;
    pMyObject->PubwoProcessAsync     = HttpSpoPubwoProcessAsync;
    pMyObject->PubwoSendComplete     = HttpSpoPubwoSendComplete;
    pMyObject->PubwoNotify           = HttpSpoPubwoNotify;

    AnscInitializeLock       (&pMyObject->SyncLock       );
    AnscSListInitializeHeader(&pMyObject->PsoSList       );
    AnscInitializeLock       (&pMyObject->PsoSListLock   );

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
