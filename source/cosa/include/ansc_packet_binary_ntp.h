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

    module:	ansc_packet_binary_ntp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of Network Time Protocol.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/21/01    Xuechen Yang    template in ansc_packet_binary.h
        11/06/01    Na Li           initial revision

**********************************************************************/


#ifndef  _NTM_ANSC_PACKET_BINARY_
#define  _NTM_ANSC_PACKET_BINARY_

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

/*
 * NTP defines a set of messages sent as UDP data to port 123. 
 */
#define  NTP_PORT                                   123

/***********************************************************
         DATA STRUCTURES USED BY NTP MESSAGE
***********************************************************/


/* define the mask for control bits in the NTP message header
 * the mask is 8-bits
 */
#define  NTP_DATA_LEAP_INDICATOR_SHIFT              (6) 
#define  NTP_DATA_LEAP_INDICATOR_MASK               (0x3 << NTP_DATA_LEAP_INDICATOR_SHIFT) 
#define  NTP_DATA_VERSION_NUMBER_SHIFT              (3) 
#define  NTP_DATA_VERSION_NUMBER_MASK               (0x7 << NTP_DATA_VERSION_NUMBER_SHIFT) 
#define  NTP_DATA_MODE_SHIFT                        (0) 
#define  NTP_DATA_MODE_MASK                         (0x7 << NTP_DATA_MODE_SHIFT) 

/*
 * define the scaling factor to transform the fix-point variables in the packet
 * into DOUBLE variables, or vice versa
 */
#define  NTP_DATA_ROOTDELAY_SCALE_FACTOR            (0x10000) 
#define  NTP_DATA_ROOTDISP_SCALE_FACTOR             (0x10000) 
#define  NTP_DATA_TIMESTAMP_SCALE_FACTOR            (0xFFFFFFFF) /*(0x100000000) */
#define  NTP_DATA_TIMESTAMP_SCALE_FACTOR_HALF       (0x80000000) 


/*
 * define Ntp data format. 
 */
typedef  struct
_NTP_MSG_HEADER
{
    UCHAR                           ControlBits;
    UCHAR                           Stratum;
    char                            PollInterval;
    char                            Precision;

    LONG                            RootDelay;
    LONG                            RootDispersion;

    ULONG                           ReferClockID;
    ULONG                           ReferTimeStampI;
    ULONG                           ReferTimeStampF;
    ULONG                           OriginateTimeStampI;
    ULONG                           OriginateTimeStampF;
    ULONG                           ReceiveTimeStampI;
    ULONG                           ReceiveTimeStampF;
    ULONG                           TransmitTimeStampI;
    ULONG                           TransmitTimeStampF;
    UCHAR                           Authenticator[ANSC_ZERO_ARRAY_SIZE];

}_struct_pack_
NTP_MSG_HEADER,  *PNTP_MSG_HEADER;


/*
 * the fixed fields of the NTP message 
 */ 
#define  NTP_MSG_MAXIMUM_SIZE                      144
#define  NTP_MSG_MINIMUM_SIZE                      48


/* define macro to access header fields. for optional fields, the caller has to check if the
 * field exist before getting or setting the value
 */
#define  AnscNtpHasAuthenticator(p, size)           ( size >NTP_MSG_MINIMUM_SIZE ? TRUE : FALSE )

#define  AnscNtpGetLeapIndicator(p)                                                                \
            ( (p->ControlBits & NTP_DATA_LEAP_INDICATOR_MASK) >> NTP_DATA_LEAP_INDICATOR_SHIFT )    

#define  AnscNtpSetLeapIndicator(p, LeapIndicator)                                                 \
            ( p->ControlBits =                                                                     \
                (p->ControlBits & ~NTP_DATA_LEAP_INDICATOR_MASK) |                                 \
                (LeapIndicator << NTP_DATA_LEAP_INDICATOR_SHIFT) )  
  
#define  AnscNtpGetVersionNumber(p)                                                                \
            ( (p->ControlBits & NTP_DATA_VERSION_NUMBER_MASK) >> NTP_DATA_VERSION_NUMBER_SHIFT )    

#define  AnscNtpSetVersionNumber(p, VersionNumber)                                                 \
            ( p->ControlBits =                                                                     \
                (UCHAR)((p->ControlBits & ~NTP_DATA_VERSION_NUMBER_MASK) |                         \
                (VersionNumber << NTP_DATA_VERSION_NUMBER_SHIFT)) )  

#define  AnscNtpGetMode(p)                                                                         \
            ( (UCHAR)((p->ControlBits & NTP_DATA_MODE_MASK) >> NTP_DATA_MODE_SHIFT) )    

#define  AnscNtpSetMode(p, Mode)                                                                   \
            ( p->ControlBits =                                                                     \
                (p->ControlBits & ~NTP_DATA_MODE_MASK) |                                           \
                (Mode << NTP_DATA_MODE_SHIFT) )  

#define  AnscNtpGetStratum(p)                  (p->Stratum)
#define  AnscNtpSetStratum(p, value)           (p->Stratum = value)

#define  AnscNtpGetPollInterval(p)             (p->PollInterval)
#define  AnscNtpSetPollInterval(p, value)      (p->PollInterval = value)

