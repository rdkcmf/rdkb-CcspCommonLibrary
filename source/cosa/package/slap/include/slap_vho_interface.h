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

    module:	slap_vho_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Var Helper Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/24/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_VHO_INTERFACE_
#define  _SLAP_VHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
          SLAP STANDARD VAR HELPER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPVHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVHO_ALLOC_ARRAY1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVHO_ALLOC_ARRAY12)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       var_count
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVHO_ALLOC_ARRAY2)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

typedef  BOOL
(*PFN_SLAPVHO_EQUAL_ARRAYS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array1,
        ANSC_HANDLE                 slap_array2
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVHO_CLONE_ARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    );

typedef  void
(*PFN_SLAPVHO_CLEAN_ARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    );

typedef  ULONG
(*PFN_SLAPVHO_IMCP_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 variable
    );

typedef  void
(*PFN_SLAPVHO_CONV_FROM_IMCP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 imcp_variable,
        ANSC_HANDLE                 slap_variable
    );

typedef  void
(*PFN_SLAPVHO_CONV_TO_IMCP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 imcp_variable
    );

typedef  ULONG
(*PFN_SLAPVHO_CONV_FROM_SYSR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       record_type
    );

typedef  ULONG
(*PFN_SLAPVHO_CONV_TO_SYSR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       slap_syntax
    );

typedef  ULONG
(*PFN_SLAPVHO_CONV_FROM_MOAM)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       moam_type
    );

typedef  ULONG
(*PFN_SLAPVHO_CONV_FROM_MIB2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mib2_syntax
    );

typedef  void
(*PFN_SLAPVHO_CONV_FROM_MIB2_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 mib2_variable,
        ANSC_HANDLE                 slap_variable
    );

typedef  void
(*PFN_SLAPVHO_CONV_TO_MIB2_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 mib2_variable,
        ULONG                       mib2_syntax
    );

typedef  BOOL
(*PFN_SLAPVHO_EQUAL_VARS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable1,
        ANSC_HANDLE                 slap_variable2
    );

typedef  void
(*PFN_SLAPVHO_CLONE_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_variable,
        ANSC_HANDLE                 dst_variable
    );

typedef  void
(*PFN_SLAPVHO_CLEAN_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable
    );

typedef  void
(*PFN_SLAPVHO_SHIFT_PLIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list,
        ULONG                       tbs_num
    );

typedef  BOOL
(*PFN_SLAPVHO_EQUAL_PLISTS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list1,
        ANSC_HANDLE                 param_list2,
        BOOL                        b_syntax_only
    );

typedef  void
(*PFN_SLAPVHO_INIT_PLIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    );

typedef  void
(*PFN_SLAPVHO_CLONE_PLIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_param_list,
        ANSC_HANDLE*                dst_param_list
    );

typedef  void
(*PFN_SLAPVHO_CLEAN_PLIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    );

/*
 * The SLAP Var Helper Object is implemented to reduce the code size. All its memory functions
 * are converted from previously-defined inline functions and macros. Because SLAP variable manipu-
 * lation is a popular operation in many modules, such optimization has significant benefit.
 */
#define  SLAP_VAR_HELPER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocBoolArray;                                         \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocBoolArrayFromImcpVar;                              \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocCharArray;                                         \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocCharArrayFromImcpVar;                              \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocIntArray;                                          \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocIntArrayFromImcpVar;                               \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocStringArray;                                       \
    PFN_SLAPVHO_ALLOC_ARRAY12       AllocStringArray2;                                      \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocStringArrayFromImcpVar;                            \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocUcharArray;                                        \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocUcharArrayFromImcpVar;                             \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocUint32Array;                                       \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocUint32ArrayFromImcpVar;                            \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocPtrArray;                                          \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocPtrArrayFromImcpVar;                               \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocHandleArray;                                       \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocHandleArrayFromImcpVar;                            \
    PFN_SLAPVHO_ALLOC_ARRAY1        AllocObjectArray;                                       \
    PFN_SLAPVHO_ALLOC_ARRAY2        AllocObjectArrayFromImcpVar;                            \
    PFN_SLAPVHO_EQUAL_ARRAYS        EqualVarArrays;                                         \
    PFN_SLAPVHO_CLONE_ARRAY         CloneVarArray;                                          \
    PFN_SLAPVHO_CLEAN_ARRAY         CleanVarArray;                                          \
                                                                                            \
    PFN_SLAPVHO_IMCP_GET_SIZE       ImcpGetVarSize;                                         \
    PFN_SLAPVHO_CONV_FROM_IMCP      ConvFromImcpVar;                                        \
    PFN_SLAPVHO_CONV_TO_IMCP        ConvToImcpVar;                                          \
    PFN_SLAPVHO_CONV_FROM_SYSR      ConvFromSysRecordType;                                  \
    PFN_SLAPVHO_CONV_TO_SYSR        ConvToSysRecordType;                                    \
    PFN_SLAPVHO_CONV_FROM_MOAM      ConvFromMoamType;                                       \
    PFN_SLAPVHO_CONV_FROM_MIB2      ConvFromMib2Syntax;                                     \
    PFN_SLAPVHO_CONV_FROM_MIB2_VAR  ConvFromMib2Var;                                        \
    PFN_SLAPVHO_CONV_TO_MIB2_VAR    ConvToMib2Var;                                          \
                                                                                            \
    PFN_SLAPVHO_EQUAL_VARS          EqualVariables;                                         \
    PFN_SLAPVHO_CLONE_VAR           CloneVariable;                                          \
    PFN_SLAPVHO_CLEAN_VAR           CleanVariable;                                          \
                                                                                            \
    PFN_SLAPVHO_SHIFT_PLIST         ShiftLeftParamList;                                     \
    PFN_SLAPVHO_SHIFT_PLIST         ShiftRightParamList;                                    \
    PFN_SLAPVHO_EQUAL_PLISTS        EqualParamLists;                                        \
    PFN_SLAPVHO_INIT_PLIST          InitParamList;                                          \
    PFN_SLAPVHO_CLONE_PLIST         CloneParamList;                                         \
    PFN_SLAPVHO_CLEAN_PLIST         CleanParamList;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_VAR_HELPER_OBJECT
{
    SLAP_VAR_HELPER_CLASS_CONTENT
}
SLAP_VAR_HELPER_OBJECT,  *PSLAP_VAR_HELPER_OBJECT;

#define  ACCESS_SLAP_VAR_HELPER_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_VAR_HELPER_OBJECT, Linkage)


#endif
