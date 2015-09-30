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

    module:	ansc_packet_binary.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of different network protocols.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/18/01    modify the file name from ansc_protocol_binary
                    to ansc_packet_binary.
        01/19/01    restructure the container object, add and
                    remove a few things
        09/11/01    add ppp related packet structures
        10/03/01    break it up into several header files; one for
                    each specific protocol suite

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_
#define  _ANSC_PACKET_BINARY_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif


/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */
#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
  GENERAL NETWORK RELATED DATA STRUCTURES AND DEFINITIONS
***********************************************************/

/*
 * Following definitions can not be categorized for any specific protocol. Instead, they generalize
 * the characteristics for network interfaces, mac protocols, network protocols, and so on. Some of
 * the definitions can also be found in other modules, and we need to maintain the consistency.
 */
#define  ANSC_PROTO_NAME_ARRAY_SIZE                 19      /* must be modified every time we add more protocols into the ATO */
#define  ANSC_MAX_MAC_HEADER_SIZE                   32
#define  ANSC_MAX_MAC_ADDRESS_SIZE                  6
#define  ANSC_MAC_ADDRESS_SIZE                      6

#define  ANSC_INTERFACE_TYPE_ETHERNET               1
#define  ANSC_INTERFACE_TYPE_CABLE                  2
#define  ANSC_INTERFACE_TYPE_DSL                    3
#define  ANSC_INTERFACE_TYPE_WIRELESS               4
#define  ANSC_INTERFACE_TYPE_DIALUP                 5

#define  ANSC_NETWORK_TYPE_IP4                      1
#define  ANSC_NETWORK_TYPE_IP6                      2
#define  ANSC_NETWORK_TYPE_ATM                      3

#define  AnscSetBroadcastMacAddr(addr)                                                      \
         {                                                                                  \
            AnscWriteUshort(&addr[0], 0xFFFF    );                                          \
            AnscWriteUlong (&addr[2], 0xFFFFFFFF);                                          \
         }


/***********************************************************
    TCP/UDP APPLICATION PROTOCOL PORT NUMBER DEFINITIONS
***********************************************************/

/*
 * We shall maintain a mapping list between the port number and the popular application protocols,
 * and make this list upgradable from the management server. But we use the following static
 * definition temporarily.
 */
#define  WILDCARD_PORT                              0xFFFF
#define  DHCP_SERVER_PORT                           67
#define  DHCP_CLIENT_PORT                           68
#define  HTTP_SERVER_PORT                           80
#define  HTTP_SERVER_TLS_PORT                       443
#define  HTTP_PORT                                  HTTP_SERVER_PORT
#define  HTTPS_PORT                                 HTTP_SERVER_TLS_PORT
#define  PPTP_PORT                                  1723
#define  RTP_PORT                                   5004
#define  RTCP_PORT                                  5005
#define  DNS_SERVER_PORT                            53
#define  SNMP_RESPONDER_PORT                        161
#define  SNMP_NOTIFICATION_PORT                     162
#define  SYSLOG_PORT                                514
#define  TOD_PORT                                   37
#define  TFTP_PORT                                  69
#define  MSNP_PORT                                  1863
#define  SIP_PORT                                   5060
#define  SIP_TLS_PORT                               5061
#define  L2TP_PORT                                  1701
#define  FTP_CTRL_PORT                              21
#define  STUN_PORT                                  3478
#define  ECHO_PORT                                  7
#define  CWMP_PORT                                  7547

#define  APP_PORT_DHCP_SERVER                       67
#define  APP_PORT_DHCP_CLIENT                       68
#define  APP_PORT_HTTP                              80
#define  APP_PORT_HTTP_TLS                          443
#define  APP_PORT_HTTPS                             APP_PORT_HTTP_TLS
#define  APP_PORT_PPTP                              1723
#define  APP_PORT_RTP                               5004
#define  APP_PORT_RTCP                              5005
#define  APP_PORT_DNS                               53
#define  APP_PORT_SNMP_RESPONDER                    161
#define  APP_PORT_SNMP_NOTIFICATION                 162
#define  APP_PORT_SYSLOG                            514
#define  APP_PORT_TOD                               37
#define  APP_PORT_TFTP                              69
#define  APP_PORT_MSNP                              1863
#define  APP_PORT_SIP                               5060
#define  APP_PORT_SIP_TLS                           5061
#define  APP_PORT_L2TP                              L2TP_PORT
#define  APP_PORT_FTP_CTRL                          FTP_CTRL_PORT
#define  APP_PORT_STUN                              STUN_PORT
#define  APP_PORT_ECHO                              ECHO_PORT
#define  APP_PORT_CWMP                              CWMP_PORT


/***********************************************************
         INCLUDE HEADER FILES FOR SPECIFIC PROTOCOLS
***********************************************************/

/*
 * The protocol message format definitions spread across multiple header files. Many existing
 * implementions included this file as the primary entry for containing protocol definitions.
 * We include all the protocol-specific header files here.
 */
#include "ansc_packet_binary_eth.h"
#include "ansc_packet_binary_ip4.h"
#include "ansc_packet_binary_ip6.h"
#include "ansc_packet_binary_arp4.h"
#include "ansc_packet_binary_tcp.h"
#include "ansc_packet_binary_udp.h"
#include "ansc_packet_binary_icmp4.h"
#include "ansc_packet_binary_icmp6.h"
#include "ansc_packet_binary_igmp2.h"
#include "ansc_packet_binary_ah.h"
#include "ansc_packet_binary_esp.h"
#include "ansc_packet_binary_ipcomp.h"
#include "ansc_packet_binary_gre.h"
#include "ansc_packet_binary_pptp.h"
#include "ansc_packet_binary_ppp.h"
#include "ansc_packet_binary_pppoe.h"
#include "ansc_packet_binary_dhcp.h"
#include "ansc_packet_binary_rtp.h"
#include "ansc_packet_binary_rtcp.h"
#include "ansc_packet_binary_dns.h"
#include "ansc_packet_binary_http.h"
#include "ansc_packet_binary_ntp.h"
#include "ansc_packet_binary_l2tp.h"
#include "ansc_packet_binary_rip2.h"
#include "ansc_packet_binary_tftp.h"
#include "ansc_packet_binary_kerberos5.h"
#include "ansc_packet_binary_kbkm.h"
#include "ansc_packet_binary_sip.h"
#include "ansc_packet_binary_tls.h"
#include "ansc_packet_binary_ssl2.h"
#include "ansc_packet_binary_ike.h"
#include "ansc_packet_binary_isakmp.h"
#include "ansc_packet_binary_imcp.h"
#include "ansc_packet_binary_radius.h"
#include "ansc_packet_binary_ospf2.h"
#include "ansc_packet_binary_pimdm2.h"
#include "ansc_packet_binary_llc.h"
#include "ansc_packet_binary_bridge1d.h"
#include "ansc_packet_binary_eap.h"
#include "ansc_packet_binary_vlan.h"
#include "ansc_packet_binary_wps.h"
#include "ansc_packet_binary_lltd.h"
#include "ansc_packet_binary_stun.h"


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
