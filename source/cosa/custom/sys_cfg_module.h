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

    module:	sys_cfg_module.h

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

        05/04/02    initial revision.

**********************************************************************/


#ifndef  _SYS_CFG_MODULE_
#define  _SYS_CFG_MODULE_


#include "sys_definitions.h"


/* it's snmp, only several of them are required */

/*
#define  BISGA_MODULE_NM_APP_PORT
#define  BISGA_MODULE_NM_PROTOCOL
#define  BISGA_MODULE_NM_LINK_TYPE
#define  BISGA_MODULE_NM_SRV_STATE
#define  BISGA_MODULE_NM_VPN_PROTO

#define  BISGA_MODULE_NET_DRIVER_LIB
#define  BISGA_MODULE_BSS
#define  BISGA_MODULE_GNPS
#define  BISGA_MODULE_IP4_STACK
#define  BISGA_MODULE_TCP_STACK
#define  BISGA_MODULE_UDP_STACK
#define  BISGA_MODULE_GTP_STACK

#define  BISGA_MODULE_AM_IP4_STACK
#define  BISGA_MODULE_SM_IP4_STACK
#define  BISGA_MODULE_PM_IP4_STACK

#define  BISGA_MODULE_DHCP_SERVER
#define  BISGA_MODULE_DHCP_SERVER2
#define  BISGA_MODULE_AM_DHCP_SERVER
#define  BISGA_MODULE_SM_DHCP_SERVER
#define  BISGA_MODULE_PM_DHCP_SERVER
#define  BISGA_MODULE_RM_DHCP_SERVER

#define  BISGA_MODULE_DNS_REDIRECTOR
#define  BISGA_MODULE_AM_DNS_REDIRECTOR
#define  BISGA_MODULE_SM_DNS_REDIRECTOR
#define  BISGA_MODULE_PM_DNS_REDIRECTOR
#define  BISGA_MODULE_RM_DNS_REDIRECTOR

#define  BISGA_MODULE_SB_IP4_STACK
#define  BISGA_MODULE_SB_WEB_SERVER

#define  BISGA_MODULE_RGWM
#define  BISGA_MODULE_AM_RGWM
#define  BISGA_MODULE_SM_RGWM
#define  BISGA_MODULE_PM_RGWM
#define  BISGA_MODULE_RM_RGWM

#define  BISGA_MODULE_RAM
#define  BISGA_MODULE_AM_RAM
#define  BISGA_MODULE_SM_RAM
#define  BISGA_MODULE_PM_RAM
#define  BISGA_MODULE_RM_RAM

#define  BISGA_MODULE_FIREWALL
#define  BISGA_MODULE_AM_FIREWALL
#define  BISGA_MODULE_SM_FIREWALL
#define  BISGA_MODULE_PM_FIREWALL
#define  BISGA_MODULE_RM_FIREWALL
#define  BISGA_MODULE_NM_FIREWALL

#define  BISGA_MODULE_SM_NAT
#define  BISGA_MODULE_PM_NAT

#define  BISGA_MODULE_GFWM
#define  BISGA_MODULE_GNAT
#define  BISGA_MODULE_CTP

#define  BISGA_MODULE_CBHM10
*/

#define  BISGA_MODULE_SOCKET_LIB
#define  BISGA_MODULE_SNMP
#define  BISGA_MODULE_MIB2
#define  BISGA_MODULE_MOAM




