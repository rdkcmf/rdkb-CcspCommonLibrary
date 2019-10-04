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

    module: dslh_cpeco_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Cpe Controller Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        09/09/2005    initial revision.
        03/06/2008    Bin added TR64Manager;
        02/04/2010    Bin added Statistics interface for TR69
        11/01/2010    Bin added DataModelAgent module.
        01/05/2011    Bin moved "ValidateHierarchyInterface" from ObjController object;

**********************************************************************/


#ifndef  _DSLH_CPECO_INTERFACE_
#define  _DSLH_CPECO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "dslh_properties.h"

#include "dslh_ifo_mco.h"
/*#include "dslh_ifo_cfg.h"*/

/*
#include "sys_ifo_ira.h"
#include "sys_ifo_ram.h"
*/


/***********************************************************
       DSLH CPE CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  CPECONTROLLER_MAX_INTERFACE_COUNT                      16
#define  CPECONTROLLER_MAX_REG_OBJECT_COUNT                     128

/*
 * Define CWMP statistics related structure
 */
#define  DSLH_CWMP_CONNECTION_STATUS_SUCCESS                     1
#define  DSLH_CWMP_CONNECTION_STATUS_FAILURE                     0

typedef  struct
_DSLH_CWMP_STATISTICS
{
    ULONG                           uTcpSuccess;
    ULONG                           uTcpFailures;
    ULONG                           uTlsFailures;
    ANSC_UNIVERSAL_TIME             LastConnectionTime;
    ULONG                           uLastConnectionStatus;
    ANSC_UNIVERSAL_TIME             LastInformResponseTime;
    ANSC_UNIVERSAL_TIME             LastReceivedSPVTime;
}
DSLH_CWMP_STATISTICS,  *PDSLH_CWMP_STATISTICS;


/* define CPE controller event notification type */
#define  DSLH_CPECO_EVENT_Unkonwn                   0
#define  DSLH_CPECO_EVENT_InterfaceUp               1
#define  DSLH_CPECO_EVENT_InterfaceDown             2
#define  DSLH_CPECO_EVENT_InterfaceIpAddrChanged    3


#define  DSLH_CPECO_CWMP_START_INTERVAL				5 * 1000           /* in milli-seconds */

#define  DSLH_LAN_DEVICE_INTERFACE                              0x00000001
#define  DSLH_LAN_LAYER3_INTERFACE                              0x00000002
#define  DSLH_LAN_XXX_INTERFACE_CONFIG_INTERFACE                0x00000004
#define  DSLH_WLAN_CONFIGURATION_INTERFACE                      0x00000008
#define  DSLH_WAN_DEVICE_INTERFACE                              0x00000010
#define  DSLH_WAN_CONNECTION_DEVICE_INTERFACE                   0x00000020
#define  DSLH_WAN_LAYER3_CONNECTION_INTERFACE                   0x00000040
#define  DSLH_DEFAULT_LAYER3_CONNECTION_INTERFACE               0x00000080
#define  DSLH_NULL_STRING_INTERFACE                             0x00000100
#define  DSLH_ALL_FULL_HIERARCHICAL_INTERFACE                   0x000001FF

#define  DSLH_MAX_NUM_OF_INTERFACE_VALIDATED                      9

#define  DSLH_ANY_WAN_INTERFACE_NAME                            "WAN"
#define  DSLH_ANY_LAN_INTERFACE_NAME                            "LAN"
#define  DSLH_ANY_LOCAL_INTERFACE_NAME                          "Local"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHCPECO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHCPECO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_REG_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_REG_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr,
        ANSC_HANDLE                 hParamArray
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_REG_OBJ2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        void*                       pfnObjcoConstructor,
        ANSC_HANDLE                 hParamArray
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_LOAD_LIB)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile,
        BOOL                        bPopulateTree
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_LOAD_LIB2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bPopulateTree
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_LOAD_LIB3)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCPECO_ADDIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_DSLHCPECO_GETIF)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ifName
    );

typedef  void
(*PFN_DSLHCPECO_EVENT_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hEventContext
    );

typedef  BOOL
(*PFN_DSLHCPECO_VALID)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName,
        ULONG                       uType
    );

typedef ANSC_STATUS
(*PFN_DSLHCPECO_SETPARAMETERARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParameterArray,
        ANSC_HANDLE                 hParameterHolder,
        ULONG                       uCount
    );

typedef ANSC_STATUS
(*PFN_DSLHCPECO_REGDATAMODEL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        char*                       pXMLFile,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    );

