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

    module:	http_sso_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Simple Server Object.

        *   HttpSsoAcquireWsso
        *   HttpSsoReleaseWsso
        *   HttpSsoManufactureWssoPool
        *   HttpSsoDestroyWssoPool
        *   HttpSsoAcquireWsto
        *   HttpSsoReleaseWsto
        *   HttpSsoManufactureWstoPool
        *   HttpSsoDestroyWstoPool
        *   HttpSsoAcquireBmoReq
        *   HttpSsoReleaseBmoReq
        *   HttpSsoManufactureBmoReqPool
        *   HttpSsoDestroyBmoReqPool
        *   HttpSsoAcquireBmoRep
        *   HttpSsoReleaseBmoRep
        *   HttpSsoManufactureBmoRepPool
        *   HttpSsoDestroyBmoRepPool

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSsoAcquireWsso
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
HttpSsoAcquireWsso
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WssoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->WssoSList);
    AnscReleaseLock(&pMyObject->WssoSListLock);

    if ( pSLinkEntry )
    {
        pWebsSession = ACCESS_HTTP_WEBS_SESSION_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pWebsSession;
    }

    pWebsSession =
        (PHTTP_WEBS_SESSION_OBJECT)HttpCreateWebsSession
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pWebsSession )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebsSession->SetWebSocket   ((ANSC_HANDLE)pWebsSession, (ANSC_HANDLE)NULL          );
        pWebsSession->SetSessionState((ANSC_HANDLE)pWebsSession, HTTP_WSSO_STATE_INITIALIZED);
    }

    return  (ANSC_HANDLE)pWebsSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoReleaseWsso
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWsso
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWsso
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoReleaseWsso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWsso
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession = (PHTTP_WEBS_SESSION_OBJECT   )hWsso;

    pWebsSession->Reset((ANSC_HANDLE)pWebsSession);

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        pWebsSession->Remove((ANSC_HANDLE)pWebsSession);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->WssoSListLock);
        AnscSListPushEntry(&pMyObject->WssoSList, &pWebsSession->Linkage);
        AnscReleaseLock   (&pMyObject->WssoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoManufactureWssoPool
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
HttpSsoManufactureWssoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SSO_INITIAL_WSSO_NUMBER; i++ )
    {
        pWebsSession =
            (PHTTP_WEBS_SESSION_OBJECT)HttpCreateWebsSession
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebsSession )
        {
            continue;
        }
        else
        {
            pWebsSession->SetWebSocket   ((ANSC_HANDLE)pWebsSession, (ANSC_HANDLE)NULL          );
            pWebsSession->SetSessionState((ANSC_HANDLE)pWebsSession, HTTP_WSSO_STATE_INITIALIZED);
        }

        AnscAcquireLock   (&pMyObject->WssoSListLock);
        AnscSListPushEntry(&pMyObject->WssoSList, &pWebsSession->Linkage);
        AnscReleaseLock   (&pMyObject->WssoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoDestroyWssoPool
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
HttpSsoDestroyWssoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WssoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WssoSList);

    while ( pSLinkEntry )
    {
        pWebsSession = ACCESS_HTTP_WEBS_SESSION_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->WssoSList);

        pWebsSession->Remove((ANSC_HANDLE)pWebsSession);
    }

    AnscReleaseLock(&pMyObject->WssoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSsoAcquireWsto
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
HttpSsoAcquireWsto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WstoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->WstoSList);
    AnscReleaseLock(&pMyObject->WstoSListLock);

    if ( pSLinkEntry )
    {
        pWebsTrans = ACCESS_HTTP_WEBS_TRANS_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pWebsTrans;
    }

    pWebsTrans =
        (PHTTP_WEBS_TRANS_OBJECT)HttpCreateWebsTrans
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pWebsTrans )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebsTrans->SetWebSocket ((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)NULL          );
        pWebsTrans->SetTransState((ANSC_HANDLE)pWebsTrans, HTTP_WSTO_STATE_INITIALIZED);
    }

    return  (ANSC_HANDLE)pWebsTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoReleaseWsto
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWsto
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWsto
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoReleaseWsto
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWsto
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = (PHTTP_WEBS_TRANS_OBJECT     )hWsto;

    pWebsTrans->Reset((ANSC_HANDLE)pWebsTrans);

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        pWebsTrans->Remove((ANSC_HANDLE)pWebsTrans);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->WstoSListLock);
        AnscSListPushEntry(&pMyObject->WstoSList, &pWebsTrans->Linkage);
        AnscReleaseLock   (&pMyObject->WstoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoManufactureWstoPool
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
HttpSsoManufactureWstoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SSO_INITIAL_WSTO_NUMBER; i++ )
    {
        pWebsTrans =
            (PHTTP_WEBS_TRANS_OBJECT)HttpCreateWebsTrans
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebsTrans )
        {
            continue;
        }
        else
        {
            pWebsTrans->SetWebSocket ((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)NULL          );
            pWebsTrans->SetTransState((ANSC_HANDLE)pWebsTrans, HTTP_WSTO_STATE_INITIALIZED);
        }

        AnscAcquireLock   (&pMyObject->WstoSListLock);
        AnscSListPushEntry(&pMyObject->WstoSList, &pWebsTrans->Linkage);
        AnscReleaseLock   (&pMyObject->WstoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoDestroyWstoPool
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
HttpSsoDestroyWstoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->WstoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WstoSList);

    while ( pSLinkEntry )
    {
        pWebsTrans  = ACCESS_HTTP_WEBS_TRANS_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->WstoSList);

        pWebsTrans->Remove((ANSC_HANDLE)pWebsTrans);
    }

    AnscReleaseLock(&pMyObject->WstoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSsoAcquireBmoReq
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
HttpSsoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
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
        HttpSsoReleaseBmoReq
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
HttpSsoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;

    pBmoReq->Reset((ANSC_HANDLE)pBmoReq);

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
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
        HttpSsoManufactureBmoReqPool
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
HttpSsoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SSO_INITIAL_BMO_NUMBER; i++ )
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
        HttpSsoDestroyBmoReqPool
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
HttpSsoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
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
        HttpSsoAcquireBmoRep
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
HttpSsoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
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
        HttpSsoReleaseBmoRep
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
HttpSsoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
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
        HttpSsoManufactureBmoRepPool
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
HttpSsoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_SSO_INITIAL_BMO_NUMBER; i++ )
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
        HttpSsoDestroyBmoRepPool
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
HttpSsoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
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
