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

    module:	http_apo_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Advanced Proxy Object.

        *   HttpApoGetWamIf
        *   HttpApoSetWamIf
        *   HttpApoGetSbcIf
        *   HttpApoSetSbcIf
        *   HttpApoGetCbcIf
        *   HttpApoSetCbcIf
        *   HttpApoGetPbcIf
        *   HttpApoSetPbcIf
        *   HttpApoGetHfpIf
        *   HttpApoSetHfpIf
        *   HttpApoGetProxyMode
        *   HttpApoSetProxyMode
        *   HttpApoGetProperty
        *   HttpApoSetProperty
        *   HttpApoResetProperty
        *   HttpApoReset

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


#include "http_apo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoGetWamIf
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
HttpApoGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->hWamIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetWamIf
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
HttpApoSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf       = (PHTTP_WAM_INTERFACE          )pMyObject->hWamIf;

    *pWamIf = *(PHTTP_WAM_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoGetSbcIf
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
HttpApoGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->hSbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetSbcIf
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
HttpApoSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_SBC_INTERFACE             pSbcIf       = (PHTTP_SBC_INTERFACE          )pMyObject->hSbcIf;

    *pSbcIf = *(PHTTP_SBC_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoGetCbcIf
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
HttpApoGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->hCbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetCbcIf
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
HttpApoSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_CBC_INTERFACE             pCbcIf       = (PHTTP_CBC_INTERFACE          )pMyObject->hCbcIf;

    *pCbcIf = *(PHTTP_CBC_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoGetPbcIf
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
HttpApoGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->hPbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetPbcIf
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
HttpApoSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_PBC_INTERFACE             pPbcIf       = (PHTTP_PBC_INTERFACE          )pMyObject->hPbcIf;

    *pPbcIf = *(PHTTP_PBC_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpApoGetHfpIf
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
HttpApoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetHfpIf
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
HttpApoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;

    *pHfpIf = *(PHTTP_HFP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpApoGetProxyMode
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
HttpApoGetProxyMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    return  pMyObject->ProxyMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetProxyMode
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
HttpApoSetProxyMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    
    pMyObject->ProxyMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoGetProperty
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
HttpApoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;

    *(PHTTP_ADVANCED_PROXY_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoSetProperty
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
HttpApoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;

    *pProperty = *(PHTTP_ADVANCED_PROXY_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoResetProperty
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
HttpApoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;

    pProperty->HostAddress.Value      = 0;
    pProperty->HostPort               = HTTP_SERVER_PORT;
    pProperty->ProxyType              = HTTP_PROXY_TYPE_MEDIUM;

    pProperty->bChainedProxy          = FALSE;
    pProperty->NextProxyAddress.Value = 0;
    pProperty->NextProxyPort          = HTTP_SERVER_PORT;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoReset
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
HttpApoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
