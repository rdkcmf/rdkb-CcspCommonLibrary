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

    module:	http_spo_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Simple Proxy Object.

        *   HttpSpoAcquirePso
        *   HttpSpoReleasePso
        *   HttpSpoManufacturePsoPool
        *   HttpSpoDestroyPsoPool
        *   HttpSpoAcquireBmoReq
        *   HttpSpoReleaseBmoReq
        *   HttpSpoManufactureBmoReqPool
        *   HttpSpoDestroyBmoReqPool
        *   HttpSpoAcquireBmoRep
        *   HttpSpoReleaseBmoRep
        *   HttpSpoManufactureBmoRepPool
        *   HttpSpoDestroyBmoRepPool

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSpoAcquirePso
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     resource object.

**********************************************************************/

ANSC_HANDLE
HttpSpoAcquirePso
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_PROXY_SESSION_OBJECT      pSession     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PsoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);
    AnscReleaseLock(&pMyObject->PsoSListLock);

    if ( pSLinkEntry )
    {
        pSession = ACCESS_HTTP_PROXY_SESSION_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pSession;
    }

    pSession =
        (PHTTP_PROXY_SESSION_OBJECT)HttpCreateProxySession
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSession )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSession->SetClientSocket((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
        pSession->SetServerSocket((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
        pSession->SetSbcContext  ((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
        pSession->SetCbcContext  ((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
    }

    return  (ANSC_HANDLE)pSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoReleasePso
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPso
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPso
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoReleasePso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPso
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_PROXY_SESSION_OBJECT      pSession     = (PHTTP_PROXY_SESSION_OBJECT )hPso;

    pSession->Reset((ANSC_HANDLE)pSession);

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        pSession->Remove((ANSC_HANDLE)pSession);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->PsoSListLock);
        AnscSListPushEntry(&pMyObject->PsoSList, &pSession->Linkage);
        AnscReleaseLock   (&pMyObject->PsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoManufacturePsoPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture a pool of resource
        objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoManufacturePsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_PROXY_SESSION_OBJECT      pSession     = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SPO_INITIAL_PSO_NUMBER; i++ )
    {
        pSession =
            (PHTTP_PROXY_SESSION_OBJECT)HttpCreateProxySession
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSession )
        {
            continue;
        }
        else
        {
            pSession->SetClientSocket((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
            pSession->SetServerSocket((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
            pSession->SetSbcContext  ((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
            pSession->SetCbcContext  ((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL);
        }

        AnscAcquireLock   (&pMyObject->PsoSListLock);
        AnscSListPushEntry(&pMyObject->PsoSList, &pSession->Linkage);
        AnscReleaseLock   (&pMyObject->PsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoDestroyPsoPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy a pool of resource objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoDestroyPsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_PROXY_SESSION_OBJECT      pSession     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PsoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);

    while ( pSLinkEntry )
    {
        pSession    = ACCESS_HTTP_PROXY_SESSION_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);

        pSession->Remove((ANSC_HANDLE)pSession);
    }

    AnscReleaseLock(&pMyObject->PsoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSpoAcquireBmoReq
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     resource object.

**********************************************************************/

ANSC_HANDLE
HttpSpoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoReqSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoReqSList);
    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    if ( pSLinkEntry )
    {
        pBmoReq = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pBmoReq;
    }

    pBmoReq =
        (PHTTP_BMO_REQ_OBJECT)HttpCreateBmoReq
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pBmoReq )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBmoReq->SetHfpIf      ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf);
        pBmoReq->SetRecvContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)NULL  );
        pBmoReq->SetSendContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)NULL  );
    }

    return  (ANSC_HANDLE)pBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoReleaseBmoReq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;

    pBmoReq->Reset((ANSC_HANDLE)pBmoReq);

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        pBmoReq->Remove((ANSC_HANDLE)pBmoReq);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->BmoReqSListLock);
        AnscSListPushEntry(&pMyObject->BmoReqSList, &pBmoReq->Linkage);
        AnscReleaseLock   (&pMyObject->BmoReqSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoManufactureBmoReqPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture a pool of resource
        objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SPO_INITIAL_BMO_NUMBER; i++ )
    {
        pBmoReq =
            (PHTTP_BMO_REQ_OBJECT)HttpCreateBmoReq
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBmoReq )
        {
            continue;
        }
        else
        {
            pBmoReq->SetHfpIf      ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf);
            pBmoReq->SetRecvContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)NULL  );
            pBmoReq->SetSendContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)NULL  );
        }

        AnscAcquireLock   (&pMyObject->BmoReqSListLock);
        AnscSListPushEntry(&pMyObject->BmoReqSList, &pBmoReq->Linkage);
        AnscReleaseLock   (&pMyObject->BmoReqSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoDestroyBmoReqPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy a pool of resource objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoReqSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoReqSList);

    while ( pSLinkEntry )
    {
        pBmoReq     = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoReqSList);

        pBmoReq->Remove((ANSC_HANDLE)pBmoReq);
    }

    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSpoAcquireBmoRep
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     resource object.

**********************************************************************/

ANSC_HANDLE
HttpSpoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoRepSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoRepSList);
    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    if ( pSLinkEntry )
    {
        pBmoRep = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pBmoRep;
    }

    pBmoRep =
        (PHTTP_BMO_REP_OBJECT)HttpCreateBmoRep
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pBmoRep )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBmoRep->SetHfpIf      ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf);
        pBmoRep->SetRecvContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)NULL  );
        pBmoRep->SetSendContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)NULL  );
    }

    return  (ANSC_HANDLE)pBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoReleaseBmoRep
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoRep
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT       )hBmoRep;

    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        pBmoRep->Remove((ANSC_HANDLE)pBmoRep);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->BmoRepSListLock);
        AnscSListPushEntry(&pMyObject->BmoRepSList, &pBmoRep->Linkage);
        AnscReleaseLock   (&pMyObject->BmoRepSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoManufactureBmoRepPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture a pool of resource
        objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_SPO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SPO_INITIAL_BMO_NUMBER; i++ )
    {
        pBmoRep =
            (PHTTP_BMO_REP_OBJECT)HttpCreateBmoRep
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBmoRep )
        {
            continue;
        }
        else
        {
            pBmoRep->SetHfpIf      ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf);
            pBmoRep->SetRecvContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)NULL  );
            pBmoRep->SetSendContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)NULL  );
        }

        AnscAcquireLock   (&pMyObject->BmoRepSListLock);
        AnscSListPushEntry(&pMyObject->BmoRepSList, &pBmoRep->Linkage);
        AnscReleaseLock   (&pMyObject->BmoRepSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoDestroyBmoRepPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy a pool of resource objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSpoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject    = (PHTTP_SIMPLE_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoRepSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoRepSList);

    while ( pSLinkEntry )
    {
        pBmoRep     = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoRepSList);

        pBmoRep->Remove((ANSC_HANDLE)pBmoRep);
    }

    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    return  ANSC_STATUS_SUCCESS;
}
