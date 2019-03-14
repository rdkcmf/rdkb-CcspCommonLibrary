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

    module:	sys_cpo_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Ctlv Processor
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#ifndef  _SYS_CPO_INTERNAL_API_
#define  _SYS_CPO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_CPO_STATES.C
***********************************************************/

ANSC_STATUS
SysCpoSetSysIraIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SysCpoGetSysSfcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysCpoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysCpoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysCpoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysCpoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_CPO_OPERATION.C
***********************************************************/

ANSC_STATUS
SysCpoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysCpoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_CPO_SFCIF.C
***********************************************************/

ANSC_STATUS
SysCpoSfcVerify
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
SysCpoSfcProvision
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
SysCpoSfcSerialize
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppBuffer,
        PULONG                      pulSize
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_CPO_PROVISION.C
***********************************************************/

ANSC_STATUS
SysCpoProvisionFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulCtlvSize
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_CPO_SERIALIZE.C
***********************************************************/

ULONG
SysCpoQueryFolderSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName
    );

ANSC_STATUS
SysCpoSerializeFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulBufferSize
    );


#endif

