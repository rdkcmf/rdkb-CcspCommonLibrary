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

    module:	sys_co_name.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the Sys
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


#ifndef  _SYS_CO_NAME_
#define  _SYS_CO_NAME_


/***********************************************************
            GENERAL SYS FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SYS_PACKAGE_WRAPPER_NAME                   "packageWrapper"
#define  SYS_ENV_CONTROLLER_NAME                    "envController"
#define  SYS_SECURITY_AGENT_NAME                    "securityAgent"
#define  SYS_USER_PROFILE_NAME                      "userProfile"
#define  SYS_INFO_REPOSITORY_NAME                   "infoRepository"
#define  SYS_RECYCLE_BIN_NAME                       "recycleBin"
#define  SYS_MODULE_TABLE_NAME                      "moduleTable"
#define  SYS_MODULE_ENTRY_NAME                      "moduleEntry"
#define  SYS_MODULE_CARRIER_NAME                    "moduleCarrier"
#define  SYS_MODULE_LOADER_NAME                     "moduleLoader"
#define  SYS_MODULE_MONITOR_NAME                    "moduleMonitor"
#define  SYS_ACCESS_MANAGER_NAME                    "accessManager"
#define  SYS_SETUP_MANAGER_NAME                     "setupManager"
#define  SYS_SETUP_STORAGE_NAME                     "setupStorage"
#define  SYS_PROVISION_MANAGER_NAME                 "provisionManager"
#define  SYS_RUNTIME_MONITOR_NAME                   "runtimeMonitor"
#define  SYS_NAME_MAPPER_NAME                       "nameMapper"
#define  SYS_NAME_MAPPER_SIMPLE_NAME                "nameMapperSimple"
#define  SYS_SERVICE_BROKER_NAME                    "serviceBroker"

#define  SYS_REPOSITORY_DRIVER_NAME                 "repositoryDriver"
#define  SYS_REPOSITORY_FOLDER_NAME                 "repositoryFolder"
#define  SYS_REPOSITORY_RECORD_NAME                 "repositoryRecord"

#define  SYS_RRO_SINT_NAME                          "rroSint"
#define  SYS_RRO_UINT_NAME                          "rroUint"
#define  SYS_RRO_BOOL_NAME                          "rroBool"
#define  SYS_RRO_ASTR_NAME                          "rroAstr"
#define  SYS_RRO_BSTR_NAME                          "rroBstr"
#define  SYS_RRO_HCXT_NAME                          "rroHcxt"
#define  SYS_RRO_ENUM_NAME                          "rroEnum"

#define  SYS_MCO_NDL_NAME                           "mcoNdl"
#define  SYS_MCO_SLIB_NAME                          "mcoSlib"
#define  SYS_MCO_BSS_NAME                           "mcoBss"
#define  SYS_MCO_GNPS_NAME                          "mcoGnps"
#define  SYS_MCO_IP4S_NAME                          "mcoIp4s"
#define  SYS_MCO_TCPS_NAME                          "mcoTcps"
#define  SYS_MCO_UDPS_NAME                          "mcoUdps"
#define  SYS_MCO_GTPS_NAME                          "mcoGtps"
#define  SYS_MCO_PNS_NAME                           "mcoPns"
#define  SYS_MCO_PAC_NAME                           "mcoPac"
#define  SYS_MCO_LNS_NAME                           "mcoLns"
#define  SYS_MCO_LAC_NAME                           "mcoLac"
#define  SYS_MCO_RIP2_NAME                          "mcoRip2"
#define  SYS_MCO_OSPF_NAME                          "mcoOspf"
#define  SYS_MCO_DHCPS_NAME                         "mcoDhcps"
#define  SYS_MCO_DHCPS2_NAME                        "mcoDhcps2"
#define  SYS_MCO_DNSR_NAME                          "mcoDnsr"
#define  SYS_MCO_WEBP_NAME                          "mcoWebp"
#define  SYS_MCO_WEBS_NAME                          "mcoWebs"
#define  SYS_MCO_BMW_NAME                           "mcoBmw"
#define  SYS_MCO_WCP_NAME                           "mcoWcp"
#define  SYS_MCO_RGWM_NAME                          "mcoRgwm"
#define  SYS_MCO_RAM_NAME                           "mcoRam"
#define  SYS_MCO_CYFW_NAME                          "mcoCyfw"
#define  SYS_MCO_NAT_NAME                           "mcoNat"
#define  SYS_MCO_SNMP_NAME                          "mcoSnmp"
#define  SYS_MCO_MIB2_NAME                          "mcoMib2"
#define  SYS_MCO_MOAM_NAME                          "mcoMoam"
#define  SYS_MCO_KB5C_NAME                          "mcoKb5c"
#define  SYS_MCO_KB5S_NAME                          "mcoKb5s"
#define  SYS_MCO_KB5A_NAME                          "mcoKb5a"
#define  SYS_MCO_KBKMC_NAME                         "mcoKbkmc"
#define  SYS_MCO_KBKMS_NAME                         "mcoKbkms"
#define  SYS_MCO_CBHM10_NAME                        "mcoCbhm10"
#define  SYS_MCO_GFWM_NAME                          "mcoGfwm"
#define  SYS_MCO_GNAT_NAME                          "mcoGnat"
#define  SYS_MCO_CTP_NAME                           "mcoCtp"
#define  SYS_MCO_IPSEC_NAME                         "mcoIpsec"
#define  SYS_MCO_CCGM_NAME                          "mcoCcgm"
#define  SYS_MCO_BEEP_NAME                          "mcoBeep"
#define  SYS_MCO_BREE_NAME                          "mcoBree"
#define  SYS_MCO_BWRM_NAME                          "mcoBwrm"
#define  SYS_MCO_BWSP_NAME                          "mcoBwsp"
#define  SYS_MCO_SLAP_NAME                          "mcoSlap"
#define  SYS_MCO_TLS_NAME                           "mcoTls"

#define  SYS_AMO_NDL_NAME                           "amoNdl"
#define  SYS_AMO_SLIB_NAME                          "amoSlib"
#define  SYS_AMO_BSS_NAME                           "amoBss"
#define  SYS_AMO_GNPS_NAME                          "amoGnps"
#define  SYS_AMO_IP4S_NAME                          "amoIp4s"
#define  SYS_AMO_TCPS_NAME                          "amoTcps"
#define  SYS_AMO_UDPS_NAME                          "amoUdps"
#define  SYS_AMO_GTPS_NAME                          "amoGtps"
#define  SYS_AMO_PNS_NAME                           "amoPns"
#define  SYS_AMO_PAC_NAME                           "amoPac"
#define  SYS_AMO_LNS_NAME                           "amoLns"
#define  SYS_AMO_LAC_NAME                           "amoLac"
#define  SYS_AMO_RIP2_NAME                          "amoRip2"
#define  SYS_AMO_OSPF_NAME                          "amoOspf"
#define  SYS_AMO_DHCPS_NAME                         "amoDhcps"
#define  SYS_AMO_DHCPS2_NAME                        "amoDhcps2"
#define  SYS_AMO_DNSR_NAME                          "amoDnsr"
#define  SYS_AMO_WEBP_NAME                          "amoWebp"
#define  SYS_AMO_WEBS_NAME                          "amoWebs"
#define  SYS_AMO_BMW_NAME                           "amoBmw"
#define  SYS_AMO_WCP_NAME                           "amoWcp"
#define  SYS_AMO_RGWM_NAME                          "amoRgwm"
#define  SYS_AMO_RAM_NAME                           "amoRam"
#define  SYS_AMO_CYFW_NAME                          "amoCyfw"
#define  SYS_AMO_NAT_NAME                           "amoNat"
#define  SYS_AMO_SNMP_NAME                          "amoSnmp"
#define  SYS_AMO_MIB2_NAME                          "amoMib2"
#define  SYS_AMO_MOAM_NAME                          "amoMoam"
#define  SYS_AMO_KB5C_NAME                          "amoKb5c"
#define  SYS_AMO_KB5S_NAME                          "amoKb5s"
#define  SYS_AMO_KB5A_NAME                          "amoKb5a"
#define  SYS_AMO_KBKMC_NAME                         "amoKbkmc"
#define  SYS_AMO_KBKMS_NAME                         "amoKbkms"
#define  SYS_AMO_CBHM10_NAME                        "amoCbhm10"
#define  SYS_AMO_GFWM_NAME                          "amoGfwm"
#define  SYS_AMO_GNAT_NAME                          "amoGnat"
#define  SYS_AMO_CTP_NAME                           "amoCtp"
#define  SYS_AMO_IPSEC_NAME                         "amoIpsec"
#define  SYS_AMO_CCGM_NAME                          "amoCcgm"
#define  SYS_AMO_BEEP_NAME                          "amoBeep"
#define  SYS_AMO_BREE_NAME                          "amoBree"
#define  SYS_AMO_BWRM_NAME                          "amoBwrm"
#define  SYS_AMO_BWSP_NAME                          "amoBwsp"
#define  SYS_AMO_SLAP_NAME                          "amoSlap"
#define  SYS_AMO_TLS_NAME                           "amoTls"

#define  SYS_SMO_NDL_NAME                           "smoNdl"
#define  SYS_SMO_SLIB_NAME                          "smoSlib"
#define  SYS_SMO_BSS_NAME                           "smoBss"
#define  SYS_SMO_GNPS_NAME                          "smoGnps"
#define  SYS_SMO_IP4S_NAME                          "smoIp4s"
#define  SYS_SMO_TCPS_NAME                          "smoTcps"
#define  SYS_SMO_UDPS_NAME                          "smoUdps"
#define  SYS_SMO_GTPS_NAME                          "smoGtps"
#define  SYS_SMO_PNS_NAME                           "smoPns"
#define  SYS_SMO_PAC_NAME                           "smoPac"
#define  SYS_SMO_LNS_NAME                           "smoLns"
#define  SYS_SMO_LAC_NAME                           "smoLac"
#define  SYS_SMO_RIP2_NAME                          "smoRip2"
#define  SYS_SMO_OSPF_NAME                          "smoOspf"
#define  SYS_SMO_DHCPS_NAME                         "smoDhcps"
#define  SYS_SMO_DHCPS2_NAME                        "smoDhcps2"
#define  SYS_SMO_DNSR_NAME                          "smoDnsr"
#define  SYS_SMO_WEBP_NAME                          "smoWebp"
#define  SYS_SMO_WEBS_NAME                          "smoWebs"
#define  SYS_SMO_BMW_NAME                           "smoBmw"
#define  SYS_SMO_WCP_NAME                           "smoWcp"
#define  SYS_SMO_RGWM_NAME                          "smoRgwm"
#define  SYS_SMO_RAM_NAME                           "smoRam"
#define  SYS_SMO_CYFW_NAME                          "smoCyfw"
#define  SYS_SMO_NAT_NAME                           "smoNat"
#define  SYS_SMO_SNMP_NAME                          "smoSnmp"
#define  SYS_SMO_MIB2_NAME                          "smoMib2"
#define  SYS_SMO_MOAM_NAME                          "smoMoam"
#define  SYS_SMO_KB5C_NAME                          "smoKb5c"
#define  SYS_SMO_KB5S_NAME                          "smoKb5s"
#define  SYS_SMO_KB5A_NAME                          "smoKb5a"
#define  SYS_SMO_KBKMC_NAME                         "smoKbkmc"
#define  SYS_SMO_KBKMS_NAME                         "smoKbkms"
#define  SYS_SMO_CBHM10_NAME                        "smoCbhm10"
#define  SYS_SMO_GFWM_NAME                          "smoGfwm"
#define  SYS_SMO_GNAT_NAME                          "smoGnat"
#define  SYS_SMO_CTP_NAME                           "smoCtp"
#define  SYS_SMO_IPSEC_NAME                         "smoIpsec"
#define  SYS_SMO_CCGM_NAME                          "smoCcgm"
#define  SYS_SMO_BEEP_NAME                          "smoBeep"
#define  SYS_SMO_BREE_NAME                          "smoBree"
#define  SYS_SMO_BWRM_NAME                          "smoBwrm"
#define  SYS_SMO_BWSP_NAME                          "smoBwsp"
#define  SYS_SMO_SLAP_NAME                          "smoSlap"
#define  SYS_SMO_TLS_NAME                           "smoTls"

#define  SYS_PMO_NDL_NAME                           "pmoNdl"
#define  SYS_PMO_SLIB_NAME                          "pmoSlib"
#define  SYS_PMO_BSS_NAME                           "pmoBss"
#define  SYS_PMO_GNPS_NAME                          "pmoGnps"
#define  SYS_PMO_IP4S_NAME                          "pmoIp4s"
#define  SYS_PMO_TCPS_NAME                          "pmoTcps"
#define  SYS_PMO_UDPS_NAME                          "pmoUdps"
#define  SYS_PMO_GTPS_NAME                          "pmoGtps"
#define  SYS_PMO_PNS_NAME                           "pmoPns"
#define  SYS_PMO_PAC_NAME                           "pmoPac"
#define  SYS_PMO_LNS_NAME                           "pmoLns"
#define  SYS_PMO_LAC_NAME                           "pmoLac"
#define  SYS_PMO_RIP2_NAME                          "pmoRip2"
#define  SYS_PMO_OSPF_NAME                          "pmoOspf"
#define  SYS_PMO_DHCPS_NAME                         "pmoDhcps"
#define  SYS_PMO_DHCPS2_NAME                        "pmoDhcps2"
#define  SYS_PMO_DNSR_NAME                          "pmoDnsr"
#define  SYS_PMO_WEBP_NAME                          "pmoWebp"
#define  SYS_PMO_WEBS_NAME                          "pmoWebs"
#define  SYS_PMO_BMW_NAME                           "pmoBmw"
#define  SYS_PMO_WCP_NAME                           "pmoWcp"
#define  SYS_PMO_RGWM_NAME                          "pmoRgwm"
#define  SYS_PMO_RAM_NAME                           "pmoRam"
#define  SYS_PMO_CYFW_NAME                          "pmoCyfw"
#define  SYS_PMO_NAT_NAME                           "pmoNat"
#define  SYS_PMO_SNMP_NAME                          "pmoSnmp"
#define  SYS_PMO_MIB2_NAME                          "pmoMib2"
#define  SYS_PMO_MOAM_NAME                          "pmoMoam"
#define  SYS_PMO_KB5C_NAME                          "pmoKb5c"
#define  SYS_PMO_KB5S_NAME                          "pmoKb5s"
#define  SYS_PMO_KB5A_NAME                          "pmoKb5a"
#define  SYS_PMO_KBKMC_NAME                         "pmoKbkmc"
#define  SYS_PMO_KBKMS_NAME                         "pmoKbkms"
#define  SYS_PMO_CBHM10_NAME                        "pmoCbhm10"
#define  SYS_PMO_GFWM_NAME                          "pmoGfwm"
#define  SYS_PMO_GNAT_NAME                          "pmoGnat"
#define  SYS_PMO_CTP_NAME                           "pmoCtp"
#define  SYS_PMO_IPSEC_NAME                         "pmoIpsec"
#define  SYS_PMO_CCGM_NAME                          "pmoCcgm"
#define  SYS_PMO_BEEP_NAME                          "pmoBeep"
#define  SYS_PMO_BREE_NAME                          "pmoBree"
#define  SYS_PMO_BWRM_NAME                          "pmoBwrm"
#define  SYS_PMO_BWSP_NAME                          "pmoBwsp"
#define  SYS_PMO_SLAP_NAME                          "pmoSlap"
#define  SYS_PMO_TLS_NAME                           "pmoTls"

#define  SYS_RMO_NDL_NAME                           "rmoNdl"
#define  SYS_RMO_SLIB_NAME                          "rmoSlib"
#define  SYS_RMO_BSS_NAME                           "rmoBss"
#define  SYS_RMO_GNPS_NAME                          "rmoGnps"
#define  SYS_RMO_IP4S_NAME                          "rmoIp4s"
#define  SYS_RMO_TCPS_NAME                          "rmoTcps"
#define  SYS_RMO_UDPS_NAME                          "rmoUdps"
#define  SYS_RMO_GTPS_NAME                          "rmoGtps"
#define  SYS_RMO_PNS_NAME                           "rmoPns"
#define  SYS_RMO_PAC_NAME                           "rmoPac"
#define  SYS_RMO_LNS_NAME                           "rmoLns"
#define  SYS_RMO_LAC_NAME                           "rmoLac"
#define  SYS_RMO_RIP2_NAME                          "rmoRip2"
#define  SYS_RMO_OSPF_NAME                          "rmoOspf"
#define  SYS_RMO_DHCPS_NAME                         "rmoDhcps"
#define  SYS_RMO_DHCPS2_NAME                        "rmoDhcps2"
#define  SYS_RMO_DNSR_NAME                          "rmoDnsr"
#define  SYS_RMO_WEBP_NAME                          "rmoWebp"
#define  SYS_RMO_WEBS_NAME                          "rmoWebs"
#define  SYS_RMO_BMW_NAME                           "rmoBmw"
#define  SYS_RMO_WCP_NAME                           "rmoWcp"
#define  SYS_RMO_RGWM_NAME                          "rmoRgwm"
#define  SYS_RMO_RAM_NAME                           "rmoRam"
#define  SYS_RMO_CYFW_NAME                          "rmoCyfw"
#define  SYS_RMO_NAT_NAME                           "rmoNat"
#define  SYS_RMO_SNMP_NAME                          "rmoSnmp"
#define  SYS_RMO_MIB2_NAME                          "rmoMib2"
#define  SYS_RMO_MOAM_NAME                          "rmoMoam"
#define  SYS_RMO_KB5C_NAME                          "rmoKb5c"
#define  SYS_RMO_KB5S_NAME                          "rmoKb5s"
#define  SYS_RMO_KB5A_NAME                          "rmoKb5a"
#define  SYS_RMO_KBKMC_NAME                         "rmoKbkmc"
#define  SYS_RMO_KBKMS_NAME                         "rmoKbkms"
#define  SYS_RMO_CBHM10_NAME                        "rmoCbhm10"
#define  SYS_RMO_GFWM_NAME                          "rmoGfwm"
#define  SYS_RMO_GNAT_NAME                          "rmoGnat"
#define  SYS_RMO_CTP_NAME                           "rmoCtp"
#define  SYS_RMO_IPSEC_NAME                         "rmoIpsec"
#define  SYS_RMO_CCGM_NAME                          "rmoCcgm"
#define  SYS_RMO_BEEP_NAME                          "rmoBeep"
#define  SYS_RMO_BREE_NAME                          "rmoBree"
#define  SYS_RMO_BWRM_NAME                          "rmoBwrm"
#define  SYS_RMO_BWSP_NAME                          "rmoBwsp"
#define  SYS_RMO_SLAP_NAME                          "rmoSlap"
#define  SYS_RMO_TLS_NAME                           "rmoTls"

#define  SYS_NMO_APP_NAME                           "nmoApp"
#define  SYS_NMO_PROTO_NAME                         "nmoProto"
#define  SYS_NMO_LINK_NAME                          "nmoLink"
#define  SYS_NMO_SRVS_NAME                          "nmoSrvs"
#define  SYS_NMO_VPNP_NAME                          "nmoVpnp"
#define  SYS_NMO_RIP2_NAME                          "nmoRip2"
#define  SYS_NMO_KB5C_NAME                          "nmoKb5c"
#define  SYS_NMO_KB5S_NAME                          "nmoKb5s"
#define  SYS_NMO_KB5A_NAME                          "nmoKb5a"
#define  SYS_NMO_KBKMC_NAME                         "nmoKbkmc"
#define  SYS_NMO_KBKMS_NAME                         "nmoKbkms"
#define  SYS_NMO_IPSEC_NAME                         "nmoIpsec"
#define  SYS_NMO_CYFW_NAME                          "nmoCyfw"

#define  SYS_SBO_IP4S_NAME                          "sboIp4s"
#define  SYS_SBO_WEBS_NAME                          "sboWebs"


#endif
