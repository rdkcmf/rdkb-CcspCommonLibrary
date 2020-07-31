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

    module:	http_pso_client.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Proxy Session Object.

        *   HttpPsoGetClientBuffer
        *   HttpPsoQueryForClient
        *   HttpPsoRecvFromClient
        *   HttpPsoFinishedByClient
        *   HttpPsoSendToClient
        *   HttpPsoAcceptClient
        *   HttpPsoAuthenticate

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/24/02    initial revision.

**********************************************************************/


#include "http_pso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetClientBuffer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve buffer for receiving data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of buffer descriptor.

**********************************************************************/

ANSC_HANDLE
HttpPsoGetClientBuffer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoQueryForClient
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
HttpPsoQueryForClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = NULL;
    ULONG                           ulPsoQmode     = HTTP_PSO_QMODE_FORWARD;
    ULONG                           ulBmoState1    = HTTP_BMO_STATE_EMPTY;
    ULONG                           ulBmoState2    = HTTP_BMO_STATE_EMPTY;

    /*
     * If the SBC (Server Behavior Controller) has allowed everything to be relayed internally by
     * the proxy object itself (or simply because there's no SBC registered), we SHALL pass payload
     * data from one socket to the other, whenever there's data available.
     */
    if ( (pMyObject->SessionState == HTTP_PSO_STATE_ESTABLISHED) &&
         (pMyObject->SbcPmode     == HTTP_SBC_PMODE_RELAY2     ) )
    {
        return  HTTP_PSO_QMODE_FORWARD;
    }
    else
    {
        pBmoReq = (PHTTP_BMO_REQ_OBJECT)pMyObject->GetCurBmoReq((ANSC_HANDLE)pMyObject);

        if ( !pBmoReq )
        {
            return  HTTP_PSO_QMODE_FORWARD;
        }
    }

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

                ulPsoQmode = HTTP_PSO_QMODE_COLLECT;

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :

                ulPsoQmode = HTTP_PSO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_PART_BODY :

                if ( ulBmoState1 == HTTP_BMO_STATE_PART_BODY )
                {
                    if ( AnscBdoGetLeftSize(pBufferDesp) > HTTP_SPO_RECV_BUFFER_ROOM )
                    {
                        ulPsoQmode = HTTP_PSO_QMODE_COLLECT;
                    }
                    else
                    {
                        ulPsoQmode = HTTP_PSO_QMODE_PROCESS;
                    }
                }
                else
                {
                    ulPsoQmode = HTTP_PSO_QMODE_PROCESS;
                }

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                ulPsoQmode = HTTP_PSO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_UNKNOWN :

                ulPsoQmode = HTTP_PSO_QMODE_PROCESS;

                break;

        default :

                ulPsoQmode = HTTP_PSO_QMODE_COLLECT;

                break;
    }

    return  ulPsoQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoRecvFromClient
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
HttpPsoRecvFromClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )pMyObject->GetCurBmoReq((ANSC_HANDLE)pMyObject);
    
    if ( !pBmoReq )
    {
        pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;

        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

        return  ANSC_STATUS_INTERNAL_ERROR;
    }

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
        pMyObject->SessionState = HTTP_PSO_STATE_FINISHED;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoFinishedByClient
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
HttpPsoFinishedByClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject      = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )pMyObject->GetLastBmoReq((ANSC_HANDLE)pMyObject);
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    ULONG                           ulBmoState     = HTTP_BMO_STATE_EMPTY;

    if ( !pBmoReq )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        ulBmoState = pBmoReq->GetState((ANSC_HANDLE)pBmoReq);

        if ( (ulBmoState == HTTP_BMO_STATE_COMPLETE   ) ||
             (ulBmoState == HTTP_BMO_STATE_OVER_PACKED) )
        {
            if ( pBufferDesp )
            {
                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
            }

            return  ANSC_STATUS_UNAPPLICABLE;
        }
    }

    returnStatus =
        pBmoReq->CloseUp
            (
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBufferDesp
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSendToClient
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
HttpPsoSendToClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT          )hMessage;
    ANSC_HANDLE                     hOrgTmhIf     = pBmoRep->GetTmhIf((ANSC_HANDLE)pBmoRep);

    pBmoRep->SetTmhIf((ANSC_HANDLE)pBmoRep, pMyObject->hTmhClientIf);

    if ( ulSendFlags & HTTP_PSO_SFLAG_HEADERS )
    {
        pBmoRep->OutputHeaders
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pClientSocket
                );
    }

    if ( ulSendFlags & HTTP_PSO_SFLAG_BODY )
    {
        pBmoRep->OutputBody
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pClientSocket
                );
    }

    pBmoRep->SetTmhIf((ANSC_HANDLE)pBmoRep, hOrgTmhIf);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpPsoAcceptClient
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to accept the client connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpPsoAcceptClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf        = (PHTTP_WAM_INTERFACE           )pMyObject->hWamIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    BOOL                            bAccepted     = TRUE;
    HTTP_WAM_CLIENT_INFO            wamClientInfo;

    wamClientInfo.Address.Value = pClientSocket->PeerAddress.Value;
    wamClientInfo.Port          = pClientSocket->PeerPort;

    bAccepted =
        pWamIf->Accept
            (
                pWamIf->hOwnerContext,
                (ANSC_HANDLE)&wamClientInfo
            );

    return  bAccepted;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoAuthenticate
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to authenticate the session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoAuthenticate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}
