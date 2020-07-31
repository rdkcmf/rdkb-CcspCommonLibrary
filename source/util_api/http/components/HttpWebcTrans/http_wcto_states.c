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

    module:	http_wcto_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Webc Trans Object.

        *   HttpWctoGetBspIf
        *   HttpWctoSetBspIf
        *   HttpWctoGetHfpIf
        *   HttpWctoSetHfpIf
        *   HttpWctoGetWebcSession
        *   HttpWctoSetWebcSession
        *   HttpWctoGetBmoReq
        *   HttpWctoSetBmoReq
        *   HttpWctoGetBmoRep
        *   HttpWctoSetBmoRep
        *   HttpWctoGetTcpSimpleClient
        *   HttpWctoSetTcpSimpleClient
        *   HttpWctoGetTransState
        *   HttpWctoSetTransState
        *   HttpWctoReturn
        *   HttpWctoReset

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

        ANSC_HANDLE
        HttpWctoGetBspIf
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
HttpWctoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hBspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetBspIf
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
HttpWctoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hBspIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWctoGetHfpIf
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
HttpWctoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetHfpIf
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
HttpWctoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWctoGetWebcSession
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
HttpWctoGetWebcSession
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hWebcSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetWebcSession
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWebcSession
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWebcSession
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoSetWebcSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebcSession
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hWebcSession = hWebcSession;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWctoGetBmoReq
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
HttpWctoGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetBmoReq
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
HttpWctoSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hBmoReq = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWctoGetBmoRep
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
HttpWctoGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetBmoRep
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
HttpWctoSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hBmoRep = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWctoGetTcpSimpleClient
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
HttpWctoGetTcpSimpleClient
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->hTcpSimpleClient;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetTcpSimpleClient
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClient
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClient
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoSetTcpSimpleClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hTcpSimpleClient = hClient;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWctoGetTransState
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
HttpWctoGetTransState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->TransState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoSetTransState
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
HttpWctoSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->TransState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoReturn
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
HttpWctoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient = (PHTTP_SIMPLE_CLIENT_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pSimpleClient->ReleaseWcto
            (
                (ANSC_HANDLE)pSimpleClient,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoReset
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
HttpWctoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    pMyObject->hWebcSession     = (ANSC_HANDLE)NULL;
    pMyObject->hBspReqContext   = (ANSC_HANDLE)NULL;
    pMyObject->hTcpSimpleClient = (ANSC_HANDLE)NULL;
    pMyObject->TransState       = HTTP_WCTO_STATE_INITIALIZED;
    pMyObject->bLastTrans       = TRUE;
    pMyObject->AuthRetries      = 0;
    pMyObject->MaxAuthRetries   = HTTP_WCTO_DEFAULT_MAX_AUTH_RETRIES;

    return  ANSC_STATUS_SUCCESS;
}
