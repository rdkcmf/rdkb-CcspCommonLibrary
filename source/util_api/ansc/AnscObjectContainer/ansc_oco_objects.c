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

    module:	ansc_oco_objects.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced objects management
        functions of the Ansc Object Container Object.

        *   AnscOcoRegisterObject1
        *   AnscOcoRegisterObject2
        *   AnscOcoCreateObjectByName
        *   AnscOcoCreateObjectByOid
        *   AnscOcoCreateObjectByType
        *   AnscOcoGetObjectByRid

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#include "ansc_oco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoRegisterObject1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       objectName,
                ULONG                       oid,
                void*                       pfnCreate,
                void*                       pfnRemove
            );

    description:

        This function is called to register a feature object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       objectName
                Specifies the name of the containter object to be
                registered.

                ULONG                       oid
                Specifies the oid of the container object to be
                registered.

                void*                       pfnCreate
                Specifies the address of the create routine.

                void*                       pfnRemove
                Specifies the address of the remove routine.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoRegisterObject1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        void*                       pfnCreate,
        void*                       pfnRemove
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    returnStatus =
        pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                objectName,
                oid,
                ANSC_OBJECT_NAME_NULL,
                ANSC_OBJECT_OID_NULL,
                ANSC_OBJECT_TYPE_NULL,
                ANSC_OBJECT_OID_NULL,
                pfnCreate,
                pfnRemove
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoRegisterObject2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       objectName,
                ULONG                       oid,
                char*                       baseObjectName,
                ULONG                       baseOid,
                char*                       derivedType,
                ULONG                       brokerOid,
                void*                       pfnCreate,
                void*                       pfnRemove
            );

    description:

        This function is called to register a feature object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       objectName
                Specifies the name of the containter object to be
                registered.

                ULONG                       oid
                Specifies the oid of the container object to be
                registered.

                char*                       baseObjectName
                Specifies the name of the base containter object to be
                registered.

                ULONG                       baseOid
                Specifies the oid of the base container object to be
                registered.

                char*                       derivedType
                Specifies the derived type of the containter object to
                be registered.

                ULONG                       brokerOid
                Specifies the oid of the broker object of the object to
                be registered.

                void*                       pfnCreate
                Specifies the address of the create routine.

                void*                       pfnRemove
                Specifies the address of the remove routine.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoRegisterObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        char*                       baseObjectName,
        ULONG                       baseOid,
        char*                       derivedType,
        ULONG                       brokerOid,
        void*                       pfnCreate,
        void*                       pfnRemove
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    returnStatus =
        pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                objectName,
                oid,
                baseObjectName,
                baseOid,
                derivedType,
                brokerOid,
                pfnCreate,
                pfnRemove
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoCreateObjectByName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                char*                       objectName
            );

    description:

        This function manufactures a container object based on the
        desired name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                This handle specifies the owner's context.

                char*                       objectName
                Specifies the name of the object to be manufactured.

    return:     target handle of the created object.

**********************************************************************/

ANSC_HANDLE
AnscOcoCreateObjectByName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       objectName
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    return  pInternalObjectMapper->CreateObjectByName
                (
                    (ANSC_HANDLE)pInternalObjectMapper,
                    hOwnerContext,
                    objectName
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoCreateObjectByOid
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                ULONG                       oid
            );

    description:

        This function manufactures a container object based on the
        desired object identification.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                This handle specifies the owner's context.

                ULONG                       oid
                Specifies the oid of the object to be manufactured.

    return:     target handle of the created object.

**********************************************************************/

ANSC_HANDLE
AnscOcoCreateObjectByOid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    return  pInternalObjectMapper->CreateObjectByOid
                (
                    (ANSC_HANDLE)pInternalObjectMapper,
                    hOwnerContext,
                    oid
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoCreateObjectByType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                char*                       baseObjectName,
                char*                       derivedType
            );

    description:

        This function manufactures a container object based on the
        desired object type and base object name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                This handle specifies the owner's context.

                char*                       baseObjectName
                Specifies the name of the base object to be
                manufactured.

                char*                       derivedType
                Specifies the derived type of the object to be created.

    return:     target handle of the created object.

**********************************************************************/

ANSC_HANDLE
AnscOcoCreateObjectByType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseObjectName,
        char*                       derivedType
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    return  pInternalObjectMapper->CreateObjectByType
                (
                    (ANSC_HANDLE)pInternalObjectMapper,
                    hOwnerContext,
                    baseObjectName,
                    derivedType
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetObjectByRid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       rid
            );

    description:

        This function maps a runtime object id into the object handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       rid
                Specifies the rid of the object to be mapped.

    return:     handle of the runtime object.

**********************************************************************/

ANSC_HANDLE
AnscOcoGetObjectByRid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;

    return  pInternalObjectMapper->GetObjectByRid
                (
                    (ANSC_HANDLE)pInternalObjectMapper,
                    rid
                );
}
