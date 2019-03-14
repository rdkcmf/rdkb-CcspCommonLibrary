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

    module:	ccsp_aem_plugin.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines the interfaces and XML node names of
        CCSP Application Package Framework Plugin Library.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        11/03/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_AEM_PLUGIN_H
#define  _CCSP_AEM_PLUGIN_H

#include "ccsp_aem.h"
#include "ansc_load_library.h"


#define CCSP_EEREF_OBJ_NAME                 "Device.SoftwareModules.ExecEnv."
#define CCSP_DUREF_OBJ_NAME                 "Device.SoftwareModules.DeploymentUnit."
#define CCSP_EUREF_OBJ_NAME                 "Device.SoftwareModules.ExecutionUnit."


/**************************************************************************************************

CCSP Application Environment Manager (AEM) plug-in XML schema

***************************************************************************************************/
/*
<!-- XML schema for Application Environment Manager (AEM) plugins -->

<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
           targetNamespace="http://www.cisco.com/XMLSchema/cosa_tr069_rpc_ext_1_0" elementFormDefault="unqualified">

  <xs:complexType name="AEPluginType">
    <xs:sequence>
      <xs:element name="lib_Path" type="xs:string"/>

      <!-- plugin initialization and shutdown -->
      <xs:element name="func_Init" type="xs:string"/>
      <xs:element name="func_Shutdown" type="xs:string"/>

      <!-- package operations - install, update, remove -->
      <xs:element name="func_InstallPkg" type="xs:string"/>
      <xs:element name="func_UpdatePkg" type="xs:string"/>
      <xs:element name="func_RemovePkg" type="xs:string"/>

      <!-- EE instance management -->
      <xs:element name="func_GetEeCount" type="xs:string"/>
      <xs:element name="func_GetEeByIndex" type="xs:string"/>
      <xs:element name="func_GetEeInfo" type="xs:string"/>
      <xs:element name="func_SetEeInfo" type="xs:string"/>

      <!-- DU instance management -->
      <xs:element name="func_GetDuCount" type="xs:string"/>
      <xs:element name="func_GetDuByIndex" type="xs:string"/>
      <xs:element name="func_GetDuInfo" type="xs:string"/>
      <xs:element name="func_SetDuInfo" type="xs:string"/>

      <!-- EU instance management -->
      <xs:element name="func_GetEuCount" type="xs:string"/>
      <xs:element name="func_GetEuByIndex" type="xs:string"/>
      <xs:element name="func_GetEuInfo" type="xs:string"/>
      <xs:element name="func_SetEuInfo" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>

  <xs:element name="AemPlugins">
    <xs:complexType>
      <xs:choice>
        <xs:element name="plugin" type="AEPluginType" maxOccurs="unbounded" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

</xs:schema>

*/
/* AEM Plug-in XML Example 
<?xml version="1.0" encoding="UTF-8"?>
< AemPlugins >
<plugin>
    <lib_Path>NAF_APP.so</lib_Path>
    <func_Init>InitNafApp</func_Init>
    <func_Shutdown>ShutdownNafApp</func_Shutdown>
    <func_InstallPkg>InstallNafApp</func_InstallPkg>
    <func_UpdatePkg>UpdateNafApp</func_UpdatePkg>
    <func_RemovePkg>RemoveNafApp</func_RemovePkg>
    <func_GetEeCount>GetNafAppEeCount</func_GetEeCount>
    <func_GetEeByIndex>GetNafAppEeByIndex</func_GetEeByIndex>
    <func_GetEeInfo>GetNafAppEeInfo</func_GetEeInfo>
    <func_SetEeInfo>SetNafAppEeInfo</func_SetEeInfo>
    <func_GetDuCount>GetNafAppDuCount</func_GetDuCount>
    <func_GetDuByIndex>GetNafAppDuByIndex</func_GetDuByIndex>
    <func_GetDuInfo>GetNafAppDuInfo</func_GetDuInfo>
    <func_SetDuInfo>SetNafAppDuInfo</func_SetDuInfo>
    <func_GetEuCount>GetNafAppEuCount</func_GetEuCount>
    <func_GetEuByIndex>GetNafAppEuByIndex</func_GetEuByIndex>
    <func_GetEuInfo>GetNafAppEuInfo</func_GetEuInfo>
    <func_SetEuInfo>SetNafAppEuInfo</func_SetEuInfo>
</plugin>
</AemPlugins>
*/


