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

    module:	ansc_aco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object interface for the
        Audio Codec Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        11/24/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_ACO_INTERFACE_
#define  _ANSC_ACO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_audio_codec_properties.h"


/***********************************************************
             GENERAL AUDIO CODEC OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition
 */
#define  ANSC_ACO_MAX_SPAD_SIZE                     2048
#define  ANSC_ACO_MIN_SPAD_SIZE                     128

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_ACO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       codec,
        PVOID                       raw,
        ULONG                       ulRawSize,
        PVOID                       com,
        PULONG                      pulComSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_DECODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       codec,
        PVOID                       com,
        ULONG                       ulComSize,
        PVOID                       raw,
        PULONG                      pulRawSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_CONVERT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pIbuffer,
        PVOID                       pObuffer,
        ULONG                       ulIsize,
        ULONG                       ulIstype,
        ULONG                       ulOstype
    );

typedef  ANSC_STATUS
(*PFN_ACO_RESET1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       codec
    );

typedef  ANSC_STATUS
(*PFN_ACO_RESET2)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ACO_ENCODE_G711)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       raw,
        ULONG                       ulRawSize,
        PVOID                       com,
        PULONG                      pulComSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_DECODE_G711)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       com,
        ULONG                       ulComSize,
        PVOID                       raw,
        PULONG                      pulRawSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_ENCODE_G722)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       raw,
        ULONG                       ulRawSize,
        PVOID                       com,
        PULONG                      pulComSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_DECODE_G722)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       com,
        ULONG                       ulComSize,
        PVOID                       raw,
        PULONG                      pulRawSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_ENCODE_G726)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       raw,
        ULONG                       ulRawSize,
        PVOID                       com,
        PULONG                      pulComSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_DECODE_G726)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       com,
        ULONG                       ulComSize,
        PVOID                       raw,
        PULONG                      pulRawSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_ENCODE_GSM610)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       raw,
        ULONG                       ulRawSize,
        PVOID                       com,
        PULONG                      pulComSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ACO_DECODE_GSM610)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       com,
        ULONG                       ulComSize,
        PVOID                       raw,
        PULONG                      pulRawSize,
        ANSC_HANDLE                 hReserved
    );

/*
 * In sample-based encodings, each audio sample is represented by a fixed number of bits. Within
 * the compressed audio data, codes for individual samples may span octet boundaries. An RTP audio
 * packet may contain any number of audio samples, subject to the constraint that the number of
 * bits per sample times the number of samples per packet yields an integral octet count.
 * Fractional encodings produce less than one octet per sample. Frame-based encodings encode a
 * fixed-length block of audio into another block of compressed data, typically also of fixed
 * length. For frame-based encodings, the sender MAY choose to combine several such frames into
 * a single RTP packet. The receiver can tell the number of frames contained in an RTP packet, if
 * all the frames have the same length, by dividing the RTP payload length by the audio frame size
 * which is defined as part of the encoding.
 */
#define  ANSC_AUDIO_CODEC_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_ENCODER_G711               EncoderG711;                                            \
    ANSC_DECODER_G711               DecoderG711;                                            \
    ANSC_ENCODER_G722               EncoderG722;                                            \
    ANSC_DECODER_G722               DecoderG722;                                            \
    ANSC_ENCODER_G726               EncoderG726;                                            \
    ANSC_DECODER_G726               DecoderG726;                                            \
    ANSC_ENCODER_GSM610             EncoderGsm610;                                          \
    ANSC_DECODER_GSM610             DecoderGsm610;                                          \
                                                                                            \
    char                            IscrtachPad[ANSC_ACO_MAX_SPAD_SIZE];                    \
    char                            OscrtachPad[ANSC_ACO_MAX_SPAD_SIZE];                    \
    char                            TscrtachPad[ANSC_ACO_MAX_SPAD_SIZE];                    \
    char                            UscrtachPad[ANSC_ACO_MIN_SPAD_SIZE];                    \
                                                                                            \
    PFN_ACO_ENCODE                  Encode;                                                 \
    PFN_ACO_DECODE                  Decode;                                                 \
    PFN_ACO_CONVERT                 ConvertIsignal;                                         \
    PFN_ACO_CONVERT                 ConvertOsignal;                                         \
    PFN_ACO_RESET1                  ResetEncoder;                                           \
    PFN_ACO_RESET1                  ResetDecoder;                                           \
                                                                                            \
    PFN_ACO_ENCODE_G711             EncodeG711A;                                            \
    PFN_ACO_DECODE_G711             DecodeG711A;                                            \
    PFN_ACO_ENCODE_G711             EncodeG711U;                                            \
    PFN_ACO_DECODE_G711             DecodeG711U;                                            \
    PFN_ACO_RESET2                  ResetEncoderG711;                                       \
    PFN_ACO_RESET2                  ResetDecoderG711;                                       \
                                                                                            \
    PFN_ACO_ENCODE_G722             EncodeG722;                                             \
    PFN_ACO_DECODE_G722             DecodeG722;                                             \
    PFN_ACO_RESET2                  ResetEncoderG722;                                       \
    PFN_ACO_RESET2                  ResetDecoderG722;                                       \
                                                                                            \
    PFN_ACO_ENCODE_G726             EncodeG726_40;                                          \
    PFN_ACO_DECODE_G726             DecodeG726_40;                                          \
    PFN_ACO_ENCODE_G726             EncodeG726_32;                                          \
    PFN_ACO_DECODE_G726             DecodeG726_32;                                          \
    PFN_ACO_ENCODE_G726             EncodeG726_24;                                          \
    PFN_ACO_DECODE_G726             DecodeG726_24;                                          \
    PFN_ACO_ENCODE_G726             EncodeG726_16;                                          \
    PFN_ACO_DECODE_G726             DecodeG726_16;                                          \
    PFN_ACO_RESET2                  ResetEncoderG726;                                       \
    PFN_ACO_RESET2                  ResetDecoderG726;                                       \
                                                                                            \
    PFN_ACO_ENCODE_GSM610           EncodeGsm610;                                           \
    PFN_ACO_DECODE_GSM610           DecodeGsm610;                                           \
    PFN_ACO_RESET2                  ResetEncoderGsm610;                                     \
    PFN_ACO_RESET2                  ResetDecoderGsm610;                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_AUDIO_CODEC_OBJECT
{
    ANSC_AUDIO_CODEC_CLASS_CONTENT
}
ANSC_AUDIO_CODEC_OBJECT,  *PANSC_AUDIO_CODEC_OBJECT;

#define  ACCESS_ANSC_AUDIO_CODEC_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_AUDIO_CODEC_OBJECT, Linkage)


#endif
