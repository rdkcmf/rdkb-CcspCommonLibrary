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

    module:	slap_oco_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Obj Container Object.

        *   SlapOcoGetSlapUoaIf
        *   SlapOcoSetSlapUoaIf
        *   SlapOcoGetSlapBssIf
        *   SlapOcoSetSlapBssIf
        *   SlapOcoGetSlapObjMapper
        *   SlapOcoSetSlapObjMapper
        *   SlapOcoGetContainerName
        *   SlapOcoSetContainerName
        *   SlapOcoGetContainerType
        *   SlapOcoSetContainerType
        *   SlapOcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/06/03    initial revision.

**********************************************************************/


#include "slap_oco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOcoGetSlapUoaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapOcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hSlapUoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoSetSlapUoaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    pMyObject->hSlapUoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOcoGetSlapBssIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapOcoGetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hSlapBssIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoSetSlapBssIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoSetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    pMyObject->hSlapBssIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOcoGetSlapObjMapper
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapOcoGetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hSlapObjMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoSetSlapObjMapper
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjMapper
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjMapper
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoSetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjMapper
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    pMyObject->hSlapObjMapper = hObjMapper;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapOcoGetContainerName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
SlapOcoGetContainerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->ContainerName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoSetContainerName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       container_name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       container_name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoSetContainerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    if ( pMyObject->ContainerName )
    {
        AnscFreeMemory(pMyObject->ContainerName);

        pMyObject->ContainerName = NULL;
    }

    pMyObject->ContainerName = AnscCloneString(container_name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOcoGetContainerType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
SlapOcoGetContainerType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->ContainerType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoSetContainerType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       container_type
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       container_type
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoSetContainerType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       container_type
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    pMyObject->ContainerType = container_type;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    if ( pMyObject->ContainerName )
    {
        AnscFreeMemory(pMyObject->ContainerName);

        pMyObject->ContainerName = NULL;
    }

    pMyObject->DelAllSrvComponents((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
