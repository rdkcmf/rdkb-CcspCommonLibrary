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

    module:	sys_smo_control.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced op-control functions
        of the Sys Setup Manager Object.

        *   SysSmoLoadComponents
        *   SysSmoResolveDependencies
        *   SysSmoUnloadComponents
        *   SysSmoSetUpEnv
        *   SysSmoCloseEnv

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/14/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoLoadComponents
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to load all the components.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoLoadComponents
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoResolveDependencies
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to resolve all the dependencies.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoResolveDependencies
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoUnloadComponents
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unload all the components.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoUnloadComponents
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty    = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_IRA_INTERFACE              pSysIraIf    = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoSetUpEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the runtime environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoSetUpEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject        = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty        = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_RCM_INTERFACE              pSysRcmIf        = (PSYS_RCM_INTERFACE         )pMyObject->hSysRcmIf;
    PSYS_IRA_INTERFACE              pSysIraIf        = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;
    ANSC_HANDLE                     hRfoKeyEnv       = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyCfg       = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyCfgSetup  = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyModuleEnv = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyModuleCfg = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyIfs       = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyObjects   = (ANSC_HANDLE                )NULL;

    if ( pMyObject->bActive )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto  EXIT1;
    }

    hRfoKeyEnv =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)NULL,
                SYS_FOLDER_L1_RUNTIME_ENVIRONMENT
            );

    hRfoKeyCfg =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)NULL,
                SYS_FOLDER_L1_CONFIGURATION
            );

    if ( !hRfoKeyEnv || !hRfoKeyCfg )
    {
        goto  EXIT2;
    }

    hRfoKeyCfgSetup =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyCfg,
                SYS_FOLDER_L2_SETUP
            );

    hRfoKeyModuleEnv =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyEnv,
                pProperty->ModuleName
            );

    if ( !hRfoKeyCfgSetup || !hRfoKeyModuleEnv )
    {
        goto  EXIT3;
    }

    hRfoKeyModuleCfg =
        pSysIraIf->AddFolder2
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyCfgSetup,
                pProperty->CsetupName,
                SYS_RFO_PERMISSION_ALL,
                SYS_RFO_ACCESS_MODE_ADAPTIVE,
                SYS_REP_FOLDER_TYPE_SESSION,
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)pSysRcmIf
            );

    if ( !hRfoKeyModuleCfg )
    {
        goto  EXIT3;
    }
    else
    {
        pMyObject->hSetupRfo = hRfoKeyModuleCfg;    /* it's actually the folder key, not folder itself */
    }

    hRfoKeyIfs =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyModuleEnv,
                SYS_FOLDER_INTERFACES
            );

    hRfoKeyObjects =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyModuleEnv,
                SYS_FOLDER_OBJECTS
            );

    if ( !hRfoKeyIfs || !hRfoKeyObjects )
    {
        goto  EXIT4;
    }

    returnStatus =
        pSysIraIf->AddRecord
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyIfs,
                pSysRcmIf->Name,
                SYS_REP_RECORD_TYPE_HCXT,
                (PVOID)&pSysRcmIf,
                sizeof(ANSC_HANDLE)
            );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT4:

    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyObjects);
    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyIfs    );

EXIT3:

    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyCfgSetup );
    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyModuleEnv);
    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyModuleCfg);

EXIT2:

    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyEnv);
    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyCfg);

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the runtime environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject       = (PSYS_SETUP_MANAGER_OBJECT  )hThisObject;
    PSYS_SETUP_MANAGER_PROPERTY     pProperty       = (PSYS_SETUP_MANAGER_PROPERTY)&pMyObject->Property;
    PSYS_RCM_INTERFACE              pSysRcmIf       = (PSYS_RCM_INTERFACE         )pMyObject->hSysRcmIf;
    PSYS_IRA_INTERFACE              pSysIraIf       = (PSYS_IRA_INTERFACE         )pMyObject->hSysIraIf;
    ANSC_HANDLE                     hRfoKeyCfg      = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hRfoKeyCfgSetup = (ANSC_HANDLE                )NULL;

    if ( !pMyObject->bActive )
    {
        returnStatus = ANSC_STATUS_INTERNAL_ERROR;

        goto  EXIT1;
    }

    hRfoKeyCfg =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                (ANSC_HANDLE)NULL,
                SYS_FOLDER_L1_CONFIGURATION
            );

    if ( !hRfoKeyCfg )
    {
        goto  EXIT2;
    }

    hRfoKeyCfgSetup =
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyCfg,
                SYS_FOLDER_L2_SETUP
            );

    if ( !hRfoKeyCfgSetup )
    {
        goto  EXIT3;
    }

    returnStatus =
        pSysIraIf->DelFolder2
            (
                pSysIraIf->hOwnerContext,
                hRfoKeyCfgSetup,
                pProperty->CsetupName,
                TRUE,
                TRUE
            );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyCfgSetup);

EXIT2:

    pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hRfoKeyCfg);

EXIT1:

    return  returnStatus;
}
