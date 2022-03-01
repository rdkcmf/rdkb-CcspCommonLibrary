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

    module:	ansc_debug.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some debuging and tracing
        macros and functions.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/02/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions
        03/22/06    added trace ID/level/mask control

**********************************************************************/


#ifndef  _ANSC_DEBUG_
#define  _ANSC_DEBUG_

#include "ansc_wrapper_base.h"
#include "ansc_common_structures.h"
#include "ccsp_trace.h"

/*
 *  Whether a debug trace is output depends on the following factors:
 *      1) global debug trace is enabled or not.
 *          _DEBUG should be defined to enable debug trace in kernel/user_debug.h.
 *      2) module trace ID, ANSC_TRACE_MODULE_ID defined in each module's global header file
 *          is configured in the global trace ID table.
 *      3) the trace level passed in the trace statement is higher (smaller value)
 *          than the trace level configured for the trace ID.
 *      4) the trace mask passed in the trace statement overlaps with the trace
 *          mask configured for the trace ID.
 */

#ifdef   _DEBUG
    #define  ANSC_DEBUG_ENABLED
#endif

/*
 *  the custom trace ID header file defines:
 *      1) platform-specific trace IDs
 *      2) default Custom Trace ID configuration: level/mask configured for each trace ID
 *      3) custom ID name table
 */
#include "ansc_debug_custom.h"
#include "user_base.h"

/***********************************************************
                   CONSTANT DEFINITIONS
***********************************************************/

/*
 *  trace IDs -- the type is USHORT and it has to be contiguous numbers.
 *
 *  An array will be used to hold the trace IDs.
 */
#define  ANSC_TRACE_ID_DEFAULT                      0
#define  ANSC_TRACE_ID_PLATFORM_MEMORY              1
#define  ANSC_TRACE_ID_PLATFORM_TASK                2
#define  ANSC_TRACE_ID_PLATFORM_PROTECTION          3
#define  ANSC_TRACE_ID_PLATFORM_PACKET              4
#define  ANSC_TRACE_ID_PLATFORM_STREE               5
#define  ANSC_TRACE_ID_PLATFORM_TIME                6
#define  ANSC_TRACE_ID_PLATFORM_OTHER               7
#define  ANSC_TRACE_ID_WRAPPER_CRYPTO               8

#define  ANSC_TRACE_ID_MESSAGE_BUS                  9
#define  ANSC_TRACE_ID_BASEIF                       10
#define  ANSC_TRACE_ID_DM_LIB                       11
#define  ANSC_TRACE_ID_DM_API                       12

#define  ANSC_TRACE_ID_COMPONENT                    13

#define  ANSC_TRACE_ID_SSP                          14

/* add new ones below and replace ANSC_TRACE_ID_SSP in ANSC_TRACE_ID_DFT_MAX */


#define  ANSC_TRACE_ID_DFT_MAX                      ANSC_TRACE_ID_SSP
#define  ANSC_TRACE_ID_CUSTOM_START                 ANSC_TRACE_ID_DFT_MAX + 1

#define  ANSC_TRACE_INVALID_ID                      0xFFFF

/*
 *  trace level
 */
#if defined(FEATURE_SUPPORT_SYSLOG)

#define  ANSC_TRACE_LEVEL_DEATH                     CCSP_TRACE_LEVEL_ALERT           /* system is going to die                               */
#define  ANSC_TRACE_LEVEL_CRITICAL                  CCSP_TRACE_LEVEL_CRITICAL           /* system is unable to perform its functionality        */
#define  ANSC_TRACE_LEVEL_ERROR                     CCSP_TRACE_LEVEL_ERROR           /* handled errors                                       */
#define  ANSC_TRACE_LEVEL_WARNING                   CCSP_TRACE_LEVEL_WARNING           /* warning about abnormal situation -- default level    */

#define  ANSC_TRACE_LEVEL_TEST                      CCSP_TRACE_LEVEL_NOTICE           /* For testing purpose -- introduce debug trace on 
                                                                    specific points, with minimal impact on timing      */
#define  ANSC_TRACE_LEVEL_FLOW                      CCSP_TRACE_LEVEL_NOTICE          /* trace showing the processing flow, 
                                                                    usually major routine entry/exit                    */
#define  ANSC_TRACE_LEVEL_ENTRY                     CCSP_TRACE_LEVEL_NOTICE          /* trace for regular routine entry/exit                 */
#define  ANSC_TRACE_LEVEL_INFO                      CCSP_TRACE_LEVEL_INFO          /* informational trace in regular routines              */
#define  ANSC_TRACE_LEVEL_VERBOSE                   CCSP_TRACE_LEVEL_DEBUG          /* say whatever you like, level 1                       */
#define  ANSC_TRACE_LEVEL_VERBOSE2                  CCSP_TRACE_LEVEL_DEBUG          /* say whatever you like, level 2                       */
#define  ANSC_TRACE_LEVEL_DUMPING                   CCSP_TRACE_LEVEL_DEBUG          /* dumping everything                                   */

