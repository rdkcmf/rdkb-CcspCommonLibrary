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

    module:	web_sso_cspif.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Web Simple Server Object.

        *   WebSsoCspGetCfgMode
        *   WebSsoCspIsReadOnly
        *   WebSsoCspIsSwitchOn
        *   WebSsoCspEngage
        *   WebSsoCspCancel
        *   WebSsoCspAddHttpDaemon
        *   WebSsoCspDelHttpDaemon
        *   WebSsoCspSetHttpServerMode
        *   WebSsoCspSetPersistentConn
        *   WebSsoCspSetProductName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/02    initial revision.
        02/19/04    updated to support multiple HTTP daemons

**********************************************************************/


#include "web_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebSsoCspGetCfgMode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the configuration mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     configuration mode.

**********************************************************************/

ULONG
WebSsoCspGetCfgMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_CFG_MODE_RDWR;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebSsoCspIsReadOnly
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the configuration mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     configuration mode.

**********************************************************************/

BOOL
WebSsoCspIsReadOnly
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebSsoCspIsSwitchOn
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current module status.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     module status.

**********************************************************************/

BOOL
WebSsoCspIsSwitchOn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    return  pMyObject->bActive;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the system module.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    returnStatus = pMyObject->Engage((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the system module.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;

    returnStatus = pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspAddHttpDaemon
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDaemonInfo
            );

    description:

        This function is called to retrieve server's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDaemonInfo
                Specifies the HTTP daemon to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspAddHttpDaemon
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonInfo
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty    = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    if ( pProperty->HttpDaemonCount >= WEB_MAX_DAEMON_NUMBER )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pProperty->HttpDaemonArray[pProperty->HttpDaemonCount++] = *(PWEB_HTTP_DAEMON_INFO)hDaemonInfo;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspDelHttpDaemon
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usDaemonPort
            );

    description:

        This function is called to configure server's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      usDaemonPort
                Specifies the HTTP daemon port to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspDelHttpDaemon
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usDaemonPort
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty    = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;
    ULONG                           j            = 0;

    for ( i = 0; i < pProperty->HttpDaemonCount; i++ )
    {
        if ( pProperty->HttpDaemonArray[i].DaemonPort == usDaemonPort )
        {
            for ( j = i; j < (pProperty->HttpDaemonCount - 1); j++ )
            {
                pProperty->HttpDaemonArray[j] = pProperty->HttpDaemonArray[j + 1];
            }

            pProperty->HttpDaemonCount--;

            break;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspSetHttpServerMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure server's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the server property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspSetHttpServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject       = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SIMPLE_SERVER_PROPERTY     pProperty       = (PWEB_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;

    pProperty->HttpServerMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspSetPersistentConn
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bPersistentConn
            );

    description:

        This function is called to configure server's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bPersistentConn
                Specifies the server property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspSetPersistentConn
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bPersistentConn
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject       = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager = (PWEB_SITE_MANAGER_OBJECT   )pMyObject->hWebSiteManager;

    pWebSiteManager->SetPersistentConn((ANSC_HANDLE)pWebSiteManager, bPersistentConn);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoCspSetProductName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pProductName
            );

    description:

        This function is called to configure server's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pProductName
                Specifies the server property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSsoCspSetProductName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pProductName
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject       = (PWEB_SIMPLE_SERVER_OBJECT  )hThisObject;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager = (PWEB_SITE_MANAGER_OBJECT   )pMyObject->hWebSiteManager;

    pWebSiteManager->SetProductName((ANSC_HANDLE)pWebSiteManager, pProductName);

    return  ANSC_STATUS_SUCCESS;
}
