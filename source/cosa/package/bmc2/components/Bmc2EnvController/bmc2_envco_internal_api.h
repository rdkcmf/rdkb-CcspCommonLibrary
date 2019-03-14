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

    module:	bmc2_envco_internal_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bmc2 Env Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_ENVCO_INTERNAL_API_
#define  _BMC2_ENVCO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN BMC2_ENVCO_STATES.C
***********************************************************/

ANSC_HANDLE
Bmc2EnvcoGetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2EnvcoGetBmc2ComExecutor
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2EnvcoGetBwrmEnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2EnvcoGetBwrmRamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2EnvcoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoSetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
Bmc2EnvcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
Bmc2EnvcoGetSlapContainerServices
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoSetSlapContainerServices
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

void
Bmc2EnvcoSetPseudoTermDevName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPseudoTermDevName
    );

ANSC_STATUS
Bmc2EnvcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2EnvcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2EnvcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BMC2_ENVCO_OPERATION.C
***********************************************************/

ANSC_STATUS
Bmc2EnvcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2EnvcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN BMC2_ENVCO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
Bmc2EnvcoGetComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    );

ANSC_STATUS
Bmc2EnvcoAddComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    );

ANSC_STATUS
Bmc2EnvcoDelComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComTerminal
    );

ANSC_STATUS
Bmc2EnvcoDelAllComTerminals
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2EnvcoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainName
    );

ANSC_STATUS
Bmc2EnvcoAddCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty,
        ANSC_HANDLE                 hCommandInsertBefore
    );

ANSC_HANDLE
Bmc2EnvcoDelCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName,
        char*                       pDomainNamePattern
    );

ANSC_STATUS
Bmc2EnvcoDelAllCommandProperties
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN BMC2_ENVCO_SCCIF.C
***********************************************************/

ANSC_STATUS
Bmc2EnvcoSccSetRootInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRootPath,
        char*                       pRootDomainCommand
    );

ANSC_STATUS
Bmc2EnvcoSccSetMultiUserCtrl
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

ANSC_STATUS
Bmc2EnvcoSccAddCommand
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainNamePattern,
        char*                       pCommand,
        char*                       pScpPage,           /* could be NULL */
        BOOL                        bReadOnly,
        ULONG                       ulPermissions
    );

ANSC_HANDLE
Bmc2EnvcoSccAddTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminalInfo,
        ANSC_HANDLE                 hBmc2IceIf
    );

ANSC_STATUS
Bmc2EnvcoSccDelTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    );

ANSC_STATUS
Bmc2EnvcoSccGetCommands
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommandPrefix,     /* could be NULL */
        SLAP_STRING_ARRAY**         ppStringArray
    );

BOOL
Bmc2EnvcoSccIsCommandValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand,
        PBOOL                       pbAuthorized
    );

ANSC_STATUS
Bmc2EnvcoSccExecuteCommand
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        ANSC_HANDLE                 hCommandReq,
        ANSC_HANDLE                 hCommandRep
    );

ANSC_STATUS
Bmc2EnvcoSccAbortExecution
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    );

ANSC_HANDLE
Bmc2EnvcoSccGetCommandSyntax
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand
    );

#endif
