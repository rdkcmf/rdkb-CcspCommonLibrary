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

    module:	http_wsto_tmh.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Webs Trans Object.

        *   HttpWstoTmhNotify
        *   HttpWstoTmhSerialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#include "http_wsto_global.h"


static BOOL
HttpWstoTmhIsMultipart
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT   )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hOwnerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT      )hBmoReq;
    PHTTP_BCC_INTERFACE             pBccIf        = (PHTTP_BCC_INTERFACE       )pBmoReq->hBccIf;
    PHTTP_FUM_INTERFACE             pFumIf        = (PHTTP_FUM_INTERFACE       )pSimpleServer->GetFumIf((ANSC_HANDLE)pSimpleServer);
    PHTTP_CGI_INTERFACE             pCgiIf        = (PHTTP_CGI_INTERFACE       )pBmoReq->hCgiIf;
    PUCHAR                          pReqUri       = (PUCHAR                    )pCgiIf->GetPathInfo(pCgiIf->hOwnerContext);
    ANSC_HANDLE                     hMdhIf        = (ANSC_HANDLE               )NULL;

    hMdhIf = pFumIf->GetMdhIf(pFumIf->hOwnerContext, pReqUri);

    if ( !hMdhIf )
    {
        return FALSE;
    }
    else
    {
        PHTTP_HFO_CONTENT_TYPE  pContentType;

        pContentType = 
            (PHTTP_HFO_CONTENT_TYPE)pBccIf->GetHeaderField
                (
                    pBccIf->hOwnerContext, 
                    HTTP_HEADER_ID_CONTENT_TYPE
                );

        if ( (pContentType->MediaType == IANA_MEDIA_TYPE_CODE_MULTIPART ) &&
             (pContentType->SubType   == IANA_MT_MP_STYPE_CODE_FORM_DATA) )
        {
            return TRUE;
        }
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoTmhNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the BMO object to process signi-
        ficant HTTP transaction events.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the HTTP message object who made the call.

                ULONG                       ulEvent
                Specifies the transaction event to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoTmhNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT    )pMyObject->hOwnerContext;
    PHTTP_WSP_INTERFACE             pWspIf        = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession  = (PHTTP_WEBS_SESSION_OBJECT     )pMyObject->hWebsSession;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT          )hMessage;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hReserved;
    PANSC_BUFFER_DESCRIPTOR         pNewBdo       = NULL;
    ULONG                           ulBodySize    = 0;
    ULONG                           ulPartSize    = 0;

    if ( pMyObject->TransState == HTTP_WSTO_STATE_FINISHED )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    /*
     * There're two types of event categorized by the associated processing focus: trigger-event
     * and final-event. Trigger event solicits certain actions to be taken, such as making a filter
     * decision based on header field values. Final event, on the other hand, signals the end of
     * current state-transition.
     *
     *      event_arrived_header:        trigger event - header inspection
     *      event_arrived_end_of_header: final event   - prepare for body receiving
     *      event_arrived_body_part:     both          - prepare for more body
     *      event_arrived_body:          trigger event - body inspection
     *      event_completed:             final event   - prepare for next transaction
     *      event_over_packed:           final event   - prepare for next transaction
     */
    switch ( ulEvent )
    {
        case    HTTP_TMH_EVENT_ARRIVED_HEADER :

                pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_STORE);

                /*
                 * We're here if all HTTP headers have arrived. We cannot rely on the reception of
                 * "end_of_header" event to apply body size limitations. The "end_of_header" event
                 * will not be indicated if HTTP headers and (partial) body are transmitted in the
                 * same TCP segment (or the underlying TCP module may have aggregated the multiple
                 * segments). If we can identify the boundary between HTTP headers and the body,
                 * depending on the body size limitation, we may be able to allocate a single flat
                 * buffer to store the message body; if we cannot identify the boundary however, a
                 * buffer of "max_message_size" will be allocated.
                 */
                ulBodySize = pBmoReq->PredictBodySize((ANSC_HANDLE)pBmoReq);

                if ( (ulBodySize != 0xFFFFFFFF) && (ulBodySize > 0) )
                {
                    if ( pSimpleServer->Property.ServerMode & HTTP_SERVER_MODE_memoryConserving )
                    {
                        if ( ulBodySize > pSimpleServer->Property.MaxBodySizeCap )
                        {
                            /*
                            pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);

                            pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                            returnStatus          = ANSC_STATUS_SUCCESS;

                            break;
                            */
                            
                            if ( !HttpWstoTmhIsMultipart(hThisObject, hMessage) )
                            {
                                pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_EMPTY);
                            }
                        }
                    }
                }

                break;

        case    HTTP_TMH_EVENT_END_OF_HEADER :

                /*
                 * The appearance of this event indicates that there's message body data following
                 * the header fields; otherwise, EVENT_COMPLETED or EVENT_OVER_PACKED should be
                 * notified instead.
                 */
                ulBodySize = pBmoReq->PredictBodySize((ANSC_HANDLE)pBmoReq);

                if ( ulBodySize > HTTP_SSO_MAX_MESSAGE_SIZE )
                {
                    ulBodySize = HTTP_SSO_MAX_MESSAGE_SIZE;
                }

                if ( ulBodySize == 0xFFFFFFFF )
                {
                    ulBodySize = HTTP_SSO_MAX_MESSAGE_SIZE;
                }
                else if ( pSimpleServer->Property.ServerMode & HTTP_SERVER_MODE_memoryConserving )
                {
                    if ( ulBodySize > pSimpleServer->Property.MaxBodySizeCap )
                    {
                        /*
                        pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);

                        pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                        returnStatus          = ANSC_STATUS_SUCCESS;

                        break;
                        */
                        if ( !HttpWstoTmhIsMultipart(hThisObject, hMessage) )
                        {
                            pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_EMPTY);
                        }
                    }
                    else if ( ulBodySize > pSimpleServer->Property.ReqBodySizeCap )
                    {
                        ulBodySize = HTTP_SSO_MAX_MESSAGE_SIZE;
                    }
                    else
                    {
                        ulBodySize += HTTP_SSO_RECV_BUFFER_ROOM;
                    }
                }
                else
                {
                    ulBodySize += HTTP_SSO_RECV_BUFFER_ROOM;
                }

                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pWebSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pWebSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY_PART :

                /*
                 * There're some body parts already arrived, and now we need allocate more buffers
                 * for more body parts. If we know the size of the rest of the body, we allocate a
                 * descriptor big enough for holding the rest payload; otherwise, we use the pre-
                 * defined message size.
                 */
                ulBodySize = pBmoReq->PredictBodySize((ANSC_HANDLE)pBmoReq);
                ulPartSize = HTTP_SSO_MAX_MESSAGE_SIZE;

                if ( ulBodySize == 0xFFFFFFFF )
                {
                    if ( pSimpleServer->Property.ServerMode & HTTP_SERVER_MODE_memoryConserving )
                    {
                        if ( pBmoReq->GetBodySize((ANSC_HANDLE)pBmoReq) > pSimpleServer->Property.MaxBodySizeCap )
                        {
                            /*
                            pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);

                            pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                            returnStatus          = ANSC_STATUS_SUCCESS;

                            break;
                            */
                            if ( !HttpWstoTmhIsMultipart(hThisObject, hMessage) )
                            {
                                pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_EMPTY);
                            }
                        }
                    }
                }

                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulPartSize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pWebSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pWebSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                /*
                 * If this is a big file transfer, we need to notify the web server so the web
                 * session doesn't timeout.
                 */
                if ( pBmoReq->GetBodySize((ANSC_HANDLE)pBmoReq) > 200000 )
                {
                    returnStatus =
                        pWspIf->Notify
                            (
                                pWspIf->hOwnerContext,
                                (ANSC_HANDLE)pBmoReq,
                                0           /* reserved for future use */
                            );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY :

                break;

        case    HTTP_TMH_EVENT_COMPLETED :

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the wsto_query() function next time.
                 */
                pMyObject->TransState = HTTP_WSTO_STATE_ESTABLISHED;

                ulBodySize = HTTP_SSO_MAX_HEADERS_SIZE;
                pNewBdo    =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pWebSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pWebSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                returnStatus =
                    pWspIf->Serve
                        (
                            pWspIf->hOwnerContext,
                            (ANSC_HANDLE)pWebsSession,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_TMH_EVENT_OVER_PACKED :

                returnStatus =
                    pWspIf->Serve
                        (
                            pWspIf->hOwnerContext,
                            (ANSC_HANDLE)pWebsSession,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the wsto_query() function next time.
                 */
                pMyObject->TransState = HTTP_WSTO_STATE_ESTABLISHED;

                pWebSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pWebSocket,
                        AnscBdoGetBlock     (pBufferDesp),
                        AnscBdoGetUsableSize(pBufferDesp),
                        (ANSC_HANDLE)pBufferDesp
                    );

                pWebSocket->RecvPacketSize = AnscBdoGetBlockSize(pBufferDesp);
                returnStatus               = ANSC_STATUS_DO_IT_AGAIN;

                AnscBdoSetBlockSize(pBufferDesp, 0);

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoTmhSerialize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hSerializeContext
            );

    description:

        This function is called by the BMO object to output the HTTP
        stream payload data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the HTTP message object who made the call.

                PVOID                       buffer
                Specifies the payload data buffer to be serialized.

                ULONG                       ulSize
                Specifies the payload buffer size.

                ANSC_HANDLE                 hSerializeContext
                Specifies the context used for serialization.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoTmhSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    )
{
    UNREFERENCED_PARAMETER(hMessage);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;

    if ( hSerializeContext == (ANSC_HANDLE)pWebSocket )
    {
        returnStatus =
            pWebSocket->Send
                (
                    (ANSC_HANDLE)pWebSocket,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}
