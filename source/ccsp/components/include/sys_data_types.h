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

    module:	sys_data_types.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the system- and package-wide runtime
        environmental variables.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/02    initial revision.

**********************************************************************/


#ifndef  _SYS_DATA_TYPES_
#define  _SYS_DATA_TYPES_


/***********************************************************
         SYS INFO REPOSITORY ENUM DATA TYPE DEFINITION
***********************************************************/

/*
 * The internal implementation of Repository Rro Enum Object is no differently from the Rro Uint
 * Object, but its existence usually indicates the need for translating an integer-based Enum data
 * type into some meaningful human-readable string when the content is being rendered. For example:
 * the IP transport type, such as TCP and UDP, is a popular configuration parameter across the
 * system. This variable is stored as a one-byte integer internally, but shall be translated into
 * its corresponding protocol name when presenting such information to human users.
 */
#define  SYS_DTYPE_ENUM_APP_PORT                    "appPort"               /* application port number contained in TCP/UDP header */
#define  SYS_DTYPE_ENUM_IP_TRANSPORT                "ip4TransportType"      /* transport protocol type contained in IP header      */
#define  SYS_DTYPE_ENUM_LINK_TYPE                   "phyiscalLinkType"      /* physical link type: defined in gnps_properties.h"   */
#define  SYS_DTYPE_ENUM_IP4S_PARTY                  "ip4sParty"             /* active parties: defined in ip4s_properties.h"       */
#define  SYS_DTYPE_ENUM_SERVICE_STATE               "serviceState"          /* various runtime state of system service modules     */


/***********************************************************
         SYS INFO REPOSITORY MASK DATA TYPE DEFINITION
***********************************************************/

/*
 * The internal implementation of Repository Rro Mask Object is no differently from the Rro Uint
 * Object, but its existence usually indicates the need for translating an integer-based Mask data
 * type into some meaningful human-readable string when the content is being rendered. For example:
 * the IP transport type, such as TCP and UDP, is a popular configuration parameter across the
 * system. This variable is stored as a one-byte integer internally, but shall be translated into
 * its corresponding protocol name when presenting such information to human users.
 */
#define  SYS_DTYPE_MASK_VPN_PROTOCOL                "vpnProtocol"           /* VPN protocols that are used for remote user access  */


#endif
