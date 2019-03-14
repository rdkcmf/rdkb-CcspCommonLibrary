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

    module:	ansc_object.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        data structures and basic types.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/16/01    initial revision.
        10/14/01    extend the ansc platform to support some base
                    functional objects derived from the component
                    object class.

**********************************************************************/


#ifndef  _ANSC_OBJECT_
#define  _ANSC_OBJECT_


/***********************************************************
         DEFINITION OF SOME UTILITY DATA STRUCTURES
***********************************************************/

/*
 * While it's not part of Ansc's responsibility nor it would cause any significant problem for
 * objects management if we don't centrally control the oid assignment, we assign a unique range
 * to each module container.
 */
#define  ANSC_OID_BASE                              0
#define  SCE_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00000000
#define  NSM_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00010000
#define  SLEE_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00020000
#define  PSE_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00030000
#define  GPP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00040000
#define  SPP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00050000
#define  SPA_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00060000
#define  PQA_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00070000
#define  SFC_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00080000

#define  SYS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00100000
#define  BMW_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00110000
#define  BMS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00120000
#define  BMN_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x00130000
#define  SLAP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00140000
#define  BMW2_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00150000
#define  BEEP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00160000
#define  BREE_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00170000
#define  BWRM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00180000
#define  BWSP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x00190000
#define  UPNP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x001A0000
#define  UQOS_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x001B0000
#define  BMC2_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x001C0000

#define  PPLM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01000000
#define  PPTM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01010000
#define  POEMC_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x01020000
#define  POEMS_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x01030000
#define  DHCPS_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x01040000
#define  DHCPC_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x01050000
#define  RTP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01060000
#define  L2TM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01070000
#define  NTM_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01080000
#define  SIP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01090000
#define  SDP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x010A0000
#define  DNS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x010B0000
#define  IP4_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x010C0000
#define  ARP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x010D0000
#define  ICMP4_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x010E0000
#define  IGMP2_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x010F0000
#define  TCP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01100000
#define  UDP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01110000
#define  GTP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01120000
#define  BSS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01130000
#define  HTTP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01140000
#define  WEB_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01150000
#define  RAM_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01160000
#define  CYFW_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01170000
#define  SNMP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01180000
#define  MIB2_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01190000
#define  MOAM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x011A0000
#define  KB5_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x011B0000
#define  KBKM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x011C0000
#define  RADS_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x011D0000
#define  CONN_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x011E0000
#define  PING_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x011F0000
#define  TFTP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01200000
#define  MSNP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01210000
#define  TLS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x01220000
#define  RMIB_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01230000
#define  RAUTH_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x01240000
#define  IDSE_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01250000
#define  GIDS_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01260000
#define  TELNET_COMPONENT_OID_BASE                  ANSC_OID_BASE   + 0x01270000
#define  SCLI_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01280000
#define  LDAP_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x01290000
#define  VOH_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x012A0000
#define  EAP_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x012B0000
#define  IGMP3_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x012C0000
#define  WPS_COMPONENT_OID_BASE                     ANSC_OID_BASE   + 0x012D0000
#define  LLTD_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x012E0000
#define  STUN_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x012F0000

#define  MSDM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x02000000
#define  MMCD_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x02010000

#define  GNPS_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08000000
#define  IP4S_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08010000
#define  IP6S_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08020000
#define  RIP2_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08030000
#define  OSPF_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08040000
#define  SNPU_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08050000
#define  DFSV_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x08060000
#define  PIMDM2_COMPONENT_OID_BASE                  ANSC_OID_BASE   + 0x08070000
#define  SNPU3_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x08080000
#define  SNPU4_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x08090000

#define  RGWM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A000000
#define  CBHM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A010000
#define  PCBM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A020000
#define  CCGM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A030000
#define  TANK_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A040000
#define  MCLB_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A050000
#define  BOND_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A060000
#define  HMSD_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A070000
#define  DSLH_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A080000
#define  DSLG_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A090000
#define  DSLM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A0A0000
#define  BBHM_COMPONENT_OID_BASE                    ANSC_OID_BASE   + 0x0A0B0000
#define  CCSP_CWMP_COMPONENT_OID_BASE               ANSC_OID_BASE   + 0x0A0C0000

