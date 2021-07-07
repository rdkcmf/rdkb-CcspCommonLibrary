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

    module:	http_sco_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Simple Client Object.

        *   HttpScoAcquireWcso
        *   HttpScoReleaseWcso
        *   HttpScoManufactureWcsoPool
        *   HttpScoDestroyWcsoPool
        *   HttpScoAcquireWcto
        *   HttpScoReleaseWcto
        *   HttpScoManufactureWctoPool
        *   HttpScoDestroyWctoPool
        *   HttpScoAcquireBmoReq
        *   HttpScoReleaseBmoReq
        *   HttpScoManufactureBmoReqPool
        *   HttpScoDestroyBmoReqPool
        *   HttpScoAcquireBmoRep
        *   HttpScoReleaseBmoRep
        *   HttpScoManufactureBmoRepPool
        *   HttpScoDestroyBmoRepPool

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


#include "http_sco_global.h"
#include "ansc_crypto_external_api.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoAcquireWcso
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
HttpScoAcquireWcso
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WcsoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->WcsoSList);
    AnscReleaseLock(&pMyObject->WcsoSListLock);

    if ( pSLinkEntry )
    {
        pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pWebcSession;
    }

    pWebcSession =
        (PHTTP_WEBC_SESSION_OBJECT)HttpCreateWebcSession
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pWebcSession )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebcSession->SetSessionState((ANSC_HANDLE)pWebcSession, HTTP_WCSO_STATE_INITIALIZED);

        pWebcSession->bRemoveMe = FALSE;
    }

    return  (ANSC_HANDLE)pWebcSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoReleaseWcso
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWcso
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWcso
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoReleaseWcso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWcso
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = (PHTTP_WEBC_SESSION_OBJECT   )hWcso;

    pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);
    pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);
    pWebcSession->Reset        ((ANSC_HANDLE)pWebcSession);

    pWebcSession->bRemoveMe = FALSE;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        pWebcSession->Remove((ANSC_HANDLE)pWebcSession);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->WcsoSListLock);
        AnscSListPushEntry(&pMyObject->WcsoSList, &pWebcSession->Linkage);
        AnscReleaseLock   (&pMyObject->WcsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoManufactureWcsoPool
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
HttpScoManufactureWcsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SCO_INITIAL_WCSO_NUMBER; i++ )
    {
        pWebcSession =
            (PHTTP_WEBC_SESSION_OBJECT)HttpCreateWebcSession
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebcSession )
        {
            continue;
        }
        else
        {
            pWebcSession->SetSessionState((ANSC_HANDLE)pWebcSession, HTTP_WCSO_STATE_INITIALIZED);
        }

        AnscAcquireLock   (&pMyObject->WcsoSListLock);
        AnscSListPushEntry(&pMyObject->WcsoSList, &pWebcSession->Linkage);
        AnscReleaseLock   (&pMyObject->WcsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDestroyWcsoPool
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
HttpScoDestroyWcsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WcsoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WcsoSList);

    while ( pSLinkEntry )
    {
        pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->WcsoSList);

        pWebcSession->Remove((ANSC_HANDLE)pWebcSession);
    }

    AnscReleaseLock(&pMyObject->WcsoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoAcquireWcto
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
HttpScoAcquireWcto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WctoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->WctoSList);
    AnscReleaseLock(&pMyObject->WctoSListLock);

    if ( pSLinkEntry )
    {
        pWebcTrans = ACCESS_HTTP_WEBC_TRANS_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pWebcTrans;
    }

    pWebcTrans =
        (PHTTP_WEBC_TRANS_OBJECT)HttpCreateWebcTrans
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pWebcTrans )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebcTrans->SetTransState((ANSC_HANDLE)pWebcTrans, HTTP_WCTO_STATE_INITIALIZED);
    }

    return  (ANSC_HANDLE)pWebcTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoReleaseWcto
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWcto
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWcto
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoReleaseWcto
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWcto
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = (PHTTP_WEBC_TRANS_OBJECT     )hWcto;

    pWebcTrans->Reset((ANSC_HANDLE)pWebcTrans);

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        pWebcTrans->Remove((ANSC_HANDLE)pWebcTrans);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->WctoSListLock);
        AnscSListPushEntry(&pMyObject->WctoSList, &pWebcTrans->Linkage);
        AnscReleaseLock   (&pMyObject->WctoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoManufactureWctoPool
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
HttpScoManufactureWctoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SCO_INITIAL_WCTO_NUMBER; i++ )
    {
        pWebcTrans =
            (PHTTP_WEBC_TRANS_OBJECT)HttpCreateWebcTrans
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebcTrans )
        {
            continue;
        }
        else
        {
            pWebcTrans->SetTransState((ANSC_HANDLE)pWebcTrans, HTTP_WCTO_STATE_INITIALIZED);
        }

        AnscAcquireLock   (&pMyObject->WctoSListLock);
        AnscSListPushEntry(&pMyObject->WctoSList, &pWebcTrans->Linkage);
        AnscReleaseLock   (&pMyObject->WctoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDestroyWctoPool
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
HttpScoDestroyWctoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WctoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WctoSList);

    while ( pSLinkEntry )
    {
        pWebcTrans  = ACCESS_HTTP_WEBC_TRANS_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->WctoSList);

        pWebcTrans->Remove((ANSC_HANDLE)pWebcTrans);
    }

    AnscReleaseLock(&pMyObject->WctoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoAcquireBmoReq
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
HttpScoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
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
        HttpScoReleaseBmoReq
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
HttpScoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;

    pBmoReq->Reset((ANSC_HANDLE)pBmoReq);

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
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
        HttpScoManufactureBmoReqPool
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
HttpScoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SCO_INITIAL_BMO_NUMBER; i++ )
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
        HttpScoDestroyBmoReqPool
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
HttpScoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
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
        HttpScoAcquireBmoRep
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
HttpScoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
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
        HttpScoReleaseBmoRep
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
HttpScoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
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
        HttpScoManufactureBmoRepPool
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
HttpScoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SCO_INITIAL_BMO_NUMBER; i++ )
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
        HttpScoDestroyBmoRepPool
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
HttpScoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetPeerAddresses
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPeerAddresses,
                PBOOL                       pbSameAsCurrent
            )

    description:

        This function is called to save resolved host addresses.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPeerAddresses
                The host addresses to be saved.

                PBOOL                       pbSameAsCurrent
                On return, it indicates whether the given addresses
                are the exact same as previously saved ones.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoSetPeerAddresses
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPeerAddresses,
        PBOOL                       pbSameAsCurrent
    )
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SCO_HOST_ADDRESSES        pCurAddr     = (PHTTP_SCO_HOST_ADDRESSES    )&pMyObject->PeerAddresses;
    PHTTP_SCO_HOST_ADDRESSES        pNewAddr     = (PHTTP_SCO_HOST_ADDRESSES    )hPeerAddresses;
    BOOL                            bSame        = TRUE;
    ULONG                           i;

    if ( pCurAddr->NumPeers != pNewAddr->NumPeers )
    {
        bSame   = FALSE;
    }
    else
    {
        ULONG                       ulNumAddr   = pNewAddr->NumPeers;

        for ( i = 0; i < ulNumAddr; i ++ )
        {
            ULONG                   j;
            ULONG                   ulAddr  = pNewAddr->PeerAddresses[i].Value;

            for ( j = 0; j < ulNumAddr; j ++ )
            {
                if ( pCurAddr->PeerAddresses[i].Value == ulAddr )
                {
                    break;
                }
            }

            if ( j >= ulNumAddr )
            {
                break;
            }
        }

        if ( i < ulNumAddr )
        {
            bSame   = FALSE;
        }
    }

    *pbSameAsCurrent    = bSame;

    if ( !bSame )
    {
        *pCurAddr               = *pNewAddr;
        pMyObject->LastPicked   = (ULONG)-1;
    }

    for ( i = 0; i < pCurAddr->NumPeers; i ++ )
    {
        pCurAddr->PeerPickFlags[i]  = 0;
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpScoGetNextPeerAddr
            (
                ANSC_HANDLE                 hThisObject,
                uint32_t                   *pPeerAddr
            )

    description:

        This function is called to get next peer address.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                uint32_t                   *pPeerAddr
                On successful return, it contains the next peer
                address to contact.

    return:     TRUE to indicate there is still peer address
                available, otherwise it means HTTP client has
                already tried all hosts.

**********************************************************************/

BOOL
HttpScoGetNextPeerAddr
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                   *pPeerAddr
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PHTTP_SCO_HOST_ADDRESSES        pCurAddr     = (PHTTP_SCO_HOST_ADDRESSES    )&pMyObject->PeerAddresses;
    BOOL                            bHasNext     = TRUE;
    ULONG                           SPMode       = pProperty->SPMode;
    ULONG                           ulPeers      = pCurAddr->NumPeers;
    ULONG                           ulLastPicked = pMyObject->LastPicked;
    
    switch ( SPMode )
    {
        default:
        case    HTTP_SCO_MHOST_MODE_SEQUENT:

                {
                    ULONG           i;
                    ULONG           ulIndex;
                    ULONG           ulStart = ( ulLastPicked == (ULONG)-1 ) ? 0 : ulLastPicked + 1;

                    if ( ulStart >= ulPeers )
                    {
                        ulStart = 0;
                    }

                    bHasNext = FALSE;

                    for ( i = 0; i < ulPeers; i ++ )
                    {
                        ulIndex = (i + ulStart) % ulPeers;

                        if ( pCurAddr->PeerPickFlags[ulIndex] )
                        {
                            continue;
                        }

                        *pPeerAddr  = pCurAddr->PeerAddresses[ulIndex].Value;
                        bHasNext    = TRUE;

                        pCurAddr->PeerPickFlags[ulIndex] = 1;
                        pMyObject->LastPicked            = ulIndex;

                        break;
                    }
                }

                break;

        case    HTTP_SCO_MHOST_MODE_RANDOM:

                {
                    ULONG               i;
                    ANSC_IPV4_ADDRESS   optAddr[HTTP_SCO_MAX_RESOLVED_HOST];
                    ULONG               optAddrIndex[HTTP_SCO_MAX_RESOLVED_HOST];
                    ULONG               optAddrNum   = 0;
                    ULONG               ulRand;
                    ULONG               ulIndex;
                    ULONG               ulRandSeed;

                    for ( i = 0; i < ulPeers; i ++ )
                    {
                        if ( !pCurAddr->PeerPickFlags[i] )
                        {
                            optAddr[optAddrNum].Value   = pCurAddr->PeerAddresses[i].Value;
                            optAddrIndex[optAddrNum]    = i;
                            optAddrNum ++;
                        }
                    }

                    if ( optAddrNum == 0 )
                    {
                        bHasNext    = FALSE;
                        break;
                    }

                    /*
                    ulRand      = _ansc_rand();
                    */

                    ulRandSeed  = AnscGetTickInMilliSeconds();
                    ulRand      = AnscCryptoGetRandomUlong(NULL, ulRandSeed);

                    ulIndex     = ulRand % optAddrNum;

                    *pPeerAddr  = optAddr[ulIndex].Value;

                    ulIndex     = optAddrIndex[ulIndex];
                    pCurAddr->PeerPickFlags[ulIndex] = 1;
                    pMyObject->LastPicked            = ulIndex;
                }

                break;
    }

    return bHasNext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpScoGetCurPeerAddr
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get current peer address.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     current picked peer address.

**********************************************************************/

ULONG
HttpScoGetCurPeerAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SCO_HOST_ADDRESSES        pCurAddr     = (PHTTP_SCO_HOST_ADDRESSES    )&pMyObject->PeerAddresses;
    ULONG                           ulPeers      = pCurAddr->NumPeers;
    ULONG                           ulLastPicked = pMyObject->LastPicked;

    if ( ulLastPicked >= ulPeers )
    {
        return 0;
    }
    else
    {
        return pCurAddr->PeerAddresses[ulLastPicked].Value;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoResetPeerAddrPick
            (
                ANSC_HANDLE                 hThisObject,
                uint32_t                    ipAddr
            )

    description:

        This function is called to reset peer address pick flag
        and mark the given address as selected.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                uint32_t                    ipAddr
                This address will be marked alone.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoResetPeerAddrPick
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SCO_HOST_ADDRESSES        pCurAddr     = (PHTTP_SCO_HOST_ADDRESSES    )&pMyObject->PeerAddresses;
    ULONG                           i, ulIndex   = (ULONG)-1;

    for ( i = 0; i < pCurAddr->NumPeers; i ++ )
    {
        if ( ipAddr == pCurAddr->PeerAddresses[i].Value )
        {
            pCurAddr->PeerPickFlags[i]  = 1;
            ulIndex = i;
        }
        else
        {
            pCurAddr->PeerPickFlags[i]  = 0;
        }
    }

    pMyObject->LastPicked   = ulIndex;

    return returnStatus;
}

