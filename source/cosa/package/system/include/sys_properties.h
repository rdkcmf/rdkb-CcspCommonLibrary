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

    module:	sys_properties.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that are
        applicable to Sys modules and objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/02/02    initial revision.

**********************************************************************/


#ifndef  _SYS_PROPERTIES_
#define  _SYS_PROPERTIES_


#include "sys_definitions.h"


/***********************************************************
            SYS PACKAGE WRAPPER PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys Package Wrapper Object serves as a standard packaging interface for different products.
 * It encapsulates all the API functions that are available for third-party developers to integrate
 * ANSC system modules into different runtime platforms.
 */
typedef  struct
_SYS_PACKAGE_WRAPPER_PROPERTY
{
    char                            PackageName[ANSC_OBJECT_NAME_SIZE ];
    char                            Description[ANSC_MAX_STRING_SIZE  ];
    char                            PackagedBy [ANSC_MAX_USERNAME_SIZE];
    char                            Timestamp  [ANSC_OBJECT_NAME_SIZE ];
}
SYS_PACKAGE_WRAPPER_PROPERTY,  *PSYS_PACKAGE_WRAPPER_PROPERTY;


/***********************************************************
            SYS ENV CONTROLLER PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * binary module loading, the concept of "Environment Control" is really important.
 */
typedef  struct
_SYS_ENV_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
SYS_ENV_CONTROLLER_PROPERTY,  *PSYS_ENV_CONTROLLER_PROPERTY;


/***********************************************************
            SYS SECURITY AGENT PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys Security Agent Object is responsible for setting up the security sub-system for the run-
 * time environment. It provides User Authentication, Access Control and Authorization services for
 * other modules. Internally, it maintains a list of System Principals (users), and a set of
 * Property Groups along with their assoicated Operation Templates.
 */
typedef  struct
_SYS_SECURITY_AGENT_PROPERTY
{
    BOOL                            bCreateDefAdmin;
    BOOL                            bCreateDefGuest;
}
SYS_SECURITY_AGENT_PROPERTY,  *PSYS_SECURITY_AGENT_PROPERTY;


/***********************************************************
             SYS USER PROFILE PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys User Profile Object is responsible for storing the authentication and authorization
 * information associated with certain Security Principal, such as user name, password and the
 * access control list.
 */
typedef  struct
_SYS_USER_PROFILE_PROPERTY
{
    char                            Name     [ANSC_MAX_USERNAME_SIZE ];
    char                            Pswd     [ANSC_MAX_PASSWORD_SIZE ];
    char                            EmailAddr[ANSC_EMAIL_ADDRESS_SIZE];
}
SYS_USER_PROFILE_PROPERTY,  *PSYS_USER_PROFILE_PROPERTY;


/***********************************************************
            SYS INFO REPOSITORY PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys Info Repository Object is responsible for creating and managing a system-wide central
 * in-memory database. This database/repository is a tree-like structure, which consists of a set
 * of nodes at each level (only one at the root level). Each node may be associated with several
 * properties, which can be one of the pre-defined data types: integer, string, boolean, and
 * binary string.
 */
typedef  struct
_SYS_INFO_REPOSITORY_PROPERTY
{
    ULONG                           MaxLevels;
}
SYS_INFO_REPOSITORY_PROPERTY,  *PSYS_INFO_REPOSITORY_PROPERTY;


/***********************************************************
            SYS MODULE CARRIER PROPERTY DEFINITION
***********************************************************/

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_MCO_PROPERTY_TRAILER_SIZE              256

#define  SYS_MODULE_TYPE_SYSTEM                     0x00000001
#define  SYS_MODULE_TYPE_DRIVER                     0x00000002
#define  SYS_MODULE_TYPE_ACCESSORY                  0x00000004
#define  SYS_MODULE_TYPE_SERVICE                    0x00000008

#define  SYS_MODULE_TYPE_NON_SYS                    0x0000000E
#define  SYS_MODULE_TYPE_ALL                        0x0000000F

