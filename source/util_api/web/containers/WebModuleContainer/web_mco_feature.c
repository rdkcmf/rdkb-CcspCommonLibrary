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

    module:	web_mco_feature.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web Module Container Object.

        *   WebMcoEnrollFeatureObjects
        *   WebMcoManufactureFeatureObjects
        *   WebMcoDestroyFeatureObjects

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/16/02    initial revision.

**********************************************************************/


#include "web_mco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoEnrollFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enroll all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject             = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT  )pMyObject->hInternalObjectMapper;


    /******************************************************************
                  REGISTER ALL THE DEFAULT FEATURE OBJECTS
    ******************************************************************/

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Service Provider Object is such
     * an object.
     */
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                WEB_SIMPLE_SERVER_NAME,
                WEB_SIMPLE_SERVER_OID,
                ANSC_OBJECT_NAME_NULL,
                ANSC_OBJECT_OID_NULL,
                WEB_SIMPLE_SERVER_TYPE,
                ANSC_OBJECT_OID_NULL,
                WebSsoCreate,
                WebSsoRemove
            );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoManufactureFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hHttpHco;
    PWEB_SIMPLE_SERVER_OBJECT       pWebSso      = (PWEB_SIMPLE_SERVER_OBJECT    )pMyObject->hWebSso;

    if ( !pHttpHco )
    {
        pHttpHco =
            (PHTTP_HELPER_CONTAINER_OBJECT)HttpCreateHelperContainer
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pHttpHco )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hHttpHco = (ANSC_HANDLE)pHttpHco;
        }

        pHttpHco->Engage((ANSC_HANDLE)pHttpHco);
    }

    if ( !pWebSso )
    {
        pWebSso =
            (PWEB_SIMPLE_SERVER_OBJECT)WebCreateSimpleServer
                (
                    (ANSC_HANDLE)pHttpHco,
                    (ANSC_HANDLE)pHttpHco,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebSso )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hWebSso = (ANSC_HANDLE)pWebSso;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoDestroyFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hHttpHco;
    PWEB_SIMPLE_SERVER_OBJECT       pWebSso      = (PWEB_SIMPLE_SERVER_OBJECT    )pMyObject->hWebSso;

    if ( pWebSso )
    {
        pWebSso->Remove((ANSC_HANDLE)pWebSso);

        pMyObject->hWebSso = (ANSC_HANDLE)NULL;
    }

    if ( pHttpHco )
    {
        pHttpHco->Cancel((ANSC_HANDLE)pHttpHco);
        pHttpHco->Remove((ANSC_HANDLE)pHttpHco);

        pMyObject->hHttpHco = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
