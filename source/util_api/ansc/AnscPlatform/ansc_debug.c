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

    module:	ansc_debug.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to debugging and tracing.

        *   AnscTrace1
        *   AnscTrace2
        *   AnscTrace3
        *   AnscPrintBinaryMessage
        *   AnscPrintMultiBytes
        *   AnscDumpMemory
        *   AnscTraceSetIdCfg
        *   AnscTraceGetIdCfg
        *   AnscTraceGetIdCfgs
        *   AnscTraceIdToName
        *   AnscTraceNameToId
        *   AnscTraceLevelToName
        *   AnscTraceNameToLevel
        *   AnscTraceMaskToName
        *   AnscTraceNameToMask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/10/02    initial revision.
        03/22/06    added trace ID/level/mask control

**********************************************************************/


#include "ansc_global.h"


static  BOOL  g_bSelectModuleName = FALSE;
static  BOOL  g_bSelectModuleOid  = FALSE;
static  BOOL  g_bSelectTraceId    = FALSE;

static  char*
g_tModuleNameTable[] =
{
    "ansc",
    "ip4s",
    "snmp"
};

static  ULONG
g_tModuleOidTable[] =
{
    IP4S_COMPONENT_OID_BASE,
    SNMP_COMPONENT_OID_BASE
};

static  char*
g_tTraceIdTable[] =
{
    "test"
};


/*
 *  This array remembers the ID to Name mappings.
 */
