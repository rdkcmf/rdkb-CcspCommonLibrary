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

    module:	slap_definitions.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Slap Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/03    initial revision.
        04/23/07    Bin added 2 more content type "url" and "subnetmask"
		02/28/08    Bin changed "SLAP_MAX_PARAM_ARRAY_SIZE" from 8 to 64
        09/30/10    Bin added ipv6_list support.

**********************************************************************/


#ifndef  _SLAP_DEFINITIONS_
#define  _SLAP_DEFINITIONS_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif


#include "user_base.h"
#include "ansc_wrapper_base.h"
#include "ansc_common_structures.h"

/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */
#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
         SLAP STANDARD ELEMENT PARAMETERS DEFINITION
***********************************************************/

/*
 * SLAP maintains all the system-created object containers in a one dimensional table sorted by
 * containers' names; while external modules may dynamically create containers in runtime, but
 * these containers may be isolated from the internal management.
 */
#define  SLAP_CONTAINER_TYPE_DEFAULT                0x00000001
#define  SLAP_CONTAINER_TYPE_STANDARD               0x00000002
#define  SLAP_CONTAINER_TYPE_SYSTEM                 0x00000004

/*
 * In BMW 1.0, we introduced the concept of "Rendering Attribute", which can be associated with a
 * folder or record object. This attribute specifies the "Content Type" of the folder or record,
 * and serves as the guideline for rendering engine. Although this feature was incorporated into
 * BISGA instead of BMW 1.0 directly, its existence was primarily to facilitate the GUI interface
 * development by standardizing the rendering process for various data representations. Here, we
 * inherit this feature in SLAP.
 */
#define  SLAP_CONTENT_TYPE_UNSPECIFIED              0
#define  SLAP_CONTENT_TYPE_MAPPED                   1       /* applies to uint32, rendered by the Name Mapper                      */
#define  SLAP_CONTENT_TYPE_IP4_ADDR                 2       /* applies to uint32 and ucharArray, rendered as decimal dotted string */
#define  SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD        3       /* applies to uint32 and ucharArray, rendered as decimal dotted string */
#define  SLAP_CONTENT_TYPE_MAC_ADDR                 4       /* applies to ucharArray, rendered as hex-decimal dotted               */
#define  SLAP_CONTENT_TYPE_IP4_ADDR_LIST            5       /* applies to ucharArray, rendered as an array of IP addresses         */
#define  SLAP_CONTENT_TYPE_MAC_ADDR_LIST            6       /* applies to ucharArray, rendered as an array of MAC addresses        */
#define  SLAP_CONTENT_TYPE_TCPUDP_PORT              7       /* applies to uint32, mandate the range checking on the value          */
#define  SLAP_CONTENT_TYPE_CALENDAR_TIME            8       /* applies to ucharArray, data is structured as ANSC_UNIVERSAL_TIME    */
#define  SLAP_CONTENT_TYPE_PASSWORD                 9       /* applies to string, all the characters should be masked              */
#define  SLAP_CONTENT_TYPE_MASKED                   10      /* applies to uint32, rendered as a set of checkboxes                  */
#define  SLAP_CONTENT_TYPE_OID_LIST                 11      /* applies to uint32Array, data is presented as dotted decimal string  */
#define  SLAP_CONTENT_TYPE_ASCII_STRING             12      /* applies to ucharArray, data is presented as an octet string         */
#define  SLAP_CONTENT_TYPE_HEX_STRING               13      /* applies to ucharArray, data is presented as an octet string         */
#define  SLAP_CONTENT_TYPE_URL_STRING               14      /* applies to string, it will be a full url string                     */
#define  SLAP_CONTENT_TYPE_SUBNET_MASK              15      /* applies to uint32, it will be a valid subnet mask value             */
#define  SLAP_CONTENT_TYPE_IP6_ADDR                 16      /* applies to ucharArray, rendered as a IPv6 address string            */
#define  SLAP_CONTENT_TYPE_IP6_ADDR_LIST            17      /* applies to ucharArray, rendered as an array of IPv6 addresses       */
#define  SLAP_CONTENT_TYPE_IP4_ADDR_RANGE_LIST      18      /* applies to ucharArray, rendered as an array of IPv4 addresses       */


#define  SLAP_CONTENT_TYPE_TABLE_2D                 10001
#define  SLAP_CONTENT_TYPE_TABLE_3D                 10002
#define  SLAP_CONTENT_TYPE_SIMPLE_CHART_PIE         10003
#define  SLAP_CONTENT_TYPE_SIMPLE_CHART_BAR         10004
#define  SLAP_CONTENT_TYPE_SIMPLE_GRAPH             10005

#define  SLAP_CONTENT_NAME_UNSPECIFIED              "unspecified"
#define  SLAP_CONTENT_NAME_MAPPED                   "mapped"
#define  SLAP_CONTENT_NAME_IP4_ADDR                 "ip4_addr"
#define  SLAP_CONTENT_NAME_IP4_ADDR_WILDCARD        "ip4_addr_wildcard"
#define  SLAP_CONTENT_NAME_MAC_ADDR                 "mac_addr"
#define  SLAP_CONTENT_NAME_IP4_ADDR_LIST            "ip4_addr_list"
#define  SLAP_CONTENT_NAME_IP4_ADDR_RANGE_LIST      "ip4_addr_range_list"
#define  SLAP_CONTENT_NAME_MAC_ADDR_LIST            "mac_addr_list"
#define  SLAP_CONTENT_NAME_TCPUDP_PORT              "tcpudp_port"
#define  SLAP_CONTENT_NAME_CALENDAR_TIME            "calendar_time"
#define  SLAP_CONTENT_NAME_PASSWORD                 "password"
#define  SLAP_CONTENT_NAME_MASKED                   "masked"
#define  SLAP_CONTENT_NAME_OID_LIST                 "oid_list"
#define  SLAP_CONTENT_NAME_ASCII_STRING             "ascii_string"
#define  SLAP_CONTENT_NAME_HEX_STRING               "hex_string"
#define  SLAP_CONTENT_NAME_URL_STRING               "url"
#define  SLAP_CONTENT_NAME_SUBNET_MASK              "subnet_mask"
#define  SLAP_CONTENT_NAME_IP6_ADDR                 "ip6_addr"
#define  SLAP_CONTENT_NAME_IP6_ADDR_LIST            "ip6_addr_list"

