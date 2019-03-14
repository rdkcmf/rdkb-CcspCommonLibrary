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

    module:	web_properties.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Web Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#ifndef  _WEB_PROPERTIES_
#define  _WEB_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Web message definitions, so we
 * include the header file here.
 */
#include  "web_definitions.h"


/***********************************************************
   WEB SIMPLE SERVER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * Web Server is an application program that accepts connections in order to service requests by
 * sending back responses. Any given program may be capable of being both a client and a server;
 * our use of these terms refers only to the role being performed by the program for a particular
 * connection, rather than to the program's capabilities in general. Likewise, any server may act
 * as an origin server, proxy, gateway, or tunnel, switching behavior based on the nature of each
 * request.
 */
#define  WEB_SERVER_TYPE_SMALL                      HTTP_SERVER_TYPE_SMALL
#define  WEB_SERVER_TYPE_MEDIUM                     HTTP_SERVER_TYPE_MEDIUM
#define  WEB_SERVER_TYPE_BIG                        HTTP_SERVER_TYPE_BIG
#define  WEB_SERVER_TYPE_GIANT                      HTTP_SERVER_TYPE_GIANT

#define  WEB_SERVER_FLAG_CLEAR                      0x00000001
#define  WEB_SERVER_FLAG_TLS                        0x00000002
#define  WEB_SERVER_FLAG_TLS_AUTH_CLIENT            0x00000004

#define  WEB_MAX_DAEMON_NUMBER                      8

typedef  struct
_WEB_HTTP_DAEMON_INFO
{
    ANSC_IPV4_ADDRESS               DaemonAddress;
    USHORT                          DaemonPort;
    ULONG                           ServerType;
    ULONG                           ServerFlag;
}
WEB_HTTP_DAEMON_INFO,  *PWEB_HTTP_DAEMON_INFO;

typedef  struct
_WEB_SIMPLE_SERVER_PROPERTY
{
    ULONG                           HttpDaemonCount;
    WEB_HTTP_DAEMON_INFO            HttpDaemonArray[WEB_MAX_DAEMON_NUMBER];

    ULONG                           HttpServerMode;         /* as defined in http_properties.h */
    ULONG                           HttpReqBodySizeCap;     /* as defined in http_properties.h */
    ULONG                           HttpMaxBodySizeCap;     /* as defined in http_properties.h */
}
WEB_SIMPLE_SERVER_PROPERTY,  *PWEB_SIMPLE_SERVER_PROPERTY;


/***********************************************************
    WEB SITE MANAGER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * An origin server that does differentiate resources based on the host requested (sometimes
 * referred to as virtual hosts or vanity host names) MUST use the following rules for determining
 * the requested resource on an HTTP/1.1 request:
 *
 *      1. If Request-URI is an absoluteURI, the host is part of the Request-URI. Any
 *         Host header field value in the request MUST be ignored.
 *
 *      2. If the Request-URI is not an absoluteURI, and the request includes a Host
 *         header field, the host is determined by the Host header field value.
 *
 *      3. If the host as determined by rule 1 or 2 is not a valid host on the server,
 *         the response MUST be a 400 (Bad Request) error message.
 *
 * Recipients of an HTTP/1.0 request that lacks a Host header field MAY attempt to use heuristics
 * (e.g., examination of the URI path for something unique to a particular host) in order to
 * determine what exact resource is being requested.
 */
#define  WEB_SERVER_PRODUCT_NAME_SIZE               256
#define  WEB_MAX_SESSION_COUNT_FROM_ONE_PEER        0       /* no limit */

typedef  struct
_WEB_SITE_MANAGER_PROPERTY
{
    BOOL                            bSupportVirtualHosts;
    BOOL                            bAsyncProcessEnabled;
    BOOL                            bPersistentConn;
    char                            ProductName[WEB_SERVER_PRODUCT_NAME_SIZE];
    ULONG                           MaxSessionCountPerPeer;
}
WEB_SITE_MANAGER_PROPERTY,  *PWEB_SITE_MANAGER_PROPERTY;


/***********************************************************
    WEB VIRTUAL HOST CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

#define  WEB_MAX_HOST_NAME_SIZE                     128
#define  WEB_WILDCARD_HOST_NAME                     "*"
#define  WEB_DEF_SESSION_TIMEOUT                    60 * 20             /* in seconds - 20 minutes */

#define  WEB_DEF_VHO_COOKIE_MAX_AGE                 WEB_DEF_SESSION_TIMEOUT
#define  WEB_DEF_VHO_COOKIE_NAME                    "broadway-bisga-web-vho-id"
#define  WEB_DEF_VHO_COOKIE_COMMENT                 "general-session-id"

#define  WEB_DEF_LSM_COOKIE_MAX_AGE                 3600 * 24 * 30      /* in seconds - 30 days    */
#define  WEB_DEF_LSM_COOKIE_NAME                    "client-id"
#define  WEB_DEF_LSM_COOKIE_COMMENT                 "for-session-tracking"
#define  WEB_DEF_LSM_CLIENT_ID                      "anybody"

#define  WEB_SEESION_EXPIRED(st, ct, duration)                      \
    ( ((ct) >= (st)) ?                                              \
        ( (ct) - (st) >= (duration) ) :                             \
        ( 0xFFFFFFFF - (st) + (ct) >= (duration) ) )

