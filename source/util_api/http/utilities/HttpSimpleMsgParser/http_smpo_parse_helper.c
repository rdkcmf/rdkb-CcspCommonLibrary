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

    module:	http_smpo_parse_helper.c

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
        HttpSmpoUtilParseRequestLineVersion
            (
                PUCHAR                      pVersion,
                ULONG                       ulVersionLen,
                PULONG                      pMajorVersion,
                PULONG                      pMinorVersion
            )

    description:

        This function is called to parse HTTP version.

    argument:   PUCHAR                      pVersion
                String containing HTTP version.

                ULONG                       ulVersionLen
                The length of the version string.

                PULONG                      pMajorVersion
                Containing result major version if it succeeds.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRequestLineVersion
    (
        PUCHAR                      pVersion,
        ULONG                       ulVersionLen,
        PULONG                      pMajorVersion,
        PULONG                      pMinorVersion
    )
{
    UNREFERENCED_PARAMETER(ulVersionLen);
    PUCHAR                          pMajor, pMinor;

    pMajor  = (PUCHAR)_ansc_strchr((const char *)pVersion, '/');
    if (!pMajor)
        return FALSE;

    pMajor ++;
    *pMajorVersion  = (ULONG)AnscString2Int((const char *)pMajor);

    pMinor  = (PUCHAR)_ansc_strchr((const char *)pMajor, '.');
    if (!pMinor)
        return FALSE;

    pMinor ++;
    *pMinorVersion  = (ULONG)AnscString2Int((const char *)pMinor);

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRquestLineUri
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hUri,
                PUCHAR                      pBuf,
                ULONG                       ulBufLen
            )

    description:

        This function is called to parse HTTP Uri.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hUri
                HTTP Uri object.

                PUCHAR                      pBuf
                Buffer of Uri string.

                ULONG                       ulBufLen
                The size of Uri string buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRquestLineUri
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulBufLen
    )
{
    /* Request-URI = "*" | absoluteURI | abs_path | authority */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    BOOL                            bSucc   = TRUE;

    if (strcmp((char *)pBuf, "*") == 0 )
    {
        pUri->Type  = HTTP_URI_TYPE_ASTERISK;
    }
    else
    {
        bSucc   = HttpSmpoUtilParseAbsoluteURI(hHttpSmpo, hUri, pBuf, ulBufLen);

        if (!bSucc)
        {
            bSucc   = HttpSmpoUtilParseAbsPath(hHttpSmpo, hUri, pBuf, ulBufLen);

            if (!bSucc)
            {
                bSucc   = HttpSmpoUtilParseAuthority(hHttpSmpo, hUri, pBuf, ulBufLen);
            }
        }
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        void
        HttpSmpoUtilCopyHeaderString
            (
                PUHCAR                      pString,
                ULONG                       ulStringLen,
                char                        *pBuf,
                ULONG                       ulBufSize
            )

    description:

        This function is called to copy given string to the
        specified buffer.

    argument:   PUCHAR                      pString
                String to be copied from.

                ULONG                       ulStringLen
                The size of string.

                char                        *pBuf
                The destination buffer.

                ULONG                       ulBufSize
                The size of destination buffer.
    
    return:     void.

**********************************************************************/

void
HttpSmpoUtilCopyHeaderString
    (
        PUCHAR                      pString,
        ULONG                       ulStringLen,
        char                        *pBuf,
        ULONG                       ulBufSize
    )
{
    if ( ulStringLen == 0 )
    {
        return;
    }

    if (ulStringLen >= ulBufSize)
    {
        ulStringLen = ulBufSize - 1;
    }

    AnscCopyMemory(pBuf, pString, ulStringLen);
    pBuf[ulStringLen] = 0;
}


/**********************************************************************

    caller:     component objects

    prototype:

        void
        HttpSmpoUtilInitializeUri
            (
                PHTTP_REQUEST_URI           pUri
            )

    description:

        This function is called to initialize request URI object
        with default values.

    argument:   PHTTP_REQUEST_URI           pUri
                The request URI object.

    return:     void.

**********************************************************************/

void
HttpSmpoUtilInitializeUri
    (
        PHTTP_REQUEST_URI           pUri
    )
{
    pUri->Type              = HTTP_URI_TYPE_RESERVED;
    pUri->HostPort          = 80;
    pUri->HostName[0]       = 0;
    pUri->QueryParams[0]    = 0;
    pUri->PathLevel         = 1;
    pUri->PathArray[0][0]   = '/';
    pUri->PathArray[0][1]   = 0;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilLinearWhiteSpace
            (
                PUCHAR                      pBuf,
                ULONG                       ulBufLen
            )

    description:

        This function is called to remove all leading
        linear-white-space characters.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hUri
                HTTP Uri object.

                PUCHAR                      pBuf
                Buffer of Uri string.

                ULONG                       ulBufLen
                The size of Uri string buffer.
    
    return:     the position of the first non 
                liear-white-space character.

**********************************************************************/

PUCHAR
HttpSmpoUtilLinearWhiteSpace
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pNext   = pBuf;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    BOOL                            bCRLF   = FALSE;
    UCHAR                           c;
    PUCHAR                          pAfter;

    while (pNext <= pLast)
    {
        c   = *pNext;

        if (c != HTTP_SMPO_CHAR_SPACE && c != HTTP_SMPO_CHAR_HTAB && c != HTTP_SMPO_CHAR_CR)
        {
            return pNext;
        }
        else
        {
            bCRLF = (c == HTTP_SMPO_CHAR_CR && *(pNext + 1) == HTTP_SMPO_CHAR_LF);

            if (!bCRLF)
            {
                pNext ++;
            }
            else
            {
                pAfter = pNext + 2;
                
                if (pAfter >= pLast)
                {
                    return pNext;
                }

                c   = *pAfter;
                if (c != HTTP_SMPO_CHAR_SPACE && c != HTTP_SMPO_CHAR_HTAB)
                {
                    return pNext;
                }
                else
                {
                    pNext   = pAfter + 1;
                }
            }
        }
    }

    return pNext;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseHttpDate
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse HTTP date.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date string.

                ULONG                       ulSize
                The size of date string buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseHttpDate
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    BOOL                            bSucc       = FALSE;
    PUCHAR                          pNext;
    ULONG                           ulDateFmt   = 0;

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_COMMA, ulSize);

    if (!pNext)
    {
        ulDateFmt   = 2;
    }
    else
    {
        if (pNext - pBuf == 3)
        {
            ulDateFmt   = 0;
        }
        else
        {
            ulDateFmt   = 1;
        }
    }

    switch (ulDateFmt)
    {
        case    0:
            
            bSucc   = HttpSmpoUtilParseRfc1123Date(hDate, pBuf, ulSize);
            
            break;

        case    1:
            
            bSucc   = HttpSmpoUtilParseRfc850Date(hDate, pBuf, ulSize);
            
            break;

        case    2:
           
            bSucc   = HttpSmpoUtilParseAsciiTimeDate(hDate, pBuf, ulSize);

           break;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRfc1123Date
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse RFC 1123 date.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date string.

                ULONG                       ulSize
                The size of date string buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRfc1123Date
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    BOOL                            bSucc   = TRUE;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* rfc1123-date = wkday "," SP date1 SP time SP "GMT" */

    /* wkday */
    pToken  = pBuf;
    pNext       = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);
    if (!pNext)
    {
        return FALSE;
    }
    ulTokenSize = pNext - pToken;

    pDate->DayOfWeek    = HttpSmpoUtilGetDayOfWeek(pToken, ulTokenSize);

    /* date1 */
    pToken  = pNext + 1;
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseDate1(hDate, pToken, pLast - pToken + 1);

    if (!pToken)
    {
        return FALSE;
    }

    /* time */
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseTime(hDate, pToken, pLast - pToken + 1);

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRfc850Date
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse RFC 850 date.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date string.

                ULONG                       ulSize
                The size of date string buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRfc850Date
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    BOOL                            bSucc   = TRUE;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* rfc850-date = weekday "," SP date2 SP time SP "GMT" */

    /* weekday */
    pToken  = pBuf;
    pNext       = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);
    if (!pNext)
    {
        return FALSE;
    }

    ulTokenSize = pNext - pToken;

    pDate->DayOfWeek    = HttpSmpoUtilGetDayOfWeek(pToken, ulTokenSize);

    /* date2 */
    pToken  = pNext + 1;
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseDate2(hDate, pToken, pLast - pToken + 1);

    if (!pToken)
    {
        return FALSE;
    }

    /* time */
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseTime(hDate, pToken, pLast - pToken + 1);

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseAsciiTimeDate
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse ASCII date.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date string.

                ULONG                       ulSize
                The size of date string buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAsciiTimeDate
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    BOOL                            bSucc   = TRUE;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* asctime-date = wkday SP date3 SP time SP 4DIGIT */

    /* wkday */
    pToken  = pBuf;
    pNext       = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

    if (!pNext)
    {
        return FALSE;
    }

    ulTokenSize = pNext - pToken;

    pDate->DayOfWeek    = HttpSmpoUtilGetDayOfWeek(pToken, ulTokenSize);

    /* date3 */
    pToken  = pNext + 1;
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseDate3(hDate, pToken, pLast - pToken + 1);

    if (!pToken)
    {
        return FALSE;
    }

    /* time */
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pToken  = HttpSmpoUtilParseTime(hDate, pToken, pLast - pToken + 1);

    /* year */
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
    pDate->Year = _ansc_atoi((const char *)pToken);

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        USHORT
        HttpSmpoUtilGetDayOfWeek
            (
                PUCHAR                      pDayOfWeek,
                ULONG                       ulSize
            )

    description:

        This function is called to get day of week.

    argument:   PUCHAR                      pDayOfWeek
                The name of day of week.

                ULONG                       ulSize
                The name size.
    
    return:     the code of day of week.

**********************************************************************/

USHORT
HttpSmpoUtilGetDayOfWeek
    (
        PUCHAR                      pDayOfWeek,
        ULONG                       ulSize
    )
{
    USHORT                          usDayOfWeek = 0;

    if (ulSize == 3)
    {
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_MON, ulSize, FALSE))
            usDayOfWeek = 1;
        else
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_TUE, ulSize, FALSE))
            usDayOfWeek = 2;
        else
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_WED, ulSize, FALSE))
            usDayOfWeek = 3;
        else
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_THU, ulSize, FALSE))
            usDayOfWeek = 4;
        else
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_FRI, ulSize, FALSE))
            usDayOfWeek = 5;
        else
        if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WKDAY_SAT, ulSize, FALSE))
            usDayOfWeek = 6;
    }
    else
    {
        switch (ulSize)
        {
            case    6:

                if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WEEKDAY_MONDAY, ulSize, FALSE))
                    usDayOfWeek = 1;
                else
                if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WEEKDAY_FRIDAY, ulSize, FALSE))
                    usDayOfWeek = 5;

                break;

            case    7:

                usDayOfWeek = 2;

                break;

            case    8:

                if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WEEKDAY_THURSDAY, ulSize, FALSE))
                    usDayOfWeek = 4;
                else
                if (AnscEqualString2((char *)pDayOfWeek, HTTP_SMPO_WEEKDAY_SATURDAY, ulSize, FALSE))
                    usDayOfWeek = 6;

                break;

            case    9:

                usDayOfWeek = 3;

                break;
        }
    }

    return usDayOfWeek;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilGetWeekDayName
            (
                USHORT                      usWkDay
            )

    description:

        This function is called to parse week day name.

    argument:   USHORT                      usWkDay
                The code of week of day.

    return:     the name of the given week day code.

