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

    module:	bmc2_properties.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Bmc2 Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/20/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_PROPERTIES_
#define  _BMC2_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Bmc2 message definitions, so we
 * include the header file here.
 */
#include  "bmc2_definitions.h"


/***********************************************************
   BMC2 ENV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bmc2 Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * bmc2 object loading, the concept of "Environment Control" is really important.
 */
#define  BMC2_DEF_ROOT_PATH                         "/"
#define  BMC2_DEF_ROOT_DOMAIN_COMMAND               "r_domain"
#define  BMC2_MAX_COMMAND_NAME_SIZE                 32

typedef  struct
_BMC2_ENV_CONTROLLER_PROPERTY
{
    BOOL                            bMultiUserCtrl;
    BOOL                            bCacheScpPages;
    char                            RootPath         [ANSC_MAX_FILE_NAME_SIZE   ];
    char                            RootDomainCommand[BMC2_MAX_COMMAND_NAME_SIZE];
}
BMC2_ENV_CONTROLLER_PROPERTY,  *PBMC2_ENV_CONTROLLER_PROPERTY;


/***********************************************************
    BMC2 COM TERMINAL CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bmc2 Com Terminal Object encapsulates the implementation of a Virtual Terminal. The Shell
 * program creates a Virtual Terminal for each login user. Multiple Virtual Terminals share the
 * same backend object access layer (i.e. SLAP). Each Virtual Terminal can only be associated with
 * a single user through its life cycle. The BMC Input and Output objects are unique within each
 * Virtual Terminal.
 */
#define  BMC2_MAX_USER_ID_SIZE                      128

typedef  struct
_BMC2_COM_TERMINAL_PROPERTY
{
    char                            UserIdentifier[BMC2_MAX_USER_ID_SIZE];
    ULONG                           UserPermission;
    ULONG                           MaxLineNumber;
    ULONG                           MaxColumnNumber;
    BOOL                            bReadOnly;
}
BMC2_COM_TERMINAL_PROPERTY,  *PBMC2_COM_TERMINAL_PROPERTY;


/***********************************************************
    BMC2 COM EXECUTOR CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bmc2 Com Executor Object encapsulates the implementation of the SCP (Shell Command Page)
 * execution environment. It's responsible for instantiating the SCP engine and execute the SCP
 * page. By including Com Executor as part of BMC2 module, we simplify the module loading and the
 * overall software architecture (at least on paper) comparing to the multi-layered BMW structure.
 */
typedef  struct
_BMC2_COM_EXECUTOR_PROPERTY
{
    ULONG                           Dummy;
}
BMC2_COM_EXECUTOR_PROPERTY,  *PBMC2_COM_EXECUTOR_PROPERTY;


#endif
