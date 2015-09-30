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

    module: dslh_definitions_diagnostics.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameter model per
        TR-069 (CPE WAN Management Protocol).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Paul Klingensmith

    ---------------------------------------------------------------

    revision:

        09/07/05    initial revision.
        11/22/05    Modified all structures to support request type, id
                    and value for state instead of string.

**********************************************************************/


#ifndef  _DSLH_DEFINITIONS_DIAGNOSTICS_
#define  _DSLH_DEFINITIONS_DIAGNOSTICS_

/*
  * Each type of diagnostic supported by the DSLH model must be uniquely identified */
#define DSLH_DIAGNOSTIC_TYPE_Ip                     0
#define DSLH_DIAGNOSTIC_TYPE_WanAtmF5Lb             1
#define DSLH_DIAGNOSTIC_TYPE_WanDsl                 2
#define DSLH_DIAGNOSTIC_TYPE_Ping                   3
#define DSLH_DIAGNOSTIC_TYPE_Ppp                    4
#define DSLH_DIAGNOSTIC_TYPE_Traceroute             5
#define DSLH_DIAGNOSTIC_TYPE_NSLookup               6
#define DSLH_DIAGNOSTIC_TYPE_Download               7
#define DSLH_DIAGNOSTIC_TYPE_Upload                 8
#define DSLH_DIAGNOSTIC_TYPE_UdpEcho                9
#define DSLH_DIAGNOSTIC_TYPE_INVALID                0xFFFFFFFF

/*
 * ACS requested diagnostics must be handled differently from internally or GUI generated
 * requests. A notify message must be sent to the ACS upon completion of the diagnostic.
 */
#define DSLH_DIAGNOSTIC_REQUEST_TYPE_Reserved       0
#define DSLH_DIAGNOSTIC_REQUEST_TYPE_Acs            1
#define DSLH_DIAGNOSTIC_REQUEST_TYPE_Gui            2
#define DSLH_DIAGNOSTIC_REQUEST_TYPE_INVALID        0xFFFFFFFF

#define DSLH_DIAG_STATE_TYPE_None                   0
#define DSLH_DIAG_STATE_TYPE_Requested              1
#define DSLH_DIAG_STATE_TYPE_Complete               2
#define DSLH_DIAG_STATE_TYPE_Error_HostName         3
#define DSLH_DIAG_STATE_TYPE_Error_Internal         4
#define DSLH_DIAG_STATE_TYPE_Error_Other            5
#define DSLH_DIAG_STATE_TYPE_Error_MaxHopCount      6

#define DSLH_DIAG_STATE_TYPE_PING_Error_HostName    3
#define DSLH_DIAG_STATE_TYPE_PING_Error_Internal    4
#define DSLH_DIAG_STATE_TYPE_PING_Error_Other       5

#define DSLH_DIAG_STATE_TYPE_TRAC_Error_HostName    3
#define DSLH_DIAG_STATE_TYPE_TRAC_Error_MaxHopCount 4

#define DSLH_DIAG_STATE_TYPE_Canceled               10
#define DSLH_DIAG_STATE_TYPE_Inprogress             11
#define DSLH_DIAG_STATE_TYPE_Stop                   12

#define DSLH_DIAG_STATE_NAME_None                   "None"
#define DSLH_DIAG_STATE_NAME_Requested              "Requested"
#define DSLH_DIAG_STATE_NAME_Complete               "Complete"
#define DSLH_DIAG_STATE_NAME_Stop                   "Stop"
#define DSLH_DIAG_STATE_NAME_Error_Internal         "Error_Internal"
#define DSLH_DIAG_STATE_NAME_Error_Other            "Error_Other"
#define DSLH_DIAG_STATE_NAME_Error_HostName         "Error_CannotResolveHostName"
#define DSLH_DIAG_STATE_NAME_Error_MaxHopCount      "Error_MaxHopCountExceeded"
#define DSLH_DIAG_STATE_NAME_Canceled               "Canceled"
#define DSLH_DIAG_STATE_NAME_Error_DNSServer        "Error_DNSServerNotResolved"

#define DSLH_DIAG_INVALID_REQUEST                   0

/***********************************************************
      COMMON STRUCTURE FIELDS
***********************************************************/

