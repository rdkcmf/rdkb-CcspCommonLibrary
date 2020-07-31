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

    module:	http_pso_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Proxy Session Object.

        *   HttpPsoGetLastBmoReq
        *   HttpPsoGetLastBmoRep
        *   HttpPsoGetCurBmoReq
        *   HttpPsoGetCurBmoRep
        *   HttpPsoGetBmoReqByTid
        *   HttpPsoGetBmoRepByTid
        *   HttpPsoAddNewBmoReq
        *   HttpPsoAddNewBmoRep
        *   HttpPsoDelAllBmoReqs
        *   HttpPsoDelAllBmoReps

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/03/02    initial revision.

**********************************************************************/


#include "http_pso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetLastBmoReq
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the latest message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetLastBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    
    AnscAcquireLock(&pMyObject->BmoReqSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoReqSList);
    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    if ( pSLinkEntry )
    {
        pBmoReq = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetLastBmoRep
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the latest message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetLastBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    
    AnscAcquireLock(&pMyObject->BmoRepSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoRepSList);
    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    if ( pSLinkEntry )
    {
        pBmoRep = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetCurBmoReq
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetCurBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBmoState   = HTTP_BMO_STATE_EMPTY;

    AnscAcquireLock(&pMyObject->BmoReqSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoReqSList);
    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    if ( pSLinkEntry )
    {
        pBmoReq    = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);
        ulBmoState = pBmoReq->GetState((ANSC_HANDLE)pBmoReq);

        if ( (ulBmoState != HTTP_BMO_STATE_COMPLETE   ) &&
             (ulBmoState != HTTP_BMO_STATE_OVER_PACKED) )
        {
            return  (ANSC_HANDLE)pBmoReq;
        }
    }

    pBmoReq = (PHTTP_BMO_REQ_OBJECT)pMyObject->AddNewBmoReq((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetCurBmoRep
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetCurBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBmoState   = HTTP_BMO_STATE_EMPTY;

    AnscAcquireLock(&pMyObject->BmoRepSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoRepSList);
    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    if ( pSLinkEntry )
    {
        pBmoRep    = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);
        ulBmoState = pBmoRep->GetState((ANSC_HANDLE)pBmoRep);

        if ( (ulBmoState != HTTP_BMO_STATE_COMPLETE   ) &&
             (ulBmoState != HTTP_BMO_STATE_OVER_PACKED) )
        {
            return  (ANSC_HANDLE)pBmoRep;
        }
    }

    pBmoRep = (PHTTP_BMO_REP_OBJECT)pMyObject->AddNewBmoRep((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetBmoReqByTid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       id
            );

    description:

        This function is called to retrieve the current message object
        by matching the transaction id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       id
                Specifies the transaction id to be matched.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetBmoReqByTid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoReqSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoReqSList);

    while ( pSLinkEntry )
    {
        pBmoReq     = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pBmoReq->GetTransactionId((ANSC_HANDLE)pBmoReq) == id )
        {
            AnscReleaseLock(&pMyObject->BmoReqSListLock);

            return  (ANSC_HANDLE)pBmoReq;
        }
    }

    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetBmoRepByTid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       id
            );

    description:

        This function is called to retrieve the current message object
        by matching the transaction id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       id
                Specifies the transaction id to be matched.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetBmoRepByTid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoRepSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->BmoRepSList);

    while ( pSLinkEntry )
    {
        pBmoRep     = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pBmoRep->GetTransactionId((ANSC_HANDLE)pBmoRep) == id )
        {
            AnscReleaseLock(&pMyObject->BmoRepSListLock);

            return  (ANSC_HANDLE)pBmoRep;
        }
    }

    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoAddCurBmoReq
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a new message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoAddNewBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_SIMPLE_PROXY_OBJECT       pSimpleProxy = (PHTTP_SIMPLE_PROXY_OBJECT )pMyObject->hOwnerContext;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhClientIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhClientIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;

    pBmoReq = (PHTTP_BMO_REQ_OBJECT)pSimpleProxy->AcquireBmoReq((ANSC_HANDLE)pSimpleProxy);

    if ( !pBmoReq )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBmoReq->SetHfpIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf                         );
        pBmoReq->SetTmhIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pTmhClientIf                   );
        pBmoReq->SetTransactionId((ANSC_HANDLE)pBmoReq, AnscSListQueryDepth(&pMyObject->BmoReqSList));
    }

    AnscAcquireLock   (&pMyObject->BmoReqSListLock);
    AnscSListPushEntry(&pMyObject->BmoReqSList, &pBmoReq->Linkage);
    AnscReleaseLock   (&pMyObject->BmoReqSListLock);

    return  (ANSC_HANDLE)pBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoAddCurBmoRep
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a new message object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message object.

