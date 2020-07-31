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

    module:	http_bmo_body.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Basic Message Object.

        *   HttpBmoClearBody
        *   HttpBmoCopyBodyFrom
        *   HttpBmoCopyBodyInto
        *   HttpBmoSaveAsBody
        *   HttpBmoSaveAsMbo
        *   HttpBmoCreateMessageBody
        *   HttpBmoAppendBody
        *   HttpBmoAppendBodyBdo
        *   HttpBmoAppendBodyFile
        *   HttpBmoSaveBodyAsFile
        *   HttpBmoSaveFormAsFile

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/26/02    initial revision.

**********************************************************************/


#include "http_bmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoClearBody
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean the message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoClearBody
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( pMessageBody )
    {
        pMyObject->hMessageBody = (ANSC_HANDLE)NULL;

        pMessageBody->ClearBody((ANSC_HANDLE)pMessageBody);
        pMessageBody->Remove   ((ANSC_HANDLE)pMessageBody);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoCopyBodyFrom
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the message body from the
        internal buffer into an external buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the body to be
                copied to.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied body data as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->CopyBodyFrom
                (
                    (ANSC_HANDLE)pMessageBody,
                    buffer,
                    pulSize
                );
    }
    else
    {
        *pulSize     = 0;
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoCopyBodyInto
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the message body from the
        external buffer into an internal buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the body to be
                copied from.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied body data as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->CopyBodyInto
                (
                    (ANSC_HANDLE)pMessageBody,
                    buffer,
                    pulSize
                );
    }
    else
    {
        *pulSize     = 0;
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSaveAsBody
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to save the external buffer block as
        the HTTP message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer holding the message body.

                ULONG                       ulSize
                Specifies the size of the external buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( !pMessageBody )
    {
        pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT)pMyObject->CreateMessageBody((ANSC_HANDLE)pMyObject);

        if ( !pMessageBody )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
        }
    }

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->SaveAsBody
                (
                    (ANSC_HANDLE)pMessageBody,
                    buffer,
                    ulSize
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSaveAsMbo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMbo
            );

    description:

        This function is called to save the external message object as
        the HTTP message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMbo
                Specifies the external message body object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSaveAsMbo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMbo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )hMbo;

    pMyObject->ClearBody((ANSC_HANDLE)pMyObject);

    pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

    pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
    pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
    pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoCreateMessageBody
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a Http Message Body Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message body object.

**********************************************************************/

ANSC_HANDLE
HttpBmoCreateMessageBody
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject                = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody             = NULL;
    PHTTP_HFO_TRANSFER_ENCODING     pHttpHfoTransferEncoding = NULL;
    ULONG                           ulIndex                  = 0;
    ULONG                           ulEncodingType           = IANA_TCODING_CODE_IDENTITY;

    pHttpHfoTransferEncoding =
        (PHTTP_HFO_TRANSFER_ENCODING)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_TRANSFER_ENCODING
            );

    if ( pHttpHfoTransferEncoding && (pHttpHfoTransferEncoding->EncodingCount > 0) )
    {
        ulIndex        = pHttpHfoTransferEncoding->EncodingCount - 1;
        ulEncodingType = pHttpHfoTransferEncoding->EncodingArray[ulIndex].Type;
    }

    switch ( ulEncodingType )
    {
        case    IANA_TCODING_CODE_IDENTITY :

                pMessageBody =
                    (PHTTP_MESSAGE_BODY_OBJECT)HttpCreateMboIdentity
                        (
                            pMyObject->hContainerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    IANA_TCODING_CODE_CHUNKED :

                pMessageBody =
                    (PHTTP_MESSAGE_BODY_OBJECT)HttpCreateMboChunked
                        (
                            pMyObject->hContainerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)NULL
                        );

                break;

        default :

                break;
    }

    return  (ANSC_HANDLE)pMessageBody;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoAppendBody
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to append the external buffer block to
        the HTTP message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer holding the message body.

                ULONG                       ulSize
                Specifies the size of the external buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoAppendBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR   )NULL;

    if ( !pMessageBody )
    {
        pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT)pMyObject->CreateMessageBody((ANSC_HANDLE)pMyObject);

        if ( !pMessageBody )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
        }
    }

    if ( TRUE )
    {
        pBodyBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ulSize,
                    0,
                    ulSize
                );

        if ( !pBodyBdo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    AnscBdoGetBlock(pBodyBdo),
                    buffer,
                    ulSize
                );
        }
    }

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->AppendBodyBdo
                (
                    (ANSC_HANDLE)pMessageBody,
                    (ANSC_HANDLE)pBodyBdo
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoAppendBodyBdo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to append the external buffer block to
        the HTTP message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the external buffer holding the message body.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR   )hBdo;

    if ( !pMessageBody )
    {
        pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT)pMyObject->CreateMessageBody((ANSC_HANDLE)pMyObject);

        if ( !pMessageBody )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
        }
    }

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->AppendBodyBdo
                (
                    (ANSC_HANDLE)pMessageBody,
                    (ANSC_HANDLE)pBodyBdo
                );
    }

    return  returnStatus;
}

