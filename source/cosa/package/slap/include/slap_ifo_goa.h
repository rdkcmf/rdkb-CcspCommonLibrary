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

    module:	slap_ifo_goa.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Generic Object Access Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_IFO_GOA_
#define  _SLAP_IFO_GOA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
       SLAP GENERIC OBJECT ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_GOA_INTERFACE_NAME                    "slapGenericObjectAccessIf"
#define  SLAP_GOA_INTERFACE_ID                      0

#define  SLAP_GOA_MODE_thisProcess                  0x00000001
#define  SLAP_GOA_MODE_imcpClient                   0x00000002
#define  SLAP_GOA_MODE_imcpServer                   0x00000004
#define  SLAP_GOA_MODE_mobilitySupport              0x00000008
#define  SLAP_GOA_MODE_enablePooling                0x00000010
#define  SLAP_GOA_MODE_autoExport                   0x00000020
#define  SLAP_GOA_MODE_all                          0xFFFFFFFF

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_SLAPGOAIF_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  USHORT
(*PFN_SLAPGOAIF_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ULONG
(*PFN_SLAPGOAIF_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipaddr
    );

typedef  ANSC_HANDLE
(*PFN_SLAPGOAIF_CREATE_CON)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_DELETE_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_HANDLE
(*PFN_SLAPGOAIF_ACQ_CON)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_REL_CON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_HANDLE
(*PFN_SLAPGOAIF_CREATE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_DELETE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_INVOKE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_INVOKE_OBJ2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        ULONG                       ulTimeout
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_ATTACH_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapContainer
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_DETACH_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_UNLINK_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPGOAIF_ACQ_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_REL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_FLUSH_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_ENROLL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

typedef  ANSC_STATUS
(*PFN_SLAPGOAIF_UNBIND_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  BOOL
(*PFN_SLAPGOAIF_VERIFY_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

/*
 * SLAP_GOA interface is built on top of SLAP_UOA and SLAP_LOA implementations. It provides a
 * generic mechanism for upper control/management modules to access any Service Logic Object within
 * the system. The SLAP_GOA mechanism automatially locates the target SLAP object by resolving the
 * obj_name to party_name mapping. Note that the limitation of SLAP_LOA applies to SLAP_GOA: only
 * limited container and object manipulation is exposed for the remote objects.
 */
#define  SLAP_GOA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPGOAIF_GET_MODE          GetAggregationMode;                                     \
    PFN_SLAPGOAIF_SET_MODE          SetAggregationMode;                                     \
    PFN_SLAPGOAIF_GET_PORT          GetLpcPartyPort;                                        \
    PFN_SLAPGOAIF_SET_PORT          SetLpcPartyPort;                                        \
    PFN_SLAPGOAIF_GET_ADDR          GetLpcPartyAddr;                                        \
    PFN_SLAPGOAIF_SET_ADDR          SetLpcPartyAddr;                                        \
    PFN_SLAPGOAIF_GET_ADDR          GetLpcManagerAddr;                                      \
    PFN_SLAPGOAIF_SET_ADDR          SetLpcManagerAddr;                                      \
                                                                                            \
    PFN_SLAPGOAIF_CREATE_CON        CreateContainer;                                        \
    PFN_SLAPGOAIF_DELETE_CON        DeleteContainer;                                        \
                                                                                            \
    PFN_SLAPGOAIF_CREATE_OBJ        CreateObject;                                           \
    PFN_SLAPGOAIF_DELETE_OBJ        DeleteObject;                                           \
    PFN_SLAPGOAIF_INVOKE_OBJ        InvokeObject;                                           \
    PFN_SLAPGOAIF_INVOKE_OBJ2       InvokeObject2;                                          \
    PFN_SLAPGOAIF_ATTACH_OBJ        AttachObject;                                           \
    PFN_SLAPGOAIF_DETACH_OBJ        DetachObject;                                           \
    PFN_SLAPGOAIF_UNLINK_OBJ        UnlinkObject;                                           \
    PFN_SLAPGOAIF_ACQ_OBJ           AcqObjectAccess;                                        \
    PFN_SLAPGOAIF_REL_OBJ           RelObjectAccess;                                        \
    PFN_SLAPGOAIF_FLUSH_ALL         FlushAllObjects;                                        \
                                                                                            \
    PFN_SLAPGOAIF_ENROLL_OBJ        EnrollObjDescriptor;                                    \
    PFN_SLAPGOAIF_UNBIND_OBJ        UnbindObjDescriptor;                                    \
    PFN_SLAPGOAIF_VERIFY_OBJ        VerifyObjDescriptor;                                    \
                                                                                            \
    PFN_SLAPGOAIF_ENROLL_OBJ        EnrollMobileObject;                                     \
    PFN_SLAPGOAIF_UNBIND_OBJ        UnbindMobileObject;                                     \
    PFN_SLAPGOAIF_VERIFY_OBJ        VerifyMobileObject;                                     \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_GOA_INTERFACE
{
    SLAP_GOA_INTERFACE_CLASS_CONTENT
}
SLAP_GOA_INTERFACE,  *PSLAP_GOA_INTERFACE;

#define  ACCESS_SLAP_GOA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SLAP_GOA_INTERFACE, Linkage)


#endif
