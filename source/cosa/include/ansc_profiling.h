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

    module:	ansc_profiling.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all profiling related data 
        structures and macros.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding, Hua

    ---------------------------------------------------------------

    revision:

        08/28/05    initial revision.

**********************************************************************/


#ifndef  _ANSC_PROFILING_
#define  _ANSC_PROFILING_


/***********************************************************
               PROFILING COUNTER DEFINITIONS
***********************************************************/

#define  ANSC_PROFILING_COUNTER_base                0

#define  ANSC_PROFILING_COUNTER_bspBase             ANSC_PROFILING_COUNTER_base         + 1
#define  ANSC_PROFILING_COUNTER_bspEthIsr           ANSC_PROFILING_COUNTER_bspBase
#define  ANSC_PROFILING_COUNTER_sspHook             ANSC_PROFILING_COUNTER_bspBase      + 1

#define  ANSC_PROFILING_COUNTER_anscBase            ANSC_PROFILING_COUNTER_bspBase      + 16
#define  ANSC_PROFILING_COUNTER_anscAllocMemory     ANSC_PROFILING_COUNTER_anscBase
#define  ANSC_PROFILING_COUNTER_anscFlushMemory     ANSC_PROFILING_COUNTER_anscBase     + 1
#define  ANSC_PROFILING_COUNTER_anscCopyMemory      ANSC_PROFILING_COUNTER_anscBase     + 2
#define  ANSC_PROFILING_COUNTER_anscZeroMemory      ANSC_PROFILING_COUNTER_anscBase     + 3
#define  ANSC_PROFILING_COUNTER_anscEqualMemory     ANSC_PROFILING_COUNTER_anscBase     + 4

#define  ANSC_PROFILING_COUNTER_gnpsBase            ANSC_PROFILING_COUNTER_anscBase     + 16
#define  ANSC_PROFILING_COUNTER_gnpsEdoRecv         ANSC_PROFILING_COUNTER_gnpsBase
#define  ANSC_PROFILING_COUNTER_gnpsEdoSend         ANSC_PROFILING_COUNTER_gnpsBase     + 1
#define  ANSC_PROFILING_COUNTER_gnpsEdoSendOs       ANSC_PROFILING_COUNTER_gnpsBase     + 2
#define  ANSC_PROFILING_COUNTER_gnpsLdoRecv         ANSC_PROFILING_COUNTER_gnpsBase     + 3
#define  ANSC_PROFILING_COUNTER_gnpsLdoSend         ANSC_PROFILING_COUNTER_gnpsBase     + 4
#define  ANSC_PROFILING_COUNTER_gnpsLdoSnpu3Recv    ANSC_PROFILING_COUNTER_gnpsBase     + 5
#define  ANSC_PROFILING_COUNTER_gnpsLdoSnpu3Send    ANSC_PROFILING_COUNTER_gnpsBase     + 6
#define  ANSC_PROFILING_COUNTER_gnpsFfoSnpu3        ANSC_PROFILING_COUNTER_gnpsBase     + 7
#define  ANSC_PROFILING_COUNTER_gnpsLtoSnpu3Send    ANSC_PROFILING_COUNTER_gnpsBase     + 8
#define  ANSC_PROFILING_COUNTER_gnpsPdoRecv         ANSC_PROFILING_COUNTER_gnpsBase     + 9
#define  ANSC_PROFILING_COUNTER_gnpsLcoL2nRecvQ     ANSC_PROFILING_COUNTER_gnpsBase     + 10

#define  ANSC_PROFILING_COUNTER_snpu3Base           ANSC_PROFILING_COUNTER_gnpsBase     + 16
#define  ANSC_PROFILING_COUNTER_snpu3DpcProc        ANSC_PROFILING_COUNTER_snpu3Base
#define  ANSC_PROFILING_COUNTER_snpu3DpcFastPath    ANSC_PROFILING_COUNTER_snpu3Base    + 1
#define  ANSC_PROFILING_COUNTER_snpu3GetCircuit     ANSC_PROFILING_COUNTER_snpu3Base    + 2
#define  ANSC_PROFILING_COUNTER_snpu3PathSetup      ANSC_PROFILING_COUNTER_snpu3Base    + 3
#define  ANSC_PROFILING_COUNTER_snpu3DpcGetCircuit  ANSC_PROFILING_COUNTER_snpu3Base    + 4
#define  ANSC_PROFILING_COUNTER_snpu3DpcGetCircuitQ ANSC_PROFILING_COUNTER_snpu3Base    + 5

