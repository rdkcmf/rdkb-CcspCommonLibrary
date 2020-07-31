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

    module:	bmc2_reqco_states.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoGetBmc2PecIf
        *   Bmc2ReqcoGetBmc2ComDomain
        *   Bmc2ReqcoSetBmc2ComDomain
        *   Bmc2ReqcoGetBmc2EnvController
        *   Bmc2ReqcoSetBmc2EnvController
        *   Bmc2ReqcoGetCommandRequest
        *   Bmc2ReqcoSetCommandRequest
        *   Bmc2ReqcoGetCommandReply
        *   Bmc2ReqcoSetCommandReply
        *   Bmc2ReqcoGetCommandProperty
        *   Bmc2ReqcoSetCommandProperty
        *   Bmc2ReqcoGetCliArgumentCount
        *   Bmc2ReqcoGetCliArgument
        *   Bmc2ReqcoAddCliArgument
        *   Bmc2ReqcoAbort
        *   Bmc2ReqcoReset

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


#include "bmc2_reqco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetBmc2PecIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetBmc2PecIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hBmc2PecIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetBmc2ComDomain
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetBmc2ComDomain
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hBmc2ComDomain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetBmc2ComDomain
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hComDomain
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hComDomain
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetBmc2ComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hBmc2ComDomain = hComDomain;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetBmc2EnvController
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hBmc2EnvController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetBmc2EnvController
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEnvController
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEnvController
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hBmc2EnvController = hEnvController;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetCommandRequest
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetCommandRequest
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hCommandRequest;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetCommandRequest
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCommandReq
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCommandReq
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetCommandRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandReq
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hCommandRequest = hCommandReq;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetCommandReply
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetCommandReply
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hCommandReply;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetCommandReply
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCommandRep
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCommandRep
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetCommandReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandRep
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hCommandReply = hCommandRep;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetCommandProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hCommandProperty;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoSetCommandProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCommandProperty
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCommandProperty
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoSetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hCommandProperty = hCommandProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ReqcoGetCliArgumentCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
Bmc2ReqcoGetCliArgumentCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->CliArgumentCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        Bmc2ReqcoGetCliArgument
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                Specifies the cli input argument index.

    return:     object state.

**********************************************************************/

char*
Bmc2ReqcoGetCliArgument
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    if ( ulIndex >= pMyObject->CliArgumentCount )
    {
        return  NULL;
    }

    return  AnscCloneString(pMyObject->CliArgumentArray[ulIndex]);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAddCliArgument
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pArgument
            );

    description:

        This function is called to add a cli argument.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pArgument
                Specifies the cli argument to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAddCliArgument
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pArgument
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    if ( pMyObject->CliArgumentCount >= BMC2_REQCO_MAX_ARGUMENT_NUMBER )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    pMyObject->CliArgumentArray[pMyObject->CliArgumentCount++] = AnscCloneString(pArgument);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoRemoveCliArguments
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to remove all cli arguments.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoRemoveCliArguments
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    ULONG                           i;

    for ( i = 0; i < pMyObject->CliArgumentCount; i ++ )
    {
        if ( pMyObject->CliArgumentArray[i] )
        {
            AnscFreeMemory(pMyObject->CliArgumentArray[i]);
            pMyObject->CliArgumentArray[i] = NULL;
        }
    }

    pMyObject->CliArgumentCount = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAbort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to abort the request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAbort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->bAborted = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->ClearCcoQueue((ANSC_HANDLE)pMyObject);
    pMyObject->RemoveCliArguments((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
