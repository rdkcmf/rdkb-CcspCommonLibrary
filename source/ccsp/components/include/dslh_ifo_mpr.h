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

    module:	dslh_ifo_mpr.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Dslh Management Parameter Register Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/14/2005    initial revision.
        01/06/2011    Bin added "ResetNotifyParam" to ignore change from ACS.
        02/07/2011    Bin added "UnregisterObject" support;
        02/28/2011    Bin added a set of ReqCounter apis to better support dynamic tables;
 
**********************************************************************/


#ifndef  _DSLH_IFO_MPR_
#define  _DSLH_IFO_MPR_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
   DSLH MANAGEMENT PARAMETER REGISTER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  DSLH_MPR_INTERFACE_NAME                    "dslhManagementParameterRegisterIf"
#define  DSLH_MPR_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_REG_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamDescr,
        char*                       pObjName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_REG_PARAMN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        BOOL                        bActiveNotification
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_DEL_PARAMN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_MOD_PARAMN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        ULONG                       ulLimit
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_REG_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_UNREG_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjFullName,
        char*                       pLastName,
        BOOL                        bIsTable
    );

typedef  ULONG
(*PFN_DSLHMPRIF_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_OBJEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_VARRO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_VARRO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_OBJRO_INS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  SLAP_VARIABLE*
(*PFN_DSLHMPRIF_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  SLAP_VARIABLE*
(*PFN_DSLHMPRIF_GET_VALUE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE3)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  char*
(*PFN_DSLHMPRIF_GET_STR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_DSLHMPRIF_GET_VALUE_STR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_STR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

typedef  int
(*PFN_DSLHMPRIF_GET_VALUE_INT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_INT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        int                         iParamValue
    );

typedef  ULONG
(*PFN_DSLHMPRIF_GET_VALUE_UINT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_UINT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        ULONG                       ulParamValue
    );

typedef  BOOL
(*PFN_DSLHMPRIF_GET_VALUE_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        BOOL                        bParamValue
    );

typedef  char*
(*PFN_DSLHMPRIF_GET_VALUE_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

typedef  char*
(*PFN_DSLHMPRIF_GET_VALUE_BASE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_VALUE_BASE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

typedef  ULONG
(*PFN_DSLHMPRIF_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord
    );

typedef  ANSC_HANDLE
(*PFN_DSLHMPRIF_GET_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord,
        ULONG                       ulIndex
    );

typedef  SLAP_VARIABLE*
(*PFN_DSLHMPRIF_GET_ENTRY_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_SET_ENTRY_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_APPLY_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord
    );

typedef  ULONG
(*PFN_DSLHMPRIF_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_INC_COUNTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPRIF_REG_EMPTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

/*
 * Parameter names used a hierarchical form similar to a directory tree. The name of a particular
 * Parameter is represented by the concatenation of each successive node in the hierarchical sepa-
 * rated with a '.', starting at the trunk of the hierarchy and leading to the leaves. When specify-
 * ing a partial path, indicating an intermediate node in the hierarchy, the trailing '.' is always
 * used as the last character.
 */
#define  DSLH_MPR_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_DSLHMPRIF_REG_PARAM         RegParameter;                                           \
    PFN_DSLHMPRIF_REG_PARAMN        RegNotifyParam;                                         \
    PFN_DSLHMPRIF_DEL_PARAMN        DelNotifyParam;                                         \
    PFN_DSLHMPRIF_MOD_PARAMN        ModNotifyParam;                                         \
    PFN_DSLHMPRIF_DEL_PARAMN        ResetNotifyParam;                                       \
    PFN_DSLHMPRIF_REG_OBJ           RegObject;                                              \
    PFN_DSLHMPRIF_UNREG_OBJ         UnregisterObject;                                       \
    PFN_DSLHMPRIF_GET_TYPE          GetVarDataType;                                         \
    PFN_DSLHMPRIF_GET_OBJEO         GetObjEntity;                                           \
    PFN_DSLHMPRIF_GET_VARRO         GetVarRecord;                                           \
    PFN_DSLHMPRIF_GET_VARRO2        GetVarRecord2;                                          \
    PFN_DSLHMPRIF_GET_OBJRO         GetObjRecord;                                           \
    PFN_DSLHMPRIF_GET_OBJRO_INS     GetObjRecordInstance;                                   \
                                                                                            \
    PFN_DSLHMPRIF_GET_VALUE         GetParamValue;                                          \
    PFN_DSLHMPRIF_GET_VALUE2        GetParamValue2;                                         \
    PFN_DSLHMPRIF_SET_VALUE         SetParamValue;                                          \
    PFN_DSLHMPRIF_SET_VALUE         SetParamValue2;                                         \
    PFN_DSLHMPRIF_SET_VALUE3        SetParamValue3;                                         \
    PFN_DSLHMPRIF_SET_VALUE3        SetParamValue4;                                         \
    PFN_DSLHMPRIF_GET_VALUE_STR     GetParamValueString;                                    \
    PFN_DSLHMPRIF_SET_VALUE_STR     SetParamValueString;                                    \
    PFN_DSLHMPRIF_GET_VALUE_INT     GetParamValueInt;                                       \
    PFN_DSLHMPRIF_SET_VALUE_INT     SetParamValueInt;                                       \
    PFN_DSLHMPRIF_GET_VALUE_UINT    GetParamValueUint;                                      \
    PFN_DSLHMPRIF_SET_VALUE_UINT    SetParamValueUint;                                      \
    PFN_DSLHMPRIF_GET_VALUE_BOOL    GetParamValueBool;                                      \
    PFN_DSLHMPRIF_SET_VALUE_BOOL    SetParamValueBool;                                      \
    PFN_DSLHMPRIF_GET_VALUE_TIME    GetParamValueTime;                                      \
    PFN_DSLHMPRIF_SET_VALUE_TIME    SetParamValueTime;                                      \
    PFN_DSLHMPRIF_GET_VALUE_BASE    GetParamValueBase64;                                    \
    PFN_DSLHMPRIF_SET_VALUE_BASE    SetParamValueBase64;                                    \
                                                                                            \
    PFN_DSLHMPRIF_GET_COUNT         GetEntryObjectCount;                                    \
    PFN_DSLHMPRIF_GET_ENTRY         GetEntryObject;                                         \
    PFN_DSLHMPRIF_GET_ENTRY_VALUE   GetEntryParamValue;                                     \
    PFN_DSLHMPRIF_SET_ENTRY_VALUE   SetEntryParamValue;                                     \
    PFN_DSLHMPRIF_APPLY_ENTRY       ApplyEntryChanges;                                      \
                                                                                            \
    PFN_DSLHMPRIF_GET_STR           GetRootObjName;                                         \
    PFN_DSLHMPRIF_GET_ULONG         GetReqCounter;                                          \
    PFN_DSLHMPRIF_INC_COUNTER       IncReqCounter;                                          \
    PFN_DSLHMPRIF_REG_EMPTY         RegEmptyObject;                                         \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_MPR_INTERFACE
{
    DSLH_MPR_INTERFACE_CLASS_CONTENT
}
DSLH_MPR_INTERFACE,  *PDSLH_MPR_INTERFACE;

#define  ACCESS_DSLH_MPR_INTERFACE(p)               \
         ACCESS_CONTAINER(p, DSLH_MPR_INTERFACE, Linkage)


#endif