/* AEM Plugin XML Node Name */
#define  CCSP_AEM_XMLNODE_rootName                   "AemPlugins"
#define  CCSP_AEM_XMLNODE_plugin                     "plugin"
#define  CCSP_AEM_XMLNODE_lib_Path                   "lib_Path"
#define  CCSP_AEM_XMLNODE_func_Init                  "func_Init"
#define  CCSP_AEM_XMLNODE_func_Shutdown              "func_Shutdown"
#define  CCSP_AEM_XMLNODE_func_InstallPkg            "func_InstallPkg"
#define  CCSP_AEM_XMLNODE_func_UpdatePkg             "func_UpdatePkg"
#define  CCSP_AEM_XMLNODE_func_RemovePkg             "func_RemovePkg"
#define  CCSP_AEM_XMLNODE_func_GetEeCount            "func_GetEeCount"
#define  CCSP_AEM_XMLNODE_func_GetEeByIndex          "func_GetEeByIndex"
#define  CCSP_AEM_XMLNODE_func_GetEeInfo             "func_GetEeInfo"
#define  CCSP_AEM_XMLNODE_func_SetEeInfo             "func_SetEeInfo"
#define  CCSP_AEM_XMLNODE_func_GetDuCount            "func_GetDuCount"
#define  CCSP_AEM_XMLNODE_func_GetDuByIndex          "func_GetDuByIndex"
#define  CCSP_AEM_XMLNODE_func_GetDuInfo             "func_GetDuInfo"
#define  CCSP_AEM_XMLNODE_func_SetDuInfo             "func_SetDuInfo"
#define  CCSP_AEM_XMLNODE_func_GetEuCount            "func_GetEuCount"
#define  CCSP_AEM_XMLNODE_func_GetEuByIndex          "func_GetEuByIndex"
#define  CCSP_AEM_XMLNODE_func_GetEuInfo             "func_GetEuInfo"
#define  CCSP_AEM_XMLNODE_func_SetEuInfo             "func_SetEuInfo"


/**************************************************************************************************

CCSP Application Environment Manager (AEM) Structure Definition

***************************************************************************************************/
typedef struct
_CCSP_AEM_EE_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hParent;
    ANSC_HANDLE                     hHandle;
    ULONG                           uInsNumber;
    CCSP_AEM_EE_INFO                eeInfo;
    CCSP_AEM_EE_STATE               eeState;
    CCSP_AEM_EE_RES_USAGE           eeUsage;
    char                            BackAlias[CCSP_AEM_EE_INFO_MAX_ALIAS_LEN+1];
    BOOL                            Reset;
    BOOL                            BackEnabled;
    int                             BackInitialRunLevel;
    int                             RequestedRunLevel;
    char*                           ParentExecEnv;
    char*                           ProcessRefList;
    char*                           ActiveEUs;
}
CCSP_AEM_EE_OBJECT, *PCCSP_AEM_EE_OBJECT;

#define  ACCESS_CCSP_AEM_EE_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_EE_OBJECT, Linkage)

#define  CcspAemInitEEObject(ee)                                                \
         {                                                                      \
            AnscZeroMemory(ee, sizeof(CCSP_AEM_EE_OBJECT));                     \
                                                                                \
            ee->eeUsage.AllocatedDiskSpace = -1;                                \
            ee->eeUsage.AvailableDiskSpace = -1;                                \
            ee->eeUsage.AllocatedMemory    = -1;                                \
            ee->eeUsage.AvailableMemory    = -1;                                \
            ee->eeState.Status             = 1;                                 \
            ee->eeState.Enabled            = TRUE;                              \
            ee->eeState.InitialRunLevel    = 0;                                 \
            ee->eeState.CurrentRunLevel    = -1;                                \
            ee->BackEnabled                = TRUE;                              \
            ee->BackInitialRunLevel        = 0;                                 \
            ee->RequestedRunLevel          = -1;                                \
         } 

