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

    module:	ansc_object_mapper_operation.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the General Object Mapper Object.

        *   AnscOmoRegisterObject
        *   AnscOmoCreateObjectByName
        *   AnscOmoCreateObjectByOid
        *   AnscOmoCreateObjectByType
        *   AnscOmoGetObjectByRid
        *   AnscOmoGetObjectsCount
        *   AnscOmoSerialize
        *   AnscOmoInterpret

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/19/01    initial revision.

**********************************************************************/


#include "ansc_object_mapper_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to register an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object name.

                ULONG                       oid
                Specifies the object id.

                char*                       baseName
                Specifies the base object name.

                ULONG                       baseOid
                Specifies the base object id.

                char*                       type
                Specifies the derived object type.

                ULONG                       brokerOid
                Specifies the broker object id.

                PFN_ANSC_OBJECT_CREATE      pfnCreate
                Specifies the object creation routine.

                PFN_ANSC_OBJECT_REMOVE      pfnRemove
                Specifies the object removal routine.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;

    pDescriptor =
        (PANSC_OBJECT_DESCRIPTOR_OBJECT)pObjectMapper->GetDescriptorByOid
            (
                (ANSC_HANDLE)pObjectMapper,
                oid
            );

    if ( pDescriptor )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)AnscCreateObjectDescriptor((ANSC_HANDLE)pObjectMapper);

    if ( !pDescriptor )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pDescriptor->SetContainerContext((ANSC_HANDLE)pDescriptor, pObjectMapper->hContainerContext);
        pDescriptor->SetCreateEntry     ((ANSC_HANDLE)pDescriptor, (ANSC_HANDLE)pfnCreate);
        pDescriptor->SetRemoveEntry     ((ANSC_HANDLE)pDescriptor, (ANSC_HANDLE)pfnRemove);

        pDescriptor->SetName            ((ANSC_HANDLE)pDescriptor, name);
        pDescriptor->SetOid             ((ANSC_HANDLE)pDescriptor, oid);
        pDescriptor->SetBaseName        ((ANSC_HANDLE)pDescriptor, baseName);
        pDescriptor->SetBaseOid         ((ANSC_HANDLE)pDescriptor, baseOid);
        pDescriptor->SetDerivedType     ((ANSC_HANDLE)pDescriptor, type);
        pDescriptor->SetBrokerOid       ((ANSC_HANDLE)pDescriptor, brokerOid);
    }

    returnStatus =
        pObjectMapper->AddDescriptor
            (
                (ANSC_HANDLE)pObjectMapper,
                (ANSC_HANDLE)pDescriptor
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoCreateObjectByName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                char*                       name
            );

    description:

        This function is called to create an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                Specifies the context of the object owner.

                char*                       name
                Specifies the object name to be created.

    return:     handle of the newly created object.

**********************************************************************/

ANSC_HANDLE
AnscOmoCreateObjectByName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       name
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;

    pDescriptor =
        (PANSC_OBJECT_DESCRIPTOR_OBJECT)pObjectMapper->GetDescriptorByName
            (
                (ANSC_HANDLE)pObjectMapper,
                name
            );

    if ( !pDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  pDescriptor->CreateObject((ANSC_HANDLE)pDescriptor, hOwnerContext);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoCreateObjectByOid
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                ULONG                       oid
            );

    description:

        This function is called to create an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                Specifies the context of the object owner.

                ULONG                       oid
                Specifies the object id to be created.

    return:     handle of the newly created object.

**********************************************************************/

ANSC_HANDLE
AnscOmoCreateObjectByOid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;

    pDescriptor =
        (PANSC_OBJECT_DESCRIPTOR_OBJECT)pObjectMapper->GetDescriptorByOid
            (
                (ANSC_HANDLE)pObjectMapper,
                oid
            );

    if ( !pDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  pDescriptor->CreateObject((ANSC_HANDLE)pDescriptor, hOwnerContext);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoCreateObjectByType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext,
                char*                       baseName,
                char*                       type
            );

    description:

        This function is called to create an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                Specifies the context of the object owner.

                char*                       baseName
                Specifies the base object name to be created.

                char*                       type
                Specifies the derived object type to be created.

    return:     handle of the newly created object.

**********************************************************************/

ANSC_HANDLE
AnscOmoCreateObjectByType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseName,
        char*                       type
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;

    pDescriptor =
        (PANSC_OBJECT_DESCRIPTOR_OBJECT)pObjectMapper->GetDescriptorByType
            (
                (ANSC_HANDLE)pObjectMapper,
                baseName,
                type
            );

    if ( !pDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  pDescriptor->CreateObject((ANSC_HANDLE)pDescriptor, hOwnerContext);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoGetObjectByRid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       rid
            );

    description:

        This function is called to map the runtime object id to the
        object handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       rid
                Specifies the runtime object id to be mapped.

    return:     handle of the corresponding object.

