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

    module:	bmc2_envco_interface.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Env Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/21/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_ENVCO_INTERFACE_
#define  _BMC2_ENVCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bmc2_properties.h"
#include "bmc2_ifo_scc.h"


/***********************************************************
        BMC2 ENV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BMC2_ENVCO_COMPO_TABLE_SIZE                128

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2ENVCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2ENVCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BMC2ENVCO_GET_COMTO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_ADD_COMTO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_DEL_COMTO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BMC2ENVCO_GET_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainName
    );

typedef  ANSC_STATUS
(*PFN_BMC2ENVCO_ADD_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty,
        ANSC_HANDLE                 hCommandInsertBefore
    );

typedef  ANSC_HANDLE
(*PFN_BMC2ENVCO_DEL_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainNamePattern
    );

typedef  void
(*PFN_BMC2ENVCO_SET_PTERMDN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSeudoTermDevName
    );

/*
 * The Bmc2 Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * bmc2 object loading, the concept of "Environment Control" is really important.
 */
#define  BMC2_ENV_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BMC2_ENV_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hBmc2SccIf;                                             \
    ANSC_HANDLE                     hBmc2ComExecutor;                                       \
    ANSC_HANDLE                     hBwrmEnvController;                                     \
    ANSC_HANDLE                     hSlapGoaIf;                                             \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    char*                           pPseudoTermDevName;                                     \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerApp;                                      \
    ANSC_HANDLE                     hSlapContainerServices;                                 \
    ANSC_HANDLE                     hSlapBmc2App;                                           \
    ANSC_HANDLE                     hSlapBmc2Server;                                        \
    ANSC_HANDLE                     hSlapVarEntity;                                         \
    ANSC_HANDLE                     hSlapVarConverter;                                      \
                                                                                            \
    QUEUE_HEADER                    ComtoQueue;                                             \
    ANSC_LOCK                       ComtoQueueLock;                                         \
    QUEUE_HEADER                    CompoTable[BMC2_ENVCO_COMPO_TABLE_SIZE];                \
    ANSC_LOCK                       CompoTableLock;                                         \
                                                                                            \
    PFN_BMC2ENVCO_GET_IF            GetBmc2SccIf;                                           \
    PFN_BMC2ENVCO_GET_CONTEXT       GetBmc2ComExecutor;                                     \
    PFN_BMC2ENVCO_GET_CONTEXT       GetBwrmEnvController;                                   \
    PFN_BMC2ENVCO_GET_IF            GetBwrmRamIf;                                           \
    PFN_BMC2ENVCO_GET_IF            GetSlapGoaIf;                                           \
    PFN_BMC2ENVCO_SET_IF            SetSlapGoaIf;                                           \
    PFN_BMC2ENVCO_GET_IF            GetSlapUoaIf;                                           \
    PFN_BMC2ENVCO_SET_IF            SetSlapUoaIf;                                           \
    PFN_BMC2ENVCO_GET_CONTEXT       GetSlapContainerServices;                               \
    PFN_BMC2ENVCO_SET_CONTEXT       SetSlapContainerServices;                               \
                                                                                            \
    PFN_BMC2ENVCO_GET_PROPERTY      GetProperty;                                            \
    PFN_BMC2ENVCO_SET_PROPERTY      SetProperty;                                            \
    PFN_BMC2ENVCO_RESET             ResetProperty;                                          \
    PFN_BMC2ENVCO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BMC2ENVCO_ENGAGE            Engage;                                                 \
    PFN_BMC2ENVCO_CANCEL            Cancel;                                                 \
    PFN_BMC2ENVCO_SETUP_ENV         SetupEnv;                                               \
    PFN_BMC2ENVCO_CLOSE_ENV         CloseEnv;                                               \
                                                                                            \
    PFN_BMC2ENVCO_GET_COMTO         GetComTerminal;                                         \
    PFN_BMC2ENVCO_ADD_COMTO         AddComTerminal;                                         \
    PFN_BMC2ENVCO_DEL_COMTO         DelComTerminal;                                         \
    PFN_BMC2ENVCO_DEL_ALL           DelAllComTerminals;                                     \
                                                                                            \
    PFN_BMC2ENVCO_GET_COMPO         GetCommandProperty;                                     \
    PFN_BMC2ENVCO_ADD_COMPO         AddCommandProperty;                                     \
    PFN_BMC2ENVCO_DEL_COMPO         DelCommandProperty;                                     \
    PFN_BMC2ENVCO_DEL_ALL           DelAllCommandProperties;                                \
                                                                                            \
    PFN_BMC2SCCIF_SET_INFO          SccSetRootInfo;                                         \
    PFN_BMC2SCCIF_SET_CONTROL       SccSetMultiUserCtrl;                                    \
    PFN_BMC2SCCIF_ADD_COMMAND       SccAddCommand;                                          \
                                                                                            \
    PFN_BMC2SCCIF_ADD_TERMINAL      SccAddTerminal;                                         \
    PFN_BMC2SCCIF_DEL_TERMINAL      SccDelTerminal;                                         \
    PFN_BMC2SCCIF_GET_COMMANDS      SccGetCommands;                                         \
    PFN_BMC2SCCIF_IS_VALID          SccIsCommandValid;                                      \
    PFN_BMC2SCCIF_EXECUTE           SccExecuteCommand;                                      \
    PFN_BMC2SCCIF_ABORT             SccAbortExecution;                                      \
    PFN_BMC2SCCIF_GET_CMD_SYNTAX    SccGetCommandSyntax;                                    \
                                                                                            \
    PFN_BMC2ENVCO_SET_PTERMDN       SetPseudoTermDevName;                                   \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_ENV_CONTROLLER_OBJECT
{
    BMC2_ENV_CONTROLLER_CLASS_CONTENT
}
BMC2_ENV_CONTROLLER_OBJECT,  *PBMC2_ENV_CONTROLLER_OBJECT;

#define  ACCESS_BMC2_ENV_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BMC2_ENV_CONTROLLER_OBJECT, Linkage)


#endif