**********************************************************************/

PUCHAR
HttpSmpoUtilGetWeekDayName
    (
        USHORT                      usWkDay
    )
{
    char*                          pSrc    = HTTP_SMPO_WKDAY_SUN;

    switch (usWkDay)
    {
    case 1:
        pSrc    = HTTP_SMPO_WKDAY_MON;
        break;

    case 2:
        pSrc    = HTTP_SMPO_WKDAY_TUE;
        break;

    case 3:
        pSrc    = HTTP_SMPO_WKDAY_WED;
        break;

    case 4:
        pSrc    = HTTP_SMPO_WKDAY_THU;
        break;

    case 5:
        pSrc    = HTTP_SMPO_WKDAY_FRI;
        break;

    case 6:
        pSrc    = HTTP_SMPO_WKDAY_SAT;
        break;
    }

    return (PUCHAR)pSrc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        USHORT
        HttpSmpoUtilGetMonth
            (
                PUCHAR                      pMonth,
                ULONG                       ulSize
            )

    description:

        This function is called to get month number by name.

    argument:   PUCHAR                      pMonth
                Method name.

                ULONG                       ulSize
                The length of the month name.
    
    return:     the month number.

**********************************************************************/

USHORT
HttpSmpoUtilGetMonth
    (
        PUCHAR                      pMonth,
        ULONG                       ulSize
    )
{
    USHORT                          usMonth = 1;

    if (ulSize == 3)
    {
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_FEB, ulSize, FALSE))
            usMonth = 2;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_MAR, ulSize, FALSE))
            usMonth = 3;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_APR, ulSize, FALSE))
            usMonth = 4;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_MAY, ulSize, FALSE))
            usMonth = 5;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_JUN, ulSize, FALSE))
            usMonth = 6;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_JUL, ulSize, FALSE))
            usMonth = 7;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_AUG, ulSize, FALSE))
            usMonth = 8;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_SEP, ulSize, FALSE))
            usMonth = 9;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_OCT, ulSize, FALSE))
            usMonth = 10;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_NOV, ulSize, FALSE))
            usMonth = 11;
        else
        if (AnscEqualString2((char *)pMonth, HTTP_SMPO_MONTH_DEC, ulSize, FALSE))
            usMonth = 12;
    }

    return usMonth;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilGetMonthName
            (
                USHORT                      usMonth
            )

    description:

        This function is called to get month name by number.

    argument:   USHORT                      usMonth
                Month name.

    return:     month name.

