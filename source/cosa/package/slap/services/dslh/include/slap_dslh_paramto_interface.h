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

    module:	slap_dslh_paramto_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Slap Dslh ParamTree Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        09/19/05    initial revision.
		11/17/06    Bin Zhu added new apis to support GUI;
        03/22/10    Bin added api "GetParamInfoFromCLI" to support CLI;
		10/06/11	Ported to CCSP Cosa framework.

*********************************************************************/


#ifndef  _SLAP_DSLH_PARAMTO_INTERFACE_
#define  _SLAP_DSLH_PARAMTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"
#include "slap_definitions.h"


/***********************************************************
       SLAP DSLH PARAMTREE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  SLAP_DSLHPARAMTO_UOA_NAME                  "Services.Dslh.ParamTree"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_SLAPDSLHPARAMTO_GET_NAME1)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName,
        SLAP_STRING_ARRAY**			pNameArray
    );

typedef  ULONG
(*PFN_SLAPDSLHPARAMTO_GET_NAMES)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName,
        ULONG                       uNextLevel,
        SLAP_STRING_ARRAY**			pNameArray
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPDSLHPARAMTO_GET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName
    );

typedef ULONG
(*PFN_SLAPDSLHPARAMTO_GET_PARAMI)
	(
		ANSC_HANDLE					hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName,
		ULONG                       uNextLevel,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_INT_ARRAY**			pAccessArray,
		SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray,
		SLAP_STRING_ARRAY**			pAttrArray
	);

typedef ULONG
(*PFN_SLAPDSLHPARAMTO_GET_SHORT)
	(
		ANSC_HANDLE					hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray
	);

typedef ULONG
(*PFN_SLAPDSLHPARAMTO_GET_HIDDEN)
	(
		ANSC_HANDLE					hThisObject,
		char*						subsystem_prefix,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_STRING_ARRAY**			pValueArray
	);

typedef ULONG
(*PFN_SLAPDSLHPARAMTO_GET_OBJINF)
	(
		ANSC_HANDLE					hThisObject,
		char*						subsystem_prefix,
		SLAP_STRING_ARRAY**			pAddableArray,
		SLAP_STRING_ARRAY**			pDeletableAttrArray
	);

typedef void
(*PFN_SLAPDSLHPARAMTO_GET_PVAL)
	(
		ANSC_HANDLE					hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName,
		char**						ppType,
		char**						ppValue
	);


typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_SET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_APPLY)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
        char*                       pObjName
    );

typedef  ULONG
(*PFN_SLAPDSLHPARAMTO_ADD_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
        char*                       pObjName
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_DEL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
        char*                       pObjName
    );

typedef  INT
(*PFN_SLAPDSLHPARAMTO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							priority
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID
    );

typedef  BOOL
(*PFN_SLAPDSLHPARAMTO_IS_STH1)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix
    );

typedef  BOOL
(*PFN_SLAPDSLHPARAMTO_IS_STH2)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_SET_PAR1)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        char*                       pParamValue
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_SET_PAR2)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        int                         iParamValue
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_SET_PAR3)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        ULONG                       ulParamValue
    );

typedef  ANSC_STATUS
(*PFN_SLAPDSLHPARAMTO_SET_PAR4)
    (
        ANSC_HANDLE                 hThisObject,
		char*						subsystem_prefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_BOOL                   bParamValue
    );

typedef  SLAP_STRING
(*PFN_SLAPDSLHPARAMTO_ESCAPE)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_STRING                 pInputString
    );

/*
 * Parameter names used a hierarchical form similar to a directory tree. The name of a particular
 * Parameter is represented by the concatenation of each successive node in the hierarchical sepa-
 * rated with a '.', starting at the trunk of the hierarchy and leading to the leaves. When specify-
 * ing a partial path, indicating an intermediate node in the hierarchy, the trailing '.' is always
 * used as the last character.
 */
#define  SLAP_DSLH_PARAMTREE_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_SLAPDSLHPARAMTO_GET_NAME1   GetChildObjNames;                                       \
    PFN_SLAPDSLHPARAMTO_GET_NAMES   GetParamNames;                                          \
    PFN_SLAPDSLHPARAMTO_GET_PARAM   GetParamValue;                                          \
    PFN_SLAPDSLHPARAMTO_SET_PARAM   SetParamValue;                                          \
    PFN_SLAPDSLHPARAMTO_SET_PAR1    SetParamValueString;                                    \
    PFN_SLAPDSLHPARAMTO_SET_PAR2    SetParamValueInt;                                       \
    PFN_SLAPDSLHPARAMTO_SET_PAR3    SetParamValueUint;                                      \
    PFN_SLAPDSLHPARAMTO_SET_PAR4    SetParamValueBool;                                      \
/*    PFN_SLAPDSLHPARAMTO_SET_PAR1    SetParamValueString2;                                   \
	PFN_SLAPDSLHPARAMTO_SET_PAR2    SetParamValueInt2;                                      \
    PFN_SLAPDSLHPARAMTO_SET_PAR3    SetParamValueUint2;                                     \
    PFN_SLAPDSLHPARAMTO_SET_PAR4    SetParamValueBool2;                                     \
*/    PFN_SLAPDSLHPARAMTO_APPLY       ApplyChanges;                                           \
	PFN_SLAPDSLHPARAMTO_GET_PARAMI  GetParamInfo;											\
    PFN_SLAPDSLHPARAMTO_GET_SHORT	GetParamInfoShort;									\
    PFN_SLAPDSLHPARAMTO_GET_HIDDEN  GetHiddenInfo;											\
	PFN_SLAPDSLHPARAMTO_GET_OBJINF  GetObjectInfo;											\
    PFN_SLAPDSLHPARAMTO_GET_PVAL    GetParamTypeAndValue;                                   \
	PFN_SLAPDSLHPARAMTO_ADD_OBJ     AddObject;                                              \
    PFN_SLAPDSLHPARAMTO_DEL_OBJ     DelObject;                                              \
    PFN_SLAPDSLHPARAMTO_ACQ_ACCESS  AcqWriteAccess;                                         \
    PFN_SLAPDSLHPARAMTO_REL_ACCESS  RelWriteAccess;                                         \
    PFN_SLAPDSLHPARAMTO_IS_STH1     IsParamTreeReadOnly;                                    \
    PFN_SLAPDSLHPARAMTO_IS_STH2     IsParameterReadOnly;                                    \
	PFN_SLAPDSLHPARAMTO_ESCAPE		EscapeXmlString;										\
    /* end of object class content */                                                       \

typedef  struct
_SLAP_DSLH_PARAMTREE_OBJECT
{
    SLAP_DSLH_PARAMTREE_CLASS_CONTENT
}
SLAP_DSLH_PARAMTREE_OBJECT,  *PSLAP_DSLH_PARAMTREE_OBJECT;

#define  ACCESS_SLAP_DSLH_PARAMTREE_OBJECT(p)       \
         ACCESS_CONTAINER(p, SLAP_DSLH_PARAMTREE_OBJECT, Linkage)
#define  ACCESS_SLAP_DSLH_PARAMTREE_FIELD(f)        \
         ACCESS_FIELD_OFFSET(SLAP_DSLH_PARAMTREE_OBJECT, f)


#endif
