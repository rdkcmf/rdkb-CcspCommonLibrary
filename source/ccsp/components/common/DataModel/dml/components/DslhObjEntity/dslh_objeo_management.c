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

    module:	dslh_objeo_management.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Dslh Obj Entity Object.

        *   DslhObjeoGetObjEntity2
        *   DslhObjeoGetObjEntity3
        *   DslhObjeoAddObjEntity
        *   DslhObjeoDelObjEntity
        *   DslhObjeoAddVarEntity
        *   DslhObjeoDelVarEntity
        *   DslhObjeoGetVarEntity
        *   DslhObjeoGetObjRecordCount
        *   DslhObjeoGetObjRecordCount2
        *   DslhObjeoGetObjRecord
        *   DslhObjeoGetObjRecord2
        *   DslhObjeoAddObjRecord
        *   DslhObjeoDelObjRecord
        *   DslhObjeoPopulateObjRecordByName
        *   DslhObjeoDeleteObjRecordByName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        01/11/2011    Bin added "GetVarEntity";
        01/18/2011    Bin added dynamically populate/Delete object apis;

**********************************************************************/


#include "dslh_objeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjeoGetObjEntity2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve an obj entity recursively.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the name of the obj entity in the format of
                a token chain.

    return:     obj entity.

**********************************************************************/

ANSC_HANDLE
DslhObjeoGetObjEntity2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PANSC_TOKEN_CHAIN               pObjNameTokens  = (PANSC_TOKEN_CHAIN      )hTokenChain;
    PANSC_STRING_TOKEN              pObjNameToken   = (PANSC_STRING_TOKEN     )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    BOOL                            bObjEntityFound = FALSE;

    if ( !pObjNameTokens )
    {
        return  (ANSC_HANDLE)pMyObject;
    }
    else
    {
        pObjNameToken = AnscTcUnlinkToken(pObjNameTokens);
    }

    if ( !pObjNameToken )
    {
        return  (ANSC_HANDLE)pMyObject;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjeoQueue);

    while ( pSLinkEntry )
    {
        pChildObjEntity = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp
                (
                    pChildObjEntity->LastName,
                    pObjNameToken->Name
                ) == 0 )
        {
            bObjEntityFound = TRUE;

            break;
        }
    }

    AnscFreeMemory(pObjNameToken);

    if ( bObjEntityFound )
    {
        if ( AnscTcGetTokenCount(pObjNameTokens) == 0 )
        {
            return  (ANSC_HANDLE)pChildObjEntity;
        }
        else
        {
            return  pChildObjEntity->GetObjEntity2
                        (
                            (ANSC_HANDLE)pChildObjEntity,
                            (ANSC_HANDLE)pObjNameTokens
                        );
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjeoGetObjEntity3
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the first obj entity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     obj entity.

**********************************************************************/

ANSC_HANDLE
DslhObjeoGetObjEntity3
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjeoQueue);

    if ( pSLinkEntry )
    {
        pChildObjEntity = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pChildObjEntity;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoAddObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjEntity
            );

    description:

        This function is called to add an obj entity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjEntity
                Specifies the new obj entity to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoAddObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjEntity
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)hObjEntity;

    AnscQueuePushEntry(&pMyObject->ObjeoQueue, &pChildObjEntity->Linkage);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoDelObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to delete an entity specified by the name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the obj entity to be deleted;

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoDelObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjeoQueue);

    while ( pSLinkEntry != NULL )
    {
        pChildObjEntity = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);

        if( pChildObjEntity->LastName && strcmp(pChildObjEntity->LastName, pObjName) == 0)
        {
            AnscQueuePopEntryByLink(&pMyObject->ObjeoQueue, &pChildObjEntity->Linkage);

            pChildObjEntity->Remove(pChildObjEntity);

            return ANSC_STATUS_SUCCESS;
        }

        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoAddVarEntity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarEntity
            );

    description:

        This function is called to add an var entity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarEntity
                Specifies the new var entity to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoAddVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)hVarEntity;

    AnscQueuePushEntry(&pMyObject->VareoTable, &pChildVarEntity->Linkage);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoDelVarEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pVarName
            );

    description:

        This function is called to delete a var entity specified by name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pVarName
                Specifies the var entity to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoDelVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pVarName
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY         )NULL;

    if( pVarName == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VareoTable);

    while ( pSLinkEntry != NULL )
    {
        pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);

        if( pChildVarEntity->ParamDescr && strcmp(pChildVarEntity->ParamDescr->Name, pVarName) == 0)
        {
            AnscQueuePopEntryByLink(&pMyObject->VareoTable, &pChildVarEntity->Linkage);

            DslhVareoRemove(pChildVarEntity);

            return ANSC_STATUS_SUCCESS;
        }

        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjeoGetVarEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve VarEntity specified by name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name;

    return:     The handle of the VarEntity;

