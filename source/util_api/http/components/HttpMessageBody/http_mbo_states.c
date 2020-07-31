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

    module:	http_mbo_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Message Body Object.

        *   HttpMboGetHfpIf
        *   HttpMboSetHfpIf
        *   HttpMboGetBccIf
        *   HttpMboSetBccIf
        *   HttpMboSetFumIf
        *   HttpMboGetState
        *   HttpMboSetState
        *   HttpMboGetMode
        *   HttpMboSetMode
        *   HttpMboReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#include "http_mbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpMboGetHfpIf
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
HttpMboGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSetHfpIf
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
HttpMboSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpMboGetBccIf
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
HttpMboGetBccIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    return  pMyObject->hBccIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSetBccIf
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
HttpMboSetBccIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->hBccIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSetFumIf
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
HttpMboSetFumIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->hFumIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetState
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
HttpMboGetState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    return  pMyObject->State;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSetState
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
HttpMboSetState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->State = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetMode
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
HttpMboGetMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    return  pMyObject->Mode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSetMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->Mode = ulMode;

    if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        pMyObject->ClearBody((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboReset
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
HttpMboReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    if ( pMyObject->Mode == HTTP_MBO_MODE_STORE_EXTERNAL )
    {
        PHTTP_BMO_REQ_OBJECT        pBmoRequest  = (PHTTP_BMO_REQ_OBJECT)pMyObject->hOwnerContext;
        PHTTP_RCP_INTERFACE         pRcpIf       = (PHTTP_RCP_INTERFACE          )pBmoRequest->hRcpIf;

        if ( pRcpIf )
        {
            PUCHAR                  pReqUri      = (PUCHAR)pRcpIf->GetPathInfo(pRcpIf->hOwnerContext, (ANSC_HANDLE)pBmoRequest);
            PHTTP_FUM_INTERFACE     pFumIf       = (PHTTP_FUM_INTERFACE )pMyObject->hFumIf;
            PHTTP_MDH_INTERFACE     pMdhIf       = (PHTTP_MDH_INTERFACE )pFumIf->GetMdhIf(pFumIf->hOwnerContext, pReqUri);

            if ( pMdhIf )
            {
                pMdhIf->Finish((ANSC_HANDLE)pMdhIf, pBmoRequest->GetWebSessionId((ANSC_HANDLE)pBmoRequest));
            }
        }
    }

    pMyObject->ClearBody((ANSC_HANDLE)pMyObject);

    pMyObject->State        = HTTP_MBO_STATE_EMPTY;
    pMyObject->Mode         = HTTP_MBO_MODE_RELAY;
    pMyObject->TransferSize = 0;
    pMyObject->EntitySize   = 0;

    pMyObject->MPState         = HTTP_MBO_MP_STATE_INIT;
    if ( pMyObject->hLastMPBdo )
    {
        AnscFreeBdo(pMyObject->hLastMPBdo);
        pMyObject->hLastMPBdo       = NULL;
    }

    if ( pMyObject->pMPFieldName )
    {
        AnscFreeMemory(pMyObject->pMPFieldName);
        pMyObject->pMPFieldName    = NULL;
    }

    if ( pMyObject->pMPFileName )
    {
        AnscFreeMemory(pMyObject->pMPFileName);
        pMyObject->pMPFileName      = NULL;
    }

    if ( pMyObject->pMPHeadersBuf)
    {
        AnscFreeMemory(pMyObject->pMPHeadersBuf);
        pMyObject->pMPHeadersBuf    = NULL;
    }

    if ( pMyObject->pBoundaryStr )
    {
        AnscFreeMemory(pMyObject->pBoundaryStr);
        pMyObject->pBoundaryStr     = NULL;
    }

    pMyObject->bMPFileData      = FALSE;

    return  ANSC_STATUS_SUCCESS;
}
