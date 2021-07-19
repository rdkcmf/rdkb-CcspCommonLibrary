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

    module:	http_tro_server.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Trans Record Object.

        *   HttpTroQueryForServer
        *   HttpTroRecvFromServer
        *   HttpTroFinishedByServer
        *   HttpTroSendToServer
        *   HttpTroConnectToServer
        *   HttpTroSetUpConnection

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/05/02    initial revision.

**********************************************************************/


#include "http_tro_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpTroQueryForServer
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
HttpTroQueryForServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;
    ULONG                           ulTroQmode     = HTTP_TRO_QMODE_FORWARD;
    ULONG                           ulBmoState1    = HTTP_BMO_STATE_EMPTY;
    ULONG                           ulBmoState2    = HTTP_BMO_STATE_EMPTY;

    /*
     * Once we receive the first byte from the server, we consider that the transaction record has
     * been established. The importance of this step is to support the request pipeline that may be
     * engaged by the client.
     */
    if ( pMyObject->TransState != HTTP_TRO_STATE_ESTABLISHED )
    {
        pMyObject->TransState = HTTP_TRO_STATE_ESTABLISHED;
    }

    /*
     * If the CBC (Client Behavior Controller) has allowed everything to be relayed internally by
     * the proxy object itself (or simply because there's no CBC registered), we SHALL pass payload
     * data from one socket to the other, whenever there's data available.
     */
    if ( (pMyObject->TransState == HTTP_TRO_STATE_ESTABLISHED) &&
         (pMyObject->CbcPmode   == HTTP_CBC_PMODE_RELAY2     ) )
    {
        return  HTTP_TRO_QMODE_FORWARD;
    }

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

                ulTroQmode = HTTP_TRO_QMODE_COLLECT;

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :

                ulTroQmode = HTTP_TRO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_PART_BODY :

                if ( ulBmoState1 == HTTP_BMO_STATE_PART_BODY )
                {
                    if ( AnscBdoGetLeftSize(pBufferDesp) > HTTP_SPO_RECV_BUFFER_ROOM )
                    {
                        ulTroQmode = HTTP_TRO_QMODE_COLLECT;
                    }
                    else
                    {
                        ulTroQmode = HTTP_TRO_QMODE_PROCESS;
                    }
                }
                else
                {
                    ulTroQmode = HTTP_TRO_QMODE_PROCESS;
                }

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                ulTroQmode = HTTP_TRO_QMODE_PROCESS;

                break;

        case    HTTP_BMO_STATE_UNKNOWN :

                ulTroQmode = HTTP_TRO_QMODE_PROCESS;

                break;

        default :

                ulTroQmode = HTTP_TRO_QMODE_COLLECT;

                break;
    }

    return  ulTroQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroRecvFromServer
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
HttpTroRecvFromServer
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
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;
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
        pMyObject->TransState = HTTP_TRO_STATE_FINISHED;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroFinishedByServer
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hBufferContext
            );

    description:

        This function is called when the server connection is being
        closed which indicates that no more data will be sent to the
        client from the server.

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
HttpTroFinishedByServer
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
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;
    ULONG                           ulBmoState     = pBmoRep->GetState((ANSC_HANDLE)pBmoRep);

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
        HttpTroSendToServer
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
HttpTroSendToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )hMessage;
    ANSC_HANDLE                     hOrgTmhIf      = pBmoReq->GetTmhIf((ANSC_HANDLE)pBmoReq);

    if ( !pServerSocket )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    pBmoReq->SetTmhIf((ANSC_HANDLE)pBmoReq, pMyObject->hTmhClientIf);

    if ( ulSendFlags & HTTP_TRO_SFLAG_HEADERS )
    {
        pBmoReq->OutputHeaders
                (
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pServerSocket
                );
    }

    if ( ulSendFlags & HTTP_TRO_SFLAG_BODY )
    {
        pBmoReq->OutputBody
                (
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pServerSocket
                );
    }

    pBmoReq->SetTmhIf((ANSC_HANDLE)pBmoReq, hOrgTmhIf);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroConnectToServer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to connect to the server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroConnectToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf         = (PHTTP_WAM_INTERFACE           )pMyObject->hWamIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )hReserved;
    ULONG                           ulWamPmode     = HTTP_WAM_PMODE_PASS;
    HTTP_WAM_SERVER_INFO            wamServerInfo;
    errno_t                         rc             = -1;

    AnscZeroMemory(pMyObject->ServerName,  ANSC_DOMAIN_NAME_SIZE                       );
    rc = strcpy_s(pMyObject->ServerName, sizeof(pMyObject->ServerName), pBmoReq->GetServerName((ANSC_HANDLE)pBmoReq));
    ERR_CHK(rc);

    AnscZeroMemory(wamServerInfo.HostName, ANSC_DOMAIN_NAME_SIZE                       );
    rc = strcpy_s(wamServerInfo.HostName, sizeof(wamServerInfo.HostName), pBmoReq->GetServerName((ANSC_HANDLE)pBmoReq));
    ERR_CHK(rc);

    pMyObject->ServerAddr.Value = 0;
    pMyObject->ServerPort       = pBmoReq->GetServerPort((ANSC_HANDLE)pBmoReq);

    wamServerInfo.Port = pBmoReq->GetServerPort((ANSC_HANDLE)pBmoReq);
    ulWamPmode         =
        pWamIf->Connect
            (
                pWamIf->hOwnerContext,
                (ANSC_HANDLE)&wamServerInfo
            );

    switch ( ulWamPmode )
    {
        case    HTTP_WAM_PMODE_PASS :

                break;

        case    HTTP_WAM_PMODE_PEND :

                pMyObject->TransState = HTTP_TRO_STATE_PENDING;

                return  ANSC_STATUS_SUCCESS;

                break;

        case    HTTP_WAM_PMODE_DENY :

                pMyObject->TransState = HTTP_TRO_STATE_FINISHED;

                return  ANSC_STATUS_SUCCESS;

                break;

        default :

                return  ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    returnStatus =
        pMyObject->SetUpConnection
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pMyObject->TransState = HTTP_TRO_STATE_SERVER_CONNECTED;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetUpConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to set up a connection to server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetUpConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT   )pMyObject->hOwnerContext;
    PANSC_SIMPLE_PROXY_TCP_OBJECT   pTcpSpo        = (PANSC_SIMPLE_PROXY_TCP_OBJECT )pAdvancedProxy->hSimpleProxy;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->hServerSocket;

    AnscResolveHostName(pMyObject->ServerName, pMyObject->ServerAddr.Dot);

    if ( pMyObject->ServerAddr.Value == 0 )
    {
        pMyObject->TransState = HTTP_TRO_STATE_FINISHED;

        return  ANSC_STATUS_FAILURE;
    }

    pServerSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pTcpSpo->PubAddSocket
            (
                (ANSC_HANDLE)pTcpSpo,
                pMyObject->ServerAddr.Dot,
                pMyObject->ServerPort,
                0,
                pClientSocket->hClientContext
            );

    if ( !pServerSocket )
    {
        pMyObject->TransState = HTTP_TRO_STATE_FINISHED;

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hServerSocket = (ANSC_HANDLE)pServerSocket;
    }

    return  ANSC_STATUS_SUCCESS;
}
