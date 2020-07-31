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

    module:	http_wsto_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Webs Trans Object.

        *   HttpWstoGetWspIf
        *   HttpWstoSetWspIf
        *   HttpWstoGetHfpIf
        *   HttpWstoSetHfpIf
        *   HttpWstoGetWebsSession
        *   HttpWstoSetWebsSession
        *   HttpWstoGetBmoReq
        *   HttpWstoSetBmoReq
        *   HttpWstoGetBmoRep
        *   HttpWstoSetBmoRep
        *   HttpWstoGetWebSocket
        *   HttpWstoSetWebSocket
        *   HttpWstoGetClientAddr
        *   HttpWstoGetClientPort
        *   HttpWstoGetTransState
        *   HttpWstoSetTransState
        *   HttpWstoReturn
        *   HttpWstoReset

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

        ANSC_HANDLE
        HttpWstoGetWspIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hWspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetWspIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hWspIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWstoGetHfpIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetHfpIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWstoGetWebsSession
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetWebsSession
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hWebsSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetWebsSession
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWebsSession
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWebsSession
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetWebsSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hWebsSession = hWebsSession;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWstoGetBmoReq
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetBmoReq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hBmoReq = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWstoGetBmoRep
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetBmoRep
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hBmoRep = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWstoGetWebSocket
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpWstoGetWebSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->hWebSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetWebSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetWebSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hWebSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpWstoGetClientAddr
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

PUCHAR
HttpWstoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;

    if ( !pWebSocket )
    {
        return  NULL;
    }

    return  pWebSocket->GetPeerAddress((ANSC_HANDLE)pWebSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpWstoGetClientPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

USHORT
HttpWstoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;

    if ( !pWebSocket )
    {
        return  0;
    }

    return  pWebSocket->GetPeerPort((ANSC_HANDLE)pWebSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWstoGetTransState
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpWstoGetTransState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    return  pMyObject->TransState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoSetTransState
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulState
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulState
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->TransState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoReturn
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return object back to the global
        object pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT   )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pSimpleServer->ReleaseWsto
            (
                (ANSC_HANDLE)pSimpleServer,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

    pMyObject->hWebsSession = (ANSC_HANDLE)NULL;
    pMyObject->hWebSocket   = (ANSC_HANDLE)NULL;
    pMyObject->TransState   = HTTP_WSTO_STATE_INITIALIZED;

    return  ANSC_STATUS_SUCCESS;
}
