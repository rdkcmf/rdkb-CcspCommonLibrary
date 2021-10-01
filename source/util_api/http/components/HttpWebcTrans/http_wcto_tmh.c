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

    module:	http_wcto_tmh.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Webc Trans Object.

        *   HttpWctoTmhNotify
        *   HttpWctoTmhSerialize

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


#include "http_wcto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoTmhNotify
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
HttpWctoTmhNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject        = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient    = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_BSP_INTERFACE             pBspIf           = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession     = (PHTTP_WEBC_SESSION_OBJECT     )pMyObject->hWebcSession;
    PHTTP_CAS_INTERFACE             pCasIf           = (PHTTP_CAS_INTERFACE           )pSimpleClient->GetCasIf((ANSC_HANDLE)pSimpleClient);
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT          )hMessage;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = (PANSC_BUFFER_DESCRIPTOR       )hReserved;
    PANSC_BUFFER_DESCRIPTOR         pNewBdo          = NULL;
    ULONG                           ulBodySize       = 0;
    ULONG                           ulPartSize       = 0;

    if ( pMyObject->TransState == HTTP_WCTO_STATE_FINISHED )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pMyObject->TransState == HTTP_WCTO_STATE_REQ_SENT )
    {
        pMyObject->TransState = HTTP_WCTO_STATE_REP_RECEIVING;
    }

#ifdef   _DEBUG
    if ( !pBufferDesp )
    {
    AnscTrace
        (
            "HttpWctoTmhNotify - ulEvent = %lu, pBufferDesp = NULL.\n", 
            ulEvent
        );
    }
    else
    {
    AnscTrace
        (
            "HttpWctoTmhNotify - ulEvent = %lu, buf = 0x%8p, size = %lu, offset = %lu, block size = %lu, RefCount = %lu\n", 
            ulEvent, 
            pBufferDesp->Buffer,
            pBufferDesp->BufferSize,
            pBufferDesp->Offset,
            pBufferDesp->BlockSize,
            pBufferDesp->RefCount
        );
    }
