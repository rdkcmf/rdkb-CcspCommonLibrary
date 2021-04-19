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

    module:	slap_eco_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Env Controller Object.

        *   SlapEcoGetSlapContainer
        *   SlapEcoAddSlapContainer
        *   SlapEcoDelSlapContainer1
        *   SlapEcoDelSlapContainer2
        *   SlapEcoDelAllSlapContainers

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/09/03    initial revision.

**********************************************************************/


#include "slap_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapContainer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       container_name
            );

    description:

        This function is called to retrieve an obj_container by
        matching the container name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       container_name
                Specifies the container name to be matched.

    return:     container object.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex       = AnscHashString(container_name, AnscSizeOfString(container_name), SLAP_ECO_OCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OcoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OcoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSlapObjContainer = ACCESS_SLAP_OBJ_CONTAINER_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pSlapObjContainer->GetContainerName((ANSC_HANDLE)pSlapObjContainer),container_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->OcoTableLock);

            return  (ANSC_HANDLE)pSlapObjContainer;
        }
    }

    AnscReleaseLock(&pMyObject->OcoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoAddSlapContainer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContainer
            );

    description:

        This function is called to add a slap object container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContainer
                Specifies the container object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoAddSlapContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )hContainer;
    char*                           pContainerName    = (char*                      )pSlapObjContainer->GetContainerName((ANSC_HANDLE)pSlapObjContainer);
    ULONG                           ulHashIndex       = AnscHashString(pContainerName, AnscSizeOfString(pContainerName), SLAP_ECO_OCO_TABLE_SIZE);

    pSlapObjContainer->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pMyObject->OcoTableLock);
    AnscQueuePushEntry(&pMyObject->OcoTable[ulHashIndex], &pSlapObjContainer->Linkage);
    AnscReleaseLock   (&pMyObject->OcoTableLock);

    if ( pSlapObjContainer->GetContainerType((ANSC_HANDLE)pSlapObjContainer) & SLAP_CONTAINER_TYPE_DEFAULT )
    {
        pMyObject->hSlapDefContainer = (ANSC_HANDLE)pSlapObjContainer;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoDelSlapContainer1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       container_name
            );

    description:

        This function is called to delete an obj_container by
        matching the container name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       container_name
                Specifies the container name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoDelSlapContainer1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex       = AnscHashString(container_name, AnscSizeOfString(container_name), SLAP_ECO_OCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OcoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OcoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSlapObjContainer = ACCESS_SLAP_OBJ_CONTAINER_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pSlapObjContainer->GetContainerName((ANSC_HANDLE)pSlapObjContainer),container_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->OcoTable[ulHashIndex], &pSlapObjContainer->Linkage);

            if ( pSlapObjContainer->GetContainerType((ANSC_HANDLE)pSlapObjContainer) & SLAP_CONTAINER_TYPE_DEFAULT )
            {
                pMyObject->hSlapDefContainer = (ANSC_HANDLE)NULL;
            }

            pSlapObjContainer->Remove((ANSC_HANDLE)pSlapObjContainer);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->OcoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoDelSlapContainer2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContainer
            );

    description:

        This function is called to delete an obj_container by
        matching the container object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContainer
                Specifies the container object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoDelSlapContainer2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )hContainer;
    ULONG                           ulHashIndex       = pSlapObjContainer->HashIndex;

    AnscAcquireLock        (&pMyObject->OcoTableLock);
    AnscQueuePopEntryByLink(&pMyObject->OcoTable[ulHashIndex], &pSlapObjContainer->Linkage);
    AnscReleaseLock        (&pMyObject->OcoTableLock);

    if ( pSlapObjContainer->GetContainerType((ANSC_HANDLE)pSlapObjContainer) & SLAP_CONTAINER_TYPE_DEFAULT )
    {
        pMyObject->hSlapDefContainer = (ANSC_HANDLE)NULL;
    }

    pSlapObjContainer->Remove((ANSC_HANDLE)pSlapObjContainer);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoDelAllSlapContainers
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all obj_container objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoDelAllSlapContainers
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->OcoTableLock);

    for ( i = 0; i < SLAP_ECO_OCO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OcoTable[i]);

        while ( pSLinkEntry )
        {
            pSlapObjContainer = ACCESS_SLAP_OBJ_CONTAINER_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscQueuePopEntry(&pMyObject->OcoTable[i]);

            pSlapObjContainer->Remove((ANSC_HANDLE)pSlapObjContainer);
        }
    }

    AnscReleaseLock(&pMyObject->OcoTableLock);

    if ( TRUE )
    {
        pMyObject->hSlapDefContainer = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
