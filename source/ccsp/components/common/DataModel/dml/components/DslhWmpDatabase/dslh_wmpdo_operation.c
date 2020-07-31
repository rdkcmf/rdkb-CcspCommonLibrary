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

    module:	dslh_wmpdo_operation.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoEngage
        *   DslhWmpdoCancel
        *   DslhWmpdoSetupEnv
        *   DslhWmpdoCloseEnv
        *   DslhWmpdoMonitorTimerInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/19/05    initial revision.

**********************************************************************/


#include "dslh_wmpdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT  )pMyObject->hDslhCpeController;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord     = (PDSLH_OBJ_RECORD_OBJECT      )pMyObject->hRootObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT      )pMyObject->hRootObjEntity;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pDslhCpeController )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( !pRootObjEntity )
    {
        pRootObjEntity =
            (PDSLH_OBJ_ENTITY_OBJECT)DslhCreateObjEntity
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if  ( !pRootObjEntity )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRootObjEntity = (ANSC_HANDLE)pRootObjEntity;
        }
    }

    if ( !pRootObjRecord )
    {
        pRootObjRecord =
            (PDSLH_OBJ_RECORD_OBJECT)DslhCreateObjRecord
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if  ( !pRootObjRecord )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRootObjRecord = (ANSC_HANDLE)pRootObjRecord;
        }

        pRootObjRecord->hParentObjRecord    = NULL;
        pRootObjRecord->hDslhCpeController  = (ANSC_HANDLE)pDslhCpeController;
        pRootObjRecord->hDslhObjController  = NULL;
        pRootObjRecord->SetDslhObjEntity    ((ANSC_HANDLE)pRootObjRecord, (ANSC_HANDLE)pRootObjEntity    );
    }

    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    pMyObject->bActive = TRUE;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord     = (PDSLH_OBJ_RECORD_OBJECT      )pMyObject->hRootObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT      )pMyObject->hRootObjEntity;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMonitorTimerObj   = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hMonitorTimerObj;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pMonitorTimerObj->Stop((ANSC_HANDLE)pMonitorTimerObj);

    if ( pRootObjRecord )
    {
        pRootObjRecord->Remove((ANSC_HANDLE)pRootObjRecord);

        pMyObject->hRootObjRecord = (ANSC_HANDLE)NULL;
    }

    if ( pRootObjEntity )
    {
        pRootObjEntity->Remove((ANSC_HANDLE)pRootObjEntity);

        pMyObject->hRootObjEntity = (ANSC_HANDLE)NULL;
    }

    returnStatus = pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    /*PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController     = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;*/
    /*
    PSLAP_POA_INTERFACE             pSlapPoaIf             = (PSLAP_POA_INTERFACE        )pDslhCpeController->hSlapPoaIf;
    PSLAP_GOA_INTERFACE             pSlapGoaIf             = (PSLAP_GOA_INTERFACE        )pDslhCpeController->hSlapGoaIf;
    PPOAM_IREP_DRIVER_OBJECT        pPoamIrepDriver        = (PPOAM_IREP_DRIVER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRoot        = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoProvision   = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoWmpDatabase = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoGoodOldValue= (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    */

    /*PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor         = (PSLAP_OBJECT_DESCRIPTOR    )NULL;*/

