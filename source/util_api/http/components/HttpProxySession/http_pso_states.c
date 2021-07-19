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

    module:	http_pso_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Proxy Session Object.

        *   HttpPsoGetWamIf
        *   HttpPsoSetWamIf
        *   HttpPsoGetSbcIf
        *   HttpPsoSetSbcIf
        *   HttpPsoGetCbcIf
        *   HttpPsoSetCbcIf
        *   HttpPsoGetPbcIf
        *   HttpPsoSetPbcIf
        *   HttpPsoGetHfpIf
        *   HttpPsoSetHfpIf
        *   HttpPsoGetClientSocket
        *   HttpPsoSetClientSocket
        *   HttpPsoGetServerSocket
        *   HttpPsoSetServerSocket
        *   HttpPsoGetSbcContext
        *   HttpPsoSetSbcContext
        *   HttpPsoGetCbcContext
        *   HttpPsoSetCbcContext
        *   HttpPsoGetServerName
        *   HttpPsoSetServerName
        *   HttpPsoGetClientAddr
        *   HttpPsoGetServerAddr
        *   HttpPsoGetClientPort
        *   HttpPsoGetServerPort
        *   HttpPsoGetSbcPmode
        *   HttpPsoSetSbcPmode
        *   HttpPsoGetCbcPmode
        *   HttpPsoSetCbcPmode
        *   HttpPsoGetSessionState
        *   HttpPsoSetSessionState
        *   HttpPsoReturn
        *   HttpPsoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

**********************************************************************/


#include "http_pso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetWamIf
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
HttpPsoGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hWamIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetWamIf
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
HttpPsoSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hWamIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetSbcIf
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
HttpPsoGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hSbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetSbcIf
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
HttpPsoSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hSbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetCbcIf
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
HttpPsoGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hCbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetCbcIf
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
HttpPsoSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hCbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetPbcIf
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
HttpPsoGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hPbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetPbcIf
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
HttpPsoSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hPbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetHfpIf
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
HttpPsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetHfpIf
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
HttpPsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetClientSocket
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
HttpPsoGetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hClientSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetClientSocket
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
HttpPsoSetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hClientSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetServerSocket
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
HttpPsoGetServerSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hServerSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetServerSocket
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
HttpPsoSetServerSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hServerSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetSbcContext
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
HttpPsoGetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hSbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetSbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoSetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hSbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoGetCbcContext
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
HttpPsoGetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->hCbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetCbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoSetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hCbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpPsoGetServerName
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

char*
HttpPsoGetServerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    
    return  pMyObject->ServerName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetServerName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoSetServerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    errno_t                         rc           = -1;

    AnscZeroMemory(pMyObject->ServerName, ANSC_DOMAIN_NAME_SIZE);
    rc = strcpy_s(pMyObject->ServerName, sizeof(pMyObject->ServerName), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpPsoGetClientAddr
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
HttpPsoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    
    if ( !pClientSocket )
    {
        return  NULL;
    }

    return  pClientSocket->GetPeerAddress((ANSC_HANDLE)pClientSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpPsoGetServerAddr
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
HttpPsoGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    
    return  pMyObject->ServerAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpPsoGetClientPort
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
HttpPsoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    
    if ( !pClientSocket )
    {
        return  0;
    }

    return  pClientSocket->GetPeerPort((ANSC_HANDLE)pClientSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpPsoGetServerPort
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
HttpPsoGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject     = (PHTTP_PROXY_SESSION_OBJECT    )hThisObject;
    
    return  pMyObject->ServerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoGetSbcPmode
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
HttpPsoGetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->SbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetSbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoSetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->SbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoGetCbcPmode
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
HttpPsoGetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->CbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetCbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoSetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->CbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoGetSessionState
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
HttpPsoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    return  pMyObject->SessionState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoSetSessionState
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
HttpPsoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->SessionState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoReturn
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
HttpPsoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_SIMPLE_PROXY_OBJECT       pSimpleProxy = (PHTTP_SIMPLE_PROXY_OBJECT )pMyObject->hOwnerContext;
    PHTTP_WAM_INTERFACE             pWamIf       = (PHTTP_WAM_INTERFACE       )pMyObject->hWamIf;

    returnStatus =
        pWamIf->Close
            (
                pWamIf->hOwnerContext,
                (ANSC_HANDLE)pMyObject
            );

    returnStatus =
        pSimpleProxy->ReleasePso
            (
                (ANSC_HANDLE)pSimpleProxy,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoReset
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
HttpPsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

    pMyObject->hClientSocket = (ANSC_HANDLE)NULL;
    pMyObject->hServerSocket = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext   = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext   = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex     = 0;
    pMyObject->SbcPmode      = HTTP_SBC_PMODE_RELAY2;
    pMyObject->CbcPmode      = HTTP_CBC_PMODE_RELAY2;
    pMyObject->SessionState  = HTTP_PSO_STATE_INITIALIZED;
    pMyObject->bEstablished  = FALSE;

    return  ANSC_STATUS_SUCCESS;
}
