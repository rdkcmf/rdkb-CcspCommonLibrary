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

    module:	http_mboidt_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Mbo Identity Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/02/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBOIDT_INTERFACE_
#define  _HTTP_MBOIDT_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "http_mbo_interface.h"
#include "http_mbo_exported_api.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP MBO IDENTITY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPMBOIDT_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBOIDT_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPMBOIDT_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBOIDT_GET_CHUNK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_ADD_CHUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChunkInfo,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    );

typedef  ULONG
(*PFN_HTTPMBOIDT_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_PROCESS_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_APPEND_TRAILER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOIDT_PARSE_TRAILER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    );

/*
 * Transfer-coding values are used to indicate an encoding transformation that has been, can be, or
 * may need to be applied to an entity-body in order to ensure "safe transport" through the network.
 * This differs from a content coding in that the transfer-coding is a property of the message, not
 * of the original entity.
 *
 *      transfer-coding         = "chunked" | transfer-extension
 *      transfer-extension      = token *( ";" parameter )
 *
 * Parameters are in the form of attribute/value pairs.
 *
 *      parameter               = attribute "=" value
 *      attribute               = token
 *      value                   = token | quoted-string
 *
 * All transfer-coding values are case-insensitive. HTTP/1.1 uses transfer-coding values in the TE
 * header field and in the Transfer-Encoding header field.
 */
#define  HTTP_MBO_IDENTITY_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    HTTP_MESSAGE_BODY_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_MBO_IDENTITY_OBJECT
{
    HTTP_MBO_IDENTITY_CLASS_CONTENT
}
HTTP_MBO_IDENTITY_OBJECT,  *PHTTP_MBO_IDENTITY_OBJECT;

#define  ACCESS_HTTP_MBO_IDENTITY_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_MBO_IDENTITY_OBJECT, Linkage)


#endif
