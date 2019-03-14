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

    module:	dslg_definitions_user.h

        For DSL Gateway Model Implementation (DSLG),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Dslg Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/20/05    initial revision.

**********************************************************************/


#ifndef  _DSLG_DEFINITIONS_USER_
#define  _DSLG_DEFINITIONS_USER_

#define  DSLG_USER_GROUP_NAME_SIZE                  32
#define  DSLG_USERNAME_SIZE                         128
#define  DSLG_PASSWORD_SIZE                         128

/***********************************************************
      DSLG USER MANAGEMENT MODEL PARAMETERS DEFINITION
***********************************************************/

/*
 * The administrator may create one or more Policy Groups to control the access for specific users
 * or groups of users. By default, Dslg will contain a single Default Policy Group with a default
 * set of privileges (which can be changed - See Section 14.1) for users within the group. Unless
 * configured otherwise via the local user database or RADIUS, all authenticated users will be
 * placed in the Default Policy Group.
 */
#define  DSLG_USER_PERMISSION_deviceManagement      0x00000001
#define  DSLG_USER_PERMISSION_vpnClientAccess       0x00000002
#define  DSLG_USER_PERMISSION_internetAccess        0x00000004
#define  DSLG_USER_PERMISSION_bypassContentFilter   0x00000008

/*
 *  Definitions of User Session
 */
#define  DSLG_USER_SESSION_DefDevLogInactTimeout     300     /* in seconds */
#define  DSLG_USER_SESSION_DefDevLockoutTimeout      300     /* in seconds */
#define  DSLG_USER_SESSION_DefUserLoginInactTimeout  900     /* in seconds */
#define  DSLG_USER_SESSION_DefUserMaxSessionTime     3600    /* in seconds */


typedef  struct
_DSLG_USER_GROUP
{
    char                            GroupName[DSLG_USER_GROUP_NAME_SIZE];
    ULONG                           Permissions;
}
DSLG_USER_GROUP,  *PDSLG_USER_GROUP;

/*
 * Dslg will maintain a local database containing usernames and their corresponding passwords. This
 * database will be used in conjunction with external databases, such as RADIUS to authenticate a
 * particular user's right to access a service.
 */
typedef  struct
_DSLG_LOCAL_USER
{
    char                            UserName [DSLG_USERNAME_SIZE       ];
    char                            Password [DSLG_PASSWORD_SIZE       ];
    char                            GroupName[DSLG_USER_GROUP_NAME_SIZE];
}
DSLG_LOCAL_USER,  *PDSLG_LOCAL_USER;

/*
 * Dslg will support two RADIUS servers, the primary and secondary authentication servers. Dslg
 * will not initially support RADIUS accounting. When the RADIUS servers are configured, they are
 * both marked as "responsiive". The primary is queried first. If this request times out then it is
 * marked as "unresponsive", and the second server is queried. If all the servers are to be marked
 * as "unresponsive", then both are reset to "responsive".
 */
typedef  struct
_DSLG_RADIUS_PARAMS
{
    BOOL                            bEnabled;
    ANSC_IPV4_ADDRESS               ServerAddr1;
    ULONG                           ServerPort1;
    char                            ServerSecret1[DSLG_PASSWORD_SIZE];
    ANSC_IPV4_ADDRESS               ServerAddr2;
    ULONG                           ServerPort2;
    char                            ServerSecret2[DSLG_PASSWORD_SIZE];
    ULONG                           ServerTimeout;
    ULONG                           ServerRetries;
    char                            GroupName[DSLG_USER_GROUP_NAME_SIZE];
}
DSLG_RADIUS_PARAMS,  *PDSLG_RADIUS_PARAMS;


#endif
