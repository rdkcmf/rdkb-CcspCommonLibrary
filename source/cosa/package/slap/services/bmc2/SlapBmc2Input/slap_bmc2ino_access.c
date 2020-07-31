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

    module:	slap_bmc2ino_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Bmc2 Input Object.

        *   SlapBmc2InoRead
        *   SlapBmc2InoRead2

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


#include "slap_bmc2ino_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapBmc2InoRead
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to read user input from the console.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     user input.

**********************************************************************/

char*
SlapBmc2InoRead
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_INPUT_OBJECT         pMyObject          = (PSLAP_BMC2_INPUT_OBJECT    )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain     = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ReqController->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PBMC2_ICE_INTERFACE             pBmc2IceIf         = (PBMC2_ICE_INTERFACE        )pBmc2ComTerminal->GetBmc2IceIf((ANSC_HANDLE)pBmc2ComTerminal);
    char*                           pUserInput         = NULL;

    if ( !pBmc2IceIf )
    {
        return  NULL;
    }

        pBmc2IceIf->ReadInput
            (
                pBmc2IceIf->hOwnerContext,
                &pUserInput
            );


    return  pUserInput;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapBmc2InoRead2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pDefaultValue,
                SLAP_BOOL                   bSizeFixed,
                SLAP_BOOL                   bMaskInput,
                SLAP_UINT32                 ulWidth
            );

    description:

        This function is called to read user input from the console.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pDefaultValue
                Specifies the default value to be printed.

                SLAP_BOOL                   bSizeFixed
                Specifies whether the input field is fixed.

                SLAP_BOOL                   bMaskInput
                Specifies whether the input should be masked.

                SLAP_UINT32                 ulWidth
                Specifies the size of the input field.

    return:     user input.

**********************************************************************/

char*
SlapBmc2InoRead2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDefaultValue,
        SLAP_BOOL                   bSizeFixed,
        SLAP_BOOL                   bMaskInput,
        SLAP_UINT32                 ulWidth
    )
{
    PSLAP_BMC2_INPUT_OBJECT         pMyObject           = (PSLAP_BMC2_INPUT_OBJECT    )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController  = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain      = (PBMC2_COM_DOMAIN_OBJECT    )pBmc2ReqController->hBmc2ComDomain;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal    = (PBMC2_COM_TERMINAL_OBJECT  )pBmc2ComDomain->hBmc2ComTerminal;
    PBMC2_ICE_INTERFACE             pBmc2IceIf          = (PBMC2_ICE_INTERFACE        )pBmc2ComTerminal->GetBmc2IceIf((ANSC_HANDLE)pBmc2ComTerminal);
    PBMC2_ICE_TEXTBOX_INFO          pBmc2IceTextboxInfo = (PBMC2_ICE_TEXTBOX_INFO     )NULL;
    char*                           pUserInput          = NULL;

    if ( !pBmc2IceIf )
    {
        return  NULL;
    }
    else
    {
        pBmc2IceTextboxInfo = (PBMC2_ICE_TEXTBOX_INFO)AnscAllocateMemory(sizeof(BMC2_ICE_TEXTBOX_INFO));
    }

    if ( !pBmc2IceTextboxInfo )
    {
        return  NULL;
    }
    else
    {
        pBmc2IceTextboxInfo->Name         = NULL;
        pBmc2IceTextboxInfo->DefaultValue = AnscCloneString(pDefaultValue);
        pBmc2IceTextboxInfo->bSizeFixed   = bSizeFixed;
        pBmc2IceTextboxInfo->bMaskInput   = bMaskInput;
        pBmc2IceTextboxInfo->Width        = ulWidth;
    }

        pBmc2IceIf->ReadInput2
            (
                pBmc2IceIf->hOwnerContext,
                (ANSC_HANDLE)pBmc2IceTextboxInfo,
                &pUserInput
            );

    AnscFreeMemory(pBmc2IceTextboxInfo);

    return  pUserInput;
}