**********************************************************************/

PUCHAR
HttpSmpoUtilGetMonthName
    (
        USHORT                      usMonth
    )
{
    char*                         pSrc    = HTTP_SMPO_MONTH_JAN;

    switch (usMonth)
    {
    case 2:
        pSrc    = HTTP_SMPO_MONTH_FEB;
        break;

    case 3:
        pSrc    = HTTP_SMPO_MONTH_MAR;
        break;

    case 4:
        pSrc    = HTTP_SMPO_MONTH_APR;
        break;

    case 5:
        pSrc    = HTTP_SMPO_MONTH_MAY;
        break;

    case 6:
        pSrc    = HTTP_SMPO_MONTH_JUN;
        break;

    case 7:
        pSrc    = HTTP_SMPO_MONTH_JUL;
        break;
            
    case 8:
        pSrc    = HTTP_SMPO_MONTH_AUG;
        break;

    case 9:
        pSrc    = HTTP_SMPO_MONTH_SEP;
        break;

    case 10:
        pSrc    = HTTP_SMPO_MONTH_OCT;
        break;

    case 11:
        pSrc    = HTTP_SMPO_MONTH_NOV;
        break;

    case 12:
        pSrc    = HTTP_SMPO_MONTH_DEC;
        break;
    }

    return (PUCHAR)pSrc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilParseDate1
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse date1.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.
        
                PUCHAR                      pBuf
                Buffer of date1 string.

                ULONG                       ulSize
                The length of the buffer.
    
    return:     the next character after date1 string.

**********************************************************************/

PUCHAR
HttpSmpoUtilParseDate1
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* date1 = 2DIGIT SP month SP 4DIGIT  ; day month year (e.g., 02 Jun 1982) */
    
    pToken  = pBuf;
    pDate->DayOfMonth   = _ansc_atoi((const char *)pToken);

    pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, pLast - pToken + 1);

    if (!pToken)
    {
        return NULL;
    }

    pToken ++;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, pLast - pToken + 1);
    if (!pNext)
    {
        return NULL;
    }

    ulTokenSize = pNext - pToken;
    pDate->Month    = HttpSmpoUtilGetMonth(pToken, ulTokenSize);

    pToken  = pNext + 1;
    pDate->Year = _ansc_atoi((const char *)pToken);
    
    pNext   = pToken;

    while (pNext <= pLast)
    {
        if (*pNext < '0' || *pNext > '9')
        {
            break;
        }

        pNext ++;
    }

    return (pNext > pLast)?NULL:pNext;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilParseDate2
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse date2.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date2 string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the next character after date2 string.