**********************************************************************/
ANSC_HANDLE
DslhObjeoGetVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY         )NULL;

    if( pParamName == NULL)
    {
        return NULL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VareoTable);

    while ( pSLinkEntry != NULL )
    {
        pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);

        if( pChildVarEntity->ParamDescr && strcmp(pChildVarEntity->ParamDescr->Name, pParamName) == 0 )
        {
            return pChildVarEntity;
        }

        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
    }

    return  NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjeoGetObjRecordCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of instances.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     instance number.

**********************************************************************/

ULONG
DslhObjeoGetObjRecordCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;

    return  AnscQueueQueryDepth(&pMyObject->ObjInsQueue);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjeoGetObjRecordCount2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pPartialPath
            );

    description:

        This function is called to retrieve the number of instances
        that match the specified partial path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pPartialPath
                Specifies the partial object path to be matched.

    return:     instance number.

**********************************************************************/

ULONG
DslhObjeoGetObjRecordCount2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulMatchCount   = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);
        pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjeoInstance->hObjRecord;

        if ( _ansc_strstr
                (
                    pDslhObjRecord->FullName,
                    pPartialPath
                ) )
        {
            ulMatchCount++;
        }
    }

    return  ulMatchCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjeoGetObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to retrieve an object record by
        matching a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name to be matched.

                SLAP_VARIABLE*              pParamValue
                Specifies the parameter value to be matched.

    return:     object record.

**********************************************************************/

ANSC_HANDLE
DslhObjeoGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PDSLH_VAR_RECORD_OBJECT         pDslhVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;
    PSLAP_VARIABLE                  pCurParamValue = (PSLAP_VARIABLE         )NULL;
    BOOL                            bValueMatched  = FALSE;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);
        pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjeoInstance->hObjRecord;
        pDslhVarRecord =
            (PDSLH_VAR_RECORD_OBJECT)pDslhObjRecord->GetVarRecord
                (
                    (ANSC_HANDLE)pDslhObjRecord,
                    pParamName
                );

        if ( !pDslhVarRecord )
        {
            continue;
        }
        else
        {
            pCurParamValue = pDslhVarRecord->GetValue((ANSC_HANDLE)pDslhVarRecord);
        }

        if ( !pCurParamValue )
        {
            continue;
        }
        else
        {
             SlapEqualVariables(pParamValue, pCurParamValue, bValueMatched);
        }

        SlapFreeVariable(pCurParamValue);

        if ( bValueMatched )
        {
            break;
        }
    }

    if ( bValueMatched )
    {
        return  (ANSC_HANDLE)pDslhObjRecord;
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjeoGetObjRecord2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pPartialPath,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve an object record by
        matching the partial path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pPartialPath
                Specifies the partial object path to be matched.

                ULONG                       ulIndex
                Specifies the index of the matched object record.

    return:     object record.

**********************************************************************/

ANSC_HANDLE
DslhObjeoGetObjRecord2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath,
        ULONG                       ulIndex
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulMatchCount   = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);
        pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjeoInstance->hObjRecord;

        if ( _ansc_strstr
                (
                    pDslhObjRecord->FullName,
                    pPartialPath
                ) )
        {
            if ( ulMatchCount == ulIndex )
            {
                return  (ANSC_HANDLE)pDslhObjRecord;
            }
            else
            {
                ulMatchCount++;
            }
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoAddObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to add an object record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the object record to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoAddObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;

    pObjeoInstance = (PDSLH_OBJEO_INSTANCE)AnscAllocateMemory(sizeof(DSLH_OBJEO_INSTANCE));

    if ( !pObjeoInstance )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjeoInstance->hObjRecord = hObjRecord;
    }

    AnscQueuePushEntry(&pMyObject->ObjInsQueue, &pObjeoInstance->Linkage);

    pMyObject->InsUpdateTimestamp = AnscGetTickInSeconds();

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoDelObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to delete an object record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the object record to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoDelObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pObjeoInstance->hObjRecord == hObjRecord )
        {
            AnscQueuePopEntryByLink(&pMyObject->ObjInsQueue, &pObjeoInstance->Linkage);

            AnscFreeMemory(pObjeoInstance);

            pMyObject->InsUpdateTimestamp = AnscGetTickInSeconds();

            break;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoPopulateObjRecordByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to populate object specified by name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                The object specified by name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhObjeoPopulateObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);
        pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjeoInstance->hObjRecord;

        pDslhObjRecord->PopulateObjRecordByName(pDslhObjRecord,pObjName);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoDeleteObjRecordByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to Delete object specified by name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                The object specified by name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhObjeoDeleteObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject      = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance = (PDSLH_OBJEO_INSTANCE   )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjInsQueue);

    while ( pSLinkEntry )
    {
        pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);
        pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjeoInstance->hObjRecord;

        pDslhObjRecord->DeleteObjRecordByName(pDslhObjRecord,pObjName);
    }

    return ANSC_STATUS_SUCCESS;
}
