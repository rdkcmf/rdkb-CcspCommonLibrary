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

    module: dslh_dmagnt_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh DataModel Agent
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/04/2010    initial revision.
        02/08/2011    changed the internal "DataModelLibrary" table
                      to a dynamic table.
        02/15/2011    added "SupportedDataModel" support;
        03/21/2011    added api to retrieve instance number by index

**********************************************************************/


#ifndef  _DSLH_DMAGNT_INTERNAL_API_
#define  _DSLH_DMAGNT_INTERNAL_API_

#define _DATA_MODEL_XML_PATH                 "/tr106_DeviceType.xml"

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_DMAGNT_OPERATION.C
***********************************************************/

ANSC_STATUS
DslhDmagntEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhDmagntCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_DMAGNT_CONTROL.C
***********************************************************/
ANSC_STATUS
DslhDmagntLoadDataModelXML
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhDmagntLoadDataModelXML2
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhDmagntLoadDataModelXMLInternal
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pXMLNode,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhDmagntUnloadDataModelXML
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength
    );

ANSC_STATUS
DslhDmagntRegisterInternalApi
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pApiName,
        ANSC_HANDLE                 hRegApi
    );

ANSC_STATUS
DslhDmagntMemoryUsage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhDmagntMemoryTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhDmagntMemoryCheck
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhDmagntGetFcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhDmagntSetFcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
DslhDmagntRegBaseDataModel
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCompName
    );

ANSC_STATUS
DslhDmagntGenerateDTXmlString
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_DMAGNT_UTILITIES.C
***********************************************************/
BOOL
DslhDmagntInitManagerServerInfo
    (
    );

ANSC_HANDLE
DslhDmagntCreatePluginInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhDmagntRemovePluginInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPluginInfo
    );

ANSC_STATUS
DslhDmagntParseSourceInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    );

ANSC_STATUS
DslhDmagntParseSourceName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        char*                       pName
    );

ANSC_STATUS
DslhDmagntRegisterDataModelObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName,
        ANSC_HANDLE                 hPluginInfo,
        BOOL                        bPopulateTree
    );

ANSC_STATUS
DslhDmagntUnloadDataModelObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName
    );

BOOL
DslhDmagntCheckApiResolved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    );

BOOL
DslhDmagntCheckBaseObjNames
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRootName,
        ANSC_HANDLE                 hXMLNode
    );

ANSC_HANDLE
DslhDmagntParseObjFuncInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    );

ANSC_HANDLE
DslhDmagntParseRootFuncInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hAtomFunc
    );

ANSC_STATUS
DslhDmagntParseObjectInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName,
        ANSC_HANDLE                 hPluginInfo,
        ANSC_HANDLE                 hObjectDesp
    );

ANSC_HANDLE
DslhDmagntParseParamArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    );

ANSC_STATUS
DslhDmagntParseParamInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamNode,
        ANSC_HANDLE                 hPluginInfo,
        ANSC_HANDLE                 hParamDesp
    );

BOOL
GetParamUlongValue_X_CISCO_COM_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
GetEntryCount_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
GetEntry_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        PULONG                      pInsNumber
    );

BOOL
IsUpdated_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Synchronize_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
GetParamUlongValue_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

int
GetParamStringValue_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    ManagementServer.

    *  ManagementServer_GetParamBoolValue
    *  ManagementServer_GetParamIntValue
    *  ManagementServer_GetParamUlongValue
    *  ManagementServer_GetParamStringValue
    *  ManagementServer_SetParamBoolValue
    *  ManagementServer_SetParamIntValue
    *  ManagementServer_SetParamUlongValue
    *  ManagementServer_SetParamStringValue
    *  ManagementServer_Validate
    *  ManagementServer_Commit
    *  ManagementServer_Rollback

***********************************************************************/

BOOL
ManagementServer_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
ManagementServer_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
ManagementServer_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
ManagementServer_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
ManagementServer_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
ManagementServer_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
ManagementServer_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
ManagementServer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
ManagementServer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
ManagementServer_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
ManagementServer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.SupportedDataModel.{i}.

    *  SupportedDataModel_GetEntryCount
    *  SupportedDataModel_GetEntry
    *  SupportedDataModel_GetParamStringValue

***********************************************************************/
ULONG
SupportedDataModel_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
SupportedDataModel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ULONG
SupportedDataModel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );


/***********************************************************************

 APIs for CCSP internal base data model:

    com.cisco.spvtg.ccsp.XXX.

    *  CcspComp_GetParamUlongValue 
    *  CcspComp_GetParamStringValue 
    *  CcspComp_GetBulkParamValues
    *  CcspComp_SetParamBoolValue
    *  CcspLog_GetParamBoolValue
    *  CcspLog_GetParamUlongValue
    *  CcspLog_SetParamBoolValue
    *  CcspLog_SetParamUlongValue
    *  CcspLog_Validate
    *  CcspLog_Commit
    *  CcspLog_Rollback
    *  CcspMem_GetParamUlongValue 
    *  CcspLog_GetBulkParamValues
    *  CcspLog_SetBulkParamValues

***********************************************************************/
BOOL
CcspComp_GetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    );

BOOL
CcspComp_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
CcspComp_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
CcspComp_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
CcspLog_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
CcspLog_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

BOOL
CcspLog_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
CcspLog_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
CcspLog_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
CcspLog_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
CcspLog_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
CcspMem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

BOOL
CcspLog_GetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    );

BOOL
CcspLog_SetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize,
        PULONG                      pUlErrorIndex
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_DMAGNT_EXPORTED.C
***********************************************************/

ANSC_STATUS
COSANotifyValueChange
    (
        void*                       hDmlAgent,
        char*                       pParamName   
    );

BOOL
COSANotifyDiagComplete
    (
        void*                       hDmlAgent
    );

BOOL
COSAValidateHierarchyInterface
    (
        void*                       hDmlAgent,
        char*                       pHierarchName,
        ULONG                       uType
    );

BOOL
COSAGetParamValueBool
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

int
COSAGetParamValueInt
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

ULONG
COSAGetParamValueUlong
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

void*
COSAGetParamValue
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

int
COSAGetParamValueString
    (
        void*                       hDmlAgent,
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    );

ANSC_STATUS
COSASetParamValueBool
    (
        char*                       pParamName,
        BOOL                        bParamValue
    );

ANSC_STATUS
COSASetParamValueInt
    (
        char*                       pParamName,
        int                         iParamValue
    );

ANSC_STATUS
COSASetParamValueUlong
    (
        char*                       pParamName,
        ULONG                       ulParamValue
    );

ANSC_STATUS
COSASetParamValueString
    (
        char*                       pParamName,
        char*                       pParamValue
    );


ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                       bus_handle,
        parameterValStruct_t        *val,
        ULONG                       *parameterValueLength
    );

ANSC_STATUS
COSASetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val
    );

ANSC_STATUS
COSAGetInstanceNumbers
    (
        char*                      pObjName,
        ULONG*                     pInsList,
        ULONG*                     pInsCnt
    );

void*
COSACreateSlapObject
    (
        void*                       hSlapObjDescr,
        void*                       hInsContext
    );

ULONG
COSAReleaseSlapObject
    (
        void*                       hSlapObj,
        void*                       hSlapObjDescr
    );

void*
COSAAcquirePoamObject
    (
        void*                       hSlapObjDescr
    );

ULONG
COSAReleasePoamObject
    (
        void*                       hPoamObj
    );

ULONG
COSAGetLPCRole
    (
    );

void*
COSAGetRegistryRootFolder
    (
        void*                       hDmlAgent
    );

/* retrieve each entry's instance number by index */
ULONG
COSAGetInstanceNumberByIndex
    (
        void*                       hDmlAgent,
        char*                      pObjName,
        ULONG                      ulIndex
    );

void*
COSAGetMessageBusHandle
    (
        void*                       hDmlAgent
    );

void*
COSAGetInterfaceByName
    (
        void*                       hDmlAgent,
        char*                     pIfName
    );

void *
COSARegisterCallBackAfterInitDml
    (
        void*                       hDmlAgent,
        ANSC_HANDLE  handle
    );

char*
COSAGetSubsystemPrefix
    (
        void*                       hDmlAgent
    );

char*
COSAGetSubsystemPrefix2
    (
    );

BOOL
COSARepopulateTable
    (
        void*                       hDmlAgent,
        char*        pTabName
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_DMAGNT_MWSIF.C
***********************************************************/
char*
COSAMwsIfGetResource
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUrlPath,           /* absolute URL path to web root */
        char*                       pMediaType,         /* OUT - media type associated with the URL, used to indicate content-type */
        PULONG                      pulMediaTypeLength, /* IN/OUT */
        PULONG                      pulResourceLen      /* OUT - length of resource */
    );
    
#endif
