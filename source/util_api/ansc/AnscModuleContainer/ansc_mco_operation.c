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

    module:	ansc_mco_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Module Container Object.

        *   AnscMcoSubEngage
        *   AnscMcoSubCancel
        *   AnscMcoIgnite
        *   AnscMcoPutOut
        *   AnscMcoSwitch

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
        AnscMcoSubEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscMcoSubEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_MODULE_CONTAINER_OBJECT   pMyObject      = (PANSC_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_QUERY_INTERFACE_OBJECT    pQueryIfObject = (PANSC_QUERY_INTERFACE_OBJECT )pMyObject->hQueryIfObject;

    if ( !pQueryIfObject )
    {
        pQueryIfObject =
            (PANSC_QUERY_INTERFACE_OBJECT)AnscCreateQueryInterface
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pQueryIfObject )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hQueryIfObject = (ANSC_HANDLE)pQueryIfObject;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscMcoSubCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscMcoSubCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_MODULE_CONTAINER_OBJECT   pMyObject      = (PANSC_MODULE_CONTAINER_OBJECT)hThisObject;
    PANSC_QUERY_INTERFACE_OBJECT    pQueryIfObject = (PANSC_QUERY_INTERFACE_OBJECT )pMyObject->hQueryIfObject;

    if ( pQueryIfObject )
    {
        pQueryIfObject->Remove((ANSC_HANDLE)pQueryIfObject);

        pMyObject->hQueryIfObject = (ANSC_HANDLE)NULL;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscMcoIgnite
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
AnscMcoIgnite
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
        AnscMcoPutOut
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
AnscMcoPutOut
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
        AnscMcoSwitch
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
AnscMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(bSwitchOn);
    return  ANSC_STATUS_SUCCESS;
}
