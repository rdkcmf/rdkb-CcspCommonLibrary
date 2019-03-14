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

    module: dslh_definitions_tr143.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the definitions of structure used by TR143

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        06/15/2010    initial revision.

**********************************************************************/


#ifndef  _DSLH_DEFINITIONS_TR143_H_
#define  _DSLH_DEFINITIONS_TR143_H_

/*
 * Each status of diagnostic supported by the DSLH model must be uniquely identified */
#define DSLH_TR143_DIAGNOSTIC_None                              0
#define DSLH_TR143_DIAGNOSTIC_Requested                         1
#define DSLH_TR143_DIAGNOSTIC_Completed                         2
#define DSLH_TR143_DIAGNOSTIC_Error_InitConnectionFailed        3
#define DSLH_TR143_DIAGNOSTIC_Error_NoResponse                  4
#define DSLH_TR143_DIAGNOSTIC_Error_TransferFailed              5
#define DSLH_TR143_DIAGNOSTIC_Error_PasswordRequestFailed       6
#define DSLH_TR143_DIAGNOSTIC_Error_LoginFailed                 7
#define DSLH_TR143_DIAGNOSTIC_Error_NoTransferMode              8
#define DSLH_TR143_DIAGNOSTIC_Error_NoPASV                      9
#define DSLH_TR143_DIAGNOSTIC_Error_IncorrectSize               10
#define DSLH_TR143_DIAGNOSTIC_Error_Timeout                     11

#define DSLH_TR143_MAX_STRING_LENGTH                            256
/***********************************************************
      TR143 DOWNLOAD DIAGNOSTICS INFO STRUCTURE
***********************************************************/

typedef struct
_DSLH_TR143_DOWNLOAD_DIAG_INFO
{
    char                            Interface[DSLH_TR143_MAX_STRING_LENGTH];     
	char							IfAddrName[DSLH_TR143_MAX_STRING_LENGTH];
    char                            DownloadURL[DSLH_TR143_MAX_STRING_LENGTH];            
    ULONG                           DSCP;                   
    ULONG                           EthernetPriority;  
    ULONG                           DiagnosticsState;
}
DSLH_TR143_DOWNLOAD_DIAG_INFO, *PDSLH_TR143_DOWNLOAD_DIAG_INFO;

#define DslhInitDownloadDiagInfo(d_info)                                         \
        {                                                                        \
                AnscZeroMemory(d_info->Interface, DSLH_TR143_MAX_STRING_LENGTH); \
                AnscZeroMemory(d_info->DownloadURL, DSLH_TR143_MAX_STRING_LENGTH); \
                d_info->DSCP                = 0;                                 \
                d_info->EthernetPriority    = 0;                                 \
                d_info->DiagnosticsState    = 0;                                 \
        }                                                                        \
    
#define DslhFreeDownloadDiagInfo(d_info)                                         \
        {                                                                        \
            AnscFreeMemory(d_info);                                              \
        }

/***********************************************************
      TR143 UPLOAD DIAGNOSTICS INFO STRUCTURE
***********************************************************/

typedef struct
_DSLH_TR143_UPLOAD_DIAG_INFO
{
    char                            Interface[DSLH_TR143_MAX_STRING_LENGTH];             
	char							IfAddrName[DSLH_TR143_MAX_STRING_LENGTH];
    char                            UploadURL[DSLH_TR143_MAX_STRING_LENGTH];            
    ULONG                           DSCP;                   
    ULONG                           EthernetPriority;  
    ULONG                           TestFileLength;
    ULONG                           DiagnosticsState;
}
DSLH_TR143_UPLOAD_DIAG_INFO, *PDSLH_TR143_UPLOAD_DIAG_INFO;

#define DslhInitUploadDiagInfo(u_info)                                           \
        {                                                                        \
                AnscZeroMemory(u_info->Interface, DSLH_TR143_MAX_STRING_LENGTH); \
                AnscZeroMemory(u_info->UploadURL, DSLH_TR143_MAX_STRING_LENGTH); \
                u_info->DSCP                = 0;                                 \
                u_info->EthernetPriority    = 0;                                 \
                u_info->TestFileLength      = 1000000;                           \
                u_info->DiagnosticsState    = 0;                                 \
        }                                                                        \
    
#define DslhFreeUploadDiagInfo(u_info)                                           \
        {                                                                        \
            AnscFreeMemory(u_info);                                              \
        }

/***********************************************************
      TR143 UDP ECHO SERVER CONFIGURE
***********************************************************/

typedef struct
_DSLH_TR143_UDP_ECHO_CONFIG
{
    BOOL                            Enable;
    char                            Interface[DSLH_TR143_MAX_STRING_LENGTH];
    char                            IfAddrName[DSLH_TR143_MAX_STRING_LENGTH];
    /*ULONG                           SourceIPAddress;*/
    char                            SourceIPName[DSLH_TR143_MAX_STRING_LENGTH];
    ULONG                           UDPPort;
    BOOL                            EchoPlusEnabled;
    BOOL                            EchoPlusSupported;
}
DSLH_TR143_UDP_ECHO_CONFIG, *PDSLH_TR143_UDP_ECHO_CONFIG;

#define DslhInitUDPEchoConfig(u_info)                                           \
        {                                                                        \
                AnscZeroMemory(u_info->Interface, DSLH_TR143_MAX_STRING_LENGTH); \
                AnscZeroMemory(u_info->IfAddrName, DSLH_TR143_MAX_STRING_LENGTH); \
                AnscZeroMemory(u_info->SourceIPName, DSLH_TR143_MAX_STRING_LENGTH);\
                u_info->Enable              = FALSE;                             \
                u_info->UDPPort             = 0;                                 \
                u_info->EchoPlusEnabled     = FALSE;                             \
                u_info->EchoPlusSupported   = FALSE;                             \
        }                                                                        \

#define DslhFreeUDPEchoConfig(u_info)                                            \
        {                                                                        \
            AnscFreeMemory(u_info);                                              \
        }

#endif
