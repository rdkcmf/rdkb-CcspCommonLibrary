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

    module:	dslh_varro_config.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-config functions
        of the Dslh Var Record Object.

        *   DslhVarroLoadConfig
        *   DslhVarroSaveConfig

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

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroLoadConfig
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to load configuration from the system
        registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroLoadConfig
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pDslhCpeController->GetDslhMprIf((ANSC_HANDLE)pDslhCpeController);

    if ( pVarEntity->ParamDescr->NotifyStatus == DSLH_CWMP_NOTIFY_STATUS_alwaysOn )
    {
        pMyObject->Notification = DSLH_CWMP_NOTIFICATION_active;
    }
    else if ( pVarEntity->ParamDescr->NotifyStatus == DSLH_CWMP_NOTIFY_STATUS_alwaysOff )
    {
        pMyObject->Notification = DSLH_CWMP_NOTIFICATION_off;
    }

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

    pMyObject->AccessList = NULL;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSaveConfig
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save configuration into the system
        registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSaveConfig
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

#if 0
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDatabase  = (PPOAM_IREP_FOLDER_OBJECT   )pObjRecord->hIrepFoWmpDatabase;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFoParameter;
    PSLAP_VARIABLE                  pSlapVariable        = (PSLAP_VARIABLE             )NULL;
    PCHAR                           pFullName            = NULL;

    if ( !pPoamIrepFoParameter )
    {
        if( DSLH_CWMP_NOTIFICATION_off == pMyObject->Notification && pMyObject->AccessList == NULL) 
        {
            /* no special attribute, we don't need to save anything */
            return ANSC_STATUS_SUCCESS;
        }

        pFullName = pMyObject->GetFullName(pMyObject);

        if( pFullName == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }

        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT)
            pPoamIrepFoDatabase->AddFolder(pPoamIrepFoDatabase, pFullName, 0);

        AnscFreeMemory(pFullName);

        if( !pPoamIrepFoParameter)
        {
            return  ANSC_STATUS_NOT_READY;
        }

        pMyObject->hIrepFoParameter = pPoamIrepFoParameter;
    }

    pPoamIrepFoParameter->EnableFileSync((ANSC_HANDLE)pPoamIrepFoParameter, FALSE);

    SlapAllocVariable(pSlapVariable);

    if ( !pSlapVariable )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }

    if ( DSLH_CWMP_NOTIFICATION_off == pMyObject->Notification )
    {
        pPoamIrepFoParameter->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoParameter,
                    DSLH_VARRO_RR_NAME_Notification
                );
    }
    else
    {
        pSlapVariable->Syntax         = SLAP_VAR_SYNTAX_int;
        pSlapVariable->Variant.varInt = pMyObject->Notification;

        returnStatus =
            pPoamIrepFoParameter->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoParameter,
                    DSLH_VARRO_RR_NAME_Notification,
                    SYS_REP_RECORD_TYPE_SINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    if ( pMyObject->AccessList == NULL)
    {
        pPoamIrepFoParameter->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoParameter,
                    DSLH_VARRO_RR_NAME_AccessList
                );
    }
    else
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
        pSlapVariable->Variant.varString = AnscCloneString(pMyObject->AccessList);

        returnStatus =
            pPoamIrepFoParameter->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoParameter,
                    DSLH_VARRO_RR_NAME_AccessList,
                    SYS_REP_RECORD_TYPE_ASTR,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

EXIT1:

    pPoamIrepFoParameter->EnableFileSync((ANSC_HANDLE)pPoamIrepFoParameter, TRUE);
#endif

    return  returnStatus;
}