#define DSLH_DIAG_INFO_BASE_CONTENT                         \
    SINGLE_LINK_ENTRY               Linkage;                \
    ULONG                           StructSize;             \
    ULONG                           DiagnosticState;        \
    ULONG                           RequestId;              \
    ULONG                           RequestType;            \
    ULONG                           DiagnosticType;         \
    BOOL                            bCanceled;              \

typedef struct
_DSLH_DIAG_INFO_BASE
{
    DSLH_DIAG_INFO_BASE_CONTENT
}
DSLH_DIAG_INFO_BASE, *PDSLH_DIAG_INFO_BASE;

#define  DslhInitDiagInfo(d_info)                                                                               \
            {                                                                                                   \
                d_info->StructSize          = sizeof(DSLH_DIAG_INFO_BASE);                                      \
                d_info->DiagnosticState     = DSLH_DIAG_STATE_TYPE_None;                                        \
                d_info->RequestId           = DSLH_DIAG_INVALID_REQUEST;                                        \
                d_info->RequestType         = DSLH_DIAGNOSTIC_REQUEST_TYPE_INVALID;                             \
                d_info->DiagnosticType      = DSLH_DIAGNOSTIC_TYPE_INVALID;                                     \
                d_info->bCanceled           = FALSE;                                                            \
            }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR IP PING
***********************************************************/

/*
 *  Diagnostic states - Names are only defined for the standard requested types in TR-69
 */
#define DSLH_PING_MAX_STRING_LENGTH_DiagnosticState 64      /* Needs to be large enough to hold the largest defined state */
#define DSLH_PING_MAX_STRING_LENGTH_Host            256
#define DSLH_PING_MAX_STRING_LENGTH_Interface       256

/* Range limits imposed by TR-69 on various parameters */
#define DSLH_PING_MIN_NumberOfRepetitions           1
#define DSLH_PING_DEFAULT_NumberOfRepetitions       3       /* 3 ICMP requests by default */
#define DSLH_PING_MIN_Timeout                       1
#define DSLH_PING_DEFAULT_Timeout                   1000    /* 100 ms timeout */
#define DSLH_PING_MIN_DataBlockSize                 1
#define DSLH_PING_DEFAULT_DataBlockSize             32      /* 32 byte payload by default */
#define DSLH_PING_MAX_DataBlockSize                 65535
#define DSLH_PING_MIN_DSCP                          0
#define DSLH_PING_DEFAULT_DSCP                      0       /* No special QoS handling by default */
#define DSLH_PING_MAX_DSCP                          64

typedef  struct
_DSLH_PING_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    char                            Interface [DSLH_PING_MAX_STRING_LENGTH_Interface];
    char                            IfAddrName[DSLH_PING_MAX_STRING_LENGTH_Interface];
    char                            Host      [DSLH_PING_MAX_STRING_LENGTH_Host     ];
    ULONG                           NumberOfRepetitions;
    ULONG                           Timeout;
    ULONG                           DataBlockSize;
    ULONG                           DSCP;

    ULONG                           SuccessCount;
    ULONG                           FailureCount;
    ULONG                           AverageResponseTime;
    ULONG                           MinimumResponseTime;
    ULONG                           MaximumResponseTime;

    ANSC_HANDLE                     hConnContext;                                       /* handle to the IP connection */
}
DSLH_PING_INFO,  *PDSLH_PING_INFO;

#define  DslhInitPingInfo(ping_info)                                                                            \
        {                                                                                                       \
            DslhInitDiagInfo(ping_info);                                                                        \
            ping_info->StructSize               = sizeof(DSLH_PING_INFO);                                       \
            ping_info->DiagnosticType           = DSLH_DIAGNOSTIC_TYPE_Ping;                                    \
            ping_info->NumberOfRepetitions      = DSLH_PING_DEFAULT_NumberOfRepetitions;                        \
            ping_info->Timeout                  = DSLH_PING_DEFAULT_Timeout;                                    \
            ping_info->DataBlockSize            = DSLH_PING_DEFAULT_DataBlockSize;                              \
            ping_info->DSCP                     = DSLH_PING_DEFAULT_DSCP;                                       \
            ping_info->SuccessCount             = 0;                                                            \
            ping_info->FailureCount             = 0;                                                            \
            ping_info->AverageResponseTime      = 0;                                                            \
            ping_info->MinimumResponseTime      = 0;                                                            \
            ping_info->MaximumResponseTime      = 0;                                                            \
            ping_info->hConnContext             = NULL;                                                         \
                                                                                                                \
            AnscZeroMemory(ping_info->Interface,  DSLH_PING_MAX_STRING_LENGTH_Interface);                       \
            AnscZeroMemory(ping_info->IfAddrName, DSLH_PING_MAX_STRING_LENGTH_Interface);                       \
            AnscZeroMemory(ping_info->Host,       DSLH_PING_MAX_STRING_LENGTH_Host     );                       \
        }

