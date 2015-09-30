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

    module:	bwsp_properties.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Bwsp Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_PROPERTIES_
#define  _BWSP_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Bwsp message definitions, so we
 * include the header file here.
 */
#include  "bwsp_definitions.h"
#include  "web_properties.h"


/***********************************************************
    BWSP SITE CREATOR CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The BWSP Site Creator Object serves the role as a central controller for the management web site.
 * It's responsible for setting up the web operation environment and creating the host object and
 * the resource owner objects accordingly. It also serves as a functional container for all the
 * other BWSP objects as well.
 */
#define  BWSP_SESSION_TIMEOUT                       WEB_DEF_SESSION_TIMEOUT
#define  BWSP_PHO_COOKIE_MAX_AGE                    WEB_DEF_SESSION_TIMEOUT
#define  BWSP_PHO_COOKIE_NAME                       "bwsp-pho-id"
#define  BWSP_PHO_COOKIE_COMMENT                    "bwsp-general-session-id"

#define  BWSP_LSM_COOKIE_MAX_AGE                    WEB_DEF_LSM_COOKIE_MAX_AGE
#define  BWSP_LSM_COOKIE_NAME                       "bwsp-lsm"
#define  BWSP_LSM_COOKIE_COMMENT                    "bwsp-session-tracking"

typedef  struct
_BWSP_SITE_CREATOR_PROPERTY
{
    ULONG                           SessionTimeout;
    ULONG                           PhoCookieMaxAge;
    ULONG                           LsmCookieMaxAge;
    char                            PhoCookieName[HTTP_MAX_COOKIE_NAME_SIZE];
    char                            LsmCookieName[HTTP_MAX_COOKIE_NAME_SIZE];
}
BWSP_SITE_CREATOR_PROPERTY,  *PBWSP_SITE_CREATOR_PROPERTY;


/***********************************************************
    BWSP PORTAL HOST CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Host request-header field specifies the Internet host and port number of the resource being
 * requested, as obtained from the original URI given by the user or referring resource (generally
 * an HTTP URL). The Host field value MUST represent the naming authority of the origin server or
 * gateway given by the original URL. This allows the origin server or gateway to differentiate be-
 * tween internally-ambiguous URLs, such as the root "/" URL of a server for multiple host names on
 * a single IP address.
 *
 *      Host = "Host" ":" host [ ":" port ] ;
 *
 * A "host" without any trailing port information implies the default port for the service
 * requested (e.g., "80" for an HTTP URL). For example, a request on the origin server for
 *
 *      <http://www.w3.org/pub/WWW/>
 *
 * would properly include:
 *
 *      GET /pub/WWW/ HTTP/1.1
 *      Host: www.w3.org
 *
 * A client MUST include a Host header field in all HTTP/1.1 request messages. If the requested URI
 * does not include an Internet host name for the service being requested, then the Host header
 * field MUST be given with an empty value. An HTTP/1.1 proxy MUST ensure that any request message
 * it forwards does contain an appropriate Host header field that identifies the service being
 * requested by the proxy. All Internet-based HTTP/1.1 servers MUST respond with a 400 (Bad Request)
 * status code to any HTTP/1.1 request message which lacks a Host header field.
 */
typedef  struct  _WEB_VIRTUAL_HOST_PROPERTY  BWSP_PORTAL_HOST_PROPERTY,  *PBWSP_PORTAL_HOST_PROPERTY;


/***********************************************************
    BWSP WEB SERVLET CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * This object is derived from the Web Resource Owner Object, which is implemented by the baseline
 * Web module. Here we merely use this object as a placeholder and the starting point for laying
 * down the rest web-based management framework. This object MUST be explicitly registered to the
 * web server during environment setup.
 */
typedef  struct  _WEB_RESOURCE_OWNER_PROPERTY  BWSP_WEB_SERVLET_PROPERTY,  *PBWSP_WEB_SERVLET_PROPERTY;


#endif
