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

    module:	http_sso_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Simple Server Object.

        *   HttpSsoEngage
        *   HttpSsoCancel
        *   HttpSsoWorkerInit
        *   HttpSsoWorkerUnload

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


#include "http_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty     = (PHTTP_SIMPLE_SERVER_PROPERTY  )&pMyObject->Property;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    ULONG                           ulEngineCount = HTTP_MSERVER_ENGINE_NUMBER;
    ULONG                           ulSocketCount = HTTP_MSERVER_SOCKET_NUMBER;
    ULONG                           ulTcpDsoMode  = ANSC_DSTO_MODE_EVENT_SYNC | ANSC_DSTO_MODE_FOREIGN_BUFFER | ANSC_DSTO_MODE_COMPACT;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pMyObject->ManufactureWssoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureWstoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoRepPool((ANSC_HANDLE)pMyObject);

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_TLS_ENABLED )
    {
        ulTcpDsoMode |= ANSC_DSTO_MODE_TLS_ENABLED;
    }

    if ( pMyObject->ServerMode & HTTP_SSO_MODE_TLS_AUTH_CLIENT )
    {
        ulTcpDsoMode |= ANSC_DSTO_MODE_TLS_REQ_CERT;
    }

    if ( pProperty->ServerMode & HTTP_SERVER_MODE_useXsocket )
    {
        ulTcpDsoMode |= ANSC_DSTO_MODE_XSOCKET;
    }

    switch ( pProperty->ServerType )
    {
        case    HTTP_SERVER_TYPE_SMALL :

                ulEngineCount = HTTP_SSERVER_ENGINE_NUMBER;
                ulSocketCount = HTTP_SSERVER_SOCKET_NUMBER;

                break;

        case    HTTP_SERVER_TYPE_MEDIUM :

                ulEngineCount = HTTP_MSERVER_ENGINE_NUMBER;
                ulSocketCount = HTTP_MSERVER_SOCKET_NUMBER;

                break;

        case    HTTP_SERVER_TYPE_BIG :

                ulEngineCount = HTTP_BSERVER_ENGINE_NUMBER;
                ulSocketCount = HTTP_BSERVER_SOCKET_NUMBER;

                break;

        case    HTTP_SERVER_TYPE_GIANT :

                ulEngineCount = HTTP_GSERVER_ENGINE_NUMBER;
                ulSocketCount = HTTP_GSERVER_SOCKET_NUMBER;

                break;

        default :

                ulEngineCount = HTTP_MSERVER_ENGINE_NUMBER;
                ulSocketCount = HTTP_MSERVER_SOCKET_NUMBER;

                break;
    }

    pDaemonServer->SetHostAddress   ((ANSC_HANDLE)pDaemonServer, pProperty->HostAddress.Dot);
    pDaemonServer->SetHostPort      ((ANSC_HANDLE)pDaemonServer, pProperty->HostPort       );
    pDaemonServer->SetMaxMessageSize((ANSC_HANDLE)pDaemonServer, HTTP_SSO_MAX_MESSAGE_SIZE );
    pDaemonServer->SetEngineCount   ((ANSC_HANDLE)pDaemonServer, ulEngineCount             );
    pDaemonServer->SetMinSocketCount((ANSC_HANDLE)pDaemonServer, 0                         );
    pDaemonServer->SetMaxSocketCount((ANSC_HANDLE)pDaemonServer, ulSocketCount             );
    pDaemonServer->SetMode          ((ANSC_HANDLE)pDaemonServer, ulTcpDsoMode              );

    returnStatus = pDaemonServer->Engage((ANSC_HANDLE)pDaemonServer);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject     = (PHTTP_SIMPLE_SERVER_OBJECT    )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    returnStatus = pDaemonServer->Cancel((ANSC_HANDLE)pDaemonServer);

    pMyObject->DestroyWssoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyWstoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoRepPool((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoWorkerInit
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoWorkerInit
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
        HttpSsoWorkerUnload
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoWorkerUnload
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
