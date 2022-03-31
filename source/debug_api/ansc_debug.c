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

    module:	ansc_debug.c

        For CCSP trace related APIs

    ---------------------------------------------------------------

    description:

        This API header file defines all the CCSP trace related
        functions.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Hui Ma

    ---------------------------------------------------------------

    revision:

        04/25/11    initial revision.

**********************************************************************/

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/
#include "ccsp_trace.h"
#include "user_time.h"
#include "ansc_time.h"
#include "ansc_debug.h"

/*Structure defined to get the log level type from the given Log Names */
#define NUM_LOGLEVEL_TYPES (sizeof(loglevel_type_table)/sizeof(loglevel_type_table[0]))

typedef struct loglevel_pair {
  char     *name;
  int      level;
} LOGLEVEL_PAIR;

LOGLEVEL_PAIR loglevel_type_table[] = {
  { "RDK_LOG_ERROR",  RDK_LOG_ERROR },
  { "RDK_LOG_WARN",   RDK_LOG_WARN   },
  { "RDK_LOG_NOTICE", RDK_LOG_NOTICE },
  { "RDK_LOG_INFO",   RDK_LOG_INFO },
  { "RDK_LOG_DEBUG",  RDK_LOG_DEBUG },
  { "RDK_LOG_FATAL",  RDK_LOG_FATAL }
};

int loglevel_type_from_name(char *name, int *type_ptr)
{
  int rc = -1;
  unsigned int i = 0;
  if((name == NULL) || (type_ptr == NULL))
     return 0;

  for (i = 0 ; i < NUM_LOGLEVEL_TYPES ; ++i)
  {
      rc = strncmp(name, loglevel_type_table[i].name, strlen(name));
      if(rc == 0)
      {
        *type_ptr = loglevel_type_table[i].level;
        break;
      }
  }
  return rc;
}

/**********************************************************************
                    VARIABLES FOR TRACE LEVEL
**********************************************************************/
INT  g_iTraceLevel = CCSP_TRACE_LEVEL_WARNING;
char *g_TraceLevelStr[] = {"EMERGENCY", "ALERT", "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO", "DEBUG"};
static void
AnscSetTraceLevel_ccsp
    (
        INT  traceLevel
    )
{
    g_iTraceLevel = traceLevel;
    if(g_iTraceLevel > CCSP_TRACE_LEVEL_DEBUG) g_iTraceLevel = CCSP_TRACE_LEVEL_DEBUG;
    else if(g_iTraceLevel < CCSP_TRACE_LEVEL_ALERT) g_iTraceLevel = CCSP_TRACE_LEVEL_ALERT;
}

static void
AnscSetTraceLevel_ansc
    (
        INT  traceLevel
    )
{
    int ansc_level = 0;

    if(traceLevel > CCSP_TRACE_LEVEL_DEBUG) traceLevel = CCSP_TRACE_LEVEL_DEBUG;
    else if(traceLevel < CCSP_TRACE_LEVEL_ALERT) traceLevel = CCSP_TRACE_LEVEL_ALERT;
    
    switch(traceLevel)
    {
    case CCSP_TRACE_LEVEL_EMERGENCY:
#ifndef FEATURE_SUPPORT_RDKLOG
        ansc_level = ANSC_TRACE_LEVEL_DEATH;
        break;
    case CCSP_TRACE_LEVEL_ALERT:
    case CCSP_TRACE_LEVEL_CRITICAL:
#endif
        ansc_level = ANSC_TRACE_LEVEL_CRITICAL;
        break;
    case CCSP_TRACE_LEVEL_ERROR:
        ansc_level = ANSC_TRACE_LEVEL_ERROR;
        break;
    case CCSP_TRACE_LEVEL_WARNING:
        ansc_level = ANSC_TRACE_LEVEL_WARNING;
        break;
    case CCSP_TRACE_LEVEL_NOTICE:
        ansc_level = ANSC_TRACE_LEVEL_TEST;
        break;
    case CCSP_TRACE_LEVEL_INFO:
        ansc_level = ANSC_TRACE_LEVEL_FLOW; /*for AnscTrace(), this macro compares with ANSC_TRACE_LEVEL_FLOW*/
        break;
    case CCSP_TRACE_LEVEL_DEBUG:
        ansc_level = ANSC_TRACE_LEVEL_VERBOSE;
        break;
    default:
        ansc_level = ANSC_TRACE_INVALID_LEVEL;
    }

    AnscTraceSetAllIdcfgLevels(ansc_level);
}

void
AnscSetTraceLevel
    (
        INT  traceLevel
    )
{
    AnscSetTraceLevel_ccsp(traceLevel);    
    AnscSetTraceLevel_ansc(traceLevel);    
}

void Ccsplog3(char *pComponentName, char* LogMsg)
{
    char* LogLevel = NULL;
    int level = 0;

    LogLevel = strtok_r(LogMsg, ",", &LogMsg);
    if (!loglevel_type_from_name(LogLevel, &level))
    {
        printf("\ntype name found - %d\n",level);
    }
    else
    {
        printf("unrecognized type name");
        level = RDK_LOG_INFO;
    }
    CcspTraceExec(pComponentName, level, (LogMsg));
}