#define  ANSC_TRACE_INVALID_LEVEL                   0xFFFF

#else

#define  ANSC_TRACE_LEVEL_DEATH                     1           /* system is going to die                               */
#define  ANSC_TRACE_LEVEL_CRITICAL                  2           /* system is unable to perform its functionality        */
#define  ANSC_TRACE_LEVEL_ERROR                     4           /* handled errors                                       */
#define  ANSC_TRACE_LEVEL_WARNING                   6           /* warning about abnormal situation -- default level    */

#define  ANSC_TRACE_LEVEL_TEST                      8           /* For testing purpose -- introduce debug trace on 
                                                                    specific points, with minimal impact on timing      */
#define  ANSC_TRACE_LEVEL_FLOW                      10          /* trace showing the processing flow, 
                                                                    usually major routine entry/exit                    */
#define  ANSC_TRACE_LEVEL_ENTRY                     11          /* trace for regular routine entry/exit                 */
#define  ANSC_TRACE_LEVEL_INFO                      12          /* informational trace in regular routines              */
#define  ANSC_TRACE_LEVEL_VERBOSE                   24          /* say whatever you like, level 1                       */
#define  ANSC_TRACE_LEVEL_VERBOSE2                  26          /* say whatever you like, level 2                       */
#define  ANSC_TRACE_LEVEL_DUMPING                   32          /* dumping everything                                   */

#define  ANSC_TRACE_INVALID_LEVEL                   0xFFFF

#endif

/*
 *  debug mask
 */
#define  ANSC_TRACE_MASK_INIT                       0x00000001  /* trace for module initialization                      */
#define  ANSC_TRACE_MASK_CFG                        0x00000002  /* trace for configuration                              */
#define  ANSC_TRACE_MASK_CONTROL_PATH               0x00000010  /* trace for control path -- slow packet processing path,
                                                                    which includes data path setup.                     */ 
#define  ANSC_TRACE_MASK_DATA_PATH                  0x00000020  /* trace for data path -- it is on fast packet processing
                                                                    path, be aware of the latency introduced!           */
#define  ANSC_TRACE_MASK_MONITOR                    0x00000100  /* trace for monitoring function to periodically print
                                                                    debug trace                                         */
#define  ANSC_TRACE_MASK_ALL                        0xFFFFFFFFu

#define  ANSC_TRACE_INVALID_MASK                    0

/***********************************************************
                     GLOBAL VARIABLES
***********************************************************/

typedef  struct
_ANSC_TRACE_ID_CFG
{
    USHORT                          Id;
    USHORT                          Level;
    ULONG                           Mask;
}
ANSC_TRACE_ID_CFG, *PANSC_TRACE_ID_CFG;

typedef  struct
_ANSC_TRACE_VALUE_NAME
{
    union
    {
        ULONG                       Id;
        ULONG                       Level;
        ULONG                       Mask;
    }Value;

    char*                           pName;
}
ANSC_TRACE_VALUE_NAME, *PANSC_TRACE_VALUE_NAME;

/*
 *  If gTraceIdArray[id].ID doesn't match id, this TRACE ID is not configured.
 *
 *  gTraceIdArray is initialized in ansc_debug.c. It also includes
 *  the default TRACE ID configuration, ANSC_TRACE_CUSTOM_ID_CFG,
 *  from ansc_trace_custom.h.
 */
extern   ANSC_TRACE_ID_CFG                          gTraceIdArray[];

/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 *  assert facility
 */
    #define  AnscAssert                             UserAssert

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */

    #ifdef  ANSC_DEBUG_ENABLED
        #ifdef  _ANSC_TRACE_F_
            #define  AnscTraceWrapper               AnscTraceF
        #else
            #define  AnscTraceWrapper               UserTrace
        #endif
    #else
        #define  AnscTraceWrapper                   0 &&
    #endif

    #define  ANSC_TRACE_PLATFORM_MEMORY             USER_TRACE_PLATFORM_MEMORY
    #define  ANSC_TRACE_PLATFORM_TASK               USER_TRACE_PLATFORM_TASK



/***********************************************************
              ID/LEVEL/MASK TRACE FACILITY
***********************************************************/

/*
 * Instead of using a single compiling-time definition to turn on/off the debuging/tracing ability,
 * we define a set of BOOLEAN values to dynamically control the output. While this doesn't make any
 * difference for ANSC platform functions, it does benefit Container-managed Objects.
 */