**********************************************************************/

PUCHAR
HttpSmpoUtilParseDate2
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* date2 = 2DIGIT "-" month "-" 2DIGIT  ; day-month-year (e.g., 02-Jun-82) */
    
    pToken  = pBuf;
    pDate->DayOfMonth   = _ansc_atoi((const char *)pToken);

    pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_HYPHEN, pLast - pToken + 1);

    if (!pToken)
    {
        return NULL;
    }

    pToken ++;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_HYPHEN, pLast - pToken + 1);
    if (!pNext)
    {
        return NULL;
    }

    ulTokenSize = pNext - pToken;
    pDate->Month    = HttpSmpoUtilGetMonth(pToken, ulTokenSize);

    pToken  = pNext + 1;
    pDate->Year = _ansc_atoi((const char *)pToken);

    pNext   = pToken;

    while (pNext <= pLast)
    {
        if (*pNext < '0' || *pNext > '9')
        {
            break;
        }

        pNext ++;
    }

    return (pNext > pLast)?NULL:pNext;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilParseDate3
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse date3.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of date3 string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the next character after date3 string.

**********************************************************************/

PUCHAR
HttpSmpoUtilParseDate3
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

    /* date3 = month SP ( 2DIGIT | ( SP 1DIGIT ) )  ; month day (e.g., Jun 2) */
    
    pToken  = pBuf;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, pLast - pToken + 1);

    if (!pNext)
    {
        return NULL;
    }

    ulTokenSize = pNext - pToken;
    pDate->Month    = HttpSmpoUtilGetMonth(pToken, ulTokenSize);

    pToken  = pNext + 1;
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

    pDate->DayOfMonth   = _ansc_atoi((const char *)pToken);

    pNext   = pToken;

    while (pNext <= pLast)
    {
        if (*pNext < '0' || *pNext > '9')
        {
            break;
        }

        pNext ++;
    }

    return (pNext > pLast)?NULL:pNext;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilParseTime
            (
                ANSC_HANDLE                 hDate,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse HTTP time.

    argument:   ANSC_HANDLE                 hDate
                HTTP date object.

                PUCHAR                      pBuf
                Buffer of time string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the next character after time string.

**********************************************************************/

PUCHAR
HttpSmpoUtilParseTime
    (
        ANSC_HANDLE                 hDate,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_DATE                      pDate   = (PHTTP_DATE)hDate;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;

    /* time = 2DIGIT ":" 2DIGIT ":" 2DIGIT  ; 00:00:00 - 23:59:59 */
    
    pToken  = pBuf;
    pDate->Hour     = _ansc_atoi((const char *)pToken);

    pToken  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);

    if (!pToken)
    {
        return FALSE;
    }

    pToken ++;
    pDate->Minute   = _ansc_atoi((const char *)pToken);

    pToken   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);
    if (!pToken)
    {
        return FALSE;
    }

    pToken ++;
    pDate->Second   = _ansc_atoi((const char *)pToken);

    pDate->MilliSecond  = 0;

    pNext   = pToken;

    while (pNext <= pLast)
    {
        if (*pNext < '0' || *pNext > '9')
        {
            break;
        }

        pNext ++;
    }

    return (pNext > pLast)?NULL:pNext;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseMediaRange
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hMediaRange,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse media range.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hMediaRange
                Handle to media range object.

                PUCHAR                      pBuf
                Buffer of the media range string to be parsed.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseMediaRange
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMediaRange,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_MEDIA_RANGE               pMediaRange = (PHTTP_MEDIA_RANGE)hMediaRange;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;

#if 0   /* media range definition */
    media-range = ( "*/*" | ( type "/" "*" ) | ( type "/" subtype ) )  *( ";" parameter ) 
#endif

    /* type */
    pToken  = pBuf;
    pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SLASH, pLast - pToken + 1);

    if (!pNext)
    {
        return FALSE;
    }

    ulTokenSize = pNext - pToken;

    pMediaRange->MediaType  = HttpSmpoUtilGetMediaType(pToken, ulTokenSize);

    /* subtype */
    pToken  = pNext + 1;
    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, pLast - pToken + 1);

    if (pNext)
    {
        ulTokenSize = pNext - pToken;
    }
    else
    {
        ulTokenSize = pLast - pToken + 1;
    }

    pMediaRange->SubType    = 
        HttpSmpoUtilGetMediaSubType
            (
                hHttpSmpo, 
                pMediaRange->MediaType, 
                pToken, 
                ulTokenSize
            );

    pMediaRange->Quality    = 1000;

    /* parameters */
    if (pNext)
    {
        pToken      = pNext + 1;
        ulTokenSize = pLast - pNext + 1;

        HttpSmpoUtilCopyHeaderString(pToken, ulTokenSize, pMediaRange->Parameters, HTTP_MAX_HEADER_PARAM_SIZE);

        pNext   = (PUCHAR)_ansc_strstr(pMediaRange->Parameters, HTTP_SMPO_STRING_QUALITY);

        if (pNext)
        {
            pNext   += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);

            pMediaRange->Quality    = HttpSmpoUtilGetQuality(pNext, pLast - pNext + 1);
        }
    }

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetMediaType
            (
                PUCHAR                      pType,
                ULONG                       ulTypeSize
            )

    description:

        This function is called to get media type by name.

    argument:   PUCHAR                      pType
                Buffer of media type string.

                ULONG                       ulTypeSize
                The length of buffer.
    
    return:     media type.

