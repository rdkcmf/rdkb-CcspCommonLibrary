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

    module:	web_upo_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Uri Path Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/02    initial revision.

**********************************************************************/


#ifndef  _WEB_UPO_INTERFACE_
#define  _WEB_UPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "web_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT WEB URI PATH OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  WEB_UPO_PATH_NAME_SIZE                     128
#define  WEB_UPO_UPO_TABLE_SIZE                     16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBUPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBUPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBUPO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  char*
(*PFN_WEBUPO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_WEBUPO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_WEBUPO_GET_UPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_HANDLE
(*PFN_WEBUPO_ADD_UPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_DEL_UPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_WEBUPO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The HTTP protocol does not place any a priori limit on the length of a URI. Servers MUST be able
 * to handle the URI of any resource they serve, and SHOULD be able to handle URIs of unbounded
 * length if they provide GET-based forms that could generate such URIs. A server SHOULD return 414
 * (Request-URI Too Long) status if a URI is longer than the server can handle.
 *
 * The "http" scheme is used to locate network resources via the HTTP protocol:
 *
 *      http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
 *
 * If the port is empty or not given, port 80 is assumed. The semantics are that the identified
 * resource is located at the server listening for TCP connections on that port of that host, and
 * the Request-URI for the resource is abs_path. The use of IP addresses in URLs SHOULD be avoided
 * whenever possible (see RFC 1900). If the abs_path is not present in the URL, it MUST be given as
 * "/" when used as a Request-URI for a resource. If a proxy receives a host name which is not a
 * fully qualified domain name, it MAY add its domain to the host name it received. If a proxy
 * receives a fully qualified domain name, the proxy MUST NOT change the host name.
 */
#define  WEB_URI_PATH_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    char                            PathName[WEB_UPO_PATH_NAME_SIZE];                       \
    ANSC_HANDLE                     hResourceOwner;                                         \
    ULONG                           HashIndex;                                              \
                                                                                            \
    QUEUE_HEADER                    UpoTable[WEB_UPO_UPO_TABLE_SIZE];                       \
    ANSC_LOCK                       UpoTableLock;                                           \
                                                                                            \
    PFN_WEBUPO_IS_SOMETHING         IsRemoveable;                                           \
    PFN_WEBUPO_GET_NAME             GetPathName;                                            \
    PFN_WEBUPO_SET_NAME             SetPathName;                                            \
    PFN_WEBUPO_GET_CONTEXT          GetResourceOwner;                                       \
    PFN_WEBUPO_SET_CONTEXT          SetResourceOwner;                                       \
    PFN_WEBUPO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBUPO_GET_UPO              GetUriPath;                                             \
    PFN_WEBUPO_ADD_UPO              AddUriPath;                                             \
    PFN_WEBUPO_DEL_UPO              DelUriPath;                                             \
    PFN_WEBUPO_DEL_ALL              DelAllUriPaths;                                         \
    /* end of object class content */                                                       \

typedef  struct
_WEB_URI_PATH_OBJECT
{
    WEB_URI_PATH_CLASS_CONTENT
}
WEB_URI_PATH_OBJECT,  *PWEB_URI_PATH_OBJECT;

#define  ACCESS_WEB_URI_PATH_OBJECT(p)              \
         ACCESS_CONTAINER(p, WEB_URI_PATH_OBJECT, Linkage)


#endif
