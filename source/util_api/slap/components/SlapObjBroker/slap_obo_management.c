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

    module:	slap_obo_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Obj Broker Object.

        *   SlapOboGetObjBroker
        *   SlapOboAddObjBroker
        *   SlapOboDelObjBroker
        *   SlapOboDelAllObjBrokers

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/05/03    initial revision.

**********************************************************************/


#include "slap_obo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOboGetObjBroker
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to retrieve an obj broker object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj broker.

    return:     obj broker object.

**********************************************************************/

ANSC_HANDLE
SlapOboGetObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject     = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pSonObjBroker = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulHashIndex   = 0;

    ulHashIndex =
        AnscHashString
            (
                path,
                AnscSizeOfString(path),
                SLAP_OBO_OBO_TABLE_SIZE
            );

    AnscAcquireLock(&pMyObject->OboTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OboTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSonObjBroker = ACCESS_SLAP_OBJ_BROKER_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(path,pSonObjBroker->PathName) == 0 )
        {
            AnscReleaseLock(&pMyObject->OboTableLock);

            return  (ANSC_HANDLE)pSonObjBroker;
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOboAddObjBroker
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to add an obj broker object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj broker.

    return:     obj broker object.

**********************************************************************/

ANSC_HANDLE
SlapOboAddObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject     = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pSonObjBroker = NULL;
    ULONG                           ulHashIndex   = 0;

    pSonObjBroker =
        (PSLAP_OBJ_BROKER_OBJECT)pMyObject->GetObjBroker
            (
                (ANSC_HANDLE)pMyObject,
                path
            );

    if ( pSonObjBroker )
    {
        return  (ANSC_HANDLE)pSonObjBroker;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    path,
                    AnscSizeOfString(path),
                    SLAP_OBO_OBO_TABLE_SIZE
                );
    }

    pSonObjBroker =
        (PSLAP_OBJ_BROKER_OBJECT)SlapCreateObjBroker
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSonObjBroker )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSonObjBroker->HashIndex      = ulHashIndex;
        pSonObjBroker->hSlapObjEntity = (ANSC_HANDLE)NULL;

        pSonObjBroker->SetPathName((ANSC_HANDLE)pSonObjBroker, path);
    }

    AnscAcquireLock   (&pMyObject->OboTableLock);
    AnscQueuePushEntry(&pMyObject->OboTable[ulHashIndex], &pSonObjBroker->Linkage);
    AnscReleaseLock   (&pMyObject->OboTableLock);

    return  (ANSC_HANDLE)pSonObjBroker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOboDelObjBroker
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to delete an obj broker object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj broker.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOboDelObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject     = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pSonObjBroker = NULL;
    ULONG                           ulHashIndex   = 0;

    pSonObjBroker =
        (PSLAP_OBJ_BROKER_OBJECT)pMyObject->GetObjBroker
            (
                (ANSC_HANDLE)pMyObject,
                path
            );

    if ( !pSonObjBroker )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        ulHashIndex = pSonObjBroker->HashIndex;
    }

    AnscAcquireLock        (&pMyObject->OboTableLock);
    AnscQueuePopEntryByLink(&pMyObject->OboTable[ulHashIndex], &pSonObjBroker->Linkage);
    AnscReleaseLock        (&pMyObject->OboTableLock);

    pSonObjBroker->Remove((ANSC_HANDLE)pSonObjBroker);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOboDelAllObjBrokers
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all obj broker objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOboDelAllObjBrokers
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject     = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pSonObjBroker = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->OboTableLock);

    for ( i = 0; i < SLAP_OBO_OBO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OboTable[i]);

        while ( pSLinkEntry )
        {
            pSonObjBroker = ACCESS_SLAP_OBJ_BROKER_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscQueuePopEntry(&pMyObject->OboTable[i]);

            pSonObjBroker->Remove((ANSC_HANDLE)pSonObjBroker);
        }
    }

    AnscReleaseLock(&pMyObject->OboTableLock);

    return  ANSC_STATUS_SUCCESS;
}
