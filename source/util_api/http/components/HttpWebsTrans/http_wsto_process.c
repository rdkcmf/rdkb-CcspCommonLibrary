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

    module:	http_wsto_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Webs Trans Object.

        *   HttpWstoQuery
        *   HttpWstoRecv
        *   HttpWstoSend
        *   HttpWstoFinish

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWstoQuery
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hBufferContext
            );

    description:

        This function is called to query the processing mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hBufferContext
                Specifies the context handle associated with the buffer.

    return:     process mode.

**********************************************************************/

ULONG
HttpWstoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    ULONG                           ulWstoQmode   = HTTP_WSTO_QMODE_COLLECT;
    ULONG                           ulBmoState1   = HTTP_BMO_STATE_EMPTY;
    ULONG                           ulBmoState2   = HTTP_BMO_STATE_EMPTY;

    /*
     * The decision on how to process the received payload should be made based on two message
     * states: the message state before payload arrival and the state after. The caller must ensure
     * the consistency of the buffer descriptor.
     */
    buffer      = AnscBdoGetBlock    (pBufferDesp);
    ulSize      = AnscBdoGetBlockSize(pBufferDesp);
    ulBmoState1 = pBmoReq->GetState((ANSC_HANDLE)pBmoReq);
    ulBmoState2 =
        pBmoReq->Examine
            (
                (ANSC_HANDLE)pBmoReq,
                buffer,
                ulSize
            );

    switch ( ulBmoState2 )
    {
        case    HTTP_BMO_STATE_EMPTY :
        case    HTTP_BMO_STATE_PART_HEADER :

                ulWstoQmode = HTTP_WSTO_QMODE_COLLECT;

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :

                ulWstoQmode = HTTP_WSTO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_PART_BODY :

                if ( ulBmoState1 == HTTP_BMO_STATE_PART_BODY )
                {
                    if ( AnscBdoGetLeftSize(pBufferDesp) >= HTTP_SSO_RECV_BUFFER_ROOM )
                    {
                        ulWstoQmode = HTTP_WSTO_QMODE_COLLECT;
                    }
                    else
                    {
                        ulWstoQmode = HTTP_WSTO_QMODE_PROCESS;
                    }
                }
                else
                {
                    ulWstoQmode = HTTP_WSTO_QMODE_PROCESS;
                }

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                ulWstoQmode = HTTP_WSTO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_UNKNOWN :

                ulWstoQmode = HTTP_WSTO_QMODE_PROCESS;

                break;

        default :

                ulWstoQmode = HTTP_WSTO_QMODE_COLLECT;

                break;
    }

    return  ulWstoQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoRecv
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hBufferContext
            );

    description:

        This function is called to process received payloads.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hBufferContext
                Specifies the context handle associated with the buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT    )pMyObject->hOwnerContext;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    pBmoReq->SetFumIf((ANSC_HANDLE)pBmoReq, pSimpleServer->GetFumIf((ANSC_HANDLE)pSimpleServer));

    pBmoReq->SetWebSessionId((ANSC_HANDLE)pBmoReq, (ULONG)pMyObject->hWebsSession);

    /*
     * At time like this, you will always have two options on how to proceed with the message
     * processing:
     *
     *      $ Examine the current object states and the content of the message payload
     *        to make the processing decisions in this object.
     *
     *      $ Offload the examination and certain state-transition functions to another
     *        object and provide an interface to be notified when something happens.
     *
     * Guess which one we're using here ...
     */
    returnStatus =
        pBmoReq->Process
            (
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBufferDesp
            );

    if ( (returnStatus != ANSC_STATUS_SUCCESS    ) &&
         (returnStatus != ANSC_STATUS_DO_IT_AGAIN) )
    {
        pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
    }

    if ( (pMyObject->TransState == HTTP_WSTO_STATE_ESTABLISHED) ||
         (pMyObject->TransState == HTTP_WSTO_STATE_FINISHED   ) )
    {
        /*
         * To avoid unnessary memory consumption, we release the resources allocated for this
         * transaction right away...
         */
        pMyObject->Close((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSend
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage,
                ULONG                       ulSendFlags
            );

    description:

        This function is called to send a message to client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the message to be sent.

                ULONG                       ulSendFlags
                Specifies the operational flags for sending.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT          )hMessage;
    ANSC_HANDLE                     hOrgTmhIf     = pBmoRep->GetTmhIf((ANSC_HANDLE)pBmoRep);

    pBmoRep->SetTmhIf((ANSC_HANDLE)pBmoRep, pMyObject->hTmhIf);

    if ( ulSendFlags & HTTP_WSTO_SFLAG_HEADERS )
    {
        pBmoRep->OutputHeaders
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pWebSocket
                );
    }

    if ( ulSendFlags & HTTP_WSTO_SFLAG_BODY )
    {
        pBmoRep->OutputBody
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pWebSocket
                );
    }

    pBmoRep->SetTmhIf((ANSC_HANDLE)pBmoRep, hOrgTmhIf);

    if ( pWebSocket->bBroken )
    {
        pMyObject->TransState = HTTP_WSTO_STATE_FINISHED;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoFinish
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hBufferContext
            );

    description:

        This function is called when the client connection is being
        closed which indicates that no more data will be sent to the
        server from the client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hBufferContext
                Specifies the context handle associated with the buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoFinish
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    ULONG                           ulBmoState    = pBmoReq->GetState((ANSC_HANDLE)pBmoReq);

    if ( (ulBmoState == HTTP_BMO_STATE_COMPLETE   ) ||
         (ulBmoState == HTTP_BMO_STATE_OVER_PACKED) )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pBmoReq->CloseUp
            (
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBufferDesp
            );

    return  returnStatus;
}