#define  SLAP_CONTENT_NAME_TABLE_2D                 "table_2d"
#define  SLAP_CONTENT_NAME_TABLE_3D                 "table_3d"
#define  SLAP_CONTENT_NAME_SIMPLE_CHART_PIE         "simple_chart_pie"
#define  SLAP_CONTENT_NAME_SIMPLE_CHART_BAR         "simple_chart_bar"
#define  SLAP_CONTENT_NAME_SIMPLE_GRAPH             "simple_graph"

#define  SLAP_VAR_SYNTAX_other                      SLAP_VAR_SYNTAX_TYPE_other
#define  SLAP_VAR_SYNTAX_void                       SLAP_VAR_SYNTAX_TYPE_void
#define  SLAP_VAR_SYNTAX_bool                       SLAP_VAR_SYNTAX_TYPE_bool
#define  SLAP_VAR_SYNTAX_boolArray                  SLAP_VAR_SYNTAX_TYPE_boolArray
#define  SLAP_VAR_SYNTAX_charArray                  SLAP_VAR_SYNTAX_TYPE_charArray
#define  SLAP_VAR_SYNTAX_int                        SLAP_VAR_SYNTAX_TYPE_int
#define  SLAP_VAR_SYNTAX_intArray                   SLAP_VAR_SYNTAX_TYPE_intArray
#define  SLAP_VAR_SYNTAX_string                     SLAP_VAR_SYNTAX_TYPE_string
#define  SLAP_VAR_SYNTAX_stringArray                SLAP_VAR_SYNTAX_TYPE_stringArray
#define  SLAP_VAR_SYNTAX_ucharArray                 SLAP_VAR_SYNTAX_TYPE_ucharArray
#define  SLAP_VAR_SYNTAX_uint32                     SLAP_VAR_SYNTAX_TYPE_uint32
#define  SLAP_VAR_SYNTAX_uint32Array                SLAP_VAR_SYNTAX_TYPE_uint32Array
#define  SLAP_VAR_SYNTAX_ptr                        SLAP_VAR_SYNTAX_TYPE_ptr
#define  SLAP_VAR_SYNTAX_ptrArray                   SLAP_VAR_SYNTAX_TYPE_ptrArray
#define  SLAP_VAR_SYNTAX_handle                     SLAP_VAR_SYNTAX_TYPE_handle
#define  SLAP_VAR_SYNTAX_handleArray                SLAP_VAR_SYNTAX_TYPE_handleArray
#define  SLAP_VAR_SYNTAX_object                     SLAP_VAR_SYNTAX_TYPE_object
#define  SLAP_VAR_SYNTAX_objectArray                SLAP_VAR_SYNTAX_TYPE_objectArray
#define  SLAP_VAR_SYNTAX_variable                   SLAP_VAR_SYNTAX_TYPE_variable

#define  SLAP_VAR_SYNTAX_TYPE_other                 0
#define  SLAP_VAR_SYNTAX_TYPE_void                  0
#define  SLAP_VAR_SYNTAX_TYPE_bool                  1
#define  SLAP_VAR_SYNTAX_TYPE_boolArray             2
#define  SLAP_VAR_SYNTAX_TYPE_charArray             3
#define  SLAP_VAR_SYNTAX_TYPE_int                   4
#define  SLAP_VAR_SYNTAX_TYPE_intArray              5
#define  SLAP_VAR_SYNTAX_TYPE_string                6
#define  SLAP_VAR_SYNTAX_TYPE_stringArray           7
#define  SLAP_VAR_SYNTAX_TYPE_ucharArray            8
#define  SLAP_VAR_SYNTAX_TYPE_uint32                9
#define  SLAP_VAR_SYNTAX_TYPE_uint32Array           10
#define  SLAP_VAR_SYNTAX_TYPE_ptr                   11
#define  SLAP_VAR_SYNTAX_TYPE_ptrArray              12
#define  SLAP_VAR_SYNTAX_TYPE_handle                13
#define  SLAP_VAR_SYNTAX_TYPE_handleArray           14
#define  SLAP_VAR_SYNTAX_TYPE_object                15
#define  SLAP_VAR_SYNTAX_TYPE_objectArray           16
#define  SLAP_VAR_SYNTAX_TYPE_variable              100

#define  SLAP_VAR_SYNTAX_NAME_other                 "other"
#define  SLAP_VAR_SYNTAX_NAME_void                  "void"
#define  SLAP_VAR_SYNTAX_NAME_bool                  "bool"
#define  SLAP_VAR_SYNTAX_NAME_boolArray             "boolArray"
#define  SLAP_VAR_SYNTAX_NAME_charArray             "charArray"
#define  SLAP_VAR_SYNTAX_NAME_int                   "int"
#define  SLAP_VAR_SYNTAX_NAME_intArray              "intArray"
#define  SLAP_VAR_SYNTAX_NAME_string                "string"
#define  SLAP_VAR_SYNTAX_NAME_stringArray           "stringArray"
#define  SLAP_VAR_SYNTAX_NAME_ucharArray            "ucharArray"
#define  SLAP_VAR_SYNTAX_NAME_uint32                "uint32"
#define  SLAP_VAR_SYNTAX_NAME_uint32Array           "uint32Array"
#define  SLAP_VAR_SYNTAX_NAME_ptr                   "ptr"
#define  SLAP_VAR_SYNTAX_NAME_ptrArray              "ptrArray"
#define  SLAP_VAR_SYNTAX_NAME_handle                "handle"
#define  SLAP_VAR_SYNTAX_NAME_handleArray           "handleArray"
#define  SLAP_VAR_SYNTAX_NAME_object                "object"
#define  SLAP_VAR_SYNTAX_NAME_objectArray           "objectArray"
#define  SLAP_VAR_SYNTAX_NAME_variable              "variable"

