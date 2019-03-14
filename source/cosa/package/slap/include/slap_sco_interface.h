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

    module:	slap_sco_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Srv Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SCO_INTERFACE_
#define  _SLAP_SCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

#include "slap_definitions.h"
#include "slap_ifo_uoa.h"


/***********************************************************
         SLAP STANDARD SRV COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPSCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_SLAPSCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_SLAPSCO_GET_NAME2)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ULONG
(*PFN_SLAPSCO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_type
    );

typedef  ULONG
(*PFN_SLAPSCO_GET_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_SET_ID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       instance_id
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SLAPSCO_MATCH1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ULONG                       instance_id
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCO_CONVERT)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        tpl_params,
        SLAP_PARAMETER_LIST*        tbc_params
    );

/*
 * Every SLAP Component Object must provide a 'Dispatch' method which allows all the object member
 * functions to be invokded in the same way. Therefore, a generic object access interface can be
 * constructed and presented to external modules. Note that this object is still derived from the
 * old ANSC_LIGHT_COMPONENT_OBJECT to use the basic identification and object creation/removal
 * framework it provides.
 */
#define  SLAP_SRV_COMPONENT_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    char*                           ObjName;                                                \
    ULONG                           ObjType;                                                \
    ULONG                           ObjInstanceId;                                          \
    ANSC_HANDLE                     hInsContext;                                            \
    ANSC_HANDLE                     hSlapObjContainer;                                      \
    ANSC_HANDLE                     hSlapObjRecord;                                         \
    ANSC_HANDLE                     hSlapObjEntity;                                         \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hSlapBssIf;                                             \
    ANSC_HANDLE                     hSlapOlaIf;                                             \
    ULONG                           HashIndex;                                              \
                                                                                            \
    PFN_SLAPSCO_GET_NAME2           GetObjName2;                                            \
    PFN_SLAPSCO_SET_NAME            SetObjName;                                             \
    PFN_SLAPSCO_GET_TYPE            GetObjType;                                             \
    PFN_SLAPSCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPSCO_ENGAGE              Engage;                                                 \
    PFN_SLAPSCO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_SLAPSCO_MATCH1              Match1;                                                 \
    PFN_SLAPSCO_INVOKE              InvokeDispatch;                                         \
    PFN_SLAPSCO_CONVERT             ConvertParams;                                          \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_SRV_COMPONENT_OBJECT
{
    SLAP_SRV_COMPONENT_CLASS_CONTENT
}
SLAP_SRV_COMPONENT_OBJECT,  *PSLAP_SRV_COMPONENT_OBJECT;

#define  ACCESS_SLAP_SRV_COMPONENT_OBJECT(p)        \
         ACCESS_CONTAINER(p, SLAP_SRV_COMPONENT_OBJECT, Linkage)
#define  ACCESS_SLAP_SRV_COMPONENT_FIELD(f)         \
         ACCESS_FIELD_OFFSET(SLAP_SRV_COMPONENT_OBJECT, f)


#endif
