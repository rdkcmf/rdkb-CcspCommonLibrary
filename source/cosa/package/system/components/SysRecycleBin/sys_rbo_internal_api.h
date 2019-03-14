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

    module:	sys_rbo_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Recycle Bin
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/02    initial revision.

**********************************************************************/


#ifndef  _SYS_RBO_INTERNAL_API_
#define  _SYS_RBO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RBO_STATES.C
***********************************************************/

ANSC_HANDLE
SysRboGetServedFolder
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRboSetServedFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_HANDLE
SysRboGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRboSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
SysRboGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRboSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRboResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRboReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_RBO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
SysRboDelAllFolders
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRboDelAllRecords
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RBO_ACCESS.C
***********************************************************/

ULONG
SysRboGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
SysRboGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRboAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_HANDLE
SysRboGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

ANSC_STATUS
SysRboAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

ANSC_HANDLE
SysRboGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );


#endif