#define  SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                            \
    /* duplication of the base object class content */                                                  \
    /* start of object class content */                                                                 \
    BOOL                            bSelfContained;                                                     \
    ULONG                           ModuleType;                                                         \
    char                            ModuleName    [SYS_MAX_MODULE_NAME_SIZE];                           \
    ULONG                           DependedMcount;                                                     \
    char                            DependedMarray[SYS_MAX_DEPENDENT_NUMBER][SYS_MAX_MODULE_NAME_SIZE]; \
    ANSC_HANDLE                     DependedQarray[SYS_MAX_DEPENDENT_NUMBER];                           \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_MODULE_CARRIER_PROPERTY
{
    SYS_MODULE_CARRIER_PROPERTY_CONTENT

    char                            Trailer[SYS_MCO_PROPERTY_TRAILER_SIZE];
}
SYS_MODULE_CARRIER_PROPERTY,  *PSYS_MODULE_CARRIER_PROPERTY;

typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_NDL_PROPERTY,    *PSYS_MCO_NDL_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_SLIB_PROPERTY,   *PSYS_MCO_SLIB_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BSS_PROPERTY,    *PSYS_MCO_BSS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_GNPS_PROPERTY,   *PSYS_MCO_GNPS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_IP4S_PROPERTY,   *PSYS_MCO_IP4S_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_TCPS_PROPERTY,   *PSYS_MCO_TCPS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_UDPS_PROPERTY,   *PSYS_MCO_UDPS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_GTPS_PROPERTY,   *PSYS_MCO_GTPS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_PNS_PROPERTY,    *PSYS_MCO_PNS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_PAC_PROPERTY,    *PSYS_MCO_PAC_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_LNS_PROPERTY,    *PSYS_MCO_LNS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_LAC_PROPERTY,    *PSYS_MCO_LAC_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_WEBS_PROPERTY,   *PSYS_MCO_WEBS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BMW_PROPERTY,    *PSYS_MCO_BMW_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_DHCPS_PROPERTY,  *PSYS_MCO_DHCPS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_DHCPS2_PROPERTY, *PSYS_MCO_DHCPS2_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_DNSR_PROPERTY,   *PSYS_MCO_DNSR_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_RGWM_PROPERTY,   *PSYS_MCO_RGWM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_RAM_PROPERTY,    *PSYS_MCO_RAM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_SNMP_PROPERTY,   *PSYS_MCO_SNMP_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_MIB2_PROPERTY,   *PSYS_MCO_MIB2_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_MOAM_PROPERTY,   *PSYS_MCO_MOAM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_RIP2_PROPERTY,   *PSYS_MCO_RIP2_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_CYFW_PROPERTY,   *PSYS_MCO_CYFW_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_NAT_PROPERTY,    *PSYS_MCO_NAT_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_KB5C_PROPERTY,   *PSYS_MCO_KB5C_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_KB5S_PROPERTY,   *PSYS_MCO_KB5S_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_KB5A_PROPERTY,   *PSYS_MCO_KB5A_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_KBKMC_PROPERTY,  *PSYS_MCO_KBKMC_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_KBKMS_PROPERTY,  *PSYS_MCO_KBKMS_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_CBHM10_PROPERTY, *PSYS_MCO_CBHM10_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_GFWM_PROPERTY,   *PSYS_MCO_GFWM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_GNAT_PROPERTY,   *PSYS_MCO_GNAT_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_CTP_PROPERTY,    *PSYS_MCO_CTP_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_IPSEC_PROPERTY,  *PSYS_MCO_IPSEC_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_CCGM_PROPERTY,   *PSYS_MCO_CCGM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BEEP_PROPERTY,   *PSYS_MCO_BEEP_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BREE_PROPERTY,   *PSYS_MCO_BREE_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BWRM_PROPERTY,   *PSYS_MCO_BWRM_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_BWSP_PROPERTY,   *PSYS_MCO_BWSP_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_SLAP_PROPERTY,   *PSYS_MCO_SLAP_PROPERTY;
typedef  struct  _SYS_MODULE_CARRIER_PROPERTY  SYS_MCO_TLS_PROPERTY,    *PSYS_MCO_TLS_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_SETUP_MANAGER_PROPERTY_CONTENT                                                             \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    char                            CsetupName[SYS_MAX_MODULE_NAME_SIZE];                               \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_SETUP_MANAGER_PROPERTY
{
    SYS_SETUP_MANAGER_PROPERTY_CONTENT
}
SYS_SETUP_MANAGER_PROPERTY,  *PSYS_SETUP_MANAGER_PROPERTY;

typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_NDL_PROPERTY,    *PSYS_SMO_NDL_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_SLIB_PROPERTY,   *PSYS_SMO_SLIB_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_BSS_PROPERTY,    *PSYS_SMO_BSS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_GNPS_PROPERTY,   *PSYS_SMO_GNPS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_IP4S_PROPERTY,   *PSYS_SMO_IP4S_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_TCPS_PROPERTY,   *PSYS_SMO_TCPS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_UDPS_PROPERTY,   *PSYS_SMO_UDPS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_GTPS_PROPERTY,   *PSYS_SMO_GTPS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_PNS_PROPERTY,    *PSYS_SMO_PNS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_PAC_PROPERTY,    *PSYS_SMO_PAC_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_LNS_PROPERTY,    *PSYS_SMO_LNS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_LAC_PROPERTY,    *PSYS_SMO_LAC_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_DHCPS_PROPERTY,  *PSYS_SMO_DHCPS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_DHCPS2_PROPERTY, *PSYS_SMO_DHCPS2_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_DNSR_PROPERTY,   *PSYS_SMO_DNSR_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_RGWM_PROPERTY,   *PSYS_SMO_RGWM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_RAM_PROPERTY,    *PSYS_SMO_RAM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_SNMP_PROPERTY,   *PSYS_SMO_SNMP_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_MIB2_PROPERTY,   *PSYS_SMO_MIB2_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_MOAM_PROPERTY,   *PSYS_SMO_MOAM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_RIP2_PROPERTY,   *PSYS_SMO_RIP2_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_CYFW_PROPERTY,   *PSYS_SMO_CYFW_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_NAT_PROPERTY,    *PSYS_SMO_NAT_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_KB5C_PROPERTY,   *PSYS_SMO_KB5C_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_KB5S_PROPERTY,   *PSYS_SMO_KB5S_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_KB5A_PROPERTY,   *PSYS_SMO_KB5A_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_KBKMC_PROPERTY,  *PSYS_SMO_KBKMC_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_KBKMS_PROPERTY,  *PSYS_SMO_KBKMS_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_CBHM10_PROPERTY, *PSYS_SMO_CBHM10_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_GFWM_PROPERTY,   *PSYS_SMO_GFWM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_GNAT_PROPERTY,   *PSYS_SMO_GNAT_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_CTP_PROPERTY,    *PSYS_SMO_CTP_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_IPSEC_PROPERTY,  *PSYS_SMO_IPSEC_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_CCGM_PROPERTY,   *PSYS_SMO_CCGM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_BEEP_PROPERTY,   *PSYS_SMO_BEEP_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_BREE_PROPERTY,   *PSYS_SMO_BREE_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_BWRM_PROPERTY,   *PSYS_SMO_BWRM_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_BWSP_PROPERTY,   *PSYS_SMO_BWSP_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_SLAP_PROPERTY,   *PSYS_SMO_SLAP_PROPERTY;
typedef  struct  _SYS_SETUP_MANAGER_PROPERTY  SYS_SMO_TLS_PROPERTY,    *PSYS_SMO_TLS_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_ACCESS_MANAGER_PROPERTY_CONTENT                                                            \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_ACCESS_MANAGER_PROPERTY
{
    SYS_ACCESS_MANAGER_PROPERTY_CONTENT
}
SYS_ACCESS_MANAGER_PROPERTY,  *PSYS_ACCESS_MANAGER_PROPERTY;

typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_NDL_PROPERTY,    *PSYS_AMO_NDL_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_SLIB_PROPERTY,   *PSYS_AMO_SLIB_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_BSS_PROPERTY,    *PSYS_AMO_BSS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_GNPS_PROPERTY,   *PSYS_AMO_GNPS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_IP4S_PROPERTY,   *PSYS_AMO_IP4S_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_TCPS_PROPERTY,   *PSYS_AMO_TCPS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_UDPS_PROPERTY,   *PSYS_AMO_UDPS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_GTPS_PROPERTY,   *PSYS_AMO_GTPS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_PNS_PROPERTY,    *PSYS_AMO_PNS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_PAC_PROPERTY,    *PSYS_AMO_PAC_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_LNS_PROPERTY,    *PSYS_AMO_LNS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_LAC_PROPERTY,    *PSYS_AMO_LAC_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_DHCPS_PROPERTY,  *PSYS_AMO_DHCPS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_DHCPS2_PROPERTY, *PSYS_AMO_DHCPS2_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_DNSR_PROPERTY,   *PSYS_AMO_DNSR_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_RGWM_PROPERTY,   *PSYS_AMO_RGWM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_RAM_PROPERTY,    *PSYS_AMO_RAM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_SNMP_PROPERTY,   *PSYS_AMO_SNMP_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_MIB2_PROPERTY,   *PSYS_AMO_MIB2_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_MOAM_PROPERTY,   *PSYS_AMO_MOAM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_RIP2_PROPERTY,   *PSYS_AMO_RIP2_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_CYFW_PROPERTY,   *PSYS_AMO_CYFW_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_NAT_PROPERTY,    *PSYS_AMO_NAT_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_KB5C_PROPERTY,   *PSYS_AMO_KB5C_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_KB5S_PROPERTY,   *PSYS_AMO_KB5S_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_KB5A_PROPERTY,   *PSYS_AMO_KB5A_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_KBKMC_PROPERTY,  *PSYS_AMO_KBKMC_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_KBKMS_PROPERTY,  *PSYS_AMO_KBKMS_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_CBHM10_PROPERTY, *PSYS_AMO_CBHM10_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_GFWM_PROPERTY,   *PSYS_AMO_GFWM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_GNAT_PROPERTY,   *PSYS_AMO_GNAT_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_CTP_PROPERTY,    *PSYS_AMO_CTP_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_IPSEC_PROPERTY,  *PSYS_AMO_IPSEC_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_CCGM_PROPERTY,   *PSYS_AMO_CCGM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_BEEP_PROPERTY,   *PSYS_AMO_BEEP_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_BREE_PROPERTY,   *PSYS_AMO_BREE_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_BWRM_PROPERTY,   *PSYS_AMO_BWRM_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_BWSP_PROPERTY,   *PSYS_AMO_BWSP_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_SLAP_PROPERTY,   *PSYS_AMO_SLAP_PROPERTY;
typedef  struct  _SYS_ACCESS_MANAGER_PROPERTY  SYS_AMO_TLS_PROPERTY,    *PSYS_AMO_TLS_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_PROVISION_MANAGER_PROPERTY_CONTENT                                                         \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_PROVISION_MANAGER_PROPERTY
{
    SYS_PROVISION_MANAGER_PROPERTY_CONTENT
}
SYS_PROVISION_MANAGER_PROPERTY,  *PSYS_PROVISION_MANAGER_PROPERTY;

typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_NDL_PROPERTY,    *PSYS_PMO_NDL_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_SLIB_PROPERTY,   *PSYS_PMO_SLIB_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_BSS_PROPERTY,    *PSYS_PMO_BSS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_GNPS_PROPERTY,   *PSYS_PMO_GNPS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_IP4S_PROPERTY,   *PSYS_PMO_IP4S_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_TCPS_PROPERTY,   *PSYS_PMO_TCPS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_UDPS_PROPERTY,   *PSYS_PMO_UDPS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_GTPS_PROPERTY,   *PSYS_PMO_GTPS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_PNS_PROPERTY,    *PSYS_PMO_PNS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_PAC_PROPERTY,    *PSYS_PMO_PAC_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_LNS_PROPERTY,    *PSYS_PMO_LNS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_LAC_PROPERTY,    *PSYS_PMO_LAC_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_DHCPS_PROPERTY,  *PSYS_PMO_DHCPS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_DHCPS2_PROPERTY, *PSYS_PMO_DHCPS2_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_DNSR_PROPERTY,   *PSYS_PMO_DNSR_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_RGWM_PROPERTY,   *PSYS_PMO_RGWM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_RAM_PROPERTY,    *PSYS_PMO_RAM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_SNMP_PROPERTY,   *PSYS_PMO_SNMP_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_MIB2_PROPERTY,   *PSYS_PMO_MIB2_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_MOAM_PROPERTY,   *PSYS_PMO_MOAM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_RIP2_PROPERTY,   *PSYS_PMO_RIP2_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_CYFW_PROPERTY,   *PSYS_PMO_CYFW_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_NAT_PROPERTY,    *PSYS_PMO_NAT_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_KB5C_PROPERTY,   *PSYS_PMO_KB5C_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_KB5S_PROPERTY,   *PSYS_PMO_KB5S_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_KB5A_PROPERTY,   *PSYS_PMO_KB5A_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_KBKMC_PROPERTY,  *PSYS_PMO_KBKMC_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_KBKMS_PROPERTY,  *PSYS_PMO_KBKMS_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_CBHM10_PROPERTY, *PSYS_PMO_CBHM10_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_GFWM_PROPERTY,   *PSYS_PMO_GFWM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_GNAT_PROPERTY,   *PSYS_PMO_GNAT_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_CTP_PROPERTY,    *PSYS_PMO_CTP_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_IPSEC_PROPERTY,  *PSYS_PMO_IPSEC_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_CCGM_PROPERTY,   *PSYS_PMO_CCGM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_BEEP_PROPERTY,   *PSYS_PMO_BEEP_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_BREE_PROPERTY,   *PSYS_PMO_BREE_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_BWRM_PROPERTY,   *PSYS_PMO_BWRM_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_BWSP_PROPERTY,   *PSYS_PMO_BWSP_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_SLAP_PROPERTY,   *PSYS_PMO_SLAP_PROPERTY;
typedef  struct  _SYS_PROVISION_MANAGER_PROPERTY  SYS_PMO_TLS_PROPERTY,    *PSYS_PMO_TLS_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_RUNTIME_MONITOR_PROPERTY_CONTENT                                                           \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    char                            MonitorName[SYS_MAX_MODULE_NAME_SIZE];                              \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_RUNTIME_MONITOR_PROPERTY
{
    SYS_RUNTIME_MONITOR_PROPERTY_CONTENT
}
SYS_RUNTIME_MONITOR_PROPERTY,  *PSYS_RUNTIME_MONITOR_PROPERTY;

typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_NDL_PROPERTY,    *PSYS_RMO_NDL_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_SLIB_PROPERTY,   *PSYS_RMO_SLIB_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_BSS_PROPERTY,    *PSYS_RMO_BSS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_GNPS_PROPERTY,   *PSYS_RMO_GNPS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_IP4S_PROPERTY,   *PSYS_RMO_IP4S_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_TCPS_PROPERTY,   *PSYS_RMO_TCPS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_UDPS_PROPERTY,   *PSYS_RMO_UDPS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_GTPS_PROPERTY,   *PSYS_RMO_GTPS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_PNS_PROPERTY,    *PSYS_RMO_PNS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_PAC_PROPERTY,    *PSYS_RMO_PAC_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_LNS_PROPERTY,    *PSYS_RMO_LNS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_LAC_PROPERTY,    *PSYS_RMO_LAC_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_DHCPS_PROPERTY,  *PSYS_RMO_DHCPS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_DHCPS2_PROPERTY, *PSYS_RMO_DHCPS2_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_DNSR_PROPERTY,   *PSYS_RMO_DNSR_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_RGWM_PROPERTY,   *PSYS_RMO_RGWM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_RAM_PROPERTY,    *PSYS_RMO_RAM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_SNMP_PROPERTY,   *PSYS_RMO_SNMP_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_MIB2_PROPERTY,   *PSYS_RMO_MIB2_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_MOAM_PROPERTY,   *PSYS_RMO_MOAM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_RIP2_PROPERTY,   *PSYS_RMO_RIP2_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_CYFW_PROPERTY,   *PSYS_RMO_CYFW_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_NAT_PROPERTY,    *PSYS_RMO_NAT_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_KB5C_PROPERTY,   *PSYS_RMO_KB5C_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_KB5S_PROPERTY,   *PSYS_RMO_KB5S_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_KB5A_PROPERTY,   *PSYS_RMO_KB5A_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_KBKMC_PROPERTY,  *PSYS_RMO_KBKMC_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_KBKMS_PROPERTY,  *PSYS_RMO_KBKMS_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_CBHM10_PROPERTY, *PSYS_RMO_CBHM10_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_GFWM_PROPERTY,   *PSYS_RMO_GFWM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_GNAT_PROPERTY,   *PSYS_RMO_GNAT_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_CTP_PROPERTY,    *PSYS_RMO_CTP_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_IPSEC_PROPERTY,  *PSYS_RMO_IPSEC_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_CCGM_PROPERTY,   *PSYS_RMO_CCGM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_BEEP_PROPERTY,   *PSYS_RMO_BEEP_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_BREE_PROPERTY,   *PSYS_RMO_BREE_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_BWRM_PROPERTY,   *PSYS_RMO_BWRM_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_BWSP_PROPERTY,   *PSYS_RMO_BWSP_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_SLAP_PROPERTY,   *PSYS_RMO_SLAP_PROPERTY;
typedef  struct  _SYS_RUNTIME_MONITOR_PROPERTY  SYS_RMO_TLS_PROPERTY,    *PSYS_RMO_TLS_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_NAME_MAPPER_PROPERTY_CONTENT                                                               \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    char                            MapperName[SYS_MAX_MODULE_NAME_SIZE];                               \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_NAME_MAPPER_PROPERTY
{
    SYS_NAME_MAPPER_PROPERTY_CONTENT
}
SYS_NAME_MAPPER_PROPERTY,  *PSYS_NAME_MAPPER_PROPERTY;

typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_APP_PROPERTY,   *PSYS_NMO_APP_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_PROTO_PROPERTY, *PSYS_NMO_PROTO_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_LINK_PROPERTY,  *PSYS_NMO_LINK_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_SRVS_PROPERTY,  *PSYS_NMO_SRVS_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_VPNP_PROPERTY,  *PSYS_NMO_VPNP_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_RIP2_PROPERTY,  *PSYS_NMO_RIP2_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_KB5C_PROPERTY,  *PSYS_NMO_KB5C_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_KB5S_PROPERTY,  *PSYS_NMO_KB5S_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_KB5A_PROPERTY,  *PSYS_NMO_KB5A_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_KBKMC_PROPERTY, *PSYS_NMO_KBKMC_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_KBKMS_PROPERTY, *PSYS_NMO_KBKMS_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_IPSEC_PROPERTY, *PSYS_NMO_IPSEC_PROPERTY;
typedef  struct  _SYS_NAME_MAPPER_PROPERTY  SYS_NMO_CYFW_PROPERTY,  *PSYS_NMO_CYFW_PROPERTY;

/*
 * To build a bisga package, we must encapsulate all the components we have developed as loadable
 * modules. A loadable module constructs a self-contained runtime environment for the encapsulated
 * component, just like the legacy "ssp" mechanism used to demostrate the functionality of each
 * newly developed component. In addition to creating and managing the functional comoponent, the
 * Module Carrier Object also answers queries to the module dependencies.
 */
