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

    module:	ansc_ipc_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc IPC Port Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_IPC_INTERFACE_
#define  _ANSC_IPC_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ipc_transmitter_interface.h"


/***********************************************************
       PLATFORM INDEPENDENT IPC PORT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IPO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_IPO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_IPO_GET_PORT_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_SET_PORT_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_IPO_GET_PID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_SET_PID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       pid
    );

typedef  ANSC_HANDLE
(*PFN_IPO_GET_TRANSMITTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_SET_TRANSMITTER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTransmitter
    );

typedef  ANSC_HANDLE
(*PFN_IPO_GET_RECEIVER_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_IPO_GET_RECEIVER_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_STATUS
(*PFN_IPO_ADD_RECEIVER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReceiver
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_RECEIVER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReceiver
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_ALL_RECEIVERS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_ADD_MONITOR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMonitor
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_MONITOR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMonitor
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_ALL_MONITORS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPO_CALL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       dstPortName,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn
    );

typedef  ANSC_STATUS
(*PFN_IPO_SERV)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       srcPortName,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn
    );

typedef  ANSC_STATUS
(*PFN_IPO_WARN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       srcPortName,
        ANSC_HANDLE                 hStatus
    );

typedef  ANSC_STATUS
(*PFN_IPO_CALL_ASYNC)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       dstPortName,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn,
        PFN_ANSC_ASYNC_NOTIFY       pfnAsyncNotify,
        ANSC_HANDLE                 hAsyncContext
    );

typedef  ANSC_HANDLE
(*PFN_IPO_GET_PATH_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_IPO_GET_PATH_BYPID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       pid
    );

typedef  ANSC_HANDLE
(*PFN_IPO_ADD_PATH_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_IPO_ADD_PATH_BYPID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       pid
    );

typedef  ANSC_STATUS
(*PFN_IPO_ADD_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPath
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPath
    );

typedef  ANSC_STATUS
(*PFN_IPO_DEL_ALL_PATHS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Although the object name suggests that this object is for Internet Process Communication (IPC),
 * the port communications may simply be directly function call in some cases depending on the
 * architecture of the underlying platform. For instance, on most real-time operating systems, if
 * the Aps package is deployed on the same master CPU, the communication between the Sce and the
 * Aps is straightforward.
 */
#define  ANSC_IPC_PORT_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            PortName[ANSC_OBJECT_NAME_SIZE];                        \
    ULONG                           Pid;                                                    \
    ANSC_HANDLE                     hTransmitter;                                           \
    ULONG                           SeqNumber;                                              \
                                                                                            \
    QUEUE_HEADER                    ReceiverQueue;                                          \
    ANSC_LOCK                       ReceiverQueueLock;                                      \
    QUEUE_HEADER                    MonitorQueue;                                           \
    ANSC_LOCK                       MonitorQueueLock;                                       \
                                                                                            \
    PFN_IPO_CREATE                  Create;                                                 \
    PFN_IPO_REMOVE                  Remove;                                                 \
    PFN_IPO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_IPO_GET_PORT_NAME           GetPortName;                                            \
    PFN_IPO_SET_PORT_NAME           SetPortName;                                            \
    PFN_IPO_GET_PID                 GetPid;                                                 \
    PFN_IPO_SET_PID                 SetPid;                                                 \
    PFN_IPO_GET_TRANSMITTER         GetTransmitter;                                         \
    PFN_IPO_SET_TRANSMITTER         SetTransmitter;                                         \
                                                                                            \
    PFN_IPO_GET_RECEIVER_BYNAME     GetReceiverByName;                                      \
    PFN_IPO_GET_RECEIVER_BYID       GetReceiverById;                                        \
    PFN_IPO_ADD_RECEIVER            AddReceiver;                                            \
    PFN_IPO_DEL_RECEIVER            DelReceiver;                                            \
    PFN_IPO_DEL_ALL_RECEIVERS       DelAllReceivers;                                        \
                                                                                            \
    PFN_IPO_ADD_MONITOR             AddMonitor;                                             \
    PFN_IPO_DEL_MONITOR             DelMonitor;                                             \
    PFN_IPO_DEL_ALL_MONITORS        DelAllMonitors;                                         \
                                                                                            \
    PFN_IPO_CALL                    Call;                                                   \
    PFN_IPO_SERV                    Serv;                                                   \
    PFN_IPO_WARN                    Warn;                                                   \
    PFN_IPO_CALL_ASYNC              CallAsync;                                              \
                                                                                            \
    SLIST_HEADER                    PathList;                                               \
    ANSC_LOCK                       PathListLock;                                           \
                                                                                            \
    PFN_IPO_GET_PATH_BYNAME         GetPathByDstPortName;                                   \
    PFN_IPO_GET_PATH_BYPID          GetPathByDstPid;                                        \
    PFN_IPO_ADD_PATH_BYNAME         AddPathByDstPortName;                                   \
    PFN_IPO_ADD_PATH_BYPID          AddPathByDstPid;                                        \
                                                                                            \
    PFN_IPO_ADD_PATH                AddPath;                                                \
    PFN_IPO_DEL_PATH                DelPath;                                                \
    PFN_IPO_DEL_ALL_PATHS           DelAllPaths;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_PORT_OBJECT
{
    ANSC_IPC_PORT_CLASS_CONTENT
}
ANSC_IPC_PORT_OBJECT,  *PANSC_IPC_PORT_OBJECT;

