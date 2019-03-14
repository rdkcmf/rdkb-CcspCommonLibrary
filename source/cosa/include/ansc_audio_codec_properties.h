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

    module:	ansc_audio_codec_properties.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        data structures and basic types related to Audio Codec.

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


#ifndef  _ANSC_AUDIO_CODEC_PROPERTIES_
#define  _ANSC_AUDIO_CODEC_PROPERTIES_


/***********************************************************
    DEFINITION OF COMMONLY-USED STRUCTURES AND PARAMETERS
***********************************************************/

/*
 * Most audio codecs can only work on linear PCM (16-bit samples) input while some codes demand
 * either A-law or U-law G.711 audio input instead. Another important aspect of audio codec is
 * the octet-packing mechanism. Most reference codec implementations don't even bother to pack
 * compressed audio data.
 */
#define  ANSC_ACODEC_CTYPE_RESERVED                 0
#define  ANSC_ACODEC_CTYPE_G711A                    1
#define  ANSC_ACODEC_CTYPE_G711U                    2
#define  ANSC_ACODEC_CTYPE_G722                     3
#define  ANSC_ACODEC_CTYPE_G723                     4
#define  ANSC_ACODEC_CTYPE_G726_40                  5
#define  ANSC_ACODEC_CTYPE_G726_32                  6
#define  ANSC_ACODEC_CTYPE_G726_24                  7
#define  ANSC_ACODEC_CTYPE_G726_16                  8
#define  ANSC_ACODEC_CTYPE_GSM610                   9

#define  ANSC_ACODEC_STYPE_L08                      0x00000001
#define  ANSC_ACODEC_STYPE_L16                      0x00000002
#define  ANSC_ACODEC_STYPE_ALAW                     0x00000004
#define  ANSC_ACODEC_STYPE_ULAW                     0x00000008
#define  ANSC_ACODEC_STYPE_G722                     0x00000010
#define  ANSC_ACODEC_STYPE_G726_40                  0x00000020
#define  ANSC_ACODEC_STYPE_G726_32                  0x00000040
#define  ANSC_ACODEC_STYPE_G726_24                  0x00000080
#define  ANSC_ACODEC_STYPE_G726_16                  0x00000100
#define  ANSC_ACODEC_STYPE_GSM610                   0x00000200

#define  ANSC_ACODEC_PMODE_WORD                     1           /* 16-bit sample, right-justified */
#define  ANSC_ACODEC_PMODE_BYTE                     2           /*  8-bit sample, right-justified */
#define  ANSC_ACODEC_PMODE_FRAG                     3           /* <8-bit sample, rtp-specific    */

typedef  struct
_ANSC_ACODEC_ATTRIBUTE
{
    ULONG                           CodecType;
    ULONG                           IsignalType;                /* l16-bit, l08-bit, a-law, u-law */
    ULONG                           OsignalType;                /* l16-bit, l08-bit, a-law, u-law */
    ULONG                           IpackMode;                  /* MUST always be rtp-compliant   */
    ULONG                           OpackMode;                  /* MUST always be rtp-compliant   */
    ULONG                           SamplingRate;
}
ANSC_ACODEC_ATTRIBUTE,  *PANSC_ACODEC_ATTRIBUTE;

#define  ANSC_ACODEC_ENCODER_CONTENT                                                        \
    /* start of data structure content */                                                   \
    ULONG                           IsignalsMask;                                           \
    ULONG                           OsignalsMask;                                           \
    ULONG                           DefIsignalType;                                         \
    ULONG                           DefOsignalType;                                         \
    ULONG                           IpackMode;                                              \
    ULONG                           OpackMode;                                              \

typedef  struct
_ANSC_ACODEC_ENCODER
{
    ANSC_ACODEC_ENCODER_CONTENT
}
ANSC_ACODEC_ENCODER,  *PANSC_ACODEC_ENCODER;

typedef  struct  _ANSC_ACODEC_ENCODER  ANSC_ACODEC_DECODER,  *PANSC_ACODEC_DECODER;


/***********************************************************
       DEFINITION OF ATTRIBUTE AND CONSTANTS FOR G.711
***********************************************************/

