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

    module:	bmc2_comto_internal_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bmc2 Com Terminal
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMTO_INTERNAL_API_
#define  _BMC2_COMTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN BMC2_COMTO_STATES.C
***********************************************************/

ANSC_HANDLE
Bmc2ComtoGetBmc2IceIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetBmc2IceIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
Bmc2ComtoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    );

char*
Bmc2ComtoGetUserIdentifier
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetUserIdentifier
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    );

ANSC_STATUS
Bmc2ComtoSetPseudoTermDevName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPseudoTermDevName
    );

ULONG
Bmc2ComtoGetUserPermission
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetUserPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPermission
    );

ULONG
Bmc2ComtoGetMaxLineNumber
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetMaxLineNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulLineNumber
    );

ULONG
Bmc2ComtoGetMaxColumnNumber
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetMaxColumnNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulColumnNumber
    );

BOOL
Bmc2ComtoGetReadOnly
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetReadOnly
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

ANSC_STATUS
Bmc2ComtoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2ComtoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2ComtoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BMC2_COMTO_OPERATION.C
***********************************************************/

ANSC_STATUS
Bmc2ComtoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN BMC2_COMTO_MANAGEMENT.C
***********************************************************/

ULONG
Bmc2ComtoGetDomainCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ComtoGetCurComDomain
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoAddComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    );

ANSC_STATUS
Bmc2ComtoDelCurComDomain
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComtoDelComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    );

ANSC_STATUS
Bmc2ComtoDelAllComDomains
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
