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

    module:	ansc_packet_binary_sip.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Session Initiation Protocol (SIP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/25/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_SIP_
#define  _ANSC_PACKET_BINARY_SIP_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
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
                DATA STRUCTURES USED BY SIP
***********************************************************/

/*
 * A SIP message is either a request from a client to a server, or a response from a server to a
 * client. Both Request and Response messages use the basic format of RFC 2822, even though the
 * syntax differs in character set and syntax specifics. (SIP allows header fields that would not
 * be valid RFC 2822 header fields, for example.) Both types of messages consist of a start-line,
 * one or more header fields, an empty line indicating the end of the header fields, and an
 * optional message-body.
 *
 *          generic-message  =  start-line
 *                              *message-header
 *                              CRLF
 *                              [ message-body ]
 *          start-line       =  Request-Line / Status-Line
 *
 * The start-line, each message-header line, and the empty line MUST be terminated by a carriage-
 * return line-feed sequence (CRLF). Note that the empty line MUST be present even if the message
 * body is not. Except for the above difference in character sets, much of SIP's message and header
 * field syntax is identical to HTTP/1.1.
 */
#define  SIP_CARRIAGE_RETURN                        0x0D
#define  SIP_LINE_FEED                              0x0A
#define  SIP_HORIZONTAL_TAB                         0x09
#define  SIP_SPACE                                  0x20
#define  SIP_DOUBLE_QUOTE                           0x22
#define  SIP_COLON                                  (UCHAR)':'

#define  AnscSipIsUpAlpha(single_char)              ( (single_char >= 'A') && (single_char <= 'Z') )
#define  AnscSipIsLoAlpha(single_char)              ( (single_char >= 'a') && (single_char <= 'z') )
#define  AnscSipIsAlpha(single_char)                ( AnscSipIsUpAlpha(single_char) || AnscSipIsLoAlpha(single_char) )
#define  AnscSipIsDigit(single_char)                ( (single_char >= '0') && (single_char <= '9') )
#define  AnscSipIsAlphaOrDigit(single_char)         ( AnscSipIsAlpha(single_char) || AnscSipIsDigit(single_char) )

#define  AnscSipIsCr(single_char)                   ( (UCHAR)single_char == SIP_CARRIAGE_RETURN )
#define  AnscSipIsLf(single_char)                   ( (UCHAR)single_char == SIP_LINE_FEED       )
#define  AnscSipIsHt(single_char)                   ( (UCHAR)single_char == SIP_HORIZONTAL_TAB  )
#define  AnscSipIsSp(single_char)                   ( (UCHAR)single_char == SIP_SPACE           )
#define  AnscSipIsDq(single_char)                   ( (UCHAR)single_char == SIP_DOUBLE_QUOTE    )
#define  AnscSipIsCl(single_char)                   ( (UCHAR)single_char == SIP_COLON           )

#define  AnscSipIsLws(single_char)                  ( AnscSipIsHt(single_char) || AnscSipIsSp(single_char) )

#define  AnscSipIsEol(char_pointer)                                                        \
         ( AnscSipIsLf(*char_pointer) || (AnscSipIsCr(*char_pointer) && AnscSipIsLf(*(char_pointer + 1))) )

#define  AnscSipFindEol                             AnscSipFindCrLf

#define  AnscSipFindCrLf(buffer, size, where)                                               \
         {                                                                                  \
            char*                   text_stream1 = buffer;                                  \
            ULONG                   text_size1   = size;                                    \
                                                                                            \
            while ( text_size1 >= 2 )                                                       \
            {                                                                               \
                if ( AnscSipIsCr(*text_stream1) && AnscSipIsLf(*(text_stream1 + 1)) )       \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    text_stream1++;                                                         \
                    text_size1  --;                                                         \
                }                                                                           \
            }                                                                               \
                                                                                            \
            where = (text_size1 < 2)? NULL : text_stream1;                                  \
         }

#define  AnscSipFindHfStart(buffer, size, start)                                            \
         {                                                                                  \
            char*                   text_stream1 = buffer;                                  \
            ULONG                   text_size1   = size;                                    \
                                                                                            \
            while ( (text_size1 > 0) &&                                                     \
                    (AnscSipIsCr(*text_stream1) || AnscSipIsLf(*text_stream1)) )            \
            {                                                                               \
                text_stream1++;                                                             \
                text_size1  --;                                                             \
            }                                                                               \
                                                                                            \
            start = (text_size1 == 0)? NULL : text_stream1;                                 \
         }

