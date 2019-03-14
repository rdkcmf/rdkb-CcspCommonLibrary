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

    module:	slap_ifo_loa.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Local Object Access Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_IFO_LOA_
#define  _SLAP_IFO_LOA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
        SLAP LOCAL OBJECT ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_LOA_INTERFACE_NAME                    "slapLocalObjectAccessIf"
#define  SLAP_LOA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPLOAIF_ACQ_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        char*                       container_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_REL_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_LOCATE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char**                      lpc_party_name,
        ANSC_HANDLE*                lpc_party_addr
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_DELETE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_INVOKE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        BOOL                        bTranslate,
        ULONG                       ulTimeout
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLOAIF_ACQ_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_REL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_REL_OBJ2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_FLUSH_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_ENROLL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

typedef  ANSC_STATUS
(*PFN_SLAPLOAIF_UNBIND_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  BOOL
(*PFN_SLAPLOAIF_VERIFY_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

/*
 * Not only SLAP provides a powerful yet generic framework for object access/management, but also
 * it can be used as the foundation for IPC (Inter-Process Communication) between among different
 * processes. This benefit comes with the fact that SLAP requires the serialization of all input
 * and output parameters when invoking object methods. The SLAP_LOA interface allows upper layer to
 * access SLAP containers/objects in a very similar way as the SLAP objects in the same process.
 */
#define  SLAP_LOA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPLOAIF_ACQ_CON           AcqContainerAccess;                                     \
    PFN_SLAPLOAIF_REL_CON           RelContainerAccess;                                     \
                                                                                            \
    PFN_SLAPLOAIF_LOCATE_OBJ        LocateObject;                                           \
    PFN_SLAPLOAIF_DELETE_OBJ        DeleteObject;                                           \
    PFN_SLAPLOAIF_INVOKE_OBJ        InvokeObject;                                           \
    PFN_SLAPLOAIF_ACQ_OBJ           AcqObjectAccess;                                        \
    PFN_SLAPLOAIF_REL_OBJ           RelObjectAccess;                                        \
    PFN_SLAPLOAIF_REL_OBJ2          RelObjectAccess2;                                       \
    PFN_SLAPLOAIF_FLUSH_ALL         FlushAllObjects;                                        \
                                                                                            \
    PFN_SLAPLOAIF_ENROLL_OBJ        EnrollMobileObject;                                     \
    PFN_SLAPLOAIF_UNBIND_OBJ        UnbindMobileObject;                                     \
    PFN_SLAPLOAIF_VERIFY_OBJ        VerifyMobileObject;                                     \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_LOA_INTERFACE
{
    SLAP_LOA_INTERFACE_CLASS_CONTENT
}
SLAP_LOA_INTERFACE,  *PSLAP_LOA_INTERFACE;

#define  ACCESS_SLAP_LOA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SLAP_LOA_INTERFACE, Linkage)


#endif
