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

    module:	ansc_packet_binary_rtsp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Real Time Streaming Protocol (RTSP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        12/26/07    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_RTSP_
#define  _ANSC_PACKET_BINARY_RTSP_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif

#ifndef  _ANSC_STATIC_
#define  _ANSC_STATIC_                              static
#endif

#ifndef  _ANSC_INLINE_
    #if  defined(_MSC_VER)
        #define  _ANSC_INLINE_                      __inline
    #else
        #define  _ANSC_INLINE_                      __inline__
    #endif
#endif


/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */
#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
                DATA STRUCTURES USED BY RTSP
***********************************************************/

#define  APP_PORT_RTSP                              554
#define  APP_PORT_RTSP2                             8554

/*
    RTSP is a text-based protocol and uses the ISO 10646 character set in
    UTF-8 encoding (RFC 2279 [21]). Lines are terminated by CRLF, but
    receivers should be prepared to also interpret CR and LF by
    themselves as line terminators.

    RTSP messages can be carried over any lower-layer transport protocol
    that is 8-bit clean.

    1) Request

       Request      =       Request-Line          ; Section 6.1
                    *(      general-header        ; Section 5
                    |       request-header        ; Section 6.2
                    |       entity-header )       ; Section 8.1
                            CRLF
                            [ message-body ]      ; Section 4.3

      Request-Line = Method SP Request-URI SP RTSP-Version CRLF

       Method         =         "DESCRIBE"              ; Section 10.2
                      |         "ANNOUNCE"              ; Section 10.3
                      |         "GET_PARAMETER"         ; Section 10.8
                      |         "OPTIONS"               ; Section 10.1
                      |         "PAUSE"                 ; Section 10.6
                      |         "PLAY"                  ; Section 10.5
                      |         "RECORD"                ; Section 10.11
                      |         "REDIRECT"              ; Section 10.10
                      |         "SETUP"                 ; Section 10.4
                      |         "SET_PARAMETER"         ; Section 10.9
                      |         "TEARDOWN"              ; Section 10.7
                      |         extension-method

      extension-method = token

      Request-URI = "*" | absolute_URI

      RTSP-Version = "RTSP" "/" 1*DIGIT "." 1*DIGIT

    The methods which ALG cares about are:
        SETUP
        TEARDOWN

    SETUP request requires "Transport" request header:
       Transport           =    "Transport" ":"
                                1\#transport-spec
       transport-spec      =    transport-protocol/profile[/lower-transport]
                                *parameter
       transport-protocol  =    "RTP"
       profile             =    "AVP"
       lower-transport     =    "TCP" | "UDP"
       parameter           =    ( "unicast" | "multicast" )
                           |    ";" "destination" [ "=" address ]
                           |    ";" "interleaved" "=" channel [ "-" channel ]
                           |    ";" "append"
                           |    ";" "ttl" "=" ttl
                           |    ";" "layers" "=" 1*DIGIT
                           |    ";" "port" "=" port [ "-" port ]
                           |    ";" "client_port" "=" port [ "-" port ]
                           |    ";" "server_port" "=" port [ "-" port ]
                           |    ";" "ssrc" "=" ssrc
                           |    ";" "mode" = <"> 1\#mode <">
       ttl                 =    1*3(DIGIT)
       port                =    1*5(DIGIT)
       ssrc                =    8*8(HEX)
       channel             =    1*3(DIGIT)
       address             =    host
       mode                =    <"> *Method <"> | Method

    2) Response

         Response    =     Status-Line         ; Section 7.1
                     *(    general-header      ; Section 5
                     |     response-header     ; Section 7.1.2
                     |     entity-header )     ; Section 8.1
                           CRLF
                           [ message-body ]    ; Section 4.3

         Status-Line =   RTSP-Version SP Status-Code SP Reason-Phrase CRLF

    Session ID from the "session:" field of response of "SETUP" request is carried
    in the later packets.
 */
#define  RTSP_CARRIAGE_RETURN                       0x0D
#define  RTSP_LINE_FEED                             0x0A
#define  RTSP_COLON                                 ':'
#define  RTSP_HORIZONTAL_TAB                        0x09
#define  RTSP_SPACE                                 0x20
#define  RTSP_DOUBLE_QUOTE                          0x22
#define  RTSP_SEMI_COLON                            ';'
#define  RTSP_HYPHEN                                '-'

#define  RTSP_REQUEST_SETUP                         1
#define  RTSP_REQUEST_TEARDOWN                      2
#define  RTSP_HEADER_CSEQ                           3
#define  RTSP_HEADER_SESSION                        4
#define  RTSP_HEADER_TRANSPORT                      5

#define  RTSP_REQUEST_STR_SETUP                     "SETUP "
#define  RTSP_REQUEST_STR_TEARDOWN                  "TEARDOWN "
#define  RTSP_VERSION_STR                           "RTSP/"
#define  RTSP_HEADER_CSEQ_STR                       "CSeq: "
#define  RTSP_HEADER_SESSION_STR                    "Session: "
#define  RTSP_HEADER_TRANSPORT_STR                  "Transport: "

#define  RTSP_SESSION_STATE_INIT                    0
#define  RTSP_SESSION_STATE_DESCRIBE                1
#define  RTSP_SESSION_STATE_SETUP                   2
#define  RTSP_SESSION_STATE_SETUP_DONE              3
#define  RTSP_SESSION_STATE_ACTION                  4
#define  RTSP_SESSION_STATE_TEARDOWN                5

#define  RTSP_TRANSPORT_STR_unicast                 "unicast;"
#define  RTSP_TRANSPORT_STR_multicast               "multicast;"
#define  RTSP_TRANSPORT_STR_client_port             "client_port="
#define  RTSP_TRANSPORT_STR_server_port             "server_port="
#define  RTSP_TRANSPORT_STR_source                  "source="
#define  RTSP_TRANSPORT_STR_destination             "destination="

#define  RTSP_TRANSPORT_FIELD_unicast               1
#define  RTSP_TRANSPORT_FIELD_multicast             2
#define  RTSP_TRANSPORT_FIELD_client_port           3
#define  RTSP_TRANSPORT_FIELD_server_port           4
#define  RTSP_TRANSPORT_FIELD_source                5
#define  RTSP_TRANSPORT_FIELD_destination           6


#define  AnscRtspIsLf(ucChar)                       (ucChar == RTSP_LINE_FEED)
#define  AnscRtspIsCr(ucChar)                       (ucChar == RTSP_CARRIAGE_RETURN)

#define  AnscRtspIsEol(pCurChar)                                                        \
         ( AnscRtspIsLf(*pCurChar)                                                      \
           || ( AnscRtspIsCr(*pCurChar)                                                 \
                && (*(pCurChar + 1) != 0) && AnscRtspIsLf(*(pCurChar + 1)) ) )


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspCheckRequestType
    (
        char*                       pBuf,
        ULONG                       ulBufSize,
        char**                      pReqList,
        ULONG                       ulNumReq,
        PBOOLEAN                    pbRequest
    )
{
    char*                           pCurChar    = pBuf;
    ULONG                           ulIndex     = 0;
    ULONG                           ulReqStrLen = 0;

    /* skip whitespaces */
    while ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar == RTSP_SPACE) )
    {
        pCurChar++;
    }

    if ( AnscRtspIsEol(pCurChar) )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    if ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar != 0) )
    {
        if ( _ansc_strncmp(pCurChar, RTSP_VERSION_STR, _ansc_strlen(RTSP_VERSION_STR)) == 0 )
        {
            /* This is a response */
            *pbRequest = FALSE;

            return  ANSC_STATUS_SUCCESS;
        }

        for ( ulIndex = 0; ulIndex < ulNumReq; ulIndex++ )
        {
            ulReqStrLen = _ansc_strlen(pReqList[ulIndex]);

            if ( _ansc_strncmp(pCurChar, pReqList[ulIndex], ulReqStrLen) ==  0 )
            {
                /* This is a request */
                *pbRequest = TRUE;

                return  ANSC_STATUS_SUCCESS;
            }
        }
    }

    return  ANSC_STATUS_CANT_FIND;
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspGetRequest
    (
        char*                       pBuf,
        ULONG                       ulBufSize,
        char**                      pReqList,
        ULONG                       ulNumReq,
        PBOOLEAN                    pbRequest,
        PUCHAR                      pucReq,
        char**                      ppReqUrl
    )
{
    char*                           pCurChar    = pBuf;
    ULONG                           ulIndex     = 0;
    ULONG                           ulReqStrLen = 0;
    char*                           pNextChar   = NULL;

    /* skip whitespaces */
    while ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar == RTSP_SPACE) )
    {
        pCurChar++;
    }

    if ( AnscRtspIsEol(pCurChar) )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    if ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar != 0) )
    {
        if ( _ansc_strncmp(pCurChar, RTSP_VERSION_STR, _ansc_strlen(RTSP_VERSION_STR)) == 0 )
        {
            /* This is a response */
            *pbRequest = FALSE;

            return  ANSC_STATUS_SUCCESS;
        }

        for ( ulIndex = 0; ulIndex < ulNumReq; ulIndex++ )
        {
            ulReqStrLen = _ansc_strlen(pReqList[ulIndex]);

            if ( _ansc_strncmp(pCurChar, pReqList[ulIndex], ulReqStrLen) ==  0 )
            {
                /* This is a request */
                *pbRequest = TRUE;

                if ( AnscEqualString(pReqList[ulIndex], RTSP_REQUEST_STR_SETUP, FALSE) )
                {
                    *pucReq = RTSP_REQUEST_SETUP;
                }
                else if ( AnscEqualString(pReqList[ulIndex], RTSP_REQUEST_STR_TEARDOWN, FALSE) )
                {
                    *pucReq = RTSP_REQUEST_TEARDOWN;
                }

                /*
                 *  retrieve the request URL
                 */
                pCurChar += ulReqStrLen;

                if ( *pCurChar != 0 )
                {
                    pNextChar = _ansc_strchr(pCurChar, RTSP_SPACE);

                    if ( !pNextChar )
                    {
                        *ppReqUrl = NULL;
                    }
                    else
                    {
                        *ppReqUrl = AnscAllocateMemory((pNextChar - pCurChar) + 1);

                        if ( *ppReqUrl )
                        {
                            _ansc_strncpy(*ppReqUrl, pCurChar, (pNextChar - pCurChar));
                        }
                        else
                        {
                            AnscTraceWarning(("AnscRtspCheckRequestType -- failed to allocate memory for RTSP URL!\n"));
                        }
                    }
                }

                return  ANSC_STATUS_SUCCESS;
            }
        }
    }

    return  ANSC_STATUS_CANT_FIND;
}


