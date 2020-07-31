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

    module:	web_sso_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Simple Server Object.

        *   WebSsoGetSiteManager
        *   WebSsoGetHfpIf
        *   WebSsoSetHfpIf
        *   WebSsoGetFumIf
        *   WebSsoGetCspIf
        *   WebSsoGetServerMode
        *   WebSsoSetServerMode
        *   WebSsoGetProperty
        *   WebSsoSetProperty
        *   WebSsoResetProperty
        *   WebSsoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/02    initial revision.

**********************************************************************/


#include "web_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebSsoGetSiteManager
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
WebSsoGetSiteManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hWebSiteManager;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebSsoGetHfpIf
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
WebSsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoSetHfpIf
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
WebSsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject         = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty         = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    PHTTP_HFP_INTERFACE             pHfpIf            = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    PHTTP_SIMPLE_SERVER_OBJECT      pHttpSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT )NULL;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager   = (PWEB_SITE_MANAGER_OBJECT   )pMyObject->hWebSiteManager;
    ULONG                           i                 = 0;

    *pHfpIf = *(PHTTP_HFP_INTERFACE)hInterface;

    for ( i = 0; i < pProperty->HttpDaemonCount; i++ )
    {
        pHttpSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hHttpSsoArray[i];

        if ( pHttpSimpleServer )
        {
            pHttpSimpleServer->SetHfpIf((ANSC_HANDLE)pHttpSimpleServer, (ANSC_HANDLE)pHfpIf);
        }
    }

    pWebSiteManager->SetHfpIf((ANSC_HANDLE)pWebSiteManager, (ANSC_HANDLE)pHfpIf);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebSsoGetFumIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
WebSsoGetFumIf
    (
        ANSC_HANDLE                 hThisObject/*,
        ANSC_HANDLE                 hInterface*/
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject         = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    return pMyObject->hFumIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebSsoGetCspIf
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
WebSsoGetCspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hCspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebSsoGetServerMode
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
WebSsoGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    return  pMyObject->ServerMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoSetServerMode
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
WebSsoSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    pMyObject->ServerMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoGetProperty
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
WebSsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty    = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    *(PWEB_SIMPLE_SERVER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoSetProperty
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
WebSsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty    = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PWEB_SIMPLE_SERVER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoResetProperty
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
WebSsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty    = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    pProperty->HttpDaemonCount    = 0;
    pProperty->HttpServerMode     = HTTP_SERVER_MODE_memoryConserving;
    pProperty->HttpReqBodySizeCap = HTTP_SSO_DEF_BODY_SIZE_CAP;
    pProperty->HttpMaxBodySizeCap = HTTP_SSO_MAX_BODY_SIZE_CAP;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoReset
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
WebSsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