#define  DslhCleanPingInfo(ping_info)                                                                           \
        {                                                                                                       \
            AnscFreeMemory(ping_info->hConnContext);                                                            \
        }

#define  DslhFreePingInfo(ping_info)                                                                            \
        {                                                                                                       \
            DslhCleanPingInfo(ping_info);                                                                       \
            AnscFreeMemory(ping_info);                                                                          \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR IP DIAGNOSTICS
***********************************************************/

typedef  struct
_DSLH_IPD_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    ANSC_HANDLE                     hConnContext; /* Connection structure for IP or PPP */
    ANSC_STATUS                     DefGatewayPingResults;
    ANSC_STATUS                     WanIpAddressStatus;
    ANSC_STATUS                     DnsLookupResults;
    ANSC_STATUS                     DnsReverseLookupResults;
    ANSC_STATUS                     PingDnsServerResults;
}
DSLH_IPD_INFO,  *PDSLH_IPD_INFO;

#define  DslhInitIpdInfo(ipd_info)                                                                              \
        {                                                                                                       \
            DslhInitDiagInfo(ipd_info);                                                                         \
            ipd_info->StructSize                = sizeof(DSLH_IPD_INFO);                                        \
            ipd_info->DiagnosticType            = DSLH_DIAGNOSTIC_TYPE_Ip;                                      \
            ipd_info->hConnContext              = NULL;                                                         \
            ipd_info->DefGatewayPingResults     = ANSC_STATUS_FAILURE;                                          \
            ipd_info->WanIpAddressStatus        = ANSC_STATUS_FAILURE;                                          \
            ipd_info->DnsLookupResults          = ANSC_STATUS_FAILURE;                                          \
            ipd_info->DnsReverseLookupResults   = ANSC_STATUS_FAILURE;                                          \
            ipd_info->PingDnsServerResults      = ANSC_STATUS_FAILURE;                                          \
        }

#define  DslhCleanIpdInfo(ipd_info)                                                                             \
        {                                                                                                       \
            AnscFreeMemory(ipd_info->hConnContext);                                                             \
        }

#define  DslhFreeIpdInfo(ipd_info)                                                                              \
        {                                                                                                       \
            DslhCleanIpdInfo(ipd_info);                                                                         \
            AnscFreeMemory(ipd_info);                                                                           \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR PPP DIAGNOSTICS
***********************************************************/

typedef  struct
_DSLH_PPPD_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    ANSC_HANDLE                     hConnContext;
    ANSC_STATUS                     AuthenticationStatus;
    ANSC_STATUS                     PPPoEConnectionStatus;
    ANSC_STATUS                     PPPConnectionStatus;
    ANSC_STATUS                     PPPServerStatus;
}
DSLH_PPPD_INFO,  *PDSLH_PPPD_INFO;

#define  DslhInitPppdInfo(pppd_info)                                                                            \
        {                                                                                                       \
            DslhInitDiagInfo(pppd_info);                                                                        \
            pppd_info->StructSize               = sizeof(DSLH_PPPD_INFO);                                       \
            pppd_info->DiagnosticType           = DSLH_DIAGNOSTIC_TYPE_Ppp;                                     \
            pppd_info->hConnContext             = NULL;                                                         \
            pppd_info->AuthenticationStatus     = ANSC_STATUS_FAILURE;                                          \
            pppd_info->PPPoEConnectionStatus    = ANSC_STATUS_FAILURE;                                          \
            pppd_info->PPPConnectionStatus      = ANSC_STATUS_FAILURE;                                          \
            pppd_info->PPPServerStatus          = ANSC_STATUS_FAILURE;                                          \
        }