#define  ACCESS_ANSC_IPC_PORT_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_IPC_PORT_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT IPC PORT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IPP_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_IPP_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_IPP_GET_PORT_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_SET_PORT_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_IPP_GET_PID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_SET_PID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       pid
    );

typedef  ANSC_HANDLE
(*PFN_IPP_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPort
    );

typedef  ANSC_HANDLE
(*PFN_IPP_GET_TRANSMITTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_SET_TRANSMITTER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTransmitter
    );

typedef  ULONG
(*PFN_IPP_GET_STATUS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IPP_SET_STATUS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       status
    );

typedef  ANSC_STATUS
(*PFN_IPP_CALL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn
    );

typedef  ANSC_STATUS
(*PFN_IPP_CALL_ASYNC)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn,
        PFN_ANSC_ASYNC_NOTIFY       pfnAsyncNotify,
        ANSC_HANDLE                 hAsyncContext
    );

/*
 * Although the object name suggests that this object is for Internet Process Communication (IPC),
 * the port communications may simply be directly function call in some cases depending on the
 * architecture of the underlying platform. For instance, on most real-time operating systems, if
 * the Aps package is deployed on the same master CPU, the communication between the Sce and the
 * Aps is straightforward.
 */
#define  ANSC_IPC_PATH_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            SrcPortName[ANSC_OBJECT_NAME_SIZE];                     \
    char                            DstPortName[ANSC_OBJECT_NAME_SIZE];                     \
    ULONG                           SrcPid;                                                 \
    ULONG                           DstPid;                                                 \
    ANSC_HANDLE                     hSrcPort;                                               \
    ANSC_HANDLE                     hDstPort;                                               \
    ANSC_HANDLE                     hTransmitter;                                           \
    ULONG                           Status;                                                 \
                                                                                            \
    PFN_IPP_CREATE                  Create;                                                 \
    PFN_IPP_REMOVE                  Remove;                                                 \
    PFN_IPP_RESET                   Reset;                                                  \
                                                                                            \
    PFN_IPP_GET_PORT_NAME           GetSrcPortName;                                         \
    PFN_IPP_SET_PORT_NAME           SetSrcPortName;                                         \
    PFN_IPP_GET_PORT_NAME           GetDstPortName;                                         \
    PFN_IPP_SET_PORT_NAME           SetDstPortName;                                         \
    PFN_IPP_GET_PID                 GetSrcPid;                                              \
    PFN_IPP_SET_PID                 SetSrcPid;                                              \
    PFN_IPP_GET_PID                 GetDstPid;                                              \
    PFN_IPP_SET_PID                 SetDstPid;                                              \
    PFN_IPP_GET_PORT                GetSrcPort;                                             \
    PFN_IPP_SET_PORT                SetSrcPort;                                             \
    PFN_IPP_GET_PORT                GetDstPort;                                             \
    PFN_IPP_SET_PORT                SetDstPort;                                             \
    PFN_IPP_GET_TRANSMITTER         GetTransmitter;                                         \
    PFN_IPP_SET_TRANSMITTER         SetTransmitter;                                         \
    PFN_IPP_GET_STATUS              GetStatus;                                              \
    PFN_IPP_SET_STATUS              SetStatus;                                              \
                                                                                            \
    PFN_IPP_CALL                    Call;                                                   \
    PFN_IPP_CALL_ASYNC              CallAsync;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_PATH_OBJECT
{
    ANSC_IPC_PATH_CLASS_CONTENT
}
ANSC_IPC_PATH_OBJECT,  *PANSC_IPC_PATH_OBJECT;

