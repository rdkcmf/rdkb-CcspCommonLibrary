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

    module:	web_sso_operation.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web Simple Server Object.

        *   WebSsoEngage
        *   WebSsoCancel

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

        ANSC_STATUS
        WebSsoEngage
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
WebSsoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject         = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty         = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    PWEB_HTTP_DAEMON_INFO           pHttpDaemonInfo   = (PWEB_HTTP_DAEMON_INFO      )NULL;
    PHTTP_SIMPLE_SERVER_OBJECT      pHttpSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT )NULL;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager   = (PWEB_SITE_MANAGER_OBJECT   )pMyObject->hWebSiteManager;
    PHTTP_WSP_INTERFACE             pWspIf            = (PHTTP_WSP_INTERFACE        )pWebSiteManager->GetWspIf((ANSC_HANDLE)pWebSiteManager);
    PHTTP_HFP_INTERFACE             pHfpIf            = (PHTTP_HFP_INTERFACE        )pMyObject->hHfpIf;
    ULONG                           ulHttpServerMode  = (ULONG                      )HTTP_SSO_MODE_COMPACT;
    ULONG                           i                 = 0;
    HTTP_SIMPLE_SERVER_PROPERTY     httpSsoProperty;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    for ( i = 0; i < pProperty->HttpDaemonCount; i++ )
    {
        pHttpDaemonInfo   = (PWEB_HTTP_DAEMON_INFO)&pProperty->HttpDaemonArray[i];
        pHttpSimpleServer = NULL;
        ulHttpServerMode  = HTTP_SSO_MODE_COMPACT;

        if ( pHttpDaemonInfo->ServerFlag & WEB_SERVER_FLAG_TLS )
        {
            ulHttpServerMode |= HTTP_SSO_MODE_TLS_ENABLED;
        }

        if ( pHttpDaemonInfo->ServerFlag & WEB_SERVER_FLAG_TLS_AUTH_CLIENT )
        {
            ulHttpServerMode |= HTTP_SSO_MODE_TLS_AUTH_CLIENT;
        }

        if ( TRUE )
        {
            pHttpSimpleServer =
                (PHTTP_SIMPLE_SERVER_OBJECT)HttpCreateSimpleServer
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pHttpSimpleServer )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                pMyObject->hHttpSsoArray[i] = (ANSC_HANDLE)pHttpSimpleServer;
            }

            httpSsoProperty.HostAddress.Value = pHttpDaemonInfo->DaemonAddress.Value;
            httpSsoProperty.HostPort          = pHttpDaemonInfo->DaemonPort;
            httpSsoProperty.ServerType        = pHttpDaemonInfo->ServerType;
            httpSsoProperty.ServerMode        = pProperty->HttpServerMode;
            httpSsoProperty.ReqBodySizeCap    = pProperty->HttpReqBodySizeCap;
            httpSsoProperty.MaxBodySizeCap    = pProperty->HttpMaxBodySizeCap;

            pHttpSimpleServer->SetWspIf     ((ANSC_HANDLE)pHttpSimpleServer, (ANSC_HANDLE)pWspIf          );
            pHttpSimpleServer->SetHfpIf     ((ANSC_HANDLE)pHttpSimpleServer, (ANSC_HANDLE)pHfpIf          );
            pHttpSimpleServer->SetFumIf     ((ANSC_HANDLE)pHttpSimpleServer, pMyObject->hFumIf            );
            pHttpSimpleServer->SetServerMode((ANSC_HANDLE)pHttpSimpleServer, ulHttpServerMode             );
            pHttpSimpleServer->SetProperty  ((ANSC_HANDLE)pHttpSimpleServer, (ANSC_HANDLE)&httpSsoProperty);
            pHttpSimpleServer->Engage       ((ANSC_HANDLE)pHttpSimpleServer);
        }
    }

    pWebSiteManager->SetHfpIf((ANSC_HANDLE)pWebSiteManager, (ANSC_HANDLE)pHfpIf);
    pWebSiteManager->Engage  ((ANSC_HANDLE)pWebSiteManager);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCancel
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
WebSsoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject         = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty         = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    PHTTP_SIMPLE_SERVER_OBJECT      pHttpSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT )NULL;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager   = (PWEB_SITE_MANAGER_OBJECT   )pMyObject->hWebSiteManager;
    ULONG                           i                 = 0;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    for ( i = 0; i < pProperty->HttpDaemonCount; i++ )
    {
        pHttpSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hHttpSsoArray[i];

        if ( pHttpSimpleServer )
        {
            pHttpSimpleServer->Cancel((ANSC_HANDLE)pHttpSimpleServer);
            pHttpSimpleServer->Remove((ANSC_HANDLE)pHttpSimpleServer);

            pMyObject->hHttpSsoArray[i] = (ANSC_HANDLE)NULL;
        }
    }

    pWebSiteManager->Cancel((ANSC_HANDLE)pWebSiteManager);

    return  returnStatus;
}
