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

    module:	poam_irepfo_interface.h

        For Proxy Object Access Model Implementation (POAM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Poam Irep Folder Object.

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


#ifndef  _POAM_IREPFO_INTERFACE_
#define  _POAM_IREPFO_INTERFACE_

#include "slap_definitions.h"
#include "ansc_lco_interface.h"


#define  IREPFO_FULLNAME_LENGTH                   256
/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
 
typedef  char*
(*PFN_POAMIREPFO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_POAMIREPFO_GET_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    );

typedef  ANSC_HANDLE
(*PFN_POAMIREPFO_GET_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    );

typedef  ANSC_HANDLE
(*PFN_POAMIREPFO_ADD_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName,
        SLAP_UINT32                 ulReserved
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_DEL_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    );

typedef  ULONG
(*PFN_POAMIREPFO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_POAMIREPFO_GET_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_POAMIREPFO_ENUM_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_SORT_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

typedef  SLAP_VARIABLE*
(*PFN_POAMIREPFO_GET_RECORD1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32*                pulRecordType
    );

typedef  SLAP_VARIABLE*
(*PFN_POAMIREPFO_GET_RECORD2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType
    );

typedef  SLAP_VARIABLE*
(*PFN_POAMIREPFO_GET_RECORD3)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        char**                      ppRecordName
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_SET_RECORD1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_ADD_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar,
        ULONG                       ulReserved
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_DEL_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

typedef  char*
(*PFN_POAMIREPFO_ENUM_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        SLAP_UINT32*                pulRecordSize
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_SORT_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_ENABLE_SYNC)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_GET_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_VARIABLE*
(*PFN_POAMIREPFO_GET_LRECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulRecordType
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_SET_LRECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalRecord
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_ADD_LRECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalRecord
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_DEL_LRECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_POAMIREPFO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef ANSC_STATUS
(*PFN_POAMIREPFO_DELRECRECURSIVE)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pRecordName
    );

/*
 * The Poam Irep Folder Object is designed to provide a unified interface for accessing the folder
 * and record objects contained in the System Repository. Note that the methods provided by this
 * object is a subset of the SYS_IRA interface. This is because the there're many features (e.g.
 * access control and rendering attributes) are usually used only by internal system modules.
 */
#define  POAM_IREP_FOLDER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                                \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                            \
    UCHAR                           Name[IREPFO_FULLNAME_LENGTH];                           \
                                                                                            \
    PFN_POAMIREPFO_GET_NAME         GetFolderName;                                          \
                                                                                            \
    PFN_POAMIREPFO_GET_FOLDER1      GetFolder;                                              \
    PFN_POAMIREPFO_GET_FOLDER2      GetFolderByIndex;                                       \
    PFN_POAMIREPFO_ADD_FOLDER       AddFolder;                                              \
    PFN_POAMIREPFO_DEL_FOLDER       DelFolder;                                              \
    PFN_POAMIREPFO_GET_COUNT        GetFolderCount;                                         \
    PFN_POAMIREPFO_ENUM_FOLDER      EnumFolder;                                             \
    PFN_POAMIREPFO_SORT_FOLDER      SortFolder;                                             \
                                                                                            \
    PFN_POAMIREPFO_GET_RECORD1      GetRecord;                                              \
    PFN_POAMIREPFO_GET_RECORD2      GetRecordByIndex;                                       \
    PFN_POAMIREPFO_GET_RECORD3      GetRecordByIndex2;                                      \
    PFN_POAMIREPFO_SET_RECORD1      SetRecord;                                              \
    PFN_POAMIREPFO_ADD_RECORD       AddRecord;                                              \
    PFN_POAMIREPFO_DEL_RECORD       DelRecord;                                              \
    PFN_POAMIREPFO_GET_COUNT        GetRecordCount;                                         \
    PFN_POAMIREPFO_GET_ARRAY        GetRecordArray;                                         \
    PFN_POAMIREPFO_ENUM_RECORD      EnumRecord;                                             \
    PFN_POAMIREPFO_SORT_RECORD      SortRecord;                                             \
                                                                                            \
    PFN_POAMIREPFO_CLEAR            Clear;                                                  \
    PFN_POAMIREPFO_CLOSE            Close;                                                  \
    PFN_POAMIREPFO_ENABLE_SYNC      EnableFileSync;                                         \
    PFN_POAMIREPFO_DELRECRECURSIVE  DelRecordRecursive;                                     \
    /* end of object class content */                                                             \

typedef  struct
_POAM_IREP_FOLDER_OBJECT
{
    POAM_IREP_FOLDER_CLASS_CONTENT
}
POAM_IREP_FOLDER_OBJECT,  *PPOAM_IREP_FOLDER_OBJECT;

#define RETURN_STATUS_PSM_TO_ANSC(psmRet, anscRet)      \
    switch(psmRet)                                      \
    {                                                   \
        case CCSP_SUCCESS :                       \
            anscRet = ANSC_STATUS_SUCCESS;              \
            break;                                      \
                                                        \
        case CCSP_CR_ERR_INVALID_PARAM :                     \
            anscRet = ANSC_STATUS_BAD_PARAMETER;        \
            break;                                      \
                                                        \
        case CCSP_FAILURE :                         \
            anscRet = ANSC_STATUS_FAILURE;              \
            break;                                      \
        default:                                        \
            anscRet = ANSC_STATUS_FAILURE;              \
    }                                                   \
                                                        \


#endif