#ifndef  ANSC_TRACE_MODULE_ID
    #define  ANSC_TRACE_MODULE_ID                   ANSC_TRACE_ID_DEFAULT
#endif

#ifdef   ANSC_DEBUG_ENABLED

    #define  AnscTraceIsOn(level)                   (_ANSC_TRACE_DFT_LEVEL_ >= level)

    #define  AnscTraceCheck(id, level, mask)                                                    \
                ( (level <= _ANSC_TRACE_DFT_LEVEL_) && (mask & _ANSC_TRACE_DFT_MASK_)           \
                  && ( (id == gTraceIdArray[id].Id)                                             \
                       && (level <= gTraceIdArray[id].Level)                                    \
                       && (mask & gTraceIdArray[id].Mask) ) )

    #define  AnscTraceCheck2(level, mask)                                                       \
                ( (level <= _ANSC_TRACE_DFT_LEVEL_) && (mask & _ANSC_TRACE_DFT_MASK_)           \
                  && ( (ANSC_TRACE_MODULE_ID == gTraceIdArray[ANSC_TRACE_MODULE_ID].Id)         \
                       && (level <= gTraceIdArray[ANSC_TRACE_MODULE_ID].Level)                  \
                       && (mask & gTraceIdArray[ANSC_TRACE_MODULE_ID].Mask) ) )

#if defined(FEATURE_SUPPORT_SYSLOG)

    #define  AnscTraceExec(id, level, mask, msg)                                                \
                CcspTraceExec(pComponentName, level, msg)

    #if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_FLOW)
        #define  AnscTrace(msg...)                                                                  \
                    if ( (ANSC_TRACE_MODULE_ID == gTraceIdArray[ANSC_TRACE_MODULE_ID].Id)           \
                          && (ANSC_TRACE_LEVEL_FLOW <= gTraceIdArray[ANSC_TRACE_MODULE_ID].Level) ) \
                 CcspTraceExec(pComponentName, ANSC_TRACE_LEVEL_WARNING, (msg))
    #else
        #define  AnscTrace							0 && 
    #endif

#else
    #define  AnscTraceExec(id, level, mask, msg)                                                \
                if ( AnscTraceCheck(id, level, mask) )                                          \
                {                                                                               \
                    AnscTraceWrapper msg;                                                       \
                }

    #if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_FLOW)
        #define  AnscTrace                                                                         \
                    if ( (ANSC_TRACE_MODULE_ID == gTraceIdArray[ANSC_TRACE_MODULE_ID].Id)           \
                          && (ANSC_TRACE_LEVEL_FLOW <= gTraceIdArray[ANSC_TRACE_MODULE_ID].Level) ) \
                    AnscTraceWrapper
    #else
        #define  AnscTrace							0 && 
    #endif

#endif

#else
    #undef   _ANSC_TRACE_DFT_LEVEL_
    #undef   _ANSC_TRACE_DFT_MASK_

    #define  _ANSC_TRACE_DFT_LEVEL_                 0
    #define  _ANSC_TRACE_DFT_MASK_                  0

    #define  AnscTraceIsOn(level)                   0

    #define  AnscTraceCheck(id, level, mask)        FALSE
    #define  AnscTraceExec(id, level, mask, msg)
    #define  AnscTrace                              0 &&
#endif


/*
 *  Use the following format to completely compile out debug trace statements when needed
 */
#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_DEATH)
    #define  AnscTraceDeath(msg)                    AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_DEATH,     ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceDeath(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_ERROR)
    #define  AnscTraceError(msg)                    AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_ERROR,     ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceError(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_WARNING)
    #define  AnscTraceWarning(msg)                  AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_WARNING,   ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceWarning(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_TEST)
    #define  AnscTraceTest(msg)                     AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_TEST,      ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceTest(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_FLOW)
    #define  AnscTraceFlow(msg)                     AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_FLOW,      ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceFlow(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_ENTRY)
    #define  AnscTraceEntry(msg)                    AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_ENTRY,     ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceEntry(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_INFO)
    #define  AnscTraceInfo(msg)                     AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_INFO,      ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceInfo(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_VERBOSE)
    #define  AnscTraceVerbose(msg)                  AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_VERBOSE,   ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceVerbose(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_VERBOSE2)
    #define  AnscTraceVerbose2(msg)                 AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_VERBOSE2,  ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceVerbose2(msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_DUMPING)
    #define  AnscTraceDumping(msg)                  AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_DUMPING,   ANSC_TRACE_MASK_ALL, msg)
#else
    #define  AnscTraceDumping(msg)
#endif



#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_DEATH)
    #define  AnscTraceDeathM(mask, msg)             AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_DEATH,     mask, msg)
