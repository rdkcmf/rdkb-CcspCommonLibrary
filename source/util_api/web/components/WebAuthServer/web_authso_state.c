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

    module:	web_authso_state.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Auth Server Object.

        *   WebAuthsoReset
        *   WebAuthsoResetProperty
        *   WebAuthsoGetProperty
        *   WebAuthsoSetProperty

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/12/05    initial revision.

**********************************************************************/


#include "web_authso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoSetAcmIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            )

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebAuthsoSetAcmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;

    pMyObject->hAcmIf   = hInterface;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoReset
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

ANSC_STATUS
WebAuthsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;

    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);
    HttpAuthoReset((ANSC_HANDLE)pMyObject);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoResetProperty
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

ANSC_STATUS
WebAuthsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;
    PWEB_AUTH_SERVER_PROPERTY       pProperty    = &pMyObject->Property;
    errno_t                         rc           = -1;

    AnscZeroMemory(pProperty, sizeof(WEB_AUTH_SERVER_PROPERTY));

    pProperty->AuthType         = HTTP_AUTH_TYPE_DIGEST;
    pProperty->Algorithm        = WEB_AUTH_ALGORITHM_MD5;
    pProperty->bNoQop           = FALSE;
    pProperty->NonceTimeout     = WEB_AUTH_NONCE_TIMEOUT_INTERVAL;
    
    rc = strcpy_s((char*)pProperty->Realm, sizeof(pProperty->Realm), "All Protected");
    ERR_CHK(rc);
    rc = strcpy_s((char*)pProperty->Domain, sizeof(pProperty->Domain), "/");
    ERR_CHK(rc);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebAuthsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;
    PWEB_AUTH_SERVER_PROPERTY       pProperty    = &pMyObject->Property;

    *((PWEB_AUTH_SERVER_PROPERTY)hProperty) = *pProperty;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebAuthsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;
    PWEB_AUTH_SERVER_PROPERTY       pProperty    = &pMyObject->Property;

    *pProperty  = *((PWEB_AUTH_SERVER_PROPERTY)hProperty);

    return ANSC_STATUS_SUCCESS;
}

