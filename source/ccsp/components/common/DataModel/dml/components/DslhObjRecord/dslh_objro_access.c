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

    module:	dslh_objro_access.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced access functions
        of the Dslh Obj Record Object.

        *   DslhObjroGetTotalParamNumber
        *   DslhObjroGetAllParamValues
        *   DslhObjroGetAllParamInfos
        *   DslhObjroGetAllParamAttribs
        *   DslhObjroSetAllParamAttribs
        *   DslhObjroVerifyChanges
        *   DslhObjroCommitChanges
        *   DslhObjroCommitChanges2
        *   DslhObjroCancelChanges
        *   DslhObjroAddChildObject
        *   DslhObjroDelChildObject

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
        DslhObjroGetTotalParamNumber
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bFromAcs,
				BOOL						bSelfIncluded
            );

    description:

        This function is called to retrieve the total number of
        parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				BOOL						bSelfIncluded
				include itself or not;

    return:     parameter number.

**********************************************************************/

ULONG
DslhObjroGetTotalParamNumber
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs,
		BOOL						bSelfIncluded
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_OBJCO_TABLE_OBJECT        pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulParamNumber       = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pChildObjRecord->hDslhObjEntity;
        pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pChildObjRecord->hDslhObjController;
        pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )pChildObjController;

        if ( pChildObjEntity->ObjDescr->bDynamic && pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
        {
            if ( pChildObjcoTable->IsUpdated((ANSC_HANDLE)pChildObjcoTable) )
            {
                /*AnscTrace("Delete and Repopulate the record in '%s'\n", pChildObjEntity->LastName);*/

                pChildObjRecord->DelAllObjRecords  ((ANSC_HANDLE)pChildObjRecord);

                pChildObjcoTable->Lock(pChildObjcoTable);
                pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);
                pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);
                pChildObjcoTable->Unlock(pChildObjcoTable);

               /*AnscTrace("End of the repopulation in '%s'\n", pChildObjEntity->LastName);*/
            }
        }

        ulParamNumber += pChildObjRecord->GetTotalParamNumber((ANSC_HANDLE)pChildObjRecord, bFromAcs, bSelfIncluded);
    }

    ulParamNumber += pMyObject->GetVarRecordCount((ANSC_HANDLE)pMyObject, bFromAcs);

	if( bSelfIncluded)
	{
        if(!bFromAcs)
        {
		    ulParamNumber += 1;
        }
        else if (pObjEntity != NULL && pObjEntity->ObjDescr && !pObjEntity->ObjDescr->bInvisible)
        {
            ulParamNumber += 1;
        }
	}

    return  ulParamNumber;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroGetAllParamValues
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParamValueArray,
                PULONG                      pulArraySize,
                BOOL                        bFromAcs
            );

    description:

        This function is called to retrieve the value of all parameters
        under this branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParamValueArray
                Specifies the output parameter value array where the
                result is to be placed.

                PULONG                      pulArraySize
                Returns the number of parameters returned.

                BOOL                        bFromAcs
                whether it's from ACS or not

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroGetAllParamValues
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamValueArray,
        PULONG                      pulArraySize,
        BOOL                        bFromAcs,
        unsigned int                writeID
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject            = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity           = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_CWMP_PARAM_VALUE          pParameterValueArray = (PDSLH_CWMP_PARAM_VALUE )hParamValueArray;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord      = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity      = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord      = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i                    = 0;
    ULONG                           ulMaxParamCount      = *pulArraySize;
    ULONG                           ulTotalParamCount    = 0;
    ULONG                           ulParamCount         = 0;
    char**                          ppNameArray          = NULL;
    PSLAP_VARIABLE*                 ppValueArray         = NULL;
    ULONG                           uChildCount          = 0;
    ULONG                           j                    = 0;

    /* Don't count the invisible object in. We will quit here */
    if( pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs)
    {
        *pulArraySize = 0;

        return ANSC_STATUS_SUCCESS;
    }

    uChildCount = AnscQueueQueryDepth(&pMyObject->VarroTable);

    if( uChildCount > 0)
    {
        /* create the array */
        ppNameArray     = (char**)AnscAllocateMemory(sizeof(char*) * uChildCount);
        ppValueArray    = (PSLAP_VARIABLE*)AnscAllocateMemory(sizeof(PSLAP_VARIABLE) * uChildCount);

        if( !ppNameArray || !ppValueArray)
        {
            return ANSC_STATUS_FAILURE;
        }

        /* init the parameter names and SLAP_VARIABLE */
        ulParamCount = 0;

        if( TRUE )
        {
            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

            while ( pSLinkEntry )
            {
                pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
                pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
                pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                if ( pChildVarEntity->ParamDescr->bInvisible && bFromAcs )
                {
                    continue;
                }
           
                /* we will build an array for the parameters and call "GetValue" once */
                /* pParameterValueArray[ulParamCount].Value = pChildVarRecord->GetValue   ((ANSC_HANDLE)pChildVarRecord); */

                ppNameArray[ulParamCount]   = pChildVarRecord->GetLastName(pChildVarRecord);
                ppValueArray[ulParamCount]  = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

                if( ppValueArray[ulParamCount] == NULL)
                {
                    break;
                }

                ppValueArray[ulParamCount]->Name        = pChildVarRecord->GetFullName((ANSC_HANDLE)pChildVarRecord);
                ppValueArray[ulParamCount]->ContentType = pChildVarEntity->ContentType;
                ppValueArray[ulParamCount]->Syntax      = pChildVarEntity->Syntax;

                ulParamCount++;

            }
        }

        /* call get bulk from ObjController */
        if( pObjController )
        {
            returnStatus = pObjController->GetBulkParamValue(pObjController, ppNameArray, ppValueArray, ulParamCount);
        }

        /* Check parameter value and change it to be 2(notReady)  Begining*/        
        if ( ( writeID == DSLH_MPA_ACCESS_CONTROL_SNMP ) && 
             ( pObjEntity->ObjDescr->bWritable == TRUE ) &&
             ( pObjController->bCommitted == FALSE )        )
        {
            for( j = 0; j < ulParamCount; j ++)
            {
                if ( !AnscEqualString( ppNameArray[j], "Enable", TRUE ) )
                    continue;
        
                if ( ppValueArray[j]->Syntax != SLAP_VAR_SYNTAX_bool )
                    continue;
        
                ppValueArray[j]->Variant.varBool = 2;
        
                break;                        
            }
        }
        /* Check parameter value and change it to be 2  Ending*/

        /* copy value back */
        for( i = 0; i < ulParamCount; i ++)
        {
            pParameterValueArray[i].Name  = ppValueArray[i]->Name;
            pParameterValueArray[i].Value = ppValueArray[i];
            ppValueArray[i]->Name         = NULL;
        }    

        ulTotalParamCount = ulParamCount;

        /* clean the arrays */
        if( ppNameArray != NULL)
        {
            AnscFreeMemory(ppNameArray);
            ppNameArray = NULL;
        }

        if( ppValueArray != NULL)
        {
            AnscFreeMemory(ppValueArray);
            ppValueArray = NULL;
        }

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            goto EXIT;
        }
    }

    /* GetValues from Child objects */
    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);
        ulParamCount    = ulMaxParamCount - ulTotalParamCount;
        returnStatus    =
            pChildObjRecord->GetAllParamValues
                (
                    (ANSC_HANDLE)pChildObjRecord,
                    (ANSC_HANDLE)&pParameterValueArray[ulTotalParamCount],
                    &ulParamCount,
                    bFromAcs,
                    writeID
                );

        ulTotalParamCount += ulParamCount;

        /* if there's a failure and the request is from ACS, an error should be returned */
        if( returnStatus != ANSC_STATUS_SUCCESS && bFromAcs)
        {
            goto EXIT;
        }
    }

