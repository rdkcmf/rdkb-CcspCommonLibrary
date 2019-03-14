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

    module:	http_mbofile_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Mbo File Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/18/05    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBOFILE_INTERFACE_
#define  _HTTP_MBOFILE_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "http_mbo_interface.h"
#include "http_mbo_exported_api.h"


/***********************************************************
    PLATFORM INDEPENDENT HTTP MBO FILE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_MBOFILE_DEF_WRITE_UNIT                8192

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPMBOFILE_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOFILE_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBOFILE_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOFILE_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPMBOFILE_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOFILE_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

/*
 * Unlike other derived MessageBody objects, HttpMboFile object does not correspond to any specific
 * HTTP transfer-encoding type. It can only be instantiated by the BMW Web Server when it intends
 * to send back the complete/partial content of a disk file.
 */
#define  HTTP_MBO_FILE_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    HTTP_MESSAGE_BODY_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    char*                           FileName;                                               \
    int                             RangeFrom;                                              \
    int                             RangeTo;                                                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_MBO_FILE_OBJECT
{
    HTTP_MBO_FILE_CLASS_CONTENT
}
HTTP_MBO_FILE_OBJECT,  *PHTTP_MBO_FILE_OBJECT;

#define  ACCESS_HTTP_MBO_FILE_OBJECT(p)             \
         ACCESS_CONTAINER(p, HTTP_MBO_FILE_OBJECT, Linkage)


#endif
