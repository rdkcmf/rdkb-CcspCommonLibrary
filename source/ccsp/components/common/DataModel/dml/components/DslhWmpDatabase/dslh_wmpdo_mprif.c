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

    module:	dslh_wmpdo_mprif.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoMprRegParameter
        *   DslhWmpdoMprRegNotifyParam
        *   DslhWmpdoMprDelNotifyParam
        *   DslhWmpdoMprModNotifyParam
        *   DslhWmpdoMprResetNotifyParam
        *   DslhWmpdoMprRegObject
        *   DslhWmpdoMprUnregisterObject
        *   DslhWmpdoMprGetVarDataType
        *   DslhWmpdoMprGetObjEntity
        *   DslhWmpdoMprGetVarRecord
        *   DslhWmpdoMprGetVarRecord2
        *   DslhWmpdoMprGetObjRecord
        *   DslhWmpdoMprGetObjRecordInstance
        *   DslhWmpdoMprGetParamValue
        *   DslhWmpdoMprGetParamValue2
        *   DslhWmpdoMprSetParamValue
        *   DslhWmpdoMprSetParamValue2
        *   DslhWmpdoMprSetParamValue3
        *   DslhWmpdoMprSetParamValue4
        *   DslhWmpdoMprGetParamValueString
        *   DslhWmpdoMprSetParamValueString
        *   DslhWmpdoMprGetParamValueInt
        *   DslhWmpdoMprSetParamValueInt
        *   DslhWmpdoMprGetParamValueUint
        *   DslhWmpdoMprSetParamValueUint
        *   DslhWmpdoMprGetParamValueBool
        *   DslhWmpdoMprSetParamValueBool
        *   DslhWmpdoMprGetParamValueTime
        *   DslhWmpdoMprSetParamValueTime
        *   DslhWmpdoMprGetParamValueBase64
        *   DslhWmpdoMprSetParamValueBase64
        *   DslhWmpdoMprGetEntryObjectCount
        *   DslhWmpdoMprGetEntryObject
        *   DslhWmpdoMprGetEntryParamValue
        *   DslhWmpdoMprSetEntryParamValue
        *   DslhWmpdoMprApplyEntryChanges
        *   DslhWmpdoMprGetRootObjName
        *   DslhWmpdoMprGetReqCounter
        *   DslhWmpdoMprIncReqCounter
        *   DslhWmpdoMprRegEmptyObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin  Zhu

    ---------------------------------------------------------------

    revision:

        09/19/2005    initial revision.
        01/06/2011    Bin added "ResetNotifyParam" to ignore change from ACS.
        01/12/2011    Bin added "pRootObjName".
        02/07/2011    Bin added "UnregisterObject" support;
        02/28/2011    Bin added a set of ReqCounter apis to better support dynamic tables;

**********************************************************************/


