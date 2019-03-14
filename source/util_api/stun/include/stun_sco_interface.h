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

    module:	stun_sco_interface.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Stun Simple Client Objects.

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


#ifndef  _STUN_CSO_INTERFACE_
#define  _STUN_CSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_scuowo_interface.h"
#include "stun_properties.h"

#include "stun_ifo_bsm.h"


/***********************************************************
       STUN SIMPLE CLIENT COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  STUN_SCO_CLIENT_STATE_Idle                 0
#define  STUN_SCO_CLIENT_STATE_Discovering          1
#define  STUN_SCO_CLIENT_STATE_Maintaining          2
#define  STUN_SCO_CLIENT_STATE_Notifying            3

#define  STUN_SCO_CLIENT_STAGE_None                 0
#define  STUN_SCO_CLIENT_STAGE_Discovering          1
#define  STUN_SCO_CLIENT_STAGE_Maintaining          2

#define  STUN_SCO_SOCKET_STATE_Idle                 0
#define  STUN_SCO_SOCKET_STATE_Requesting           1

#define  STUN_SCO_STAGE_TIMER_INTERVAL              10 * 1000   /* in milliseconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_STUNSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_STUNSCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  char*
(*PFN_STUNSCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_STUNSCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_STUNSCO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAddr
    );

typedef  USHORT
(*PFN_STUNSCO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  int
(*PFN_STUNSCO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        int                         iSeconds
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SETUP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  BOOL
(*PFN_STUNSCO_VERIFY)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_STUNSCO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

/*
 * The STUN client is typically embedded in an application which needs to obtain a public IP
 * address and port that can be used to receive data. For example, it might need to obtain an IP
 * address and port to receive Real Time Transport Protocol (RTP) traffic.
 */
