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

    module:	http_ifo_mdh.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Multipart Data Handler Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        08/03/07    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_MDH_
#define  _HTTP_IFO_MDH_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    HTTP BODY CONTENT CARRIER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_MDH_INTERFACE_ID                      0
#define  HTTP_MDH_INTERFACE_NAME                    "HttpMdhInterface"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTPMDHIF_BEGIN)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       WebSessionId
    );

typedef  ANSC_STATUS
(*PFN_HTTPMDHIF_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       WebSessionId,
        PUCHAR                      pRequestUri,
        ULONG                       ulReqBodySize,
        PUCHAR                      pFieldName,
        PUCHAR                      pFileName,
        PUCHAR                      pData,
        ULONG                       ulDataLen
    );

typedef  ANSC_STATUS
(*PFN_HTTPMDHIF_SAVE_AS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       WebSessionId,
        PUCHAR                      pRequestUri,
        PUCHAR                      pFieldName,
        PUCHAR                      pFileLocation
    );

typedef  ANSC_STATUS
(*PFN_HTTPMDHIF_FINISH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       WebSessionId
    );

/*
 * HTTP MDH interface is defined to give users ability to save upload
 * file content to whatever they want. In this way, HTTP server doesn't
 * have to save 'big' file content in memory. In case to support big
 * file upload, this will save a lot of dynamic memory usage.
 */
#define  HTTP_MDH_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPMDHIF_BEGIN             Begin;                                                  \
    PFN_HTTPMDHIF_RECV              DataReceived;                                           \
    PFN_HTTPMDHIF_SAVE_AS           SaveFileAs;                                             \
    PFN_HTTPMDHIF_FINISH            Finish;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_MDH_INTERFACE
{
    HTTP_MDH_INTERFACE_CLASS_CONTENT
}
HTTP_MDH_INTERFACE,  *PHTTP_MDH_INTERFACE;

#define  ACCESS_HTTP_MDH_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_MDH_INTERFACE, Linkage)


#endif

