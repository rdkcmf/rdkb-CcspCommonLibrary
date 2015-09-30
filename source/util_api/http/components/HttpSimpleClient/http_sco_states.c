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

    module:	http_sco_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Simple Client Object.

        *   HttpScoGetBspIf
        *   HttpScoSetBspIf
        *   HttpScoGetHfpIf
        *   HttpScoSetHfpIf
        *   HttpScoGetCasIf
        *   HttpScoGetClientAuthObj
        *   HttpScoGetClientMode
        *   HttpScoSetClientMode
        *   HttpScoGetProductName
        *   HttpScoSetProductName
        *   HttpScoSetSessionIdleTimeout
        *   HttpScoGetProperty
        *   HttpScoSetProperty
        *   HttpScoResetProperty
        *   HttpScoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/20/02    initial revision.

**********************************************************************/


#include "http_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoGetBspIf
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
HttpScoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pMyObject->hBspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetBspIf
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
HttpScoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PHTTP_BSP_INTERFACE             pBspIf       = (PHTTP_BSP_INTERFACE         )pMyObject->hBspIf;

    *pBspIf = *(PHTTP_BSP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoGetHfpIf
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
HttpScoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetHfpIf
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
HttpScoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;

    *pHfpIf = *(PHTTP_HFP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoGetCasIf
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
HttpScoGetCasIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pMyObject->hCasIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoGetClientAuthObj
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
HttpScoGetClientAuthObj
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pMyObject->hClientAuthObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpScoGetClientMode
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
HttpScoGetClientMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pMyObject->ClientMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetClientMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoSetClientMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pMyObject->ClientMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpScoGetProductName
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
HttpScoGetProductName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ProductName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetProductName
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
HttpScoSetProductName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty->ProductName, HTTP_MAX_PRODUCT_NAME_SIZE);
    AnscCopyString(pProperty->ProductName, name                      );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetSessionIdleTimeout
            (
                ANSC_HANDLE                 hThisObject,
                ULONG						ulTimeout
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG						ulTimeout
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoSetSessionIdleTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG						ulTimeout
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

	pMyObject->SessionIdleTimeout = ulTimeout;

    return  ANSC_STATUS_SUCCESS;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoGetProperty
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
HttpScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    *(PHTTP_SIMPLE_CLIENT_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSetProperty
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
HttpScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    *pProperty = *(PHTTP_SIMPLE_CLIENT_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoResetProperty
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
HttpScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(HTTP_SIMPLE_CLIENT_PROPERTY));

    pProperty->bReuseConnections = TRUE;

    AnscCopyString(pProperty->ProductName, HTTP_SCO_PRODUCT_NAME);

    pProperty->SPMode   = HTTP_SCO_MHOST_MODE_RANDOM;

    pMyObject->LastPicked   = (ULONG)-1;
    AnscZeroMemory(&pMyObject->PeerAddresses, sizeof(HTTP_SCO_HOST_ADDRESSES));

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoReset
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
HttpScoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoReset
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
HttpScoGetCredential
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pHostName,
        USHORT                      HostPort,
        PUCHAR                      pUriPath,
        PUCHAR*                     ppUserName,
        PUCHAR*                     ppPassword
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    *ppUserName = AnscCloneString(pMyObject->pAuthUserName);
    *ppPassword = AnscCloneString(pMyObject->pAuthPassword);

    return returnStatus;
}

