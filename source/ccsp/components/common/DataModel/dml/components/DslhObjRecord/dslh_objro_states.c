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

    module:	dslh_objro_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Obj Record Object.

        *   DslhObjroSetDslhObjEntity
        *   DslhObjroSetLastName
        *   DslhObjroSetFullName
        *   DslhObjroDestroy
        *   DslhObjroReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/26/2005    initial revision.
        01/31/2011    Bin updated to remove child entries in ObjController
                      by calling "DelEntryObject".

**********************************************************************/


#include "dslh_objro_global.h"



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroSetDslhObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjEntity
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjEntity
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroSetDslhObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjEntity
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject    = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity   = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;

    pMyObject->hDslhObjEntity = hObjEntity;
    pObjEntity                = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;

    pObjEntity->AddObjRecord((ANSC_HANDLE)pObjEntity, (ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroSetLastName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pName
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pName
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroSetLastName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject    = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;

    if ( pMyObject->LastName )
    {
        AnscFreeMemory(pMyObject->LastName);

        pMyObject->LastName = NULL;
    }

    pMyObject->LastName = AnscCloneString(pName);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroSetFullName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pName
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pName
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroSetFullName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject    = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;

    if ( pMyObject->FullName )
    {
        AnscFreeMemory(pMyObject->FullName);

        pMyObject->FullName = NULL;
    }

    pMyObject->FullName = AnscCloneString(pName);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroDestroy
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroDestroy
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject               = (PDSLH_OBJ_RECORD_OBJECT )hThisObject;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController          = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJCO_TABLE_OBJECT        pObjcoTable             = (PDSLH_OBJCO_TABLE_OBJECT   )pObjController;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity              = (PDSLH_OBJ_ENTITY_OBJECT )pMyObject->hDslhObjEntity;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY         )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord         = (PDSLH_OBJ_RECORD_OBJECT    )NULL;

    /* Destroy all the child objects underneath first */
    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pChildObjRecord->Destroy ((ANSC_HANDLE)pChildObjRecord);

        if( pObjEntity->ObjDescr != NULL && pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
        {
            if(pObjcoTable != NULL)
            {
                pObjcoTable->DelEntryObject(pObjcoTable, pChildObjRecord->hDslhObjController);
            }
        }
    }

    /* remove it from ObjEntity's object list */
    pObjEntity->DelObjRecord((ANSC_HANDLE)pObjEntity, (ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroReset
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
DslhObjroReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject         = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController    = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord   = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord   = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;

    if ( pMyObject->LastName )
    {
        AnscFreeMemory(pMyObject->LastName);

        pMyObject->LastName = NULL;
    }

    if ( pMyObject->FullName )
    {
        AnscFreeMemory(pMyObject->FullName);

        pMyObject->FullName = NULL;
    }


    if ( TRUE )
    {
        if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
        {
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->VarroTable);

            while ( pSLinkEntry )
            {
                pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
                pSLinkEntry     = AnscQueuePopEntry(&pMyObject->VarroTable);

                pChildVarRecord->Reset ((ANSC_HANDLE)pChildVarRecord);
                pChildVarRecord->Remove((ANSC_HANDLE)pChildVarRecord);
            }
        }
    }

    /* delete child object record */
    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ObjroQueue);

        while ( pSLinkEntry )
        {
            pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ObjroQueue);

            pChildObjRecord->Reset ((ANSC_HANDLE)pChildObjRecord);
            pChildObjRecord->Remove((ANSC_HANDLE)pChildObjRecord);
        }
    }

    if ( pObjController )
    {
        pObjController->Remove((ANSC_HANDLE)pObjController);
        pMyObject->hDslhObjController = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
