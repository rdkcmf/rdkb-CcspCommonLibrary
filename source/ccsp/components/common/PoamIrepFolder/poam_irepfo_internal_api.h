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

    module:	poam_irepfo_internal_api.h

        For Proxy Object Access Model Implementation (POAM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Poam Irep Folder
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#ifndef  _POAM_IREPFO_INTERNAL_API_
#define  _POAM_IREPFO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN POAM_IREPFO_STATES.C
***********************************************************/

ANSC_STATUS
PoamIrepFoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN POAM_IREPFO_ACCESS.C
***********************************************************/

char*
PoamIrepFoGetFolderName
    (
        ANSC_HANDLE                 hThisObject
    );

SLAP_OBJECT
PoamIrepFoGetFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    );

SLAP_OBJECT
PoamIrepFoGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    );

SLAP_OBJECT
PoamIrepFoAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName,
        SLAP_UINT32                 ulReserved
    );

ANSC_STATUS
PoamIrepFoDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    );

ULONG
PoamIrepFoGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    );

char*
PoamIrepFoEnumFolder
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    );

ANSC_STATUS
PoamIrepFoSortFolder
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

SLAP_VARIABLE*
PoamIrepFoGetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32*                pulRecordType
    );

SLAP_VARIABLE*
PoamIrepFoGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType
    );

SLAP_VARIABLE*
PoamIrepFoGetRecordByIndex2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        char**                      ppRecordName
    );

ANSC_STATUS
PoamIrepFoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar
    );

ANSC_STATUS
PoamIrepFoAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar,
        ULONG                       ulReserved
    );

ANSC_STATUS
PoamIrepFoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

ANSC_STATUS
PoamIrepFoDelRecordRecursive
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pRecordName
    )
;

ULONG
PoamIrepFoGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    );

SLAP_UCHAR_ARRAY*
PoamIrepFoGetRecordArray
    (
        ANSC_HANDLE                 hThisObject
    );

char*
PoamIrepFoEnumRecord
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        SLAP_UINT32*                pulRecordSize
    );

ANSC_STATUS
PoamIrepFoSortRecord
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

ANSC_STATUS
PoamIrepFoClear
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
PoamIrepFoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
PoamIrepFoEnableFileSync
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN POAM_IREPFO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
PoamIrepFoGetAllLocalRecords
    (
        ANSC_HANDLE                 hThisObject
    );

SLAP_VARIABLE*
PoamIrepFoGetLocalRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulRecordType
    );

ANSC_STATUS
PoamIrepFoSetLocalRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalRecord
    );

ANSC_STATUS
PoamIrepFoAddLocalRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalRecord
    );

ANSC_STATUS
PoamIrepFoDelLocalRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
PoamIrepFoDelAllLocalRecords
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN POAM_IREPFO_UTILITIES.C
***********************************************************/

ANSC_STATUS
PoamIrepFoDecodeRecordArray
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


#endif