#define  DOT11_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x0B000000
#define  OGA11_COMPONENT_OID_BASE                   ANSC_OID_BASE   + 0x0B010000
#define  DOT11E_COMPONENT_OID_BASE                  ANSC_OID_BASE   + 0x0B020000

/*
 * We define some const values that will be used regularly for describing the bisga objects. For
 * instance, objects that are not derived from any base class shall define a null object as the
 * base class and zero as the base object id.
 */
#define  ANSC_OBJECT_NAME_NULL                      "nullObject"
#define  ANSC_OBJECT_TYPE_NULL                      "nullType"
#define  ANSC_OBJECT_OID_NULL                       0
#define  ANSC_OBJECT_RID_CONTAINER                  0

/*
 * There are much information exchanged between the Sce and other modules in the system. Most of
 * the information is object-related. To fully describe an object, we use the following data
 * structure, which includes the object name, oid, base object name, object type as derived from
 * the base object, and so forth.
 */
typedef  struct
_ANSC_OBJECT_TAG
{
    SINGLE_LINK_ENTRY               Linkage;
    char                            BaseName   [ANSC_OBJECT_NAME_SIZE];
    char                            DerivedType[ANSC_OBJECT_NAME_SIZE];
    char                            Name       [ANSC_OBJECT_NAME_SIZE];
    ULONG                           BaseOid;
    ULONG                           Oid;
    ULONG                           BrokerOid;
    void*                           pfnObjectCreate;
    void*                           pfnObjectRemove;
}
ANSC_OBJECT_TAG,  *PANSC_OBJECT_TAG;

#define  ACCESS_ANSC_OBJECT_TAG(p)                  \
         ACCESS_CONTAINER(p, ANSC_OBJECT_TAG, Linkage)


/***********************************************************
            FUNCTIONAL OBJECT CLASS DEFINITIONS
***********************************************************/

/*
 * As a common practice, any functional object defined as part of the ansc wrapper platform doesn't
 * occupy any oid space because it's not expected to be manufactured in runtime by external objects
 * via IPC or the provisioning interface. However, there's a need for deriving ansc functional
 * objects from the base component object instead of encapsulating them separately in a different
 * class definition, for the sake of easy integration and unified object implementation across
 * containers. While these objects don't need an oid field in their definitions, we define them
 * below for the sake of completeness.
 */
#define  ANSC_FUNCTIONAL_OID_BASE                   0xFFFFFFF0

#define  ANSC_TIMER_SCHEDULER_NAME                  "timerScheduler"
#define  ANSC_TIMER_SCHEDULER_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x0001
#define  ANSC_TIMER_SCHEDULER_TYPE                  ANSC_OBJECT_TYPE_NULL

#define  ANSC_TIMER_DESCRIPTOR_NAME                 "timerDescriptor"
#define  ANSC_TIMER_DESCRIPTOR_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x0002
#define  ANSC_TIMER_DESCRIPTOR_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_OBJECT_CONTAINER_NAME                 "objectContainer"
#define  ANSC_OBJECT_CONTAINER_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x0003
#define  ANSC_OBJECT_CONTAINER_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_PIPE_CONNECTOR_NAME                   "pipeConnector"
#define  ANSC_PIPE_CONNECTOR_OID                    ANSC_FUNCTIONAL_OID_BASE    - 0x0004
#define  ANSC_PIPE_CONNECTOR_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLEX_CONNECTOR_NAME                "simplexConnector"
#define  ANSC_SIMPLEX_CONNECTOR_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0005
#define  ANSC_SIMPLEX_CONNECTOR_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_IP4_FRAGMENTER_NAME                   "ip4Fragmenter"
#define  ANSC_IP4_FRAGMENTER_OID                    ANSC_FUNCTIONAL_OID_BASE    - 0x0006
#define  ANSC_IP4_FRAGMENTER_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  ANSC_WALL_CLOCK_NAME                       "wallClock"
#define  ANSC_WALL_CLOCK_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x0007
#define  ANSC_WALL_CLOCK_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_NTP_CLOCK_NAME                        "ntpClock"
#define  ANSC_NTP_CLOCK_OID                         ANSC_FUNCTIONAL_OID_BASE    - 0x0008
#define  ANSC_NTP_CLOCK_TYPE                        "ntp"