#define  DslhCleanPppdInfo(pppd_info)                                                                           \
        {                                                                                                       \
            AnscFreeMemory(pppd_info->hConnContext);                                                            \
        }

#define  DslhFreePppdInfo(pppd_info)                                                                            \
        {                                                                                                       \
            DslhCleanPppdInfo(pppd_info);                                                                       \
            AnscFreeMemory(pppd_info);                                                                          \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR WAN-DSL DIAGNOSTICS
***********************************************************/

/*
 * The WAN DSL Diagnostics object allows the ACS or GUI to perform a diagnostics on
 * the WAN DSL connection.
 */

/* Diagnostic states */
#define DSLH_WDD_MAX_STRING_LENGTH_LoopDiagnosticState  32      /* Needs to be large enough to hold the largest defined state */
#define DSLH_WDD_MAX_STRING_LENGTH_HLINpsds             32
#define DSLH_WDD_MAX_STRING_LENGTH_QLNpsds              32
#define DSLH_WDD_MAX_STRING_LENGTH_SNRpsds              32
#define DSLH_WDD_MAX_STRING_LENGTH_BITSpsds             32
#define DSLH_WDD_MAX_STRING_LENGTH_GAINSpsds            32

/*
 * The strings contained by this structure have to predefined maximum length. Those using this
 * this structure will have to allocate the strings once the length is determined.
 */
typedef  struct
_DSLH_WDD_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    LONG                            ACTPSDds;
    LONG                            ACTPSDus;
    LONG                            ACTATPds;
    LONG                            ACTATPus;
    LONG                            HLINSCds;
    char*                           HLINpsds;
    char*                           QLNpsds;
    char*                           SNRpsds;
    char*                           BITSpsds;
    char*                           GAINSpsds;
}
DSLH_WDD_INFO,  *PDSLH_WDD_INFO;

/*
 * Memory for the result strings must be allocted by the backend due to the unknown length
 * of such strings and the inability to impose a maximum string length on the results.
 */
#define  DslhInitWddInfo(wdd_info)                                                                                  \
        {                                                                                                           \
            DslhInitDiagInfo(wdd_info);                                                                             \
            wdd_info->StructSize                = sizeof(DSLH_WDD_INFO);                                            \
            wdd_info->DiagnosticType            = DSLH_DIAGNOSTIC_TYPE_WanDsl;                                      \
            wdd_info->ACTPSDds                  = 0;                                                                \
            wdd_info->ACTPSDus                  = 0;                                                                \
            wdd_info->ACTATPds                  = 0;                                                                \
            wdd_info->ACTATPus                  = 0;                                                                \
            wdd_info->HLINSCds                  = 0;                                                                \
                                                                                                                    \
            wdd_info->HLINpsds = NULL;                                                                              \
            wdd_info->QLNpsds  = NULL;                                                                              \
            wdd_info->SNRpsds  = NULL;                                                                              \
            wdd_info->BITSpsds = NULL;                                                                              \
            wdd_info->GAINSpsds= NULL;                                                                              \
        }

#define  DslhCloneWddInfo(wdd_info_t, wdd_info_s)                                                                   \
        {                                                                                                           \
            wdd_info_t->StructSize              = wdd_info_s->StructSize;                                           \
            wdd_info_t->DiagnosticState         = wdd_info_s->DiagnosticState;                                      \
            wdd_info_t->ACTPSDds                = wdd_info_s->ACTPSDds;                                             \
            wdd_info_t->ACTPSDus                = wdd_info_s->ACTPSDus;                                             \
            wdd_info_t->ACTATPds                = wdd_info_s->ACTATPds;                                             \
            wdd_info_t->ACTATPus                = wdd_info_s->ACTATPus;                                             \
            wdd_info_t->HLINSCds                = wdd_info_s->HLINSCds;                                             \
            wdd_info_t->RequestId               = wdd_info_s->RequestId;                                            \
            wdd_info_t->RequestType             = wdd_info_s->RequestType;                                          \
                                                                                                                    \
            wdd_info_t->HLINpsds                = AnscCloneString(wdd_info_s->HLINpsds );                           \
            wdd_info_t->QLNpsds                 = AnscCloneString(wdd_info_s->QLNpsds  );                           \
            wdd_info_t->SNRpsds                 = AnscCloneString(wdd_info_s->SNRpsds  );                           \
            wdd_info_t->BITSpsds                = AnscCloneString(wdd_info_s->BITSpsds );                           \
            wdd_info_t->GAINSpsds               = AnscCloneString(wdd_info_s->GAINSpsds);                           \
        }