#define  SLAP_VAR_USAGE_TYPE_default                0x00000001
#define  SLAP_VAR_USAGE_TYPE_input                  0x00000002
#define  SLAP_VAR_USAGE_TYPE_output                 0x00000004

/*
 * SLAP can be viewed as a playground/environment where programmers can implement the logic for
 * different service modules in any way they want as long as the behavior logic objects conform to
 * the aggregation specification. SLAP can also be viewed as an interface between the lower layer
 * baseline modules and the upper layer control modules. This interface is known as UOA (Universal
 * Object Access). Through this interface, SLAP provides a generic object-access mechansim.
 */
typedef  void           SLAP_VOID,      *PSLAP_VOID;
typedef  ULONG          SLAP_BOOL,      *PSLAP_BOOL;
typedef  char           SLAP_CHAR,      *PSLAP_CHAR;
typedef  char*          SLAP_STRING,    *PSLAP_STRING;
typedef  int            SLAP_INT,       *PSLAP_INT;
typedef  UCHAR          SLAP_UCHAR,     *PSLAP_UCHAR;
typedef  ULONG          SLAP_UINT32,    *PSLAP_UINT32;
typedef  void*          SLAP_PTR,       *PSLAP_PTR;
typedef  ANSC_HANDLE    SLAP_HANDLE,    *PSLAP_HANDLE;
typedef  ANSC_HANDLE    SLAP_OBJECT,    *PSLAP_OBJECT;

typedef  struct
_SLAP_VAR_ARRAY
{
    ULONG                           Size;           /* total number of bytes allocated */
    ULONG                           VarCount;       /* number of elements in the array */
    ULONG                           Syntax;         /* variable syntax of each element */
    union
    {
        SLAP_BOOL                   arrayBool  [ANSC_ZERO_ARRAY_SIZE];
        SLAP_CHAR                   arrayChar  [ANSC_ZERO_ARRAY_SIZE];
        SLAP_INT                    arrayInt   [ANSC_ZERO_ARRAY_SIZE];
        SLAP_STRING                 arrayString[ANSC_ZERO_ARRAY_SIZE];
        SLAP_UCHAR                  arrayUchar [ANSC_ZERO_ARRAY_SIZE];
        SLAP_UINT32                 arrayUint32[ANSC_ZERO_ARRAY_SIZE];
        SLAP_PTR                    arrayPtr   [ANSC_ZERO_ARRAY_SIZE];
        SLAP_HANDLE                 arrayHandle[ANSC_ZERO_ARRAY_SIZE];
        SLAP_OBJECT                 arrayObject[ANSC_ZERO_ARRAY_SIZE];
    }Array;
}_struct_pack_
SLAP_VAR_ARRAY,  *PSLAP_VAR_ARRAY;

#define  SlapAllocBoolArray(var_data, var_length, slap_array)                               \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_BOOL_ARRAY)SlapVhoAllocBoolArray                                     \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocBoolArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)          \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_BOOL_ARRAY)SlapVhoAllocBoolArrayFromImcpVar                          \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocCharArray(var_data, var_length, slap_array)                               \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_CHAR_ARRAY)SlapVhoAllocCharArray                                     \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocCharArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)          \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_CHAR_ARRAY)SlapVhoAllocCharArrayFromImcpVar                          \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocIntArray(var_data, var_length, slap_array)                                \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_INT_ARRAY)SlapVhoAllocIntArray                                       \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocIntArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)           \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_INT_ARRAY)SlapVhoAllocIntArrayFromImcpVar                            \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocStringArray(var_data, var_length, slap_array)                             \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_STRING_ARRAY)SlapVhoAllocStringArray                                 \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocStringArray2(var_count, slap_array)                                       \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_STRING_ARRAY)SlapVhoAllocStringArray2                                \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        var_count                                                           \
                    );                                                                      \
         }

#define  SlapAllocStringArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)        \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_STRING_ARRAY)SlapVhoAllocStringArrayFromImcpVar                      \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocUcharArray(var_data, var_length, slap_array)                              \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_UCHAR_ARRAY)SlapVhoAllocUcharArray                                   \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocUcharArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)         \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_UCHAR_ARRAY)SlapVhoAllocUcharArrayFromImcpVar                        \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocUint32Array(var_data, var_length, slap_array)                             \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_UINT32_ARRAY)SlapVhoAllocUint32Array                                 \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocUint32ArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)        \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_UINT32_ARRAY)SlapVhoAllocUint32ArrayFromImcpVar                      \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocPtrArray(var_data, var_length, slap_array)                                \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_PTR_ARRAY)SlapVhoAllocPtrArray                                       \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocPtrArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)           \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_PTR_ARRAY)SlapVhoAllocPtrArrayFromImcpVar                            \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocHandleArray(var_data, var_length, slap_array)                             \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_HANDLE_ARRAY)SlapVhoAllocHandleArray                                 \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocHandleArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)        \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_HANDLE_ARRAY)SlapVhoAllocHandleArrayFromImcpVar                      \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapAllocObjectArray(var_data, var_length, slap_array)                             \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_OBJECT_ARRAY)SlapVhoAllocObjectArray                                 \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )var_data,                                              \
                        var_length                                                          \
                    );                                                                      \
         }

#define  SlapAllocObjectArrayFromImcpVar(imcp_var_data, imcp_var_length, slap_array)        \
         {                                                                                  \
            slap_array =                                                                    \
                (PSLAP_OBJECT_ARRAY)SlapVhoAllocObjectArrayFromImcpVar                      \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (PUCHAR     )imcp_var_data,                                         \
                        imcp_var_length                                                     \
                    );                                                                      \
         }

