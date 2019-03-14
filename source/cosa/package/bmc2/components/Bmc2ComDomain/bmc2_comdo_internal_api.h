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

    module:	bmc2_comdo_internal_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bmc2 Com Domain
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


#ifndef  _BMC2_COMDO_INTERNAL_API_
#define  _BMC2_COMDO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN BMC2_COMDO_STATES.C
***********************************************************/

ANSC_HANDLE
Bmc2ComdoGetBmc2ComTerminal
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetBmc2ComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
Bmc2ComdoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    );

ANSC_HANDLE
Bmc2ComdoGetBmc2ReqController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetBmc2ReqController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqController
    );

char*
Bmc2ComdoGetDomainTitle
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetDomainTitle
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainTitle
    );

ANSC_STATUS
Bmc2ComdoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BMC2_COMDO_OPERATION.C
***********************************************************/

ANSC_STATUS
Bmc2ComdoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN BMC2_COMDO_MANAGEMENT.C
***********************************************************/

SLAP_STRING_ARRAY*
Bmc2ComdoGetCommandsByPrefix
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandPrefix
    );

ANSC_HANDLE
Bmc2ComdoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    );

ANSC_STATUS
Bmc2ComdoAddCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty
    );

ANSC_STATUS
Bmc2ComdoDelCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandName
    );

ANSC_STATUS
Bmc2ComdoDelAllCommandProperties
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComdoSetCommandHelpSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommand,
        ULONG                       ulReqArgCount,
        char**                      pReqArgs,
        ULONG                       ulOptArgCount,
        char**                      pOptArgs
    );

PBMC2_CMD_ARGUMENT
Bmc2ComdoParseHelpSyntax
    (
        ULONG                       ulArgCount,
        char**                      pArgs
    );

BOOL
Bmc2ComdoParseSimpleArg
    (
        char*                       pArgSyntax,
        PBMC2_CMD_SIMPLE_ARG        pCmdSimpleArg
    );

BOOL
Bmc2ComdoParseDepArg
    (
        char*                       pArgSyntax,
        PBMC2_CMD_DEP_ARG           pCmdDepArg
    );

ULONG
Bmc2ComdoParseArgValueType
    (
        char*                       pType,
        ULONG                       ulLen,
        PULONG                      pulMinOccur,
        PULONG                      pulMaxOccur,
        PULONG                      pulMinLength,
        PULONG                      pulMaxLength,
		PBOOL						pbStrDupAllowed
    );

BOOL
Bmc2ComdoParseArgHelpItem
    (
        char*                       pHelp,
        ULONG                       ulLen,
        PBMC2_CMD_ARG_HELP          pHelpItem
    );

PBMC2_CMD_ARG_HELP
Bmc2ComdoParseArgHelpItems
    (
        char*                       pHelp,
        ULONG                       ulLen,
        PULONG                      pulItemCount
    );

PBMC2_CMD_ARG_VRANGE
Bmc2ComdoParseArgValueRange
    (
        ULONG                       ValueType,
        char*                       pVrange,
        ULONG                       ulLen,
        PBOOL                       pbSucc
    );


#endif
