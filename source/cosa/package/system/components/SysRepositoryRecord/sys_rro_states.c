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

    module:	sys_rro_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Repository Record Object.

        *   SysRroGetBirthTime
        *   SysRroSetBirthTime
        *   SysRroGetRenderAttr
        *   SysRroSetRenderAttr
        *   SysRroGetParentFolder
        *   SysRroSetParentFolder
        *   SysRroGetUserContext
        *   SysRroSetUserContext
        *   SysRroGetUserReserved
        *   SysRroSetUserReserved
        *   SysRroGetRecordName
        *   SysRroSetRecordName
        *   SysRroGetRecordType
        *   SysRroSetRecordType
        *   SysRroGetPermission
        *   SysRroSetPermission
        *   SysRroGetContentType
        *   SysRroSetContentType

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/07/02    initial revision.

**********************************************************************/


#include "sys_rro_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetBirthTime
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
SysRroGetBirthTime
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->BirthTime;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetBirthTime
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
SysRroSetBirthTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    pMyObject->BirthTime = ulTime;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRroGetRenderAttr
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
SysRroGetRenderAttr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->hRenderAttr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetRenderAttr
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
SysRroSetRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRenderAttr
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject      = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;
    PSYS_RRO_RENDER_ATTR            pDstRenderAttr = (PSYS_RRO_RENDER_ATTR         )pMyObject->hRenderAttr;
    PSYS_RRO_RENDER_ATTR            pSrcRenderAttr = (PSYS_RRO_RENDER_ATTR         )hRenderAttr;

    if ( pDstRenderAttr )
    {
        SysFreeRroRenderAttr(pDstRenderAttr);
    }

    if ( !pSrcRenderAttr )
    {
        pMyObject->hRenderAttr = (ANSC_HANDLE)NULL;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pDstRenderAttr = (PSYS_RRO_RENDER_ATTR)SysAllocateRroRenderAttr();
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

    SysRroCopyRenderAttr(pDstRenderAttr, pSrcRenderAttr);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRroGetParentFolder
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
SysRroGetParentFolder
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->hParentFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetParentFolder
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
SysRroSetParentFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    pMyObject->hParentFolder = hFolder;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRroGetUserContext
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
SysRroGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetUserContext
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
SysRroSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    pMyObject->hUserContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetUserReserved
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
SysRroGetUserReserved
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->UserReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetUserReserved
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
SysRroSetUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    pMyObject->UserReserved = reserved;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SysRroGetRecordName
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
SysRroGetRecordName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->RecordName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetRecordName
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
SysRroSetRecordName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;
    errno_t    rc = -1;

    if ( pMyObject->RecordName )
    {
        AnscFreeMemory(pMyObject->RecordName);

        pMyObject->RecordName = NULL;
    }

    if ( name )
    {
        pMyObject->RecordName = (char*)AnscAllocateMemory(AnscSizeOfString(name) + 1);
    }
    else
    {
        pMyObject->RecordName = (char*)AnscAllocateMemory(1);
    }

    if ( pMyObject->RecordName && name )
    {
        rc = STRCPY_S_NOCLOBBER(pMyObject->RecordName, (AnscSizeOfString(name) + 1), name);
        ERR_CHK(rc);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetRecordType
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
SysRroGetRecordType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->RecordType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetRecordType
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
SysRroSetRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulType);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetPermission
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
SysRroGetPermission
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->Permission;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetPermission
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
SysRroSetPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       permission
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    pMyObject->Permission = permission;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetContentType
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
SysRroGetContentType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;
    PSYS_RRO_RENDER_ATTR            pRenderAttr  = (PSYS_RRO_RENDER_ATTR         )pMyObject->hRenderAttr;

    if ( pRenderAttr )
    {
        return  pRenderAttr->ContentType;
    }

    return  SYS_RECORD_CONTENT_DEFAULT;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetContentType
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
SysRroSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulType);
    return  ANSC_STATUS_UNAPPLICABLE;
}