#include "dslh_wmpdo_global.h"
#include "dslh_wmpdo_priv.h"
#include "dslh_ifo_tr69.h"
#include "ccsp_message_bus.h"
#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"
#include "ccsp_trace.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprRegParameter
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParamDescr,
                char*                       pObjName
            );

    description:

        This function is called to register a CWMP parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParamDescr
                Specifies the parameter descriptor to be registered.

                char*                       pObjName
                Specifies the corresponding object name.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprRegParameter
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamDescr,
        char*                       pObjName
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject           = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hRootObjEntity;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr         = (PDSLH_CWMP_PARAM_DESCR     )hParamDescr;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity          = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pParamPathTokens    = (PANSC_TOKEN_CHAIN          )NULL;
    char*                           pParamEnumTokens    = (char*                      )NULL;
    ULONG                           ulParamDataType     = (ULONG                      )0;
    ULONG                           ulParamSyntax       = (ULONG                      )0;
    ULONG                           ulParamContentType  = (ULONG                      )0;
    ULONG                           ulFormatValue1      = (ULONG                      )0;
    ULONG                           ulFormatValue2      = (ULONG                      )0;

    pParamPathTokens = AnscTcAllocate(pObjName, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pParamPathTokens )
    {
        return  ANSC_STATUS_BAD_NAME;
    }

    pObjEntity =
        (PDSLH_OBJ_ENTITY_OBJECT)pRootObjEntity->GetObjEntity2
            (
                (ANSC_HANDLE)pRootObjEntity,
                (ANSC_HANDLE)pParamPathTokens
            );

    if ( !pObjEntity )
    {
        pObjEntity =
            (PDSLH_OBJ_ENTITY_OBJECT)pRootObjEntity->GetObjEntity2
                (
                    (ANSC_HANDLE)pRootObjEntity,
                    (ANSC_HANDLE)pParamPathTokens
                );

        AnscTrace("Failed to RegParameter '%s': GetObjEntity2\n", pParamDescr->Name);

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }

    /* check whether the same parameter already exists or not */
    if( pObjEntity->GetVarEntity(pObjEntity, pParamDescr->Name) != NULL)
    {
        AnscTraceWarning(("Failed to RegParameter '%s'. The same parameter's registered already.\n", pParamDescr->Name));

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }

    if ( !pParamDescr->DataType )
    {
        AnscTraceWarning(("Failed to RegParameter '%s':DataType\n", pParamDescr->Name));

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pMyObject->ParseParamDataType
                (
                    (ANSC_HANDLE)pMyObject,
                    pParamDescr->DataType,
                    &ulParamDataType,
                    &ulFormatValue1,
                    &ulFormatValue2,
                    &pParamEnumTokens
                );
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTraceWarning(("Failed to RegParameter '%s':ParseParamDataType\n", pParamDescr->Name));

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }

    if ( !pParamDescr->Syntax )
    {
        AnscTraceWarning(("Failed to RegParameter '%s':Syntax\n", pParamDescr->Name));

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pMyObject->ParseParamSyntax
                (
                    (ANSC_HANDLE)pMyObject,
                    pParamDescr->Syntax,
                    &ulParamSyntax,
                    &ulParamContentType
                );
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTraceWarning(("Failed to RegParameter '%s':ParseParamSyntax '%s' \n", pParamDescr->Name, pParamDescr->Syntax));

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }

    pVarEntity =
        (PDSLH_VAR_ENTITY_OBJECT)DslhCreateVarEntity
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pObjEntity,
                (ANSC_HANDLE)NULL
            );

    if ( !pVarEntity )
    {
        AnscTraceWarning(("Failed to RegParameter '%s':CreateVarEntity\n", pParamDescr->Name));

        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }

    pVarEntity->hParentObjEntity   = (ANSC_HANDLE)pObjEntity;
    pVarEntity->ParamDescr         = pParamDescr;
    pVarEntity->DataType           = ulParamDataType;
    pVarEntity->Syntax             = ulParamSyntax;
    pVarEntity->bsUpdate           = pParamDescr->bsUpdate;
    pVarEntity->ContentType        = ulParamContentType;
    pVarEntity->FormatValue1       = ulFormatValue1;
    pVarEntity->FormatValue2       = ulFormatValue2;
    pVarEntity->Notification       = (pParamDescr->NotifyStatus == DSLH_CWMP_NOTIFY_STATUS_alwaysOn)? DSLH_CWMP_NOTIFICATION_active : DSLH_CWMP_NOTIFICATION_off;

    // Assign appropriate ContentType for 'dateTime', or the object type will default to 'string'.  RTian 2013/11/11
    if(pVarEntity->DataType == DSLH_CWMP_DATA_TYPE_dateTime) pVarEntity->ContentType = SLAP_CONTENT_TYPE_CALENDAR_TIME;

    if ( pParamEnumTokens )
    {
        returnStatus =
            pMyObject->ParseParamEnumTokens
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pVarEntity,
                    pParamEnumTokens
                );
    }

    returnStatus =
        pObjEntity->AddVarEntity
            (
                (ANSC_HANDLE)pObjEntity,
                (ANSC_HANDLE)pVarEntity
            );

    if ( pVarEntity->ParamDescr->NotifyStatus == DSLH_CWMP_NOTIFY_STATUS_alwaysOff )
    {
        returnStatus =
            pObjEntity->EnableForbidNotify
                (
                    (ANSC_HANDLE)pObjEntity,
                    TRUE
                );
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pParamPathTokens);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprRegNotifyParam
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord,
                BOOL                        bActiveNotification
            );

    description:

        This function is called to register a Var Record that has been
        configured for Passive/Active notification.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the var record object to be registered.

                BOOL                        bActiveNotification
                Specifies the notification attribute.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprRegNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        BOOL                        bActiveNotification
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject     = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    /*PDSLH_WMP_DATABASE_PROPERTY     pProperty     = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;*/
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PDSLH_VAR_RECORD_OBJECT         pDslhVarRecord= (PDSLH_VAR_RECORD_OBJECT    )hVarRecord;

    /*
     * Bin updated in Plugfest on 06/27/07
     *
     * We don't need to care the monitor is enabled or not, we should
     * add to the queue no matter what.
     */
    /* if ( !pProperty->bMonitorEnabled )*/
    if( FALSE)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pMonitorParam = (PDSLH_WMPDO_MONITOR_PARAM)AnscAllocateMemory(sizeof(DSLH_WMPDO_MONITOR_PARAM));
    }

    if ( !pMonitorParam )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMonitorParam->hVarRecord          = hVarRecord;
        pMonitorParam->bActiveNotification = bActiveNotification;
        pMonitorParam->PreviousValue       = NULL;
        pMonitorParam->NotificationLimit   = pDslhVarRecord->NotificationLimit;
        pMonitorParam->Timestamp           = 0;
    }

    /* Bin added on 04/11/09
     * Sometimes the value changes so fast and we fail to catch the change with the timer we defined.
     * we decide to retrieve current value right away */
    pMonitorParam->PreviousValue = pDslhVarRecord->GetValue((ANSC_HANDLE)pDslhVarRecord);
    if(pDslhVarRecord->RequesterID != 0)
    CcspTraceInfo(("<<<  pDslhVarRecord->RequesterID %lu >>>\n", pDslhVarRecord->RequesterID));
    
    AnscAcquireLock   (&pMyObject->MpoQueueLock);
    AnscQueuePushEntry(&pMyObject->MpoQueue, &pMonitorParam->Linkage);
    AnscReleaseLock   (&pMyObject->MpoQueueLock);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprDelNotifyParam
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord
            );

    description:

        This function is called to delete a Var Record that has been
        configured for Passive/Active notification.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the var record object to be unregistered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprDelNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject     = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    /*PDSLH_WMP_DATABASE_PROPERTY     pProperty     = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;*/
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY         )NULL;

    /*
     * Bin updated in Plugfest on 06/27/07
     *
     * We don't need to care the monitor is enabled or not, we should
     * remove it from the queue no matter what.
     */
    /* if ( !pProperty->bMonitorEnabled )*/
    if( FALSE)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->MpoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MpoQueue);

    while ( pSLinkEntry )
    {
        pMonitorParam = ACCESS_DSLH_WMPDO_MONITOR_PARAM(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pMonitorParam->hVarRecord == hVarRecord )
        {
            AnscQueuePopEntryByLink(&pMyObject->MpoQueue, &pMonitorParam->Linkage);

            DslhWmpdoFreeMonitorParam(pMonitorParam);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->MpoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprModNotifyParam
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord,
                ULONG                       ulLimit
            );

    description:

        This function is called to modify the notification limit on an
        active notification parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the var record object to be modified.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprModNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        ULONG                       ulLimit
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject     = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY         )NULL;
    PDSLH_VAR_RECORD_OBJECT         pDslhVarRecord= (PDSLH_VAR_RECORD_OBJECT    )hVarRecord;

    pDslhVarRecord->NotificationLimit = ulLimit;

    AnscAcquireLock(&pMyObject->MpoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MpoQueue);

    while ( pSLinkEntry )
    {
        pMonitorParam = ACCESS_DSLH_WMPDO_MONITOR_PARAM(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pMonitorParam->hVarRecord == hVarRecord )
        {
            pMonitorParam->NotificationLimit = ulLimit;

            break;
        }
    }

    AnscReleaseLock(&pMyObject->MpoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprResetNotifyParam
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarRecord
            );

    description:

        This function is called to reset the back value of a Var Record 
        to ignore value change from ACS.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarRecord
                Specifies the var record object to be unregistered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprResetNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject     = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->MpoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MpoQueue);

    while ( pSLinkEntry )
    {
        pMonitorParam = ACCESS_DSLH_WMPDO_MONITOR_PARAM(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pMonitorParam->hVarRecord == hVarRecord )
        {
            if( pMonitorParam->PreviousValue != NULL)
            {
                SlapFreeVariable(pMonitorParam->PreviousValue);
                pMonitorParam->PreviousValue       = NULL;
            }

            break;
        }
    }

    AnscReleaseLock(&pMyObject->MpoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprRegObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr
            );

    description:

        This function is called to register a CWMP object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprRegObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject        = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity   = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hRootObjEntity;
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr     = (PDSLH_CWMP_OBJECT_DESCR    )hObjDescr;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity       = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntityParent = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pObjPathTokens   = (PANSC_TOKEN_CHAIN          )NULL;
    PANSC_STRING_TOKEN              pObjNameToken    = (PANSC_STRING_TOKEN         )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf       = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    PDSLH_CWMP_PARAM_DESCR          pParamDesp       = (PDSLH_CWMP_PARAM_DESCR     )NULL;
    char                            pParamName[256]  = { 0 };
    char                            pObjName[256]    = { 0 };
    errno_t                         rc               = -1;

    pObjEntity = pDslhMprIf->GetObjEntity(pDslhMprIf->hOwnerContext, pObjectDescr->Name);

    if( pObjEntity != NULL)
    {
        AnscTraceWarning(("The object of '%s' is already registered.\n", pObjectDescr->Name));

        /* if the interface 'hDslhTr69If' == NULL, assign with this one */
        if( pObjEntity->ObjDescr->hDslhTr69If == NULL && pObjectDescr->hDslhTr69If != NULL)
        {
            COSACloneTR69Interface(pObjectDescr->hDslhTr69If, pObjEntity->ObjDescr->hDslhTr69If);
        }

		return DslhWmpdoMprRegObject_priv_CheckParameterUnderDevice(pObjectDescr);
    }

    pObjPathTokens = AnscTcAllocate(pObjectDescr->Name, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pObjPathTokens )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pSLinkEntry = AnscQueuePopEntryByIndex(&pObjPathTokens->TokensQueue, AnscQueueQueryDepth(&pObjPathTokens->TokensQueue) - 1);
    }

    if ( !pSLinkEntry )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else
    {
        pObjNameToken = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
    }

    pObjEntityParent =
        (PDSLH_OBJ_ENTITY_OBJECT)pRootObjEntity->GetObjEntity2
            (
                (ANSC_HANDLE)pRootObjEntity,
                (ANSC_HANDLE)pObjPathTokens
            );

    if ( !pObjEntityParent )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT2;
    }

    pObjEntity =
        (PDSLH_OBJ_ENTITY_OBJECT)DslhCreateObjEntity
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pObjEntity,
                (ANSC_HANDLE)NULL
            );

    if ( !pObjEntity )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT2;
    }

    pObjEntity->hParentObjEntity  = (ANSC_HANDLE)pObjEntityParent;
    pObjEntity->ObjDescr          = pObjectDescr;
    pObjEntity->LastName          = AnscCloneString(pObjNameToken->Name);
    pObjEntity->bForbidNotify     = FALSE;

    returnStatus =
        pObjEntityParent->AddObjEntity
            (
                (ANSC_HANDLE)pObjEntityParent,
                (ANSC_HANDLE)pObjEntity
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        goto EXIT2;
    }
    
    /* if it's a table, register a parameter named "xxxxNumberOfEntries" in parent object*/
    if( pObjectDescr->Type == DSLH_CWMP_OBJECT_TYPE_table && !pObjectDescr->bInvisible)
    {
        rc = sprintf_s(pParamName, sizeof(pParamName), "%s%s", pObjEntity->LastName, TR69_NUMBER_OF_ENTRIES_STRING);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        /* check whether the same parameter already exists or not */
        if( pObjEntityParent->GetVarEntity(pObjEntityParent, pParamName) == NULL)
        {
            pParamDesp      = (PDSLH_CWMP_PARAM_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_DESCR));

            if( pParamDesp != NULL)
            {
                pParamDesp->Name         = AnscCloneString(pParamName);
                pParamDesp->DataType     = AnscCloneString("unsignedInt");
                pParamDesp->Syntax       = AnscCloneString("uint32");
                pParamDesp->bWritable    = FALSE;
                pParamDesp->bInvisible   = FALSE;
                pParamDesp->bNeedReboot  = FALSE;
                pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_configurable;

                /* We need to collect it in parameter array in order to register to CR. */
                if ( pCpeController->uParameterArrayIndex < pCpeController->uParameterCount )
                {
                    name_spaceType_t *              pParameterArray   = pCpeController->hParameterArray;
                    char *                          pParameterHolder  = pCpeController->hParameterHolder;

                    if ( pParameterArray && pParameterHolder )
                    {
                        _ansc_sprintf( &pParameterHolder[pCpeController->uParameterIndex], "%s%s", pObjEntityParent->ObjDescr->Name, pParamName );
                        pParameterArray[pCpeController->uParameterArrayIndex].name_space   = &pParameterHolder[pCpeController->uParameterIndex];
                        pParameterArray[pCpeController->uParameterArrayIndex++].dataType   = ccsp_unsignedInt;
                        pCpeController->uParameterIndex += AnscSizeOfString( &pParameterHolder[pCpeController->uParameterIndex] ) + 1;
                    }
                }
                else
                {
                    AnscTraceWarning(("Failed to register: %s -- There are too many parameters and increase the array size.\n", pParamName));
                }


                /* get the parent object name */
                AnscCopyMemory(pObjName, pObjectDescr->Name, AnscSizeOfString(pObjectDescr->Name) - AnscSizeOfString(pObjEntity->LastName) -1 );

                if( ANSC_STATUS_SUCCESS != pDslhMprIf->RegParameter(pDslhMprIf->hOwnerContext, (ANSC_HANDLE)pParamDesp, pObjName))
                {
                    DslhCwmpFreeParamDescr(pParamDesp);
                }
            }
        }
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pObjNameToken )
    {
        AnscFreeMemory(pObjNameToken);
    }

