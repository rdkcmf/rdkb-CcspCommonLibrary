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

    module:	slap_properties.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Slap Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_PROPERTIES_
#define  _SLAP_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Slap message definitions, so we
 * include the header file here.
 */
#include  "slap_definitions.h"
#include  "web_properties.h"


/***********************************************************
   SLAP ENV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Slap Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * slap object loading, the concept of "Environment Control" is really important.
 */
#define  SLAP_DEF_OBJECT_POOL_SIZE                  16

typedef  struct
_SLAP_ENV_CONTROLLER_PROPERTY
{
    BOOL                            bPoolingEnabled;
    ULONG                           ObjectPoolSize;
}
SLAP_ENV_CONTROLLER_PROPERTY,  *PSLAP_ENV_CONTROLLER_PROPERTY;


/***********************************************************
   SLAP ACCESS MANAGER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Slap Access Manager Object is responsible for setting up the object access environment and
 * providing a generic object access mechanism, which allows the upper-layer modules to access any
 * SLAP object within the system in the same way. Not only it encapsulates the local SLAP object
 * access mechanism implemented by SlapEnvController, but also it may create IMCP LPC-based remote
 * SLAP object access channel.
 */
typedef  struct
_SLAP_ACCESS_MANAGER_PROPERTY
{
    ULONG                           AggregationMode;
    USHORT                          LpcPartyPort;
    ANSC_IPV4_ADDRESS               MyAddress;
    ANSC_IPV4_ADDRESS               ManagerAddress;
}
SLAP_ACCESS_MANAGER_PROPERTY,  *PSLAP_ACCESS_MANAGER_PROPERTY;


/***********************************************************
     SLAP OBJ MAPPER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * SLAP supports a hierarchical object name space, which can be extended to support new objects and
 * remote object invocation. SLAP Obj Mapper Object is designed to manage such name space by employ-
 * ing a tree-like structure to maintain all the registered OBJECT_DESCRIPTORS. This object is very
 * similar to WebResourceLocator object in many aspects while it also resembles AnscObjectMapper
 * object in some way.
 */
typedef  struct
_SLAP_OBJ_MAPPER_PROPERTY
{
    ULONG                           Dummy;
}
SLAP_OBJ_MAPPER_PROPERTY,  *PSLAP_OBJ_MAPPER_PROPERTY;


/***********************************************************
    SLAP LOAM CLIENT CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Slap Loam Client Object is responsible for accessing SLAP objects created in other address
 * spaces and processes. It communicates with the Slap Loam Server Object to resolve the SLAP
 * object name to the corresponding SLAP module and establishes the LPC communication channcel
 * directly between this SLAP and peer SLAP environments.
 */
typedef  struct
_SLAP_LOAM_CLIENT_PROPERTY
{
    BOOL                            bMobilityOn;
}
SLAP_LOAM_CLIENT_PROPERTY,  *PSLAP_LOAM_CLIENT_PROPERTY;


/***********************************************************
    SLAP LOAM SERVER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Slap Loam Server Object is responsible for processing IMCP-based SLAP object access and
 * management requests. It serves as a proxy and shields the underlying SLAP_UOA implementation
 * from having to deal with the various IPC/LPC/RPC situations. In addition to communicateing with
 * the Slap Loam Client Object directly, the Loam Server also interacts with the Loam Manager to
 * resolve the local SLAP object name.
 */
typedef  struct
_SLAP_LOAM_SERVER_PROPERTY
{
    BOOL                            bMobilityOn;
}
SLAP_LOAM_SERVER_PROPERTY,  *PSLAP_LOAM_SERVER_PROPERTY;


/***********************************************************
    SLAP LOAM BROKER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Slap Loam Broker Object is responsible for processing IMCP-based SLAP object location/query
 * requests. It resolves the mapping from SLAP objects to the corresponding SLAP environments. It
 * serves as a central LPC server to keep trace of all the local LPC connectors by accepting every
 * HELLO message it receives.
 */
typedef  struct
_SLAP_LOAM_BROKER_PROPERTY
{
    ANSC_IPV4_ADDRESS               MyAddress;
}
SLAP_LOAM_BROKER_PROPERTY,  *PSLAP_LOAM_BROKER_PROPERTY;


#endif
