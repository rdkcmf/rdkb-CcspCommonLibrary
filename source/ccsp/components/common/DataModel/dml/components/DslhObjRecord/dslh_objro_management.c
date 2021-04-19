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

    module:	dslh_objro_management.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Dslh Obj Record Object.

        *   DslhObjroGetObjRecordCount
        *   DslhObjroGetObjRecord
        *   DslhObjroGetObjRecord2
        *   DslhObjroAddObjRecord
        *   DslhObjroDelAllObjRecords
        *   DslhObjroGetVarRecordCount
        *   DslhObjroGetVarRecord
        *   DslhObjroAddVarRecord

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/27/05    initial revision.

**********************************************************************/


#include "dslh_objro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjroGetObjRecordCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of child obj
        records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record count.

**********************************************************************/

ULONG
DslhObjroGetObjRecordCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;

    return  AnscQueueQueryDepth(&pMyObject->ObjroQueue);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjroGetObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a child obj record by
        matching the last name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the obj record name to be matched.

    return:     obj record.

**********************************************************************/

ANSC_HANDLE
DslhObjroGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( name && pChildObjRecord->LastName && strcmp(pChildObjRecord->LastName,name) == 0 )
        {
            return  (ANSC_HANDLE)pChildObjRecord;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjroGetObjRecord2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve a child obj record
        recursively.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the name of the obj record in the format of
                a token chain.

    return:     obj record.

**********************************************************************/

ANSC_HANDLE
DslhObjroGetObjRecord2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PANSC_TOKEN_CHAIN               pObjNameTokens      = (PANSC_TOKEN_CHAIN          )hTokenChain;
    PANSC_STRING_TOKEN              pObjNameToken       = (PANSC_STRING_TOKEN         )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_OBJCO_TABLE_OBJECT        pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY         )NULL;
    BOOL                            bObjRecordFound     = FALSE;

    if ( !pObjNameTokens )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pObjNameToken = AnscTcUnlinkToken(pObjNameTokens);
    }

    if ( !pObjNameToken )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pObjNameToken->Name && pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pChildObjRecord->LastName && strcmp(pChildObjRecord->LastName,pObjNameToken->Name) == 0 )
        {
            bObjRecordFound = TRUE;

            break;
        }
    }

    AnscFreeMemory(pObjNameToken);

    if ( bObjRecordFound )
    {
        pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pChildObjRecord->hDslhObjEntity;
        pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pChildObjRecord->hDslhObjController;
        pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )pChildObjController;

        if ( AnscTcGetTokenCount(pObjNameTokens) == 0 )
        {
            if ( pChildObjEntity->ObjDescr->bDynamic && pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
            {
                if ( pChildObjcoTable->IsUpdated((ANSC_HANDLE)pChildObjcoTable) )
                {
					AnscTrace("Delete and Repopulate the record in '%s'\n", pChildObjEntity->LastName);

                    pChildObjRecord->DelAllObjRecords  ((ANSC_HANDLE)pChildObjRecord);

                    pChildObjcoTable->Lock(pChildObjcoTable);
                    pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);
                    pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);
                    pChildObjcoTable->Unlock(pChildObjcoTable);

   				    AnscTrace("End of the repopulation in '%s'\n", pChildObjEntity->LastName);

                }
            }

            return  (ANSC_HANDLE)pChildObjRecord;
        }
        else
        {
            /*
             * If this obj record is a dynamic table object, we need to check whether the table
             * content has been updated. If it has, we need to remove all child objects and para-
             * meters under this object and re-populate them.
             */
            if ( pChildObjEntity->ObjDescr->bDynamic && pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
            {
                if ( pChildObjcoTable->IsUpdated((ANSC_HANDLE)pChildObjcoTable) )
                {
					AnscTrace("Delete and Repopulate the record in '%s'\n", pChildObjEntity->LastName);

                    pChildObjRecord->DelAllObjRecords  ((ANSC_HANDLE)pChildObjRecord);

                    pChildObjcoTable->Lock(pChildObjcoTable);
                    pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);
                    pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);
                    pChildObjcoTable->Unlock(pChildObjcoTable);

                    AnscTrace("End of the repopulation in '%s'\n", pChildObjEntity->LastName);

                }
            }

            return  pChildObjRecord->GetObjRecord2
                        (
                            (ANSC_HANDLE)pChildObjRecord,
                            (ANSC_HANDLE)pObjNameTokens
                        );
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroAddObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to add a child obj record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the obj record to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroAddObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord = (PDSLH_OBJ_RECORD_OBJECT)hObjRecord;
    char                            next_ins_name[16];

    AnscQueuePushEntry(&pMyObject->ObjroQueue, &pChildObjRecord->Linkage);

    if ( pObjEntity->ObjDescr && (pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table) )
    {
        pMyObject->NextInstanceNumber = (ULONG)_ansc_atoi(pChildObjRecord->LastName);

        do
        {
            pMyObject->NextInstanceNumber++;

            if ( pMyObject->NextInstanceNumber == 0 )
            {
                pMyObject->NextInstanceNumber = 1;
            }

            AnscZeroMemory(next_ins_name, 16);
            _ansc_itoa(pMyObject->NextInstanceNumber, next_ins_name, 10);

            pChildObjRecord =
                (PDSLH_OBJ_RECORD_OBJECT)pMyObject->GetObjRecord
                    (
                        (ANSC_HANDLE)pMyObject,
                        next_ins_name
                    );
        }
        while ( pChildObjRecord );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroDelAllObjRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy all child obj records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroDelAllObjRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ObjroQueue);

        pChildObjRecord->Destroy((ANSC_HANDLE)pChildObjRecord);
        pChildObjRecord->Reset  ((ANSC_HANDLE)pChildObjRecord);
        pChildObjRecord->Remove ((ANSC_HANDLE)pChildObjRecord);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjroGetVarRecordCount
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bExcludeHidden
            );

    description:

        This function is called to retrieve the number of child var
        records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bExcludeHidden
                exclude hidden parameters or not;

    return:     record count.

**********************************************************************/

ULONG
DslhObjroGetVarRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bExcludeHidden
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject        = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord  = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulVarRecordCount = 0;

    if( bExcludeHidden )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

        while ( pSLinkEntry )
        {
            pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
            pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if( pChildVarEntity->ParamDescr && !pChildVarEntity->ParamDescr->bInvisible)
            {
                ulVarRecordCount ++;
            }
        }

    }
    else
    {
        /* include all the parameters */
        ulVarRecordCount += AnscQueueQueryDepth(&pMyObject->VarroTable);
    }

    return  ulVarRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjroGetVarRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a child var record by
        matching the last name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the var record name to be matched.

    return:     var record.

**********************************************************************/

ANSC_HANDLE
DslhObjroGetVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

    while ( pSLinkEntry )
    {
        pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pChildVarRecord->GetLastName((ANSC_HANDLE)pChildVarRecord),name) == 0 )
        {
            return  (ANSC_HANDLE)pChildVarRecord;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroAddVarRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord
            );

    description:

        This function is called to add a child var record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the var record to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroAddVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)hVarRecord;

    AnscQueuePushEntry(&pMyObject->VarroTable, &pChildVarRecord->Linkage);

    return  ANSC_STATUS_SUCCESS;
}
