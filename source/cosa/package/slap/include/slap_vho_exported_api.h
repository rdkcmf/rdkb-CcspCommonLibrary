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

    module:	slap_vho_exported_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Slap Var Helper
        Object.

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


#ifndef  _SLAP_VHO_EXPORTED_API_
#define  _SLAP_VHO_EXPORTED_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_VHO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
SlapCreateVarHelper
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SLAP_VHO_BASE.C
***********************************************************/

ANSC_HANDLE
SlapVhoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
SlapVhoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapVhoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapVhoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_VHO_ARRAY.C
***********************************************************/

ANSC_HANDLE
SlapVhoAllocBoolArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocBoolArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocCharArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocCharArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocIntArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocIntArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocStringArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocStringArray2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       var_count
    );

ANSC_HANDLE
SlapVhoAllocStringArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocUcharArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocUint32Array
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocUint32ArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocPtrArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocPtrArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocHandleArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocHandleArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

ANSC_HANDLE
SlapVhoAllocObjectArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    );

ANSC_HANDLE
SlapVhoAllocObjectArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    );

BOOL
SlapVhoEqualVarArrays
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array1,
        ANSC_HANDLE                 slap_array2
    );

ANSC_HANDLE
SlapVhoCloneVarArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    );

void
SlapVhoCleanVarArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SLAP_VHO_CONVERSION.C
***********************************************************/

ULONG
SlapVhoImcpGetVarSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 variable
    );

void
SlapVhoConvFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 imcp_variable,
        ANSC_HANDLE                 slap_variable
    );

void
SlapVhoConvToImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 imcp_variable
    );

ULONG
SlapVhoConvFromSysRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       record_type
    );

ULONG
SlapVhoConvToSysRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       slap_syntax
    );

ULONG
SlapVhoConvFromMoamType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       moam_type
    );

ULONG
SlapVhoConvFromMib2Syntax
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mib2_syntax
    );

void
SlapVhoConvFromMib2Var
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 mib2_variable,
        ANSC_HANDLE                 slap_variable
    );

void
SlapVhoConvToMib2Var
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 mib2_variable,
        ULONG                       mib2_syntax
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_VHO_VARIABLE.C
***********************************************************/

BOOL
SlapVhoEqualVariables
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable1,
        ANSC_HANDLE                 slap_variable2
    );

void
SlapVhoCloneVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_variable,
        ANSC_HANDLE                 dst_variable
    );

void
SlapVhoCleanVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_VHO_PLIST.C
***********************************************************/

void
SlapVhoShiftLeftParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list,
        ULONG                       tbs_num
    );

void
SlapVhoShiftRightParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list,
        ULONG                       tbs_num
    );

BOOL
SlapVhoEqualParamLists
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 list1,
        ANSC_HANDLE                 list2,
        BOOL                        b_syntax_only
    );

void
SlapVhoInitParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    );

void
SlapVhoCloneParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_param_list,
        ANSC_HANDLE*                dst_param_list
    );

void
SlapVhoCleanParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    );


#endif
