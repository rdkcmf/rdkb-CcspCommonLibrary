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

    module:	slap_bmc2como_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Command Object.

        *   SlapBmc2ComoGetCommandName
        *   SlapBmc2ComoGetParamByIndex
        *   SlapBmc2ComoRemoveAllParams
        *   SlapBmc2ComoSetParamByIndex
        *   SlapBmc2ComoIsAborted

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#include "slap_bmc2como_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapBmc2ComoGetCommandName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the command name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     command name.

**********************************************************************/

char*
SlapBmc2ComoGetCommandName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PBMC2_COMMAND_PROPERTY          pBmc2CommandProperty = (PBMC2_COMMAND_PROPERTY     )pBmc2ReqController->hCommandProperty;

    if ( !pBmc2CommandProperty )
    {
        return  NULL;
    }

    return  AnscCloneString(pBmc2CommandProperty->CommandName);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapBmc2ComoGetParamByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 param_index
            );

    description:

        This function is called to retrieve an input parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 param_index
                Specifies the index of the parameter to be retrieved.

    return:     parameter variable.

**********************************************************************/

SLAP_VARIABLE*
SlapBmc2ComoGetParamByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 param_index
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PSLAP_VARIABLE                  pReturnedVar         = (PSLAP_VARIABLE             )NULL;

    if ( param_index >= pBmc2ReqController->CliArgumentCount )
    {
        return  NULL;
    }
    else
    {
        pReturnedVar = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pReturnedVar )
        {
            return  NULL;
        }
        else
        {
            SlapInitVariable(pReturnedVar);
        }
    }

    if ( TRUE )
    {
        pReturnedVar->Name              = NULL;
        pReturnedVar->ContentType       = SLAP_CONTENT_TYPE_UNSPECIFIED;
        pReturnedVar->UsageType         = 0;
        pReturnedVar->Syntax            = SLAP_VAR_SYNTAX_string;
        pReturnedVar->Variant.varString = AnscCloneString(pBmc2ReqController->CliArgumentArray[param_index]);
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2ComoRemoveAllParams
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to remove all parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     parameter variable.

**********************************************************************/

ANSC_STATUS
SlapBmc2ComoRemoveAllParams
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;

    return  pBmc2ReqController->RemoveCliArguments((ANSC_HANDLE)pBmc2ReqController);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2ComoSetParamByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 param_index,
                char*                       param_value
            )

    description:

        This function is called to set an parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 param_index
                Specifies the index of the parameter to be retrieved.

                char*                       param_value
                Specifies the parameter value to be set.

    return:     parameter variable.

**********************************************************************/

ANSC_STATUS
SlapBmc2ComoSetParamByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 param_index,
        char*                       param_value
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    ULONG                           ulNewItemCount       = 0;
    ULONG                           i;

    if ( param_index >= pBmc2ReqController->CliArgumentCount )
    {
        ulNewItemCount = param_index - pBmc2ReqController->CliArgumentCount + 1;

        for ( i = 0; i < ulNewItemCount; i ++ )
        {
            pBmc2ReqController->AddCliArgument((ANSC_HANDLE)pBmc2ReqController, NULL);
        }
    }

    if ( param_index >= pBmc2ReqController->CliArgumentCount )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    if ( pBmc2ReqController->CliArgumentArray[param_index] )
    {
        AnscFreeMemory(pBmc2ReqController->CliArgumentArray[param_index]);
        pBmc2ReqController->CliArgumentArray[param_index] = NULL;
    }

    pBmc2ReqController->CliArgumentArray[param_index] = AnscCloneString(param_value);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_BOOL
        SlapBmc2ComoIsAborted
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the command has been
        aborted.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

SLAP_BOOL
SlapBmc2ComoIsAborted
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;

    return  pBmc2ReqController->bAborted;
}
