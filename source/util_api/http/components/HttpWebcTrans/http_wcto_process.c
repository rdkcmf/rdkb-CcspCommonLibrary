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

    module:	http_wcto_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Webc Trans Object.

        *   HttpWctoQuery
        *   HttpWctoRecv
        *   HttpWctoSend
        *   HttpWctoFinish
        *   HttpWctoAbort

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

        ULONG
        HttpWctoQuery
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
HttpWctoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR   )hBufferContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT      )pMyObject->hBmoRep;
    ULONG                           ulWctoQmode   = HTTP_WCTO_QMODE_COLLECT;
    ULONG                           ulBmoState1   = HTTP_BMO_STATE_EMPTY;
    ULONG                           ulBmoState2   = HTTP_BMO_STATE_EMPTY;

    /*
     * The decision on how to process the received payload should be made based on two message
     * states: the message state before payload arrival and the state after. The caller must ensure
     * the consistency of the buffer descriptor.
     */
    buffer      = AnscBdoGetBlock    (pBufferDesp);
    ulSize      = AnscBdoGetBlockSize(pBufferDesp);
    ulBmoState1 = pBmoRep->GetState((ANSC_HANDLE)pBmoRep);
    ulBmoState2 =
        pBmoRep->Examine
            (
                (ANSC_HANDLE)pBmoRep,
                buffer,
                ulSize
            );

    switch ( ulBmoState2 )
    {
        case    HTTP_BMO_STATE_EMPTY :
        case    HTTP_BMO_STATE_PART_HEADER :

                ulWctoQmode = HTTP_WCTO_QMODE_COLLECT;

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :

                ulWctoQmode = HTTP_WCTO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_PART_BODY :

                if ( ulBmoState1 == HTTP_BMO_STATE_PART_BODY )
                {
                    if ( AnscBdoGetLeftSize(pBufferDesp) > HTTP_WCSO_RECV_BUFFER_ROOM )
                    {
                        ulWctoQmode = HTTP_WCTO_QMODE_COLLECT;
                    }
                    else
                    {
                        ulWctoQmode = HTTP_WCTO_QMODE_PROCESS;
                    }
                }
                else
                {
                    ulWctoQmode = HTTP_WCTO_QMODE_PROCESS;
                }

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                ulWctoQmode = HTTP_WCTO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_UNKNOWN :

                ulWctoQmode = HTTP_WCTO_QMODE_PROCESS;

                break;

        default :

                ulWctoQmode = HTTP_WCTO_QMODE_COLLECT;

                break;
    }

    return  ulWctoQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoRecv
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
HttpWctoRecv
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
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR   )hBufferContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT      )pMyObject->hBmoRep;
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
        pBmoRep->Process
            (
                (ANSC_HANDLE)pBmoRep,
                (ANSC_HANDLE)pBufferDesp
            );

    if ( (returnStatus != ANSC_STATUS_SUCCESS    ) &&
         (returnStatus != ANSC_STATUS_DO_IT_AGAIN) )
    {
        pMyObject->TransState = HTTP_WCTO_STATE_FINISHED;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSend
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage,
                ULONG                       ulSendFlags
            );

    description:

        This function is called to send a message to server.

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
HttpWctoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject        = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_BMO_REQ_OBJECT            pBmoReq          = (PHTTP_BMO_REQ_OBJECT          )hMessage;
    ANSC_HANDLE                     hOrgTmhIf        = pBmoReq->GetTmhIf((ANSC_HANDLE)pBmoReq);

    pBmoReq->SetTmhIf((ANSC_HANDLE)pBmoReq, pMyObject->hTmhIf);

    if ( ulSendFlags & HTTP_WCTO_SFLAG_HEADERS )
    {
        pBmoReq->OutputHeaders
                (
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pTcpSimpleClient
                );
    }

    if ( ulSendFlags & HTTP_WCTO_SFLAG_BODY )
    {
        pBmoReq->OutputBody
                (
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pTcpSimpleClient
                );
    }

    pBmoReq->SetTmhIf((ANSC_HANDLE)pBmoReq, hOrgTmhIf);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoFinish
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
HttpWctoFinish
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
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT      )pMyObject->hBmoRep;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR   )hBufferContext;
    ULONG                           ulBmoState    = pBmoRep->GetState((ANSC_HANDLE)pBmoRep);

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
        pBmoRep->CloseUp
            (
                (ANSC_HANDLE)pBmoRep,
                (ANSC_HANDLE)pBufferDesp
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoAbort
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent
            );

    description:

        This function is called when the client connection is being
        closed due to network failure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the internal event to be reported.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoAbort
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf        = (PHTTP_BSP_INTERFACE       )pMyObject->hBspIf;
    
    returnStatus =
        pBspIf->Notify
            (
                pBspIf->hOwnerContext,
                ulEvent,
                0,
                pMyObject->hBspReqContext,
                pMyObject->hBmoReq,
                pMyObject->hBmoRep
            );

    return  returnStatus;
}
