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

    module:	dslh_varro_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Var Record Object.

        *   DslhVarroGetLastName
        *   DslhVarroGetFullName
        *   DslhVarroSetNotification
        *   DslhVarroSetAccessList
        *   DslhVarroReset
		*	DslhVarroIsRecordWritable
        *   DslhVarroNotifyValueChanged

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/28/05    initial revision.

**********************************************************************/


#include "dslh_varro_global.h"
#include "ccsp_trace.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVarroGetDslhObjRecord
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

ANSC_HANDLE
DslhVarroGetDslhObjRecord
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    return  pMyObject->hDslhObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetDslhObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetDslhObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    pMyObject->hDslhObjRecord = hObjRecord;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVarroGetDslhVarEntity
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

ANSC_HANDLE
DslhVarroGetDslhVarEntity
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    return  pMyObject->hDslhVarEntity;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetDslhVarEntity
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVarEntity
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVarEntity
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetDslhVarEntity
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    pMyObject->hDslhVarEntity = hVarEntity;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVarroGetPoamIrepFoParameter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

ANSC_HANDLE
DslhVarroGetPoamIrepFoParameter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    return  pMyObject->hIrepFoParameter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetPoamIrepFoParameter
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPoamIrepFolder
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPoamIrepFolder
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetPoamIrepFoParameter
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPoamIrepFolder
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    pMyObject->hIrepFoParameter = hPoamIrepFolder;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhVarroGetLastName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

char*
DslhVarroGetLastName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity   = (PDSLH_VAR_ENTITY_OBJECT)pMyObject->hDslhVarEntity;

    return  pVarEntity->ParamDescr->Name;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhVarroGetFullName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

