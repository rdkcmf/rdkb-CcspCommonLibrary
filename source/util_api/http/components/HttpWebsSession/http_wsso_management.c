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

    module:	http_wsso_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Webs Session Object.

        *   HttpWssoGetEndWsto
        *   HttpWssoGetCurWsto
        *   HttpWssoAddNewWsto
        *   HttpWssoDelAllWstos

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


#include "http_wsso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWssoGetEndWsto
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
HttpWssoGetEndWsto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    
    AnscAcquireLock(&pMyObject->WstoSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WstoSList);

    if ( pSLinkEntry )
    {
        pWebsTrans = ACCESS_HTTP_WEBS_TRANS_OBJECT(pSLinkEntry);

        pWebsTrans->AcquireAccess((ANSC_HANDLE)pWebsTrans);
    }
    else
    {
        pWebsTrans = NULL;
    }

    AnscReleaseLock(&pMyObject->WstoSListLock);

    return  (ANSC_HANDLE)pWebsTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWssoGetCurWsto
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
HttpWssoGetCurWsto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulWstoState   = HTTP_WSTO_STATE_INITIALIZED;

    AnscAcquireLock(&pMyObject->WstoSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WstoSList);

    if ( pSLinkEntry )
    {
        pWebsTrans  = ACCESS_HTTP_WEBS_TRANS_OBJECT(pSLinkEntry);
        ulWstoState = pWebsTrans->GetTransState((ANSC_HANDLE)pWebsTrans);

        if ( (ulWstoState != HTTP_WSTO_STATE_ESTABLISHED) &&
             (ulWstoState != HTTP_WSTO_STATE_FINISHED   ) )
        {
            pWebsTrans->AcquireAccess((ANSC_HANDLE)pWebsTrans);

            AnscReleaseLock(&pMyObject->WstoSListLock);

            return  (ANSC_HANDLE)pWebsTrans;
        }
    }

    AnscReleaseLock(&pMyObject->WstoSListLock);

    /*
     * Remove all existing HTTP transactions to reserve memory. Note that this MAY cause problem if
     * the asynchronous mode is enabled. However, I remember it's specified that HTTP client should
     * NOT start a new transaction until the old name is completed. So we should be OK.
     */
    pMyObject->DelAllWstos((ANSC_HANDLE)pMyObject);

    pWebsTrans = (PHTTP_WEBS_TRANS_OBJECT)pMyObject->AddNewWsto((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pWebsTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWssoAddCurWsto
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
HttpWssoAddNewWsto
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT    )pMyObject->hOwnerContext;
    PHTTP_WSP_INTERFACE             pWspIf        = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = NULL;

    pWebsTrans = (PHTTP_WEBS_TRANS_OBJECT)pSimpleServer->AcquireWsto((ANSC_HANDLE)pSimpleServer);

    if ( !pWebsTrans )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pWebsTrans->SetWspIf      ((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)pWspIf             );
        pWebsTrans->SetHfpIf      ((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)pHfpIf             );
        pWebsTrans->SetWebsSession((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)pMyObject          );
        pWebsTrans->SetWebSocket  ((ANSC_HANDLE)pWebsTrans, (ANSC_HANDLE)pWebSocket         );
        pWebsTrans->SetTransState ((ANSC_HANDLE)pWebsTrans, HTTP_WSTO_STATE_CLIENT_CONNECTED);
        pWebsTrans->Open          ((ANSC_HANDLE)pWebsTrans);
    }

    AnscAcquireLock(&pMyObject->WstoSListLock);

    AnscSListPushEntry(&pMyObject->WstoSList, &pWebsTrans->Linkage);

    pWebsTrans->AcquireAccess((ANSC_HANDLE)pWebsTrans);

    AnscReleaseLock(&pMyObject->WstoSListLock);

    return  (ANSC_HANDLE)pWebsTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoDelAllWstos
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
HttpWssoDelAllWstos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    AnscAcquireLock(&pMyObject->WstoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->WstoSList);

    while ( pSLinkEntry )
    {
        pWebsTrans  = ACCESS_HTTP_WEBS_TRANS_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->WstoSList);

        pWebsTrans->AcquireAccess((ANSC_HANDLE)pWebsTrans);
        pWebsTrans->ReleaseAccess((ANSC_HANDLE)pWebsTrans);
        pWebsTrans->Close        ((ANSC_HANDLE)pWebsTrans);
        pWebsTrans->Return       ((ANSC_HANDLE)pWebsTrans);
    }

    AnscReleaseLock(&pMyObject->WstoSListLock);

    return  ANSC_STATUS_SUCCESS;
}
