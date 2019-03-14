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

    module:	sys_rbo_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Recycle Bin Objects.

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


#ifndef  _SYS_RBO_INTERFACE_
#define  _SYS_RBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "sys_properties.h"

#include "sys_ifo_rca.h"
#include "sys_ifo_rcg.h"
#include "sys_ifo_rcm.h"


/***********************************************************
      SYS REPOSITORY FOLDER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSRBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSRBO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_SYSRBO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_SYSRBO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_ACQUIRE_AS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_RELEASE_AS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_SYSRBO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_ADD_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

typedef  ANSC_HANDLE
(*PFN_SYSRBO_GET_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_SYSRBO_ADD_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

typedef  ANSC_HANDLE
(*PFN_SYSRBO_GET_RECORD1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

/*
 * The primary use of the Recycle Bin Object is to tracking the changes made to certain Repository
 * Folder and Record Objects, which is useful information for configuration management and policy
 * administration. However instead of implementing it as an inherent feature of the Repository
 * Folder Object, we make it attachable to the folder to avoid wasting resource if not needed.
 */
#define  SYS_RECYCLE_BIN_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SYS_RECYCLE_BIN_PROPERTY        Property;                                               \
    ANSC_HANDLE                     hServedFolder;                                          \
    ANSC_HANDLE                     hUserContext;                                           \
                                                                                            \
    QUEUE_HEADER                    FolderQueue;                                            \
    ANSC_LOCK                       FolderQueueLock;                                        \
    QUEUE_HEADER                    RecordQueue;                                            \
    ANSC_LOCK                       RecordQueueLock;                                        \
                                                                                            \
    PFN_SYSRBO_GET_CONTEXT          GetServedFolder;                                        \
    PFN_SYSRBO_SET_CONTEXT          SetServedFolder;                                        \
    PFN_SYSRBO_GET_CONTEXT          GetUserContext;                                         \
    PFN_SYSRBO_SET_CONTEXT          SetUserContext;                                         \
                                                                                            \
    PFN_SYSRBO_GET_PROPERTY         GetProperty;                                            \
    PFN_SYSRBO_SET_PROPERTY         SetProperty;                                            \
    PFN_SYSRBO_RESET                ResetProperty;                                          \
    PFN_SYSRBO_RESET                Reset;                                                  \
                                                                                            \
    PFN_SYSRBO_DEL_ALL              DelAllFolders;                                          \
    PFN_SYSRBO_DEL_ALL              DelAllRecords;                                          \
                                                                                            \
    PFN_SYSRBO_GET_COUNT            GetFolderCount;                                         \
    PFN_SYSRBO_GET_COUNT            GetRecordCount;                                         \
                                                                                            \
    PFN_SYSRBO_ADD_FOLDER           AddFolder;                                              \
    PFN_SYSRBO_GET_FOLDER1          GetFolderByIndex;                                       \
    PFN_SYSRBO_ADD_RECORD           AddRecord;                                              \
    PFN_SYSRBO_GET_RECORD1          GetRecordByIndex;                                       \
    /* end of object class content */                                                       \

typedef  struct
_SYS_RECYCLE_BIN_OBJECT
{
    SYS_RECYCLE_BIN_CLASS_CONTENT
}
SYS_RECYCLE_BIN_OBJECT,  *PSYS_RECYCLE_BIN_OBJECT;

#define  ACCESS_SYS_RECYCLE_BIN_OBJECT(p)           \
         ACCESS_CONTAINER(p, SYS_RECYCLE_BIN_OBJECT, Linkage)


#endif
