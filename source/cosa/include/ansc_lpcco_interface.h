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

    module:	ansc_lpcco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Lpc Connector Object.

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


#ifndef  _ANSC_LPCCO_INTERFACE_
#define  _ANSC_LPCCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_ifo_imc.h"
#include "ansc_ifo_cpc.h"


/***********************************************************
    PLATFORM INDEPENDENT LPC CONNECTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_LPCCO_PARTY_TABLE_SIZE                16
#define  ANSC_LPCCO_DEF_CONN_TIMEOUT                3000    /* 3 seconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_LPCCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_LPCCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_LPCCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOpMode
    );

typedef  ULONG
(*PFN_LPCCO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_LPCCO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_IPCCO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_GET_ARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE*                pPartyArray,
        PULONG                      pulCount
    );

typedef  ANSC_HANDLE
(*PFN_IPCCO_GET_PARTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

typedef  ANSC_HANDLE
(*PFN_IPCCO_GET_PARTY2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_ADD_PARTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 party_addr
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_DEL_PARTY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_IPCCO_GET_PCALL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    );

typedef  ANSC_HANDLE
(*PFN_IPCCO_POP_PCALL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        ULONG                       msg_type,
        ULONG                       seq_number,
        ULONG                       error_code,
        ANSC_HANDLE                 output_bdo
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_ADD_PCALL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPendingCall
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_DEL_PCALL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    );

typedef  ANSC_STATUS
(*PFN_IPCCO_POP_ALL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       error_code
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_ENROLL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_UNBIND)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_LPCCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Ansc Lpc Connector object implements the IMC interface by encapsulating the LPC calls as TCP
 * messages and transmit them over TCP socket. Such mechanism applies to most of the communication
 * scenarios on desktop operating systems, except the interaction between a User-mode module and a
 * Kernel-mode module, which requires some special consideration.
 */
