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

    module:	dslh_objro_control.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Dslh Obj Record Object.

        *   DslhObjroPopulateObjRecords
        *   DslhObjroPopulateVarRecords
        *   DslhObjroParamValueChanged
        *   DslhObjroChildObjectCreated
        *   DslhObjroChildObjectRemoved
        *   DslhObjroPopulateObjRecordByName
        *   DslhObjroDeleteObjRecordByName

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
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#include "dslh_objro_global.h"
#include "dslh_ifo_tr69.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroPopulateObjRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to populate obj records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroPopulateObjRecords
    (
        ANSC_HANDLE                 hThisObject
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
    PFN_DSLHOBJCO_CREATE            pfnObjcoCreate      = (PFN_DSLHOBJCO_CREATE       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY         )NULL;
    char*                           pChildFullName      = (char*                      )NULL;
    char*                           pChildLastName      = (char*                      )NULL;
    ULONG                           ulFullNameSize      = (ULONG                      )0;
    ULONG                           ulObjectCount       = (ULONG                      )0;
    ULONG                           ulObjInsNumber      = (ULONG                      )0;
    ULONG                           i                   = 0;
    char                            child_name[16]      = { 0 };
    PDSLH_TR69_INTERFACE            pDslhTr69If         = {0};
    BOOL                            bCommitted          = TRUE;
    errno_t                         rc                  = -1;

    /*
     * Under each Obj Record folder, there's always a folder named "Parameters", which is where all
     * parameter values are stored. If this Obj Record has children, each child must have its own
     * folder, whose name is the same as the instance number (i.e. the "LastName' of the child Obj
     * Record. Note that all record folders are sorted.
     */

    /* AnscTrace("DslhObjroPopulateObjRecords - object '%s' is %s.\n", pObjEntity->LastName, pMyObject->FullName); */

    if ( pObjEntity->ObjDescr && (pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table) )
    {
        if ( !pObjcoTable )
        {
            return  ANSC_STATUS_INTERNAL_ERROR;
        }

        /*
         * If this obj record is a dynamic table object, we need to dynamically populate the
         * child objects underneath it. Note there's no need to check whether the table content
         * has been updated since there's no child entry object.
         */
        if ( pObjEntity->ObjDescr->bDynamic )
        {
            pObjcoTable->Synchronize((ANSC_HANDLE)pObjcoTable);
        }

        /*
         * The Table is not configurable means that all the entry objects were pre-created. We
         * need to enumerate these entries from the table and configure each one of them accord-
         * ingly.
         */
        ulObjectCount = pObjcoTable->GetEntryObjectCount((ANSC_HANDLE)pObjcoTable);

        for ( i = 0; i < ulObjectCount; i++ )
        {
            pChildObjController =
                (PDSLH_OBJ_CONTROLLER_OBJECT)pObjcoTable->GetEntryObject
                    (
                        (ANSC_HANDLE)pObjcoTable,
                        i,
                        &ulObjInsNumber
                    );

            if ( !pChildObjController )
            {
                break;
            }
            else
            {
                pChildObjController->hDslhCpeController = pMyObject->hDslhCpeController;
                pChildObjController->hParentInsContext  = pObjController->GetInsContext(pObjController);
                pChildObjController->Engage              ((ANSC_HANDLE)pChildObjController);
            }

            AnscZeroMemory(child_name, 16);
            _ansc_itoa(ulObjInsNumber, child_name, 10);

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

                goto  EXIT2;
            }
            else
            {
                pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

                pChildLastName  = child_name;
                if ( !pChildLastName ) /*RDKB-5792, CID-24199, adding NULL check*/
                {
                    returnStatus = ANSC_STATUS_RESOURCES;
                    goto  EXIT3;
                }
                ulFullNameSize  = (pMyObject->FullName? AnscSizeOfString(pMyObject->FullName) : 0) + AnscSizeOfString(pChildLastName) + 4;

                pChildFullName  = (char*)AnscAllocateMemory(ulFullNameSize);
                if ( !pChildFullName ) /*RDKB-5792, CID-24199, adding NULL check*/
                {
                    returnStatus = ANSC_STATUS_RESOURCES;
                    goto  EXIT3;
                }
                else if ( !pMyObject->FullName || (AnscSizeOfString(pMyObject->FullName) == 0) )
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s.",
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }
                else
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s%s.",
                            pMyObject->FullName,
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }

                pChildObjRecord->hParentObjRecord    = (ANSC_HANDLE)pMyObject;
                pChildObjRecord->hDslhCpeController  = (ANSC_HANDLE)pMyObject->hDslhCpeController;
                pChildObjRecord->hDslhObjController  = (ANSC_HANDLE)pChildObjController;
                pChildObjRecord->hIrepFoWmpDatabase  = (ANSC_HANDLE)pMyObject->hIrepFoWmpDatabase;
                pChildObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pChildObjRecord, (ANSC_HANDLE)pChildObjEntity    );
                pChildObjRecord->SetLastName         ((ANSC_HANDLE)pChildObjRecord, pChildLastName                  );
                pChildObjRecord->SetFullName         ((ANSC_HANDLE)pChildObjRecord, pChildFullName                  );

                pChildObjController->hDslhObjRecord  = (ANSC_HANDLE)pChildObjRecord;
                pChildObjController->hParamArray     = (ANSC_HANDLE)pChildObjEntity->ObjDescr->hParamArray;

                /* Get bcommitted now. This means this Entry has already been committed to backend. */
                pDslhTr69If                          = (PDSLH_TR69_INTERFACE)(pChildObjController->hDslhTr69If);
                bCommitted                           = TRUE;
                if( pDslhTr69If->bUseFullName)
                {
                    if( pDslhTr69If->Methods.MethodWithFullName.GetEntryStatus != NULL)
                    {
                        bCommitted =  pDslhTr69If->Methods.MethodWithFullName.GetEntryStatus(pChildObjController->hInsContext, pChildObjRecord->FullName, "Committed");
                    }
                }
                else
                {
                    if( pDslhTr69If->Methods.MethodWithLastName.GetEntryStatus != NULL)
                    {
                        bCommitted =  pDslhTr69If->Methods.MethodWithLastName.GetEntryStatus(pChildObjController->hInsContext, "Committed");
                    }
                }
                pChildObjController->bCommitted = bCommitted;

                AnscFreeMemory(pChildFullName);
                pChildFullName = NULL;
            }

            if( pChildObjController->CheckInstance(pChildObjController))
            {
                returnStatus =
                    pMyObject->AddObjRecord
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pChildObjRecord
                        );

                returnStatus = pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);
            }
            else
            {
                AnscTrace("The instance '%s' shouldn't be created.\n", pChildObjRecord->FullName);

                pChildObjRecord->Remove(pChildObjRecord);
            }
        }
    }
    else
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->ObjeoQueue);

        while ( pSLinkEntry )
        {
            pChildObjEntity  = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);
            pfnObjcoCreate   = (PFN_DSLHOBJCO_CREATE)pChildObjEntity->ObjDescr->pfnObjcoConstructor;

            if( pfnObjcoCreate == NULL)
            {
                pfnObjcoCreate = DslhCreateObjController;
            }

            pChildObjController =
                (PDSLH_OBJ_CONTROLLER_OBJECT)pfnObjcoCreate
                    (
                        pMyObject,
                        (ANSC_HANDLE)pObjController,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pChildObjController )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT1;
            }
            else
            {
                pChildObjController->hDslhCpeController  = pMyObject->hDslhCpeController;
                pChildObjController->hParentInsContext   = pObjController? pObjController->GetInsContext(pObjController) : (ANSC_HANDLE)NULL;
                pChildObjController->hParamArray         = pChildObjEntity->ObjDescr->hParamArray;
                pChildObjController->hDslhTr69If         = pChildObjEntity->ObjDescr->hDslhTr69If;
                pChildObjController->hInsContext         = (pChildObjEntity->ObjDescr->hContextToController) ? pChildObjEntity->ObjDescr->hContextToController : NULL;
                pChildObjController->Engage              ((ANSC_HANDLE)pChildObjController);
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

                goto  EXIT2;
            }
            else
            {
                pChildLastName  = pChildObjEntity->LastName;
                ulFullNameSize  = (pMyObject->FullName? AnscSizeOfString(pMyObject->FullName) : 0) + AnscSizeOfString(pChildLastName) + 4;
                pChildFullName  = (char*)AnscAllocateMemory(ulFullNameSize);

                if ( !pChildFullName )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT3;
                }
                else if ( !pMyObject->FullName || (AnscSizeOfString(pMyObject->FullName) == 0) )
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s.",
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }
                else
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s%s.",
                            pMyObject->FullName,
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }

                pChildObjRecord->hParentObjRecord    = (ANSC_HANDLE)pMyObject;
                pChildObjRecord->hDslhCpeController  = pMyObject->hDslhCpeController;
                pChildObjRecord->hDslhObjController  = (ANSC_HANDLE)pChildObjController;
                pChildObjRecord->hIrepFoWmpDatabase  = (ANSC_HANDLE)pMyObject->hIrepFoWmpDatabase;
                pChildObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pChildObjRecord, (ANSC_HANDLE)pChildObjEntity    );
                pChildObjRecord->SetLastName         ((ANSC_HANDLE)pChildObjRecord, pChildLastName                  );
                pChildObjRecord->SetFullName         ((ANSC_HANDLE)pChildObjRecord, pChildFullName                  );

                pChildObjController->hDslhObjRecord  = (ANSC_HANDLE)pChildObjRecord;

                AnscFreeMemory(pChildFullName);
                pChildFullName = NULL;
            }

            if( pChildObjController->CheckInstance(pChildObjController))
            {
                returnStatus =
                    pMyObject->AddObjRecord
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pChildObjRecord
                        );

                returnStatus = pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);

                if( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    AnscTrace("Failed to populate record for '%s'\n", pChildObjRecord->FullName);
                }
            }
            else
            {
                AnscTrace("The instance '%s' shouldn't be created.\n", pChildObjRecord->FullName);

                pChildObjRecord->Remove(pChildObjRecord);
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/



EXIT3:

    if ( pChildObjRecord )
    {
        pChildObjRecord->Remove((ANSC_HANDLE)pChildObjRecord);
    }

EXIT2:

    if ( pChildObjController )
    {
        pChildObjController->Remove((ANSC_HANDLE)pChildObjController);
    }

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroPopulateVarRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to populate var records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroPopulateVarRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject            = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity           = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord      = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity      = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord      = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry          = (PSINGLE_LINK_ENTRY         )NULL;


    /*
     * We first try to populate the var records under each child obj record...
     */
    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);
    }

    if ( TRUE ) /* for ( i = 0; i < DSLH_OBJEO_VAREO_TABLE_SIZE; i++ )*/
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->VareoTable);

        while ( pSLinkEntry )
        {
            pChildVarEntity      = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

            pChildVarRecord =
                (PDSLH_VAR_RECORD_OBJECT)DslhCreateVarRecord
                    (
                        pMyObject,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pChildVarRecord )
            {
                return ANSC_STATUS_RESOURCES;
            }
            else
            {
                pChildVarRecord->hDslhObjRecord			= (ANSC_HANDLE)pMyObject;
                pChildVarRecord->hDslhVarEntity         = (ANSC_HANDLE)pChildVarEntity;
                pChildVarRecord->hIrepFoParameter       = (ANSC_HANDLE)NULL;
                pChildVarRecord->Notification           = pChildVarEntity->Notification;
            }

            if( pObjController->CheckParamExistence(pObjController, pChildVarEntity->ParamDescr->Name))
            {
                returnStatus =
                    pMyObject->AddVarRecord
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pChildVarRecord
                        );

                if( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    AnscTrace("Failed to 'AddVarRecord' '%s'\n", pChildVarEntity->ParamDescr->Name);
                }

                pChildVarRecord->LoadConfig((ANSC_HANDLE)pChildVarRecord);
            }
            else
            {
                AnscTrace("The parameter '%s' shouldn't be created.\n", pChildVarEntity->ParamDescr->Name);

                pChildVarRecord->Remove(pChildVarRecord);
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroParamValueChanged
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the associated ObjController when
        the value of a parameter is changed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter whose value has
                just been modified.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroParamValueChanged
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject          = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord    = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity    = (PDSLH_VAR_ENTITY_OBJECT    )NULL;

    /*
     * To avoid dead lock, this function MUST NOT be called directly within any callbacks provided
     * by the ObjController.
     */
    AnscAcquireTsLock(&pDslhWmpDatabase->AccessTsLock);

    pChildVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pMyObject->GetVarRecord
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pChildVarRecord )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto  EXIT1;
    }
    else
    {
        pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
    }

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

    /*
     * If this parameter is persistent, we must save the current value into the system registry.
     * For example: the "InternetGateway.DeviceInfo.FirstUseDate" parameter will be updated when
     * CPE successfully establishes a network connection for the first time. The updated value
     * should be saved into the registry so it can be accessed later.
     */
    pChildVarRecord->SynValue((ANSC_HANDLE)pChildVarRecord);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pDslhWmpDatabase->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroChildObjectCreated
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildObjController,
                ULONG                       ulInstanceNumber
            );

    description:

        This function is called by the associated ObjController when
        a new child object is created.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildObjController
                Specifies the newly created child obj controller.

                ULONG                       ulInstanceNumber
                Specifies the instance number of the new child object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroChildObjectCreated
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildObjController,
        ULONG                       ulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)hChildObjController;
    char*                           pChildFullName      = (char*                      )NULL;
    char*                           pChildLastName      = (char*                      )NULL;
    ULONG                           ulFullNameSize      = (ULONG                      )0;
    char                            child_name[16];
    errno_t                         rc                  = -1;

    /*
     * To avoid dead lock, this function MUST NOT be called directly within any callbacks provided
     * by the ObjController.
     */
    AnscAcquireTsLock(&pDslhWmpDatabase->AccessTsLock);

    AnscZeroMemory(child_name, 16);
    _ansc_itoa(ulInstanceNumber, child_name, 10);

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
        ulFullNameSize  = (pMyObject->FullName? AnscSizeOfString(pMyObject->FullName) : 0) + AnscSizeOfString(pChildLastName) + 4;
        pChildFullName  = (char*)AnscAllocateMemory(ulFullNameSize);

        if ( !pChildFullName )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            rc = sprintf_s
                (
                    pChildFullName,
                    ulFullNameSize,
                    "%s%s.",
                    pMyObject->FullName,
                    pChildLastName
                );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
        }

        pChildObjRecord->hParentObjRecord    = (ANSC_HANDLE)pMyObject;
        pChildObjRecord->hDslhCpeController  = pMyObject->hDslhCpeController;
        pChildObjRecord->hDslhObjController  = (ANSC_HANDLE)pChildObjController;
        pChildObjRecord->hIrepFoWmpDatabase   = (ANSC_HANDLE)pMyObject->hIrepFoWmpDatabase;
        pChildObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pChildObjRecord, (ANSC_HANDLE)pChildObjEntity    );
        pChildObjRecord->SetLastName         ((ANSC_HANDLE)pChildObjRecord, pChildLastName                  );
        pChildObjRecord->SetFullName         ((ANSC_HANDLE)pChildObjRecord, pChildFullName                  );

        pChildObjController->hDslhObjRecord = (ANSC_HANDLE)pChildObjRecord;

        AnscFreeMemory(pChildFullName);
        pChildFullName = NULL;
    }

    returnStatus =
        pMyObject->AddObjRecord
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pChildObjRecord
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to AddObjRecord '%s' in 'ChildObjectCreated'.\n", pChildObjRecord->FullName);
    }

    returnStatus = pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to PopulateObjRecords '%s' in 'ChildObjectCreated'.\n", pChildObjRecord->FullName);
    }

    returnStatus = pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to PopulateVarRecords '%s' in 'ChildObjectCreated'.\n", pChildObjRecord->FullName);
    }


    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pChildObjRecord )
    {
        pChildObjRecord->Remove((ANSC_HANDLE)pChildObjRecord);
    }