**********************************************************************/

ANSC_HANDLE
AnscOmoGetObjectByRid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  (ANSC_HANDLE)rid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscOmoGetObjectsCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the count of objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of object descriptors.

**********************************************************************/

ULONG
AnscOmoGetObjectsCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper  = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    ULONG                           ulObjectsCount = 0;
    ULONG                           i              = 0;

    AnscAcquireLock(&pObjectMapper->DescriptorTableLock);

    for ( i = 0; i < DEFAULT_OD_TABLE_SIZE; i++ )
    {
        ulObjectsCount += AnscSListQueryDepth(&pObjectMapper->DescriptorTable[i]);
    }

    AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

    return  ulObjectsCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoSerialize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSerialized
            );

    description:

        This function is called to serialize an object mapper stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSerialized
                Specifies the serialized object descriptor stream.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper   = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_TAG                pObjectTagArray = (PANSC_OBJECT_TAG)hSerialized;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           tableIndex      = 0;
    ULONG                           arrayIndex      = 0;
    errno_t                         rc              = -1;

    AnscAcquireLock(&pObjectMapper->DescriptorTableLock);

    for ( tableIndex = 0; tableIndex < DEFAULT_OD_TABLE_SIZE; tableIndex++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pObjectMapper->DescriptorTable[tableIndex]);

        while ( pSLinkEntry )
        {
            pDescriptor = ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            pObjectTagArray[arrayIndex].BaseOid         = pDescriptor->BaseOid;
            pObjectTagArray[arrayIndex].Oid             = pDescriptor->Oid;
            pObjectTagArray[arrayIndex].BrokerOid       = pDescriptor->BrokerOid;
            pObjectTagArray[arrayIndex].pfnObjectCreate = pDescriptor->pfnObjectCreate;
            pObjectTagArray[arrayIndex].pfnObjectRemove = pDescriptor->pfnObjectRemove;

            rc = strcpy_s(pObjectTagArray[arrayIndex].BaseName, sizeof(pObjectTagArray[arrayIndex].BaseName),    pDescriptor->BaseName);
            ERR_CHK(rc);
            rc = strcpy_s(pObjectTagArray[arrayIndex].Name, sizeof(pObjectTagArray[arrayIndex].Name),        pDescriptor->Name);
            ERR_CHK(rc);
            rc = strcpy_s(pObjectTagArray[arrayIndex].DerivedType, sizeof(pObjectTagArray[arrayIndex].DerivedType), pDescriptor->DerivedType);
            ERR_CHK(rc);

            arrayIndex++;
        }
    }

    AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoInterpret
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSerialized,
                ULONG                       ulCount
            );

    description:

        This function is called to interpret an object mapper stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSerialized
                Specifies the serialized object descriptor stream.

                ULONG                       ulCount
                Specifies the number of object descriptors contained
                in the serialized stream.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoInterpret
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerialized,
        ULONG                       ulCount
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper   = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_TAG                pObjectTagArray = (PANSC_OBJECT_TAG)hSerialized;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor     = NULL;
    ULONG                           i               = 0;

    for ( i = 0; i < ulCount; i++ )
    {
        pDescriptor =
            (PANSC_OBJECT_DESCRIPTOR_OBJECT)pObjectMapper->GetDescriptorByOid
                (
                    (ANSC_HANDLE)pObjectMapper,
                    pObjectTagArray[i].Oid
                );

        if ( pDescriptor )
        {
            continue;
        }

        pDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)AnscCreateObjectDescriptor((ANSC_HANDLE)pObjectMapper);

        if ( !pDescriptor )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pDescriptor->SetContainerContext((ANSC_HANDLE)pDescriptor, pObjectMapper->hContainerContext);
            pDescriptor->SetCreateEntry     ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].pfnObjectCreate);
            pDescriptor->SetRemoveEntry     ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].pfnObjectRemove);

            pDescriptor->SetName            ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].Name);
            pDescriptor->SetOid             ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].Oid);
            pDescriptor->SetBaseName        ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].BaseName);
            pDescriptor->SetBaseOid         ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].BaseOid);
            pDescriptor->SetDerivedType     ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].DerivedType);
            pDescriptor->SetBrokerOid       ((ANSC_HANDLE)pDescriptor, pObjectTagArray[i].BrokerOid);
        }

            pObjectMapper->AddDescriptor
                (
                    (ANSC_HANDLE)pObjectMapper,
                    (ANSC_HANDLE)pDescriptor
                );
    }

    return  ANSC_STATUS_SUCCESS;
}
