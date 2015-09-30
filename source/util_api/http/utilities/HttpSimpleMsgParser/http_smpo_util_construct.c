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

    module:	http_smpo_util_parsing.c

        For HTTP simple message parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

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

**********************************************************************/


#include "http_smpo_global.h"


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilCopyString
            (
                PUCHAR                      pDest,
                ULONG                       ulDestSize,
                ULONG                       *pulDestStart,
                PUCHAR                      pSrc,
                ULONG                       ulSrcSize
            )

    description:

        This function is called to copy given string into destination
        buffer.

    argument:   PUCHAR                      pDest
                Destination buffer.

                ULONG                       ulDestSize
                The length of destination buffer.
    
                ULONG                       *pulDestStart
                Where to start copying and it contains the next 
                available position when this operation is done.

                PUCHAR                      pSrc
                The source buffer.

                ULONG                       ulSrcSize
                The size of source buffer.

    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilCopyString
    (
        PUCHAR                      pDest,
        ULONG                       ulDestSize,     
        ULONG                       *pulDestStart,        /* where copy begins, 0-based */
        PUCHAR                      pSrc,
        ULONG                       ulSrcSize
    )
{
    BOOL                            bBufExhausted   = FALSE;
    ULONG                           ulCopySize, ulDestLeft;
    ULONG                           ulDestStart     = *pulDestStart;

    ulCopySize      = ulSrcSize;
    ulDestLeft      = ulDestSize - ulDestStart;

    if (ulDestLeft < ulSrcSize)
    {
        ulCopySize      = ulDestLeft;
        bBufExhausted   = TRUE;
    }

    if (ulCopySize != 0)
    {
        AnscCopyMemory(&pDest[ulDestStart], pSrc, ulCopySize);
        ulDestStart += ulCopySize;
    }

    *pulDestStart   = ulDestStart;

    return !bBufExhausted;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildRequestUri
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                PHTTP_REQUEST_URI           pUri
            )

    description:

        This function is called to build request URI.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                PHTTP_REQUEST_URI           pUri
                HTTP Uri object.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildRequestUri
    (
        PVOID                       buffer,
        ULONG                       ulDestSize,
        ULONG                       *pulBufStart,
        PHTTP_REQUEST_URI           pUri
    )
{
    /*
     *  Request-URI = "*" | absoluteURI | abs_path | authority
     */
    BOOL                            bCopySucc       = FALSE;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PUCHAR                          pBuf            = (PUCHAR)buffer;
    ULONG                           ulStart         = *pulBufStart;
    ULONG                           ulSize;
    BOOL                            bAppendPort     = FALSE;
    BOOL                            bHttps          = FALSE;

    bHttps  = ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) != 0 );

    switch (pUri->Type)
    {
    case HTTP_URI_TYPE_ASTERISK:
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             &ulStart,
                             HTTP_SMPO_REQUEST_URI_ASTERISK,
                             HTTP_SMPO_REQUEST_URI_ASTERISK_LENGTH
                         );
        break;

    case HTTP_URI_TYPE_ABSOLUTE_URI:
        /*
         *     absoluteURI = scheme ":" *( uchar | reserved ) 
         */

        if ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) == 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf, 
                                 ulDestSize, 
                                 &ulStart,
                                 HTTP_SMPO_SCHEME,
                                 HTTP_SMPO_SCHEME_LENGTH
                             );
        }
        else
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf, 
                                 ulDestSize, 
                                 &ulStart,
                                 HTTP_SMPO_SCHEME_HTTPS,
                                 HTTP_SMPO_SCHEME_HTTPS_LENGTH
                             );
        }

        if (!bCopySucc)
            return FALSE;

        ulSize      = AnscSizeOfString(pUri->HostName);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf, 
                             ulDestSize, 
                             &ulStart,
                             pUri->HostName,
                             ulSize
                         );

        /*  port number */
        if (bHttps)
        {
            bAppendPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
        }
        else
        {
            bAppendPort = (pUri->HostPort != HTTP_SERVER_PORT);
        }

        if (bAppendPort)
        {
            UCHAR                   portNum[8];

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf, 
                                 ulDestSize, 
                                 &ulStart,
                                 (PUCHAR)":",
                                 1
                             );
            if (!bCopySucc)
                return FALSE;

            _ansc_sprintf(portNum, "%d", pUri->HostPort);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf, 
                                 ulDestSize, 
                                 &ulStart,
                                 portNum,
                                 AnscSizeOfString(portNum)
                             );
            if (!bCopySucc)
                return FALSE;
        }

        if (pUri->PathLevel >= 2)
        {
            ULONG                   ulPathLevel, i;

            ulPathLevel     = pUri->PathLevel;
            for (i = 1; i < ulPathLevel; i ++)
            {
                ulSize      = 1;
                bCopySucc   = HttpSmpoUtilCopyString
                                 ( 
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "/",
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;

                if (
                       AnscEqualString(pUri->PathArray[i], "/", TRUE)
                   )
                {
                    /* already counted */
                    continue;
                }

                ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[i],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        else
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 "/",
                                 1
                             );
        }
        break;

    case HTTP_URI_TYPE_ABS_PATH:
        {
            /*  
             *  abs_path = "/" rel_path 
             *  rel_path = [path] [";" params] ["?" query] 
             *  path = fsegment *( "/" segment ) 
             */
            ULONG                   ulPathLevel, i;

            ulPathLevel     = pUri->PathLevel;

            if (
                   (ulPathLevel == 1 && AnscEqualString(pUri->PathArray[0], "/", TRUE)) ||
                   ulPathLevel == 0
               )
            {
                ulSize      = 1;
                bCopySucc   = HttpSmpoUtilCopyString
                                 ( 
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "/",
                                     ulSize
                                 );
            }
            else
            {
                for (i = 1; i < ulPathLevel; i ++)
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     ( 
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;

                    if (
                           AnscEqualString(pUri->PathArray[i], "/", TRUE)
                       )
                    {
                        /* already counted */
                        continue;
                    }

                    ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                    bCopySucc   = HttpSmpoUtilCopyString
                                     (
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         pUri->PathArray[i],
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;
                }
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        break;

    case HTTP_URI_TYPE_RELATIVE_URI:
        {
            /*
             *  relativeURI = net_path | abs_path | rel_path 
             *  net_path = "//" net_loc [abs_path] 
             */
            ULONG                   i, ulPathLevel;

#if 0   /* ??? */
            ulSize      = AnscSizeOfString(pUri->HostName);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "//",
                                     2
                                 );
                if (!bCopySucc)
                    return FALSE;

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf, 
                                     ulDestSize, 
                                     &ulStart,
                                     pUri->HostName,
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }
#endif

            ulPathLevel     = pUri->PathLevel;

            if (ulPathLevel == 1)
            {
                ulSize      = AnscSizeOfString(pUri->PathArray[0]);

                if (ulSize != 1 || pUri->PathArray[0][0] != '/')
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     ( 
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                }

                bCopySucc   = HttpSmpoUtilCopyString
                                 ( 
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[0],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            for (i = 1; i < ulPathLevel; i ++)
            {
                ULONG               ulPathRoot;
                BOOL                bAddSlash;

                bAddSlash   = TRUE;

                if (i == 1)
                {
                    ulPathRoot  = AnscSizeOfString(pUri->PathArray[0]);
                    bAddSlash   = (ulPathRoot != 0);
                }

                if (bAddSlash)
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     ( 
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;
                }

                ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[i],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );



                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        break;

    case HTTP_URI_TYPE_AUTHORITY:
        /*
         *  authority = authority-server | reg_name
         *  authority-server = [ [ userinfo "@" ] hostport ]
         *  hostport = host [ ":" port ]
         *  reg_name = 1*( uri-unreserved | escape | "$" | "," |
         *                 ";" | ":" | "@" | "&" | "=" | "+" )
         */
        ulSize  = AnscSizeOfString(pUri->HostName);
        if (ulSize != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 pUri->HostName,
                                 ulSize
                             );

            if (!bCopySucc)
                return FALSE;

            if (bHttps)
            {
                bAppendPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bAppendPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bAppendPort)
            {
                UCHAR               buf[8];

                AnscInt2String(pUri->HostPort, buf, 10);

                ulSize  = AnscSizeOfString(buf);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     ":",
                                     1
                                 );
                if (!bCopySucc)
                    return FALSE;

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     buf,
                                     ulSize
                                 );

                if (!bCopySucc)
                    return FALSE;
            }
        }
        break;
    } 

    *pulBufStart    = ulStart;

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildMethod
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulMethod,
                PUCHAR                      pMethodName
            )

    description:

        This function is called to build HTTP method.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulMethod
                HTTP method code.

                PUCHAR                      pMethodName
                Method name used if ulMethod is defined.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildMethod
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulMethod,
        PUCHAR                      pMethodName
    )
{
    ULONG                           ulLen, ulBufLeft;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc        = NULL;
    ULONG                           ulStart     = *pulStart;

    ulLen = 0;

    switch (ulMethod)
    {
    case HTTP_METHOD_OPTIONS:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS);
        pSrc    = HTTP_METHOD_NAME_OPTIONS;
        break;

    case HTTP_METHOD_GET:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_GET);
        pSrc    = HTTP_METHOD_NAME_GET;
        break;

    case HTTP_METHOD_HEAD:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_HEAD);
        pSrc    = HTTP_METHOD_NAME_HEAD;
        break;

    case HTTP_METHOD_POST:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_POST);
        pSrc    = HTTP_METHOD_NAME_POST;
        break;

    case HTTP_METHOD_PUT:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_PUT);
        pSrc    = HTTP_METHOD_NAME_PUT;
        break;

    case HTTP_METHOD_DELETE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_DELETE);
        pSrc    = HTTP_METHOD_NAME_DELETE;
        break;

    case HTTP_METHOD_TRACE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_TRACE);
        pSrc    = HTTP_METHOD_NAME_TRACE;
        break;

    case HTTP_METHOD_CONNECT:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_CONNECT);
        pSrc    = HTTP_METHOD_NAME_CONNECT;
        break;

    case HTTP_METHOD_NOTIFY:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY);
        pSrc    = HTTP_METHOD_NAME_NOTIFY;
        break;

    case HTTP_METHOD_SEARCH:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_SEARCH);
        pSrc    = HTTP_METHOD_NAME_SEARCH;
        break;

    case HTTP_METHOD_M_SEARCH:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH);
        pSrc    = HTTP_METHOD_NAME_M_SEARCH;
        break;

    case HTTP_METHOD_M_POST:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_M_POST);
        pSrc    = HTTP_METHOD_NAME_M_POST;
        break;

    case HTTP_METHOD_SUBSCRIBE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE);
        pSrc    = HTTP_METHOD_NAME_SUBSCRIBE;
        break;

    case HTTP_METHOD_UNSUBSCRIBE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE);
        pSrc    = HTTP_METHOD_NAME_UNSUBSCRIBE;
        break;

    default:
        ulLen   = pMethodName ? AnscSizeOfString(pMethodName) : 0;
        pSrc    = pMethodName;
        break;
    }

    if (ulLen != 0)
    {
        ulBufLeft   = ulDestSize - ulStart;

        if (ulLen > ulBufLeft)
        {
            bCopySucc   = FALSE;
            ulLen       = ulBufLeft;
        }

        AnscCopyMemory(&pBuf[ulStart], pSrc, ulLen);

        ulStart += ulLen;
    }

    *pulStart   = ulStart;

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildHttpVersion
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       MajorVerion,
                ULONG                       MinorVersion
            )

    description:

        This function is called to build HTTP version.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       MajorVersion
                HTTP major version.

                ULONG                       MinorVersion
                HTTP minor version.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildHttpVersion
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       MajorVersion,
        ULONG                       MinorVersion
    )
{
    /*
     *  HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT 
     */
    UCHAR                           buf[80];
    ULONG                           ulSize;
    BOOL                            bCopySucc;
    ULONG                           ulStart = *pulStart;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         HTTP_SMPO_NAME,
                         HTTP_SMPO_NAME_LENGTH
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         "/",
                         1
                     );
    if (!bCopySucc)
        return FALSE;

    AnscInt2String(MajorVersion, buf, 10);      
    ulSize  = AnscSizeOfString(buf);           /* major version number length */

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         buf,
                         ulSize
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         ".",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    AnscInt2String(MinorVersion, buf, 10);
    ulSize  = AnscSizeOfString(buf);           /* minor version number length */

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         buf,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    *pulStart   = ulStart;

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildMediaType
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulMediaType
            )

    description:

        This function is called to build media type.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulMediaType
                Media type.
    
    return:     status of operation.