#define  ANSC_CIRCULAR_BUFFER_NAME                  "circularBuffer"
#define  ANSC_CIRCULAR_BUFFER_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x0009
#define  ANSC_CIRCULAR_BUFFER_TYPE                  ANSC_OBJECT_TYPE_NULL

#define  ANSC_AUDIO_CODEC_NAME                      "audioCodec"
#define  ANSC_AUDIO_CODEC_OID                       ANSC_FUNCTIONAL_OID_BASE    - 0x000A
#define  ANSC_AUDIO_CODEC_TYPE                      ANSC_OBJECT_TYPE_NULL

#define  ANSC_VIDEO_CODEC_NAME                      "videoCodec"
#define  ANSC_VIDEO_CODEC_OID                       ANSC_FUNCTIONAL_OID_BASE    - 0x000B
#define  ANSC_VIDEO_CODEC_TYPE                      ANSC_OBJECT_TYPE_NULL

#define  ANSC_TASK_ORGANIZER_NAME                   "taskOrganizer"
#define  ANSC_TASK_ORGANIZER_OID                    ANSC_FUNCTIONAL_OID_BASE    - 0x000C
#define  ANSC_TASK_ORGANIZER_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  ANSC_TASK_ENTRY_NAME                       "taskEntry"
#define  ANSC_TASK_ENTRY_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x000D
#define  ANSC_TASK_ENTRY_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_ATOM_TABLE_NAME                       "atomTable"
#define  ANSC_ATOM_TABLE_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x000E
#define  ANSC_ATOM_TABLE_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_OCO_VER1_NAME                         "ocoVer1"
#define  ANSC_OCO_VER1_OID                          ANSC_FUNCTIONAL_OID_BASE    - 0x000F
#define  ANSC_OCO_VER1_TYPE                         "ver1"

#define  ANSC_OCO_VER2_NAME                         "ocoVer2"
#define  ANSC_OCO_VER2_OID                          ANSC_FUNCTIONAL_OID_BASE    - 0x0010
#define  ANSC_OCO_VER2_TYPE                         "ver2"

