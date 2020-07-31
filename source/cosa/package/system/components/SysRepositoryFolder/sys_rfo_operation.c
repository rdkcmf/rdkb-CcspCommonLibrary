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

    module:	sys_rfo_operation.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Repository Folder Object.

        *   SysRfoAcquireAccess
        *   SysRfoReleaseAccess
        *   SysRfoEngage
        *   SysRfoCancel
        *   SysRfoDelete
        *   SysRfoIssueKey
        *   SysRfoClearKey

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/06/02    initial revision.

**********************************************************************/


#include "sys_rfo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to synchronize object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to synchronize object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelete
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete this folder object if it's
        empty and has no external reference.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelete
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    if ( pMyObject->RefCount > 0 )
    {
        pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

        return  ANSC_STATUS_ACCESS_DENIED;
    }

    pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);
    pMyObject->Remove       ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoIssueKey
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to acquire a folder-access key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulAccessMode
                Specifies the access mode for this reference.

    return:     access key.

**********************************************************************/

ANSC_HANDLE
SysRfoIssueKey
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RFO_RENDER_ATTR            pRenderAttr  = (PSYS_RFO_RENDER_ATTR           )pMyObject->hRenderAttr;
    PSYS_RFO_KEY                    pAccessKey   = NULL;

    if ( (ulAccessMode & SYS_RFO_ACCESS_MODE_ADAPTIVE) != 0 )
    {
        ulAccessMode &= 0xFFFF0000;
        ulAccessMode |= pProperty->Permission;
    }

    if ( !pMyObject->GrantAccess
            (
                (ANSC_HANDLE)pMyObject,
                ulAccessMode
            ) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pAccessKey = (PSYS_RFO_KEY)AnscAllocateMemory(sizeof(SYS_RFO_KEY));

        if ( !pAccessKey )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pAccessKey->Permission  = pProperty->Permission;
            pAccessKey->FolderType  = pProperty->FolderType;
            pAccessKey->ContentType = SYS_FOLDER_CONTENT_DEFAULT;
            pAccessKey->AccessMode  = ulAccessMode;
            pAccessKey->hRepFolder  = (ANSC_HANDLE)pMyObject;
            pAccessKey->CloseFlags  = 0;

            if ( pRenderAttr )
            {
                pAccessKey->ContentType = SysRfoRenderAttrGetContentType(pRenderAttr);
            }
        }
    }

    pMyObject->RefCount++;

    /*
     * If this is the first attempt to access that this folder since the last user returned his
     * access-key, we synchornize the folder content if it's dynamic. Since this function is called
     * immediately after a new folder is added, the Repository Content Generator Object associated
     * with a dynamic folder MUST be prepared to synchornize the content during a create_folder()
     * system call.
     */
    if ( pMyObject->RefCount == 1 )
    {
        pMyObject->OpenFirst((ANSC_HANDLE)pMyObject);
    }

    return  (ANSC_HANDLE)pAccessKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoClearKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hKey
            );

    description:

        This function is called to return a folder-access key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hKey
                Specifies the access key issued by issue_key().

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoClearKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pAccessKey   = (PSYS_RFO_KEY                   )hKey;

    if ( pAccessKey->hRepFolder != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pMyObject->RefCount--;

    /*
     * If this is the last access-key that has been issued, we SHALL let the associated Repository
     * Auditor Object examine the content and make amendments if needed. The Auditor MAY refuse the
     * changes that have been made during the last opening period, hence the content MAY be changed
     * back. This behavior may cause confusion to users who access the repository content via some
     * sort of graphical interface.
     */
    if ( pMyObject->RefCount == 0 )
    {
            pMyObject->CloseLast
                (
                    (ANSC_HANDLE)pMyObject,
                    pAccessKey->CloseFlags
                );
    }

    AnscFreeMemory(pAccessKey);

    return  ANSC_STATUS_SUCCESS;
}
