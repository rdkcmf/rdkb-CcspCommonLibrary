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

    module:	bspeng_ifo_soa.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        APIs for the BSP Slap Object Access interface.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#ifndef  _BSPENG_IFO_SOA_
#define  _BSPEND_IFO_SOA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"
#include "slap_definitions.h"


/***********************************************************
     BSPENG SLAP OBJECT ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

#define  BSPENG_SOA_STATUS_continue                 0
#define  BSPENG_SOA_STATUS_terminate                1
#define  BSPENG_SOA_STATUS_abort                    2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BSPSOAIF_GET_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_HANDLE
(*PFN_BSPSOAIF_GET_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BSPSOAIF_RET_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    );

typedef  ANSC_STATUS
(*PFN_BSPSOAIF_GET_CP_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage,
        PUCHAR                      *ppStream,
        PULONG                      pulStreamSize
    );

typedef  ANSC_STATUS
(*PFN_BSPSOAIF_INVOKE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulSoaStatus
    );

typedef  BOOL
(*PFN_BSPSOAIF_IS_BIOBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_BSPSOAIF_WRITE_ASTR)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pAsciiStr
    );

typedef  ANSC_STATUS
(*PFN_BSPSOAIF_WRITE_BSTR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBinaryStr,
        ULONG                       ulStrLen
    );

typedef  BOOL
(*PFN_BSPSOAIF_INTERRUPTED)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * BSPENG SOA interface is defined to separate the core BSP engine and the script
 * execution environment.Thus, BSP engine can be implemented as module-independent.
 */
#define  BSPENG_SOA_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BSPSOAIF_GET_OBJ            GetSlapObject;                                          \
    PFN_BSPSOAIF_INVOKE             InvokeObject;                                           \
                                                                                            \
    PFN_BSPSOAIF_IS_BIOBJ           IsBuiltInObj;                                           \
    PFN_BSPSOAIF_WRITE_ASTR         WriteAString;                                           \
    PFN_BSPSOAIF_WRITE_BSTR         WriteBString;                                           \
                                                                                            \
    PFN_BSPSOAIF_GET_PAGE           GetCookedPage;                                          \
    PFN_BSPSOAIF_RET_PAGE           RetCookedPage;                                          \
    PFN_BSPSOAIF_GET_CP_DATA        GetCookedPageData;                                      \
                                                                                            \
    PFN_BSPSOAIF_INTERRUPTED        IsInterrupted;                                          \
    /* end of object class content */                                                       \

typedef  struct
_BSPENG_SOA_INTERFACE
{
    BSPENG_SOA_INTERFACE_CLASS_CONTENT
}
BSPENG_SOA_INTERFACE,  *PBSPENG_SOA_INTERFACE;

#define  ACCESS_BSPENG_SOA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BSPENG_SOA_INTERFACE, Linkage)


#endif

