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

    module: cosa_plugin_api.h

        The apis defined to import TR69 Data Model in
        Cisco OpenService Architecture (COSA). 

    ---------------------------------------------------------------

    description:

        This file defines the interface and apis to import TR069 Data
        Model.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/01/2010    initial revision.

**********************************************************************/


#ifndef  _COSA_PLUGIN_API_H
#define  _COSA_PLUGIN_API_H

#include "ccsp_base_api.h"

/***********************************************************
          COSA STRUCTURE DEFINTIONS
***********************************************************/

typedef BOOL
(* COSARegisterProc)
    (
        void*                       hContext,    /* this will be the handle passed in by COSA_PLUGIN_INFO */
        char*                       pApiName,
        void*                       hModuleProc                  
    );

typedef void*
(* COSAAcquireProc)
   (
       char*                       pApiName
   );


typedef  struct
_COSA_PLUGIN_INFO
{
    ULONG                           uPluginVersion;
    COSAAcquireProc                 AcquireFunction;
    COSARegisterProc                RegisterFunction;
    void*                           hContext;
    void*                           hFcContext;       /* Handle of Functional Component */
    void*                           hDmlAgent;
}
COSA_PLUGIN_INFO,  *PCOSA_PLUGIN_INFO;


/***********************************************************
          COSA_PLUGIN_IMPORT_API DEFINITIONS
***********************************************************/
typedef int (* COSAInitProc)
    (
        ULONG                       uMaxVersionSupported, 
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    );

typedef BOOL (* COSASupportProc)
    (
        char*                       pObjectName
    );

typedef void (* COSAUnloadProc)
    (
        void
    );

typedef int (* COSAMemoryCheckProc)
    (
        void
    );

typedef int (* COSAMemoryUsageProc)
    (
        void
    );

typedef int (* COSAMemoryTableProc)
    (
        void
    );

/**************************************************************************************
  COSA FRAMEWORK AVAILABLE APIS LIST

  *  COSANotifyValueChange
  *  COSANotifyDiagnosticsComplete
  *  COSAValidateHierachyInterface
  *  COSAGetParamValueBool
  *  COSAGetParamValueInt
  *  COSAGetParamValueUlong
  *  COSAGetParamValueString
  *  COSAGetDiagPluginInfo
  *  COSACreateSlapObject
  *  COSAAcquirePoamObject
  *  COSAReleasePoamObject
  *  COSAGetRegistryRootFolder
  
  Description:

     The above apis are defined by COSA framework for the 
  shared libraries to use.


  Usage Example:
  
      PCOSA_PLUGIN_INFO  
           pPlugInfo  = (PCOSA_PLUGIN_INFO)hCosaPlugInfo;
      void*
           hProc      = pPlugInfo->AcquireFunction("COSANotifyDiagnosticsComplete");

      COSANotifyDiagCompleteProc
           pProc      = (COSANotifyDiagCompleteProc)hProc;

      if( pProc != NULL)
      {
          pProc();        Notify COSA Framework that diagnostics is completed.
      }

**************************************************************************************/
/* COSANotifyValueChange */
typedef BOOL
(*COSANotifyValueChangeProc)
   (
       void*                       hDmlAgent,
       char*                       pParamName   /* full param name; */
   );

/* COSANotifyDiagnosticsComplete */
typedef BOOL
(*COSANotifyDiagCompleteProc)
   (
       void*                       hDmlAgent
   );

/**************************************************************************
 *
 * uType can be any of the consts defined below
 *
 **************************************************************************/
#define  COSA_LAN_DEVICE_INTERFACE                              0x00000001
#define  COSA_LAN_LAYER3_INTERFACE                              0x00000002
#define  COSA_LAN_XXX_INTERFACE_CONFIG_INTERFACE                0x00000004
#define  COSA_WLAN_CONFIGURATION_INTERFACE                      0x00000008
#define  COSA_WAN_DEVICE_INTERFACE                              0x00000010
#define  COSA_WAN_CONNECTION_DEVICE_INTERFACE                   0x00000020
#define  COSA_WAN_LAYER3_CONNECTION_INTERFACE                   0x00000040
#define  COSA_DEFAULT_LAYER3_CONNECTION_INTERFACE               0x00000080
#define  COSA_NULL_STRING_INTERFACE                             0x00000100
#define  COSA_ALL_FULL_HIERARCHICAL_INTERFACE                   0x000001FF

