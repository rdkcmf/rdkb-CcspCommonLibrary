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

    module:	dslh_wmpdo_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Wmp Database
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin  Zhu

    ---------------------------------------------------------------

    revision:

        09/09/2005    initial revision.
        01/06/2011    Bin added "ResetNotifyParam" to ignore change from ACS.
        02/07/2011    Bin added "UnregisterObject" support;
        02/28/2011    Bin added a set of ReqCounter apis to better support dynamic tables;

**********************************************************************/


#ifndef  _DSLH_WMPDO_INTERNAL_API_
#define  _DSLH_WMPDO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_STATES.C
***********************************************************/

ANSC_HANDLE
DslhWmpdoGetDslhMpaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhWmpdoGetDslhMprIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhWmpdoGetSlapVarConverter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhWmpdoGetSlapVarMapper
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhWmpdoGetDslhCpeController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoSetDslhCpeController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCpeController
    );

ANSC_STATUS
DslhWmpdoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
DslhWmpdoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
DslhWmpdoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_OPERATION.C
***********************************************************/

ANSC_STATUS
DslhWmpdoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoMonitorTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_CONTROL.C
***********************************************************/

ANSC_STATUS
DslhWmpdoCreateRecordTree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoConfigRecordTree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoEnableMonitor
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_UTILITIES.C
***********************************************************/

ANSC_STATUS
DslhWmpdoParseParamDataType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDataTypeName,
        PULONG                      pulDataType,
        PULONG                      pulFormatValue1,
        PULONG                      pulFormatValue2,
        char**                      ppEnumTokens
    );

ANSC_STATUS
DslhWmpdoParseParamSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSyntaxString,
        PULONG                      pulSyntax,
        PULONG                      pulContentType
    );

ANSC_STATUS
DslhWmpdoParseParamEnumTokens
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity,
        char*                       pEnumTokens
    );

ANSC_STATUS
DslhWmpdoInitObjVarArray
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoResetObjVarArray
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhWmpdoAddVarRecordIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

ULONG
DslhWmpdoAddObjRecordIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

ANSC_STATUS
DslhWmpdoFlushDynObjVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDynObjRecord
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_MPRIF.C
***********************************************************/

ANSC_STATUS
DslhWmpdoMprRegParameter
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamDescr,
        char*                       pObjName
    );

ANSC_STATUS
DslhWmpdoMprRegNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        BOOL                        bActiveNotification
    );

ANSC_STATUS
DslhWmpdoMprDelNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

ANSC_STATUS
DslhWmpdoMprModNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord,
        ULONG                       ulLimit
    );

ANSC_STATUS
DslhWmpdoMprResetNotifyParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

ANSC_STATUS
DslhWmpdoMprRegObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    );

ANSC_STATUS
DslhWmpdoMprUnregisterObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjFullName,
        char*                       pLastName,
        BOOL                        bIsTable
    );

ULONG
DslhWmpdoMprGetVarDataType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_HANDLE
DslhWmpdoMprGetObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

ANSC_HANDLE
DslhWmpdoMprGetVarRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_HANDLE
DslhWmpdoMprGetVarRecord2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    );

ANSC_HANDLE
DslhWmpdoMprGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

ANSC_HANDLE
DslhWmpdoMprGetObjRecordInstance
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

SLAP_VARIABLE*
DslhWmpdoMprGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

SLAP_VARIABLE*
DslhWmpdoMprGetParamValue2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_STATUS
DslhWmpdoMprSetParamValue2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_STATUS
DslhWmpdoMprSetParamValue3
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_STATUS
DslhWmpdoMprSetParamValue4
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

char*
DslhWmpdoMprGetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

int
DslhWmpdoMprGetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        int                         iParamValue
    );

ULONG
DslhWmpdoMprGetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        ULONG                       ulParamValue
    );

BOOL
DslhWmpdoMprGetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        BOOL                        bParamValue
    );

char*
DslhWmpdoMprGetParamValueTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

char*
DslhWmpdoMprGetParamValueBase64
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetParamValueBase64
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamName,
        char*                       pParamValue
    );

ULONG
DslhWmpdoMprGetEntryObjectCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord
    );

ANSC_HANDLE
DslhWmpdoMprGetEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTableObjRecord,
        ULONG                       ulIndex
    );

SLAP_VARIABLE*
DslhWmpdoMprGetEntryParamValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pParamName
    );

ANSC_STATUS
DslhWmpdoMprSetEntryParamValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord,
        char*                       pAccessEntity,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_STATUS
DslhWmpdoMprApplyEntryChanges
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObjRecord
    );

char*
DslhWmpdoMprGetRootObjName
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhWmpdoMprGetReqCounter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoMprIncReqCounter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhWmpdoMprRegEmptyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

/***********************************************************
         FUNCTIONS IMPLEMENTED IN DSLH_WMPDO_MPAIF.C
***********************************************************/

BOOL
DslhWmpdoMpaLockWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity
    );

ANSC_STATUS
DslhWmpdoMpaUnlockWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    );

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
    );

ANSC_STATUS
DslhWmpdoMpaSetCommit
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        int*                        piStatus,
        int                         sessionId,
        ULONG                       writeID,
        BOOL                        bCommit
    );

ANSC_STATUS
DslhWmpdoMpaGetParameterValues
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        SLAP_STRING_ARRAY*          pParamNameArray,
		ULONG						uMaxEntry,
        void**                      ppParamValueArray,
        PULONG                      pulArraySize,
        unsigned int                writeID
    );

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

ANSC_STATUS
DslhWmpdoMpaSetParameterAttributes
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        void*                       pSetParamAttribArray,
        ULONG                       ulArraySize
    );

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

ANSC_STATUS
DslhWmpdoMpaAddObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        PULONG                      pulObjInsNumber,
        int*                        piStatus
    );

ANSC_STATUS
DslhWmpdoMpaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        int*                        piStatus
    );


#endif
