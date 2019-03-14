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

    module:	ansc_object_mapper_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to object mapping operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_OBJECT_MAPPER_INTERFACE_
#define  _ANSC_OBJECT_MAPPER_INTERFACE_


/***********************************************************
    PLATFORM INDEPENDENT OBJECT MAPPER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  DEFAULT_OD_TABLE_SIZE                      32

typedef  ANSC_HANDLE
(*PFN_ANSC_OBJECT_CREATE)
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

typedef  ANSC_STATUS
(*PFN_ANSC_OBJECT_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_OMO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_OMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_OMO_GET_CONTAINER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OMO_SET_CONTAINER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

typedef  ANSC_STATUS
(*PFN_OMO_REGISTER_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       oid,
        char*                       baseName,
        ULONG                       baseOid,
        char*                       type,
        ULONG                       brokerOid,
        PFN_ANSC_OBJECT_CREATE      pfnCreate,
        PFN_ANSC_OBJECT_REMOVE      pfnRemove
    );

typedef  ANSC_HANDLE
(*PFN_OMO_CREATE_OBJECT_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_OMO_CREATE_OBJECT_BYOID)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    );

typedef  ANSC_HANDLE
(*PFN_OMO_CREATE_OBJECT_BYTYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseName,
        char*                       type
    );

typedef  ANSC_HANDLE
(*PFN_OMO_GET_OBJECT_BYRID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    );

typedef  ULONG
(*PFN_OMO_GET_OBJECTS_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OMO_SERIALIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized
    );

typedef  ANSC_STATUS
(*PFN_OMO_INTERPRET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized,
        ULONG                       ulCount
    );

typedef  ANSC_STATUS
(*PFN_OMO_ADD_OD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

typedef  ANSC_STATUS
(*PFN_OMO_DEL_OD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

typedef  ANSC_STATUS
(*PFN_OMO_DEL_ALL_ODS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_OMO_GET_OD_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_OMO_GET_OD_BYOID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );

typedef  ANSC_HANDLE
(*PFN_OMO_GET_OD_BYTYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       baseName,
        char*                       type
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_OBJECT_MAPPER_CLASS_CONTENT                                                   \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hContainerContext;                                      \
    ANSC_HANDLE                     hOwnerContext;                                          \
                                                                                            \
    PFN_OMO_CREATE                  Create;                                                 \
    PFN_OMO_REMOVE                  Remove;                                                 \
    PFN_OMO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_OMO_GET_CONTAINER           GetContainerContext;                                    \
    PFN_OMO_SET_CONTAINER           SetContainerContext;                                    \
                                                                                            \
    PFN_OMO_REGISTER_OBJECT         RegisterObject;                                         \
    PFN_OMO_CREATE_OBJECT_BYNAME    CreateObjectByName;                                     \
    PFN_OMO_CREATE_OBJECT_BYOID     CreateObjectByOid;                                      \
    PFN_OMO_CREATE_OBJECT_BYTYPE    CreateObjectByType;                                     \
    PFN_OMO_GET_OBJECT_BYRID        GetObjectByRid;                                         \
                                                                                            \
    SLIST_HEADER                    DescriptorTable[DEFAULT_OD_TABLE_SIZE];                 \
    ANSC_LOCK                       DescriptorTableLock;                                    \
                                                                                            \
    PFN_OMO_GET_OBJECTS_COUNT       GetObjectsCount;                                        \
    PFN_OMO_SERIALIZE               Serialize;                                              \
    PFN_OMO_INTERPRET               Interpret;                                              \
                                                                                            \
    PFN_OMO_ADD_OD                  AddDescriptor;                                          \
    PFN_OMO_DEL_OD                  DelDescriptor;                                          \
    PFN_OMO_DEL_ALL_ODS             DelAllDescriptors;                                      \
                                                                                            \
    PFN_OMO_GET_OD_BYNAME           GetDescriptorByName;                                    \
    PFN_OMO_GET_OD_BYOID            GetDescriptorByOid;                                     \
    PFN_OMO_GET_OD_BYTYPE           GetDescriptorByType;                                    \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_OBJECT_MAPPER_OBJECT
{
    ANSC_OBJECT_MAPPER_CLASS_CONTENT
}
ANSC_OBJECT_MAPPER_OBJECT,  *PANSC_OBJECT_MAPPER_OBJECT;

#define  ACCESS_ANSC_OBJECT_MAPPER_OBJECT(p)        \
         ACCESS_CONTAINER(p, ANSC_OBJECT_MAPPER_OBJECT, Linkage)


/***********************************************************
  PLATFORM INDEPENDENT OBJECT DESCRIPTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object descriptor object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ODO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_ODO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ODO_GET_CONTAINER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_CONTAINER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

typedef  ANSC_HANDLE
(*PFN_ODO_GET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_HANDLE
(*PFN_ODO_GET_CREATE_ENTRY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_CREATE_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

typedef  ANSC_HANDLE
(*PFN_ODO_GET_REMOVE_ENTRY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_REMOVE_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

typedef  char*
(*PFN_ODO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_ODO_GET_OID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ODO_SET_OID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );

typedef  ANSC_HANDLE
(*PFN_ODO_CREATE_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_ODO_REMOVE_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_OBJECT_DESCRIPTOR_CLASS_CONTENT                                               \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hContainerContext;                                      \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ANSC_HANDLE                     hMapperReserved;                                        \
    ULONG                           HashIndex;                                              \
                                                                                            \
    PFN_ANSC_OBJECT_CREATE          pfnObjectCreate;                                        \
    PFN_ANSC_OBJECT_REMOVE          pfnObjectRemove;                                        \
    char                            BaseName[ANSC_OBJECT_NAME_SIZE];                        \
    char                            DerivedType[ANSC_OBJECT_NAME_SIZE];                     \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ULONG                           BaseOid;                                                \
    ULONG                           Oid;                                                    \
    ULONG                           BrokerOid;                                              \
                                                                                            \
    PFN_ODO_CREATE                  Create;                                                 \
    PFN_ODO_REMOVE                  Remove;                                                 \
    PFN_ODO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_ODO_GET_CONTAINER           GetContainerContext;                                    \
    PFN_ODO_SET_CONTAINER           SetContainerContext;                                    \
    PFN_ODO_GET_RESERVED            GetMapperReserved;                                      \
    PFN_ODO_SET_RESERVED            SetMapperReserved;                                      \
                                                                                            \
    PFN_ODO_GET_CREATE_ENTRY        GetCreateEntry;                                         \
    PFN_ODO_SET_CREATE_ENTRY        SetCreateEntry;                                         \
    PFN_ODO_GET_REMOVE_ENTRY        GetRemoveEntry;                                         \
    PFN_ODO_SET_REMOVE_ENTRY        SetRemoveEntry;                                         \
                                                                                            \
    PFN_ODO_GET_NAME                GetName;                                                \
    PFN_ODO_SET_NAME                SetName;                                                \
    PFN_ODO_GET_OID                 GetOid;                                                 \
    PFN_ODO_SET_OID                 SetOid;                                                 \
    PFN_ODO_GET_NAME                GetBaseName;                                            \
    PFN_ODO_SET_NAME                SetBaseName;                                            \
    PFN_ODO_GET_OID                 GetBaseOid;                                             \
    PFN_ODO_SET_OID                 SetBaseOid;                                             \
    PFN_ODO_GET_NAME                GetDerivedType;                                         \
    PFN_ODO_SET_NAME                SetDerivedType;                                         \
    PFN_ODO_GET_OID                 GetBrokerOid;                                           \
    PFN_ODO_SET_OID                 SetBrokerOid;                                           \
                                                                                            \
    PFN_ODO_CREATE_OBJECT           CreateObject;                                           \
    PFN_ODO_REMOVE_OBJECT           RemoveObject;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_OBJECT_DESCRIPTOR_OBJECT
{
    ANSC_OBJECT_DESCRIPTOR_CLASS_CONTENT
}
ANSC_OBJECT_DESCRIPTOR_OBJECT,  *PANSC_OBJECT_DESCRIPTOR_OBJECT;

#define  ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_OBJECT_DESCRIPTOR_OBJECT, Linkage)


#endif
