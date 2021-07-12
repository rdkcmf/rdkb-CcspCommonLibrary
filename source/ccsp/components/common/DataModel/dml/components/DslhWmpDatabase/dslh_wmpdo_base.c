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

    module:	dslh_wmpdo_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoCreate
        *   DslhWmpdoRemove
        *   DslhWmpdoEnrollObjects
        *   DslhWmpdoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/06/2005    initial revision.
        01/12/2011    Bin added "pRootObjName".
        02/07/2011    Bin added "UnregisterObject" support;
        02/28/2011    Bin added a set of ReqCounter apis to better support dynamic tables;

**********************************************************************/


#include "dslh_wmpdo_global.h"
#ifdef FEATURE_SUPPORT_RDKLOG
#include "rdk_debug.h"
#endif
#include "safec_lib_common.h"

#define DEBUG_INI_NAME "/etc/debug.ini"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhWmpdoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Wmp Database Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_WMP_DATABASE_OBJECT)AnscAllocateMemory(sizeof(DSLH_WMP_DATABASE_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, DSLH_WMP_DATABASE_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = DSLH_WMP_DATABASE_OID;
    pBaseObject->Create            = DslhWmpdoCreate;
    pBaseObject->Remove            = DslhWmpdoRemove;
    pBaseObject->EnrollObjects     = DslhWmpdoEnrollObjects;
    pBaseObject->Initialize        = DslhWmpdoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject         = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf        = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
#ifndef _DSLH_TCP_CONNREQ_HANDLER
    PSLAP_VAR_CONVERTER_OBJECT      pSlapVarConverter = (PSLAP_VAR_CONVERTER_OBJECT   )pMyObject->hSlapVarConverter;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper    = (PSLAP_VAR_MAPPER_OBJECT      )pMyObject->hSlapVarMapper;
#endif
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMonitorTimerObj  = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hMonitorTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pMonitorTimerIf   = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hMonitorTimerIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pDslhMprIf )
    {
        AnscFreeMemory(pDslhMprIf);

        pMyObject->hDslhMprIf = (ANSC_HANDLE)NULL;
    }

    if ( pDslhMpaIf )
    {
        AnscFreeMemory(pDslhMpaIf);

        pMyObject->hDslhMpaIf = (ANSC_HANDLE)NULL;
    }
#ifndef _DSLH_TCP_CONNREQ_HANDLER
    if ( pSlapVarConverter )
    {
        pSlapVarConverter->Remove((ANSC_HANDLE)pSlapVarConverter);

        pMyObject->hSlapVarConverter = (ANSC_HANDLE)NULL;
    }

    if ( pSlapVarMapper )
    {
        pSlapVarMapper->Remove((ANSC_HANDLE)pSlapVarMapper);

        pMyObject->hSlapVarMapper = (ANSC_HANDLE)NULL;
    }
