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

    module:	bspeng_var_slap_conv.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the string component
        object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/18/03    initial revision.


    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/

#ifndef  _BSPENG_VAR_SLAP_CONV_
#define  _BSPENG_VAR_SLAP_CONV_


/* 
 * These definitions are used to compare a slap variable with others.
 * The reason is some SLAP object methods may return a slap object
 * instead of a simple value. And sometimes, this object may contains
 * a NULL object handle. 
 */
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_VOID         0
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_BOOL    1
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_INT     2
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32  3
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_STRING  4
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_HANDLE  5
#define  BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_REAL    6


typedef  struct 
_BSPENG_SLAP_VAR_COMPARE_VALUE
{
    ULONG                           ulType;

    union 
    {
        BOOL                        boolValue;
        int                         intValue;
        ULONG                       uintValue;
        char                        *stringValue;
        ANSC_HANDLE                 objValue;
    }
    value;
}
BSPENG_SLAP_VAR_COMPARE_VALUE, *PBSPENG_SLAP_VAR_COMPARE_VALUE;

/*
 * Defines some utilities that do conversions between BSP script
 * variables and SLAP variables.
 */

ANSC_STATUS
BspEng_VC_Var2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    );


ANSC_STATUS
BspEng_VC_Slap2Var
    (
        ANSC_HANDLE                 hSlapVar,
        ANSC_HANDLE                 hVar
    );


ANSC_STATUS
BspEng_VC_PrepareSlapParamList
    (
        ANSC_HANDLE                 hSlapParamList,
        ANSC_HANDLE                 hVars,
        ULONG                       ulVars
    );


ANSC_STATUS
BspEng_VC_DeleteSlapParamList
    (
        ANSC_HANDLE                 hSlapParamList
    );


ANSC_STATUS
BspEng_PreparePropertyParamList
    (
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hSlapParamList,
        BOOL                        bGetProperty,
        PUCHAR                      pPropertyName,
        ANSC_HANDLE                 hPropertyValue      /* local script variable */
    );

ANSC_STATUS
BspEng_GetSlapVarCompareValue
    (
        ANSC_HANDLE                 hSlapVar,
        ANSC_HANDLE                 hCompareValue
    );

ANSC_STATUS
BspEng_PrepBspArrayVar
    (
        ANSC_HANDLE                 hArrayVar,
        ULONG                       ulItems,
        BSP_TEMPLATE_ARRAY_TYPE     arrayItemType
    );

ANSC_STATUS
BspEng_VC_VarArray2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    );

ANSC_STATUS
BspEng_VC_BulkData2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    );

ULONG
BspEng_VC_GetBulkDataByteSize
    (
        ULONG                       ulItems,
        BSP_TEMPLATE_ARRAY_TYPE     arrayItemType
    );

#endif