ANSC_TRACE_ID_CFG                   gTraceIdArray[]         =
{
    {ANSC_TRACE_ID_DEFAULT            , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_MEMORY    , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_TASK      , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_PROTECTION, ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_PACKET    , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_STREE     , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_TIME      , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_PLATFORM_OTHER     , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_WRAPPER_CRYPTO     , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},

    {ANSC_TRACE_ID_MESSAGE_BUS        , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_BASEIF             , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_DM_LIB             , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    {ANSC_TRACE_ID_DM_API             , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},

    {ANSC_TRACE_ID_COMPONENT          , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},
    
    {ANSC_TRACE_ID_SSP                , ANSC_TRACE_LEVEL_WARNING, ANSC_TRACE_MASK_ALL},

    ANSC_TRACE_CUSTOM_ID_CFG
};

static   ULONG                      gTraceMaxId             = sizeof(gTraceIdArray)/sizeof(gTraceIdArray[0]);

static   ANSC_TRACE_VALUE_NAME      gTraceIdNameArray[]     =
{
    {{ANSC_TRACE_ID_DEFAULT}            , "default"       },
    {{ANSC_TRACE_ID_PLATFORM_MEMORY}    , "memory"        },
    {{ANSC_TRACE_ID_PLATFORM_TASK}      , "task"          },
    {{ANSC_TRACE_ID_PLATFORM_PROTECTION}, "protection"    },
    {{ANSC_TRACE_ID_PLATFORM_PACKET}    , "packet"        },
    {{ANSC_TRACE_ID_PLATFORM_STREE}     , "stree"         },
    {{ANSC_TRACE_ID_PLATFORM_TIME}      , "time"          },
    {{ANSC_TRACE_ID_PLATFORM_OTHER}     , "otherwrapper"  },
    {{ANSC_TRACE_ID_WRAPPER_CRYPTO }    , "wrappercrypto" },

    {{ANSC_TRACE_ID_MESSAGE_BUS}        , "msgbus"        },
    {{ANSC_TRACE_ID_BASEIF}             , "baseif"        },
    {{ANSC_TRACE_ID_DM_LIB}             , "dmlib"         },
   {{ANSC_TRACE_ID_DM_API}             , "dmapi"         },

    {{ANSC_TRACE_ID_COMPONENT}          , "component"     },
    
    {{ANSC_TRACE_ID_SSP}                , "ssp"           },

    ANSC_TRACE_CUSTOM_ID_NAMES
};

static   ANSC_TRACE_VALUE_NAME      gTraceLevelNameArray[]  =
{
    {{ANSC_TRACE_LEVEL_DEATH}   , "death"     },
    {{ANSC_TRACE_LEVEL_CRITICAL}, "critical"  },
    {{ANSC_TRACE_LEVEL_ERROR}   , "error"     },
    {{ANSC_TRACE_LEVEL_WARNING} , "warning"   },

    {{ANSC_TRACE_LEVEL_TEST}    , "test"      },
                         
    {{ANSC_TRACE_LEVEL_FLOW}    , "flow"      },
                         
    {{ANSC_TRACE_LEVEL_ENTRY}   , "entry"     },
    {{ANSC_TRACE_LEVEL_INFO}    , "info"      },
    {{ANSC_TRACE_LEVEL_VERBOSE} , "verbose"   },
    {{ANSC_TRACE_LEVEL_VERBOSE2}, "verbose2"  },
    {{ANSC_TRACE_LEVEL_DUMPING} , "dumping"   }
};

static   ANSC_TRACE_VALUE_NAME      gTraceMaskNameArray[]   =
{
    {{ANSC_TRACE_MASK_INIT}        , "init"       },
    {{ANSC_TRACE_MASK_CFG}         , "cfg"        },
    {{ANSC_TRACE_MASK_CONTROL_PATH}, "controlpath"},
                            
    {{ANSC_TRACE_MASK_DATA_PATH}   , "datapath"   },
                            
    {{ANSC_TRACE_MASK_MONITOR}     , "monitor"    },
                            
    {{ANSC_TRACE_MASK_ALL}         , "all"        }
};


void
AnscTrace1
    (
        char*                       pFileName,
        char*                       pDebugStr,
        ...
    )
{
    BOOL                            bModuleMatched = FALSE;
    ULONG                           ulModuleCount  = sizeof(g_tModuleNameTable) / sizeof(char*);
    ULONG                           i              = 0;

    if ( g_bSelectModuleName )
    {
        for ( i = 0; i < ulModuleCount; i++ )
        {
            if ( (AnscSizeOfString(pFileName) >= AnscSizeOfString(g_tModuleNameTable[i])) &&
                 AnscEqualString2
                    (
                        pFileName,
                        g_tModuleNameTable[i],
                        AnscSizeOfString(g_tModuleNameTable[i]),
                        FALSE
                    ) )
            {
                bModuleMatched = TRUE;

                break;
            }
        }
    }
    else
    {
        bModuleMatched = TRUE;
    }

    if ( bModuleMatched )
    {
        char*                       pTempChar = (char*)AnscAllocateMemory(1024);
        va_list				        val;

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

            UserTrace  (pTempChar);

        AnscFreeMemory(pTempChar);
    }

    return;
}


void
AnscTrace2
    (
        ULONG                       ulOid,
        char*                       pDebugStr,
        ...
    )
{
    BOOL                            bModuleMatched = FALSE;
    ULONG                           ulModuleCount  = sizeof(g_tModuleOidTable) / sizeof(ULONG);
    ULONG                           i              = 0;

    if ( g_bSelectModuleOid )
    {
        for ( i = 0; i < ulModuleCount; i++ )
        {
            if ( (ulOid >= g_tModuleOidTable[i]             ) &&
                 (ulOid <  g_tModuleOidTable[i] + 0x00010000) )
            {
                bModuleMatched = TRUE;

                break;
            }
        }
    }
    else
    {
        bModuleMatched = TRUE;
    }

    if ( bModuleMatched )
    {
        char*                       pTempChar = (char*)AnscAllocateMemory(1024);
        va_list				        val;

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

            UserTrace  (pTempChar);

        AnscFreeMemory(pTempChar);
    }

    return;
}


void
AnscTrace3
    (
        char*                       pTraceId,
        char*                       pDebugStr,
        ...
    )
{
    BOOL                            bTraceIdMatched = FALSE;
    ULONG                           ulTraceIdCount  = sizeof(g_tTraceIdTable) / sizeof(char*);
    ULONG                           i               = 0;

    if ( g_bSelectTraceId )
    {
        for ( i = 0; i < ulTraceIdCount; i++ )
        {
            if ( (AnscSizeOfString(pTraceId) >= AnscSizeOfString(g_tTraceIdTable[i]))&&
                 AnscEqualString2
                    (
                        pTraceId,
                        g_tTraceIdTable[i],
                        AnscSizeOfString(g_tTraceIdTable[i]),
                        FALSE
                    ) )
            {
                bTraceIdMatched = TRUE;

                break;
            }
        }
    }
    else
    {
        bTraceIdMatched = TRUE;
    }

    if ( bTraceIdMatched )
    {
        char*                       pTempChar = (char*)AnscAllocateMemory(1024);
        va_list				        val;

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

            UserTrace  (pTempChar);

        AnscFreeMemory(pTempChar);
    }

    return;
}


void
AnscPrintBinaryMessage
    (
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PUCHAR                          hex_stream = (PUCHAR)buffer;
    ULONG                           i          = 0;
    ULONG                           j          = 0;

    AnscTrace("   ***** binary message size %04lu bytes: *****    \n", ulSize);

    for ( i = 0; i < ulSize / 16; i++ )
    {
        AnscTrace
            (
                "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                hex_stream[i * 16 + 0 ],
                hex_stream[i * 16 + 1 ],
                hex_stream[i * 16 + 2 ],
                hex_stream[i * 16 + 3 ],
                hex_stream[i * 16 + 4 ],
                hex_stream[i * 16 + 5 ],
                hex_stream[i * 16 + 6 ],
                hex_stream[i * 16 + 7 ],
                hex_stream[i * 16 + 8 ],
                hex_stream[i * 16 + 9 ],
                hex_stream[i * 16 + 10],
                hex_stream[i * 16 + 11],
                hex_stream[i * 16 + 12],
                hex_stream[i * 16 + 13],
                hex_stream[i * 16 + 14],
                hex_stream[i * 16 + 15]
            );
    }

    if ( ulSize % 16 )
    {
        for ( j = 0; j < ulSize % 16; j++ )
        {
            AnscTrace("%02X ", hex_stream[i * 16 + j]);
        }

        AnscTrace("\n");
    }

    AnscTrace("   ***** ***************************** *****    \n\n");

    return;
}


/**********************************************************************

    caller:

    prototype:

        void
        AnscPrintMultiBytes
            (
                OUT PCHAR                   str,
                IN  PUCHAR                  pBytes,
                IN  USHORT                  count,
                IN  UCHAR                   seperator,
                IN  UCHAR                   Mode
            );

    description:

        Print multi bytes to string in hexdecimal format and in
        the order/reversed-order they appear in the memory
        The last seperator behind string won't be printed out

    arguments:  PCHAR                       str
                Printing buffer for output

                PUCHAR                      pBytes
                Pointer to the bytes to be printed out

                USHORT                      count
                length of the string to be printed out

                UCHAR                       seperator
                Character for seperating

                UCHAR                       Mode
                Bit 0:      1 -- Reverse
                            0 -- Normal

    return:     none

**********************************************************************/

static  void
AnscPrintMultiBytes
    (
        char*                   str,
        PUCHAR                  pBytes,
        USHORT                  count,
        UCHAR                   seperator,
        UCHAR                   Mode
    )
{
    /*
     *    Holds the current byte
     */
    UCHAR                           CurrByte;
    /*
     *    Holds the current 4-digit binary
     */
    UCHAR                           CurrBin;
    /*
     *    Holds the current printing position
     */
    PCHAR                           pCurrPos    = str;
    /*
     *    Temporary Counter
     */
    USHORT                          index;
    /*
     *    Operation Mode
     */
    BOOLEAN                         Reverse     = Mode & 0x01;    /* Bit 0 of Mode is Reverse */

    if( Reverse )
    {
        index = count - 1;
    }
    else
    {
        index = 0;
    }

    while( TRUE )
    {
        CurrByte = pBytes[ index ];

        /*
         *      Higher 4-digit
         */
        CurrBin = ( CurrByte & 0xF0 ) >> 4;
        if ( CurrBin >= 0x0A )
        {
            *pCurrPos = 'A' + CurrBin - 0x0A;
        }
        else
        {
            *pCurrPos = '0' + CurrBin;
        }
        pCurrPos++;

        /*
         *      Lower 4-digit
         */
        CurrBin = CurrByte & 0x0F;
        if ( CurrBin >= 0x0A )
        {
            *pCurrPos = 'A' + CurrBin - 0x0A;
        }
        else
        {
            *pCurrPos = '0' + CurrBin;
        }
        pCurrPos++;

        /*
         *  check the loopEnd condition before handling seperator
         *  to prevent the last additional seperator showing up
         */
        if ( Reverse )
        {
            if ( index == 0 )
            {       /*Loop end*/
                    break;
            }
            index--;
        }
        else
        {
            index++;
            if( index < count )
            {
                    /*Continue loop*/
            }
            else
            {       /*loop end*/
                    break;
            }
        }

        if ( seperator != 0 )
        {
            *pCurrPos = seperator;
            pCurrPos++;
        }

    }   /*while( TRUE )*/

    /*
     *  Append the null terminater
     */
    *pCurrPos = 0;

    return;
}


/**********************************************************************

    caller:

    prototype:

        void
        AnscDumpMemory
            (
                PUCHAR                      pMem,
                ULONG                       ulSize,
                ULONG                       Format
            );

    description:

        Dump the designated memory in the format of 16 bytes per row
        to debug window.

    arguments:  PUCHAR                      pMem
                Pointer to the memory starting point.

                ULONG                       ulSize
                Size of the memory to print out.

                ULONG                       Format
                Reserved for future use.

    return:     none

**********************************************************************/

void
AnscDumpMemory
    (
        PUCHAR                      pMem,
        ULONG                       ulSize,
        ULONG                       Format
    )
{
    UNREFERENCED_PARAMETER(Format);
    /**************************************************************
                            Local Variables
    **************************************************************/
    UCHAR                           bStep               = 16;
    PUCHAR                          pCurr;
    UCHAR                           CurrByte;
    char                            PrintBuffer[128]    = {0};
    char                            CharBuffer[24]      = {0};
    ULONG                           Index;
    ULONG                           i;

    /**************************************************************
                            Code Section
    **************************************************************/

    pCurr = pMem;

    for( Index = 0; Index < ulSize/bStep; Index ++ )
    {
        /*
         *      Print the address
         */
#ifdef _64BIT_ARCH_SUPPORT_
        _ansc_sprintf(PrintBuffer, "%p", pCurr);
#else
        _ansc_sprintf(PrintBuffer, "0x%08X", (unsigned int)pCurr);
#endif
        _ansc_strcat (PrintBuffer, ": ");

        /*
         *      Print the memory image
         */
        AnscPrintMultiBytes( PrintBuffer + _ansc_strlen(PrintBuffer), pCurr, bStep, ' ', FALSE );

        for( i = 0; i < bStep; i++ )
        {
            CurrByte = *pCurr++;

            if ( CurrByte == '%' )                              /* escape the character '%' */
            {
                CharBuffer[ i ] = '.';
            }
            else if ( (CurrByte >= 0x20) && (CurrByte < 128) )  /* Non control charactor */
            {
                CharBuffer[ i ] = CurrByte;
            }
            else
            {
                CharBuffer[ i ] = '.';
            }
        }

        _ansc_strcat( PrintBuffer, " " );
        AnscTraceWrapper(PrintBuffer);

        CharBuffer[i    ] = '\r';
        CharBuffer[i + 1] = '\n';
        CharBuffer[i + 2] = 0;
        AnscTraceWrapper(CharBuffer);

    }   /*for( Index = 0; Index < ulSize/bStep; Index +=bStep )*/

    if ( pCurr < pMem+ulSize )
    {
        /*
         *      Print the address
         */
#ifdef _64BIT_ARCH_SUPPORT_
        _ansc_sprintf(PrintBuffer, "%p", pCurr);
#else
        _ansc_sprintf(PrintBuffer, "0x%08X", (unsigned int)pCurr);
#endif
        _ansc_strcat (PrintBuffer, ": " );

        /*
         *      Print the memory image
         */
        for( i = 0; i < bStep; i++ )
        {
            if ( pCurr < pMem+ulSize )
            {
                CurrByte = *pCurr++;
                AnscPrintMultiBytes( PrintBuffer + _ansc_strlen( PrintBuffer ), &CurrByte, 1, ' ', FALSE );
            }
            else
            {   /*Fill in spaces*/
                CurrByte = ' ';
                _ansc_strcat( PrintBuffer, "  " );
            }

            _ansc_strcat( PrintBuffer, " " );

            if ( CurrByte == '%' )                              /* escape the character '%' */
            {
                CharBuffer[ i ] = '%';
                CharBuffer[ i ] = CurrByte;
            }
            else if ( (CurrByte >= 0x20) && (CurrByte < 128) )  /* Non control charactor */
            {
                CharBuffer[ i ] = CurrByte;
            }
            else
            {
                CharBuffer[ i ] = '.';
            }
        }   /*for( i = 0; i < bStep; i++ )*/

        AnscTraceWrapper(PrintBuffer);

        CharBuffer[i    ] = '\r';
        CharBuffer[i + 1] = '\n';
        CharBuffer[i + 2] = 0;
        AnscTraceWrapper(CharBuffer);

    }   /*if ( pCurr < pMem )*/

    return;

}    /*AnscDumpMemory*/


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscTraceIpPacket
            (
                ANSC_HANDLE                 hIpPacket
            );

    description:

        This function is called to print out IP packet info.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hIpPacket
                Specifies the IP packet.

    return:     void

**********************************************************************/

void
AnscTraceIpPacket
    (
        ANSC_HANDLE                 hIpPacket
    )
{
    PIPV4_HEADER                    pIp4Header          = (PIPV4_HEADER)hIpPacket;
    PUDP_HEADER                     pUdpHeader          = (PUDP_HEADER)AnscIpv4GetPayload(pIp4Header);
    PTCP_HEADER                     pTcpHeader          = (PTCP_HEADER)pUdpHeader;
    PICMPV4_HEADER                  pIcmpHeader;

    PUCHAR                          pTpPayload;
    ULONG                           ulTpPayloadLen;
    char                            TcpFlagStr[7]       = "      \0";

    ULONG                           ulTimeStamp;
    ULONG                           seconds;
    ULONG                           mSeconds;

    ULONG                           currentOffset       = 0;
    ULONG                           ulFragmentSize      = 0;

    /*
     *  get current time
     */
    ulTimeStamp = AnscGetTickInMilliSeconds();

    seconds     = ulTimeStamp;
    mSeconds    = ulTimeStamp % 1000;

    /*
     *  print the packet
     */
    AnscTraceWrapper
        (
            "%lu.%03lu IP: len = %d, ",
            seconds,
            mSeconds,
            AnscIpv4GetTotalLength(pIp4Header)
        );

    pIcmpHeader  = (PICMPV4_HEADER)pUdpHeader;

    if ( !AnscIpv4IsFragment(pIp4Header) || (AnscIpv4IsFragment(pIp4Header) && AnscIpv4IsFirstFragment(pIp4Header)))
    {
        if(AnscIpv4GetTransportType(pIp4Header) == TCP_TRANSPORT)
        {
            AnscTraceWrapper
                (
                    "protocol=%d, src=%d.%d.%d.%d:%d, dest=%d.%d.%d.%d:%d ",
                    AnscIpv4GetTransportType(pIp4Header),
                    AnscIpv4GetSrcAddressDot(pIp4Header)[0],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[1],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[2],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[3],
                    AnscTcpGetSrcPort(pTcpHeader),
                    AnscIpv4GetDstAddressDot(pIp4Header)[0],
                    AnscIpv4GetDstAddressDot(pIp4Header)[1],
                    AnscIpv4GetDstAddressDot(pIp4Header)[2],
                    AnscIpv4GetDstAddressDot(pIp4Header)[3],
                    AnscTcpGetDstPort(pTcpHeader)
                );

            if (AnscTcpIsUrg(pTcpHeader))
            {
                TcpFlagStr[0] = 'U';
            }
            if (AnscTcpIsAck(pTcpHeader))
            {
                TcpFlagStr[1] = 'A';
            }
            if (AnscTcpIsPsh(pTcpHeader))
            {
                TcpFlagStr[2] = 'P';
            }
            if (AnscTcpIsRst(pTcpHeader))
            {
                TcpFlagStr[3] = 'R';
            }
            if (AnscTcpIsSyn(pTcpHeader))
            {
                TcpFlagStr[4] = 'S';
            }
            if (AnscTcpIsFin(pTcpHeader))
            {
                TcpFlagStr[5] = 'F';
            }
            TcpFlagStr[6] = 0;

            AnscTraceWrapper
                (
                    " %s ",
                    TcpFlagStr
                );

            pTpPayload      = AnscTcpGetPayload(pTcpHeader);
            ulTpPayloadLen  = AnscIpv4GetTotalLength(pIp4Header) - ((ULONG)pTpPayload - (ULONG)pIp4Header);

            AnscTraceWrapper
                (
                    "Seq=%lu~%lu, Ack=%lu",
                    AnscTcpGetSeqNumber(pTcpHeader),
                    AnscTcpGetSeqNumber(pTcpHeader) + ulTpPayloadLen,
                    AnscTcpGetAckNumber(pTcpHeader)
                );
        }
        else if (AnscIpv4GetTransportType(pIp4Header) == UDP_TRANSPORT)
        {
            AnscTraceWrapper
                (
                    "protocol=%d, src=%d.%d.%d.%d:%d, dest=%d.%d.%d.%d:%d",
                    AnscIpv4GetTransportType(pIp4Header),
                    AnscIpv4GetSrcAddressDot(pIp4Header)[0],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[1],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[2],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[3],
                    AnscUdpGetSrcPort(pUdpHeader),
                    AnscIpv4GetDstAddressDot(pIp4Header)[0],
                    AnscIpv4GetDstAddressDot(pIp4Header)[1],
                    AnscIpv4GetDstAddressDot(pIp4Header)[2],
                    AnscIpv4GetDstAddressDot(pIp4Header)[3],
                    AnscUdpGetDstPort(pUdpHeader)
                );
        }
        else if (AnscIpv4GetTransportType(pIp4Header) == ICMP_TRANSPORT)
        {
            AnscTraceWrapper
                (
                    "protocol=%d, src=%d.%d.%d.%d, dest=%d.%d.%d.%d, type=%d",
                    AnscIpv4GetTransportType(pIp4Header),
                    AnscIpv4GetSrcAddressDot(pIp4Header)[0],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[1],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[2],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[3],
                    AnscIpv4GetDstAddressDot(pIp4Header)[0],
                    AnscIpv4GetDstAddressDot(pIp4Header)[1],
                    AnscIpv4GetDstAddressDot(pIp4Header)[2],
                    AnscIpv4GetDstAddressDot(pIp4Header)[3],
                    AnscIcmpv4GetType(pIcmpHeader)
                );
        }
        else
        {
            AnscTraceWrapper
                (
                    "protocol=%d, src=%d.%d.%d.%d, dest=%d.%d.%d.%d",
                    AnscIpv4GetTransportType(pIp4Header),
                    AnscIpv4GetSrcAddressDot(pIp4Header)[0],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[1],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[2],
                    AnscIpv4GetSrcAddressDot(pIp4Header)[3],
                    AnscIpv4GetDstAddressDot(pIp4Header)[0],
                    AnscIpv4GetDstAddressDot(pIp4Header)[1],
                    AnscIpv4GetDstAddressDot(pIp4Header)[2],
                    AnscIpv4GetDstAddressDot(pIp4Header)[3]
                );
        }
    }
    else
    {
        AnscTraceWrapper
            (
                "IP: protocol=%d, src=%d.%d.%d.%d, dest=%d.%d.%d.%d ",
                AnscIpv4GetTransportType(pIp4Header),
                AnscIpv4GetSrcAddressDot(pIp4Header)[0],
                AnscIpv4GetSrcAddressDot(pIp4Header)[1],
                AnscIpv4GetSrcAddressDot(pIp4Header)[2],
                AnscIpv4GetSrcAddressDot(pIp4Header)[3],
                AnscIpv4GetDstAddressDot(pIp4Header)[0],
                AnscIpv4GetDstAddressDot(pIp4Header)[1],
                AnscIpv4GetDstAddressDot(pIp4Header)[2],
                AnscIpv4GetDstAddressDot(pIp4Header)[3]
            );
    }

    if ( AnscIpv4GetTos(pIp4Header) != 0 )
    {
        AnscTraceWrapper(" Tos=0x%X", AnscIpv4GetTos(pIp4Header));
    }

    if ( AnscIpv4IsDontFragment(pIp4Header) )
    {
        AnscTraceWrapper(" DF");
    }

    AnscTraceWrapper(" TTL=%d", AnscIpv4GetTtl(pIp4Header));

    if (!AnscIpv4IsFragment(pIp4Header))
    {
        AnscTraceWrapper("\n");
    }
    else
    {
        currentOffset       = AnscIpv4GetFragmentOffset(pIp4Header) * 8;
        ulFragmentSize      = (ULONG)AnscIpv4GetTotalLength(pIp4Header) - AnscIpv4GetHeaderSize(pIp4Header);

        AnscTraceWrapper
            (
                " id = %X, %d@%d\n",
                AnscIpv4GetId(pIp4Header),
                (int)ulFragmentSize,
                (int)currentOffset
            );
    }

    return;
}


ANSC_STATUS
AnscTraceSetIdCfg
    (
        PANSC_TRACE_ID_CFG          pIdCfg
    )
{
    AnscAssert(pIdCfg != NULL);

    if ( pIdCfg->Id >= gTraceMaxId )
    {
        AnscTraceWrapper
            (
                "AnscTraceSetIdCfg -- invalid ID: %u, maximum allowed %u!\n",
                pIdCfg->Id,
                (unsigned int)gTraceMaxId
            );

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        gTraceIdArray[pIdCfg->Id] = *pIdCfg;

        return  ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
AnscTraceSetAllIdcfgLevels
    (
        ULONG level
    )
{
    int i = 0;

    for (i=0; i<(int)gTraceMaxId; i++)
        gTraceIdArray[i].Level = (level>ANSC_TRACE_INVALID_LEVEL) ? ANSC_TRACE_INVALID_LEVEL:level;
    
    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************
                      TRACE FACILITY FUNCTIONS
**********************************************************************/

ANSC_STATUS
AnscTraceGetIdCfg
    (
        PANSC_TRACE_ID_CFG          pIdCfg
    )
{
    AnscAssert(pIdCfg != NULL);

    if ( pIdCfg->Id >= gTraceMaxId )
    {
        AnscTraceWrapper
            (
                "AnscTraceGetIdCfg -- invalid ID: %u, maximum allowed %u!\n",
                 pIdCfg->Id,
                (unsigned int)gTraceMaxId
            );

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        *pIdCfg = gTraceIdArray[pIdCfg->Id];

        return  ANSC_STATUS_SUCCESS;
    }
}


ANSC_STATUS
AnscTraceGetIdCfgs
    (
        PULONG                      pulCount,
        PANSC_TRACE_ID_CFG*         ppIdCfg
    )
{
    AnscAssert(ppIdCfg != NULL);

    *pulCount = gTraceMaxId;
    *ppIdCfg  = gTraceIdArray;

    return  ANSC_STATUS_SUCCESS;
}


char*
AnscTraceIdToName
    (
        USHORT                      usId
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceIdNameArray) / sizeof(gTraceIdNameArray[0]); ulIndex++ )
    {
        if ( usId == (USHORT)gTraceIdNameArray[ulIndex].Value.Id )
        {
            return  gTraceIdNameArray[ulIndex].pName;
        }
    }

    /*DH  DEBUG*/
    AnscTraceWrapper("AnscTraceIdToName -- invalid id %d\n", usId);

    return  "unknown";
}


USHORT
AnscTraceNameToId
    (
        char*                       pName
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceIdNameArray) / sizeof(gTraceIdNameArray[0]); ulIndex++ )
    {
        if ( strcasecmp(pName, gTraceIdNameArray[ulIndex].pName) == 0 )
        {
            return  (USHORT)gTraceIdNameArray[ulIndex].Value.Id;
        }
    }

    return  ANSC_TRACE_INVALID_ID;
}


