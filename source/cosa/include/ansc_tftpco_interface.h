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

    module:	ansc_tftpco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TFTP Client Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/16/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_TFTPCO_INTERFACE_
#define  _ANSC_TFTPCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "ansc_ifo_ftc.h"
#include "ansc_scuowo_interface.h"


/***********************************************************
     PLATFORM INDEPENDENT TFTP CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_TFTPCO_MAX_FILENAME_SIZE              128
#define  ANSC_TFTPCO_MAX_MESSAGE_SIZE               640
#define  ANSC_TFTPCO_DEF_CONN_TIMEOUT               5000    /* 5 seconds, in milli-seconds     */
#define  ANSC_TFTPCO_MAX_RETRY_NUMBER               3       /* we try 3 times before giving up */

#define  ANSC_TFTPCO_STATE_INITIALIZED              1
#define  ANSC_TFTPCO_STATE_REQ_DELAYED              2
#define  ANSC_TFTPCO_STATE_REQ_SENT                 3
#define  ANSC_TFTPCO_STATE_ESTABLISHED              4
#define  ANSC_TFTPCO_STATE_IN_PROGRESS              5
#define  ANSC_TFTPCO_STATE_TERMINATED               6

#define  ANSC_TFTPCO_RMODE_UNKNOWN                  0
#define  ANSC_TFTPCO_RMODE_READ                     1
#define  ANSC_TFTPCO_RMODE_WRITE                    2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TFTPCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TFTPCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_TFTPCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    );

typedef  char*
(*PFN_TFTPCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  PUCHAR
(*PFN_TFTPCO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_TFTPCO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_READ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       fileName,
        ULONG                       fileMode,
        ULONG                       ulSecToWait
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_WRITE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       fileName,
        ULONG                       fileMode,
        void*                       fileData,
        ULONG                       fileSize,
        ULONG                       ulSecToWait
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_PREPARE)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       fileData,
        ULONG                       fileSize
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_EXTRACT)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       fileData,
        ULONG                       fileSize
    );

typedef  ULONG
(*PFN_TFTPCO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TFTPCO_GET_BDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TFTPCO_POP_BDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_ADD_BDO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SEND_RRQ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       fileName,
        ULONG                       fileMode
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SEND_WRQ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       fileName,
        ULONG                       fileMode
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SEND_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      blockNum,
        void*                       blockData,
        ULONG                       blockSize
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SEND_ACK)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      blockNum
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SEND_ERROR)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      errorCode,
        char*                       errorMsg
    );

typedef  ULONG
(*PFN_TFTPCO_GET_INTERVAL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_INTERVAL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeoutInterval
    );

typedef  ULONG
(*PFN_TFTPCO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TFTPCO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       RetryCount
    );


/*
 * Hosts on the ARPA Internet that choose to implement a Time Protocol are expected to adopt and
 * implement this standard. This protocol provides a site-independent, machine readable date and
 * time. The Time service sends back to the originating source the time in seconds since midnight
 * on January first 1900. One motivation arises from the fact that not all systems have a date/time
 * clock, and all are subject to occasional human or machine error.  The use of time-servers makes
 * it possible to quickly confirm or correct a system's idea of the time, by making a brief poll of
 * several independent sites on the network.
 */
