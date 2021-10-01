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

    module:	http_wcso_sctowo.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced scto-worker functions
        of the Http Webc Session Object.

        *   HttpWcsoSctowoQuery
        *   HttpWcsoSctowoProcess
        *   HttpWcsoSctowoNotify

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

        ULONG
        HttpWcsoSctowoQuery
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called by the internal Client Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

    return:     process mode.

**********************************************************************/

ULONG
HttpWcsoSctowoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = (PANSC_BUFFER_DESCRIPTOR       )pTcpSimpleClient->GetBufferContext((ANSC_HANDLE)pTcpSimpleClient);
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans       = (PHTTP_WEBC_TRANS_OBJECT       )NULL;
    ULONG                           ulSctoPmode      = ANSC_SCTOWO_PMODE_PROCESS;
    ULONG                           ulWctoQmode      = HTTP_WCTO_QMODE_PROCESS;

    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    pMyObject->Timestamp = AnscGetTickInSeconds();
    pWebcTrans           = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetCurWcto((ANSC_HANDLE)pMyObject);

    if ( !pWebcTrans )
    {
        pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

        return  ANSC_SCTOWO_PMODE_PROCESS;
    }

    pBufferDesp->BlockSize += ulSize;
    ulWctoQmode             =
        pWebcTrans->Query
            (
                (ANSC_HANDLE)pWebcTrans,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulWctoQmode )
    {
        case    HTTP_WCTO_QMODE_COLLECT :

                ulSctoPmode = ANSC_SCTOWO_PMODE_COLLECT;

                break;

        case    HTTP_WCTO_QMODE_PROCESS :

                ulSctoPmode = ANSC_SCTOWO_PMODE_PROCESS;

                break;

        default :

                ulSctoPmode = ANSC_SCTOWO_PMODE_COLLECT;

                break;
    }

    if ( ulSctoPmode == ANSC_SCTOWO_PMODE_COLLECT )
    {
        pBufferDesp->BlockSize -= ulSize;
    }

    pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
    pMyObject ->ReleaseAccess((ANSC_HANDLE)pMyObject );

    return  ulSctoPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSctowoProcess
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called by the internal Client Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSctowoProcess
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = (PANSC_BUFFER_DESCRIPTOR       )pTcpSimpleClient->GetBufferContext((ANSC_HANDLE)pTcpSimpleClient);
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans       = (PHTTP_WEBC_TRANS_OBJECT       )NULL;

    if ( pMyObject->bRemoveMe )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    pMyObject->Timestamp = AnscGetTickInSeconds();
    pWebcTrans           = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetCurWcto((ANSC_HANDLE)pMyObject);

    if ( !pWebcTrans )
    {
        /*
        pMyObject->Close((ANSC_HANDLE)pMyObject);
        */
        pMyObject->bRemoveMe = TRUE;

        pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
#ifndef  _ANSC_HTTP_SUPPORT_PIPELING
    else if ( pWebcTrans->TransState != HTTP_WCTO_STATE_REQ_SENT &&
              pWebcTrans->TransState != HTTP_WCTO_STATE_REP_RECEIVING )
    {
        /* received garbage data from web server, drop it siliently */
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
        pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

        return ANSC_STATUS_SUCCESS;
    }
#endif
    else
    {
        pTcpSimpleClient->SetBufferContext
            (
                (ANSC_HANDLE)pTcpSimpleClient,
                NULL,
                0,
                (ANSC_HANDLE)NULL
            );
    }

    buffer       = AnscBdoGetBlock    (pBufferDesp);
    ulSize       = AnscBdoGetBlockSize(pBufferDesp);

#ifdef   _DEBUG
    AnscTrace("HttpWcsoSctowoProcess - processing buffer %8p (size = %lu)\n", buffer, ulSize);
#endif

    returnStatus =
        pWebcTrans->Recv
            (
                (ANSC_HANDLE)pWebcTrans,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    if ( (returnStatus != ANSC_STATUS_SUCCESS    ) &&
         (returnStatus != ANSC_STATUS_DO_IT_AGAIN) )
    {
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);

        /*
        pMyObject->Close((ANSC_HANDLE)pMyObject);
        */
        pMyObject->bRemoveMe = TRUE;
    }
    else if ( (pWebcTrans->GetTransState((ANSC_HANDLE)pWebcTrans) == HTTP_WCTO_STATE_FINISHED) &&
              (pWebcTrans->IsLastTrans  ((ANSC_HANDLE)pWebcTrans) == TRUE                    ) )
    {
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);

        /*
        pMyObject->Close((ANSC_HANDLE)pMyObject);
        */
        pMyObject->bRemoveMe = TRUE;
    }
    else
    {
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
    }

    pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSctowoNotify
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the internal Client Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the event to be notified.

                ANSC_HANDLE                 hReserved
                Specifies the transparent context associated with event.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSctowoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf           = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans       = (PHTTP_WEBC_TRANS_OBJECT       )NULL;
    
    if ( pMyObject->bRemoveMe )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pMyObject->SessionState == HTTP_WCSO_STATE_FINISHED )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetCurWcto((ANSC_HANDLE)pMyObject);
    pMyObject->Timestamp = AnscGetTickInSeconds();

    if ( TRUE )
    {
        AnscTrace("HttpWcsoSctowoNotify - Notify critical event %lu\n", ulEvent);
        /* make sure critical events are not lost */
        switch ( ulEvent )
        {
            case    ANSC_SCTOWO_EVENT_SOCKET_ERROR :
            case    ANSC_SCTOWO_EVENT_SOCKET_TIMEOUT :
            case    ANSC_SCTOWO_EVENT_RESOURCES :

                pBspIf->Notify
                        (
                            pBspIf->hOwnerContext,
                            HTTP_BSP_EVENT_NETWORK_FAILURE,
                            0,
                            pMyObject->hBspReqContext,
                            NULL,
                            NULL
                        );

                break;

            case    ANSC_SCTOWO_EVENT_TLS_TIMEOUT :
                    pBspIf->Notify
                            (
                                pBspIf->hOwnerContext,
                                HTTP_BSP_EVENT_TLS_TIMEOUT,
                                0,
                                pMyObject->hBspReqContext,
                                NULL,
                                NULL
                            );

                    break;

            case    ANSC_SCTOWO_EVENT_TLS_ERROR :

                    pBspIf->Notify
                            (
                                pBspIf->hOwnerContext,
                                HTTP_BSP_EVENT_TLS_ERROR,
                                (ULONG)hReserved,
                                pMyObject->hBspReqContext,
                                NULL,
                                NULL
                            );

                    break;

            case    ANSC_SCTOWO_EVENT_SOCKET_CLOSED :

							pBspIf->Notify
							(
								pBspIf->hOwnerContext,
								HTTP_BSP_EVENT_SOCKET_CLOSED,
								0,
								pMyObject->hBspReqContext,
                                pWebcTrans?pWebcTrans->hBmoReq:NULL,
                                pWebcTrans?pWebcTrans->hBmoRep:NULL
							);

                    break;
        }

    }

    if ( pWebcTrans )
    {
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
    }

    pMyObject->bRemoveMe = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoReqRetryTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread which tries next peer of
        the given URL.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoReqRetryTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    
    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    returnStatus = pMyObject->ConnectPeer((ANSC_HANDLE)pMyObject, pMyObject->RetryPeerAddr);

    pMyObject->RetryPeerAddr    = 0;

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pMyObject->ConnectionFailed((ANSC_HANDLE)pMyObject);
    }
    else
    {
        ANSC_HANDLE                 hReqUri = pMyObject->hReqUri;

        pMyObject->SessionState = HTTP_WCSO_STATE_SERVER_CONNECTED;

        pMyObject->hReqUri  = NULL;

        /* re-send the request to the new peer */
        returnStatus =
            pMyObject->Request
                (
                    (ANSC_HANDLE)pMyObject,
                    pMyObject->ReqMethod,
                    hReqUri,
                    pMyObject->hBspReqContext
                );

        AnscFreeMemory(hReqUri);
    }

    pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

