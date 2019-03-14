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

    module:	sys_smo_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Setup Manager Object.

        *   SysSmoGetSetupRfo
        *   SysSmoGetSysRcmIf
        *   SysSmoResetProperty
        *   SysSmoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/14/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysSmoGetSetupRfo
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
SysSmoGetSetupRfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;

    return  pMyObject->hSetupRfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysSmoGetSysRcmIf
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
SysSmoGetSysRcmIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;

    return  pMyObject->hSysRcmIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoResetProperty
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
SysSmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;

    SysMcoResetProperty((ANSC_HANDLE)pMyObject);

    pProperty->ModuleType = SYS_MODULE_TYPE_ACCESSORY;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReset
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
SysSmoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;

    SysMcoReset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
