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

    module:	slap_oco_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Obj Container Object.

        *   SlapOcoAssignInstanceId
        *   SlapOcoGetSrvComponent
        *   SlapOcoAddSrvComponent
        *   SlapOcoDelSrvComponent
        *   SlapOcoDelAllSrvComponents

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

        ULONG
        SlapOcoAssignInstanceId
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to assign a valid instance id to the
        specified obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj name of the srv component object.

    return:     instance id.

**********************************************************************/

ULONG
SlapOcoAssignInstanceId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject         = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    do
    {
        pMyObject->GlobalInsId++;

        if ( pMyObject->GlobalInsId == 0 )
        {
            pMyObject->GlobalInsId++;
        }
    }
    while ( pMyObject->GetSrvComponent((ANSC_HANDLE)pMyObject, obj_name, pMyObject->GlobalInsId) != (ANSC_HANDLE)NULL );

    return  pMyObject->GlobalInsId;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOcoGetSrvComponent
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                ULONG                       instance_id
            );

    description:

        This function is called to retrieve a srv component object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj name of the srv component object.

                ULONG                       instance_id
                Specifies the runtime instance id assigned by the
                obj container.

    return:     srv component object.

**********************************************************************/

ANSC_HANDLE
SlapOcoGetSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ULONG                       instance_id
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject         = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    ULONG                           ulHashIndex       = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_OCO_SCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->ScoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ScoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSlapSrvComponent = ACCESS_SLAP_SRV_COMPONENT_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pSlapSrvComponent->Match1
                (
                    (ANSC_HANDLE)pSlapSrvComponent,
                    obj_name,
                    instance_id
                ) )
        {
            AnscReleaseLock(&pMyObject->ScoTableLock);

            return  (ANSC_HANDLE)pSlapSrvComponent;
        }
    }

    AnscReleaseLock(&pMyObject->ScoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoAddSrvComponent
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvComponent
            );

    description:

        This function is called to add a srv component object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvComponent
                Specifies the srv component object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoAddSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject         = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)hSrvComponent;
    char*                           pObjName          = (char*                     )pSlapSrvComponent->ObjName;
    ULONG                           ulHashIndex       = AnscHashString(pObjName, AnscSizeOfString(pObjName), SLAP_OCO_SCO_TABLE_SIZE);
    ULONG                           ulInstanceId      = pMyObject->AssignInstanceId((ANSC_HANDLE)pMyObject, pObjName);

    pSlapSrvComponent->HashIndex = ulHashIndex;

    pSlapSrvComponent->hSlapObjContainer = (ANSC_HANDLE)pMyObject;
    pSlapSrvComponent->ObjInstanceId     = ulInstanceId;

    AnscAcquireLock   (&pMyObject->ScoTableLock);
    AnscQueuePushEntry(&pMyObject->ScoTable[ulHashIndex], &pSlapSrvComponent->Linkage);
    AnscReleaseLock   (&pMyObject->ScoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoDelSrvComponent
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSrvComponent
            );

    description:

        This function is called to delete a srv component object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSrvComponent
                Specifies the srv component object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoDelSrvComponent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject         = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)hSrvComponent;
    ULONG                           ulHashIndex       = pSlapSrvComponent->HashIndex;

    AnscAcquireLock        (&pMyObject->ScoTableLock);
    AnscQueuePopEntryByLink(&pMyObject->ScoTable[ulHashIndex], &pSlapSrvComponent->Linkage);
    AnscReleaseLock        (&pMyObject->ScoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoDelAllSrvComponents
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all srv component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOcoDelAllSrvComponents
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject         = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf        = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->ScoTableLock);

    for ( i = 0; i < SLAP_OCO_SCO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ScoTable[i]);

        while ( pSLinkEntry )
        {
            pSlapSrvComponent = ACCESS_SLAP_SRV_COMPONENT_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscQueuePopEntry(&pMyObject->ScoTable[i]);

            pSlapUoaIf->DeleteObject2(pSlapUoaIf->hOwnerContext, pSlapSrvComponent->hSlapObjRecord);
        }
    }

    AnscReleaseLock(&pMyObject->ScoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
