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

    module:	http_spo_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Simple Proxy Object.

        *   HttpSpoEngage
        *   HttpSpoCancel
        *   HttpSpoWorkerInit
        *   HttpSpoWorkerUnload

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/21/02    initial revision.

**********************************************************************/


#include "http_spo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoEngage
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
HttpSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT    )hThisObject;
    PHTTP_SIMPLE_PROXY_PROPERTY     pProperty     = (PHTTP_SIMPLE_PROXY_PROPERTY  )&pMyObject->Property;
    PANSC_SIMPLE_PROXY_TCP_OBJECT   pSimpleProxy  = (PANSC_SIMPLE_PROXY_TCP_OBJECT)pMyObject->hSimpleProxy;
    ULONG                           ulEngineCount = HTTP_MPROXY_ENGINE_NUMBER;
    ULONG                           ulSocketCount = HTTP_MPROXY_SOCKET_NUMBER;
    ULONG                           ulTcpSpoMode  = ANSC_SPTO_MODE_DAEMON_TIMEOUT | ANSC_SPTO_MODE_BROKER_TIMEOUT | ANSC_SPTO_MODE_FOREIGN_BUFFER;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pMyObject->ManufacturePsoPool   ((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoRepPool((ANSC_HANDLE)pMyObject);

    switch ( pProperty->ProxyType )
    {
        case    HTTP_PROXY_TYPE_SMALL :

                ulEngineCount = HTTP_SPROXY_ENGINE_NUMBER;
                ulSocketCount = HTTP_SPROXY_SOCKET_NUMBER;

                break;

        case    HTTP_PROXY_TYPE_MEDIUM :

                ulEngineCount = HTTP_MPROXY_ENGINE_NUMBER;
                ulSocketCount = HTTP_MPROXY_SOCKET_NUMBER;

                break;

        case    HTTP_PROXY_TYPE_BIG :

                ulEngineCount = HTTP_BPROXY_ENGINE_NUMBER;
                ulSocketCount = HTTP_BPROXY_SOCKET_NUMBER;

                break;

        case    HTTP_PROXY_TYPE_GIANT :

                ulEngineCount = HTTP_GPROXY_ENGINE_NUMBER;
                ulSocketCount = HTTP_GPROXY_SOCKET_NUMBER;

                break;

        default :

                ulEngineCount = HTTP_MPROXY_ENGINE_NUMBER;
                ulSocketCount = HTTP_MPROXY_SOCKET_NUMBER;

                break;
    }

    pSimpleProxy->SetHostAddress   ((ANSC_HANDLE)pSimpleProxy, pProperty->HostAddress.Dot);
    pSimpleProxy->SetHostPort      ((ANSC_HANDLE)pSimpleProxy, pProperty->HostPort       );
    pSimpleProxy->SetMaxMessageSize((ANSC_HANDLE)pSimpleProxy, HTTP_SPO_MAX_MESSAGE_SIZE );
    pSimpleProxy->SetEngineCount   ((ANSC_HANDLE)pSimpleProxy, ulEngineCount             );
    pSimpleProxy->SetMinSocketCount((ANSC_HANDLE)pSimpleProxy, 0                         );
    pSimpleProxy->SetMaxSocketCount((ANSC_HANDLE)pSimpleProxy, ulSocketCount             );
    pSimpleProxy->SetSocketTimeOut ((ANSC_HANDLE)pSimpleProxy, HTTP_SPO_SOCKET_TIMEOUT   );
    pSimpleProxy->SetMode          ((ANSC_HANDLE)pSimpleProxy, ulTcpSpoMode              );

    returnStatus = pSimpleProxy->Engage((ANSC_HANDLE)pSimpleProxy);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoCancel
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
HttpSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_PROXY_OBJECT       pMyObject     = (PHTTP_SIMPLE_PROXY_OBJECT    )hThisObject;
    PANSC_SIMPLE_PROXY_TCP_OBJECT   pSimpleProxy  = (PANSC_SIMPLE_PROXY_TCP_OBJECT)pMyObject->hSimpleProxy;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    returnStatus = pSimpleProxy->Cancel((ANSC_HANDLE)pSimpleProxy);

    pMyObject->DestroyPsoPool   ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoRepPool((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSpoWorkerInit
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
HttpSpoWorkerInit
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
        HttpSpoWorkerUnload
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
HttpSpoWorkerUnload
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
