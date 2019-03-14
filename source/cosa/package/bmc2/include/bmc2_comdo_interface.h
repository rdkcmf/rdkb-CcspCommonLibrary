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

    module:	bmc2_comdo_interface.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Com Domain Objects.

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


#ifndef  _BMC2_COMDO_INTERFACE_
#define  _BMC2_COMDO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "bmc2_properties.h"


/***********************************************************
         BMC2 COM DOMAIN COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BMC2_COMDO_COMPO_TABLE_SIZE                16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2COMDO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMDO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BMC2COMDO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_STRING_ARRAY*
(*PFN_BMC2COMDO_GET_COMMANDS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandPrefix
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMDO_GET_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_ADD_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_DEL_COMPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMDO_SET_CMDHELP)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        ULONG                       ulReqArgCount,
        char**                      pReqArgs,
        ULONG                       ulOptArgCount,
        char**                      pOptArgs
    );

/*
 * The Bmc2 Com Domain Object encapsulates the implementation of a CLI Domain. Some advanced CLI
 * interfaces adopt a concept known as "Hierarchical Menu System". By dividing the CLI command set
 * into different domains, it becomes possible to reuse same command syntax for different purposes
 * in different domains. Furthermore, it's easier to apply access control based on the domain.
 */
#define  BMC2_COM_DOMAIN_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBmc2ComTerminal;                                       \
    ANSC_HANDLE                     hBmc2EnvController;                                     \
    ANSC_HANDLE                     hBmc2ReqController;                                     \
    char*                           DomainTitle;                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerDomain;                                   \
    ANSC_HANDLE                     hSlapBmc2Domain;                                        \
                                                                                            \
    QUEUE_HEADER                    CompoQueue;                                             \
    ANSC_LOCK                       CompoQueueLock;                                         \
                                                                                            \
    PFN_BMC2COMDO_GET_CONTEXT       GetBmc2ComTerminal;                                     \
    PFN_BMC2COMDO_SET_CONTEXT       SetBmc2ComTerminal;                                     \
    PFN_BMC2COMDO_GET_CONTEXT       GetBmc2EnvController;                                   \
    PFN_BMC2COMDO_SET_CONTEXT       SetBmc2EnvController;                                   \
    PFN_BMC2COMDO_GET_CONTEXT       GetBmc2ReqController;                                   \
    PFN_BMC2COMDO_SET_CONTEXT       SetBmc2ReqController;                                   \
    PFN_BMC2COMDO_GET_NAME          GetDomainTitle;                                         \
    PFN_BMC2COMDO_SET_NAME          SetDomainTitle;                                         \
    PFN_BMC2COMDO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BMC2COMDO_ENGAGE            Engage;                                                 \
    PFN_BMC2COMDO_CANCEL            Cancel;                                                 \
    PFN_BMC2COMDO_SETUP_ENV         SetupEnv;                                               \
    PFN_BMC2COMDO_CLOSE_ENV         CloseEnv;                                               \
                                                                                            \
    PFN_BMC2COMDO_GET_COMMANDS      GetCommandsByPrefix;                                    \
    PFN_BMC2COMDO_GET_COMPO         GetCommandProperty;                                     \
    PFN_BMC2COMDO_ADD_COMPO         AddCommandProperty;                                     \
    PFN_BMC2COMDO_DEL_COMPO         DelCommandProperty;                                     \
    PFN_BMC2COMDO_DEL_ALL           DelAllCommandProperties;                                \
    PFN_BMC2COMDO_SET_CMDHELP       SetCommandHelpSyntax;                                   \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_COM_DOMAIN_OBJECT
{
    BMC2_COM_DOMAIN_CLASS_CONTENT
}
BMC2_COM_DOMAIN_OBJECT,  *PBMC2_COM_DOMAIN_OBJECT;

#define  ACCESS_BMC2_COM_DOMAIN_OBJECT(p)           \
         ACCESS_CONTAINER(p, BMC2_COM_DOMAIN_OBJECT, Linkage)


#endif
