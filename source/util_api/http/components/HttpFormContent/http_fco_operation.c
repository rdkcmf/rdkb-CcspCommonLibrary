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

    module:	http_fco_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Form Content Object.

        *   HttpFcoEngage
        *   HttpFcoCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/


#include "http_fco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpFcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE      )pMyObject->hHfpIf;
    PHTTP_FORM_INPUT                pUriParams    = (PHTTP_FORM_INPUT         )pMyObject->hUriParams;
    PHTTP_FORM_INPUT                pFormInput    = (PHTTP_FORM_INPUT         )pMyObject->hFormInput;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT     )pMyObject->hOwnerContext;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO           )pBmoReq->hReqInfo;
    PHTTP_FORM_ELEMENT              pFormElement  = NULL;
    char*                           pQueryString  = NULL;
    char*                           pRawFormData  = NULL;
    ULONG                           ulRawDataSize = pBmoReq->GetBodySize2((ANSC_HANDLE)pBmoReq);
    char*                           pBoundaryStr  = NULL;
    ULONG                           ulEncType1    = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
    ULONG                           ulEncType2    = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
    ULONG                           ulMediaType   = IANA_MEDIA_TYPE_CODE_APPLICATION;
    ULONG                           ulSubType     = IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED;
    ULONG                           i             = 0;

#ifdef _ANSC_HTTP_CGI_USED_
    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    pQueryString = pReqInfo->RequestUri.QueryParams;
    pRawFormData = (char*)HttpBmoReqCgiGetFormContent(pBmoReq);

    if ( !pQueryString && !pRawFormData )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_GET )
    {
        ulEncType1   = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
        ulEncType2   = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
        pRawFormData = NULL;
    }
    else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_POST )
    {
        ulEncType1   = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
           	HttpBmoReqCgiGetContentType((ANSC_HANDLE)pBmoReq, &ulMediaType, &ulSubType);

        if ( (ulMediaType == IANA_MEDIA_TYPE_CODE_APPLICATION       ) &&
             (ulSubType   == IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED) )
        {
            ulEncType2 = HTTP_HFP_FORM_ENCTYPE_URLENCODED;
        }
        else if ( (ulMediaType == IANA_MEDIA_TYPE_CODE_MULTIPART ) &&
                  (ulSubType   == IANA_MT_MP_STYPE_CODE_FORM_DATA) )
        {
            ulEncType2 = HTTP_HFP_FORM_ENCTYPE_MULTIPART;
        }
        else
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pQueryString )
    {
        pUriParams =
            (PHTTP_FORM_INPUT)pHfpIf->ParseHtmlFormInput
                (
                    pHfpIf->hOwnerContext,
                    pQueryString,
                    AnscSizeOfString(pQueryString),
                    ulEncType1,
                    NULL        /* no boundary delimiter */
                );

        pMyObject->hUriParams = (ANSC_HANDLE)pUriParams;
    }
    else
    {
        pUriParams            = NULL;
        pMyObject->hUriParams = (ANSC_HANDLE)NULL;
    }

    if ( pRawFormData )
    {
        switch ( ulEncType2 )
        {
            case    HTTP_HFP_FORM_ENCTYPE_URLENCODED :

                    pFormInput =
                        (PHTTP_FORM_INPUT)pHfpIf->ParseHtmlFormInput
                            (
                                pHfpIf->hOwnerContext,
                                pRawFormData,
                                AnscSizeOfString(pRawFormData),
                                ulEncType2,
                                NULL        /* no boundary delimiter */
                            );

                    pMyObject->hFormInput = (ANSC_HANDLE)pFormInput;

                    break;

            case    HTTP_HFP_FORM_ENCTYPE_MULTIPART :

                    pBoundaryStr = (char*)HttpBmoReqCgiGetBoundaryDelimiter((ANSC_HANDLE)pBmoReq);

                    if ( pBoundaryStr )
                    {
                        pFormInput =
                            (PHTTP_FORM_INPUT)pHfpIf->ParseHtmlFormInput
                                (
                                    pHfpIf->hOwnerContext,
                                    pRawFormData,
                                    ulRawDataSize,
                                    ulEncType2,
                                    pBoundaryStr
                                );

                        AnscFreeMemory(pBoundaryStr);

                        pMyObject->hFormInput = (ANSC_HANDLE)pFormInput;
                    }
                    else
                    {
                        pFormInput            = NULL;
                        pMyObject->hFormInput = (ANSC_HANDLE)NULL;
                    }

                    break;

            default :

                    pFormInput            = NULL;
                    pMyObject->hFormInput = (ANSC_HANDLE)NULL;

                    break;
        }
    }
    else
    {
        pFormInput            = NULL;
        pMyObject->hFormInput = (ANSC_HANDLE)NULL;
    }

    if ( pUriParams )
    {
        for ( i = 0; i < pUriParams->ElementCount; i++ )
        {
            pFormElement = &pUriParams->ElementArray[i];
            pMyObject->AddFormElement((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pFormElement);
        }
    }

    if ( pFormInput )
    {
        for ( i = 0; i < pFormInput->ElementCount; i++ )
        {
            pFormElement = &pFormInput->ElementArray[i];
            pMyObject->AddFormElement((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pFormElement);
        }
    }
#endif
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpFcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}