#else
    #define  AnscTraceDeathM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_ERROR)
    #define  AnscTraceErrorM(mask, msg)             AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_ERROR,     mask, msg)
#else
    #define  AnscTraceErrorM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_WARNING)
    #define  AnscTraceWarningM(mask, msg)           AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_WARNING,   mask, msg)
#else
    #define  AnscTraceWarningM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_TEST)
    #define  AnscTraceTestM(mask, msg)              AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_TEST,      mask, msg)
#else
    #define  AnscTraceTestM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_FLOW)
    #define  AnscTraceFlowM(mask, msg)              AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_FLOW,      mask, msg)
#else
    #define  AnscTraceFlowM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_ENTRY)
    #define  AnscTraceEntryM(mask, msg)             AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_ENTRY,     mask, msg)
#else
    #define  AnscTraceEntryM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_INFO)
    #define  AnscTraceInfoM(mask, msg)              AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_INFO,      mask, msg)
#else
    #define  AnscTraceInfoM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_VERBOSE)
    #define  AnscTraceVerboseM(mask, msg)           AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_VERBOSE,   mask, msg)
#else
    #define  AnscTraceVerboseM(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_VERBOSE2)
    #define  AnscTraceVerbose2M(mask, msg)          AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_VERBOSE2,  mask, msg)
#else
    #define  AnscTraceVerbose2M(mask, msg)
#endif

#if      (_ANSC_TRACE_DFT_LEVEL_ >= ANSC_TRACE_LEVEL_DUMPING)
    #define  AnscTraceDumpingM(mask, msg)           AnscTraceExec(ANSC_TRACE_MODULE_ID, ANSC_TRACE_LEVEL_DUMPING,   mask, msg)
#else
    #define  AnscTraceDumpingM(mask, msg)
#endif


#define  AnscTraceLM(level, mask, msg)              AnscTraceExec(ANSC_TRACE_MODULE_ID, level, mask, msg)
#define  AnscTraceILM(id, level, mask, msg)         AnscTraceExec(id, level, mask, msg)

/***********************************************************
                    FUNCTION PROTOTYPES
***********************************************************/

void
AnscTrace1
    (
        char*                       pFileName,
        char*                       pDebugStr,
        ...
    );

void
AnscTrace2
    (
        ULONG                       ulOid,
        char*                       pDebugStr,
        ...
    );

void
AnscTrace3
    (
        char*                       pTraceId,
        char*                       pDebugStr,
        ...
    );

void
AnscPrintBinaryMessage
    (
        PVOID                       buffer,
        ULONG                       ulSize
    );

// Assume it is not needed now.
/*void
AnscDumpMemory
    (
        PUCHAR                      pMem,
        ULONG                       ulSize,
        ULONG                       Format
    );
*/
void
AnscTraceIpPacket
    (
        ANSC_HANDLE                 hIpPacket
    );

/*
__inline  static  void
AnscTraceF(char*  pDebugStr, ...)
{
    char*                           pTempChar = (char*)AnscAllocateMemory(1024);
    va_list				            val;

    if ( !pTempChar )
    {
        return;
    }
    else
    {
        va_start(val, pDebugStr);
        _ansc_vsprintf(pTempChar, pDebugStr, val);
        va_end(val);
    }

    AnscTrace1(__FILE__, pTempChar);

    AnscFreeMemory(pTempChar);

    return;
}
 */
 
#ifdef  _ANSC_TRACE_F_
__inline  static  void
AnscTraceF(char*  pDebugStr, ...)
{
    /* TraceF is obsoleted */
    UNREFERENCED_PARAMETER(pDebugStr);
    return;
}
#endif

ANSC_STATUS
AnscTraceSetIdCfg
    (
        PANSC_TRACE_ID_CFG          pIdCfg
    );

ANSC_STATUS
AnscTraceSetAllIdcfgLevels
    (
        ULONG level
    );


ANSC_STATUS
AnscTraceGetIdCfg
    (
        PANSC_TRACE_ID_CFG          pIdCfg
    );

ANSC_STATUS
AnscTraceGetIdCfgs
    (
        PULONG                      pulCount,
        PANSC_TRACE_ID_CFG*         ppIdCfg
    );

char*
AnscTraceIdToName
    (
        USHORT                      usId
    );

USHORT
AnscTraceNameToId
    (
        char*                       pName
    );

char*
AnscTraceLevelToName
    (
        USHORT                      usLevel
    );

USHORT
AnscTraceNameToLevel
    (
        char*                       pName
    );

char*
AnscTraceMaskToName
    (
        USHORT                      ulMask
    );

ULONG
AnscTraceNameToMask
    (
        char*                       pName
    );


#endif