EXIT1:

    if ( pObjPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pObjPathTokens);
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprUnregisterObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjFullName,
                char*                       pLastName,
                BOOL                        bIsTable
            );

    description:

        This function is called to unregister an object and remove 
        the related records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjFullName
                The full name of the object;

                char*                       pLastName
                The last name of the object;

                BOOL                        bIsTable
                whether it's a table object or not;

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprUnregisterObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjFullName,
        char*                       pLastName,
        BOOL                        bIsTable
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject        = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity       = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntityParent = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf       = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    char                            pParamName[256]  = { 0 };
    errno_t                         rc               = -1;

    pObjEntity = pDslhMprIf->GetObjEntity(pDslhMprIf->hOwnerContext, pObjFullName);

    if( pObjEntity == NULL)
    {
        AnscTraceWarning(("The object '%s' doesn't exist, failed to unregister.\n", pObjFullName));

        return ANSC_STATUS_BAD_NAME;
    }

    pObjEntityParent =
        (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->hParentObjEntity;

    if ( !pObjEntityParent )
    {
        return ANSC_STATUS_BAD_NAME;
    }

    /* remove the record */    
    returnStatus = pObjEntityParent->DeleteObjRecordByName(pObjEntityParent, pLastName);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTraceWarning(("Failed to DeleteObjRecordByName '%s'.\n", pLastName));

        return returnStatus;
    }

    /* remove the entity */
    returnStatus =
        pObjEntityParent->DelObjEntity
            (
                (ANSC_HANDLE)pObjEntityParent,
                (ANSC_HANDLE)pLastName
            );

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTraceWarning(("Failed to DelObjEntity '%s'.\n", pLastName));

        return returnStatus;
    }


    /* if it's a table, unregister the parameter named "xxxxNumberOfEntries" in parent object*/
    if( bIsTable)
    {
        rc = sprintf_s(pParamName, sizeof(pParamName), "%s%s", pLastName, TR69_NUMBER_OF_ENTRIES_STRING);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        returnStatus = pObjEntityParent->DelVarEntity(pObjEntityParent, pParamName);

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceWarning(("Failed to DelVarEntity '%s'.\n", pParamName));

            return returnStatus;
        }
    }

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhWmpdoMprGetVarDataType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve the data type of a
        parameter object by matching the parameter name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name to be matched.

    return:     data type.

