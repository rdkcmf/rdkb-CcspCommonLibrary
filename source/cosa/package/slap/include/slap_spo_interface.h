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

    module:	slap_spo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Srv Proxy Objects.

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


#ifndef  _SLAP_SPO_INTERFACE_
#define  _SLAP_SPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

#include "slap_definitions.h"
#include "slap_ifo_uoa.h"
#include "slap_ifo_goa.h"
#include "slap_ifo_poa.h"


/***********************************************************
          SLAP STANDARD SRV PROXY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_SPO_OBJ_MODE_acquired                 1
#define  SLAP_SPO_OBJ_MODE_referenced               2
#define  SLAP_SPO_OBJ_MODE_created                  3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPSPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPSPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_SLAPSPO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

typedef  char*
(*PFN_SLAPSPO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ULONG
(*PFN_SLAPSPO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_type
    );

typedef  ULONG
(*PFN_SLAPSPO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_mode
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_CONVERT1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_CONVERT2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_CONVERT3)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPSPO_CONVERT4)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

/*
 * The SLAP Srv Proxy Object is not derived from the base Srv Component Object. Hence it's not
 * manageable via UOA. While it's possible to manage the proxy objects in the same way as the other
 * component objects, doing so complicates the proxying scenario and introduce more overhead.
 */
#define  SLAP_SRV_PROXY_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    ULONG                           CallTimeout;                                            \
    char*                           ObjName;                                                \
    ULONG                           ObjType;                                                \
    ULONG                           ObjMode;                                                \
    ANSC_HANDLE                     hSlapGoaObj;                                            \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hSlapGoaIf;                                             \
    ANSC_HANDLE                     hSlapPoaIf;                                             \
                                                                                            \
    PFN_SLAPSPO_GET_TIME            GetCallTimeout;                                         \
    PFN_SLAPSPO_SET_TIME            SetCallTimeout;                                         \
    PFN_SLAPSPO_GET_NAME            GetObjName;                                             \
    PFN_SLAPSPO_SET_NAME            SetObjName;                                             \
    PFN_SLAPSPO_GET_TYPE            GetObjType;                                             \
    PFN_SLAPSPO_SET_TYPE            SetObjType;                                             \
    PFN_SLAPSPO_GET_MODE            GetObjMode;                                             \
    PFN_SLAPSPO_SET_MODE            SetObjMode;                                             \
                                                                                            \
    PFN_SLAPSPO_GET_CONTEXT         GetSlapGoaObj;                                          \
    PFN_SLAPSPO_SET_CONTEXT         SetSlapGoaObj;                                          \
    PFN_SLAPSPO_GET_IF              GetSlapUoaIf;                                           \
    PFN_SLAPSPO_SET_IF              SetSlapUoaIf;                                           \
    PFN_SLAPSPO_GET_IF              GetSlapGoaIf;                                           \
    PFN_SLAPSPO_SET_IF              SetSlapGoaIf;                                           \
    PFN_SLAPSPO_GET_IF              GetSlapPoaIf;                                           \
    PFN_SLAPSPO_SET_IF              SetSlapPoaIf;                                           \
    PFN_SLAPSPO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPSPO_ENGAGE              Engage;                                                 \
    PFN_SLAPSPO_CANCEL              Cancel;                                                 \
    PFN_SLAPSPO_INVOKE              InvokeDispatch;                                         \
                                                                                            \
    PFN_SLAPSPO_CONVERT1            PoaToGoaParamList;                                      \
    PFN_SLAPSPO_CONVERT2            GoaToPoaParamList;                                      \
    PFN_SLAPSPO_CONVERT3            PoaToGoaVariable;                                       \
    PFN_SLAPSPO_CONVERT4            GoaToPoaVariable;                                       \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_SRV_PROXY_OBJECT
{
    SLAP_SRV_PROXY_CLASS_CONTENT
}
SLAP_SRV_PROXY_OBJECT,  *PSLAP_SRV_PROXY_OBJECT;

#define  ACCESS_SLAP_SRV_PROXY_OBJECT(p)            \
         ACCESS_CONTAINER(p, SLAP_SRV_PROXY_OBJECT, Linkage)
#define  ACCESS_SLAP_SRV_PROXY_FIELD(f)             \
         ACCESS_FIELD_OFFSET(SLAP_SRV_PROXY_OBJECT, f)


#endif