typedef  struct
_WEB_VIRTUAL_HOST_PROPERTY
{
    char                            HostName[WEB_MAX_HOST_NAME_SIZE];
    USHORT                          HostPort;

    BOOL                            bDefaultVirtualHost;
    BOOL                            bEnableSessionTracking;
    ULONG                           SessionTimeOut;
    ULONG                           SessionMaxLifespan; /* if used, it controls max session duration, 0 means no limitation */

    ULONG                           VhoCookieMaxAge;
    ULONG                           LsmCookieMaxAge;
    char                            VhoCookieName[HTTP_MAX_COOKIE_NAME_SIZE];
    char                            LsmCookieName[HTTP_MAX_COOKIE_NAME_SIZE];
}
WEB_VIRTUAL_HOST_PROPERTY,  *PWEB_VIRTUAL_HOST_PROPERTY;


/***********************************************************
  WEB RESOURCE LOCATOR CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The most common form of Request-URI is that used to identify a resource on an origin server or
 * gateway. In this case the absolute path of the URI MUST be transmitted as the Request-URI, and
 * the network location of the URI (authority) MUST be transmitted in a Host header field.
 */
#define  WEB_MAX_PATH_LEVEL                         32

typedef  struct
_WEB_RESOURCE_LOCATOR_PROPERTY
{
    ULONG                           MaxPathLevel;
}
WEB_RESOURCE_LOCATOR_PROPERTY,  *PWEB_RESOURCE_LOCATOR_PROPERTY;

/*
 * The Request-URI is a Uniform Resource Identifier and identifies the resource upon which to apply
 * the request.
 *
 *      Request-URI = "*" | absoluteURI | abs_path | authority
 *
 * The four options for Request-URI are dependent on the nature of the request. The asterisk "*"
 * means that the request does not apply to a particular resource, but to the server itself, and is
 * only allowed when the method used does not necessarily apply to a resource.
 */
#define  WEB_MAX_PATH_NAME_SIZE                     256
#define  WEB_DEF_ROOT_PATH                          "/"

#define  WEB_ROO_TYPE_RESERVED                      0
#define  WEB_ROO_TYPE_FILE_SYSTEM                   1
#define  WEB_ROO_TYPE_GENERAL_CGI                   2

#define  WEB_ROO_FLAG_STANDARD                      0x00000001
#define  WEB_ROO_FLAG_DEFAULT                       0x00000002
#define  WEB_ROO_FLAG_UPNP_SUPPORT                  0x00000004

#define  WEB_ROO_AUTH_TYPE_RESERVED                 0
#define  WEB_ROO_AUTH_TYPE_NONE                     1
#define  WEB_ROO_AUTH_TYPE_BASIC                    2
#define  WEB_ROO_AUTH_TYPE_DIGEST                   3

#define  WEB_RESOURCE_OWNER_PROPERTY_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    /* start of object class content */                                                     \
    char                            RootPath[ANSC_MAX_FILE_NAME_SIZE];                      \
    char                            RegPath [WEB_MAX_PATH_NAME_SIZE ];                      \
    ULONG                           OwnerFlag;                                              \
    ULONG                           OwnerType;                                              \
    ULONG                           AuthType;                                               \
    /* end of object class content */                                                       \

typedef  struct
_WEB_RESOURCE_OWNER_PROPERTY
{
    WEB_RESOURCE_OWNER_PROPERTY_CONTENT
}
WEB_RESOURCE_OWNER_PROPERTY,  *PWEB_RESOURCE_OWNER_PROPERTY;

/*
 * The Roo FSM (File System Mapper) Object maps a HTTP request URI directly to a disk file, which
 * is the service that web applications were designed to provide. We provide a simple File Mapping
 * object to demostrate this ability.
 */
#define  WEB_ROO_FSM_PROPERTY_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    WEB_RESOURCE_OWNER_PROPERTY_CONTENT                                                     \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_WEB_ROO_FSM_PROPERTY
{
    WEB_ROO_FSM_PROPERTY_CONTENT
}
WEB_ROO_FSM_PROPERTY,  *PWEB_ROO_FSM_PROPERTY;


/***********************************************************
 WEB AUTH SERVER OBJECT CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * This structure defines the properties used to control/perform HTTP authentication
 * on server side. 
 */

#define  WEB_AUTH_ALGORITHM_MD5                     0
#define  WEB_AUTH_ALGORITHM_MD5_SESS                1

#define  WEB_AUTH_NONCE_TIMEOUT_INTERVAL            0           /* never expire */

#define  WEB_AUTH_REALM_NAME_MAXLEN                 64
#define  WEB_AUTH_DOMAIN_MAXLEN                     256

#define  WEB_AUTH_SERVER_PROPERTY_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    /* start of object class content */                                                     \
    ULONG                           AuthType;                                               \
    ULONG                           Algorithm;                                              \
    BOOL                            bNoQop;                                                 \
    ULONG                           NonceTimeout;                                           \
    UCHAR                           Realm [WEB_AUTH_REALM_NAME_MAXLEN + 1];                 \
    UCHAR                           Domain[WEB_AUTH_DOMAIN_MAXLEN     + 1];                 \
    /* end of object class content */                                                       \

typedef  struct
_WEB_AUTH_SERVER_PROPERTY
{
    WEB_AUTH_SERVER_PROPERTY_CONTENT
}
WEB_AUTH_SERVER_PROPERTY,  *PWEB_AUTH_SERVER_PROPERTY;


#define  WEB_AUTH_SESS_INFO_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    /* start of object class content */                                                     \
    ULONG                           NC;                                                     \
    ULONG                           NonceTimestamp;                                         \
    UCHAR                           Nonce[ANSC_MD5_OUTPUT_SIZE * 2 +1];                     \
                                                                                            \
    WEB_AUTH_SERVER_PROPERTY        ServerAuthInfo;                                         \
    /* end of object class content */                                                       \

typedef  struct
_WEB_AUTH_SESSION_INFO
{
    WEB_AUTH_SESS_INFO_CONTENT
}
WEB_AUTH_SESSION_INFO,     *PWEB_AUTH_SESSION_INFO;


#endif
