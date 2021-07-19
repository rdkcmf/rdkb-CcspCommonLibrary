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

    module:	http_bmorep_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Bmo Rep Object.

        *   HttpBmoRepGetStartLineSize
        *   HttpBmoRepPredictBodySize
        *   HttpBmoRepIsHttpVersion11
        *   HttpBmoRepGetReqInfo
        *   HttpBmoRepGetRepInfo
        *   HttpBmoRepGetCode
        *   HttpBmoRepGetMajorVersion
        *   HttpBmoRepGetMinorVersion
        *   HttpBmoRepGetText
        *   HttpBmoRepGetProgressBits
        *   HttpBmoRepSetReqInfo
        *   HttpBmoRepSetRepInfo
        *   HttpBmoRepSetCode
        *   HttpBmoRepSetMajorVersion
        *   HttpBmoRepSetMinorVersion
        *   HttpBmoRepSetText
        *   HttpBmoRepSetProgressBits

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#include "http_bmorep_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepGetStartLineSize
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
HttpBmoRepGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE )pMyObject->hHfpIf;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;
    ULONG                           ulLineSize   = 0;

    if ( !pRepInfo )
    {
        return  0;
    }
    else
    {
        ulLineSize =
            pHfpIf->GetStatusLineSize
                (
                    pHfpIf->hOwnerContext,
                    (ANSC_HANDLE)pRepInfo
                );
    }

    return  ulLineSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepPredictBodySize
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
HttpBmoRepPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject                = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo                 = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    PHTTP_RESPONSE_INFO             pRepInfo                 = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;
    PHTTP_HFO_CONTENT_LENGTH        pHttpHfoContentLength    = NULL;
    PHTTP_HFO_CONTENT_ENCODING      pHttpHfoContentEncoding  = NULL;
    PHTTP_HFO_CONTENT_TYPE          pHttpHfoContentType      = NULL;
    PHTTP_HFO_TRANSFER_ENCODING     pHttpHfoTransferEncoding = NULL;
    ULONG                           ulBodySize               = 0xFFFFFFFF;

    if ( !pRepInfo )
    {
        return  0xFFFFFFFF;
    }
    else if ( (pRepInfo->StatusCode  < HTTP_STATUS_SUCCESS     ) ||
              (pRepInfo->StatusCode == HTTP_STATUS_NO_CONTENT  ) ||
              (pRepInfo->StatusCode == HTTP_STATUS_NOT_MODIFIED) )
    {
        return  0;
    }
    else if ( pReqInfo && (pReqInfo->Method == HTTP_METHOD_HEAD) )
    {
        return  0;
    }
    else
    {
        pHttpHfoContentLength   =
            (PHTTP_HFO_CONTENT_LENGTH)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_LENGTH
                );

        pHttpHfoContentEncoding =
            (PHTTP_HFO_CONTENT_ENCODING)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_ENCODING
                );

        pHttpHfoContentType     =
            (PHTTP_HFO_CONTENT_TYPE)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_TYPE
                );

        pHttpHfoTransferEncoding =
            (PHTTP_HFO_TRANSFER_ENCODING)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_TRANSFER_ENCODING
                );
    }

    if ( pHttpHfoContentLength )
    {
        ulBodySize = pHttpHfoContentLength->BodySize;
    }
    else if ( !pHttpHfoContentEncoding && !pHttpHfoContentType )
    {
        ulBodySize = 0xFFFFFFFF;
    }
    else if ( !pHttpHfoTransferEncoding )
    {
        ulBodySize = 0;
    }
    else
    {
        ulBodySize = 0xFFFFFFFF;
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoRepIsHttpVersion11
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check the protocol version number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoRepIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    if ( (pRepInfo->MajorVersion == 1) && (pRepInfo->MinorVersion == 1) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoRepGetReqInfo
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
HttpBmoRepGetReqInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    return  (ANSC_HANDLE)pReqInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoRepGetRepInfo
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
HttpBmoRepGetRepInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    return  (ANSC_HANDLE)pRepInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepGetCode
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
HttpBmoRepGetCode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    return  pRepInfo->StatusCode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepGetMajorVersion
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
HttpBmoRepGetMajorVersion
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    return  pRepInfo->MajorVersion;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepGetMinorVersion
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
HttpBmoRepGetMinorVersion
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
     PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    return  pRepInfo->MinorVersion;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoRepGetText
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
HttpBmoRepGetText
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    return  pRepInfo->ReasonPhrase;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoRepGetProgressBits
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
HttpBmoRepGetProgressBits
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    
    return  pMyObject->ProgressBits;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetReqInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInfo
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInfo
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetReqInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( pReqInfo )
    {
        AnscFreeMemory(pReqInfo);

        pMyObject->hReqInfo = (ANSC_HANDLE)NULL;
    }

    pMyObject->hReqInfo = hInfo;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetRepInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInfo
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInfo
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetRepInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    if ( pRepInfo )
    {
        AnscFreeMemory(pRepInfo);

        pMyObject->hRepInfo = (ANSC_HANDLE)NULL;
    }

    pMyObject->hRepInfo = hInfo;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetCode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       code
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       code
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetCode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    if ( !pRepInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pRepInfo->StatusCode = code;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetMajorVersion
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       version
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       version
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetMajorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    if ( !pRepInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pRepInfo->MajorVersion = version;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetMinorVersion
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       version
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       version
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetMinorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;

    if ( !pRepInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pRepInfo->MinorVersion = version;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetText
            (
                ANSC_HANDLE                 hThisObject,
                char*                       text
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       text
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetText
    (
        ANSC_HANDLE                 hThisObject,
        char*                       text
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    PHTTP_RESPONSE_INFO             pRepInfo     = (PHTTP_RESPONSE_INFO )pMyObject->hRepInfo;
    errno_t                         rc           = -1;

    if ( !pRepInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscZeroMemory(pRepInfo->ReasonPhrase, HTTP_MAX_REASON_PHRASE_SIZE);
    rc = strcpy_s(pRepInfo->ReasonPhrase, sizeof(pRepInfo->ReasonPhrase), text);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRepSetProgressBits
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       progress_bits
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       progress_bits
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoRepSetProgressBits
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       progress_bits
    )
{
    PHTTP_BMO_REP_OBJECT            pMyObject    = (PHTTP_BMO_REP_OBJECT)hThisObject;
    
    pMyObject->ProgressBits = progress_bits;

    return  ANSC_STATUS_SUCCESS;
}
