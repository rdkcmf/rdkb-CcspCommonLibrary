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

    module:	slap_bmc2como_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Slap Bmc2 Command Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_BMC2COMO_INTERFACE_
#define  _SLAP_BMC2COMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scocol_interface.h"
#include "slap_scocol_exported_api.h"


/***********************************************************
        SLAP BMC2 COMMAND COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  SLAP_BMC2COMO_UOA_NAME                     "Services.Bmc2.Command"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2COMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BMC2COMO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_VARIABLE*
(*PFN_BMC2COMO_GET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 param_index
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMO_REMOVE_ALL_PARAMS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMO_SET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 param_index,
        char*                       param_value
    );

typedef  SLAP_BOOL
(*PFN_BMC2COMO_IS_ABORTED)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Slap Bmc2 Command Object makes available to the script all the information that the client
 * provides when executing a CLI commandform. This includes the CLI command name and all the input
 * arguments specified by the user.
 */
#define  SLAP_BMC2_COMMAND_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_COLLECTION_CLASS_CONTENT                                                       \
    /* start of object class content */                                                     \
    PFN_BMC2COMO_GET_NAME           GetCommandName;                                         \
    PFN_BMC2COMO_GET_PARAM          GetParamByIndex;                                        \
    PFN_BMC2COMO_REMOVE_ALL_PARAMS  RemoveAllParams;                                        \
    PFN_BMC2COMO_SET_PARAM          SetParamByIndex;                                        \
    PFN_BMC2COMO_IS_ABORTED         IsAborted;                                              \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_BMC2_COMMAND_OBJECT
{
    SLAP_BMC2_COMMAND_CLASS_CONTENT
}
SLAP_BMC2_COMMAND_OBJECT,  *PSLAP_BMC2_COMMAND_OBJECT;

#define  ACCESS_SLAP_BMC2_COMMAND_OBJECT(p)         \
         ACCESS_CONTAINER(p, SLAP_BMC2_COMMAND_OBJECT, Linkage)
#define  ACCESS_SLAP_BMC2_COMMAND_FIELD(f)          \
         ACCESS_FIELD_OFFSET(SLAP_BMC2_COMMAND_OBJECT, f)


#endif
