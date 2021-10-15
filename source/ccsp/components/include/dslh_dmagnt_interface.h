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

    module: dslh_dmagnt_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh DataModelAgent Object;

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin     Zhu 

    ---------------------------------------------------------------

    revision:

        11/01/2010    initial revision.
        09/15/2011    added LibCallback interface;

**********************************************************************/


#ifndef  _DSLH_DMAGNT_INTERFACE_
#define  _DSLH_DMAGNT_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "cosa_plugin_api.h"

/* COSA current supported version */
#define COSA_CURRENT_SUPPORT_VERSION              1

/* COSA notify string definition */
#define COSA_XML_notify_off                       "off"
#define COSA_XML_notify_configurable              "configurable"
#define COSA_XML_notify_configPassive             "configPassive"
#define COSA_XML_notify_on                        "on"

#define COSA_XML_notify_alwaysOff                 "alwaysOff"
#define COSA_XML_notify_alwaysOn                  "alwaysOn"

#define COSA_XML_bsUpdate_firmware                "firmware"
#define COSA_XML_bsUpdate_rfcUpdate               "rfcUpdate"
#define COSA_XML_bsUpdate_allUpdate               "allUpdate"

/* COSA object type definition */
#define COSA_XML_type_object                      "object"
#define COSA_XML_type_staticTable                 "staticTable"
#define COSA_XML_type_dynamicTable                "dynamicTable"
#define COSA_XML_type_writableTable               "writableTable"
#define COSA_XML_type_dynWritableTable            "dynWritableTable"

/* COSA XML node name definitions */
#define  COSA_XML_NODE_dataModelInfo              "dataModelInfo"
#define  COSA_XML_NODE_rootName                   "rootName"
#define  COSA_XML_NODE_version                    "version"
#define  COSA_XML_NODE_moduleName                 "moduleName"
#define  COSA_XML_NODE_author                     "author"
#define  COSA_XML_NODE_description                "description"
#define  COSA_XML_NODE_library                    "library"
#define  COSA_XML_NODE_slapObjName                "slapObjName"
#define  COSA_XML_NODE_objects                    "objects"
#define  COSA_XML_NODE_object                     "object"
#define  COSA_XML_NODE_parameters                 "parameters"
#define  COSA_XML_NODE_parameter                  "parameter"
#define  COSA_XML_NODE_functions                  "functions"
#define  COSA_XML_NODE_objectType                 "objectType"
#define  COSA_XML_NODE_name                       "name"
#define  COSA_XML_NODE_baseObjName                "baseObjName"
#define  COSA_XML_NODE_maxInstance                "maxInstance"
#define  COSA_XML_NODE_func_GetEntryCount         "func_GetEntryCount"
#define  COSA_XML_NODE_func_GetEntryStatus        "func_GetEntryStatus"
#define  COSA_XML_NODE_func_GetEntry              "func_GetEntry"
#define  COSA_XML_NODE_func_AddEntry              "func_AddEntry"
#define  COSA_XML_NODE_func_DelEntry              "func_DelEntry"
#define  COSA_XML_NODE_func_IsUpdated             "func_IsUpdated"
#define  COSA_XML_NODE_func_Synchronize           "func_Synchronize"
#define  COSA_XML_NODE_func_Lock                  "func_Lock"
#define  COSA_XML_NODE_func_Unlock                "func_Unlock"
#define  COSA_XML_NODE_func_CheckInstance         "func_CheckInstance"
#define  COSA_XML_NODE_func_GetParamBoolValue     "func_GetParamBoolValue"
#define  COSA_XML_NODE_func_GetParamIntValue      "func_GetParamIntValue"
#define  COSA_XML_NODE_func_GetParamUlongValue    "func_GetParamUlongValue"
#define  COSA_XML_NODE_func_GetParamStringValue   "func_GetParamStringValue"
#define  COSA_XML_NODE_func_GetParamBulkValues    "func_GetBulkParamValues"
#define  COSA_XML_NODE_func_SetParamBoolValue     "func_SetParamBoolValue"
#define  COSA_XML_NODE_func_SetParamIntValue      "func_SetParamIntValue"
#define  COSA_XML_NODE_func_SetParamUlongValue    "func_SetParamUlongValue"
#define  COSA_XML_NODE_func_SetParamStringValue   "func_SetParamStringValue"
#define  COSA_XML_NODE_func_SetParamBulkValues    "func_SetBulkParamValues"
#define  COSA_XML_NODE_func_Validate              "func_Validate"
#define  COSA_XML_NODE_func_Commit                "func_Commit"
#define  COSA_XML_NODE_func_Rollback              "func_Rollback"
#define  COSA_XML_NODE_lib_path                   "path"
#define  COSA_XML_NODE_lib_init                   "func_Init"
#define  COSA_XML_NODE_lib_async_init             "func_Async_Init"
#define  COSA_XML_NODE_lib_unload                 "func_Unload"
#define  COSA_XML_NODE_lib_memorycheck            "func_MemoryCheck"
#define  COSA_XML_NODE_lib_memoryusage            "func_MemoryUsage"
#define  COSA_XML_NODE_lib_memorytable            "func_MemoryTable"
#define  COSA_XML_NODE_lib_support                "func_IsObjSupported"
#define  COSA_XML_NODE_lib_inProcess              "inProcess"
#define  COSA_XML_NODE_libFunctions               "libFunctions"

