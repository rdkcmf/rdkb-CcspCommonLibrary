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

    module:	dslh_wmpdo_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Wmp Database Object.

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
        02/28/2011    Bin added a set of ReqCounter apis to better support dynamic tables;

**********************************************************************/


#ifndef  _DSLH_WMPDO_INTERFACE_
#define  _DSLH_WMPDO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "dslh_properties.h"
#include "slap_definitions.h"

#include "dslh_ifo_mpr.h"
#include "dslh_ifo_mpa.h"


/***********************************************************
        DSLH WMP DATABASE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHWMPDO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHWMPDO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_CREATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_CONFIG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_PARSE_DATATYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDataTypeName,
        PULONG                      pulDataType,
        PULONG                      pulFormatValue1,
        PULONG                      pulFormatValue2,
        char**                      ppEnumTokens
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_PARSE_SYNTAX)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSyntaxString,
        PULONG                      pulSyntax,
        PULONG                      pulContentType
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_PARSE_TOKENS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity,
        char*                       pEnumTokens
    );

typedef  ULONG
(*PFN_DSLHWMPDO_ADD_VAR_TO_A)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

typedef  ULONG
(*PFN_DSLHWMPDO_ADD_OBJ_TO_A)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjReord
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_ARRAY_ACT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHWMPDO_FLUSH_DYN_OBJVAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDynObjRecord
    );


/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
#define  DSLH_WMP_DATABASE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    DSLH_WMP_DATABASE_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hDslhMprIf;                                             \
    ANSC_HANDLE                     hDslhMpaIf;                                             \
    ANSC_HANDLE                     hSlapVarConverter;                                      \
    ANSC_HANDLE                     hSlapVarMapper;                                         \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    ANSC_HANDLE                     hRootObjEntity;                                         \
    ANSC_HANDLE                     hRootObjRecord;                                         \
    ANSC_HANDLE                     hVarRecordArray;                                        \
    ANSC_HANDLE                     hObjRecordArray;                                        \
    ULONG                           ulVarRecordCount;                                       \
    ULONG                           ulObjRecordCount;                                       \
    ULONG                           SessionID;                                              \
    ANSC_TS_LOCK                    AccessTsLock;                                           \
    ANSC_LOCK                       MpaWriteLock;                                           \
    BOOL                            bMpaWriteLocked;                                        \
    char*                           LockedEntity;                                           \
    char*                           pRootObjName;                                           \
    ULONG                           uReqCounter;                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hPoamIrepFoWmpDatabase;                                 \
    ANSC_HANDLE                     hSlapDslhParamTree;                                     \
                                                                                            \
    ANSC_HANDLE                     hMonitorTimerObj;                                       \
    ANSC_HANDLE                     hMonitorTimerIf;                                        \
    QUEUE_HEADER                    MpoQueue;                                               \
    ANSC_LOCK                       MpoQueueLock;                                           \
                                                                                            \
    PFN_DSLHWMPDO_GET_IF            GetDslhMprIf;                                           \
    PFN_DSLHWMPDO_GET_IF            GetDslhMpaIf;                                           \
    PFN_DSLHWMPDO_GET_CONTEXT       GetSlapVarConverter;                                    \
    PFN_DSLHWMPDO_GET_CONTEXT       GetSlapVarMapper;                                       \
    PFN_DSLHWMPDO_GET_CONTEXT       GetDslhCpeController;                                   \
    PFN_DSLHWMPDO_SET_CONTEXT       SetDslhCpeController;                                   \
    PFN_DSLHWMPDO_GET_PROPERTY      GetProperty;                                            \
    PFN_DSLHWMPDO_SET_PROPERTY      SetProperty;                                            \
    PFN_DSLHWMPDO_RESET             ResetProperty;                                          \
    PFN_DSLHWMPDO_RESET             Reset;                                                  \
                                                                                            \
    PFN_DSLHWMPDO_ENGAGE            Engage;                                                 \
    PFN_DSLHWMPDO_CANCEL            Cancel;                                                 \
    PFN_DSLHWMPDO_SETUP_ENV         SetupEnv;                                               \
    PFN_DSLHWMPDO_CLOSE_ENV         CloseEnv;                                               \
    PFN_DSLHWMPDO_INVOKE            MonitorTimerInvoke;                                     \
                                                                                            \
    PFN_DSLHWMPDO_CREATE            CreateRecordTree;                                       \
    PFN_DSLHWMPDO_CONFIG            ConfigRecordTree;                                       \
    PFN_DSLHWMPDO_ENABLE            EnableMonitor;                                          \
                                                                                            \
    PFN_DSLHWMPDO_PARSE_DATATYPE    ParseParamDataType;                                     \
    PFN_DSLHWMPDO_PARSE_SYNTAX      ParseParamSyntax;                                       \
    PFN_DSLHWMPDO_PARSE_TOKENS      ParseParamEnumTokens;                                   \
    PFN_DSLHWMPDO_ADD_VAR_TO_A      AddVarRecordIntoArray;                                  \
    PFN_DSLHWMPDO_ADD_OBJ_TO_A      AddObjRecordIntoArray;                                  \
    PFN_DSLHWMPDO_ARRAY_ACT         ResetObjVarArray;                                       \
    PFN_DSLHWMPDO_ARRAY_ACT         InitObjVarArray;                                        \
    PFN_DSLHWMPDO_FLUSH_DYN_OBJVAR  FlushDynObjVar;                                         \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_WMP_DATABASE_OBJECT
{
    DSLH_WMP_DATABASE_CLASS_CONTENT
}
DSLH_WMP_DATABASE_OBJECT,  *PDSLH_WMP_DATABASE_OBJECT;

#define  ACCESS_DSLH_WMP_DATABASE_OBJECT(p)         \
         ACCESS_CONTAINER(p, DSLH_WMP_DATABASE_OBJECT, Linkage)


/***********************************************************
      DSLH WMP DATABASE MONITOR PARAM OBJECT DEFINITION
***********************************************************/

