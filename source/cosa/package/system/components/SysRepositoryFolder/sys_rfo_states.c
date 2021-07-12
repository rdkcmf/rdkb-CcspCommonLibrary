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

    module:	sys_rfo_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Repository Folder Object.

        *   SysRfoGetBirthTime
        *   SysRfoSetBirthTime
        *   SysRfoGetRenderAttr
        *   SysRfoSetRenderAttr
        *   SysRfoGetRecycleBin
        *   SysRfoSetRecycleBin
        *   SysRfoGetParentFolder
        *   SysRfoSetParentFolder
        *   SysRfoGetUserContext
        *   SysRfoSetUserContext
        *   SysRfoGetUserReserved
        *   SysRfoSetUserReserved
        *   SysRfoGetRcaIf
        *   SysRfoSetRcaIf
        *   SysRfoGetRcgIf
        *   SysRfoSetRcgIf
        *   SysRfoGetRcmIf
        *   SysRfoSetRcmIf
        *   SysRfoGetFolderName
        *   SysRfoSetFolderName
        *   SysRfoGetFolderType
        *   SysRfoSetFolderType
        *   SysRfoGetPermission
        *   SysRfoSetPermission
        *   SysRfoGetContentType
        *   SysRfoSetContentType
        *   SysRfoGetProperty
        *   SysRfoSetProperty
        *   SysRfoResetProperty
        *   SysRfoReset

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetBirthTime
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

ULONG
SysRfoGetBirthTime
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->BirthTime;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetBirthTime
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTime
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTime
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetBirthTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->BirthTime = ulTime;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRenderAttr
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
SysRfoGetRenderAttr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hRenderAttr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetRenderAttr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRenderAttr
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRenderAttr
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRenderAttr
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject      = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RFO_RENDER_ATTR            pDstRenderAttr = (PSYS_RFO_RENDER_ATTR           )pMyObject->hRenderAttr;
    PSYS_RFO_RENDER_ATTR            pSrcRenderAttr = (PSYS_RFO_RENDER_ATTR           )hRenderAttr;

    if ( pDstRenderAttr )
    {
        SysFreeRfoRenderAttr(pDstRenderAttr);
    }

    if ( !pSrcRenderAttr )
    {
        pMyObject->hRenderAttr = (ANSC_HANDLE)NULL;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pDstRenderAttr = (PSYS_RFO_RENDER_ATTR)SysAllocateRfoRenderAttr();
    }

    if ( !pDstRenderAttr )
    {
        pMyObject->hRenderAttr = (ANSC_HANDLE)NULL;

        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hRenderAttr = (ANSC_HANDLE)pDstRenderAttr;
    }

    SysRfoCopyRenderAttr(pDstRenderAttr, pSrcRenderAttr);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRecycleBin
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
SysRfoGetRecycleBin
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hRecycleBin;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetRecycleBin
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecycleBin
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecycleBin
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecycleBin
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hRecycleBin = hRecycleBin;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetParentFolder
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
SysRfoGetParentFolder
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hParentFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetParentFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetParentFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hParentFolder = hFolder;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetUserContext
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
SysRfoGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetUserContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hUserContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetUserReserved
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

ULONG
SysRfoGetUserReserved
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->UserReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetUserReserved
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       reserved
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       reserved
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->UserReserved = reserved;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRcaIf
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
SysRfoGetRcaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hOpenRcaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetRcaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRcaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hOpenRcaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRcgIf
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
SysRfoGetRcgIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hOpenRcgIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetRcgIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRcgIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hOpenRcgIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRcmIf
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
SysRfoGetRcmIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    return  pMyObject->hOpenRcmIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetRcmIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRcmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->hOpenRcmIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SysRfoGetFolderName
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

char*
SysRfoGetFolderName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    return  pProperty->FolderName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetFolderName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetFolderName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    errno_t   rc = -1;

    AnscZeroMemory(pProperty->FolderName, SYS_MAX_FOLDER_NAME_SIZE);
    rc = STRCPY_S_NOCLOBBER(pProperty->FolderName, sizeof(pProperty->FolderName), name  );
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetFolderType
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

ULONG
SysRfoGetFolderType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    return  pProperty->FolderType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetFolderType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulType
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulType
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    pProperty->FolderType = ulType;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetPermission
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

ULONG
SysRfoGetPermission
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    return  pProperty->Permission;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetPermission
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       permission
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       permission
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       permission
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    pProperty->Permission = permission;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetContentType
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

ULONG
SysRfoGetContentType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RFO_RENDER_ATTR            pRenderAttr  = (PSYS_RFO_RENDER_ATTR           )pMyObject->hRenderAttr;

    if ( pRenderAttr )
    {
        return  pRenderAttr->ContentType;
    }

    return  SYS_FOLDER_CONTENT_DEFAULT;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetContentType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulType
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulType
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    )
{
    UNREFERENCED_PARAMETER(ulType);
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoGetProperty
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
SysRfoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    *(PSYS_REPOSITORY_FOLDER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSetProperty
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
SysRfoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_REPOSITORY_FOLDER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoResetProperty
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
SysRfoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;

    pProperty->FolderType = SYS_REP_FOLDER_TYPE_STORAGE;
    pProperty->Permission = SYS_RFO_PERMISSION_ALL;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoReset
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
SysRfoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    pMyObject->DelAllFolders((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllRecords((ANSC_HANDLE)pMyObject);

    if ( pMyObject->FolderArray != NULL )
    {
        AnscFreeMemory(pMyObject->FolderArray);

        pMyObject->FolderSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
        pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
        pMyObject->FolderArray       = NULL;
        pMyObject->FolderArrayMaxLen = SYS_RFO_DEF_ARRAY_SIZE;
        pMyObject->FolderArrayCurLen = 0;
    }

    if ( pMyObject->RecordArray != NULL )
    {
        AnscFreeMemory(pMyObject->RecordArray);

        pMyObject->RecordSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
        pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
        pMyObject->RecordArray       = NULL;
        pMyObject->RecordArrayMaxLen = SYS_RFO_DEF_ARRAY_SIZE;
        pMyObject->RecordArrayCurLen = 0;
    }

    if ( pMyObject->AbsolutePath )
    {
        AnscFreeMemory(pMyObject->AbsolutePath);

        pMyObject->AbsolutePath = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
