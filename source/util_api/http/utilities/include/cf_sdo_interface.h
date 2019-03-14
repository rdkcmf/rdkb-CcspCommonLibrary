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

    module:	cf_sdo_interface.h

        For Server Detection Object (SDO),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Server Detection Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/26/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_SDO_INTERFACE_DEC
#define  _CF_SDO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_SDO_COMPONENT_NAME                      "CfSdoComponent"
#define  CF_SDO_COMPONENT_OID                       ANSC_OBJECT_OID_NULL
#define  CF_SDO_COMPONENT_TYPE                      ANSC_OBJECT_TYPE_NULL

#define  CF_SDO_SERVER_TYPE_BASE                    0
#define  CF_SDO_SERVER_TYPE_UNKNOWN                 CF_SDO_SERVER_TYPE_BASE
#define  CF_SDO_SERVER_TYPE_TCP                     CF_SDO_SERVER_TYPE_BASE + 1
#define  CF_SDO_SERVER_TYPE_UDP                     CF_SDO_SERVER_TYPE_BASE + 2

/*
 * The first 4 bytes (Big endian) is used to indicate the
 * length of message body itself (the 4 bytes of the length
 * of indicator is not included).
 *
 * In this version, we specify a 10 bytes message used for
 * doing server detection. The message body is "1357924680"
 */
const static CHAR                   g_msgDetection[] = 
    {0x00, 0x00, 0x00, 0x0A, '1', '3', '5', '7', '9', '2', '4', '6', '8', '0'};

const static                        g_msgDetectionLen = sizeof(g_msgDetection)/sizeof(g_msgDetection[0]);


#define  CF_SDO_SERVER_ADDRESS_LENGTH               5
#define  CF_SDO_SERVER_DETECTION_INTERVAL           600                     /* 600 seconds */
#define  CF_SDO_SERVER_DETECTION_NAP_INTERVAL       100                     /* 500 milliseconds */
#define  CF_SDO_SERVER_DETECTION_RETRY_TIMES        3                       

typedef  struct
_CF_SDO_SERVER_INFO
{
    UCHAR                           Address[CF_SDO_SERVER_ADDRESS_LENGTH];  /* IP address */
    USHORT                          Port;                                   /* port number */
    ULONG                           ulStartTime;                            /* start time of testing in seconds */
    BOOL                            bAvailable;                             /* available or not */
    ULONG                           ulRRTime;                               /* Round Trip Time in seconds */
    BOOL                            bTesting;                               /* on testing */
    ULONG                           ulLastCheckTime;                        /* last checking time */
    ULONG                           ulLastFailTime;                         /* last failure time */
    /* some statistical data */
    ULONG                           ulSuccTimes;                            /* success times */
    ULONG                           ulSuccTimesScale;                       /* scale */
    ULONG                           ulFailTimes;                            /* times of filaures */
    ULONG                           ulFailTimesScale;                       /* scale */
}
CF_SDO_SERVER_INFO, *PCF_SDO_SERVER_INFO;


typedef
void
(*PFN_CF_SDO_SET_SERVER_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerType
    );

typedef
void
(*PFN_CF_SDO_SET_TIMEOUT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimeout
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_CREATE_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_CREATE_CLIENT_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_SDO_REMOVE_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_CREATE_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_SDO_REMOVE_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_START_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_STOP_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_ADD_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pServerAddress,
        USHORT                      usServerPort
    );

typedef
void
(*PFN_CF_SDO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_SDO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_SDO_GET_FASTEST_SERVER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_SDO_SET_SERVER_RRTIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex,
        ULONG                       ulRRTime
    );

typedef
void
(*PFN_CF_SDO_SET_SERVER_AVAILABLE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex,
        BOOL                        bAvailable
    );

typedef
void
(*PFN_CF_SDO_SET_SERVER_ONTESTING)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex
    );

typedef
ANSC_HANDLE
(*PFN_CF_SDO_GET_EMERGENT_SERVER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_SDO_ENGAGE_CLIENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo              
    );

typedef
ULONG
(*PFN_CF_SDO_GET_TESTING_SERVER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_SDO_GET_START_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex
    );

typedef
void
(*PFN_CF_SDO_SET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex,
        ULONG                       ulLastTime
    );

typedef
ULONG
(*PFN_CF_SDO_GET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex
    );

typedef
void
(*PFN_CF_SDO_SET_LAST_FAIL_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex,
        ULONG                       ulLastFailureTime
    );

typedef
ULONG
(*PFN_CF_SDO_GET_LAST_FAIL_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulServerIndex
    );

typedef
void
(*PFN_CF_SDO_WAKE_UP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_SDO_SET_LAST_ROUND_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulLastRoundTime
    );

