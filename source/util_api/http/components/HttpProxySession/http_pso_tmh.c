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

    module:	http_pso_tmh.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Proxy Session Object.

        *   HttpPsoTmhClientNotify
        *   HttpPsoTmhClientSerialize
        *   HttpPsoTmhServerNotify
        *   HttpPsoTmhServerSerialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/25/02    initial revision.

**********************************************************************/


#include "http_pso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoTmhClientNotify
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
HttpPsoTmhClientNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PHTTP_SBC_INTERFACE             pSbcIf         = (PHTTP_SBC_INTERFACE           )pMyObject->hSbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf         = (PHTTP_PBC_INTERFACE           )pMyObject->hPbcIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )hMessage;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hReserved;
    PANSC_BUFFER_DESCRIPTOR         pNewBdo        = NULL;
    ULONG                           ulSbcPmode     = HTTP_SBC_PMODE_RELAY1;
    ULONG                           ulBodySize     = 0;
    ULONG                           ulPartSize     = 0;

    if ( pMyObject->SessionState == HTTP_PSO_STATE_FINISHED )
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

                /*
                 * If this request is the first message of the session, the HTTP server connection
                 * must haven't been made yet: we first connect to the target server (with the per-
                 * mission from the WAM object) before processing the header fields; otherwise, we
                 * jumpstart the header processing for the SBC object.
                 */
                if ( (pMyObject->SessionState == HTTP_PSO_STATE_INITIALIZED     ) ||
                     (pMyObject->SessionState == HTTP_PSO_STATE_CLIENT_CONNECTED) )
                {
                    returnStatus =
                        pMyObject->ConnectToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        return  returnStatus;
                    }
                    else if ( pMyObject->SessionState == HTTP_PSO_STATE_FINISHED )
                    {
                        return  returnStatus;
                    }
                    else
                    {
                        pMyObject->SessionState = HTTP_PSO_STATE_ESTABLISHED;
                        pServerSocket           = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;
                    }
                }

                /*
                 * The SBC object now has a chance to examine all the HTTP header fields and make
                 * a decision on how to proceed with the current HTTP transaction.
                 */
                returnStatus =
                    pPbcIf->RecvReqHeaders
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                ulSbcPmode   =
                    pSbcIf->RequestHeaders
                        (
                            pSbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                pMyObject->SbcPmode = ulSbcPmode;

                if ( ulSbcPmode == HTTP_SBC_PMODE_RELAY1 )
                {
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_RELAY);

                    returnStatus =
                        pMyObject->SendToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_RELAY2 )
                {
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_RELAY);

                    returnStatus =
                        pMyObject->SendToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_STORE )
                {
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_STORE);

                    returnStatus =
                        pMyObject->SendToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_QUEUE )
                {
                    /*
                     * We don't have to do anything at this moment, the final decision will be made
                     * by the SBC object when the complete message body arrives ...
                     */
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_STORE);
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_RESET )
                {
                    /*
                     * We don't have to do anything at this moment, we will drop the whole message
                     * including both headers and body completely when it arrives ...
                     */
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_RELAY);
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_CLOSE )
                {
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_RELAY);

                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_RELAY);

                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }

                break;

        case    HTTP_TMH_EVENT_END_OF_HEADER :

                /*
                 * The appearance of this event indicates that there's message body data following
                 * the header fields; otherwise, EVENT_COMPLETED or EVENT_OVER_PACKED should be
                 * notified instead.
                 */
                ulBodySize = pBmoReq->PredictBodySize((ANSC_HANDLE)pBmoReq);

                if ( pMyObject->SbcPmode == HTTP_SBC_PMODE_RELAY2 )
                {
                    ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                }
                else if ( pMyObject->SbcPmode == HTTP_SBC_PMODE_QUEUE )
                {
                    if ( ulBodySize == 0xFFFFFFFF )
                    {
                        ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                    }
                }
                else
                {
                    ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
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
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pClientSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pClientSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY_PART :

                /*
                 * If we're here for processing this event, the "hReserved" parameter specifies
                 * a buffer descriptor which holds the corresponding body part. In case you are
                 * wondering why we need to consider the scenario where the current cbc_pmode is
                 * pmode_relay2 (i.e., forward everything directly from now on), since the lower
                 * layer object, which is the HTTP Simple Proxy Object in this case, should have
                 * taken care of direct-relaying already. This consideration is valid because
                 * "arrived_body_part" event may be indicated right after the event of "arrived_
                 * header" if the received payload contains both header fields and body part.
                 */
                if ( (pMyObject->SbcPmode == HTTP_SBC_PMODE_RELAY1) ||
                     (pMyObject->SbcPmode == HTTP_SBC_PMODE_RELAY2) ||
                     (pMyObject->SbcPmode == HTTP_SBC_PMODE_STORE ) )
                {
                    if ( pBufferDesp && (AnscBdoGetBlockSize(pBufferDesp) > 0) )
                    {
                        returnStatus =
                            pServerSocket->Send
                                (
                                    (ANSC_HANDLE)pServerSocket,
                                    AnscBdoGetBlock    (pBufferDesp),
                                    AnscBdoGetBlockSize(pBufferDesp),
                                    (ANSC_HANDLE)NULL
                                );
                    }
                }

                if ( (pMyObject->SbcPmode != HTTP_SBC_PMODE_STORE) &&
                     (pMyObject->SbcPmode != HTTP_SBC_PMODE_QUEUE) )
                {
                    if ( pBufferDesp )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }
                }

                /*
                 * There're some body parts already arrived, and now we need allocate more buffers
                 * for more body parts. If we know the size of the rest of the body, we allocate a
                 * descriptor big enough for holding the rest payload; otherwise, we use the pre-
                 * defined message size.
                 */
                ulBodySize = pBmoReq->PredictBodySize((ANSC_HANDLE)pBmoReq);
                ulPartSize = HTTP_SPO_MAX_MESSAGE_SIZE;

                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulPartSize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pClientSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pClientSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY :

                /*
                 * If we're here for processing this event, the "hReserved" parameter specifies
                 * a buffer descriptor which holds the corresponding body part. In case you are
                 * wondering why we need to consider the scenario where the current cbc_pmode is
                 * pmode_relay2 (i.e., forward everything directly from now on), since the lower
                 * layer object, which is the HTTP Simple Proxy Object in this case, should have
                 * taken care of direct-relaying already. This consideration is valid because
                 * "arrived_body" event may be indicated right after the event of "arrived_header"
                 * if the received payload contains both header fields and body part.
                 */
                if ( (pMyObject->SbcPmode == HTTP_SBC_PMODE_RELAY1) ||
                     (pMyObject->SbcPmode == HTTP_SBC_PMODE_RELAY2) ||
                     (pMyObject->SbcPmode == HTTP_SBC_PMODE_STORE ) )
                {
                    if ( pBufferDesp && (AnscBdoGetBlockSize(pBufferDesp) > 0) )
                    {
                        returnStatus =
                            pServerSocket->Send
                                (
                                    (ANSC_HANDLE)pServerSocket,
                                    AnscBdoGetBlock    (pBufferDesp),
                                    AnscBdoGetBlockSize(pBufferDesp),
                                    (ANSC_HANDLE)NULL
                                );
                    }
                }

                if ( (pMyObject->SbcPmode != HTTP_SBC_PMODE_STORE) &&
                     (pMyObject->SbcPmode != HTTP_SBC_PMODE_QUEUE) )
                {
                    if ( pBufferDesp )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }

                    break;
                }
                else
                {
                    returnStatus =
                        pPbcIf->RecvReqBody
                            (
                                pPbcIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq
                            );

                    ulSbcPmode   =
                        pSbcIf->RequestBody
                            (
                                pSbcIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq
                            );

                    pMyObject->SbcPmode = ulSbcPmode;
                }

                if ( ulSbcPmode == HTTP_SBC_PMODE_RELAY1 )
                {
                    returnStatus =
                        pMyObject->SendToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                HTTP_PSO_SFLAG_HEADERS | HTTP_PSO_SFLAG_BODY
                            );
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_RELAY2 )
                {
                    returnStatus =
                        pMyObject->SendToServer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                HTTP_PSO_SFLAG_HEADERS | HTTP_PSO_SFLAG_BODY
                            );
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_STORE )
                {
                    /*
                     * Invalid processing mode if returned by the SBC object, since there's no more
                     * message data to be stored. Don't do anything, treat it like a reset ...
                     */
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_QUEUE )
                {
                    /*
                     * Invalid processing mode if returned by the SBC object, since there's no more
                     * message data to be queued. Don't do anything, treat it like a reset ...
                     */
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_RESET )
                {
                    /*
                     * The SBC object should always return this state as an indication that message
                     * should silently discarded. This state is very useful if the SBC object has
                     * chosen to construct its own response message, which could be sent via the
                     * send_to() member function.
                     */
                }
                else if ( ulSbcPmode == HTTP_SBC_PMODE_CLOSE )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }

                break;

        case    HTTP_TMH_EVENT_COMPLETED :

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the pso_query() function next time.
                 */
                ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                pNewBdo    =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pClientSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pClientSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                returnStatus =
                    pPbcIf->ReqComplete
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                break;

        case    HTTP_TMH_EVENT_OVER_PACKED :

                returnStatus =
                    pPbcIf->ReqComplete
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the pso_query() function next time.
                 */
                pClientSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pClientSocket,
                        AnscBdoGetBlock     (pBufferDesp),
                        AnscBdoGetUsableSize(pBufferDesp),
                        (ANSC_HANDLE)pBufferDesp
                    );

                pClientSocket->RecvPacketSize = AnscBdoGetBlockSize(pBufferDesp);
                returnStatus                  = ANSC_STATUS_DO_IT_AGAIN;

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
        HttpPsoTmhClientSerialize
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
HttpPsoTmhClientSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    )
{
    UNREFERENCED_PARAMETER(hMessage);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;

    if ( hSerializeContext == (ANSC_HANDLE)pClientSocket )
    {
        returnStatus =
            pClientSocket->Send
                (
                    (ANSC_HANDLE)pClientSocket,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }
    else if ( hSerializeContext == (ANSC_HANDLE)pServerSocket )
    {
        returnStatus =
            pServerSocket->Send
                (
                    (ANSC_HANDLE)pServerSocket,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoTmhServerNotify
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
HttpPsoTmhServerNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PHTTP_CBC_INTERFACE             pCbcIf         = (PHTTP_CBC_INTERFACE           )pMyObject->hCbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf         = (PHTTP_PBC_INTERFACE           )pMyObject->hPbcIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT          )hMessage;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hReserved;
    PANSC_BUFFER_DESCRIPTOR         pNewBdo        = NULL;
    ULONG                           ulCbcPmode     = HTTP_CBC_PMODE_RELAY1;
    ULONG                           ulBodySize     = 0;
    ULONG                           ulPartSize     = 0;

    if ( pMyObject->SessionState == HTTP_PSO_STATE_FINISHED )
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

                /*
                 * The CBC object now has a chance to examine all the HTTP header fields and make
                 * a decision on how to proceed with the current HTTP transaction.
                 */
                returnStatus =
                    pPbcIf->RecvRepHeaders
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoRep
                        );

                ulCbcPmode   =
                    pCbcIf->ReplyHeaders
                        (
                            pCbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoRep
                        );

                pMyObject->CbcPmode = ulCbcPmode;

                if ( ulCbcPmode == HTTP_CBC_PMODE_RELAY1 )
                {
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_RELAY);

                    returnStatus =
                        pMyObject->SendToClient
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_RELAY2 )
                {
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_RELAY);

                    returnStatus =
                        pMyObject->SendToClient
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_STORE )
                {
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_STORE);

                    returnStatus =
                        pMyObject->SendToClient
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_PSO_SFLAG_HEADERS
                            );
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_QUEUE )
                {
                    /*
                     * We don't have to do anything at this moment, the final decision will be made
                     * by the CBC object when the complete message body arrives ...
                     */
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_STORE);
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_RESET )
                {
                    /*
                     * We don't have to do anything at this moment, we will drop the whole message
                     * including both headers and body completely when it arrives ...
                     */
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_RELAY);
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_CLOSE )
                {
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_RELAY);

                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_RELAY);

                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }

                break;

        case    HTTP_TMH_EVENT_END_OF_HEADER :

                /*
                 * The appearance of this event indicates that there's message body data following
                 * the header fields; otherwise, EVENT_COMPLETED or EVENT_OVER_PACKED should be
                 * notified instead.
                 */
                ulBodySize = pBmoRep->PredictBodySize((ANSC_HANDLE)pBmoRep);

                if ( pMyObject->CbcPmode == HTTP_CBC_PMODE_RELAY2 )
                {
                    ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                }
                else if ( pMyObject->CbcPmode == HTTP_CBC_PMODE_QUEUE )
                {
                    if ( ulBodySize == 0xFFFFFFFF )
                    {
                        ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                    }
                }
                else
                {
                    ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
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
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pServerSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pServerSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY_PART :

                /*
                 * If we're here for processing this event, the "hReserved" parameter specifies
                 * a buffer descriptor which holds the corresponding body part. In case you are
                 * wondering why we need to consider the scenario where the current cbc_pmode is
                 * pmode_relay2 (i.e., forward everything directly from now on), since the lower
                 * layer object, which is the HTTP Simple Proxy Object in this case, should have
                 * taken care of direct-relaying already. This consideration is valid because
                 * "arrived_body_part" event may be indicated right after the event of "arrived_
                 * header" if the received payload contains both header fields and body part.
                 */
                if ( (pMyObject->CbcPmode == HTTP_CBC_PMODE_RELAY1) ||
                     (pMyObject->CbcPmode == HTTP_CBC_PMODE_RELAY2) ||
                     (pMyObject->CbcPmode == HTTP_CBC_PMODE_STORE ) )
                {
                    if ( pBufferDesp && (AnscBdoGetBlockSize(pBufferDesp) > 0) )
                    {
                        returnStatus =
                            pClientSocket->Send
                                (
                                    (ANSC_HANDLE)pClientSocket,
                                    AnscBdoGetBlock    (pBufferDesp),
                                    AnscBdoGetBlockSize(pBufferDesp),
                                    (ANSC_HANDLE)NULL
                                );
                    }
                }

                if ( (pMyObject->CbcPmode != HTTP_CBC_PMODE_STORE) &&
                     (pMyObject->CbcPmode != HTTP_CBC_PMODE_QUEUE) )
                {
                    if ( pBufferDesp )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }
                }

                /*
                 * There're some body parts already arrived, and now we need allocate more buffers
                 * for more body parts. If we know the size of the rest of the body, we allocate a
                 * descriptor big enough for holding the rest payload; otherwise, we use the pre-
                 * defined message size.
                 */
                ulBodySize = pBmoRep->PredictBodySize((ANSC_HANDLE)pBmoRep);
                ulPartSize = HTTP_SPO_MAX_MESSAGE_SIZE;

                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulPartSize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pServerSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pServerSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY :

                /*
                 * If we're here for processing this event, the "hReserved" parameter specifies
                 * a buffer descriptor which holds the corresponding body part. In case you are
                 * wondering why we need to consider the scenario where the current cbc_pmode is
                 * pmode_relay2 (i.e., forward everything directly from now on), since the lower
                 * layer object, which is the HTTP Simple Proxy Object in this case, should have
                 * taken care of direct-relaying already. This consideration is valid because
                 * "arrived_body" event may be indicated right after the event of "arrived_header"
                 * if the received payload contains both header fields and body part.
                 */
                if ( (pMyObject->CbcPmode == HTTP_CBC_PMODE_RELAY1) ||
                     (pMyObject->CbcPmode == HTTP_CBC_PMODE_RELAY2) ||
                     (pMyObject->CbcPmode == HTTP_CBC_PMODE_STORE ) )
                {
                    if ( pBufferDesp && (AnscBdoGetBlockSize(pBufferDesp) > 0) )
                    {
                        returnStatus =
                            pClientSocket->Send
                                (
                                    (ANSC_HANDLE)pClientSocket,
                                    AnscBdoGetBlock    (pBufferDesp),
                                    AnscBdoGetBlockSize(pBufferDesp),
                                    (ANSC_HANDLE)NULL
                                );
                    }
                }

                if ( (pMyObject->CbcPmode != HTTP_CBC_PMODE_STORE) &&
                     (pMyObject->CbcPmode != HTTP_CBC_PMODE_QUEUE) )
                {
                    if ( pBufferDesp )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }

                    break;
                }
                else
                {
                    returnStatus =
                        pPbcIf->RecvRepBody
                            (
                                pPbcIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep
                            );

                    ulCbcPmode   =
                        pCbcIf->ReplyBody
                            (
                                pCbcIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep
                            );

                    pMyObject->CbcPmode = ulCbcPmode;
                }

                if ( ulCbcPmode == HTTP_CBC_PMODE_RELAY1 )
                {
                    returnStatus =
                        pMyObject->SendToClient
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_PSO_SFLAG_HEADERS | HTTP_PSO_SFLAG_BODY
                            );
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_RELAY2 )
                {
                    returnStatus =
                        pMyObject->SendToClient
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_PSO_SFLAG_HEADERS | HTTP_PSO_SFLAG_BODY
                            );
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_STORE )
                {
                    /*
                     * Invalid processing mode if returned by the CBC object, since there's no more
                     * message data to be stored. Don't do anything, treat it like a reset ...
                     */
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_QUEUE )
                {
                    /*
                     * Invalid processing mode if returned by the CBC object, since there's no more
                     * message data to be queued. Don't do anything, treat it like a reset ...
                     */
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_RESET )
                {
                    /*
                     * The CBC object should always return this state as an indication that message
                     * should silently discarded. This state is very useful if the CBC object has
                     * chosen to construct its own response message, which could be sent via the
                     * send_to() member function.
                     */
                }
                else if ( ulCbcPmode == HTTP_CBC_PMODE_CLOSE )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;
                }

                break;

        case    HTTP_TMH_EVENT_COMPLETED :

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the pso_query() function next time.
                 */
                ulBodySize = HTTP_SPO_MAX_MESSAGE_SIZE;
                pNewBdo    =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
                    returnStatus            = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pServerSocket->SetBufferContext
                        (
                            (ANSC_HANDLE)pServerSocket,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                returnStatus =
                    pPbcIf->RepComplete
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                break;

        case    HTTP_TMH_EVENT_OVER_PACKED :

                returnStatus =
                    pPbcIf->RepComplete
                        (
                            pPbcIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the pso_query() function next time.
                 */
                pServerSocket->SetBufferContext
                    (
                        (ANSC_HANDLE)pServerSocket,
                        AnscBdoGetBlock     (pBufferDesp),
                        AnscBdoGetUsableSize(pBufferDesp),
                        (ANSC_HANDLE)pBufferDesp
                    );

                pServerSocket->RecvPacketSize = AnscBdoGetBlockSize(pBufferDesp);
                returnStatus                  = ANSC_STATUS_DO_IT_AGAIN;

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
        HttpPsoTmhServerSerialize
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
HttpPsoTmhServerSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    )
{
    UNREFERENCED_PARAMETER(hMessage);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;

    if ( hSerializeContext == (ANSC_HANDLE)pClientSocket )
    {
        returnStatus =
            pClientSocket->Send
                (
                    (ANSC_HANDLE)pClientSocket,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }
    else if ( hSerializeContext == (ANSC_HANDLE)pServerSocket )
    {
        returnStatus =
            pServerSocket->Send
                (
                    (ANSC_HANDLE)pServerSocket,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}