#define  CcspAemFreeEEObject(ee)                                                \
         {                                                                      \
            if( ee->ParentExecEnv)  AnscFreeMemory(ee->ParentExecEnv);          \
            if( ee->ProcessRefList) AnscFreeMemory(ee->ProcessRefList);         \
            if( ee->ActiveEUs)      AnscFreeMemory(ee->ActiveEUs);              \
            AnscFreeMemory(ee);                                                 \
         } 

/*********************************************************************************/

typedef struct
_CCSP_AEM_DU_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hParent;
    ANSC_HANDLE                     hEeHandle;
    ANSC_HANDLE                     hHandle;
    ULONG                           uInsNumber;
    CCSP_AEM_DU_META_INFO           duInfo;
    CCSP_AEM_DU_STATE               duState;
    char                            BackAlias[CCSP_AEM_DU_META_INFO_MAX_ALIAS_LEN+1];
    char*                           VendorLogList;
    char*                           VendorConfigList;
    char*                           ExecutionUnitList;
    char*                           ExecutionEnvRef;
    char*                           Username;
    char*                           Password;
}
CCSP_AEM_DU_OBJECT, *PCCSP_AEM_DU_OBJECT;

#define  ACCESS_CCSP_AEM_DU_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_DU_OBJECT, Linkage)

#define  CcspAemInitDUObject(du)                                                \
         {                                                                      \
            AnscZeroMemory(du, sizeof(CCSP_AEM_DU_OBJECT));                     \
            du->duState.Status      = 1;                                        \
                                                                                \
         } 

#if 1
/* Values come from GetDuInfo should not be released by us */
#define  CcspAemFreeDUObject(du)                                                \
         {                                                                      \
            if( du->ExecutionUnitList)AnscFreeMemory(du->ExecutionUnitList);    \
            if( du->ExecutionEnvRef)  AnscFreeMemory(du->ExecutionEnvRef);      \
            if( du->Username)         AnscFreeMemory(du->Username);             \
            if( du->Password)         AnscFreeMemory(du->Password);             \
            AnscFreeMemory(du);                                                 \
         } 
#else
#define  CcspAemFreeDUObject(du)                                                \
         {                                                                      \
            if( du->VendorLogList)    AnscFreeMemory(du->VendorLogList);        \
            if( du->VendorConfigList) AnscFreeMemory(du->VendorConfigList);     \
            if( du->ExecutionUnitList)AnscFreeMemory(du->ExecutionUnitList);    \
            if( du->ExecutionEnvRef)  AnscFreeMemory(du->ExecutionEnvRef);      \
            if( du->Username)         AnscFreeMemory(du->Username);             \
            if( du->Password)         AnscFreeMemory(du->Password);             \
            AnscFreeMemory(du);                                                 \
         } 
#endif
/*********************************************************************************/

typedef struct
_CCSP_AEM_EU_OBJECT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hParent;
    ANSC_HANDLE                     hEeHandle;
    ANSC_HANDLE                     hDuHandle;
    ANSC_HANDLE                     hHandle;
    ULONG                           uInsNumber;
    CCSP_AEM_EU_META_INFO           euInfo;
    CCSP_AEM_EU_STATE               euState;
    CCSP_AEM_EU_RES_USAGE           euUsage;
    char                            BackAlias[CCSP_AEM_EU_META_INFO_MAX_ALIAS_LEN+1];
    BOOL                            BackAutoStart;
    LONG                            BackRunLevel;
    char*                           VendorLogList;
    char*                           VendorConfigList;
    char*                           SupportedDataModelList;
    char*                           ExecutionEnvRef;
    char*                           AssociatedProcessList;
    char*                           References;
    ULONG                           RequestState;
}
CCSP_AEM_EU_OBJECT, *PCCSP_AEM_EU_OBJECT;

#define  ACCESS_CCSP_AEM_EU_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_EU_OBJECT, Linkage)

