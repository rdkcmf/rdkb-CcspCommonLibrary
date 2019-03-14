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

    module:	cf_auth_msg_exported_api.h

        For Authentication Message object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Authentication 
        Message object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/


#ifndef  _CF_AUTH_MSG_EXPORTED_API_
#define  _CF_AUTH_MSG_EXPORTED_API_


/***********************************************************
        Authentication Request Message object
***********************************************************/

ANSC_HANDLE
CfARComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfARCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfARCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfARCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfARCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        Pattern Object (required by AP)
***********************************************************/

ANSC_HANDLE
CfPatternComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfPatternCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfPatternCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPatternCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPatternCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );



/***********************************************************
        PC Content Filtering Object
***********************************************************/

ANSC_HANDLE
CfPCFilterComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfPCFilterCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfPCFilterCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPCFilterCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPCFilterCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            PC Firewall Object
***********************************************************/

ANSC_HANDLE
CfPCFirewallComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfPCFirewallCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfPCFirewallCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPCFirewallCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfPCFirewallCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            Changing Password Object
***********************************************************/

ANSC_HANDLE
CfChangePassComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfChangePassCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfChangePassCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfChangePassCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfChangePassCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            Upgrade Message Object
***********************************************************/

ANSC_HANDLE
CfUpgradeMsgComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfUpgradeMsgCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfUpgradeMsgCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfUpgradeMsgCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfUpgradeMsgCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            Authentication Response Message
***********************************************************/

ANSC_HANDLE
CfAPComponentCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
CfAPCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
CfAPCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfAPCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CfAPCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
