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

        02/28/02    initial revision.

**********************************************************************/


#include "http_smpo_global.h"


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeRequestUri
            (
                PHTTP_REQUEST_URI           pUri
            )

    description:

        This function is called to get size of request Uri.

    argument:   PHTTP_REQUEST_URI           pUri
                HTTP request Uri object.
    
    return:     the length of request Uri string needed.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetSizeRequestUri
    (
        PHTTP_REQUEST_URI           pUri
    )
{
    ULONG                           ulSize = 0;
    BOOL                            bHttps = FALSE;

    bHttps  = ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) != 0 );

    switch (pUri->Type)
    {
    case HTTP_URI_TYPE_ASTERISK:
        ulSize  = HTTP_SMPO_REQUEST_URI_ASTERISK_LENGTH;
        break;

    case HTTP_URI_TYPE_ABSOLUTE_URI:
        /*
         *     absoluteURI = scheme ":" *( uchar | reserved ) 
         */
        
        if (!bHttps)
        {
            ulSize  += HTTP_SMPO_SCHEME_LENGTH;
        }
        else
        {
            ulSize  += HTTP_SMPO_SCHEME_HTTPS_LENGTH;
        }

        ulSize  += AnscSizeOfString(pUri->HostName);

        if (TRUE)
        {
            ULONG                   ulHostPort;
            UCHAR                   pHostPort[8];
            BOOL                    bCountPort;

            AnscInt2String(pUri->HostPort, (PUCHAR)pHostPort, 10);

            if (bHttps)
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bCountPort)
            {
                ulHostPort = AnscSizeOfString(pHostPort);

                ulSize  += 1;                   /* ":" */
                ulSize  += ulHostPort;
            }
        }

        /*
         * check path segments
         */
        if (pUri->PathLevel >= 2)
        {
            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;

            ulSize      += 1;               /* "/" */
            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                if (
                       i == ulLevels - 1       &&
                       AnscEqualString(pUri->PathArray[i], "/", TRUE)
                   )
                {
                    /* already counted */
                    break;
                }

                /* count each (f)segment */
                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        else
        {
            ulSize  += 1;               /* '/' */
        }
        break;

    case HTTP_URI_TYPE_ABS_PATH:
        {
            /*  
             *  abs_path = "/" rel_path 
             *  rel_path = [path] [";" params] ["?" query] 
             *  path = fsegment *( "/" segment ) 
             */

            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;
            BOOL                    bIsSlash;

            ulSize      = 1;            /* "/" */
            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                /* count each (f)segment */
                bIsSlash    = AnscEqualString(pUri->PathArray[i], "/", TRUE);
                if (bIsSlash && i == ulLevels - 1)
                    break;

                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1 && !bIsSlash)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        break;

    case HTTP_URI_TYPE_RELATIVE_URI:
        {
            /*
             *  relativeURI = net_path | abs_path | rel_path 
             *  net_path = "//" net_loc [abs_path] 
             */
            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;
            ULONG                   ulPathRoot;
/*            ULONG                   ulHostName; */

            ulSize      = 0;

#if 0   /* ??? */
            ulHostName  = AnscSizeOfString(pUri->HostName);

            if (ulHostName != 0)
            {
                ulSize  += 2;           /* "//" */
                ulSize  += ulHostName;  /* net_loc */
            }
#endif

            ulPathRoot  = AnscSizeOfString(pUri->PathArray[0]);

            if (ulPathRoot != 0)
                ulSize      += 1;            /* "/" */

            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                /* count each (f)segment */
                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        break;

    case HTTP_URI_TYPE_AUTHORITY:
        {
            /*
             *  authority = authority-server | reg_name
             *  authority-server = [ [ userinfo "@" ] hostport ]
             *  hostport = host [ ":" port ]
             *  reg_name = 1*( uri-unreserved | escape | "$" | "," |
             *                 ";" | ":" | "@" | "&" | "=" | "+" )
             */
            
            ULONG                   ulHostName;
            ULONG                   ulHostPort;
            UCHAR                   pHostPort[8];
            BOOL                    bCountPort;

            ulHostName = AnscSizeOfString(pUri->HostName);
            AnscInt2String(pUri->HostPort, (PUCHAR)pHostPort, 10);
            ulSize  = ulHostName;

            if (bHttps)
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bCountPort)
            {
                ulHostPort = AnscSizeOfString(pHostPort);

                ulSize  += 1;                   /* ":" */
                ulSize  += ulHostPort;
            }
        }
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetMethodLength
            (
                ULONG               ulMethod,
                PUCHAR              pMethodName
            )

    description:

        This function is called to get size of method.

    argument:   ULONG               ulMethod
                HTTP method code.

                PUCHAR              pMethodName
                Method name used if ulMethod is not valid.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetMethodLength
    (
        ULONG                       ulMethod,
        PUCHAR                      pMethodName
    )
{
    ULONG                           ulLen;

    ulLen = 0;

    switch (ulMethod)
    {
    case HTTP_METHOD_OPTIONS:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS);
        break;

    case HTTP_METHOD_GET:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_GET);
        break;

    case HTTP_METHOD_HEAD:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_HEAD);
        break;

    case HTTP_METHOD_POST:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_POST);
        break;

    case HTTP_METHOD_PUT:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_PUT);
        break;

    case HTTP_METHOD_DELETE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_DELETE);
        break;

    case HTTP_METHOD_TRACE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_TRACE);
        break;

    case HTTP_METHOD_CONNECT:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_CONNECT);
        break;

    case HTTP_METHOD_NOTIFY:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY);
        break;

    case HTTP_METHOD_SEARCH:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_SEARCH);
        break;

    case HTTP_METHOD_M_SEARCH:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH);
        break;

    case HTTP_METHOD_M_POST:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_M_POST);
        break;

    case HTTP_METHOD_SUBSCRIBE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE);
        break;

    case HTTP_METHOD_UNSUBSCRIBE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE);
        break;

    default:
        if (pMethodName)
        {
            ulLen   = AnscSizeOfString(pMethodName);
        }
        break;
    }

    return ulLen;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetHttpVersionLength
            (
                ULONG               MajorVerion,
                ULONG               MinorVersion
            )

    description:

        This function is called to get size of HTTP version.

    argument:   ULONG               MajorVersion
                HTTP major version.

                ULONG               MinorVersion
                HTTP minor version.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetHttpVersionLength
    (
        ULONG                       MajorVersion,
        ULONG                       MinorVersion
    )
{
    /*
     *  HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT 
     */
    UCHAR                           buf[80];
    ULONG                           ulSize;

    ulSize  = HTTP_SMPO_NAME_LENGTH;            /* "HTTP" --- protocol name */
    ulSize  += 1;                               /* "/" */

    AnscInt2String(MajorVersion, buf, 10);      
    ulSize  += AnscSizeOfString(buf);           /* major version number length */

    ulSize  += 1;                               /* "." */

    AnscInt2String(MinorVersion, buf, 10);
    ulSize  += AnscSizeOfString(buf);           /* minor version number length */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetMediaTypeLength
            (
                ULONG               ulMediaType
            )

    description:

        This function is called to get size of media type.

    argument:   ULONG               ulMediaType
                media type.
    
    return:     the string length.

**********************************************************************/

static 
ULONG
HttpSmpoUtilGetMediaTypeLength
    (
        ULONG                       ulMediaType
    )
{
    ULONG                           ulSize = 0;

    switch (ulMediaType)
    {
    case IANA_MEDIA_TYPE_CODE_TEXT:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT);
        break;

    case IANA_MEDIA_TYPE_CODE_MULTIPART:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART);
        break;

    case IANA_MEDIA_TYPE_CODE_MESSAGE:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE);
        break;

    case IANA_MEDIA_TYPE_CODE_APPLICATION:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION);
        break;

    case IANA_MEDIA_TYPE_CODE_IMAGE:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE);
        break;

    case IANA_MEDIA_TYPE_CODE_AUDIO:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO);
        break;

    case IANA_MEDIA_TYPE_CODE_VIDEO:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO);
        break;

    default:
        ulSize = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSubTypeLength
            (
                ULONG               ulType,
                ULONG               ulSubType
            )

    description:

        This function is called to get size of media subtype.

    argument:   ULONG               ulType
                Media type.

                ULONG               ulSubType
                Media subtype.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetMediaSubTypeLength
    (
        ULONG                       ulType,
        ULONG                       ulSubType
    )
{
    ULONG                           ulSize = 0;
    PUCHAR                          pName;

    pName   = AnscGetIanaMediaSubtypeNameByType(ulType, ulSubType);

    if (pName)
    {
        ulSize = AnscSizeOfString(pName);
    }
    else
    {
        ulSize = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetCharsetLength
            (
                ULONG               ulCharset
            )

    description:

        This function is called to get size of charset.

    argument:   ULONG               ulCharset
                Charset code.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetCharsetLength
    (
        ULONG                       ulCharset
    )
{
    ULONG                           ulSize = 0;

    switch (ulCharset)
    {
    case IANA_CHARSET_CODE_US_ASCII:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_US_ASCII);
        break;

    case IANA_CHARSET_CODE_ISO_8859_1:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_1);
        break;

    case IANA_CHARSET_CODE_ISO_8859_2:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_2);
        break;

    case IANA_CHARSET_CODE_ISO_8859_3:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_3);
        break;

    case IANA_CHARSET_CODE_ISO_8859_4:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_4);
        break;

    case IANA_CHARSET_CODE_ISO_8859_5:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_5);
        break;

    case IANA_CHARSET_CODE_ISO_8859_6:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_6);
        break;

    case IANA_CHARSET_CODE_ISO_8859_7:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_7);
        break;

    case IANA_CHARSET_CODE_ISO_8859_8:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_8);
        break;

    case IANA_CHARSET_CODE_ISO_8859_9:
        ulSize = AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_9);
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetContentCodingLength
            (
                ULONG               ulCoding
            )

    description:

        This function is called to get size of content coding.

    argument:   ULONG               ulCoding
                Content coding.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetContentCodingLength
    (
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize = 0;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        break;
        
    case IANA_CCODING_CODE_IDENTITY:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetTransferCodingLength
            (
                ULONG               ulCoding
            )

    description:

        This function is called to get size of transfer coding.

    argument:   ULONG               ulCoding
                Transfer coding number.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetTransferCodingLength
    (
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize = 0;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        break;
        
    case IANA_CCODING_CODE_IDENTITY:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        break;

    case IANA_TCODING_CODE_CHUNKED:
        ulSize = AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED);
        break;
        
    case IANA_TCODING_CODE_TRAILERS:
        ulSize = AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS);
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetQualityLength
            (
                ULONG               ulQuality
            )

    description:

        This function is called to get size of quality.

    argument:   ULONG               ulQuality
                Quality value.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetQualityLength
    (
        ULONG                       ulQuality
    )
{
    ULONG                           ulSize = 0;
    if (ulQuality == 1000)
        ulSize = 3;                          /* "1.0" */
    else
    if (ulQuality == 0)
        ulSize = 1;                          /* "0" */
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
                break;
            }
            else
            {
                ulSize  --;
            }
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetUlongStringLength
            (
                ULONG               ulValue
            )

    description:

        This function is called to get size of a ULONG value.

    argument:   ULONG               ulValue
                ULONG value.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetUlongStringLength
    (
        ULONG                       ulValue
    )
{
    UCHAR                       buf[16];
    ULONG                       ulSize;

    AnscInt2String(ulValue, buf, 10);
    ulSize   = AnscSizeOfString(buf);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetHttpDateLength
            (
                PHTTP_DATE          pDate
            )

    description:

        This function is called to get size of HTTP date.

    argument:   PHTTP_DATE          pDate
                HTTP date object.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetHttpDateLength
    (
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
    ULONG                           ulSize  = 0;
    ULONG                           ulDate1 = 0;
    ULONG                           ulTime  = 0;

    ulSize  += 3;                       /* wkday */
    ulSize  += 1;                       /* "," */
    ulSize  += 1;                       /* space */

    ulDate1 += 2;                       /* day */
    ulDate1 += 1;                       /* space */
    ulDate1 += 3;                       /* month */
    ulDate1 += 1;                       /* space */
    ulDate1 += 4;                       /* year */

    ulSize  += ulDate1;                 /* date1 */

    ulTime  += 1;                       /* space */
    ulTime  += 2;                       /* hour */
    ulTime  += 1;                       /* ":" */
    ulTime  += 2;                       /* minute */
    ulTime  += 1;                       /* ":" */
    ulTime  += 2;                       /* second */

    ulSize  += ulTime;                  /* time */
    ulSize  += 1;                       /* space */
    ulSize  += 3;                       /* "GMT" */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetHeaderLength
            (
                ULONG               ulHeaderId
            )

    description:

        This function is called to get size of header name.

    argument:   ULONG               ulHeaderId
                The ID of a HTTP header.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetHeaderLength
    (
        ULONG                       ulHeaderId
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    ULONG                           ulSize = 0;

    switch (ulHeaderId)
    {
    case HTTP_HEADER_ID_ACCEPT:
        ulSize  = HTTP_SMPO_ACCEPT_LENGTH;
        break;

    case HTTP_HEADER_ID_ACCEPT_CHARSET:
        ulSize  = HTTP_SMPO_ACCEPT_CHARSET_LENGTH;
        break;

    case HTTP_HEADER_ID_ACCEPT_ENCODING:
        ulSize  = HTTP_SMPO_ACCEPT_ENCODING_LENGTH;
        break;

    case HTTP_HEADER_ID_ACCEPT_LANGUAGE:
        ulSize  = HTTP_SMPO_ACCEPT_LANGUAGE_LENGTH;
        break;

    case HTTP_HEADER_ID_ACCEPT_RANGES:
        ulSize  = HTTP_SMPO_ACCEPT_RANGES_LENGTH;
        break;

    case HTTP_HEADER_ID_AGE:
        ulSize  = HTTP_SMPO_AGE_LENGTH;
        break;

    case HTTP_HEADER_ID_ALLOW:
        ulSize  = HTTP_SMPO_AGE_LENGTH;
        break;

    case HTTP_HEADER_ID_AUTHORIZATION:
        ulSize  = HTTP_SMPO_AUTHORIZATION_LENGTH;
        break;

    case HTTP_HEADER_ID_CACHE_CONTROL:
        ulSize  = HTTP_SMPO_CACHE_CONTROL_LENGTH;
        break;

    case HTTP_HEADER_ID_CONNECTION:
        ulSize  = HTTP_SMPO_CONNECTION_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_ENCODING:
        ulSize  = HTTP_SMPO_CONTENT_ENCODING_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_LANGUAGE:
        ulSize  = HTTP_SMPO_CONTENT_LANGUAGE_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_LENGTH:
        ulSize  = HTTP_SMPO_CONTENT_LENGTH_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_LOCATION:
        ulSize  = HTTP_SMPO_CONTENT_LOCATION_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_MD5:
        ulSize  = HTTP_SMPO_CONTENT_MD5_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_RANGE:
        ulSize  = HTTP_SMPO_CONTENT_RANGE_LENGTH;
        break;

    case HTTP_HEADER_ID_CONTENT_TYPE:
        ulSize  = HTTP_SMPO_CONTENT_TYPE_LENGTH;
        break;

    case HTTP_HEADER_ID_DATE:
        ulSize  = HTTP_SMPO_DATE_HEADER_LENGTH;
        break;

    case HTTP_HEADER_ID_ETAG:
        ulSize  = HTTP_SMPO_ETAG_LENGTH;
        break;

    case HTTP_HEADER_ID_EXPECT:
        ulSize  = HTTP_SMPO_EXPECT_LENGTH;
        break;

    case HTTP_HEADER_ID_EXPIRES:
        ulSize  = HTTP_SMPO_EXPIRES_LENGTH;
        break;

    case HTTP_HEADER_ID_FROM:
        ulSize  = HTTP_SMPO_EXPIRES_LENGTH;
        break;

    case HTTP_HEADER_ID_HOST:
        ulSize  = HTTP_SMPO_HEADER_HOST_LENGTH;
        break;

    case HTTP_HEADER_ID_IF_MATCH:
        ulSize  = HTTP_SMPO_IF_MATCH_LENGTH;
        break;

    case HTTP_HEADER_ID_IF_MODIFIED_SINCE:
        ulSize  = HTTP_SMPO_IF_MODIFIED_SINCE_LENGTH;
        break;

    case HTTP_HEADER_ID_IF_NONE_MATCH:
        ulSize  = HTTP_SMPO_IF_NONE_MATCH_LENGTH;
        break;

    case HTTP_HEADER_ID_IF_RANGE:
        ulSize  = HTTP_SMPO_IF_RANGE_LENGTH;
        break;

    case HTTP_HEADER_ID_IF_UNMODIFIED_SINCE:
        ulSize  = HTTP_SMPO_IF_UNMODIFIED_SINCE_LENGTH;
        break;

    case HTTP_HEADER_ID_LAST_MODIFIED:
        ulSize  = HTTP_SMPO_LAST_MODIFIED_LENGTH;
        break;

    case HTTP_HEADER_ID_LOCATION:
        ulSize  = HTTP_SMPO_LOCATION_LENGTH;
        break;

    case HTTP_HEADER_ID_MAX_FORWARDS:
        ulSize  = HTTP_SMPO_MAX_FORWARDS_LENGTH;
        break;

    case HTTP_HEADER_ID_PRAGMA:
        ulSize  = HTTP_SMPO_PRAGMA_LENGTH;
        break;

    case HTTP_HEADER_ID_PROXY_AUTHENTICATE:
        ulSize  = HTTP_SMPO_PROXY_AUTHENTICATE_LENGTH;
        break;

    case HTTP_HEADER_ID_PROXY_AUTHORIZATION:
        ulSize  = HTTP_SMPO_PROXY_AUTHORIZATION_LENGTH;
        break;

    case HTTP_HEADER_ID_RANGE:
        ulSize  = HTTP_SMPO_RANGE_LENGTH;
        break;

    case HTTP_HEADER_ID_REFERER:
        ulSize  = HTTP_SMPO_REFERER_LENGTH;
        break;

    case HTTP_HEADER_ID_RETRY_AFTER:
        ulSize  = HTTP_SMPO_RETRY_AFTER_LENGTH;
        break;

    case HTTP_HEADER_ID_SERVER:
        ulSize  = HTTP_SMPO_SERVER_LENGTH;
        break;

    case HTTP_HEADER_ID_TE:
        ulSize  = HTTP_SMPO_TE_LENGTH;
        break;

    case HTTP_HEADER_ID_TRAILER:
        ulSize  = HTTP_SMPO_TRAILER_LENGTH;
        break;

    case HTTP_HEADER_ID_TRANSFER_ENCODING:
        ulSize  = HTTP_SMPO_TRANSFER_ENCODING_LENGTH;
        break;

    case HTTP_HEADER_ID_UPGRADE:
        ulSize  = HTTP_SMPO_UPGRADE_LENGTH;
        break;

    case HTTP_HEADER_ID_USER_AGENT:
        ulSize  = HTTP_SMPO_USER_AGENT_LENGTH;
        break;

    case HTTP_HEADER_ID_VARY:
        ulSize  = HTTP_SMPO_VARY_LENGTH;
        break;

    case HTTP_HEADER_ID_VIA:
        ulSize  = HTTP_SMPO_VIA_LENGTH;
        break;

    case HTTP_HEADER_ID_WARNING:
        ulSize  = HTTP_SMPO_WARNING_LENGTH;
        break;

    case HTTP_HEADER_ID_WWW_AUTHENTICATE:
        ulSize  = HTTP_SMPO_WWW_AUTHENTICATE_LENGTH;
        break;
    }

    return ulSize;
}



/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeBasicCredential
            (
                PHTTP_CREDENTIAL_BASIC  pBasic
            )

    description:

        This function is called to get size of basic credential.

    argument:   PHTTP_CREDENTIAL_BASIC  pBasic
                Basic credential object.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetSizeBasicCredential
    (
        PHTTP_CREDENTIAL_BASIC      pBasic
    )
{
    ULONG                           ulSize  = 0;
    ULONG                           ulUser, ulPass, ulBufLen;
    PUCHAR                          pBuf, pEncBuf;

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

        pBuf[ulBufLen]      = 0;

        /* Encode the USER:PASS */
        pEncBuf = AnscBase64Encode(pBuf, ulBufLen - 1);   
        AnscFreeMemory(pBuf);

        /* Basic */
        ulSize  += HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH;

        /* Space */
        ulSize  += HTTP_SMPO_SPACE_LENGTH;

        /* basic-cookie */
        ulSize  += AnscSizeOfString(pEncBuf);

        AnscFreeMemory(pEncBuf);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeBasicChallenge
            (
                PHTTP_CHALLENGE_BASIC   pBasic
            )

    description:

        This function is called to get size of basic challenge.

    argument:   PHTTP_CHALLENGE_BASIC   pBasic
                Basic challenge object.
    
    return:     the string length.

**********************************************************************/

static
ULONG
HttpSmpoUtilGetSizeBasicChallenge
    (
        PHTTP_CHALLENGE_BASIC       pBasic
    )
{
    ULONG                           ulSize  = 0;

    /* "Basic" */
    ulSize  += HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH;

    /* Space */
    ulSize  += HTTP_SMPO_SPACE_LENGTH;

    /* "realm" */
    ulSize  += HTTP_SMPO_REALM_LENGTH;

    /* "=" */
    ulSize  ++;

    /* realm value */
    ulSize  += AnscSizeOfString(pBasic->Realm)  + 2;    /* two '"' (double quote) */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeRequestLine
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of request line.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeRequestLine
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * Even the definition of "Request-Line" requires a CRLF at
     * the end, but here we don't count it.
     *
     * Request-Line = Method SP Request-URI SP HTTP-Version CRLF
     */

    PHTTP_REQUEST_INFO              pRequestInfo;
    ULONG                           ulSize = 0;

    pRequestInfo = (PHTTP_REQUEST_INFO)hHttpHfo;

    if (!pRequestInfo)
        return 0;

    ulSize  = HttpSmpoUtilGetMethodLength(pRequestInfo->Method, pRequestInfo->MethodName);
    ulSize  += 1;                   /* space */

    ulSize  += HttpSmpoUtilGetSizeRequestUri(&pRequestInfo->RequestUri);

    ulSize  += 1;                   /* space */    
    ulSize  += HttpSmpoUtilGetHttpVersionLength(pRequestInfo->MajorVersion, pRequestInfo->MinorVersion);
    
    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeRequestLine
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of status line.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeStatusLine
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
     */
    ULONG                           ulSize = 0;
    PHTTP_RESPONSE_INFO             pResponseInfo;
    UCHAR                           buf[8];

    pResponseInfo   = (PHTTP_RESPONSE_INFO)hHttpHfo;
    if (!pResponseInfo)
        return 0;

    /* HTTP-Version */
    ulSize  += HttpSmpoUtilGetHttpVersionLength(pResponseInfo->MajorVersion, pResponseInfo->MinorVersion);
    ulSize  += 1;                                   /* space */

    AnscInt2String(pResponseInfo->StatusCode, buf, 10);
    ulSize  += AnscSizeOfString(buf);               /* status code */

    ulSize  += 1;                                   /* space */
    ulSize  += AnscSizeOfString(pResponseInfo->ReasonPhrase);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeHeader
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeHeader
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    PHTTP_HEADER_FIELD              pHeader;
    ULONG                           ulSize = 0;

    pHeader = (PHTTP_HEADER_FIELD)hHttpHfo;

    if (!pHeader)
        return 0;

    if (pHeader->Flags & HTTP_FIELD_FLAG_LINE_PRESENT)
    {
        /* this header is still valid, so return the size of HeaderLine */
        ulSize = AnscSizeOfString(pHeader->HeaderLine);
    }
    else
    {
        PFN_HTTP_SMPO_GET_HEADER_SIZE   pfGetSize = NULL;

        switch (pHeader->HeaderId)
        {
        case HTTP_HEADER_ID_ACCEPT:
            pfGetSize = HttpSmpoUtilGetSizeAccept;
            break;

        case HTTP_HEADER_ID_ACCEPT_CHARSET:
            pfGetSize = HttpSmpoUtilGetSizeAcceptCharset;
            break;

        case HTTP_HEADER_ID_ACCEPT_ENCODING:
            pfGetSize = HttpSmpoUtilGetSizeAcceptEncoding;
            break;

        case HTTP_HEADER_ID_ACCEPT_LANGUAGE:
            pfGetSize = HttpSmpoUtilGetSizeAcceptLanguage;
            break;

        case HTTP_HEADER_ID_ACCEPT_RANGES:
            pfGetSize = HttpSmpoUtilGetSizeAcceptRanges;
            break;

        case HTTP_HEADER_ID_AGE:
            pfGetSize = HttpSmpoUtilGetSizeAge;
            break;

        case HTTP_HEADER_ID_ALLOW:
            pfGetSize = HttpSmpoUtilGetSizeAllow;
            break;

        case HTTP_HEADER_ID_AUTHORIZATION:
            pfGetSize = HttpSmpoUtilGetSizeAuthorization;
            break;

        case HTTP_HEADER_ID_CACHE_CONTROL:
            pfGetSize = HttpSmpoUtilGetSizeCacheControl;
            break;

        case HTTP_HEADER_ID_CONNECTION:
            pfGetSize = HttpSmpoUtilGetSizeConnection;
            break;

        case HTTP_HEADER_ID_CONTENT_ENCODING:
            pfGetSize = HttpSmpoUtilGetSizeContentEncoding;
            break;

        case HTTP_HEADER_ID_CONTENT_LANGUAGE:
            pfGetSize = HttpSmpoUtilGetSizeContentLanguage;
            break;

        case HTTP_HEADER_ID_CONTENT_LENGTH:
            pfGetSize = HttpSmpoUtilGetSizeContentLength;
            break;

        case HTTP_HEADER_ID_CONTENT_LOCATION:
            pfGetSize = HttpSmpoUtilGetSizeContentLocation;
            break;

        case HTTP_HEADER_ID_CONTENT_MD5:
            pfGetSize = HttpSmpoUtilGetSizeContentMD5;
            break;

        case HTTP_HEADER_ID_CONTENT_RANGE:
            pfGetSize = HttpSmpoUtilGetSizeContentRange;
            break;

        case HTTP_HEADER_ID_CONTENT_TYPE:
            pfGetSize = HttpSmpoUtilGetSizeContentType;
            break;

        case HTTP_HEADER_ID_DATE:
            pfGetSize = HttpSmpoUtilGetSizeDate;
            break;

        case HTTP_HEADER_ID_ETAG:
            pfGetSize = HttpSmpoUtilGetSizeETag;
            break;

        case HTTP_HEADER_ID_EXPECT:
            pfGetSize = HttpSmpoUtilGetSizeExpect;
            break;

        case HTTP_HEADER_ID_EXPIRES:
            pfGetSize = HttpSmpoUtilGetSizeExpires;
            break;

        case HTTP_HEADER_ID_FROM:
            pfGetSize = HttpSmpoUtilGetSizeFrom;
            break;

        case HTTP_HEADER_ID_HOST:
            pfGetSize = HttpSmpoUtilGetSizeHost;
            break;

        case HTTP_HEADER_ID_IF_MATCH:
            pfGetSize = HttpSmpoUtilGetSizeIfMatch;
            break;

        case HTTP_HEADER_ID_IF_MODIFIED_SINCE:
            pfGetSize = HttpSmpoUtilGetSizeIfModifiedSince;
            break;

        case HTTP_HEADER_ID_IF_NONE_MATCH:
            pfGetSize = HttpSmpoUtilGetSizeIfNoneMatch;
            break;

        case HTTP_HEADER_ID_IF_RANGE:
            pfGetSize = HttpSmpoUtilGetSizeIfRange;
            break;

        case HTTP_HEADER_ID_IF_UNMODIFIED_SINCE:
            pfGetSize = HttpSmpoUtilGetSizeIfUnmodifiedSince;
            break;

        case HTTP_HEADER_ID_LAST_MODIFIED:
            pfGetSize = HttpSmpoUtilGetSizeLastModified;
            break;

        case HTTP_HEADER_ID_LOCATION:
            pfGetSize = HttpSmpoUtilGetSizeLocation;
            break;

        case HTTP_HEADER_ID_MAX_FORWARDS:
            pfGetSize = HttpSmpoUtilGetSizeMaxForwards;
            break;

        case HTTP_HEADER_ID_PRAGMA:
            pfGetSize = HttpSmpoUtilGetSizePragma;
            break;

        case HTTP_HEADER_ID_PROXY_AUTHENTICATE:
            pfGetSize = HttpSmpoUtilGetSizeProxyAuthenticate;
            break;

        case HTTP_HEADER_ID_PROXY_AUTHORIZATION:
            pfGetSize = HttpSmpoUtilGetSizeProxyAuthorization;
            break;

        case HTTP_HEADER_ID_RANGE:
            pfGetSize = HttpSmpoUtilGetSizeRange;
            break;

        case HTTP_HEADER_ID_REFERER:
            pfGetSize = HttpSmpoUtilGetSizeReferer;
            break;

        case HTTP_HEADER_ID_RETRY_AFTER:
            pfGetSize = HttpSmpoUtilGetSizeTryAfter;
            break;

        case HTTP_HEADER_ID_SERVER:
            pfGetSize = HttpSmpoUtilGetSizeServer;
            break;

        case HTTP_HEADER_ID_TE:
            pfGetSize = HttpSmpoUtilGetSizeTE;
            break;

        case HTTP_HEADER_ID_TRAILER:
            pfGetSize = HttpSmpoUtilGetSizeTrailer;
            break;

        case HTTP_HEADER_ID_TRANSFER_ENCODING:
            pfGetSize = HttpSmpoUtilGetSizeTransferEncoding;
            break;

        case HTTP_HEADER_ID_UPGRADE:
            pfGetSize = HttpSmpoUtilGetSizeUpgrade;
            break;

        case HTTP_HEADER_ID_USER_AGENT:
            pfGetSize = HttpSmpoUtilGetSizeUserAgent;
            break;

        case HTTP_HEADER_ID_VARY:
            pfGetSize = HttpSmpoUtilGetSizeVary;
            break;

        case HTTP_HEADER_ID_VIA:
            pfGetSize = HttpSmpoUtilGetSizeVia;
            break;

        case HTTP_HEADER_ID_WARNING:
            pfGetSize = HttpSmpoUtilGetSizeWarning;
            break;

        case HTTP_HEADER_ID_WWW_AUTHENTICATE:
            pfGetSize = HttpSmpoUtilGetSizeWWWAuthenticate;
            break;

        case HTTP_HEADER_ID_PROXY_CONNECTION:
            pfGetSize = HttpSmpoUtilGetSizeProxyConnection;
            break;

        case HTTP_HEADER_ID_COOKIE:
            pfGetSize = HttpSmpoUtilGetSizeCookie;
            break;

        case HTTP_HEADER_ID_SET_COOKIE:
            pfGetSize = HttpSmpoUtilGetSizeSetCookie;
            break;
        }

        /* call the corresponding function to get size */
        if (pfGetSize)
            ulSize = (*pfGetSize)(hHttpMP, hHttpHfo);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAccept
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAccept
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
#if 0 
    Accept = "Accept" ":" #( media-range [accept-params] )
    media-range = ( "*/*" | ( type "/" "*" ) | ( type "/" subtype ) ) 
                     *( ";" parameter ) 
    accept-params = ";" "q" "=" qvalue *( accept-extension ) 
#endif

    PHTTP_HFO_ACCEPT                pAccept = (PHTTP_HFO_ACCEPT)hHttpHfo;
    ULONG                           ulSize  = 0;
    ULONG                           ulRangeCount, i;
    PHTTP_MEDIA_RANGE               pRange;
    ULONG                           ulMediaType, ulMediaSubType, ulQuality, ulParams;

    ulSize  = HTTP_SMPO_ACCEPT_LENGTH;               /* "Accept" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;    /* ": " */

    ulRangeCount = pAccept->RangeCount;
    for (i = 0; i < ulRangeCount; i ++)
    {
        pRange          = &pAccept->RangeArray[i];

        ulMediaType     = HttpSmpoUtilGetMediaTypeLength(pRange->MediaType);
        ulMediaSubType  = HttpSmpoUtilGetMediaSubTypeLength(pRange->MediaType, pRange->SubType);

        ulQuality       = HttpSmpoUtilGetQualityLength(pRange->Quality);

        ulParams = AnscSizeOfString(pRange->Parameters);

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;    /* ", " field separator */

        ulSize  += ulMediaType;                     /* media type */
        ulSize  += 1;                               /* "/" */
        ulSize  += ulMediaSubType;                  /* media subtype */

        if (ulParams != 0)
        {
            ulSize  += 1;                           /* ";" */
            ulSize  += ulParams;                    /* parameters */
        }

        ulSize  += 1;                               /* ";" */
        ulSize  += HTTP_SMPO_QUALITY_LENGTH;        /* "q" */
        ulSize  += 1;                               /* "=" */
        ulSize  += ulQuality;                       /* quality value */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAcceptCharset
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAcceptCharset
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Accept-Charset = "Accept-Charset" ":" 1#( charset [";" "q" "=" qvalue] ) 
     */
    ULONG                           ulSize          = 0;
    PHTTP_HFO_ACCEPT_CHARSET        pAcceptCharset  = (PHTTP_HFO_ACCEPT_CHARSET)hHttpHfo;
    ULONG                           ulCharsetCount, i;
    PHTTP_CHARSET                   pCharset;
    ULONG                           ulTypeLen, ulQuality;
    
    ulSize  = HTTP_SMPO_ACCEPT_CHARSET_LENGTH;      /* "Accept-Charset" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */

    ulCharsetCount  = pAcceptCharset->CharsetCount;
    for (i = 0; i < ulCharsetCount; i ++)
    {
        pCharset    = &pAcceptCharset->CharsetArray[i];
        ulTypeLen   = HttpSmpoUtilGetCharsetLength(pCharset->Type);
        ulQuality   = HttpSmpoUtilGetQualityLength(pCharset->Quality);

        if (i != 0)         /* not the first charset */
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += ulTypeLen;
        ulSize      += 3;                           /* ";q=" */
        ulSize      += ulQuality;                   /* quality value */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAcceptEncoding
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAcceptEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Accept-Encoding = "Accept-Encoding" ":" 1#( codings [ ";" "q" "=" qvalue ] )
     */
    PHTTP_HFO_ACCEPT_ENCODING       pAcceptEncoding;
    ULONG                           ulSize = 0;
    ULONG                           ulEncodingCount, i;
    ULONG                           ulCodingSize, ulQuality;
    PHTTP_ENCODING                  pEncoding;

    pAcceptEncoding = (PHTTP_HFO_ACCEPT_ENCODING)hHttpHfo;

    ulSize          = HTTP_SMPO_ACCEPT_ENCODING_LENGTH;     /* "Accept-Encoding" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */

    ulEncodingCount = pAcceptEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding       = &pAcceptEncoding->EncodingArray[i];
        ulCodingSize    = HttpSmpoUtilGetContentCodingLength(pEncoding->Type);
        ulQuality       = HttpSmpoUtilGetQualityLength(pEncoding->Quality);

        if (i != 0)
            ulSize += HTTP_SMPO_FIELD_SEPARATOR_LENGTH; /* field separator ", " */

        ulSize          += ulCodingSize;                /* encoding name */
        ulSize          += 3;                           /* ";q=" */
        ulSize          += ulQuality;                   /* quality value */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAcceptLanguage
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAcceptLanguage
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Accept-Language = "Accept-Language" ":" 1#( language-range [";" "q" "=" qvalue] ) 
     */
    PHTTP_HFO_ACCEPT_LANGUAGE       pAcceptLanguage = (PHTTP_HFO_ACCEPT_LANGUAGE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulLanguageCount, i;
    ULONG                           ulQuality;
    PHTTP_LANGUAGE                  pLanguage;

    ulSize          = HTTP_SMPO_ACCEPT_LANGUAGE_LENGTH;     /* "Accept-Language" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */

    ulLanguageCount = pAcceptLanguage->LanguageCount;
    for (i = 0; i < ulLanguageCount; i ++)
    {
        pLanguage   = &pAcceptLanguage->LanguageArray[i];
        ulQuality   = HttpSmpoUtilGetQualityLength(pLanguage->Quality);

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += AnscSizeOfString(pLanguage->Tag);    /* language range */
        ulSize      += 3;                                   /* ";q=" */
        ulSize      += ulQuality;                           /* quality value */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAcceptRanges
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAcceptRanges
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_ACCEPT_RANGES_LENGTH;      /* "Accept-Ranges" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */

    ulRangeCount    = pAcceptRanges->RangeCount;
    for (i = 0; i < ulRangeCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;    /* ", " */

        ulSize      += AnscSizeOfString(pAcceptRanges->RangeArray[i].Unit);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAge
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAge
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Age = "Age" ":" age-value 
     */
    PHTTP_HFO_AGE                   pAge    = (PHTTP_HFO_AGE)hHttpHfo;
    ULONG                           ulSize  = 0;

    ulSize  += HTTP_SMPO_AGE_LENGTH;                                /* "Age"     */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                   /* ": "      */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pAge->DeltaSeconds);  /* age value */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAllow
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAllow
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * Allow = "Allow" ":" 1#Method 
     */
    PHTTP_HFO_ALLOW                 pAllow  = (PHTTP_HFO_ALLOW)hHttpHfo;
    ULONG                           ulSize  = 0;
    ULONG                           ulMethodCount, i;
    
    ulSize          += HTTP_SMPO_ALLOW_LENGTH;              /* "Allow" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */
    
    ulMethodCount   = pAllow->MethodCount;
    for (i = 0; i < ulMethodCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += HttpSmpoUtilGetMethodLength(pAllow->MethodArray[i], NULL);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeAuthorization
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeAuthorization
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Authorization = "Authorization" ":" credentials
     */
    PHTTP_HFO_AUTHORIZATION         pAuthorization  = (PHTTP_HFO_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize          = 0;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    ulSize      += HTTP_SMPO_AUTHORIZATION_LENGTH;                  /* "Authorization" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;               /* ": " */

    pCredential = &pAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        ulSize  += HttpSmpoUtilGetSizeBasicCredential(&pCredential->Credential.Basic);
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        break;

    case HTTP_AUTH_TYPE_NONE:
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeCacheControl
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeCacheControl
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Cache-Control = "Cache-Control" ":" 1#cache-directive
     */
    PHTTP_HFO_CACHE_CONTROL         pCacheControl   = (PHTTP_HFO_CACHE_CONTROL)hHttpHfo;
    ULONG                           ulSize          = 0;

    ulSize  += HTTP_SMPO_CACHE_CONTROL_LENGTH;              /* "Cache-Control" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */
    ulSize  += AnscSizeOfString(pCacheControl->Directives); /* directives */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeConnection
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeConnection
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_CONNECTION_LENGTH;             /* "Connection" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */
    
    ulTokenCount    = pConnection->TokenCount;

    for (i = 0; i <ulTokenCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += AnscSizeOfString(pConnection->TokenArray[i].Token);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentEncoding
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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
    
    ulSize          += HTTP_SMPO_CONTENT_ENCODING_LENGTH;       /* "Content-Encoding" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */
    
    ulEncodingCount = pContentEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pContentEncoding->EncodingArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        ulSize      += HttpSmpoUtilGetContentCodingLength(pEncoding->Type);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentLanguage
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentLanguage
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_CONTENT_LANGUAGE_LENGTH;       /* "Content-Language" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;

    ulLanguageCount = pContentLanguage->LanguageCount;
    for (i = 0; i < ulLanguageCount; i ++)
    {
        pLanguage   = &pContentLanguage->LanguageArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += AnscSizeOfString(pLanguage->Tag);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentLength
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentLength
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Length = "Content-Length" ":" 1*DIGIT 
     */
    PHTTP_HFO_CONTENT_LENGTH        pContentLength  = (PHTTP_HFO_CONTENT_LENGTH)hHttpHfo;
    ULONG                           ulSize          = 0;

    ulSize  += HTTP_SMPO_CONTENT_LENGTH_LENGTH;                             /* "Content-Length" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                           /* ": " */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pContentLength->BodySize);    /* body size */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentLocation
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentLocation
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Location = "Content-Location" ":" ( absoluteURI | relativeURI )
     */
    PHTTP_HFO_CONTENT_LOCATION      pContentLocation    = (PHTTP_HFO_CONTENT_LOCATION)hHttpHfo;
    ULONG                           ulSize              = 0;

    ulSize  += HTTP_SMPO_CONTENT_LOCATION_LENGTH;                               /* "Content-Location" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                               /* ": "*/
    ulSize  += HttpSmpoUtilGetSizeRequestUri(&pContentLocation->LocationUri);   /* uri */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentMD5
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentMD5
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-MD5 = "Content-MD5" ":" md5-digest
     *  md5-digest = 16 (OCTET)
     */
    PHTTP_HFO_CONTENT_MD5           pContentMD5 = (PHTTP_HFO_CONTENT_MD5)hHttpHfo;
    ULONG                           ulSize      = 0;
    PUCHAR                          pEncMD5;
    ULONG                           ulEncMD5Len;

    ulSize  += HTTP_SMPO_CONTENT_MD5_LENGTH;            /* "Content-MD5" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    pEncMD5 = AnscBase64Encode(pContentMD5->Digest, ANSC_MD5_OUTPUT_SIZE);
    ulEncMD5Len = 0;
    if (pEncMD5)
    {
        ulEncMD5Len = AnscSizeOfString(pEncMD5);
        AnscFreeMemory(pEncMD5);
    }

    ulSize  += ulEncMD5Len;

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentRange
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize  += HTTP_SMPO_CONTENT_RANGE_LENGTH;              /* "Content-Range" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */
    ulSize  += HTTP_SMPO_BYTES_UNIT_LENGTH;                 /* "bytes" */
    ulSize  += 1;                                           /* space */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pContentRange->FirstBytePos);   /* first byte position */
    ulSize  += 1;                                           /* "-" */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pContentRange->LastBytePos);    /* last byte position */
    ulSize  += 1;                                           /* "/" */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pContentRange->InstaceLength);  /* instance length */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeContentType
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeContentType
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Type = "Content-Type" ":" media-type
     *  media-type = type "/" subtype *( ";" parameter )
     */
    PHTTP_HFO_CONTENT_TYPE          pContentType    = (PHTTP_HFO_CONTENT_TYPE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulParams;

    ulSize  += HTTP_SMPO_CONTENT_TYPE_LENGTH;                               /* "Content-Type" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                           /* ": " */

    ulSize  += HttpSmpoUtilGetMediaTypeLength(pContentType->MediaType);     /* media type */
    ulSize  += 1;                                                           /* "/" */
    ulSize  += HttpSmpoUtilGetMediaSubTypeLength(pContentType->MediaType, pContentType->SubType);    /* subtype */
    
    ulParams = AnscSizeOfString(pContentType->Parameters);
    if (ulParams != 0)
    {
        ulSize  += 1;                       /* ";" */
        ulSize  += ulParams;                /* parameter */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeDate
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeDate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Date = "Date" ":" HTTP-date
     *  rfc1123-date = wkday "," SP date1 SP time SP "GMT"
     */
    PHTTP_HFO_DATE                  pDate   = (PHTTP_HFO_DATE)hHttpHfo;
    ULONG                           ulSize  = 0;

    /* only rfc1123 */
    ulSize  += HTTP_SMPO_DATE_HEADER_LENGTH;                /* "Date" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */
    ulSize  += HttpSmpoUtilGetHttpDateLength(&pDate->Date); /* rfc1123-date */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeETag
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeETag
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize  += HTTP_SMPO_ETAG_LENGTH;               /* "ETag" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */
    
    if (pETag->bWeak)
        ulSize  += 2;                               /* "W/" */

    ulSize  += AnscSizeOfString(pETag->Tag);        /* opaque-tag */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeExpect
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeExpect
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_EXPECT_LENGTH;                 /* "Expect" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTokenCount    = pExpect->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pExpectToken    = &pExpect->TokenArray[i];

        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;    /* ", " */

        ulSize          += AnscSizeOfString(pExpectToken->Token);   /* token */

        ulParams        = AnscSizeOfString(pExpectToken->Parameters);
        if (ulParams != 0)
        {
            ulSize      += 1;                                   /* "=" */
            ulSize      += ulParams;                            /* Parameters */
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeExpires
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeExpires
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Expires = "Expires" ":" HTTP-date 
     */
    PHTTP_HFO_EXPIRES               pExpires    = (PHTTP_HFO_EXPIRES)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_EXPIRES_LENGTH;                    /* "Expires" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HttpSmpoUtilGetHttpDateLength(&pExpires->Date);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeFrom
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeFrom
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize  += HTTP_SMPO_FROM_LENGTH;               /* "From" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;   /* ": " */

    ulSize  += AnscSizeOfString(pFrom->MailBox.LocalPart);  /* local part */
    ulSize  += 1;                                           /* "@" */
    ulSize  += AnscSizeOfString(pFrom->MailBox.DomainName); /* domain name */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeHost
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeHost
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Host = "Host" ":" host [":" port]  ; Section 3.2.2  
     */
    PHTTP_HFO_HOST                  pHost   = (PHTTP_HFO_HOST)hHttpHfo;
    ULONG                           ulSize  = 0;

    ulSize  += HTTP_SMPO_HEADER_HOST_LENGTH;            /* "Host" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulSize  += AnscSizeOfString(pHost->HostName);       /* host name */
    if (pHost->HostPort != 80)
    {
        ulSize  += 1;                                   /* ":" */
        ulSize  += HttpSmpoUtilGetUlongStringLength(pHost->HostPort);   /* port */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeIfMatch
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeIfMatch
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize      += HTTP_SMPO_IF_MATCH_LENGTH;               /* "If-Match" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTagCount  = pIfMatch->TagCount;
    for (i = 0; i < ulTagCount; i ++)
    {
        pTag    = &pIfMatch->TagArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        if (pTag->bWeak)
            ulSize  += 2;                                   /* "W/" */

        ulSize      += AnscSizeOfString(pTag->Tag);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeIfModifiedSince
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeIfModifiedSince
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  If-Modified-Since = "If-Modified-Since" ":" HTTP-date 
     */
    PHTTP_HFO_IF_MODIFIED_SINCE     pIfModifiedSince    = (PHTTP_HFO_IF_MODIFIED_SINCE)hHttpHfo;
    ULONG                           ulSize              = 0;

    ulSize  += HTTP_SMPO_IF_MODIFIED_SINCE_LENGTH;          /* "If-Modified-Since" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */
    
    ulSize  += HttpSmpoUtilGetHttpDateLength(&pIfModifiedSince->Date);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeIfNoneMatch
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeIfNoneMatch
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize      += HTTP_SMPO_IF_NONE_MATCH_LENGTH;          /* "If-None-Match" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTagCount  = pIfNoneMatch->TagCount;
    for (i = 0; i < ulTagCount; i ++)
    {
        pTag    = &pIfNoneMatch->TagArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        if (pTag->bWeak)
            ulSize  += 2;                                   /* "W/" */

        ulSize      += AnscSizeOfString(pTag->Tag);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeIfRange
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeIfRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize      += HTTP_SMPO_IF_RANGE_LENGTH;               /* "If-Range" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    if (pIfRange->bTagUsed)
    {
        if (pIfRange->ETag.bWeak)
            ulSize  += 2;                                   /* "W/" */

        ulSize      += AnscSizeOfString(pIfRange->ETag.Tag);    /* opaque-tag */
    }
    else
    {
        ulSize  += HttpSmpoUtilGetHttpDateLength(&pIfRange->Date);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeIfUnmodifiedSince
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeIfUnmodifiedSince
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  If-Unmodified-Since = "If-Unmodified-Since" ":" HTTP-date 
     */
    PHTTP_HFO_IF_UNMODIFIED_SINCE   pIfUnmodifiedSince  = (PHTTP_HFO_IF_UNMODIFIED_SINCE)hHttpHfo;
    ULONG                           ulSize              = 0;

    ulSize  += HTTP_SMPO_IF_UNMODIFIED_SINCE_LENGTH;        /* "If-Unmodified-Since" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HttpSmpoUtilGetHttpDateLength(&pIfUnmodifiedSince->Date);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeLastModified
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeLastModified
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Last-Modified = "Last-Modified" ":" HTTP-date
     */
    PHTTP_HFO_LAST_MODIFIED         pLastModified    = (PHTTP_HFO_LAST_MODIFIED)hHttpHfo;
    ULONG                           ulSize           = 0;

    ulSize  += HTTP_SMPO_LAST_MODIFIED_LENGTH;              /* "Last-Modified" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HttpSmpoUtilGetHttpDateLength(&pLastModified->Date);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeLocation
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeLocation
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Location = "Location" ":" absoluteURI
     */
    PHTTP_HFO_LOCATION              pLocation   = (PHTTP_HFO_LOCATION)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_LOCATION_LENGTH;                   /* "Location" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HttpSmpoUtilGetSizeRequestUri(&pLocation->PreferredUri);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeMaxForwards
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeMaxForwards
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Max-Forwards = "Max-Forwards" ":" 1*DIGIT 
     */
    PHTTP_HFO_MAX_FORWARDS          pMaxForwards    = (PHTTP_HFO_MAX_FORWARDS)hHttpHfo;
    ULONG                           ulSize          = 0;

    ulSize  += HTTP_SMPO_MAX_FORWARDS_LENGTH;               /* "Max-Forwards" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HttpSmpoUtilGetUlongStringLength(pMaxForwards->HopCount);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizePragma
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizePragma
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Pragma = "Pragma" ":" 1#pragma-directive
     */
    PHTTP_HFO_PRAGMA                pPragma = (PHTTP_HFO_PRAGMA)hHttpHfo;
    ULONG                           ulSize  = 0;

    ulSize  += HTTP_SMPO_PRAGMA_LENGTH;                     /* "Pragma" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += AnscSizeOfString(pPragma->Directives);       /* directives */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeProxyAuthenticate
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeProxyAuthenticate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Proxy-Authenticate = "Proxy-Authenticate" ":" 1#challenge 
     */
    PHTTP_HFO_PROXY_AUTHENTICATE    pProxyAuthenticate  = (PHTTP_HFO_PROXY_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;


    ulSize              += HTTP_SMPO_PROXY_AUTHENTICATE_LENGTH;         /* "Proxy-Authenticate" */
    ulSize              += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulChallengeCount    = pProxyAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pProxyAuthenticate->ChallengeArray[i];

        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;            /* ", " */

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            ulSize  += HttpSmpoUtilGetSizeBasicChallenge(&pChallenge->Challenge.Basic);
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeProxyAuthorization
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeProxyAuthorization
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Proxy-Authorization = "Proxy-Authorization" ":" credentials
     */
    PHTTP_HFO_PROXY_AUTHORIZATION   pProxyAuthorization = (PHTTP_HFO_PROXY_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    ulSize  += HTTP_SMPO_PROXY_AUTHORIZATION_LENGTH;        /* "Proxy-Authorization" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    pCredential = &pProxyAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        ulSize  += HttpSmpoUtilGetSizeBasicCredential(&pCredential->Credential.Basic);
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        break;

    case HTTP_AUTH_TYPE_NONE:
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        break;
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeRange
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeRange
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize  += HTTP_SMPO_RANGE_LENGTH;                      /* "Range" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += HTTP_SMPO_BYTES_UNIT_LENGTH;                 /* "bytes" */
    ulSize  += 1;                                           /* "=" */

    if (pRange->bFbpPresent)
        ulSize  += HttpSmpoUtilGetUlongStringLength(pRange->FirstBytePos);  /* first byte position */

    if (pRange->bLbpPresent)
    {
        ulSize  += 1;                                       /* "-" */
        ulSize  += HttpSmpoUtilGetUlongStringLength(pRange->LastBytePos);   /* last byte position */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeReferer
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeReferer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Referer = "Referer" ":" ( absoluteURI | relativeURI )
     */
    PHTTP_HFO_REFERER               pReferer    = (PHTTP_HFO_REFERER)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_REFERER_LENGTH;                                /* "Referer" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                       /* ": "*/
    ulSize  += HttpSmpoUtilGetSizeRequestUri(&pReferer->ReferrerUri);   /* uri */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeTryAfter
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeTryAfter
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Retry-After = "Retry-After" ":" ( HTTP-date | delta-seconds ) 
     */
    PHTTP_HFO_RETRY_AFTER           pRetryAfter = (PHTTP_HFO_RETRY_AFTER)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_RETRY_AFTER_LENGTH;                /* "Retry-After" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": "*/

    if (pRetryAfter->bDateUsed)
        ulSize  += HttpSmpoUtilGetHttpDateLength(&pRetryAfter->Date);
    else
        ulSize  += HttpSmpoUtilGetUlongStringLength(pRetryAfter->DeltaSeconds);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeServer
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeServer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Server = "Server" ":" 1*( product | comment ) 
     */
    PHTTP_HFO_SERVER                pServer = (PHTTP_HFO_SERVER)hHttpHfo;
    ULONG                           ulSize  = 0;

    ulSize  += HTTP_SMPO_SERVER_LENGTH;                     /* "Server" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": "*/

    ulSize  += AnscSizeOfString(pServer->ProductToken);     /* product tokens */

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeTE
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeTE
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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
    ULONG                           ulQuality, ulType;

    ulSize          += HTTP_SMPO_TE_LENGTH;                         /* "TE" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": "*/

    ulEncodingCount = pTE->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTE->EncodingArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;            /* ", " */

        ulType      = HttpSmpoUtilGetTransferCodingLength(pEncoding->Type);
        ulQuality   = HttpSmpoUtilGetQualityLength(pEncoding->Quality);

        ulSize      += ulType;                                      /* tranfer coding */
        ulSize      += 3;                                           /* ";q=" */
        ulSize      += ulQuality;                                    /* quality value */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeTrailer
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeTrailer
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Trailer = "Trailer" ":" 1#field-name
     */
    PHTTP_HFO_TRAILER               pTrailer    = (PHTTP_HFO_TRAILER)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulFieldCount, i;

    ulSize          += HTTP_SMPO_TRAILER_LENGTH;                    /* "Trailer" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": "*/

    ulFieldCount    = pTrailer->FieldCount;
    for (i = 0; i < ulFieldCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;                    /* ", " */

        ulSize      += HttpSmpoUtilGetHeaderLength(pTrailer->FieldArray[i]);  /* field-name */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeTransferEncoding
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeTransferEncoding
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_TRANSFER_ENCODING_LENGTH;      /* "Transfer-Encoding" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */
    
    ulEncodingCount = pTransferEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTransferEncoding->EncodingArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += HttpSmpoUtilGetTransferCodingLength(pEncoding->Type);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeUpgrade
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeUpgrade
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Upgrade = "Upgrade" ":" 1#product 
     */
    PHTTP_HFO_UPGRADE               pUpgrade    = (PHTTP_HFO_UPGRADE)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_UPGRADE_LENGTH;                    /* "Upgrade" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += AnscSizeOfString(pUpgrade->ProductToken);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeUserAgent
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeUserAgent
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  User-Agent = "User-Agent" ":" 1*( product | comment ) 
     */
    PHTTP_HFO_USER_AGENT            pUserAgent  = (PHTTP_HFO_USER_AGENT)hHttpHfo;
    ULONG                           ulSize      = 0;

    ulSize  += HTTP_SMPO_USER_AGENT_LENGTH;                 /* "User-Agent" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulSize  += AnscSizeOfString(pUserAgent->ProductToken);

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeVary
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeVary
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Vary = "Vary" ":" ( "*" | 1#field-name ) 
     */
    PHTTP_HFO_VARY                  pVary   = (PHTTP_HFO_VARY)hHttpHfo;
    ULONG                           ulSize  = 0;
    ULONG                           ulFieldCount, i;

    ulSize      += HTTP_SMPO_VARY_LENGTH;                       /* "Vary" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulFieldCount = pVary->FieldCount;
    for (i = 0; i < ulFieldCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += HttpSmpoUtilGetHeaderLength(pVary->FieldArray[i]);
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeVia
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeVia
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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
    ULONG                           ulHost, ulPort;

    ulSize          += HTTP_SMPO_VIA_LENGTH;                    /* "Via" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulViaHopCount   = pVia->ViaHopCount;
    for (i = 0; i <ulViaHopCount; i ++)
    {
        pViaHop     = &pVia->ViaHopArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulProtocolName  = AnscSizeOfString(pViaHop->Protocol);
        if (ulProtocolName != 0)
        {
            ulSize  += ulProtocolName;                          /* protocol name */
            ulSize  += 1;                                       /* "/" */
        }

        ulProtocolVersion   = AnscSizeOfString(pViaHop->Version);
        ulSize              += ulProtocolVersion;

        ulHost              = AnscSizeOfString(pViaHop->HostName);
        ulSize              += ulHost;

        if (pViaHop->HostPort != 80)
        {
            ulPort          = HttpSmpoUtilGetUlongStringLength(pViaHop->HostPort);
            ulSize          += ulPort;
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeWarning
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeWarning
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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

    ulSize          += HTTP_SMPO_WARNING_LENGTH;                /* "Warning" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulWarningCount  = pWarning->WarningCount;
    for (i = 0; i < ulWarningCount; i ++)
    {
        pWarningValue   = &pWarning->WarningArray[i];

        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize          += HttpSmpoUtilGetUlongStringLength(pWarningValue->Code);   /* warning code */
        ulSize          += 1;                                                       /* space */

        ulSize          += AnscSizeOfString(pWarningValue->HostName);               /* host name */
        
        if (pWarningValue->HostPort != 80)
        {
            ulSize      += 1;                                                       /* ":" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(pWarningValue->HostPort);   /* port */
        }

        ulSize          += 1;                                                       /* space */
        ulSize          += AnscSizeOfString(pWarningValue->Text);                   /* warning text */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeWWWAuthenticate
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge 
     */
    PHTTP_HFO_WWW_AUTHENTICATE      pWWWAuthenticate    = (PHTTP_HFO_WWW_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    
    ulSize              += HTTP_SMPO_WWW_AUTHENTICATE_LENGTH;       /* "WWW-Authenticate" */
    ulSize              += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulChallengeCount    = pWWWAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pWWWAuthenticate->ChallengeArray[i];
        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            ulSize  += HttpSmpoUtilGetSizeBasicChallenge(&pChallenge->Challenge.Basic);
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeProxyConnection
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeProxyConnection
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Proxy-Connection        = "Proxy-Connection" ":" 1#(proxy-connection-token)
     *  proxy-connection-token  = token
     */
    PHTTP_HFO_PROXY_CONNECTION      pProxyConnection    = (PHTTP_HFO_PROXY_CONNECTION)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulTokenCount, i;
    PHTTP_CONNECTION_TOKEN          pToken;
    
    ulSize              += HTTP_SMPO_PROXY_CONNECTION_LENGTH;       /* "Proxy-Connection" */
    ulSize              += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTokenCount        = pProxyConnection->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pToken          = &pProxyConnection->TokenArray[i];
        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        ulSize          += AnscSizeOfString(pToken->Token);         /* token */
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeCookie
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeCookie
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     *  cookie-value = cookie-name "=" value [";" cookie-path] [";" cookie-domain]
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulCount, i;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    ULONG                           ulVersion;
    
    ulSize  += HTTP_SMPO_COOKIE_LENGTH;                 /* "Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulCount = pCookie->CookieCount;

    if (ulCount != 0)
    {
        ulVersion   = pCookie->CookieArray[0].Version;

        if (ulVersion != 1)
        {
            ulSize      += HTTP_SMPO_SET_COOKIE_ATTR_PREFIX_LENGTH;
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulVersion);
            ulSize      += HTTP_SMPO_SET_COOKIE_VERSION_LENGTH;
        }

        for (i = 0; i < ulCount; i ++)
        {
            if (i != 0)
                ulSize  ++;         /* ";" or "," */

            pCookieContent  = &pCookie->CookieArray[i];
            
            /* name */
            pValue      = pCookieContent->Name;
            ulValueLen  = AnscSizeOfString(pValue);
            ulSize      += ulValueLen;

            /* "=" */
            ulSize  ++;

            /* value */
            pValue      = pCookieContent->Value;
            ulValueLen  = AnscSizeOfString(pValue);
            ulSize      += ulValueLen;

            /* path */
            pValue      = pCookieContent->Path;
            ulValueLen  = AnscSizeOfString(pValue);

            if (ulValueLen != 0)
            {
                ulSize      ++;     /* ";" */
                ulSize      += ulValueLen;
            }

            /* domain */
            pValue      = pCookieContent->Domain;
            ulValueLen  = AnscSizeOfString(pValue);

            if (ulValueLen != 0)
            {
                ulSize      ++;     /* ";" */
                ulSize      += ulValueLen;
            }
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeSetCookie
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeSetCookie
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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
    PHTTP_HFO_SET_COOKIE            pSetCookie          = (PHTTP_HFO_SET_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulValue;
    
    ulSize  += HTTP_SMPO_SET_COOKIE_LENGTH;                 /* "Set-Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulCount = pSetCookie->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        pCookieContent  = &pSetCookie->CookieArray[i];

        /* name */
        pValue          = pCookieContent->Name;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* "=" */
        ulSize ++;

        /* value */
        pValue          = pCookieContent->Value;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* comment */
        pValue          = pCookieContent->Comment;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            ulSize ++;  /* ";" */   
            ulSize      += HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* path */
        pValue          = pCookieContent->Path;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;     
            ulSize      += HTTP_SMPO_SET_COOKIE_PATH_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* domain */
        pValue          = pCookieContent->Domain;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;     
            ulSize      += HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* version */
        ulValue         = pCookieContent->Version;
        if (ulValue != 0)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_VERSION_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* max-age */
        ulValue         = pCookieContent->MaxAgeInSeconds;
        if (ulValue != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_SECURE_LENGTH;
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeCookie2
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeCookie2
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     *  cookie-value = cookie-name "=" value [";" cookie-path] [";" cookie-domain]
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    
    ulSize  += HTTP_SMPO_COOKIE2_LENGTH;                /* "Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulSize  += HTTP_SMPO_COOKIE2_CONTENT_LENGTH;

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeSetCookie2
            (
                ANSC_HANDLE         hHttpMP,
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:   ANSC_HANDLE         hHttpMp
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.
    
    return:     the string length.

**********************************************************************/

ULONG
HttpSmpoUtilGetSizeSetCookie2
    (
        ANSC_HANDLE                 hHttpMP,
        ANSC_HANDLE                 hHttpHfo
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
    PHTTP_HFO_SET_COOKIE2           pSetCookie2          = (PHTTP_HFO_SET_COOKIE2)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulValue;
    
    ulSize  += HTTP_SMPO_SET_COOKIE_LENGTH;                 /* "Set-Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulCount = pSetCookie2->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        pCookieContent  = &pSetCookie2->CookieArray[i];

        /* name */
        pValue          = pCookieContent->Name;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* "=" */
        ulSize ++;

        /* value */
        pValue          = pCookieContent->Value;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* comment */
        pValue          = pCookieContent->Comment;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            ulSize ++;  /* ";" */   
            ulSize      += HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* comment URL */
        pValue          = pCookieContent->CommentUrl;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            ulSize ++;  /* ";" */   
            ulSize      += HTTP_SMPO_SET_COOKIE_COMMENT_URL_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* path */
        pValue          = pCookieContent->Path;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;     
            ulSize      += HTTP_SMPO_SET_COOKIE_PATH_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* domain */
        pValue          = pCookieContent->Domain;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;     
            ulSize      += HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* Port */
        pValue          = pCookieContent->Port;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;     
            ulSize      += HTTP_SMPO_SET_COOKIE_PORT_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* version */
        ulValue         = pCookieContent->Version;
        if (ulValue != 0)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_VERSION_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* max-age */
        ulValue         = pCookieContent->MaxAgeInSeconds;
        if (ulValue != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_SECURE_LENGTH;
        }

        /* secure */
        if (pCookieContent->bDiscard)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_DISCARD_LENGTH;
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

            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_EXPIRES_LENGTH;
            ulSize ++;
            ulSize      += AnscSizeOfString(expires);
        }
    }

    return ulSize;
}

