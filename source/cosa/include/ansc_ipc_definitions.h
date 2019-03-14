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

    module:	ansc_ipc_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the popular object and
        data structures for IPC/LPC/RPC scenarios.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/22/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_IPC_DEFINITIONS_
#define  _ANSC_IPC_DEFINITIONS_


/***********************************************************
        STANDARD IPC FUNDAMANTAL DATA TYPE DEFINITION
***********************************************************/

/*
 * In addition to the error information provided by the 'returnStatus', we define following IPC-
 * specific error codes in an attempt to offer more useful details for diagnosing IPC/LPC/RPC
 * communication scenarios.
 */
#define  ANSC_LPC_ERROR_noError                     0
#define  ANSC_LPC_ERROR_badPartyName                1
#define  ANSC_LPC_ERROR_managerUnavailable          2
#define  ANSC_LPC_ERROR_partyDisconnected           3
#define  ANSC_LPC_ERROR_requestTimeout              4
#define  ANSC_LPC_ERROR_resources                   5
#define  ANSC_LPC_ERROR_invalidReqType              6
#define  ANSC_LPC_ERROR_invalidReqData              7
#define  ANSC_LPC_ERROR_invalidReply                8
#define  ANSC_LPC_ERROR_networkFailure              9
#define  ANSC_LPC_ERROR_connectorReset              10
#define  ANSC_LPC_ERROR_internalError               11

#define  ANSC_LPC_PARTY_STATE_connectedIn           0x00000001
#define  ANSC_LPC_PARTY_STATE_connectedOut          0x00000002

#define  ANSC_LPC_PARTY_TYPE_self                   1
#define  ANSC_LPC_PARTY_TYPE_manager                2
#define  ANSC_LPC_PARTY_TYPE_client                 3

/*
 * We need a unified IPC framework in place to standardize the inter-module communication required
 * by most of the desktop operating systems. Because different operating systems may employ vastly
 * different IPC mechanisms, we need to shield the ANSC modules from dealing with such details. We
 * encapsulate the basic IPC functionalities in a set of ANSC interfaces, which may be derived to
 * use different mechanisms.
 */
#define  ANSC_LPC_MANAGER_PORT_NUMBER               20372
#define  ANSC_LPC_MANAGER_PARTY_NAME                "lpcManager"

#define  ANSC_LPC_OPMODE_STANDALONE                 1
#define  ANSC_LPC_OPMODE_CLIENT                     2
#define  ANSC_LPC_OPMODE_SERVER                     3

#define  ANSC_LPC_DEF_CALL_TIMEOUT                  60 * 1000   /* 60 seconds, in milliseconds   */
#define  ANSC_LPC_DEF_MESSAGE_SIZE                  2048        /* default buffer size, 2K bytes */

/*
 * To complete shield the ANSC modules from having to understand the underlying IPC mechanism, we
 * use the following data structure to encapsulate the 'address' of the IPC parties. Note that this
 * structure serves as a virtual data type, which should be derived to reflect the IPC address.
 */
#define  ANSC_LPC_PARTY_ADDR_CONTENT                                                        \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ULONG                           StructSize;                                             \
    ULONG                           RefCount;                                               \
    ULONG                           ActiveCalls;                                            \
    ULONG                           Timestamp;                                              \
    ULONG                           PartyState;                                             \
    ULONG                           PartyType;                                              \
    char                            PartyName[ANSC_OBJECT_NAME_SIZE];                       \
    ANSC_IPV4_ADDRESS               PartyAddr;                                              \
    UCHAR                           PartyProtocol;                                          \
    USHORT                          PartyPort;                                              \
    ANSC_HANDLE                     PartySocket;                                            \
    ANSC_HANDLE                     PartyReserved;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_LPC_PARTY_ADDR
{
    ANSC_LPC_PARTY_ADDR_CONTENT
}
ANSC_LPC_PARTY_ADDR,  *PANSC_LPC_PARTY_ADDR;

#define  ACCESS_ANSC_LPC_PARTY_ADDR(p)              \
         ACCESS_CONTAINER(p, ANSC_LPC_PARTY_ADDR, Linkage)


#endif