#define  COSA_XML_NODE_param_name                 "name"
#define  COSA_XML_NODE_param_type                 "type"
#define  COSA_XML_NODE_param_syntax               "syntax"
#define  COSA_XML_NODE_param_bsUpdate             "bsUpdate"
#define  COSA_XML_NODE_param_notify               "notify"
#define  COSA_XML_NODE_param_notifyThrdEnabled    "notifyThresholdEnabled"
#define  COSA_XML_NODE_param_notifyThresholdMin   "notifyThresholdMin"
#define  COSA_XML_NODE_param_notifyThresholdMax   "notifyThresholdMax"
#define  COSA_XML_NODE_param_persistent           "persistent"
#define  COSA_XML_NODE_param_invisible            "invisible"
#define  COSA_XML_NODE_param_rebootNeeded         "rebootNeeded"
#define  COSA_XML_NODE_param_writable             "writable"
#define  COSA_XML_NODE_param_monitor              "monitor"
#define  COSA_XML_NODE_param_func_GetValue        "func_GetValue"
#define  COSA_XML_NODE_param_CharsToRestrict      "restrictedChars"

/* COSA IrepFolder Name */
#define COSA_IREP_FOLDER_NAME                     "COSALibrary"


/* COSA status definitions */
#define  COSA_STATUS_SUCCESS                      0x00000000
#define  COSA_STATUS_PENDING                      0x00000001
#define  COSA_STATUS_ERROR_LOAD_LIBRARY           0x00000002
#define  COSA_STATUS_ERROR_GET_PROC               0x00000003
#define  COSA_STATUS_ERROR_INIT                   0x00000004
#define  COSA_STATUS_ERROR_VERSION                0x00000005
#define  COSA_STATUS_ERROR_UNRESOLVED_API         0x00000006
#define  COSA_STATUS_ERROR_UNKNOWN_OBJ_NAME       0x00000007
#define  COSA_STATUS_ERROR_REG_FAILURE            0x00000008
#define  COSA_STATUS_ERROR_RESOURCES              0x00000009
#define  COSA_STATUS_FAILURE                      0xFFFFFFFF

typedef  struct
_PLUGIN_INFORMATION
{
    SINGLE_LINK_ENTRY               Linkage; 
    ULONG                           uLoadStatus;
    ULONG                           Version;
    char*                           ModuleName;
    char*                           Author;
    char*                           Description;
    char*                           SourceName;
    ANSC_HANDLE                     hInstance;
    COSAInitProc                    InitProc;
    COSAInitProc                    AsyncInitProc;    
    COSASupportProc                 SupportProc;
    COSAUnloadProc                  UnloadProc;
    COSAMemoryCheckProc             MemoryCheckProc;
    COSAMemoryUsageProc             MemoryUsageProc;
    COSAMemoryTableProc             MemoryTableProc;
    ANSC_HANDLE                     hAtomFunc;
    ULONG                           uRegParamCount;
    ULONG                           uRegObjCount;
    COSA_PLUGIN_INFO                cosaPlugin;
    ANSC_HANDLE                     hDslhTr69If;
    BOOL                            bFromSlap;
    ANSC_HANDLE                     hPoamObj;
    ANSC_HANDLE                     hReserved;
}
PLUGIN_INFORMATION,  *PPLUGIN_INFORMATION;

#define  ACCESS_PLUGIN_INFORMATION(p)                         \
         ACCESS_CONTAINER(p, PLUGIN_INFORMATION, Linkage)