#define  SlapEqualVarArrays(array1, array2, b_equal_array)                                  \
         {                                                                                  \
            b_equal_array =                                                                 \
                SlapVhoEqualVarArrays                                                       \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (ANSC_HANDLE)array1,                                                \
                        (ANSC_HANDLE)array2                                                 \
                    );                                                                      \
         }

#define  SlapCloneVarArray(src_array, dst_array)                                            \
         {                                                                                  \
            dst_array =                                                                     \
                (PSLAP_VAR_ARRAY)SlapVhoCloneVarArray                                       \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        (ANSC_HANDLE)src_array                                              \
                    );                                                                      \
         }

#define  SlapCleanVarArray(tbc_array)               SlapVhoCleanVarArray((ANSC_HANDLE)NULL, tbc_array)
#define  SlapFreeVarArray(tbf_array)                                                        \
         {                                                                                  \
            SlapCleanVarArray(tbf_array);                                                   \
            AnscFreeMemory   (tbf_array);                                                   \
         }

typedef  struct  _SLAP_VAR_ARRAY  SLAP_BOOL_ARRAY,    *PSLAP_BOOL_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_CHAR_ARRAY,    *PSLAP_CHAR_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_STRING_ARRAY,  *PSLAP_STRING_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_INT_ARRAY,     *PSLAP_INT_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_UCHAR_ARRAY,   *PSLAP_UCHAR_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_UINT32_ARRAY,  *PSLAP_UINT32_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_PTR_ARRAY,     *PSLAP_PTR_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_HANDLE_ARRAY,  *PSLAP_HANDLE_ARRAY;
typedef  struct  _SLAP_VAR_ARRAY  SLAP_OBJECT_ARRAY,  *PSLAP_OBJECT_ARRAY;

/*
 * The definition of SLAP_VARIABLE is a big union of all possible data types that are exchanged
 * between the SLAP client modules and SLAP service objects. This list MUST be expressive and
 * complete since it will be used to describe the 'invoke parameters' and 'return values' of the
 * UOA interface.
 */
typedef  struct
_SLAP_VARIABLE
{
    char*                           Name;                   /* reserved for future use              */
    ULONG                           ContentType;            /* the content type of the variable     */
    ULONG                           UsageType;              /* indicate usage occasion, rarely used */
    ULONG                           Syntax;                 /* one of the above syntax values       */
	ULONG                           ReqSenderID;  
    union
    {
        SLAP_BOOL                   varBool;
        SLAP_BOOL_ARRAY*            varBoolArray;
        SLAP_CHAR_ARRAY*            varCharArray;
        SLAP_INT                    varInt;
        SLAP_INT_ARRAY*             varIntArray;
        SLAP_CHAR*                  varString;
        SLAP_STRING_ARRAY*          varStringArray;
        SLAP_UCHAR_ARRAY*           varUcharArray;
        SLAP_UINT32                 varUint32;
        SLAP_UINT32_ARRAY*          varUint32Array;
        SLAP_PTR                    varPtr;
        SLAP_PTR_ARRAY*             varPtrArray;
        SLAP_HANDLE                 varHandle;
        SLAP_HANDLE_ARRAY*          varHandleArray;
        SLAP_OBJECT                 varObject;
        SLAP_OBJECT_ARRAY*          varObjectArray;
    }Variant;
}
SLAP_VARIABLE,  *PSLAP_VARIABLE;

#define  SLAP_TRUE                                  0xFFFFFFFF      /* ensure the conversion between SLAP_BOOL and BOOL is correct */
#define  SLAP_FALSE                                 0

#define  SlapMib2ResolveVarIndex                    SlapMib2DboResolveVarIndex
#define  SlapMib2ConvertMibValue                    SlapMib2DboConvertMibValue

#define  SlapResolveInvokeParam(method_descr, input_param_list, index)                      \
         (method_descr->ParamListI->ParamArray[index].Syntax == SLAP_VAR_SYNTAX_variable)? &input_param_list->ParamArray[index] : input_param_list->ParamArray[index].Variant.varBool

#define  SlapImcpGetVarSize(slap_var, imcp_var_size)                                        \
         {                                                                                  \
            imcp_var_size = SlapVhoImcpGetVarSize((ANSC_HANDLE)NULL, slap_var);             \
         }

#define  SlapConvFromImcpVar(imcp_var, slap_var)                                            \
         {                                                                                  \
            SlapVhoConvFromImcpVar((ANSC_HANDLE)NULL, imcp_var, slap_var);                  \
         }

#define  SlapConvToImcpVar(slap_var, imcp_var)                                              \
         {                                                                                  \
            SlapVhoConvToImcpVar((ANSC_HANDLE)NULL, slap_var, imcp_var);                    \
         }

#define  SlapConvFromSysRecordType(record_type, slap_syntax)                                \
         {                                                                                  \
            slap_syntax = SlapVhoConvFromSysRecordType((ANSC_HANDLE)NULL, record_type);     \
         }

#define  SlapConvToSysRecordType(slap_syntax, record_type)                                  \
         {                                                                                  \
            record_type = SlapVhoConvToSysRecordType((ANSC_HANDLE)NULL, slap_syntax);       \
         }

#define  SlapConvFromMoamType(moam_type, slap_syntax)                                       \
         {                                                                                  \
            slap_syntax = SlapVhoConvFromMoamType((ANSC_HANDLE)NULL, moam_type);            \
         }

#define  SlapConvFromMib2Syntax(mib2_syntax, slap_syntax)                                   \
         {                                                                                  \
            slap_syntax = SlapVhoConvFromMib2Syntax((ANSC_HANDLE)NULL, mib2_syntax);        \
         }

#define  SlapConvFromMib2Var(mib2_var, slap_var)                                            \
         {                                                                                  \
            SlapVhoConvFromMib2Var((ANSC_HANDLE)NULL, mib2_var, slap_var);                  \
         }