#define  ANSC_LPC_CONNECTOR_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAnscImcIf;                                             \
    ANSC_HANDLE                     hAnscCpcIf;                                             \
    ANSC_HANDLE                     hConnTimerObj;                                          \
    ANSC_HANDLE                     hConnTimerIf;                                           \
    ULONG                           LpcOpmode;                                              \
    char*                           PartyName;                                              \
    ULONG                           SeqNumber;                                              \
    ULONG                           CallTimeout;                                            \
    ANSC_LOCK                       AccessLock;                                             \
    BOOL                            bConnected;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    PartyTable[ANSC_LPCCO_PARTY_TABLE_SIZE];                \
    ANSC_LOCK                       PartyTableLock;                                         \
    QUEUE_HEADER                    PcallQueue;                                             \
    ANSC_LOCK                       PcallQueueLock;                                         \
                                                                                            \
    PFN_LPCCO_GET_IF                GetAnscImcIf;                                           \
    PFN_LPCCO_GET_IF                GetAnscCpcIf;                                           \
    PFN_LPCCO_SET_IF                SetAnscCpcIf;                                           \
    PFN_LPCCO_GET_SIZE              GetPartyAddrSize;                                       \
    PFN_LPCCO_GET_TIME              GetCallTimeout;                                         \
    PFN_LPCCO_SET_TIME              SetCallTimeout;                                         \
    PFN_LPCCO_RESET                 Reset;                                                  \
                                                                                            \
    PFN_IPCCO_GET_COUNT             GetPartyCount;                                          \
    PFN_IPCCO_GET_ARRAY             GetPartyArray;                                          \
    PFN_IPCCO_GET_PARTY             GetPartyAddr;                                           \
    PFN_IPCCO_GET_PARTY2            GetPartyAddr2;                                          \
    PFN_IPCCO_ADD_PARTY             AddPartyAddr;                                           \
    PFN_IPCCO_DEL_PARTY             DelPartyAddr;                                           \
    PFN_IPCCO_DEL_ALL               DelAllPartyAddrs;                                       \
                                                                                            \
    PFN_IPCCO_GET_PCALL             GetPendingCall;                                         \
    PFN_IPCCO_POP_PCALL             PopPendingCall;                                         \
    PFN_IPCCO_ADD_PCALL             AddPendingCall;                                         \
    PFN_IPCCO_DEL_PCALL             DelPendingCall;                                         \
    PFN_IPCCO_POP_ALL               PopAllPendingCalls;                                     \
    PFN_IPCCO_DEL_ALL               DelAllPendingCalls;                                     \
                                                                                            \
    PFN_LPCCO_ACQ_ACCESS            AcqAccess;                                              \
    PFN_LPCCO_REL_ACCESS            RelAccess;                                              \
    PFN_LPCCO_ENGAGE                Engage;                                                 \
    PFN_LPCCO_CANCEL                Cancel;                                                 \
    PFN_LPCCO_ENROLL                Enroll;                                                 \
    PFN_LPCCO_UNBIND                Unbind;                                                 \
    PFN_LPCCO_CONNECT               ConnectToManager;                                       \
    PFN_LPCCO_INVOKE                ConnTimerInvoke;                                        \
                                                                                            \
    PFN_ANSCIMCIF_GEN_ADDR          ImcGenPartyAddr;                                        \
    PFN_ANSCIMCIF_GET_MODE          ImcGetLpcOpmode;                                        \
    PFN_ANSCIMCIF_SET_MODE          ImcSetLpcOpmode;                                        \
    PFN_ANSCIMCIF_GET_NAME          ImcGetPartyName;                                        \
    PFN_ANSCIMCIF_SET_NAME          ImcSetPartyName;                                        \
    PFN_ANSCIMCIF_IS_CONNECTED      ImcIsPartyConnected;                                    \
                                                                                            \
    PFN_ANSCIMCIF_ENGAGE            ImcEngage;                                              \
    PFN_ANSCIMCIF_CANCEL            ImcCancel;                                              \
    PFN_ANSCIMCIF_CALL              ImcCallRequest;                                         \
    PFN_ANSCIMCIF_CALL2             ImcCallRequest2;                                        \
                                                                                            \
    PFN_ANSCCPCIF_DISPATCH          CpcCallDispatch;                                        \
    PFN_ANSCCPCIF_FREE              CpcFreeOutputBuffer;                                    \
    PFN_ANSCCPCIF_NOTIFY            CpcNotifyEvent;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_LPC_CONNECTOR_OBJECT
{
    ANSC_LPC_CONNECTOR_CLASS_CONTENT
}
ANSC_LPC_CONNECTOR_OBJECT,  *PANSC_LPC_CONNECTOR_OBJECT;

#define  ACCESS_ANSC_LPC_CONNECTOR_OBJECT(p)        \
         ACCESS_CONTAINER(p, ANSC_LPC_CONNECTOR_OBJECT, Linkage)


/***********************************************************
        LPC PARTY PENDING LPC CALL OBJECT DEFINITION
***********************************************************/

/*
 * Each LPC call initiated by a LPC party is uniquely identified by a sequence number. This number
 * begins with one, and increase by one every time a call is made. After making a call into the
 * LPC Connector, the calling task/thread is blocked on the 'call event'. When the corresponding
 * REPLY message is received, the 'call event' will be signalled.
 */
#define  ANSC_LPC_CALL_STATE_initialized            1
#define  ANSC_LPC_CALL_STATE_resolving              2
#define  ANSC_LPC_CALL_STATE_connecting             3
#define  ANSC_LPC_CALL_STATE_calling                4

typedef  struct
_ANSC_LPCCO_PENDING_CALL
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           PartyName;
    ULONG                           ImcpMsgType;
    ULONG                           CallSeqNumber;
    ULONG                           CallState;
    ULONG                           CallError;
    ANSC_EVENT*                     CallEvent;
    ANSC_HANDLE                     CallOutputBdo;
}
ANSC_LPCCO_PENDING_CALL,  *PANSC_LPCCO_PENDING_CALL;

#define  ACCESS_ANSC_LPCCO_PENDING_CALL(p)          \
         ACCESS_CONTAINER(p, ANSC_LPCCO_PENDING_CALL, Linkage)


#endif