#endif

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

                pBmoRep->SetMboMode((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_STORE);

                break;

        case    HTTP_TMH_EVENT_END_OF_HEADER :

                /*
                 * The appearance of this event indicates that there's message body data following
                 * the header fields; otherwise, EVENT_COMPLETED or EVENT_OVER_PACKED should be
                 * notified instead.
                 */
                ulBodySize = pBmoRep->PredictBodySize((ANSC_HANDLE)pBmoRep);

                if ( ulBodySize == 0xFFFFFFFF )
                {
                    ulBodySize = HTTP_WCSO_MAX_MESSAGE_SIZE;
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
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pTcpSimpleClient->SetBufferContext
                        (
                            (ANSC_HANDLE)pTcpSimpleClient,
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
                ulBodySize = pBmoRep->PredictBodySize((ANSC_HANDLE)pBmoRep);
                /*when allocate Bdo, will add addtional 16 bytes and cause Bdo->Buffer's real size equal 8196 (2 pages).*/
                ulPartSize = HTTP_WCSO_MAX_MESSAGE_SIZE - 16;

#ifdef HTTPC_BDO_USE_MMAP
                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdoWithMmap
                        (
                            ulPartSize,
                            0,
                            0
                        );
#else                
                pNewBdo =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulPartSize,
                            0,
                            0
                        );
#endif

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pTcpSimpleClient->SetBufferContext
                        (
                            (ANSC_HANDLE)pTcpSimpleClient,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                break;

        case    HTTP_TMH_EVENT_ARRIVED_BODY :

                break;

        case    HTTP_TMH_EVENT_COMPLETED :

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the wcto_query() function next time.
                 */
                pMyObject->TransState = HTTP_WCTO_STATE_ESTABLISHED;

                ulBodySize = HTTP_WCSO_MAX_MESSAGE_SIZE;
                pNewBdo    =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pTcpSimpleClient->SetBufferContext
                        (
                            (ANSC_HANDLE)pTcpSimpleClient,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

                if ( pMyObject->AuthRetries < pMyObject->MaxAuthRetries )
                {
                    returnStatus = 
                        pCasIf->ProcessResponse
                            (
                                pCasIf->hOwnerContext,
                                pMyObject->hBmoReq,
                                (ANSC_HANDLE)pBmoRep,
                                (ANSC_HANDLE)pMyObject
                            );
                }
                else
                {
                    returnStatus    = ANSC_STATUS_SUCCESS;
                }

                if ( returnStatus == ANSC_STATUS_DO_IT_AGAIN )
                {
                    pMyObject->TransState   = HTTP_WCTO_STATE_REQ_SENT;
                    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);
                    
                    pMyObject->AuthRetries ++;

                    returnStatus            = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pMyObject->AuthRetries  = 0;

                    returnStatus =
                        pBspIf->Browse
                            (
                                pBspIf->hOwnerContext,
                                pMyObject->hBmoReq,
                                (ANSC_HANDLE)pBmoRep,
                                pMyObject->hBspReqContext
                            );

                    pMyObject->bLastTrans = pMyObject->DoLastTrans((ANSC_HANDLE)pMyObject);
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;

                    if ( returnStatus == ANSC_STATUS_DO_IT_AGAIN )
                    {
                        pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

                        returnStatus = ANSC_STATUS_SUCCESS;
                    }
                    else if ( returnStatus == ANSC_STATUS_NO_MORE_DATA )
                    {
                        pMyObject->hBspReqContext = NULL;
                        pWebcSession->hBspReqContext = NULL;
                        returnStatus = ANSC_STATUS_SUCCESS;
                    }
                }

                break;

        case    HTTP_TMH_EVENT_OVER_PACKED :

                pMyObject->TransState = HTTP_WCTO_STATE_ESTABLISHED;

                if ( pMyObject->AuthRetries < pMyObject->MaxAuthRetries )
                {
                    returnStatus = 
                        pCasIf->ProcessResponse
                            (
                                pCasIf->hOwnerContext,
                                pMyObject->hBmoReq,
                                (ANSC_HANDLE)pBmoRep,
                                (ANSC_HANDLE)pMyObject
                            );
                }
                else
                {
                    returnStatus = ANSC_STATUS_SUCCESS;
                }

                if ( returnStatus == ANSC_STATUS_DO_IT_AGAIN )
                {
                    pMyObject->TransState   = HTTP_WCTO_STATE_REQ_SENT;
                    pBmoRep->Reset((ANSC_HANDLE)pBmoRep);

                    pMyObject->AuthRetries ++;

                    returnStatus = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    pMyObject->AuthRetries  = 0;

                    returnStatus =
                        pBspIf->Browse
                            (
                                pBspIf->hOwnerContext,
                                pMyObject->hBmoReq,
                                (ANSC_HANDLE)pBmoRep,
                                pMyObject->hBspReqContext
                            );

                    pMyObject->bLastTrans = pMyObject->DoLastTrans((ANSC_HANDLE)pMyObject);
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;

                    if ( returnStatus == ANSC_STATUS_DO_IT_AGAIN )
                    {
                        pBmoRep->Reset((ANSC_HANDLE)pBmoRep);
                        
                        returnStatus = ANSC_STATUS_SUCCESS;
                    }
                    else if ( returnStatus == ANSC_STATUS_NO_MORE_DATA )
                    {
                        pMyObject->hBspReqContext = NULL;
                        pWebcSession->hBspReqContext = NULL;
                        returnStatus = ANSC_STATUS_SUCCESS;
                    }
                }

                /*
                 * Before indicating this event, the corresponding message object MUST have already
                 * changed its internal state to either "state_complete" or "state_over_packed" so
                 * a new message object will be created by the wcto_query() function next time.
                 */
                /*CID:63286 Dereference after null check*/
                if (pBufferDesp) 
                {
                    pTcpSimpleClient->SetBufferContext
                    (
                        (ANSC_HANDLE)pTcpSimpleClient,
                        AnscBdoGetBlock     (pBufferDesp),
                        AnscBdoGetUsableSize(pBufferDesp),
                        (ANSC_HANDLE)pBufferDesp
                    );

                    pTcpSimpleClient->RecvPacketSize = AnscBdoGetBlockSize(pBufferDesp);
                    returnStatus                     = ANSC_STATUS_DO_IT_AGAIN;

                    AnscBdoSetBlockSize(pBufferDesp, 0);
                }

                break;

        case    HTTP_TMH_EVENT_GARBAGE_DATA_DROPPED :

                ulBodySize = HTTP_WCSO_MAX_MESSAGE_SIZE;
                pNewBdo    =
                    (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                        (
                            ulBodySize,
                            0,
                            0
                        );

                if ( !pNewBdo )
                {
                    pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;
                    returnStatus          = ANSC_STATUS_SUCCESS;

                    break;
                }
                else
                {
                    pTcpSimpleClient->SetBufferContext
                        (
                            (ANSC_HANDLE)pTcpSimpleClient,
                            AnscBdoGetEndOfBlock(pNewBdo),
                            AnscBdoGetLeftSize  (pNewBdo),
                            (ANSC_HANDLE)pNewBdo
                        );
                }

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
        HttpWctoTmhSerialize
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
HttpWctoTmhSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    )
{
    UNREFERENCED_PARAMETER(hMessage);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject        = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;

    if ( hSerializeContext == (ANSC_HANDLE)pTcpSimpleClient )
    {
        returnStatus =
            pTcpSimpleClient->Send
                (
                    (ANSC_HANDLE)pTcpSimpleClient,
                    buffer,
                    ulSize,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}
