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

    module:	http_smpo_parse_uri.c

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

        BOOL
        HttpSmpoUtilParseAbsoluteURI
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hRequestUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse absolute URI.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hRequestUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of absolute Uri string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAbsoluteURI
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* absoluteURI = scheme ":" *( uchar | reserved ) */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pScheme = pBuf;
    PUCHAR                          pNext;
    ULONG                           ulLen;
    BOOL                            bHttp   = FALSE;
    USHORT                          usPort  = 0;
    PUCHAR                          pHost;

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_COLON, ulSize);

    if (!pNext)
    {
        return FALSE;
    }

    pHost   = pNext + 1;

    ulLen   = pNext - pBuf;

    if ( ulLen == HTTP_SMPO_PROTOCOL_HTTPS_LENGTH &&
        AnscEqualString2((char *)pScheme, HTTP_SMPO_PROTOCOL_HTTPS, HTTP_SMPO_PROTOCOL_HTTPS_LENGTH, FALSE) )
    {
        pUri->Flag  |= HTTP_URI_FLAG_HTTPS;
        bHttp       = TRUE;
        usPort      = HTTPS_PORT;
    }
    else 
        if ( ulLen == 4 && AnscEqualString2((char *)pScheme, "http", 4, FALSE) )
        {
            bHttp   = TRUE;
            usPort  = HTTP_PORT;
        }

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SLASH, pNext - pScheme);

    if (pNext)
    {
        /* scheme name shouldn't have slash in it */

        return FALSE;
    }

    pUri->Type  = HTTP_URI_TYPE_ABSOLUTE_URI;
    HttpSmpoUtilCopyHeaderString(pBuf, ulSize, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

    if (bHttp)
    {
        PUCHAR                      pLast   = pBuf + ulSize - 1;

        pUri->HostPort  = usPort;

        pNext   = pHost;

        if ( *(pNext + 1) == HTTP_SMPO_CHAR_SLASH )
        {
            pHost   = pNext + 2;
        }

        if (pHost)
        {
            BOOL                    bPath       = TRUE;
            PUCHAR                  pPort;
            PUCHAR                  pHostEnd    = NULL;

            pNext   = _ansc_memchr(pHost, HTTP_SMPO_CHAR_SLASH, pLast - pHost + 1);

            if (!pNext)
            {
                pNext       = _ansc_memchr(pHost, HTTP_SMPO_CHAR_SPACE, pLast - pHost + 1);
                bPath       = FALSE;
                pHostEnd    = pLast;
            }
            else
            {
                pHostEnd    = pNext - 1;
            }

            if (TRUE)
            {
                if (bPath && pNext)
                {
                    HttpSmpoUtilParseAbsPath(hHttpSmpo, hRequestUri, pNext, pLast - pNext + 1);
                    pUri->Type  = HTTP_URI_TYPE_ABSOLUTE_URI;
                }
                else
                {
                    pUri->PathArray[0][0]   = '/';
                    pUri->PathArray[0][1]   = 0;
                }

                if (pNext)
                {
                    pPort   = _ansc_memchr(pHost, HTTP_SMPO_CHAR_COLON, pNext - pHost);
                }
                else
                {
                    pPort   = _ansc_memchr(pHost, HTTP_SMPO_CHAR_COLON, pHostEnd - pHost + 1);
                }

                if (pPort)
                {
                    pUri->HostPort = _ansc_atoi((const char *)pPort + 1);
                }
                else
                {
                    pUri->HostPort  = usPort;
                }

                HttpSmpoUtilCopyHeaderString
                    (
                        pHost, 
                        pPort ? (pPort - pHost) : ( pNext ? (pNext - pHost) : (pLast - pHost + 1) ), 
                        pUri->HostName, 
                        ANSC_DOMAIN_NAME_SIZE
                    );
            }
        }
    }

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRelativeURI
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hRequestUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse relative URI.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hRequestUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of relative Uri string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRelativeURI
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* relativeURI = net_path | abs_path | rel_path */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hRequestUri;
    BOOL                            bSucc   = FALSE;

    bSucc   = HttpSmpoUtilParseNetPath(hHttpSmpo, hRequestUri, pBuf, ulSize);

    if (!bSucc)
    {
        bSucc   = HttpSmpoUtilParseAbsPath(hHttpSmpo, hRequestUri, pBuf, ulSize);

        if (!bSucc)
        {
            bSucc   = HttpSmpoUtilParseRelPath(hHttpSmpo, hRequestUri, pBuf, ulSize);
        }
    }

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_RELATIVE_URI;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseNetPath
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hRequestUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse Net-Path.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hRequestUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of Net-Path string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseNetPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* net_path = "//" net_loc [abs_path] */
    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hRequestUri;
    BOOL                            bSucc   = TRUE;
    PUCHAR                          pNetLoc;
    PUCHAR                          pToken  = pBuf;
    PUCHAR                          pLast   = pBuf + ulSize - 1;

    if (ulSize < 2 || *pToken != HTTP_SMPO_CHAR_SLASH || *(pToken + 1) != HTTP_SMPO_CHAR_SLASH)
    {
        return FALSE;
    }

    pNetLoc = pToken + 2;

    pToken  = _ansc_memchr(pNetLoc, HTTP_SMPO_CHAR_SLASH, pLast - pNetLoc + 1);

    if (!pToken)
    {
        HttpSmpoUtilCopyHeaderString(pNetLoc, pLast - pNetLoc + 1, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);
    }
    else
    {
        bSucc   = HttpSmpoUtilParseAbsPath(hHttpSmpo, hRequestUri, pToken, pLast - pToken + 1);
    }

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_RELATIVE_URI;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseAbsPath
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hRequestUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse Abs-Path.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hRequestUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of Abs-Path string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAbsPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* abs_path = "/" rel_path */
    
    PHTTP_REQUEST_URI               pUri        = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pRelPath    = pBuf;
    BOOL                            bSucc       = TRUE;

    if (ulSize == 0 || *pRelPath != HTTP_SMPO_CHAR_SLASH)
    {
        return FALSE;
    }

    pUri->PathArray[0][0]   = '/';
    pUri->PathArray[0][1]   = 0;

    pUri->PathLevel = 1;

    bSucc   = HttpSmpoUtilParseRelPath(hHttpSmpo, hRequestUri, pRelPath + 1, ulSize - 1);

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_ABS_PATH;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRelPath
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hRequestUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse Rel-Path.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hRequestUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of Rel-Path string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRelPath
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* rel_path = [path] [";" params] ["?" query] */

    PHTTP_REQUEST_URI               pUri        = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    ULONG                           ulLen;
    PUCHAR                          pNext;
    PUCHAR                          pQuery;
    ULONG                           ulQueryLen;
    PUCHAR                          pPath       = pBuf;
    PUCHAR                          pPathEnd;

    pQuery  = _ansc_memchr(pPath, HTTP_SMPO_CHAR_QUESTION, pLast - pPath + 1);

    if (pQuery)
    {
        ulQueryLen  = pLast - pQuery;
        pPathEnd    = pQuery - 1;
    }
    else
    {
        ulQueryLen  = 0;
        pPathEnd    = pLast;
    }

    /* parsing path segments */
    while (pPath)
    {
        pNext   = _ansc_memchr(pPath, HTTP_SMPO_CHAR_SLASH, pPathEnd - pPath + 1);

        if (pNext)
        {
            ulLen   = pNext - pPath;
        }
        else
        {
            ulLen   = pPathEnd - pPath + 1;
        }

        if (ulLen == 0)
        {
            if (pNext)
            {
                pUri->PathArray[pUri->PathLevel][0] = '/';
                pUri->PathArray[pUri->PathLevel][1] = 0;
            }
            else
            {
                pUri->PathLevel --;
            }
        }
        else
        {
            HttpSmpoUtilCopyHeaderString(pPath, ulLen, pUri->PathArray[pUri->PathLevel], HTTP_MAX_PATH_NAME_SIZE);
        }

        pUri->PathLevel ++;

        if (pNext)
        {
            if (pNext == pPathEnd)
            {
                pUri->PathArray[pUri->PathLevel][0] = '/';
                pUri->PathArray[pUri->PathLevel][1] = 0;
                pUri->PathLevel ++;

                pPath   = NULL;
            }
            else
            {
                pPath   = pNext + 1;
            }
        }
        else
        {
            pPath   = NULL;
        }

        /* RDKB-6239, CID-24419; Out of bound memory
        ** Max value "->PathLevel" can achieve is 15 to avoid memory curruption
        */
        if (pUri->PathLevel >= (HTTP_MAX_PATH_LEVEL_NUMBER-1)) 
        {
            break; /* too many paths */
        }
    }

    /* parse query parameters */
    /* CID:58183 Dereference after null check*/
    /* Incrementing null pointer pQuery*/
    if ( pQuery )
         pQuery ++;
    HttpSmpoUtilCopyHeaderString(pQuery, ulQueryLen, pUri->QueryParams, HTTP_MAX_URI_QUERY_SIZE);

    return TRUE;
}




