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

    module: kernel_base_custom_linux.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This header file gives the custom ANSC wrapper base 
        definitions.

    ---------------------------------------------------------------

    environment:

        Linux 2.4 or later

    ---------------------------------------------------------------

    author:

        Ding Hua

**********************************************************************/

#ifndef  _KERNEL_BASE_CUSTOM_LINUX_H_
#define  _KERNEL_BASE_CUSTOM_LINUX_H_


#include "kernel_base_custom_linux_build.h"


/***************************************************************
        Platform/Compiler Specific Definitions
***************************************************************/

/*
 *  Indicate whether massive PRAGMA functions are supported
 */
/*#define  _ANSC_ENABLE_PRAGMA_*/

/*
 *  Endian order    -- note, if AL_IS_LITTLE_ENDIAN is still used
 *  in any platform, replace it with _ANSC_LITTLE_ENDIAN_!!!
 */
/*#define  _ANSC_LITTLE_ENDIAN_*/
/*#define  _ANSC_BIG_ENDIAN_*/

/*
 *  Indicates whether word access has to occur on word boundary
 */
#define  _ANSC_BYTE_ACCESS_
/*#define  _ANSC_NO_HALFWORD_ACCESS_ */  /* for old cpu arch that does not support half-word access at boundary 2*/

/***************************************************************
                OS Specific Definitions
***************************************************************/

/*#define  _ANSC_LINUX_2_6_*/

/*
#define  _ANSC_DEFINED_UCHAR
#define  _ANSC_DEFINED_ULONG
#define  _ANSC_DEFINED_USHORT
#define  _ANSC_DEFINED_BOOL
#define  _ANSC_DEFINED_UINT
 */

/*#define  _ANSC_LINUX_NO_need_resched*/
/*#define  _ANSC_LINUX_NEED_memchr*/

/***************************************************************
            Individual Module Conditional Definitions
***************************************************************/

/*
 *  Leave them here temporarily, should move to individual module
 *  custom definition header files or the make file
 */

/*
 *  Indicate whether the ANSC built-in profiling is enabled
 *  NOT defined in makefile, defined in kernel_base_custom_linux_build.h
 */
/*#define  _ANSC_ENABLE_PROFILING_*/
/*
 *  Indicates whether ANSC timer descriptoir object profiling is required
 */
/*#define  _ANSC_TDO_PROFILING_*/
/*
 *  Indicates whether ANSC spinlock profiling is required
 */
/*#define  _ANSC_SPINLOCK_PROFILING_*/

/*#define  _ANSC_SOCKET_LIBRARY_*/                  /*defined in makefile*/
/*#define  _ANSC_SOCKET_TLS_LAYER_*/

/*#define  _ANSC_SIP_ALG_*/                         /*defined in makefile*/
#define  USE_PUBLIC_KEY

/*#define  _ANSC_MULTICAST_ROUTING_*/

#define  _ANSC_LEAN_SNPU10_
#define  _ANSC_MEAN_SNPU10_

#define  _ANSC_ARP_CACHE_ARRAY_
/*#define  _ANSC_X509_CERT_*/

/*#define  _ANSC_FILE_SEARCH_*/
/*#define  _ANSC_FILE_GZIP_*/
/*#define  _ANSC_FILE_ZLIB_*/

/*#define  _ANSC_SLAP_LPC_*/

#define  _ANSC_DIRTY_PDO_
/*#define  _ANSC_TRACE_F_*/

/*#define  _ANSC_IP4S_IGMPV2_*/
#define  _ANSC_IP4S_IGMPV3_
#define  _ANSC_IGMP_PROXY_
#define  _ANSC_IGMP_SNOOPING_

/*#define  _ANSC_MIB2_PRINT_MIB_REG_*/

/*#define  _ANSC_GNPS_GRE_*/
/*#define  _ANSC_GNPS_PPTP_PAC_*/
/*#define  _ANSC_GNPS_L2TP_LAC_*/
#define  _ANSC_GNPS_PPPOE_
#define  _ANSC_GNPS_PPP_
#define  _ANSC_GNPS_SNPU3_
/*#define  _ANSC_GNPS_SNPU4_*/
/*#define  _ANSC_GNPS_TRUNKING_*/

/*#define _ANSC_IP4S_TCP_*/
#define _ANSC_IP4S_UDP_
/*#define _ANSC_IP4S_GTP_*/
/*#define _ANSC_PPP_MSCHAP2_*/

/*#define _ANSC_SNMP_ */

#define  PPTM_TCO_HOST_NAME                         "Bond"
#define  PPTM_TCO_VENDOR_STRING                     "AL"

#endif