#endif
    if ( pMonitorTimerObj )
    {
        pMonitorTimerObj->Remove((ANSC_HANDLE)pMonitorTimerObj);
    }

    if ( pMonitorTimerIf )
    {
        AnscFreeMemory(pMonitorTimerIf);
    }

    AnscFreeTsLock(&pMyObject->AccessTsLock);
    AnscFreeLock  (&pMyObject->MpaWriteLock);
    AnscFreeLock  (&pMyObject->MpoQueueLock);
    AnscCoRemove  ((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject         = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->hDslhMprIf;
    PDSLH_MPA_INTERFACE             pDslhMpaIf        = (PDSLH_MPA_INTERFACE          )pMyObject->hDslhMpaIf;
#ifndef _DSLH_TCP_CONNREQ_HANDLER
    PSLAP_VAR_CONVERTER_OBJECT      pSlapVarConverter = (PSLAP_VAR_CONVERTER_OBJECT   )pMyObject->hSlapVarConverter;
    PSLAP_VAR_MAPPER_OBJECT         pSlapVarMapper    = (PSLAP_VAR_MAPPER_OBJECT      )pMyObject->hSlapVarMapper;
#endif
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMonitorTimerObj  = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hMonitorTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pMonitorTimerIf   = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hMonitorTimerIf;
    errno_t                         rc                = -1;

    if ( !pDslhMprIf )
    {
        pDslhMprIf = (PDSLH_MPR_INTERFACE)AnscAllocateMemory(sizeof(DSLH_MPR_INTERFACE));

        if ( !pDslhMprIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDslhMprIf = (ANSC_HANDLE)pDslhMprIf;
        }

        rc = STRCPY_S_NOCLOBBER(pDslhMprIf->Name, sizeof(pDslhMprIf->Name), DSLH_MPR_INTERFACE_NAME);
        ERR_CHK(rc);

        pDslhMprIf->InterfaceId          = DSLH_MPR_INTERFACE_ID;
        pDslhMprIf->hOwnerContext        = (ANSC_HANDLE)pMyObject;
        pDslhMprIf->Size                 = sizeof(DSLH_MPR_INTERFACE);

        pDslhMprIf->RegParameter         = DslhWmpdoMprRegParameter;
        pDslhMprIf->RegNotifyParam       = DslhWmpdoMprRegNotifyParam;
        pDslhMprIf->DelNotifyParam       = DslhWmpdoMprDelNotifyParam;
        pDslhMprIf->ModNotifyParam       = DslhWmpdoMprModNotifyParam;
        pDslhMprIf->ResetNotifyParam     = DslhWmpdoMprResetNotifyParam;
        pDslhMprIf->RegObject            = DslhWmpdoMprRegObject;
        pDslhMprIf->UnregisterObject     = DslhWmpdoMprUnregisterObject;
        pDslhMprIf->GetVarDataType       = DslhWmpdoMprGetVarDataType;
        pDslhMprIf->GetObjEntity         = DslhWmpdoMprGetObjEntity;
        pDslhMprIf->GetVarRecord         = DslhWmpdoMprGetVarRecord;
        pDslhMprIf->GetVarRecord2        = DslhWmpdoMprGetVarRecord2;
        pDslhMprIf->GetObjRecord         = DslhWmpdoMprGetObjRecord;
        pDslhMprIf->GetObjRecordInstance = DslhWmpdoMprGetObjRecordInstance;

        pDslhMprIf->GetParamValue        = DslhWmpdoMprGetParamValue;
        pDslhMprIf->GetParamValue2       = DslhWmpdoMprGetParamValue2;
        pDslhMprIf->SetParamValue        = DslhWmpdoMprSetParamValue;
        pDslhMprIf->SetParamValue2       = DslhWmpdoMprSetParamValue2;
        pDslhMprIf->SetParamValue3       = DslhWmpdoMprSetParamValue3;
        pDslhMprIf->SetParamValue4       = DslhWmpdoMprSetParamValue4;
        pDslhMprIf->GetParamValueString  = DslhWmpdoMprGetParamValueString;
        pDslhMprIf->SetParamValueString  = DslhWmpdoMprSetParamValueString;
        pDslhMprIf->GetParamValueInt     = DslhWmpdoMprGetParamValueInt;
        pDslhMprIf->SetParamValueInt     = DslhWmpdoMprSetParamValueInt;
        pDslhMprIf->GetParamValueUint    = DslhWmpdoMprGetParamValueUint;
        pDslhMprIf->SetParamValueUint    = DslhWmpdoMprSetParamValueUint;
        pDslhMprIf->GetParamValueBool    = DslhWmpdoMprGetParamValueBool;
        pDslhMprIf->SetParamValueBool    = DslhWmpdoMprSetParamValueBool;
        pDslhMprIf->GetParamValueTime    = DslhWmpdoMprGetParamValueTime;
        pDslhMprIf->SetParamValueTime    = DslhWmpdoMprSetParamValueTime;
        pDslhMprIf->GetParamValueBase64  = DslhWmpdoMprGetParamValueBase64;
        pDslhMprIf->SetParamValueBase64  = DslhWmpdoMprSetParamValueBase64;

        pDslhMprIf->GetEntryObjectCount  = DslhWmpdoMprGetEntryObjectCount;
        pDslhMprIf->GetEntryObject       = DslhWmpdoMprGetEntryObject;
        pDslhMprIf->GetEntryParamValue   = DslhWmpdoMprGetEntryParamValue;
        pDslhMprIf->SetEntryParamValue   = DslhWmpdoMprSetEntryParamValue;
        pDslhMprIf->ApplyEntryChanges    = DslhWmpdoMprApplyEntryChanges;
        pDslhMprIf->GetRootObjName       = DslhWmpdoMprGetRootObjName;
        pDslhMprIf->GetReqCounter        = DslhWmpdoMprGetReqCounter;
        pDslhMprIf->IncReqCounter        = DslhWmpdoMprIncReqCounter;
        pDslhMprIf->RegEmptyObject       = DslhWmpdoMprRegEmptyObject;
    }

    if ( !pDslhMpaIf )
    {
        pDslhMpaIf = (PDSLH_MPA_INTERFACE)AnscAllocateMemory(sizeof(DSLH_MPA_INTERFACE));

        if ( !pDslhMpaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hDslhMpaIf = (ANSC_HANDLE)pDslhMpaIf;
        }

        rc = STRCPY_S_NOCLOBBER(pDslhMpaIf->Name, sizeof(pDslhMpaIf->Name), DSLH_MPA_INTERFACE_NAME);
        ERR_CHK(rc);

        pDslhMpaIf->InterfaceId            = DSLH_MPA_INTERFACE_ID;
        pDslhMpaIf->hOwnerContext          = (ANSC_HANDLE)pMyObject;
        pDslhMpaIf->Size                   = sizeof(DSLH_MPA_INTERFACE);

        pDslhMpaIf->LockWriteAccess        = DslhWmpdoMpaLockWriteAccess;
        pDslhMpaIf->UnlockWriteAccess      = DslhWmpdoMpaUnlockWriteAccess;

        pDslhMpaIf->SetParameterValues     = DslhWmpdoMpaSetParameterValues;
        pDslhMpaIf->SetCommit              = DslhWmpdoMpaSetCommit;
        pDslhMpaIf->GetParameterValues     = DslhWmpdoMpaGetParameterValues;
        pDslhMpaIf->GetParameterNames      = DslhWmpdoMpaGetParameterNames;
        pDslhMpaIf->SetParameterAttributes = DslhWmpdoMpaSetParameterAttributes;
        pDslhMpaIf->GetParameterAttributes = DslhWmpdoMpaGetParameterAttributes;
        pDslhMpaIf->AddObject              = DslhWmpdoMpaAddObject;
        pDslhMpaIf->DeleteObject           = DslhWmpdoMpaDeleteObject;
    }
#ifndef _DSLH_TCP_CONNREQ_HANDLER
    if ( !pSlapVarConverter )
    {
        pSlapVarConverter =
            (PSLAP_VAR_CONVERTER_OBJECT)SlapCreateVarConverter
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapVarConverter )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapVarConverter = (ANSC_HANDLE)pSlapVarConverter;
        }
    }

    if ( !pSlapVarMapper )
    {
        pSlapVarMapper =
            (PSLAP_VAR_MAPPER_OBJECT)SlapCreateVarMapper
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapVarMapper )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapVarMapper = (ANSC_HANDLE)pSlapVarMapper;
        }
    }
