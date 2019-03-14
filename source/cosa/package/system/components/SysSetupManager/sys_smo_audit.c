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

    module:	sys_smo_audit.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Setup Manager Object.

        *   SysSmoReactRfoAdded
        *   SysSmoReactRfoDeleted
        *   SysSmoReactRroAdded
        *   SysSmoReactRroModified
        *   SysSmoReactRroDeleted

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/15/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReactRfoAdded
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the System Repository Folder Object
        when somethings happens to the associated folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that is associated with
                the indicated event.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoReactRfoAdded
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReactRfoDeleted
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the System Repository Folder Object
        when somethings happens to the associated folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that is associated with
                the indicated event.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoReactRfoDeleted
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReactRroAdded
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the System Repository Folder Object
        when somethings happens to the associated folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that is associated with
                the indicated event.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoReactRroAdded
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReactRroModified
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the System Repository Folder Object
        when somethings happens to the associated folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that is associated with
                the indicated event.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoReactRroModified
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoReactRroDeleted
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the System Repository Folder Object
        when somethings happens to the associated folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that is associated with
                the indicated event.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoReactRroDeleted
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}
