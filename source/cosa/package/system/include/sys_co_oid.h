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

    module:	sys_co_oid.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Sys
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


#ifndef  _SYS_CO_OID_
#define  _SYS_CO_OID_


/***********************************************************
            GENERAL SYS FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SYS_FEATURE_OBJECT_OID_BASE                SYS_COMPONENT_OID_BASE      + 0x1000
#define  SYS_GENERAL_FO_OID_BASE                    SYS_FEATURE_OBJECT_OID_BASE + 0x0000

#define  SYS_PACKAGE_WRAPPER_OID                    SYS_GENERAL_FO_OID_BASE     + 0x0001
#define  SYS_ENV_CONTROLLER_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0002
#define  SYS_SECURITY_AGENT_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0003
#define  SYS_USER_PROFILE_OID                       SYS_GENERAL_FO_OID_BASE     + 0x0004
#define  SYS_INFO_REPOSITORY_OID                    SYS_GENERAL_FO_OID_BASE     + 0x0005
#define  SYS_RECYCLE_BIN_OID                        SYS_GENERAL_FO_OID_BASE     + 0x0006
#define  SYS_MODULE_TABLE_OID                       SYS_GENERAL_FO_OID_BASE     + 0x0007
#define  SYS_MODULE_ENTRY_OID                       SYS_GENERAL_FO_OID_BASE     + 0x0008
#define  SYS_MODULE_CARRIER_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0009
#define  SYS_MODULE_LOADER_OID                      SYS_GENERAL_FO_OID_BASE     + 0x000A
#define  SYS_MODULE_MONITOR_OID                     SYS_GENERAL_FO_OID_BASE     + 0x000B
#define  SYS_ACCESS_MANAGER_OID                     SYS_GENERAL_FO_OID_BASE     + 0x000C
#define  SYS_SETUP_MANAGER_OID                      SYS_GENERAL_FO_OID_BASE     + 0x000D
#define  SYS_SETUP_STORAGE_OID                      SYS_GENERAL_FO_OID_BASE     + 0x000E
#define  SYS_PROVISION_MANAGER_OID                  SYS_GENERAL_FO_OID_BASE     + 0x000F
#define  SYS_RUNTIME_MONITOR_OID                    SYS_GENERAL_FO_OID_BASE     + 0x0010
#define  SYS_NAME_MAPPER_OID                        SYS_GENERAL_FO_OID_BASE     + 0x0011
#define  SYS_NAME_MAPPER_SIMPLE_OID                 SYS_GENERAL_FO_OID_BASE     + 0x0012
#define  SYS_SERVICE_BROKER_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0013
#define  SYS_CTLV_PROCESSOR_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0014

#define  SYS_REPOSITORY_DRIVER_OID                  SYS_GENERAL_FO_OID_BASE     + 0x0021
#define  SYS_REPOSITORY_FOLDER_OID                  SYS_GENERAL_FO_OID_BASE     + 0x0022
#define  SYS_REPOSITORY_RECORD_OID                  SYS_GENERAL_FO_OID_BASE     + 0x0023

#define  SYS_RRO_SINT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0031
#define  SYS_RRO_UINT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0032
#define  SYS_RRO_BOOL_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0033
#define  SYS_RRO_ASTR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0034
#define  SYS_RRO_BSTR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0035
#define  SYS_RRO_HCXT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0036
#define  SYS_RRO_ENUM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0037

#define  SYS_MCO_NDL_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0101
#define  SYS_MCO_SLIB_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0102
#define  SYS_MCO_BSS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0103
#define  SYS_MCO_GNPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0104
#define  SYS_MCO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0105
#define  SYS_MCO_TCPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0106
#define  SYS_MCO_UDPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0107
#define  SYS_MCO_GTPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0108
#define  SYS_MCO_PNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0109
#define  SYS_MCO_PAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x010A
#define  SYS_MCO_LNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x010B
#define  SYS_MCO_LAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x010C
#define  SYS_MCO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x010D
#define  SYS_MCO_OSPF_OID                           SYS_GENERAL_FO_OID_BASE     + 0x010E
#define  SYS_MCO_DHCPS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x010F
#define  SYS_MCO_DHCPS2_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0110
#define  SYS_MCO_DNSR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0111
#define  SYS_MCO_WEBP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0112
#define  SYS_MCO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0113
#define  SYS_MCO_BMW_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0114
#define  SYS_MCO_WCP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0115
#define  SYS_MCO_RGWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0116
#define  SYS_MCO_RAM_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0117
#define  SYS_MCO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0118
#define  SYS_MCO_NAT_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0119
#define  SYS_MCO_SNMP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011A
#define  SYS_MCO_MIB2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011B
#define  SYS_MCO_MOAM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011C
#define  SYS_MCO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011D
#define  SYS_MCO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011E
#define  SYS_MCO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x011F
#define  SYS_MCO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0120
#define  SYS_MCO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0121
#define  SYS_MCO_CBHM10_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0122
#define  SYS_MCO_GFWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0123
#define  SYS_MCO_GNAT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0124
#define  SYS_MCO_CTP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0125
#define  SYS_MCO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0126
#define  SYS_MCO_CCGM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0127
#define  SYS_MCO_BEEP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0128
#define  SYS_MCO_BREE_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0129
#define  SYS_MCO_BWRM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x012A
#define  SYS_MCO_BWSP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x012B
#define  SYS_MCO_SLAP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x012C
#define  SYS_MCO_TLS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x012D

#define  SYS_AMO_NDL_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0201
#define  SYS_AMO_SLIB_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0202
#define  SYS_AMO_BSS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0203
#define  SYS_AMO_GNPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0204
#define  SYS_AMO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0205
#define  SYS_AMO_TCPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0206
#define  SYS_AMO_UDPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0207
#define  SYS_AMO_GTPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0208
#define  SYS_AMO_PNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0209
#define  SYS_AMO_PAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x020A
#define  SYS_AMO_LNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x020B
#define  SYS_AMO_LAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x020C
#define  SYS_AMO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x020D
#define  SYS_AMO_OSPF_OID                           SYS_GENERAL_FO_OID_BASE     + 0x020E
#define  SYS_AMO_DHCPS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x020F
#define  SYS_AMO_DHCPS2_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0210
#define  SYS_AMO_DNSR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0211
#define  SYS_AMO_WEBP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0212
#define  SYS_AMO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0213
#define  SYS_AMO_BMW_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0214
#define  SYS_AMO_WCP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0215
#define  SYS_AMO_RGWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0216
#define  SYS_AMO_RAM_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0217
#define  SYS_AMO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0218
#define  SYS_AMO_NAT_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0219
#define  SYS_AMO_SNMP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021A
#define  SYS_AMO_MIB2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021B
#define  SYS_AMO_MOAM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021C
#define  SYS_AMO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021D
#define  SYS_AMO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021E
#define  SYS_AMO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x021F
#define  SYS_AMO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0220
#define  SYS_AMO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0221
#define  SYS_AMO_CBHM10_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0222
#define  SYS_AMO_GFWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0223
#define  SYS_AMO_GNAT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0224
#define  SYS_AMO_CTP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0225
#define  SYS_AMO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0226
#define  SYS_AMO_CCGM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0227
#define  SYS_AMO_BEEP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0228
#define  SYS_AMO_BREE_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0229
#define  SYS_AMO_BWRM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x022A
#define  SYS_AMO_BWSP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x022B
#define  SYS_AMO_SLAP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x022C
#define  SYS_AMO_TLS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x022D

#define  SYS_SMO_NDL_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0301
#define  SYS_SMO_SLIB_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0302
#define  SYS_SMO_BSS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0303
#define  SYS_SMO_GNPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0304
#define  SYS_SMO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0305
#define  SYS_SMO_TCPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0306
#define  SYS_SMO_UDPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0307
#define  SYS_SMO_GTPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0308
#define  SYS_SMO_PNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0309
#define  SYS_SMO_PAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x030A
#define  SYS_SMO_LNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x030B
#define  SYS_SMO_LAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x030C
#define  SYS_SMO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x030D
#define  SYS_SMO_OSPF_OID                           SYS_GENERAL_FO_OID_BASE     + 0x030E
#define  SYS_SMO_DHCPS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x030F
#define  SYS_SMO_DHCPS2_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0310
#define  SYS_SMO_DNSR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0311
#define  SYS_SMO_WEBP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0312
#define  SYS_SMO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0313
#define  SYS_SMO_BMW_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0314
#define  SYS_SMO_WCP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0315
#define  SYS_SMO_RGWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0316
#define  SYS_SMO_RAM_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0317
#define  SYS_SMO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0318
#define  SYS_SMO_NAT_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0319
#define  SYS_SMO_SNMP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031A
#define  SYS_SMO_MIB2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031B
#define  SYS_SMO_MOAM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031C
#define  SYS_SMO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031D
#define  SYS_SMO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031E
#define  SYS_SMO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x031F
#define  SYS_SMO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0320
#define  SYS_SMO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0321
#define  SYS_SMO_CBHM10_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0322
#define  SYS_SMO_GFWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0323
#define  SYS_SMO_GNAT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0324
#define  SYS_SMO_CTP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0325
#define  SYS_SMO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0326
#define  SYS_SMO_CCGM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0327
#define  SYS_SMO_BEEP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0328
#define  SYS_SMO_BREE_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0329
#define  SYS_SMO_BWRM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x032A
#define  SYS_SMO_BWSP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x032B
#define  SYS_SMO_SLAP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x032C
#define  SYS_SMO_TLS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x032D

#define  SYS_PMO_NDL_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0401
#define  SYS_PMO_SLIB_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0402
#define  SYS_PMO_BSS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0403
#define  SYS_PMO_GNPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0404
#define  SYS_PMO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0405
#define  SYS_PMO_TCPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0406
#define  SYS_PMO_UDPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0407
#define  SYS_PMO_GTPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0408
#define  SYS_PMO_PNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0409
#define  SYS_PMO_PAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x040A
#define  SYS_PMO_LNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x040B
#define  SYS_PMO_LAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x040C
#define  SYS_PMO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x040D
#define  SYS_PMO_OSPF_OID                           SYS_GENERAL_FO_OID_BASE     + 0x040E
#define  SYS_PMO_DHCPS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x040F
#define  SYS_PMO_DHCPS2_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0410
#define  SYS_PMO_DNSR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0411
#define  SYS_PMO_WEBP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0412
#define  SYS_PMO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0413
#define  SYS_PMO_BMW_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0414
#define  SYS_PMO_WCP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0415
#define  SYS_PMO_RGWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0416
#define  SYS_PMO_RAM_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0417
#define  SYS_PMO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0418
#define  SYS_PMO_NAT_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0419
#define  SYS_PMO_SNMP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041A
#define  SYS_PMO_MIB2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041B
#define  SYS_PMO_MOAM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041C
#define  SYS_PMO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041D
#define  SYS_PMO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041E
#define  SYS_PMO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x041F
#define  SYS_PMO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0420
#define  SYS_PMO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0421
#define  SYS_PMO_CBHM10_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0422
#define  SYS_PMO_GFWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0423
#define  SYS_PMO_GNAT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0424
#define  SYS_PMO_CTP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0425
#define  SYS_PMO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0426
#define  SYS_PMO_CCGM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0427
#define  SYS_PMO_BEEP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0428
#define  SYS_PMO_BREE_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0429
#define  SYS_PMO_BWRM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x042A
#define  SYS_PMO_BWSP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x042B
#define  SYS_PMO_SLAP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x042C
#define  SYS_PMO_TLS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x042D

#define  SYS_RMO_NDL_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0501
#define  SYS_RMO_SLIB_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0502
#define  SYS_RMO_BSS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0503
#define  SYS_RMO_GNPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0504
#define  SYS_RMO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0505
#define  SYS_RMO_TCPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0506
#define  SYS_RMO_UDPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0507
#define  SYS_RMO_GTPS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0508
#define  SYS_RMO_PNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0509
#define  SYS_RMO_PAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x050A
#define  SYS_RMO_LNS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x050B
#define  SYS_RMO_LAC_OID                            SYS_GENERAL_FO_OID_BASE     + 0x050C
#define  SYS_RMO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x050D
#define  SYS_RMO_OSPF_OID                           SYS_GENERAL_FO_OID_BASE     + 0x050E
#define  SYS_RMO_DHCPS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x050F
#define  SYS_RMO_DHCPS2_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0510
#define  SYS_RMO_DNSR_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0511
#define  SYS_RMO_WEBP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0512
#define  SYS_RMO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0513
#define  SYS_RMO_BMW_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0514
#define  SYS_RMO_WCP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0515
#define  SYS_RMO_RGWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0516
#define  SYS_RMO_RAM_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0517
#define  SYS_RMO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0518
#define  SYS_RMO_NAT_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0519
#define  SYS_RMO_SNMP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051A
#define  SYS_RMO_MIB2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051B
#define  SYS_RMO_MOAM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051C
#define  SYS_RMO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051D
#define  SYS_RMO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051E
#define  SYS_RMO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x051F
#define  SYS_RMO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0520
#define  SYS_RMO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0521
#define  SYS_RMO_CBHM10_OID                         SYS_GENERAL_FO_OID_BASE     + 0x0522
#define  SYS_RMO_GFWM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0523
#define  SYS_RMO_GNAT_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0524
#define  SYS_RMO_CTP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0525
#define  SYS_RMO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0526
#define  SYS_RMO_CCGM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0527
#define  SYS_RMO_BEEP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0528
#define  SYS_RMO_BREE_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0529
#define  SYS_RMO_BWRM_OID                           SYS_GENERAL_FO_OID_BASE     + 0x052A
#define  SYS_RMO_BWSP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x052B
#define  SYS_RMO_SLAP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x052C
#define  SYS_RMO_TLS_OID                            SYS_GENERAL_FO_OID_BASE     + 0x052D

#define  SYS_NMO_APP_OID                            SYS_GENERAL_FO_OID_BASE     + 0x0601
#define  SYS_NMO_PROTO_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0602
#define  SYS_NMO_LINK_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0603
#define  SYS_NMO_SRVS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0604
#define  SYS_NMO_VPNP_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0605
#define  SYS_NMO_RIP2_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0606
#define  SYS_NMO_KB5C_OID                           SYS_GENERAL_FO_OID_BASE     + 0x061C
#define  SYS_NMO_KB5S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x061D
#define  SYS_NMO_KB5A_OID                           SYS_GENERAL_FO_OID_BASE     + 0x061E
#define  SYS_NMO_KBKMC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x061F
#define  SYS_NMO_KBKMS_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0620
#define  SYS_NMO_IPSEC_OID                          SYS_GENERAL_FO_OID_BASE     + 0x0621
#define  SYS_NMO_CYFW_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0622

#define  SYS_SBO_IP4S_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0701
#define  SYS_SBO_WEBS_OID                           SYS_GENERAL_FO_OID_BASE     + 0x0702


#endif
