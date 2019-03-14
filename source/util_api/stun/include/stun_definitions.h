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

    module:	stun_definitions.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the STUN element parameters that can be
        applied to the STUN Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/29/08    initial revision.

**********************************************************************/


#ifndef  _STUN_DEFINITIONS_
#define  _STUN_DEFINITIONS_


/***********************************************************
           STUN STANDARD STATE MACHINE DEFINITIONS
***********************************************************/


#define  STUN_NAT_TYPE_Unknown                      0
#define  STUN_NAT_TYPE_FullCone                     1
#define  STUN_NAT_TYPE_RestrictedCone               2
#define  STUN_NAT_TYPE_PortRestrictedCone           3
#define  STUN_NAT_TYPE_Symmetric                    4

/*
 * Reliability is accomplished through client retransmissions. Clients SHOULD retransmit the
 * request starting with an interval of 100ms, doubling every retransmit until the interval reaches
 * 1.6s. Retransmissions continue with intervals of 1.6s until a response is received, or a total
 * of 9 requests have been sent. If no response is received by 1.6 seconds after the last request
 * has been sent, the client SHOULD consider the transaction to have failed. In other words,
 * requests would be sent at times 0ms, 100ms, 300ms, 700ms, 1500ms, 3100ms, 4700ms, 6300ms, and
 * 7900ms. At 9500ms, the client considers the transaction to have failed if no response has been
 * received.
 */
#define  STUN_MIN_RETRANSMISSION_INTERVAL           100     /* in milliseconds */
#define  STUN_MAX_RETRANSMISSION_INTERVAL           1600    /* in milliseconds */
#define  STUN_MIN_RETRANSMISSION_TIMES              9       /* in milliseconds */

/*
 * STUN can be used to discover the lifetimes of the bindings created by the NAT. In many cases,
 * the client will need to refresh the binding, either through a new STUN request, or an
 * application packet, in order for the application to continue to use the binding. By discovering
 * the binding lifetime, the client can determine how frequently it needs to refresh.
 */
#define  STUN_DISCOVER_PORT_BASE                    47890

/*
 * STUN is a simple client-server protocol. A client sends a request to a server, and the server
 * returns a response. There are two types of requests - Binding Requests, sent over UDP, and
 * Shared Secret Requests, sent over TLS [2] over TCP. Shared Secret Requests ask the server to
 * return a temporary username and password. This username and password are used in a subsequent
 * Binding Request and Binding Response, for the purposes of authentication and message integrity.
 */
typedef  struct
_STUN_BINDING_INFO
{
    BOOL                            bNatDetected;
    ULONG                           NatType;
    ANSC_IPV4_ADDRESS               Ip4Addr;
    USHORT                          UdpPort;
    ULONG                           LifeTime;       /* life time of the NAT binding */
    ULONG                           Timestamp;
}
STUN_BINDING_INFO,  *PSTUN_BINDING_INFO;

#define  StunInitBindingInfo(binding_info)                                                  \
         {                                                                                  \
            binding_info->bNatDetected  = FALSE;                                            \
            binding_info->NatType       = 0;                                                \
            binding_info->Ip4Addr.Value = 0;                                                \
            binding_info->UdpPort       = 0;                                                \
            binding_info->LifeTime      = 0;                                                \
            binding_info->Timestamp     = 0;                                                \
         }


#endif