/* COSAValidateHierachyInterface */
typedef BOOL
(*COSAValidateHierarchyInterfaceProc)
    (
        void*                       hDmlAgent,
        char*                       pHierarchName,
        ULONG                       uType
    );

/* COSAGetParamValueBool */
typedef BOOL
(*COSAGetParamValueBoolProc)
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

/* COSAGetParamValueInt */
typedef int
(*COSAGetParamValueIntProc)
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

/* COSAGetParamValueUlong */
typedef ULONG
(*COSAGetParamValueUlongProc)
    (
        void*                       hDmlAgent,
        char*                       pParamName
    );

/***********************************************
 *
 * Returned values:
 *
 * 0 = SUCCESS; -1 = FAILURE; 1 = NEW_SIZE
 *
 ***********************************************/
/* COSAGetParamValueString */
typedef int
(*COSAGetParamValueStringProc)
    (
        void*                       hDmlAgent,
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    );


/* COSASetParamValueBool */
typedef ANSC_STATUS
(*COSASetParamValueBoolProc)
    (
        char*                       pParamName,
        BOOL                        bParamValue
    );

/* COSASetParamValueInt */
typedef ANSC_STATUS
(*COSASetParamValueIntProc)
    (
        char*                       pParamName,
        int                         iParamValue
    );

/* COSASetParamValueUlong */
typedef ANSC_STATUS
(*COSASetParamValueUlongProc)
    (
        char*                       pParamName,
        ULONG                       ulParamValue
    );

/* COSASetParamValueString */
typedef ANSC_STATUS
(*COSASetParamValueStringProc)
    (
        char*                       pParamName,
        char*                       pParamValue
    );


typedef ANSC_STATUS
(*COSAGetParamValueByPathNameProc)
    (
        void*                       bus_handle,
        parameterValStruct_t        *val,
        ULONG                       *parameterValueLength
    );

typedef ANSC_STATUS
(*COSASetParamValueByPathNameProc)
    (
        parameterValStruct_t       *val
    );

typedef ANSC_STATUS
(*COSAGetInstanceNumbersProc)
    (
        char*                      pObjName,
        ULONG*                     pInsList,
        ULONG*                     pInsCnt
    );

/* COSAGetDiagPluginInfo */    
typedef void*
(*COSAGetCommonHandleProc)
    (
        void*                       hThisObject
    );

/* COSACreateSlapObject */
typedef void*
(*COSACreateSlapObjProc)
    (
        void*                       hSlapObjDescr,
        void*                       hInsContext
    );

/* COSAAcquirePoamObject */
typedef void*
(*COSAAcquirePoamObjProc)
    (
        void*                       hSlapObjDescr
    );

/* COSAReleasePoamObject */
typedef ULONG
(*COSAReleasePoamObjProc)
    (
        void*                       hPoamObj
    );

/* COSAGetRegistryRootFolder */
typedef void*
(*COSAGetHandleProc)
    (
        void*                       hDmlAgent
    );

/* COSAGetInstanceNumberByIndex */
typedef ULONG
(*COSAGetInstanceNumberByIndexProc)
    (
        void*                       hDmlAgent,
        char*                      pObjName,
        ULONG                      ulIndex
    );

typedef void*
(*COSAGetInterfaceByNameProc)
    (
        void*                       hDmlAgent,
        char*                   pIfName
    );

/*COSARegisterCallBackAfterInitDml*/    
typedef void*
(*COSARegisterCallBackAfterInitDmlProc)
    (
        void*                       hDmlAgent,
        void*                   handle
    );

/*COSAGetMessageBusHandle*/    
typedef void*
(*COSAGetMessageBusHandleProc)
    (
        void*                       hDmlAgent
    );

/*COSAGetSubsystemPrefix*/    
typedef char*
(*COSAGetSubsystemPrefixProc)
    (
        void*                       hDmlAgent
    );

/*COSARepopulateTable*/    
typedef BOOL
(*COSARepopulateTableProc)
    (
        void*                       hDmlAgent,
        char*        pTabName
    );

#endif /* _COSA_PLUGIN_API_H */


