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

    module:	ansc_policy_parser_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to policy parsing operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/09/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_POLICY_PARSER_INTERFACE_
#define  _ANSC_POLICY_PARSER_INTERFACE_


/***********************************************************
    PLATFORM INDEPENDENT POLICY PARSER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy parser object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_PPO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_PPO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_PPO_IS_QUALIFIED)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  BOOL
(*PFN_PPO_IS_COMPLETED)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_PPO_GET_ENCODED_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PPO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_PPO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_HANDLE
(*PFN_PPO_GET_ROOT_NODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PPO_SET_ROOT_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_POLICY_PARSER_CLASS_CONTENT                                                   \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ANSC_HANDLE                     hRootNode;                                              \
                                                                                            \
    PFN_PPO_CREATE                  Create;                                                 \
    PFN_PPO_REMOVE                  Remove;                                                 \
    PFN_PPO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_PPO_IS_QUALIFIED            IsQualified;                                            \
    PFN_PPO_IS_COMPLETED            IsCompleted;                                            \
    PFN_PPO_GET_ENCODED_SIZE        GetEncodedSize;                                         \
    PFN_PPO_ENCODE                  Encode;                                                 \
    PFN_PPO_DECODE                  Decode;                                                 \
                                                                                            \
    PFN_PPO_GET_ROOT_NODE           GetRootNode;                                            \
    PFN_PPO_SET_ROOT_NODE           SetRootNode;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_POLICY_PARSER_OBJECT
{
    ANSC_POLICY_PARSER_CLASS_CONTENT
}
ANSC_POLICY_PARSER_OBJECT,  *PANSC_POLICY_PARSER_OBJECT;

#define  ACCESS_ANSC_POLICY_PARSER_OBJECT(p)        \
         ACCESS_CONTAINER(p, ANSC_POLICY_PARSER_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT POLICY NODE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PNO_STATUS_MORE_NODE                  0x00000001
#define  ANSC_PNO_STATUS_MORE_DATA                  0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_PNO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_PNO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PNO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_PNO_GET_STATUS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_PNO_GET_ENCODED_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PNO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  char*
(*PFN_PNO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PNO_GET_PARENT_NODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_PARENT_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentNode
    );

typedef  ANSC_HANDLE
(*PFN_PNO_ADD_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    );

typedef  ANSC_HANDLE
(*PFN_PNO_ADD_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PNO_DEL_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    );

typedef  ANSC_STATUS
(*PFN_PNO_DEL_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PNO_INSERT_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode,
        ANSC_HANDLE                 hAfterChild
    );

typedef  ANSC_HANDLE
(*PFN_PNO_REMOVE_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PNO_GET_CHILD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_PNO_GET_HEAD_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PNO_GET_NEXT_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisChild
    );

typedef  ANSC_HANDLE
(*PFN_PNO_GET_TAIL_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ITEM_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ITEM_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ITEM_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ITEM_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ULONG
(*PFN_PNO_GET_ITEM_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ATTR_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ATTR_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ATTR_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_ATTR_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ULONG
(*PFN_PNO_GET_ATTR_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_DATA_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_DATA_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_DATA_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_DATA_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

typedef  ULONG
(*PFN_PNO_GET_DATA_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_ATTR_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_ATTR_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_ATTR_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_ATTR_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_DATA_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_DATA_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_DATA_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    );

typedef  ANSC_STATUS
(*PFN_PNO_SET_DATA_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_POLICY_NODE_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ANSC_HANDLE                     hParentNode;                                            \
                                                                                            \
    QUEUE_HEADER                    ChildNodeQueue;                                         \
    ANSC_LOCK                       ChildNodeQueueLock;                                     \
                                                                                            \
    PFN_PNO_CREATE                  Create;                                                 \
    PFN_PNO_REMOVE                  Remove;                                                 \
    PFN_PNO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_PNO_GET_STATUS              GetStatus;                                              \
    PFN_PNO_GET_ENCODED_SIZE        GetEncodedSize;                                         \
    PFN_PNO_ENCODE                  Encode;                                                 \
    PFN_PNO_DECODE                  Decode;                                                 \
                                                                                            \
    PFN_PNO_GET_NAME                GetName;                                                \
    PFN_PNO_SET_NAME                SetName;                                                \
    PFN_PNO_GET_PARENT_NODE         GetParentNode;                                          \
    PFN_PNO_SET_PARENT_NODE         SetParentNode;                                          \
                                                                                            \
    PFN_PNO_ADD_CHILD               AddChild;                                               \
    PFN_PNO_ADD_CHILD_BYNAME        AddChildByName;                                         \
    PFN_PNO_DEL_CHILD               DelChild;                                               \
    PFN_PNO_DEL_CHILD_BYNAME        DelChildByName;                                         \
    PFN_PNO_INSERT_CHILD            InsertChild;                                            \
    PFN_PNO_REMOVE_CHILD            RemoveChild;                                            \
                                                                                            \
    PFN_PNO_GET_CHILD_BYNAME        GetChildByName;                                         \
    PFN_PNO_GET_HEAD_CHILD          GetHeadChild;                                           \
    PFN_PNO_GET_NEXT_CHILD          GetNextChild;                                           \
    PFN_PNO_GET_TAIL_CHILD          GetTailChild;                                           \
                                                                                            \
    PFN_PNO_GET_ITEM_STRING         GetItemString;                                          \
    PFN_PNO_GET_ITEM_ULONG          GetItemUlong;                                           \
    PFN_PNO_GET_ITEM_BOOLEAN        GetItemBoolean;                                         \
    PFN_PNO_GET_ITEM_BINARY         GetItemBinary;                                          \
    PFN_PNO_GET_ITEM_SIZE           GetItemSize;                                            \
                                                                                            \
    PFN_PNO_GET_ATTR_STRING         GetAttrString;                                          \
    PFN_PNO_GET_ATTR_ULONG          GetAttrUlong;                                           \
    PFN_PNO_GET_ATTR_BOOLEAN        GetAttrBoolean;                                         \
    PFN_PNO_GET_ATTR_BINARY         GetAttrBinary;                                          \
    PFN_PNO_GET_ATTR_SIZE           GetAttrSize;                                            \
                                                                                            \
    PFN_PNO_GET_DATA_STRING         GetDataString;                                          \
    PFN_PNO_GET_DATA_ULONG          GetDataUlong;                                           \
    PFN_PNO_GET_DATA_BOOLEAN        GetDataBoolean;                                         \
    PFN_PNO_GET_DATA_BINARY         GetDataBinary;                                          \
    PFN_PNO_GET_DATA_SIZE           GetDataSize;                                            \
                                                                                            \
    PFN_PNO_SET_ATTR_STRING         SetAttrString;                                          \
    PFN_PNO_SET_ATTR_ULONG          SetAttrUlong;                                           \
    PFN_PNO_SET_ATTR_BOOLEAN        SetAttrBoolean;                                         \
    PFN_PNO_SET_ATTR_BINARY         SetAttrBinary;                                          \
                                                                                            \
    PFN_PNO_SET_DATA_STRING         SetDataString;                                          \
    PFN_PNO_SET_DATA_ULONG          SetDataUlong;                                           \
    PFN_PNO_SET_DATA_BOOLEAN        SetDataBoolean;                                         \
    PFN_PNO_SET_DATA_BINARY         SetDataBinary;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_POLICY_NODE_OBJECT
{
    ANSC_POLICY_NODE_CLASS_CONTENT
}
ANSC_POLICY_NODE_OBJECT,  *PANSC_POLICY_NODE_OBJECT;

#define  ACCESS_ANSC_POLICY_NODE_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_POLICY_NODE_OBJECT, Linkage)


#endif