EXIT:

    *pulArraySize = ulTotalParamCount;

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroGetAllParamInfos
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParamInfoArray,
                PULONG                      pulArraySize,
                BOOL                        bNextLevelOnly,
                BOOL                        bFromAcs
            );

    description:

        This function is called to retrieve the information of all
        parameters under this branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParamInfoArray
                Specifies the output parameter information array where
                the result is to be placed.

                PULONG                      pulArraySize
                Returns the number of parameters returned.

                BOOL                        bNextLevelOnly
                Indicate whether only partial path under this level is
                to be returned.

                BOOL                        bFromAcs
                Whether it's from ACS or not.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroGetAllParamInfos
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamInfoArray,
        PULONG                      pulArraySize,
        BOOL                        bNextLevelOnly,
        BOOL                        bFromACS
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;
    PDSLH_CWMP_PARAM_INFO           pParameterInfoArray = (PDSLH_CWMP_PARAM_INFO  )hParamInfoArray;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord     = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity     = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
	PDSLH_OBJ_ENTITY_OBJECT         pObjEntityChild     = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
	PDSLH_OBJ_ENTITY_OBJECT         pObjEntityParent    = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY     )NULL;
	PDSLH_OBJ_RECORD_OBJECT         pObjRecordParent    = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    ULONG                           i                   = 0;
    ULONG                           ulMaxParamCount     = *pulArraySize;
    ULONG                           ulTotalParamCount   = 0;
    ULONG                           ulParamCount        = 0;


    if ( bNextLevelOnly )
    {
        BOOL                        bThisWritable       = FALSE;
        BOOL                        bThisIsTableObj     = FALSE;

        if( pObjEntity->ObjDescr)
        {
            bThisWritable = pObjEntity->ObjDescr->bWritable;
            if ( pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table )
            {
                PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable     = (PDSLH_OBJCO_TABLE_OBJECT)pMyObject->hDslhObjController;

                bThisIsTableObj = TRUE;

                if( pObjcoTable == NULL)
                {
                    AnscTraceEntry(("???NULL Table ObjController for entity: %s\n", pObjEntity->LastName));
                }
                else if ( pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
                {
                    bThisWritable = FALSE;
                }
            }
        }

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

        while ( pSLinkEntry )
        {
            PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable;
            BOOL                        bWritable;

            pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
            pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pChildObjRecord->hDslhObjEntity;
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if(pChildObjEntity->ObjDescr == NULL)
            {
                AnscTraceEntry(("???NULL ObjDescr for ObjEntity with LastName: %s\n", pObjEntity->LastName));

                continue;
            }

            if ( pChildObjEntity->ObjDescr->bInvisible && bFromACS)
            {
                continue;
            }

            pObjcoTable = (PDSLH_OBJCO_TABLE_OBJECT)pChildObjRecord->hDslhObjController;
            bWritable   = pChildObjEntity->ObjDescr->bWritable;

            if ( bWritable )
            {
                if ( pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table )
                {
                    pObjcoTable     = (PDSLH_OBJCO_TABLE_OBJECT)pChildObjRecord->hDslhObjController;

                    pObjEntityChild = (PDSLH_OBJ_ENTITY_OBJECT)pChildObjEntity->GetObjEntity3((ANSC_HANDLE)pChildObjEntity);

                    if ( pChildObjRecord->GetObjRecordCount((ANSC_HANDLE)pChildObjRecord) >= pObjEntityChild->ObjDescr->MaxInstanceNumber )
					{
						bWritable = FALSE;
					}

                    if( pObjcoTable == NULL)
                    {
                        AnscTraceEntry(("???NULL Table ObjController for entity: %s\n", pChildObjEntity->LastName));
                    }
                    else if ( pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
                    {
                        bWritable = FALSE;
                    }
                }
                else if ( bThisIsTableObj && !bThisWritable )
                {
                    bWritable = FALSE;
                }
            }

            pParameterInfoArray[ulParamCount].Name      = AnscCloneString(pChildObjRecord->FullName);
            pParameterInfoArray[ulParamCount].bWritable = bWritable;
            ulParamCount++;
        }

        ulTotalParamCount += ulParamCount;

        if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
        {
            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

            while ( pSLinkEntry )
            {
                pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
                pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
                pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                if(pChildVarEntity->ParamDescr == NULL)
                {
                    AnscTraceEntry(("???NULL ParamDescr for VarEntity.\n"));

                    continue;
                }

                if ( pChildVarEntity->ParamDescr->bInvisible && bFromACS)
                {
                    continue;
                }

                pParameterInfoArray[ulParamCount].Name      = pChildVarRecord->GetFullName((ANSC_HANDLE)pChildVarRecord);
                pParameterInfoArray[ulParamCount].bWritable = pChildVarRecord->IsRecordWritable(pChildVarRecord);
                ulParamCount++;
            }
        }

        ulTotalParamCount = ulParamCount;
    }
    else
    {
        if( pMyObject->FullName != NULL && AnscSizeOfString(pMyObject->FullName) > 0)
        {
            pParameterInfoArray[ulParamCount].Name      = AnscCloneString(pMyObject->FullName);
            pParameterInfoArray[ulParamCount].bWritable = FALSE;

            if( pObjEntity->ObjDescr && pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
            {
                if( pObjEntity->ObjDescr->bWritable)
                {
                    PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable     = (PDSLH_OBJCO_TABLE_OBJECT)pMyObject->hDslhObjController;
                    ULONG                       ulRecordCount   = pMyObject->GetObjRecordCount((ANSC_HANDLE)pMyObject);
                    BOOL                        bWritable       = TRUE;

					pObjEntityChild = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

					if( pObjEntityChild != NULL && pObjEntityChild->ObjDescr != NULL && ulRecordCount >= pObjEntityChild->ObjDescr->MaxInstanceNumber )
					{
						bWritable = FALSE;
					}

                    if ( pObjcoTable == NULL)
                    {
                        AnscTraceEntry(("???NULL Table ObjController for record: %s\n", pMyObject->FullName));
                    }
                    else if (pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
                    {
						bWritable = FALSE;
                    }

					pParameterInfoArray[ulParamCount].bWritable = bWritable;
                }
			}
			else
			{
				pObjRecordParent = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hParentObjRecord;

				if( pObjRecordParent != NULL)
				{
					pObjEntityParent = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecordParent->hDslhObjEntity;

					if( pObjEntityParent->ObjDescr && pObjEntityParent->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
					{
                        BOOL                        bWritable   = pObjEntityParent->ObjDescr->bWritable;
                        PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable = (PDSLH_OBJCO_TABLE_OBJECT)pObjRecordParent->hDslhObjController;

                        if( pObjcoTable == NULL)
                        {
                            AnscTraceEntry(("???NULL Table ObjController for record: %s\n", pObjRecordParent->FullName));
                        }
                        else if ( bWritable)
                        {
                            if ( pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
                            {
                                bWritable = FALSE;
                            }
                        }

						pParameterInfoArray[ulParamCount].bWritable = bWritable;
					}
				}
			}

			ulParamCount++;
		}

        if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
        {
            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

            while ( pSLinkEntry )
            {
                pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
                pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
                pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                if ( pChildVarEntity->ParamDescr->bInvisible && bFromACS )
                {
                    continue;
                }

                pParameterInfoArray[ulParamCount].Name      = pChildVarRecord->GetFullName((ANSC_HANDLE)pChildVarRecord);
                pParameterInfoArray[ulParamCount].bWritable = pChildVarRecord->IsRecordWritable(pChildVarRecord);
                ulParamCount++;
            }
        }

        ulTotalParamCount = ulParamCount;

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

        while ( pSLinkEntry )
        {
            pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            /* Don't count the invisible object in if it's from ACS .  */
            pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT)pChildObjRecord->hDslhObjEntity;

            if( pObjEntity && pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromACS)
            {
                continue;
            }

            ulParamCount    = ulMaxParamCount - ulTotalParamCount;
            returnStatus    =
                pChildObjRecord->GetAllParamInfos
                    (
                        (ANSC_HANDLE)pChildObjRecord,
                        (ANSC_HANDLE)&pParameterInfoArray[ulTotalParamCount],
                        &ulParamCount,
                        FALSE,
                        bFromACS
                    );

            ulTotalParamCount += ulParamCount;
        }
    }

    *pulArraySize = ulTotalParamCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroGetAllParamAttribs
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParamAttribArray,
                PULONG                      pulArraySize,
                BOOL                        bFromAcs
            );

    description:

        This function is called to retrieve the attribute of all
        parameters under this branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParamAttribArray
                Specifies the output parameter attribute array where
                the result is to be placed.

                PULONG                      pulArraySize
                Returns the number of parameters returned.

                BOOL                        bFromAcs
                Whether it's from ACS or not

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroGetAllParamAttribs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamAttribArray,
        PULONG                      pulArraySize,
        BOOL                        bFromAcs
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject             = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity            = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hDslhObjEntity;
    PDSLH_CWMP_PARAM_ATTRIB         pParameterAttribArray = (PDSLH_CWMP_PARAM_ATTRIB)hParamAttribArray;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord       = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity       = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord       = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry           = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i                     = 0;
    ULONG                           ulMaxParamCount       = *pulArraySize;
    ULONG                           ulTotalParamCount     = 0;
    ULONG                           ulParamCount          = 0;

    /* Don't count the invisible object in. We will quit here */
    if( pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs)
    {
        *pulArraySize = 0;

        return ANSC_STATUS_SUCCESS;
    }

    if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

        while ( pSLinkEntry )
        {
            pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
            pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pChildVarEntity->ParamDescr->bInvisible && bFromAcs)
            {
                continue;
            }

            pParameterAttribArray[ulParamCount].Name         = pChildVarRecord->GetFullName((ANSC_HANDLE)pChildVarRecord);
            pParameterAttribArray[ulParamCount].Notification = pChildVarRecord->Notification;

            if( pChildVarRecord->AccessList == NULL)
            {
                pParameterAttribArray[ulParamCount].AccessList   = AnscCloneString(DSLH_MPA_ENTITY_Subscriber);
            }
            else
            {
                pParameterAttribArray[ulParamCount].AccessList   = AnscCloneString(pChildVarRecord->AccessList);
            }

            ulParamCount++;
        }
    }

    ulTotalParamCount = ulParamCount;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);
        ulParamCount    = ulMaxParamCount - ulTotalParamCount;
        returnStatus    =
            pChildObjRecord->GetAllParamAttribs
                (
                    (ANSC_HANDLE)pChildObjRecord,
                    (ANSC_HANDLE)&pParameterAttribArray[ulTotalParamCount],
                    &ulParamCount,
                    bFromAcs
                );

        ulTotalParamCount += ulParamCount;
    }

    *pulArraySize = ulTotalParamCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroSetAllParamAttribs
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSetParamAttrib
            );

    description:

        This function is called to configure the attribute of all
        parameters under this branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSetParamAttrib
                Specifies the parameter attribute to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroSetAllParamAttribs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSetParamAttrib
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_CWMP_SET_PARAM_ATTRIB     pSetParamAttrib = (PDSLH_CWMP_SET_PARAM_ATTRIB)hSetParamAttrib;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           i               = 0;

    /* Don't count the invisible object in. We will quit here */
    if( pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible)
    {
        return ANSC_STATUS_SUCCESS;
    }

    if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

        while ( pSLinkEntry )
        {
            pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
            pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pChildVarEntity->ParamDescr->bInvisible )
            {
                continue;
            }

            if ( pSetParamAttrib->bNotificationChange )
            {
                returnStatus =
                    pChildVarRecord->SetNotification
                        (
                            (ANSC_HANDLE)pChildVarRecord,
                            pSetParamAttrib->Notification
                        );
            }

            if ( pSetParamAttrib->bAccessListChange )
            {
                returnStatus =
                    pChildVarRecord->SetAccessList
                        (
                            (ANSC_HANDLE)pChildVarRecord,
                            pSetParamAttrib->AccessList
                        );
            }

            pChildVarRecord->SaveConfig((ANSC_HANDLE)pChildVarRecord);
        }
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);
        returnStatus    =
            pChildObjRecord->SetAllParamAttribs
                (
                    (ANSC_HANDLE)pChildObjRecord,
                    (ANSC_HANDLE)pSetParamAttrib
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjroVerifyChanges
            (
                ANSC_HANDLE                 hThisObject,
                char**                      ppFaultParamName
            );

    description:

        This function is called to verify the previous changes made to
        the parameters are acceptable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char**                      ppFaultParamName
                Returns the parameter name causing the problem if an
                error is detected.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhObjroVerifyChanges
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppFaultParamName
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController  = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    char*                           pErrorParamName = (char*                      )NULL;
    BOOL                            bTestResult     = TRUE;

    bTestResult =
        pObjController->Validate
            (
                (ANSC_HANDLE)pObjController,
                pMyObject->hDslhRvqIf,
                ppFaultParamName
            );

    if ( bTestResult )
    {
        return  bTestResult;
    }
    else if ( !*ppFaultParamName )
    {
        return  bTestResult;
    }
    else
    {
        pErrorParamName = (char*)AnscAllocateMemory(AnscSizeOfString(pMyObject->FullName) + AnscSizeOfString(*ppFaultParamName) + 8);
    }

    if ( !pErrorParamName )
    {
        return  bTestResult;
    }
    else
    {
        _ansc_sprintf
            (
                pErrorParamName,
                "%s%s",
                pMyObject->FullName,
                *ppFaultParamName
            );
    }

    AnscFreeMemory(*ppFaultParamName);

    *ppFaultParamName = pErrorParamName;

    return  bTestResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroCommitChanges
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to commit the previous changes made to
        the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroCommitChanges
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject      = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;

    return  pObjController->Commit((ANSC_HANDLE)pObjController, pMyObject->hDslhRvqIf);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroCommitChanges2
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to commit the previous changes made to
        the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroCommitChanges2
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject          = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity         = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController     = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord    = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity    = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY         )NULL;
    char*                           pParamFullName     = (char*                      )NULL;
    ULONG                           i                  = 0;
    BOOL                            bTestResult        = TRUE;
    char*                           pFaultParamName    = NULL;

    bTestResult =
        pObjController->Validate
            (
                (ANSC_HANDLE)pObjController,
                pMyObject->hDslhRvqIf,
                &pFaultParamName
            );

    if ( !bTestResult )
    {
        if( pFaultParamName != NULL)
        {
            AnscTraceEntry(("GUI:Failed to validate parameter '%s'\n", pFaultParamName));

            AnscFreeMemory(pFaultParamName);
        }
        else
        {
            AnscTraceEntry(("GUI:Failed to validate parameter values.\n"));
        }

        return ANSC_STATUS_FAILURE;
    }

    returnStatus = pObjController->Commit((ANSC_HANDLE)pObjController, pMyObject->hDslhRvqIf);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTraceEntry(("Failed in 'DslhObjroCommitChanges2'\n"));

        return  returnStatus;
    }

    /*
     * The difference between this function and the above CommitChanges() is that this call MUST
     * commit all the modified VarRecords as well. Since it's easy to tell whether a VarRecord has
     * been updated or not (i.e. it was modified if 'TempParamValue' is not NULL), we can simply
     * call pVarRecord->CommitChange() without introducing big overhead.
     */
    if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ )*/
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

        while ( pSLinkEntry )
        {
            pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if ( !pChildVarRecord->TempParamValue )
            {
                continue;
            }

            /*
             * ACS can configure the CPE to initiate a session when certain parameters change in
             * value. The following values are defined:
             *
             *      0 - Notification off. The CPE need not inform the ACS of a change to
             *          the specified parameter(s).
             *      1 - Passive notification. Whenever the specified parameter value
             *          changes, the CPE MUST include the new value in the ParameterList
             *          in the Inform message that is sent the next time a session is
             *          established to the ACS.
             *      2 - Active notification. Whenever the specified parameter value
             *          changes, the CPE MUST initiate a session to the ACS, and include
             *          the new value in the ParameterList in the associated Inform
             *          message.
             */
            pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;

            switch ( pChildVarEntity->Notification )
            {
                case    DSLH_CWMP_NOTIFICATION_off :

                        returnStatus = ANSC_STATUS_SUCCESS;

                        break;

                case    DSLH_CWMP_NOTIFICATION_passive :

                        pChildVarRecord->NotifyValueChanged((ANSC_HANDLE)pChildVarRecord);

                        break;

                case    DSLH_CWMP_NOTIFICATION_active :

                        pChildVarRecord->NotifyValueChanged((ANSC_HANDLE)pChildVarRecord);

                        break;

                default :

                        break;
            }

            if( ANSC_STATUS_SUCCESS != pChildVarRecord->CommitChange((ANSC_HANDLE)pChildVarRecord, FALSE))
            {
                AnscTraceEntry(("Failed in 'pChildVarRecord->CommitChange'\n"));
            }

        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroCancelChanges
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the previous changes made to
        the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroCancelChanges
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject      = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;

    return  pObjController->Rollback((ANSC_HANDLE)pObjController);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroAddChildObject
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulInstanceNumber
            );

    description:

        This function is called to add a new child object instance.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulInstanceNumber
                Returns the instance number of the new object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroAddChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController      = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJCO_TABLE_OBJECT        pObjcoTable         = (PDSLH_OBJCO_TABLE_OBJECT   )pObjController;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    char*                           pChildFullName      = (char*                      )NULL;
    char*                           pChildLastName      = (char*                      )NULL;
    ULONG                           ulObjInsNumber      = (ULONG                      )0;
    char                            child_name[16];

    /* the object has to be a table in order to add a child object */
    if( pObjEntity->ObjDescr == NULL || pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pulInstanceNumber  = 0;

    ulObjInsNumber      = pMyObject->NextInstanceNumber++;
    pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjcoTable->NewEntryObject((ANSC_HANDLE)pObjcoTable);

    if ( !pChildObjController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pChildObjController->hDslhCpeController = pMyObject->hDslhCpeController;
        pChildObjController->hParentInsContext  = pObjController->GetInsContext(pObjController);

        returnStatus =
            pObjcoTable->AddEntryObject
                (
                    (ANSC_HANDLE)pObjcoTable,
                    (ANSC_HANDLE)pChildObjController,
                    &ulObjInsNumber
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pMyObject->NextInstanceNumber --;

            goto EXIT1;
        }

        pChildObjController->Engage((ANSC_HANDLE)pChildObjController);
    }

    AnscZeroMemory(child_name, 16);
    _ansc_itoa(ulObjInsNumber, child_name, 10);

    if( ulObjInsNumber == 0)
    {
        AnscTrace("ulObjInsNumber == %d \n", ulObjInsNumber);
    }

    pChildObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)DslhCreateObjRecord
            (
                pMyObject,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pChildObjRecord )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }
    else
    {
        pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);
        pChildLastName  = child_name;
        pChildFullName  = (char*)AnscAllocateMemory(AnscSizeOfString(pMyObject->FullName) + AnscSizeOfString(pChildLastName) + 4);

        if ( !pChildFullName )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            _ansc_sprintf
                (
                    pChildFullName,
                    "%s%s.",
                    pMyObject->FullName,
                    pChildLastName
                );
        }

        pChildObjRecord->hParentObjRecord	  = (ANSC_HANDLE)pMyObject;
        pChildObjRecord->hDslhCpeController	  = (ANSC_HANDLE)pMyObject->hDslhCpeController;
        pChildObjRecord->hDslhObjController   = (ANSC_HANDLE)pChildObjController;
        pChildObjRecord->hIrepFoWmpDatabase   = (ANSC_HANDLE)pMyObject->hIrepFoWmpDatabase;
        pChildObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pChildObjRecord, (ANSC_HANDLE)pChildObjEntity    );
        pChildObjRecord->SetLastName         ((ANSC_HANDLE)pChildObjRecord, pChildLastName                  );
        pChildObjRecord->SetFullName         ((ANSC_HANDLE)pChildObjRecord, pChildFullName                  );

        pChildObjController->hDslhObjRecord = (ANSC_HANDLE)pChildObjRecord;
        pChildObjController->hParamArray    = (ANSC_HANDLE)pChildObjEntity->ObjDescr->hParamArray;
        pChildObjController->bCommitted     = FALSE;

        AnscFreeMemory(pChildFullName);
    }

    returnStatus =
        pMyObject->AddObjRecord
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pChildObjRecord
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to 'AddObjRecord' '%s'\n", pChildObjRecord->FullName);
    }

    returnStatus = pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to 'PopulateObjRecords' '%s'\n", pChildObjRecord->FullName);
    }

    returnStatus = pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to 'PopulateVarRecords' '%s'\n", pChildObjRecord->FullName);
    }

    *pulInstanceNumber = ulObjInsNumber;


    return  returnStatus;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pChildObjRecord )
    {
        pChildObjRecord->Remove((ANSC_HANDLE)pChildObjRecord);
    }

