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

    module:	dslh_vareo_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Var Entity Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_VAREO_INTERFACE_
#define  _DSLH_VAREO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "dslh_properties.h"


/***********************************************************
         DSLH VAR ENTITY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  DSLH_VAREO_DEF_TOKEN_TABLE_SIZE            8

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHVAREO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHVAREO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_DSLHVAREO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_DSLHVAREO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

typedef  BOOL
(*PFN_DSLHVAREO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  int
(*PFN_DSLHVAREO_GET_INT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_INT)
    (
        ANSC_HANDLE                 hThisObject,
        int                         iType
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_CFG_TABLE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTableSize
    );

typedef  ANSC_HANDLE
(*PFN_DSLHVAREO_GET_TOKEN1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pString
    );

typedef  ANSC_HANDLE
(*PFN_DSLHVAREO_GET_TOKEN2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCode
    );

typedef  ANSC_STATUS
(*PFN_DSLHVAREO_ADD_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pString,
        ULONG                       ulEnumCode
    );

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
#define  DSLH_VAR_ENTITY_CLASS_CONTENT                                                      \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hParentObjEntity;                                       \
    DSLH_CWMP_PARAM_DESCR*          ParamDescr;                                             \
    ULONG                           DataType;                                               \
    ULONG                           Syntax;                                                 \
    ULONG                           ContentType;                                            \
    ULONG                           FormatValue1;                                           \
    ULONG                           FormatValue2;                                           \
    int                             Notification;                                           \
                                                                                            \
    QUEUE_HEADER*                   TokenTable;                                             \
    ULONG                           bsUpdate;                                               \
                                                                                            \
    /* end of object class content */                                                       \


#if 0
    PFN_DSLHVAREO_ACTION            Reset;                                                  \
	PFN_DSLHVAREO_ACTION            Remove;                                                 \
                                                                                            \
    ULONG                           CallEntry_GetValue;                                     \
    ULONG                           CallEntry_TstValue;                                     \
    ULONG                           CallEntry_SetValue;                                     \
                                                                                            \
    PFN_DSLHVAREO_GET_TOKEN1        GetTokenValueByString;                                  \
    PFN_DSLHVAREO_GET_TOKEN2        GetTokenValueByCode;                                    \
    PFN_DSLHVAREO_ADD_TOKEN         AddTokenValue;                                          \
    PFN_DSLHVAREO_CFG_TABLE         CfgTokenTable;                                          \

#endif


typedef  struct
_DSLH_VAR_ENTITY_OBJECT
{
    DSLH_VAR_ENTITY_CLASS_CONTENT
}
DSLH_VAR_ENTITY_OBJECT,  *PDSLH_VAR_ENTITY_OBJECT;

#define  ACCESS_DSLH_VAR_ENTITY_OBJECT(p)           \
         ACCESS_CONTAINER(p, DSLH_VAR_ENTITY_OBJECT, Linkage)


/***********************************************************
        DSLH VAR ENTITY ENUMERATION VALUE DEFINITION
***********************************************************/

/*
 * For strings listed in TR-069 specification, a maximum allowed length may be listed using the
 * form string(N), where N is the maximum string length in characters. For all strings a maximum
 * length is either explicitly indicated or implied by the size of the elements composing the
 * string. For strings in which the content is an enumeration, the longest enumerated value deter-
 * mines the maximum length. If a string does not have an explicitly indicated maximum length or is
 * not an enumeration, the default maximum is 16 characters.
 */
typedef  struct
_DSLH_VAREO_TOKEN_VALUE
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           String;
    ULONG                           EnumCode;
}
DSLH_VAREO_TOKEN_VALUE,  *PDSLH_VAREO_TOKEN_VALUE;

#define  ACCESS_DSLH_VAREO_TOKEN_VALUE(p)           \
         ACCESS_CONTAINER(p, DSLH_VAREO_TOKEN_VALUE, Linkage)

#define  DslhVareoFreeTokenValue(token_value)                                               \
         {                                                                                  \
            if ( token_value->String )                                                      \
            {                                                                               \
                AnscFreeMemory(token_value->String);                                        \
            }                                                                               \
                                                                                            \
            AnscFreeMemory(token_value);                                                    \
         }


#endif
