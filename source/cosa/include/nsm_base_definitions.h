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

    module:	nsm_base_definitions.h

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the common constant values
        and data structures shared by different components.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/12/01    initial revision.

**********************************************************************/


#ifndef  _NSM_BASE_DEFINTIONS_
#define  _NSM_BASE_DEFINTIONS_


/***********************************************************
       POLICY FIELDS DEFINITION FOR PACKET PROCESSING
***********************************************************/

/*
 * The interaction between different nsm components is mostly triggered by packets received from
 * the physical network or switch fiberic. The nsm container and its components are designed to
 * perform different functionalities and be isolated as much as possible. However, there're still
 * some common knowledge have to be defined.
 */
#define  NSM_FLOW_DIRECTION_INGRESS                 0x00000001
#define  NSM_FLOW_DIRECTION_EGRESS                  0x00000002

#define  NSM_SLEE_ACTION_PROCEED                    0
#define  NSM_SLEE_ACTION_DISCARD                    1
#define  NSM_SLEE_ACTION_DROPOUT                    2

#define  NSM_SLEE_LAYER_PHYSICAL                    1
#define  NSM_SLEE_LAYER_MAC                         2
#define  NSM_SLEE_LAYER_NETWORK                     3
#define  NSM_SLEE_LAYER_TRANSPORT                   4
#define  NSM_SLEE_LAYER_APP                         5
#define  NSM_SLEE_LAYER_URL                         6
#define  NSM_SLEE_LAYER_BODY                        100

/*
 * Deploying a new protocol support in Slee environment usually means implementing a set of new
 * extension and feature objects: Packet Header Object, Protocol Unit Object, Packet Strata Object,
 * Service Protocol Object, Service Bundle Object and so on. The relationship of all these objects
 * is identified by the same Psp (Protocol Service Provider) type.
 */
#define  NSM_PSP_TYPE_PHY_RESERVED                  0

#define  NSM_PSP_TYPE_MAC_ETHERNET                  1

#define  NSM_PSP_TYPE_NET_IP                        0x0800
#define  NSM_PSP_TYPE_NET_ARP                       0x0806

#define  NSM_PSP_TYPE_TSP_IP                        4
#define  NSM_PSP_TYPE_TSP_ICMP                      1
#define  NSM_PSP_TYPE_TSP_IGMP                      2
#define  NSM_PSP_TYPE_TSP_TCP                       6
#define  NSM_PSP_TYPE_TSP_UDP                       17
#define  NSM_PSP_TYPE_TSP_ESP                       50
#define  NSM_PSP_TYPE_TSP_AH                        51
#define  NSM_PSP_TYPE_TSP_IPCOMP                    108

#define  NSM_PSP_TYPE_APP_HTTP                      80
#define  NSM_PSP_TYPE_APP_FTP                       21

/*
 * Unlike the host-based protocol stack, which clearly defines the responsibility for each layer,
 * the protocol stack for router/gateway shall be optimized for the maximum efficiency and minimum
 * overhead. For instance, if all the service feature objects at the transport layer can handle
 * Ip fragments, then there's no need to queue the fragments and indicate them together.
 */
#define  NSM_SLEE_SWITCH_MODE_UNKNOWN               0
#define  NSM_SLEE_SWITCH_MODE_NOLIMIT               1
#define  NSM_SLEE_SWITCH_MODE_INORDER               2
#define  NSM_SLEE_SWITCH_MODE_INSQUAD               3
#define  NSM_SLEE_SWITCH_MODE_INWHOLE               4

/*
 * The Slee Session Entry is a generalized concept on grouping a sequential set of network packets
 * in a meaningful way. At any layer, there may be a need to manage certain type of packets in the
 * same way. For instance, mac-layer session is identified only by the source mac address while
 * network-layer session is identified by both the source and destination addresses. Although the
 * basic state-access and attribute management functions can be generalized, different layer may
 * have differnet bundle and band management schema.
 */
#define  NSM_SLEE_BAND_MAC_DEFAULT                  0x00000000
#define  NSM_SLEE_BAND_MAC_UNICAST                  0x00000001
#define  NSM_SLEE_BAND_MAC_MULTICAST                0x00000002
#define  NSM_SLEE_BAND_MAC_BROADCAST                0x00000004

#define  NSM_SLEE_BAND_NET_DEFAULT                  0x00000000
#define  NSM_SLEE_BAND_NET_INITIATOR                0x00000001
#define  NSM_SLEE_BAND_NET_RESPONDER                0x00000002
#define  NSM_SLEE_BAND_NET_SIGNALING                0x00000004

#define  NSM_SLEE_BAND_TSP_DEFAULT                  0x00000000
#define  NSM_SLEE_BAND_TSP_CLIENT                   0x00000001
#define  NSM_SLEE_BAND_TSP_SERVER                   0x00000002
#define  NSM_SLEE_BAND_TSP_SIGNAL                   0x00000004

#define  NSM_SLEE_BAND_APP_DEFAULT                  0x00000000
#define  NSM_SLEE_BAND_APP_CLIENT                   0x00000001
#define  NSM_SLEE_BAND_APP_SERVER                   0x00000002


#endif
