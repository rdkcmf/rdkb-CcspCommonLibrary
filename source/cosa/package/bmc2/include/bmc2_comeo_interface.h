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

    module:	bmc2_comeo_interface.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Com Executor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMEO_INTERFACE_
#define  _BMC2_COMEO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "bmc2_properties.h"

#include "bspeng_ifo_soa.h"

/***********************************************************
        BMC2 COM EXECUTOR COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

#define  BMC2_SOA_INTERFACE_NAME                    "bmc2SlapObjectAccessIf"
#define  BMC2_SOA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2COMEO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMEO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMEO_PREPARE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMEO_EXECUTE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage,
        ANSC_HANDLE                 hBmc2PecIf
    );

typedef  ANSC_HANDLE
(*PFN_BREESPO_CREATE_SOAIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmc2PecIf
    );

/*
 * The Bmc2 Com Executor Object encapsulates the implementation of the SCP (Shell Command Page)
 * execution environment. It's responsible for instantiating the SCP engine and execute the SCP
 * page. By including Com Executor as part of BMC2 module, we simplify the module loading and the
 * overall software architecture (at least on paper) comparing to the multi-layered BMW structure.
 */
#define  BMC2_COM_EXECUTOR_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BMC2_COM_EXECUTOR_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hBmc2EnvController;                                     \
    ANSC_HANDLE                     hBmc2SoaIf;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BMC2COMEO_GET_CONTEXT       GetBmc2EnvController;                                   \
    PFN_BMC2COMEO_SET_CONTEXT       SetBmc2EnvController;                                   \
    PFN_BMC2COMEO_GET_PROPERTY      GetProperty;                                            \
    PFN_BMC2COMEO_SET_PROPERTY      SetProperty;                                            \
    PFN_BMC2COMEO_RESET             ResetProperty;                                          \
    PFN_BMC2COMEO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BMC2COMEO_ENGAGE            Engage;                                                 \
    PFN_BMC2COMEO_CANCEL            Cancel;                                                 \
    PFN_BMC2COMEO_SETUP_ENV         SetupEnv;                                               \
    PFN_BMC2COMEO_CLOSE_ENV         CloseEnv;                                               \
                                                                                            \
    PFN_BMC2COMEO_PREPARE           PreparePage;                                            \
    PFN_BMC2COMEO_EXECUTE           ExecutePage;                                            \
                                                                                            \
    PFN_BREESPO_CREATE_SOAIF        CreateBmc2SoaIf;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_COM_EXECUTOR_OBJECT
{
    BMC2_COM_EXECUTOR_CLASS_CONTENT
}
BMC2_COM_EXECUTOR_OBJECT,  *PBMC2_COM_EXECUTOR_OBJECT;

#define  ACCESS_BMC2_COM_EXECUTOR_OBJECT(p)         \
         ACCESS_CONTAINER(p, BMC2_COM_EXECUTOR_OBJECT, Linkage)


#endif