#define  ANSC_PROFILING_COUNTER_ip4Base             ANSC_PROFILING_COUNTER_snpu3Base    + 16
#define  ANSC_PROFILING_COUNTER_ip4Snpu3Transform   ANSC_PROFILING_COUNTER_ip4Base

#define  ANSC_PROFILING_COUNTER_ip4sBase            ANSC_PROFILING_COUNTER_ip4Base      + 16
#define  ANSC_PROFILING_COUNTER_ip4sShoIsLocalPkt   ANSC_PROFILING_COUNTER_ip4Base

#define  ANSC_PROFILING_COUNTER_bondBase            ANSC_PROFILING_COUNTER_ip4sBase     + 16
#define  ANSC_PROFILING_COUNTER_bondSmcSwitchL2     ANSC_PROFILING_COUNTER_bondBase
#define  ANSC_PROFILING_COUNTER_bondGnpsLpcRecv     ANSC_PROFILING_COUNTER_bondBase     + 1
#define  ANSC_PROFILING_COUNTER_bondGnpsLpcSend     ANSC_PROFILING_COUNTER_bondBase     + 2
#define  ANSC_PROFILING_COUNTER_bondSnpu3LpcRecv    ANSC_PROFILING_COUNTER_bondBase     + 3
#define  ANSC_PROFILING_COUNTER_bondSnpu3LpcSend    ANSC_PROFILING_COUNTER_bondBase     + 4

#define  ANSC_PROFILING_COUNTER_dslgBase            ANSC_PROFILING_COUNTER_bondBase     + 16
#define  ANSC_PROFILING_COUNTER_dslgSmcSwitchL2     ANSC_PROFILING_COUNTER_dslgBase
#define  ANSC_PROFILING_COUNTER_dslgGnpsLpcRecv     ANSC_PROFILING_COUNTER_dslgBase     + 1
#define  ANSC_PROFILING_COUNTER_dslgGnpsLpcSend     ANSC_PROFILING_COUNTER_dslgBase     + 2
#define  ANSC_PROFILING_COUNTER_dslgSnpu3LpcRecv    ANSC_PROFILING_COUNTER_dslgBase     + 3
#define  ANSC_PROFILING_COUNTER_dslgSnpu3LpcSend    ANSC_PROFILING_COUNTER_dslgBase     + 4

#define  ANSC_PROFILING_COUNTER_fwBase              ANSC_PROFILING_COUNTER_dslgBase     + 16
#define  ANSC_PROFILING_COUNTER_fwExtZoneProc       ANSC_PROFILING_COUNTER_fwBase
#define  ANSC_PROFILING_COUNTER_fwIntZoneProc       ANSC_PROFILING_COUNTER_fwBase       + 1
#define  ANSC_PROFILING_COUNTER_fwSameZoneTypeProc  ANSC_PROFILING_COUNTER_fwBase       + 2
#define  ANSC_PROFILING_COUNTER_fwLocalZoneProc     ANSC_PROFILING_COUNTER_fwBase       + 3
#define  ANSC_PROFILING_COUNTER_fwTimer             ANSC_PROFILING_COUNTER_fwBase       + 4
#define  ANSC_PROFILING_COUNTER_fwSessionTblClean   ANSC_PROFILING_COUNTER_fwBase       + 5
#define  ANSC_PROFILING_COUNTER_fwBufferCopy        ANSC_PROFILING_COUNTER_fwBase       + 6
#define  ANSC_PROFILING_COUNTER_fwBufferGrow        ANSC_PROFILING_COUNTER_fwBase       + 7

#define  ANSC_PROFILING_COUNTER_ipsecBase           ANSC_PROFILING_COUNTER_fwBase       + 16
#define  ANSC_PROFILING_COUNTER_ipsecEgressProc     ANSC_PROFILING_COUNTER_ipsecBase
#define  ANSC_PROFILING_COUNTER_ipsecEgressCompl    ANSC_PROFILING_COUNTER_ipsecBase    + 1
#define  ANSC_PROFILING_COUNTER_ipsecEgressCompl2   ANSC_PROFILING_COUNTER_ipsecBase    + 2
#define  ANSC_PROFILING_COUNTER_ipsecIngressProc    ANSC_PROFILING_COUNTER_ipsecBase    + 3
#define  ANSC_PROFILING_COUNTER_ipsecIngressCompl   ANSC_PROFILING_COUNTER_ipsecBase    + 4
#define  ANSC_PROFILING_COUNTER_ipsecIngressCompl2  ANSC_PROFILING_COUNTER_ipsecBase    + 5

