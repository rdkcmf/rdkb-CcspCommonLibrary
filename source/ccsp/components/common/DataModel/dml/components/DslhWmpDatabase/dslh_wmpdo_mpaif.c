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

    module:	dslh_wmpdo_mpaif.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoMpaLockWriteAccess
        *   DslhWmpdoMpaUnlockWriteAccess
        *   DslhWmpdoMpaSetParameterValues
        *   DslhWmpdoMpaGetParameterValues
        *   DslhWmpdoMpaGetParameterNames
        *   DslhWmpdoMpaSetParameterAttributes
        *   DslhWmpdoMpaGetParameterAttributes 
        *   DslhWmpdoMpaAddObject
        *   DslhWmpdoMpaDeleteObject

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
#include "ccsp_base_api.h"
#include "poam_irepfo_interface.h"
#include "dslh_dmagnt_interface.h"
#include "sys_definitions.h"
#include "ccsp_psm_helper.h"

extern ULONG    g_lastWriteEntity;

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhWmpdoMpaLockWriteAccess
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity
            );

    description:

        This function is called to lock the write access on the
        parameter database. If it succeeds, no other entities can write
        into the database until UnlockWriteAccess() is called.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the entity needs the write access.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhWmpdoMpaLockWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty      = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    BOOL                            bWriteLocked   = FALSE;

    AnscAcquireLock(&pMyObject->MpaWriteLock);

    if ( pMyObject->bMpaWriteLocked )
    {
        if ( AnscEqualString
                (
                    pAccessEntity,
                    pMyObject->LockedEntity,
                    TRUE
                ) )
        {
            bWriteLocked = TRUE;
        }
        else
        {
            bWriteLocked = FALSE;
        }

        goto  EXIT1;
    }
    else if ( pMyObject->LockedEntity )
    {
        AnscFreeMemory(pMyObject->LockedEntity);

        pMyObject->LockedEntity = NULL;
    }

    bWriteLocked               = TRUE;
    pMyObject->bMpaWriteLocked = TRUE;
    pMyObject->LockedEntity    = AnscCloneString(pAccessEntity);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseLock(&pMyObject->MpaWriteLock);

    return  bWriteLocked;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaUnlockWriteAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the write lock previously
        acquired by calling LockWriteAccess().

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaUnlockWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty      = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;

    AnscAcquireLock(&pMyObject->MpaWriteLock);

    if ( !pMyObject->bMpaWriteLocked )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;
        goto  EXIT1;
    }
    else if ( pMyObject->LockedEntity )
    {
        AnscFreeMemory(pMyObject->LockedEntity);

        pMyObject->LockedEntity = NULL;
    }

    pMyObject->bMpaWriteLocked = FALSE;

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscReleaseLock(&pMyObject->MpaWriteLock);

    return  returnStatus;
}

/**
 * @brief Load Global User Monitor Parameters from PSM
 *        EnableMonitorUserChangedParams -> bGlobalMonitor (bool)
 *        UserOpComponents -> uMonitorComponents (bitmask)
 * @param pDmlAgent - pointer to DataModel Agent
 */
static void LoadUserMonitorParameters
    (
        PDSLH_DATAMODEL_AGENT_OBJECT pDmlAgent
    )
{
    INT            ret                               = CCSP_SUCCESS;
    UINT           Type                              = 0;
    SLAP_VARIABLE  Value                             = {0};

    SlapInitVariable(&Value);

    ret = PSM_Get_Record_Value(COSAGetMessageBusHandle(), COSAGetSubsystemPrefix2(), 
                               CCSP_USER_CHANGED_MONITOR_PARAM, &Type, &Value); 
    if (ret == CCSP_SUCCESS)
    {
        pDmlAgent->bGlobalMonitor = Value.Variant.varBool;
        SlapCleanVariable(&Value);
    }

    ret = PSM_Get_Record_Value(COSAGetMessageBusHandle(), COSAGetSubsystemPrefix2(),
                               CCSP_USER_COMPONENTS_PARAM, &Type, &Value); 
    if (ret == CCSP_SUCCESS)
    {
        pDmlAgent->uMonitorComponents = Value.Variant.varUint32;
        SlapCleanVariable(&Value);
    }
}

/**
 * @brief Load UserChanged Flag from PSM
 *        Load UserChanged.paramName from PSM
 * @param pDmlAgent   pointer to DataModel Agent
 * @param paramName   Parameter full name (including object prefix and index)
 * @return user changed flag from PSM, or
 *         0 if user changed flag not available in PSM
 */