/*
 * In the early 1960's an interest was expressed in encoding the analog signals in telephone
 * network, mainly to reduce costs in switching and multiplexing equipments and to allow the
 * integration of communication and computing, increasing the efficiency in operation and
 * maintenance.
 *
 * In 1972, the then CCITT published the Recommendation G.711 that constitutes the principal
 * reference as far as transmission systems are concerned. The basic principle of the algorithm
 * is to code speech using 8 bits per sample, the input voiceband signal being sampled at 8 kHZ,
 * keeping the telephony bandwidth of 300 ~ 3400 HZ. With this combination, each void channel
 * requires 64 kbit/s.
 */
typedef  struct
_ANSC_ENCODER_G711
{
    ANSC_ACODEC_ENCODER_CONTENT
}
ANSC_ENCODER_G711,  *PANSC_ENCODER_G711;

typedef  struct  _ANSC_ENCODER_G711  ANSC_DECODER_G711,  *PANSC_DECODER_G711;


/***********************************************************
       DEFINITION OF ATTRIBUTE AND CONSTANTS FOR G.722
***********************************************************/

/*
 * G.722 is specified in ITU-T Recommendation G.722, "7 kHZ audio-coding within 64 kbit/s". The
 * G.722 encoder produces a stream of octets, each of which SHALL be octet-aligned in an RTP packet.
 * The first bit transmitted in the G.722 octet, whcih is the most significant bit of the higher
 * sub-band sample, SHALL correspond to the most significant bit of the octet in the RTP packet.
 */
#define  ANSC_G722_MAX_STATE_SIZE                   512

typedef  struct
_ANSC_ENCODER_G722
{
    ANSC_ACODEC_ENCODER_CONTENT

    char                            State[ANSC_G722_MAX_STATE_SIZE];
}
ANSC_ENCODER_G722,  *PANSC_ENCODER_G722;

typedef  struct  _ANSC_ENCODER_G722  ANSC_DECODER_G722,  *PANSC_DECODER_G722;


/***********************************************************
       DEFINITION OF ATTRIBUTE AND CONSTANTS FOR G.726
***********************************************************/

/*
 * ITU-T Recommendation G.726 describes, among others, the algorithm recommended for conversion
 * of a single 64 kbit/s A-law or mu-law PCM channel encoded at 8000 samples/sec to and from a
 * 40, 32, 24, or 16 kbit/s channel. The conversion is applied to the PCM stream using an Adaptive
 * Differential Pulse Code Modulation (ADPCM) transcoding technique. The ADPCM representation
 * consists of a series of codewords with a one-to-one correspondance to the samples in the PCM
 * stream. The G.726 data rates of 40, 32, 24, and 16 kbit/s have codewords of 5, 4, 3, and 2 bits
 * respectively.
 */
#define  ANSC_G726_MAX_STATE_SIZE                   256

typedef  struct
_ANSC_ENCODER_G726
{
    ANSC_ACODEC_ENCODER_CONTENT

    short                           ResetFlag;
    char                            State[ANSC_G726_MAX_STATE_SIZE];
}
ANSC_ENCODER_G726,  *PANSC_ENCODER_G726;

typedef  struct  _ANSC_ENCODER_G726  ANSC_DECODER_G726,  *PANSC_DECODER_G726;


/***********************************************************
      DEFINITION OF ATTRIBUTE AND CONSTANTS FOR GSM_610
***********************************************************/

/*
 * GSM (Group Speciale Mobile) denotes the European GSM 06.10 standard for full-rate speech
 * transcoding, ETS 300 961, which is based on RPE/LTP (Residual Pulse Excitation/Long Term
 * Prediction) coding at a rate of 13kb/s.
 */
#define  ANSC_GSM610_DEF_FRAME_SIZE                 20          /* in milliseconds    */
#define  ANSC_GSM610_RAW_BLOCK_SIZE                 320         /* 16 bits per sample */
#define  ANSC_GSM610_COM_BLOCK_SIZE                 33
#define  ANSC_GSM610_RAW_SAMPLE_COUNT               160
#define  ANSC_GSM610_RTP_SIGNATURE                  0x0D

typedef  struct
_ANSC_ENCODER_GSM610
{
    ANSC_ACODEC_ENCODER_CONTENT
}
ANSC_ENCODER_GSM610,  *PANSC_ENCODER_GSM610;

typedef  struct  _ANSC_ENCODER_GSM610  ANSC_DECODER_GSM610,  *PANSC_DECODER_GSM610;


#endif
