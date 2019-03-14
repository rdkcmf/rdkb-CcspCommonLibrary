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

    module:	ansc_object_mapper_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the object_mapper.

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


#ifndef  _ANSC_OBJECT_MAPPER_INTERNAL_API_
#define  _ANSC_OBJECT_MAPPER_INTERNAL_API_


/***********************************************************
     FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_MAPPER_BASE.C
***********************************************************/

ANSC_HANDLE
AnscOmoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscOmoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
   FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_MAPPER_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscOmoRegisterObject
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

ANSC_HANDLE
AnscOmoCreateObjectByName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       name
    );

ANSC_HANDLE
AnscOmoCreateObjectByOid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    );

ANSC_HANDLE
AnscOmoCreateObjectByType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseName,
        char*                       type
    );

ANSC_HANDLE
AnscOmoGetObjectByRid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    );

ULONG
AnscOmoGetObjectsCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOmoSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized
    );

ANSC_STATUS
AnscOmoInterpret
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized,
        ULONG                       ulCount
    );


/***********************************************************
  FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_MAPPER_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscOmoGetContainerContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOmoSetContainerContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

ANSC_STATUS
AnscOmoAddDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

ANSC_STATUS
AnscOmoDelDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

ANSC_STATUS
AnscOmoDelAllDescriptors
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscOmoGetDescriptorByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_HANDLE
AnscOmoGetDescriptorByOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );

ANSC_HANDLE
AnscOmoGetDescriptorByType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       baseName,
        char*                       type
    );


/***********************************************************
   FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_DESCRIPTOR_BASE.C
***********************************************************/

ANSC_HANDLE
AnscOdoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscOdoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_DESCRIPTOR_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscOdoGetContainerContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetContainerContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

ANSC_HANDLE
AnscOdoGetMapperReserved
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetMapperReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMapperReserved
    );

ANSC_HANDLE
AnscOdoGetCreateEntry
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetCreateEntry
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

ANSC_HANDLE
AnscOdoGetRemoveEntry
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetRemoveEntry
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

char*
AnscOdoGetName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ULONG
AnscOdoGetOid
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );

char*
AnscOdoGetBaseName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetBaseName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ULONG
AnscOdoGetBaseOid
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetBaseOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );

char*
AnscOdoGetDerivedType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetDerivedType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       type
    );

ULONG
AnscOdoGetBrokerOid
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOdoSetBrokerOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    );


/***********************************************************
 FUNCTIONS IMPLEMENTED IN ANSC_OBJECT_DESCRIPTOR_OPERATION.C
***********************************************************/

ANSC_HANDLE
AnscOdoCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscOdoRemoveObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    );


#endif