const char* CcspTraceGetRdkLogModule(const char* pComponentName)
{
    if(!pComponentName)
        return  "LOG.RDK.Misc";
    if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.pam"))
        return "LOG.RDK.PAM";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.tr069pa"))
        return  "LOG.RDK.TR69";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.psm"))
        return  "LOG.RDK.PSM";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.moca"))
        return  "LOG.RDK.MOCA";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.CR"))
        return "LOG.RDK.CR";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.mta"))
        return "LOG.RDK.MTA";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.cm"))
        return "LOG.RDK.CM";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.tdm"))
        return "LOG.RDK.TDM";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.ssd"))
        return "LOG.RDK.SSD";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.fu"))
        return "LOG.RDK.FU";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.wifi"))
        return "LOG.RDK.WIFI";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.harvester"))
        return "LOG.RDK.Harvester";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.notifycomponent"))
        return  "LOG.RDK.NOTIFY";
    else if(!strcmp(pComponentName,"LOG.RDK.LM"))
        return "LOG.RDK.LM";
    else if(!strcmp(pComponentName,"CCSP_SNMNP_Plugin"))
        return "LOG.RDK.SNMP";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.hotspot"))
        return "LOG.RDK.HOTSPOT";
    else if(!strcmp(pComponentName,"dhcp_snooperd"))
        return "LOG.RDK.DHCPSNOOP";
    else if(!strcmp(pComponentName,"LOG.RDK.PWRMGR"))
        return  "LOG.RDK.PWRMGR";
    else if(!strcmp(pComponentName,"LOG.RDK.FSC"))
        return  "LOG.RDK.FSC";
    else if(!strcmp(pComponentName,"LOG.RDK.MESH"))
        return  "LOG.RDK.MESH";
    else if(!strcmp(pComponentName,"LOG.RDK.BLE"))
        return  "LOG.RDK.BLE";
    else if(!strcmp(pComponentName,"LOG.RDK.MeshService"))
        return  "LOG.RDK.MeshService";
    else if(!strcmp(pComponentName,"LOG.RDK.CPUPROCANALYZER"))
        return  "LOG.RDK.CPUPROCANALYZER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.ethagent"))
        return  "LOG.RDK.ETHAGENT";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.wanagent"))
        return  "LOG.RDK.WANAGENT";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.telcovoipagent"))
        return  "LOG.RDK.TELCOVOIPAGENT";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.advsec"))
        return "LOG.RDK.ADVSEC";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.telemetry"))
        return "LOG.RDK.T2";
#ifdef FEATURE_RDKB_DHCP_MANAGER
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.dhcpmgr"))
        return "LOG.RDK.DHCPMGR";
#endif
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xdns"))
        return  "LOG.RDK.XDNS";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.dslagent"))
        return  "LOG.RDK.DSLAGENT";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.vlanagent"))
        return  "LOG.RDK.VLANAGENT";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xtmagent"))
        return  "LOG.RDK.XTMAGENT";
#if defined (FEATURE_RDKB_WAN_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.wanmanager"))
        return  "LOG.RDK.WANMANAGER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xdslmanager"))
        return  "LOG.RDK.XDSLMANAGER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.vlanmanager"))
        return  "LOG.RDK.VLANMANAGER";
#ifdef FEATURE_RDKB_GPON_MANAGER
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.gponmanager"))
        return  "LOG.RDK.GPONMANAGER";
#endif
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.platformmanager"))
        return  "LOG.RDK.PLATFORMMANAGER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xtmmanager"))
        return  "LOG.RDK.XTMMANAGER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.pppmanager"))
        return  "LOG.RDK.PPPMANAGER";
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.ledmanager"))
        return  "LOG.RDK.RDKLEDMANAGER";
#endif // FEATURE_RDKB_WAN_MANAGER
#if defined (FEATURE_RDKB_WAN_MANAGER) || defined (FEATURE_FWUPGRADE_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.fwupgrademanager"))
        return  "LOG.RDK.FWUPGRADEMANAGER";
#endif
#if defined (FEATURE_RDKB_WAN_MANAGER) || defined (FEATURE_RDKB_TELCOVOICE_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.telcovoicemanager"))
        return  "LOG.RDK.TELCOVOICEMANAGER";
#endif
#if defined(FEATURE_RDKB_NFC_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.nfcmanager"))
        return "LOG.RDK.RDKNFCMANAGER";
#endif //FEATURE_RDKB_NFC_MANAGER
#if defined(FEATURE_RDKB_CELLULAR_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.cellularmanager"))
        return "LOG.RDK.CELLULARMANAGER";
#endif //FEATURE_RDKB_CELLULAR_MANAGER
#if defined (FEATURE_RDKB_INTER_DEVICE_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.interdevicemanager"))
	return "LOG.RDK.INTERDEVICEMANAGER";
#endif //FEATURE_RDKB_INTER_DEVICE_MANAGER
#if defined (FEATURE_RDKB_THERMAL_MANAGER)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.thermalmanager"))
        return "LOG.RDK.RDKTHERMALMANAGER";
#endif //FEATURE_RDKB_THERMAL_MANAGER
#if defined(GATEWAY_FAILOVER_SUPPORTED)
    else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.gatewaymanager"))
        return "LOG.RDK.GATEWAYMANAGER";
#endif
    else
        return  "LOG.RDK.Misc";
}

