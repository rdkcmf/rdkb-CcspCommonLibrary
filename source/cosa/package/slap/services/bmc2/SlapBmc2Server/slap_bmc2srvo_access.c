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

    module:	slap_bmc2srvo_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Server Object.

        *   SlapBmc2SrvoExecute
        *   SlapBmc2SrvoTransfer

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


#include "slap_bmc2srvo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2SrvoExecute
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to stop execution of the current page
        and transfers control to the page specified in page_path. The
        user's current environment (i.e. session state and any current
        transaction state) is carried over to the new page. After that
        page has finished execution, control passes back to the
        original page and execution resumes at the statement after the
        Execute method call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the path of the page to be executed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2SrvoExecute
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)AnscGetTaskStorageUnit("Bmc2ReqController");
    SLAP_PARAMETER_LIST*            pSlapParamList     = (SLAP_PARAMETER_LIST*       )NULL;

    if ( !pBmc2ReqController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pSlapParamList = (SLAP_PARAMETER_LIST*)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

        if ( !pSlapParamList )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            SlapInitParamList(pSlapParamList);
        }

        pSlapParamList->ParamCount                      = 1;
        pSlapParamList->ParamArray[0].Name              = NULL;
        pSlapParamList->ParamArray[0].ContentType       = 0;
        pSlapParamList->ParamArray[0].UsageType         = 0;
        pSlapParamList->ParamArray[0].Syntax            = SLAP_VAR_SYNTAX_string;
        pSlapParamList->ParamArray[0].Variant.varString = AnscCloneString(page_path);
    }

    returnStatus =
        pBmc2ReqController->AddControlCommand2
            (
                (ANSC_HANDLE)pBmc2ReqController,
                BMC2_REQCO_COMMAND_execute,
                pSlapParamList
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2SrvoTransfer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to stop execution of the current page
        and transfers control to the page specified in page_path. The
        user's current environment (i.e. session state and any current
        transaction state) is carried over to the new page. Unlike the
        Execute method, execution does not resume in the original page,
        but ends when the new page has completed executing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the path of the page to be executed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2SrvoTransfer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)AnscGetTaskStorageUnit("Bmc2ReqController");
    SLAP_PARAMETER_LIST*            pSlapParamList     = (SLAP_PARAMETER_LIST*       )NULL;

    if ( !pBmc2ReqController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pSlapParamList = (SLAP_PARAMETER_LIST*)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

        if ( !pSlapParamList )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            SlapInitParamList(pSlapParamList);
        }

        pSlapParamList->ParamCount                      = 1;
        pSlapParamList->ParamArray[0].Name              = NULL;
        pSlapParamList->ParamArray[0].ContentType       = 0;
        pSlapParamList->ParamArray[0].UsageType         = 0;
        pSlapParamList->ParamArray[0].Syntax            = SLAP_VAR_SYNTAX_string;
        pSlapParamList->ParamArray[0].Variant.varString = AnscCloneString(page_path);
    }

    returnStatus =
        pBmc2ReqController->AddControlCommand2
            (
                (ANSC_HANDLE)pBmc2ReqController,
                BMC2_REQCO_COMMAND_transfer,
                pSlapParamList
            );

    return  returnStatus;
}