EXIT1:

    AnscReleaseTsLock(&pDslhWmpDatabase->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroChildObjectRemoved
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulInstanceNumber
            );

    description:

        This function is called by the associated ObjController when
        an existing child object is removed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulInstanceNumber
                Specifies the instance number of the child object to
                be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjroChildObjectRemoved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY         )NULL;
    BOOL                            bChildObjFound      = FALSE;
    char                            child_name[16];

    /*
     * To avoid dead lock, this function MUST NOT be called directly within any callbacks provided
     * by the ObjController.
     */
    AnscAcquireTsLock(&pDslhWmpDatabase->AccessTsLock);

    if ( ulInstanceNumber == 0 )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ObjroQueue);

        while ( pSLinkEntry )
        {
            pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ObjroQueue);

            pChildObjRecord->Destroy((ANSC_HANDLE)pChildObjRecord);
            pChildObjRecord->Reset  ((ANSC_HANDLE)pChildObjRecord);
            pChildObjRecord->Remove ((ANSC_HANDLE)pChildObjRecord);
        }
    }
    else
    {
        AnscZeroMemory(child_name, 16);
        _ansc_itoa(ulInstanceNumber, child_name, 10);

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

        while ( pSLinkEntry )
        {
            pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if ( strcmp(pChildObjRecord->LastName,child_name) == 0 )
            {
                bChildObjFound = TRUE;

                AnscQueuePopEntryByLink(&pMyObject->ObjroQueue, &pChildObjRecord->Linkage);

                pChildObjRecord->Destroy((ANSC_HANDLE)pChildObjRecord);
                pChildObjRecord->Reset  ((ANSC_HANDLE)pChildObjRecord);
                pChildObjRecord->Remove ((ANSC_HANDLE)pChildObjRecord);

                break;
            }
        }

        if ( !bChildObjFound )
        {
            returnStatus = ANSC_STATUS_BAD_INDEX;
        }
        else
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pDslhWmpDatabase->AccessTsLock);

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroPopulateObjRecordByName
        (
            ANSC_HANDLE                 hThisObject,
            char*                       pObjName
        );

    description:

        This function is called to populate certain ObjRecord specified by name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                The obj record specified by name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhObjroPopulateObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_RECORD_OBJECT         pMyObject           = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController      = (PDSLH_OBJ_CONTROLLER_OBJECT)pMyObject->hDslhObjController;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pChildObjController = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord     = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity     = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PFN_DSLHOBJCO_CREATE            pfnObjcoCreate      = (PFN_DSLHOBJCO_CREATE       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY         )NULL;
    char                            pParamName[256]     = { 0 };
    BOOL                            bIsTable            = FALSE;
    BOOL                            bExist              = FALSE;
    char*                           pChildFullName      = (char*                      )NULL;
    char*                           pChildLastName      = (char*                      )NULL;
    ULONG                           ulFullNameSize      = (ULONG                      )0;
    errno_t                         rc                  = -1;

    /* delete previous one if exists */
    /* pMyObject->DeleteObjRecordByName(pMyObject, pObjName); */

    /* populate and create a new one.*/
    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->ObjeoQueue);

    while ( pSLinkEntry )
    {
        pChildObjEntity  = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if( strcmp(pChildObjEntity->LastName, pObjName) == 0 )
        {
            pfnObjcoCreate   = (PFN_DSLHOBJCO_CREATE)pChildObjEntity->ObjDescr->pfnObjcoConstructor;

            if( pfnObjcoCreate == NULL)
            {
                pfnObjcoCreate = DslhCreateObjController;
            }

            pChildObjController =
                (PDSLH_OBJ_CONTROLLER_OBJECT)pfnObjcoCreate
                    (
                        pMyObject,
                        (ANSC_HANDLE)pObjController,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pChildObjController )
            {
                return ANSC_STATUS_RESOURCES;

                goto  EXIT1;
            }
            else
            {
                pChildObjController->hDslhCpeController  = pMyObject->hDslhCpeController;
                pChildObjController->hParentInsContext   = pObjController? pObjController->GetInsContext(pObjController) : (ANSC_HANDLE)NULL;
                pChildObjController->hParamArray         = pChildObjEntity->ObjDescr->hParamArray;
                pChildObjController->hDslhTr69If         = pChildObjEntity->ObjDescr->hDslhTr69If;
                pChildObjController->Engage              ((ANSC_HANDLE)pChildObjController);
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

                goto  EXIT2;
            }
            else
            {
                pChildLastName  = pChildObjEntity->LastName;
                ulFullNameSize  = (pMyObject->FullName? AnscSizeOfString(pMyObject->FullName) : 0) + AnscSizeOfString(pChildLastName) + 4;
                pChildFullName  = (char*)AnscAllocateMemory(ulFullNameSize);

                if ( !pChildFullName )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT3;
                }
                else if ( !pMyObject->FullName || (AnscSizeOfString(pMyObject->FullName) == 0) )
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s.",
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }
                else
                {
                    rc = sprintf_s
                        (
                            pChildFullName,
                            ulFullNameSize,
                            "%s%s.",
                            pMyObject->FullName,
                            pChildLastName
                        );
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                }

                pChildObjRecord->hParentObjRecord    = (ANSC_HANDLE)pMyObject;
                pChildObjRecord->hDslhCpeController  = pMyObject->hDslhCpeController;
                pChildObjRecord->hDslhObjController  = (ANSC_HANDLE)pChildObjController;
                pChildObjRecord->hIrepFoWmpDatabase  = (ANSC_HANDLE)pMyObject->hIrepFoWmpDatabase;
                pChildObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pChildObjRecord, (ANSC_HANDLE)pChildObjEntity    );
                pChildObjRecord->SetLastName         ((ANSC_HANDLE)pChildObjRecord, pChildLastName                  );
                pChildObjRecord->SetFullName         ((ANSC_HANDLE)pChildObjRecord, pChildFullName                  );

                pChildObjController->hDslhObjRecord  = (ANSC_HANDLE)pChildObjRecord;

                AnscFreeMemory(pChildFullName);
                pChildFullName = NULL;
            }

            if( pChildObjController->CheckInstance(pChildObjController))
            {
                returnStatus =
                    pMyObject->AddObjRecord
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pChildObjRecord
                        );

                pChildObjRecord->PopulateObjRecords((ANSC_HANDLE)pChildObjRecord);
                pChildObjRecord->PopulateVarRecords((ANSC_HANDLE)pChildObjRecord);

                if( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    AnscTrace("Failed to populate record for '%s'\n", pChildObjRecord->FullName);
                }
            }
            else
            {
                AnscTrace("The instance '%s' shouldn't be created.\n", pChildObjRecord->FullName);

                pChildObjRecord->Remove(pChildObjRecord);
            }

            if( pChildObjEntity->ObjDescr != NULL && pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
            {
                bIsTable = TRUE;
            }

            break;
        }
    }

    /* if it's a table object, add the VarRecord for the count of the table */
    if( bIsTable )
    {
        rc = sprintf_s(pParamName, sizeof(pParamName), "%s%s", pObjName, TR69_NUMBER_OF_ENTRIES_STRING);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        /* check whether it exists or not */

        bExist      = FALSE;
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

        while ( pSLinkEntry )
        {
            pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if ( strcmp(pChildVarRecord->GetLastName((ANSC_HANDLE)pChildVarRecord),pParamName) == 0 )
            {
                bExist = TRUE;

                break;
            }
        }

        /* if not exists, add it */
        if( !bExist)
        {
            pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->VareoTable);

            while ( pSLinkEntry )
            {
                pChildVarEntity      = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
                pSLinkEntry          = AnscQueueGetNextEntry(pSLinkEntry);

                if( pChildVarEntity->ParamDescr &&
                    strcmp(pChildVarEntity->ParamDescr->Name, pParamName) == 0 )
                {
                    pChildVarRecord =
                        (PDSLH_VAR_RECORD_OBJECT)DslhCreateVarRecord
                            (
                                pMyObject,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( pChildVarRecord )
                    {
                        pChildVarRecord->hDslhObjRecord			= (ANSC_HANDLE)pMyObject;
                        pChildVarRecord->hDslhVarEntity         = (ANSC_HANDLE)pChildVarEntity;
                        pChildVarRecord->hIrepFoParameter       = (ANSC_HANDLE)NULL;
                        pChildVarRecord->Notification           = pChildVarEntity->Notification;

                        pMyObject->AddVarRecord
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pChildVarRecord
                            );
                    }

                    break;
                }
            }
        }
    }

    goto EXIT1;