static BOOL LoadUserChangeFlag
    (
        PDSLH_DATAMODEL_AGENT_OBJECT pDmlAgent,
        const char* paramName
    )
{
    // Load User Changed Flag from persistent storage for Parameters that have user change monitoring enabled
    INT            ret                               = CCSP_SUCCESS;
    CHAR           fullName[256]                     = {0};
    UINT           Type                              = 0;
    SLAP_VARIABLE  Value                             = {0};

    /* Get the full name */
    _ansc_sprintf( fullName, "UserChanged.%s", paramName);

    ret = PSM_Get_Record_Value(COSAGetMessageBusHandle(), COSAGetSubsystemPrefix2(), fullName, &Type, &Value); 

    if ( ret == CCSP_SUCCESS )
    {
        BOOL retVal = Value.Variant.varBool;
        SlapCleanVariable(&Value);
        return retVal;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Save UserChanged Flag to PSM
 *        Save UserChanged.paramName from PSM
 * @param pDmlAgent   pointer to DataModel Agent
 * @param paramName   Parameter full name (including object prefix and index)
 */
static void SaveUserChangeFlag
    (
        PDSLH_DATAMODEL_AGENT_OBJECT pDmlAgent,
        const char* paramName
    )
{
    // Save User Changed flag to persistent storage
    PSLAP_VARIABLE pSlapVariable = (PSLAP_VARIABLE)NULL;

    SlapAllocVariable(pSlapVariable);
    if (pSlapVariable)
    {
        INT         ret   = CCSP_SUCCESS;
        CHAR        fullName[IREPFO_FULLNAME_LENGTH]    = {0};

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
        pSlapVariable->Variant.varBool   = 1;

        /* Get the full name */
        _ansc_sprintf( fullName, "UserChanged.%s", paramName);

        // write to PSM
        ret = PSM_Set_Record_Value(COSAGetMessageBusHandle(), COSAGetSubsystemPrefix2(),
                                  fullName, SYS_REP_RECORD_TYPE_BOOL, pSlapVariable);

        SlapFreeVariable(pSlapVariable);
    }
}


/**
 * @brief Check User Changed Flag
 *        Called when a paramter is set, to prevent override of user
 *        changed flag. If a parameter is set to preserve user changed
 *        value, (with <monitor>true</monitor> in its XML definition),
 *        after user has made the change this parameter, it is no longer
 *        overridable by service provider.
 * @param pMyObject  - pointer to CWMP Data Base
 * @param writeID    - 0x00000001 - WEBUI
 *                   - 0x00000002 - ACS
 *                   - 0x00000004 - SNMP
 *                   - 0x00000008 - CLI
 * @param pVarRecord - Paremter Value record in Data Base
 * @param pParamValue - New parameter value to be set
 * @return CCSP_SUCCESS - it is ok to set the parameter
 * @rerurn CCSP_ERR_NOT_WRITABLE - it is not ok to set the parameter
 */
static ANSC_STATUS CheckUserChangedFlag
    (
    PDSLH_WMP_DATABASE_OBJECT       pMyObject,
    ULONG                           writeID,
    PDSLH_VAR_RECORD_OBJECT         pVarRecord,
    PDSLH_CWMP_PARAM_VALUE          pParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT  pCpeController = (PDSLH_CPE_CONTROLLER_OBJECT )pMyObject->hDslhCpeController;
    PDSLH_DATAMODEL_AGENT_OBJECT pDmlAgent      = (PDSLH_DATAMODEL_AGENT_OBJECT)pCpeController->hDslhDataModelAgent;

    if (!pDmlAgent->bMonitorParamLoaded)
    {
        LoadUserMonitorParameters(pDmlAgent);
        pDmlAgent->bMonitorParamLoaded = 1;
    }

    if (pDmlAgent->bGlobalMonitor)
    {
        if (!pVarRecord->bUserChangeFlagLoaded)
        {
            pVarRecord->bUserChanged = LoadUserChangeFlag(pDmlAgent, pParamValue->Name);
            pVarRecord->bUserChangeFlagLoaded = 1;
        }

        if (pVarRecord->bUserChanged)
        {
            if (!(writeID & pDmlAgent->uMonitorComponents)) // not from UI
            {
                AnscTraceError(("!!! Set parameter for %s denied due to user change flag !!!", pParamValue->Name));

                return CCSP_ERR_NOT_WRITABLE;
            }
        }
        else // not user changed
        {
            if (writeID & pDmlAgent->uMonitorComponents) // from UI
            {
                BOOL bValueMatched = FALSE;
                        
                PSLAP_VARIABLE pCurParamValue = pVarRecord->GetValue((ANSC_HANDLE)pVarRecord);
                if (pCurParamValue)
                {
                    SlapEqualVariables(pParamValue->Value, pCurParamValue, bValueMatched);
                    SlapFreeVariable(pCurParamValue);
                }

                if (!bValueMatched) // different value
                {
                    pVarRecord->bUserChanged = 1;

                    SaveUserChangeFlag(pDmlAgent, pParamValue->Name);
                }
            }
        }
    }

    return CCSP_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaSetParameterValues
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                void*                       pParamValueArray,
                ULONG                       ulArraySize,
                int                         sessionId,
                ULONG                       writeID,
                BOOL                        bCommit,
                char**                      ppInvalidParameterName
            );

    description:

        This function is called to configure the value of the
        specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                void*                       pParamValueArray
                Specifies the array of ParameterValue structures.

                ULONG                       ulArraySize
                Specifies the number of elements in the
                pParamValueArray.

                int*                        piStatus
                Returns the status of the SET operation: 0 = Parameter
                changes have been validated and applied; 1 = Parameter
                changes have been validated and committed, but not yet
                applied.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaSetParameterValues
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        void*                       pParamValueArray,
        ULONG                       ulArraySize,
        int*                        piStatus,
        int                         sessionId,
        ULONG                       writeID,
        BOOL                        bCommit,
        char**                      ppInvalidParameterName
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject            = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty            = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_CWMP_PARAM_VALUE          pParameterValueArray = (PDSLH_CWMP_PARAM_VALUE     )pParamValueArray;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord           = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf           = (PDSLH_MPA_INTERFACE        )pMyObject->hDslhMpaIf;
    ULONG                           ulParameterCount     = (ULONG                      )0;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT*        pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
    PDSLH_VAR_RECORD_OBJECT*        pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    ULONG                           ulObjectCount        = (ULONG                      )0;
    char*                           pFaultParamName      = (char*                      )NULL;
    BOOL                            bFaultEncountered    = (BOOL                       )FALSE;
    BOOL                            bRebootNeeded        = (BOOL                       )FALSE;
    ULONG                           i                    = 0;
    BOOL                            bFromAcs             = FALSE;
    BOOL                            bFromSnmp            = FALSE;
    ULONG                           dataType             = 0;

    bFromAcs     = (writeID == DSLH_MPA_ACCESS_CONTROL_ACS) && AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);
    bFromSnmp    = (writeID == DSLH_MPA_ACCESS_CONTROL_SNMP);
    *piStatus    = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    if ( (ulArraySize == 0) || !pParameterValueArray )
    {
        *piStatus    = 0;
        returnStatus = ANSC_STATUS_SUCCESS;

        goto  EXIT0;
    }


    pMyObject->SessionID       = sessionId;


    /*
     * The SetParameterValues() operation is divided into three steps: the first round we validate
     * each name/value pair contained in the ParamValueArray. This is done by performing
     *
     *      - Standard sanity checks on the name and value;
     *      - Calling pVarRecord->TstValue() to validate the value
     */
    for ( i = 0; i < ulArraySize; i++ )
    {
        if ( !pParameterValueArray[i].Name )
        {
            bFaultEncountered = TRUE;

			returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

            break;
        }
        else if ( !pParameterValueArray[i].Value )
        {
            bFaultEncountered = TRUE;
            
            *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

			returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;


            break;
        }

        pVarRecord =
            (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pParameterValueArray[i].Name
                );

        if ( !pVarRecord )
        {
            bFaultEncountered = TRUE;

            *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

			returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            break;
        }
        else
        {
            pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;
        }

		if( pVarEntity)
		{
			if(pVarEntity->ParamDescr->bInvisible && bFromAcs)
			{
				/* If it's a hidden parameter and it's from ACS, we will reject it */

				bFaultEncountered = TRUE;
				*ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

				returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

				break;
			}

			/* Bin added to check data type */
			dataType = pVarEntity->DataType;

			if( dataType == DSLH_CWMP_DATA_TYPE_boolean)
			{
				if( pParameterValueArray[i].Value->Syntax != SLAP_VAR_SYNTAX_bool)
				{
					bFaultEncountered = TRUE;

					*ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);
            
					returnStatus = CCSP_ERR_INVALID_PARAMETER_TYPE;

					break;
				}
			}
			else if( dataType == DSLH_CWMP_DATA_TYPE_int)
			{
				if( pParameterValueArray[i].Value->Syntax != SLAP_VAR_SYNTAX_int)
				{
					bFaultEncountered = TRUE;

					*ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);
            
					returnStatus = CCSP_ERR_INVALID_PARAMETER_TYPE;

					break;
				}
			}
			else if( dataType == DSLH_CWMP_DATA_TYPE_unsignedInt)
			{
				if( pParameterValueArray[i].Value->Syntax != SLAP_VAR_SYNTAX_uint32)
				{
					bFaultEncountered = TRUE;

					*ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);
            
					returnStatus = CCSP_ERR_INVALID_PARAMETER_TYPE;

					break;
				}
			}
			else
			{
				if( pParameterValueArray[i].Value->Syntax != SLAP_VAR_SYNTAX_string)
				{
					bFaultEncountered = TRUE;

					*ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);
            
					returnStatus = CCSP_ERR_INVALID_PARAMETER_TYPE;

					break;
				}
			}
		}

        if ( !pVarRecord->IsRecordWritable(pVarRecord))
        {
            bFaultEncountered = TRUE;

            *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

			returnStatus = CCSP_ERR_NOT_WRITABLE;

            break;
        }
        else if ( !pVarRecord->TstValue((ANSC_HANDLE)pVarRecord, pParameterValueArray[i].Value) )
        {
            bFaultEncountered = TRUE;

            *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

			returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;

            break;
        }

        /* Access bit mask check */
        if ( TRUE )
        {
            if ( 
                   ( writeID != 0xFFFFFFFF    ) &&
                   ( pVarRecord->AccessList ) &&
                   ( writeID != AccessListToAccessControlBitMask(pVarRecord->AccessList) )
               )
            {
                AnscTraceError(("!!! Set parameter for %s denied due to access control !!!", pParameterValueArray[i].Name));
                bFaultEncountered = TRUE;

	            *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);
	
   			    returnStatus = CCSP_ERR_NOT_WRITABLE;

	            break;
            }
        }

		pVarRecord->ReqSenderID = writeID;
        /* User Changed Flag Check */
        if (pVarEntity && pVarEntity->ParamDescr->bMonitor)
        {
            returnStatus = CheckUserChangedFlag(pMyObject, writeID, pVarRecord, &pParameterValueArray[i]);

            if (returnStatus != CCSP_SUCCESS)
            {
                bFaultEncountered = TRUE;
                *ppInvalidParameterName = AnscCloneString(pParameterValueArray[i].Name);

                break;
            }
        }

        pVarRecord->SetValue((ANSC_HANDLE)pVarRecord, pParameterValueArray[i].Value);

        pVarRecord->SaveOldValue((ANSC_HANDLE)pVarRecord);

        pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord;

        if ( (pMyObject->hVarRecordArray == NULL) || (pMyObject->hObjRecordArray == NULL) )
        {
            /* init the variable and object record arrays; */ 
            pMyObject->InitObjVarArray(hThisObject);

            pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
            pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
        }

        ulParameterCount =
            pMyObject->AddVarRecordIntoArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pVarRecord
                );

        ulObjectCount    =
            pMyObject->AddObjRecordIntoArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pObjRecord
                );
    }

    if ( bFaultEncountered )
    {
        /* returnStatus = ANSC_STATUS_BAD_PARAMETER; */

        goto  EXIT2;
    }
    
    /*
     * In the second round, we perform object-level validation by going through each ObjRecord
     * object in the pObjRecordArray created in step1 and calling pObjRecord->VerifyChanges() on
     * each object. Note that one ObjRecord may contain multiple VarRecord objects.
     */
    for ( i = 0; i < ulObjectCount; i++ )
    {        
        pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecordArray[i];

        if ( pObjRecord != NULL)
        {
            if ( !pObjRecord->VerifyChanges((ANSC_HANDLE)pObjRecord, &pFaultParamName) )
            {
                bFaultEncountered = TRUE;

                if ( !pFaultParamName )
                {
                    AnscTraceEntry(("Failed to VerifyChange in '%s' while no 'FaultParamName' returned....\n", pObjRecord->FullName));
                }
                else
                {
                    *ppInvalidParameterName = pFaultParamName;
                }

				break;
            }
        }
    }

    if ( bFaultEncountered )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_VALUE;

        goto  EXIT2;
    }

    /*
     * In the third round, we first commit the parameter changes at the object level by calling
     * CommitChanges() of each ObjRecord; and then we commit the change for each individual para-
     * meter by calling CommitChange() of each VarRecord.
     */

    if ( bCommit == TRUE )
    {
        returnStatus = 
            pDslhMpaIf->SetCommit
                (
                    (ANSC_HANDLE)pDslhMpaIf->hOwnerContext,
                    pAccessEntity,
                    piStatus,
                    sessionId,
                    writeID,
                    TRUE
                );

        pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
        pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
        ulObjectCount        = pMyObject->ulObjRecordCount; 
        ulParameterCount     = pMyObject->ulVarRecordCount; 

        /* Currently we don't handle the returnStatus of SetCommit */
    
        returnStatus = ANSC_STATUS_SUCCESS;

        goto  EXIT1;
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;

        /* keep the var and obj record arrays and wait for the commit action */

        goto EXIT0;
    }
    


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pVarRecordArray )
    {
        /*
         * Rollback any changes we may have made...
         */
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            for ( i = 0; i < ulParameterCount; i++ )
            {
                pVarRecord = (PDSLH_VAR_RECORD_OBJECT)pVarRecordArray[i];

                if ( pVarRecord )
                {
                    pVarRecord->CancelChange((ANSC_HANDLE)pVarRecord);
                }
            }
        }
    }

    /* We just roll back when access origin is not from SNMP and invalid error */
    if ( !bFromSnmp )
    {
        if ( pObjRecordArray )
        {
            /*
             * Rollback any changes we may have made...
             */
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                for ( i = 0; i < ulObjectCount; i++ )
                {
                    pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecordArray[i];

                    if ( pObjRecord )
                    {
                        pObjRecord->CancelChanges((ANSC_HANDLE)pObjRecord);
                    }
                }
            }
        }
    }

