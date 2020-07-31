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

    module:	http_apo_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Advanced Proxy Object.

        *   HttpApoAcquirePso
        *   HttpApoReleasePso
        *   HttpApoManufacturePsoPool
        *   HttpApoDestroyPsoPool
        *   HttpApoAcquireTro
        *   HttpApoReleaseTro
        *   HttpApoManufactureTroPool
        *   HttpApoDestroyTroPool
        *   HttpApoAcquireBmoReq
        *   HttpApoReleaseBmoReq
        *   HttpApoManufactureBmoReqPool
        *   HttpApoDestroyBmoReqPool
        *   HttpApoAcquireBmoRep
        *   HttpApoReleaseBmoRep
        *   HttpApoManufactureBmoRepPool
        *   HttpApoDestroyBmoRepPool

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_apo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoAcquirePso
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
HttpApoAcquirePso
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_PSO_VER2_OBJECT           pPsoVer2     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PsoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);
    AnscReleaseLock(&pMyObject->PsoSListLock);

    if ( pSLinkEntry )
    {
        pPsoVer2 = ACCESS_HTTP_PSO_VER2_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pPsoVer2;
    }

    pPsoVer2 =
        (PHTTP_PSO_VER2_OBJECT)HttpCreatePsoVer2
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pPsoVer2 )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pPsoVer2->SetClientSocket((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
        pPsoVer2->SetSbcContext  ((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
        pPsoVer2->SetCbcContext  ((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
    }

    return  (ANSC_HANDLE)pPsoVer2;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoReleasePso
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
HttpApoReleasePso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPso
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_PSO_VER2_OBJECT           pPsoVer2     = (PHTTP_PSO_VER2_OBJECT )hPso;

    pPsoVer2->Reset((ANSC_HANDLE)pPsoVer2);

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        pPsoVer2->Remove((ANSC_HANDLE)pPsoVer2);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->PsoSListLock);
        AnscSListPushEntry(&pMyObject->PsoSList, &pPsoVer2->Linkage);
        AnscReleaseLock   (&pMyObject->PsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoManufacturePsoPool
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
HttpApoManufacturePsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_PSO_VER2_OBJECT           pPsoVer2     = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_APO_INITIAL_PSO_NUMBER; i++ )
    {
        pPsoVer2 =
            (PHTTP_PSO_VER2_OBJECT)HttpCreatePsoVer2
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pPsoVer2 )
        {
            continue;
        }
        else
        {
            pPsoVer2->SetClientSocket((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
            pPsoVer2->SetSbcContext  ((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
            pPsoVer2->SetCbcContext  ((ANSC_HANDLE)pPsoVer2, (ANSC_HANDLE)NULL);
        }

        AnscAcquireLock   (&pMyObject->PsoSListLock);
        AnscSListPushEntry(&pMyObject->PsoSList, &pPsoVer2->Linkage);
        AnscReleaseLock   (&pMyObject->PsoSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoDestroyPsoPool
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
HttpApoDestroyPsoPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_PSO_VER2_OBJECT           pPsoVer2     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PsoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);

    while ( pSLinkEntry )
    {
        pPsoVer2    = ACCESS_HTTP_PSO_VER2_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->PsoSList);

        pPsoVer2->Remove((ANSC_HANDLE)pPsoVer2);
    }

    AnscReleaseLock(&pMyObject->PsoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoAcquireTro
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
HttpApoAcquireTro
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->TroSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->TroSList);
    AnscReleaseLock(&pMyObject->TroSListLock);

    if ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pTransRecord;
    }

    pTransRecord =
        (PHTTP_TRANS_RECORD_OBJECT)HttpCreateTransRecord
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pTransRecord )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pTransRecord->SetClientSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
        pTransRecord->SetServerSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
        pTransRecord->SetSbcContext  ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
        pTransRecord->SetCbcContext  ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
    }

    return  (ANSC_HANDLE)pTransRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoReleaseTro
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTro
            );

    description:

        This function is called to release a resource object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTro
                Specifies the resource object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoReleaseTro
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTro
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hTro;

    pTransRecord->Reset((ANSC_HANDLE)pTransRecord);

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        pTransRecord->Remove((ANSC_HANDLE)pTransRecord);
    }
    else
    {
        AnscAcquireLock   (&pMyObject->TroSListLock);
        AnscSListPushEntry(&pMyObject->TroSList, &pTransRecord->Linkage);
        AnscReleaseLock   (&pMyObject->TroSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoManufactureTroPool
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
HttpApoManufactureTroPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_APO_INITIAL_TRO_NUMBER; i++ )
    {
        pTransRecord =
            (PHTTP_TRANS_RECORD_OBJECT)HttpCreateTransRecord
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTransRecord )
        {
            continue;
        }
        else
        {
            pTransRecord->SetClientSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
            pTransRecord->SetServerSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
            pTransRecord->SetSbcContext  ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
            pTransRecord->SetCbcContext  ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL);
        }

        AnscAcquireLock   (&pMyObject->TroSListLock);
        AnscSListPushEntry(&pMyObject->TroSList, &pTransRecord->Linkage);
        AnscReleaseLock   (&pMyObject->TroSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoDestroyTroPool
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
HttpApoDestroyTroPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->TroSList);

    while ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->TroSList);

        pTransRecord->Remove((ANSC_HANDLE)pTransRecord);
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoAcquireBmoReq
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
HttpApoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
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
        HttpApoReleaseBmoReq
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
HttpApoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    pBmoReq->Reset((ANSC_HANDLE)pBmoReq);

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
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
        HttpApoManufactureBmoReqPool
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
HttpApoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_APO_INITIAL_BMO_NUMBER; i++ )
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
        HttpApoDestroyBmoReqPool
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
HttpApoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
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
        HttpApoAcquireBmoRep
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
HttpApoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
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
        HttpApoReleaseBmoRep
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
HttpApoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT         )hBmoRep;

    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
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
        HttpApoManufactureBmoRepPool
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
HttpApoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->ProxyMode & HTTP_APO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < HTTP_APO_INITIAL_BMO_NUMBER; i++ )
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
        HttpApoDestroyBmoRepPool
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
HttpApoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
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
