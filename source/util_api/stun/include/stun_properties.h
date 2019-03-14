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

    module:	stun_properties.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
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


#ifndef  _STUN_PROPERTIES_
#define  _STUN_PROPERTIES_


#include  "stun_definitions.h"


/***********************************************************
   STUN SIMPLE CLIENT CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The STUN client is typically embedded in an application which needs to obtain a public IP
 * address and port that can be used to receive data. For example, it might need to obtain an IP
 * address and port to receive Real Time Transport Protocol (RTP) traffic.
 */
#define  STUN_MAX_USERNAME_SIZE                     257
#define  STUN_MAX_PASSWORD_SIZE                     257
#define  STUN_MIN_KEEP_ALIVE_INTERVAL               10

typedef  struct
_STUN_SIMPLE_CLIENT_PROPERTY
{
    ANSC_IPV4_ADDRESS               ClientAddr;         /* this value is zero by default */
    USHORT                          ClientPort;         /* this value is zero by default */
    ULONG                           ClientMode;         /* scuo mode of the UDP socket   */

    ANSC_IPV4_ADDRESS               ServerAddr;
    USHORT                          ServerPort;
    char                            Username[STUN_MAX_USERNAME_SIZE];
    char                            Password[STUN_MAX_PASSWORD_SIZE];

    int                             MinKeepAliveInterval;
    int                             CurKeepAliveInterval;
    int                             MaxKeepAliveInterval;
}
STUN_SIMPLE_CLIENT_PROPERTY,  *PSTUN_SIMPLE_CLIENT_PROPERTY;


#endif
