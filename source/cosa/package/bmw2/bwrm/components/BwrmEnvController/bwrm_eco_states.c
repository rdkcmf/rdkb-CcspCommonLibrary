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

    module:	bwrm_eco_states.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bwrm Env Controller Object.

        *   BwrmEcoGetBwrmRamIf
        *   BwrmEcoGetBwrmFileManager
        *   BwrmEcoGetBwrmPageManager
        *   BwrmEcoGetProperty
        *   BwrmEcoSetProperty
        *   BwrmEcoResetProperty
        *   BwrmEcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#include "bwrm_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmEcoGetBwrmRamIf
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
BwrmEcoGetBwrmRamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBwrmRamIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmEcoGetBwrmFileManager
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
BwrmEcoGetBwrmFileManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBwrmFileManager;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmEcoGetBwrmPageManager
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
BwrmEcoGetBwrmPageManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBwrmPageManager;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoGetProperty
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
BwrmEcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_ENV_CONTROLLER_PROPERTY   pProperty    = (PBWRM_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *(PBWRM_ENV_CONTROLLER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoSetProperty
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
BwrmEcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_ENV_CONTROLLER_PROPERTY   pProperty    = (PBWRM_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PBWRM_ENV_CONTROLLER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoResetProperty
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
BwrmEcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_ENV_CONTROLLER_PROPERTY   pProperty    = (PBWRM_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(BWRM_ENV_CONTROLLER_PROPERTY));

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoReset
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
BwrmEcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