#define  CcspAemInitEUObject(eu)                                                \
         {                                                                      \
            AnscZeroMemory(eu, sizeof(CCSP_AEM_EU_OBJECT));                     \
                                                                                \
            eu->euUsage.DiskSpaceInUse     = -1;                                \
            eu->euUsage.MemoryInUse        = -1;                                \
            eu->euState.Status             = 1;                                 \
            eu->euState.ExecutionFaultCode = 1;                                 \
         } 

#if 1
/* Values come from GetEuInfo should not be released by us */
#define  CcspAemFreeEUObject(eu)                                                       \
{                                                                                      \
            if( eu->ExecutionEnvRef)        AnscFreeMemory(eu->ExecutionEnvRef);       \
            if( eu->SupportedDataModelList) AnscFreeMemory(eu->SupportedDataModelList);\
            AnscFreeMemory(eu);                                                        \
         } 
#else
#define  CcspAemFreeEUObject(eu)                                                       \
{                                                                                      \
            if( eu->ExecutionEnvRef)        AnscFreeMemory(eu->ExecutionEnvRef);       \
            if( eu->SupportedDataModelList) AnscFreeMemory(eu->SupportedDataModelList);\
            if( eu->VendorLogList)          AnscFreeMemory(eu->VendorLogList);         \
            if( eu->VendorConfigList)       AnscFreeMemory(eu->VendorConfigList);      \
            if( eu->AssociatedProcessList)  AnscFreeMemory(eu->AssociatedProcessList); \
            if( eu->References)             AnscFreeMemory(eu->References);            \
            AnscFreeMemory(eu);                                                        \
         } 
#endif
/**************************************************************************************************

CCSP Application Environment Manager (AEM) plug-in Interface Definition

***************************************************************************************************/
typedef  struct
_CCSP_AEM_PLUGIN_IF
{
    PFN_CCSP_AEMI_INIT_AE           InitPlugin;
    PFN_CCSP_AEMI_SHUTDOWN_AE       ShutPlugin;
    PFN_CCSP_AEMI_INSTALL           InstallPackage;
    PFN_CCSP_AEMI_UPDATE            UpdatePackage;
    PFN_CCSP_AEMI_REMOVE            RemovePackage;
    PFN_CCSP_AEMI_GET_EE_COUNT      GetEeCount;
    PFN_CCSP_AEMI_GET_EE            GetEeByIndex;
    PFN_CCSP_AEMI_GET_EE_INFO       GetEeInfo;
    PFN_CCSP_AEMI_SET_EE_INFO       SetEeInfo;
    PFN_CCSP_AEMI_GET_DU_COUNT      GetDuCount;
    PFN_CCSP_AEMI_GET_DU            GetDuByIndex;
    PFN_CCSP_AEMI_GET_DU_INFO       GetDuInfo;
    PFN_CCSP_AEMI_SET_DU_INFO       SetDuInfo;
    PFN_CCSP_AEMI_GET_EU_COUNT      GetEuCount;
    PFN_CCSP_AEMI_GET_EU            GetEuByIndex;
    PFN_CCSP_AEMI_GET_EU_INFO       GetEuInfo;
    PFN_CCSP_AEMI_SET_EU_INFO       SetEuInfo;
}
CCSP_AEM_PLUGIN_IF,  *PCCSP_AEM_PLUGIN_IF;

/**************************************************************************************************

CCSP Application Environment Manager (AEM) plug-in Object

***************************************************************************************************/
#define  CCSP_AEM_EE_INDEX_UNIT                 10
#define  CCSP_AEM_DU_INDEX_UNIT                 100
#define  CCSP_AEM_EU_INDEX_UNIT                 100

#define  AEM_PLUGIN_LIB_LEN                     64

typedef ANSC_STATUS
(*PFN_CCSP_AEM_PLUGIN_LOAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlHandle
    );

typedef ANSC_STATUS
(*PFN_CCSP_AEM_PLUGIN_COMMIT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHandle
    );

typedef ANSC_STATUS
(*PFN_CCSP_AEM_PLUGIN_INSTALL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHandle
    );