**********************************************************************/

ULONG
DslhWmpdoMprGetVarDataType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord     = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity     = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pVarRecord )
    {
        return  DSLH_CWMP_DATA_TYPE_unspecified;
    }
    else
    {
        pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;
    }

    return  pVarEntity->DataType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to retrieve a static object by matching
        the object name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object name to be matched.

    return:     static object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hRootObjEntity;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pObjPathTokens = (PANSC_TOKEN_CHAIN          )NULL;

#if 1 /* this update brought regressions to bsp and scp pages */
    /* We don't allow multiple '.' together like "..." in the objName */
    if( pObjName && (pObjName[0] == '.' || _ansc_strstr(pObjName, "..") != NULL))
    {
        AnscTraceWarning(("****\n====\nInvalid name: %s\n====\n****\n", pObjName));

        return (ANSC_HANDLE)NULL;
    }
#endif

    pObjPathTokens = AnscTcAllocate(pObjName, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pObjPathTokens )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjEntity =
        (PDSLH_OBJ_ENTITY_OBJECT)pRootObjEntity->GetObjEntity2
            (
                (ANSC_HANDLE)pRootObjEntity,
                (ANSC_HANDLE)pObjPathTokens
            );

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pObjPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pObjPathTokens);
    }

    return  (ANSC_HANDLE)pObjEntity;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetVarRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter object by
        matching the parameter name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name to be matched.

    return:     parameter object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject        = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord   = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord       = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pParamPathTokens = (PANSC_TOKEN_CHAIN          )NULL;
    PANSC_STRING_TOKEN              pParamNameToken  = (PANSC_STRING_TOKEN         )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;

#if 1 /* this update brought regressions to bsp and scp pages */
    /* We don't allow multiple '.' together like "..." in the Name */
    if( pParamName && (pParamName[0] == '.' || _ansc_strstr(pParamName, "..") != NULL))
    {
        AnscTraceWarning(("****\n====\nInvalid name: %s\n====\n****\n", pParamName));
        return (ANSC_HANDLE)NULL;
    }
#endif

    pParamPathTokens = AnscTcAllocate(pParamName, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pParamPathTokens )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSLinkEntry = AnscQueuePopEntryByIndex(&pParamPathTokens->TokensQueue, AnscQueueQueryDepth(&pParamPathTokens->TokensQueue) - 1);
    }

    if ( !pSLinkEntry )
    {

        goto  EXIT1;
    }
    else
    {
        pParamNameToken = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
    }
    
    if(pRootObjRecord)
    {
        pObjRecord =
            (PDSLH_OBJ_RECORD_OBJECT)pRootObjRecord->GetObjRecord2
            (
                (ANSC_HANDLE)pRootObjRecord,
                (ANSC_HANDLE)pParamPathTokens
            );
    }

    if ( !pObjRecord )
    {

        goto  EXIT2;
    }

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pObjRecord->GetVarRecord
            (
                (ANSC_HANDLE)pObjRecord,
                pParamNameToken->Name
            );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pParamNameToken )
    {
        AnscFreeMemory(pParamNameToken);
    }

EXIT1:

    if ( pParamPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pParamPathTokens);
    }

    return  (ANSC_HANDLE)pVarRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetVarRecord2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter object by
        matching the parameter name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object name to be matched.

                char*                       pParamName
                Specifies the parameter name to be matched.

    return:     parameter object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetVarRecord2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject        = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord   = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord       = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pParamPathTokens = (PANSC_TOKEN_CHAIN          )NULL;

#if 1 /* this update brought regressions to bsp and scp pages */
    /* We don't allow multiple '.' together like "..." in the objName */
    if( pObjName && (pObjName[0] == '.' || _ansc_strstr(pObjName, "..") != NULL))
    {
        AnscTraceWarning(("****\n====\nInvalid name: %s\n====\n****\n", pObjName));

        return (ANSC_HANDLE)NULL;
    }
#endif

    pParamPathTokens = AnscTcAllocate(pObjName, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pParamPathTokens )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pRootObjRecord->GetObjRecord2
            (
                (ANSC_HANDLE)pRootObjRecord,
                (ANSC_HANDLE)pParamPathTokens
            );

    if ( !pObjRecord )
    {

        goto  EXIT1;
    }

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pObjRecord->GetVarRecord
            (
                (ANSC_HANDLE)pObjRecord,
                pParamName
            );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pParamPathTokens);
    }

    return  (ANSC_HANDLE)pVarRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to retrieve a configuration object by
        matching the object name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object name to be matched.

    return:     configuration object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pObjPathTokens = (PANSC_TOKEN_CHAIN          )NULL;

#if 1 /* this update brought regressions to bsp and scp pages */
    /* We don't allow multiple '.' together like "..." in the objName */
    if( pObjName && (pObjName[0] == '.' || _ansc_strstr(pObjName, "..") != NULL))
    {
        AnscTraceWarning(("****\n====\nInvalid name: %s\n====\n****\n", pObjName));
        return (ANSC_HANDLE)NULL;
    }