#define  SYS_SERVICE_BROKER_PROPERTY_CONTENT                                                            \
    /* duplication of the base object class content */                                                  \
    SYS_MODULE_CARRIER_PROPERTY_CONTENT                                                                 \
    /* start of object class content */                                                                 \
    /* end of object class content */                                                                   \

typedef  struct
_SYS_SERVICE_BROKER_PROPERTY
{
    SYS_SERVICE_BROKER_PROPERTY_CONTENT
}
SYS_SERVICE_BROKER_PROPERTY,  *PSYS_SERVICE_BROKER_PROPERTY;

typedef  struct  _SYS_SERVICE_BROKER_PROPERTY  SYS_SBO_IP4S_PROPERTY,  *PSYS_SBO_IP4S_PROPERTY;
typedef  struct  _SYS_SERVICE_BROKER_PROPERTY  SYS_SBO_WEBS_PROPERTY,  *PSYS_SBO_WEBS_PROPERTY;


/***********************************************************
             SYS MODULE LOADER PROPERTY DEFINITION
***********************************************************/

/*
 * The Sys Module Loader Object is responsible for loading both internal component modules and
 * externally registered modules (via psp_register_module() API function). This task can be broken
 * down into three stages:
 *
 *      (1) calculate the module dependencies
 *      (2) load all the dependent modules
 *      (3) load the target module
 */
typedef  struct
_SYS_MODULE_LOADER_PROPERTY
{
    ULONG                           Dummy;
}
SYS_MODULE_LOADER_PROPERTY,  *PSYS_MODULE_LOADER_PROPERTY;


/***********************************************************
              SYS RECYCLE BIN PROPERTY DEFINITION
***********************************************************/

/*
 * The primary use of the Recycle Bin Object is to tracking the changes made to certain Repository
 * Folder and Record Objects, which is useful information for configuration management and policy
 * administration. However instead of implementing it as an inherent feature of the Repository
 * Folder Object, we make it attachable to the folder to avoid wasting resource if not needed.
 */
typedef  struct
_SYS_RECYCLE_BIN_PROPERTY
{
    ULONG                           Dummy;
}
SYS_RECYCLE_BIN_PROPERTY,  *PSYS_RECYCLE_BIN_PROPERTY;


/***********************************************************
           SYS REPOSITORY DRIVER PROPERTY DEFINITION
***********************************************************/

/*
 * The primary difference between the Info Repository Object and the Repository Driver Object is
 * that SysIro is a system-dependent object for providing the required information-access interface
 * while this object implements a relatively independent tree-like data-management mechanism. Such
 * separation is not very rare across the ANSC platform.
 */
#define  SYS_RDO_PATH_SEPARATORS                    "/"

typedef  struct
_SYS_REPOSITORY_DRIVER_PROPERTY
{
    char                            DriverName[SYS_MAX_DRIVER_NAME_SIZE];
}
SYS_REPOSITORY_DRIVER_PROPERTY,  *PSYS_REPOSITORY_DRIVER_PROPERTY;


/***********************************************************
           SYS REPOSITORY FOLDER PROPERTY DEFINITION
***********************************************************/

/*
 * Reassembling the typical operations related to a tree-node, the Sys Repository Folder Object
 * manages multiple sub folders and a set of records. Instead of deriving more-specific folder
 * object for each of the three folder types: storage, guarded, and dynamic. We simple encapsulate
 * all the necessary interfaces in this base object while still opens to the possibility that
 * internal/external modules may further implement special folders.
 */
typedef  struct
_SYS_REPOSITORY_FOLDER_PROPERTY
{
    char                            FolderName[SYS_MAX_FOLDER_NAME_SIZE];
    ULONG                           FolderType;
    ULONG                           Permission;
}
SYS_REPOSITORY_FOLDER_PROPERTY,  *PSYS_REPOSITORY_FOLDER_PROPERTY;


#endif