EXIT1:

    pMyObject->ResetObjVarArray(hThisObject);

EXIT0:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaSetCommit
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                int*                        piStatus,
                int                         sessionId,
                ULONG                       writeID,
                BOOL                        bCommit
            )

    description:

        This function is called to commit the value of the
        specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                int*                        piStatus
                Returns the status of the SET operation: 0 = Parameter
                changes have been validated and applied; 1 = Parameter
                changes have been validated and committed, but not yet
                applied.

                int                         sessionId
                Idetification of the specific value set session.

                ULONG                       writeID
                Indicate the write permission.

                BOOL                        bCommit
                [TRUE]  Apply the changes.
                [FALSE] Rollback the temp values set when one of 
                        the Validation fails.


    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaSetCommit
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        int*                        piStatus,
        int                         sessionId,
        ULONG                       writeID,
        BOOL                        bCommit
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject            = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty            = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord           = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_RECORD_OBJECT*        pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    ULONG                           ulParameterCount     = (ULONG                      )pMyObject->ulVarRecordCount;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT*        pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
    ULONG                           ulObjectCount        = (ULONG                      )pMyObject->ulObjRecordCount;
    char*                           pFaultParamName      = (char*                      )NULL;
    BOOL                            bFaultEncountered    = (BOOL                       )FALSE;
    BOOL                            bRebootNeeded        = (BOOL                       )FALSE;
    ULONG                           i                    = 0;
    BOOL                            bFromAcs             = FALSE;

    bFromAcs     = (writeID == DSLH_MPA_ACCESS_CONTROL_ACS) && AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);
    *piStatus    = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    if ( pMyObject->SessionID != (ULONG)sessionId )
    {
        AnscTraceError(("!!! SessionID does not match !!!\n"));

        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT1;
    }

    if ( bCommit == TRUE )
    {
        /* Apply the pending changes */

        if ( !pVarRecordArray || !pObjRecordArray )
        {
            AnscTraceError(("!!! No Var/Obj to be applied !!!\n"));

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT2;
        }

        for ( i = 0; i < ulObjectCount; i++ )
        {
            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecordArray[i];

            if ( !pObjRecord )
            {
                /*
                 * Impossible, no need for error handling...
                 */
            }
            else
            {
                if( pObjRecord->CommitChanges((ANSC_HANDLE)pObjRecord) != ANSC_STATUS_SUCCESS)
			    {
				    AnscTrace("Failed to 'CommitChanges' in record %s\n", pObjRecord->FullName);
			    }
                else
                {
                    g_lastWriteEntity = writeID;
                }

                /* We need mark  controller to be committed. */
                if ( TRUE )
                {
                    pObjController               = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
                    pObjController->bCommitted   = TRUE;
                }

                pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
                pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
                ulObjectCount        = pMyObject->ulObjRecordCount; 
                ulParameterCount     = pMyObject->ulVarRecordCount; 
            }
        }

        for ( i = 0; i < ulParameterCount; i++ )
        {
            pVarRecord = (PDSLH_VAR_RECORD_OBJECT)pVarRecordArray[i];

            if ( !pVarRecord )
            {
                continue;
            }

            pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

            pVarRecord->CommitChange((ANSC_HANDLE)pVarRecord, bFromAcs);

            if ( pVarEntity->ParamDescr->bNeedReboot )
            {
                bRebootNeeded = TRUE;
            }

            pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )pMyObject->hObjRecordArray;
            pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )pMyObject->hVarRecordArray;
            ulObjectCount        = pMyObject->ulObjRecordCount; 
            ulParameterCount     = pMyObject->ulVarRecordCount; 
        }

        *piStatus    = bRebootNeeded? 1 : 0;
        returnStatus = ANSC_STATUS_SUCCESS;

        goto EXIT2;
    }
    else
    {
        if ( !pVarRecordArray || !pObjRecordArray )
        {
            AnscTraceError(("!!! No Var/Obj will be rolled back !!!\n"));

            returnStatus = ANSC_STATUS_SUCCESS;

            goto EXIT2;
        }

        /* Rollback the temp changes */

        if ( pVarRecordArray )
        {
            /*
             * Rollback any changes we may have made...
             */
            
            for ( i = 0; i < ulParameterCount; i++ )
            {
                pVarRecord = (PDSLH_VAR_RECORD_OBJECT)pVarRecordArray[i];

                if ( pVarRecord )
                {
                    pVarRecord->CancelChange((ANSC_HANDLE)pVarRecord);
                }
            }

            AnscFreeMemory(pVarRecordArray);

            pMyObject->hVarRecordArray  = NULL;
            pMyObject->ulVarRecordCount = 0;
        }

        if ( pObjRecordArray )
        {
            /*
             * Rollback any changes we may have made...
             */
            
            for ( i = 0; i < ulObjectCount; i++ )
            {
                pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pObjRecordArray[i];

                if ( pObjRecord )
                {
                    pObjRecord->CancelChanges((ANSC_HANDLE)pObjRecord);
                }
            }

            AnscFreeMemory(pObjRecordArray);

            pMyObject->hObjRecordArray  = NULL;
            pMyObject->ulObjRecordCount = 0;
        }

        *piStatus    = 0;
        returnStatus = ANSC_STATUS_SUCCESS;

        goto EXIT1;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pMyObject->hVarRecordArray )
    {
        AnscFreeMemory(pMyObject->hVarRecordArray);
    }

    pMyObject->hVarRecordArray  = NULL;
    pMyObject->ulVarRecordCount = 0;

    if ( pMyObject->hObjRecordArray )
    {
        AnscFreeMemory(pMyObject->hObjRecordArray);
    }

    pMyObject->hObjRecordArray  = NULL;
    pMyObject->ulObjRecordCount = 0;

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaGetParameterValues
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                SLAP_STRING_ARRAY*          pParamNameArray,
				ULONG						uMaxEntry,
                void**                      ppParamValueArray,
                PULONG                      pulArraySize
            );

    description:

        This function is called to retrieve the value of the
        specified parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

				ULONG						uMaxEntry,
				The maxi number of entries allowed;

                SLAP_STRING_ARRAY*          pParamNameArray
                Specifies the name of the parameters whose values are
                to be returned.

                void**                      ppParamValueArray
                Returns the array of ParameterValue structures.

                PULONG                      pulArraySize
                Specifies the number of elements in the returned
                *ppParamValueArray.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaGetParameterValues
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        SLAP_STRING_ARRAY*          pParamNameArray,
        ULONG                       uMaxEntry,
        void**                      ppParamValueArray,
        PULONG                      pulArraySize,
        unsigned int                writeID
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject            = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty            = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_CWMP_SOAP_FAULT           pCwmpSoapFault       = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    PDSLH_CWMP_PARAM_VALUE          pParameterValueArray = (PDSLH_CWMP_PARAM_VALUE     )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf           = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    ULONG                           ulParameterCount     = (ULONG                      )0;
    ULONG                           ulParameterIndex     = (ULONG                      )0;
    ULONG                           ulParamCopyCount     = (ULONG                      )0;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord           = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity           = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    ANSC_HANDLE*                    phAnyRecordArray     = (ANSC_HANDLE*               )NULL;
    SLAP_VARIABLE*                  pParameterVar        = (SLAP_VARIABLE*             )NULL;
    ULONG                           i                    = 0;
    ULONG                           j                    = 0;
    BOOL                            bFromAcs             = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);
    ULONG                           ulParamCount         = (ULONG                      )0;
    ULONG                           ulObjectCount        = (ULONG                      )0;
    PDSLH_OBJ_RECORD_OBJECT*        pObjRecordArray      = (PDSLH_OBJ_RECORD_OBJECT*   )NULL;
    PDSLH_VAR_RECORD_OBJECT*        pVarRecordArray      = (PDSLH_VAR_RECORD_OBJECT*   )NULL;
    BOOL                            bExists              = FALSE;
    ULONG                           ulSameObj            = 0;
    char**                          ppNameArray          = NULL;
    PSLAP_VARIABLE*                 ppValueArray         = NULL;
    char *                          pString              = NULL;

    *ppParamValueArray = NULL;
    *pulArraySize      = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */

    if ( !pParamNameArray )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT2;
    }
    else if ( pParamNameArray->VarCount == 0 )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }
    else
    {
        phAnyRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pParamNameArray->VarCount);
    }

    if ( !phAnyRecordArray )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }

    /*
     * Keep records of VarRecord and ObjRecord for GetValues from parameter level. */
    pVarRecordArray = (PDSLH_VAR_RECORD_OBJECT*)AnscAllocateMemory(sizeof(PDSLH_VAR_RECORD_OBJECT) * pParamNameArray->VarCount);
    pObjRecordArray = (PDSLH_OBJ_RECORD_OBJECT*)AnscAllocateMemory(sizeof(PDSLH_OBJ_RECORD_OBJECT) * pParamNameArray->VarCount);

    if ( !pVarRecordArray  || !pObjRecordArray )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT2;
    }

    /*
     * The GetParameterValues() operation is divided into two steps: the first round we validate
     * each parameter name and calculate the total number of parameters that need to be returned.
     */
    for ( i = 0; i < pParamNameArray->VarCount; i++ )
    {
        /*
         * If a Parameter name argument is given as a partial path name, the request is to be inter-
         * preted as a request to return all the Parameters in the branch of the naming hierarchy
         * that shares the same prefix as the argument.
         */
        if ( !pParamNameArray->Array.arrayString[i] || (AnscSizeOfString(pParamNameArray->Array.arrayString[i]) == 0) )
        {
            pObjRecord          = pRootObjRecord;
            phAnyRecordArray[i] = (ANSC_HANDLE)pObjRecord;
            ulParameterCount   += pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, bFromAcs, FALSE );
        }
        else if ( DslhCwmpIsPartialName(pParamNameArray->Array.arrayString[i]) )
        {
            pObjRecord =
                (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pParamNameArray->Array.arrayString[i]
                    );

            if ( !pObjRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

            if(!pObjEntity || (pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs))
            {
                /* If it's a hidden parameter and it's from ACS, we will reject it */

                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            phAnyRecordArray[i] = (ANSC_HANDLE)pObjRecord;

            ulParameterCount += pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, bFromAcs, FALSE);
        }
        else
        {

            pVarRecord =
                (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pParamNameArray->Array.arrayString[i]
                    );

            if ( !pVarRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }
            else
            {
                pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

                if(!pVarEntity || (pVarEntity->ParamDescr->bInvisible && bFromAcs))
                {
                    /* If it's a hidden parameter and it's from ACS, we will reject it */

                    returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                    goto  EXIT3;
                }

                phAnyRecordArray[i] = (ANSC_HANDLE)pVarRecord;
            }

            ulParameterCount++;

            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord;

            pVarRecordArray[ulParamCount]  = pVarRecord;
            ulParamCount ++;

            /* add objRecord if not added yet */
            bExists = FALSE;
            for( j = 0; j < ulObjectCount; j ++)
            {
                if( pObjRecordArray[j] == pObjRecord)
                {
                    bExists = TRUE;

                    break;
                }
            }

            if( !bExists)
            {
                pObjRecordArray[ulObjectCount] = pObjRecord;
                ulObjectCount ++;
            }
        }
    }

    /*
     * In TR-069 Plugfests, there\ufffds a test case to trigger the 9004 Resources Exceeded fault by
     * retrieving information on the Entire Object Model in a single "GetParameterNames",
     * "GetParameterValues" or "GetParameterAttributes" call. As Bin suggested, to address this
     * issue, we should impose an upper limit on the maximum number of parameters that can be
     * returned in a single response. DSLH_WMPDO_MAX_PARAMS_IN_RESPONSE is defined for this purpose.
     * By default, it's set to 1000. This value should be adjusted based on the available DRAM on
     * the target platform.
     */
    if ( ulParameterCount > uMaxEntry )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        CcspTraceError(("Total parameter count exceeds %d !!\n", uMaxEntry));

        goto  EXIT3;
    }

    /*
     * In the second round, we allocate the pParamValueArray to be returned based on the total
     * parameter number calculated in the first round. We then copy the parameter name/value pairs
     * into this array.
     */
    ulParameterIndex     = 0;
    ulParamCopyCount     = 0;

    if (!ulParameterCount)
    {
        /*
         * If we didn't get any parameters, it is not an error,
         * just there is parameters/instances.
         *
        returnStatus = ANSC_STATUS_BAD_PARAMETER;
         */
        returnStatus = ANSC_STATUS_SUCCESS;

        goto  EXIT3;
    }

    pParameterValueArray = (PDSLH_CWMP_PARAM_VALUE)AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_VALUE) * ulParameterCount);

    if ( !pParameterValueArray )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT3;
    }

    for ( i = 0; i < pParamNameArray->VarCount; i++ )
    {
        if ( !pParamNameArray->Array.arrayString[i]                         ||
             (AnscSizeOfString(pParamNameArray->Array.arrayString[i]) == 0) ||
             DslhCwmpIsPartialName(pParamNameArray->Array.arrayString[i]) )
        {
            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)phAnyRecordArray[i];

            ulParamCopyCount = ulParameterCount - ulParameterIndex;
            returnStatus     =
                pObjRecord->GetAllParamValues
                    (
                        (ANSC_HANDLE)pObjRecord,
                        (ANSC_HANDLE)&pParameterValueArray[ulParameterIndex],
                        &ulParamCopyCount,
                        bFromAcs,
                        writeID
                    );

            ulParameterIndex += ulParamCopyCount;
        }
        else
        {
            /* Will be treated differently with GetBulkParamValue */
#if 0
            pVarRecord = (PDSLH_VAR_RECORD_OBJECT)phAnyRecordArray[i];

            pParameterValueArray[ulParameterIndex].Name  = AnscCloneString(pParamNameArray->Array.arrayString[i]);
            pParameterValueArray[ulParameterIndex].Value = (SLAP_VARIABLE*)pVarRecord->GetValue((ANSC_HANDLE)pVarRecord);

            ulParameterIndex++;
#endif
        }
    }

    /* Then get value from parameters */
    if( ulParamCount > 0)
    {
        /* create the array */
        ppNameArray     = (char**)AnscAllocateMemory(sizeof(char*) * ulParamCount);
        ppValueArray    = (PSLAP_VARIABLE*)AnscAllocateMemory(sizeof(PSLAP_VARIABLE) * ulParamCount);

        if( !ppNameArray || !ppValueArray)
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }

        /* take care of the parameters */
        for( i = 0; i < ulObjectCount; i ++)
        {
            pObjRecord = pObjRecordArray[i]; 
            ulSameObj  = 0;

            for( j = 0; j < ulParamCount; j ++)
            {
                pVarRecord = pVarRecordArray[j];

                if( pObjRecord == (PDSLH_OBJ_RECORD_OBJECT)pVarRecord->hDslhObjRecord)
                {
                    ppNameArray[ulSameObj]   = pVarRecord->GetLastName(pVarRecord);
                    ppValueArray[ulSameObj]  = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

                    if( ppValueArray[ulSameObj] == NULL)
                    {
                        break;
                    }

                    pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

                    ppValueArray[ulSameObj]->Name        = pVarRecord->GetFullName((ANSC_HANDLE)pVarRecord);
                    ppValueArray[ulSameObj]->ContentType = pVarEntity->ContentType;
                    ppValueArray[ulSameObj]->Syntax      = pVarEntity->Syntax;

                    ulSameObj ++;
                }
            }

            /* get bulk value */
            pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;

            if( pObjController != NULL)
            {
                returnStatus = pObjController->GetBulkParamValue(pObjController, ppNameArray, ppValueArray, ulSameObj);

                if( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    AnscTrace("Failed in ->GetBulkParamValue.\n");
                }
            }

            /* Check parameter value and change it to be 2(notReady)  Begining*/
            pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

            if ( ( writeID == DSLH_MPA_ACCESS_CONTROL_SNMP ) && 
                 ( pObjEntity->ObjDescr->bWritable == TRUE ) &&
                 ( pObjController->bCommitted == FALSE )        )
            {
                for( j = 0; j < ulSameObj; j ++)
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

            for( j = 0 ; j < ulSameObj; j ++)
            {
                /* copy the value back */
                pParameterValueArray[ulParameterIndex].Name  = ppValueArray[j]->Name;
                pParameterValueArray[ulParameterIndex].Value = ppValueArray[j];
                ppValueArray[j]->Name                        = NULL;

                ulParameterIndex ++;
            }
        }
    }

    /*
	AnscTrace("Total parameter count in 'GetParameterValues' == %d\n", ulParameterIndex);
    */

    *ppParamValueArray = pParameterValueArray;
    *pulArraySize      = ulParameterIndex;
    returnStatus       = ANSC_STATUS_SUCCESS; 

    goto  EXIT3;

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