**********************************************************************/

static 
BOOL
HttpSmpoUtilBuildMediaType
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulMediaType
    )
{
    ULONG                           ulSize      = 0;
    ULONG                           ulStart     = *pulStart;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc;

    switch (ulMediaType)
    {
    case IANA_MEDIA_TYPE_CODE_TEXT:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT);
        pSrc    = IANA_MEDIA_TYPE_TEXT_TEXT;
        break;

    case IANA_MEDIA_TYPE_CODE_MULTIPART:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART);
        pSrc    = IANA_MEDIA_TYPE_TEXT_MULTIPART;
        break;

    case IANA_MEDIA_TYPE_CODE_MESSAGE:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE);
        pSrc    = IANA_MEDIA_TYPE_TEXT_MESSAGE;
        break;

    case IANA_MEDIA_TYPE_CODE_APPLICATION:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION);
        pSrc    = IANA_MEDIA_TYPE_TEXT_APPLICATION;
        break;

    case IANA_MEDIA_TYPE_CODE_IMAGE:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE);
        pSrc    = IANA_MEDIA_TYPE_TEXT_IMAGE;
        break;

    case IANA_MEDIA_TYPE_CODE_AUDIO:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO);
        pSrc    = IANA_MEDIA_TYPE_TEXT_AUDIO;
        break;

    case IANA_MEDIA_TYPE_CODE_VIDEO:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO);
        pSrc    = IANA_MEDIA_TYPE_TEXT_VIDEO;
        break;

    default:
        ulSize  = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
        pSrc    = HTTP_SMPO_MEDIA_TYPE_ANY;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             &ulStart,
                             pSrc,
                             ulSize
                         );
    }

    *pulStart   = ulStart;

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildMediaSubType
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulType,
                ULONG                       ulSubType
            )

    description:

        This function is called to build media subtype.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulType
                Media type.

                ULONG                       ulSubType
                Media subtype.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildMediaSubType
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulType,
        ULONG                       ulSubType
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    pSrc    = AnscGetIanaMediaSubtypeNameByType(ulType, ulSubType);

    if (!pSrc)
    {
        ulSize  = HTTP_SMPO_MEDIA_SUBTYPE_ANY_LENGTH;
        pSrc    = HTTP_SMPO_MEDIA_SUBTYPE_ANY;
    }
    else
    {
        ulSize  = AnscSizeOfString(pSrc);
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf, 
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}

