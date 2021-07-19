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

    module:	web_roo_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Resource Owner Object.

        *   WebRooGetAuthServer
        *   WebRooGetHfpIf
        *   WebRooSetHfpIf
        *   WebRooGetRcpIf
        *   WebRooGetPathName
        *   WebRooSetPathName
        *   WebRooIsDefaultOwner
        *   WebRooGetProperty
        *   WebRooSetProperty
        *   WebRooResetProperty
        *   WebRooReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "web_roo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRooGetAuthServer
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
WebRooGetAuthServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;

    return  pMyObject->hAuthServer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRooGetHfpIf
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
WebRooGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooSetHfpIf
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
WebRooSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRooGetRcpIf
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
WebRooGetRcpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;

    return  pMyObject->hRcpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooGetPathName
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
WebRooGetPathName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;

    return  pProperty->RegPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooSetPathName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooSetPathName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    rc = strcpy_s(pProperty->RegPath, sizeof(pProperty->RegPath), path);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebRooIsDefaultOwner
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
WebRooIsDefaultOwner
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;

    return  ((pProperty->OwnerFlag & WEB_ROO_FLAG_DEFAULT) != 0);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooGetProperty
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
WebRooGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;

    *(PWEB_RESOURCE_OWNER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooSetProperty
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
WebRooSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PWEB_RESOURCE_OWNER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooResetProperty
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
WebRooResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    rc = strcpy_s(pProperty->RootPath, sizeof(pProperty->RootPath), WEB_DEF_ROOT_PATH);
    ERR_CHK(rc);
    rc = strcpy_s(pProperty->RegPath, sizeof(pProperty->RegPath),  WEB_DEF_ROOT_PATH);
    ERR_CHK(rc);

    pProperty->OwnerFlag = WEB_ROO_FLAG_STANDARD;
    pProperty->OwnerType = WEB_ROO_TYPE_FILE_SYSTEM;
    pProperty->AuthType  = WEB_ROO_AUTH_TYPE_NONE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReset
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
WebRooReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
