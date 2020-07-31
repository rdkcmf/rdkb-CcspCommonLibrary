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

    module:	sys_rdo_operation.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Repository Driver Object.

        *   SysRdoEngage
        *   SysRdoCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/05/02    initial revision.

**********************************************************************/


#include "sys_rdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoEngage
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
SysRdoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_PROPERTY pProperty    = (PSYS_REPOSITORY_DRIVER_PROPERTY)&pMyObject->Property;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pRootFolder->HashIndex = 0;
    pRootFolder->Timestamp = AnscGetTickInSeconds();

    pRootFolder->SetFolderName  ((ANSC_HANDLE)pRootFolder, pProperty->DriverName      );
    pRootFolder->SetFolderType  ((ANSC_HANDLE)pRootFolder, SYS_REP_FOLDER_TYPE_STORAGE);
    pRootFolder->SetPermission  ((ANSC_HANDLE)pRootFolder, SYS_RFO_PERMISSION_ALL     );
    pRootFolder->SetParentFolder((ANSC_HANDLE)pRootFolder, (ANSC_HANDLE)NULL          );
    pRootFolder->Engage         ((ANSC_HANDLE)pRootFolder);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoCancel
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
SysRdoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pRootFolder->Cancel((ANSC_HANDLE)pRootFolder);

    return  ANSC_STATUS_SUCCESS;
}
