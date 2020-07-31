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

    module:	web_mco_operation.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web Module Container Object.

        *   WebMcoIgnite
        *   WebMcoPutOut
        *   WebMcoSwitch

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
        WebMcoIgnite
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage operations for encapsulated
        component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoIgnite
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;

    if ( TRUE )
    {
        pMyObject->RegisterIfs((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoPutOut
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel operations for encapsulated
        component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoPutOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoSwitch
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bSwitchOn
            );

    description:

        This function is called to turn on/off the module as a whole.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bSwitchOn
                Specifies whether to turn on or off the module.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;
    PWEB_SIMPLE_SERVER_OBJECT       pWebSso      = (PWEB_SIMPLE_SERVER_OBJECT   )pMyObject->hWebSso;

    if ( bSwitchOn )
    {
        pWebSso->Engage((ANSC_HANDLE)pWebSso);
    }
    else
    {
        pWebSso->Cancel((ANSC_HANDLE)pWebSso);
    }

    return  ANSC_STATUS_SUCCESS;
}
