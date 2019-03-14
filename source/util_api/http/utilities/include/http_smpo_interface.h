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

    module:	http_smpo_interface.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the HTTP message parser mainly
        focusing on header parsing.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/20/02

**********************************************************************/

#ifndef  _HTTP_SMPO_INTERFACE
#define  _HTTP_SMPO_INTERFACE


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "http_definitions.h"
#include "http_ifo_hfp.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_SMPO_COMPONENT_NAME                   "HttpSimpleMsgParser"
#define  HTTP_SMPO_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  HTTP_SMPO_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  HTTP_HFP_INTERFACE_NAME                    "HttpHfpIf"
#define  HTTP_HFP_INTERFACE_ID                      0

#define  HTTP_SMPO_HEADER_TABLE_SIZE                32
#define  HTTP_SMPO_MEDIA_TABLE_SIZE                 32

#define  HTTP_SMPO_HEADER_MAX_LEN                   64

typedef  
ANSC_HANDLE
(*PFN_SMPO_PARSE_HDR)
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    );

typedef  struct
_HTTP_SMPO_HEADER_XINFO
{
    SINGLE_LINK_ENTRY               Linkage;
    UCHAR                           Name[HTTP_SMPO_HEADER_MAX_LEN];
    ULONG                           HeaderId;
    PFN_SMPO_PARSE_HDR              xFunc;
}
HTTP_SMPO_HEADER_XINFO, *PHTTP_SMPO_HEADER_XINFO;

#define  ACCESS_HTTP_SMPO_HEADER_XINFO_OBJECT(p)                \
         ACCESS_CONTAINER(p, HTTP_SMPO_HEADER_XINFO, Linkage)   \


/*
 * This function returns the "HFPIF" interface to the caller, and then
 * the caller can call the functions defined in the interface object
 * "HTTP_HFP_INTERFACE_CLASS_CONTENT" to parse HTTP header and construct
 * HTTP message or header from given header.
 */
typedef
ANSC_HANDLE
(*PFN_HTTPSMPO_GET_HFPIF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_HTTPSMPO_REGISTER_HDRX)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulHeaderId,
        PFN_SMPO_PARSE_HDR          xFunc
    );

typedef
ANSC_STATUS
(*PFN_HTTPSMPO_REMOVE_XTABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_HTTPSMPO_REGISTER_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_HTTPSMPO_GET_HDRX)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulSize
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  HTTP_SMPO_COMPONENT_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    SLIST_HEADER                    HeaderXList[HTTP_SMPO_HEADER_TABLE_SIZE];               \
    SLIST_HEADER                    MediaSubtypeList[HTTP_SMPO_MEDIA_TABLE_SIZE];           \
                                                                                            \
    PFN_HTTPHFPIF_PARSE             ParseHeader;                                            \
    PFN_HTTPHFPIF_PARSE             ParseRequestLine;                                       \
    PFN_HTTPHFPIF_PARSE             ParseStatusLine;                                        \
    PFN_HTTPHFPIF_PARSE             ParseChunkedLine;                                       \
    PFN_HTTPHFPIF_PARSE_URL         ParseUrl;                                               \
    PFN_HTTPHFPIF_PARSE_URL_L       ParseUrlList;                                           \
                                                                                            \
    PFN_HTTPHFPIF_GET_SIZE          GetHeaderSize;                                          \
    PFN_HTTPHFPIF_GET_SIZE          GetRequestLineSize;                                     \
    PFN_HTTPHFPIF_GET_SIZE          GetStatusLineSize;                                      \
                                                                                            \
    PFN_HTTPHFPIF_BUILD             BuildHeader;                                            \
    PFN_HTTPHFPIF_BUILD             BuildRequestLine;                                       \
    PFN_HTTPHFPIF_BUILD             BuildStatusLine;                                        \
                                                                                            \
    PFN_HTTPSMPO_GET_HFPIF          GetHfpIf;                                               \
    PFN_HTTPSMPO_REGISTER_HDRX      RegisterHeaderXInfo;                                    \
    PFN_HTTPSMPO_REMOVE_XTABLE      RemoveXTable;                                           \
    PFN_HTTPSMPO_REGISTER_ALL       RegisterAllHeadersXInfo;                                \
    PFN_HTTPSMPO_GET_HDRX           GetHeaderXInfo;                                         \
                                                                                            \
    /* end of object class content */                                                       \


/* HTTPMP object  */
typedef  struct
_HTTP_SIMPLE_MSG_PARSER
{
    HTTP_SMPO_COMPONENT_CLASS_CONTENT
}
HTTP_SIMPLE_MSG_PARSER, *PHTTP_SIMPLE_MSG_PARSER;


#define  ACCESS_HTTP_SIMPLE_MSG_PARSER_OBJECT(p)                \
         ACCESS_CONTAINER(p, HTTP_SIMPLE_MSG_PARSER, Linkage)


#endif