**********************************************************************/

ULONG
HttpSmpoUtilGetMediaType
    (
        PUCHAR                      pType,
        ULONG                       ulTypeSize
    )
{
    ULONG                       ulMediaType;

    ulMediaType = IANA_MEDIA_TYPE_CODE_RESERVED;

    if (ulTypeSize == 1 && *pType == HTTP_SMPO_CHAR_ASTERISK)
    {
        return IANA_MEDIA_TYPE_CODE_STAR;
    }

    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT) == ulTypeSize && 
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_TEXT, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_TEXT;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_MULTIPART, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MULTIPART;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_MESSAGE, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MESSAGE;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_APPLICATION, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_APPLICATION;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_IMAGE, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_IMAGE;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_AUDIO, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_AUDIO;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_VIDEO, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_VIDEO;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MODEL) == ulTypeSize &&
        AnscEqualString2((char *)pType, IANA_MEDIA_TYPE_TEXT_MODEL, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MODEL;

    return ulMediaType;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilParseMediaSubType
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pSubType,
                ULONG                       ulSubTypeSize
            )

    description:

        This function is called to get media subtype by name.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pSubType
                Buffer of media subtype string.

                ULONG                       ulSubTypeSize
                The length of buffer.
    
    return:     the media subtype.

**********************************************************************/

ULONG
HttpSmpoUtilGetMediaSubType
    (
        ANSC_HANDLE                 hHttpSmpo,
        ULONG                       ulMediaType,
        PUCHAR                      pSubType,
        ULONG                       ulSubTypeSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    return 
        AnscGetIanaMediaSubtypeByName2
            (
                ulMediaType,
                pSubType,
                ulSubTypeSize
            );
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetQuality
            (
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to get quality value.

    argument:   PUCHAR                      pBuf
                Buffer of quality string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the quality value.

**********************************************************************/

ULONG
HttpSmpoUtilGetQuality
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pToken, pNext;
    ULONG                           ulInt, ulFrac;

    ulInt   = ulFrac    = 0;

    pToken  = pBuf;
    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_DOT, ulSize);

    ulInt   = _ansc_atoi((const char *)pToken);

    if (pNext)
    {
        ulFrac  = _ansc_atoi((const char *)pNext + 1);

        if (ulFrac <= 9)
        {
            ulFrac  *= 100;
        }
        else
        if (ulFrac <= 99)
        {
            ulFrac  *= 10;
        }
    }

    return (ulInt * 1000 + ulFrac);
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetCharsetType
            (
                PUCHAR                      pCharsetName,
                ULONG                       ulSize
            )

    description:

        This function is called to get charset type.

    argument:   PUCHAR                      pCharsetName
                Buffer of charset name.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the charset code.

**********************************************************************/

ULONG
HttpSmpoUtilGetCharsetType
    (
        PUCHAR                      pCharsetName,
        ULONG                       ulSize
    )
{
    ULONG                           ulType  = IANA_CHARSET_CODE_RESERVED;

    if (AnscSizeOfString(IANA_CHARSET_TEXT_US_ASCII) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_US_ASCII, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_US_ASCII;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_1) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_1, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_1;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_2) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_2, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_2;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_3) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_3, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_3;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_4) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_4, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_4;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_5) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_5, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_5;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_6) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_6, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_6;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_7) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_7, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_7;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_8) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_8, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_8;
    else
    if (AnscSizeOfString(IANA_CHARSET_TEXT_ISO_8859_9) == ulSize &&
        AnscEqualString2((char *)pCharsetName, IANA_CHARSET_TEXT_ISO_8859_9, ulSize, FALSE))
        ulType  = IANA_CHARSET_CODE_ISO_8859_9;

    return ulType;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetEncodingType
            (
                PUCHAR                      pEncType,
                ULONG                       ulSize
            )

    description:

        This function is called to get encoding type.

    argument:   PUCHAR                      pEncType
                Buffer of encoding type string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the code of encoding type.

**********************************************************************/

