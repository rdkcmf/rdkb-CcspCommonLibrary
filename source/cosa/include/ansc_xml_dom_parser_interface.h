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

    module:	ansc_xml_dom_parser_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to xml parsing operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu     (Original)
        Kang Quan   (Porting)

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_XML_DOM_PARSER_INTERFACE_
#define  _ANSC_XML_DOM_PARSER_INTERFACE_

#include "ansc_policy_parser_interface.h"
#include "ansc_xml_parser_interface.h"


/***********************************************************
     PLATFORM INDEPENDENT XML PARSER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the xml parser object definition.
 */
#define  MAXIMUM_CHARS_ONELINE                      96
#define  MAXIMUM_ATTRIBUTE_NAME                     64
/*
#define  MAXIMUM_NODE_NAME                          256
 */
#define  MAXIMUM_NODE_NAME                          ANSC_OBJECT_NAME_SIZE
#define  MAXIMUM_PATH_NAME                          256
#define  MAXIMUM_FILE_NAME                          128
#define  MAXIMUM_PASSPORT_SIZE                      64

#define  PASSPORT_ENCAPSULATED_NAME                 "HappySaturday"
#define  USER_NAME_ITEM                             "UserName"
#define  PASSWORD_ITEM                              "Passport"


typedef  struct
_ANSC_XML_ATTRIBUTE
{
    SINGLE_LINK_ENTRY       Linkage;                            /* used to link into the upper level list   */
    char                    Name[MAXIMUM_ATTRIBUTE_NAME];       /* XML the name string of the attribute     */
    ANSC_HANDLE             hParentNode;                        /* pointer back to the parent node          */
    ANSC_HANDLE             hXMLContext;                        /* pointer back to the global context       */
    BOOLEAN                 bEncrypted;                         /* indicate if the data is encrypted or not */
    ULONG                   DataSize;                           /* indicate the length of data              */
    PVOID                   StringData;                         /* The data will always be string           */
}
ANSC_XML_ATTRIBUTE,  *PANSC_XML_ATTRIBUTE;

#define  ACCESS_XML_ATTRIBUTE(p)                       \
         ACCESS_CONTAINER(p, ANSC_XML_ATTRIBUTE, Linkage)

typedef  struct
_ANSC_XML_CONTEXT
{
    char                    PathName[MAXIMUM_PATH_NAME];        /* XML the full path string of the configuration file   */
    char                    FileName[MAXIMUM_FILE_NAME];        /* XML the configuration file name                      */
    char                    BackupFileName[MAXIMUM_FILE_NAME];  /* XML the temporary file name                          */
    ANSC_HANDLE             hFile;                              /* handle of the configuration file                     */
    ANSC_HANDLE             hTempFile;                          /* handle of the temporary file                         */
#if 0
    KEY                     EncryptionKey;                      /* derived from the secret value passed by user         */
    IV                      EncryptionIV;                       /* initialization vector used in encryption             */
#endif
    ANSC_LOCK               TreeLock;                           /* synchronize the access to the internal tree          */
    ANSC_HANDLE             hRootNode;                          /* handle of the root node of configuration file        */
}
ANSC_XML_CONTEXT,  *PANSC_XML_CONTEXT;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_XML_DOM_PARSER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSC_POLICY_PARSER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \


typedef  struct
_ANSC_XML_DOM_PARSER_OBJECT
{
    ANSC_XML_DOM_PARSER_CLASS_CONTENT
}
ANSC_XML_DOM_PARSER_OBJECT,  *PANSC_XML_DOM_PARSER_OBJECT;

#define  ACCESS_ANSC_XML_DOM_PARSER_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_XML_DOM_PARSER_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT XML NODE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the xml node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef
ANSC_HANDLE
(*PFN_XML_NODE_OPEN_ATTR)
    (
        ANSC_HANDLE                 hNode,
        PUCHAR                      pAttributeName
    );

typedef
ANSC_HANDLE
(*PFN_XML_NODE_COPY)
    (
        ANSC_HANDLE                 hNode
    );

typedef
ANSC_HANDLE
(*PFN_XML_NODE_GET_FIRST_ATTR)
    (
        ANSC_HANDLE                 hNode
    );

typedef
ANSC_HANDLE
(*PFN_XML_NODE_GET_NEXT_ATTR)
    (
        ANSC_HANDLE                 hNode,
        ANSC_HANDLE                 hAttr
    );

typedef
ULONG
(*PFN_XML_NODE_GET_LEVEL)
    (
        ANSC_HANDLE                 hNode
    );

typedef
void
(*PFN_XML_NODE_SET_LEVEL)
    (
        ANSC_HANDLE                 hNode,
        ULONG                       ulLevel
    );

typedef
BOOL
(*PFN_XML_NODE_ENUM_KID_PROC)
    (
        ANSC_HANDLE                 hNode,
        PVOID                       pData
    );

typedef
BOOL
(*PFN_XML_NODE_ENUM_CHILD)
    (
        ANSC_HANDLE                 hNode,
        PFN_XML_NODE_ENUM_KID_PROC  Proc,
        PVOID                       pData
    );

typedef  ANSC_STATUS
(*PFN_PNO_GET_DATA_LONG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PLONG                       plTarget
    );
/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_XML_DOM_NODE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ANSC_HANDLE                     hParentNode;                                            \
                                                                                            \
    QUEUE_HEADER                    ChildNodeQueue;                                         \
    ULONG                           ChildNodeQueueLock;                                     \
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
    PFN_PNO_GET_DATA_LONG           GetDataLong;                                            \
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
    /* start of object class content */                                                     \
                                                                                            \
    /* XML the name string of the node  */                                                  \
    /* The base class has already defined the variable "Name"  */                           \
    /*    char                    Name[MAXIMUM_NODE_NAME];     */                           \
                                                                                            \
    /* pointer back to the global context */                                                \
    ANSC_HANDLE                     hXMLContext;                                            \
    /* indicate how deep this node is */                                                    \
    ULONG                           Level;                                                  \
    /* The text in this node ( not exists in AL_STORE) */                                   \
    PVOID                           StringData;                                             \
    /* The text size of this node ( not exists in AL_STORE) */                              \
    ULONG                           DataSize;                                               \
    /* maintain a list of the attributes under this node */                                 \
    QUEUE_HEADER                    AttributesList;                                         \
    /* lock for accessing attribute list */                                                 \
    ULONG                           AttributesListLock;                                     \
    /* write XML header or not */                                                           \
    BOOL                            bIgnoreXMLHeader;                                       \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_XML_DOM_NODE_OBJECT
{
    ANSC_XML_DOM_NODE_CLASS_CONTENT
}
ANSC_XML_DOM_NODE_OBJECT,  *PANSC_XML_DOM_NODE_OBJECT;

#define  ACCESS_ANSC_XML_DOM_NODE_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_XML_DOM_NODE_OBJECT, Linkage)

#endif