#if 0
    /*
     * Enroll and create SlapDslhParamTree object.
     */
    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapDslhParamtoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        pMyObject->hSlapDslhParamTree =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pDslhCpeController->hSlapDefContainer,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );
    }

    /*
     * Enroll and create PoamIrepDriver object.
     */
    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)PoamIrepDoGetPoamObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapPoaIf->EnrollProxyObject
                (
                    pSlapPoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        pPoamIrepDriver = (PPOAM_IREP_DRIVER_OBJECT)
            pSlapPoaIf->AcqObjectAccess
                (
                    pSlapPoaIf->hOwnerContext,
                    pObjDescriptor->ObjName
                );
    }

    /*
     * Enroll PoamIrepFolder object.
     */
    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)PoamIrepFoGetPoamObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapPoaIf->EnrollProxyObject
                (
                    pSlapPoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );
    }

    if( TRUE )
    {
        if ( !pPoamIrepDriver )
        {
            AnscTrace("DslhWmpDatabase: Cannot access System Registry!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        pPoamIrepFoRoot =
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepDriver->GetRootFolder
                (
                    (ANSC_HANDLE)pPoamIrepDriver
                );

        if ( !pPoamIrepFoRoot )
        {
            AnscTrace("DslhWmpDatabase: Cannot access Root Folder!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }

        pPoamIrepFoProvision =
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRoot->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoRoot,
                    "/Configuration/Provision"
                );

        if ( !pPoamIrepFoProvision )
        {
            AnscTrace("DslhWmpDatabase: Cannot access /Configuration/Provision folder!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT3;
        }

        pPoamIrepFoWmpDatabase =
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoProvision->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoProvision,
                    DSLH_WMPDO_L1_NAME
                );

        if ( !pPoamIrepFoWmpDatabase )
        {
            pPoamIrepFoWmpDatabase =
                pPoamIrepFoProvision->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoProvision,
                        DSLH_WMPDO_L1_NAME,
                        0
                    );
        }

        if ( !pPoamIrepFoWmpDatabase )
        {
            AnscTrace("DslhWmpDatabase: Cannot access /Configuration/Provision/WmpDatabase folder!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT4;
        }
        else
        {
            pMyObject->hPoamIrepFoWmpDatabase = (ANSC_HANDLE)pPoamIrepFoWmpDatabase;
            /*
             * All record folders should be sorted by name ...
             */
            returnStatus =
                pPoamIrepFoWmpDatabase->SortFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoWmpDatabase,
                        SYS_RFO_SORTING_TYPE_BYNAME,
                        TRUE
                    );
        }
    }
#endif
    returnStatus = ANSC_STATUS_SUCCESS;

#if 0
    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT4:

    if ( pPoamIrepFoProvision )
    {
        pPoamIrepFoProvision->Remove((ANSC_HANDLE)pPoamIrepFoProvision);
    }

EXIT3:

    if ( pPoamIrepFoRoot )
    {
        pPoamIrepFoRoot->Remove((ANSC_HANDLE)pPoamIrepFoRoot);
    }

EXIT2:

    if ( pPoamIrepDriver )
    {
        pPoamIrepDriver->Remove((ANSC_HANDLE)pPoamIrepDriver);
    }

EXIT1:
#endif
    return  returnStatus;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    /*PDSLH_WMP_DATABASE_OBJECT       pMyObject              = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;*/
    /*
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoWmpDatabase = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hPoamIrepFoWmpDatabase;

    if ( pPoamIrepFoWmpDatabase )
    {
        pPoamIrepFoWmpDatabase->Remove((ANSC_HANDLE)pPoamIrepFoWmpDatabase);

        pMyObject->hPoamIrepFoWmpDatabase = (ANSC_HANDLE)NULL;
    }
    */

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMonitorTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to periodically poll the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMonitorTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam      = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PDSLH_VAR_RECORD_OBJECT         pDslhVarRecord     = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pDslhVarEntity     = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_CWMP_PARAM_DESCR          pParamDesp         = (PDSLH_CWMP_PARAM_DESCR     )NULL;
    PSLAP_VARIABLE                  pNewParamValue     = (PSLAP_VARIABLE             )NULL;
    BOOL                            bValueNotUpdated   = (BOOL                       )TRUE;
    char*                           pParamFullName     = (char*                      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY         )NULL;
    BOOL                            bActiveNotify      = FALSE;
    int                             oldValue           = 0; /* Only for notification with threshold */
    int                             newValue           = 0; /* Only for notification with threshold */

    AnscAcquireLock(&pMyObject->MpoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MpoQueue);

    while ( pSLinkEntry )
    {
        pMonitorParam  = ACCESS_DSLH_WMPDO_MONITOR_PARAM(pSLinkEntry);
        pDslhVarRecord = (PDSLH_VAR_RECORD_OBJECT)pMonitorParam->hVarRecord;
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);

        /*
         * DO NOT synchronize the value if value-changing is in progress...
         */
        if ( pDslhVarRecord->TempParamValue )
        {
            continue;
        }

        /*
         * DO NOT synchronize the value if the active notification parameter is rate-regulated and
         * the last update was notified less than "NotificationLimit" seconds ago.
         */
        if ( (pDslhVarRecord->Notification      == DSLH_CWMP_NOTIFICATION_active) &&
             (pMonitorParam ->NotificationLimit >  0                            ) )
        {
            if ( (AnscGetTickInSeconds() - pMonitorParam->Timestamp) < pMonitorParam->NotificationLimit )
            {
                continue;
            }
        }

        if ( pMonitorParam->PreviousValue )
        {
            pNewParamValue   = pDslhVarRecord->GetValue((ANSC_HANDLE)pDslhVarRecord);
            bValueNotUpdated = TRUE;

            if ( !pNewParamValue )
            {
                continue;
            }
            else
            {
            	SlapEqualVariables(pMonitorParam->PreviousValue, pNewParamValue, bValueNotUpdated);
            }

            if ( !bValueNotUpdated )
            {
                /* Notificatoin with threshold check */
                pDslhVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pDslhVarRecord->hDslhVarEntity;
                pParamDesp     = (PDSLH_CWMP_PARAM_DESCR )pDslhVarEntity->ParamDescr;
                
                if ( pParamDesp && pParamDesp->bThresholdEnabled == TRUE )
                {
                    oldValue  = pMonitorParam ->PreviousValue->Variant.varInt;
                    newValue  = pNewParamValue->Variant.varInt;
                    
                    if ( 
                           (oldValue < pParamDesp->NotifyThresholdMin && newValue < pParamDesp->NotifyThresholdMin) ||
                           (oldValue > pParamDesp->NotifyThresholdMax && newValue > pParamDesp->NotifyThresholdMax) ||
                           (oldValue >= pParamDesp->NotifyThresholdMin && oldValue <= pParamDesp->NotifyThresholdMax && newValue >= pParamDesp->NotifyThresholdMin && newValue <= pParamDesp->NotifyThresholdMax)
                       )
                    {
                        if ( pMonitorParam->PreviousValue )
                        {
                            SlapFreeVariable(pMonitorParam->PreviousValue);
                        }
                        
                        pMonitorParam->PreviousValue = pNewParamValue;
                        continue;
                    }
                }
                
                /* save the previous value as old */
                if( pDslhVarRecord->OldParamValue != NULL)
                {
                    SlapFreeVariable(pDslhVarRecord->OldParamValue);
                }

                pDslhVarRecord->OldParamValue = pMonitorParam->PreviousValue;
                /* SlapFreeVariable(pMonitorParam->PreviousValue); */

                pMonitorParam->PreviousValue = pNewParamValue;
                pParamFullName               = pDslhVarRecord->GetFullName((ANSC_HANDLE)pDslhVarRecord);

                if ( pParamFullName )
                {
                    bActiveNotify |= (pDslhVarRecord->Notification == DSLH_CWMP_NOTIFICATION_active);					 
                    pMonitorParam->Timestamp = AnscGetTickInSeconds();

                    pDslhVarRecord->NotifyValueChanged(pDslhVarRecord);

                    AnscTraceWarning(("DslhWmpdoMonitorTimerInvoke - value change detected on parameter <%s>.\n", pParamFullName));
					pDslhVarRecord->ReqSenderID = 0;
					pMonitorParam->PreviousValue->ReqSenderID = 0;
                    AnscFreeMemory(pParamFullName);
                }
            }
            else
            {
                SlapFreeVariable(pNewParamValue);
            }
        }
        else
        {
            pMonitorParam->PreviousValue = pDslhVarRecord->GetValue((ANSC_HANDLE)pDslhVarRecord);
        }
    }

    AnscReleaseLock(&pMyObject->MpoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