char*
AnscTraceLevelToName
    (
        USHORT                      usLevel
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceLevelNameArray) / sizeof(gTraceLevelNameArray[0]); ulIndex++ )
    {
        if ( usLevel == (USHORT)gTraceLevelNameArray[ulIndex].Value.Level )
        {
            return  gTraceLevelNameArray[ulIndex].pName;
        }
    }

    return  "unknown";
}


USHORT
AnscTraceNameToLevel
    (
        char*                       pName
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceLevelNameArray) / sizeof(gTraceLevelNameArray[0]); ulIndex++ )
    {
        if ( strcasecmp(pName, gTraceLevelNameArray[ulIndex].pName) == 0 )
        {
            return  (USHORT)gTraceLevelNameArray[ulIndex].Value.Level;
        }
    }

    return  ANSC_TRACE_INVALID_LEVEL;
}


char*
AnscTraceMaskToName
    (
        USHORT                      ulMask
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceMaskNameArray) / sizeof(gTraceMaskNameArray[0]); ulIndex++ )
    {
        if ( ulMask == gTraceMaskNameArray[ulIndex].Value.Mask )
        {
            return  gTraceMaskNameArray[ulIndex].pName;
        }
    }

    return  "unknown";
}


ULONG
AnscTraceNameToMask
    (
        char*                       pName
    )
{
    ULONG                           ulIndex;

    for ( ulIndex = 0; ulIndex < sizeof(gTraceMaskNameArray) / sizeof(gTraceMaskNameArray[0]); ulIndex++ )
    {
        if ( strcasecmp(pName, gTraceMaskNameArray[ulIndex].pName) == 0 )
        {
            return  gTraceMaskNameArray[ulIndex].Value.Mask;
        }
    }

    return  ANSC_TRACE_INVALID_MASK;
}