typedef  ANSC_STATUS
(*PFN_DML_CALLBACK)
    (
        ANSC_HANDLE                 hContext
    );


typedef  struct
_DSLHDMAGNT_CALLBACK
{
    SINGLE_LINK_ENTRY               Linkage; 
    PFN_DML_CALLBACK                func;
}
DSLHDMAGNT_CALLBACK,  *PDSLHDMAGNT_CALLBACK;

#define  ACCESS_DSLHDMAGNT_CALLBACK(p)                         \
         ACCESS_CONTAINER(p, DSLHDMAGNT_CALLBACK, Linkage)

/***********************************************************
    DSLH LIBRARY CALLBACK SERVICE INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  CCSP_LIBCBK_INTERFACE_NAME                    "dslhLibCallbackSvcIf"
#define  CCSP_LIBCBK_INTERFACE_ID                      0

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  DSLH_LCB_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    COSAInitProc                    InitLibrary;                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_LCB_INTERFACE
{
    DSLH_LCB_INTERFACE_CLASS_CONTENT
}
DSLH_LCB_INTERFACE,  *PDSLH_LCB_INTERFACE;

#define  ACCESS_DSLH_LCB_INTERFACE(p)               \
         ACCESS_CONTAINER(p, DSLH_LCB_INTERFACE, Linkage)

/***********************************************************
       DSLH DATAMODELAGENT COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_REGBASE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCompName
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_LOAD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_LOAD2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    );


typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_MEMORY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_REGAPI)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pApiName,
        ANSC_HANDLE                 hRegApi
    );

typedef  ANSC_HANDLE
(*PFN_DSLHDMAGNT_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHDMAGNT_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

#define  DSLH_DATAMODEL_AGENT_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    QUEUE_HEADER                    sPluginInfoList;                                        \
    ANSC_HANDLE                     hAtomFunctions;                                         \
    ANSC_HANDLE                     hIrepFolderCOSA;                                        \
    ANSC_HANDLE                     hDslhMwsIf;                                             \
    ANSC_HANDLE                     hFcContext;                                             \
    ANSC_HANDLE                     hDbusHandle;                                            \
    ANSC_HANDLE                     hDslhCcdIf;                                             \
    ANSC_HANDLE                     hDslhLcbIf;                                             \
    BOOL                            bPluginUpdated;                                         \
    char*                           pPrefix;                                                \
    char*                           pDTXml;                                                 \
                                                                                            \
    BOOL                            bActive;                                                \
    BOOL                            bMonitorParamLoaded;                                    \
    BOOL                            bGlobalMonitor;                                         \
    ULONG                           uMonitorComponents;                                     \
                                                                                            \
    PFN_DSLHDMAGNT_ACTION           Engage;                                                 \
    PFN_DSLHDMAGNT_ACTION           Cancel;                                                 \
                                                                                            \
    PFN_DSLHDMAGNT_REGBASE          RegBaseDataModel;                                       \
    PFN_DSLHDMAGNT_LOAD             LoadDataModelXML;                                       \
    PFN_DSLHDMAGNT_LOAD2            LoadDataModelXML2;                                      \
    PFN_DSLHDMAGNT_UNLOAD           UnloadDataModelXML;                                     \
    PFN_DSLHDMAGNT_MEMORY           MemoryUsage;                                            \
    PFN_DSLHDMAGNT_MEMORY           MemoryTable;                                            \
    PFN_DSLHDMAGNT_MEMORY           MemoryCheck;                                            \
    PFN_DSLHDMAGNT_REGAPI           RegisterInternalApi;                                    \
    PFN_DSLHDMAGNT_GET_CONTEXT      GetFcContext;                                           \
    PFN_DSLHDMAGNT_SET_CONTEXT      SetFcContext;                                           \
    PFN_DSLHDMAGNT_ACTION           GenerateDTXmlString;                                    \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_DATAMODEL_AGENT_OBJECT
{
    DSLH_DATAMODEL_AGENT_CLASS_CONTENT
}
DSLH_DATAMODEL_AGENT_OBJECT,  *PDSLH_DATAMODEL_AGENT_OBJECT;

#define  ACCESS_DSLH_DATAMODEL_AGENT_OBJECT(p)       \
         ACCESS_CONTAINER(p, DSLH_DATAMODEL_AGENT_OBJECT, Linkage)


#endif
