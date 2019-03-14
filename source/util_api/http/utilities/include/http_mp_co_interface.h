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

    module:	htt_pmp_co_interface.h

        For HTTP message parser (syntax parser),
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

#ifndef  _ANSC_HTTPMP_CO_INTERFACE
#define  _ANSC_HTTPMP_CO_INTERFACE

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "http_definitions.h"
#include "http_ifo_hfp.h"

#include "ansc_lex_interface.h"
#include "http_mp_co_util_def.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_HTTPMP_COMPONENT_NAME                 "HttpMsgParserComponent"
#define  ANSC_HTTPMP_COMPONENT_OID                  ANSC_OBJECT_OID_NULL
#define  ANSC_HTTPMP_COMPONENT_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  HTTP_HFP_INTERFACE_NAME                    "HttpHfpIf"
#define  HTTP_HFP_INTERFACE_ID                      0

/*
 * This function returns the "HFPIF" interface to the caller, and then
 * the caller can call the functions defined in the interface object
 * "HTTP_HFP_INTERFACE_CLASS_CONTENT" to parse HTTP header and construct
 * HTTP message or header from given header.
 */
typedef
ANSC_HANDLE
(*PFN_HTTPMP_GET_HFPIF)
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
#define  ANSC_HTTPMP_COMPONENT_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    /* Manager object */                                                                    \
    /*ANSC_HANDLE                     hCallMgr;*/                                           \
    /* HTTP tokenizer object */                                                             \
    ANSC_HANDLE                     hHttpTokenizer;                                         \
    /* used to set up entry points in NON-recursive parsing function */                     \
    ANSC_HANDLE                     hExpHttpRequest;                                        \
    ANSC_HANDLE                     hExpHttpStatus;                                         \
    ANSC_HANDLE                     hExpHttpHeaders;                                        \
    ANSC_HANDLE                     hExpHttpEntityHeader;                                   \
    ANSC_HANDLE                     hExpHttpChunk;                                          \
                                                                                            \
    PFN_HTTPHFPIF_PARSE             ParseHeader;                                            \
    PFN_HTTPHFPIF_PARSE             ParseRequestLine;                                       \
    PFN_HTTPHFPIF_PARSE             ParseStatusLine;                                        \
    PFN_HTTPHFPIF_PARSE             ParseChunkedLine;                                       \
                                                                                            \
    PFN_HTTPHFPIF_GET_SIZE          GetHeaderSize;                                          \
    PFN_HTTPHFPIF_GET_SIZE          GetRequestLineSize;                                     \
    PFN_HTTPHFPIF_GET_SIZE          GetStatusLineSize;                                      \
                                                                                            \
    PFN_HTTPHFPIF_BUILD             BuildHeader;                                            \
    PFN_HTTPHFPIF_BUILD             BuildRequestLine;                                       \
    PFN_HTTPHFPIF_BUILD             BuildStatusLine;                                        \
    /* end of object class content */                                                       \
    PFN_HTTPMP_GET_HFPIF            GetHfpIf;                                               \
                                                                                            



/* HTTPMP object  */
typedef  struct
_ANSC_HTTPMP_COMPONENT_OBJECT
{
    ANSC_HTTPMP_COMPONENT_CLASS_CONTENT
}
ANSC_HTTPMP_COMPONENT_OBJECT,    *PANSC_HTTPMP_COMPONENT_OBJECT;


#define  ACCESS_HTTPMP_COMPONENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_HTTPMP_COMPONENT_OBJECT, Linkage)

#endif
