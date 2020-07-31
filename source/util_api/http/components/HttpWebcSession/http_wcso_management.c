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

    module:	http_wcso_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Webc Session Object.

        *   HttpWcsoGetEndWcto
        *   HttpWcsoGetCurWcto
        *   HttpWcsoAddNewWcto
        *   HttpWcsoDelAllWctos

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#include "http_wcso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWcsoGetEndWcto
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the last record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpWcsoGetEndWcto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    
    AnscAcquireLock(&pMyObject->WctoSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WctoSList);

    if ( pSLinkEntry )
    {
        pWebcTrans = ACCESS_HTTP_WEBC_TRANS_OBJECT(pSLinkEntry);

        pWebcTrans->AcquireAccess((ANSC_HANDLE)pWebcTrans);
    }
    else
    {
        pWebcTrans = NULL;
    }

    AnscReleaseLock(&pMyObject->WctoSListLock);

    return  (ANSC_HANDLE)pWebcTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWcsoGetCurWcto
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpWcsoGetCurWcto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulWctoState  = HTTP_WCTO_STATE_INITIALIZED;

    AnscAcquireLock(&pMyObject->WctoSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WctoSList);

    if ( pSLinkEntry )
    {
        pWebcTrans  = ACCESS_HTTP_WEBC_TRANS_OBJECT(pSLinkEntry);
        ulWctoState = pWebcTrans->GetTransState((ANSC_HANDLE)pWebcTrans);

        if ( ulWctoState != HTTP_WCTO_STATE_FINISHED )
        {
            pWebcTrans->AcquireAccess((ANSC_HANDLE)pWebcTrans);

            AnscReleaseLock(&pMyObject->WctoSListLock);

            return  (ANSC_HANDLE)pWebcTrans;
        }
        else if ( pWebcTrans->IsLastTrans((ANSC_HANDLE)pWebcTrans) )
        {
            AnscReleaseLock(&pMyObject->WctoSListLock);

            return  (ANSC_HANDLE)NULL;
        }
    }

    AnscReleaseLock(&pMyObject->WctoSListLock);

    /*
     * Remove all existing HTTP transactions to reserve memory. Note that this MAY cause problem if
     * the asynchronous mode is enabled. However, I remember it's specified that HTTP client should
     * NOT start a new transaction until the old name is completed. So we should be OK.
     */
    pMyObject->DelAllWctos((ANSC_HANDLE)pMyObject);

    pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->AddNewWcto((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pWebcTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWcsoAddCurWcto
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a new record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpWcsoAddNewWcto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient    = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_BSP_INTERFACE             pBspIf           = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PHTTP_HFP_INTERFACE             pHfpIf           = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans       = NULL;

    pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pSimpleClient->AcquireWcto((ANSC_HANDLE)pSimpleClient);

    if ( !pWebcTrans )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebcTrans->SetBspIf          ((ANSC_HANDLE)pWebcTrans, (ANSC_HANDLE)pBspIf          );
        pWebcTrans->SetHfpIf          ((ANSC_HANDLE)pWebcTrans, (ANSC_HANDLE)pHfpIf          );
        pWebcTrans->SetWebcSession    ((ANSC_HANDLE)pWebcTrans, (ANSC_HANDLE)pMyObject       );
        pWebcTrans->SetTcpSimpleClient((ANSC_HANDLE)pWebcTrans, (ANSC_HANDLE)pTcpSimpleClient);
        pWebcTrans->SetTransState     ((ANSC_HANDLE)pWebcTrans, HTTP_WCTO_STATE_INITIALIZED  );
        pWebcTrans->Open              ((ANSC_HANDLE)pWebcTrans);
    }

    AnscAcquireLock(&pMyObject->WctoSListLock);

    AnscSListPushEntry(&pMyObject->WctoSList, &pWebcTrans->Linkage);

    pWebcTrans->AcquireAccess((ANSC_HANDLE)pWebcTrans);

    AnscReleaseLock(&pMyObject->WctoSListLock);

    return  (ANSC_HANDLE)pWebcTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDelAllWctos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDelAllWctos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject     = (PHTTP_WEBC_SESSION_OBJECT )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    AnscAcquireLock(&pMyObject->WctoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WctoSList);

    while ( pSLinkEntry )
    {
        pWebcTrans  = ACCESS_HTTP_WEBC_TRANS_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->WctoSList);

        pWebcTrans->AcquireAccess((ANSC_HANDLE)pWebcTrans);
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
        pWebcTrans->Close        ((ANSC_HANDLE)pWebcTrans);
        pWebcTrans->Return       ((ANSC_HANDLE)pWebcTrans);
    }

    AnscReleaseLock(&pMyObject->WctoSListLock);

    return  ANSC_STATUS_SUCCESS;
}
