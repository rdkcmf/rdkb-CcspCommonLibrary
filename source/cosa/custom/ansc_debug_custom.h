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

    module:	ansc_debug.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This custom header file defines:

        *   platform-specific trace IDs
        *   default Custom Trace ID configuration: level/mask 
            configured for each trace ID
        *   custom ID name table

    ---------------------------------------------------------------

    environment:

        platform/model dependent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        03/22/06    initial revision.

**********************************************************************/


#ifndef  _ANSC_DEBUG_CUSTOM_
#define  _ANSC_DEBUG_CUSTOM_


/***********************************************************
                DEFAULT DEBUG LEVEL/MASK
***********************************************************/

/* _ANSC_TRACE_LITE_ is defined in ansc_debug_custom_build.h, which is generated by build script */
#include "ansc_debug_custom_build.h"


#ifdef   _ANSC_TRACE_LITE_
    #define  _ANSC_TRACE_DFT_LEVEL_                 ANSC_TRACE_LEVEL_TEST
    #define  _ANSC_TRACE_DFT_MASK_                  ANSC_TRACE_MASK_ALL
#else
    #define  _ANSC_TRACE_DFT_LEVEL_                 ANSC_TRACE_INVALID_LEVEL
    #define  _ANSC_TRACE_DFT_MASK_                  ANSC_TRACE_MASK_ALL
#endif

/***********************************************************
                PLATFORM CUSTOM TRACE IDS
***********************************************************/

#define  ANSC_TRACE_ID_PLATFORM_START               ANSC_TRACE_ID_CUSTOM_START

#define  ANSC_TRACE_ID_LINUX_L2Hook                 ANSC_TRACE_ID_PLATFORM_START + 0
#define  ANSC_TRACE_ID_LINUX_NetFilter              ANSC_TRACE_ID_PLATFORM_START + 1
#define  ANSC_TRACE_ID_LINUX_Ip4sTlr                ANSC_TRACE_ID_PLATFORM_START + 2

#define  ANSC_TRACE_ID_PLATFORM_END                 ANSC_TRACE_ID_LINUX_Ip4sTlr

/***********************************************************
                   PLATFORM TRACE ID CONFIGURATION
***********************************************************/

#define  ANSC_TRACE_PLATFORM_CUSTOM_ID_CFG                                                  \
            {ANSC_TRACE_ID_LINUX_L2Hook,    ANSC_TRACE_LEVEL_FLOW,  ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_LINUX_NetFilter, ANSC_TRACE_LEVEL_FLOW,  ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_LINUX_Ip4sTlr,   ANSC_TRACE_LEVEL_FLOW,  ANSC_TRACE_MASK_ALL}

/***********************************************************
                   PLATFORM TRACE ID NAME TABLE
***********************************************************/

#define  ANSC_TRACE_PLATFORM_CUSTOM_ID_NAMES                    \
            {{ANSC_TRACE_ID_LINUX_L2Hook},    "LinuxL2Hook"   },  \
            {{ANSC_TRACE_ID_LINUX_NetFilter}, "LinuxNetFilter"},  \
            {{ANSC_TRACE_ID_LINUX_Ip4sTlr},   "LinuxIp4sTlr"  }



/***********************************************************
                   MODEL CUSTOM TRACE IDS
***********************************************************/

#define  ANSC_TRACE_ID_MODEL_START                  ANSC_TRACE_ID_PLATFORM_END + 1

#define  ANSC_TRACE_ID_DSLG                         ANSC_TRACE_ID_MODEL_START + 0
#define  ANSC_TRACE_ID_DSLH                         ANSC_TRACE_ID_MODEL_START + 1
#define  ANSC_TRACE_ID_DSLH_ObjCtrl                 ANSC_TRACE_ID_MODEL_START + 2
#define  ANSC_TRACE_ID_DSLM                         ANSC_TRACE_ID_MODEL_START + 3

#define  ANSC_TRACE_ID_BOND                         ANSC_TRACE_ID_MODEL_START + 4

#define  ANSC_TRACE_ID_CBHM                         ANSC_TRACE_ID_MODEL_START + 5

/***********************************************************
                   MODEL TRACE ID CONFIGURATION
***********************************************************/

#define  ANSC_TRACE_MODEL_CUSTOM_ID_CFG                                                         \
            {ANSC_TRACE_ID_DSLG,            ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_DSLH,            ANSC_TRACE_LEVEL_WARNING,   ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_DSLH_ObjCtrl,    ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_DSLM,            ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_BOND,            ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL},   \
            {ANSC_TRACE_ID_CBHM,            ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL}

/***********************************************************
                   MODEL TRACE ID NAME TABLE
***********************************************************/

#define  ANSC_TRACE_MODEL_CUSTOM_ID_NAMES                       \
            {{ANSC_TRACE_ID_DSLG},         "Dslg"          },     \
            {{ANSC_TRACE_ID_DSLH},         "Dslh"          },     \
            {{ANSC_TRACE_ID_DSLH_ObjCtrl}, "Dslh Obj Ctrl" },     \
            {{ANSC_TRACE_ID_DSLM},         "Dslm"          },     \
            {{ANSC_TRACE_ID_BOND},         "Bond"          },     \
            {{ANSC_TRACE_ID_CBHM},         "CableHome"     }


/***********************************************************
                   COMPLETE TRACE ID CONFIGURATION
***********************************************************/
#define  ANSC_TRACE_CUSTOM_ID_CFG                   \
            ANSC_TRACE_PLATFORM_CUSTOM_ID_CFG,      \
            ANSC_TRACE_MODEL_CUSTOM_ID_CFG

/***********************************************************
                   COMPLETE TRACE ID NAME TABLE
***********************************************************/
#define  ANSC_TRACE_CUSTOM_ID_NAMES                 \
            ANSC_TRACE_PLATFORM_CUSTOM_ID_NAMES,    \
            ANSC_TRACE_MODEL_CUSTOM_ID_NAMES


#endif