#if 0
    /* We don't handle error status here 
     * Bin commented out this check on 08/10/2009
     */

EXIT4:

    /* release all the values we have */
    if ( pParameterValueArray )
    {
        for ( i = 0; i < ulParameterIndex; i++ )
        {
            DslhCwmpCleanParamValue((&pParameterValueArray[i]));
        }

        AnscFreeMemory(pParameterValueArray);
    }

    /* set the fault code to 9002 */
    returnStatus = ANSC_STATUS_INTERNAL_ERROR;

#endif

EXIT3:

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

    if ( pVarRecordArray )
    {
        AnscFreeMemory(pVarRecordArray);
    }

    if ( pObjRecordArray )
    {
        AnscFreeMemory(pObjRecordArray);
    }

    if ( phAnyRecordArray )
    {
        AnscFreeMemory(phAnyRecordArray);
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        goto  EXIT2;
    }

EXIT2:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaGetParameterNames
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pParamPath,
                BOOL                        bNextLevel,
                void**                      ppParamInfoArray,
                PULONG                      pulArraySize
            );

    description:

        This function is called to retrieve the list of the parameters
        supported by the CPE.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pParamPath
                Specifies a string containing either a complete
                parameter name, or a partial path name representing a
                subset of the name hierarchy.

                BOOL                        bNextLevel
                If false, the response lists the full path name of all
                parameters whose name begins with the string given by
                the ParameterPath argument. If true, the response lists
                only the partial path one level below the specified
                ParameterPath.

                void**                      ppParamInfoArray
                Returns the array of ParameterInfo structures.

                PULONG                      pulArraySize
                Specifies the number of elements in the returned
                *ppParamInfoArray.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaGetParameterNames
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamPath,
        BOOL                        bNextLevel,
        void**                      ppParamInfoArray,
        PULONG                      pulArraySize
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject           = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty           = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord      = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_CWMP_PARAM_INFO           pParameterInfoArray = (PDSLH_CWMP_PARAM_INFO      )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    ULONG                           ulParameterCount    = (ULONG                      )0;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord          = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity          = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    BOOL                            bTargetIsObject     = FALSE;
    ULONG                           i                   = 0;
    BOOL                            bFromAcs            = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);
    char*                           pRootObjName    = pDslhMprIf->GetRootObjName(pDslhMprIf->hOwnerContext);

    *ppParamInfoArray = NULL;
    *pulArraySize     = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */


    /*
     * The GetParameterNames() operation is divided into two steps: the first round we calculate
     * the total number of parameters/objects that need to be returned.
     */
    if ( !pParamPath || (AnscSizeOfString(pParamPath) == 0) )
    {
        pObjRecord = pRootObjRecord;
        
       #if 0 
            (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pRootObjName
                );

        if ( !pObjRecord )
        {
            returnStatus = ANSC_STATUS_BAD_PARAMETER;

            goto  EXIT1;
        }
       #endif
       
        bTargetIsObject = TRUE;

        bTargetIsObject = TRUE;
    }
    else if ( DslhCwmpIsPartialName(pParamPath) )
    {
        pObjRecord =
            (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pParamPath
                );

        if ( !pObjRecord )
        {
            returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            goto  EXIT1;
        }

        pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

        if(!pObjEntity || (pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs))
        {
            /* If it's a hidden parameter and it's from ACS, we will reject it */
            returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            goto  EXIT1;
        }

        bTargetIsObject = TRUE;
    }
    else
    {
        pVarRecord =
            (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pParamPath
                );

        if ( !pVarRecord )
        {
            returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            goto  EXIT1;
        }

        pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

        if(!pVarEntity || (pVarEntity->ParamDescr->bInvisible && bFromAcs))
        {
            /* If it's a hidden parameter and it's from ACS, we will reject it */
            returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            goto  EXIT1;
        }

        bTargetIsObject = FALSE;
    }

    if ( bTargetIsObject )
    {
        if ( bNextLevel )
        {
            ulParameterCount = pObjRecord->GetObjRecordCount((ANSC_HANDLE)pObjRecord) + pObjRecord->GetVarRecordCount((ANSC_HANDLE)pObjRecord, bFromAcs);
        }
        else
        {
            ulParameterCount = pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, bFromAcs, TRUE);
        }
    }