**********************************************************************/

ANSC_HANDLE
HttpPsoAddNewBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_SIMPLE_PROXY_OBJECT       pSimpleProxy = (PHTTP_SIMPLE_PROXY_OBJECT )pMyObject->hOwnerContext;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhServerIf;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PHTTP_REQUEST_INFO              pHttpReqInfo = NULL;

    pBmoRep = (PHTTP_BMO_REP_OBJECT)pSimpleProxy->AcquireBmoRep((ANSC_HANDLE)pSimpleProxy);

    if ( !pBmoRep )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBmoRep->SetHfpIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf                         );
        pBmoRep->SetTmhIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pTmhServerIf                   );
        pBmoRep->SetTransactionId((ANSC_HANDLE)pBmoRep, AnscSListQueryDepth(&pMyObject->BmoRepSList));
    }

    AnscAcquireLock   (&pMyObject->BmoRepSListLock);
    AnscSListPushEntry(&pMyObject->BmoRepSList, &pBmoRep->Linkage);
    AnscReleaseLock   (&pMyObject->BmoRepSListLock);

    pBmoReq =
        (PHTTP_BMO_REQ_OBJECT)pMyObject->GetBmoReqByTid
            (
                (ANSC_HANDLE)pMyObject,
                pBmoRep->GetTransactionId((ANSC_HANDLE)pBmoRep)
            );

    if ( pBmoReq )
    {
        pHttpReqInfo = (PHTTP_REQUEST_INFO)AnscAllocateMemory(sizeof(HTTP_REQUEST_INFO));

        if ( pHttpReqInfo )
        {
            *pHttpReqInfo = *(PHTTP_REQUEST_INFO)pBmoReq->GetReqInfo((ANSC_HANDLE)pBmoReq);
             pBmoRep->SetReqInfo
                    (
                        (ANSC_HANDLE)pBmoRep,
                        (ANSC_HANDLE)pHttpReqInfo
                    );
        }
    }

    return  (ANSC_HANDLE)pBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoDelAllBmoReqs
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all message objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoDelAllBmoReqs
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_SIMPLE_PROXY_OBJECT       pSimpleProxy = (PHTTP_SIMPLE_PROXY_OBJECT )pMyObject->hOwnerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoReqSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoReqSList);

    while ( pSLinkEntry )
    {
        pBmoReq      = ACCESS_HTTP_BMO_REQ_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->BmoReqSList);

        pSimpleProxy->ReleaseBmoReq
                (
                    (ANSC_HANDLE)pSimpleProxy,
                    (ANSC_HANDLE)pBmoReq
                );
    }

    AnscReleaseLock(&pMyObject->BmoReqSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoDelAllBmoReps
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all message objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoDelAllBmoReps
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_SIMPLE_PROXY_OBJECT       pSimpleProxy = (PHTTP_SIMPLE_PROXY_OBJECT )pMyObject->hOwnerContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BmoRepSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->BmoRepSList);

    while ( pSLinkEntry )
    {
        pBmoRep      = ACCESS_HTTP_BMO_REP_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->BmoRepSList);

        pSimpleProxy->ReleaseBmoRep
                (
                    (ANSC_HANDLE)pSimpleProxy,
                    (ANSC_HANDLE)pBmoRep
                );
    }

    AnscReleaseLock(&pMyObject->BmoRepSListLock);

    return  ANSC_STATUS_SUCCESS;
}