#define  DslhCleanWddInfo(wdd_info)                                                                                 \
        {                                                                                                           \
            if ( wdd_info->HLINpsds ) { AnscFreeMemory(wdd_info->HLINpsds ); }                                      \
            if ( wdd_info->QLNpsds  ) { AnscFreeMemory(wdd_info->QLNpsds  ); }                                      \
            if ( wdd_info->SNRpsds  ) { AnscFreeMemory(wdd_info->SNRpsds  ); }                                      \
            if ( wdd_info->BITSpsds ) { AnscFreeMemory(wdd_info->BITSpsds ); }                                      \
            if ( wdd_info->GAINSpsds) { AnscFreeMemory(wdd_info->GAINSpsds); }                                      \
                                                                                                                    \
            wdd_info->HLINpsds = NULL;                                                                              \
            wdd_info->QLNpsds  = NULL;                                                                              \
            wdd_info->SNRpsds  = NULL;                                                                              \
            wdd_info->BITSpsds = NULL;                                                                              \
            wdd_info->GAINSpsds= NULL;                                                                              \
        }

#define  DslhFreeWddInfo(wdd_info)                                                                                  \
        {                                                                                                           \
            DslhCleanWddInfo(wdd_info);                                                                             \
            AnscFreeMemory  (wdd_info);                                                                             \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR WAN ATM F5 DIAGNOSTICS
***********************************************************/

/*
 * The WAN ATM F5 Loopback Diagnostics object allows the ACS or GUI to perform diagnostics
 * on the ATM link.
 */

/* Diagnostic states */
#define DSLH_WDAF5D_MAX_STRING_LENGTH_DiagnosticState   32  /* Needs to be large enough to hold the largest defined state */

/* Range limits imposed by TR-69 on various parameters */
#define DSLH_WDAF5D_MIN_NumberOfRepetitions             1
#define DSLH_WDAF5D_DEFAULT_NumberOfRepetitions         3
#define DSLH_WDAF5D_MIN_Timeout                         1
#define DSLH_WDAF5D_DEFAULT_Timeout                     100

typedef  struct
_DSLH_WDAF5D_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    ULONG                           NumberOfRepetitions;
    ULONG                           Timeout;
    ULONG                           SuccessCount;
    ULONG                           FailureCount;
    ULONG                           AverageResponseTime;
    ULONG                           MinimumResponseTime;
    ULONG                           MaximumResponseTime;
    char                            DestinationAddress[256];
}
DSLH_WDAF5D_INFO,  *PDSLH_WDAF5D_INFO;

#define  DslhInitWdaf5dInfo(wdaf5d_info)                                                                        \
        {                                                                                                       \
            DslhInitDiagInfo(wdaf5d_info);                                                                      \
            wdaf5d_info->StructSize             = sizeof(DSLH_WDAF5D_INFO);                                     \
            wdaf5d_info->DiagnosticType         = DSLH_DIAGNOSTIC_TYPE_WanAtmF5Lb;                              \
            wdaf5d_info->NumberOfRepetitions    = DSLH_WDAF5D_DEFAULT_NumberOfRepetitions;                      \
            wdaf5d_info->Timeout                = DSLH_WDAF5D_DEFAULT_Timeout;                                  \
            wdaf5d_info->SuccessCount           = 0;                                                            \
            wdaf5d_info->FailureCount           = 0;                                                            \
            wdaf5d_info->AverageResponseTime    = 0;                                                            \
            wdaf5d_info->MinimumResponseTime    = 0;                                                            \
            wdaf5d_info->MaximumResponseTime    = 0;                                                            \
                                                                                                                \
            AnscZeroMemory(wdaf5d_info->DestinationAddress, 256);                                               \
        }

#define  DslhCleanWdaf5dInfo(wdaf5d_info)                                                                       \
        {                                                                                                       \
        }

