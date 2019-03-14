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

    module:	sys_smo_rcmif.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Sys Setup Manager Object.

        *   SysSmoRcmNotify
        *   SysSmoRcmInit
        *   SysSmoRcmStop

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

        ANSC_STATUS
        SysSmoRcmNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ULONG                       ulEvent,
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

                ULONG                       ulEvent
                Specifies the type of the event just occurred.

                ANSC_HANDLE                 hReserved
                Specifies an event-specific context handler.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoRcmNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT    )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY  )&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE           )pMyObject->hSysIraIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRepFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT)hFolder;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    switch ( ulEvent )
    {
        case    SYS_RCM_EVENT_FOLDER_ADDED :

                returnStatus =
                    pMyObject->ReactRfoAdded
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pRepFolder,
                            hReserved
                        );

                break;

        case    SYS_RCM_EVENT_FOLDER_DELETED :

                returnStatus =
                    pMyObject->ReactRfoDeleted
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pRepFolder,
                            hReserved
                        );

                break;

        case    SYS_RCM_EVENT_RECORD_ADDED :

                returnStatus =
                    pMyObject->ReactRroAdded
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pRepFolder,
                            hReserved
                        );

                break;

        case    SYS_RCM_EVENT_RECORD_MODIFIED :

                returnStatus =
                    pMyObject->ReactRroModified
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pRepFolder,
                            hReserved
                        );

                break;

        case    SYS_RCM_EVENT_RECORD_DELETED :

                returnStatus =
                    pMyObject->ReactRroDeleted
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pRepFolder,
                            hReserved
                        );

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoRcmInit
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called by the System Repository Folder Object
        when the folder is opened for the first time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder that has been just
                opened by third-party component.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoRcmInit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT    )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY  )&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE           )pMyObject->hSysIraIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRepFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT)hFolder;
    PSYS_RFO_KEY                    pRfoKeySetUp = (PSYS_RFO_KEY                 )NULL;
    ULONG                           ulPermission = pRepFolder->GetPermission((ANSC_HANDLE)pRepFolder);

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    /*
     * We need to decide which interface to use to access the repository: the standard IRA or the
     * Repository Folder Object directly? Before making the decision, we have to find out why the
     * associated folder context parameter is not a folder access-key, but folder object itself.
     * That is because the notification and internal synchronization are handled by the folder
     * object, not the System Information Repository Object, which implements the IRA interface.
     */
    pRfoKeySetUp = (PSYS_RFO_KEY)AnscAllocateMemory(sizeof(SYS_RFO_KEY));

    if ( !pRfoKeySetUp )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pRfoKeySetUp->AccessMode = SYS_RFO_PERMISSION_ALL;
        pRfoKeySetUp->hRepFolder = (ANSC_HANDLE)pRepFolder;
    }

    /*
     * We have to be careful about how to access the associated folder object: we CANNOT explicitly
     * open/close the folder again because it will cause confusion to the internal synchronization
     * mechanism. Furthermore, we shall be ensure the consistency of the folder's permission. Same
     * permission must remain after content manipulation. Following are the procedures for
     * initializing the folder access:
     *
     *      (1) Modify folder permission to allow content manipulation.
     *      (2) Clean up the existing folder content.
     *      (3) Create a sub-folder for each "setup category"
     *      (4) Set up parameters for each sub-folder.
     *      (5) Modify folder permission back to the original value.
     */
    returnStatus =
        pSysIraIf->SetRfoPermission
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)pRfoKeySetUp,
                SYS_RFO_PERMISSION_ALL
            );

    if ( TRUE )
    {
        returnStatus =
            pSysIraIf->ClearFolder
                (
                    pSysIraIf->hOwnerContext,
                    (ANSC_HANDLE)pRfoKeySetUp
                );
    }

    returnStatus =
        pMyObject->InitSetUpAccess
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pRfoKeySetUp
            );

    /*
     * We have changed the folder permission at the root setup level. Don't forget to change it
     * back; otherwise, the consistency of the folder content may be compromised.
     */
    returnStatus =
        pSysIraIf->SetRfoPermission
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)pRfoKeySetUp,
                ulPermission
            );

    if ( pRfoKeySetUp )
    {
        AnscFreeMemory(pRfoKeySetUp);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoRcmStop
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called by the System Repository Folder Object
        when the last user finishes access to the folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the repository folder whose access is to be
                stopped and synchronized.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoRcmStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT    )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY  )&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE           )pMyObject->hSysIraIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRepFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT)hFolder;
    PSYS_RFO_KEY                    pRfoKeySetUp = (PSYS_RFO_KEY                 )NULL;
    ULONG                           ulPermission = pRepFolder->GetPermission((ANSC_HANDLE)pRepFolder);

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    /*
     * We need to decide which interface to use to access the repository: the standard IRA or the
     * Repository Folder Object directly? Before making the decision, we have to find out why the
     * associated folder context parameter is not a folder access-key, but folder object itself.
     * That is because the notification and internal synchronization are handled by the folder
     * object, not the System Information Repository Object, which implements the IRA interface.
     */
    pRfoKeySetUp = (PSYS_RFO_KEY)AnscAllocateMemory(sizeof(SYS_RFO_KEY));

    if ( !pRfoKeySetUp )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pRfoKeySetUp->AccessMode = SYS_RFO_PERMISSION_ALL;
        pRfoKeySetUp->hRepFolder = (ANSC_HANDLE)pRepFolder;
    }

    /*
     * We have to be careful about how to access the associated folder object: we CANNOT explicitly
     * open/close the folder again because it will cause confusion to the internal synchronization
     * mechanism. Furthermore, we shall be ensure the consistency of the folder's permission. Same
     * permission must remain after content manipulation. Following are the procedures for
     * initializing the folder access:
     *
     *      (1) Modify folder permission to allow content manipulation.
     *      (2) Clean up the existing folder content.
     *      (3) Create a sub-folder for each "setup category"
     *      (4) Set up parameters for each sub-folder.
     *      (5) Modify folder permission back to the original value.
     */
    returnStatus =
        pSysIraIf->SetRfoPermission
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)pRfoKeySetUp,
                SYS_RFO_PERMISSION_ALL
            );

    returnStatus =
        pMyObject->StopSetUpAccess
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pRfoKeySetUp
            );

    /*
     * We have changed the folder permission at the root setup level. Don't forget to change it
     * back; otherwise, the consistency of the folder content may be compromised.
     */
    returnStatus =
        pSysIraIf->SetRfoPermission
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)pRfoKeySetUp,
                ulPermission
            );

    if ( pRfoKeySetUp )
    {
        AnscFreeMemory(pRfoKeySetUp);
    }

    return  returnStatus;
}