ULONG
HttpSmpoUtilGetEncodingType
    (
        PUCHAR                      pEncType,
        ULONG                       ulSize
    )
{
    ULONG                           ulType  = IANA_CCODING_CODE_RESERVED;

    if (AnscSizeOfString(IANA_CCODING_TEXT_GZIP) == ulSize &&
        AnscEqualString2((char *)pEncType, IANA_CCODING_TEXT_GZIP, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_GZIP;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS) == ulSize && 
        AnscEqualString2((char *)pEncType, IANA_CCODING_TEXT_COMPRESS, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_COMPRESS;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE) == ulSize &&
        AnscEqualString2((char *)pEncType, IANA_CCODING_TEXT_DEFLATE, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_DEFLATE;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY) == ulSize &&
        AnscEqualString2((char *)pEncType, IANA_CCODING_TEXT_IDENTITY, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_IDENTITY;

    return ulType;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetMethodId
            (
                PUCHAR                      pMethod,
                ULONG                       ulSize
            )

    description:

        This function is called to HTTP method code.

    argument:   PUCHAR                      pMethod
                The name of HTTP method.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the method code.

**********************************************************************/

ULONG
HttpSmpoUtilGetMethodId
    (
        PUCHAR                      pMethod,
        ULONG                       ulSize
    )
{
    ULONG                           ulMethod    = HTTP_METHOD_RESERVED;

    if (AnscSizeOfString(HTTP_METHOD_NAME_GET) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_GET, ulSize, FALSE))
        ulMethod = HTTP_METHOD_GET;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_POST) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_POST, ulSize, FALSE))
        ulMethod = HTTP_METHOD_POST;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_OPTIONS, ulSize, FALSE))
        ulMethod = HTTP_METHOD_OPTIONS;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_HEAD) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_HEAD, ulSize, FALSE))
        ulMethod = HTTP_METHOD_HEAD;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_PUT) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_PUT, ulSize, FALSE))
        ulMethod = HTTP_METHOD_PUT;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_DELETE) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_DELETE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_DELETE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_CONNECT) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_CONNECT, ulSize, FALSE))
        ulMethod = HTTP_METHOD_CONNECT;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_TRACE) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_TRACE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_TRACE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_NOTIFY, ulSize, FALSE))
        ulMethod = HTTP_METHOD_NOTIFY;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_SEARCH) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_SEARCH, ulSize, FALSE))
        ulMethod = HTTP_METHOD_SEARCH;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_M_SEARCH, ulSize, FALSE))
        ulMethod = HTTP_METHOD_M_SEARCH;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_M_POST) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_M_POST, ulSize, FALSE))
        ulMethod = HTTP_METHOD_M_POST;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_SUBSCRIBE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_SUBSCRIBE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE) == ulSize &&
        AnscEqualString2((char *)pMethod, HTTP_METHOD_NAME_UNSUBSCRIBE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_UNSUBSCRIBE;

    return ulMethod;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseCredentials
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hCredentials,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse credentials.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hCredentials
                Handle to credentials object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* credentials = basic-credentials | auth-scheme #auth-param */
    /* basic-credentials = "Basic" SP basic-cookie */
    /* auth-scheme = token */
    /* auth-param = token "=" quoted-string */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PUCHAR                          pToken          = pBuf;
    PUCHAR                          pLast           = pBuf + ulSize - 1;
    PUCHAR                          pNext;
    ULONG                           ulTokenSize;
    BOOL                            bSucc           = TRUE;

    /* scheme */
    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, pLast - pToken + 1);

    if (!pNext)
    {
        return FALSE;
    }

    ulTokenSize = pNext - pToken;

    pNext ++;

    if (AnscSizeOfString(HTTP_AUTH_TEXT_NONE) == ulTokenSize &&
        AnscEqualString2((char *)pToken, HTTP_AUTH_TEXT_NONE, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_NONE;
    }
    else
    if (AnscSizeOfString(HTTP_AUTH_TEXT_BASIC) == ulTokenSize &&
        AnscEqualString2((char *)pToken, HTTP_AUTH_TEXT_BASIC, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_BASIC;

        HttpSmpoUtilParseBasicCredentials(hHttpSmpo, hCredentials, pNext, pLast - pNext + 1);
    }
    else
    if (AnscSizeOfString(HTTP_AUTH_TEXT_DIGEST) == ulTokenSize &&
        AnscEqualString2((char *)pToken, HTTP_AUTH_TEXT_DIGEST, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_DIGEST;

        HttpSmpoUtilParseDigestCredentials(hHttpSmpo, hCredentials, pNext, pLast - pNext + 1);
    }
    else
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_RESERVED;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseBasicCredentials
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hCredentials,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse basic credentials.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hCredentials
                Handle to credentials object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseBasicCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* basic-credentials = "Basic" SP basic-cookie */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PHTTP_CREDENTIAL_BASIC          pBasicCookie    = &pCredentials->Credential.Basic;
    PUCHAR                          pDecodedString;
    PUCHAR                          pString         = NULL;
    ULONG                           ulDecodedStringLen;

    pString = (PUCHAR)AnscAllocateMemory(ulSize + 1);
    
    if (!pString)
    {
        return FALSE;
    }

    AnscCopyMemory(pString, pBuf, ulSize);
    pString[ulSize] = 0;

    pDecodedString  = AnscBase64Decode((PUCHAR)pString, &ulDecodedStringLen);

    if (pDecodedString)
    {
        PUCHAR                      pPos;
        ULONG                       ulCopySize;

        /* parse the decoded string to get user & password */
        pPos = _ansc_memchr(pDecodedString, HTTP_SMPO_CHAR_COLON, ulDecodedStringLen);

        if (!pPos)
        {
            ulCopySize  = ulDecodedStringLen;
            if (ulCopySize >= HTTP_MAX_USERNAME_SIZE)
                ulCopySize  = HTTP_MAX_USERNAME_SIZE - 1;

            AnscCopyMemory(pBasicCookie->UserName, pDecodedString, ulCopySize);
            pBasicCookie->UserName[ulCopySize]  = 0;
        }
        else
        {
            PUCHAR                  pUser, pPass;
            ULONG                   ulUser, ulPass;

            pPass   = pPos + 1;
            *pPos   = 0;
            pUser   = pDecodedString;

            ulUser  = AnscSizeOfString((const char *)pUser);
            ulPass  = ulDecodedStringLen - ulUser - 1;

            ulCopySize  = ulUser;
            if (ulCopySize >= HTTP_MAX_USERNAME_SIZE)
                ulCopySize  = HTTP_MAX_USERNAME_SIZE - 1;

            AnscCopyMemory(pBasicCookie->UserName, pUser, ulCopySize);
            pBasicCookie->UserName[ulCopySize]  = 0;

            ulCopySize  = ulPass;
            if (ulCopySize >= HTTP_MAX_PASSWORD_SIZE)
                ulCopySize  = HTTP_MAX_PASSWORD_SIZE - 1;

            AnscCopyMemory(pBasicCookie->Password, pPass, ulCopySize);
            pBasicCookie->Password[ulCopySize]  = 0;
        }

        AnscFreeMemory(pDecodedString);
    }

    AnscFreeMemory(pString);

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseDigestCredentials
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hCredentials,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse digest credentials.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hCredentials
                Handle to credentials object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseDigestCredentials
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* auth-scheme = token */
    /* auth-param = token "=" quoted-string */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PHTTP_CREDENTIAL_DIGEST         pDigest         = &pCredentials->Credential.Digest;

    HttpSmpoUtilCopyHeaderString(pBuf, ulSize, pDigest->UserName, HTTP_MAX_USERNAME_SIZE);

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseMailBox
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hMailBox,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse mailbox.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hMailBox
                Handle to mailbox object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseMailBox
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMailBox,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* mailbox =  addr-spec / [phrase] route-addr */
    /* addr-spec   =  local-part "@" domain */
    /* route-addr  =  "<" [route] addr-spec ">" */
    /* route       =  1#("@" domain) ":" */
    /* local-part  =  word *("." word) */
    /* domain      =  sub-domain *("." sub-domain) */
    
    BOOL                            bSucc       = TRUE;
    PUCHAR                          pToken      = pBuf;
    PUCHAR                          pLast       = pToken + ulSize - 1;
    PUCHAR                          pNext;

    /* try addr-spec first */
    bSucc = HttpSmpoUtilParseAddrSpec(hHttpSmpo, hMailBox, pBuf, ulSize);

    if (!bSucc)
    {
        /* try [phrase] route-addr */

        pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_LESS, pLast - pToken + 1);

        if (!pNext)
        {
            return FALSE;
        }

        pToken  = pNext + 1;

        pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_LARGE, pLast - pToken + 1);

        if (!pNext)
        {
            return FALSE;
        }

        pLast   = pNext - 1;

        /* skip route */
        pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);

        if (pNext)
        {
            pToken  = pNext + 1;
        }

        bSucc = HttpSmpoUtilParseAddrSpec(hHttpSmpo, hMailBox, pToken, pLast - pToken + 1);
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseAddrSpec
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hMailBox,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse Addr-Spec.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hMailBox
                Handle to mailbox object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAddrSpec
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hMailBox,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* addr-spec   =  local-part "@" domain */
    /* local-part  =  word *("." word) */
    /* domain      =  sub-domain *("." sub-domain) */
    
    PANSC_RFC822_MAILBOX            pMailBox    = (PANSC_RFC822_MAILBOX)hMailBox;
    PUCHAR                          pToken      = pBuf;
    PUCHAR                          pLast       = pToken + ulSize - 1;
    PUCHAR                          pNext;
    ULONG                           ulLen;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_AT, ulSize);

    if (!pNext)
    {
        return FALSE;
    }

    ulLen   = pNext - pToken;
    HttpSmpoUtilCopyHeaderString(pToken, ulLen, pMailBox->LocalPart, ANSC_MAX_RFC822_LPART_SIZE);

    pToken  = pNext + 1;
    ulLen   = pLast - pToken + 1;
    HttpSmpoUtilCopyHeaderString(pToken, ulLen, pMailBox->DomainName, ANSC_MAX_RFC822_DNAME_SIZE);

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseChallenge
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hChallenge,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse challenge.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hChallenge
                Handle to challenge object.

                PUCHAR                      pBuf
                Buffer of credentials string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseChallenge
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hChallenge,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* challenge = auth-scheme 1*SP realm *( "," auth-param ) */

    PHTTP_AUTH_CHALLENGE            pChallenge  = (PHTTP_AUTH_CHALLENGE)hChallenge;
    PUCHAR                          pScheme     = pBuf;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    PUCHAR                          pRealm;
    ULONG                           ulLen;
    
    pRealm  = _ansc_memchr(pScheme, HTTP_SMPO_CHAR_SPACE, pLast - pScheme + 1);

    if (!pRealm)
    {
        return FALSE;
    }

    ulLen   = pRealm - pScheme;

    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_NONE) &&
        AnscEqualString2(HTTP_AUTH_TEXT_NONE, (char *)pScheme, ulLen, FALSE))
    {
        pChallenge->AuthType    = HTTP_AUTH_TYPE_NONE;
    }
    else
    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_BASIC) &&
        AnscEqualString2(HTTP_AUTH_TEXT_BASIC, (char *)pScheme, ulLen, FALSE))
    {
        PHTTP_CHALLENGE_BASIC       pBasicChallenge = &pChallenge->Challenge.Basic;

        pChallenge->AuthType    = HTTP_AUTH_TYPE_BASIC;

        pRealm  = HttpSmpoUtilLinearWhiteSpace(pRealm, pLast - pRealm + 1);
        HttpSmpoUtilCopyHeaderString(pRealm, pLast - pRealm + 1, pBasicChallenge->Realm, HTTP_MAX_REALM_SIZE);
    }
    else
    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_DIGEST) &&
        AnscEqualString2(HTTP_AUTH_TEXT_DIGEST, (char *)pScheme, ulLen, FALSE))
    {
        PHTTP_CHALLENGE_DIGEST      pDigestChallenge = &pChallenge->Challenge.Digest;

        pChallenge->AuthType    = HTTP_AUTH_TYPE_DIGEST;

        pRealm  = HttpSmpoUtilLinearWhiteSpace(pRealm, pLast - pRealm + 1);
        HttpSmpoUtilCopyHeaderString(pRealm, pLast - pRealm + 1, pDigestChallenge->Realm, HTTP_MAX_REALM_SIZE);
    }
    else
    {
        pChallenge->AuthType    = HTTP_AUTH_TYPE_RESERVED;
    }

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetTransferEncoding
            (
                PUCHAR                      pTEncoding,
                ULONG                       ulSize
            )

    description:

        This function is called to get transfer encoding by name.

    argument:   PUCHAR                      pTEncoding
                Buffer of transfer encoding name.

                ULONG                       ulSize
                The length of buffer.
    
    return:     the IANA code of transfer encoding.

