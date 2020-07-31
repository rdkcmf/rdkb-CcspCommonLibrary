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

    module:	http_wsso_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Webs Session Object.

        *   HttpWssoQuery
        *   HttpWssoRecv
        *   HttpWssoFinish
        *   HttpWssoAccept

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

        ULONG
        HttpWssoQuery
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
HttpWssoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = (PHTTP_WEBS_TRANS_OBJECT       )NULL;
    ULONG                           ulWssoQmode   = HTTP_WSSO_QMODE_PROCESS;
    ULONG                           ulWstoQmode   = HTTP_WSTO_QMODE_PROCESS;

    pWebsTrans = (PHTTP_WEBS_TRANS_OBJECT)pMyObject->GetCurWsto((ANSC_HANDLE)pMyObject);

    if ( !pWebsTrans )
    {
        return  HTTP_WSSO_QMODE_PROCESS;
    }

    ulWstoQmode =
        pWebsTrans->Query
            (
                (ANSC_HANDLE)pWebsTrans,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulWstoQmode )
    {
        case    HTTP_WSTO_QMODE_COLLECT :

                ulWssoQmode = HTTP_WSSO_QMODE_COLLECT;

                break;

        case    HTTP_WSTO_QMODE_PROCESS :

                ulWssoQmode = HTTP_WSSO_QMODE_PROCESS;

                break;

        default :

                ulWssoQmode = HTTP_WSSO_QMODE_COLLECT;

                break;
    }

    pWebsTrans->ReleaseAccess((ANSC_HANDLE)pWebsTrans);

    return  ulWssoQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoRecv
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
HttpWssoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = (PHTTP_WEBS_TRANS_OBJECT       )pMyObject->GetCurWsto((ANSC_HANDLE)pMyObject);

    if ( !pWebsTrans )
    {
        pMyObject->SessionState = HTTP_WSSO_STATE_FINISHED;

        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    buffer       = AnscBdoGetBlock    (pBufferDesp);
    ulSize       = AnscBdoGetBlockSize(pBufferDesp);
    returnStatus =
        pWebsTrans->Recv
            (
                (ANSC_HANDLE)pWebsTrans,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    if ( (returnStatus != ANSC_STATUS_SUCCESS    ) &&
         (returnStatus != ANSC_STATUS_DO_IT_AGAIN) )
    {
        pMyObject->SessionState = HTTP_WSSO_STATE_FINISHED;
    }

    pWebsTrans->ReleaseAccess((ANSC_HANDLE)pWebsTrans);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoFinish
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
HttpWssoFinish
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans    = (PHTTP_WEBS_TRANS_OBJECT       )pMyObject->GetEndWsto((ANSC_HANDLE)pMyObject);

    if ( !pWebsTrans )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else if ( pWebsTrans->GetTransState((ANSC_HANDLE)pWebsTrans) == HTTP_WSTO_STATE_FINISHED )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        pWebsTrans->ReleaseAccess((ANSC_HANDLE)pWebsTrans);

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    buffer       = AnscBdoGetBlock    (pBufferDesp);
    ulSize       = AnscBdoGetBlockSize(pBufferDesp);
    returnStatus =
        pWebsTrans->Finish
            (
                (ANSC_HANDLE)pWebsTrans,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    pWebsTrans->ReleaseAccess((ANSC_HANDLE)pWebsTrans);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpWssoAccept
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
HttpWssoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PHTTP_WSP_INTERFACE             pWspIf        = (PHTTP_WSP_INTERFACE           )pMyObject->hWspIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;
    BOOL                            bAccepted     = TRUE;
    HTTP_WSP_CLIENT_INFO            wspClientInfo;

    wspClientInfo.Address.Value = pWebSocket->PeerAddress.Value;
    wspClientInfo.Port          = pWebSocket->PeerPort;

    bAccepted =
        pWspIf->Accept
            (
                pWspIf->hOwnerContext,
                (ANSC_HANDLE)&wspClientInfo
            );

    return  bAccepted;
}