#define  AnscNtpGetPrecision(p)                (p->Precision)
#define  AnscNtpSetPrecision(p, value)         (p->Precision = value)

#define  AnscNtpGetRootDelay(p)                                                                    \
            ( (LONG)(AnscUlongFromNToH(AnscReadUlong(&p->RootDelay))) / ((DOUBLE)NTP_DATA_ROOTDELAY_SCALE_FACTOR) )

#define  AnscNtpSetRootDelay(p, rootDelay)                                                         \
            AnscWriteUlong( &p->RootDelay, AnscUlongFromHToN((LONG)(rootDelay * NTP_DATA_ROOTDELAY_SCALE_FACTOR)) )

#define  AnscNtpGetRootDispersion(p)                                                               \
            ( (LONG)(AnscUlongFromNToH(AnscReadUlong(&p->RootDispersion))) / ((DOUBLE)NTP_DATA_ROOTDELAY_SCALE_FACTOR) )

#define  AnscNtpSetRootDispersion(p, rootDispersion)                                               \
            AnscWriteUlong( &p->RootDispersion, AnscUlongFromHToN((LONG)(rootDispersion * NTP_DATA_ROOTDISP_SCALE_FACTOR)) )

#define  AnscNtpGetReferClockID(p)                  AnscUlongFromNToH(AnscReadUlong(&p->ReferClockID))
#define  AnscNtpSetReferClockID(p, value)           AnscWriteUlong(&p->ReferClockID, AnscUlongFromHToN(value))

#define  AnscNtpGetReferTimeStampI(p)               AnscUlongFromNToH(AnscReadUlong(&p->ReferTimeStampI))
#define  AnscNtpSetReferTimeStampI(p, value)        AnscWriteUlong(&p->ReferTimeStampI, AnscUlongFromHToN(value))

#define  AnscNtpGetReferTimeStampF(p)               AnscUlongFromNToH(AnscReadUlong(&p->ReferTimeStampF))
#define  AnscNtpSetReferTimeStampF(p, value)        AnscWriteUlong(&p->ReferTimeStampF, AnscUlongFromHToN(value))

#define  AnscNtpGetOriginateTimeStampI(p)           AnscUlongFromNToH(AnscReadUlong(&p->OriginateTimeStampI))
#define  AnscNtpSetOriginateTimeStampI(p, value)    AnscWriteUlong(&p->OriginateTimeStampI, AnscUlongFromHToN(value))

#define  AnscNtpGetOriginateTimeStampF(p)           AnscUlongFromNToH(AnscReadUlong(&p->OriginateTimeStampF))
#define  AnscNtpSetOriginateTimeStampF(p, value)    AnscWriteUlong(&p->OriginateTimeStampF, AnscUlongFromHToN(value))

#define  AnscNtpGetReceiveTimeStampI(p)             AnscUlongFromNToH(AnscReadUlong(&p->ReceiveTimeStampI))
#define  AnscNtpSetReceiveTimeStampI(p, value)      AnscWriteUlong(&p->ReceiveTimeStampI, AnscUlongFromHToN(value))

#define  AnscNtpGetReceiveTimeStampF(p)             AnscUlongFromNToH(AnscReadUlong(&p->ReceiveTimeStampF))
#define  AnscNtpSetReceiveTimeStampF(p, value)      AnscWriteUlong(&p->ReceiveTimeStampF, AnscUlongFromHToN(value))

#define  AnscNtpGetTransmitTimeStampI(p)            AnscUlongFromNToH(AnscReadUlong(&p->TransmitTimeStampI))
#define  AnscNtpSetTransmitTimeStampI(p, value)     AnscWriteUlong(&p->TransmitTimeStampI, AnscUlongFromHToN(value))

#define  AnscNtpGetTransmitTimeStampF(p)            AnscUlongFromNToH(AnscReadUlong(&p->TransmitTimeStampF))
#define  AnscNtpSetTransmitTimeStampF(p, value)     AnscWriteUlong(&p->TransmitTimeStampF, AnscUlongFromHToN(value))

/*
 * define macros for operation
 */
#define  AnscNtpIsTimeAILessTimeBI(TimeAI, TimeBI)                                                 \
         (                                                                                         \
            ((TimeAI < TimeBI) && ( TimeBI - TimeAI < NTP_DATA_TIMESTAMP_SCALE_FACTOR_HALF )) ||   \
            ((TimeAI > TimeBI) && ( TimeAI - TimeBI > NTP_DATA_TIMESTAMP_SCALE_FACTOR_HALF ))      \
         )
#define  AnscNtpIsTimeALessTimeB(TimeAI, TimeAF, TimeBI, TimeBF)                                   \
         (                                                                                         \
            AnscNtpIsTimeAILessTimeBI(TimeAI, TimeBI)   ||                                         \
            ( (TimeAI == TimeBI) && (TimeAF < TimeBF) )                                            \
         )

#define  AnscNtpIsTimeOverflow(oldTime, newTime)                                                   \
            ( (oldTime > newTime) && (oldTime - newTime > NTP_DATA_TIMESTAMP_SCALE_FACTOR_HALF) )

#define  AnscNtpAbs(value)                          ((value) > 0? (value) : (-(value)))

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif

#endif
