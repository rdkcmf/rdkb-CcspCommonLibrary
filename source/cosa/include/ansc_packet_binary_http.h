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

    module:	ansc_packet_binary_http.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the HyperText Transfer Protocol (HTTP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_HTTP_
#define  _ANSC_PACKET_BINARY_HTTP_


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
                DATA STRUCTURES USED BY HTTP
***********************************************************/

/*
 * HTTP messages consist of requests from client to server and responses from server to client.
 *
 *      HTTP-message = Request | Response     ; HTTP/1.1 messages
 *
 * Request and Response messages use the generic message format of RFC 822 for transferring
 * entities (the payload of the message). Both types of message consist of a start-line, zero or
 * more header fields (also known as "headers"), an empty line (i.e., a line with nothing preceding
 * the CRLF) indicating the end of the header fields, and possibly a message-body.
 *
 *      generic-message = start-line
 *                        *(message-header CRLF)
 *                        CRLF
 *                        [ message-body ]
 *      start-line      = Request-Line | Status-Line
 *
 * In the interest of robustness, servers SHOULD ignore any empty line(s) received where a Request-
 * Line is expected. In other words, if the server is reading the protocol stream at the beginning
 * of a message and receives a CRLF first, it should ignore the CRLF.
 */
#define  HTTP_CARRIAGE_RETURN                       0x0D
#define  HTTP_LINE_FEED                             0x0A
#define  HTTP_HORIZONTAL_TAB                        0x09
#define  HTTP_SPACE                                 0x20
#define  HTTP_DOUBLE_QUOTE                          0x22
#define  HTTP_COLON                                 (UCHAR)':'

#define  AnscHttpIsUpAlpha(single_char)             ( (single_char >= 'A') && (single_char <= 'Z') )
#define  AnscHttpIsLoAlpha(single_char)             ( (single_char >= 'a') && (single_char <= 'z') )
#define  AnscHttpIsAlpha(single_char)               ( AnscHttpIsUpAlpha(single_char) || AnscHttpIsLoAlpha(single_char) )
#define  AnscHttpIsDigit(single_char)               ( (single_char >= '0') && (single_char <= '9') )
#define  AnscHttpIsAlphaOrDigit(single_char)        ( AnscHttpIsAlpha(single_char) || AnscHttpIsDigit(single_char) )

#define  AnscHttpIsCr(single_char)                  ( (UCHAR)single_char == HTTP_CARRIAGE_RETURN )
#define  AnscHttpIsLf(single_char)                  ( (UCHAR)single_char == HTTP_LINE_FEED       )
#define  AnscHttpIsHt(single_char)                  ( (UCHAR)single_char == HTTP_HORIZONTAL_TAB  )
#define  AnscHttpIsSp(single_char)                  ( (UCHAR)single_char == HTTP_SPACE           )
#define  AnscHttpIsDq(single_char)                  ( (UCHAR)single_char == HTTP_DOUBLE_QUOTE    )
#define  AnscHttpIsCl(single_char)                  ( (UCHAR)single_char == HTTP_COLON           )

#define  AnscHttpIsLws(single_char)                 ( AnscHttpIsHt(single_char) || AnscHttpIsSp(single_char) )

#define  AnscHttpIsEol(char_pointer)                                                        \
         ( AnscHttpIsLf(*char_pointer) || (AnscHttpIsCr(*char_pointer) && AnscHttpIsLf(*(char_pointer + 1))) )

#define  AnscHttpFindEol                            AnscHttpFindCrLf

#define  AnscHttpFindCrLf(buffer, size, where)                                              \
         {                                                                                  \
            char*                   text_stream1 = buffer;                                  \
            ULONG                   text_size1   = size;                                    \
                                                                                            \
            while ( text_size1 >= 2 )                                                       \
            {                                                                               \
                if ( AnscHttpIsCr(*text_stream1) && AnscHttpIsLf(*(text_stream1 + 1)) )     \
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

#define  AnscHttpFindChunkData(buffer, size, data_start)                                    \
         {                                                                                  \
            char*                   where_is_eol = NULL;                                    \
                                                                                            \
            AnscHttpFindEol(buffer, size, where_is_eol);                                    \
                                                                                            \
            if ( where_is_eol != NULL )                                                     \
            {                                                                               \
                data_start = (PVOID)((ULONG)where_is_eol + 2);                              \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                data_start = NULL;                                                          \
            }                                                                               \
         }

#define  AnscHttpGetChunkLineSize(buffer, size, line_size)                                  \
         {                                                                                  \
            char*                   chunk_data = NULL;                                      \
                                                                                            \
            AnscHttpFindChunkData(buffer, size, chunk_data);                                \
                                                                                            \
            line_size = (chunk_data != NULL)? (ULONG)chunk_data - (ULONG)buffer : size;     \
         }

#define  AnscHttpGetTrailerSize(buffer, size, tr_size)                                      \
         {                                                                                  \
            char*                   trailer_start = (char*)buffer;                          \
            char*                   body_start    = NULL;                                   \
                                                                                            \
            if ( size < 2 )                                                                 \
            {                                                                               \
                tr_size = size;                                                             \
            }                                                                               \
            else if (AnscHttpIsCr(*trailer_start) && AnscHttpIsLf(*(trailer_start + 1)) )   \
            {                                                                               \
                tr_size = 2;                                                                \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                AnscHttpFindBodyStart(buffer, size, body_start);                            \
                                                                                            \
                tr_size = (body_start != NULL)? (ULONG)body_start - (ULONG)buffer : size;   \
            }                                                                               \
         }

#define  AnscHttpIsTrailerComplete(buffer, size, b_complete)                                \
         {                                                                                  \
            char*                   trailer_start = (char*)buffer;                          \
            char*                   body_start    = NULL;                                   \
                                                                                            \
            if ( size < 2 )                                                                 \
            {                                                                               \
                b_complete = FALSE;                                                         \
            }                                                                               \
            else if (AnscHttpIsCr(*trailer_start) && AnscHttpIsLf(*(trailer_start + 1)) )   \
            {                                                                               \
                b_complete = TRUE;                                                          \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                AnscHttpFindBodyStart(buffer, size, body_start);                            \
                                                                                            \
                b_complete = (body_start != NULL)? TRUE : FALSE;                            \
            }                                                                               \
         }

#define  AnscHttpFindHfStart(buffer, size, start)                                           \
         {                                                                                  \
            char*                   text_stream1 = buffer;                                  \
            ULONG                   text_size1   = size;                                    \
                                                                                            \
            while ( (text_size1 > 0) &&                                                     \
                    (AnscHttpIsCr(*text_stream1) || AnscHttpIsLf(*text_stream1)) )          \
            {                                                                               \
                text_stream1++;                                                             \
                text_size1  --;                                                             \
            }                                                                               \
                                                                                            \
            start = (text_size1 == 0)? NULL : text_stream1;                                 \
         }

#define  AnscHttpFindBodyStart(buffer, size, start)                                         \
         {                                                                                  \
            char*                   text_stream2 = buffer;                                  \
            ULONG                   text_size2   = size;                                    \
            ULONG                   eol_count   = 0;                                        \
                                                                                            \
            while ( (text_size2 > 0) && (eol_count < 2) )                                   \
            {                                                                               \
                if ( AnscHttpIsCr(*text_stream2) )                                          \
                {                                                                           \
                    eol_count  ++;                                                          \
                    text_stream2++;                                                         \
                    text_size2  --;                                                         \
                                                                                            \
                    if ( text_size2 > 0 )                                                   \
                    {                                                                       \
                        if ( AnscHttpIsLf(*text_stream2) )                                  \
                        {                                                                   \
                            text_stream2++;                                                 \
                            text_size2  --;                                                 \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                else if ( AnscHttpIsLf(*text_stream2) )                                     \
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

#define  AnscHttpIsHfComplete(buffer, size, b_complete)                                     \
         {                                                                                  \
            char*                   body_start = NULL;                                      \
                                                                                            \
            AnscHttpFindBodyStart(buffer, size, body_start);                                \
                                                                                            \
            b_complete = (body_start != NULL)? TRUE : FALSE;                                \
         }

#define  AnscHttpGetHfSize(buffer, size, hf_size)                                           \
         {                                                                                  \
            char*                   body_start = NULL;                                      \
                                                                                            \
            AnscHttpFindBodyStart(buffer, size, body_start);                                \
                                                                                            \
            hf_size = (body_start != NULL)? (ULONG)body_start - (ULONG)buffer : size;       \
         }

#define  AnscHttpFindNextHeader(buffer, size, next)                                         \
         {                                                                                  \
            char*                   text_stream3 = buffer;                                  \
            ULONG                   text_size3   = size;                                    \
                                                                                            \
            while ( text_size3 > 0 )                                                        \
            {                                                                               \
                if ( AnscHttpIsCr(*text_stream3) )                                          \
                {                                                                           \
                    text_stream3++;                                                         \
                    text_size3  --;                                                         \
                                                                                            \
                    if ( text_size3 > 0 )                                                   \
                    {                                                                       \
                        if ( AnscHttpIsLf(*text_stream3) )                                  \
                        {                                                                   \
                            text_stream3++;                                                 \
                            text_size3  --;                                                 \
                        }                                                                   \
                                                                                            \
                        if ( text_size3 > 0 )                                               \
                        {                                                                   \
                            if ( !AnscHttpIsLws(*text_stream3) )                            \
                            {                                                               \
                                break;                                                      \
                            }                                                               \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                else if ( AnscHttpIsLf(*text_stream3) )                                     \
                {                                                                           \
                    text_stream3++;                                                         \
                    text_size3  --;                                                         \
                                                                                            \
                    if ( text_size3 > 0 )                                                   \
                    {                                                                       \
                        if ( !AnscHttpIsLws(*text_stream3) )                                \
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

#define  AnscHttpGetHfLineSize(buffer, size, line_size)                                     \
         {                                                                                  \
            char*                   next_header = NULL;                                     \
                                                                                            \
            AnscHttpFindNextHeader(buffer, size, next_header);                              \
                                                                                            \
            line_size = (next_header != NULL)? (ULONG)next_header - (ULONG)buffer : size;   \
         }

#define  AnscHttpPrepareHeader(raw_buffer, raw_size, header_buffer, header_size)            \
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
                    if ( AnscHttpIsDq(*src_stream) )                                        \
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
                else if ( AnscHttpIsLws(*src_stream) ||                                     \
                          AnscHttpIsCr (*src_stream) ||                                     \
                          AnscHttpIsLf (*src_stream) )                                      \
                {                                                                           \
                    if ( dst_size == 0 )                                                    \
                    {                                                                       \
                        *dst_stream = (char)HTTP_SPACE;                                     \
                                                                                            \
                        dst_stream++;                                                       \
                        dst_size  ++;                                                       \
                    }                                                                       \
                    else if ( !AnscHttpIsLws(*(dst_stream - 1)) )                           \
                    {                                                                       \
                        *dst_stream = (char)HTTP_SPACE;                                     \
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
                    if ( AnscHttpIsDq(*src_stream) )                                        \
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
                if ( !AnscHttpIsLws(*dst_stream) )                                          \
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

#define  AnscHttpGetHfValue(hf_line, size, hf_value)                                        \
         {                                                                                  \
            char*                   text_stream4 = (char*)hf_line;                          \
            ULONG                   text_size4   = size;                                    \
                                                                                            \
            while ( text_size4 > 0 )                                                        \
            {                                                                               \
                if ( !AnscHttpIsCl(*text_stream4) )                                         \
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
                    if ( AnscHttpIsLws(*text_stream4) )                                     \
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
