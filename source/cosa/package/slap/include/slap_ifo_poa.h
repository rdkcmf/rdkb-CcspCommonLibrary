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

    module:	slap_ifo_poa.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Proxy Object Access Objects.

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


#ifndef  _SLAP_IFO_POA_
#define  _SLAP_IFO_POA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
       SLAP PROXY OBJECT ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_POA_INTERFACE_NAME                    "slapProxyObjectAccessIf"
#define  SLAP_POA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPPOAIF_ACQ_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPPOAIF_REL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPPOAIF_CREATE_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 hSlapGoaObj
    );

typedef  ANSC_STATUS
(*PFN_SLAPPOAIF_ENROLL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

typedef  ANSC_STATUS
(*PFN_SLAPPOAIF_UNBIND_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  BOOL
(*PFN_SLAPPOAIF_VERIFY_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

/*
 * SLAP_POA interface is built on top of SLAP_GOA. It allows the upper control/management modules
 * to access any proxied Service Logic Object via the regular object member functions instead of
 * the mandatory Invoke() entry point. SLAP_POA should be used to access SLAP_LOA objects only, but
 * there's no limitation against using it to access SLAP_UOA objects.
 */
#define  SLAP_POA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPPOAIF_ACQ_OBJ           AcqObjectAccess;                                        \
    PFN_SLAPPOAIF_REL_OBJ           RelObjectAccess;                                        \
                                                                                            \
    PFN_SLAPPOAIF_CREATE_OBJ        CreateProxyObject;                                      \
    PFN_SLAPPOAIF_ENROLL_OBJ        EnrollProxyObject;                                      \
    PFN_SLAPPOAIF_UNBIND_OBJ        UnbindProxyObject;                                      \
    PFN_SLAPPOAIF_VERIFY_OBJ        VerifyProxyObject;                                      \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_POA_INTERFACE
{
    SLAP_POA_INTERFACE_CLASS_CONTENT
}
SLAP_POA_INTERFACE,  *PSLAP_POA_INTERFACE;

#define  ACCESS_SLAP_POA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SLAP_POA_INTERFACE, Linkage)


#endif
