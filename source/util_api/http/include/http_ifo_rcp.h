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

    module:	http_ifo_rcp.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Request Context Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/15/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_RCP_
#define  _HTTP_IFO_RCP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  HTTP REQUEST CONTEXT PROVIDER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  char*
(*PFN_HTTPRCPIF_GET_ROOT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_PATH_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_PATH_TRAN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_USER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_SCRIPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  USHORT
(*PFN_HTTPRCPIF_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_PROTOCOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  char*
(*PFN_HTTPRCPIF_GET_SOFTWARE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

/*
 * While the server-side processing is on request-by-request basis, the reply status and content
 * generated as the result of the processing is usually part of a much bigger context. As standard
 * practice, we SHALL encapsulate all the state information within a single context object and
 * make it available to the processor objects. However, the legacy CGI programming exercise brings
 * up a very interesting issue: how do we best present both message- and context-access functions
 * via single interface?
 */
#define  HTTP_RCP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPRCPIF_GET_ROOT          GetDocumentRoot;                                        \
    PFN_HTTPRCPIF_GET_PATH_INFO     GetPathInfo;                                            \
    PFN_HTTPRCPIF_GET_PATH_TRAN     GetPathTranslated;                                      \
    PFN_HTTPRCPIF_GET_USER          GetRemoteUser;                                          \
    PFN_HTTPRCPIF_GET_SCRIPT        GetScriptName;                                          \
    PFN_HTTPRCPIF_GET_NAME          GetServerName;                                          \
    PFN_HTTPRCPIF_GET_PORT          GetServerPort;                                          \
    PFN_HTTPRCPIF_GET_PROTOCOL      GetServerProtocol;                                      \
    PFN_HTTPRCPIF_GET_SOFTWARE      GetServerSoftware;                                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_RCP_INTERFACE
{
    HTTP_RCP_INTERFACE_CLASS_CONTENT
}
HTTP_RCP_INTERFACE,  *PHTTP_RCP_INTERFACE;

#define  ACCESS_HTTP_RCP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_RCP_INTERFACE, Linkage)


#endif
