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

    module:	http_mbo_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Message Body Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBO_INTERFACE_
#define  _HTTP_MBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"

/***********************************************************
  PLATFORM INDEPENDENT HTTP MESSAGE BODY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_MBO_STATE_RESERVED                    0
#define  HTTP_MBO_STATE_EMPTY                       1
#define  HTTP_MBO_STATE_PART_BODY                   2
#define  HTTP_MBO_STATE_COMPLETE                    3
#define  HTTP_MBO_STATE_OVER_PACKED                 4
#define  HTTP_MBO_STATE_UNKNOWN                     5

#define  HTTP_MBO_MODE_RELAY                        1
#define  HTTP_MBO_MODE_STORE                        2
#define  HTTP_MBO_MODE_EMPTY                        3
#define  HTTP_MBO_MODE_STORE_EXTERNAL               4   /* this mode is used internally, must not be set by external module */

#define  HTTP_MBO_MP_STATE_INIT                     0
#define  HTTP_MBO_MP_STATE_PART_DATA                1

#define  HTTP_MBO_MP_PART_HEADER_SIZE               2048
#define  HTTP_MBO_MP_PART_BDO_SIZE                  4096

#define  HTTP_MBO_MP_PART_HEADER_MIN_SIZE           49

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPMBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPMBO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPMBO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPMBO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_APPEND_BDO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_APPEND_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFile,
        int                         iRangeFrom,
        int                         iRangeTo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_COPY_BODY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SAVE_BODY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_SAVE_FILE1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    );

typedef  ULONG
(*PFN_HTTPMBO_EXAMINE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_CLOSEUP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_OUTPUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * HTTP messages consist of requests from client to server and responses from server to client.
 *
 *      HTTP-message = Request | Response ; HTTP/1.1 messages
 *
 * Request and Response messages use the generic message format of RFC 822 for transferring
 * entities (the payload of hte message). Both types of message consist of a start-line, zero or
 * more header fields (also known as "headers"), an empty line (i.e., a line with nothing preceding
 * the CRLF) indicating the end of the header fields, and possibly a message-body.
 *
 *      generic-message = start-line
 *                        *(message-header CRLF)
 *                        CRLF
 *                        [message-body]
 *      start-line      = Request-Line | Status-Line
 *
 * In the interest of robustness, servers SHOULD ignore any empty line(s) received where a Request-
 * Line is expected. In other words, if the server is reading the protocol stream at the begining
 * of a message and receives a CRLF first, it should ignore the CRLF.
 */
#define  HTTP_MESSAGE_BODY_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hBccIf;                                                 \
    ULONG                           State;                                                  \
    ULONG                           Mode;                                                   \
    ULONG                           TransferSize;                                           \
    ULONG                           EntitySize;                                             \
    ULONG                           ArrivedSize;                                            \
                                                                                            \
    QUEUE_HEADER                    BdoQueue;                                               \
    ANSC_LOCK                       BdoQueueLock;                                           \
                                                                                            \
    ANSC_HANDLE                     hFumIf;                                                 \
    ULONG                           MPState;                                                \
    ANSC_HANDLE                     hLastMPBdo;                                             \
    BOOL                            bMPFileData;                                            \
    PUCHAR                          pMPFieldName;                                           \
    PUCHAR                          pMPFileName;                                            \
    PUCHAR                          pBoundaryStr;                                           \
    PUCHAR                          pMPHeadersBuf;                                          \
                                                                                            \
    PFN_HTTPMBO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPMBO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPMBO_GET_IF              GetBccIf;                                               \
    PFN_HTTPMBO_SET_IF              SetBccIf;                                               \
    PFN_HTTPMBO_SET_IF              SetFumIf;                                               \
    PFN_HTTPMBO_SET_IF              SetCgiIf;                                               \
    PFN_HTTPMBO_GET_STATE           GetState;                                               \
    PFN_HTTPMBO_SET_STATE           SetState;                                               \
    PFN_HTTPMBO_GET_MODE            GetMode;                                                \
    PFN_HTTPMBO_SET_MODE            SetMode;                                                \
    PFN_HTTPMBO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPMBO_GET_SIZE            GetBodySize;                                            \
    PFN_HTTPMBO_GET_SIZE            GetBodySize2;                                           \
    PFN_HTTPMBO_GET_SIZE            GetTransferSize;                                        \
    PFN_HTTPMBO_GET_SIZE            GetEntitySize;                                          \
    PFN_HTTPMBO_GET_SIZE            GetArrivedSize;                                         \
    PFN_HTTPMBO_APPEND_BDO          AppendBodyBdo;                                          \
    PFN_HTTPMBO_APPEND_FILE         AppendBodyFile;                                         \
    PFN_HTTPMBO_CLEAR               ClearBody;                                              \
    PFN_HTTPMBO_COPY_BODY           CopyBodyFrom;                                           \
    PFN_HTTPMBO_COPY_BODY           CopyBodyInto;                                           \
    PFN_HTTPMBO_SAVE_BODY           SaveAsBody;                                             \
    PFN_HTTPMBO_SAVE_FILE1          SaveBodyAsFile;                                         \
                                                                                            \
    PFN_HTTPMBO_EXAMINE             Examine;                                                \
    PFN_HTTPMBO_PROCESS             Process;                                                \
    PFN_HTTPMBO_CLOSEUP             CloseUp;                                                \
    PFN_HTTPMBO_OUTPUT              Output;                                                 \
    PFN_HTTPMBO_REMOVE              RemoveCoding;                                           \
                                                                                            \
    PFN_HTTPMBO_APPEND_BDO          ProcessMPData;                                          \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_MESSAGE_BODY_OBJECT
{
    HTTP_MESSAGE_BODY_CLASS_CONTENT
}
HTTP_MESSAGE_BODY_OBJECT,  *PHTTP_MESSAGE_BODY_OBJECT;

#define  ACCESS_HTTP_MESSAGE_BODY_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_MESSAGE_BODY_OBJECT, Linkage)


#endif