/***********************************************************
             VARIOUS NAME MAPPER COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_NM_APP_PORT
#include "sys_nmoapp_interface.h"
#include "sys_nmoapp_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_PROTOCOL
#include "sys_nmoproto_interface.h"
#include "sys_nmoproto_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_LINK_TYPE
#include "sys_nmolink_interface.h"
#include "sys_nmolink_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_SRV_STATE
#include "sys_nmosrvs_interface.h"
#include "sys_nmosrvs_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_VPN_PROTO
#include "sys_nmovpnp_interface.h"
#include "sys_nmovpnp_exported_api.h"
#endif


/***********************************************************
            GNPS AND TCP/IP STACK COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_NET_DRIVER_LIB
#include "sys_mcondl_interface.h"
#include "sys_mcondl_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SOCKET_LIB
#include "sys_mcoslib_interface.h"
#include "sys_mcoslib_exported_api.h"
#endif

#ifdef   BISGA_MODULE_BSS
#include "sys_mcobss_interface.h"
#include "sys_mcobss_exported_api.h"
#endif

#ifdef   BISGA_MODULE_GNPS
#include "sys_mcognps_interface.h"
#include "sys_mcognps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_IP4_STACK
#include "sys_mcoip4s_interface.h"
#include "sys_mcoip4s_exported_api.h"
#endif

#ifdef   BISGA_MODULE_TCP_STACK
#include "sys_mcotcps_interface.h"
#include "sys_mcotcps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_UDP_STACK
#include "sys_mcoudps_interface.h"
#include "sys_mcoudps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_GTP_STACK
#include "sys_mcogtps_interface.h"
#include "sys_mcogtps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_IP4_STACK
#include "sys_amoip4s_interface.h"
#include "sys_amoip4s_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_IP4_STACK
#include "sys_smoip4s_interface.h"
#include "sys_smoip4s_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_IP4_STACK
#include "sys_pmoip4s_interface.h"
#include "sys_pmoip4s_exported_api.h"
#endif


/***********************************************************
                 DHCP SERVER COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_DHCP_SERVER
#include "sys_mcodhcps_interface.h"
#include "sys_mcodhcps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_DHCP_SERVER2
#include "sys_mcodhcps2_interface.h"
#include "sys_mcodhcps2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_DHCP_SERVER
#include "sys_amodhcps_interface.h"
#include "sys_amodhcps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_DHCP_SERVER
#include "sys_pmodhcps_interface.h"
#include "sys_pmodhcps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_DHCP_SERVER
#include "sys_smodhcps_interface.h"
#include "sys_smodhcps_exported_api.h"
#endif

#ifdef   BISGA_MODULE_RM_DHCP_SERVER
#include "sys_rmodhcps_interface.h"
#include "sys_rmodhcps_exported_api.h"
#endif


/***********************************************************
     DNS REDIRECTOR, PROXY AND SERVER COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_DNS_REDIRECTOR
#include "sys_mcodnsr_interface.h"
#include "sys_mcodnsr_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_DHCP_SERVER
#include "sys_amodnsr_interface.h"
#include "sys_amodnsr_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_DHCP_SERVER
#include "sys_pmodnsr_interface.h"
#include "sys_pmodnsr_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_DHCP_SERVER
#include "sys_smodnsr_interface.h"
#include "sys_smodnsr_exported_api.h"
#endif


/***********************************************************
             WEB SERVER AND BMW COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_WEB_SERVER
#include "sys_mcowebs_interface.h"
#include "sys_mcowebs_exported_api.h"
#endif

#ifdef   BISGA_MODULE_BMW
#include "sys_mcobmw_interface.h"
#include "sys_mcobmw_exported_api.h"
#endif


/***********************************************************
           VARIOUS SERVICE BROKER COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_SB_IP4_STACK
#include "sys_sboip4s_interface.h"
#include "sys_sboip4s_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SB_WEB_SERVER
#include "sys_sbowebs_interface.h"
#include "sys_sbowebs_exported_api.h"
#endif


/***********************************************************
    VARIOUS VPN REMOTE ACCESS PROTOCOL COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_PPTP_PNS
#include "sys_mcopns_interface.h"
#include "sys_mcopns_exported_api.h"
#endif


/***********************************************************
             RGWM MODEL PACKAGE COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_RGWM
#include "sys_mcorgwm_interface.h"
#include "sys_mcorgwm_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_RGWM
#include "sys_amorgwm_interface.h"
#include "sys_amorgwm_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_RGWM
#include "sys_pmorgwm_interface.h"
#include "sys_pmorgwm_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_RGWM
#include "sys_smorgwm_interface.h"
#include "sys_smorgwm_exported_api.h"
#endif


/***********************************************************
           REMOTE ACCESS MANAGER COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_RAM
#include "sys_mcoram_interface.h"
#include "sys_mcoram_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_RAM
#include "sys_amoram_interface.h"
#include "sys_amoram_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_RAM
#include "sys_pmoram_interface.h"
#include "sys_pmoram_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_RAM
#include "sys_smoram_interface.h"
#include "sys_smoram_exported_api.h"
#endif


/***********************************************************
       VARIOUS HTTP RELATED UTILITIY COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_WCP
#include "sys_mcobmw_wcp_interface.h"
#include "sys_mcobmw_wcp_exported_api.h"
#endif

#ifdef   BISGA_MODULE_HTTP_HFP
#include "sys_mcohfp_http_interface.h"
#include "sys_mcohfp_http_exported_api.h"
#endif


/***********************************************************
          LEGACY FIREWALL AND NAT COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_FIREWALL
#include "sys_mcocyfw_interface.h"
#include "sys_mcocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_FIREWALL
#include "sys_amocyfw_interface.h"
#include "sys_amocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_FIREWALL
#include "sys_pmocyfw_interface.h"
#include "sys_pmocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_FIREWALL
#include "sys_smocyfw_interface.h"
#include "sys_smocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_RM_FIREWALL
#include "sys_rmocyfw_interface.h"
#include "sys_rmocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_FIREWALL
#include "sys_nmocyfw_interface.h"
#include "sys_nmocyfw_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_NAT
#include "sys_pmonat_interface.h"
#include "sys_pmonat_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_NAT
#include "sys_smonat_interface.h"
#include "sys_smonat_exported_api.h"
#endif


/***********************************************************
           GENERIC IPSEC RELATED COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_IPSEC
#include "sys_mcoipsec_interface.h"
#include "sys_mcoipsec_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_IPSEC
#include "sys_smoipsec_interface.h"
#include "sys_smoipsec_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_IPSEC
#include "sys_nmoipsec_interface.h"
#include "sys_nmoipsec_exported_api.h"
#endif


/***********************************************************
          GENERIC FIREWALL AND NAT COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_GFWM
#include "sys_mcogfwm_interface.h"
#include "sys_mcogfwm_exported_api.h"
#endif

#ifdef   BISGA_MODULE_GNAT
#include "sys_mcognat_interface.h"
#include "sys_mcognat_exported_api.h"
#endif

#ifdef   BISGA_MODULE_CTP
#include "sys_mcoctp_interface.h"
#include "sys_mcoctp_exported_api.h"
#endif


/***********************************************************
                   RIPV1/V2 COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_RIP2
#include "sys_mcorip2_interface.h"
#include "sys_mcorip2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_AM_RIP2
#include "sys_amorip2_interface.h"
#include "sys_amorip2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_SM_RIP2
#include "sys_smorip2_interface.h"
#include "sys_smorip2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_PM_RIP2
#include "sys_pmorip2_interface.h"
#include "sys_pmorip2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_NM_RIP2
#include "sys_nmorip2_interface.h"
#include "sys_nmorip2_exported_api.h"
#include "sys_nmorip2_definitions.h"
#endif


/***********************************************************
                SNMPV1/V2/V3 COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_SNMP
#include "sys_mcosnmp_interface.h"
#include "sys_mcosnmp_exported_api.h"
#endif

#ifdef   BISGA_MODULE_MIB2
#include "sys_mcomib2_interface.h"
#include "sys_mcomib2_exported_api.h"
#endif

#ifdef   BISGA_MODULE_MOAM
#include "sys_mcomoam_interface.h"
#include "sys_mcomoam_exported_api.h"
#endif


/***********************************************************
             CBHM MODEL PACKAGE COMPONENT MODULES
***********************************************************/

#ifdef   BISGA_MODULE_CBHM10
#include "sys_mcocbhm10_interface.h"
#include "sys_mcocbhm10_exported_api.h"
#endif


/***********************************************************
              SYS REQUIRED COMPONENT MODULE LIST
***********************************************************/

/*
 * During runtime environment initialization, the Module Loader Object must go through a list of
 * registered modules (along with the pointer to object constructor function) to construct the
 * dependency tree, and load each module in certain order defined by the tree. To make the module
 * information known to Module Loader Object, all the required modules must be registered during
 * environment setup, we use following data structure to store such information. All the module
 * information must be provided in a separate config file in such format.
 */
#include "sys_cfg_module_cbhm.h"


#endif