#if 0
    /* After the discussion with Kang, we feel that we should response with
     * the parameter entry no matter what bNextLevel is.
     */
    else if ( bNextLevel )
    {
        /*
         * If NextLeve is true, the ParameterPath must only be a partial path (referring to an
         * object). If ParameterPath refers to a Parameter and NextLevel is true, the CPE must
         * return a fault with fault code 9003 (Invalid Argument).
         */
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT1;
    }
#endif
    else
    {
        ulParameterCount = 1;
    }

    if ( ulParameterCount == 0 )
    {
        ppParamInfoArray = NULL;
        pulArraySize     = 0;
        /*
         * If we didn't get any parameters, it is not an error,
         * just there is parameters/instances.
         *
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;
         */
        returnStatus = ANSC_STATUS_SUCCESS;

        goto  EXIT1;
    }


    /*
     * In TR-069 Plugfests, there\ufffds a test case to trigger the 9004 Resources Exceeded fault by
     * retrieving information on the Entire Object Model in a single "GetParameterNames",
     * "GetParameterValues" or "GetParameterAttributes" call. As Bin suggested, to address this
     * issue, we should impose an upper limit on the maximum number of parameters that can be
     * returned in a single response. DSLH_WMPDO_MAX_PARAMS_IN_RESPONSE is defined for this purpose.
     * By default, it's set to 256. This value should be adjusted based on the available DRAM on
     * the target platform.
     */

    /*
    if ( ulParameterCount > DSLH_WMPDO_MAX_PARAM_NAMES_IN_RESPONSE )
    {
        pCwmpSoapFault->soap_faultcode           = AnscCloneString(DSLH_CPE_SOAP_faultcode_resources);
        pCwmpSoapFault->soap_faultstring         = AnscCloneString(DSLH_CPE_SOAP_faulttext_resources);
        pCwmpSoapFault->Fault.FaultCode          = DSLH_CPE_CWMP_FaultCode_resources;
        pCwmpSoapFault->Fault.FaultString        = AnscCloneString(DSLH_CPE_CWMP_FaultText_resources);
        pCwmpSoapFault->SetParamValuesFaultCount = 0;

        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT2;
    }
    */

    /*
     * In the second round, we allocate the pParamInfoArray to be returned based on the total
     * parameter number calculated in the first round. We then copy the parameter name/info pairs
     * into this array.
     */
