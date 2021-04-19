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

    module:	http_smpo_parse.c

        For HTTP Simple message parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module provides some pure interfaces which are 
        called to parse a specific HTTP header or construct
        a specific header. Also there're APIs for request
        line and status line.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

  ---------------------------------------------------------------

    last modified:

        02/27/02

**********************************************************************/

#include "http_smpo_global.h"



/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAccept
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAccept
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Accept = "Accept" ":" #( media-range [accept-params] ) */

    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PUCHAR                          pLast       = pMsg + ulSize - 1;
    PHTTP_HFO_ACCEPT                pHfoAccept  = NULL;
    BOOL                            bSucc       = TRUE;
    PHTTP_MEDIA_RANGE               pMediaRange;
    PUCHAR                          pToken      = pMsg;
    ULONG                           ulTokenSize;
    PUCHAR                          pNext;

    pHfoAccept  = (PHTTP_HFO_ACCEPT)AnscAllocateMemory(sizeof(HTTP_HFO_ACCEPT));

    if (pHfoAccept)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoAccept->RangeCount >= HTTP_MAX_MEDIA_RANGE_NUMBER)
            {
                break;
            }

            pMediaRange = &pHfoAccept->RangeArray[pHfoAccept->RangeCount ++];

            bSucc  = 
                HttpSmpoUtilParseMediaRange
                    (
                        hHttpSmpo,
                        (ANSC_HANDLE)pMediaRange,
                        pToken,
                        ulTokenSize
                    );

            if (!bSucc)
            {
                pHfoAccept->RangeCount --;
            }
            
            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAccept;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAcceptCharset
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAcceptCharset
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Accept-Charset = "Accept-Charset" ":" 1#( charset [";" "q" "=" qvalue] ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_ACCEPT_CHARSET        pHfoAcceptCharset   = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_CHARSET                   pCharset;

    pHfoAcceptCharset  = (PHTTP_HFO_ACCEPT_CHARSET)AnscAllocateMemory(sizeof(HTTP_HFO_ACCEPT_CHARSET));

    if (pHfoAcceptCharset)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoAcceptCharset->CharsetCount >= HTTP_MAX_CHARSET_NUMBER)
            {
                break;
            }

            pCharset    = &pHfoAcceptCharset->CharsetArray[pHfoAcceptCharset->CharsetCount ++];
            
            pCharset->Type  = HttpSmpoUtilGetCharsetType(pToken, ulLen);

            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pCharset->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAcceptCharset;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAcceptEncoding
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAcceptEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Accept-Encoding = "Accept-Encoding" ":" 1#( codings [ ";" "q" "=" qvalue ] ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_ACCEPT_ENCODING       pHfoAcceptEncoding  = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_ENCODING                  pEncoding;

    pHfoAcceptEncoding  = (PHTTP_HFO_ACCEPT_ENCODING)AnscAllocateMemory(sizeof(HTTP_HFO_ACCEPT_ENCODING));

    if (pHfoAcceptEncoding)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoAcceptEncoding->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding    = &pHfoAcceptEncoding->EncodingArray[pHfoAcceptEncoding->EncodingCount ++];
            
            pEncoding->Type  = HttpSmpoUtilGetEncodingType(pToken, ulLen);

            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pEncoding->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAcceptEncoding;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAcceptLanguage
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAcceptLanguage
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Accept-Language = "Accept-Language" ":" 1#( language-range [";" "q" "=" qvalue] ) */
    /* language-range = ( ( 1*8ALPHA *( "-" 1*8ALPHA ) ) | "*" ) */
    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_ACCEPT_LANGUAGE       pHfoAcceptLanguage  = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_LANGUAGE                  pLanguage;

    pHfoAcceptLanguage  = (PHTTP_HFO_ACCEPT_LANGUAGE)AnscAllocateMemory(sizeof(HTTP_HFO_ACCEPT_LANGUAGE));

    if (pHfoAcceptLanguage)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoAcceptLanguage->LanguageCount >= HTTP_MAX_LANGUAGE_NUMBER)
            {
                break;
            }

            pLanguage    = &pHfoAcceptLanguage->LanguageArray[pHfoAcceptLanguage->LanguageCount ++];

            /* Tag */
            HttpSmpoUtilCopyHeaderString(pToken, ulLen, pLanguage->Tag, HTTP_MAX_LANGUAGE_TAG_SIZE);
            
            /* Quality */
            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;

            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality + 1;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pLanguage->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAcceptLanguage;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAcceptRanges
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAcceptRanges
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Accept-Ranges = "Accept-Ranges" ":" acceptable-ranges */
    /* acceptable-ranges = 1#range-unit | "none" */
    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_ACCEPT_RANGES         pHfoAcceptRanges    = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_RANGE                     pRange;

    pHfoAcceptRanges  = (PHTTP_HFO_ACCEPT_RANGES)AnscAllocateMemory(sizeof(HTTP_HFO_ACCEPT_RANGES));

    if (pHfoAcceptRanges)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoAcceptRanges->RangeCount >= HTTP_MAX_RANGE_NUMBER)
            {
                break;
            }

            pRange  = &pHfoAcceptRanges->RangeArray[pHfoAcceptRanges->RangeCount ++];

            /* unit */
            HttpSmpoUtilCopyHeaderString(pToken, ulTokenSize, pRange->Unit, HTTP_MAX_LANGUAGE_TAG_SIZE);

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAcceptRanges;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAge
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAge
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    UNREFERENCED_PARAMETER(ulSize);
    /* Age = "Age" ":" age-value */
    /* age-value = delta-seconds */

    PHTTP_HFO_AGE                   pHfoAge     = NULL;

    pHfoAge  = (PHTTP_HFO_AGE)AnscAllocateMemory(sizeof(HTTP_HFO_AGE));

    if (pHfoAge)
    {
        pHfoAge->DeltaSeconds   = _ansc_atoi(pBuf);
    }

    return (ANSC_HANDLE)pHfoAge;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAllow
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAllow
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Allow = "Allow" ":" 1#Method */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_ALLOW                 pHfoAllow           = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    pHfoAllow  = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if (pHfoAllow)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoAllow->MethodCount >= HTTP_MAX_METHOD_NUMBER)
            {
                break;
            }

            pHfoAllow->MethodArray[pHfoAllow->MethodCount ++]   =
                HttpSmpoUtilGetMethodId(pToken, ulTokenSize);

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoAllow;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseAuthorization
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseAuthorization
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Authorization = "Authorization" ":" credentials */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_AUTHORIZATION         pHfoAuthorization   = NULL;
    PHTTP_AUTH_CREDENTIAL           pCredentials;

    pHfoAuthorization  = (PHTTP_HFO_AUTHORIZATION)AnscAllocateMemory(sizeof(HTTP_HFO_AUTHORIZATION));

    if (pHfoAuthorization)
    {
        pCredentials = &pHfoAuthorization->Credential;

        HttpSmpoUtilParseCredentials(hHttpSmpo, (ANSC_HANDLE)pCredentials, pMsg, ulSize);
    }

    return (ANSC_HANDLE)pHfoAuthorization;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseCacheControl
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseCacheControl
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_CACHE_CONTROL         pHfoCacheCtrl   = NULL;

    pHfoCacheCtrl  = (PHTTP_HFO_CACHE_CONTROL)AnscAllocateMemory(sizeof(HTTP_HFO_CACHE_CONTROL));

    if (pHfoCacheCtrl)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                pHfoCacheCtrl->Directives,
                HTTP_MAX_CACHE_DIRECTIVE_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoCacheCtrl;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseConnection
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseConnection
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_CONNECTION            pHfoConnection  = (PHTTP_HFO_CONNECTION)NULL;
    PUCHAR                          pLast           = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_CONNECTION_TOKEN          pConnToken;
    ULONG                           ulTokenType;

    pHfoConnection = (PHTTP_HFO_CONNECTION)AnscAllocateMemory(sizeof(HTTP_HFO_CONNECTION));

    if (pHfoConnection)
    {
        pHfoConnection->TokenCount = 0;

        /* parse connection tokens */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoConnection->TokenCount >= HTTP_MAX_CONNECTION_TOKEN_NUMBER)
            {
                break;
            }

            pConnToken  = &pHfoConnection->TokenArray[pHfoConnection->TokenCount ++];

            HttpSmpoUtilCopyHeaderString
                (
                    pToken,
                    ulTokenSize,
                    pConnToken->Token,
                    HTTP_MAX_CONNECTION_TOKEN_SIZE
                );

            ulTokenType = IANA_CONNECTION_CODE_RESERVED;

            if (AnscEqualString2((char *)pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_CLOSE;
            }
            else
            if (AnscEqualString2((char *)pToken, IANA_CONNECTION_TEXT_KEEP_ALIVE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_KEEP_ALIVE;
            }

            pConnToken->OptionType  = ulTokenType;

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoConnection;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentEncoding
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Content-Encoding = "Content-Encoding" ":" 1#content-coding */
    /* content-coding = token  */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_CONTENT_ENCODING      pHfoContentEncoding = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_ENCODING                  pEncoding;

    pHfoContentEncoding  = (PHTTP_HFO_CONTENT_ENCODING)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_ENCODING));

    if (pHfoContentEncoding)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoContentEncoding->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding    = &pHfoContentEncoding->EncodingArray[pHfoContentEncoding->EncodingCount ++];
            
            pEncoding->Type  = HttpSmpoUtilGetEncodingType(pToken, ulLen);

            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pEncoding->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoContentEncoding;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentLanguage
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentLanguage
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Content-Language = "Content-Language" ":" 1#language-tag */
    /* language-tag = primary-tag *( "-" subtag ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_CONTENT_LANGUAGE      pHfoContentLanguage = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_LANGUAGE                  pLanguage;

    pHfoContentLanguage  = (PHTTP_HFO_CONTENT_LANGUAGE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LANGUAGE));

    if (pHfoContentLanguage)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoContentLanguage->LanguageCount >= HTTP_MAX_LANGUAGE_NUMBER)
            {
                break;
            }

            pLanguage    = &pHfoContentLanguage->LanguageArray[pHfoContentLanguage->LanguageCount ++];

            /* Tag */
            HttpSmpoUtilCopyHeaderString(pToken, ulLen, pLanguage->Tag, HTTP_MAX_LANGUAGE_TAG_SIZE);
            
            /* Quality */
            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pLanguage->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoContentLanguage;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentLength
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentLength
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    UNREFERENCED_PARAMETER(ulSize);
    /* Content-Length = "Content-Length" ":" 1*DIGIT */

    PHTTP_HFO_CONTENT_LENGTH        pHfoContentLength   = NULL;

    pHfoContentLength  = (PHTTP_HFO_CONTENT_LENGTH)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LENGTH));

    if (pHfoContentLength)
    {
        pHfoContentLength->BodySize = _ansc_atoi(pBuf);
    }

    return (ANSC_HANDLE)pHfoContentLength;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentLocation
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentLocation
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Content-Location = "Content-Location" ":" ( absoluteURI | relativeURI ) */

    PHTTP_HFO_CONTENT_LOCATION      pHfoContentLocation = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoContentLocation  = (PHTTP_HFO_CONTENT_LOCATION)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LOCATION));

    if (pHfoContentLocation)
    {
        bSucc   = 
            HttpSmpoUtilParseAbsoluteURI
                (
                    hHttpSmpo, 
                    (ANSC_HANDLE)&pHfoContentLocation->LocationUri,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            bSucc   = 
                HttpSmpoUtilParseRelativeURI
                    (
                        hHttpSmpo, 
                        (ANSC_HANDLE)&pHfoContentLocation->LocationUri,
                        pBuf,
                        ulSize
                    );
        }

        if (!bSucc)
        {
            AnscFreeMemory(pHfoContentLocation);
            pHfoContentLocation = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoContentLocation;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentMD5
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentMD5
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_CONTENT_MD5           pHfoContentMd5  = NULL;

    pHfoContentMd5  = (PHTTP_HFO_CONTENT_MD5)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_MD5));

    if (pHfoContentMd5)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                (char *)pHfoContentMd5->Digest,
                ANSC_MD5_OUTPUT_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoContentMd5;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentRange
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Content-Range = "Content-Range" ":" content-range-spec */
    /* content-range-spec = byte-content-range-spec */
    /* byte-content-range-spec = bytes-unit SP first-byte-pos "-" last-byte-pos "/" entity-length */

    PHTTP_HFO_CONTENT_RANGE         pHfoContentRange    = NULL;
    PUCHAR                          pToken              = (PUCHAR)pBuf;
    PUCHAR                          pNext;
    PUCHAR                          pEntityLen;

    pHfoContentRange  = (PHTTP_HFO_CONTENT_RANGE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_RANGE));

    if (pHfoContentRange)
    {
        pNext       = _ansc_memchr(pToken, HTTP_SMPO_CHAR_HYPHEN, ulSize);
        pEntityLen  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulSize);

        if (pNext && pEntityLen)
        {
            pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, ulSize);

            if (!pToken)
            {
                AnscFreeMemory(pHfoContentRange);
                pHfoContentRange    = NULL;
            }
            else
            {
                pToken ++;
                pHfoContentRange->FirstBytePos  = _ansc_atoi((const char *)pToken);

                pNext ++;
                pHfoContentRange->LastBytePos   = _ansc_atoi((const char *)pNext);
            }

            pEntityLen ++;
            /*CID: 64364 Explicit null dereferenced*/
            if (pHfoContentRange)
                pHfoContentRange->InstaceLength     = _ansc_atoi((const char *)pEntityLen);
        }
        else
        {
            AnscFreeMemory(pHfoContentRange);
            pHfoContentRange    = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoContentRange;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseContentType
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseContentType
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Content-Type = "Content-Type" ":" media-type */
    /* media-type = type "/" subtype *( ";" parameter ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_CONTENT_TYPE          pHfoContentType     = NULL;
    PUCHAR                          pType               = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pParams;
    PUCHAR                          pSubType;
    ULONG                           ulLen;

    pParams = _ansc_memchr(pType, HTTP_SMPO_CHAR_SEMICOLON, ulSize);

    pSubType    = _ansc_memchr(pType, HTTP_SMPO_CHAR_SLASH, ulSize);

    if (!pSubType)
    {
        return (ANSC_HANDLE)NULL;
    }

    pHfoContentType  = (PHTTP_HFO_CONTENT_TYPE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_TYPE));

    if (pHfoContentType)
    {
        pHfoContentType->MediaType = HttpSmpoUtilGetMediaType(pType, pSubType - pType);

        pSubType ++;
        if (pParams)
        {
            ulLen   = pParams - pSubType;
        }
        else
        {
            ulLen   = pLast - pSubType + 1;
        }

        pHfoContentType->SubType    = 
            HttpSmpoUtilGetMediaSubType
                (
                    hHttpSmpo, 
                    pHfoContentType->MediaType, 
                    pSubType, 
                    ulLen
                );

        if (pParams)
        {
            pParams ++;
            pParams = HttpSmpoUtilLinearWhiteSpace(pParams, pLast - pParams + 1);
            HttpSmpoUtilCopyHeaderString
                (
                    pParams, 
                    pLast - pParams + 1, 
                    pHfoContentType->Parameters, 
                    HTTP_MAX_HEADER_PARAM_SIZE
               );
        }
    }

    return (ANSC_HANDLE)pHfoContentType;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseDate
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseDate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_HFO_DATE                  pHfoDate    = NULL;
    PHTTP_DATE                      pDate       = NULL;
    BOOL                            bSucc       = TRUE;

    pHfoDate    = (PHTTP_HFO_DATE)AnscAllocateMemory(sizeof(HTTP_HFO_DATE));

    if (pHfoDate)
    {
        pDate   = &pHfoDate->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoDate);
            pHfoDate    = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoDate;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseETag
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseETag
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* ETag = "ETag" ":" entity-tag */
    /* entity-tag = [weak] opaque-tag */
    /* weak = "W/" */
    /* opaque-tag = quoted-string */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_ETAG                  pHfoEtag            = NULL;
    PUCHAR                          pToken              = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pTag;

    pHfoEtag  = (PHTTP_HFO_ETAG)AnscAllocateMemory(sizeof(HTTP_HFO_ETAG));

    if (pHfoEtag)
    {
        pTag    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulSize);

        if (pTag)
        {
            pHfoEtag->bWeak = TRUE;
            pTag ++;
        }
        else
        {
            pHfoEtag->bWeak = FALSE;
            pTag    = pToken;
        }

        HttpSmpoUtilCopyHeaderString(pTag, pLast - pTag + 1, pHfoEtag->Tag, HTTP_MAX_ETAG_SIZE);
    }

    return (ANSC_HANDLE)pHfoEtag;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseExpect
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseExpect
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Expect = "Expect" ":" 1#expectation */
    /* expectation = "100-continue" | expectation-extension */
    /* expectation-extension = token [ "=" ( token | quoted-string ) *expect-params ] */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_EXPECT                pHfoExpect          = NULL;
    PUCHAR                          pToken              = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pParam;
    PHTTP_EXPECT_TOKEN              pExpectToken;

    pHfoExpect  = (PHTTP_HFO_EXPECT)AnscAllocateMemory(sizeof(HTTP_HFO_EXPECT));

    if (pHfoExpect)
    {
        while (pToken)
        {
            if (pHfoExpect->TokenCount >= HTTP_MAX_EXPECT_TOKEN_NUMBER)
            {
                break;
            }

            pExpectToken    = &pHfoExpect->TokenArray[pHfoExpect->TokenCount ++];

            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);
            
            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pParam  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, ulTokenSize);

            if (pParam)
            {
                ulTokenSize = pParam - pToken;
            }

            /* token */
            HttpSmpoUtilCopyHeaderString(pToken, ulTokenSize, pExpectToken->Token, HTTP_MAX_EXPECT_TOKEN_SIZE);

            if (pParam)
            {
                pParam  ++;
                ulTokenSize = pNext - pParam;

                HttpSmpoUtilCopyHeaderString(pParam, ulTokenSize, pExpectToken->Parameters, HTTP_MAX_HEADER_PARAM_SIZE);
            }

            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoExpect;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseExpires
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseExpires
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_HFO_EXPIRES               pHfoExpires = NULL;
    PHTTP_DATE                      pDate       = NULL;
    BOOL                            bSucc       = TRUE;

    pHfoExpires    = (PHTTP_HFO_EXPIRES)AnscAllocateMemory(sizeof(HTTP_HFO_EXPIRES));

    if (pHfoExpires)
    {
        pDate   = &pHfoExpires->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoExpires);
            pHfoExpires = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoExpires;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseFrom
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseFrom
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* From = "From" ":" mailbox */
    /* mailbox =  addr-spec / [phrase] route-addr */
    /* addr-spec   =  local-part "@" domain */
    /* route-addr  =  "<" [route] addr-spec ">" */

    PHTTP_HFO_FROM                  pHfoFrom    = NULL;
    BOOL                            bSucc       = TRUE;

    pHfoFrom    = (PHTTP_HFO_FROM)AnscAllocateMemory(sizeof(HTTP_HFO_FROM));

    if (pHfoFrom)
    {
        bSucc   = HttpSmpoUtilParseMailBox(hHttpSmpo, (ANSC_HANDLE)&pHfoFrom->MailBox, pBuf, ulSize);

        if (!bSucc)
        {
            AnscFreeMemory(pHfoFrom);
            pHfoFrom = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoFrom;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseHost
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseHost
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Host = "Host" ":" host [":" port] */

    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PHTTP_HFO_HOST                  pHfoHost    = NULL;
    PUCHAR                          pToken      = pMsg;
    PUCHAR                          pPort;
    ULONG                           ulTokenSize;

    pHfoHost    = (PHTTP_HFO_HOST)AnscAllocateMemory(sizeof(HTTP_HFO_HOST));

    if (pHfoHost)
    {
        pPort   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, ulSize);

        if (!pPort)
        {
            ulTokenSize = ulSize;
        }
        else
        {
            ulTokenSize = pPort - pToken;
        }

        /* host name */
        HttpSmpoUtilCopyHeaderString(pToken, ulTokenSize, pHfoHost->HostName, ANSC_DOMAIN_NAME_SIZE);

        /* port number */
        if (!pPort)
        {
            pHfoHost->HostPort  = HTTP_SERVER_PORT;
        }
        else
        {
            pPort ++;
            pHfoHost->HostPort  = _ansc_atoi((const char *)pPort);
        }
    }

    return (ANSC_HANDLE)pHfoHost;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseIfMatch
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseIfMatch
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* If-Match = "If-Match" ":" ( "*" | 1#entity-tag ) */
    /* entity-tag = [weak] opaque-tag */

    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PHTTP_HFO_IF_MATCH              pHfoIfMatch = NULL;
    PUCHAR                          pToken      = pMsg;
    PUCHAR                          pLast       = pMsg + ulSize - 1;
    PHTTP_ENTITY_TAG                pEntityTag;
    PUCHAR                          pNext;
    PUCHAR                          pTag;
    ULONG                           ulLen;

    pHfoIfMatch    = (PHTTP_HFO_IF_MATCH)AnscAllocateMemory(sizeof(HTTP_HFO_IF_MATCH));

    if (pHfoIfMatch)
    {
        while (pToken)
        {
            if (pHfoIfMatch->TagCount >= HTTP_MAX_ETAG_NUMBER)
            {
                break;
            }

            pEntityTag  = &pHfoIfMatch->TagArray[pHfoIfMatch->TagCount ++];

            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulLen   = pNext - pToken;
            }
            else
            {
                ulLen   = pLast - pToken + 1;
            }

            pTag    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulLen);

            pEntityTag->bWeak   = (pTag != NULL);

            if (pTag)
            {
                pToken  = pTag + 1;
            }

            HttpSmpoUtilCopyHeaderString(pToken, pNext - pToken, pEntityTag->Tag, HTTP_MAX_ETAG_SIZE);

            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoIfMatch;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseIfModifiedSince
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseIfModifiedSince
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_HFO_IF_MODIFIED_SINCE     pHfoIfModifiedSince = NULL;
    PHTTP_DATE                      pDate               = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoIfModifiedSince    = (PHTTP_HFO_IF_MODIFIED_SINCE)AnscAllocateMemory(sizeof(HTTP_HFO_IF_MODIFIED_SINCE));

    if (pHfoIfModifiedSince)
    {
        pDate   = &pHfoIfModifiedSince->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoIfModifiedSince);
            pHfoIfModifiedSince    = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoIfModifiedSince;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseIfNoneMatch
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseIfNoneMatch
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* If-Match = "If-Match" ":" ( "*" | 1#entity-tag ) */
    /* entity-tag = [weak] opaque-tag */

    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_IF_NONE_MATCH         pHfoIfNoneMatch = NULL;
    PUCHAR                          pToken          = pMsg;
    PUCHAR                          pLast           = pMsg + ulSize - 1;
    PHTTP_ENTITY_TAG                pEntityTag;
    PUCHAR                          pNext;
    PUCHAR                          pTag;
    ULONG                           ulLen;

    pHfoIfNoneMatch    = (PHTTP_HFO_IF_NONE_MATCH)AnscAllocateMemory(sizeof(HTTP_HFO_IF_NONE_MATCH));

    if (pHfoIfNoneMatch)
    {
        while (pToken)
        {
            if (pHfoIfNoneMatch->TagCount >= HTTP_MAX_ETAG_NUMBER)
            {
                break;
            }

            pEntityTag  = &pHfoIfNoneMatch->TagArray[pHfoIfNoneMatch->TagCount ++];

            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulLen   = pNext - pToken;
            }
            else
            {
                ulLen   = pLast - pToken + 1;
            }

            pTag    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulLen);

            pEntityTag->bWeak   = (pTag != NULL);

            if (pTag)
            {
                pToken  = pTag + 1;
            }

            HttpSmpoUtilCopyHeaderString(pToken, pNext - pToken + 1, pEntityTag->Tag, HTTP_MAX_ETAG_SIZE);

            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoIfNoneMatch;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseIfRange
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseIfRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* If-Range = "If-Range" ":" ( entity-tag | HTTP-date ) */

    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PHTTP_HFO_IF_RANGE              pHfoIfRange = NULL;
    PHTTP_DATE                      pDate       = NULL;
    BOOL                            bSucc       = TRUE;

    pHfoIfRange    = (PHTTP_HFO_IF_RANGE)AnscAllocateMemory(sizeof(HTTP_HFO_IF_RANGE));

    if (pHfoIfRange)
    {
        pDate   = &pHfoIfRange->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (bSucc)
        {
            pHfoIfRange->bTagUsed   = FALSE;
        }
        else
        {
            PHTTP_ENTITY_TAG        pEntityTag  = &pHfoIfRange->ETag;
            PUCHAR                  pToken      = pMsg;
            PUCHAR                  pLast       = pMsg + ulSize - 1;
            PUCHAR                  pTag;

            pHfoIfRange->bTagUsed   = TRUE;
        
            pTag    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulSize);

            pEntityTag->bWeak   = (pTag != NULL);

            if (pTag)
            {
                pToken  = pTag + 1;
            }

            HttpSmpoUtilCopyHeaderString(pToken, pLast - pToken + 1, pEntityTag->Tag, HTTP_MAX_ETAG_SIZE);
        }
    }

    return (ANSC_HANDLE)pHfoIfRange;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseIfUnmodifiedSince
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_HFO_IF_UNMODIFIED_SINCE   pHfoIfUnmodifiedSince   = NULL;
    PHTTP_DATE                      pDate                   = NULL;
    BOOL                            bSucc                   = TRUE;

    pHfoIfUnmodifiedSince    = (PHTTP_HFO_IF_UNMODIFIED_SINCE)AnscAllocateMemory(sizeof(HTTP_HFO_IF_UNMODIFIED_SINCE));

    if (pHfoIfUnmodifiedSince)
    {
        pDate   = &pHfoIfUnmodifiedSince->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoIfUnmodifiedSince);
            pHfoIfUnmodifiedSince    = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoIfUnmodifiedSince;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseLastModified
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseLastModified
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_HFO_LAST_MODIFIED         pHfoLastModified        = NULL;
    PHTTP_DATE                      pDate                   = NULL;
    BOOL                            bSucc                   = TRUE;

    pHfoLastModified    = (PHTTP_HFO_LAST_MODIFIED)AnscAllocateMemory(sizeof(HTTP_HFO_LAST_MODIFIED));

    if (pHfoLastModified)
    {
        pDate   = &pHfoLastModified->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoLastModified);
            pHfoLastModified    = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoLastModified;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseLocation
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseLocation
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Location = "Location" ":" absoluteURI */

    PHTTP_HFO_LOCATION              pHfoLocation        = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoLocation  = (PHTTP_HFO_LOCATION)AnscAllocateMemory(sizeof(HTTP_HFO_LOCATION));

    if (pHfoLocation)
    {
        bSucc   = 
            HttpSmpoUtilParseAbsoluteURI
                (
                    hHttpSmpo, 
                    (ANSC_HANDLE)&pHfoLocation->PreferredUri,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoLocation);
            pHfoLocation = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoLocation;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseMaxForwards
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseMaxForwards
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    UNREFERENCED_PARAMETER(ulSize);
    /* Max-Forwards = "Max-Forwards" ":" 1*DIGIT */

    PHTTP_HFO_MAX_FORWARDS          pHfoMaxForwards     = NULL;

    pHfoMaxForwards  = (PHTTP_HFO_MAX_FORWARDS)AnscAllocateMemory(sizeof(HTTP_HFO_MAX_FORWARDS));

    if (pHfoMaxForwards)
    {
        pHfoMaxForwards->HopCount = _ansc_atoi(pBuf);
    }

    return (ANSC_HANDLE)pHfoMaxForwards;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParsePragma
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParsePragma
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PHTTP_HFO_PRAGMA                pHfoPragma  = NULL;

    pHfoPragma = (PHTTP_HFO_PRAGMA)AnscAllocateMemory(sizeof(HTTP_HFO_PRAGMA));

    if (pHfoPragma)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                pHfoPragma->Directives,
                HTTP_MAX_PRAGMA_DIRECTIVE_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoPragma;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseProxyAuthenticate
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Proxy-Authenticate = "Proxy-Authenticate" ":" 1#challenge */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_PROXY_AUTHENTICATE    pHfoProxyAuth       = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bSucc;

    pHfoProxyAuth  = (PHTTP_HFO_PROXY_AUTHENTICATE)AnscAllocateMemory(sizeof(HTTP_HFO_PROXY_AUTHENTICATE));

    if (pHfoProxyAuth)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            /* pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1); */
            pNext   = NULL; /* always assume one challenge in each header field */

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoProxyAuth->ChallengeCount >= HTTP_MAX_CHALLENGE_NUMBER)
            {
                break;
            }

            pChallenge  = &pHfoProxyAuth->ChallengeArray[pHfoProxyAuth->ChallengeCount ++];

            bSucc   = HttpSmpoUtilParseChallenge(hHttpSmpo, (ANSC_HANDLE)pChallenge, pToken, ulTokenSize);
            if (!bSucc)
            {
                pHfoProxyAuth->ChallengeCount --;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoProxyAuth;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseProxyAuthorization
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseProxyAuthorization
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Proxy-Authorization = "Proxy-Authorization" ":" credentials */

    PUCHAR                          pMsg                    = (PUCHAR)pBuf;
    PHTTP_HFO_PROXY_AUTHORIZATION   pHfoProxyAuthorization  = NULL;
    PHTTP_AUTH_CREDENTIAL           pCredentials;

    pHfoProxyAuthorization  = (PHTTP_HFO_PROXY_AUTHORIZATION)AnscAllocateMemory(sizeof(HTTP_HFO_PROXY_AUTHORIZATION));

    if (pHfoProxyAuthorization)
    {
        pCredentials = &pHfoProxyAuthorization->Credential;

        HttpSmpoUtilParseCredentials(hHttpSmpo, (ANSC_HANDLE)pCredentials, pMsg, ulSize);
    }

    return (ANSC_HANDLE)pHfoProxyAuthorization;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseRange
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Range = "Range" ":" ranges-specifier */
    /* ranges-specifier = byte-ranges-specifier */
    /* byte-ranges-specifier = bytes-unit "=" byte-range-set */
    /* byte-range-set = 1#( byte-range-spec | suffix-byte-range-spec ) */
    /* byte-range-spec = first-byte-pos "-" [last-byte-pos] */
    /* suffix-byte-range-spec = "-" suffix-length */

    PUCHAR                          pMsg                    = (PUCHAR)pBuf;
    PHTTP_HFO_RANGE                 pHfoRange               = NULL;
    PUCHAR                          pToken                  = pMsg;
    PUCHAR                          pLast                   = pToken + ulSize - 1;
    PUCHAR                          pNext;
    PUCHAR                          pHyphen;
    ULONG                           ulLen;

    pHfoRange  = (PHTTP_HFO_RANGE)AnscAllocateMemory(sizeof(HTTP_HFO_RANGE));

    if (pHfoRange)
    {
        pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, pLast - pToken + 1);
        if (pToken)
        {
            pToken ++;
        }

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulLen   = pNext - pToken;
            }
            else
            {
                ulLen   = pLast - pToken + 1;
            }

            /* process byte-range-set */
            pHyphen = _ansc_memchr(pToken, HTTP_SMPO_CHAR_HYPHEN, ulLen);

            if (!pHyphen)
            {
                AnscFreeMemory(pHfoRange);
                return (ANSC_HANDLE)NULL;
            }

            ulLen   = pHyphen - pToken;

            if (ulLen == 0)
            {
                pHfoRange->bFbpPresent  = FALSE;
            }
            else
            {
                pHfoRange->bFbpPresent  = TRUE;
                pHfoRange->FirstBytePos  = _ansc_atoi((const char *)pToken);
            }

            pToken  = pHyphen + 1;

            if (pToken >= pLast)
            {
                ulLen   = 0;
            }
            else
            {
                if (pNext)
                {
                    ulLen   = pNext - pToken;
                }
                else
                {
                    ulLen   = pLast - pToken + 1;
                }
            }

            if (ulLen == 0)
            {
                pHfoRange->bLbpPresent  = FALSE;
            }
            else
            {
                pHfoRange->bLbpPresent  = TRUE;
                pHfoRange->LastBytePos  = _ansc_atoi((const char *)pToken);
            }

            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoRange;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseReferer
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseReferer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Referer = "Referer" ":" ( absoluteURI | relativeURI ) */

    PHTTP_HFO_REFERER               pHfoReferer         = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoReferer  = (PHTTP_HFO_REFERER)AnscAllocateMemory(sizeof(HTTP_HFO_REFERER));

    if (pHfoReferer)
    {
        bSucc   = 
            HttpSmpoUtilParseAbsoluteURI
                (
                    hHttpSmpo, 
                    (ANSC_HANDLE)&pHfoReferer->ReferrerUri,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            bSucc   = 
                HttpSmpoUtilParseRelativeURI
                    (
                        hHttpSmpo, 
                        (ANSC_HANDLE)&pHfoReferer->ReferrerUri,
                        pBuf,
                        ulSize
                    );
        }

        if (!bSucc)
        {
            AnscFreeMemory(pHfoReferer);
            pHfoReferer = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoReferer;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseRetryAfter
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseRetryAfter
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Retry-After = "Retry-After" ":" ( HTTP-date | delta-seconds ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_RETRY_AFTER           pHfoRetryAfter      = NULL;
    PHTTP_DATE                      pDate               = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoRetryAfter    = (PHTTP_HFO_RETRY_AFTER)AnscAllocateMemory(sizeof(HTTP_HFO_RETRY_AFTER));

    if (pHfoRetryAfter)
    {
        pDate   = &pHfoRetryAfter->Date;

        bSucc   = 
            HttpSmpoUtilParseHttpDate
                (
                    (ANSC_HANDLE)pDate,
                    pBuf,
                    ulSize
                );

        if (bSucc)
        {
            pHfoRetryAfter->bDateUsed   = TRUE;
        }
        else
        {
            pHfoRetryAfter->bDateUsed   = FALSE;

            pHfoRetryAfter->DeltaSeconds    = _ansc_atoi((const char *)pMsg);
        }
    }

    return (ANSC_HANDLE)pHfoRetryAfter;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseServer
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseServer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_SERVER                pHfoServer      = NULL;

    pHfoServer  = (PHTTP_HFO_SERVER)AnscAllocateMemory(sizeof(HTTP_HFO_SERVER));

    if (pHfoServer)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                pHfoServer->ProductToken,
                HTTP_MAX_PRODUCT_TOKEN_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoServer;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseTE
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseTE
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* TE = "TE" ":" #(t-codings) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_TE                    pHfoTE              = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_ENCODING                  pEncoding;

    pHfoTE  = (PHTTP_HFO_TE)AnscAllocateMemory(sizeof(HTTP_HFO_TE));

    if (pHfoTE)
    {
        pToken  = pBuf;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoTE->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding   = &pHfoTE->EncodingArray[pHfoTE->EncodingCount ++];
            
            pEncoding->Type  = HttpSmpoUtilGetTransferEncoding(pToken, ulLen);

            pQuality    = (PUCHAR)_ansc_strstr((const char *)pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);
                
                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pEncoding->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoTE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseTrailer
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseTrailer
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pHttpSmpo   = (PHTTP_SIMPLE_MSG_PARSER)hHttpSmpo;
    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PUCHAR                          pLast       = pMsg + ulSize - 1;
    PHTTP_HFO_TRAILER               pHfoTrailer = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_SMPO_HEADER_XINFO         pXinfo;

    /* Trailer = "Trailer" ":" 1#field-name */

    pHfoTrailer = (PHTTP_HFO_TRAILER)AnscAllocateMemory(sizeof(HTTP_HFO_TRAILER));

    if (pHfoTrailer)
    {
        pToken  = pBuf;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);
            
            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pXinfo  = pHttpSmpo->GetHeaderXInfo((ANSC_HANDLE)pHttpSmpo, pToken, ulTokenSize);

            if (pXinfo)
            {
                pHfoTrailer->FieldArray[pHfoTrailer->FieldCount ++]   = pXinfo->HeaderId;
            }
            else
            {
                if (ulTokenSize == 1 && *pToken == HTTP_SMPO_CHAR_ASTERISK)
                {
                    pHfoTrailer->FieldArray[pHfoTrailer->FieldCount ++] = HTTP_HEADER_ID_WILDCARD;
                }
            }

            if (pHfoTrailer->FieldCount >= HTTP_MAX_FIELD_NUMBER)
            {
                break;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoTrailer;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseTransferEncoding
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseTransferEncoding
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_TRANSFER_ENCODING     pHfoTransferEncoding= NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_ENCODING                  pEncoding;

    pHfoTransferEncoding  = (PHTTP_HFO_TRANSFER_ENCODING)AnscAllocateMemory(sizeof(HTTP_HFO_TRANSFER_ENCODING));

    if (pHfoTransferEncoding)
    {
        pToken  = pBuf;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoTransferEncoding->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding           = &pHfoTransferEncoding->EncodingArray[pHfoTransferEncoding->EncodingCount ++];
            pEncoding->Type     = HttpSmpoUtilGetTransferEncoding(pToken, ulTokenSize);
            pEncoding->Quality  = HTTP_ENCODING_QUALITY_BEST;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoTransferEncoding;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseUpgrade
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseUpgrade
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_UPGRADE               pHfoUpgrade     = NULL;

    pHfoUpgrade  = (PHTTP_HFO_UPGRADE)AnscAllocateMemory(sizeof(HTTP_HFO_UPGRADE));

    if (pHfoUpgrade)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                pHfoUpgrade->ProductToken,
                HTTP_MAX_PRODUCT_TOKEN_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoUpgrade;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseUserAgent
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseUserAgent
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_USER_AGENT            pHfoUserAgent   = NULL;

    pHfoUserAgent  = (PHTTP_HFO_USER_AGENT)AnscAllocateMemory(sizeof(HTTP_HFO_USER_AGENT));

    if (pHfoUserAgent)
    {
        HttpSmpoUtilCopyHeaderString
            (
                pMsg,
                ulSize,
                pHfoUserAgent->ProductToken,
                HTTP_MAX_PRODUCT_TOKEN_SIZE
            );
    }

    return (ANSC_HANDLE)pHfoUserAgent;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseVary
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseVary
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pHttpSmpo   = (PHTTP_SIMPLE_MSG_PARSER)hHttpSmpo;
    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PUCHAR                          pLast       = pMsg + ulSize - 1;
    PHTTP_HFO_VARY                  pHfoVary    = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_SMPO_HEADER_XINFO         pXinfo;

    /* Trailer = "Trailer" ":" 1#field-name */

    pHfoVary = (PHTTP_HFO_VARY)AnscAllocateMemory(sizeof(HTTP_HFO_VARY));

    if (pHfoVary)
    {
        pToken  = pBuf;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);
            
            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pXinfo  = pHttpSmpo->GetHeaderXInfo((ANSC_HANDLE)pHttpSmpo, pToken, ulTokenSize);

            if (pXinfo)
            {
                pHfoVary->FieldArray[pHfoVary->FieldCount ++]   = pXinfo->HeaderId;
            }
            else
            {
                if (ulTokenSize == 1 && *pToken == HTTP_SMPO_CHAR_ASTERISK)
                {
                    pHfoVary->FieldArray[pHfoVary->FieldCount ++] = HTTP_HEADER_ID_WILDCARD;
                }
            }

            if (pHfoVary->FieldCount >= HTTP_MAX_FIELD_NUMBER)
            {
                break;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoVary;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseVia
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseVia
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Via = "Via" ":" 1#( received-protocol received-by [comment] ) */
    /* received-protocol = [protocol-name "/"] protocol-version */
    /* received-by = ( host [":" port] ) | pseudonym  */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_VIA                   pHfoVia             = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pHostPort;
    ULONG                           ulLen;
    PUCHAR                          pVersion;
    ULONG                           ulVersion;
    PUCHAR                          pName;
    ULONG                           ulName;
    PHTTP_VIA_HOP                   pHop;
    PUCHAR                          pHost;
    ULONG                           ulHost;
    PUCHAR                          pPort;

    pHfoVia  = (PHTTP_HFO_VIA)AnscAllocateMemory(sizeof(HTTP_HFO_VIA));

    if (pHfoVia)
    {
        pToken  = pBuf;

        while (pToken)
        {
            if (pHfoVia->ViaHopCount >= HTTP_MAX_VIA_HOP_NUMBER)
            {
                break;
            }

            pHop    = &pHfoVia->ViaHopArray[pHfoVia->ViaHopCount ++];

            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pHostPort   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, ulTokenSize);

            if (pHostPort)
            {
                ulLen       = pHostPort - pToken;

                pVersion    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, ulLen);

                if (!pVersion)
                {
                    pVersion    = pToken;
                    pName       = NULL;
                    ulName      = 0;
                    ulVersion   = ulLen;
                }
                else
                {
                    pName       = pToken;
                    ulName      = pVersion - pToken;

                    pVersion ++;
                    ulVersion   = pHostPort - pVersion;
                }

                if (pName && ulName != 0)
                {
                    HttpSmpoUtilCopyHeaderString(pName, ulName, pHop->Protocol, HTTP_MAX_VIA_TOKEN_SIZE);
                }

                if (pVersion && ulVersion != 0)
                {
                    HttpSmpoUtilCopyHeaderString(pVersion, ulVersion, pHop->Version, HTTP_MAX_VIA_TOKEN_SIZE);
                }

                /* host and port */
                pToken  = pHostPort + 1;
                pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
                pPort   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);

                if (pPort)
                {
                    pHost   = pToken;
                    ulHost  = pPort - pHost + 1;

                    pPort ++;
                }
                else
                {
                    pHost   = pToken;
                    ulHost  = pNext - pHost;
                }

                if (pHost && ulHost)
                {
                    HttpSmpoUtilCopyHeaderString(pHost, ulHost, pHop->HostName, HTTP_MAX_VIA_TOKEN_SIZE);
                }

                if (pPort)
                {
                    pHop->HostPort  = _ansc_atoi((const char *)pPort);
                }
                else
                {
                    pHop->HostPort  = HTTP_SERVER_PORT;
                }
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoVia;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseWarning
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseWarning
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Warning = "Warning" ":" 1#warning-value */
    /* warning-value = warn-code SP warn-agent SP warn-text */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_WARNING               pHfoWarning         = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_WARNING_VALUE             pWarning;
    PUCHAR                          pText;
    ULONG                           ulLen;
    PUCHAR                          pHost;
    ULONG                           ulHost;
    PUCHAR                          pPort;

    pHfoWarning  = (PHTTP_HFO_WARNING)AnscAllocateMemory(sizeof(HTTP_HFO_WARNING));

    if (pHfoWarning)
    {
        pToken  = pBuf;

        while (pToken)
        {
            if (pHfoWarning->WarningCount >= HTTP_MAX_WARNING_VALUE_NUMBER)
            {
                break;
            }

            pWarning    = &pHfoWarning->WarningArray[pHfoWarning->WarningCount ++];

            /* code */
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pWarning->Code  = _ansc_atoi((const char *)pToken);

            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            /* agent */
            pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, ulTokenSize);
            if(pToken)/*RDKB-6238, CID-24268, if NULL break, else break*/
            {
                pToken ++;
            } else
            {
                break;
            }

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            /* host and port */
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, ulTokenSize);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            /* text */
            pText   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, ulTokenSize);

            if (pText)
            {
                PUCHAR              pWtext  = pText;

                ulTokenSize = pText - pToken;

                if (pNext)
                {
                    ulLen   = pNext - pText;
                }
                else
                {
                    ulLen   = pLast - pText + 1;
                }
                
                pWtext   = HttpSmpoUtilLinearWhiteSpace(pWtext, ulLen);

                if (pNext)
                {
                    ulLen   = pNext - pWtext;
                }
                else
                {
                    ulLen   = pLast - pWtext + 1;
                }

                HttpSmpoUtilCopyHeaderString(pWtext, ulLen, pWarning->Text, HTTP_MAX_WARNING_TEXT_SIZE);
            }

            pPort   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, ulTokenSize);

            if (pPort)
            {
                pHost   = pToken;
                ulHost  = pPort - pHost;

                pPort ++;
            }
            else
            {
                pHost   = pToken;
                ulHost  = pNext - pHost;

                if (pText)
                {
                    ulHost  = pText - pHost;
                }
            }

            if (pHost && ulHost)
            {
                HttpSmpoUtilCopyHeaderString(pHost, ulHost, pWarning->HostName, ANSC_DOMAIN_NAME_SIZE);
            }

            if (pPort)
            {
                pWarning->HostPort  = _ansc_atoi((const char *)pPort);
            }
            else
            {
                pWarning->HostPort  = HTTP_SERVER_PORT;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoWarning;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseWwwAuthenticate
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseWwwAuthenticate
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_WWW_AUTHENTICATE      pHfoWwwAuth         = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bSucc;

    pHfoWwwAuth  = (PHTTP_HFO_WWW_AUTHENTICATE)AnscAllocateMemory(sizeof(HTTP_HFO_WWW_AUTHENTICATE));

    if (pHfoWwwAuth)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            /* pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1); */
            pNext   = NULL; /* always assume one challenge in each header field */

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoWwwAuth->ChallengeCount >= HTTP_MAX_CHALLENGE_NUMBER)
            {
                break;
            }

            pChallenge  = &pHfoWwwAuth->ChallengeArray[pHfoWwwAuth->ChallengeCount ++];

            bSucc   = HttpSmpoUtilParseChallenge(hHttpSmpo, (ANSC_HANDLE)pChallenge, pToken, ulTokenSize);
            if (!bSucc)
            {
                pHfoWwwAuth->ChallengeCount --;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoWwwAuth;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseProxyConnection
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseProxyConnection
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_PROXY_CONNECTION      pHfoProxyConnection = (PHTTP_HFO_PROXY_CONNECTION)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_CONNECTION_TOKEN          pConnToken;
    ULONG                           ulTokenType;

    pHfoProxyConnection = (PHTTP_HFO_PROXY_CONNECTION)AnscAllocateMemory(sizeof(HTTP_HFO_PROXY_CONNECTION));

    if (pHfoProxyConnection)
    {
        pHfoProxyConnection->TokenCount = 0;

        /* parse connection tokens */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoProxyConnection->TokenCount >= HTTP_MAX_CONNECTION_TOKEN_NUMBER)
            {
                break;
            }

            pConnToken  = &pHfoProxyConnection->TokenArray[pHfoProxyConnection->TokenCount ++];

            HttpSmpoUtilCopyHeaderString
                (
                    pToken,
                    ulTokenSize,
                    pConnToken->Token,
                    HTTP_MAX_CONNECTION_TOKEN_SIZE
                );

            ulTokenType = IANA_CONNECTION_CODE_RESERVED;

            if (AnscEqualString2((char *)pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_CLOSE;
            }
            else
            if (AnscEqualString2((char *)pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_KEEP_ALIVE;
            }

            pConnToken->OptionType  = ulTokenType;

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoProxyConnection;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseCookie
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseCookie
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value ) */
    /* cookie-version  =  "$Version" "=" value ( ";" | "," ) */
    /* cookie-value = cookie-name "=" cookie-name-value [";" cookie-path] [";" cookie-domain] */
    /* value = token | quoted-string  */
    /* cookie-path = "$Path" "=" value */
    /* cookie-domain = "$Domain" "=" value */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_COOKIE                pHfoCookie          = (PHTTP_HFO_COOKIE)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount             = 0;
    PUCHAR                          pValue;
    ULONG                           ulIndex             = 0;
    ULONG                           ulLen;
    PUCHAR                          pName;
    BOOL                            bVersionPresent     = FALSE;
    BOOL                            bVersion, bDomain, bPath, bPort;

    /* get cookie content count */
    pToken  = pMsg;

    ulCount = 0;

    while (pToken)
    {
        pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
        pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

        pName   = pToken;
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        pName   = HttpSmpoUtilLinearWhiteSpace(pName, ulLen);
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            if ( pLast >= pName )
            {
                ulLen   = pLast - pName + 1;
            }
            else
            {
                ulLen   = 0;
            }
        }

        if ( ulLen == 0 )
        {
            break;
        }

        ulCount ++;

        pValue  = _ansc_memchr(pName, HTTP_SMPO_CHAR_EQUAL, ulLen);
        
        if (pValue)
        {
            ulLen   = pValue - pName;
        }
        else
        {
            ulLen   = pNext - pName;
        }

        /* check the name */
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_VERSION) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_VERSION, (char *)pName, ulLen, FALSE))
        {
            bVersionPresent = TRUE;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PATH, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_DOMAIN, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PORT, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }

        if (pNext)
        {
            pToken = pNext + 1;
        }
        else
        {
            pToken  = NULL;
        }
    }

    if (ulCount != 0 && bVersionPresent)
    {
        ulCount --;
    }

    pHfoCookie = 
        (PHTTP_HFO_COOKIE)AnscAllocateMemory
            (
                sizeof(HTTP_HFO_COOKIE) + ulCount * sizeof(HTTP_COOKIE_CONTENT)
            );

    if (pHfoCookie)
    {
        pHfoCookie->CookieCount = ulCount;

        pHfoCookie->CookieArray[0].Version = 1;

        /* parse cookie contents */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pValue  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, ulTokenSize);

            /* cookie name */
            pName   = pToken;
            if (pValue)
            {
                ulLen   = pValue - pName;
            }
            else
            {
                ulLen   = pNext - pName;
            }

            bVersion    = FALSE;
            bDomain     = FALSE;
            bPath       = FALSE;
            bPort       = FALSE;

            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_VERSION) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_VERSION, (char *)pName, ulLen, FALSE))
            {
                bVersion = TRUE;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PATH, (char *)pName, ulLen, FALSE))
            {
                bPath   = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_DOMAIN, (char *)pName, ulLen, FALSE))
            {
                bDomain = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PORT, (char *)pName, ulLen, FALSE))
            {
                bPort   = TRUE;
                ulIndex --;
            }

            if (bVersion)
            {
                pCookieContent  = &pHfoCookie->CookieArray[0];
            }
            else
            {
                if (ulIndex >= ulCount)
                {
                    break;
                }

                pCookieContent  = &pHfoCookie->CookieArray[ulIndex ++];
            }

            if (!bVersion && !bPath && !bDomain && !bPort)
            {
                /* cookie value name */
                HttpSmpoUtilCopyHeaderString(pToken, ulLen, pCookieContent->Name, HTTP_MAX_COOKIE_NAME_SIZE);
            }

            /* cookie value */
            if (bVersion)
            {
                if ( pValue )
                {
                    pValue ++;
                    if ( *pValue == HTTP_SMPO_CHAR_DQUOTE )
                    {
                        pValue ++;
                    }
                    pHfoCookie->CookieArray[0].Version = _ansc_atoi((const char *)pValue);
                }
            }
            else
            if (bPath)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Path, HTTP_MAX_PATH_NAME_SIZE);
                }
            }
            else
            if (bDomain)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Domain, ANSC_DOMAIN_NAME_SIZE);
                }
            }
            else
            if (bPort)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Port, HTTP_MAX_COOKIE_PORT_SIZE);
                }
            }
            else
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Value, HTTP_MAX_COOKIE_VALUE_SIZE);
                }
            }

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoCookie;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseSetCookie
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseSetCookie
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* Set-Cookie  = "Set-Cookie:" 1#Cookie-Entity */
    /* Cookie-Entity = cookie-name "=" cookie-name-value *(";" cookie-av) */
    /* cookie-name-value = cookie-name-value-token | quoted-string */
    /*
        cookie-av       =   "Comment"   "="     comment
                    |       "Domain"    "="     set-cookie-domain
                    |       "Max-Age"   "="     delta-seconds
                    |       "Path"      "="     set-cookie-path
                    |       "Secure"
                    |       "Version"   "="     1*DIGIT
                    |       "expires"   "="     cookie-expires-date
                    |       token       "="     value
    */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_SET_COOKIE            pHfoSetCookie       = (PHTTP_HFO_SET_COOKIE)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount             = 0;
    PUCHAR                          pValue;
    ULONG                           ulIndex             = 0;
    ULONG                           ulLen;
    BOOL                            bSecure;
    BOOL                            bMaxAge;
    BOOL                            bDomain;
    BOOL                            bPath;
    BOOL                            bComment;
    BOOL                            bExpires;
    PUCHAR                          pName;
    BOOL                            bVersion;
    BOOL                            bDiscard;
    BOOL                            bCommentUrl;
    BOOL                            bPort;

    /* get cookie content count */
    pToken  = pMsg;

    ulCount = 0;

    while (pToken)
    {
        pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
        pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

        pName   = pToken;
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        if ( ulLen == 0 )
        {
            break;
        }

        ulCount ++;

        pName   = HttpSmpoUtilLinearWhiteSpace(pName, ulLen);
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        pValue  = _ansc_memchr(pName, HTTP_SMPO_CHAR_EQUAL, ulLen);
        
        if (pValue)
        {
            ulLen   = pValue - pName;
        }
        else
        {
            ulLen   = pNext ? pNext - pName : pLast - pName + 1;
        }

        /* check the name */
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_COMMENT) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_COMMENT, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_DOMAIN) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_DOMAIN, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_PORT) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_PORT, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_PATH) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_PATH, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_SECURE) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_SECURE, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_DISCARD) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_DISCARD, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_VERSION) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_VERSION, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_EXPIRES) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_EXPIRES, (char *)pName, ulLen, FALSE))
        {
            ulCount --;
        }

        if (pNext)
        {
            pToken = pNext + 1;
        }
        else
        {
            pToken  = NULL;
        }
    }

    pHfoSetCookie = 
        (PHTTP_HFO_SET_COOKIE)AnscAllocateMemory
            (
                sizeof(HTTP_HFO_SET_COOKIE) + ulCount * sizeof(HTTP_COOKIE_CONTENT)
            );

    if (pHfoSetCookie)
    {
        ULONG                       i;

        pHfoSetCookie->CookieCount = ulCount;

        for (i = 0; i < ulCount; i ++)
        {
            pHfoSetCookie->CookieArray[i].bPermanent        = FALSE;
            pHfoSetCookie->CookieArray[i].MaxAgeInSeconds   = HTTP_COOKIE_MAX_AGE_ABSENT;
        }

        /* parse cookie contents */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if ( ulTokenSize == 0 )
            {
                break;
            }

            pValue  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, ulTokenSize);

            /* cookie name */
            pName   = pToken;
            if (pValue)
            {
                ulLen   = pValue - pName;
            }
            else
            {
                ulLen   = pNext ? pNext - pName : pLast - pName + 1;
            }

            bComment    = FALSE;
            bDomain     = FALSE;
            bPath       = FALSE;
            bSecure     = FALSE;
            bMaxAge     = FALSE;
            bExpires    = FALSE;
            bCommentUrl = FALSE;
            bDiscard    = FALSE;
            bPort       = FALSE;
            bVersion    = FALSE;

            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_COMMENT) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_COMMENT, (char *)pName, ulLen, FALSE))
            {
                bComment    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL, (char *)pName, ulLen, FALSE))
            {
                bCommentUrl = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_DOMAIN) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_DOMAIN, (char *)pName, ulLen, FALSE))
            {
                bDomain     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_PORT) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_PORT, (char *)pName, ulLen, FALSE))
            {
                bPort       = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE, (char *)pName, ulLen, FALSE))
            {
                bMaxAge     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_PATH) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_PATH, (char *)pName, ulLen, FALSE))
            {
                bPath       = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_SECURE) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_SECURE, (char *)pName, ulLen, FALSE))
            {
                bSecure     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_DISCARD) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_DISCARD, (char *)pName, ulLen, FALSE))
            {
                bDiscard    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_VERSION) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_VERSION, (char *)pName, ulLen, FALSE))
            {
                bVersion    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_EXPIRES) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_EXPIRES, (char *)pName, ulLen, FALSE))
            {
                bExpires    = TRUE;
                ulIndex --;
            }

            if (ulIndex >= pHfoSetCookie->CookieCount)
            {
                break;
            }

            pCookieContent  = &pHfoSetCookie->CookieArray[ulIndex ++];

            if (!bComment && !bMaxAge && !bSecure && !bPath && !bDomain && !bPort && !bDiscard && !bCommentUrl && !bVersion)
            {
                /* cookie value name */
                HttpSmpoUtilCopyHeaderString(pToken, ulLen, pCookieContent->Name, HTTP_MAX_COOKIE_NAME_SIZE);
            }

            /* cookie value */
            if (bPath)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Path, HTTP_MAX_PATH_NAME_SIZE);
            }
            else
            if (bDomain)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Domain, ANSC_DOMAIN_NAME_SIZE);
            }
            else
            if (bPort)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Port, HTTP_MAX_COOKIE_PORT_SIZE);
            }
            else
            if (bComment)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Comment, HTTP_MAX_COOKIE_COMMENT_SIZE);
            }
            else
            if (bCommentUrl)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->CommentUrl, ANSC_URI_STRING_SIZE);
            }
            else
            if (bMaxAge)
            {
                pValue ++;

                pCookieContent->MaxAgeInSeconds = _ansc_atoi((const char *)pValue);
            }
            else
            if (bVersion)
            {
                pValue ++;

                if (*pValue == '"')
                {
                    pValue ++;
                }

                pCookieContent->Version = _ansc_atoi((const char *)pValue);
            }
            else
            if (bSecure)
            {
                pCookieContent->bSecure = TRUE;
            }
            else
            if (bDiscard)
            {
                pCookieContent->bDiscard = TRUE;
            }
            else
            if ( bExpires )
            {
                ANSC_UNIVERSAL_TIME *pTime      = &pCookieContent->Expires;
                /*CID: 137562 Uninitialized scalar variable*/
                char                wday[16] = {0};
                char                mon[3] = {0};
                char*               pDate = NULL;

                pTime->bDayLightSaving = FALSE;

                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                pDate = _ansc_memchr(pValue, HTTP_SMPO_CHAR_COMMA, ulLen);

                if ( pDate )
                {
                    ULONG           ulWdayLen = ((PUCHAR)pDate) - pValue;
                    
                    AnscCopyMemory(wday, pValue, ulWdayLen);
                    wday[ulWdayLen] = 0;

                    pDate ++;
                    if ( *pDate == ' ' )
                    {
                        pDate ++;
                    }
                }

                if ( pDate )
                {
                    _ansc_sscanf
                        (
                            pDate, 
                            "%d-%3c-%d %d:%d:%d GMT",
                            (int *)&pTime->DayOfMonth,
                            mon,
                            (int *)&pTime->Year,
                            (int *)&pTime->Hour,
                            (int *)&pTime->Minute,
                            (int *)&pTime->Second
                        );

                    pCookieContent->bPermanent = TRUE;
                }

                pTime->Month = HttpSmpoUtilGetMonth((PUCHAR)mon, 3);

                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_SUNDAY) == 0 )
                    pTime->DayOfWeek = 0;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_MONDAY) == 0 )
                    pTime->DayOfWeek = 1;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_TUESDAY) == 0 )
                    pTime->DayOfWeek = 2;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_WEDNESDAY) == 0 )
                    pTime->DayOfWeek = 3;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_THURSDAY) == 0 )
                    pTime->DayOfWeek = 4;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_FRIDAY) == 0 )
                    pTime->DayOfWeek = 5;
                else
                if (strcasecmp(wday, HTTP_SMPO_WEEKDAY_SATURDAY) == 0 )
                    pTime->DayOfWeek = 6;
            }
            else if ( pValue )
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Value, HTTP_MAX_COOKIE_VALUE_SIZE);
            }

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoSetCookie;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseCookie2
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseCookie2
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    UNREFERENCED_PARAMETER(ulSize);
    UNREFERENCED_PARAMETER(pBuf);
    /* Cookie2: $Version="1" */

    PHTTP_HFO_COOKIE2               pHfoCookie2         = (PHTTP_HFO_COOKIE2)NULL;

    pHfoCookie2 = (PHTTP_HFO_COOKIE2)AnscAllocateMemory(sizeof(HTTP_HFO_COOKIE2));

    return (ANSC_HANDLE)pHfoCookie2;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseSetCookie2
            (
                ANSC_HANDLE                 hHttpSmpo,
                void                        *pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse a HTTP header field.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                void                        *pBuf
                The buffer containing the header string to
                be parsed.

                ULONG                       ulSize
                Size of header string.
    
    return:     handle to the specific header.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseSetCookie2
    (
        ANSC_HANDLE                 hHttpSmpo,
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Set-Cookie  = "Set-Cookie:" 1#Cookie-Entity */
    /* Cookie-Entity = cookie-name "=" cookie-name-value *(";" cookie-av) */
    /* cookie-name-value = cookie-name-value-token | quoted-string */
    /*
     *   cookie-av       =   "Comment"   "="     comment
     *               |       "Domain"    "="     set-cookie-domain
     *               |       "Max-Age"   "="     delta-seconds
     *               |       "Path"      "="     set-cookie-path
     *               |       "Secure"
     *               |       "Version"   "="     1*DIGIT
     *               |       "expires"   "="     cookie-expires-date
     *               |       token       "="     value
     */


    return HttpSmpoParseSetCookie(hHttpSmpo, pBuf, ulSize);
}


