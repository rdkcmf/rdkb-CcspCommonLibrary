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

    module: dslh_dmagnt_operation.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Dslh Cpe Controller Object.

        *   DslhDmagntEngage
        *   DslhDmagntCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/08/2010    initial revision.
        03/21/2011    added api to retrieve instance number by index

**********************************************************************/


#include "dslh_dmagnt_global.h"
#include "ccsp_ifo_ccd.h"
#include "poam_irepfo_interface.h"
#include "poam_irepfo_exported_api.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntEngage
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
DslhDmagntEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject              = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions         = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomFunctions;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController     = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRoot        = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoProvision   = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA        = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFolderCOSA;

    PDSLH_MWS_INTERFACE             pDslhMwsIf             = (PDSLH_MWS_INTERFACE        )pMyObject->hDslhMwsIf;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if( pDslhMwsIf != NULL)
    {
        pDslhCpeController->AddInterface(pDslhCpeController, pMyObject->hDslhMwsIf);
    }

    pMyObject->hDslhCcdIf = pDslhCpeController->GetInterfaceByName(pDslhCpeController, CCSP_CCD_INTERFACE_NAME);
    pMyObject->hDslhLcbIf = pDslhCpeController->GetInterfaceByName(pDslhCpeController, CCSP_LIBCBK_INTERFACE_NAME);

    /* register internal apis */
    if( pAtomFunctions == NULL)
    {
        pMyObject->hAtomFunctions =
            AnscCreateAtomTable
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    NULL
                );

        pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomFunctions;
    }

    if( pAtomFunctions != NULL)
    {                                                                                         
        /* Register the API exported by framework */
        pMyObject->RegisterInternalApi(pMyObject, "COSANotifyValueChange",            COSANotifyValueChange);
        pMyObject->RegisterInternalApi(pMyObject, "COSANotifyDiagComplete",           COSANotifyDiagComplete);
        pMyObject->RegisterInternalApi(pMyObject, "COSAValidateHierarchyInterface",   COSAValidateHierarchyInterface);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValueBool",            COSAGetParamValueBool);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValueInt",             COSAGetParamValueInt);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValueUlong",           COSAGetParamValueUlong);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValueString",          COSAGetParamValueString);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValue",                COSAGetParamValue);
        pMyObject->RegisterInternalApi(pMyObject, "COSASetParamValueBool",            COSASetParamValueBool);
        pMyObject->RegisterInternalApi(pMyObject, "COSASetParamValueInt",             COSASetParamValueInt);
        pMyObject->RegisterInternalApi(pMyObject, "COSASetParamValueUlong",           COSASetParamValueUlong);
        pMyObject->RegisterInternalApi(pMyObject, "COSASetParamValueString",          COSASetParamValueString);

        pMyObject->RegisterInternalApi(pMyObject, "COSAGetRegistryRootFolder",        COSAGetRegistryRootFolder);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetInstanceNumberByIndex",     COSAGetInstanceNumberByIndex);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetInterfaceByName",           COSAGetInterfaceByName);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetMessageBusHandle",          COSAGetMessageBusHandle);
        pMyObject->RegisterInternalApi(pMyObject, "COSARegisterCallBackAfterInitDml", COSARegisterCallBackAfterInitDml);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetSubsystemPrefix",           COSAGetSubsystemPrefix);
        pMyObject->RegisterInternalApi(pMyObject, "COSARepopulateTable",              COSARepopulateTable);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetParamValueByPathName",      COSAGetParamValueByPathName);
        pMyObject->RegisterInternalApi(pMyObject, "COSASetParamValueByPathName",      COSASetParamValueByPathName);
        pMyObject->RegisterInternalApi(pMyObject, "COSAGetInstanceNumbers",           COSAGetInstanceNumbers);

        /* Register the API  for CCSP Common Components*/
#if 0
        pMyObject->RegisterInternalApi(pMyObject, "CcspComp_GetParamUlongValue",      CcspComp_GetParamUlongValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspComp_GetParamStringValue",     CcspComp_GetParamStringValue);
#endif
        pMyObject->RegisterInternalApi(pMyObject, "CcspComp_GetBulkParamValues",      CcspComp_GetBulkParamValues);
        pMyObject->RegisterInternalApi(pMyObject, "CcspComp_SetParamBoolValue",       CcspComp_SetParamBoolValue);

        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_GetParamBoolValue",        CcspLog_GetParamBoolValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_GetParamUlongValue",       CcspLog_GetParamUlongValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_SetParamBoolValue",        CcspLog_SetParamBoolValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_SetParamUlongValue",       CcspLog_SetParamUlongValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_Validate",                 CcspLog_Validate );
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_Commit",                   CcspLog_Commit );
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_Rollback",                 CcspLog_Rollback );
        pMyObject->RegisterInternalApi(pMyObject, "CcspMem_GetParamUlongValue",       CcspMem_GetParamUlongValue);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_GetBulkParamValues",       CcspLog_GetBulkParamValues);
        pMyObject->RegisterInternalApi(pMyObject, "CcspLog_SetBulkParamValues",       CcspLog_SetBulkParamValues);

    }

    /*
     * Enroll and create PoamIrepFolder object.
     */
    if ( TRUE )
    {

        pPoamIrepFoRoot =
            (PPOAM_IREP_FOLDER_OBJECT)PoamIrepFoCreate(NULL, NULL, ""); 

        if ( !pPoamIrepFoRoot )
        {
            AnscTrace("DslhDataModelAgent: Cannot access Root Folder!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }

        pPoamIrepFoProvision =
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRoot->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoRoot,
                    "Provision"
                );

        if ( !pPoamIrepFoProvision )
        {
            AnscTrace("DslhDataModelAgent: Cannot access /Configuration/Provision folder!!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT3;
        }

        pPoamIrepFoCOSA =
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoProvision->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoProvision,
                    COSA_IREP_FOLDER_NAME
                );

        if ( !pPoamIrepFoCOSA )
        {
            pPoamIrepFoCOSA =
                pPoamIrepFoProvision->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoProvision,
                        COSA_IREP_FOLDER_NAME,
                        0
                    );
        }

        if ( !pPoamIrepFoCOSA )
        {
            AnscTrace("DslhDataModelAgent: Cannot access /Configuration/Provision/%s folder!!\n", COSA_IREP_FOLDER_NAME);

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT4;
        }
        else
        {
            pMyObject->hIrepFolderCOSA = (ANSC_HANDLE)pPoamIrepFoCOSA;
        }
    }

    pMyObject->bActive = TRUE;

    returnStatus = ANSC_STATUS_SUCCESS;


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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntCancel
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
DslhDmagntCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PPLUGIN_INFORMATION             pInfo             = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    /* remove all the pluginInfo  */

    pSListEntry =   AnscQueueGetFirstEntry(&pMyObject->sPluginInfoList);

    while( pSListEntry != NULL)
    {
        pInfo       = ACCESS_PLUGIN_INFORMATION(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry(pSListEntry);

        if( pInfo != NULL)
        {
            DslhDmagntRemovePluginInfo(pMyObject, pInfo);
        }              
    }

    AnscQueueInitializeHeader(&pMyObject->sPluginInfoList);

    pMyObject->bActive = FALSE;

    return ANSC_STATUS_SUCCESS;
}