#define  DslhFreeWdaf5dInfo(wdaf5d_info)                                                                        \
        {                                                                                                       \
            DslhCleanWdaf5dInfo(wdaf5d_info);                                                                   \
            AnscFreeMemory(wdaf5d_info);                                                                        \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR IP ROUTEHOPS
***********************************************************/

/*
 *  Diagnostic states - Names are only defined for the standard requested types in TR-69
 */
#define DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHost         256
#define DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHostAddress  256
#define DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopRTTimes      64

/* Range limits imposed by TR-69 on various parameters */
#define DSLH_ROUTEHOPS_MIN_HopErrorCode                  1
#define DSLH_ROUTEHOPS_MAX_HopErrorCode                  100

typedef  struct
_DSLH_ROUTEHOPS_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    char                            HopHost        [DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHost        ];
    char                            HopHostAddress [DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHostAddress ];
    ULONG                           HopErrorCode;
    char                            HopRTTimes     [DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopRTTimes];

    ANSC_HANDLE                     hConnContext;                                       /* handle to the IP connection */
}
DSLH_ROUTEHOPS_INFO,  *PDSLH_ROUTEHOPS_INFO;

#define  DslhInitRoutehopsInfo(hop_info)                                                                    \
        {                                                                                                   \
            DslhInitDiagInfo(hop_info);                                                                     \
            hop_info->HopErrorCode             = 0;                                                         \
                                                                                                            \
            AnscZeroMemory(hop_info->HopHost, DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHost);                    \
            AnscZeroMemory(hop_info->HopHostAddress, DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopHostAddress);      \
            AnscZeroMemory(hop_info->HopRTTimes, DSLH_ROUTEHOPS_MAX_STRING_LENGTH_HopRTTimes);              \
        }

#define  DslhCleanRoutehopsInfo(hop_info)                                                                   \
        {                                                                                                   \
            AnscFreeMemory(hop_info->hConnContext);                                                         \
        }

#define  DslhFreeRoutehopsInfo(hop_info)                                                                    \
        {                                                                                                   \
            DslhCleanRoutehopsInfo(hop_info);                                                               \
            AnscFreeMemory(hop_info);                                                                       \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR IP TRACEROUTE
***********************************************************/

/*
 *  Diagnostic states - Names are only defined for the standard requested types in TR-69
 */
#define DSLH_TRACEROUTE_MAX_STRING_LENGTH_DiagnosticState 64      /* Needs to be large enough to hold the largest defined state */
#define DSLH_TRACEROUTE_MAX_STRING_LENGTH_Host            256
#define DSLH_TRACEROUTE_MAX_STRING_LENGTH_Interface       256

/* Range limits imposed by TR-69 on various parameters */
#define DSLH_TRACEROUTE_MAX_HOPS_COUNT                    64
#define DSLH_TRACEROUTE_MIN_NumberOfTries                 1
#define DSLH_TRACEROUTE_DEFAULT_NumberOfTries             3       /* 3 ICMP requests by default */
#define DSLH_TRACEROUTE_MIN_Timeout                       1
#define DSLH_TRACEROUTE_DEFAULT_Timeout                   5000    /* 100 ms timeout */
#define DSLH_TRACEROUTE_MIN_DataBlockSize                 1
#define DSLH_TRACEROUTE_DEFAULT_DataBlockSize             38      /* 32 byte payload by default */
#define DSLH_TRACEROUTE_MAX_DataBlockSize                 65535
#define DSLH_TRACEROUTE_MIN_DSCP                          0
#define DSLH_TRACEROUTE_DEFAULT_DSCP                      0       /* No special QoS handling by default */
#define DSLH_TRACEROUTE_MIN_MaxHopCount                   0
#define DSLH_TRACEROUTE_DEFAULT_MaxhopCount               30
#define DSLH_TRACEROUTE_MAX_DSCP                          64

typedef  struct
_DSLH_TRACEROUTE_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    char                            Interface [DSLH_TRACEROUTE_MAX_STRING_LENGTH_Interface];
    char                            IfAddrName[DSLH_TRACEROUTE_MAX_STRING_LENGTH_Interface];
    char                            Host      [DSLH_TRACEROUTE_MAX_STRING_LENGTH_Host     ];
    ULONG                           NumberOfTries;
    ULONG                           Timeout;
    ULONG                           DataBlockSize;
    ULONG                           DSCP;

    ULONG                           MaxHopCount;
    ULONG                           ResponseTime;
    ULONG                           RouteHopsNumberOfEntries;

    BOOL                            bRouteUpdated;
    ULONG                           UpdatedAt;
    ANSC_HANDLE                     hConnContext;                                       /* handle to the IP connection */
    DSLH_ROUTEHOPS_INFO             hDiagRouteHops[DSLH_TRACEROUTE_MAX_HOPS_COUNT];
}
DSLH_TRACEROUTE_INFO,  *PDSLH_TRACEROUTE_INFO;

