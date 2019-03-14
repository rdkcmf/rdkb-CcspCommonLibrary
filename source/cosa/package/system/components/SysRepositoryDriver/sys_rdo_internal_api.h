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

    module:	sys_rdo_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Repository
        Driver Object.

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


#ifndef  _SYS_RDO_INTERNAL_API_
#define  _SYS_RDO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RDO_STATES.C
***********************************************************/

ANSC_HANDLE
SysRdoGetRootFolder
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRdoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRdoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRdoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRdoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_RDO_OPERATION.C
***********************************************************/

ANSC_STATUS
SysRdoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRdoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RDO_ACCESS.C
***********************************************************/

ANSC_HANDLE
SysRdoAddFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
SysRdoAddFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysRdoDelFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

ANSC_STATUS
SysRdoDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

ANSC_HANDLE
SysRdoGetFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

ANSC_HANDLE
SysRdoGetFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysRdoAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulPermission,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
SysRdoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

ANSC_HANDLE
SysRdoGetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

ANSC_STATUS
SysRdoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );


#endif
