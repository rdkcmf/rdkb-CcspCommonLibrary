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