EXIT1:

    if ( pChildObjController )
    {
        pChildObjController->Remove((ANSC_HANDLE)pChildObjController);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroDelChildObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to delete an existing child object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the name of the child object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroDelChildObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController      = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJCO_TABLE_OBJECT        pObjcoTable         = (PDSLH_OBJCO_TABLE_OBJECT   )pObjController;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY         )NULL;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    BOOL                            bChildObjFound      = FALSE;

    /* the object has to be a table in order to delete child object */
    if( pObjEntity->ObjDescr == NULL || pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
    {
        return ANSC_STATUS_FAILURE;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( AnscEqualString
                (
                    pChildObjRecord->LastName,
                    pObjName,
                    TRUE
                ) )
        {
            bChildObjFound = TRUE;

            break;
        }
    }

    if ( !bChildObjFound )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pChildObjRecord->hDslhObjEntity;
        pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)pChildObjRecord->hDslhObjController;
    }

    /* remove all the child objects/tables first */
    pChildObjRecord->Destroy((ANSC_HANDLE)pChildObjRecord);

    returnStatus =
        pObjcoTable->DelEntryObject
            (
                (ANSC_HANDLE)pObjcoTable,
                (ANSC_HANDLE)pChildObjController
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {

        AnscTrace("Failed to 'DelEntryObject': %s\n", pChildObjRecord->FullName);

        return returnStatus;
    }


    /* popup it from the queue */
    AnscQueuePopEntryByLink(&pMyObject->ObjroQueue, &pChildObjRecord->Linkage);

    pChildObjRecord->Reset  ((ANSC_HANDLE)pChildObjRecord);
    pChildObjRecord->Remove ((ANSC_HANDLE)pChildObjRecord);

    return  ANSC_STATUS_SUCCESS;
}
