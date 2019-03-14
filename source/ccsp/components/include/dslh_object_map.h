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

    module:	dslh_object_map.h

        For Management Information Base v2 Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file encapsulates all the Cwmp Configuration Objects
        that have been packaged and need to be instantiated in
        runtime.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_OBJECT_MAP_
#define  _DSLH_OBJECT_MAP_


#include "dslh_definitions.h"

#include "dslh_parammap_internetgatewaydevice.h"
#include "dslh_parammap_deviceinfo.h"
#include "dslh_parammap_vendorconfigfile.h"
#include "dslh_parammap_vendorconfigfile_i.h"
#include "dslh_parammap_deviceconfig.h"
#include "dslh_parammap_managementserver.h"
#include "dslh_parammap_time.h"
#include "dslh_parammap_userinterface.h"
#include "dslh_parammap_layer3forwarding.h"
#include "dslh_parammap_forwarding.h"
#include "dslh_parammap_forwarding_i.h"
#include "dslh_parammap_lanconfigsecurity.h"
#include "dslh_parammap_ippingdiagnostics.h"
#include "dslh_parammap_landevice.h"
#include "dslh_parammap_landevice_i.h"
#include "dslh_parammap_lanhostconfigmanagement.h"
#include "dslh_parammap_ipinterface.h"
#include "dslh_parammap_ipinterface_i.h"
#include "dslh_parammap_lanethernetinterfaceconfig.h"
#include "dslh_parammap_lanethernetinterfaceconfig_i.h"
#include "dslh_parammap_lanethernetinterfaceconfig_i_stats.h"
#include "dslh_parammap_lanusbinterfaceconfig.h"
#include "dslh_parammap_lanusbinterfaceconfig_i.h"
#include "dslh_parammap_lanusbinterfaceconfig_i_stats.h"
#include "dslh_parammap_wlanconfiguration.h"
#include "dslh_parammap_wlanconfiguration_i.h"
#include "dslh_parammap_wlanconfiguration_i_associateddevice.h"
#include "dslh_parammap_wlanconfiguration_i_associateddevice_i.h"
#include "dslh_parammap_wlanconfiguration_i_wepkey.h"
#include "dslh_parammap_wlanconfiguration_i_wepkey_i.h"
#include "dslh_parammap_wlanconfiguration_i_presharedkey.h"
#include "dslh_parammap_wlanconfiguration_i_presharedkey_i.h"
#include "dslh_parammap_hosts.h"
#include "dslh_parammap_host.h"
#include "dslh_parammap_host_i.h"
#include "dslh_parammap_wandevice.h"
#include "dslh_parammap_wandevice_i.h"
#include "dslh_parammap_wancommoninterfaceconfig.h"
#include "dslh_parammap_connection.h"
#include "dslh_parammap_connection_i.h"
#include "dslh_parammap_wandslinterfaceconfig.h"
#include "dslh_parammap_wandslinterfaceconfig_stats.h"
#include "dslh_parammap_wandslinterfaceconfig_stats_total.h"
#include "dslh_parammap_wandslinterfaceconfig_stats_showtime.h"
#include "dslh_parammap_wandslinterfaceconfig_stats_lastshowtime.h"
#include "dslh_parammap_wandslinterfaceconfig_stats_currentday.h"
#include "dslh_parammap_wandslinterfaceconfig_stats_quarterhour.h"
#include "dslh_parammap_wanethernetinterfaceconfig.h"
#include "dslh_parammap_wanethernetinterfaceconfig_stats.h"
#include "dslh_parammap_wandslconnectionmanagement.h"
#include "dslh_parammap_connectionservice.h"
#include "dslh_parammap_connectionservice_i.h"
#include "dslh_parammap_wandsldiagnostics.h"
#include "dslh_parammap_wanconnectiondevice.h"
#include "dslh_parammap_wanconnectiondevice_i.h"
#include "dslh_parammap_wandsllinkconfig.h"
#include "dslh_parammap_wanatmf5loopbackdiagnostics.h"
#include "dslh_parammap_wanethernetlinkconfig.h"
#include "dslh_parammap_wanpotslinkconfig.h"
#include "dslh_parammap_wanipconnection.h"
#include "dslh_parammap_wanipconnection_i.h"
#include "dslh_parammap_wanipconnection_i_portmapping.h"
#include "dslh_parammap_wanipconnection_i_portmapping_i.h"
#include "dslh_parammap_wanipconnection_i_stats.h"
#include "dslh_parammap_wanpppconnection.h"
#include "dslh_parammap_wanpppconnection_i.h"
#include "dslh_parammap_wanpppconnection_i_portmapping.h"
#include "dslh_parammap_wanpppconnection_i_portmapping_i.h"
#include "dslh_parammap_wanpppconnection_i_stats.h"


/***********************************************************
       DSLH CWMP CONFIGURATION OBJECT LIST DEFINITION
***********************************************************/

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
DSLH_CWMP_OBJECT_DESCR  g_dslhCwmpObjectMap[] =
{
    #ifdef  1
    {
        "InternetGatewayDevice",                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_InternetGatewayDevice                           /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.DeviceInfo",                             /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_DeviceInfo                                      /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.DeviceInfo.VendorConfigFile",            /* object name          */
        DSLH_CWMP_OBJECT_TYPE_table,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_VendorConfigFile_Table                          /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.DeviceInfo.VendorConfigFile.{i}",        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_entry,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_VendorConfigFile_Entry                          /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.DeviceConfig",                           /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_DeviceConfig                                    /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.ManagementServer",                       /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_ManagementServer                                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.Time",                                   /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_Time                                            /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.UserInterface",                          /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_UserInterface                                   /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.Layer3Forwarding",                       /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_Layer3Forwarding                                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.Layer3Forwarding.Forwarding",            /* object name          */
        DSLH_CWMP_OBJECT_TYPE_table,                                    /* object type          */
        TRUE,                                                           /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_Forwarding_Table                                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.Layer3Forwarding.Forwarding.{i}",        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_entry,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_Forwarding_Entry                                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANConfigSecurity",                      /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANConfigSecurity                               /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.IPPingDiagnostics",                      /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_IPPingDiagnostics                               /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice",                              /* object name          */
        DSLH_CWMP_OBJECT_TYPE_table,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANDevice_Table                                 /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}",                          /* object name          */
        DSLH_CWMP_OBJECT_TYPE_entry,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANDevice_Entry                                 /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement",  /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANHostConfigManagement                         /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPInterface",
                                                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_table,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_IPInterface_Table                               /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPInterface.{i}",
                                                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_entry,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_IPInterface_Entry                               /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig",
                                                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_table,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANEthernetInterfaceConfig_Table                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}",
                                                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_entry,                                    /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANEthernetInterfaceConfig_Entry                /* object constructor   */
    },
    #endif

    #ifdef  1
    {
        "InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}.Stats",
                                                                        /* object name          */
        DSLH_CWMP_OBJECT_TYPE_regular,                                  /* object type          */
        FALSE,                                                          /* object writability   */
        TRUE,                                                           /* object accessibility */
        DslhCreateCfgoo_LANEthernetInterfaceConfig_Entry_Stats          /* object constructor   */
    },
    #endif

    #if  1      /* mark the end of the object map */
    {
        NULL,                                                           /* object name          */
        0,                                                              /* object type          */
        FALSE,                                                          /* object writability   */
        FALSE,                                                          /* object accessibility */
        NULL                                                            /* object constructor   */
    },
    #endif
};


#endif
