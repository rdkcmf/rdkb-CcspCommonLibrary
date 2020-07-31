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

    module:	slap_bmc2termo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Bmc2 Terminal Object.

        *   SlapBmc2TermoGetSlapGoaIf
        *   SlapBmc2TermoGetOlmContainer
        *   SlapBmc2TermoReset

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


#include "slap_bmc2termo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapBmc2TermoGetSlapGoaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapBmc2TermoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pBmc2ComTerminal->hBmc2EnvController;

    return  pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapBmc2TermoGetOlmContainer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapBmc2TermoGetOlmContainer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject          = (PSLAP_BMC2_TERMINAL_OBJECT )hThisObject;
    PBMC2_COM_TERMINAL_OBJECT       pBmc2ComTerminal   = (PBMC2_COM_TERMINAL_OBJECT  )pMyObject->hInsContext;

    return  pBmc2ComTerminal->hSlapContainerTerminal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapBmc2TermoReset
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
SlapBmc2TermoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_TERMINAL_OBJECT      pMyObject        = (PSLAP_BMC2_TERMINAL_OBJECT)hThisObject;

    SlapScoGalReset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
