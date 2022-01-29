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

    module: dslgm_module_custom.h

        For DSL Gateway Model Implementation (DSLG),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        DSLG and DSLM.

        Note that some definitions for DSLM are paired up with DSLG.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Ding Hua

**********************************************************************/

#ifndef  _DSLGM_MODULE_CUSTOM_H_
#define  _DSLGM_MODULE_CUSTOM_H_

#include <ccsp_custom.h>

/**********************************************************************
                      GLOBAL DEVICE DEFINITIONS
**********************************************************************/

#define  DSLG_MANUFACTURER_NAME                     "Cisco-Linksys, LLC."
#define  DSLG_MODEL_NAME                            "WAG310G"                               /* Should be read from board NVRAM */
#define  DSLG_PRODUCT_DESCRIPTION                   "Wireless-G ADSL2+ Gateway with VoIP"   /* Should be read from board NVRAM */
#define  DSLG_PRODUCT_CLASS                         "ADSLGateway"                           /* Should be read from board NVRAM 
                                                                                               Some ACS server cannot take a space in product class */
#define  DSLG_PRODUCT_DFT_SERIALNO                  "SQL00G000000"

/*
 *  DSLG_SW_VERSION has to be synchronized with dslg_version.mk
 */
#define  DSLG_SW_VERSION                            "1.00.03(%s)-%s"
/*
 *  defined in dslg_version_custom.h
 *
#define  DSLG_SW_BUILD_NUMBER                       ""
 */

#define  DSLG_BL_VERSION                            "1.06"

/**********************************************************************
                  COMPONENT INCLUSION DEFINITIONS
**********************************************************************/

#define  _DSLG_L2BRIDGING_
/*#define  _DSLM_L2BRIDGING_*/
/*#define  _DSLH_L2BRIDGING_*/

#define  _DSLG_QOS_
#define  _DSLM_QOS_

#define  _DSLG_FIREWALL_

/*#define  _DSLG_VPN_*/
#define  _DSLG_IDS_
/*#define  _DSLG_DEV_MONITOR_*/

/*#define  _DSLM_UPNP_*/

/*#define  _DSLH_TR104_*/
/*
#define  _DSLH_TR064_
#define  _DSLH_TR098_
#define  _DSLH_UPNP_
*/
//#define  _DSLH_TR106_
#define  _CCSP_CWMP_STUN_ENABLED

#define  _DSLH_CONNREQURL_NO_PATH

#define  _DSLG_IGMP_

#if defined(BUILD_WEB)
#define  _BBHM_WEB_GUI_REQUIRED
#endif

/*#define  _DSLH_TCP_CONNREQ_HANDLER*/ /* Already defined in bbhm_$board.mk */


/*#define  _DSLH_LINKSYS_EXTENSION_*/

/*#define  _DSLG_SECCO_STATS_SUPPORT_*/

/*#define  _DSLG_LAYER3_ASYNC_RECV_ */

/**********************************************************************
                    OS/PLATFORM SPECIFIC DEFINITIONS
**********************************************************************/

    #define  DSLG_DEF_XML_CONFIG_FILE_PATH_S        ""
    /*
     *  Region code is injected into default configuration file name
     *
    #define  DSLG_DEF_XML_CONFIG_FILE_NAME          "/usr/dslg/config/dslg_def_cfg.xml.gz"
     */
    #define  DSLG_DEF_XML_CONFIG_FILE_NAME_S        "/config/dslg_def_cfg.xml.gz"
    #define  DSLG_CUR_XML_CONFIG_FILE_NAME_S        "/config/dslg_cur_cfg.xml.gz"
    #define  DSLG_BAK_XML_CONFIG_FILE_NAME_S        "/config/dslg_bak_cfg.xml.gz"
    #define  DSLG_TMP_XML_CONFIG_FILE_NAME_S        "/var/tmp/dslg_tmp_cfg.xml.gz"

    #define  DSLG_DEF_XML_CONFIG_FILE_PATH_T        ""
    #define  DSLG_DEF_XML_CONFIG_FILE_NAME_T        "/config/tr069_def_cfg.xml.gz"
    #define  DSLG_CUR_XML_CONFIG_FILE_NAME_T        "/config/tr069_cur_cfg.xml.gz"
    #define  DSLG_BAK_XML_CONFIG_FILE_NAME_T        "/config/tr069_bak_cfg.xml.gz"
    #define  DSLG_TMP_XML_CONFIG_FILE_NAME_T        "/var/tmp/tr069_tmp_cfg.xml.gz"

    #define  DSLG_DEF_CTLV_CONFIG_FILE_PATH         ""
    /*
     *  Region code is injected into default configuration file name
     *
    #define  DSLG_DEF_CTLV_CONFIG_FILE_NAME         "/usr/dslg/config/dslg_def_cfg.bps.gz"
     */
    #define  DSLG_DEF_CTLV_CONFIG_FILE_NAME         "/config/dslg_def_cfg.bps.gz"
    #define  DSLG_CUR_CTLV_CONFIG_FILE_NAME         "/config/dslg_cur_cfg.bps.gz"
    #define  DSLG_BAK_CTLV_CONFIG_FILE_NAME         "/config/dslg_bak_cfg.bps.gz"
    #define  DSLG_TMP_CTLV_CONFIG_FILE_NAME         "/var/tmp/dslg_tmp_cfg.bps.gz"


    #define  DSLG_DEVICE_LOG_FILE_NAME              "/var/dslg/local_log.log"


/**********************************************************************
                      MODULE SPECIFIC DEFINITIONS
**********************************************************************/

/*
 *  DSLM
 */
#define  DSLM_WAN_DSL_SHOWTIME_SUPPORTED            0

#define  DSLM_CONFIG_FILE_NAME                      "dslm_cfg.txt"

    #define  DSLM_TEMP_FIRMWARE_IMAGE               "/var/dslg_tmpfw.bin"

    #define  DSLM_VOICE_STATUS_FILE                 "/tmp/stop_voice"
    #define  DSLM_VOICE_WAIT_DELAY                  30
    #define  DSLM_VOICE_RESET_DELAY                 10

/*
 *  DSLM Web Server
 */
#define  DSLM_WEBS_PORT_UM							8081
#define  DSLM_WEBS_PORT_DM							80

#define  DSLM_WEBS_ROOT_PATH_CPE_LINUX              "/config/bmw2/"
#define  DSLM_WEBS_ROOT_PATH_UM_LINUX               "/config/bmw2/"
#define  DSLM_WEBS_ROOT_PATH_DM_LINUX               "/config/bmw2/"

/*
 *  Manufacture Data
 */
    #define  DSLG_MFD_FILE_SN                       "/var/mfd_sn"

/*
 *  region options
 */
    #define  DSLG_MFD_FILE_REGION                   "/var/mfd_region"
    #define  DSLG_MFD_FILE_LANGUAGE                 "/var/mfd_language"

/*
 *  certificates
 */
    #define  DSLM_TR069_SSL_ROOT_CA_CERT_FILE       "/config/rootca.cer"
    #define  DSLM_TR069_SSL_CLIENT_CERT_FILE        "/var/mfd_client.cer"
    #define  DSLM_TR069_SSL_CLIENT_PRIV_KEY_FILE    "/var/mfd_client.pk"


#endif