#define  DslhInitTracerouteInfo(tracert_info)                                                                   \
        {                                                                                                       \
            DslhInitDiagInfo(tracert_info);                                                                     \
            tracert_info->StructSize               = sizeof(DSLH_TRACEROUTE_INFO);                              \
            tracert_info->DiagnosticType           = DSLH_DIAGNOSTIC_TYPE_Traceroute;                           \
            tracert_info->NumberOfTries            = DSLH_TRACEROUTE_DEFAULT_NumberOfTries;                     \
            tracert_info->Timeout                  = DSLH_TRACEROUTE_DEFAULT_Timeout;                           \
            tracert_info->DataBlockSize            = DSLH_TRACEROUTE_DEFAULT_DataBlockSize;                     \
            tracert_info->DSCP                     = DSLH_TRACEROUTE_DEFAULT_DSCP;                              \
            tracert_info->MaxHopCount              = DSLH_TRACEROUTE_DEFAULT_MaxhopCount;                       \
            tracert_info->ResponseTime             = 0;                                                         \
            tracert_info->RouteHopsNumberOfEntries = 0;                                                         \
            tracert_info->hConnContext             = NULL;                                                      \
            tracert_info->bRouteUpdated            = FALSE;                                                     \
            tracert_info->UpdatedAt                = 0;                                                         \
                                                                                                                \
            AnscZeroMemory(tracert_info->Interface, DSLH_TRACEROUTE_MAX_STRING_LENGTH_Interface);               \
            AnscZeroMemory(tracert_info->Host,      DSLH_TRACEROUTE_MAX_STRING_LENGTH_Host     );               \
            AnscZeroMemory((PUCHAR)tracert_info->hDiagRouteHops,                                                \
                            sizeof(DSLH_ROUTEHOPS_INFO)*DSLH_TRACEROUTE_MAX_HOPS_COUNT);                        \
        }

#define  DslhCloneTracerouteInfo(tracert_info_t, tracert_info_s)                                                \
        {                                                                                                       \
            tracert_info_t->StructSize              = tracert_info_s->StructSize;                               \
            tracert_info_t->DiagnosticType          = tracert_info_s->DiagnosticType;                           \
            tracert_info_t->NumberOfTries           = tracert_info_s->NumberOfTries;                            \
            tracert_info_t->Timeout                 = tracert_info_s->Timeout;                                  \
            tracert_info_t->DataBlockSize           = tracert_info_s->DataBlockSize;                            \
            tracert_info_t->DSCP                    = tracert_info_s->DSCP;                                     \
            tracert_info_t->MaxHopCount             = tracert_info_s->MaxHopCount;                              \
            tracert_info_t->ResponseTime            = tracert_info_s->ResponseTime;                             \
            tracert_info_t->RouteHopsNumberOfEntries= tracert_info_s->RouteHopsNumberOfEntries;                 \
            tracert_info_t->hConnContext            = tracert_info_s->hConnContext;                             \
            tracert_info_t->bRouteUpdated           = tracert_info_s->bRouteUpdated;                            \
                                                                                                                \
            AnscCopyMemory(tracert_info_t->Interface, tracert_info_s->Interface,                                \
                           DSLH_TRACEROUTE_MAX_STRING_LENGTH_Interface);                                        \
            AnscCopyMemory(tracert_info_t->Host, tracert_info_s->Host,                                          \
                           DSLH_TRACEROUTE_MAX_STRING_LENGTH_Host);                                             \
            AnscCopyMemory((PUCHAR)tracert_info_t->hDiagRouteHops, (PUCHAR)tracert_info_s->hDiagRouteHops,      \
                            sizeof(DSLH_ROUTEHOPS_INFO)*DSLH_TRACEROUTE_MAX_HOPS_COUNT);                        \
        }