/*
    AnscTrace("The ulParameterCount = %d\n", ulParameterCount);
*/
    pParameterInfoArray = (PDSLH_CWMP_PARAM_INFO)AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_INFO) * ulParameterCount);

    if ( !pParameterInfoArray )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }

    if ( bTargetIsObject )
    {
        returnStatus =
            pObjRecord->GetAllParamInfos
                (
                    (ANSC_HANDLE)pObjRecord,
                    pParameterInfoArray,
                    &ulParameterCount,
                    bNextLevel,
                    bFromAcs
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

            goto  EXIT2;
        }
    }
    else
    {
        pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

        pParameterInfoArray[0].Name      = pVarRecord->GetFullName((ANSC_HANDLE)pVarRecord);
        pParameterInfoArray[0].bWritable = pVarRecord->IsRecordWritable(pVarRecord);
    }

    /*
    AnscTrace("Total parameter count in 'GetParameterNames' == %d\n", ulParameterCount);
    */

    *ppParamInfoArray = pParameterInfoArray;
    *pulArraySize     = ulParameterCount;
    returnStatus      = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pParameterInfoArray )
    {
        AnscFreeMemory(pParameterInfoArray);
    }

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaSetParameterAttributes
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                void*                       pSetParamAttribArray,
                ULONG                       ulArraySize
            );

    description:

        This function is called to configure the attribute of the
        specified parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                void*                       pSetParamAttribArray
                Specifies the array of ParameterAttribute structures.

                ULONG                       ulArraySize
                Specifies the number of elements in the
                pParamAttributeArray.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaSetParameterAttributes
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        void*                       pSetParamAttribArray,
        ULONG                       ulArraySize
    )
{
    ANSC_STATUS                     returnStatus             = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject                = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty                = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_CWMP_SOAP_FAULT           pCwmpSoapFault           = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    PDSLH_CWMP_SET_PARAM_ATTRIB     pSetParameterAttribArray = (PDSLH_CWMP_SET_PARAM_ATTRIB)pSetParamAttribArray;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord               = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity               = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf               = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;    
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord               = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity               = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    BOOL                            bTargetIsObject          = (BOOL                       )FALSE;
    ANSC_HANDLE*                    phAnyRecordArray         = (ANSC_HANDLE*               )NULL;
    ULONG                           ulEntityCount            = (ULONG                      )0;
    ULONG                           i                        = 0;
    BOOL                            bFromAcs                 = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);

    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord          = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity          = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry              = (PSINGLE_LINK_ENTRY     )NULL;
    
    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */

    if ( (ulArraySize == 0) || !pSetParameterAttribArray )
    {
        returnStatus = ANSC_STATUS_SUCCESS;

        goto  EXIT1;
    }

    /*
     * This method may be used by a Server to modify attributes associated with one or more CPE
     * Parameter. The 'Name' argument in each SetParameterAttributesStruct specifies the name of a
     * Parameter to apply the new attributes. Alternatively, this may be a partial path name,
     * indicating that the new attributes are to be applied to all Parameters below this point in
     * the naming hierarchy.
     */
    phAnyRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * ulArraySize);

    if ( !phAnyRecordArray )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT2;
    }

    /*
     * The SetParameterAttributes() operation is divided into two steps: the first round we
     * validate each name/attribute pair contained in the SetParamAttribArray. We also calculate
     * the total number of parameters to be returned in this round. But wait, why do we need the
     * total number again?
     */
    for ( i = 0; i < ulArraySize; i++ )
    {
        if ( (pSetParameterAttribArray[i].Notification != DSLH_CWMP_NOTIFICATION_off    ) &&
             (pSetParameterAttribArray[i].Notification != DSLH_CWMP_NOTIFICATION_passive) &&
             (pSetParameterAttribArray[i].Notification != DSLH_CWMP_NOTIFICATION_active ) )
        {
            returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

            goto  EXIT3;
        }

        if ( !pSetParameterAttribArray[i].Name )
        {
            pObjRecord      = pRootObjRecord;
            bTargetIsObject = TRUE;
        }
        else if ( DslhCwmpIsPartialName(pSetParameterAttribArray[i].Name) )
        {
            pObjRecord =
                (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pSetParameterAttribArray[i].Name
                    );

            if ( !pObjRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

            if(!pObjEntity || (pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs))
            {
                /* If it's a hidden parameter and it's from ACS, we will reject it */
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            bTargetIsObject = TRUE;
        }
        else
        {
            pVarRecord =
                (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pSetParameterAttribArray[i].Name
                    );

            if ( !pVarRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

            if(!pVarEntity || (pVarEntity->ParamDescr->bInvisible && bFromAcs))
            {
                /* If it's a hidden parameter and it's from ACS, we will reject it */
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            bTargetIsObject = FALSE;
        }

        if ( bTargetIsObject )
        {
            /*
             * The CPE may return a "notification request rejected" error if an attempt is made to
             * set notification on a parameter deemed inappropriate (e.g., a continuously varying
             * statistics).
             */
            pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

            /* We don't have a path where all the parameters can be set as active */
            /* 
             * Tom commented out in order to pass e4200 VETI-193:
             * [consona]DUT always returns FaultCode 9002 after calling SetParameterAttributes to Active Notification for partial path.
             * Still need more test here.
             */
            /*           
            if (pSetParameterAttribArray[i].Notification == DSLH_CWMP_NOTIFICATION_active) 
            {
                returnStatus = ANSC_STATUS_BAD_PARAMETER;

                goto  EXIT3;
            }
            */
    
            if (TRUE)
            {
                 pSLinkEntry = AnscQueueGetFirstEntry(&pObjRecord->VarroTable);
                 while ( pSLinkEntry )
                 {
                      pChildVarRecord = ACCESS_DSLH_VAR_RECORD_OBJECT(pSLinkEntry);
                      pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pChildVarRecord->hDslhVarEntity;
                      pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                      if ( (pChildVarEntity->ParamDescr->NotifyStatus == DSLH_CWMP_NOTIFY_STATUS_alwaysOff) 
                           &&  (pSetParameterAttribArray[i].Notification == DSLH_CWMP_NOTIFICATION_passive || pSetParameterAttribArray[i].Notification == DSLH_CWMP_NOTIFICATION_active))
                      {
                           returnStatus = CCSP_ERR_SETATTRIBUTE_REJECTED;
                           goto  EXIT3;
                      }
                 }
            }

            phAnyRecordArray[i] = (ANSC_HANDLE)pObjRecord;
            
        }
        else
        {
            pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;
            
            /* This is a temp action. In the future, we need make the relation clear. Yan/Tom  */
/*
            if ( (pVarEntity->ParamDescr->NotifyStatus     == DSLH_CWMP_NOTIFY_STATUS_alwaysOff || pVarEntity->ParamDescr->NotifyStatus     == DSLH_CWMP_NOTIFY_STATUS_configurable) 
                &&  (pSetParameterAttribArray[i].Notification == DSLH_CWMP_NOTIFICATION_active    ) )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            if ( (pVarEntity->ParamDescr->NotifyStatus     == DSLH_CWMP_NOTIFY_STATUS_alwaysOn) &&
                 (pSetParameterAttribArray[i].Notification != DSLH_CWMP_NOTIFICATION_active    ) )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }
*/
            phAnyRecordArray[i] = (ANSC_HANDLE)pVarRecord;
            
        }
    }

    /*
     * In the second round, we configure the parameter attributes by going through each ObjRecord
     * and VarRecord object contained in the phAnyRecordArray. At this point, all SET operations
     * must succeed.
     */
    for ( i = 0; i < ulArraySize; i++ )
    {
        if ( !pSetParameterAttribArray[i].Name || DslhCwmpIsPartialName(pSetParameterAttribArray[i].Name) )
        {
            pObjRecord   = (PDSLH_OBJ_RECORD_OBJECT)phAnyRecordArray[i];
            returnStatus =
                pObjRecord->SetAllParamAttribs
                    (
                        (ANSC_HANDLE)pObjRecord,
                        (ANSC_HANDLE)&pSetParameterAttribArray[i]
                    );
        }
        else
        {
            pVarRecord = (PDSLH_VAR_RECORD_OBJECT)phAnyRecordArray[i];

            if ( pSetParameterAttribArray[i].bNotificationChange )
            {
                returnStatus =
                    pVarRecord->SetNotification
                        (
                            (ANSC_HANDLE)pVarRecord,
                            pSetParameterAttribArray[i].Notification
                        );
            }

            if ( pSetParameterAttribArray[i].bAccessListChange )
            {
                returnStatus =
                    pVarRecord->SetAccessList
                        (
                            (ANSC_HANDLE)pVarRecord,
                            pSetParameterAttribArray[i].AccessList
                        );
            }

            pVarRecord->SaveConfig((ANSC_HANDLE)pVarRecord);
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT3;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( phAnyRecordArray )
    {
        AnscFreeMemory(phAnyRecordArray);
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        goto  EXIT2;
    }

EXIT2:


    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaGetParameterAttributes
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                SLAP_STRING_ARRAY*          pParamNameArray,
				ULONG						uMaxEntry,
                void**                      ppParamAttribArray,
                PULONG                      pulArraySize
            );

    description:

        This function is called to retrieve the attibutes of the
        specified parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                SLAP_STRING_ARRAY*          pParamNameArray
                Specifies the name of the parameters whose attributes
                are to be returned.

				ULONG						uMaxEntry,
				The maxi amount of entries allowed;

                void**                      ppParamAttributeArray
                Returns the array of ParameterAttribute structures.

                PULONG                      pulArraySize
                Specifies the number of elements in the returned
                *ppParamAttributeArray.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaGetParameterAttributes
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        SLAP_STRING_ARRAY*          pParamNameArray,
		ULONG						uMaxEntry,
        void**                      ppParamAttribArray,
        PULONG                      pulArraySize
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject             = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty             = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord        = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_CWMP_SOAP_FAULT           pCwmpSoapFault        = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    PDSLH_CWMP_PARAM_ATTRIB         pParameterAttribArray = (PDSLH_CWMP_PARAM_ATTRIB    )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf            = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    ULONG                           ulParameterCount      = (ULONG                      )0;
    ULONG                           ulParameterIndex      = (ULONG                      )0;
    ULONG                           ulParamCopyCount      = (ULONG                      )0;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord            = (PDSLH_VAR_RECORD_OBJECT    )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity            = (PDSLH_VAR_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord            = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity            = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    ANSC_HANDLE*                    phAnyRecordArray      = (ANSC_HANDLE*               )NULL;
    SLAP_VARIABLE*                  pParameterVar         = (SLAP_VARIABLE*             )NULL;
    ULONG                           i                     = 0;
    BOOL                            bFromAcs              = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);

    *ppParamAttribArray = NULL;
    *pulArraySize       = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */

    if ( !pParamNameArray )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT2;
    }
    else if ( pParamNameArray->VarCount == 0 )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT2;
    }
    else
    {
        phAnyRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pParamNameArray->VarCount);
    }

    if ( !phAnyRecordArray )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT2;
    }

    /*
     * The GetParameterAttributes() operation is divided into two steps: the first round we
     * validate each parameter name and calculate the total number of parameters that need to be
     * returned.
     */
    for ( i = 0; i < pParamNameArray->VarCount; i++ )
    {
        /*
         * If a Parameter name argument is given as a partial path name, the request is to be inter-
         * preted as a request to return all the Parameters in the branch of the naming hierarchy
         * that shares the same prefix as the argument.
         */
        if ( !pParamNameArray->Array.arrayString[i] || (AnscSizeOfString(pParamNameArray->Array.arrayString[i]) == 0) )
        {
            pObjRecord          = pRootObjRecord;
            phAnyRecordArray[i] = (ANSC_HANDLE)pObjRecord;
            ulParameterCount   += pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, FALSE, FALSE);
        }
        else if ( DslhCwmpIsPartialName(pParamNameArray->Array.arrayString[i]) )
        {
            pObjRecord =
                (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pParamNameArray->Array.arrayString[i]
                    );

            if ( !pObjRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

            if(!pObjEntity || (pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible && bFromAcs))
            {
                /* If it's a hidden parameter and it's from ACS, we will reject it */
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            phAnyRecordArray[i] = (ANSC_HANDLE)pObjRecord;

            ulParameterCount += pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, FALSE, FALSE);
        }
        else
        {
            pVarRecord =
                (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
                    (
                        (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                        pParamNameArray->Array.arrayString[i]
                    );

            if ( !pVarRecord )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;

            if(!pVarEntity || (pVarEntity->ParamDescr->bInvisible && bFromAcs))
            {
                /* If it's a hidden parameter and it's from ACS, we will reject it */
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT3;
            }

            phAnyRecordArray[i] = (ANSC_HANDLE)pVarRecord;

            ulParameterCount++;
        }
    }

    /*
     * In TR-069 Plugfests, there\ufffds a test case to trigger the 9004 Resources Exceeded fault by
     * retrieving information on the Entire Object Model in a single "GetParameterNames",
     * "GetParameterValues" or "GetParameterAttributes" call. As Bin suggested, to address this
     * issue, we should impose an upper limit on the maximum number of parameters that can be
     * returned in a single response. DSLH_WMPDO_MAX_PARAMS_IN_RESPONSE is defined for this purpose.
     * By default, it's set to 1000. This value should be adjusted based on the available DRAM on
     * the target platform.
     */
	if( ulParameterCount == 0)
	{
        ppParamAttribArray = NULL;
        pulArraySize       = 0;

        goto  EXIT3;
        
	}

    if ( ulParameterCount > uMaxEntry )
    {
        returnStatus = CCSP_ERR_RESOURCE_EXCEEDED;

        goto  EXIT3;
    }

    /*
     * In the second round, we allocate the pParamAttribArray to be returned based on the total
     * parameter number calculated in the first round. We then copy the parameter name/attribute
     * pairs into this array.
     */
    ulParameterIndex      = 0;
    ulParamCopyCount      = 0;
    pParameterAttribArray = (PDSLH_CWMP_PARAM_ATTRIB)AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_ATTRIB) * ulParameterCount);

    if ( !pParameterAttribArray )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT3;
    }

    for ( i = 0; i < pParamNameArray->VarCount; i++ )
    {
        if ( !pParamNameArray->Array.arrayString[i]                         ||
             (AnscSizeOfString(pParamNameArray->Array.arrayString[i]) == 0) ||
             DslhCwmpIsPartialName(pParamNameArray->Array.arrayString[i]) )
        {
            pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)phAnyRecordArray[i];

            ulParamCopyCount = ulParameterCount - ulParameterIndex;
            returnStatus     =
                pObjRecord->GetAllParamAttribs
                    (
                        (ANSC_HANDLE)pObjRecord,
                        (ANSC_HANDLE)&pParameterAttribArray[ulParameterIndex],
                        &ulParamCopyCount,
                        bFromAcs
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

                goto  EXIT4;
            }

            ulParameterIndex += ulParamCopyCount;
        }
        else
        {
            pVarRecord = (PDSLH_VAR_RECORD_OBJECT)phAnyRecordArray[i];

            pParameterAttribArray[ulParameterIndex].Name         = AnscCloneString(pParamNameArray->Array.arrayString[i]);
            pParameterAttribArray[ulParameterIndex].Notification = pVarRecord->Notification;

            if( pVarRecord->AccessList == NULL)
            {
                pParameterAttribArray[ulParameterIndex].AccessList   = AnscCloneString(DSLH_MPA_ENTITY_Subscriber);
            }
            else
            {
                pParameterAttribArray[ulParameterIndex].AccessList   = AnscCloneString(pVarRecord->AccessList);
            }

            ulParameterIndex++;
        }
    }

    /*
	AnscTrace("Total parameter count in 'GetParameterAttributes' == %d\n", ulParameterIndex);
     */

    *ppParamAttribArray = pParameterAttribArray;
    *pulArraySize       = ulParameterIndex;
    returnStatus        = ANSC_STATUS_SUCCESS;

    goto  EXIT3;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT4:

    if ( pParameterAttribArray )
    {
        for ( i = 0; i < ulParameterIndex; i++ )
        {
            DslhCwmpCleanParamAttrib((&pParameterAttribArray[i]));
        }

        AnscFreeMemory(pParameterAttribArray);
    }

