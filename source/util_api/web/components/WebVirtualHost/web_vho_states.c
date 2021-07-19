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

    module:	web_vho_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Virtual Host Object.

        *   WebVhoGetHostName
        *   WebVhoSetHostName
        *   WebVhoGetHostPort
        *   WebVhoSetHostPort
        *   WebVhoGetHfpIf
        *   WebVhoSetHfpIf
        *   WebVhoGetLsmIf
        *   WebVhoSetLsmIf
        *   WebVhoIsDefaultHost
        *   WebVhoGetProperty
        *   WebVhoSetProperty
        *   WebVhoResetProperty
        *   WebVhoReset
        *   WebVhoSetHttpAuth
        *   WebVhoSetHttpAuth2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#include "web_vho_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebVhoGetHostName
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
WebVhoGetHostName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    return  pProperty->HostName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetHostName
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
WebVhoSetHostName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    rc = strcpy_s(pProperty->HostName, sizeof(pProperty->HostName), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        WebVhoGetHostPort
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
WebVhoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    return  pProperty->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetHostPort
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
WebVhoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    pProperty->HostPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebVhoGetHfpIf
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
WebVhoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetHfpIf
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
WebVhoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebVhoGetLsmIf
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
WebVhoGetLsmIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    return  pMyObject->hLsmIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetLsmIf
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
WebVhoSetLsmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_LSM_INTERFACE              pLsmIf       = (PWEB_LSM_INTERFACE        )pMyObject->hLsmIf;

    *pLsmIf = *(PWEB_LSM_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebVhoIsDefaultHost
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

BOOL
WebVhoIsDefaultHost
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    return  pProperty->bDefaultVirtualHost;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoGetProperty
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
WebVhoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    *(PWEB_VIRTUAL_HOST_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetProperty
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
WebVhoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    *pProperty = *(PWEB_VIRTUAL_HOST_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoResetProperty
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
WebVhoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    rc = strcpy_s(pProperty->HostName, sizeof(pProperty->HostName), WEB_WILDCARD_HOST_NAME);
    ERR_CHK(rc);

    pProperty->HostPort               = 0;      /* wildcard, accept request on any port */
    pProperty->bDefaultVirtualHost    = FALSE;
    pProperty->bEnableSessionTracking = FALSE;
    pProperty->SessionTimeOut         = WEB_DEF_SESSION_TIMEOUT;
    pProperty->VhoCookieMaxAge        = WEB_DEF_VHO_COOKIE_MAX_AGE;
    pProperty->LsmCookieMaxAge        = WEB_DEF_LSM_COOKIE_MAX_AGE;
    pProperty->SessionMaxLifespan     = 0;

    rc = strcpy_s(pProperty->VhoCookieName, sizeof(pProperty->VhoCookieName), WEB_DEF_VHO_COOKIE_NAME);
    ERR_CHK(rc);
    rc = strcpy_s(pProperty->LsmCookieName, sizeof(pProperty->LsmCookieName), WEB_DEF_LSM_COOKIE_NAME);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoReset
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
WebVhoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    pMyObject->DelAllGsos((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetHttpAuth
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                ULONG                       http_auth_method,
                ANSC_HANDLE                 hWebAcmIf
            );

    description:

        This function is called to set HTTP authentication to
        the matching resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path of the resource owner.

                ULONG                       http_auth_method
                Specifies HTTP authentication method to be used.

                ANSC_HANDLE                 hWebAcmIf
                Specifies the handle to WEB ACM interface.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoSetHttpAuth
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ULONG                       http_auth_method,
        ANSC_HANDLE                 hWebAcmIf
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner   = NULL;

    pResourceOwner  = 
        (PWEB_RESOURCE_OWNER_OBJECT)pResourceLocator->GetResourceOwner1
            (
                (ANSC_HANDLE)pResourceLocator,
                path
            );

    if ( !pResourceOwner )
    {
        returnStatus    = ANSC_STATUS_FAILURE;
    }
    else
    {
        WEB_RESOURCE_OWNER_PROPERTY property;
        PWEB_AUTH_SERVER_OBJECT     pAuthServer;

        pResourceOwner->GetProperty
            (
                (ANSC_HANDLE)pResourceOwner,
                &property
            );

        if ( property.AuthType != http_auth_method )
        {
            property.AuthType   = http_auth_method;

            pResourceOwner->SetProperty
                (
                    (ANSC_HANDLE)pResourceOwner,
                    &property
                );

            /* re-engage the resource owner object */
            pResourceOwner->Cancel((ANSC_HANDLE)pResourceOwner);
            pResourceOwner->Engage((ANSC_HANDLE)pResourceOwner);

            pAuthServer = 
                (PWEB_AUTH_SERVER_OBJECT)pResourceOwner->GetAuthServer
                    (
                        (ANSC_HANDLE)pResourceOwner
                    );

            if ( pAuthServer )
            {
                pAuthServer->SetAcmIf((ANSC_HANDLE)pAuthServer, hWebAcmIf);
            }
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSetHttpAuth2
            (
                ANSC_HANDLE                 hThisObject,
		        char*						host_name,
		        USHORT						host_port,
                char*                       path,
                ANSC_HANDLE                 hWebAcmIf,
		        ULONG						http_auth_method,  
                ULONG                       alg,               
                BOOL                        bNoQop,
                ULONG                       NonceTimeout,      
                char*                       realm,             
                char*                       domain             
            );

    description:

        This function is called to set HTTP authentication to
        the matching resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path of the resource owner.

                ANSC_HANDLE                 hWebAcmIf
                Specifies the handle to WEB ACM interface.

                ULONG                       http_auth_method
                Specifies HTTP authentication method to be used.

                ULONG                       alg
                Authentication algoritm.

                BOOL                        bNoQop
                To indicate if qop is used.

                ULONG                       NonceTimeout
                Nonce timeout value, 0 means nonce never expires.

                char*                       realm
                Authentication realm.

                char*                       domain   
                Authentication domain.
                
    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoSetHttpAuth2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hWebAcmIf,
		ULONG						http_auth_method,   /* look into http_definitions.h for HTTP_AUTH_TYPE_xxx */
        ULONG                       alg,                /* one of WEB_AUTH_ALGORITHM_xxx */
        BOOL                        bNoQop,
        ULONG                       NonceTimeout,       /* nonce timeout value in seconds */
        char*                       realm,              /* realm name */
        char*                       domain              /* domain name */
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner   = NULL;
    errno_t                         rc               = -1;

    pResourceOwner  = 
        (PWEB_RESOURCE_OWNER_OBJECT)pResourceLocator->GetResourceOwner1
            (
                (ANSC_HANDLE)pResourceLocator,
                path
            );

    if ( !pResourceOwner )
    {
        returnStatus    = ANSC_STATUS_FAILURE;
    }
    else
    {
        WEB_RESOURCE_OWNER_PROPERTY property;
        PWEB_AUTH_SERVER_OBJECT     pAuthServer;

        pResourceOwner->GetProperty
            (
                (ANSC_HANDLE)pResourceOwner,
                &property
            );

        if ( property.AuthType != http_auth_method )
        {
            property.AuthType   = http_auth_method;

            pResourceOwner->SetProperty
                (
                    (ANSC_HANDLE)pResourceOwner,
                    &property
                );

            /* re-engage the resource owner object */
            pResourceOwner->Cancel((ANSC_HANDLE)pResourceOwner);
            pResourceOwner->Engage((ANSC_HANDLE)pResourceOwner);
        }

        pAuthServer = 
            (PWEB_AUTH_SERVER_OBJECT)pResourceOwner->GetAuthServer
                (
                    (ANSC_HANDLE)pResourceOwner
                );

        if ( pAuthServer )
        {
            WEB_AUTH_SERVER_PROPERTY    authProperty;
            ULONG                       ulLen;

            pAuthServer->SetAcmIf((ANSC_HANDLE)pAuthServer, hWebAcmIf);

            pAuthServer->GetProperty((ANSC_HANDLE)pAuthServer, (ANSC_HANDLE)&authProperty);

            /* set the new property */
            authProperty.AuthType           = http_auth_method;
            authProperty.Algorithm          = alg;
            authProperty.bNoQop             = bNoQop;
            authProperty.NonceTimeout       = NonceTimeout;

            authProperty.Realm[0]           = 0;
            authProperty.Domain[0]          = 0;

            if ( realm )
            {
                ulLen = AnscSizeOfString(realm);
                if ( ulLen > WEB_AUTH_REALM_NAME_MAXLEN )
                {
                    returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                }
            
                rc = strcpy_s((char *)authProperty.Realm, sizeof(authProperty.Realm), realm); /*Need to confirm*/
                ERR_CHK(rc);
            }

            if ( domain )
            {
                ulLen = AnscSizeOfString(domain);
                if ( ulLen > WEB_AUTH_REALM_NAME_MAXLEN )
                {
                    returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                }
            
                rc = strcpy_s((char *)authProperty.Domain, sizeof(authProperty.Domain), domain);
                ERR_CHK(rc);
            }

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                returnStatus = 
                    pAuthServer->SetProperty((ANSC_HANDLE)pAuthServer, (ANSC_HANDLE)&authProperty);
            }
        }
    }

    return  returnStatus;
}