typedef
BOOL
(*PFN_CF_SDO_IS_NEW_ROUND_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_SDO_GET_SERVER_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_SERVER_DETECTION_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ULONG                           ServerType;                                         \
    ANSC_HANDLE                     hClient;                                            \
    ANSC_HANDLE                     hClientWorker;                                      \
    /* A pointer array which contains servers (_CF_SDO_SERVER_INFO) */                  \
    ANSC_HANDLE                     hServerArray;                                       \
    ANSC_LOCK                       ServerArrayLock;                                    \
    ULONG                           ulCurServerIndex;                                   \
    ULONG                           ulFastestServerIndex;                               \
    /* Timer Descriptor Object */                                                       \
    ANSC_HANDLE                     hTdo;                                               \
    /* Timer Descriptor Client Object */                                                \
    ANSC_HANDLE                     hTdoClient;                                         \
    ULONG                           ulTimeout;                                          \
    ULONG                           ulTimeoutMS;                                        \
    BOOL                            bStarted;                                           \
    ANSC_EVENT                      ReqTerminateEvent;                                  \
    BOOL                            bSleeping;                                          \
    ULONG                           ulLastRoundTime;                                    \
    BOOL                            bTaskExited;                                        \
    /* start of methods */                                                              \
    PFN_CF_SDO_SET_SERVER_TYPE      SetServerType;                                      \
    PFN_CF_SDO_SET_TIMEOUT          SetTimeout;                                         \
    PFN_CF_SDO_CREATE_CLIENT        CreateClient;                                       \
    PFN_CF_SDO_CREATE_CLIENT_WORKER CreateClientWorker;                                 \
    PFN_CF_SDO_ENGAGE_CLIENT        EngageClient;                                       \
    PFN_CF_SDO_REMOVE_CLIENT        RemoveClient;                                       \
    PFN_CF_SDO_CREATE_TIMER         CreateTimer;                                        \
    PFN_CF_SDO_REMOVE_TIMER         RemoveTimer;                                        \
    PFN_CF_SDO_START_TIMER          StartTimer;                                         \
    PFN_CF_SDO_STOP_TIMER           StopTimer;                                          \
    PFN_CF_SDO_ADD_SERVER           AddServer;                                          \
    PFN_CF_SDO_START                Start;                                              \
    PFN_CF_SDO_STOP                 Stop;                                               \
    PFN_CF_SDO_GET_FASTEST_SERVER   GetFasteseServer;                                   \
    PFN_CF_SDO_SET_SERVER_RRTIME    SetServerRRTime;                                    \
    PFN_CF_SDO_SET_SERVER_AVAILABLE SetServerAvailable;                                 \
    PFN_CF_SDO_SET_SERVER_ONTESTING SetServerOnTesting;                                 \
    PFN_CF_SDO_GET_EMERGENT_SERVER  GetEmergentServer;                                  \
    PFN_CF_SDO_CREATE_CLIENT        CreateUdpClient;                                    \
    PFN_CF_SDO_CREATE_CLIENT_WORKER CreateUdpClientWorker;                              \
    PFN_CF_SDO_ENGAGE_CLIENT        EngageUdpClient;                                    \
    PFN_CF_SDO_CREATE_CLIENT        CreateTcpClient;                                    \
    PFN_CF_SDO_CREATE_CLIENT_WORKER CreateTcpClientWorker;                              \
    PFN_CF_SDO_ENGAGE_CLIENT        EngageTcpClient;                                    \
    PFN_CF_SDO_GET_TESTING_SERVER   GetTestingServer;                                   \
    PFN_CF_SDO_GET_START_TIME       GetServerStartTime;                                 \
    PFN_CF_SDO_SET_LAST_TIME        SetServerLastTime;                                  \
    PFN_CF_SDO_GET_LAST_TIME        GetServerLastTime;                                  \
    PFN_CF_SDO_SET_LAST_FAIL_TIME   SetLastFailureTime;                                 \
    PFN_CF_SDO_GET_LAST_FAIL_TIME   GetLastFailureTime;                                 \
    PFN_CF_SDO_WAKE_UP              WakeUp;                                             \
    PFN_CF_SDO_SET_LAST_ROUND_TIME  SetLastRoundTime;                                   \
    PFN_CF_SDO_IS_NEW_ROUND_TIME    IsNewRoundTime;                                     \
    PFN_CF_SDO_GET_SERVER_COUNT     GetServerCount;                                     \


/* Filtering Message Management Object (FMMO) */
typedef  struct
_CF_SDO_OBJECT
{
    CF_SERVER_DETECTION_CLASS_CONTENT
}
CF_SDO_OBJECT,    *PCF_SDO_OBJECT;


#define  ACCESS_CF_SDO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_SDO_OBJECT, Linkage)

#endif