#define  ANSC_PROFILING_COUNTER_snpu2Base           ANSC_PROFILING_COUNTER_ipsecBase    + 16
#define  ANSC_PROFILING_COUNTER_snpu2SrvgoSnpu3Proc ANSC_PROFILING_COUNTER_snpu2Base
#define  ANSC_PROFILING_COUNTER_snpu2SrvboSnpu3Proc ANSC_PROFILING_COUNTER_snpu2Base    + 1

#define  ANSC_PROFILING_COUNTER_qosBase             ANSC_PROFILING_COUNTER_snpu2Base    + 16
#define  ANSC_PROFILING_COUNTER_qosSdfsvMeoMfc3a    ANSC_PROFILING_COUNTER_qosBase


#define  ANSC_PROFILING_TOTAL_COUNTER               ANSC_PROFILING_COUNTER_qosBase      + 16


/***********************************************************
                      DATA STRUCTURES
***********************************************************/

typedef  struct
_ANSC_PROFILING_CONTEXT
{
    BOOLEAN                         bEnabled;
    ULONG                           Calibration;
    ULONG                           CounterCurr[ANSC_PROFILING_TOTAL_COUNTER];
    ULONG                           CounterNum[ANSC_PROFILING_TOTAL_COUNTER];
    ULONG64                         CounterTotal[ANSC_PROFILING_TOTAL_COUNTER];
}
ANSC_PROFILING_CONTEXT, *PANSC_PROFILING_CONTEXT;


typedef  struct
_ANSC_PROFILING_COUNTER_NAME
{
    ULONG                           ulCounter;
    const char*                     CounterName;
}
ANSC_PROFILING_COUNTER_NAME, *PANSC_PROFILING_COUNTER_NAME;


/***********************************************************
                        OPERATIONS
***********************************************************/

#ifdef   _ANSC_ENABLE_PROFILING_

    /*
     *  All operations related to Processor Cycle Counter are processor/platform specific
     */

        #define  AnscProfilingPccFactor                 UserProfilingPccFactor

        #define  AnscProfilingStartPcc()                UserProfilingStartPcc()
        #define  AnscProfilingStopPcc()                 UserProfilingStopPcc()
        #define  AnscProfilingReadPcc()                 UserProfilingReadPcc()


    extern   ANSC_PROFILING_CONTEXT     gAnscProfilingContext;

    void
    AnscProfilingStart
        (
            void
        );

    #define  AnscProfilingStartCounter(ulCounter)                               \
                if ( gAnscProfilingContext.bEnabled )                           \
                {                                                               \
                    gAnscProfilingContext.CounterCurr[ulCounter] =              \
                        AnscProfilingReadPcc();                                 \
                }                                                               \

    #define  AnscProfilingStopCounter(ulCounter)                                \
                if ( gAnscProfilingContext.bEnabled )                           \
                {                                                               \
                    ULONG                           interval;                   \
                                                                                \
                    interval = AnscProfilingReadPcc() -                         \
                        gAnscProfilingContext.CounterCurr[ulCounter];           \
                                                                                \
                    interval /= AnscProfilingPccFactor;                         \
                                                                                \
                    gAnscProfilingContext.CounterCurr[ulCounter]    = interval; \
                    gAnscProfilingContext.CounterTotal[ulCounter]  += interval; \
                    gAnscProfilingContext.CounterNum[ulCounter]++;              \
                }

    void
    AnscProfilingGetResult
        (
            PANSC_PROFILING_CONTEXT     pResult
        );

    void
    AnscProfilingStop
        (
            void
        );

    const char*
    AnscProfilingGetCounterName
        (
            ULONG                       ulCounter
        );

#else

    #define  AnscProfilingStart()
    #define  AnscProfilingStartCounter(ulCounter)
    #define  AnscProfilingStopCounter(ulCounter)
    #define  AnscProfilingGetResult(pResult)
    #define  AnscProfilingStop()
    #define  AnscProfilingGetCounterName(ulIndex)       "Unknown"

#endif


#endif