#endif

    pObjPathTokens = AnscTcAllocate(pObjName, DSLH_PARAM_NAME_SEPARATORS);

    if ( !pObjPathTokens )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pRootObjRecord->GetObjRecord2
            (
                (ANSC_HANDLE)pRootObjRecord,
                (ANSC_HANDLE)pObjPathTokens
            );

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pObjPathTokens )
    {
        AnscTcFree((ANSC_HANDLE)pObjPathTokens);
    }

    return  (ANSC_HANDLE)pObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetObjRecordInstance
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to retrieve a runtime object instance
        by matching the value of a runtime parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object name to be matched.

                char*                       pParamName
                Specifies the parameter name to be matched.

                SLAP_VARIABLE*              pParamValue
                Specifies the parameter value to be matched.

    return:     configuration object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetObjRecordInstance
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;


#if 1 /* this update brought regressions to bsp and scp pages */
    /* We don't allow multiple '.' together like "..." in the objName */
    if( pObjName && (pObjName[0] == '.' || _ansc_strstr(pObjName, "..") != NULL))
    {
        AnscTraceWarning(("****\n====\nInvalid name: %s\n====\n****\n", pObjName));
        return (ANSC_HANDLE)NULL;
    }
#endif

    pObjEntity =
        (PDSLH_OBJ_ENTITY_OBJECT)pDslhMprIf->GetObjEntity
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName
            );

    if ( !pObjEntity )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pObjEntity->GetObjRecord
            (
                (ANSC_HANDLE)pObjEntity,
                pParamName,
                pParamValue
            );

    return  (ANSC_HANDLE)pObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhWmpdoMprGetParamValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
DslhWmpdoMprGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord     = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    SLAP_VARIABLE*                  pParamValue    = (SLAP_VARIABLE*             )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pVarRecord )
    {
        goto  EXIT1;
    }
    else
    {
        pParamValue = pVarRecord->GetValue((ANSC_HANDLE)pVarRecord);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  pParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhWmpdoMprGetParamValue2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value. This
        function allows caller provides the object and parameter names
        separately.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the corresponding object name.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
DslhWmpdoMprGetParamValue2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord     = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    SLAP_VARIABLE*                  pParamValue    = (SLAP_VARIABLE*             )NULL;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord2
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName,
                pParamName
            );

    if ( !pVarRecord )
    {
        goto  EXIT1;
    }
    else
    {
        pParamValue = pVarRecord->GetValue((ANSC_HANDLE)pVarRecord);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  pParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                SLAP_VARIABLE*              pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf        = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord         = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity         = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    char*                           pFaultParamName    = NULL;

    if ( !pDslhMpaIf->LockWriteAccess
            (
                (ANSC_HANDLE)pDslhMpaIf->hOwnerContext,
                pAccessEntity
            ) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pVarRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( !strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 )
    {
        if( !pVarRecord->AccessList)
        {
            if ( !strcmp(pAccessEntity, DSLH_MPA_ENTITY_Subscriber) == 0 )  /* by default it's subscriber */
            {
                returnStatus = ANSC_STATUS_ACCESS_DENIED;

                goto  EXIT1;
            }

        }
        else if (!_ansc_strstr( pVarRecord->AccessList, pAccessEntity ) )
        {
            returnStatus = ANSC_STATUS_ACCESS_DENIED;

            goto  EXIT1;
        }
    }

    if ( !pVarRecord->TstValue
            (
                (ANSC_HANDLE)pVarRecord,
                pParamValue
            ) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pVarRecord->SetValue
                (
                    (ANSC_HANDLE)pVarRecord,
                    pParamValue
                );
    }

    pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord;

    if ( !pObjRecord->VerifyChanges((ANSC_HANDLE)pObjRecord, &pFaultParamName) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT2;
    }
    else
    {
        returnStatus = pObjRecord->CommitChanges((ANSC_HANDLE)pObjRecord);

		if( returnStatus != ANSC_STATUS_SUCCESS)
		{
			AnscTrace("Failed to CommitChanges in '%s'\n", pParamName);

			returnStatus = ANSC_STATUS_BAD_PARAMETER;

			goto EXIT2;

		}
    }

    /*
     * Bin modified here.
     *
     * Since this value change comes from ACS, it shouldn't send any notification.
     */

    pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

    AnscTraceEntry(("The notification = %d\n", pVarEntity->Notification));

    if ( strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 ) 
    {
        /* Reset the history value and monitor */
        pDslhMprIf->ResetNotifyParam(pDslhMprIf->hOwnerContext, pVarRecord);
    }
    else
    {
        /*
         * ACS can configure the CPE to initiate a session when certain parameters change in value. The
         * following values are defined:
         *
         *      0 - Notification off. The CPE need not inform the ACS of a change to the
         *          specified parameter(s).
         *      1 - Passive notification. Whenever the specified parameter value changes, the
         *          CPE MUST include the new value in the ParameterList in the Inform message
         *          that is sent the next time a session is established to the ACS.
         *      2 - Active notification. Whenever the specified parameter value changes, the
         *          CPE MUST initiate a session to the ACS, and include the new value in the
         *          ParameterList in the associated Inform message.
         */
        switch ( pVarEntity->Notification )
        {
            case    DSLH_CWMP_NOTIFICATION_off :

                    returnStatus = ANSC_STATUS_SUCCESS;

                    break;

            case    DSLH_CWMP_NOTIFICATION_passive :

                    pVarRecord->NotifyValueChanged((ANSC_HANDLE)pVarRecord);

                    break;

            case    DSLH_CWMP_NOTIFICATION_active :

                    pVarRecord->NotifyValueChanged((ANSC_HANDLE)pVarRecord);

                    break;

            default :

                    break;
        }
    }

    /* remove al the back and temp values */
    returnStatus = pVarRecord->CommitChange ((ANSC_HANDLE)pVarRecord, FALSE);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pFaultParamName )
    {
        AnscFreeMemory(pFaultParamName);
    }

    if ( pVarRecord )
    {
        pVarRecord->CancelChange((ANSC_HANDLE)pVarRecord);
    }

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    pDslhMpaIf->UnlockWriteAccess((ANSC_HANDLE)pDslhMpaIf->hOwnerContext);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValue2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure a parameter value. The
        difference between this function and the above SetParamValue()
        is that this call doesn't commit the change at object level.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                SLAP_VARIABLE*              pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValue2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    /*PDSLH_MSO_INTERFACE             pDslhMsoIf         = (PDSLH_MSO_INTERFACE        )pDslhCpeController->GetDslhMsoIf((ANSC_HANDLE)pDslhCpeController);*/
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord         = (PDSLH_VAR_RECORD_OBJECT    )NULL;

    if ( !pDslhMpaIf->LockWriteAccess
            (
                (ANSC_HANDLE)pDslhMpaIf->hOwnerContext,
                pAccessEntity
            ) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pVarRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( !strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 )
    {
        if( !pVarRecord->AccessList)
        {
            if ( !strcmp(pAccessEntity, DSLH_MPA_ENTITY_Subscriber) == 0 )  /* by default it's subscriber */
            {
                returnStatus = ANSC_STATUS_ACCESS_DENIED;

                goto  EXIT1;
            }

        }
        else if (!_ansc_strstr( pVarRecord->AccessList, pAccessEntity ) )
        {
            returnStatus = ANSC_STATUS_ACCESS_DENIED;

            goto  EXIT1;
        }
    }

    if ( !pVarRecord->TstValue
            (
                (ANSC_HANDLE)pVarRecord,
                pParamValue
            ) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pVarRecord->SetValue
                (
                    (ANSC_HANDLE)pVarRecord,
                    pParamValue
                );
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    pDslhMpaIf->UnlockWriteAccess((ANSC_HANDLE)pDslhMpaIf->hOwnerContext);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValue3
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pObjName,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure a parameter value. The
        difference between this function and the above SetParamValue()
        is that this call doesn't commit the change at object level.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pObjName
                Specifies the corresponding object name.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                SLAP_VARIABLE*              pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValue3
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    /*PDSLH_MSO_INTERFACE             pDslhMsoIf         = (PDSLH_MSO_INTERFACE        )pDslhCpeController->GetDslhMsoIf((ANSC_HANDLE)pDslhCpeController);*/
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord         = (PDSLH_VAR_RECORD_OBJECT    )NULL;

    if ( !pDslhMpaIf->LockWriteAccess
            (
                (ANSC_HANDLE)pDslhMpaIf->hOwnerContext,
                pAccessEntity
            ) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord2
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName,
                pParamName
            );

    if ( !pVarRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( !strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 )
    {
        if( !pVarRecord->AccessList)
        {
            if ( !strcmp(pAccessEntity, DSLH_MPA_ENTITY_Subscriber) == 0 )  /* by default it's subscriber */
            {
                returnStatus = ANSC_STATUS_ACCESS_DENIED;

                goto  EXIT1;
            }

        }
        else if (!_ansc_strstr( pVarRecord->AccessList, pAccessEntity ) )
        {
            returnStatus = ANSC_STATUS_ACCESS_DENIED;

            goto  EXIT1;
        }
    }

    if ( !pVarRecord->TstValue
            (
                (ANSC_HANDLE)pVarRecord,
                pParamValue
            ) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pVarRecord->SetValue
                (
                    (ANSC_HANDLE)pVarRecord,
                    pParamValue
                );
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    pDslhMpaIf->UnlockWriteAccess((ANSC_HANDLE)pDslhMpaIf->hOwnerContext);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValue4
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pObjName,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure a parameter value. This
        function does commit at object level.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pObjName
                Specifies the corresponding object name.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                SLAP_VARIABLE*              pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValue4
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf         = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord         = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity         = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    char*                           pFaultParamName    = NULL;

    if ( !pDslhMpaIf->LockWriteAccess
            (
                (ANSC_HANDLE)pDslhMpaIf->hOwnerContext,
                pAccessEntity
            ) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord2
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName,
                pParamName
            );

    if ( !pVarRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( !strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 )
    {
        if( !pVarRecord->AccessList)
        {
            if ( !strcmp(pAccessEntity, DSLH_MPA_ENTITY_Subscriber) == 0 )  /* by default it's subscriber */
            {
                returnStatus = ANSC_STATUS_ACCESS_DENIED;

                goto  EXIT1;
            }

        }
        else if (!_ansc_strstr( pVarRecord->AccessList, pAccessEntity ) )
        {
            returnStatus = ANSC_STATUS_ACCESS_DENIED;

            goto  EXIT1;
        }
    }

    if ( !pVarRecord->TstValue
            (
                (ANSC_HANDLE)pVarRecord,
                pParamValue
            ) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pVarRecord->SetValue
                (
                    (ANSC_HANDLE)pVarRecord,
                    pParamValue
                );
    }

    pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord;

    if ( !pObjRecord->VerifyChanges((ANSC_HANDLE)pObjRecord, &pFaultParamName) )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT2;
    }
    else
    {
        returnStatus = pObjRecord->CommitChanges((ANSC_HANDLE)pObjRecord);

		if ( returnStatus != ANSC_STATUS_SUCCESS)
		{
			AnscTrace("Failed to CommitChanges in '%s'\n", pParamName);

			returnStatus = ANSC_STATUS_BAD_PARAMETER;

			goto EXIT2;

		}
    }

    /*
     * Bin modified here.
     *
     * Since this value change comes from ACS, it shouldn't send any notification.
     */

    pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

    AnscTraceEntry(("The notification = %d\n", pVarEntity->Notification));

    if ( strcmp(pAccessEntity,DSLH_MPA_ENTITY_ACS) == 0 )
    {
        /* Reset the history value and monitor */
        pDslhMprIf->ResetNotifyParam(pDslhMprIf->hOwnerContext, pVarRecord);
    }
    else
    {
        /*
         * ACS can configure the CPE to initiate a session when certain parameters change in value. The
         * following values are defined:
         *
         *      0 - Notification off. The CPE need not inform the ACS of a change to the
         *          specified parameter(s).
         *      1 - Passive notification. Whenever the specified parameter value changes, the
         *          CPE MUST include the new value in the ParameterList in the Inform message
         *          that is sent the next time a session is established to the ACS.
         *      2 - Active notification. Whenever the specified parameter value changes, the
         *          CPE MUST initiate a session to the ACS, and include the new value in the
         *          ParameterList in the associated Inform message.
         */
        switch ( pVarEntity->Notification )
        {
            case    DSLH_CWMP_NOTIFICATION_off :

                    returnStatus = ANSC_STATUS_SUCCESS;

                    break;

            case    DSLH_CWMP_NOTIFICATION_passive :

                    pVarRecord->NotifyValueChanged((ANSC_HANDLE)pVarRecord);

                    break;

            case    DSLH_CWMP_NOTIFICATION_active :

                    pVarRecord->NotifyValueChanged((ANSC_HANDLE)pVarRecord);

                    break;

            default :

                    break;
        }
    }

    returnStatus = pVarRecord->CommitChange ((ANSC_HANDLE)pVarRecord, FALSE);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pFaultParamName )
    {
        AnscFreeMemory(pFaultParamName);
    }

    if ( pVarRecord )
    {
        pVarRecord->CancelChange((ANSC_HANDLE)pVarRecord);
    }

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    pDslhMpaIf->UnlockWriteAccess((ANSC_HANDLE)pDslhMpaIf->hOwnerContext);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhWmpdoMprGetParamValueString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

char*
DslhWmpdoMprGetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    char*                           pParamValue    = (char*                      )NULL;

    pParamVariable =
        pDslhMprIf->GetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pParamVariable )
    {
        return  NULL;
    }
    else if ( pParamVariable->Syntax != SLAP_VAR_SYNTAX_TYPE_string )
    {
        goto  EXIT1;
    }
    else
    {
        pParamValue = pParamVariable->Variant.varString;

        pParamVariable->Variant.varString = NULL;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  pParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                char*                       pParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;

    SlapAllocVariable(pParamVariable);

    if ( !pParamVariable )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pParamVariable->Syntax            = SLAP_VAR_SYNTAX_TYPE_string;
        pParamVariable->Variant.varString = pParamValue;
    }

    returnStatus =
        pDslhMprIf->SetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pAccessEntity,
                pParamName,
                pParamVariable
            );

    pParamVariable->Variant.varString = NULL;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        DslhWmpdoMprGetParamValueInt
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

int
DslhWmpdoMprGetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    int                             iParamValue    = (int                        )0;

    pParamVariable =
        pDslhMprIf->GetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pParamVariable )
    {
        return  0;
    }
    else if ( (pParamVariable->Syntax != SLAP_VAR_SYNTAX_TYPE_int   ) &&
              (pParamVariable->Syntax != SLAP_VAR_SYNTAX_TYPE_uint32) )
    {
        goto  EXIT1;
    }
    else
    {
        iParamValue = pParamVariable->Variant.varInt;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  iParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueInt
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                int                         iParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                int                         iParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        int                         iParamValue
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;

    SlapAllocVariable(pParamVariable);

    if ( !pParamVariable )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pParamVariable->Syntax         = SLAP_VAR_SYNTAX_TYPE_int;
        pParamVariable->Variant.varInt = iParamValue;
    }

    returnStatus =
        pDslhMprIf->SetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pAccessEntity,
                pParamName,
                pParamVariable
            );

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhWmpdoMprGetParamValueUint
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