#define  SlapConvToMib2Var(slap_var, mib2_var, mib2_syntax)                                 \
         {                                                                                  \
            SlapVhoConvToMib2Var((ANSC_HANDLE)NULL, slap_var, mib2_var, mib2_syntax);       \
         }

#define  SlapEqualVariables(var1, var2, b_equal_var)                                        \
         {                                                                                  \
            b_equal_var = SlapVhoEqualVariables((ANSC_HANDLE)NULL, var1, var2);             \
         }

#define  SlapAllocVariable(new_var)                                                         \
         {                                                                                  \
            new_var = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));            \
                                                                                            \
            if ( new_var )                                                                  \
            {                                                                               \
                SlapInitVariable(new_var);                                                  \
            }                                                                               \
         }

#define  SlapInitVariable(tbi_var)                                                          \
         {                                                                                  \
            PSLAP_VARIABLE tmp = (PSLAP_VARIABLE)tbi_var;                                              \
            tmp->Name              = NULL;                                              \
            tmp->ContentType       = SLAP_CONTENT_TYPE_UNSPECIFIED;                     \
            tmp->UsageType         = 0;                                                 \
            tmp->Syntax            = SLAP_VAR_SYNTAX_other;                             \
            tmp->Variant.varUint32 = 0;                                                 \
         }

#define  SlapCloneVariable(src_var, dst_var)                                                \
         {                                                                                  \
            SlapVhoCloneVariable((ANSC_HANDLE)NULL, src_var, dst_var);                      \
         }

#define  SlapCleanVariable(tbc_var)                                                         \
         {                                                                                  \
            SlapVhoCleanVariable((ANSC_HANDLE)NULL, tbc_var);                               \
         }

#define  SlapFreeVariable(tbf_var)                                                          \
         {                                                                                  \
            SlapCleanVariable(tbf_var);                                                     \
            AnscFreeMemory   (tbf_var);                                                     \
         }

/*
 * Following advanced macros help BMW and BMC objects to convert between GOA, UOA, LOA, and DOA
 * variables. It's dangerous for other modules to use those macros because it requires deep under-
 * standing of the internal implementation of SLAP.
 */
#define  SlapGoaObjToUoaObj(goa_obj)                \
         (SLAP_OBJECT)(((PSLAP_OBJ_WRAPPER_OBJECT)goa_obj)->GetWrappedObj((ANSC_HANDLE)goa_obj))


/***********************************************************
        SLAP UNIVERSAL INPUT/OUT PARAMETERS DEFINITION
***********************************************************/

/*
 * On top of the above 'universal' variable definition, we now can define standard input/output
 * parameters structure that will be used to describe the input and out of the member functions of
 * typical SLAP components.
 */
#define  SLAP_MAX_PARAM_ARRAY_SIZE                  64

typedef  struct
_SLAP_PARAMETER_LIST
{
    ULONG                           ParamCount;
    SLAP_VARIABLE                   ParamArray[SLAP_MAX_PARAM_ARRAY_SIZE];
}
SLAP_PARAMETER_LIST,  *PSLAP_PARAMETER_LIST;

#define  SlapShiftLeftParamList(tbs_list, tbs_num)                                          \
         {                                                                                  \
            SlapVhoShiftLeftParamList((ANSC_HANDLE)NULL, tbs_list, tbs_num);                \
         }

#define  SlapShiftRightParamList(tbs_list, tbs_num)                                         \
         {                                                                                  \
            SlapVhoShiftRightParamList((ANSC_HANDLE)NULL, tbs_list, tbs_num);               \
         }

#define  SlapEqualParamLists(param_list1, param_list2, b_syntax_only, b_equal_pl)           \
         {                                                                                  \
            b_equal_pl =                                                                    \
                SlapVhoEqualParamLists                                                      \
                    (                                                                       \
                        (ANSC_HANDLE)NULL,                                                  \
                        param_list1,                                                        \
                        param_list2,                                                        \
                        b_syntax_only                                                       \
                    );                                                                      \
         }

#define  SlapInitParamList(tbi_list)                                                        \
         {                                                                                  \
            SlapVhoInitParamList((ANSC_HANDLE)NULL, tbi_list);                              \
         }

#define  SlapCloneParamList(src_list, dst_list)                                             \
         {                                                                                  \
            SlapVhoCloneParamList((ANSC_HANDLE)NULL, src_list, &dst_list);                  \
         }

#define  SlapCleanParamList(tbc_list)                                                       \
         {                                                                                  \
            SlapVhoCleanParamList((ANSC_HANDLE)NULL, tbc_list);                             \
         }

#define  SlapFreeParamList(tbf_list)                                                        \
         {                                                                                  \
            SlapCleanParamList(tbf_list);                                                   \
            AnscFreeMemory    (tbf_list);                                                   \
         }


/***********************************************************
         SLAP STANDARD OBJECT DESCRIPTOR DEFINITION
***********************************************************/

/*
 * One essential part of the generic object-access service provided by the SLAP is runtime object
 * registration. SLAP itself doesn't implement any service objects, instead it merely serves as a
 * place holder for object aggregation and provides object management functionality. SLAP defines
 * following data structure for object registration.
 */
#define  SLAP_OBJ_TYPE_standard                     0x00000001
#define  SLAP_OBJ_TYPE_collection                   0x00000002
#define  SLAP_OBJ_TYPE_gallery                      0x00000004
#define  SLAP_OBJ_TYPE_buffer                       0x00000008
#define  SLAP_OBJ_TYPE_serializedCalls              0x00010000
#define  SLAP_OBJ_TYPE_poolable                     0x00020000
#define  SLAP_OBJ_TYPE_freelance                    0x00040000
#define  SLAP_OBJ_TYPE_mobile                       0x00080000

#define  SLAP_OBJ_TYPE_scoStandard                  0x00000001
#define  SLAP_OBJ_TYPE_scoCollection                0x00000003
#define  SLAP_OBJ_TYPE_scoGallery                   0x00000007
#define  SLAP_OBJ_TYPE_scoBuffer                    0x00000009

