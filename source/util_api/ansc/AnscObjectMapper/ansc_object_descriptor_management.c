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

    module:	ansc_object_descriptor_management.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the General Object Descriptor Object.

        *   AnscOdoGetContainerContext
        *   AnscOdoSetContainerContext
        *   AnscOdoGetMapperReserved
        *   AnscOdoSetMapperReserved
        *   AnscOdoGetCreateEntry
        *   AnscOdoSetCreateEntry
        *   AnscOdoGetRemoveEntry
        *   AnscOdoSetRemoveEntry
        *   AnscOdoGetName
        *   AnscOdoSetName
        *   AnscOdoGetOid
        *   AnscOdoSetOid
        *   AnscOdoGetBaseName
        *   AnscOdoSetBaseName
        *   AnscOdoGetBaseOid
        *   AnscOdoSetBaseOid
        *   AnscOdoGetDerivedType
        *   AnscOdoSetDerivedType
        *   AnscOdoGetBrokerOid
        *   AnscOdoSetBrokerOid

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

        ANSC_HANDLE
        AnscOdoGetContainerContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscOdoGetContainerContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->hContainerContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetContainerContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContainer
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContainer
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetContainerContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->hContainerContext = hContainer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOdoGetMapperReserved
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscOdoGetMapperReserved
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->hMapperReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetMapperReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetMapperReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->hMapperReserved = hReserved;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOdoGetCreateEntry
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscOdoGetCreateEntry
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  (ANSC_HANDLE)pObjectDescriptor->pfnObjectCreate;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetCreateEntry
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntry
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntry
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetCreateEntry
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
        )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->pfnObjectCreate = (PFN_ANSC_OBJECT_CREATE)hEntry;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOdoGetRemoveEntry
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscOdoGetRemoveEntry
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  (ANSC_HANDLE)pObjectDescriptor->pfnObjectRemove;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetRemoveEntry
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntry
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntry
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetRemoveEntry
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->pfnObjectRemove = (PFN_ANSC_OBJECT_REMOVE)hEntry;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscOdoGetName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

char*
AnscOdoGetName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->Name;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    errno_t rc = -1;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    if(!name)
    return ANSC_STATUS_FAILURE;
    rc = strcpy_s(pObjectDescriptor->Name, sizeof(pObjectDescriptor->Name), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscOdoGetOid
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscOdoGetOid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->Oid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetOid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       oid
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       oid
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->Oid = oid;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscOdoGetBaseName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

char*
AnscOdoGetBaseName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->BaseName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetBaseName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetBaseName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    errno_t rc = -1;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    if(!name)
    return ANSC_STATUS_FAILURE;

    rc = strcpy_s(pObjectDescriptor->BaseName, sizeof(pObjectDescriptor->BaseName), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscOdoGetBaseOid
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscOdoGetBaseOid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->BaseOid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetBaseOid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       oid
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       oid
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetBaseOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->BaseOid = oid;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscOdoGetDerivedType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

char*
AnscOdoGetDerivedType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->DerivedType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetDerivedType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       type
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       type
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetDerivedType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       type
    )
{
    errno_t rc = -1;
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    if(!type)
    return ANSC_STATUS_FAILURE;
    rc = strcpy_s(pObjectDescriptor->DerivedType, sizeof(pObjectDescriptor->DerivedType), type);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscOdoGetBrokerOid
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscOdoGetBrokerOid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    return  pObjectDescriptor->BrokerOid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoSetBrokerOid
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       oid
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       oid
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoSetBrokerOid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       oid
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    pObjectDescriptor->BrokerOid = oid;

    return  ANSC_STATUS_SUCCESS;
}
