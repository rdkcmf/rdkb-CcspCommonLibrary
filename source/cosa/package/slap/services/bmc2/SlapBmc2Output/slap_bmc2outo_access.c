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

    module:	slap_bmc2outo_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Output Object.

        *   SlapBmc2OutoClear
        *   SlapBmc2OutoWrite

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/29/05    initial revision.

**********************************************************************/


#include "slap_bmc2outo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2OutoClear
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clear the console screen.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2OutoClear
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_OUTPUT_OBJECT        pMyObject          = (PSLAP_BMC2_OUTPUT_OBJECT   )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ReqController->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PBMC2_ICE_INTERFACE             pBmc2IceIf         = (PBMC2_ICE_INTERFACE        )pBmc2ComTerminal->GetBmc2IceIf((ANSC_HANDLE)pBmc2ComTerminal);

    if ( !pBmc2IceIf )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    returnStatus = pBmc2IceIf->ClearScreen(pBmc2IceIf->hOwnerContext);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2OutoWrite
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pContent
            );

    description:

        This function is called to write content to the console screen.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pContent
                Specifies the content to be written to the console.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2OutoWrite
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pContent
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_OUTPUT_OBJECT        pMyObject          = (PSLAP_BMC2_OUTPUT_OBJECT   )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ReqController->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PBMC2_ICE_INTERFACE             pBmc2IceIf         = (PBMC2_ICE_INTERFACE        )pBmc2ComTerminal->GetBmc2IceIf((ANSC_HANDLE)pBmc2ComTerminal);

    if ( !pBmc2IceIf )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    returnStatus =
        pBmc2IceIf->Write
            (
                pBmc2IceIf->hOwnerContext,
                pContent
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2OutoEraseLine
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to erase current line.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2OutoEraseLine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_OUTPUT_OBJECT        pMyObject          = (PSLAP_BMC2_OUTPUT_OBJECT)hThisObject;

    returnStatus = pMyObject->MoveCursor((ANSC_HANDLE)pMyObject, "HOME");
    returnStatus = pMyObject->Write((ANSC_HANDLE)pMyObject, TELNET_ANSI_TTY_CODE_Erase_ToEnd);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2OutoMoveCursor
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pWhere
            );

    description:

        This function is called to move the cursor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pWhere
                Specifies where to move the cursor.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapBmc2OutoMoveCursor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pWhere          /* UP/DOWN/LEFT/RIGHT/HOME/END */
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_BMC2_OUTPUT_OBJECT        pMyObject          = (PSLAP_BMC2_OUTPUT_OBJECT)hThisObject;
    char*                           pCmd               = NULL;
    char*                           pCmd2              = NULL;
    int                             nSizeWhere         = AnscSizeOfString(pWhere);

    if ( nSizeWhere == 2 && AnscEqualString2(pWhere, "UP", 2, FALSE) )
    {
        pCmd = TELNET_ANSI_TTY_CODE_Cursor_Up;
    }
    else if ( nSizeWhere == 4 && AnscEqualString2(pWhere, "DOWN", 4, FALSE) )
    {
        pCmd = TELNET_ANSI_TTY_CODE_Cursor_Down;
    }
    else if ( nSizeWhere == 4 && AnscEqualString2(pWhere, "LEFT", 4, FALSE) )
    {
        pCmd = TELNET_ANSI_TTY_CODE_Cursor_Backward;
    }
    else if ( nSizeWhere == 5 && AnscEqualString2(pWhere, "RIGHT", 5, FALSE) )
    {
        pCmd = TELNET_ANSI_TTY_CODE_Cursor_Forward;
    }
    else if ( nSizeWhere == 4 && AnscEqualString2(pWhere, "HOME", 4, FALSE) )
    {
        pCmd  = TELNET_ANSI_TTY_CODE_ANSI_Cursor_Home;
        pCmd2 = TELNET_ANSI_TTY_CODE_VT100_Cursor_Home;
    }
    else if ( nSizeWhere == 3 && AnscEqualString2(pWhere, "END", 3, FALSE) )
    {
        pCmd  = TELNET_ANSI_TTY_CODE_ANSI_Cursor_End;
        pCmd2 = TELNET_ANSI_TTY_CODE_VT100_Cursor_End;
    }

    if ( pCmd != NULL )
    {
        returnStatus = pMyObject->Write((ANSC_HANDLE)pMyObject, pCmd);
    }

    if ( pCmd2 != NULL )
    {
        returnStatus = pMyObject->Write((ANSC_HANDLE)pMyObject, pCmd2);
    }

    return  returnStatus;
}

