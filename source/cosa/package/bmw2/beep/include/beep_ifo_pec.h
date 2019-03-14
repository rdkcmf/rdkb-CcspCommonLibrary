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

    module:	beep_ifo_pec.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BEEP Page Execution Controller Objects.

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


#ifndef  _BEEP_IFO_PEC_
#define  _BEEP_IFO_PEC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "beep_definitions.h"


/***********************************************************
     BEEP PAGE EXECUTION CONTROLLER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_PEC_INTERFACE_NAME                    "beepPageExecutionControllerIf"
#define  BEEP_PEC_INTERFACE_ID                      0

#define  BEEP_PEC_STATUS_continue                   0
#define  BEEP_PEC_STATUS_terminate                  1
#define  BEEP_PEC_STATUS_abort                      2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPPECIF_GET_OBJ1)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPECIF_GET_OBJ2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPECIF_GET_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPPECIF_RET_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    );

typedef  ANSC_STATUS
(*PFN_BEEPPECIF_INVOKE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulPecStatus
    );

/*
 * BEEP is the realization of a web-based execution environment, which is built on top of the SLAP
 * layer (Aggregation Plan). The PEC interface is the primary service interface used by the BREE
 * (BSP Run-time Execution Engine) to access all the BSP services offered by BMW 2.0 architecture.
 * There're a lot of similarities between the BEEP_PEC and SLAP_UOA interfaces. Such similarities
 * come from the fact that BEEP partially inherits the 'object_invoke' mechanism from SLAP directly.
 */
#define  BEEP_PEC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPPECIF_GET_OBJ1          GetRequest;                                             \
    PFN_BEEPPECIF_GET_OBJ1          GetResponse;                                            \
    PFN_BEEPPECIF_GET_OBJ1          GetApplication;                                         \
    PFN_BEEPPECIF_GET_OBJ1          GetSession;                                             \
    PFN_BEEPPECIF_GET_OBJ1          GetServer;                                              \
                                                                                            \
    PFN_BEEPPECIF_GET_OBJ2          GetBeepObject;                                          \
    PFN_BEEPPECIF_GET_PAGE          GetCookedPage;                                          \
    PFN_BEEPPECIF_RET_PAGE          RetCookedPage;                                          \
    PFN_BEEPPECIF_INVOKE            InvokeObject;                                           \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_PEC_INTERFACE
{
    BEEP_PEC_INTERFACE_CLASS_CONTENT
}
BEEP_PEC_INTERFACE,  *PBEEP_PEC_INTERFACE;

#define  ACCESS_BEEP_PEC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_PEC_INTERFACE, Linkage)


#endif
