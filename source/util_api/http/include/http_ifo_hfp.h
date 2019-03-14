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

    module:	http_ifo_hfp.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Header Field Parser Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_HFP_
#define  _HTTP_IFO_HFP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    HTTP HEADER FIELD PARSER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_HFP_FORM_ENCTYPE_URLENCODED           1
#define  HTTP_HFP_FORM_ENCTYPE_MULTIPART            2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPHFPIF_PARSE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_HANDLE
(*PFN_HTTPHFPIF_PARSE_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPHFPIF_PARSE_URL_L)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        PULONG                      pulUrlCount,
        PANSC_HANDLE                phUrlArray
    );

typedef  ANSC_HANDLE
(*PFN_HTTPHFPIF_PARSE_INPUT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ULONG                       ulEncType,
        char*                       pBoundary
    );

typedef  ANSC_STATUS
(*PFN_HTTPHFPIF_SAVE_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        ULONG                       ulEncType,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        BOOL                        bFlushBody
    );

typedef  char*
(*PFN_HTTPHFPIF_GET_FORM)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        PULONG                      pulFormLen
    );

typedef  char*
(*PFN_HTTPHFPIF_GET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       parameters,
        char*                       name
    );

typedef  ULONG
(*PFN_HTTPHFPIF_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPHFPIF_BUILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Requests are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_HFP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPHFPIF_PARSE             ParseHeader;                                            \
    PFN_HTTPHFPIF_PARSE             ParseRequestLine;                                       \
    PFN_HTTPHFPIF_PARSE             ParseStatusLine;                                        \
    PFN_HTTPHFPIF_PARSE             ParseChunkedLine;                                       \
    PFN_HTTPHFPIF_PARSE             ParseQueryString;                                       \
    PFN_HTTPHFPIF_PARSE_URL         ParseHttpUrl;                                           \
    PFN_HTTPHFPIF_PARSE_URL_L       ParseHttpUrlList;                                       \
    PFN_HTTPHFPIF_PARSE_INPUT       ParseHtmlFormInput;                                     \
    PFN_HTTPHFPIF_SAVE_FILE         SaveFormPartAsFile;                                     \
    PFN_HTTPHFPIF_GET_PARAM         GetHeaderParam;                                         \
    PFN_HTTPHFPIF_GET_FORM          GetFormDataNoFile;                                      \
                                                                                            \
    PFN_HTTPHFPIF_GET_SIZE          GetHeaderSize;                                          \
    PFN_HTTPHFPIF_GET_SIZE          GetRequestLineSize;                                     \
    PFN_HTTPHFPIF_GET_SIZE          GetStatusLineSize;                                      \
                                                                                            \
    PFN_HTTPHFPIF_BUILD             BuildHeader;                                            \
    PFN_HTTPHFPIF_BUILD             BuildRequestLine;                                       \
    PFN_HTTPHFPIF_BUILD             BuildStatusLine;                                        \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFP_INTERFACE
{
    HTTP_HFP_INTERFACE_CLASS_CONTENT
}
HTTP_HFP_INTERFACE,  *PHTTP_HFP_INTERFACE;

#define  ACCESS_HTTP_HFP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_HFP_INTERFACE, Linkage)


#endif
