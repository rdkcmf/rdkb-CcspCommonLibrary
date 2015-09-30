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

    module:	web_mco_control.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Web Module Container Object.

        *   WebMcoRegisterIfs
        *   WebMcoDispatchIfs

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
        WebMcoRegisterIfs
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to register all the runtime interfaces.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoRegisterIfs
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;
    PWEB_SIMPLE_SERVER_OBJECT       pWebSso      = (PWEB_SIMPLE_SERVER_OBJECT   )pMyObject->hWebSso;
    PWEB_SITE_MANAGER_OBJECT        pWebSmo      = (PWEB_SITE_MANAGER_OBJECT    )pWebSso->GetSiteManager((ANSC_HANDLE)pWebSso);
    PWEB_CSP_INTERFACE              pWebCspIf    = (PWEB_CSP_INTERFACE          )pWebSso->GetCspIf      ((ANSC_HANDLE)pWebSso);
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE          )pWebSmo->GetHemIf      ((ANSC_HANDLE)pWebSmo);

    returnStatus =
        pMyObject->AddModuleIf
            (
                (ANSC_HANDLE)pMyObject,
                pWebCspIf->Name,
                (ANSC_HANDLE)pWebCspIf
            );

    returnStatus =
        pMyObject->AddModuleIf
            (
                (ANSC_HANDLE)pMyObject,
                pWebHemIf->Name,
                (ANSC_HANDLE)pWebHemIf
            );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoDispatchIfs
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hIqcIf
            );

    description:

        This function is called to dispatch all the interface objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hIqcIf
                Specifies the Interface Query Controller Interface.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoDispatchIfs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;
    PWEB_SIMPLE_SERVER_OBJECT       pWebSso      = (PWEB_SIMPLE_SERVER_OBJECT   )pMyObject->hWebSso;
    PANSC_IQC_INTERFACE             pIqcIf       = (PANSC_IQC_INTERFACE         )hIqcIf;

    return  ANSC_STATUS_SUCCESS;
}