#define  ANSC_DAEMON_SERVER_TCP_NAME                "daemonServerTcp"
#define  ANSC_DAEMON_SERVER_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0011
#define  ANSC_DAEMON_SERVER_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_DAEMON_ENGINE_TCP_NAME                "daemonEngineTcp"
#define  ANSC_DAEMON_ENGINE_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0012
#define  ANSC_DAEMON_ENGINE_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_DAEMON_SOCKET_TCP_NAME                "daemonSocketTcp"
#define  ANSC_DAEMON_SOCKET_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0013
#define  ANSC_DAEMON_SOCKET_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_DAEMON_SERVER_UDP_NAME                "daemonServerUdp"
#define  ANSC_DAEMON_SERVER_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0014
#define  ANSC_DAEMON_SERVER_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_DAEMON_ENGINE_UDP_NAME                "daemonEngineUdp"
#define  ANSC_DAEMON_ENGINE_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0015
#define  ANSC_DAEMON_ENGINE_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_DAEMON_SOCKET_UDP_NAME                "daemonSocketUdp"
#define  ANSC_DAEMON_SOCKET_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0016
#define  ANSC_DAEMON_SOCKET_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_SERVER_TCP_NAME                "brokerServerTcp"
#define  ANSC_BROKER_SERVER_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0017
#define  ANSC_BROKER_SERVER_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_ENGINE_TCP_NAME                "brokerEngineTcp"
#define  ANSC_BROKER_ENGINE_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0018
#define  ANSC_BROKER_ENGINE_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_SOCKET_TCP_NAME                "brokerSocketTcp"
#define  ANSC_BROKER_SOCKET_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x0019
#define  ANSC_BROKER_SOCKET_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_SERVER_UDP_NAME                "brokerServerUdp"
#define  ANSC_BROKER_SERVER_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001A
#define  ANSC_BROKER_SERVER_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_ENGINE_UDP_NAME                "brokerEngineUdp"
#define  ANSC_BROKER_ENGINE_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001B
#define  ANSC_BROKER_ENGINE_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_BROKER_SOCKET_UDP_NAME                "brokerSocketUdp"
#define  ANSC_BROKER_SOCKET_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001C
#define  ANSC_BROKER_SOCKET_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_CLIENT_TCP_NAME                "simpleClientTcp"
#define  ANSC_SIMPLE_CLIENT_TCP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001D
#define  ANSC_SIMPLE_CLIENT_TCP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_CLIENT_UDP_NAME                "simpleClientUdp"
#define  ANSC_SIMPLE_CLIENT_UDP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001E
#define  ANSC_SIMPLE_CLIENT_UDP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_CLIENT_GTP_NAME                "simpleClientGtp"
#define  ANSC_SIMPLE_CLIENT_GTP_OID                 ANSC_FUNCTIONAL_OID_BASE    - 0x001F
#define  ANSC_SIMPLE_CLIENT_GTP_TYPE                ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_PROXY_TCP_NAME                 "simpleProxyTcp"
#define  ANSC_SIMPLE_PROXY_TCP_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x0020
#define  ANSC_SIMPLE_PROXY_TCP_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_PROXY_UDP_NAME                 "simpleProxyUdp"
#define  ANSC_SIMPLE_PROXY_UDP_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x0021
#define  ANSC_SIMPLE_PROXY_UDP_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_SOCKET_NAME                           "socket"
#define  ANSC_SOCKET_OID                            ANSC_FUNCTIONAL_OID_BASE    - 0x0022
#define  ANSC_SOCKET_TYPE                           ANSC_OBJECT_TYPE_NULL

#define  ANSC_XSOCKET_NAME                          "xsocket"
#define  ANSC_XSOCKET_OID                           ANSC_FUNCTIONAL_OID_BASE    - 0x0023
#define  ANSC_XSOCKET_TYPE                          ANSC_OBJECT_TYPE_NULL

#define  ANSC_ATO_PROTOCOL_NAME                     "atoProtocol"
#define  ANSC_ATO_PROTOCOL_OID                      ANSC_FUNCTIONAL_OID_BASE    - 0x0024
#define  ANSC_ATO_PROTOCOL_TYPE                     "protocol"

#define  ANSC_QUERY_INTERFACE_NAME                  "queryInterface"
#define  ANSC_QUERY_INTERFACE_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x0025
#define  ANSC_QUERY_INTERFACE_TYPE                  ANSC_OBJECT_TYPE_NULL

#define  ANSC_MODULE_CONTAINER_NAME                 "moduleContainer"
#define  ANSC_MODULE_CONTAINER_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x0026
#define  ANSC_MODULE_CONTAINER_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_ACCESS_MANAGER_NAME                   "accessManager"
#define  ANSC_ACCESS_MANAGER_OID                    ANSC_FUNCTIONAL_OID_BASE    - 0x0027
#define  ANSC_ACCESS_MANAGER_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  ANSC_ACCESS_KEY_NAME                       "accessKey"
#define  ANSC_ACCESS_KEY_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x0028
#define  ANSC_ACCESS_KEY_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_SYSTEM_CLOCK_NAME                     "systemClock"
#define  ANSC_SYSTEM_CLOCK_OID                      ANSC_FUNCTIONAL_OID_BASE    - 0x0029
#define  ANSC_SYSTEM_CLOCK_TYPE                     "system"

