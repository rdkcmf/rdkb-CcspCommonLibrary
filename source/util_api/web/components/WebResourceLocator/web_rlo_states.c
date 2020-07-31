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

    module:	web_rlo_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Resource Locator Object.

        *   WebRloGetRootUriPath
        *   WebRloGetProperty
        *   WebRloSetProperty
        *   WebRloResetProperty
        *   WebRloReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.

**********************************************************************/


#include "web_rlo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRloGetRootUriPath
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
WebRloGetRootUriPath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;

    return  pMyObject->hRootUriPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloGetProperty
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
WebRloGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_RESOURCE_LOCATOR_PROPERTY  pProperty    = (PWEB_RESOURCE_LOCATOR_PROPERTY)&pMyObject->Property;

    *(PWEB_RESOURCE_LOCATOR_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloSetProperty
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
WebRloSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_RESOURCE_LOCATOR_PROPERTY  pProperty    = (PWEB_RESOURCE_LOCATOR_PROPERTY)&pMyObject->Property;

    *pProperty = *(PWEB_RESOURCE_LOCATOR_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloResetProperty
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
WebRloResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_RESOURCE_LOCATOR_PROPERTY  pProperty    = (PWEB_RESOURCE_LOCATOR_PROPERTY)&pMyObject->Property;

    pProperty->MaxPathLevel = WEB_MAX_PATH_LEVEL;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloReset
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
WebRloReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath = (PWEB_URI_PATH_OBJECT          )pMyObject->hRootUriPath;

    pRootUriPath->Reset((ANSC_HANDLE)pRootUriPath);

    return  ANSC_STATUS_SUCCESS;
}
