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

    module:	http_wcso_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Webc Session Object.

        *   HttpWcsoGetPeerName
        *   HttpWcsoSetPeerName
        *   HttpWcsoGetPeerPort
        *   HttpWcsoSetPeerPort
        *   HttpWcsoGetHostAddr
        *   HttpWcsoSetHostAddr
        *   HttpWcsoGetHostPort
        *   HttpWcsoSetHostPort
        *   HttpWcsoGetSessionFlags
        *   HttpWcsoSetSessionFlags
        *   HttpWcsoGetBspIf
        *   HttpWcsoSetBspIf
        *   HttpWcsoGetHfpIf
        *   HttpWcsoSetHfpIf
        *   HttpWcsoGetSessionState
        *   HttpWcsoSetSessionState
        *   HttpWcsoGetProperty
        *   HttpWcsoSetProperty
        *   HttpWcsoResetProperty
        *   HttpWcsoReset
        *   HttpWcsoReturn

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


#include "http_wcso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpWcsoGetPeerName
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
HttpWcsoGetPeerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    return  pProperty->PeerName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetPeerName
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
HttpWcsoSetPeerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pScto        = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    AnscZeroMemory(pProperty->PeerName, ANSC_DOMAIN_NAME_SIZE);
    rc = strcpy_s(pProperty->PeerName, sizeof(pProperty->PeerName), name);
    ERR_CHK(rc);
    _ansc_strncpy(pScto->HostName, name, WEB_MAX_HOST_NAME_SIZE);
    pScto->HostName[WEB_MAX_HOST_NAME_SIZE] = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpWcsoGetPeerPort
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
HttpWcsoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    return  pProperty->PeerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetPeerPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    pProperty->PeerPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpWcsoGetHostAddr
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
HttpWcsoGetHostAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    return  pProperty->HostAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetHostAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSetHostAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    pProperty->HostAddr.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpWcsoGetHostPort
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
HttpWcsoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    return  pProperty->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetHostPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    pProperty->HostPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWcsoGetSessionFlags
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
HttpWcsoGetSessionFlags
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    return  pProperty->SessionFlags;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetSessionFlags
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulFlags
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulFlags
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSetSessionFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    pProperty->SessionFlags = ulFlags;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWcsoGetBspIf
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
HttpWcsoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    return  pMyObject->hBspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetBspIf
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
HttpWcsoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    pMyObject->hBspIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpWcsoGetHfpIf
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
HttpWcsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetHfpIf
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
HttpWcsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpWcsoGetSessionState
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
HttpWcsoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    return  pMyObject->SessionState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetSessionState
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
HttpWcsoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT)hThisObject;

    pMyObject->SessionState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    *(PHTTP_WEBC_SESSION_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    *pProperty = *(PHTTP_WEBC_SESSION_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty->PeerName, ANSC_DOMAIN_NAME_SIZE);

    pProperty->PeerAddr.Value = 0;
    pProperty->PeerPort       = HTTP_SERVER_PORT;
    pProperty->HostAddr.Value = 0;
    pProperty->HostPort       = 0;
    pProperty->SessionFlags   = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoReset
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
HttpWcsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject     = (PHTTP_WEBC_SESSION_OBJECT )hThisObject;
    
    pMyObject->SessionState     = HTTP_WCSO_STATE_INITIALIZED;
    pMyObject->RetryPeerAddr    = 0;

    if ( pMyObject->hReqUri )
    {
        AnscFreeMemory(pMyObject->hReqUri);
        pMyObject->hReqUri  = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoReturn
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
HttpWcsoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject     = (PHTTP_WEBC_SESSION_OBJECT )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient = (PHTTP_SIMPLE_CLIENT_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pSimpleClient->ReleaseWcso
            (
                (ANSC_HANDLE)pSimpleClient,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}