typedef ANSC_STATUS
(*PFN_DSLHCPECO_REGDATAMODEL2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        void*                       pfnXmlLoader,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    );
/*
 * The CPE WAN Management Protocol allows an ACS to provision a CPE or collection of CPE based on a
 * variety of criteria. The provisioning mechanism includes specific provisioning parameters and a
 * general mecahnism for adding vendor-specific provisioning capabilities as needed.
 *
 * The provisioning mechanism allows CPE provisioning at the time of initial connection to the
 * broadband access network, and the ability to re-provision at any subsequent time. This includes
 * support for asynchronous ACS-initiated re-provisioning of a CPE.
 */
#define  DSLH_CPE_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    DSLH_CPE_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hDslhWmpDatabase;                                       \
	ANSC_HANDLE                     hDslhDataModelAgent;                                    \
    ANSC_HANDLE                     hDbusHandle;                                            \
                                                                                            \
    ANSC_HANDLE                     hIfArray[CPECONTROLLER_MAX_INTERFACE_COUNT];            \
    ULONG                           uIfCount;                                               \
    ANSC_HANDLE                     hParameterArray;                                        \
    ULONG                           uParameterCount;                                        \
    ULONG                           uParameterArrayIndex;                                   \
    ANSC_HANDLE                     hParameterHolder;                                       \
    ULONG                           uParameterIndex;                                        \
    ANSC_HANDLE                     hObjDespArray[CPECONTROLLER_MAX_REG_OBJECT_COUNT];      \
    ULONG                           uObjCount;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_DSLHCPECO_GET_CONTEXT       GetDslhWmpDatabase;                                     \
                                                                                            \
    PFN_DSLHCPECO_GET_PROPERTY      GetProperty;                                            \
    PFN_DSLHCPECO_SET_PROPERTY      SetProperty;                                            \
    PFN_DSLHCPECO_RESET             ResetProperty;                                          \
    PFN_DSLHCPECO_RESET             Reset;                                                  \
                                                                                            \
    PFN_DSLHCPECO_GET_IF            GetDslhMpaIf;                                           \
    PFN_DSLHCPECO_GET_IF            GetDslhMprIf;                                           \
    PFN_DSLHCPECO_SET_IF            SetDbusHandle;                                          \
                                                                                            \
    PFN_DSLHCPECO_ENGAGE            Engage;                                                 \
    PFN_DSLHCPECO_CANCEL            Cancel;                                                 \
    PFN_DSLHCPECO_SETUP_ENV         SetupEnv;                                               \
    PFN_DSLHCPECO_CLOSE_ENV         CloseEnv;                                               \
                                                                                            \
    PFN_DSLHCPECO_INIT              InitDataModel;                                          \
    PFN_DSLHCPECO_REG_ALL           RegisterAllParams;                                      \
    PFN_DSLHCPECO_REG_OBJ           RegisterObject;                                         \
    PFN_DSLHCPECO_REG_OBJ           RegisterHiddenObject;                                   \
    PFN_DSLHCPECO_REG_OBJ2          RegisterObject2;                                        \
    PFN_DSLHCPECO_REG_OBJ2          RegisterHiddenObject2;                                  \
    PFN_DSLHCPECO_LOAD_LIB3         LoadInternalDMLibFile;                                  \
    PFN_DSLHCPECO_LOAD_LIB          LoadExternalDMLibFile;                                  \
    PFN_DSLHCPECO_LOAD_LIB2         LoadExternalDMLibFile2;                                 \
    PFN_DSLHCPECO_LOAD_LIB3         UnloadDMLibFile;                                        \
    PFN_DSLHCPECO_LOAD_LIB3         LoadRpcExtXMLFile;                                      \
    PFN_DSLHCPECO_REGDATAMODEL      RegisterCcspDataModel;                                  \
    PFN_DSLHCPECO_REGDATAMODEL2     RegisterCcspDataModel2;                                 \
                                                                                            \
    PFN_DSLHCPECO_ADDIF             AddInterface;                                           \
    PFN_DSLHCPECO_GETIF             GetInterfaceByName;                                     \
                                                                                            \
    PFN_DSLHCPECO_SETPARAMETERARRAY SetParameterArray;                                      \
    SLIST_HEADER                    DmlCallbackList;                                        \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_CPE_CONTROLLER_OBJECT
{
    DSLH_CPE_CONTROLLER_CLASS_CONTENT
}
DSLH_CPE_CONTROLLER_OBJECT,  *PDSLH_CPE_CONTROLLER_OBJECT;

#define  ACCESS_DSLH_CPE_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, DSLH_CPE_CONTROLLER_OBJECT, Linkage)


#endif