#define  ACCESS_ANSC_IPC_PATH_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_IPC_PATH_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT IPC RECEIVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IRO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_IRO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IRO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_IRO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IRO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_IRO_GET_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IRO_SET_ID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_STATUS
(*PFN_IRO_SERV)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       srcPortName,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn
    );

/*
 * Although the object name suggests that this object is for Internet Process Communication (IPC),
 * the port communications may simply be directly function call in some cases depending on the
 * architecture of the underlying platform. For instance, on most real-time operating systems, if
 * the Aps package is deployed on the same master CPU, the communication between the Sce and the
 * Aps is straightforward.
 */
#define  ANSC_IPC_RECEIVER_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    char                            ReceiverName[ANSC_OBJECT_NAME_SIZE];                    \
    ULONG                           ReceiverId;                                             \
                                                                                            \
    PFN_IRO_CREATE                  Create;                                                 \
    PFN_IRO_REMOVE                  Remove;                                                 \
    PFN_IRO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_IRO_GET_NAME                GetName;                                                \
    PFN_IRO_SET_NAME                SetName;                                                \
    PFN_IRO_GET_ID                  GetId;                                                  \
    PFN_IRO_SET_ID                  SetId;                                                  \
    PFN_IRO_SERV                    Serv;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_RECEIVER_OBJECT
{
    ANSC_IPC_RECEIVER_CLASS_CONTENT
}
ANSC_IPC_RECEIVER_OBJECT,  *PANSC_IPC_RECEIVER_OBJECT;

#define  ACCESS_ANSC_IPC_RECEIVER_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_IPC_RECEIVER_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT IPC MONITOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_IMO_STATUS_MASK_UP                    0x00000001
#define  ANSC_IMO_STATUS_MASK_DOWN                  0x00000002
#define  ANSC_IMO_STATUS_MASK_ALL                   0xFFFFFFFF

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IMO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_IMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_IMO_GET_MASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IMO_SET_MASK)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mask
    );

typedef  ANSC_STATUS
(*PFN_IMO_WARN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       srcPortName,
        ANSC_HANDLE                 hStatus
    );

/*
 * Originally any object who wants to monitor the status of an Ipc connection has to create an Ipc
 * Receiver Object even it's actually not involved in any real Ipc transactions. Due to the large
 * number of Ipc Receiver Objects, a status-change notification may take quite some time to be
 * populated through the entire module. By separating the functionality of an Ipc Receiver Object
 * and an Ipc Monitor Object, we have a better and cleaner Ipc structure and better performance.
 */
#define  ANSC_IPC_MONITOR_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ULONG                           StatusMask;                                             \
                                                                                            \
    PFN_IMO_CREATE                  Create;                                                 \
    PFN_IMO_REMOVE                  Remove;                                                 \
    PFN_IMO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_IMO_GET_MASK                GetStatusMask;                                          \
    PFN_IMO_SET_MASK                SetStatusMask;                                          \
    PFN_IMO_WARN                    Warn;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_MONITOR_OBJECT
{
    ANSC_IPC_MONITOR_CLASS_CONTENT
}
ANSC_IPC_MONITOR_OBJECT,  *PANSC_IPC_MONITOR_OBJECT;

#define  ACCESS_ANSC_IPC_MONITOR_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_IPC_MONITOR_OBJECT, Linkage)


#endif