EXIT3:

    if ( phAnyRecordArray )
    {
        AnscFreeMemory(phAnyRecordArray);
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        goto  EXIT2;
    }

EXIT2:


    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;

EXIT1:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaAddObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pObjName,
                PULONG                      pulObjInsNumber,
                int*                        piStatus
            );

    description:

        This function is called to create a new instance of a multi-
        instance object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pObjName
                Specifies the path name of the collection of objects
                for which a new instance is to be created.

                PULONG                      pulObjInsNumber
                Returns the object instance of the newly created
                object.

                int*                        piStatus
                Returns the status of the object creation.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaAddObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        PULONG                      pulObjInsNumber,
        int*                        piStatus
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject       = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty       = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord  = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_MPR_INTERFACE             pDslhMprIf      = (PDSLH_MPR_INTERFACE        )pMyObject->hDslhMprIf;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord      = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntityChild = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    BOOL                            bFromAcs        = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);

    *pulObjInsNumber = 0;
    *piStatus        = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */

    if ( !pObjName || (AnscSizeOfString(pObjName) == 0) || !DslhCwmpIsPartialName(pObjName) )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT;
    }
    else
    {
        pObjRecord =
            (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pObjName
                );
    }

    if ( !pObjRecord )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT;
    }
    else
    {
        pObjEntity      = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;
        pObjEntityChild = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);
    }

    if ( !pObjEntity->ObjDescr->bWritable || (pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table) )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT;
    }
    /*
     *  Bin added here in order to allow GUI call "AddObject" to create an invisible entry while block ACS requests. 
     */
    else if ( bFromAcs && pObjEntity->ObjDescr && pObjEntity->ObjDescr->bInvisible)
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT;
    }
    else 
    {
        PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable  = (PDSLH_OBJCO_TABLE_OBJECT)pObjRecord->hDslhObjController;

        if ( pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
        {
            returnStatus = ANSC_STATUS_BAD_PARAMETER;

            goto  EXIT;
        }
        else
        if ( pObjRecord->GetObjRecordCount((ANSC_HANDLE)pObjRecord) >= pObjEntityChild->ObjDescr->MaxInstanceNumber )
        {
            returnStatus = CCSP_ERR_RESOURCE_EXCEEDED;

            goto  EXIT;
        }
    }

    returnStatus =
        pObjRecord->AddChildObject
            (
                (ANSC_HANDLE)pObjRecord,
                pulObjInsNumber
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        returnStatus = ANSC_STATUS_BAD_PARAMETER;

        goto  EXIT;
    }

    *piStatus    = 0;
    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/


EXIT:

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoMpaDeleteObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pAccessEntity,
                char*                       pObjName,
                int*                        piStatus
            );

    description:

        This function is called to create a new instance of a multi-
        instance object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pAccessEntity
                Specifies the name of the access entity who is trying
                to making the change.

                char*                       pObjName
                Specifies the path name of the object instance to be
                removed.

                int*                        piStatus
                Returns the status of the object deletion.

                ANSC_HANDLE*                phSoapFault
                Returns the error information of the operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMpaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        int*                        piStatus
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject        = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty        = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord   = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_MPR_INTERFACE             pDslhMprIf       = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_CWMP_SOAP_FAULT           pCwmpSoapFault   = (PDSLH_CWMP_SOAP_FAULT      )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord       = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecordParent = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntityParent = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    BOOL                            bFromAcs         = AnscEqualString(pAccessEntity, DSLH_MPA_ENTITY_ACS,TRUE);

    *piStatus    = 0;

    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    /*
     * A fault response MUST make use of the SOAP Fault element using the following conventions:
     *
     *      - The SOAP faultcode element MUST indicate the source of the fault, either
     *        Client or Server, as appropriate for the particular fault. In this usage,
     *        Client represents the originator of the SOAP request, and Server represents
     *        the SOAP responder.
     *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
     *      - The SOAP detail element MUST contain a Fault structure defined in the
     *        "urn:dslforum-org:cwmp-1-0" namespace.
     */

    if ( !pObjName || (AnscSizeOfString(pObjName) == 0) || !DslhCwmpIsPartialName(pObjName) )
    {
        returnStatus = CCSP_ERR_INVALID_ARGUMENTS;

        goto  EXIT2;
    }
    else
    {
        pObjRecord =
            (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
                (
                    (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                    pObjName
                );
    }

    if ( !pObjRecord )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }
    else
    {
        pObjRecordParent = (PDSLH_OBJ_RECORD_OBJECT)pObjRecord->hParentObjRecord;
        pObjEntityParent = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecordParent->hDslhObjEntity;
    }

    if ( !pObjEntityParent->ObjDescr->bWritable || (pObjEntityParent->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table) )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }
    /*
     *  Bin added here in order to allow GUI call "DeleteObject" to delete an invisible entry while block ACS requests. 
     */
    else if ( bFromAcs && pObjEntityParent->ObjDescr->bInvisible)
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }

    if ( TRUE )
    {
        PDSLH_OBJCO_TABLE_OBJECT    pObjcoTable     = (PDSLH_OBJCO_TABLE_OBJECT)pObjRecordParent->hDslhObjController;
        
        if ( pObjcoTable->IsWritable != NULL && !pObjcoTable->IsWritable((ANSC_HANDLE)pObjcoTable) )
        {
            returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

            goto  EXIT2;
        }
    }

    returnStatus =
        pObjRecordParent->DelChildObject
            (
                (ANSC_HANDLE)pObjRecordParent,
                pObjRecord->LastName
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        returnStatus = CCSP_ERR_INVALID_PARAMETER_NAME;

        goto  EXIT2;
    }

    *piStatus    = 0;
    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:


    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;

EXIT1:


    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}