/*
 * WT-121v3-2.12 redefines the requirements for Notification described in sections A.3.3.1 and
 * A.3.2.4: "The CPE may return a "notification request rejected" error if an attempt is made to
 * set notification on a parameter deemed inappropriate for Active Notification. A parameter is
 * deemed inappropriate for Active Notification if and only if that parameter is explicitly defined
 * as such in the definition of the corresponding data model. Parameters that might be deemed
 * inappropriate for Active Notification include parameters that change frequently, such as statis-
 * tics. A CPE MUST accept a request to enable Passive Notification for any parameter."
 */
typedef  struct
_DSLH_WMPDO_MONITOR_PARAM
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hVarRecord;
    BOOL                            bActiveNotification;
    SLAP_VARIABLE*                  PreviousValue;
    ULONG                           NotificationLimit;
    ULONG                           Timestamp;
}
DSLH_WMPDO_MONITOR_PARAM,  *PDSLH_WMPDO_MONITOR_PARAM;

#define  ACCESS_DSLH_WMPDO_MONITOR_PARAM(p)         \
         ACCESS_CONTAINER(p, DSLH_WMPDO_MONITOR_PARAM, Linkage)

#define  DslhWmpdoCleanMonitorParam(monitor_param)                                          \
         {                                                                                  \
            if ( monitor_param->PreviousValue )                                             \
            {                                                                               \
                SlapFreeVariable(monitor_param->PreviousValue);                             \
                                                                                            \
                monitor_param->PreviousValue = NULL;                                        \
            }                                                                               \
         }

#define  DslhWmpdoFreeMonitorParam(monitor_param)                                           \
         {                                                                                  \
            DslhWmpdoCleanMonitorParam(monitor_param);                                      \
            AnscFreeMemory            (monitor_param);                                      \
         }


#endif
