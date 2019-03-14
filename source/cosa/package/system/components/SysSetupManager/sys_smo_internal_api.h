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

    module:	sys_smo_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Setup Manager
        Object.

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


#ifndef  _SYS_SMO_INTERNAL_API_
#define  _SYS_SMO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_SMO_STATES.C
***********************************************************/

ANSC_HANDLE
SysSmoGetSetupRfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysSmoGetSysRcmIf
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_SMO_AUDIT.C
***********************************************************/

ANSC_STATUS
SysSmoReactRfoAdded
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysSmoReactRfoDeleted
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysSmoReactRroAdded
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysSmoReactRroModified
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysSmoReactRroDeleted
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SYS_SMO_SESSION.C
***********************************************************/

ANSC_STATUS
SysSmoInitSetUpAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAccessKey
    );

ANSC_STATUS
SysSmoStopSetUpAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAccessKey
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_SMO_RCMIF.C
***********************************************************/

ANSC_STATUS
SysSmoRcmNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysSmoRcmInit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_STATUS
SysSmoRcmStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );


#endif
