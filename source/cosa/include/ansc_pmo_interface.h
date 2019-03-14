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

    module:	ansc_pmo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to property management
        operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/19/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PMO_INTERFACE_
#define  _ANSC_PMO_INTERFACE_


/***********************************************************
   PLATFORM INDEPENDENT PROPERTY MANAGER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy parser object definition.
 */
#define  ANSC_PMO_DEFAULT_SEPARATOR                 "/"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_PMO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_PMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_PMO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PMO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PMO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_STATUS
(*PFN_PMO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_HANDLE
(*PFN_PMO_GET_RSECTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PMO_SET_RSECTION)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PMO_ADD_SECTION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSection
    );

typedef  ANSC_HANDLE
(*PFN_PMO_ADD_SECTION_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bUnique
    );

typedef  ANSC_STATUS
(*PFN_PMO_DEL_SECTION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSection
    );

typedef  ANSC_STATUS
(*PFN_PMO_DEL_SECTION_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PMO_GET_SECTION)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PMO_SEARCH_SECTION)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PMO_ADD_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_HANDLE
(*PFN_PMO_ADD_PROPERTY_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       type,
        BOOL                        bUnique
    );

typedef  ANSC_STATUS
(*PFN_PMO_DEL_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_PMO_DEL_PROPERTY_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PMO_SEARCH_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

/*
 * Originally I thought that by using xml-based data description, we might be able to get rid of
 * the hassel of arranging configuration and policy information in certain way, which requires
 * tremendous effort of implementing a general tree structure to store all the data. But I was
 * very wrong. Any data description format is just a format, we still have to prepare the data in
 * certain intermidate format.
 */
#define  ANSC_PROPERTY_MANAGER_CLASS_CONTENT                                                \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ANSC_HANDLE                     hRootSection;                                           \
                                                                                            \
    PFN_PMO_CREATE                  Create;                                                 \
    PFN_PMO_REMOVE                  Remove;                                                 \
    PFN_PMO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_PMO_ENCODE                  Encode;                                                 \
    PFN_PMO_DECODE                  Decode;                                                 \
                                                                                            \
    PFN_PMO_GET_NAME                GetName;                                                \
    PFN_PMO_SET_NAME                SetName;                                                \
    PFN_PMO_GET_RSECTION            GetRootSection;                                         \
    PFN_PMO_SET_RSECTION            SetRootSection;                                         \
                                                                                            \
    PFN_PMO_ADD_SECTION             AddSection;                                             \
    PFN_PMO_ADD_SECTION_BYNAME      AddSectionByName;                                       \
    PFN_PMO_DEL_SECTION             DelSection;                                             \
    PFN_PMO_DEL_SECTION_BYNAME      DelSectionByName;                                       \
    PFN_PMO_GET_SECTION             GetSection;                                             \
    PFN_PMO_SEARCH_SECTION          SearchSection;                                          \
                                                                                            \
    PFN_PMO_ADD_PROPERTY            AddProperty;                                            \
    PFN_PMO_ADD_PROPERTY_BYNAME     AddPropertyByName;                                      \
    PFN_PMO_DEL_PROPERTY            DelProperty;                                            \
    PFN_PMO_DEL_PROPERTY_BYNAME     DelPropertyByName;                                      \
    PFN_PMO_GET_PROPERTY            GetProperty;                                            \
    PFN_PMO_SEARCH_PROPERTY         SearchProperty;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_MANAGER_OBJECT
{
    ANSC_PROPERTY_MANAGER_CLASS_CONTENT
}
ANSC_PROPERTY_MANAGER_OBJECT,  *PANSC_PROPERTY_MANAGER_OBJECT;

#define  ACCESS_ANSC_PROPERTY_MANAGER_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_MANAGER_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PSO_MODE_NAME                         0x00000001
#define  ANSC_PSO_MODE_PROPERTIES                   0x00000002
#define  ANSC_PSO_MODE_CHILDREN                     0x00000004
#define  ANSC_PSO_MODE_ALL                          0x00000007

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_PSO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_PSO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PSO_DUPLICATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_STATUS
(*PFN_PSO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  char*
(*PFN_PSO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_PARENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_SET_PARENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParent
    );

typedef  ULONG
(*PFN_PSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  ANSC_STATUS
(*PFN_PSO_ADD_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

typedef  ANSC_HANDLE
(*PFN_PSO_ADD_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bUnique
    );

typedef  ANSC_STATUS
(*PFN_PSO_DEL_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

typedef  ANSC_STATUS
(*PFN_PSO_DEL_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PSO_INSERT_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild,
        ANSC_HANDLE                 hAfterChild
    );

typedef  ANSC_HANDLE
(*PFN_PSO_REMOVE_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_SEARCH_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_HEAD_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_NEXT_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisChild
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_TAIL_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSO_ADD_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_HANDLE
(*PFN_PSO_ADD_PROPERTY_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       type,
        BOOL                        bUnique
    );

typedef  ANSC_STATUS
(*PFN_PSO_DEL_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_PSO_DEL_PROPERTY_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_CREATE_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       type
    );

typedef  ANSC_HANDLE
(*PFN_PSO_REMOVE_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_PROPERTY_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_SEARCH_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_HEAD_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_NEXT_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisProperty
    );

typedef  ANSC_HANDLE
(*PFN_PSO_GET_TAIL_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Why do we need this property manager anyway, you may ask. Is it doing the same thing as the
 * policy parser does? Both of them maintain a tree structure internally, and each node at the
 * tree contains some useful information either under the name of "attribute" or "item". Both
 * objects have to provide the member functions for searching, sorting, and data retrieval. So
 * why two objects, it seems this object is just a duplication of the policy parser. There are
 * actually some very important considerations:
 *
 *      (1) Property Manager is implemented as an internal data storage service, it
 *          shields other objects from knowing and dealing with any external data
 *          description and transportation. Although it's true that xml is widely used
 *          and accepted as a universal data description standard, the isolation may
 *          be well appreciated in many legacy environment.
 *
 *      (2) Policy Parser only provides primitive encoding/decoding capability. It only
 *          understands the primitive data format, such as integer, string, boolean and
 *          so forth. Property Manager on the other hand can handle more complicated
 *          data types. For example, an IP address can be encoded as either an integer
 *          or a string. Policy Parser merely extracts data from the xml file, it's up
 *          to the Property Manager to convert the string or integer to an IP address.
 *          Note that xml schema will not help the Policy Parser to correctly understand
 *          the data type without predefined data structures and knowledge.
 */
#define  ANSC_PROPERTY_SECTION_CLASS_CONTENT                                                \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ANSC_HANDLE                     hParent;                                                \
    ULONG                           EncodingMode;                                           \
                                                                                            \
    QUEUE_HEADER                    ChildSectionQueue;                                      \
    ANSC_LOCK                       ChildSectionQueueLock;                                  \
                                                                                            \
    PFN_PSO_CREATE                  Create;                                                 \
    PFN_PSO_REMOVE                  Remove;                                                 \
    PFN_PSO_RESET                   Reset;                                                  \
    PFN_PSO_DUPLICATE               Duplicate;                                              \
                                                                                            \
    PFN_PSO_ENCODE                  Encode;                                                 \
    PFN_PSO_DECODE                  Decode;                                                 \
                                                                                            \
    PFN_PSO_GET_NAME                GetName;                                                \
    PFN_PSO_SET_NAME                SetName;                                                \
    PFN_PSO_GET_PARENT              GetParent;                                              \
    PFN_PSO_SET_PARENT              SetParent;                                              \
    PFN_PSO_GET_MODE                GetEncodingMode;                                        \
    PFN_PSO_SET_MODE                SetEncodingMode;                                        \
                                                                                            \
    PFN_PSO_ADD_CHILD               AddChild;                                               \
    PFN_PSO_ADD_CHILD_BYNAME        AddChildByName;                                         \
    PFN_PSO_DEL_CHILD               DelChild;                                               \
    PFN_PSO_DEL_CHILD_BYNAME        DelChildByName;                                         \
    PFN_PSO_INSERT_CHILD            InsertChild;                                            \
    PFN_PSO_REMOVE_CHILD            RemoveChild;                                            \
                                                                                            \
    PFN_PSO_GET_CHILD_BYNAME        GetChildByName;                                         \
    PFN_PSO_SEARCH_CHILD            SearchChild;                                            \
    PFN_PSO_GET_HEAD_CHILD          GetHeadChild;                                           \
    PFN_PSO_GET_NEXT_CHILD          GetNextChild;                                           \
    PFN_PSO_GET_TAIL_CHILD          GetTailChild;                                           \
                                                                                            \
    QUEUE_HEADER                    PropertyQueue;                                          \
    ANSC_LOCK                       PropertyQueueLock;                                      \
                                                                                            \
    PFN_PSO_ADD_PROPERTY            AddProperty;                                            \
    PFN_PSO_ADD_PROPERTY_BYNAME     AddPropertyByName;                                      \
    PFN_PSO_DEL_PROPERTY            DelProperty;                                            \
    PFN_PSO_DEL_PROPERTY_BYNAME     DelPropertyByName;                                      \
    PFN_PSO_CREATE_PROPERTY         CreateProperty;                                         \
    PFN_PSO_REMOVE_PROPERTY         RemoveProperty;                                         \
                                                                                            \
    PFN_PSO_GET_PROPERTY_BYNAME     GetPropertyByName;                                      \
    PFN_PSO_SEARCH_PROPERTY         SearchProperty;                                         \
    PFN_PSO_GET_HEAD_PROPERTY       GetHeadProperty;                                        \
    PFN_PSO_GET_NEXT_PROPERTY       GetNextProperty;                                        \
    PFN_PSO_GET_TAIL_PROPERTY       GetTailProperty;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_SECTION_OBJECT
{
    ANSC_PROPERTY_SECTION_CLASS_CONTENT
}
ANSC_PROPERTY_SECTION_OBJECT,  *PANSC_PROPERTY_SECTION_OBJECT;

#define  ACCESS_ANSC_PROPERTY_SECTION_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_SECTION_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PROPERTY_TYPE_BASE                    0
#define  ANSC_PROPERTY_TYPE_INTEGER                 ANSC_PROPERTY_TYPE_BASE + 0x0001
#define  ANSC_PROPERTY_TYPE_BOOLEAN                 ANSC_PROPERTY_TYPE_BASE + 0x0002
#define  ANSC_PROPERTY_TYPE_STRING                  ANSC_PROPERTY_TYPE_BASE + 0x0003
#define  ANSC_PROPERTY_TYPE_TOKENS                  ANSC_PROPERTY_TYPE_BASE + 0x0004
#define  ANSC_PROPERTY_TYPE_BINARY                  ANSC_PROPERTY_TYPE_BASE + 0x0005
#define  ANSC_PROPERTY_TYPE_IPADDR                  ANSC_PROPERTY_TYPE_BASE + 0x0006
#define  ANSC_PROPERTY_TYPE_SUBNET                  ANSC_PROPERTY_TYPE_BASE + 0x0007
#define  ANSC_PROPERTY_TYPE_IPRANGE                 ANSC_PROPERTY_TYPE_BASE + 0x0008
#define  ANSC_PROPERTY_TYPE_SECTION                 ANSC_PROPERTY_TYPE_BASE + 0xFFFE
#define  ANSC_PROPERTY_TYPE_UNKNOWN                 ANSC_PROPERTY_TYPE_BASE + 0xFFFF

#define  ANSC_PROPERTY_MODE_NAME                    0x00000001
#define  ANSC_PROPERTY_MODE_VALUE                   0x00000002
#define  ANSC_PROPERTY_MODE_BOTH                    0x00000003

#define  ANSC_PROPERTY_DTYPE_ATTR_NAME              "dtype"
#define  ANSC_PROPERTY_DTYPE_INTEGER                "integer"
#define  ANSC_PROPERTY_DTYPE_BOOLEAN                "bool"
#define  ANSC_PROPERTY_DTYPE_STRING                 "string"
#define  ANSC_PROPERTY_DTYPE_TOKENS                 "tokens"
#define  ANSC_PROPERTY_DTYPE_BINARY                 "binary"
#define  ANSC_PROPERTY_DTYPE_IPADDR                 "ipaddr"
#define  ANSC_PROPERTY_DTYPE_SUBNET                 "ipsubnet"
#define  ANSC_PROPERTY_DTYPE_IPRANGE                "iprange"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_PO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_PO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PO_DUPLICATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_STATUS
(*PFN_PO_ENCODE_CONTENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_STATUS
(*PFN_PO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  ANSC_STATUS
(*PFN_PO_DECODE_CONTENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPolicyNode
    );

typedef  char*
(*PFN_PO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PO_GET_SECTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_SET_SECTION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSection
    );

typedef  ULONG
(*PFN_PO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ULONG
(*PFN_PO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

/*
 * Why do we need this property manager anyway, you may ask. Is it doing the same thing as the
 * policy parser does? Both of them maintain a tree structure internally, and each node at the
 * tree contains some useful information either under the name of "attribute" or "item". Both
 * objects have to provide the member functions for searching, sorting, and data retrieval. So
 * why two objects, it seems this object is just a duplication of the policy parser. There are
 * actually some very important considerations:
 *
 *      (1) Property Manager is implemented as an internal data storage service, it
 *          shields other objects from knowing and dealing with any external data
 *          description and transportation. Although it's true that xml is widely used
 *          and accepted as a universal data description standard, the isolation may
 *          be well appreciated in many legacy environment.
 *
 *      (2) Policy Parser only provides primitive encoding/decoding capability. It only
 *          understands the primitive data format, such as integer, string, boolean and
 *          so forth. Property Manager on the other hand can handle more complicated
 *          data types. For example, an IP address can be encoded as either an integer
 *          or a string. Policy Parser merely extracts data from the xml file, it's up
 *          to the Property Manager to convert the string or integer to an IP address.
 *          Note that xml schema will not help the Policy Parser to correctly understand
 *          the data type without predefined data structures and knowledge.
 */
#define  ANSC_PROPERTY_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ANSC_HANDLE                     hSection;                                               \
    ULONG                           Type;                                                   \
    ULONG                           EncodingMode;                                           \
                                                                                            \
    PFN_PO_CREATE                   Create;                                                 \
    PFN_PO_REMOVE                   Remove;                                                 \
    PFN_PO_RESET                    Reset;                                                  \
    PFN_PO_DUPLICATE                Duplicate;                                              \
                                                                                            \
    PFN_PO_ENCODE                   Encode;                                                 \
    PFN_PO_ENCODE_CONTENT           EncodeContent;                                          \
    PFN_PO_DECODE                   Decode;                                                 \
    PFN_PO_DECODE_CONTENT           DecodeContent;                                          \
                                                                                            \
    PFN_PO_GET_NAME                 GetName;                                                \
    PFN_PO_SET_NAME                 SetName;                                                \
    PFN_PO_GET_SECTION              GetSection;                                             \
    PFN_PO_SET_SECTION              SetSection;                                             \
    PFN_PO_GET_TYPE                 GetType;                                                \
    PFN_PO_SET_TYPE                 SetType;                                                \
    PFN_PO_GET_MODE                 GetEncodingMode;                                        \
    PFN_PO_SET_MODE                 SetEncodingMode;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_OBJECT
{
    ANSC_PROPERTY_CLASS_CONTENT
}
ANSC_PROPERTY_OBJECT,  *PANSC_PROPERTY_OBJECT;

#define  ACCESS_ANSC_PROPERTY_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_OBJECT, Linkage)


#endif
