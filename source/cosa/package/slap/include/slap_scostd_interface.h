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

    module:	slap_scostd_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Sco Standard Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/08/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SCOSTD_INTERFACE_
#define  _SLAP_SCOSTD_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_sco_interface.h"
#include "slap_sco_exported_api.h"
#include "slap_oro_interface.h"
#include "slap_oro_exported_api.h"


/***********************************************************
         SLAP STANDARD SCO STANDARD OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPSCOSTD_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOSTD_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPSCOSTD_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOSTD_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  SLAP_OBJECT
(*PFN_SLAPSCOSTD_CREATE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  SLAP_OBJECT
(*PFN_SLAPSCOSTD_CREATE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        SLAP_HANDLE                 hInsContext
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOSTD_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_OBJECT                 hSlapObject
    );

typedef  SLAP_OBJECT
(*PFN_SLAPSCOSTD_CLONE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_SLAPSCOSTD_TO_STRING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOSTD_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOSTD_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOSTD_IMPORT)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOSTD_EXPORT)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST**       param_list
    );

typedef  void*
(*PFN_SLAPSCOSTD_INVOKE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMethodDescriptor,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST*        params_out
    );

/*
 * The SLAP Sco Standard Object is derived from the SLAP Srv Component Object. It maintains two
 * standard name-method mapping tables: property mapping table and generic method mapping table.
 * External service objects that are derived from Sco Standard Object may change the default table
 * size by overriding SetupOpoTable() and SetupMdoTable() member functions. Derived objects should
 * always override BuildOpoTable() and BuildMdoTable() to add all the object properties and methods
 * into these two tables.
 */
#define  SLAP_SCO_STANDARD_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    SLAP_SRV_COMPONENT_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    PFN_SLAPSCOSTD_CREATE           CreateObject;                                           \
    PFN_SLAPSCOSTD_CREATE2          CreateObject2;                                          \
    PFN_SLAPSCOSTD_DELETE           DeleteObject ;                                          \
    PFN_SLAPSCOSTD_CLONE            CloneObject;                                            \
                                                                                            \
    PFN_SLAPSCOSTD_TO_STRING        ToString;                                               \
    PFN_SLAPSCOSTD_GET_PROPERTY     GetProperty;                                            \
    PFN_SLAPSCOSTD_SET_PROPERTY     SetProperty;                                            \
                                                                                            \
    PFN_SLAPSCOSTD_IMPORT           ImportMe;                                               \
    PFN_SLAPSCOSTD_EXPORT           ExportMe;                                               \
                                                                                            \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput0;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput1;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput2;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput3;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput4;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput5;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput6;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput7;                                        \
    PFN_SLAPSCOSTD_INVOKE           InvokeFixInput8;                                        \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_SCO_STANDARD_OBJECT
{
    SLAP_SCO_STANDARD_CLASS_CONTENT
}
SLAP_SCO_STANDARD_OBJECT,  *PSLAP_SCO_STANDARD_OBJECT;

#define  ACCESS_SLAP_SCO_STANDARD_OBJECT(p)         \
         ACCESS_CONTAINER(p, SLAP_SCO_STANDARD_OBJECT, Linkage)
#define  ACCESS_SLAP_SCO_STANDARD_FIELD(f)          \
         ACCESS_FIELD_OFFSET(SLAP_SCO_STANDARD_OBJECT, f)


#endif
