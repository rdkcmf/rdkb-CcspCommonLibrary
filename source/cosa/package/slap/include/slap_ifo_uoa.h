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

    module:	slap_ifo_uoa.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Universal Object Access Objects.

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


#ifndef  _SLAP_IFO_UOA_
#define  _SLAP_IFO_UOA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
      SLAP UNIVERSAL OBJECT ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_UOA_INTERFACE_NAME                    "slapUniversalObjectAccessIf"
#define  SLAP_UOA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPUOAIF_CREATE_CON)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_DELETE_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_HANDLE
(*PFN_SLAPUOAIF_ACQ_CON)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_REL_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_HANDLE
(*PFN_SLAPUOAIF_CREATE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_DELETE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_DELETE_OBJ2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_INVOKE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

typedef  ANSC_HANDLE
(*PFN_SLAPUOAIF_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hInsContext
    );

typedef  ULONG
(*PFN_SLAPUOAIF_GET_OPTION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in
    );

typedef  ANSC_HANDLE
(*PFN_SLAPUOAIF_ACQ_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_REL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_ENROLL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

typedef  ANSC_STATUS
(*PFN_SLAPUOAIF_UNBIND_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  BOOL
(*PFN_SLAPUOAIF_VERIFY_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

/*
 * SLAP provides a generic mechanism for upper control/management modules to access Service Logic
 * Objects. This mechanism is encapsulated by the UOA (Universal Object Access) interface. This
 * interface MUST allow external modules to identify any SLAP object by name. The internal SLAP's
 * hierarchical name space should not be exposed to the outside.
 */
#define  SLAP_UOA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPUOAIF_CREATE_CON        CreateContainer;                                        \
    PFN_SLAPUOAIF_DELETE_CON        DeleteContainer;                                        \
    PFN_SLAPUOAIF_ACQ_CON           AcqContainerAccess;                                     \
    PFN_SLAPUOAIF_REL_CON           RelContainerAccess;                                     \
                                                                                            \
    PFN_SLAPUOAIF_CREATE_OBJ        CreateObject;                                           \
    PFN_SLAPUOAIF_DELETE_OBJ        DeleteObject;                                           \
    PFN_SLAPUOAIF_DELETE_OBJ2       DeleteObject2;                                          \
    PFN_SLAPUOAIF_INVOKE_OBJ        InvokeObject;                                           \
    PFN_SLAPUOAIF_GET_CONTEXT       GetObjInsContext;                                       \
    PFN_SLAPUOAIF_SET_CONTEXT       SetObjInsContext;                                       \
    PFN_SLAPUOAIF_GET_OPTION        GetObjCallOption;                                       \
    PFN_SLAPUOAIF_ACQ_OBJ           AcqObjectAccess;                                        \
    PFN_SLAPUOAIF_REL_OBJ           RelObjectAccess;                                        \
                                                                                            \
    PFN_SLAPUOAIF_ENROLL_OBJ        EnrollObjDescriptor;                                    \
    PFN_SLAPUOAIF_UNBIND_OBJ        UnbindObjDescriptor;                                    \
    PFN_SLAPUOAIF_VERIFY_OBJ        VerifyObjDescriptor;                                    \
                                                                                            \
    PFN_SLAPUOAIF_CREATE_OBJ        CreateProxyObject;                                      \
    PFN_SLAPUOAIF_ENROLL_OBJ        EnrollProxyObject;                                      \
    PFN_SLAPUOAIF_UNBIND_OBJ        UnbindProxyObject;                                      \
    PFN_SLAPUOAIF_VERIFY_OBJ        VerifyProxyObject;                                      \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_UOA_INTERFACE
{
    SLAP_UOA_INTERFACE_CLASS_CONTENT
}
SLAP_UOA_INTERFACE,  *PSLAP_UOA_INTERFACE;

#define  ACCESS_SLAP_UOA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SLAP_UOA_INTERFACE, Linkage)
#define  ACCESS_SLAP_UOA_OBJECT(p, type)            \
         ( (type)((PSLAP_OBJ_RECORD_OBJECT)p)->hSlapSrvComponent )
#define  DOMASK_SLAP_UOA_OBJECT(p)                  \
         ( p->hSlapObjRecord )


#endif