/*
 *  Find the next header by checking the next 'CRLF' or the end of input buffer
 */
_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspGetNextHeader
    (
        char*                       pBuf,
        ULONG                       ulBufSize,
        char**                      ppNextHeader
    )
{
    char*                           pCurChar    = pBuf;

    while ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar != 0) )
    {
        if ( AnscRtspIsCr(*pCurChar) )
        {
            pCurChar++;

            if ( *pCurChar != 0 )
            {
                if ( AnscRtspIsLf(*pCurChar) )
                {
                    *ppNextHeader = pCurChar + 1;
                }
                else
                {
                    *ppNextHeader = pCurChar;
                }

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                return  ANSC_STATUS_CANT_FIND;
            }
        }
        else if ( AnscRtspIsLf(*pCurChar) )
        {
            *ppNextHeader = pCurChar + 1;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            pCurChar++;
        }
    }

    return  ANSC_STATUS_CANT_FIND;
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspGetHeaderValue
    (
        char*                       pBuf,
        ULONG                       ulBufSize,
        BOOLEAN                     bStringValue,
        PVOID                       pValue,
        ULONG                       ulValueSize
    )
{
    char*                           pCurChar    = pBuf;
    char*                           pNextChar   = pBuf;
    char                            TempStr[16] = {0};

    while ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar == RTSP_SPACE) )
    {
        pCurChar++;
    }

    if ( AnscRtspIsEol(pCurChar) )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    pNextChar = pCurChar;

    while ( ((ULONG)(pNextChar - pBuf) < ulBufSize) && (*pNextChar != 0) && !(AnscRtspIsEol(pNextChar)) )
    {
        if ( *pNextChar == RTSP_SPACE )
        {
            break;
        }

        pNextChar++;
    }

    if ( pNextChar == pCurChar )
    {
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        if ( (pNextChar - pCurChar) < sizeof(TempStr) )
        {
            if ( bStringValue )
            {
                if ( (ULONG)(pNextChar - pCurChar) < ulValueSize )
                {
                    _ansc_strncpy(pValue, pCurChar, (pNextChar - pCurChar));
                }
                else
                {
                    AnscTraceWarning
                        ((
                            "AnscRtspGetHeaderValue -- the value is too large %d for the buffer %d!\n",
                            (ULONG)(pNextChar - pCurChar),
                            ulValueSize
                        ));

                    return  ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                _ansc_strncpy(TempStr, pCurChar, (pNextChar - pCurChar));

                *(PULONG)pValue = AnscGetStringUlong(TempStr);
            }

            AnscTraceFlow(("AnscRtspGetHeaderValue -- the final value is %s!\n", bStringValue ? pValue : TempStr));

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            AnscTraceWarning(("AnscRtspGetHeaderValue -- failed to find the stop character!\n"));

            return  ANSC_STATUS_FAILURE;
        }
    }
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspFetchRequestHeaderValue
    (
        char*                       pCurData,
        ULONG                       ulBufSize,
        char*                       pRequestStr,
        BOOLEAN                     bStringValue,
        PVOID                       pValue,
        ULONG                       ulValueSize,
        char**                      ppNextHeader
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    char*                           pOrigData       = pCurData;
    char*                           pNextHeader     = pCurData;
    ULONG                           ulLength        = 0;

    if ( !pCurData )
    {
        AnscTraceWarning(("AnscRtspFetchRequestHeaderValue -- NULL pCurData!\n"));

        return  ANSC_STATUS_CANT_FIND;
    }

    do
    {
        returnStatus = AnscRtspGetNextHeader(pCurData, (ulBufSize - (pCurData - pOrigData)), &pNextHeader);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTraceWarning(("AnscRtspFetchRequestHeaderValue -- failed to get next header for the request!\n"));

            return  returnStatus;
        }

        pCurData = pNextHeader;
        ulLength = _ansc_strlen(pRequestStr);

        if ( ulLength > (ulBufSize - (pCurData - pOrigData)) )
        {
            ulLength = ulBufSize - (pCurData - pOrigData);
        }

        if ( (ulLength != 0) && AnscEqualString2(pCurData, pRequestStr, ulLength, FALSE) )
        {
            pCurData      += _ansc_strlen(pRequestStr);

            AnscRtspGetNextHeader(pCurData, (ulBufSize - (pCurData - pOrigData)), ppNextHeader);

            return  AnscRtspGetHeaderValue(pCurData, (ulBufSize - (pCurData - pOrigData)), bStringValue, pValue, ulValueSize);
        }
    }
    while ( TRUE );

    return  ANSC_STATUS_CANT_FIND;
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspFindRequestHeader
    (
        char*                       pCurData,
        ULONG                       ulBufSize,
        char*                       pRequestStr,
        char**                      ppRequestData
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    char*                           pOrigData       = pCurData;
    char*                           pNextHeader     = pCurData;

    if ( !pCurData )
    {
        AnscTraceWarning(("AnscRtspFindRequestHeader -- NULL pCurData!\n"));

        return  ANSC_STATUS_CANT_FIND;
    }

    do
    {
        returnStatus = AnscRtspGetNextHeader(pCurData, (ulBufSize - (pCurData - pOrigData)), &pNextHeader);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTraceWarning(("AnscRtspFindRequestHeader -- failed to get next header for the request!\n"));

            return  returnStatus;
        }

        pCurData = pNextHeader;

        if ( _ansc_strncmp(pCurData, pRequestStr, _ansc_strlen(pRequestStr)) == 0 )
        {
            pCurData      += _ansc_strlen(pRequestStr);
            *ppRequestData = pCurData;

            return  ANSC_STATUS_SUCCESS;
        }
    }
    while ( TRUE );

    return  ANSC_STATUS_CANT_FIND;
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspReadTransportPortRange
    (
        char*                       pBuf,
        PULONG                      pulValue1,
        PULONG                      pulValue2,
        char**                      ppNextField
    )
{
    char*                           pCurChar    = pBuf;
    char*                           pNextChar   = pBuf;
    char                            TempStr[16] = {0};

    pNextChar       = _ansc_strchr(pCurChar, RTSP_HYPHEN);

    if ( !pNextChar )
    {
        AnscTraceWarning(("AnscRtspReadTransportPortRange -- failed to parse client_port, cannot find hyphen!\n"));
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        _ansc_strncpy(TempStr, pCurChar, (pNextChar - pCurChar));
        *pulValue1 = AnscGetStringUlong(TempStr);

        pCurChar = ++pNextChar;
    }
    
    while ( (*pNextChar != 0) && !(AnscRtspIsEol(pNextChar)) )
    {
        if ( *pNextChar == RTSP_SEMI_COLON )
        {
            break;
        }

        pNextChar++;
    }

    if ( pNextChar == pCurChar )
    {
        AnscTraceWarning(("AnscRtspReadTransportPortRange -- failed to parse client_port, cannot find the end!\n"));
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        if ( (pNextChar - pCurChar) < sizeof(TempStr) )
        {
            _ansc_strncpy(TempStr, pCurChar, (pNextChar - pCurChar));
            *pulValue2 = AnscGetStringUlong(TempStr);

            if ( *pNextChar == RTSP_SEMI_COLON )
            {
                pNextChar++;
            }
            /*Here, it can be ';' or Eol*/
            *ppNextField = pNextChar;

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            AnscTraceWarning(("AnscRtspReadTransportPortRange -- address string is too long!\n"));

            return  ANSC_STATUS_FAILURE;
        }
    }
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspReadTransportIp
    (
        char*                       pBuf,
        PULONG                      pulValue1,
        char**                      ppNextField
    )
{
    char*                           pCurChar    = pBuf;
    char*                           pNextChar   = pBuf;
    char                            TempStr[16] = {0};

    while ( (*pNextChar != 0) && !(AnscRtspIsEol(pNextChar)) )
    {
        if ( *pNextChar == RTSP_SEMI_COLON )
        {
            break;
        }

        pNextChar++;
    }

    if ( pNextChar == pCurChar )
    {
        AnscTraceWarning(("AnscRtspReadTransportIp -- failed to parse IP address, cannot find the end!\n"));
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        if ( (pNextChar - pCurChar) < sizeof(TempStr) )
        {
            _ansc_strncpy(TempStr, pCurChar, (pNextChar - pCurChar));
            *pulValue1 = _ansc_inet_addr(TempStr);

            if ( *pNextChar == RTSP_SEMI_COLON )
            {
                pNextChar++;
                *ppNextField = pNextChar;
            }

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            AnscTraceWarning(("AnscRtspReadTransportIp -- address string is too long!\n"));

            return  ANSC_STATUS_FAILURE;
        }
    }
}


_ANSC_STATIC_ _ANSC_INLINE_ ANSC_STATUS
AnscRtspParseTransportField
    (
        char*                       pBuf,
        ULONG                       ulBufSize,
        PUCHAR                      pucField,
        PULONG                      pulValue1,
        PULONG                      pulValue2,
        char**                      ppNextField
    )
{
    char*                           pCurChar    = pBuf;

    *ppNextField = NULL;

    while ( ((ULONG)(pCurChar - pBuf) < ulBufSize) && (*pCurChar == RTSP_SPACE) )
    {
        pCurChar++;
    }

    if ( AnscRtspIsEol(pCurChar) )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_unicast, _ansc_strlen(RTSP_TRANSPORT_STR_unicast)) == 0 )
    {
        /*
         *  mark unicast
         */
        *pucField       = RTSP_TRANSPORT_FIELD_unicast;
        *ppNextField    = pCurChar + _ansc_strlen(RTSP_TRANSPORT_STR_unicast);

        return  ANSC_STATUS_SUCCESS;
    }
    else if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_multicast, _ansc_strlen(RTSP_TRANSPORT_STR_multicast)) == 0 )
    {
        /*
         *  mark multicast
         */
        *pucField       = RTSP_TRANSPORT_FIELD_multicast;
        *ppNextField    = pCurChar + _ansc_strlen(RTSP_TRANSPORT_STR_multicast);

        return  ANSC_STATUS_SUCCESS;
    }
    else if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_client_port, _ansc_strlen(RTSP_TRANSPORT_STR_client_port)) == 0 )
    {
        /*
         *  read the client port range
         */
        *pucField       = RTSP_TRANSPORT_FIELD_client_port;

        pCurChar       += _ansc_strlen(RTSP_TRANSPORT_STR_client_port);

        return  AnscRtspReadTransportPortRange(pCurChar, pulValue1, pulValue2, ppNextField);
    }
    else if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_server_port, _ansc_strlen(RTSP_TRANSPORT_STR_server_port)) == 0 )
    {
        /*
         *  read the server port range
         */
        *pucField       = RTSP_TRANSPORT_FIELD_server_port;

        pCurChar       += _ansc_strlen(RTSP_TRANSPORT_STR_server_port);

        return  AnscRtspReadTransportPortRange(pCurChar, pulValue1, pulValue2, ppNextField);
    }
    else if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_source, _ansc_strlen(RTSP_TRANSPORT_STR_source)) == 0 )
    {
        /*
         *  read the source IP
         */
        *pucField       = RTSP_TRANSPORT_FIELD_source;

        pCurChar       += _ansc_strlen(RTSP_TRANSPORT_STR_source);

        return  AnscRtspReadTransportIp(pCurChar, pulValue1, ppNextField);
    }
    else if ( _ansc_strncmp(pCurChar, RTSP_TRANSPORT_STR_destination, _ansc_strlen(RTSP_TRANSPORT_STR_destination)) == 0 )
    {
        /*
         *  read the destination IP
         */
        *pucField       = RTSP_TRANSPORT_FIELD_destination;

        pCurChar       += _ansc_strlen(RTSP_TRANSPORT_STR_destination);

        return  AnscRtspReadTransportIp(pCurChar, pulValue1, ppNextField);
    }
    else
    {
        /*
         *  Move to next field
         */
        *ppNextField = _ansc_strchr(pCurChar, RTSP_SEMI_COLON);

        if ( *ppNextField )
        {
            (*ppNextField)++;
        }
    }

    return  ANSC_STATUS_UNAPPLICABLE;
}


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif

