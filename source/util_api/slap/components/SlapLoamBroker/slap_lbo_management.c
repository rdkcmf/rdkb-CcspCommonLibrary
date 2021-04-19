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

    module:	slap_lbo_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Loam Broker Object.

        *   SlapLboGetObjBinding
        *   SlapLboAddObjBinding
        *   SlapLboDelObjBinding
        *   SlapLboDelObjBinding2
        *   SlapLboDelAllObjBindings

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_lbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLboGetObjBinding
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to retrieve an object binding by
        matching the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be matched.

    return:     object binding.

**********************************************************************/

ANSC_HANDLE
SlapLboGetObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding  = (PSLAP_LBO_OBJECT_BINDING)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           ulHashIndex  = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_LBO_OBO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OboTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OboTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjBinding = ACCESS_SLAP_LBO_OBJECT_BINDING(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pObjBinding->ObjectName,obj_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->OboTableLock);

            return  (ANSC_HANDLE)pObjBinding;
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboAddObjBinding
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                char*                       party_name
            );

    description:

        This function is called to add a new object binding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be added.

                char*                       party_name
                Specifies the party_name to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboAddObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char*                       party_name
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding  = (PSLAP_LBO_OBJECT_BINDING)NULL;
    ULONG                           ulHashIndex  = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_LBO_OBO_TABLE_SIZE);

    if ( pMyObject->GetObjBinding
            (
                (ANSC_HANDLE)pMyObject,
                obj_name
            ) != (ANSC_HANDLE)NULL )
    {
        return  ANSC_STATUS_COLLISION;
    }
    else
    {
        pObjBinding = (PSLAP_LBO_OBJECT_BINDING)AnscAllocateMemory(sizeof(SLAP_LBO_OBJECT_BINDING));
    }

    if ( !pObjBinding )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjBinding->HashIndex  = ulHashIndex;
        pObjBinding->ObjectName = AnscCloneString(obj_name  );
        pObjBinding->PartyName  = AnscCloneString(party_name);
    }

    if ( !pObjBinding->ObjectName || !pObjBinding->PartyName )
    {
        SlapLboFreeObjBinding(pObjBinding);

        return  ANSC_STATUS_RESOURCES;
    }

    AnscAcquireLock   (&pMyObject->OboTableLock);
    AnscQueuePushEntry(&pMyObject->OboTable[ulHashIndex], &pObjBinding->Linkage);
    AnscReleaseLock   (&pMyObject->OboTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboDelObjBinding
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to delete an object binding by matching
        the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboDelObjBinding
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding  = (PSLAP_LBO_OBJECT_BINDING)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           ulHashIndex  = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_LBO_OBO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OboTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OboTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjBinding = ACCESS_SLAP_LBO_OBJECT_BINDING(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pObjBinding->ObjectName,obj_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->OboTable[ulHashIndex], &pObjBinding->Linkage);

            SlapLboFreeObjBinding(pObjBinding);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboDelObjBinding2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to delete an object binding by matching
        the party_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the party_name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboDelObjBinding2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding  = (PSLAP_LBO_OBJECT_BINDING)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->OboTableLock);

    for ( i = 0; i < SLAP_LBO_OBO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OboTable[i]);

        while ( pSLinkEntry )
        {
            pObjBinding = ACCESS_SLAP_LBO_OBJECT_BINDING(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( strcmp(pObjBinding->PartyName,party_name) == 0 )
            {
                AnscQueuePopEntryByLink(&pMyObject->OboTable[i], &pObjBinding->Linkage);

                SlapLboFreeObjBinding(pObjBinding);
            }
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboDelAllObjBindings
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all object bindings.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboDelAllObjBindings
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding  = (PSLAP_LBO_OBJECT_BINDING)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->OboTableLock);

    for ( i = 0; i < SLAP_LBO_OBO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OboTable[i]);

        while ( pSLinkEntry )
        {
            pObjBinding = ACCESS_SLAP_LBO_OBJECT_BINDING(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->OboTable[i]);

            SlapLboFreeObjBinding(pObjBinding);
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  ANSC_STATUS_SUCCESS;
}
