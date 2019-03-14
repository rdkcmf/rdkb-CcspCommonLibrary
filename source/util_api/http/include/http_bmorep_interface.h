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

    module:	http_bmorep_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Bmo Response Object.

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


#ifndef  _HTTP_BMOREP_INTERFACE_
#define  _HTTP_BMOREP_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP BMO RESPONSE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_BMOREP_PROGRESS_serverHeadersAdded    0x00000001
#define  HTTP_BMOREP_PROGRESS_hostHeadersAdded      0x00000002
#define  HTTP_BMOREP_PROGRESS_contentHeadersAdded   0x00000004
#define  HTTP_BMOREP_PROGRESS_flushed               0x00000008

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPBMOREP_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  PUCHAR
(*PFN_HTTPBMOREP_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_HTTPBMOREP_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_HTTPBMOREP_GET_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    );

typedef  ULONG
(*PFN_HTTPBMOREP_GET_CODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

typedef  ULONG
(*PFN_HTTPBMOREP_GET_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

typedef  char*
(*PFN_HTTPBMOREP_GET_TEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_TEXT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       text
    );

typedef  ULONG
(*PFN_HTTPBMOREP_GET_BITS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBMOREP_SET_BITS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       bits
    );

/*
 * HTTP messages consist of repuests from client to server and responses from server to client.
 *
 *      HTTP-message = Response | Response ; HTTP/1.1 messages
 *
 * Response and Response messages use the generic message format of RFC 822 for transferring
 * entities (the payload of hte message). Both types of message consist of a start-line, zero or
 * more header fields (also known as "headers"), an empty line (i.e., a line with nothing preceding
 * the CRLF) indicating the end of the header fields, and possibly a message-body.
 *
 *      generic-message = start-line
 *                        *(message-header CRLF)
 *                        CRLF
 *                        [message-body]
 *      start-line      = Response-Line | Status-Line
 *
 * In the interest of robustness, servers SHOULD ignore any empty line(s) received where a Response-
 * Line is expected. In other words, if the server is reading the protocol stream at the begining
 * of a message and receives a CRLF first, it should ignore the CRLF.
 */
#define  HTTP_BMO_REP_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    HTTP_BASIC_MESSAGE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_IPV4_ADDRESS               ServerAddr;                                             \
    USHORT                          ServerPort;                                             \
    ANSC_HANDLE                     hReqInfo;                                               \
    ANSC_HANDLE                     hRepInfo;                                               \
    ULONG                           ProgressBits;                                           \
                                                                                            \
    PFN_HTTPBMOREP_GET_ADDR         GetServerAddr;                                          \
    PFN_HTTPBMOREP_SET_ADDR         SetServerAddr;                                          \
    PFN_HTTPBMOREP_GET_PORT         GetServerPort;                                          \
    PFN_HTTPBMOREP_SET_PORT         SetServerPort;                                          \
                                                                                            \
    PFN_HTTPBMOREP_GET_INFO         GetReqInfo;                                             \
    PFN_HTTPBMOREP_GET_INFO         GetRepInfo;                                             \
    PFN_HTTPBMOREP_GET_CODE         GetCode;                                                \
    PFN_HTTPBMOREP_GET_VERSION      GetMajorVersion;                                        \
    PFN_HTTPBMOREP_GET_VERSION      GetMinorVersion;                                        \
    PFN_HTTPBMOREP_GET_TEXT         GetText;                                                \
    PFN_HTTPBMOREP_GET_BITS         GetProgressBits;                                        \
                                                                                            \
    PFN_HTTPBMOREP_SET_INFO         SetReqInfo;                                             \
    PFN_HTTPBMOREP_SET_INFO         SetRepInfo;                                             \
    PFN_HTTPBMOREP_SET_CODE         SetCode;                                                \
    PFN_HTTPBMOREP_SET_VERSION      SetMajorVersion;                                        \
    PFN_HTTPBMOREP_SET_VERSION      SetMinorVersion;                                        \
    PFN_HTTPBMOREP_SET_TEXT         SetText;                                                \
    PFN_HTTPBMOREP_SET_BITS         SetProgressBits;                                        \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_BMO_REP_OBJECT
{
    HTTP_BMO_REP_CLASS_CONTENT
}
HTTP_BMO_REP_OBJECT,  *PHTTP_BMO_REP_OBJECT;

#define  ACCESS_HTTP_BMO_REP_OBJECT(p)              \
         ACCESS_CONTAINER(p, HTTP_BMO_REP_OBJECT, Linkage)


#endif
