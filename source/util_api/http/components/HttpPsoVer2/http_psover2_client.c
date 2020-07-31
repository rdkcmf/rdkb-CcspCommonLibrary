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

    module:	http_psover2_client.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Pso Ver2 Object.

        *   HttpPsoVer2QueryForClient
        *   HttpPsoVer2RecvFromClient
        *   HttpPsoVer2FinishedByClient
        *   HttpPsoVer2AcceptClient
        *   HttpPsoVer2Authenticate

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_psover2_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoVer2QueryForClient
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
HttpPsoVer2QueryForClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT         )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = (PHTTP_TRANS_RECORD_OBJECT     )NULL;
    ULONG                           ulPsoQmode     = HTTP_PSOVER2_QMODE_PROCESS;
    ULONG                           ulTroQmode     = HTTP_TRO_QMODE_FORWARD;

    pTransRecord = (PHTTP_TRANS_RECORD_OBJECT)pMyObject->GetCurTro((ANSC_HANDLE)pMyObject);

    if ( !pTransRecord )
    {
        return  HTTP_PSOVER2_QMODE_PROCESS;
    }

    ulTroQmode =
        pTransRecord->QueryForClient
            (
                (ANSC_HANDLE)pTransRecord,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    switch ( ulTroQmode )
    {
        case    HTTP_TRO_QMODE_COLLECT :

                ulPsoQmode = HTTP_PSOVER2_QMODE_COLLECT;

                break;

        case    HTTP_TRO_QMODE_FORWARD :
        case    HTTP_TRO_QMODE_PROCESS :

                ulPsoQmode = HTTP_PSOVER2_QMODE_PROCESS;

                break;

        default :

                ulPsoQmode = HTTP_PSOVER2_QMODE_COLLECT;

                break;
    }

    pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

    return  ulPsoQmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2RecvFromClient
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
HttpPsoVer2RecvFromClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT         )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = (PHTTP_TRANS_RECORD_OBJECT     )pMyObject->GetCurTro((ANSC_HANDLE)pMyObject);

    if ( !pTransRecord )
    {
        pMyObject->SessionState = HTTP_PSOVER2_STATE_FINISHED;

        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    buffer       = AnscBdoGetBlock    (pBufferDesp);
    ulSize       = AnscBdoGetBlockSize(pBufferDesp);
    returnStatus =
        pTransRecord->RecvFromClient
            (
                (ANSC_HANDLE)pTransRecord,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    if ( (returnStatus != ANSC_STATUS_SUCCESS    ) &&
         (returnStatus != ANSC_STATUS_DO_IT_AGAIN) )
    {
        pMyObject->SessionState = HTTP_PSOVER2_STATE_FINISHED;
    }

    pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2FinishedByClient
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
HttpPsoVer2FinishedByClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT         )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp    = (PANSC_BUFFER_DESCRIPTOR       )hBufferContext;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = (PHTTP_TRANS_RECORD_OBJECT     )pMyObject->GetLastTro((ANSC_HANDLE)pMyObject);

    if ( !pTransRecord )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else if ( pTransRecord->GetTransState((ANSC_HANDLE)pTransRecord) == HTTP_TRO_STATE_FINISHED )
    {
        if ( pBufferDesp )
        {
            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
        }

        pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    buffer       = AnscBdoGetBlock    (pBufferDesp);
    ulSize       = AnscBdoGetBlockSize(pBufferDesp);
    returnStatus =
        pTransRecord->FinishedByClient
            (
                (ANSC_HANDLE)pTransRecord,
                buffer,
                ulSize,
                (ANSC_HANDLE)pBufferDesp
            );

    pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpPsoVer2AcceptClient
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
HttpPsoVer2AcceptClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT         )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf         = (PHTTP_WAM_INTERFACE           )pMyObject->hWamIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    BOOL                            bAccepted      = TRUE;
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
        HttpPsoVer2Authenticate
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
HttpPsoVer2Authenticate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}