EXIT3:

    if ( pChildObjRecord )
    {
        pChildObjRecord->Remove((ANSC_HANDLE)pChildObjRecord);
    }

EXIT2:

    if ( pChildObjController )
    {
        pChildObjController->Remove((ANSC_HANDLE)pChildObjController);
    }

EXIT1:

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroDeleteObjRecordByName
        (
            ANSC_HANDLE                 hThisObject,
            char*                       pObjName
        );

    description:

        This function is called to delete populated object Record specified by name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                The obj record specified by name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhObjroDeleteObjRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    char                            pParamName[128] = { 0 };
    errno_t                         rc              = -1;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pChildObjRecord->LastName && strcmp(pChildObjRecord->LastName,pObjName) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->ObjroQueue, &pChildObjRecord->Linkage);

            pChildObjRecord->Remove(pChildObjRecord);

            break;
        }
    }

    /* if it's a table object, remove the VarRecord for the count of the table */

    rc = sprintf_s(pParamName, sizeof(pParamName), "%s%s", pObjName, TR69_NUMBER_OF_ENTRIES_STRING);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->VarroTable);

    while ( pSLinkEntry )
    {
        pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pChildVarRecord->GetLastName((ANSC_HANDLE)pChildVarRecord),pParamName) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->VarroTable, &pChildVarRecord->Linkage);

            pChildVarRecord->Remove ((ANSC_HANDLE)pChildVarRecord);

            break;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