#define  SLAP_OBJ_PATH_SEPARATORS                   "./"

#define  SLAP_OBJ_DEF_OPO_TABLE_SIZE                16
#define  SLAP_OBJ_DEF_MDO_TABLE_SIZE                16

typedef  struct
_SLAP_OBJECT_DESCRIPTOR
{
    char*                           ObjName;
    ULONG                           ObjType;
    void*                           ObjConstructor;

    ULONG                           ObjOpoTableSize;
    ULONG                           ObjMdoTableSize;
    void*                           ObjCallMap;
}
SLAP_OBJECT_DESCRIPTOR,  *PSLAP_OBJECT_DESCRIPTOR;

#define  SlapCloneObjDescriptor(src_descr, dst_descr)                                       \
         {                                                                                  \
            if ( !dst_descr )                                                               \
            {                                                                               \
                dst_descr =                                                                 \
                    (PSLAP_OBJECT_DESCRIPTOR)AnscAllocateMemory                             \
                        (                                                                   \
                            sizeof(SLAP_OBJECT_DESCRIPTOR)                                  \
                        );                                                                  \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                SlapCleanObjDescriptor(dst_descr);                                          \
            }                                                                               \
                                                                                            \
            if ( dst_descr )                                                                \
            {                                                                               \
                dst_descr->ObjName         = AnscCloneString(src_descr->ObjName);           \
                dst_descr->ObjType         = src_descr->ObjType;                            \
                dst_descr->ObjConstructor  = src_descr->ObjConstructor;                     \
                                                                                            \
                dst_descr->ObjOpoTableSize = src_descr->ObjOpoTableSize;                    \
                dst_descr->ObjMdoTableSize = src_descr->ObjMdoTableSize;                    \
                dst_descr->ObjCallMap      = src_descr->ObjCallMap;                         \
            }                                                                               \
         }

#define  SlapCleanObjDescriptor(tbc_descr)                                                  \
         {                                                                                  \
            if ( tbc_descr->ObjName )                                                       \
            {                                                                               \
                AnscFreeMemory(tbc_descr->ObjName);                                         \
                                                                                            \
                tbc_descr->ObjName = NULL;                                                  \
            }                                                                               \
         }

#define  SlapFreeObjDescriptor(tbf_descr)                                                   \
         {                                                                                  \
            SlapCleanObjDescriptor(tbf_descr);                                              \
            AnscFreeMemory        (tbf_descr);                                              \
         }


/***********************************************************
    SLAP OBJECT PROPERTY AND METHOD DESCRIPTOR DEFINITION
***********************************************************/

/*
 * In attempt to generalize and simplify the interaction between SLAP and the modules that use the
 * services provided by SLAP, we introduce the concept of "Object Property", "Method Alias" and
 * "Method Descriptor". A SLAP Srv Component Object may be associated with multiple "Properties",
 * which can be accessed (read/write) by invoking two predefined methods: GetProperty/SetProperty.
 */
typedef  SLAP_BOOL           (*PFN_SLAPOBJ_GET_BOOL        )(ANSC_HANDLE  hThisObject);
typedef  SLAP_BOOL_ARRAY*    (*PFN_SLAPOBJ_GET_BOOL_ARRAY  )(ANSC_HANDLE  hThisObject);
typedef  SLAP_CHAR           (*PFN_SLAPOBJ_GET_CHAR        )(ANSC_HANDLE  hThisObject);
typedef  SLAP_CHAR_ARRAY*    (*PFN_SLAPOBJ_GET_CHAR_ARRAY  )(ANSC_HANDLE  hThisObject);
typedef  SLAP_INT            (*PFN_SLAPOBJ_GET_INT         )(ANSC_HANDLE  hThisObject);
typedef  SLAP_INT_ARRAY*     (*PFN_SLAPOBJ_GET_INT_ARRAY   )(ANSC_HANDLE  hThisObject);
typedef  SLAP_STRING         (*PFN_SLAPOBJ_GET_STRING      )(ANSC_HANDLE  hThisObject);
typedef  SLAP_STRING_ARRAY*  (*PFN_SLAPOBJ_GET_STRING_ARRAY)(ANSC_HANDLE  hThisObject);
typedef  SLAP_UCHAR          (*PFN_SLAPOBJ_GET_UCHAR       )(ANSC_HANDLE  hThisObject);
typedef  SLAP_UCHAR_ARRAY*   (*PFN_SLAPOBJ_GET_UCHAR_ARRAY )(ANSC_HANDLE  hThisObject);
typedef  SLAP_UINT32         (*PFN_SLAPOBJ_GET_UINT32      )(ANSC_HANDLE  hThisObject);
typedef  SLAP_UINT32_ARRAY*  (*PFN_SLAPOBJ_GET_UINT32_ARRAY)(ANSC_HANDLE  hThisObject);
typedef  SLAP_PTR            (*PFN_SLAPOBJ_GET_PTR         )(ANSC_HANDLE  hThisObject);
typedef  SLAP_PTR_ARRAY*     (*PFN_SLAPOBJ_GET_PTR_ARRAY   )(ANSC_HANDLE  hThisObject);
typedef  SLAP_HANDLE         (*PFN_SLAPOBJ_GET_HANDLE      )(ANSC_HANDLE  hThisObject);
typedef  SLAP_HANDLE_ARRAY*  (*PFN_SLAPOBJ_GET_HANDLE_ARRAY)(ANSC_HANDLE  hThisObject);
typedef  SLAP_OBJECT         (*PFN_SLAPOBJ_GET_OBJECT      )(ANSC_HANDLE  hThisObject);
typedef  SLAP_OBJECT_ARRAY*  (*PFN_SLAPOBJ_GET_OBJECT_ARRAY)(ANSC_HANDLE  hThisObject);

typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_BOOL        )(ANSC_HANDLE  hThisObject, SLAP_BOOL           var_bool        );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_BOOL_ARRAY  )(ANSC_HANDLE  hThisObject, SLAP_BOOL_ARRAY*    var_bool_array  );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_CHAR        )(ANSC_HANDLE  hThisObject, SLAP_CHAR           var_char        );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_CHAR_ARRAY  )(ANSC_HANDLE  hThisObject, SLAP_CHAR_ARRAY*    var_char_array  );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_INT         )(ANSC_HANDLE  hThisObject, SLAP_INT            var_int         );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_INT_ARRAY   )(ANSC_HANDLE  hThisObject, SLAP_INT_ARRAY*     var_int_array   );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_STRING      )(ANSC_HANDLE  hThisObject, SLAP_STRING         var_string      );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_STRING_ARRAY)(ANSC_HANDLE  hThisObject, SLAP_STRING_ARRAY*  var_string_array);
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_UCHAR       )(ANSC_HANDLE  hThisObject, SLAP_UCHAR          var_uchar       );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_UCHAR_ARRAY )(ANSC_HANDLE  hThisObject, SLAP_UCHAR_ARRAY*   var_uchar_array );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_UINT32      )(ANSC_HANDLE  hThisObject, SLAP_UINT32         var_uint32      );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_UINT32_ARRAY)(ANSC_HANDLE  hThisObject, SLAP_UINT32_ARRAY*  var_uint32_array);
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_PTR         )(ANSC_HANDLE  hThisObject, SLAP_PTR            var_ptr         );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_PTR_ARRAY   )(ANSC_HANDLE  hThisObject, SLAP_PTR_ARRAY*     var_ptr_array   );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_HANDLE      )(ANSC_HANDLE  hThisObject, SLAP_HANDLE         var_handle      );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_HANDLE_ARRAY)(ANSC_HANDLE  hThisObject, SLAP_HANDLE_ARRAY*  var_handle_array);
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_OBJECT      )(ANSC_HANDLE  hThisObject, SLAP_OBJECT         var_object      );
typedef  ANSC_STATUS  (*PFN_SLAPOBJ_SET_OBJECT_ARRAY)(ANSC_HANDLE  hThisObject, SLAP_OBJECT_ARRAY*  var_handle_array);

typedef  struct
_SLAP_OBJECT_PROPERTY
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           PropertyName;
    ULONG                           HashIndex;

    char*                           GetMethodName;      /* method name to get the property value */
    ULONG                           GetMethodOffset;    /* memory offset of the get_method call  */
    char*                           SetMethodName;      /* method name to set the property value */
    ULONG                           SetMethodOffset;    /* memory offset of the set_method call  */
}
SLAP_OBJECT_PROPERTY,  *PSLAP_OBJECT_PROPERTY;

#define  ACCESS_SLAP_OBJECT_PROPERTY(p)             \
         ACCESS_CONTAINER(p, SLAP_OBJECT_PROPERTY, Linkage)

#define  SlapInitObjectProperty(tbi_prop)                                                   \
         {                                                                                  \
            tbi_prop->PropertyName    = NULL;                                               \
            tbi_prop->HashIndex       = 0;                                                  \
                                                                                            \
            tbi_prop->GetMethodName   = NULL;                                               \
            tbi_prop->GetMethodOffset = 0;                                                  \
            tbi_prop->SetMethodName   = NULL;                                               \
            tbi_prop->SetMethodOffset = 0;                                                  \
         }

#define  SlapCleanObjectProperty(tbc_prop)                                                  \
         {                                                                                  \
            if ( tbc_prop->PropertyName )                                                   \
            {                                                                               \
                AnscFreeMemory(tbc_prop->PropertyName);                                     \
                                                                                            \
                tbc_prop->PropertyName = NULL;                                              \
            }                                                                               \
         }

#define  SlapFreeObjectProperty(tbf_prop)                                                   \
         {                                                                                  \
            SlapCleanObjectProperty(tbf_prop);                                              \
            AnscFreeMemory         (tbf_prop);                                              \
         }

/*
 * Another important feature provided by SLAP is Data Type Conversion. When external modules are
 * making calls into SLAP, the input parameters may be packaged in a different way than the under-
 * lying component objects would expect. This may happen when the external modules don't have
 * enough information for packaging the parameters or doing so requires some significant work. In
 * order to perform Data Type Conversion, the SLAP component object must maintain the description
 * information for every method.
 */