#define  STUN_SIMPLE_CLIENT_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    STUN_SIMPLE_CLIENT_PROPERTY     Property;                                               \
    STUN_BINDING_INFO               BindingInfo;                                            \
    STUN_BINDING_INFO               BindingInfoA;                                           \
    ANSC_HANDLE                     hStageTimerObj;                                         \
    ANSC_HANDLE                     hStageTimerIf;                                          \
    ANSC_HANDLE                     hRetryTimerObj;                                         \
    ANSC_HANDLE                     hRetryTimerIf;                                          \
    ANSC_HANDLE                     hProbeTimerObj;                                         \
    ANSC_HANDLE                     hProbeTimerIf;                                          \
    ANSC_HANDLE                     hSimpleClientUdp1;                                      \
    ANSC_HANDLE                     hClientUdpWorker1;                                      \
    ANSC_HANDLE                     hSimpleClientUdpA;                                      \
    ANSC_HANDLE                     hClientUdpWorkerA;                                      \
    ANSC_HANDLE                     hSimpleClientUdpB;                                      \
    ANSC_HANDLE                     hClientUdpWorkerB;                                      \
    ANSC_HANDLE                     hStunBsmIf;                                             \
    ULONG                           AsyncTaskCount;                                         \
    BOOL                            bActive;                                                \
                                                                                            \
    ULONG                           ClientStage;                                            \
    ULONG                           ClientState;                                            \
    ULONG                           SocketState1;                                           \
    ULONG                           SocketStateA;                                           \
    ULONG                           SocketStateB;                                           \
    ULONG                           RetransInterval1;                                       \
    ULONG                           RetransTime1;                                           \
    void*                           LastSendMsg1;                                           \
    ULONG                           LastMsgSize1;                                           \
    BOOL                            bMsgIntegrity1;                                         \
    BOOL                            bMsgIntegrityA;                                         \
    BOOL                            bMsgIntegrityB;                                         \
    UCHAR                           TransID1[16];                                           \
    UCHAR                           TransIDA[16];                                           \
    UCHAR                           TransIDB[16];                                           \
    ANSC_EVENT                      SocketEventA;                                           \
                                                                                            \
    PFN_STUNSCO_GET_IF              GetStunBsmIf;                                           \
    PFN_STUNSCO_SET_IF              SetStunBsmIf;                                           \
    PFN_STUNSCO_GET_ADDR            GetClientAddr;                                          \
    PFN_STUNSCO_SET_ADDR            SetClientAddr;                                          \
    PFN_STUNSCO_GET_PORT            GetClientPort;                                          \
    PFN_STUNSCO_SET_PORT            SetClientPort;                                          \
    PFN_STUNSCO_GET_MODE            GetClientMode;                                          \
    PFN_STUNSCO_SET_MODE            SetClientMode;                                          \
    PFN_STUNSCO_GET_ADDR            GetServerAddr;                                          \
    PFN_STUNSCO_SET_ADDR            SetServerAddr;                                          \
    PFN_STUNSCO_GET_PORT            GetServerPort;                                          \
    PFN_STUNSCO_SET_PORT            SetServerPort;                                          \
    PFN_STUNSCO_GET_NAME            GetUsername;                                            \
    PFN_STUNSCO_SET_NAME            SetUsername;                                            \
    PFN_STUNSCO_GET_NAME            GetPassword;                                            \
    PFN_STUNSCO_SET_NAME            SetPassword;                                            \
    PFN_STUNSCO_GET_TIME            GetMinKeepAliveInterval;                                \
    PFN_STUNSCO_SET_TIME            SetMinKeepAliveInterval;                                \
    PFN_STUNSCO_GET_TIME            GetMaxKeepAliveInterval;                                \
    PFN_STUNSCO_SET_TIME            SetMaxKeepAliveInterval;                                \
                                                                                            \
    PFN_STUNSCO_GET_PROPERTY        GetProperty;                                            \
    PFN_STUNSCO_SET_PROPERTY        SetProperty;                                            \
    PFN_STUNSCO_RESET               ResetProperty;                                          \
    PFN_STUNSCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_STUNSCO_ENGAGE              Engage;                                                 \
    PFN_STUNSCO_CANCEL              Cancel;                                                 \
    PFN_STUNSCO_INVOKE              StageTimerInvoke;                                       \
    PFN_STUNSCO_INVOKE              RetryTimerInvoke;                                       \
    PFN_STUNSCO_INVOKE              ProbeTimerInvoke;                                       \
                                                                                            \
    PFN_STUNSCO_ACTION              PlayRole1;                                              \
    PFN_STUNSCO_ACTION              DiscoverBinding;                                        \
    PFN_STUNSCO_ACTION              DiscoverBindingTimeoutA;                                \
    PFN_STUNSCO_ACTION              DiscoverBindingTimeoutB;                                \
    PFN_STUNSCO_ACTION              MaintainBinding;                                        \
    PFN_STUNSCO_ACTION              NotifyBindingChange;                                    \
    PFN_STUNSCO_ACTION              AsyncDiscoverTask;                                      \
                                                                                            \
    PFN_STUNSCO_RECV                RecvBindingResponse1;                                   \
    PFN_STUNSCO_RECV                RecvBindingResponseA;                                   \
    PFN_STUNSCO_RECV                RecvBindingResponseB;                                   \
    PFN_STUNSCO_RECV                RecvBindingErrorResponse1;                              \
    PFN_STUNSCO_RECV                RecvBindingErrorResponseA;                              \
    PFN_STUNSCO_RECV                RecvBindingErrorResponseB;                              \
    PFN_STUNSCO_VERIFY              VerifyMsgIntegrity;                                     \
                                                                                            \
    PFN_STUNSCO_SEND                SendBindingRequest1;                                    \
    PFN_STUNSCO_SEND                SendBindingRequestA;                                    \
    PFN_STUNSCO_SEND                SendBindingRequestB;                                    \
                                                                                            \
    PFN_SCUOWO_QUERY                Scuowo1Query;                                           \
    PFN_SCUOWO_PROCESS              Scuowo1Process;                                         \
    PFN_SCUOWO_NOTIFY               Scuowo1Notify;                                          \
    PFN_SCUOWO_QUERY                ScuowoAQuery;                                           \
    PFN_SCUOWO_PROCESS              ScuowoAProcess;                                         \
    PFN_SCUOWO_NOTIFY               ScuowoANotify;                                          \
    PFN_SCUOWO_QUERY                ScuowoBQuery;                                           \
    PFN_SCUOWO_PROCESS              ScuowoBProcess;                                         \
    PFN_SCUOWO_NOTIFY               ScuowoBNotify;                                          \
    /* end of object class content */                                                       \

typedef  struct
_STUN_SIMPLE_CLIENT_OBJECT
{
    STUN_SIMPLE_CLIENT_CLASS_CONTENT
}
STUN_SIMPLE_CLIENT_OBJECT,  *PSTUN_SIMPLE_CLIENT_OBJECT;

#define  ACCESS_STUN_SIMPLE_CLIENT_OBJECT(p)        \
         ACCESS_CONTAINER(p, STUN_SIMPLE_CLIENT_OBJECT, Linkage)


#endif