/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildCharset
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulCharset
            )

    description:

        This function is called to build charset.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulCharset
                Charset code.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildCharset
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulCharset
    )
{
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc        = NULL;

    switch (ulCharset)
    {
    case IANA_CHARSET_CODE_US_ASCII:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_US_ASCII);
        pSrc    = IANA_CHARSET_TEXT_US_ASCII;
        break;

    case IANA_CHARSET_CODE_ISO_8859_1:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_1);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_1;
        break;

    case IANA_CHARSET_CODE_ISO_8859_2:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_2);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_2;
        break;

    case IANA_CHARSET_CODE_ISO_8859_3:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_3);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_3;
        break;

    case IANA_CHARSET_CODE_ISO_8859_4:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_4);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_4;
        break;

    case IANA_CHARSET_CODE_ISO_8859_5:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_5);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_5;
        break;

    case IANA_CHARSET_CODE_ISO_8859_6:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_6);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_6;
        break;

    case IANA_CHARSET_CODE_ISO_8859_7:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_7);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_7;
        break;

    case IANA_CHARSET_CODE_ISO_8859_8:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_8);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_8;
        break;

    case IANA_CHARSET_CODE_ISO_8859_9:
        ulSize  = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_9);
        pSrc    = IANA_CHARSET_TEXT_ISO_8859_9;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildContentCoding
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulCoding
            )

    description:

        This function is called to build content coding length.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulCoding
                Content coding.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildContentCoding
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        pSrc    = IANA_CCODING_TEXT_GZIP;
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        pSrc    = IANA_CCODING_TEXT_COMPRESS;
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        pSrc    = IANA_CCODING_TEXT_DEFLATE;
        break;
        
    case IANA_CCODING_CODE_IDENTITY:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        pSrc    = IANA_CCODING_TEXT_IDENTITY;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildTransferCoding
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulCoding
            )

    description:

        This function is called to build content coding length.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulCoding
                Transfer coding number.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildTransferCoding
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        pSrc    = IANA_CCODING_TEXT_GZIP;
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        pSrc    = IANA_CCODING_TEXT_COMPRESS;
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        pSrc    = IANA_CCODING_TEXT_DEFLATE;
        break;
        
    case IANA_CCODING_CODE_IDENTITY:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        pSrc    = IANA_CCODING_TEXT_IDENTITY;
        break;
        break;

    case IANA_TCODING_CODE_CHUNKED:
        ulSize  = AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED);
        pSrc    = IANA_TCODING_TEXT_CHUNKED;
        break;
        
    case IANA_TCODING_CODE_TRAILERS:
        ulSize  = AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS);
        pSrc    = IANA_TCODING_TEXT_TRAILERS;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildQuality
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulQuality
            )

    description:

        This function is called to build quality.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulQuality
                quality value.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildQuality
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulQuality
    )
{
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc   = TRUE;

    if (ulQuality == 1000)
    {
        ulSize = 3;                          /* "1.0" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "1.0",
                             ulSize
                         );
    }
    else
    if (ulQuality == 0)
    {
        ulSize = 1;                          /* "0" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "0",
                             ulSize
                         );
    }
    else
    {
        UCHAR                       buf[16];
        ULONG                       i;

        buf[0]      = '0';
        buf[1]      = '.';

        AnscInt2String(ulQuality, &buf[2], 10);
        ulSize   = AnscSizeOfString(buf);

        for (i = ulSize - 1; i >= 0; i --)
        {
            if (buf[i] != '0')
            {
                if (i != ulSize - 1)
                {
                    buf[i + 1] = 0;
                }
                break;
            }
        }

        ulSize   = AnscSizeOfString(buf);

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             buf,
                             ulSize
                         );
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildUlong
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulValue
            )

    description:

        This function is called to build content coding length.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulValue
                ULONG value.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildUlong
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulValue
    )
{
    UCHAR                       buf[16];
    ULONG                       ulSize;
    BOOL                        bCopySucc = TRUE;

    AnscInt2String(ulValue, buf, 10);
    ulSize      = AnscSizeOfString(buf);

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildHttpDate
            (
                PVOID                       buffer,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                PHTTP_DATE                  pDate
            )

    description:

        This function is called to build HTTP date.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                PHTTP_DATE                  pDate
                HTTP date object.
    
    return:     status of operation.

**********************************************************************/

static
ULONG
HttpSmpoUtilBuildHttpDate
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        PHTTP_DATE                  pDate
    )
{
    /* the length is calculated according to RFC 1123 format */
    /*
     *  rfc1123-date = wkday "," SP date1 SP time SP "GMT"
     *  date1 = 2DIGIT SP month SP 4DIGIT  ; day month year (e.g., 02 Jun 1982)
     *  time = 2DIGIT ":" 2DIGIT ":" 2DIGIT  ; 00:00:00 - 23:59:59  
     *  month = "Jan" | "Feb" | "Mar" | "Apr" | "May" | "Jun" | 
     *          "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec" 
     *  wkday = "Mon" | "Tue" | "Wed" | "Thu" | "Fri" | "Sat" | "Sun"
     */
    ULONG                           ulSize      = 0;
    ULONG                           ulDate1     = 0;
    ULONG                           ulTime      = 0;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc        = NULL;
    UCHAR                           buf[8];

    ulSize  = 3;                    /* wkday */

    pSrc    = HttpSmpoUtilGetWeekDayName(pDate->DayOfWeek);

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         pSrc,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         ", ",
                         2
                     );

    AnscInt2String(pDate->DayOfMonth, buf, 10);
    ulSize  = AnscSizeOfString(buf);

    if (ulSize == 1)
    {
        /* add a ZERO before the day */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "0",
                             1
                         );
        if (!bCopySucc)
            return FALSE;
    }

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         " ",
                         1
                     );
    if (!bCopySucc)
        return FALSE;

    ulSize  = 3;                        /* month */

    pSrc    = HttpSmpoUtilGetMonthName(pDate->Month);

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         pSrc,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         " ",
                         1
                     );
    if (!bCopySucc)
        return FALSE;

    AnscInt2String(pDate->Year, buf, 10);
    ulSize  = AnscSizeOfString(buf);

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         " ",
                         1
                     );
    if (!bCopySucc)
        return FALSE;

    AnscInt2String(pDate->Hour, buf, 10);
    ulSize  = AnscSizeOfString(buf);

    if (ulSize == 1)
    {
        /* add a ZERO before the hour */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "0",
                             1
                         );
        if (!bCopySucc)
            return FALSE;
    }

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );
    if (!bCopySucc)
        return FALSE;

    /* add a ZERO before the hour */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         ":",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    AnscInt2String(pDate->Minute, buf, 10);
    ulSize  = AnscSizeOfString(buf);

    if (ulSize == 1)
    {
        /* add a ZERO before the day */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "0",
                             1
                         );
        if (!bCopySucc)
            return FALSE;
    }

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    /* add a ZERO before the hour */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         ":",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    AnscInt2String(pDate->Second, buf, 10);
    ulSize  = AnscSizeOfString(buf);

    if (ulSize == 1)
    {
        /* add a ZERO before the day */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             "0",
                             1
                         );
        if (!bCopySucc)
            return FALSE;
    }

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    /* add a ZERO before the hour */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         " GMT",
                         4
                     );

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildBuildField
            (
                PUCHAR                      pBuf,
                ULONG                       ulDestSize,
                ULONG                       *pulBufStart,
                ULONG                       ulHeaderId
            )

    description:

        This function is called to build header field.

    argument:   PVOID                       buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                ULONG                       ulHeaderId
                The ID of HTTP header.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildField
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulHeaderId
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    switch (ulHeaderId)
    {
    case HTTP_HEADER_ID_ACCEPT:
        ulSize  = HTTP_SMPO_ACCEPT_LENGTH;
        pSrc    = HTTP_SMPO_ACCEPT;
        break;

    case HTTP_HEADER_ID_ACCEPT_CHARSET:
        ulSize  = HTTP_SMPO_ACCEPT_CHARSET_LENGTH;
        pSrc    = HTTP_SMPO_ACCEPT_CHARSET;
        break;

    case HTTP_HEADER_ID_ACCEPT_ENCODING:
        ulSize  = HTTP_SMPO_ACCEPT_ENCODING_LENGTH;
        pSrc    = HTTP_SMPO_ACCEPT_ENCODING;
        break;

    case HTTP_HEADER_ID_ACCEPT_LANGUAGE:
        ulSize  = HTTP_SMPO_ACCEPT_LANGUAGE_LENGTH;
        pSrc    = HTTP_SMPO_ACCEPT_LANGUAGE;
        break;

    case HTTP_HEADER_ID_ACCEPT_RANGES:
        ulSize  = HTTP_SMPO_ACCEPT_RANGES_LENGTH;
        pSrc    = HTTP_SMPO_ACCEPT_RANGES;
        break;

    case HTTP_HEADER_ID_AGE:
        ulSize  = HTTP_SMPO_AGE_LENGTH;
        pSrc    = HTTP_SMPO_AGE;
        break;

    case HTTP_HEADER_ID_ALLOW:
        ulSize  = HTTP_SMPO_AGE_LENGTH;
        pSrc    = HTTP_SMPO_AGE;
        break;

    case HTTP_HEADER_ID_AUTHORIZATION:
        ulSize  = HTTP_SMPO_AUTHORIZATION_LENGTH;
        pSrc    = HTTP_SMPO_AUTHORIZATION;
        break;

    case HTTP_HEADER_ID_CACHE_CONTROL:
        ulSize  = HTTP_SMPO_CACHE_CONTROL_LENGTH;
        pSrc    = HTTP_SMPO_CACHE_CONTROL;
        break;

    case HTTP_HEADER_ID_CONNECTION:
        ulSize  = HTTP_SMPO_CONNECTION_LENGTH;
        pSrc    = HTTP_SMPO_CONNECTION;
        break;

    case HTTP_HEADER_ID_CONTENT_ENCODING:
        ulSize  = HTTP_SMPO_CONTENT_ENCODING_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_ENCODING;
        break;

    case HTTP_HEADER_ID_CONTENT_LANGUAGE:
        ulSize  = HTTP_SMPO_CONTENT_LANGUAGE_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_LANGUAGE;
        break;

    case HTTP_HEADER_ID_CONTENT_LENGTH:
        ulSize  = HTTP_SMPO_CONTENT_LENGTH_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_LOCATION:
        ulSize  = HTTP_SMPO_CONTENT_LOCATION_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_LOCATION;
        break;

    case HTTP_HEADER_ID_CONTENT_MD5:
        ulSize  = HTTP_SMPO_CONTENT_MD5_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_MD5;
        break;

    case HTTP_HEADER_ID_CONTENT_RANGE:
        ulSize  = HTTP_SMPO_CONTENT_RANGE_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_RANGE;
        break;

    case HTTP_HEADER_ID_CONTENT_TYPE:
        ulSize  = HTTP_SMPO_CONTENT_TYPE_LENGTH;
        pSrc    = HTTP_SMPO_CONTENT_TYPE;
        break;

    case HTTP_HEADER_ID_DATE:
        ulSize  = HTTP_SMPO_DATE_HEADER_LENGTH;
        pSrc    = HTTP_SMPO_DATE_HEADER;
        break;

    case HTTP_HEADER_ID_ETAG:
        ulSize  = HTTP_SMPO_ETAG_LENGTH;
        pSrc    = HTTP_SMPO_ETAG;
        break;

    case HTTP_HEADER_ID_EXPECT:
        ulSize  = HTTP_SMPO_EXPECT_LENGTH;
        pSrc    = HTTP_SMPO_EXPECT;
        break;

    case HTTP_HEADER_ID_EXPIRES:
        ulSize  = HTTP_SMPO_EXPIRES_LENGTH;
        pSrc    = HTTP_SMPO_EXPIRES;
        break;

    case HTTP_HEADER_ID_FROM:
        ulSize  = HTTP_SMPO_EXPIRES_LENGTH;
        pSrc    = HTTP_SMPO_EXPIRES;
        break;

    case HTTP_HEADER_ID_HOST:
        ulSize  = HTTP_SMPO_HEADER_HOST_LENGTH;
        pSrc    = HTTP_SMPO_HEADER_HOST;
        break;

    case HTTP_HEADER_ID_IF_MATCH:
        ulSize  = HTTP_SMPO_IF_MATCH_LENGTH;
        pSrc    = HTTP_SMPO_IF_MATCH;
        break;

    case HTTP_HEADER_ID_IF_MODIFIED_SINCE:
        ulSize  = HTTP_SMPO_IF_MODIFIED_SINCE_LENGTH;
        pSrc    = HTTP_SMPO_IF_MODIFIED_SINCE;
        break;

    case HTTP_HEADER_ID_IF_NONE_MATCH:
        ulSize  = HTTP_SMPO_IF_NONE_MATCH_LENGTH;
        pSrc    = HTTP_SMPO_IF_NONE_MATCH;
        break;

    case HTTP_HEADER_ID_IF_RANGE:
        ulSize  = HTTP_SMPO_IF_RANGE_LENGTH;
        pSrc    = HTTP_SMPO_IF_RANGE;
        break;

    case HTTP_HEADER_ID_IF_UNMODIFIED_SINCE:
        ulSize  = HTTP_SMPO_IF_UNMODIFIED_SINCE_LENGTH;
        pSrc    = HTTP_SMPO_IF_UNMODIFIED_SINCE;
        break;

    case HTTP_HEADER_ID_LAST_MODIFIED:
        ulSize  = HTTP_SMPO_LAST_MODIFIED_LENGTH;
        pSrc    = HTTP_SMPO_LAST_MODIFIED;
        break;

    case HTTP_HEADER_ID_LOCATION:
        ulSize  = HTTP_SMPO_LOCATION_LENGTH;
        pSrc    = HTTP_SMPO_LOCATION;
        break;

    case HTTP_HEADER_ID_MAX_FORWARDS:
        ulSize  = HTTP_SMPO_MAX_FORWARDS_LENGTH;
        pSrc    = HTTP_SMPO_MAX_FORWARDS;
        break;

    case HTTP_HEADER_ID_PRAGMA:
        ulSize  = HTTP_SMPO_PRAGMA_LENGTH;
        pSrc    = HTTP_SMPO_PRAGMA;
        break;

    case HTTP_HEADER_ID_PROXY_AUTHENTICATE:
        ulSize  = HTTP_SMPO_PROXY_AUTHENTICATE_LENGTH;
        pSrc    = HTTP_SMPO_PROXY_AUTHENTICATE;
        break;

    case HTTP_HEADER_ID_PROXY_AUTHORIZATION:
        ulSize  = HTTP_SMPO_PROXY_AUTHORIZATION_LENGTH;
        pSrc    = HTTP_SMPO_PROXY_AUTHORIZATION;
        break;

    case HTTP_HEADER_ID_RANGE:
        ulSize  = HTTP_SMPO_RANGE_LENGTH;
        pSrc    = HTTP_SMPO_RANGE;
        break;

    case HTTP_HEADER_ID_REFERER:
        ulSize  = HTTP_SMPO_REFERER_LENGTH;
        pSrc    = HTTP_SMPO_REFERER;
        break;

    case HTTP_HEADER_ID_RETRY_AFTER:
        ulSize  = HTTP_SMPO_RETRY_AFTER_LENGTH;
        pSrc    = HTTP_SMPO_RETRY_AFTER;
        break;

    case HTTP_HEADER_ID_SERVER:
        ulSize  = HTTP_SMPO_SERVER_LENGTH;
        pSrc    = HTTP_SMPO_SERVER;
        break;

    case HTTP_HEADER_ID_TE:
        ulSize  = HTTP_SMPO_TE_LENGTH;
        pSrc    = HTTP_SMPO_TE;
        break;

    case HTTP_HEADER_ID_TRAILER:
        ulSize  = HTTP_SMPO_TRAILER_LENGTH;
        pSrc    = HTTP_SMPO_TRAILER;
        break;

    case HTTP_HEADER_ID_TRANSFER_ENCODING:
        ulSize  = HTTP_SMPO_TRANSFER_ENCODING_LENGTH;
        pSrc    = HTTP_SMPO_TRANSFER_ENCODING;
        break;

    case HTTP_HEADER_ID_UPGRADE:
        ulSize  = HTTP_SMPO_UPGRADE_LENGTH;
        pSrc    = HTTP_SMPO_UPGRADE;
        break;

    case HTTP_HEADER_ID_USER_AGENT:
        ulSize  = HTTP_SMPO_USER_AGENT_LENGTH;
        pSrc    = HTTP_SMPO_USER_AGENT;
        break;

    case HTTP_HEADER_ID_VARY:
        ulSize  = HTTP_SMPO_VARY_LENGTH;
        pSrc    = HTTP_SMPO_VARY;
        break;

    case HTTP_HEADER_ID_VIA:
        ulSize  = HTTP_SMPO_VIA_LENGTH;
        pSrc    = HTTP_SMPO_VIA;
        break;

    case HTTP_HEADER_ID_WARNING:
        ulSize  = HTTP_SMPO_WARNING_LENGTH;
        pSrc    = HTTP_SMPO_WARNING;
        break;

    case HTTP_HEADER_ID_WWW_AUTHENTICATE:
        ulSize  = HTTP_SMPO_WWW_AUTHENTICATE_LENGTH;
        pSrc    = HTTP_SMPO_WWW_AUTHENTICATE;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildContentCodingLength
            (
                PUCHAR                      buffer,
                ULONG                       ulBufSize,
                ULONG                       *pulBufStart,
                PUCHAR                      pHeaderName,
                ULONG                       HeaderNameLength
            )

    description:

        This function is called to prebuild header.

    argument:   PUCHAR                      buffer
                Destination buffer.

                ULONG                       ulBufSzie
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

                PUCHAR                      pHeaderName
                HTTP header name.

                ULONG                       HeaderNameLength
                The length of header name.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilPrebuildHeader
    (
        PUCHAR                      buffer,
        ULONG                       ulBufSize,     
        ULONG                       *pulStart,        /* where copy begins, 0-based */
        PUCHAR                      pHeaderName,
        ULONG                       HeaderNameLength
    )
{
    BOOL                            bCopySucc;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         pHeaderName,
                         HeaderNameLength
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_HEADER_SEPARATOR,
                         HTTP_SMPO_HEADER_SEPARATOR_LENGTH
                     );
    if (!bCopySucc)
        return FALSE;

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildBasicCredentials
            (
                PHTTP_CREDENTIAL_BASIC      pBasic
                PUCHAR                      buffer,
                ULONG                       ulBufSize,
                ULONG                       *pulBufStart
            )

    description:

        This function is called to build content coding length.

    argument:   PHTTP_CREDENTIAL_BASIC      pBasic
                Basic credential object.
                
                PUCHAR                      buffer
                Destination buffer.

                ULONG                       ulDestSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildBasicCredentials
    (
        PHTTP_CREDENTIAL_BASIC      pBasic,
        PUCHAR                      buffer,
        ULONG                       ulBufSize,     
        ULONG                       *pulStart       /* where copy begins, 0-based */
    )
{
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pBuf;
    ULONG                           ulBufLen, ulUser, ulPass;
    PUCHAR                          pEncBuf;

    ulUser      =  AnscSizeOfString(pBasic->UserName);
    ulPass      =  AnscSizeOfString(pBasic->Password);
    ulBufLen    =  ulUser;

    if (ulPass != 0)
    {
        ulBufLen    += ulPass + 1;                  /* ':' */
    }

    ulBufLen    ++;                                 /* '\0' */

    pBuf        = (PUCHAR)AnscAllocateMemory(ulBufLen);

    if (pBuf)
    {
        if (ulUser != 0)
        {
            AnscCopyMemory(pBuf, pBasic->UserName, ulUser);
        }

        if (ulPass != 0)
        {
            pBuf[ulUser]    = ':';
            AnscCopyMemory(pBuf + ulUser + 1, pBasic->Password, ulPass);
        }

        pBuf[ulBufLen-1]      = 0;

        /* Encode the USER:PASS */
        pEncBuf = AnscBase64Encode(pBuf, ulBufLen - 1);   
        if ( pEncBuf )
        {
            ULONG                   ulLen   = AnscSizeOfString(pEncBuf);

            if ( ulLen >= 2 )
            {
                if ( pEncBuf[ulLen - 1] == '\r' || pEncBuf[ulLen - 1] == '\n' )
                {
                    pEncBuf[ulLen - 1]  = 0;
                }

                if ( pEncBuf[ulLen - 2] == '\r' || pEncBuf[ulLen - 2] == '\n' )
                {
                    pEncBuf[ulLen - 2]  = 0;
                }
            }
        }
        AnscFreeMemory(pBuf);

        /* copy "Basic" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             HTTP_SMPO_CREDENTIAL_BASIC_SCHEME,
                             HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        /* SPACE */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             HTTP_SMPO_SPACE,
                             HTTP_SMPO_SPACE_LENGTH
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        /* basic-cookie */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             pEncBuf,
                             AnscSizeOfString(pEncBuf)
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        AnscFreeMemory(pEncBuf);
    }

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilBuildBasicChallenge
            (
                PHTTP_CHALLENGE_BASIC       pBasic,
                PUCHAR                      buffer,
                ULONG                       ulBufSize,
                ULONG                       *pulBufStart,
            )

    description:

        This function is called to build content coding length.

    argument:   PHTTP_CHALLENGE_BASIC       pBasic
                Basic challenge object.

                PUCHAR                      buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

                ULONG                       *pulBufStart
                Where to start copy in destination buffer.

    return:     status of operation.

**********************************************************************/

static
BOOL
HttpSmpoUtilBuildBasicChallenge
    (
        PHTTP_CHALLENGE_BASIC       pBasic,
        PUCHAR                      buffer,
        ULONG                       ulBufSize,     
        ULONG                       *pulStart       /* where copy begins, 0-based */
    )
{
    BOOL                            bCopySucc   = TRUE;

    /* copy "Basic" */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_CREDENTIAL_BASIC_SCHEME,
                         HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* SPACE */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_SPACE,
                         HTTP_SMPO_SPACE_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* copy "realm" */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_REALM,
                         HTTP_SMPO_REALM_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* '=' */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         "=",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    /* copy realm value */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         pBasic->Realm,
                         AnscSizeOfString(pBasic->Realm)
                     );

    if (!bCopySucc)
        return FALSE;

    return bCopySucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildRequestLine
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            )

    description:

        This function is called to build request line.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildRequestLine
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     *  Request-Line = Method SP Request-URI SP HTTP-Version
     */
    PHTTP_REQUEST_INFO              pRequestInfo    = (PHTTP_REQUEST_INFO)hHttpHfo;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           ulStart         = 0;
    BOOL                            bCopySucc;

    if (!hHttpHfo)
        return ANSC_STATUS_FAILURE;

    /* method */
    bCopySucc   = HttpSmpoUtilBuildMethod(buffer, ulSize, &ulStart, pRequestInfo->Method, pRequestInfo->MethodName);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* space */
    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* request uri */
    bCopySucc   = HttpSmpoUtilBuildRequestUri(buffer, ulSize, &ulStart, &pRequestInfo->RequestUri);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* space */
    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* version */
    bCopySucc   = HttpSmpoUtilBuildHttpVersion(buffer, ulSize, &ulStart, pRequestInfo->MajorVersion, pRequestInfo->MinorVersion);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildStatusLine
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            )

    description:

        This function is called to build status line.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildStatusLine
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     *  Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase
     */
    PHTTP_RESPONSE_INFO             pResponseInfo   = (PHTTP_RESPONSE_INFO)hHttpHfo;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    
    if (!hHttpHfo)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpVersion
                     (
                         buffer, 
                         ulSize, 
                         &ulStart, 
                         pResponseInfo->MajorVersion, 
                         pResponseInfo->MinorVersion
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong(buffer, ulSize, &ulStart, pResponseInfo->StatusCode);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer, 
                         ulSize, 
                         &ulStart, 
                         pResponseInfo->ReasonPhrase, 
                         AnscSizeOfString(pResponseInfo->ReasonPhrase)
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildHeader
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            )

    description:

        This function is called to build a header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildHeader
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    PHTTP_HEADER_FIELD              pHeader;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    BOOL                            bCopySucc   = TRUE;
    ULONG                           ulStart     = 0;

    pHeader = (PHTTP_HEADER_FIELD)hHttpHfo;

    if (!pHeader)
        return 0;

    if (pHeader->Flags & HTTP_FIELD_FLAG_LINE_PRESENT)
    {
        if (buffer && buffer != pHeader->HeaderLine)
        {
            /* this header is still valid, so return the size of HeaderLine */
            ULONG                   ulHeaderLineSize;
            
            ulHeaderLineSize    = AnscSizeOfString(pHeader->HeaderLine);
            bCopySucc           = HttpSmpoUtilCopyString
                                     (
                                         buffer, 
                                         ulSize, 
                                         &ulStart, 
                                         pHeader->HeaderLine, 
                                         ulHeaderLineSize
                                     );

            if (!bCopySucc)
                return ANSC_STATUS_SUCCESS;
        }        
    }
    else
    {
        PFN_HTTP_SMPO_BUILD_HEADER  pfBuildHeader = NULL;
        BOOL                        bConstructHeaderLine    = FALSE;

        switch (pHeader->HeaderId)
        {
        case HTTP_HEADER_ID_ACCEPT:
            pfBuildHeader = HttpSmpoUtilBuildAccept;
            break;

        case HTTP_HEADER_ID_ACCEPT_CHARSET:
            pfBuildHeader = HttpSmpoUtilBuildAcceptCharset;
            break;

        case HTTP_HEADER_ID_ACCEPT_ENCODING:
            pfBuildHeader = HttpSmpoUtilBuildAcceptEncoding;
            break;

        case HTTP_HEADER_ID_ACCEPT_LANGUAGE:
            pfBuildHeader = HttpSmpoUtilBuildAcceptLanguage;
            break;

        case HTTP_HEADER_ID_ACCEPT_RANGES:
            pfBuildHeader = HttpSmpoUtilBuildAcceptRanges;
            break;

        case HTTP_HEADER_ID_AGE:
            pfBuildHeader = HttpSmpoUtilBuildAge;
            break;

        case HTTP_HEADER_ID_ALLOW:
            pfBuildHeader = HttpSmpoUtilBuildAllow;
            break;

        case HTTP_HEADER_ID_AUTHORIZATION:
            pfBuildHeader = HttpSmpoUtilBuildAuthorization;
            break;

        case HTTP_HEADER_ID_CACHE_CONTROL:
            pfBuildHeader = HttpSmpoUtilBuildCacheControl;
            break;

        case HTTP_HEADER_ID_CONNECTION:
            pfBuildHeader = HttpSmpoUtilBuildConnection;
            break;

        case HTTP_HEADER_ID_CONTENT_ENCODING:
            pfBuildHeader = HttpSmpoUtilBuildContentEncoding;
            break;

        case HTTP_HEADER_ID_CONTENT_LANGUAGE:
            pfBuildHeader = HttpSmpoUtilBuildContentLanguage;
            break;

        case HTTP_HEADER_ID_CONTENT_LENGTH:
            pfBuildHeader = HttpSmpoUtilBuildContentLength;
            break;

        case HTTP_HEADER_ID_CONTENT_LOCATION:
            pfBuildHeader = HttpSmpoUtilBuildContentLocation;
            break;

        case HTTP_HEADER_ID_CONTENT_MD5:
            pfBuildHeader = HttpSmpoUtilBuildContentMD5;
            break;

        case HTTP_HEADER_ID_CONTENT_RANGE:
            pfBuildHeader = HttpSmpoUtilBuildContentRange;
            break;

        case HTTP_HEADER_ID_CONTENT_TYPE:
            pfBuildHeader = HttpSmpoUtilBuildContentType;
            break;

        case HTTP_HEADER_ID_DATE:
            pfBuildHeader = HttpSmpoUtilBuildDate;
            break;

        case HTTP_HEADER_ID_ETAG:
            pfBuildHeader = HttpSmpoUtilBuildETag;
            break;

        case HTTP_HEADER_ID_EXPECT:
            pfBuildHeader = HttpSmpoUtilBuildExpect;
            break;

        case HTTP_HEADER_ID_EXPIRES:
            pfBuildHeader = HttpSmpoUtilBuildExpires;
            break;

        case HTTP_HEADER_ID_FROM:
            pfBuildHeader = HttpSmpoUtilBuildFrom;
            break;

        case HTTP_HEADER_ID_HOST:
            pfBuildHeader = HttpSmpoUtilBuildHost;
            break;

        case HTTP_HEADER_ID_IF_MATCH:
            pfBuildHeader = HttpSmpoUtilBuildIfMatch;
            break;

        case HTTP_HEADER_ID_IF_MODIFIED_SINCE:
            pfBuildHeader = HttpSmpoUtilBuildIfModifiedSince;
            break;

        case HTTP_HEADER_ID_IF_NONE_MATCH:
            pfBuildHeader = HttpSmpoUtilBuildIfNoneMatch;
            break;

        case HTTP_HEADER_ID_IF_RANGE:
            pfBuildHeader = HttpSmpoUtilBuildIfRange;
            break;

        case HTTP_HEADER_ID_IF_UNMODIFIED_SINCE:
            pfBuildHeader = HttpSmpoUtilBuildIfUnmodifiedSince;
            break;

        case HTTP_HEADER_ID_LAST_MODIFIED:
            pfBuildHeader = HttpSmpoUtilBuildLastModified;
            break;

        case HTTP_HEADER_ID_LOCATION:
            pfBuildHeader = HttpSmpoUtilBuildLocation;
            break;

        case HTTP_HEADER_ID_MAX_FORWARDS:
            pfBuildHeader = HttpSmpoUtilBuildMaxForwards;
            break;

        case HTTP_HEADER_ID_PRAGMA:
            pfBuildHeader = HttpSmpoUtilBuildPragma;
            break;

        case HTTP_HEADER_ID_PROXY_AUTHENTICATE:
            pfBuildHeader = HttpSmpoUtilBuildProxyAuthenticate;
            break;

        case HTTP_HEADER_ID_PROXY_AUTHORIZATION:
            pfBuildHeader = HttpSmpoUtilBuildProxyAuthorization;
            break;

        case HTTP_HEADER_ID_RANGE:
            pfBuildHeader = HttpSmpoUtilBuildRange;
            break;

        case HTTP_HEADER_ID_REFERER:
            pfBuildHeader = HttpSmpoUtilBuildReferer;
            break;

        case HTTP_HEADER_ID_RETRY_AFTER:
            pfBuildHeader = HttpSmpoUtilBuildTryAfter;
            break;

        case HTTP_HEADER_ID_SERVER:
            pfBuildHeader = HttpSmpoUtilBuildServer;
            break;

        case HTTP_HEADER_ID_TE:
            pfBuildHeader = HttpSmpoUtilBuildTE;
            break;

        case HTTP_HEADER_ID_TRAILER:
            pfBuildHeader = HttpSmpoUtilBuildTrailer;
            break;

        case HTTP_HEADER_ID_TRANSFER_ENCODING:
            pfBuildHeader = HttpSmpoUtilBuildTransferEncoding;
            break;

        case HTTP_HEADER_ID_UPGRADE:
            pfBuildHeader = HttpSmpoUtilBuildUpgrade;
            break;

        case HTTP_HEADER_ID_USER_AGENT:
            pfBuildHeader = HttpSmpoUtilBuildUserAgent;
            break;

        case HTTP_HEADER_ID_VARY:
            pfBuildHeader = HttpSmpoUtilBuildVary;
            break;

        case HTTP_HEADER_ID_VIA:
            pfBuildHeader = HttpSmpoUtilBuildVia;
            break;

        case HTTP_HEADER_ID_WARNING:
            pfBuildHeader = HttpSmpoUtilBuildWarning;
            break;

        case HTTP_HEADER_ID_WWW_AUTHENTICATE:
            pfBuildHeader = HttpSmpoUtilBuildWWWAuthenticate;
            break;

        case HTTP_HEADER_ID_PROXY_CONNECTION:
            pfBuildHeader = HttpSmpoUtilBuildProxyConnection;
            break;

        case HTTP_HEADER_ID_COOKIE:
            pfBuildHeader = HttpSmpoUtilBuildCookie;
            break;

        case HTTP_HEADER_ID_SET_COOKIE:
            pfBuildHeader = HttpSmpoUtilBuildSetCookie;
            break;

        case HTTP_HEADER_ID_COOKIE2:
            pfBuildHeader = HttpSmpoUtilBuildCookie2;
            break;

        case HTTP_HEADER_ID_SET_COOKIE2:
            pfBuildHeader = HttpSmpoUtilBuildSetCookie2;
            break;
        }

        if (!pfBuildHeader)
            return ANSC_STATUS_FAILURE;

        if (!buffer)
        {
            buffer  = pHeader->HeaderLine;
            ulSize  = HTTP_HEADER_LINE_SIZE;
            /* construct header line */
            bConstructHeaderLine    = TRUE;
            /* 
             * zero out header line, so we don't need to care
             * about where it ends
             */
            AnscZeroMemory(buffer, ulSize);
        }

        /* call the corresponding function to build header */
        status  = (*pfBuildHeader)(hHttpHfo, buffer, ulSize);

        if (status == ANSC_STATUS_SUCCESS && ulSize != 0)
        {
            if (bConstructHeaderLine)
            {
                /*
                pHeader->HeaderLine[ulSize - 1] = 0;
                */
                pHeader->Flags  = HTTP_FIELD_FLAG_LINE_PRESENT;
            }
            else
            {
                ULONG               ulCopySize;
                BOOL                bTrunked    = FALSE;
                PUCHAR              pCurHeaderLine  = pHeader->HeaderLine;
                ULONG               ulCurHeaderLine;
                PUCHAR              pNewHeaderLine  = (PUCHAR)buffer;

                if (pCurHeaderLine)
                {
                    ulCurHeaderLine = AnscSizeOfString(pCurHeaderLine);
                    if (ulCurHeaderLine != 0)
                    {
                        if (ulCurHeaderLine != ulSize)
                        {
                            int     a = 0;
                        }
                    }
                }

                ulCopySize  = ulSize;
                if (ulCopySize >= HTTP_HEADER_LINE_SIZE)
                {
                    ulCopySize  = HTTP_HEADER_LINE_SIZE - 1;
                    bTrunked    = TRUE;
                }

                /* 
                 * if the message is complete, we also put them
                 * into header line so that we don't need to
                 * re-construct this header later on.
                 */
                if (!bTrunked)
                {
                    AnscCopyMemory(pHeader->HeaderLine, buffer, ulCopySize);
                    pHeader->HeaderLine[ulCopySize] = 0;
                    pHeader->Flags  = HTTP_FIELD_FLAG_LINE_PRESENT;
                }
            }
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAccept
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAccept
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *   Accept = "Accept" ":" #( media-range [accept-params] )
     *   media-range = ( "* / *" | ( type "/" "*" ) | ( type "/" subtype ) ) 
     *                *( ";" parameter ) 
     *   accept-params = ";" "q" "=" qvalue *( accept-extension ) 
     */
    PHTTP_HFO_ACCEPT                pAccept = (PHTTP_HFO_ACCEPT)hHttpHfo;
    ULONG                           ulRangeCount, i;
    PHTTP_MEDIA_RANGE               pRange;
    ULONG                           ulParams;
    ANSC_STATUS                     status  = ANSC_STATUS_SUCCESS;
    BOOL                            bCopySucc;
    ULONG                           ulStart = 0;


    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer, 
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ACCEPT,
                         HTTP_SMPO_ACCEPT_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulRangeCount = pAccept->RangeCount;
    for (i = 0; i < ulRangeCount; i ++)
    {
        pRange          = &pAccept->RangeArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }


        bCopySucc   = HttpSmpoUtilBuildMediaType
                         (
                             buffer, 
                             ulBufSize,
                             &ulStart,
                             pRange->MediaType
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             "/",
                             1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc       = HttpSmpoUtilBuildMediaSubType
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pRange->MediaType,
                                 pRange->SubType
                             );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        ulParams = AnscSizeOfString(pRange->Parameters);
        if (ulParams != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pRange->Parameters,
                                 ulParams
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer, 
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_QUALITY_PARAMS,
                             HTTP_SMPO_QUALITY_PARAMS_LENGTH
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildQuality
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pRange->Quality
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAcceptCharset
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAcceptCharset
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Accept-Charset = "Accept-Charset" ":" 1#( charset [";" "q" "=" qvalue] ) 
     */
    ULONG                           ulSize          = 0;
    PHTTP_HFO_ACCEPT_CHARSET        pAcceptCharset  = (PHTTP_HFO_ACCEPT_CHARSET)hHttpHfo;
    ULONG                           ulCharsetCount, i;
    PHTTP_CHARSET                   pCharset;
    BOOL                            bCopySucc;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           ulStart         = 0;
    
    /* "Accept-Charset" */
    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer, 
                         ulBufSize,
                         &ulStart, 
                         HTTP_SMPO_ACCEPT_CHARSET,
                         HTTP_SMPO_ACCEPT_CHARSET_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCharsetCount  = pAcceptCharset->CharsetCount;
    for (i = 0; i < ulCharsetCount; i ++)
    {
        pCharset    = &pAcceptCharset->CharsetArray[i];

        if (i != 0)         /* not the first charset */
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildCharset
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pCharset->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_QUALITY_PARAMS,
                             HTTP_SMPO_QUALITY_PARAMS_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildQuality
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pCharset->Quality
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAcceptEncoding
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAcceptEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Accept-Encoding = "Accept-Encoding" ":" 1#( codings [ ";" "q" "=" qvalue ] )
     */
    PHTTP_HFO_ACCEPT_ENCODING       pAcceptEncoding;
    ULONG                           ulSize = 0;
    ULONG                           ulEncodingCount, i;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulStart = 0;
    BOOL                            bCopySucc;
    ANSC_STATUS                     status  = ANSC_STATUS_SUCCESS;

    pAcceptEncoding = (PHTTP_HFO_ACCEPT_ENCODING)hHttpHfo;

    /* "Accept-Encoding" */
    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ACCEPT_ENCODING,
                         HTTP_SMPO_ACCEPT_ENCODING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulEncodingCount = pAcceptEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding       = &pAcceptEncoding->EncodingArray[i];

        if (i != 0)
        {
            /* field separator ", " */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildContentCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_QUALITY_PARAMS,
                             HTTP_SMPO_QUALITY_PARAMS_LENGTH
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildQuality
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Quality
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAcceptLanguage
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAcceptLanguage
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Accept-Language = "Accept-Language" ":" 1#( language-range [";" "q" "=" qvalue] ) 
     */
    PHTTP_HFO_ACCEPT_LANGUAGE       pAcceptLanguage = (PHTTP_HFO_ACCEPT_LANGUAGE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulLanguageCount, i;
    PHTTP_LANGUAGE                  pLanguage;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ACCEPT_LANGUAGE,
                         HTTP_SMPO_ACCEPT_LANGUAGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulLanguageCount = pAcceptLanguage->LanguageCount;
    for (i = 0; i < ulLanguageCount; i ++)
    {
        pLanguage   = &pAcceptLanguage->LanguageArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pLanguage->Tag);    /* language range */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pLanguage->Tag,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_QUALITY_PARAMS,
                             HTTP_SMPO_QUALITY_PARAMS_LENGTH
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildQuality
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pLanguage->Quality
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAcceptRanges
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAcceptRanges
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Accept-Ranges = "Accept-Ranges" ":" acceptable-ranges 
     *  acceptable-ranges = 1#range-unit | "none"
     *  range-unit = bytes-unit | other-range-unit
     *  bytes-unit = "bytes"
     *  other-range-unit = token
     */
    PHTTP_HFO_ACCEPT_RANGES         pAcceptRanges   = (PHTTP_HFO_ACCEPT_RANGES)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulRangeCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    /* "Accept-Ranges" */
    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ACCEPT_RANGES,
                         HTTP_SMPO_ACCEPT_RANGES_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulRangeCount    = pAcceptRanges->RangeCount;
    for (i = 0; i < ulRangeCount; i ++)
    {
        if (i != 0)
        {
            /* ", " */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }


        ulSize      = AnscSizeOfString(pAcceptRanges->RangeArray[i].Unit);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pAcceptRanges->RangeArray[i].Unit,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAge
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAge
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Age = "Age" ":" age-value 
     */
    PHTTP_HFO_AGE                   pAge    = (PHTTP_HFO_AGE)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_AGE,
                         HTTP_SMPO_AGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pAge->DeltaSeconds
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAllow
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAllow
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     * Allow = "Allow" ":" 1#Method 
     */
    PHTTP_HFO_ALLOW                 pAllow  = (PHTTP_HFO_ALLOW)hHttpHfo;
    ULONG                           ulSize  = 0;
    ULONG                           ulMethodCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ALLOW,
                         HTTP_SMPO_ALLOW_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;
    
    ulMethodCount   = pAllow->MethodCount;
    for (i = 0; i < ulMethodCount; i ++)
    {
        if (i != 0)
        {
            ulSize      = HTTP_SMPO_FIELD_SEPARATOR_LENGTH;
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildMethod
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pAllow->MethodArray[i],
                             NULL
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildAuthorization
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildAuthorization
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Authorization = "Authorization" ":" credentials
     */
    PHTTP_HFO_AUTHORIZATION         pAuthorization  = (PHTTP_HFO_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_AUTHORIZATION,
                         HTTP_SMPO_AUTHORIZATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    pCredential = &pAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        {
            PHTTP_CREDENTIAL_BASIC  pBasic;

            pBasic      = &pCredential->Credential.Basic;
            bCopySucc   = HttpSmpoUtilBuildBasicCredentials
                             (
                                 pBasic,
                                 buffer,
                                 ulBufSize,
                                 &ulStart
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        {
            PHTTP_CREDENTIAL_DIGEST pDigest;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME,
                                 HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SPACE,
                                 HTTP_SMPO_SPACE_LENGTH
                             );

            if (!bCopySucc)
            {
                return FALSE;
            }

            pDigest     = &pCredential->Credential.Digest;

            ulSize      = AnscSizeOfString(pDigest->UserName);

            if ( ulSize != 0 )
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     pDigest->UserName,
                                     ulSize
                                 );
            }

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
        break;

    case HTTP_AUTH_TYPE_NONE:
        /* do nothing */
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        /* do nothing */
        break;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildCacheControl
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildCacheControl
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Cache-Control = "Cache-Control" ":" 1#cache-directive
     */
    PHTTP_HFO_CACHE_CONTROL         pCacheControl   = (PHTTP_HFO_CACHE_CONTROL)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CACHE_CONTROL,
                         HTTP_SMPO_CACHE_CONTROL_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pCacheControl->Directives); /* directives */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pCacheControl->Directives,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildConnection
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Connection = Connection-header  ; added to remove orphan Connection-header
     *  Connection-header = "Connection" ":" 1#( connection-token )
     *  connection-token = token 
     */
    PHTTP_HFO_CONNECTION            pConnection = (PHTTP_HFO_CONNECTION)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulTokenCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONNECTION,
                         HTTP_SMPO_CONNECTION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTokenCount    = pConnection->TokenCount;
    for (i = 0; i <ulTokenCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pConnection->TokenArray[i].Token);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pConnection->TokenArray[i].Token,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentEncoding
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Encoding = "Content-Encoding" ":" 1#content-coding 
     *  content-coding = token
     */
    PHTTP_HFO_CONTENT_ENCODING      pContentEncoding    = (PHTTP_HFO_CONTENT_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_ENCODING,
                         HTTP_SMPO_CONTENT_ENCODING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;
    
    ulEncodingCount = pContentEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pContentEncoding->EncodingArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildContentCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentLanguage
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentLanguage
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Language = "Content-Language" ":" 1#language-tag
     *  language-tag = primary-tag *( "-" subtag )
     */
    PHTTP_HFO_CONTENT_LANGUAGE      pContentLanguage    = (PHTTP_HFO_CONTENT_LANGUAGE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulLanguageCount, i;
    PHTTP_LANGUAGE                  pLanguage;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_LANGUAGE,
                         HTTP_SMPO_CONTENT_LANGUAGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulLanguageCount = pContentLanguage->LanguageCount;
    for (i = 0; i < ulLanguageCount; i ++)
    {
        pLanguage   = &pContentLanguage->LanguageArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pLanguage->Tag);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pLanguage->Tag,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentLength
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentLength
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Length = "Content-Length" ":" 1*DIGIT 
     */
    PHTTP_HFO_CONTENT_LENGTH        pContentLength  = (PHTTP_HFO_CONTENT_LENGTH)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_LENGTH,
                         HTTP_SMPO_CONTENT_LENGTH_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentLength->BodySize
                         
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentLocation
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentLocation
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Location = "Content-Location" ":" ( absoluteURI | relativeURI )
     */
    PHTTP_HFO_CONTENT_LOCATION      pContentLocation    = (PHTTP_HFO_CONTENT_LOCATION)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_LOCATION,
                         HTTP_SMPO_CONTENT_LOCATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildRequestUri
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pContentLocation->LocationUri
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentMD5
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentMD5
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-MD5 = "Content-MD5" ":" md5-digest
     *  md5-digest = 16 (OCTET)
     */
    PHTTP_HFO_CONTENT_MD5           pContentMD5 = (PHTTP_HFO_CONTENT_MD5)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_MD5,
                         HTTP_SMPO_CONTENT_MD5_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentMD5->Digest,
                         ANSC_MD5_OUTPUT_SIZE
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentRange
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Range = "Content-Range" ":" content-range-spec
     *  content-range-spec = byte-content-range-spec
     *  byte-content-range-spec = bytes-unit SP first-byte-pos "-" 
     *                            last-byte-pos "/" entity-length
     */
    PHTTP_HFO_CONTENT_RANGE         pContentRange   = (PHTTP_HFO_CONTENT_RANGE)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_RANGE,
                         HTTP_SMPO_CONTENT_RANGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_BYTES_UNIT,
                         HTTP_SMPO_BYTES_UNIT_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         " ",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentRange->FirstBytePos
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "-",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentRange->LastBytePos
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "/",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentRange->InstaceLength
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildContentType
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildContentType
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Type = "Content-Type" ":" media-type
     *  media-type = type "/" subtype *( ";" parameter )
     */
    PHTTP_HFO_CONTENT_TYPE          pContentType    = (PHTTP_HFO_CONTENT_TYPE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulParams;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_TYPE,
                         HTTP_SMPO_CONTENT_TYPE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildMediaType
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentType->MediaType
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "/",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildMediaSubType
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentType->MediaType,
                         pContentType->SubType
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulParams = AnscSizeOfString(pContentType->Parameters);
    if (ulParams != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             ";",
                            1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pContentType->Parameters,
                             ulParams
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildDate
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildDate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Date = "Date" ":" HTTP-date
     *  rfc1123-date = wkday "," SP date1 SP time SP "GMT"
     */
    PHTTP_HFO_DATE                  pDate   = (PHTTP_HFO_DATE)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_DATE_HEADER,
                         HTTP_SMPO_DATE_HEADER_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpDate
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pDate->Date                         
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildETag
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildETag
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  ETag = "ETag" ":" entity-tag
     *  entity-tag = [weak] opaque-tag
     *  weak = "W/"
     *  opaque-tag = quoted-string
     */
    PHTTP_HFO_ETAG                  pETag   = (PHTTP_HFO_ETAG)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_ETAG,
                         HTTP_SMPO_ETAG_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pETag->bWeak)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_WEAK_VALUE,
                             HTTP_SMPO_WEAK_VALUE_LENGTH
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    ulSize      = AnscSizeOfString(pETag->Tag);        /* opaque-tag */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pETag->Tag,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildExpect
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildExpect
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Expect = "Expect" ":" 1#expectation
     *  expectation = "100-continue" | expectation-extension
     *  expectation-extension = token [ "=" ( token | quoted-string ) *expect-params ]
     *  expect-params = ";" token [ "=" ( token | quoted-string ) ]
     */
    PHTTP_HFO_EXPECT                pExpect = (PHTTP_HFO_EXPECT)hHttpHfo;
    ULONG                           ulSize  = 0;
    PHTTP_EXPECT_TOKEN              pExpectToken;
    ULONG                           ulTokenCount, i, ulParams;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_EXPECT,
                         HTTP_SMPO_EXPECT_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTokenCount    = pExpect->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pExpectToken    = &pExpect->TokenArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pExpectToken->Token);   /* token */

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pExpectToken->Token,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        ulParams        = AnscSizeOfString(pExpectToken->Parameters);
        if (ulParams != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pExpectToken->Parameters,
                                 ulParams
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildExpires
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildExpires
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Expires = "Expires" ":" HTTP-date 
     */
    PHTTP_HFO_EXPIRES               pExpires    = (PHTTP_HFO_EXPIRES)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_EXPIRES,
                         HTTP_SMPO_EXPIRES_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpDate
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pExpires->Date
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildFrom
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildFrom
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /* 
     *  From = "From" ":" mailbox 
     *  mailbox =  addr-spec            ; simple address
     *              | [phrase] route-addr   ; name & addr-spec
     *  addr-spec   =  local-part "@" domain        ; global address
     */
    PHTTP_HFO_FROM                  pFrom   = (PHTTP_HFO_FROM)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_FROM,
                         HTTP_SMPO_FROM_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pFrom->MailBox.LocalPart);  /* local part */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pFrom->MailBox.LocalPart,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "@",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pFrom->MailBox.DomainName); /* domain name */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pFrom->MailBox.DomainName,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildHost
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildHost
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Host = "Host" ":" host [":" port]  ; Section 3.2.2  
     */
    PHTTP_HFO_HOST                  pHost   = (PHTTP_HFO_HOST)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_HEADER_HOST,
                         HTTP_SMPO_HEADER_HOST_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pHost->HostName);       /* host name */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pHost->HostName,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pHost->HostPort != 80)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             ":",
                             1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pHost->HostPort
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildIfMatch
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildIfMatch
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  If-Match = "If-Match" ":" ( "*" | 1#entity-tag )
     *  entity-tag = [weak] opaque-tag
     *  opaque-tag = quoted-string
     *  weak = "W/"
     */
    PHTTP_HFO_IF_MATCH              pIfMatch    = (PHTTP_HFO_IF_MATCH)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulTagCount, i;
    PHTTP_ENTITY_TAG                pTag;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_IF_MATCH,
                         HTTP_SMPO_IF_MATCH_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTagCount  = pIfMatch->TagCount;
    for (i = 0; i < ulTagCount; i ++)
    {
        pTag    = &pIfMatch->TagArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        if (pTag->bWeak)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_WEAK_VALUE,
                                 HTTP_SMPO_WEAK_VALUE_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pTag->Tag);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pTag->Tag,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildIfModifiedSince
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildIfModifiedSince
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  If-Modified-Since = "If-Modified-Since" ":" HTTP-date 
     */
    PHTTP_HFO_IF_MODIFIED_SINCE     pIfModifiedSince    = (PHTTP_HFO_IF_MODIFIED_SINCE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_IF_MODIFIED_SINCE,
                         HTTP_SMPO_IF_MODIFIED_SINCE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpDate
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pIfModifiedSince->Date
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildIfNoneMatch
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildIfNoneMatch
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  If-None-Match = "If-None-Match" ":" ( "*" | 1#entity-tag )
     *  entity-tag = [weak] opaque-tag
     *  opaque-tag = quoted-string
     *  weak = "W/"
     */
    PHTTP_HFO_IF_NONE_MATCH         pIfNoneMatch    = (PHTTP_HFO_IF_NONE_MATCH)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulTagCount, i;
    PHTTP_ENTITY_TAG                pTag;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_IF_NONE_MATCH,
                         HTTP_SMPO_IF_NONE_MATCH_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTagCount  = pIfNoneMatch->TagCount;
    for (i = 0; i < ulTagCount; i ++)
    {
        pTag    = &pIfNoneMatch->TagArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        if (pTag->bWeak)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_WEAK_VALUE,
                                 HTTP_SMPO_WEAK_VALUE_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pTag->Tag);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pTag->Tag,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildIfRange
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildIfRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  If-Range = "If-Range" ":" ( entity-tag | HTTP-date )
     *  entity-tag = [weak] opaque-tag
     *  opaque-tag = quoted-string
     *  weak = "W/"
     */
    PHTTP_HFO_IF_RANGE              pIfRange    = (PHTTP_HFO_IF_RANGE)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_IF_RANGE,
                         HTTP_SMPO_IF_RANGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pIfRange->bTagUsed)
    {
        if (pIfRange->ETag.bWeak)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_WEAK_VALUE,
                                 HTTP_SMPO_WEAK_VALUE_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pIfRange->ETag.Tag);    /* opaque-tag */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pIfRange->ETag.Tag,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

    }
    else
    {
        bCopySucc   = HttpSmpoUtilBuildHttpDate
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             &pIfRange->Date
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildIfUnmodifiedSince
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  If-Unmodified-Since = "If-Unmodified-Since" ":" HTTP-date 
     */
    PHTTP_HFO_IF_UNMODIFIED_SINCE   pIfUnmodifiedSince  = (PHTTP_HFO_IF_UNMODIFIED_SINCE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_IF_UNMODIFIED_SINCE,
                         HTTP_SMPO_IF_UNMODIFIED_SINCE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpDate
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pIfUnmodifiedSince->Date
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildLastModified
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildLastModified
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Last-Modified = "Last-Modified" ":" HTTP-date
     */
    PHTTP_HFO_LAST_MODIFIED         pLastModified    = (PHTTP_HFO_LAST_MODIFIED)hHttpHfo;
    ULONG                           ulSize           = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_LAST_MODIFIED,
                         HTTP_SMPO_LAST_MODIFIED_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpDate
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pLastModified->Date
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildLocation
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildLocation
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Location = "Location" ":" absoluteURI
     */
    PHTTP_HFO_LOCATION              pLocation   = (PHTTP_HFO_LOCATION)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_LOCATION,
                         HTTP_SMPO_LOCATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildRequestUri
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pLocation->PreferredUri
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildMaxForwards
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildMaxForwards
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Max-Forwards = "Max-Forwards" ":" 1*DIGIT 
     */
    PHTTP_HFO_MAX_FORWARDS          pMaxForwards    = (PHTTP_HFO_MAX_FORWARDS)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_MAX_FORWARDS,
                         HTTP_SMPO_MAX_FORWARDS_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pMaxForwards->HopCount
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildPragma
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildPragma
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Pragma = "Pragma" ":" 1#pragma-directive
     */
    PHTTP_HFO_PRAGMA                pPragma = (PHTTP_HFO_PRAGMA)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_PRAGMA,
                         HTTP_SMPO_PRAGMA_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pPragma->Directives);       /* directives */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pPragma->Directives,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildProxyAuthenticate
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Proxy-Authenticate = "Proxy-Authenticate" ":" 1#challenge 
     */
    PHTTP_HFO_PROXY_AUTHENTICATE    pProxyAuthenticate  = (PHTTP_HFO_PROXY_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_PROXY_AUTHENTICATE,
                         HTTP_SMPO_PROXY_AUTHENTICATE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulChallengeCount    = pProxyAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pProxyAuthenticate->ChallengeArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            {
                bCopySucc   = HttpSmpoUtilBuildBasicChallenge
                                 (
                                     &pChallenge->Challenge.Basic,
                                     buffer,
                                     ulBufSize,
                                     &ulStart
                                 );

                if (!bCopySucc)
                    return ANSC_STATUS_FAILURE;
            }
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            {
            }
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildProxyAuthorization
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildProxyAuthorization
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Proxy-Authorization = "Proxy-Authorization" ":" credentials
     */
    PHTTP_HFO_PROXY_AUTHORIZATION   pProxyAuthorization = (PHTTP_HFO_PROXY_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_PROXY_AUTHORIZATION,
                         HTTP_SMPO_PROXY_AUTHORIZATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    pCredential = &pProxyAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        {
            PHTTP_CREDENTIAL_BASIC  pBasic;

            pBasic      = &pCredential->Credential.Basic;
            bCopySucc   = HttpSmpoUtilBuildBasicCredentials
                             (
                                 pBasic,
                                 buffer,
                                 ulBufSize,
                                 &ulStart
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        break;

    case HTTP_AUTH_TYPE_NONE:
        /* do nothing */
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        /* do nothing */
        break;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildRange
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildRange
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Range = "Range" ":" ranges-specifier
     *  ranges-specifier = byte-ranges-specifier
     *  byte-ranges-specifier = bytes-unit "=" byte-range-set
     *  bytes-unit = "bytes"
     *  byte-range-set = 1#( byte-range-spec | suffix-byte-range-spec )
     *  byte-range-spec = first-byte-pos "-" [last-byte-pos] 
     *  suffix-byte-range-spec = "-" suffix-length
     */
    PHTTP_HFO_RANGE                 pRange  = (PHTTP_HFO_RANGE)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_RANGE,
                         HTTP_SMPO_RANGE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_BYTES_UNIT,
                         HTTP_SMPO_BYTES_UNIT_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "=",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pRange->bFbpPresent)
    {
        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pRange->FirstBytePos
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    if (pRange->bLbpPresent)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             "-",
                             1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pRange->LastBytePos
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildReferer
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildReferer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Referer = "Referer" ":" ( absoluteURI | relativeURI )
     */
    PHTTP_HFO_REFERER               pReferer    = (PHTTP_HFO_REFERER)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_REFERER,
                         HTTP_SMPO_REFERER_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildRequestUri
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pReferer->ReferrerUri
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildRetryAfter
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildTryAfter
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Retry-After = "Retry-After" ":" ( HTTP-date | delta-seconds ) 
     */
    PHTTP_HFO_RETRY_AFTER           pRetryAfter = (PHTTP_HFO_RETRY_AFTER)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_RETRY_AFTER,
                         HTTP_SMPO_RETRY_AFTER_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pRetryAfter->bDateUsed)
    {
        bCopySucc   = HttpSmpoUtilBuildHttpDate
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             &pRetryAfter->Date
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }
    else
    {
        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pRetryAfter->DeltaSeconds
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildServer
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildServer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Server = "Server" ":" 1*( product | comment ) 
     */
    PHTTP_HFO_SERVER                pServer = (PHTTP_HFO_SERVER)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_SERVER,
                         HTTP_SMPO_SERVER_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pServer->ProductToken);     /* product tokens */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pServer->ProductToken,
                         ulSize 
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildTE
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildTE
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  TE = "TE" ":" #(t-codings)
     *  t-codings = "trailer" | ( transfer-extension [ accept-params ] )
     *  transfer-extension = token
     *  accept-params = ";" "q" "=" qvalue *( accept-extension ) 
     */
    PHTTP_HFO_TE                    pTE     = (PHTTP_HFO_TE)hHttpHfo;
    ULONG                           ulSize  = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_TE,
                         HTTP_SMPO_TE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulEncodingCount = pTE->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTE->EncodingArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildTransferCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_QUALITY_PARAMS,
                             HTTP_SMPO_QUALITY_PARAMS_LENGTH
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildQuality
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Quality
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildTrailer
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildTrailer
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Trailer = "Trailer" ":" 1#field-name
     */
    PHTTP_HFO_TRAILER               pTrailer    = (PHTTP_HFO_TRAILER)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulFieldCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_TRAILER,
                         HTTP_SMPO_TRAILER_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulFieldCount    = pTrailer->FieldCount;
    for (i = 0; i < ulFieldCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildField
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pTrailer->FieldArray[i]
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildTransferEncoding
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildTransferEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding
     *  transfer-coding = "chunked" | transfer-extension
     *  transfer-extension = token
     */
    PHTTP_HFO_TRANSFER_ENCODING     pTransferEncoding   = (PHTTP_HFO_TRANSFER_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_TRANSFER_ENCODING,
                         HTTP_SMPO_TRANSFER_ENCODING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulEncodingCount = pTransferEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTransferEncoding->EncodingArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildTransferCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildUpgrade
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildUpgrade
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Upgrade = "Upgrade" ":" 1#product 
     */
    PHTTP_HFO_UPGRADE               pUpgrade    = (PHTTP_HFO_UPGRADE)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_UPGRADE,
                         HTTP_SMPO_UPGRADE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pUpgrade->ProductToken);
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pUpgrade->ProductToken,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildUserAgent
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildUserAgent
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  User-Agent = "User-Agent" ":" 1*( product | comment ) 
     */
    PHTTP_HFO_USER_AGENT            pUserAgent  = (PHTTP_HFO_USER_AGENT)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_USER_AGENT,
                         HTTP_SMPO_USER_AGENT_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pUserAgent->ProductToken);
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pUserAgent->ProductToken,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildVary
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildVary
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Vary = "Vary" ":" ( "*" | 1#field-name ) 
     */
    PHTTP_HFO_VARY                  pVary   = (PHTTP_HFO_VARY)hHttpHfo;
    ULONG                           ulFieldCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_VARY,
                         HTTP_SMPO_VARY_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulFieldCount = pVary->FieldCount;
    for (i = 0; i < ulFieldCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildField
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pVary->FieldArray[i]
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildVia
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildVia
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Via = "Via" ":" 1#( received-protocol received-by [comment] )
     *  received-protocol = [protocol-name "/"] protocol-version
     *  received-by = ( host [":" port] ) | pseudonym
     */
    PHTTP_HFO_VIA                   pVia    = (PHTTP_HFO_VIA)hHttpHfo;
    ULONG                           ulSize  = 0;
    PHTTP_VIA_HOP                   pViaHop;
    ULONG                           ulViaHopCount, i;
    ULONG                           ulProtocolName, ulProtocolVersion;
    ULONG                           ulHost;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_VIA,
                         HTTP_SMPO_VIA_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulViaHopCount   = pVia->ViaHopCount;
    for (i = 0; i <ulViaHopCount; i ++)
    {
        pViaHop     = &pVia->ViaHopArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulProtocolName  = AnscSizeOfString(pViaHop->Protocol);
        if (ulProtocolName != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pViaHop->Protocol,
                                 ulProtocolName
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "/",
                                 1
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulProtocolVersion   = AnscSizeOfString(pViaHop->Version);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pViaHop->Version,
                             ulProtocolVersion
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        ulHost      = AnscSizeOfString(pViaHop->HostName);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pViaHop->HostName,
                             ulHost
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        if (pViaHop->HostPort != 80)
        {
            bCopySucc   = HttpSmpoUtilBuildUlong
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pViaHop->HostPort
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildWarning
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildWarning
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Warning = "Warning" ":" 1#warning-value 
     *  warning-value = warn-code SP warn-agent SP warn-text
     *  warn-agent = ( host [":" port] ) | pseudonym 
     */
    PHTTP_HFO_WARNING               pWarning    = (PHTTP_HFO_WARNING)hHttpHfo;
    ULONG                           ulSize      = 0;
    PHTTP_WARNING_VALUE             pWarningValue;
    ULONG                           ulWarningCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_WARNING,
                         HTTP_SMPO_WARNING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulWarningCount  = pWarning->WarningCount;
    for (i = 0; i < ulWarningCount; i ++)
    {
        pWarningValue   = &pWarning->WarningArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pWarningValue->Code
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             " ",
                            1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        ulSize      = AnscSizeOfString(pWarningValue->HostName);               /* host name */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pWarningValue->HostName,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
        

        if (pWarningValue->HostPort != 80)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ":",
                                1
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilBuildUlong
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pWarningValue->HostPort
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             " ",
                            1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        ulSize      = AnscSizeOfString(pWarningValue->Text);                   /* warning text */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pWarningValue->Text,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildWwwAuthenticate
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge 
     */
    PHTTP_HFO_WWW_AUTHENTICATE      pWWWAuthenticate    = (PHTTP_HFO_WWW_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_WWW_AUTHENTICATE,
                         HTTP_SMPO_WWW_AUTHENTICATE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;
    
    ulChallengeCount    = pWWWAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pWWWAuthenticate->ChallengeArray[i];
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            {
                bCopySucc   = HttpSmpoUtilBuildBasicChallenge
                                 (
                                     &pChallenge->Challenge.Basic,
                                     buffer,
                                     ulBufSize,
                                     &ulStart
                                 );

                if (!bCopySucc)
                    return ANSC_STATUS_FAILURE;
            }
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            {
                PHTTP_CHALLENGE_DIGEST  pDigestAuth = &pChallenge->Challenge.Digest;

                /* copy "Basic" */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME,
                                     HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME_LENGTH
                                 );

                if (!bCopySucc)
                    return FALSE;

                /* SPACE */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     HTTP_SMPO_SPACE,
                                     HTTP_SMPO_SPACE_LENGTH
                                 );

                if (!bCopySucc)
                    return FALSE;

                /* copy "Basic" */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     pDigestAuth->Realm,
                                     AnscSizeOfString(pDigestAuth->Realm)
                                 );

                if (!bCopySucc)
                    return FALSE;
            }
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildProxyConnection
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildProxyConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Proxy-Connection    = "Proxy-Connection" ":" 1#(proxy-connection-token
     */
    PHTTP_HFO_PROXY_CONNECTION      pProxyConnection    = (PHTTP_HFO_PROXY_CONNECTION)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulTokenCount, i;
    PHTTP_CONNECTION_TOKEN          pToken;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_PROXY_CONNECTION,
                         HTTP_SMPO_PROXY_CONNECTION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;
    
    ulTokenCount    = pProxyConnection->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pToken          = &pProxyConnection->TokenArray[i];
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pToken->Token); /* challenge */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pToken->Token,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildCookie
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           ulCount, i;
    ULONG                           ulVersion;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    UCHAR                           buf[8];
    ULONG                           bufLen;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE,
                         HTTP_SMPO_COOKIE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount     = pCookie->CookieCount;

    if(ulCount == 0)
        return ANSC_STATUS_FAILURE;

    ulVersion   = pCookie->CookieArray[0].Version;
    
    /* 
     * Don't write version into a cookie right now if
     * the version is 1 (default version or no version
     * specified). If we write version into a cookie as
     * metioned by RFC 2109, some HTTP server can not
     * respond (Browser will timeout).
     */
    if (ulVersion != 0)
    {
        AnscInt2String(ulVersion, buf, 10);
        bufLen      = AnscSizeOfString(buf);

        if (bufLen != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_ATTR_PREFIX,
                                 HTTP_SMPO_SET_COOKIE_ATTR_PREFIX_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,   
                                 buf,
                                 bufLen
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    /* write cookie-value(s) */
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pCookie->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_PATH,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;


            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_DOMAIN,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* port */
        pValue      = pCookieContent->Port;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_PORT,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }
    
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildSetCookie
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildSetCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *      Set-Cookie      =       "Set-Cookie:" 1#Cookie-Entity
     *      Cookie-Entity   =       cookie-name "=" value *(";" cookie-av)
     *      cookie-av       =       "Comment"   "="     comment
     *                      |       "Domain"    "="     domain
     *                      |       "Max-Age"   "="     delta-seconds
     *                      |       "Path"      "="     ( "/" | path )
     *                      |       "Secure"
     *                      |       "Version"   "="     1*DIGIT
     *                      |       "expires"   "="     HTTP-date
     *                      |       token       "="     value
     *
     *
     */
    PHTTP_HFO_SET_COOKIE            pSetCookie      = (PHTTP_HFO_SET_COOKIE)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    UCHAR                           buf[16];
    ULONG                           bufLen;
    ULONG                           ulValue;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_SET_COOKIE,
                         HTTP_SMPO_SET_COOKIE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount = pSetCookie->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   &= HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pSetCookie->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment */
        pValue      = pCookieContent->Comment;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PATH,
                                 HTTP_SMPO_SET_COOKIE_PATH_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* version */
        ulValue      = pCookieContent->Version;

        if (ulValue != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->Version, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* expires */
        if (pCookieContent->bPermanent)
        {
            UCHAR                   expires[48];
            PUCHAR                  pWkDay;
            PUCHAR                  pMonth;
            PANSC_UNIVERSAL_TIME    pTime;
            
            pTime   = &pCookieContent->Expires;
            pWkDay  = HttpSmpoUtilGetWeekDayName(pTime->DayOfWeek);
            pMonth  = HttpSmpoUtilGetMonthName(pTime->Month);

            _ansc_sprintf
                (
                    expires, 
                    "; expires=%s, %.2d-%s-%.4d %.2d:%.2d:%.2d GMT",
                    pWkDay,
                    pTime->DayOfMonth,
                    pMonth,
                    pTime->Year,
                    pTime->Hour,
                    pTime->Minute,
                    pTime->Second
                );

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 expires,
                                 AnscSizeOfString(expires)
                             );
        }

        /* max-age */
        ulValue     = pCookieContent->MaxAgeInSeconds;

        if (pCookieContent->bPermanent && ulValue != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->MaxAgeInSeconds, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_SECURE,
                                 HTTP_SMPO_SET_COOKIE_SECURE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildCookie2
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE2,
                         HTTP_SMPO_COOKIE2_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE2_CONTENT,
                         HTTP_SMPO_COOKIE2_CONTENT_LENGTH
                     );
    
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoUtilBuildSetCookie2
            (
                ANSC_HANDLE                 pHttpHfo,
                PVOID                       buffer,
                ULONG                       ulBufSize
            )

    description:

        This function is called to build HTTP header field.

    argument:   ANSC_HANDLE                 pHttpHfo
                Handle to HTTP header field object.
    
                PVOID                       buffer
                Destination buffer.

                ULONG                       ulBufSize
                Desination buffer size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoUtilBuildSetCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *   cookie-av       =   "Comment"   "="     comment
     *               |       "Domain"    "="     set-cookie-domain
     *               |       "Max-Age"   "="     delta-seconds
     *               |       "Path"      "="     set-cookie-path
     *               |       "Secure"
     *               |       "Version"   "="     1*DIGIT
     *               |       "expires"   "="     cookie-expires-date
     *               |       token       "="     value
     *
     *
     */
    PHTTP_HFO_SET_COOKIE2           pSetCookie2     = (PHTTP_HFO_SET_COOKIE2)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    UCHAR                           buf[16];
    ULONG                           bufLen;
    ULONG                           ulValue;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_SET_COOKIE2,
                         HTTP_SMPO_SET_COOKIE2_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount = pSetCookie2->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   &= HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );


            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pSetCookie2->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment */
        pValue      = pCookieContent->Comment;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment url */
        pValue      = pCookieContent->CommentUrl;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_URL,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_URL_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PATH,
                                 HTTP_SMPO_SET_COOKIE_PATH_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* port */
        pValue      = pCookieContent->Port;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PORT,
                                 HTTP_SMPO_SET_COOKIE_PORT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* version */
        ulValue      = pCookieContent->Version;

        if (ulValue != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->Version, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* expires */
        if (pCookieContent->bPermanent)
        {
            UCHAR                   expires[48];
            PUCHAR                  pWkDay;
            PUCHAR                  pMonth;
            PANSC_UNIVERSAL_TIME    pTime;
            
            pTime   = &pCookieContent->Expires;
            pWkDay  = HttpSmpoUtilGetWeekDayName(pTime->DayOfWeek);
            pMonth  = HttpSmpoUtilGetMonthName(pTime->Month);

            _ansc_sprintf
                (
                    expires, 
                    "; expires=%s, %.2d-%s-%.4d %.2d:%.2d:%.2d GMT",
                    pWkDay,
                    pTime->DayOfMonth,
                    pMonth,
                    pTime->Year,
                    pTime->Hour,
                    pTime->Minute,
                    pTime->Second
                );

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 expires,
                                 AnscSizeOfString(expires)
                             );
        }

        /* max-age */
        ulValue     = pCookieContent->MaxAgeInSeconds;

        if (pCookieContent->bPermanent && ulValueLen != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->MaxAgeInSeconds, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_SECURE,
                                 HTTP_SMPO_SET_COOKIE_SECURE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* discard */
        if (pCookieContent->bDiscard)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DISCARD,
                                 HTTP_SMPO_SET_COOKIE_DISCARD_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}