#define  ANSC_SMART_CLOCK_NAME                      "smartClock"
#define  ANSC_SMART_CLOCK_OID                       ANSC_FUNCTIONAL_OID_BASE    - 0x002A
#define  ANSC_SMART_CLOCK_TYPE                      "smart"

#define  ANSC_VALUE_NAME_MAPER_NAME                 "valueNameMaper"
#define  ANSC_VALUE_NAME_MAPER_OID                  ANSC_FUNCTIONAL_OID_BASE    - 0x002B
#define  ANSC_VALUE_NAME_MAPER_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_SYSLOG_SENDER_NAME                    "syslogSender"
#define  ANSC_SYSLOG_SENDER_OID                     ANSC_FUNCTIONAL_OID_BASE    - 0x002C
#define  ANSC_SYSLOG_SENDER_TYPE                    ANSC_OBJECT_TYPE_NULL

#define  ANSC_SIMPLE_SYS_TIME_NAME                  "simpleSysTime"
#define  ANSC_SIMPLE_SYS_TIME_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x002D
#define  ANSC_SIMPLE_SYS_TIME_TYPE                  ANSC_OBJECT_TYPE_NULL

#define  ANSC_TOD_CLIENT_NAME                       "todClient"
#define  ANSC_TOD_CLIENT_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x002E
#define  ANSC_TOD_CLIENT_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_TOD_SERVER_NAME                       "todServer"
#define  ANSC_TOD_SERVER_OID                        ANSC_FUNCTIONAL_OID_BASE    - 0x002F
#define  ANSC_TOD_SERVER_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  ANSC_TFTP_CLIENT_NAME                      "tftpClient"
#define  ANSC_TFTP_CLIENT_OID                       ANSC_FUNCTIONAL_OID_BASE    - 0x0030
#define  ANSC_TFTP_CLIENT_TYPE                      ANSC_OBJECT_TYPE_NULL
 
#define  ANSC_SORT_LIST_NAME                        "sortList"
#define  ANSC_SORT_LIST_OID                         ANSC_FUNCTIONAL_OID_BASE    - 0x0031
#define  ANSC_SORT_LIST_TYPE                        ANSC_OBJECT_TYPE_NULL
 
#define  ANSC_LPC_CONNECTOR_NAME                    "lpcConnector"
#define  ANSC_LPC_CONNECTOR_OID                     ANSC_FUNCTIONAL_OID_BASE    - 0x0032
#define  ANSC_LPC_CONNECTOR_TYPE                    ANSC_OBJECT_TYPE_NULL

#define  ANSC_LPCCO_TCP_NAME                        "lpccoTcp"
#define  ANSC_LPCCO_TCP_OID                         ANSC_FUNCTIONAL_OID_BASE    - 0x0033
#define  ANSC_LPCCO_TCP_TYPE                        "tcp"
 
#define  ANSC_DLS_MANAGER_NAME                      "dlsManager"
#define  ANSC_DLS_MANAGER_OID                       ANSC_FUNCTIONAL_OID_BASE    - 0x0034
#define  ANSC_DLS_MANAGER_TYPE                      ANSC_OBJECT_TYPE_NULL
 
#define  ANSC_UDP_PING_SERVER_NAME                  "udpPingServer"
#define  ANSC_UDP_PING_SERVER_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x0035
#define  ANSC_UDP_PING_SERVER_TYPE                  ANSC_OBJECT_TYPE_NULL
 
#define  ANSC_UDP_PING_CLIENT_NAME                  "udpPingClient"
#define  ANSC_UDP_PING_CLIENT_OID                   ANSC_FUNCTIONAL_OID_BASE    - 0x0036
#define  ANSC_UDP_PING_CLIENT_TYPE                  ANSC_OBJECT_TYPE_NULL


#endif
