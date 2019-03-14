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

    module:	web_ifo_hem.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Web Host Entity Manager (HEM) Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/02    initial revision.

**********************************************************************/


#ifndef  _WEB_IFO_HEM_
#define  _WEB_IFO_HEM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
     WEB HOST ENTITY MANAGER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  WEB_HEM_INTERFACE_NAME                     "webHostEntityManagerIf"
#define  WEB_HEM_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_WEBHEMIF_ATTACH1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHost
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_DETACH1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_DELETE1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_ATTACH2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        ANSC_HANDLE                 hWebRoo
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_DETACH2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        char*                       path_name
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_DELETE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        char*                       path_name
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_GET_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulMajorVersion,
        PULONG                      pulMinorVersion
    );

typedef  ANSC_HANDLE
(*PFN_WEBHEMIF_GET_OBJ)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_WEBHEMIF_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_REGISTER_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef	 ANSC_STATUS
(*PFN_WEBHEMIF_SET_HTTP_AUTH)
	(
		ANSC_HANDLE					hThisObject,
		char*						host_name,
		USHORT						host_port,
        char*                       path,
		ULONG						http_auth_method,   /* look into http_definitions.h for HTTP_AUTH_TYPE_xxx */
        ANSC_HANDLE                 hWebAcmIf
	);

typedef  ANSC_STATUS
(*PFN_WEBHEMIF_SET_HTTP_AUTH2)
    (
		ANSC_HANDLE					hThisObject,
		char*						host_name,
		USHORT						host_port,
        char*                       path,
        ANSC_HANDLE                 hWebAcmIf,
		ULONG						http_auth_method,   /* look into http_definitions.h for HTTP_AUTH_TYPE_xxx */
        ULONG                       alg,                /* one of HTTP_AUTH_ALGORITHM_xxx */
        BOOL                        bNoQop,
        ULONG                       NonceTimeout,       /* nonce timeout value in seconds */
        char*                       realm,              /* realm name */
        char*                       domain              /* domain name */
    );

/*
 * Currently, HTTP servers respond to each client request without relating that request to previous
 * or subsequent requests; the technique allows clients and servers that wish to exchange state
 * information to place HTTP requests and responses within a larger context, which we term a
 * "session". This context might be used to create, for example, a "shopping cart", in which user
 * selections can be aggregated before purchase, or a magazine browsing system, in which a user's
 * previous reading effects which offerings are presented.
 */
#define  WEB_HEM_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_WEBHEMIF_ATTACH1            AttachWebVho;                                           \
    PFN_WEBHEMIF_DETACH1            DetachWebVho;                                           \
    PFN_WEBHEMIF_DELETE1            DeleteWebVho;                                           \
    PFN_WEBHEMIF_ATTACH2            AttachWebRoo;                                           \
    PFN_WEBHEMIF_DETACH2            DetachWebRoo;                                           \
    PFN_WEBHEMIF_DELETE2            DeleteWebRoo;                                           \
                                                                                            \
    PFN_WEBHEMIF_GET_VERSION        GetHttpVersion;                                         \
    PFN_WEBHEMIF_GET_OBJ            GetHttpHco;                                             \
    PFN_WEBHEMIF_GET_IF             GetHttpHfpIf;                                           \
    PFN_WEBHEMIF_REGISTER_IF        RegisterHfpIf;                                          \
																							\
	PFN_WEBHEMIF_SET_HTTP_AUTH		SetHttpAuth;											\
    PFN_WEBHEMIF_SET_HTTP_AUTH2     SetHttpAuth2;                                           \
    /* end of object class content */                                                       \

typedef  struct
_WEB_HEM_INTERFACE
{
    WEB_HEM_INTERFACE_CLASS_CONTENT
}
WEB_HEM_INTERFACE,  *PWEB_HEM_INTERFACE;

#define  ACCESS_WEB_HEM_INTERFACE(p)                \
         ACCESS_CONTAINER(p, WEB_HEM_INTERFACE, Linkage)


#endif