#define  AnscSipFindBodyStart(buffer, size, start)                                          \
         {                                                                                  \
            char*                   text_stream2 = buffer;                                  \
            ULONG                   text_size2   = size;                                    \
            ULONG                   eol_count   = 0;                                        \
                                                                                            \
            while ( (text_size2 > 0) && (eol_count < 2) )                                   \
            {                                                                               \
                if ( AnscSipIsCr(*text_stream2) )                                           \
                {                                                                           \
                    eol_count  ++;                                                          \
                    text_stream2++;                                                         \
                    text_size2  --;                                                         \
                                                                                            \
                    if ( text_size2 > 0 )                                                   \
                    {                                                                       \
                        if ( AnscSipIsLf(*text_stream2) )                                   \
                        {                                                                   \
                            text_stream2++;                                                 \
                            text_size2  --;                                                 \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                else if ( AnscSipIsLf(*text_stream2) )                                      \
                {                                                                           \
                    eol_count  ++;                                                          \
                    text_stream2++;                                                         \
                    text_size2  --;                                                         \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    eol_count   = 0;                                                        \
                    text_stream2++;                                                         \
                    text_size2  --;                                                         \
                }                                                                           \
            }                                                                               \
                                                                                            \
            start = (eol_count == 2)? text_stream2 : NULL;                                  \
         }

#define  AnscSipIsHfComplete(buffer, size, b_complete)                                      \
         {                                                                                  \
            char*                   body_start = NULL;                                      \
                                                                                            \
            AnscSipFindBodyStart(buffer, size, body_start);                                 \
                                                                                            \
            b_complete = (body_start != NULL)? TRUE : FALSE;                                \
         }

#define  AnscSipGetHfSize(buffer, size, hf_size)                                            \
         {                                                                                  \
            char*                   body_start = NULL;                                      \
                                                                                            \
            AnscSipFindBodyStart(buffer, size, body_start);                                 \
                                                                                            \
            hf_size = (body_start != NULL)? (ULONG)body_start - (ULONG)buffer : size;       \
         }

#define  AnscSipFindNextHeader(buffer, size, next, err_detect)                              \
         {                                                                                  \
            char*                   text_stream3 = buffer;                                  \
            ULONG                   text_size3   = size;                                    \
                                                                                            \
            err_detect = FALSE;                                                             \
                                                                                            \
            while ( text_size3 > 0 )                                                        \
            {                                                                               \
                if ( *text_stream3 == 0 )                                                   \
                {                                                                           \
                    err_detect = TRUE;                                                      \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( AnscSipIsCr(*text_stream3) )                                      \
                {                                                                           \
                    text_stream3++;                                                         \
                    text_size3  --;                                                         \
                                                                                            \
                    if ( text_size3 > 0 )                                                   \
                    {                                                                       \
                        if ( AnscSipIsLf(*text_stream3) )                                   \
                        {                                                                   \
                            text_stream3++;                                                 \
                            text_size3  --;                                                 \
                        }                                                                   \
                                                                                            \
                        if ( text_size3 > 0 )                                               \
                        {                                                                   \
                            if ( !AnscSipIsLws(*text_stream3) )                             \
                            {                                                               \
                                break;                                                      \
                            }                                                               \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                else if ( AnscSipIsLf(*text_stream3) )                                      \
                {                                                                           \
                    text_stream3++;                                                         \
                    text_size3  --;                                                         \
                                                                                            \
                    if ( text_size3 > 0 )                                                   \
                    {                                                                       \
                        if ( !AnscSipIsLws(*text_stream3) )                                 \
                        {                                                                   \
                            break;                                                          \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    text_stream3++;                                                         \
                    text_size3  --;                                                         \
                }                                                                           \
            }                                                                               \
                                                                                            \
            next = (text_size3 == 0)? NULL : text_stream3;                                  \
         }

#define  AnscSipGetHfLineSize(buffer, size, line_size)                                      \
         {                                                                                  \
            char*                   next_header = NULL;                                     \
            BOOL                    b_err_exist = FALSE;                                    \
                                                                                            \
            AnscSipFindNextHeader(buffer, size, next_header, b_err_exist);                  \
                                                                                            \
            line_size = (next_header != NULL)? (ULONG)next_header - (ULONG)buffer : size;   \
         }

#define  AnscSipGetHfLineSize2(buffer, size, line_size, err_detect)                         \
         {                                                                                  \
            char*                   next_header = NULL;                                     \
                                                                                            \
            AnscSipFindNextHeader(buffer, size, next_header, err_detect);                   \
                                                                                            \
            line_size = (next_header != NULL)? (ULONG)next_header - (ULONG)buffer : size;   \
         }

#define  AnscSipPrepareHeader(raw_buffer, raw_size, header_buffer, header_size)             \
         {                                                                                  \
            char*                   src_stream = raw_buffer;                                \
            ULONG                   src_size   = raw_size;                                  \
            char*                   dst_stream = header_buffer;                             \
            ULONG                   dst_size   = 0;                                         \
            BOOL                    is_quoted  = FALSE;                                     \
                                                                                            \
            while ( src_size > 0 )                                                          \
            {                                                                               \
                if ( is_quoted )                                                            \
                {                                                                           \
                    if ( AnscSipIsDq(*src_stream) )                                         \
                    {                                                                       \
                        is_quoted = FALSE;                                                  \
                    }                                                                       \
                                                                                            \
                    *dst_stream = *src_stream;                                              \
                                                                                            \
                    dst_stream++;                                                           \
                    dst_size  ++;                                                           \
                    src_stream++;                                                           \
                    src_size  --;                                                           \
                }                                                                           \
                else if ( AnscSipIsLws(*src_stream) ||                                      \
                          AnscSipIsCr (*src_stream) ||                                      \
                          AnscSipIsLf (*src_stream) )                                       \
                {                                                                           \
                    if ( dst_size == 0 )                                                    \
                    {                                                                       \
                        *dst_stream = (char)SIP_SPACE;                                      \
                                                                                            \
                        dst_stream++;                                                       \
                        dst_size  ++;                                                       \
                    }                                                                       \
                    else if ( !AnscSipIsLws(*(dst_stream - 1)) )                            \
                    {                                                                       \
                        *dst_stream = (char)SIP_SPACE;                                      \
                                                                                            \
                        dst_stream++;                                                       \
                        dst_size  ++;                                                       \
                    }                                                                       \
                                                                                            \
                    src_stream++;                                                           \
                    src_size  --;                                                           \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    if ( AnscSipIsDq(*src_stream) )                                         \
                    {                                                                       \
                        is_quoted = TRUE;                                                   \
                    }                                                                       \
                                                                                            \
                    *dst_stream = *src_stream;                                              \
                                                                                            \
                    dst_stream++;                                                           \
                    dst_size  ++;                                                           \
                    src_stream++;                                                           \
                    src_size  --;                                                           \
                }                                                                           \
            }                                                                               \
                                                                                            \
            while ( dst_size > 0 )                                                          \
            {                                                                               \
                dst_stream--;                                                               \
                                                                                            \
                if ( !AnscSipIsLws(*dst_stream) )                                           \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    dst_size--;                                                             \
                }                                                                           \
            }                                                                               \
                                                                                            \
            header_size = dst_size;                                                         \
         }

#define  AnscSipGetHfValue(hf_line, size, hf_value)                                         \
         {                                                                                  \
            char*                   text_stream4 = (char*)hf_line;                          \
            ULONG                   text_size4   = size;                                    \
                                                                                            \
            while ( text_size4 > 0 )                                                        \
            {                                                                               \
                if ( !AnscSipIsCl(*text_stream4) )                                          \
                {                                                                           \
                    text_stream4++;                                                         \
                    text_size4  --;                                                         \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( text_size4 > 0 )                                                           \
            {                                                                               \
                text_stream4++;                                                             \
                text_size4  --;                                                             \
                                                                                            \
                while ( text_size4 > 0 )                                                    \
                {                                                                           \
                    if ( AnscSipIsLws(*text_stream4) )                                      \
                    {                                                                       \
                        text_stream4++;                                                     \
                        text_size4  --;                                                     \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \
            hf_value = (text_size4 == 0)? NULL : text_stream4;                              \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