#endif
    if ( !pMonitorTimerObj )
    {
        pMonitorTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pMonitorTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMonitorTimerObj = (ANSC_HANDLE)pMonitorTimerObj;
        }

        pMonitorTimerObj->SetTimerType((ANSC_HANDLE)pMonitorTimerObj, ANSC_TIMER_TYPE_PERIODIC              );
        pMonitorTimerObj->SetInterval ((ANSC_HANDLE)pMonitorTimerObj, DSLH_DEF_MONITOR_TIMER_INTERVAL * 1000);
        /* _ansc_strcpy(pMonitorTimerObj->Name, "DslhWmpdoMonitorTimer"); */
    }

    if ( !pMonitorTimerIf )
    {
        pMonitorTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pMonitorTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMonitorTimerIf = (ANSC_HANDLE)pMonitorTimerIf;
        }

        pMonitorTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pMonitorTimerIf->Invoke         = DslhWmpdoMonitorTimerInvoke;

        pMonitorTimerObj->SetClient((ANSC_HANDLE)pMonitorTimerObj, (ANSC_HANDLE)pMonitorTimerIf);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);
#ifdef FEATURE_SUPPORT_RDKLOG
    RDK_LOGGER_INIT();
#endif

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                       = DSLH_WMP_DATABASE_OID;
    pMyObject->Create                    = DslhWmpdoCreate;
    pMyObject->Remove                    = DslhWmpdoRemove;
    pMyObject->EnrollObjects             = DslhWmpdoEnrollObjects;
    pMyObject->Initialize                = DslhWmpdoInitialize;

    pMyObject->hDslhCpeController        = (ANSC_HANDLE)NULL;
    pMyObject->hRootObjRecord            = (ANSC_HANDLE)NULL;
    pMyObject->hRootObjEntity            = (ANSC_HANDLE)NULL;
    pMyObject->hVarRecordArray           = (ANSC_HANDLE)NULL;
    pMyObject->hObjRecordArray           = (ANSC_HANDLE)NULL;
    pMyObject->ulVarRecordCount          = 0;
    pMyObject->ulObjRecordCount          = 0;
    pMyObject->SessionID                 = 0;
    pMyObject->bMpaWriteLocked           = FALSE;
    pMyObject->LockedEntity              = NULL;
    pMyObject->pRootObjName              = NULL;
    pMyObject->bActive                   = FALSE;

    pMyObject->hPoamIrepFoWmpDatabase    = (ANSC_HANDLE)NULL;
    pMyObject->hSlapDslhParamTree        = (ANSC_HANDLE)NULL;

    pMyObject->GetDslhMprIf              = DslhWmpdoGetDslhMprIf;
    pMyObject->GetDslhMpaIf              = DslhWmpdoGetDslhMpaIf;
    pMyObject->GetSlapVarConverter       = DslhWmpdoGetSlapVarConverter;
    pMyObject->GetSlapVarMapper          = DslhWmpdoGetSlapVarMapper;
    pMyObject->GetDslhCpeController      = DslhWmpdoGetDslhCpeController;
    pMyObject->SetDslhCpeController      = DslhWmpdoSetDslhCpeController;
    pMyObject->GetProperty               = DslhWmpdoGetProperty;
    pMyObject->SetProperty               = DslhWmpdoSetProperty;
    pMyObject->ResetProperty             = DslhWmpdoResetProperty;
    pMyObject->Reset                     = DslhWmpdoReset;

    pMyObject->Engage                    = DslhWmpdoEngage;
    pMyObject->Cancel                    = DslhWmpdoCancel;
    pMyObject->SetupEnv                  = DslhWmpdoSetupEnv;
    pMyObject->CloseEnv                  = DslhWmpdoCloseEnv;
    pMyObject->MonitorTimerInvoke        = DslhWmpdoMonitorTimerInvoke;

    pMyObject->CreateRecordTree          = DslhWmpdoCreateRecordTree;
    pMyObject->ConfigRecordTree          = DslhWmpdoConfigRecordTree;
    pMyObject->EnableMonitor             = DslhWmpdoEnableMonitor;

    pMyObject->ParseParamDataType        = DslhWmpdoParseParamDataType;
    pMyObject->ParseParamSyntax          = DslhWmpdoParseParamSyntax;
    pMyObject->ParseParamEnumTokens      = DslhWmpdoParseParamEnumTokens;
    pMyObject->AddVarRecordIntoArray     = DslhWmpdoAddVarRecordIntoArray;
    pMyObject->AddObjRecordIntoArray     = DslhWmpdoAddObjRecordIntoArray;
    pMyObject->InitObjVarArray           = DslhWmpdoInitObjVarArray;
    pMyObject->ResetObjVarArray          = DslhWmpdoResetObjVarArray;
    pMyObject->FlushDynObjVar            = DslhWmpdoFlushDynObjVar;

    /*REVISIT: CID 137470 Data race condition*/
    AnscInitializeTsLock     (&pMyObject->AccessTsLock);
    AnscInitializeLock       (&pMyObject->MpaWriteLock);
    AnscQueueInitializeHeader(&pMyObject->MpoQueue    );
    AnscInitializeLock       (&pMyObject->MpoQueueLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    pMyObject->uReqCounter  = 0;

    return  ANSC_STATUS_SUCCESS;
}
