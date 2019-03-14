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

    module:	http_bmo_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Basic Message Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMO_INTERFACE_
#define  _HTTP_BMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
 PLATFORM INDEPENDENT HTTP BASIC MESSAGE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_BMO_SCRATCH_PAD1_SIZE                 4096
#define  HTTP_BMO_SCRATCH_PAD2_SIZE                 256
#define  HTTP_BMO_HFO_TABLE_SIZE                    16

#define  HTTP_BMO_STATE_RESERVED                    0
#define  HTTP_BMO_STATE_EMPTY                       1
#define  HTTP_BMO_STATE_PART_HEADER                 2
#define  HTTP_BMO_STATE_HEADER_ARRIVED              3
#define  HTTP_BMO_STATE_PART_BODY                   4
#define  HTTP_BMO_STATE_COMPLETE                    5
#define  HTTP_BMO_STATE_OVER_PACKED                 6
#define  HTTP_BMO_STATE_UNKNOWN                     7

#define  HTTP_BMO_CTSTATE_RESERVED                  0
#define  HTTP_BMO_CTSTATE_EMPTY                     1
#define  HTTP_BMO_CTSTATE_PART_INFO                 2
#define  HTTP_BMO_CTSTATE_PART_BODY                 3
#define  HTTP_BMO_CTSTATE_PART_TRAILER              4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPBMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPBMO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ULONG
(*PFN_HTTPBMO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ULONG
(*PFN_HTTPBMO_GET_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_ID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_RETURN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_HTTPBMO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  BOOL
(*PFN_HTTPBMO_GET_PTM)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_GET_HEADER_BDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_HEADER_BDO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  char*
(*PFN_HTTPBMO_GET_RAW1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

typedef  char*
(*PFN_HTTPBMO_GET_RAW1_MT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    );

typedef  char*
(*PFN_HTTPBMO_GET_RAW2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  char*
(*PFN_HTTPBMO_GET_RAW2_MT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_RAW1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        char*                       value
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_RAW2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       value
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_DEL_LINE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_PARSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_COPY_LINE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_COPY_HEADERS)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_GET_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_GET_HEADER2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SET_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_ADD_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_DEL_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPBMO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_COPY_BODY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SAVE_BODY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SAVE_MBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMbo
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_CREATE_BODY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_APPEND_BODY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_APPEND_BODY2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_APPEND_BODY3)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SAVE_FILE1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_SAVE_FILE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        BOOL                        bFlushBody
    );

typedef  ULONG
(*PFN_HTTPBMO_EXAMINE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_CLOSEUP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_PRODUCE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_OUTPUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );

typedef  void
(*PFN_HTTPBMO_SEND_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMO_BCC_GET_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_BCC_ADD_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

typedef  ULONG
(*PFN_HTTPBMO_BCC_PREDICT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_BCC_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMO_BCC_SERIALIZE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
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
#ifdef _ANSC_HTTP_FILE_USED_

#define  HTTP_BASIC_MESSAGE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTmhIf;                                                 \
    ANSC_HANDLE                     hBccIf;                                                 \
    ANSC_HANDLE                     hFumIf;                                                 \
    ANSC_HANDLE                     hRecvContext;                                           \
    ANSC_HANDLE                     hSendContext;                                           \
    ANSC_HANDLE                     hHttpWebsTrans;                                         \
    ANSC_HANDLE                     hWebServer;                                             \
    ANSC_HANDLE                     hWebHost;                                               \
    ANSC_HANDLE                     hWebServlet;                                            \
    ANSC_HANDLE                     hMessageBody;                                           \
    ANSC_HANDLE                     hHeaderBdo;                                             \
    ULONG                           State;                                                  \
    ULONG                           MboMode;                                                \
    ULONG                           TransactionId;                                          \
    char                            ScratchPad1[HTTP_BMO_SCRATCH_PAD1_SIZE];                \
    char                            ScratchPad2[HTTP_BMO_SCRATCH_PAD2_SIZE];                \
    ULONG                           PadSize1;                                               \
    ULONG                           PadSize2;                                               \
    ULONG                           WebSessionId;                                           \
                                                                                            \
    QUEUE_HEADER                    HfoTable[HTTP_BMO_HFO_TABLE_SIZE];                      \
    ANSC_LOCK                       HfoTableLock;                                           \
                                                                                            \
    PFN_HTTPBMO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPBMO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPBMO_GET_IF              GetTmhIf;                                               \
    PFN_HTTPBMO_SET_IF              SetTmhIf;                                               \
    PFN_HTTPBMO_GET_IF              GetFumIf;                                               \
    PFN_HTTPBMO_SET_IF              SetFumIf;                                               \
    PFN_HTTPBMO_GET_CONTEXT         GetRecvContext;                                         \
    PFN_HTTPBMO_SET_CONTEXT         SetRecvContext;                                         \
    PFN_HTTPBMO_GET_CONTEXT         GetSendContext;                                         \
    PFN_HTTPBMO_SET_CONTEXT         SetSendContext;                                         \
    PFN_HTTPBMO_GET_CONTEXT         GetHttpWebsTrans;                                       \
    PFN_HTTPBMO_SET_CONTEXT         SetHttpWebsTrans;                                       \
    PFN_HTTPBMO_GET_CONTEXT         GetWebServer;                                           \
    PFN_HTTPBMO_SET_CONTEXT         SetWebServer;                                           \
    PFN_HTTPBMO_GET_CONTEXT         GetWebHost;                                             \
    PFN_HTTPBMO_SET_CONTEXT         SetWebHost;                                             \
    PFN_HTTPBMO_GET_CONTEXT         GetWebServlet;                                          \
    PFN_HTTPBMO_SET_CONTEXT         SetWebServlet;                                          \
    PFN_HTTPBMO_GET_STATE           GetState;                                               \
    PFN_HTTPBMO_SET_STATE           SetState;                                               \
    PFN_HTTPBMO_GET_MODE            GetMboMode;                                             \
    PFN_HTTPBMO_SET_MODE            SetMboMode;                                             \
    PFN_HTTPBMO_GET_ID              GetTransactionId;                                       \
    PFN_HTTPBMO_SET_ID              SetTransactionId;                                       \
    PFN_HTTPBMO_GET_ID              GetWebSessionId;                                        \
    PFN_HTTPBMO_SET_ID              SetWebSessionId;                                        \
                                                                                            \
    PFN_HTTPBMO_RETURN              Return;                                                 \
    PFN_HTTPBMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPBMO_GET_SIZE            GetStartLineSize;                                       \
    PFN_HTTPBMO_GET_SIZE            GetHeadersSize;                                         \
    PFN_HTTPBMO_GET_SIZE            PredictBodySize;                                        \
    PFN_HTTPBMO_GET_SIZE            GetBodySize;                                            \
    PFN_HTTPBMO_GET_SIZE            GetBodySize2;                                           \
    PFN_HTTPBMO_GET_SIZE            GetMessageSize;                                         \
    PFN_HTTPBMO_GET_PTM             GetPtmForHeaders;                                       \
    PFN_HTTPBMO_GET_PTM             GetPtmForBody;                                          \
                                                                                            \
    PFN_HTTPBMO_GET_HEADER_BDO      GetHeaderBdo;                                           \
    PFN_HTTPBMO_SET_HEADER_BDO      SetHeaderBdo;                                           \
                                                                                            \
    PFN_HTTPBMO_IS_SOMETHING        IsProxyKeepAlive;                                       \
    PFN_HTTPBMO_IS_SOMETHING        IsCloseConnection;                                      \
    PFN_HTTPBMO_IS_SOMETHING        IsConnKeepAlive;                                        \
    PFN_HTTPBMO_IS_SOMETHING        IsChunkedCoding;                                        \
    PFN_HTTPBMO_IS_SOMETHING        IsHttpVersion11;                                        \
                                                                                            \
    PFN_HTTPBMO_GET_RAW1            GetHeaderValueById;                                     \
    PFN_HTTPBMO_GET_RAW1_MT         GetHeaderValueById2;                                    \
    PFN_HTTPBMO_GET_RAW2            GetHeaderValueByName;                                   \
    PFN_HTTPBMO_GET_RAW2_MT         GetHeaderValueByName2;                                  \
    PFN_HTTPBMO_SET_RAW2            SetHeaderValueByName;                                   \
                                                                                            \
    PFN_HTTPBMO_DEL_LINE            DelStartLine;                                           \
    PFN_HTTPBMO_PARSE               ParseStartLine;                                         \
    PFN_HTTPBMO_CLEAR               ClearHeaders;                                           \
    PFN_HTTPBMO_PARSE               ParseHeaders;                                           \
    PFN_HTTPBMO_COPY_LINE           CopyStartLineFrom;                                      \
    PFN_HTTPBMO_COPY_HEADERS        CopyHeadersFrom;                                        \
    PFN_HTTPBMO_GET_HEADER          GetHeaderField;                                         \
    PFN_HTTPBMO_GET_HEADER2         GetHeaderField2;                                        \
    PFN_HTTPBMO_ADD_HEADER          AddHeaderField;                                         \
    PFN_HTTPBMO_DEL_HEADER          DelHeaderField;                                         \
    PFN_HTTPBMO_DEL_ALL             DelAllHfos;                                             \
                                                                                            \
    PFN_HTTPBMO_CLEAR               ClearBody;                                              \
    PFN_HTTPBMO_COPY_BODY           CopyBodyFrom;                                           \
    PFN_HTTPBMO_COPY_BODY           CopyBodyInto;                                           \
    PFN_HTTPBMO_SAVE_BODY           SaveAsBody;                                             \
    PFN_HTTPBMO_SAVE_MBO            SaveAsMbo;                                              \
    PFN_HTTPBMO_CREATE_BODY         CreateMessageBody;                                      \
    PFN_HTTPBMO_APPEND_BODY         AppendBody;                                             \
    PFN_HTTPBMO_APPEND_BODY2        AppendBodyBdo;                                          \
    PFN_HTTPBMO_APPEND_BODY3        AppendBodyFile;                                         \
    PFN_HTTPBMO_SAVE_FILE1          SaveBodyAsFile;                                         \
    PFN_HTTPBMO_SAVE_FILE2          SaveFormAsFile;                                         \
                                                                                            \
    PFN_HTTPBMO_EXAMINE             Examine;                                                \
    PFN_HTTPBMO_PROCESS             Process;                                                \
    PFN_HTTPBMO_CLOSEUP             CloseUp;                                                \
    PFN_HTTPBMO_PRODUCE             Produce;                                                \
    PFN_HTTPBMO_OUTPUT              OutputHeaders;                                          \
    PFN_HTTPBMO_OUTPUT              OutputBody;                                             \
    PFN_HTTPBMO_SEND_COMPLETE       SendComplete;                                           \
                                                                                            \
    PFN_HTTPBMO_BCC_GET_HEADER      BccGetHeaderField;                                      \
    PFN_HTTPBMO_BCC_ADD_HEADER      BccAddHeaderField;                                      \
    PFN_HTTPBMO_BCC_PREDICT         BccPredictBodySize;                                     \
    PFN_HTTPBMO_BCC_NOTIFY          BccNotify;                                              \
    PFN_HTTPBMO_BCC_SERIALIZE       BccSerialize;                                           \
    /* end of object class content */                                                       \

#else
#define  HTTP_BASIC_MESSAGE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hTmhIf;                                                 \
    ANSC_HANDLE                     hBccIf;                                                 \
    ANSC_HANDLE                     hFumIf;                                                 \
    ANSC_HANDLE                     hRecvContext;                                           \
    ANSC_HANDLE                     hSendContext;                                           \
    ANSC_HANDLE                     hHttpWebsTrans;                                         \
    ANSC_HANDLE                     hWebServer;                                             \
    ANSC_HANDLE                     hWebHost;                                               \
    ANSC_HANDLE                     hWebServlet;                                            \
    ANSC_HANDLE                     hMessageBody;                                           \
    ANSC_HANDLE                     hHeaderBdo;                                             \
    ULONG                           State;                                                  \
    ULONG                           MboMode;                                                \
    ULONG                           TransactionId;                                          \
    char                            ScratchPad1[HTTP_BMO_SCRATCH_PAD1_SIZE];                \
    char                            ScratchPad2[HTTP_BMO_SCRATCH_PAD2_SIZE];                \
    ULONG                           PadSize1;                                               \
    ULONG                           PadSize2;                                               \
    ULONG                           WebSessionId;                                           \
                                                                                            \
    QUEUE_HEADER                    HfoTable[HTTP_BMO_HFO_TABLE_SIZE];                      \
    ANSC_LOCK                       HfoTableLock;                                           \
                                                                                            \
    PFN_HTTPBMO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPBMO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPBMO_GET_IF              GetTmhIf;                                               \
    PFN_HTTPBMO_SET_IF              SetTmhIf;                                               \
    PFN_HTTPBMO_GET_IF              GetFumIf;                                               \
    PFN_HTTPBMO_SET_IF              SetFumIf;                                               \
    PFN_HTTPBMO_GET_CONTEXT         GetRecvContext;                                         \
    PFN_HTTPBMO_SET_CONTEXT         SetRecvContext;                                         \
    PFN_HTTPBMO_GET_CONTEXT         GetSendContext;                                         \
    PFN_HTTPBMO_SET_CONTEXT         SetSendContext;                                         \
    PFN_HTTPBMO_GET_CONTEXT         GetHttpWebsTrans;                                       \
    PFN_HTTPBMO_SET_CONTEXT         SetHttpWebsTrans;                                       \
    PFN_HTTPBMO_GET_CONTEXT         GetWebServer;                                           \
    PFN_HTTPBMO_SET_CONTEXT         SetWebServer;                                           \
    PFN_HTTPBMO_GET_CONTEXT         GetWebHost;                                             \
    PFN_HTTPBMO_SET_CONTEXT         SetWebHost;                                             \
    PFN_HTTPBMO_GET_CONTEXT         GetWebServlet;                                          \
    PFN_HTTPBMO_SET_CONTEXT         SetWebServlet;                                          \
    PFN_HTTPBMO_GET_STATE           GetState;                                               \
    PFN_HTTPBMO_SET_STATE           SetState;                                               \
    PFN_HTTPBMO_GET_MODE            GetMboMode;                                             \
    PFN_HTTPBMO_SET_MODE            SetMboMode;                                             \
    PFN_HTTPBMO_GET_ID              GetTransactionId;                                       \
    PFN_HTTPBMO_SET_ID              SetTransactionId;                                       \
    PFN_HTTPBMO_GET_ID              GetWebSessionId;                                        \
    PFN_HTTPBMO_SET_ID              SetWebSessionId;                                        \
                                                                                            \
    PFN_HTTPBMO_RETURN              Return;                                                 \
    PFN_HTTPBMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPBMO_GET_SIZE            GetStartLineSize;                                       \
    PFN_HTTPBMO_GET_SIZE            GetHeadersSize;                                         \
    PFN_HTTPBMO_GET_SIZE            PredictBodySize;                                        \
    PFN_HTTPBMO_GET_SIZE            GetBodySize;                                            \
    PFN_HTTPBMO_GET_SIZE            GetBodySize2;                                           \
    PFN_HTTPBMO_GET_SIZE            GetMessageSize;                                         \
    PFN_HTTPBMO_GET_PTM             GetPtmForHeaders;                                       \
    PFN_HTTPBMO_GET_PTM             GetPtmForBody;                                          \
                                                                                            \
    PFN_HTTPBMO_GET_HEADER_BDO      GetHeaderBdo;                                           \
    PFN_HTTPBMO_SET_HEADER_BDO      SetHeaderBdo;                                           \
                                                                                            \
    PFN_HTTPBMO_IS_SOMETHING        IsProxyKeepAlive;                                       \
    PFN_HTTPBMO_IS_SOMETHING        IsCloseConnection;                                      \
    PFN_HTTPBMO_IS_SOMETHING        IsConnKeepAlive;                                        \
    PFN_HTTPBMO_IS_SOMETHING        IsChunkedCoding;                                        \
    PFN_HTTPBMO_IS_SOMETHING        IsHttpVersion11;                                        \
                                                                                            \
    PFN_HTTPBMO_GET_RAW1            GetHeaderValueById;                                     \
    PFN_HTTPBMO_GET_RAW1_MT         GetHeaderValueById2;                                    \
    PFN_HTTPBMO_GET_RAW2            GetHeaderValueByName;                                   \
    PFN_HTTPBMO_GET_RAW2_MT         GetHeaderValueByName2;                                  \
    PFN_HTTPBMO_SET_RAW2            SetHeaderValueByName;                                   \
                                                                                            \
    PFN_HTTPBMO_DEL_LINE            DelStartLine;                                           \
    PFN_HTTPBMO_PARSE               ParseStartLine;                                         \
    PFN_HTTPBMO_CLEAR               ClearHeaders;                                           \
    PFN_HTTPBMO_PARSE               ParseHeaders;                                           \
    PFN_HTTPBMO_COPY_LINE           CopyStartLineFrom;                                      \
    PFN_HTTPBMO_COPY_HEADERS        CopyHeadersFrom;                                        \
    PFN_HTTPBMO_GET_HEADER          GetHeaderField;                                         \
    PFN_HTTPBMO_GET_HEADER2         GetHeaderField2;                                        \
    PFN_HTTPBMO_ADD_HEADER          AddHeaderField;                                         \
    PFN_HTTPBMO_DEL_HEADER          DelHeaderField;                                         \
    PFN_HTTPBMO_DEL_ALL             DelAllHfos;                                             \
                                                                                            \
    PFN_HTTPBMO_CLEAR               ClearBody;                                              \
    PFN_HTTPBMO_COPY_BODY           CopyBodyFrom;                                           \
    PFN_HTTPBMO_COPY_BODY           CopyBodyInto;                                           \
    PFN_HTTPBMO_SAVE_BODY           SaveAsBody;                                             \
    PFN_HTTPBMO_SAVE_MBO            SaveAsMbo;                                              \
    PFN_HTTPBMO_CREATE_BODY         CreateMessageBody;                                      \
    PFN_HTTPBMO_APPEND_BODY         AppendBody;                                             \
    PFN_HTTPBMO_APPEND_BODY2        AppendBodyBdo;                                          \
                                                                                            \
    PFN_HTTPBMO_EXAMINE             Examine;                                                \
    PFN_HTTPBMO_PROCESS             Process;                                                \
    PFN_HTTPBMO_CLOSEUP             CloseUp;                                                \
    PFN_HTTPBMO_PRODUCE             Produce;                                                \
    PFN_HTTPBMO_OUTPUT              OutputHeaders;                                          \
    PFN_HTTPBMO_OUTPUT              OutputBody;                                             \
    PFN_HTTPBMO_SEND_COMPLETE       SendComplete;                                           \
                                                                                            \
    PFN_HTTPBMO_BCC_GET_HEADER      BccGetHeaderField;                                      \
    PFN_HTTPBMO_BCC_ADD_HEADER      BccAddHeaderField;                                      \
    PFN_HTTPBMO_BCC_PREDICT         BccPredictBodySize;                                     \
    PFN_HTTPBMO_BCC_NOTIFY          BccNotify;                                              \
    PFN_HTTPBMO_BCC_SERIALIZE       BccSerialize;                                           \
    /* end of object class content */                                                       \

#endif

typedef  struct
_HTTP_BASIC_MESSAGE_OBJECT
{
    HTTP_BASIC_MESSAGE_CLASS_CONTENT
}
HTTP_BASIC_MESSAGE_OBJECT,  *PHTTP_BASIC_MESSAGE_OBJECT;

#define  ACCESS_HTTP_BASIC_MESSAGE_OBJECT(p)        \
         ACCESS_CONTAINER(p, HTTP_BASIC_MESSAGE_OBJECT, Linkage)


#endif