#ifdef _ANSC_HTTP_FILE_USED_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoAppendBodyFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pFileName,
                int                         iRangeFrom,
                int                         iRangeTo
            );

    description:

        This function is called to append the complete/partial file
        content to the current message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFileName
                Specifies the name of the file whose content is to be
                appended.

                int                         iRangeFrom
                Specifies the starting offset of the file content.

                int                         iRangeTo
                Specifies the ending offset of the file content.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( !pMessageBody )
    {
        pMessageBody =
            (PHTTP_MESSAGE_BODY_OBJECT)HttpCreateMboFile
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pMessageBody )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
        }
    }

    if ( pMessageBody )
    {
        returnStatus =
            pMessageBody->AppendBodyFile
                (
                    (ANSC_HANDLE)pMessageBody,
                    pFileName,
                    iRangeFrom,
                    iRangeTo
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSaveBodyAsFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       file_name,
                BOOL                        bFlushBody
            );

    description:

        This function is called to save message body into a file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       file_name
                Specifies the file name where the body is to be saved.

                BOOL                        bFlushBody
                Specifies whether the body should be flushed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( !pMessageBody )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pMessageBody->SaveBodyAsFile
                (
                    (ANSC_HANDLE)pMessageBody,
                    file_name,
                    bFlushBody
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSaveFormAsFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       param_name,
                char*                       file_name,
                BOOL                        bFlushBody
            );

    description:

        This function is called to save form part into a file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       param_name
                Specifies the form part to be saved.

                char*                       file_name
                Specifies the file name where the body is to be saved.

                BOOL                        bFlushBody
                Specifies whether the body should be flushed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSaveFormAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        BOOL                        bFlushBody
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT      )pMyObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pBmoReq->hRcpIf;
    PHTTP_FUM_INTERFACE             pFumIf       = (PHTTP_FUM_INTERFACE       )pMyObject->hFumIf;
    PHTTP_MDH_INTERFACE             pMdhIf       = (PHTTP_MDH_INTERFACE       )NULL;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    char*                           pBoundaryStr = NULL;
    ULONG                           ulEncType    = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
    ULONG                           ulMediaType  = IANA_MEDIA_TYPE_CODE_APPLICATION;
    ULONG                           ulSubType    = IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED;
    char*                           pReqUri      = pRcpIf->GetPathInfo(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);

    pMdhIf = (PHTTP_MDH_INTERFACE)pFumIf->GetMdhIf(pFumIf->hOwnerContext, (PUCHAR)pReqUri);

    if ( pMdhIf )
    {
        return
            pMdhIf->SaveFileAs
                (
                    pMdhIf->hOwnerContext,
                    pBmoReq->GetWebSessionId((ANSC_HANDLE)pBmoReq),
                    (PUCHAR)pReqUri,
                    (PUCHAR)param_name,
                    (PUCHAR)file_name
                );
    }

    if ( !pMessageBody || (pMyObject->Oid != HTTP_BMO_REQ_OID) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) != HTTP_METHOD_CODE_POST )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        returnStatus =
        	HttpBmoReqCgiGetContentType
                (
                    (ANSC_HANDLE)pBmoReq,
                    &ulMediaType,
                    &ulSubType
                );

        if ( (ulMediaType == IANA_MEDIA_TYPE_CODE_APPLICATION       ) &&
             (ulSubType   == IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED) )
        {
            ulEncType = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
        }
        else if ( (ulMediaType == IANA_MEDIA_TYPE_CODE_MULTIPART ) &&
                  (ulSubType   == IANA_MT_MP_STYPE_CODE_FORM_DATA) )
        {
            ulEncType    = HTTP_HFP_FORM_ENCTYPE_MULTIPART;
            pBoundaryStr = (char*)HttpBmoReqCgiGetBoundaryDelimiter((ANSC_HANDLE)pBmoReq);

            if ( !pBoundaryStr )
            {
                return  ANSC_STATUS_UNAPPLICABLE;
            }
        }
        else
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
    }

    returnStatus =
        pHfpIf->SaveFormPartAsFile
            (
                pHfpIf->hOwnerContext,
                param_name,
                file_name,
                ulEncType,
                pBoundaryStr,
                (ANSC_HANDLE)pMessageBody,
                bFlushBody
            );

    if ( pBoundaryStr )
    {
        AnscFreeMemory(pBoundaryStr);
    }

    return  returnStatus;
}

#endif
