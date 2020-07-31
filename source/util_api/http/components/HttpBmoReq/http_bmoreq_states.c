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

    module:	http_bmoreq_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Bmo Req Object.

        *   HttpBmoReqGetClientAddr
        *   HttpBmoReqSetClientAddr
        *   HttpBmoReqGetClientPort
        *   HttpBmoReqSetClientPort
        *   HttpBmoReqGetDaemonAddr
        *   HttpBmoReqSetDaemonAddr
        *   HttpBmoReqGetDaemonPort
        *   HttpBmoReqSetDaemonPort
        *   HttpBmoReqGetServerMode
        *   HttpBmoReqSetServerMode
        *   HttpBmoReqGetCgiIf
        *   HttpBmoReqGetRcpIf
        *   HttpBmoReqSetRcpIf
        *   HttpBmoReqGetFormContent
        *   HttpBmoReqGenFormContent
        *   HttpBmoReqGetMappedPath
        *   HttpBmoReqSetMappedPath
        *   HttpBmoReqGetOriginalPagePath
        *   HttpBmoReqSetOriginalPagePath
        *   HttpBmoReqGetFilteredPagePath
        *   HttpBmoReqSetFilteredPagePath
        *   HttpBmoReqReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#include "http_bmoreq_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpBmoReqGetClientAddr
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

PUCHAR
HttpBmoReqGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->ClientAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetClientAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->ClientAddr.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpBmoReqGetClientPort
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

USHORT
HttpBmoReqGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->ClientPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetClientPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->ClientPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpBmoReqGetDaemonAddr
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

PUCHAR
HttpBmoReqGetDaemonAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->DaemonAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetDaemonAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetDaemonAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->DaemonAddr.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpBmoReqGetDaemonPort
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

USHORT
HttpBmoReqGetDaemonPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->DaemonPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetDaemonPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetDaemonPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->DaemonPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetServerMode
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
HttpBmoReqGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->ServerMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetServerMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       mode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       mode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->ServerMode = mode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqGetRcpIf
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
HttpBmoReqGetRcpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->hRcpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetRcpIf
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
HttpBmoReqSetRcpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE )pMyObject->hRcpIf;

    *pRcpIf = *(PHTTP_RCP_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}

#ifdef _ANSC_HTTP_CGI_USED_


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqGetCgiIf
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
HttpBmoReqGetCgiIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->hCgiIf;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqGetFormContent
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
HttpBmoReqGetFormContent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    if ( !pMyObject->hFormContent )
    {
        pMyObject->GenFormContent((ANSC_HANDLE)pMyObject);
    }

    return  pMyObject->hFormContent;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqGenFormContent
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to generate form content.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqGenFormContent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT     )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE      )pMyObject->hHfpIf;
    PHTTP_FORM_CONTENT_OBJECT       pFormContent = (PHTTP_FORM_CONTENT_OBJECT)pMyObject->hFormContent;

    if ( pFormContent )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pFormContent =
            (PHTTP_FORM_CONTENT_OBJECT)HttpCreateFormContent
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pFormContent )
    {
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hFormContent = (ANSC_HANDLE)pFormContent;

        pFormContent->SetHfpIf((ANSC_HANDLE)pFormContent, (ANSC_HANDLE)pHfpIf);
        pFormContent->Engage  ((ANSC_HANDLE)pFormContent);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetMappedPath
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

char*
HttpBmoReqGetMappedPath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->MappedPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetMappedPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetMappedPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    if ( pMyObject->MappedPath )
    {
        AnscFreeMemory(pMyObject->MappedPath);

        pMyObject->MappedPath = NULL;
    }

    pMyObject->MappedPath = AnscCloneString(page_path);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetOriginalPagePath
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

char*
HttpBmoReqGetOriginalPagePath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->OriginalPagePath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetOriginalPagePath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetOriginalPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    if ( pMyObject->OriginalPagePath )
    {
        AnscFreeMemory(pMyObject->OriginalPagePath);

        pMyObject->OriginalPagePath = NULL;
    }

    pMyObject->OriginalPagePath = AnscCloneString(page_path);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetFilteredPagePath
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

char*
HttpBmoReqGetFilteredPagePath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    return  pMyObject->FilteredPagePath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetFilteredPagePath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetFilteredPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    if ( pMyObject->FilteredPagePath )
    {
        AnscFreeMemory(pMyObject->FilteredPagePath);

        pMyObject->FilteredPagePath = NULL;
    }

    pMyObject->FilteredPagePath = AnscCloneString(page_path);

    return  ANSC_STATUS_SUCCESS;
}

#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqReset
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
HttpBmoReqReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT     )hThisObject;
    PHTTP_FORM_CONTENT_OBJECT       pFormContent = (PHTTP_FORM_CONTENT_OBJECT)pMyObject->hFormContent;

    HttpBmoReset((ANSC_HANDLE)pMyObject);

    if ( pFormContent )
    {
        pFormContent->Remove((ANSC_HANDLE)pFormContent);

        pMyObject->hFormContent = (ANSC_HANDLE)NULL;
    }

    if ( pMyObject->MappedPath )
    {
        AnscFreeMemory(pMyObject->MappedPath);

        pMyObject->MappedPath = NULL;
    }

    if ( pMyObject->OriginalPagePath )
    {
        AnscFreeMemory(pMyObject->OriginalPagePath);

        pMyObject->OriginalPagePath = NULL;
    }

    if ( pMyObject->FilteredPagePath )
    {
        AnscFreeMemory(pMyObject->FilteredPagePath);

        pMyObject->FilteredPagePath = NULL;
    }

    if ( pMyObject->hAuthInfo )
    {
        PHTTP_AUTHO_INFO            pAuthInfo = (PHTTP_AUTHO_INFO)pMyObject->hAuthInfo;

        pMyObject->hAuthInfo = NULL;

        HttpAuthInfoRemove(pAuthInfo);
    }

    return  ANSC_STATUS_SUCCESS;
}
