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

    module:	http_sso_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Simple Server Object.

        *   HttpSsoGetWspIf
        *   HttpSsoSetWspIf
        *   HttpSsoGetHfpIf
        *   HttpSsoSetHfpIf
        *   HttpSsoGetFumIf
        *   HttpSsoSetFumIf
        *   HttpSsoGetServerMode
        *   HttpSsoSetServerMode
        *   HttpSsoGetProperty
        *   HttpSsoSetProperty
        *   HttpSsoResetProperty
        *   HttpSsoReset

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

        ANSC_HANDLE
        HttpSsoGetWspIf
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
HttpSsoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    
    return  pMyObject->hWspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoSetWspIf
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
HttpSsoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_WSP_INTERFACE             pWspIf       = (PHTTP_WSP_INTERFACE         )pMyObject->hWspIf;

    *pWspIf = *(PHTTP_WSP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSsoGetHfpIf
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
HttpSsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    
    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoSetHfpIf
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
HttpSsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;

    *pHfpIf = *(PHTTP_HFP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSsoGetFumIf
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
HttpSsoGetFumIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    
    return  pMyObject->hFumIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoSetFumIf
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
HttpSsoSetFumIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    pMyObject->hFumIf   = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSsoGetServerMode
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
HttpSsoGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    
    return  pMyObject->ServerMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoSetServerMode
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
HttpSsoSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    
    pMyObject->ServerMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoGetProperty
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
HttpSsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty    = (PHTTP_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    *(PHTTP_SIMPLE_SERVER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoSetProperty
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
HttpSsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty    = (PHTTP_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PHTTP_SIMPLE_SERVER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoResetProperty
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
HttpSsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty    = (PHTTP_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    pProperty->HostAddress.Value = 0;
    pProperty->HostPort          = HTTP_SERVER_PORT;
    pProperty->ServerType        = HTTP_SERVER_TYPE_MEDIUM;
    pProperty->ServerMode        = HTTP_SERVER_MODE_memoryConserving;
    pProperty->ReqBodySizeCap    = HTTP_SSO_DEF_BODY_SIZE_CAP;
    pProperty->MaxBodySizeCap    = HTTP_SSO_MAX_BODY_SIZE_CAP;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoReset
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
HttpSsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
   UNREFERENCED_PARAMETER(hThisObject);
    
   return  ANSC_STATUS_SUCCESS;
}
