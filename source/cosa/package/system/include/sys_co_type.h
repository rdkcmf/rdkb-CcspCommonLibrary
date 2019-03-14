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

    module:	sys_co_type.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Sys
        Component Objects.

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


#ifndef  _SYS_CO_TYPE_
#define  _SYS_CO_TYPE_


/***********************************************************
             GENERAL SYS FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SYS_PACKAGE_WRAPPER_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  SYS_ENV_CONTROLLER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SYS_SECURITY_AGENT_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SYS_USER_PROFILE_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SYS_INFO_REPOSITORY_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  SYS_RECYCLE_BIN_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SYS_MODULE_TABLE_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SYS_MODULE_ENTRY_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SYS_MODULE_CARRIER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SYS_MODULE_LOADER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  SYS_MODULE_MONITOR_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SYS_ACCESS_MANAGER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SYS_SETUP_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  SYS_SETUP_STORAGE_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  SYS_PROVISION_MANAGER_TYPE                 ANSC_OBJECT_TYPE_NULL
#define  SYS_RUNTIME_MONITOR_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  SYS_NAME_MAPPER_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SYS_NAME_MAPPER_SIMPLE_TYPE                ANSC_OBJECT_TYPE_NULL
#define  SYS_SERVICE_BROKER_TYPE                    ANSC_OBJECT_TYPE_NULL

#define  SYS_REPOSITORY_DRIVER_TYPE                 ANSC_OBJECT_TYPE_NULL
#define  SYS_REPOSITORY_FOLDER_TYPE                 ANSC_OBJECT_TYPE_NULL
#define  SYS_REPOSITORY_RECORD_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  SYS_RRO_SINT_TYPE                          "sint"
#define  SYS_RRO_UINT_TYPE                          "uint"
#define  SYS_RRO_BOOL_TYPE                          "bool"
#define  SYS_RRO_ASTR_TYPE                          "astr"
#define  SYS_RRO_BSTR_TYPE                          "bstr"
#define  SYS_RRO_HCXT_TYPE                          "hcxt"
#define  SYS_RRO_ENUM_TYPE                          "enum"

#define  SYS_MCO_NDL_TYPE                           "ndl"
#define  SYS_MCO_SLIB_TYPE                          "slib"
#define  SYS_MCO_BSS_TYPE                           "bss"
#define  SYS_MCO_GNPS_TYPE                          "gnps"
#define  SYS_MCO_IP4S_TYPE                          "ip4s"
#define  SYS_MCO_TCPS_TYPE                          "tcps"
#define  SYS_MCO_UDPS_TYPE                          "udps"
#define  SYS_MCO_GTPS_TYPE                          "gtps"
#define  SYS_MCO_PNS_TYPE                           "pns"
#define  SYS_MCO_PAC_TYPE                           "pac"
#define  SYS_MCO_LNS_TYPE                           "lns"
#define  SYS_MCO_LAC_TYPE                           "lac"
#define  SYS_MCO_RIP2_TYPE                          "rip2"
#define  SYS_MCO_OSPF_TYPE                          "ospf"
#define  SYS_MCO_DHCPS_TYPE                         "dhcps"
#define  SYS_MCO_DHCPS2_TYPE                        "dhcps2"
#define  SYS_MCO_DNSR_TYPE                          "dnsr"
#define  SYS_MCO_WEBP_TYPE                          "webp"
#define  SYS_MCO_WEBS_TYPE                          "webs"
#define  SYS_MCO_BMW_TYPE                           "bmw"
#define  SYS_MCO_WCP_TYPE                           "wcp"
#define  SYS_MCO_RGWM_TYPE                          "rgwm"
#define  SYS_MCO_RAM_TYPE                           "ram"
#define  SYS_MCO_CYFW_TYPE                          "cyfw"
#define  SYS_MCO_NAT_TYPE                           "nat"
#define  SYS_MCO_SNMP_TYPE                          "snmp"
#define  SYS_MCO_MIB2_TYPE                          "mib2"
#define  SYS_MCO_MOAM_TYPE                          "moam"
#define  SYS_MCO_KB5C_TYPE                          "kb5c"
#define  SYS_MCO_KB5S_TYPE                          "kb5s"
#define  SYS_MCO_KB5A_TYPE                          "kb5a"
#define  SYS_MCO_KBKMC_TYPE                         "kbkmc"
#define  SYS_MCO_KBKMS_TYPE                         "kbkms"
#define  SYS_MCO_CBHM10_TYPE                        "cbhm10"
#define  SYS_MCO_GFWM_TYPE                          "gfwm"
#define  SYS_MCO_GNAT_TYPE                          "gnat"
#define  SYS_MCO_CTP_TYPE                           "ctp"
#define  SYS_MCO_IPSEC_TYPE                         "ipsec"
#define  SYS_MCO_CCGM_TYPE                          "ccgm"
#define  SYS_MCO_BEEP_TYPE                          "beep"
#define  SYS_MCO_BREE_TYPE                          "bree"
#define  SYS_MCO_BWRM_TYPE                          "bwrm"
#define  SYS_MCO_BWSP_TYPE                          "bwsp"
#define  SYS_MCO_SLAP_TYPE                          "slap"
#define  SYS_MCO_TLS_TYPE                           "tls"

#define  SYS_AMO_NDL_TYPE                           "ndl"
#define  SYS_AMO_SLIB_TYPE                          "slib"
#define  SYS_AMO_BSS_TYPE                           "bss"
#define  SYS_AMO_GNPS_TYPE                          "gnps"
#define  SYS_AMO_IP4S_TYPE                          "ip4s"
#define  SYS_AMO_TCPS_TYPE                          "tcps"
#define  SYS_AMO_UDPS_TYPE                          "udps"
#define  SYS_AMO_GTPS_TYPE                          "gtps"
#define  SYS_AMO_PNS_TYPE                           "pns"
#define  SYS_AMO_PAC_TYPE                           "pac"
#define  SYS_AMO_LNS_TYPE                           "lns"
#define  SYS_AMO_LAC_TYPE                           "lac"
#define  SYS_AMO_RIP2_TYPE                          "rip2"
#define  SYS_AMO_OSPF_TYPE                          "ospf"
#define  SYS_AMO_DHCPS_TYPE                         "dhcps"
#define  SYS_AMO_DHCPS2_TYPE                        "dhcps2"
#define  SYS_AMO_DNSR_TYPE                          "dnsr"
#define  SYS_AMO_WEBP_TYPE                          "webp"
#define  SYS_AMO_WEBS_TYPE                          "webs"
#define  SYS_AMO_BMW_TYPE                           "bmw"
#define  SYS_AMO_WCP_TYPE                           "wcp"
#define  SYS_AMO_RGWM_TYPE                          "rgwm"
#define  SYS_AMO_RAM_TYPE                           "ram"
#define  SYS_AMO_CYFW_TYPE                          "cyfw"
#define  SYS_AMO_NAT_TYPE                           "nat"
#define  SYS_AMO_SNMP_TYPE                          "snmp"
#define  SYS_AMO_MIB2_TYPE                          "mib2"
#define  SYS_AMO_MOAM_TYPE                          "moam"
#define  SYS_AMO_KB5C_TYPE                          "kb5c"
#define  SYS_AMO_KB5S_TYPE                          "kb5s"
#define  SYS_AMO_KB5A_TYPE                          "kb5a"
#define  SYS_AMO_KBKMC_TYPE                         "kbkmc"
#define  SYS_AMO_KBKMS_TYPE                         "kbkms"
#define  SYS_AMO_CBHM10_TYPE                        "cbhm10"
#define  SYS_AMO_GFWM_TYPE                          "gfwm"
#define  SYS_AMO_GNAT_TYPE                          "gnat"
#define  SYS_AMO_CTP_TYPE                           "ctp"
#define  SYS_AMO_IPSEC_TYPE                         "ipsec"
#define  SYS_AMO_CCGM_TYPE                          "ccgm"
#define  SYS_AMO_BEEP_TYPE                          "beep"
#define  SYS_AMO_BREE_TYPE                          "bree"
#define  SYS_AMO_BWRM_TYPE                          "bwrm"
#define  SYS_AMO_BWSP_TYPE                          "bwsp"
#define  SYS_AMO_SLAP_TYPE                          "slap"
#define  SYS_AMO_TLS_TYPE                           "tls"

#define  SYS_SMO_NDL_TYPE                           "ndl"
#define  SYS_SMO_SLIB_TYPE                          "slib"
#define  SYS_SMO_BSS_TYPE                           "bss"
#define  SYS_SMO_GNPS_TYPE                          "gnps"
#define  SYS_SMO_IP4S_TYPE                          "ip4s"
#define  SYS_SMO_TCPS_TYPE                          "tcps"
#define  SYS_SMO_UDPS_TYPE                          "udps"
#define  SYS_SMO_GTPS_TYPE                          "gtps"
#define  SYS_SMO_PNS_TYPE                           "pns"
#define  SYS_SMO_PAC_TYPE                           "pac"
#define  SYS_SMO_LNS_TYPE                           "lns"
#define  SYS_SMO_LAC_TYPE                           "lac"
#define  SYS_SMO_RIP2_TYPE                          "rip2"
#define  SYS_SMO_OSPF_TYPE                          "ospf"
#define  SYS_SMO_DHCPS_TYPE                         "dhcps"
#define  SYS_SMO_DHCPS2_TYPE                        "dhcps2"
#define  SYS_SMO_DNSR_TYPE                          "dnsr"
#define  SYS_SMO_WEBP_TYPE                          "webp"
#define  SYS_SMO_WEBS_TYPE                          "webs"
#define  SYS_SMO_BMW_TYPE                           "bmw"
#define  SYS_SMO_WCP_TYPE                           "wcp"
#define  SYS_SMO_RGWM_TYPE                          "rgwm"
#define  SYS_SMO_RAM_TYPE                           "ram"
#define  SYS_SMO_CYFW_TYPE                          "cyfw"
#define  SYS_SMO_NAT_TYPE                           "nat"
#define  SYS_SMO_SNMP_TYPE                          "snmp"
#define  SYS_SMO_MIB2_TYPE                          "mib2"
#define  SYS_SMO_MOAM_TYPE                          "moam"
#define  SYS_SMO_KB5C_TYPE                          "kb5c"
#define  SYS_SMO_KB5S_TYPE                          "kb5s"
#define  SYS_SMO_KB5A_TYPE                          "kb5a"
#define  SYS_SMO_KBKMC_TYPE                         "kbkmc"
#define  SYS_SMO_KBKMS_TYPE                         "kbkms"
#define  SYS_SMO_CBHM10_TYPE                        "cbhm10"
#define  SYS_SMO_GFWM_TYPE                          "gfwm"
#define  SYS_SMO_GNAT_TYPE                          "gnat"
#define  SYS_SMO_CTP_TYPE                           "ctp"
#define  SYS_SMO_IPSEC_TYPE                         "ipsec"
#define  SYS_SMO_CCGM_TYPE                          "ccgm"
#define  SYS_SMO_BEEP_TYPE                          "beep"
#define  SYS_SMO_BREE_TYPE                          "bree"
#define  SYS_SMO_BWRM_TYPE                          "bwrm"
#define  SYS_SMO_BWSP_TYPE                          "bwsp"
#define  SYS_SMO_SLAP_TYPE                          "slap"
#define  SYS_SMO_TLS_TYPE                           "tls"

#define  SYS_PMO_NDL_TYPE                           "ndl"
#define  SYS_PMO_SLIB_TYPE                          "slib"
#define  SYS_PMO_BSS_TYPE                           "bss"
#define  SYS_PMO_GNPS_TYPE                          "gnps"
#define  SYS_PMO_IP4S_TYPE                          "ip4s"
#define  SYS_PMO_TCPS_TYPE                          "tcps"
#define  SYS_PMO_UDPS_TYPE                          "udps"
#define  SYS_PMO_GTPS_TYPE                          "gtps"
#define  SYS_PMO_PNS_TYPE                           "pns"
#define  SYS_PMO_PAC_TYPE                           "pac"
#define  SYS_PMO_LNS_TYPE                           "lns"
#define  SYS_PMO_LAC_TYPE                           "lac"
#define  SYS_PMO_RIP2_TYPE                          "rip2"
#define  SYS_PMO_OSPF_TYPE                          "ospf"
#define  SYS_PMO_DHCPS_TYPE                         "dhcps"
#define  SYS_PMO_DHCPS2_TYPE                        "dhcps2"
#define  SYS_PMO_DNSR_TYPE                          "dnsr"
#define  SYS_PMO_WEBP_TYPE                          "webp"
#define  SYS_PMO_WEBS_TYPE                          "webs"
#define  SYS_PMO_BMW_TYPE                           "bmw"
#define  SYS_PMO_WCP_TYPE                           "wcp"
#define  SYS_PMO_RGWM_TYPE                          "rgwm"
#define  SYS_PMO_RAM_TYPE                           "ram"
#define  SYS_PMO_CYFW_TYPE                          "cyfw"
#define  SYS_PMO_NAT_TYPE                           "nat"
#define  SYS_PMO_SNMP_TYPE                          "snmp"
#define  SYS_PMO_MIB2_TYPE                          "mib2"
#define  SYS_PMO_MOAM_TYPE                          "moam"
#define  SYS_PMO_KB5C_TYPE                          "kb5c"
#define  SYS_PMO_KB5S_TYPE                          "kb5s"
#define  SYS_PMO_KB5A_TYPE                          "kb5a"
#define  SYS_PMO_KBKMC_TYPE                         "kbkmc"
#define  SYS_PMO_KBKMS_TYPE                         "kbkms"
#define  SYS_PMO_CBHM10_TYPE                        "cbhm10"
#define  SYS_PMO_GFWM_TYPE                          "gfwm"
#define  SYS_PMO_GNAT_TYPE                          "gnat"
#define  SYS_PMO_CTP_TYPE                           "ctp"
#define  SYS_PMO_IPSEC_TYPE                         "ipsec"
#define  SYS_PMO_CCGM_TYPE                          "ccgm"
#define  SYS_PMO_BEEP_TYPE                          "beep"
#define  SYS_PMO_BREE_TYPE                          "bree"
#define  SYS_PMO_BWRM_TYPE                          "bwrm"
#define  SYS_PMO_BWSP_TYPE                          "bwsp"
#define  SYS_PMO_SLAP_TYPE                          "slap"
#define  SYS_PMO_TLS_TYPE                           "tls"

#define  SYS_RMO_NDL_TYPE                           "ndl"
#define  SYS_RMO_SLIB_TYPE                          "slib"
#define  SYS_RMO_BSS_TYPE                           "bss"
#define  SYS_RMO_GNPS_TYPE                          "gnps"
#define  SYS_RMO_IP4S_TYPE                          "ip4s"
#define  SYS_RMO_TCPS_TYPE                          "tcps"
#define  SYS_RMO_UDPS_TYPE                          "udps"
#define  SYS_RMO_GTPS_TYPE                          "gtps"
#define  SYS_RMO_PNS_TYPE                           "pns"
#define  SYS_RMO_PAC_TYPE                           "pac"
#define  SYS_RMO_LNS_TYPE                           "lns"
#define  SYS_RMO_LAC_TYPE                           "lac"
#define  SYS_RMO_RIP2_TYPE                          "rip2"
#define  SYS_RMO_OSPF_TYPE                          "ospf"
#define  SYS_RMO_DHCPS_TYPE                         "dhcps"
#define  SYS_RMO_DHCPS2_TYPE                        "dhcps2"
#define  SYS_RMO_DNSR_TYPE                          "dnsr"
#define  SYS_RMO_WEBP_TYPE                          "webp"
#define  SYS_RMO_WEBS_TYPE                          "webs"
#define  SYS_RMO_BMW_TYPE                           "bmw"
#define  SYS_RMO_WCP_TYPE                           "wcp"
#define  SYS_RMO_RGWM_TYPE                          "rgwm"
#define  SYS_RMO_RAM_TYPE                           "ram"
#define  SYS_RMO_CYFW_TYPE                          "cyfw"
#define  SYS_RMO_NAT_TYPE                           "nat"
#define  SYS_RMO_SNMP_TYPE                          "snmp"
#define  SYS_RMO_MIB2_TYPE                          "mib2"
#define  SYS_RMO_MOAM_TYPE                          "moam"
#define  SYS_RMO_KB5C_TYPE                          "kb5c"
#define  SYS_RMO_KB5S_TYPE                          "kb5s"
#define  SYS_RMO_KB5A_TYPE                          "kb5a"
#define  SYS_RMO_KBKMC_TYPE                         "kbkmc"
#define  SYS_RMO_KBKMS_TYPE                         "kbkms"
#define  SYS_RMO_CBHM10_TYPE                        "cbhm10"
#define  SYS_RMO_GFWM_TYPE                          "gfwm"
#define  SYS_RMO_GNAT_TYPE                          "gnat"
#define  SYS_RMO_CTP_TYPE                           "ctp"
#define  SYS_RMO_IPSEC_TYPE                         "ipsec"
#define  SYS_RMO_CCGM_TYPE                          "ccgm"
#define  SYS_RMO_BEEP_TYPE                          "beep"
#define  SYS_RMO_BREE_TYPE                          "bree"
#define  SYS_RMO_BWRM_TYPE                          "bwrm"
#define  SYS_RMO_BWSP_TYPE                          "bwsp"
#define  SYS_RMO_SLAP_TYPE                          "slap"
#define  SYS_RMO_TLS_TYPE                           "tls"

#define  SYS_NMO_APP_TYPE                           "app"
#define  SYS_NMO_PROTO_TYPE                         "proto"
#define  SYS_NMO_LINK_TYPE                          "link"
#define  SYS_NMO_SRVS_TYPE                          "srvs"
#define  SYS_NMO_VPNP_TYPE                          "vpnp"
#define  SYS_NMO_RIP2_TYPE                          "rip2"
#define  SYS_NMO_KB5C_TYPE                          "kb5c"
#define  SYS_NMO_KB5S_TYPE                          "kb5s"
#define  SYS_NMO_KB5A_TYPE                          "kb5a"
#define  SYS_NMO_KBKMC_TYPE                         "kbkmc"
#define  SYS_NMO_KBKMS_TYPE                         "kbkms"
#define  SYS_NMO_IPSEC_TYPE                         "ipsec"
#define  SYS_NMO_CYFW_TYPE                          "cyfw"

#define  SYS_SBO_IP4S_TYPE                          "ip4s"
#define  SYS_SBO_WEBS_TYPE                          "webs"


#endif
