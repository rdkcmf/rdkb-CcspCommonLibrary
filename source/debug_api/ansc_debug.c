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
#include <stdarg.h>
volatile BOOL RDKLogEnable = TRUE;
volatile unsigned int RDKLogLevel = 4;
volatile BOOL LM_RDKLogEnable = TRUE;
volatile unsigned int LM_RDKLogLevel = 4;
volatile BOOL SNMP_RDKLogEnable = TRUE;
volatile unsigned int SNMP_RDKLogLevel = 4;
volatile BOOL TR69_RDKLogEnable = TRUE;
volatile unsigned int TR69_RDKLogLevel = 4;
volatile BOOL PAM_RDKLogEnable = TRUE;
volatile unsigned int PAM_RDKLogLevel = 4;
volatile BOOL PSM_RDKLogEnable = TRUE;
volatile unsigned int PSM_RDKLogLevel = 4;
volatile BOOL MOCA_RDKLogEnable = TRUE;
volatile unsigned int MOCA_RDKLogLevel = 4;
volatile BOOL MTA_RDKLogEnable = TRUE;
volatile unsigned int MTA_RDKLogLevel = 4;
volatile BOOL CM_RDKLogEnable = TRUE;
volatile unsigned int CM_RDKLogLevel = 4;
volatile BOOL WiFi_RDKLogEnable = TRUE;
volatile unsigned int WiFi_RDKLogLevel = 4;
volatile BOOL CR_RDKLogEnable = TRUE;
volatile unsigned int CR_RDKLogLevel = 4;
/*Added for RDKB-4343*/
volatile BOOL Harvester_RDKLogEnable = TRUE;
volatile unsigned int Harvester_RDKLogLevel = 4;
volatile unsigned int NOTIFY_RDKLogLevel = 4;
volatile BOOL NOTIFY_RDKLogEnable = TRUE;
volatile BOOL PWRMGR_RDKLogEnable = TRUE;
volatile unsigned int PWRMGR_RDKLogLevel = 4;
volatile BOOL FSC_RDKLogEnable = TRUE;
volatile unsigned int FSC_RDKLogLevel = 4;
volatile BOOL MESH_RDKLogEnable = TRUE;
volatile unsigned int MESH_RDKLogLevel = 4;
volatile BOOL MeshService_RDKLogEnable = TRUE;
volatile unsigned int MeshService_RDKLogLevel = 5;

volatile BOOL CPUPROCANALYZER_RDKLogEnable = TRUE;
volatile unsigned int CPUPROCANALYZER_RDKLogLevel = 4;
volatile BOOL ETHAGENT_RDKLogEnable = TRUE;
volatile unsigned int ETHAGENT_RDKLogLevel = 4;
volatile BOOL BLE_RDKLogEnable = TRUE;
volatile unsigned int BLE_RDKLogLevel = 4;
volatile BOOL WANAGENT_RDKLogEnable = TRUE;
volatile unsigned int WANAGENT_RDKLogLevel = 4;
volatile BOOL TELCOVOIPAGENT_RDKLogEnable = TRUE;
volatile unsigned int TELCOVOIPAGENT_RDKLogLevel = 4;

volatile BOOL ADVSEC_RDKLogEnable = TRUE;
volatile unsigned int ADVSEC_RDKLogLevel = 4;
volatile BOOL XDNS_RDKLogEnable = TRUE;
volatile unsigned int XDNS_RDKLogLevel = 4;
volatile BOOL DSLAGENT_RDKLogEnable = TRUE;
volatile unsigned int DSLAGENT_RDKLogLevel = 4;
volatile BOOL VLANAGENT_RDKLogEnable = TRUE;
volatile unsigned int VLANAGENT_RDKLogLevel = 4;
volatile BOOL XTMAGENT_RDKLogEnable = TRUE;
volatile unsigned int XTMAGENT_RDKLogLevel = 4;
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

