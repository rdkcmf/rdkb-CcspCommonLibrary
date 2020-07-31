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

    module:	http_wsso_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Webs Session Object.

        *   HttpWssoOpen
        *   HttpWssoClose
        *   HttpWssoAcquireAccess
        *   HttpWssoReleaseAccess
        *   HttpWssoEnterWspServe
        *   HttpWssoLeaveWspServe
        *   HttpWssoClearWspServe
        *   HttpWssoCloseConnection

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

        ANSC_STATUS
        HttpWssoOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to disable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    
    pMyObject->DelAllWstos((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    
    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoEnterWspServe
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enter the serving state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoEnterWspServe
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    
    /*AnscResetEvent(&pMyObject->ServeEvent);*/

    pMyObject->ServeCount++;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoLeaveWspServe
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to leave the serving state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoLeaveWspServe
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    /*AnscSetEvent(&pMyObject->ServeEvent);*/

    pMyObject->ServeCount--;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoClearWspServe
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to leave the serving state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoClearWspServe
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT )hThisObject;
    
    /*AnscWaitEvent(&pMyObject->ServeEvent, 0xFFFFFFFF);*/

    while ( pMyObject->ServeCount > 0 )
    {
        AnscSleep(200);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoCloseConnection
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the underlying web socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWssoCloseConnection
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject     = (PHTTP_WEBS_SESSION_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;

    if ( pWebSocket )
    {
        /*pWebSocket->Finish((ANSC_HANDLE)pWebSocket);*/
        pWebSocket->ToClean((ANSC_HANDLE)pWebSocket, TRUE, HTTP_SSO_SOCKET_TTC);
    }

    return  ANSC_STATUS_SUCCESS;
}
