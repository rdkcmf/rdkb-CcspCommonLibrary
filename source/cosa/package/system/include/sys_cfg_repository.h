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

    module:	sys_cfg_repository.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the system- and package-wide compiling
        and configuration settings.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/07/02    initial revision.

**********************************************************************/


#ifndef  _SYS_CFG_REPOSITORY_
#define  _SYS_CFG_REPOSITORY_


#include "sys_definitions.h"


/***********************************************************
        SYS INFO REPOSITORY SYSTEM FOLDER DEFINITIONS
***********************************************************/

/*
 * There're some pre-defined repository folders created by the runtime system itself, and are well-
 * known to all the internal and external modules. Since the System Information Repository can only
 * be accessed via string-matching, we define all the system folder-names here for easy reference.
 * Any module that needs to access system folders should include this header file.
 */
#define  SYS_FOLDER_L1_SYSTEM                       "System"
#define  SYS_FOLDER_L1_CONFIGURATION                "Configuration"
#define  SYS_FOLDER_L1_CONTROL                      "Control"
#define  SYS_FOLDER_L1_RUNTIME_ENVIRONMENT          "RuntimeEnvironment"
#define  SYS_FOLDER_L1_RUNTIME_STATUS               "RuntimeStatus"

#define  SYS_FOLDER_L2_ACTIVE_MODULES               "ActiveModules"
#define  SYS_FOLDER_L2_PRE_REGISTERED_MODULES       "PreRegisteredModules"
#define  SYS_FOLDER_L2_EXTERNAL_MODULES             "ExternalModules"
#define  SYS_FOLDER_L2_REGISTERED_OIDS              "RegisteredOids"
#define  SYS_FOLDER_L2_SETUP                        "SetUp"
#define  SYS_FOLDER_L2_PROVISION                    "Provision"
#define  SYS_FOLDER_L2_SERVICES                     "Services"
#define  SYS_FOLDER_L2_LOCAL_SYSTEM                 "LocalSystem"
#define  SYS_FOLDER_L2_ACTIVITY                     "Activity"
#define  SYS_FOLDER_L2_LOG_HISTORY                  "LogHistory"
#define  SYS_FOLDER_L2_EVENTS                       "Events"

#define  SYS_FOLDER_L2_PACKAGE_WRAPPER              "PackageWrapper"
#define  SYS_FOLDER_L2_ENV_CONTROLLER               "EnvController"
#define  SYS_FOLDER_L2_SECURITY_AGENT               "SecurityAgent"
#define  SYS_FOLDER_L2_INFO_REPOSITORY              "InfoRepository"
#define  SYS_FOLDER_L2_MODULE_LOADER                "ModuleLoader"
#define  SYS_FOLDER_L2_NAME_MAPPERS                 "NameMappers"

#define  SYS_FOLDER_DEPENDENCY                      "Dependency"
#define  SYS_FOLDER_INTERFACES                      "Interfaces"
#define  SYS_FOLDER_OBJECTS                         "Objects"
#define  SYS_FOLDER_CONTROL                         "Control"
#define  SYS_FOLDER_CONTENT                         "Content"

#define  SYS_RECORD_SERVICE_BROKER                  "ServiceBroker"
#define  SYS_RECORD_MODULE_CONSTRUCTOR              "ModuleConstructor"
#define  SYS_RECORD_SWITCH_MODE                     "SwitchMode"
#define  SYS_RECORD_TITLE                           "Title"
#define  SYS_RECORD_VALUE                           "Value"
#define  SYS_RECORD_LABEL                           "Label"
#define  SYS_RECORD_DESCRIPTION                     "Description"
#define  SYS_RECORD_TIMESTAMP                       "Timestamp"


#endif
