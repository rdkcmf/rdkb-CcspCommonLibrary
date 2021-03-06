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

    module:	slap_vmo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Var Mapper Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_VMO_INTERFACE_
#define  _SLAP_VMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"


/***********************************************************
          SLAP STANDARD VAR MAPPER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_VMO_UOA_NAME                          "System.Utility.VarMapper"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPVMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPVMO_INIT_ATO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_SLAPVMO_GET_SYNTAX)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_SLAPVMO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

/*
 * The SLAP Var Mapper Object is derived from the base Sco Standard Object. This object will be
 * created by SLAP internally inside the 'System' container. This object is responsible for mapping
 * the variable syntax/content_type names to the corresponding type definitions.
 */
#define  SLAP_VAR_MAPPER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAtoSyntax;                                             \
    ANSC_HANDLE                     hAtoContentType;                                        \
                                                                                            \
    PFN_SLAPVMO_INIT_ATO            InitAtoSyntax;                                          \
    PFN_SLAPVMO_INIT_ATO            InitAtoContentType;                                     \
                                                                                            \
    PFN_SLAPVMO_GET_SYNTAX          GetVarSyntax;                                           \
    PFN_SLAPVMO_GET_TYPE            GetVarContentType;                                      \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_VAR_MAPPER_OBJECT
{
    SLAP_VAR_MAPPER_CLASS_CONTENT
}
SLAP_VAR_MAPPER_OBJECT,  *PSLAP_VAR_MAPPER_OBJECT;

#define  ACCESS_SLAP_VAR_MAPPER_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_VAR_MAPPER_OBJECT, Linkage)
#define  ACCESS_SLAP_VAR_MAPPER_FIELD(f)            \
         ACCESS_FIELD_OFFSET(SLAP_VAR_MAPPER_OBJECT, f)


#endif