#define  ANSC_TFTP_CLIENT_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hConnTimerObj;                                          \
    ANSC_HANDLE                     hConnTimerIf;                                           \
    ANSC_HANDLE                     hSimpleClientUdp;                                       \
    ANSC_HANDLE                     hClientUdpWorker;                                       \
    ANSC_HANDLE                     hClientReserved;                                        \
    ANSC_HANDLE                     hAnscFtcIf;                                             \
                                                                                            \
    ULONG                           FileMode;                                               \
    char                            FileName  [ANSC_TFTPCO_MAX_FILENAME_SIZE];              \
    UCHAR                           ScratchPad[ANSC_TFTPCO_MAX_MESSAGE_SIZE ];              \
    ULONG                           PadCode;                                                \
    ULONG                           PadSize;                                                \
    ULONG                           ClientState;                                            \
    ULONG                           ClientRmode;                                            \
    USHORT                          CurBlockNum;                                            \
    ULONG                           RetryCount;                                             \
    ULONG                           MaxRetryCount;                                          \
    ULONG                           TimeoutInterval;                                        \
                                                                                            \
    QUEUE_HEADER                    BdoQueue;                                               \
    ANSC_LOCK                       BdoQueueLock;                                           \
                                                                                            \
    PFN_TFTPCO_GET_MODE             GetFileMode;                                            \
    PFN_TFTPCO_SET_MODE             SetFileMode;                                            \
    PFN_TFTPCO_GET_NAME             GetFileName;                                            \
    PFN_TFTPCO_SET_NAME             SetFileName;                                            \
    PFN_TFTPCO_GET_ADDR             GetClientAddress;                                       \
    PFN_TFTPCO_SET_ADDR             SetClientAddress;                                       \
    PFN_TFTPCO_GET_PORT             GetClientPort;                                          \
    PFN_TFTPCO_SET_PORT             SetClientPort;                                          \
    PFN_TFTPCO_GET_ADDR             GetServerAddress;                                       \
    PFN_TFTPCO_SET_ADDR             SetServerAddress;                                       \
    PFN_TFTPCO_GET_PORT             GetServerPort;                                          \
    PFN_TFTPCO_SET_PORT             SetServerPort;                                          \
    PFN_TFTPCO_GET_INTERVAL         GetTimeoutInterval;                                     \
    PFN_TFTPCO_SET_INTERVAL         SetTimeoutInterval;                                     \
    PFN_TFTPCO_GET_COUNT            GetRetryCount;                                          \
    PFN_TFTPCO_SET_COUNT            SetRetryCount;                                          \
    PFN_TFTPCO_GET_COUNT            GetMaxRetryCount;                                       \
    PFN_TFTPCO_SET_COUNT            SetMaxRetryCount;                                       \
                                                                                            \
    PFN_TFTPCO_GET_CONTEXT          GetSimpleClientUdp;                                     \
    PFN_TFTPCO_GET_CONTEXT          GetClientUdpWorker;                                     \
    PFN_TFTPCO_GET_CONTEXT          GetClientReserved;                                      \
    PFN_TFTPCO_SET_CONTEXT          SetClientReserved;                                      \
    PFN_TFTPCO_GET_IF               GetAnscFtcIf;                                           \
    PFN_TFTPCO_SET_IF               SetAnscFtcIf;                                           \
    PFN_TFTPCO_RESET                Reset;                                                  \
                                                                                            \
    PFN_TFTPCO_ENGAGE               Engage;                                                 \
    PFN_TFTPCO_CANCEL               Cancel;                                                 \
    PFN_TFTPCO_INVOKE               ConnTimerInvoke;                                        \
    PFN_TFTPCO_READ                 ReadFile;                                               \
    PFN_TFTPCO_WRITE                WriteFile;                                              \
                                                                                            \
    PFN_TFTPCO_PREPARE              PrepareBdos;                                            \
    PFN_TFTPCO_EXTRACT              ExtractFile;                                            \
    PFN_TFTPCO_GET_SIZE             GetFileSize;                                            \
    PFN_TFTPCO_GET_BDO              GetHeadBdo;                                             \
    PFN_TFTPCO_GET_BDO              GetTailBdo;                                             \
    PFN_TFTPCO_POP_BDO              PopHeadBdo;                                             \
    PFN_TFTPCO_ADD_BDO              AddTailBdo;                                             \
    PFN_TFTPCO_DEL_ALL              DelAllBdos;                                             \
                                                                                            \
    PFN_TFTPCO_RECV                 RecvRrq;                                                \
    PFN_TFTPCO_RECV                 RecvWrq;                                                \
    PFN_TFTPCO_RECV                 RecvData;                                               \
    PFN_TFTPCO_RECV                 RecvAck;                                                \
    PFN_TFTPCO_RECV                 RecvError;                                              \
                                                                                            \
    PFN_TFTPCO_SEND_RRQ             SendRrq;                                                \
    PFN_TFTPCO_SEND_WRQ             SendWrq;                                                \
    PFN_TFTPCO_SEND_DATA            SendData;                                               \
    PFN_TFTPCO_SEND_ACK             SendAck;                                                \
    PFN_TFTPCO_SEND_ERROR           SendError;                                              \
                                                                                            \
    PFN_SCUOWO_QUERY                ScuowoQuery;                                            \
    PFN_SCUOWO_PROCESS              ScuowoProcess;                                          \
    PFN_SCUOWO_NOTIFY               ScuowoNotify;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TFTP_CLIENT_OBJECT
{
    ANSC_TFTP_CLIENT_CLASS_CONTENT
}
ANSC_TFTP_CLIENT_OBJECT,  *PANSC_TFTP_CLIENT_OBJECT;

#define  ACCESS_ANSC_TFTP_CLIENT_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_TFTP_CLIENT_OBJECT, Linkage)


#endif
