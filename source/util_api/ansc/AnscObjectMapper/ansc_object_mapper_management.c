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

    module:	ansc_object_mapper_management.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the General Object Mapper Object.

        *   AnscOmoGetContainerContext
        *   AnscOmoSetContainerContext
        *   AnscOmoAddDescriptor
        *   AnscOmoDelDescriptor
        *   AnscOmoDelAllDescriptors
        *   AnscOmoGetDescriptorByName
        *   AnscOmoGetDescriptorByOid
        *   AnscOmoGetDescriptorByType

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoGetContainerContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscOmoGetContainerContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;

    return  pObjectMapper->hContainerContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoSetContainerContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContainer
            );

    description:

        This function is called to modify the object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContainer
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoSetContainerContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;

    pObjectMapper->hContainerContext = hContainer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoAddDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDescriptor
            );

    description:

        This function is called to add a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDescriptor
                Specifies the object descriptor to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoAddDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hDescriptor;
    ULONG                           ulHashIndex   = AnscHashUlong(pDescriptor->GetOid((ANSC_HANDLE)pDescriptor), DEFAULT_OD_TABLE_SIZE);

    pDescriptor->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pObjectMapper->DescriptorTableLock);
    AnscSListPushEntry(&pObjectMapper->DescriptorTable[ulHashIndex], &pDescriptor->Linkage);
    AnscReleaseLock   (&pObjectMapper->DescriptorTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoDelDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDescriptor
            );

    description:

        This function is called to delete a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDescriptor
                Specifies the object descriptor to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoDelDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hDescriptor;

    AnscAcquireLock        (&pObjectMapper->DescriptorTableLock);
    AnscSListPopEntryByLink(&pObjectMapper->DescriptorTable[pDescriptor->HashIndex], &pDescriptor->Linkage);
    AnscReleaseLock        (&pObjectMapper->DescriptorTableLock);

    pDescriptor->Remove((ANSC_HANDLE)pDescriptor);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoDelAllDescriptors
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all object descriptors.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoDelAllDescriptors
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           i             = 0;

    for ( i = 0; i < DEFAULT_OD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pObjectMapper->DescriptorTable[i]);

        while ( pSLinkEntry )
        {
            pDescriptor = ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            pObjectMapper->DelDescriptor
                (
                    (ANSC_HANDLE)pObjectMapper,
                    (ANSC_HANDLE)pDescriptor
                );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoGetDescriptorByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a object descriptor by
        matching the name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object name to be matched.

    return:     handle of the matched object descriptor.

**********************************************************************/

ANSC_HANDLE
AnscOmoGetDescriptorByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           i             = 0;

    AnscAcquireLock(&pObjectMapper->DescriptorTableLock);

    for ( i = 0; i < DEFAULT_OD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pObjectMapper->DescriptorTable[i]);

        while ( pSLinkEntry )
        {
            pDescriptor = ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( strcasecmp(pDescriptor->GetName((ANSC_HANDLE)pDescriptor), name) == 0 )
            {
                AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

                return  (ANSC_HANDLE)pDescriptor;
            }
        }
    }

    AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoGetDescriptorByOid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       oid
            );

    description:

        This function is called to retrieve a object descriptor by
        matching the object id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       oid
                Specifies the object oid to be matched.

    return:     handle of the matched object descriptor.

**********************************************************************/

ANSC_HANDLE
AnscOmoGetDescriptorByOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulHashIndex   = AnscHashUlong(oid, DEFAULT_OD_TABLE_SIZE);

    AnscAcquireLock(&pObjectMapper->DescriptorTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pObjectMapper->DescriptorTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pDescriptor = ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pDescriptor->GetOid((ANSC_HANDLE)pDescriptor) == oid )
        {
            AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

            return  (ANSC_HANDLE)pDescriptor;
        }
    }

    AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOmoGetDescriptorByType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       baseName,
                char*                       type
            );

    description:

        This function is called to retrieve a object descriptor by
        matching the base object name and derived type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       baseName
                Specifies the base object name to be matched.

                char*                       type
                Specifies the derived type to be matched.

    return:     handle of the matched object descriptor.

**********************************************************************/

ANSC_HANDLE
AnscOmoGetDescriptorByType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       baseName,
        char*                       type
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pDescriptor   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           i             = 0;

    AnscAcquireLock(&pObjectMapper->DescriptorTableLock);

    for ( i = 0; i < DEFAULT_OD_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pObjectMapper->DescriptorTable[i]);

        while ( pSLinkEntry )
        {
            pDescriptor = ACCESS_ANSC_OBJECT_DESCRIPTOR_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( strcasecmp(pDescriptor->GetBaseName   ((ANSC_HANDLE)pDescriptor), baseName) == 0 &&
                 strcasecmp(pDescriptor->GetDerivedType((ANSC_HANDLE)pDescriptor), type) == 0 )
            {
                AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

                return  (ANSC_HANDLE)pDescriptor;
            }
        }
    }

    AnscReleaseLock(&pObjectMapper->DescriptorTableLock);

    return  (ANSC_HANDLE)NULL;
}
