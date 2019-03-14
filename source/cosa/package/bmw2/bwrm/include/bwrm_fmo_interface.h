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

    module:	bwrm_fmo_interface.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwrm File Manager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_FMO_INTERFACE_
#define  _BWRM_FMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwrm_properties.h"


/***********************************************************
        BWRM FILE MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWRMFMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWRMFMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_GET_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void**                      file_data,
        PULONG                      file_size
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_SET_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_NEW_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    );

typedef  ANSC_STATUS
(*PFN_BWRMFMO_DEL_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );

/*
 * The Bwrm File Manager Object maps the web file access functions to the underlying file system
 * I/O wrappers. While this object is a reasonable place for implementing binary file caching
 * functionality on top of the basic file access, it's probably better to encapsulate the binary
 * file content as part of a Cooked Page Object, so we can manage the caching control for both Raw
 * and Cooked Page in the same way.
 */
#define  BWRM_FILE_MANAGER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BWRM_FILE_MANAGER_PROPERTY      Property;                                               \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BWRMFMO_GET_PROPERTY        GetProperty;                                            \
    PFN_BWRMFMO_SET_PROPERTY        SetProperty;                                            \
    PFN_BWRMFMO_RESET               ResetProperty;                                          \
    PFN_BWRMFMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BWRMFMO_ENGAGE              Engage;                                                 \
    PFN_BWRMFMO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BWRMFMO_GET_FILE            GetFile;                                                \
    PFN_BWRMFMO_SET_FILE            SetFile;                                                \
    PFN_BWRMFMO_NEW_FILE            NewFile;                                                \
    PFN_BWRMFMO_DEL_FILE            DelFile;                                                \
    /* end of object class content */                                                       \

typedef  struct
_BWRM_FILE_MANAGER_OBJECT
{
    BWRM_FILE_MANAGER_CLASS_CONTENT
}
BWRM_FILE_MANAGER_OBJECT,  *PBWRM_FILE_MANAGER_OBJECT;

#define  ACCESS_BWRM_FILE_MANAGER_OBJECT(p)         \
         ACCESS_CONTAINER(p, BWRM_FILE_MANAGER_OBJECT, Linkage)


#endif