typedef ULONG
(*PFN_CCSP_AEM_PLUGIN_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_HANDLE
(*PFN_CCSP_AEM_PLUGIN_GET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PULONG                      puInsNumber
    );

typedef ANSC_STATUS
(*PFN_CCSP_AEM_PLUGIN_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_HANDLE
(*PFN_CCSP_AEM_PLUGIN_SEARCH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uInsNumber
    );

typedef ANSC_HANDLE
(*PFN_CCSP_AEM_PLUGIN_SBYID)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       UUID
    );

#define  CCSP_AEM_PLUGINOBJ_CLASS_CONTENT                        \
    /* start of object class content */                          \
    SINGLE_LINK_ENTRY               Linkage;                     \
    ANSC_HANDLE                     hAppEnvMgr;                  \
    char                            LibPath[AEM_PLUGIN_LIB_LEN]; \
    CCSP_AEM_PLUGIN_IF              ifPlugin;                    \
    ULONG                           uIndexBase;                  \
    ANSC_HANDLE                     hInstance;                   \
    QUEUE_HEADER                    AppEUQueue;                  \
    QUEUE_HEADER                    AppDUQueue;                  \
    QUEUE_HEADER                    AppEEQueue;                  \
                                                                 \
    PFN_CCSP_AEM_PLUGIN_LOAD        LoadPluginInfo;              \
    PFN_CCSP_AEM_PLUGIN_ACTION      PopulateDataModel;           \
    PFN_CCSP_AEM_PLUGIN_ACTION      LoadConfig;                  \
    PFN_CCSP_AEM_PLUGIN_ACTION      SaveConfig;                  \
    PFN_CCSP_AEM_PLUGIN_ACTION      CleanAll;                    \
    PFN_CCSP_AEM_PLUGIN_ACTION      SynTables;                   \
                                                                 \
    PFN_CCSP_AEM_PLUGIN_GET_ULONG   GetEEEntryCount;             \
    PFN_CCSP_AEM_PLUGIN_GET_HANDLE  GetEEEntry;                  \
    PFN_CCSP_AEM_PLUGIN_GET_ULONG   GetDUEntryCount;             \
    PFN_CCSP_AEM_PLUGIN_GET_HANDLE  GetDUEntry;                  \
    PFN_CCSP_AEM_PLUGIN_GET_ULONG   GetEUEntryCount;             \
    PFN_CCSP_AEM_PLUGIN_GET_HANDLE  GetEUEntry;                  \
    PFN_CCSP_AEM_PLUGIN_COMMIT      UninstallPackage;            \
    PFN_CCSP_AEM_PLUGIN_INSTALL     InstallPackage;              \
    PFN_CCSP_AEM_PLUGIN_COMMIT      CommitEeInfo;                \
    PFN_CCSP_AEM_PLUGIN_COMMIT      CommitDuInfo;                \
    PFN_CCSP_AEM_PLUGIN_COMMIT      CommitEuInfo;                \
    PFN_CCSP_AEM_PLUGIN_SEARCH      SearchEEEntry;               \
    PFN_CCSP_AEM_PLUGIN_SBYID       SearchDUEntry;               \
    PFN_CCSP_AEM_PLUGIN_SEARCH      GetDUByIns;                  \
    PFN_CCSP_AEM_PLUGIN_SEARCH      GetEUByIns;                  \
    PFN_CCSP_AEM_PLUGIN_ACTION      Remove;                      \
    /* end of object class content */                            \

typedef  struct
_CCSP_AEM_PLUGIN_OBJECT
{
    CCSP_AEM_PLUGINOBJ_CLASS_CONTENT
}
CCSP_AEM_PLUGIN_OBJECT,  *PCCSP_AEM_PLUGIN_OBJECT;

#define  ACCESS_CCSP_AEM_PLUGIN_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_PLUGIN_OBJECT, Linkage)

/***********************************************************
 CCSP AEM Plugin External APIs
***********************************************************/
ANSC_HANDLE
CcspCreateAemPluginObject
    (
    );

void
CcspFreeAemPluginObject
    (
        ANSC_HANDLE                 hHandle
    );

#endif