ULONG
DslhWmpdoMprGetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    ULONG                           ulParamValue   = (ULONG                      )0;

    pParamVariable =
        pDslhMprIf->GetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pParamVariable )
    {
        return  0;
    }
    else if ( pParamVariable->Syntax != SLAP_VAR_SYNTAX_TYPE_uint32 )
    {
        AnscTraceWarning(("DslhWmpdoMprGetParamValueUint -- syntax of stored parameter %s is %lu!\n", pParamName, pParamVariable->Syntax)); 
        goto  EXIT1;
    }
    else
    {
        ulParamValue = pParamVariable->Variant.varUint32;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  ulParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueUint
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                ULONG                       ulParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                ULONG                       ulParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        ULONG                       ulParamValue
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    SlapAllocVariable(pParamVariable);

    if ( !pParamVariable )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pParamVariable->Syntax            = SLAP_VAR_SYNTAX_TYPE_uint32;
        pParamVariable->Variant.varUint32 = ulParamValue;
    }

    returnStatus =
        pDslhMprIf->SetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pAccessEntity,
                pParamName,
                pParamVariable
            );

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhWmpdoMprGetParamValueBool
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

BOOL
DslhWmpdoMprGetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    BOOL                            bParamValue    = (BOOL                       )FALSE;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    pParamVariable =
        pDslhMprIf->GetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pParamVariable )
    {
        return  FALSE;
    }
    else if ( pParamVariable->Syntax != SLAP_VAR_SYNTAX_TYPE_bool )
    {
        goto  EXIT1;
    }
    else
    {
        bParamValue = (pParamVariable->Variant.varBool != 0)? TRUE : FALSE;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  bParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueBool
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                BOOL                        bParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                BOOL                        bParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        BOOL                        bParamValue
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    SLAP_VARIABLE*                  pParamVariable = (SLAP_VARIABLE*             )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    SlapAllocVariable(pParamVariable);

    if ( !pParamVariable )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pParamVariable->Syntax          = SLAP_VAR_SYNTAX_TYPE_bool;
        pParamVariable->Variant.varBool = bParamValue? SLAP_TRUE : SLAP_FALSE;
    }

    returnStatus =
        pDslhMprIf->SetParamValue
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pAccessEntity,
                pParamName,
                pParamVariable
            );

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pParamVariable )
    {
        SlapFreeVariable(pParamVariable);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhWmpdoMprGetParamValueTime
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

char*
DslhWmpdoMprGetParamValueTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    return  pDslhMprIf->GetParamValueString
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pParamName
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueTime
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                char*                       pParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    return  pDslhMprIf->SetParamValueString
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pAccessEntity,
                    pParamName,
                    pParamValue
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhWmpdoMprGetParamValueBase64
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                returned.

    return:     parameter value.

**********************************************************************/

char*
DslhWmpdoMprGetParamValueBase64
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    return  pDslhMprIf->GetParamValueString
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pParamName
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetParamValueBase64
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamName,
                char*                       pParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter name whose value is to be
                configured.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetParamValueBase64
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf     = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;

    return  pDslhMprIf->SetParamValueString
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pAccessEntity,
                    pParamName,
                    pParamValue
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhWmpdoMprGetEntryObjectCount
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTableObjRecord
            );

    description:

        This function is called to retrieve the number of child entry
        objects under the specified table object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTableObjRecord
                Specifies the obj record of the table object to be
                checked on.

    return:     object count.

**********************************************************************/