void CcspTraceLogAPI(char *fileName, char *pComponentName, int level, const char *format, ...)
{
    char *ComponentName;
    volatile unsigned int LogLevel;
    volatile BOOLEAN LogEnable;

    if(RDKLogEnable == TRUE && pComponentName)
    {
        if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.pam"))
        {
            ComponentName="LOG.RDK.PAM";
            LogLevel = PAM_RDKLogLevel;
            LogEnable = PAM_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.tr069pa"))
        {
            ComponentName = "LOG.RDK.TR69";
            LogLevel = TR69_RDKLogLevel;
            LogEnable = TR69_RDKLogEnable;
            /*printf("-- LogLevel = %d\n LogEnable = %d\n",LogLevel,LogEnable);*/
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.psm"))
        {
            ComponentName= "LOG.RDK.PSM";
            LogLevel = PSM_RDKLogLevel;
            LogEnable = PSM_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.moca"))
        {
            ComponentName= "LOG.RDK.MOCA";
            LogLevel = MOCA_RDKLogLevel;
            LogEnable = MOCA_RDKLogEnable;
            /*printf("-- MOCA LogLevel = %d\n LogEnable = %d\n",LogLevel,LogEnable);*/
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.CR"))
        {
            ComponentName="LOG.RDK.CR";
            LogLevel = CR_RDKLogLevel;
            LogEnable = CR_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.mta"))
        {
            ComponentName="LOG.RDK.MTA";
            LogLevel = MTA_RDKLogLevel;
            LogEnable = MTA_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.cm"))
        {
            ComponentName="LOG.RDK.CM";
            LogLevel = CM_RDKLogLevel;
            LogEnable = CM_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.tdm"))
        {
            ComponentName="LOG.RDK.TDM";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.ssd"))
        {
            ComponentName="LOG.RDK.SSD";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.fu"))
        {
            ComponentName="LOG.RDK.FU";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.wifi"))
        {
            ComponentName="LOG.RDK.WIFI";
            LogLevel = WiFi_RDKLogLevel;
            LogEnable = WiFi_RDKLogEnable;
        }
        /*Added for rdkb-4237*/
        /* else if(!strcmp(pComponentName,"mdc"))
        {
            MDCLOG
        } */
        /*Added for RDKB-4343*/
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.harvester"))
        {
            ComponentName="LOG.RDK.Harvester";
            LogLevel = Harvester_RDKLogLevel;
            LogEnable = Harvester_RDKLogEnable;
        }
        /*Changes end here*/
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.notifycomponent"))
        {
            ComponentName= "LOG.RDK.NOTIFY";
            LogLevel = NOTIFY_RDKLogLevel;
            LogEnable = NOTIFY_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.LM"))
        {
            ComponentName="LOG.RDK.LM";
            LogLevel = LM_RDKLogLevel;
            LogEnable = LM_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"CCSP_SNMNP_Plugin"))
        {
            ComponentName="LOG.RDK.SNMP";
            LogLevel = SNMP_RDKLogLevel;
            LogEnable = SNMP_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.hotspot"))
        {
            ComponentName = "LOG.RDK.HOTSPOT";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"dhcp_snooperd"))
        {
            ComponentName = "LOG.RDK.DHCPSNOOP";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.PWRMGR"))
        {
            ComponentName= "LOG.RDK.PWRMGR";
            LogLevel = PWRMGR_RDKLogLevel;
            LogEnable = PWRMGR_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.FSC"))
        {
            ComponentName= "LOG.RDK.FSC";
            LogLevel = FSC_RDKLogLevel;
            LogEnable = FSC_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.MESH"))
        {
            ComponentName= "LOG.RDK.MESH";
            LogLevel = MESH_RDKLogLevel;
            LogEnable = MESH_RDKLogEnable;
        }
       else if(!strcmp(pComponentName,"LOG.RDK.BLE"))
        {
            ComponentName= "LOG.RDK.BLE";
            LogLevel = BLE_RDKLogLevel;
            LogEnable = BLE_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.MeshService"))
        {
            ComponentName= "LOG.RDK.MeshService";
            LogLevel = MeshService_RDKLogLevel;
            LogEnable = MeshService_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"LOG.RDK.CPUPROCANALYZER"))
        {
            ComponentName= "LOG.RDK.CPUPROCANALYZER";
            LogLevel = CPUPROCANALYZER_RDKLogLevel;
            LogEnable = CPUPROCANALYZER_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.ethagent"))
        {
            ComponentName= "LOG.RDK.ETHAGENT";
            LogLevel = ETHAGENT_RDKLogLevel;
            LogEnable = ETHAGENT_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.wanagent"))
        {
            ComponentName= "LOG.RDK.WANAGENT";
            LogLevel = WANAGENT_RDKLogLevel;
            LogEnable = WANAGENT_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.telcovoipagent"))
        {
            ComponentName= "LOG.RDK.TELCOVOIPAGENT";
            LogLevel = TELCOVOIPAGENT_RDKLogLevel;
            LogEnable = TELCOVOIPAGENT_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.advsec"))
        {
            ComponentName="LOG.RDK.ADVSEC";
            LogLevel = ADVSEC_RDKLogLevel;
            LogEnable = ADVSEC_RDKLogEnable;
        }  
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xdns"))
        {
            ComponentName= "LOG.RDK.XDNS";
            LogLevel = XDNS_RDKLogLevel;
            LogEnable = XDNS_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.dslagent"))
        {
            ComponentName= "LOG.RDK.DSLAGENT";
            LogLevel = DSLAGENT_RDKLogLevel;
            LogEnable = DSLAGENT_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.vlanagent"))
        {
            ComponentName= "LOG.RDK.VLANAGENT";
            LogLevel = VLANAGENT_RDKLogLevel;
            LogEnable = VLANAGENT_RDKLogEnable;
        }
        else if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.xtmagent"))
        {
            ComponentName= "LOG.RDK.XTMAGENT";
            LogLevel = XTMAGENT_RDKLogLevel;
            LogEnable = XTMAGENT_RDKLogEnable;
        }
        else
        {
            ComponentName = "LOG.RDK.Misc";
            LogLevel = RDKLogLevel;
            LogEnable = RDKLogEnable;
        }
        /*if( level<=RDKLogLevel)*/
        if(((unsigned int)level<=LogLevel)&&(LogEnable == TRUE))
        {
            /*  if ( level <= g_iTraceLevel)                */
            if(!strcmp(pComponentName,"com.cisco.spvtg.ccsp.tr069pa"))
            {
                /*printf("-- level = %d\n LogLevel = %d\n LogEnable = %d\n",level,LogLevel,LogEnable);*/
            }
            {
                char    sfn[32];
                va_list args;

                CcspTraceShortenFileName(sfn, 32, fileName);
                va_start(args, format);
                RDK_LOG1(level, ComponentName, format, args);
                va_end(args);
            }
        }
    }
}
