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

    module:	ansc_object_mapper_base.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Object Mapper Object.

        *   AnscOmoCreate
        *   AnscOmoRemove
        *   AnscOmoReset

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


#include "ansc_object_mapper_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscOmoCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the General Object Mapper Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created socket object.

**********************************************************************/

ANSC_HANDLE
AnscOmoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = NULL;
    ULONG                           i             = 0;

    pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)AnscAllocateMemory(sizeof(ANSC_OBJECT_MAPPER_OBJECT));

    if ( !pObjectMapper )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjectMapper->hContainerContext   = (ANSC_HANDLE)NULL;
    pObjectMapper->hOwnerContext       = hOwnerContext;

    pObjectMapper->Create              = AnscOmoCreate;
    pObjectMapper->Remove              = AnscOmoRemove;
    pObjectMapper->Reset               = AnscOmoReset;

    pObjectMapper->GetContainerContext = AnscOmoGetContainerContext;
    pObjectMapper->SetContainerContext = AnscOmoSetContainerContext;

    pObjectMapper->RegisterObject      = AnscOmoRegisterObject;
    pObjectMapper->CreateObjectByName  = AnscOmoCreateObjectByName;
    pObjectMapper->CreateObjectByOid   = AnscOmoCreateObjectByOid;
    pObjectMapper->CreateObjectByType  = AnscOmoCreateObjectByType;
    pObjectMapper->GetObjectByRid      = AnscOmoGetObjectByRid;

    pObjectMapper->GetObjectsCount     = AnscOmoGetObjectsCount;
    pObjectMapper->Serialize           = AnscOmoSerialize;
    pObjectMapper->Interpret           = AnscOmoInterpret;

    pObjectMapper->AddDescriptor       = AnscOmoAddDescriptor;
    pObjectMapper->DelDescriptor       = AnscOmoDelDescriptor;
    pObjectMapper->DelAllDescriptors   = AnscOmoDelAllDescriptors;

    pObjectMapper->GetDescriptorByName = AnscOmoGetDescriptorByName;
    pObjectMapper->GetDescriptorByOid  = AnscOmoGetDescriptorByOid;
    pObjectMapper->GetDescriptorByType = AnscOmoGetDescriptorByType;

    for ( i = 0; i < DEFAULT_OD_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pObjectMapper->DescriptorTable[i]);
    }
    AnscInitializeLock(&pObjectMapper->DescriptorTableLock);

    return  (ANSC_HANDLE)pObjectMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;

    pObjectMapper->Reset((ANSC_HANDLE)pObjectMapper);

    AnscFreeLock(&pObjectMapper->DescriptorTableLock);
    AnscFreeMemory(pObjectMapper);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOmoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function resets the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOmoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_MAPPER_OBJECT      pObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)hThisObject;

    pObjectMapper->DelAllDescriptors((ANSC_HANDLE)pObjectMapper);

    return  ANSC_STATUS_SUCCESS;
}