#define  DslhCleanTracerouteInfo(tracert_info)                                                                  \
        {                                                                                                       \
            AnscFreeMemory(tracert_info->hConnContext);                                                         \
        }

#define  DslhFreeTracerouteInfo(tracert_info)                                                                   \
        {                                                                                                       \
            DslhCleanTracerouteInfo(tracert_info);                                                              \
            AnscFreeMemory(tracert_info);                                                                       \
        }

/***********************************************************
      CPE PARAMETER DEFINITIONS FOR NSLookup
***********************************************************/

/*
 *  Diagnostic states - Names are only defined for the standard requested types in TR-157
 */
#define DSLH_NS_MAX_STRING_LENGTH_Host            256
#define DSLH_NS_MAX_STRING_LENGTH_Interface       256
#define DSLH_NS_MAX_STRING_LENGTH_DNSServer       256

/* Range limits imposed by TR-157 on various parameters */
#define DSLH_NS_MIN_NumberOfRepetitions           1
#define DSLH_NS_DEFAULT_NumberOfRepetitions       3
#define DSLH_NS_MIN_Timeout                       1000
#define DSLH_NS_DEFAULT_Timeout                   1000    /* 100 ms timeout */

typedef  struct
_DSLH_NSLOOKUP_INFO
{
    DSLH_DIAG_INFO_BASE_CONTENT
    char                            Interface[DSLH_NS_MAX_STRING_LENGTH_Interface + 1];
    char                            IfAddr   [DSLH_NS_MAX_STRING_LENGTH_Interface + 1];
    char                            HostName [DSLH_NS_MAX_STRING_LENGTH_Host + 1     ];
    char                            DNSServer[DSLH_NS_MAX_STRING_LENGTH_DNSServer + 1];
    ULONG                           NumberOfRepetitions;
    ULONG                           Timeout;
    ULONG                           SuccessCount;
    ULONG                           ResultNumberOfEntries;
    BOOL                            bForced;                                            /* Identify if we can interrupt current diagnostic */
    ANSC_HANDLE                     hDiaginfo;
    ULONG                           UpdatedAt;
}
DSLH_NSLOOKUP_INFO,  *PDSLH_NSLOOKUP_INFO;

#define  DslhInitNSLookupInfo(NSLookup_info)                                                                        \
        {                                                                                                           \
            DslhInitDiagInfo(NSLookup_info);                                                                        \
            NSLookup_info->StructSize               = sizeof(DSLH_NSLOOKUP_INFO);                                   \
            NSLookup_info->DiagnosticType           = DSLH_DIAG_STATE_TYPE_None;                                    \
            NSLookup_info->NumberOfRepetitions      = DSLH_NS_DEFAULT_NumberOfRepetitions;                          \
            NSLookup_info->Timeout                  = DSLH_NS_DEFAULT_Timeout;                                      \
            NSLookup_info->SuccessCount             = 0;                                                            \
            NSLookup_info->ResultNumberOfEntries    = 0;                                                            \
            NSLookup_info->bForced                  = FALSE;                                                        \
            NSLookup_info->hDiaginfo                = NULL;                                                         \
            NSLookup_info->UpdatedAt                = 0;                                                            \
                                                                                                                    \
            AnscZeroMemory(NSLookup_info->Interface, DSLH_NS_MAX_STRING_LENGTH_Interface + 1);                      \
            AnscZeroMemory(NSLookup_info->IfAddr   , DSLH_NS_MAX_STRING_LENGTH_Interface + 1);                      \
            AnscZeroMemory(NSLookup_info->HostName,  DSLH_NS_MAX_STRING_LENGTH_Host + 1     );                      \
            AnscZeroMemory(NSLookup_info->DNSServer, DSLH_NS_MAX_STRING_LENGTH_DNSServer + 1);                      \
        }

#define  DslhCleanNSLookupInfo(NSLookup_info)                                                                       \
        {                                                                                                           \
            AnscFreeMemory(NSLookup_info->hDiaginfo);                                                               \
        }

#define  DslhFreeNSLookupInfo(NSLookup_info)                                                                        \
        {                                                                                                           \
            DslhCleanNSLookupInfo(NSLookup_info);                                                                   \
            AnscFreeMemory(NSLookup_info);                                                                          \
        }

#endif