ULONG
DslhWmpdoMprGetEntryObjectCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    /*PDSLH_MSO_INTERFACE             pDslhMsoIf         = (PDSLH_MSO_INTERFACE        )pDslhCpeController->GetDslhMsoIf((ANSC_HANDLE)pDslhCpeController);*/
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordTable    = (PDSLH_OBJ_RECORD_OBJECT    )hTableObjRecord;

    if ( !pObjRecordTable )
    {
        return  0;
    }

    return  pObjRecordTable->GetObjRecordCount((ANSC_HANDLE)pObjRecordTable);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoMprGetEntryObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTableObjRecord,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a child entry object by
        matching the index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTableObjRecord
                Specifies the obj record of the table object to be
                checked on.

                ULONG                       ulIndex
                Specifies the index of the entry object.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoMprGetEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord,
        ULONG                       ulIndex
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordTable    = (PDSLH_OBJ_RECORD_OBJECT    )hTableObjRecord;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordEntry    = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY         )NULL;

    if ( !pObjRecordTable )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSLinkEntry = AnscQueueGetEntryByIndex(&pObjRecordTable->ObjroQueue, ulIndex);

    if ( pSLinkEntry )
    {
        pObjRecordEntry = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pObjRecordEntry;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhWmpdoMprGetEntryParamValue
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntryObjRecord,
                char*                       pParamName
            );

    description:

        This function is called to retrieve the value of the specified
        parameter under a child entry object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntryObjRecord
                Specifies the obj record of the entry object to be
                checked on.

                char*                       pParamName
                Specifies the parameter name to be matched.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
DslhWmpdoMprGetEntryParamValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pParamName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordEntry    = (PDSLH_OBJ_RECORD_OBJECT    )hEntryObjRecord;

    if ( !pObjRecordEntry )
    {
        return  NULL;
    }

    return  pDslhMprIf->GetParamValue2
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pObjRecordEntry->FullName,
                    pParamName
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprSetEntryParamValue
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntryObjRecord,
                char*                       pAccessEntity,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure the value of the specified
        parameter under a child entry object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntryObjRecord
                Specifies the obj record of the entry object to be
                checked on.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamName
                Specifies the parameter name to be matched.

                SLAP_VARIABLE*              pParamValue
                Specifies the new value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprSetEntryParamValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordEntry    = (PDSLH_OBJ_RECORD_OBJECT    )hEntryObjRecord;

    if ( !pObjRecordEntry )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  pDslhMprIf->SetParamValue3
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pAccessEntity,
                    pObjRecordEntry->FullName,
                    pParamName,
                    pParamValue
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMprApplyEntryChanges
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntryObjRecord
            );

    description:

        This function is called to commit the changes made to the
        specified entry object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntryObjRecord
                Specifies the obj record of the entry object to be
                checked on.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprApplyEntryChanges
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordEntry    = (PDSLH_OBJ_RECORD_OBJECT    )hEntryObjRecord;

    return pObjRecordEntry->CommitChanges2((ANSC_HANDLE)pObjRecordEntry);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhWmpdoMprGetRootObjName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the root object name:
        "Device." or "InternetGatewayDevice."

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     "Device." or "InternetGatewayDevice."

**********************************************************************/
char*
DslhWmpdoMprGetRootObjName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject       = (PDSLH_WMP_DATABASE_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity  = (PDSLH_OBJ_ENTITY_OBJECT  )pMyObject->hRootObjEntity;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY       )NULL;

    if( pMyObject->pRootObjName == NULL)
    {
        if( !pRootObjEntity)
        {
            pMyObject->pRootObjName = AnscCloneString(DM_ROOTNAME);
        }
        else
        {
            /* supposed to be the first one */
            pSLinkEntry = AnscQueueGetFirstEntry(&pRootObjEntity->ObjeoQueue);

            if( pSLinkEntry )
            {
                pChildObjEntity = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
                pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                if( pChildObjEntity->ObjDescr != NULL)
                {
                    pMyObject->pRootObjName = AnscCloneString(pChildObjEntity->ObjDescr->Name);
                }
                else
                {
                    pMyObject->pRootObjName = AnscCloneString(DM_ROOTNAME);
                }
            }
        }
    }

    return pMyObject->pRootObjName;
}

/**********************************************************************

    prototype:

        ULONG
        DslhWmpdoMprGetReqCounter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve current value of Request Counter;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The counter;

**********************************************************************/
ULONG
DslhWmpdoMprGetReqCounter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject       = (PDSLH_WMP_DATABASE_OBJECT)hThisObject;

    return pMyObject->uReqCounter;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhWmpdoMprIncReqCounter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to increase the Request Counter by 1;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The status of the operation;

**********************************************************************/
ANSC_STATUS
DslhWmpdoMprIncReqCounter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject       = (PDSLH_WMP_DATABASE_OBJECT)hThisObject;

    if( pMyObject->uReqCounter == 0xFFFFFFFF)
    {
        pMyObject->uReqCounter = 1;
    }
    else
    {
        pMyObject->uReqCounter ++;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhWmpdoMprRegEmptyObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to create an empty object if not exists;


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                The full object name. Should ends with a '.'

    return:     The status of the operation;

**********************************************************************/
ANSC_STATUS
DslhWmpdoMprRegEmptyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController     = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    char*                           pCopy              = pObjName;
    char*                           pTemp              = NULL;
    ULONG                           ulSize             = 0;
    char                            buffer[256]        = { 0 };

    if( pObjName == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    ulSize = AnscSizeOfString(pObjName);

    if( pObjName[ulSize - 1] != '.')
    {
        AnscTrace("Invalid object name in DslhWmpdoMprRegEmptyObject: %s\n", pObjName);

        return ANSC_STATUS_FAILURE;
    }

    /* create the object if not have */
    pTemp = _ansc_strstr(pCopy, ".");

    while( pTemp != NULL)
    {
        AnscCopyMemory(buffer, pObjName, (ULONG)(pTemp - pObjName + 1));

        if( pDslhMprIf->GetObjEntity(pDslhMprIf->hOwnerContext, buffer) == NULL)
        {
            /* try to create this object */
            AnscTraceWarning(("Register empty object : %s\n", buffer));
            pCpeController->RegisterObject2((ANSC_HANDLE)pCpeController, buffer, NULL, NULL);
        }

        /* get next */
        pCopy = pTemp + AnscSizeOfString(".");
        if( pCopy == NULL || pCopy[0] == 0)
        {
             break;
        }

        pTemp = _ansc_strstr(pCopy, ".");
    }


    return ANSC_STATUS_SUCCESS;
}
