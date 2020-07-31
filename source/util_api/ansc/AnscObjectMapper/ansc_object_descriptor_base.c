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

    module:	ansc_object_descriptor_base.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Object Descriptor Object.

        *   AnscOdoCreate
        *   AnscOdoRemove
        *   AnscOdoReset

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
        AnscOdoCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the General Object Descriptor Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created socket object.

**********************************************************************/

ANSC_HANDLE
AnscOdoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = NULL;

    pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)AnscAllocateMemory(sizeof(ANSC_OBJECT_DESCRIPTOR_OBJECT));

    if ( !pObjectDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjectDescriptor->hContainerContext   = (ANSC_HANDLE)NULL;
    pObjectDescriptor->hOwnerContext       = hOwnerContext;
    pObjectDescriptor->hMapperReserved     = (ANSC_HANDLE)NULL;
    pObjectDescriptor->pfnObjectCreate     = NULL;
    pObjectDescriptor->pfnObjectRemove     = NULL;

    pObjectDescriptor->Create              = AnscOdoCreate;
    pObjectDescriptor->Remove              = AnscOdoRemove;
    pObjectDescriptor->Reset               = AnscOdoReset;

    pObjectDescriptor->GetContainerContext = AnscOdoGetContainerContext;
    pObjectDescriptor->SetContainerContext = AnscOdoSetContainerContext;
    pObjectDescriptor->GetMapperReserved   = AnscOdoGetMapperReserved;
    pObjectDescriptor->SetMapperReserved   = AnscOdoSetMapperReserved;

    pObjectDescriptor->GetCreateEntry      = AnscOdoGetCreateEntry;
    pObjectDescriptor->SetCreateEntry      = AnscOdoSetCreateEntry;
    pObjectDescriptor->GetRemoveEntry      = AnscOdoGetRemoveEntry;
    pObjectDescriptor->SetRemoveEntry      = AnscOdoSetRemoveEntry;

    pObjectDescriptor->GetName             = AnscOdoGetName;
    pObjectDescriptor->SetName             = AnscOdoSetName;
    pObjectDescriptor->GetOid              = AnscOdoGetOid;
    pObjectDescriptor->SetOid              = AnscOdoSetOid;
    pObjectDescriptor->GetBaseName         = AnscOdoGetBaseName;
    pObjectDescriptor->SetBaseName         = AnscOdoSetBaseName;
    pObjectDescriptor->GetBaseOid          = AnscOdoGetBaseOid;
    pObjectDescriptor->SetBaseOid          = AnscOdoSetBaseOid;
    pObjectDescriptor->GetDerivedType      = AnscOdoGetDerivedType;
    pObjectDescriptor->SetDerivedType      = AnscOdoSetDerivedType;
    pObjectDescriptor->GetBrokerOid        = AnscOdoGetBrokerOid;
    pObjectDescriptor->SetBrokerOid        = AnscOdoSetBrokerOid;

    pObjectDescriptor->CreateObject        = AnscOdoCreateObject;
    pObjectDescriptor->RemoveObject        = AnscOdoRemoveObject;

    return  (ANSC_HANDLE)pObjectDescriptor;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoRemove
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
AnscOdoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->Reset((ANSC_HANDLE)pObjectDescriptor);

    AnscFreeMemory(pObjectDescriptor);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoReset
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
AnscOdoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
