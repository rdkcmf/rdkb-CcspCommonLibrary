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

    module:	http_wsso_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Webs Session Object.

        *   HttpWssoGetWspIf
        *   HttpWssoSetWspIf
        *   HttpWssoGetHfpIf
        *   HttpWssoSetHfpIf
        *   HttpWssoGetWebSocket
        *   HttpWssoSetWebSocket
        *   HttpWssoGetSessionState
        *   HttpWssoSetSessionState
        *   HttpWssoReturn
        *   HttpWssoReset

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

        ANSC_HANDLE
        HttpWssoGetWspIf
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
HttpWssoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    return  pMyObject->hWspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoSetWspIf
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
HttpWssoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    pMyObject->hWspIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWssoGetHfpIf
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
HttpWssoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoSetHfpIf
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
HttpWssoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWssoGetWebSocket
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
HttpWssoGetWebSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    return  pMyObject->hWebSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoSetWebSocket
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
HttpWssoSetWebSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    pMyObject->hWebSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWssoGetSessionState
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
HttpWssoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    return  pMyObject->SessionState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoSetSessionState
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
HttpWssoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    pMyObject->SessionState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoReturn
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
HttpWssoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pSimpleServer->ReleaseWsso
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
        HttpWssoReset
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
HttpWssoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    pMyObject->hWebSocket   = (ANSC_HANDLE)NULL;
    pMyObject->SessionState = HTTP_WSSO_STATE_INITIALIZED;

    return  ANSC_STATUS_SUCCESS;
}
