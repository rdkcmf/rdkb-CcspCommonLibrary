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

    module:	ansc_mco_query.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced if-query functions
        of the Ansc Module Container Object.

        *   AnscMcoAddModuleIf
        *   AnscMcoDelModuleIf
        *   AnscMcoQueryModuleIf

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/12/02    initial revision.

**********************************************************************/


#include "ansc_mco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscMcoAddModuleIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to add an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be added.

                ANSC_HANDLE                 hInterface
                Specifies the interface context to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscMcoAddModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_MODULE_CONTAINER_OBJECT   pMyObject      = (PANSC_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_QUERY_INTERFACE_OBJECT    pQueryIfObject = (PANSC_QUERY_INTERFACE_OBJECT )pMyObject->hQueryIfObject;

    returnStatus =
        pQueryIfObject->AddIf
            (
                (ANSC_HANDLE)pQueryIfObject,
                pIfName,
                hInterface
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscMcoDelModuleIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName
            );

    description:

        This function is called to delete an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscMcoDelModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_MODULE_CONTAINER_OBJECT   pMyObject      = (PANSC_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_QUERY_INTERFACE_OBJECT    pQueryIfObject = (PANSC_QUERY_INTERFACE_OBJECT )pMyObject->hQueryIfObject;

    returnStatus =
        pQueryIfObject->DelIf
            (
                (ANSC_HANDLE)pQueryIfObject,
                pIfName
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscMcoQueryModuleIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName
            );

    description:

        This function is called to retrieve an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be matched.

    return:     interface context.

**********************************************************************/

ANSC_HANDLE
AnscMcoQueryModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    )
{
    PANSC_MODULE_CONTAINER_OBJECT   pMyObject      = (PANSC_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_QUERY_INTERFACE_OBJECT    pQueryIfObject = (PANSC_QUERY_INTERFACE_OBJECT )pMyObject->hQueryIfObject;
    ANSC_HANDLE                     hInterface     = (ANSC_HANDLE                  )NULL;

    hInterface =
        pQueryIfObject->QueryIf
            (
                (ANSC_HANDLE)pQueryIfObject,
                pIfName
            );

    return  hInterface;
}
