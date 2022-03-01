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

    module:	moam_obj_modules.h

        For MIB Object Access Management Implementation (MOAM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file encapsulates all the Obj Module Objects that have
        been packaged and need to be instantiated in runtime.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/30/02    initial revision.

**********************************************************************/


#ifndef  _MOAM_OBJ_MODULES_
#define  _MOAM_OBJ_MODULES_


#include "moam_definitions.h"
#include "sys_definitions.h"

    #include "moam_varmap_smiv2.h"
    #include "moam_varmap_tc.h"
    #include "moam_varmap_framework.h"
    #include "moam_varmap_mpd.h"
    #include "moam_varmap_usm.h"
    #include "moam_varmap_tm.h"
    #include "moam_varmap_target.h"
    #include "moam_varmap_notify.h"
    #include "moam_varmap_community.h"
    #include "moam_varmap_vacm.h"
    #include "moam_varmap_snmp2.h"
    #include "moam_varmap_usmdh.h"

#if 0
    #include "moam_varmap_docsif.h"
    #include "moam_varmap_docsbpi.h"
    #include "moam_varmap_docsdev.h"
    #include "moam_varmap_clabdef.h"
    #include "moam_varmap_cabhcap.h"
    #include "moam_varmap_cabhcdp.h"
    #include "moam_varmap_cabhctp.h"
    #include "moam_varmap_cabhpsd.h"
    #include "moam_varmap_cabhsec.h"
#endif

/***********************************************************
          MOAM STANDARD OBJECT MODULE LIST DEFINITION
***********************************************************/

/*
 * Every MIB object used in the system MUST be associated with a unique MIB Object Descriptor data
 * structure. This descriptor retains the type, name, oid, and syntax of the corresponding MIB
 * object. When system initializes, a MIB Object Tree will be constructed based on the information
 * provided by the Object Descriptors.
 */

    #define  MOAM_OBJ_MODULE_SMIV2
    #define  MOAM_OBJ_MODULE_TC
    #define  MOAM_OBJ_MODULE_FRAMEWORK
    #define  MOAM_OBJ_MODULE_MPD
    #define  MOAM_OBJ_MODULE_SNMP2
    #define  MOAM_OBJ_MODULE_USM
    #define  MOAM_OBJ_MODULE_TM
    #define  MOAM_OBJ_MODULE_TARGET
    #define  MOAM_OBJ_MODULE_NOTIFY
    #define  MOAM_OBJ_MODULE_COMMUNITY
    #define  MOAM_OBJ_MODULE_VACM
    #define  MOAM_OBJ_MODULE_USMDH

#if 0
    #define  MOAM_OBJ_MODULE_DOCSIF
    #define  MOAM_OBJ_MODULE_DOCSBPI
    #define  MOAM_OBJ_MODULE_DOCSDEV
    #define  MOAM_OBJ_MODULE_CLABDEF
    #define  MOAM_OBJ_MODULE_CABHCAP
    #define  MOAM_OBJ_MODULE_CABHCDP
    #define  MOAM_OBJ_MODULE_CABHCTP
    #define  MOAM_OBJ_MODULE_CABHPSD
    #define  MOAM_OBJ_MODULE_CABHSEC
#endif

MOAM_OBJ_MODULE_DESCRIPTOR  g_moamObjModuleList[] =
{
    #ifdef  MOAM_OBJ_MODULE_SMIV2
    {
        "objModuleSmiv2",                           /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapSmiv2,                          /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_TC
    {
        "objModuleTc",                              /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapTc,                             /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_FRAMEWORK
    {
        "objModuleFramework",                       /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapFramework,                      /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_MPD
    {
        "objModuleMpd",                             /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapMpd,                            /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_SNMP2
    {
        "objModuleSnmp2",                           /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapSnmp2,                          /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_USM
    {
        "objModuleUsm",                             /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfUsm",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapUsm,                            /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_USMDH
    {
        "objModuleUsmdh",                           /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfUsm",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapUsmdh,                          /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_TM
    {
        "objModuleTm",                              /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapTm,                             /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_TARGET
    {
        "objModuleTarget",                          /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapTarget,                         /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_NOTIFY
    {
        "objModuleNotify",                          /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfNfy",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapNotify,                         /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_COMMUNITY
    {
        "objModuleCommunity",                       /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfEng",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCommunity,                      /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_VACM
    {
        "objModuleVacm",                            /* object module name   */
        SYS_MODULE_NAME_SNMP,                       /* object target name   */
        "snmpManagementInfoCollectorIfAcm",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapVacm,                           /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_IANAIF
    {
        "objModuleIanaif",                          /* object module name   */
        SYS_MODULE_NAME_GNPS,                       /* object target name   */
        "gnpsManagementInfoCollectorIf",            /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapIanaif,                         /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_IF
    {
        "objModuleIf",                              /* object module name   */
        SYS_MODULE_NAME_GNPS,                       /* object target name   */
        "gnpsManagementInfoCollectorIf",            /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapIf,                             /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_IP
    {
        "objModuleIp",                              /* object module name   */
        SYS_MODULE_NAME_IP4_STACK,                  /* object target name   */
        "ip4sManagementInfoCollectorIf",            /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapIp,                             /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_UDP
    {
        "objModuleUdp",                             /* object module name   */
        SYS_MODULE_NAME_UDP_STACK,                  /* object target name   */
        "udpManagementInfoCollectorIf",             /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapUdp,                            /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_TCP
    {
        "objModuleTcp",                             /* object module name   */
        SYS_MODULE_NAME_TCP_STACK,                  /* object target name   */
        "tcpManagementInfoCollectorIf",             /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapTcp,                            /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_INETADDR
    {
        "objModuleInetaddr",                        /* object module name   */
        SYS_MODULE_NAME_IP4_STACK,                  /* object target name   */
        NULL,                                       /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapInetaddr,                       /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

	#ifdef  MOAM_OBJ_MODULE_IPFORWARD
    {
        "objModuleIpForward",                       /* object module name   */
        SYS_MODULE_NAME_IP4_STACK,                  /* object target name   */
        "ip4sManagementInfoCollectorIf",            /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapIpForward,                      /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_DOCSIF
    {
        "objModuleDocsif",                          /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        NULL,                                       /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapDocsif,                         /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_DOCSBPI
    {
        "objModuleDocsbpi",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        NULL,                                       /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapDocsbpi,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_DOCSDEV
    {
        "objModuleDocsdev",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfDec, cbhmManagementInfoCollectorIfDna",
                                                    /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapDocsdev,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CLABDEF
    {
        "objModuleClabdef",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfDef",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapClabdef,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CABHCAP
    {
        "objModuleCabhcap",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfCap",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCabhcap,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CABHCDP
    {
        "objModuleCabhcdp",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfCdp",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCabhcdp,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CABHCTP
    {
        "objModuleCabhctp",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfCtp",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCabhctp,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CABHPSD
    {
        "objModuleCabhpsd",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfPsd",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCabhpsd,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #ifdef  MOAM_OBJ_MODULE_CABHSEC
    {
        "objModuleCabhsec",                         /* object module name   */
        SYS_MODULE_NAME_CBHM10,                     /* object target name   */
        "cbhmManagementInfoCollectorIfSec",         /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        MOAM_MODULE_REG_TRIGGER_SWITCHING,          /* registration trigger */
        NULL,                                       /* object constructor   */
        g_moamVarMapCabhsec,                        /* object module map    */
        TRUE                                        /* object accessibility */
    },
    #endif

    #if  1      /* mark the end of the object module list */
    {
        NULL,                                       /* object module name   */
        NULL,                                       /* object target name   */
        NULL,                                       /* object mic_if name   */
        NULL,                                       /* object mic_if handle */
        0,                                          /* registration trigger */
        NULL,                                       /* object constructor   */
        NULL,                                       /* object module map    */
        FALSE                                       /* object accessibility */
    },
    #endif
};


#endif