**********************************************************************/

ULONG
HttpSmpoUtilGetTransferEncoding
    (
        PUCHAR                      pTEncoding,
        ULONG                       ulSize
    )
{
    ULONG                           ulEncoding  = IANA_TCODING_CODE_RESERVED;

    if (AnscSizeOfString(IANA_TCODING_TEXT_GZIP) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_GZIP, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_GZIP;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_COMPRESS) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_COMPRESS, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_COMPRESS;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_DEFLATE) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_DEFLATE, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_DEFLATE;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_IDENTITY) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_IDENTITY, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_IDENTITY;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_CHUNKED, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_CHUNKED;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS) == ulSize &&
        AnscEqualString2((char *)pTEncoding, IANA_TCODING_TEXT_TRAILERS, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_TRAILERS;

    return ulEncoding;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseAuthority
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse authority.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of authority string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAuthority
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    BOOL                            bSucc   = TRUE;

    bSucc   = HttpSmpoUtilParseAuthorityServer(hHttpSmpo, hUri, pBuf, ulSize);

    if (!bSucc)
    {
        bSucc   = HttpSmpoUtilParseRegName(hHttpSmpo, hUri, pBuf, ulSize);
    }

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_AUTHORITY;
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseAuthorityServer
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse authority server.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of authority server string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseAuthorityServer
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    /* authority-server = [ [ userinfo "@" ] hostport ] */
    /* hostport = host [ ":" port ] */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken  = pBuf;
    PUCHAR                          pNext;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_AT, pLast - pToken + 1);

    if (pNext)
    {
        /* userinfo is present */
        pToken  = pNext + 1;
    }

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);

    if (pNext)
    {
        /* port is specified */

        HttpSmpoUtilCopyHeaderString(pToken, pNext - pToken, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

        pUri->HostPort  = (USHORT)_ansc_atoi((const char *)pNext + 1);
    }
    else
    {
        HttpSmpoUtilCopyHeaderString(pToken, pLast - pToken + 1, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

        pUri->HostPort  = (USHORT)HTTP_SERVER_PORT;
    }

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoUtilParseRegName
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hUri,
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to parse Reg-Name.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hUri
                Handle to Uri object.

                PUCHAR                      pBuf
                Buffer of reg-name string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoUtilParseRegName
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken  = pBuf;

    HttpSmpoUtilCopyHeaderString(pToken, pLast - pToken + 1, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);
    pUri->HostPort  = (USHORT)HTTP_SERVER_PORT;

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilGetNextCookieNameValuePair
            (
                PUCHAR                      pBuf,
                ULONG                       ulSize
            )

    description:

        This function is called to get next name-value pair in
        cookie string.

    argument:   PUCHAR                      pBuf
                Buffer of cookie string.

                ULONG                       ulSize
                The length of buffer.
    
    return:     starting position to next name-value pair.

**********************************************************************/

PUCHAR
HttpSmpoUtilGetNextCookieNameValuePair
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* find the first cookie name-value pair separated by COMMA or SEMICOLON */
    PUCHAR                          pToken  = pBuf;
    PUCHAR                          pLast   = pToken + ulSize - 1;
    PUCHAR                          pNext   = NULL, pComma, pSemi;
    PUCHAR                          pValue;
    ULONG                           ulNameLen = 0;

    pSemi  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, pLast - pToken + 1);
    pComma = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

    pValue = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, pLast - pToken + 1);

    if ( !pValue || ( pSemi && pValue > pSemi ) || ( pComma && pValue > pComma ) )
    {
        pNext = pSemi;

        if ( pComma && !pSemi )
        {
            pNext = pComma;
        }
        else
        if ( pComma && pSemi && pComma < pSemi )
        {
            pNext = pComma;
        }
    }
    else
    {
        ulNameLen = pValue - pToken;

        pNext = pValue + 1;

        if ( pNext > pLast )
        {
            return NULL;
        }

        if ( *pNext == HTTP_SMPO_CHAR_DQUOTE )
        {
            pNext = _ansc_memchr(pNext + 1, HTTP_SMPO_CHAR_DQUOTE, pLast - pNext);

            if ( pNext )
            {
                if ( pNext + 1 < pLast ) 
                    pNext ++;
                else
                    pNext = NULL;
            }

            return pNext;
        }

        if ( AnscEqualString2((char *)pToken, HTTP_SMPO_STRING_SCOOKIE_EXPIRES, ulNameLen, FALSE) )
        {
            pNext = (PUCHAR)_ansc_strstr((const char *)pNext, "GMT");

            if ( pNext )
            {
                pNext = pNext + AnscSizeOfString("GMT");

                if ( pNext >= pLast ) pNext = NULL;
            }

            return pNext;
        }

        if ( pSemi && pComma )
        {
            pNext = ( pSemi < pComma ) ? pSemi : pComma;
        }
        else if ( pSemi )
        {
            pNext = pSemi;
        }
        else
        {
            pNext = pComma;
        }
    }

    return pNext;
}

