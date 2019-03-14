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

    module:	sys_rro_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Record Objects.

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


#ifndef  _SYS_RRO_INTERFACE_
#define  _SYS_RRO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"
#include "sys_properties.h"


/***********************************************************
      SYS REPOSITORY RECORD COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSRRO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSRRO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_SYSRRO_GET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    );

typedef  ULONG
(*PFN_SYSRRO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_SYSRRO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_SYSRRO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

typedef  BOOL
(*PFN_SYSRRO_GRANT_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

typedef  ULONG
(*PFN_SYSRRO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_GET_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        PULONG                      pulDataSize
    );

typedef  ANSC_STATUS
(*PFN_SYSRRO_SET_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        ULONG                       ulDataSize
    );

/*
 * I don't even remember how many times I have to write something like this: a tree-liked data
 * repository with a few pre-defined data types. XML parser, ASN.1 coder/decoder, Configuration
 * File parser, ... to name a few. Why do we have to reimplement such construct every time? Why
 * is a general object not gonna meet all the requirements? Answer: there's no silver-bullet?
 */
#define  SYS_REPOSITORY_RECORD_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    ULONG                           HashIndex;                                              \
    ULONG                           BirthTime;                                              \
    ULONG                           Timestamp;                                              \
    ANSC_HANDLE                     hRenderAttr;                                            \
    ANSC_HANDLE                     hParentFolder;                                          \
    ANSC_HANDLE                     hUserContext;                                           \
    ULONG                           UserReserved;                                           \
                                                                                            \
    char*                           RecordName;                                             \
    ULONG                           RecordType;                                             \
    ULONG                           RecordSize;                                             \
    ULONG                           Permission;                                             \
                                                                                            \
    PFN_SYSRRO_GET_TIME             GetBirthTime;                                           \
    PFN_SYSRRO_SET_TIME             SetBirthTime;                                           \
    PFN_SYSRRO_GET_CONTEXT          GetRenderAttr;                                          \
    PFN_SYSRRO_SET_CONTEXT          SetRenderAttr;                                          \
    PFN_SYSRRO_GET_CONTEXT          GetParentFolder;                                        \
    PFN_SYSRRO_SET_CONTEXT          SetParentFolder;                                        \
    PFN_SYSRRO_GET_CONTEXT          GetUserContext;                                         \
    PFN_SYSRRO_SET_CONTEXT          SetUserContext;                                         \
    PFN_SYSRRO_GET_RESERVED         GetUserReserved;                                        \
    PFN_SYSRRO_SET_RESERVED         SetUserReserved;                                        \
                                                                                            \
    PFN_SYSRRO_GET_NAME             GetRecordName;                                          \
    PFN_SYSRRO_SET_NAME             SetRecordName;                                          \
    PFN_SYSRRO_GET_TYPE             GetRecordType;                                          \
    PFN_SYSRRO_SET_TYPE             SetRecordType;                                          \
    PFN_SYSRRO_GET_TYPE             GetPermission;                                          \
    PFN_SYSRRO_SET_TYPE             SetPermission;                                          \
    PFN_SYSRRO_GET_TYPE             GetContentType;                                         \
    PFN_SYSRRO_SET_TYPE             SetContentType;                                         \
                                                                                            \
    PFN_SYSRRO_GRANT_ACCESS         GrantAccess;                                            \
    PFN_SYSRRO_GET_SIZE             GetRecordSize;                                          \
    PFN_SYSRRO_GET_DATA             GetRecordData;                                          \
    PFN_SYSRRO_SET_DATA             SetRecordData;                                          \
    /* end of object class content */                                                       \

typedef  struct
_SYS_REPOSITORY_RECORD_OBJECT
{
    SYS_REPOSITORY_RECORD_CLASS_CONTENT
}
SYS_REPOSITORY_RECORD_OBJECT,  *PSYS_REPOSITORY_RECORD_OBJECT;

#define  ACCESS_SYS_REPOSITORY_RECORD_OBJECT(p)     \
         ACCESS_CONTAINER(p, SYS_REPOSITORY_RECORD_OBJECT, Linkage)


#endif
