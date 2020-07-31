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

    module:	http_psover2_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Pso Ver2 Object.

        *   HttpPsoVer2GetWamIf
        *   HttpPsoVer2SetWamIf
        *   HttpPsoVer2GetSbcIf
        *   HttpPsoVer2SetSbcIf
        *   HttpPsoVer2GetCbcIf
        *   HttpPsoVer2SetCbcIf
        *   HttpPsoVer2GetPbcIf
        *   HttpPsoVer2SetPbcIf
        *   HttpPsoVer2GetHfpIf
        *   HttpPsoVer2SetHfpIf
        *   HttpPsoVer2GetClientSocket
        *   HttpPsoVer2SetClientSocket
        *   HttpPsoVer2GetSbcContext
        *   HttpPsoVer2SetSbcContext
        *   HttpPsoVer2GetCbcContext
        *   HttpPsoVer2SetCbcContext
        *   HttpPsoVer2GetSbcPmode
        *   HttpPsoVer2SetSbcPmode
        *   HttpPsoVer2GetCbcPmode
        *   HttpPsoVer2SetCbcPmode
        *   HttpPsoVer2GetSessionState
        *   HttpPsoVer2SetSessionState
        *   HttpPsoVer2Return
        *   HttpPsoVer2Reset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_psover2_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetWamIf
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
HttpPsoVer2GetWamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hWamIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetWamIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hWamIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetSbcIf
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
HttpPsoVer2GetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hSbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetSbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hSbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetCbcIf
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
HttpPsoVer2GetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hCbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetCbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hCbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetPbcIf
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
HttpPsoVer2GetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hPbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetPbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hPbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetHfpIf
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
HttpPsoVer2GetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetHfpIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetClientSocket
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
HttpPsoVer2GetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hClientSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetClientSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hClientSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetSbcContext
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
HttpPsoVer2GetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hSbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetSbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hSbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetCbcContext
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
HttpPsoVer2GetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->hCbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetCbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->hCbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoVer2GetSbcPmode
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
HttpPsoVer2GetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->SbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetSbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->SbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoVer2GetCbcPmode
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
HttpPsoVer2GetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->CbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetCbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->CbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpPsoVer2GetSessionState
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
HttpPsoVer2GetSessionState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    return  pMyObject->SessionState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2SetSessionState
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulState
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulState
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2SetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->SessionState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2Return
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return object back to the global
        object pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2Return
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pAdvancedProxy->ReleasePso
            (
                (ANSC_HANDLE)pAdvancedProxy,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2Reset
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
HttpPsoVer2Reset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    
    pMyObject->hClientSocket = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext   = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext   = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex     = 0;
    pMyObject->SbcPmode      = HTTP_SBC_PMODE_RELAY2;
    pMyObject->CbcPmode      = HTTP_CBC_PMODE_RELAY2;
    pMyObject->SessionState  = HTTP_PSOVER2_STATE_INITIALIZED;

    return  ANSC_STATUS_SUCCESS;
}
