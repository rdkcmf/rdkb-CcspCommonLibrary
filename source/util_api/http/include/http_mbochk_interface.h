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

    module:	http_mbochk_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Mbo Chunked Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBOCHK_INTERFACE_
#define  _HTTP_MBOCHK_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "http_mbo_interface.h"
#include "http_mbo_exported_api.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP MBO CHUNKED OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_MBOCHK_SCRATCH_PAD1_SIZE              128
#define  HTTP_MBOCHK_SCRATCH_PAD2_SIZE              2048
#define  HTTP_MBOCHK_SCRATCH_PAD3_SIZE              2048

#define  HTTP_MBOCHK_CTSTATE_RESERVED               0
#define  HTTP_MBOCHK_CTSTATE_EMPTY                  1
#define  HTTP_MBOCHK_CTSTATE_PART_LINE              2
#define  HTTP_MBOCHK_CTSTATE_PART_BODY              3
#define  HTTP_MBOCHK_CTSTATE_COMPLETE               4
#define  HTTP_MBOCHK_CTSTATE_TRAILING               5

#define  HTTP_MBOCHK_BTYPE_CHUNK_DATA               0
#define  HTTP_MBOCHK_BTYPE_CHUNK_LINE               1
#define  HTTP_MBOCHK_BTYPE_CHUNK_CRLF               2
#define  HTTP_MBOCHK_BTYPE_TRAILER                  3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPMBOCHK_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBOCHK_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPMBOCHK_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_SET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_APPEND_CHUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        BOOL                        bHasCrlf
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPMBOCHK_GET_CHUNK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_ADD_CHUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChunkInfo,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    );

typedef  ULONG
(*PFN_HTTPMBOCHK_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_PROCESS_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_PROCESS_DATA2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        PBOOL                       pbBccIfNotified
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_APPEND_TRAILER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    );

typedef  ANSC_STATUS
(*PFN_HTTPMBOCHK_PARSE_TRAILER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    );

/*
 * The chunked encoding modifies the body of a message in order to transfer it as a series of
 * chunks, each with its own size indicator, followed by an OPTIONAL trailer containing entity
 * -header fields. This allows dynamically produced content to be transferred along with the
 * information necessary for the recipient to verify that it has received the full message.
 *
 *      Chunked-Body    = *chunk
 *                        last-chunk
 *                        trailer
 *                        CRLF
 *
 *      chunk           = chunk-size [ chunk-extension ] CRLF
 *                        chunk-data CRLF
 *      chunk-size      = 1*HEX
 *      last-chunk      = 1*("0") [ chunk-extension ] CRLF
 *
 *      chunk-extension = *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
 *      chunk-ext-name  = token
 *      chunk-ext-val   = token | quoted-string
 *      chunk-data      = chunk-size(OCTET)
 *      trailer         = *(entity-header CRLF)
 *
 * The chunk-size field is a string of hex digits indicating the size of the chunk. The chunked
 * encoding is ended by any chunk whose size is zero, followed by the trailer, which is terminated
 * by an empty line.
 *
 * The trailer allows the sender to include additional HTTP header fields at the end of the message.
 * The Trailer header field can be used to indicate which header fields are included in a trailer.
 */
#define  HTTP_MBO_CHUNKED_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_MESSAGE_BODY_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    ULONG                           ChunkTransferState;                                     \
    ULONG                           SkipSize;                                               \
    char                            ScratchPad1[HTTP_MBOCHK_SCRATCH_PAD1_SIZE];             \
    char                            ScratchPad2[HTTP_MBOCHK_SCRATCH_PAD2_SIZE];             \
    char                            ScratchPad3[HTTP_MBOCHK_SCRATCH_PAD3_SIZE];             \
    ULONG                           PadSize1;                                               \
    ULONG                           PadSize2;                                               \
    ULONG                           PadSize3;                                               \
    ULONG                           UseSize1;                                               \
    ULONG                           UseSize2;                                               \
    ULONG                           UseSize3;                                               \
                                                                                            \
    SLIST_HEADER                    ChunkInfoSList;                                         \
    ANSC_LOCK                       ChunkInfoSListLock;                                     \
                                                                                            \
    PFN_HTTPMBOCHK_GET_SIZE         GetExpectedSize;                                        \
    PFN_HTTPMBOCHK_APPEND_CHUNK     AppendChunkData;                                        \
                                                                                            \
    PFN_HTTPMBOCHK_CLEAR            ClearChunkInfos;                                        \
    PFN_HTTPMBOCHK_GET_CHUNK        GetCurChunkInfo;                                        \
    PFN_HTTPMBOCHK_ADD_CHUNK        AddChunkInfo;                                           \
                                                                                            \
    PFN_HTTPMBOCHK_PROCESS_DATA     ProcessChunkData;                                       \
    PFN_HTTPMBOCHK_PROCESS_DATA2    ProcessTrailer;                                         \
                                                                                            \
    PFN_HTTPMBOCHK_APPEND_TRAILER   AppendTrailer;                                          \
    PFN_HTTPMBOCHK_PARSE_TRAILER    ParseTrailer;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_MBO_CHUNKED_OBJECT
{
    HTTP_MBO_CHUNKED_CLASS_CONTENT
}
HTTP_MBO_CHUNKED_OBJECT,  *PHTTP_MBO_CHUNKED_OBJECT;

#define  ACCESS_HTTP_MBO_CHUNKED_OBJECT(p)          \
         ACCESS_CONTAINER(p, HTTP_MBO_CHUNKED_OBJECT, Linkage)


#endif
