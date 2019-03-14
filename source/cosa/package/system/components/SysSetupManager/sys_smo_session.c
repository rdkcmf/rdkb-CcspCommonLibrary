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

    module:	sys_smo_session.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Setup Manager Object.

        *   SysSmoInitSetUpAccess
        *   SysSmoStopSetUpAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/20/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoInitSetUpAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hAccessKey
            );

    description:

        This function is called by the System Repository Folder Object
        when the access to the folder is initialized.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hAccessKey
                Specifies the repository folder to which the access is
                to be initialized.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoInitSetUpAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAccessKey
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
        SysSmoStopSetUpAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hAccessKey
            );

    description:

        This function is called by the System Repository Folder Object
        when the access to the folder is stopped.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hAccessKey
                Specifies the repository folder to which the access is
                to be stopped.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoStopSetUpAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAccessKey
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}
