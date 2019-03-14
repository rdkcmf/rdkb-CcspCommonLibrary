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

    module:	sys_rro_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Repository
        Record Object.

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


#ifndef  _SYS_RRO_INTERNAL_API_
#define  _SYS_RRO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RRO_STATES.C
***********************************************************/

ULONG
SysRroGetBirthTime
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetBirthTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

ANSC_HANDLE
SysRroGetRenderAttr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysRroGetParentFolder
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetParentFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_HANDLE
SysRroGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ULONG
SysRroGetUserReserved
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    );

char*
SysRroGetRecordName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetRecordName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ULONG
SysRroGetRecordType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

ULONG
SysRroGetPermission
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       permission
    );

ULONG
SysRroGetContentType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RRO_ACCESS.C
***********************************************************/

BOOL
SysRroGrantAccess
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

ULONG
SysRroGetRecordSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRroGetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        PULONG                      pulDataSize
    );

ANSC_STATUS
SysRroSetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        ULONG                       ulDataSize
    );


#endif