char*
DslhVarroGetFullName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject      = (PDSLH_VAR_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity     = (PDSLH_VAR_ENTITY_OBJECT)pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    char*                           pLastName      = (char*                  )pVarEntity->ParamDescr->Name;
    char*                           pFullName      = (char*                  )NULL;
    ULONG                           ulFullNameSize = (ULONG                  )0;
    errno_t                         rc             = -1;

    ulFullNameSize = (pObjRecord->FullName? AnscSizeOfString(pObjRecord->FullName) : 0) + AnscSizeOfString(pLastName) + 4;
    pFullName      = (char*)AnscAllocateMemory(ulFullNameSize);

    if ( !pFullName )
    {
        return  NULL;
    }
    else
    {
        rc = sprintf_s
            (
                pFullName,
                ulFullNameSize,
                "%s%s",
                pObjRecord->FullName,
                pLastName
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return NULL;
        }
    }

    return  pFullName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        DslhVarroGetNotification
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

int
DslhVarroGetNotification
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    return  pMyObject->Notification;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetNotification
            (
                ANSC_HANDLE                 hThisObject,
                int                         iNotification
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         iNotification
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetNotification
    (
        ANSC_HANDLE                 hThisObject,
        int                         iNotification
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)pDslhCpeController);
    PCHAR                           pFullName            = NULL;

    /* we don't allow Notification to be changed if it's defined as Forced or always Off notification */
    if ( pVarEntity->ParamDescr->NotifyStatus != DSLH_CWMP_NOTIFY_STATUS_configurable && pVarEntity->ParamDescr->NotifyStatus != DSLH_CWMP_NOTIFY_STATUS_configActive)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

	pFullName = pMyObject->GetFullName(pMyObject);
	if(strstr(pFullName,"NumberOfEntries"))
	{

#if defined(DEVICE_GATEWAY_ASSOCIATION_FEATURE)

     if(!strstr(pFullName,"ManageableDeviceNumberOfEntries"))
        {
            AnscTraceEntry(("Param '%s' cannot be set to Notification Active.\n", pFullName));
            AnscFreeMemory(pFullName);
            return  ANSC_STATUS_UNAPPLICABLE;
        }
#endif

             AnscTraceEntry(("Param '%s' cannot be set to Notification Active.\n", pFullName));
             AnscFreeMemory(pFullName);
             return  ANSC_STATUS_UNAPPLICABLE;               

	}
        AnscFreeMemory(pFullName);
    if ( iNotification == DSLH_CWMP_NOTIFICATION_active && pVarEntity->ParamDescr->NotifyStatus != DSLH_CWMP_NOTIFY_STATUS_configActive)
    {
        pFullName = pMyObject->GetFullName(pMyObject);

        AnscTraceEntry(("Param '%s' cannot be set to Notification Active.\n", pFullName));

        AnscFreeMemory(pFullName);

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if( pMyObject->Notification == iNotification)
    {
        return ANSC_STATUS_SUCCESS;
    }

    if ( (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_passive) ||
         (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_active ) )
    {
        if((iNotification == DSLH_CWMP_NOTIFICATION_passive) ||
          (iNotification == DSLH_CWMP_NOTIFICATION_active ) )
        {
            pMyObject->Notification = iNotification;

            /* Bin updated in Plugfest on 06/27/2007
             *
             * Since it's already in the monitor queue and the monitor parameter will retrieve
             * the notification state from VarRecord directly, we don't need to
             * do anything and exit.
             */

            return ANSC_STATUS_SUCCESS;
        }

            pDslhMprIf->DelNotifyParam
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }

    pMyObject->Notification = iNotification;

    /*
     * Whenever a parameter change is sent in the Inform message due to a non-zero Notification
     * setting, the Event code "4 VALUE CHANGE" MUST be included in the list of events. The CPE may
     * return a "notification request rejected" error if an attempt is made to set notification on
     * a parameter deemed inappropriate for Active Notification.
     */
    if ( (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_passive) ||
         (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_active ) )
    {
        if ( (pVarEntity->DataType     == DSLH_CWMP_DATA_TYPE_string) &&
             (pVarEntity->FormatValue1 >= 8192                      ) )
        {
            /*
             * The parameter value is too big. The backend should initiate the Notification when
             * the parameter value is modified.
             */
        }
        else
        {
        if(pMyObject->RequesterID != 0)
        CcspTraceInfo(("<<< %s pMyObject->RequesterID %lu >>>\n",__FUNCTION__,pMyObject->RequesterID));

        if(pMyObject->RequesterID == DSLH_MPA_ACCESS_CONTROL_ACS) //If notification is set from TR69, then only push into queue
        {
            pDslhMprIf->RegNotifyParam
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_active)
                );
        }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhVarroGetAccessList
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

char*
DslhVarroGetAccessList
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    return  pMyObject->AccessList;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetAccessList
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessList
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessList
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetAccessList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessList
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject    = (PDSLH_VAR_RECORD_OBJECT)hThisObject;

    if ( pMyObject->AccessList )
    {
        AnscFreeMemory(pMyObject->AccessList);

        pMyObject->AccessList = NULL;
    }

    /*
     *  By default, we assigned NULL to AccessList which means "subscriber"
     *
     *  Then if SetAccessList to an empty string, it means no access, we need
     *  to set an empty string instead of NULL.
     */

    if( pAccessList == NULL)
    {
        pMyObject->AccessList = AnscCloneString("");
    }
    else
    {
        pMyObject->AccessList = AnscCloneString(pAccessList);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroReset
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
DslhVarroReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)pDslhCpeController);
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFoParameter;*/

    if ( (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_passive) ||
         (pMyObject->Notification == DSLH_CWMP_NOTIFICATION_active ) )
    {
            pDslhMprIf->DelNotifyParam
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }

    if ( pMyObject->AccessList )
    {
        AnscFreeMemory(pMyObject->AccessList);

        pMyObject->AccessList = NULL;
    }

/*
    if ( pPoamIrepFoParameter )
    {
        pPoamIrepFoParameter->Remove((ANSC_HANDLE)pPoamIrepFoParameter);
    }

    pMyObject->hIrepFoParameter = (ANSC_HANDLE)NULL;
*/

    if ( pMyObject->TempParamValue )
    {
        SlapFreeVariable(pMyObject->TempParamValue);

        pMyObject->TempParamValue = NULL;
    }

    if ( pMyObject->TempParamValueTrans )
    {
        if ( (pMyObject->TempParamValueTrans->Syntax == SLAP_VAR_SYNTAX_handle) && pMyObject->TempParamValueTrans->Variant.varHandle )
        {
            AnscFreeMemory(pMyObject->TempParamValueTrans->Variant.varHandle);
        }

        SlapFreeVariable(pMyObject->TempParamValueTrans);

        pMyObject->TempParamValueTrans = NULL;
    }

    if ( pMyObject->OldParamValue )
    {
        SlapFreeVariable(pMyObject->OldParamValue);

        pMyObject->OldParamValue = NULL;
    }

    pMyObject->bAlreadySet = FALSE;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhVarroIsRecordWritable

            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check the record is writable or not;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     true or false;

**********************************************************************/
BOOL
DslhVarroIsRecordWritable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
	PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
	ULONG							uValue				 = 0;

	uValue = pObjController->CheckParamWritability(pObjController, pVarEntity->ParamDescr->Name);

	if( DSLH_OBJCONTROLLER_PARAM_WRITE_NOTCHANGE == uValue)
	{
		return pVarEntity->ParamDescr->bWritable;
	}

	if( DSLH_OBJCONTROLLER_PARAM_WRITE_NO == uValue)
	{
		return FALSE;
	}

	return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVarroNotifyValueChanged
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state value.

**********************************************************************/

ANSC_STATUS
DslhVarroNotifyValueChanged
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject           = (PDSLH_VAR_RECORD_OBJECT      )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT      )pMyObject->hDslhObjRecord;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT  )pObjRecord->hDslhCpeController;
    PCCC_MBI_INTERFACE              pDslhMbiIf          = (PCCC_MBI_INTERFACE           )pDslhCpeController->GetInterfaceByName((ANSC_HANDLE)pDslhCpeController, CCC_MBI_INTERFACE_NAME);
    PSLAP_VARIABLE                  pNewValue           = (PSLAP_VARIABLE               )NULL;
    char*                           pParamFullName      = NULL;
    char*                           pAccessList         = NULL;

    if ( !pDslhMbiIf )
    {
        AnscTraceError(("DslhVarroNotifyValueChanged: pDslhMbiIf is NULL\n"));

        return ANSC_STATUS_FAILURE;
    }

    pParamFullName = pMyObject->GetFullName((ANSC_HANDLE)pMyObject);

    if ( pMyObject->TempParamValue )
    {
        SlapAllocVariable(pNewValue);

        if ( !pNewValue )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            return returnStatus;
        }
        
        SlapCloneVariable(pMyObject->TempParamValue, pNewValue);
    }
    else
    {
        /* New value has to be present all the time. Get current value */
        pNewValue = pMyObject->GetValue((ANSC_HANDLE)pMyObject);
    }
	if(pNewValue != NULL)
	{
		pNewValue->ReqSenderID = pMyObject->ReqSenderID;
	}

    pAccessList = pMyObject->AccessList ? AnscCloneString(pMyObject->AccessList) : NULL;

    returnStatus =
        pDslhMbiIf->SendParameterValueChangeSignal
            (
                (void*)pDslhMbiIf->hOwnerContext,
                pParamFullName,
                pMyObject->OldParamValue,
                pNewValue,
                pAccessList
            );

    if ( pNewValue )
    {
        SlapFreeVariable(pNewValue);
    }

    if ( pParamFullName )
    {
        AnscFreeMemory(pParamFullName);
    }

    if ( pAccessList )
    {
        AnscFreeMemory(pAccessList);
    }

    return returnStatus;
}
