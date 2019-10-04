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

    module: dslh_cpeco_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Cpe Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin  Zhu

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.
        02/12/09    add more statistics apis
        01/05/11    Bin moved "ValidateHierarchyInterface" from ObjController object;

**********************************************************************/


#ifndef  _DSLH_CPECO_INTERNAL_API_
#define  _DSLH_CPECO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_CPECO_STATES.C
***********************************************************/

ANSC_HANDLE
DslhCpecoGetDslhWmpDatabase
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
DslhCpecoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
DslhCpecoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoAddInterface
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
DslhCpecoGetInterfaceByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ifName
    );

ANSC_STATUS
DslhCpecoSetParameterArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParameterArray,
        ANSC_HANDLE                 hParameterHolder,
        ULONG                       uCount
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_CPECO_ACCESS.C
***********************************************************/

ANSC_HANDLE
DslhCpecoGetDslhMprIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhCpecoGetDslhMpaIf
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
DslhCpecoGetDslhMbiIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoSetDslhMbiIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
DslhCpecoSetDslhCcdIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
DslhCpecoSetDbusHandle
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHandle
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_CPECO_OPERATION.C
***********************************************************/

ANSC_STATUS
DslhCpecoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_CPECO_CONTROL.C
***********************************************************/

ANSC_STATUS
DslhCpecoLoadEvents
    (
        ANSC_HANDLE                 hDslhMsoIf,
        char*                       pSavedEvents
    );


ANSC_STATUS
DslhCpecoInitDataModel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoRegisterAllParams
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhCpecoRegisterObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr,
        ANSC_HANDLE                 hParamArray
    );

ANSC_STATUS
DslhCpecoRegisterHiddenObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr,
        ANSC_HANDLE                 hParamArray
    );


ANSC_STATUS
DslhCpecoRegisterObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        void*                       pfnObjcoConstructor,
        ANSC_HANDLE                 hParamArray
    );

ANSC_STATUS
DslhCpecoRegisterHiddenObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        void*                       pfnObjcoConstructor,
        ANSC_HANDLE                 hParamArray
    );

ANSC_STATUS
DslhCpecoInformNow
    (
        ANSC_HANDLE                 hThisObject
    );

void
DslhCpecoNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hEventContext
    );
    
ANSC_STATUS
DslhCpecoLoadExternalDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhCpecoLoadExternalDMLibFile2
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhCpecoLoadInternalDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    );

ANSC_STATUS
DslhCpecoUnloadDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    );

ANSC_STATUS
DslhCpecoLoadRpcExtXMLFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    );

BOOL
DslhCpecoValidateHierarchyInterface
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pHierarchName,
        ULONG                       uType
    );

ANSC_STATUS
DslhCpecoRegisterDataModel
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        char*                       pXMLFile,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    );

ANSC_STATUS
DslhCpecoRegisterDataModel2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        void*                       pfnXMLLoader,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    );

ANSC_STATUS
DslhCpecoRegisterDataModelInternal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        char*                       pXMLFile,
        void*                       pfnXMLLoader,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_CPECO_STAIF.C
***********************************************************/
void
DslhCpecoStatIfConnectAcs
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfIncTcpFailure
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfIncTlsFailure
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfIncTcpSuccess
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfGetTcpSuccessCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfGetTcpFailureCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfGetTlsFailureCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhCpecoStatIfResetStats
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhCpecoStatIfGetLastConnectionTime
    (
        ANSC_HANDLE                 hThisObject
    );

char*
DslhCpecoStatIfGetLastConnectionStatus
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhCpecoStatIfGetLastInformResponseTime
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhCpecoStatIfGetLastReceivedSPVTime
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