typedef  SLAP_VOID           (*PFN_SLAPOBJ_CALL_VOID        )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_VARIABLE*      (*PFN_SLAPOBJ_CALL_VARIABLE    )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_BOOL           (*PFN_SLAPOBJ_CALL_BOOL        )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_BOOL_ARRAY*    (*PFN_SLAPOBJ_CALL_BOOL_ARRAY  )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_CHAR           (*PFN_SLAPOBJ_CALL_CHAR        )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_CHAR_ARRAY*    (*PFN_SLAPOBJ_CALL_CHAR_ARRAY  )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_INT            (*PFN_SLAPOBJ_CALL_INT         )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_INT_ARRAY*     (*PFN_SLAPOBJ_CALL_INT_ARRAY   )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_STRING         (*PFN_SLAPOBJ_CALL_STRING      )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_STRING_ARRAY*  (*PFN_SLAPOBJ_CALL_STRING_ARRAY)(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_UCHAR          (*PFN_SLAPOBJ_CALL_UCHAR       )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_UCHAR_ARRAY*   (*PFN_SLAPOBJ_CALL_UCHAR_ARRAY )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_UINT32         (*PFN_SLAPOBJ_CALL_UINT32      )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_UINT32_ARRAY*  (*PFN_SLAPOBJ_CALL_UINT32_ARRAY)(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_PTR            (*PFN_SLAPOBJ_CALL_PTR         )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_PTR_ARRAY*     (*PFN_SLAPOBJ_CALL_PTR_ARRAY   )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_HANDLE         (*PFN_SLAPOBJ_CALL_HANDLE      )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_HANDLE_ARRAY*  (*PFN_SLAPOBJ_CALL_HANDLE_ARRAY)(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_HANDLE         (*PFN_SLAPOBJ_CALL_OBJECT      )(ANSC_HANDLE  hThisObject, ...);
typedef  SLAP_HANDLE_ARRAY*  (*PFN_SLAPOBJ_CALL_OBJECT_ARRAY)(ANSC_HANDLE  hThisObject, ...);

#define  SLAP_METHOD_ID_Unknown                     0
#define  SLAP_METHOD_ID_GetProperty                 1
#define  SLAP_METHOD_ID_SetProperty                 2
#define  SLAP_METHOD_ID_GetItem                     3
#define  SLAP_METHOD_ID_SetItem                     4
#define  SLAP_METHOD_ID_GetItemByIndex              5
#define  SLAP_METHOD_ID_SetItemByIndex              6

#define  SLAP_BRIDGE_ID_Unknown                     0
#define  SLAP_BRIDGE_ID_GetProperty                 1
#define  SLAP_BRIDGE_ID_SetProperty                 2
#define  SLAP_BRIDGE_ID_GetItem                     3
#define  SLAP_BRIDGE_ID_SetItem                     4
#define  SLAP_BRIDGE_ID_GetItemByIndex              5
#define  SLAP_BRIDGE_ID_SetItemByIndex              6

#define  SLAP_CALL_OPTION_passParamListI            0x00000001
#define  SLAP_CALL_OPTION_passParamListO            0x00000002
#define  SLAP_CALL_OPTION_passReturnVar             0x00000004
#define  SLAP_CALL_OPTION_skipGoaTranslation        0x00000008

typedef  struct
_SLAP_METHOD_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           MethodName;
    ULONG                           MethodId;
    ULONG                           BridgeId;
    ULONG                           HashIndex;

    ULONG                           CallOptions;        /* specifies how the parameters are passed in  */
    SLAP_PARAMETER_LIST*            ParamListI;         /* syntax description of all input parameters  */
    SLAP_PARAMETER_LIST*            ParamListO;         /* syntax description of all output parameters */
    SLAP_VARIABLE*                  ReturnVar;          /* syntax description of the returned value    */
    ULONG                           MethodOffset;       /* memory offset of the corresponding function */
}
SLAP_METHOD_DESCRIPTOR,  *PSLAP_METHOD_DESCRIPTOR;

#define  ACCESS_SLAP_METHOD_DESCRIPTOR(p)           \
         ACCESS_CONTAINER(p, SLAP_METHOD_DESCRIPTOR, Linkage)

#define  SlapInitMethodDescriptor(tbi_desp)                                                 \
         {                                                                                  \
            tbi_desp->MethodName   = NULL;                                                  \
            tbi_desp->MethodId     = 0;                                                     \
            tbi_desp->HashIndex    = 0;                                                     \
            tbi_desp->CallOptions  = 0;                                                     \
            tbi_desp->ParamListI   = NULL;                                                  \
            tbi_desp->ParamListO   = NULL;                                                  \
            tbi_desp->ReturnVar    = NULL;                                                  \
            tbi_desp->MethodOffset = 0;                                                     \
         }

#define  SlapCleanMethodDescriptor(tbc_desp)                                                \
         {                                                                                  \
            if ( tbc_desp->MethodName )                                                     \
            {                                                                               \
                AnscFreeMemory(tbc_desp->MethodName);                                       \
                                                                                            \
                tbc_desp->MethodName = NULL;                                                \
            }                                                                               \
                                                                                            \
            if ( tbc_desp->ParamListI )                                                     \
            {                                                                               \
                SlapFreeParamList(tbc_desp->ParamListI);                                    \
                                                                                            \
                tbc_desp->ParamListI = NULL;                                                \
            }                                                                               \
                                                                                            \
            if ( tbc_desp->ParamListO )                                                     \
            {                                                                               \
                SlapFreeParamList(tbc_desp->ParamListO);                                    \
                                                                                            \
                tbc_desp->ParamListO = NULL;                                                \
            }                                                                               \
                                                                                            \
            if ( tbc_desp->ReturnVar )                                                      \
            {                                                                               \
                SlapFreeVariable(tbc_desp->ReturnVar);                                      \
                                                                                            \
                tbc_desp->ReturnVar = NULL;                                                 \
            }                                                                               \
         }

#define  SlapFreeMethodDescriptor(tbf_desp)                                                 \
         {                                                                                  \
            SlapCleanMethodDescriptor(tbf_desp);                                            \
            AnscFreeMemory           (tbf_desp);                                            \
         }


/***********************************************************
        SLAP CUSTOMIZED METHOD DESCRIPTOR DEFINITION
***********************************************************/

/*
 * While the above property and method descriptors serve the purpose well, constructing a property
 * and/or method descriptor and register it can be a long and boring process. To simplify this
 * process, we introduce following light-weighted descriptor which is easy to construct and can be
 * generated automatically by a Code Generator in the future. However, parsing such descriptors is
 * apparently much slower than the regular registration. Hopefully object pooling can help to ease
 * the pain in that area.
 */
#define  SLAP_CALL_PARAMETER_LIST_SEPARATORS        " ,;"
#define  SLAP_CALL_PARAMETER_SEPARATORS             "./"

typedef  struct
_SLAP_STD_CALL_DESCRIPTOR
{
    char*                           CallName;               /* member function name, MUST not be NULL   */
    ULONG                           CallId;                 /* set to SLAP_METHOD_ID_Unknown most times */
    ULONG                           CallOptions;            /* SHOULD always be set to zero             */
    ULONG                           CallEntry;              /* the call offset from the base object     */

    char*                           PropertyName;           /* must not be NULL if it's a property call */
    char*                           CallDescrI;             /* e.g., "uint32/ip4_addr string/password"  */
    char*                           CallDescrO;             /* e.g., "uint32/ip4_addr string/password"  */
    char*                           CallDescrR;             /* e.g., "object/simple_graph"              */
}
SLAP_STD_CALL_DESCRIPTOR,  *PSLAP_STD_CALL_DESCRIPTOR;


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
