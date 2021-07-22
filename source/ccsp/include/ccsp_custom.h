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

    module: ccsp_custom.h

        For DSL Gateway Model Implementation (DSLG),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        BBHM.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Venka Gade

**********************************************************************/

#ifndef  _CCSP_CUSTOM_H_
#define  _CCSP_CUSTOM_H_

/**********************************************************************
                      GLOBAL DEVICE DEFINITIONS
**********************************************************************/
#ifdef _PSM_FILE_COMPRESSION_ENABLE

#define  PSM_DEF_XML_CONFIG_FILE_PATH          ""
#define  PSM_DEF_XML_CONFIG_FILE_NAME          "./config/bbhm_def_cfg.xml.gz"
#define  PSM_CUR_XML_CONFIG_FILE_NAME          "/tmp/bbhm_cur_cfg.xml.gz"
#define  PSM_BAK_XML_CONFIG_FILE_NAME          "/nvram/bbhm_bak_cfg.xml.gz"
#define  PSM_TMP_XML_CONFIG_FILE_NAME          "/nvram/bbhm_tmp_cfg.xml.gz"

#else

#define  PSM_DEF_XML_CONFIG_FILE_PATH          ""
#define  PSM_DEF_XML_CONFIG_FILE_NAME          "./config/bbhm_def_cfg.xml"
#define  PSM_CUR_XML_CONFIG_FILE_NAME          "/tmp/bbhm_cur_cfg.xml"
#define  PSM_BAK_XML_CONFIG_FILE_NAME          "/nvram/bbhm_bak_cfg.xml"
#define  PSM_TMP_XML_CONFIG_FILE_NAME          "/nvram/bbhm_tmp_cfg.xml"
#endif

#define  OPENSSL_CLIENT_CA_CERT_FILES          ""

#define  CCSP_MSG_BUS_CFG 					   "/tmp/ccsp_msg.cfg"
#define  CCSP_MSG_BUS_CFG_LOCAL                "/usr/ccsp/ccsp_msg.cfg"
#define  DM_ROOTNAME                           "Device."

#define  CONFIG_CCSP_WAN_MGMT_ACCESS
#ifndef CONFIG_KERNEL_NF_TRIGGER_SUPPORT
 #define CONFIG_KERNEL_NF_TRIGGER_SUPPORT
#endif
#endif
