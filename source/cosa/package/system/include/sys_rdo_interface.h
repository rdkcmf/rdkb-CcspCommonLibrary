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

    module:	sys_rdo_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Driver Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/02/02    initial revision.

**********************************************************************/


#ifndef  _SYS_RDO_INTERFACE_
#define  _SYS_RDO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"
#include "sys_properties.h"


/***********************************************************
      SYS REPOSITORY DRIVER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSRDO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_ADD_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_ADD_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_DEL_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_DEL_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_GET_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_GET_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_ADD_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulPermission,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_DEL_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

typedef  ANSC_HANDLE
(*PFN_SYSRDO_GET_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSRDO_SET_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

/*
 * The primary difference between the Info Repository Object and the Repository Driver Object is
 * that SysIro is a system-dependent object for providing the required information-access interface
 * while this object implements a relatively independent tree-like data-management mechanism. Such
 * separation is not very rare across the ANSC platform.
 */
#define  SYS_REPOSITORY_DRIVER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    SYS_REPOSITORY_DRIVER_PROPERTY  Property;                                               \
    ANSC_HANDLE                     hRootFolder;                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SYSRDO_GET_CONTEXT          GetRootFolder;                                          \
    PFN_SYSRDO_GET_PROPERTY         GetProperty;                                            \
    PFN_SYSRDO_SET_PROPERTY         SetProperty;                                            \
    PFN_SYSRDO_RESET                ResetProperty;                                          \
    PFN_SYSRDO_RESET                Reset;                                                  \
                                                                                            \
    PFN_SYSRDO_ENGAGE               Engage;                                                 \
    PFN_SYSRDO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_SYSRDO_ADD_FOLDER1          AddFolder1;                                             \
    PFN_SYSRDO_ADD_FOLDER2          AddFolder2;                                             \
    PFN_SYSRDO_DEL_FOLDER1          DelFolder1;                                             \
    PFN_SYSRDO_DEL_FOLDER2          DelFolder2;                                             \
    PFN_SYSRDO_GET_FOLDER1          GetFolder1;                                             \
    PFN_SYSRDO_GET_FOLDER2          GetFolder2;                                             \
                                                                                            \
    PFN_SYSRDO_ADD_RECORD           AddRecord;                                              \
    PFN_SYSRDO_DEL_RECORD           DelRecord;                                              \
    PFN_SYSRDO_GET_RECORD           GetRecord;                                              \
    PFN_SYSRDO_SET_RECORD           SetRecord;                                              \
    /* end of object class content */                                                       \

typedef  struct
_SYS_REPOSITORY_DRIVER_OBJECT
{
    SYS_REPOSITORY_DRIVER_CLASS_CONTENT
}
SYS_REPOSITORY_DRIVER_OBJECT,  *PSYS_REPOSITORY_DRIVER_OBJECT;

#define  ACCESS_SYS_REPOSITORY_DRIVER_OBJECT(p)     \
         ACCESS_CONTAINER(p, SYS_REPOSITORY_DRIVER_OBJECT, Linkage)


#endif
