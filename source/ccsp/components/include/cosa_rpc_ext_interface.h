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

#ifndef  _COSA_TR_069_RPC_EXT_INTERFACE_H_
#define  _COSA_TR_069_RPC_EXT_INTERFACE_H_


#ifndef  _COSA_NO_SUPPORT_PD194
#include "cosa_pd194_smm_interface.h"
#else
#define  COSA_RPC_FAULTCODE_NoError                 0
#endif


/**********************************************************************************
 Overview
   
    This header file defines interface APIs between TR-069 agent and RPC Extension
    shared library.

 **********************************************************************************/

 /**********************************************************************************
    Unload function entry prototype for all RPC Ext libraries
 **********************************************************************************/

typedef 
void
(*PFN_COSA_Rpc_Unload)
    (
    );

 /**********************************************************************************
    Init function entry prototype for supporting RPC Download/Upload
 **********************************************************************************/

#define  COSA_RPC_FAULTCODE_RequestDenied           9001
#define  COSA_RPC_FAULTCODE_InternalError           9002
#define  COSA_RPC_FAULTCODE_DownloadFailure         9010
#define  COSA_RPC_FAULTCODE_UploadFailed            9011
#define  COSA_RPC_FAULTCODE_ServerAuthFailed        9012
#define  COSA_RPC_FAULTCODE_TransferProtoNotSupport 9013
#define  COSA_RPC_FAULTCODE_McastJoinFailed         9014
#define  COSA_RPC_FAULTCODE_ServerContactFailed     9015
#define  COSA_RPC_FAULTCODE_NoAccessToFile          9016
#define  COSA_RPC_FAULTCODE_UnableCompleteDownload  9017
#define  COSA_RPC_FAULTCODE_FileCorrupted           9018
#define  COSA_RPC_FAULTCODE_FileAuthFailure         9019

typedef 
int 
(*PFN_COSA_Rpc_TransCompl)
    (
        void*                      hCosaContext,
        unsigned int               IsDownload,         /* 0 - upload, non-zero - download */
        char*                      pCommandKey,        /* the command key used to identify this operation */
        unsigned int               FaultCode,          /* 0 means no error */
        char*                      FaultString,        /* ignored if FaultCode is 0 */ 
        char*                      StartTime,          /* TR-069 data type 'dateTime' */
        char*                      CompleteTime        /* TR-069 data type 'dateTime' */
    );
 
typedef
int 
(*PFN_COSA_Rpc_Download)
    (
         void*                      hCosaContext,      
         char*                      pCommandKey,
         char*                      pFileType,         
         char*                      pURL,
         char*                      pUsername,
         char*                      pPassword,
         unsigned int               ulFileSize,
         char*                      pTargetFileName,
         unsigned int               ulDelaySeconds,
         char*                      pSuccessfulURL,
         char*                      pFailureURL
    );

typedef
int 
(*PFN_COSA_Rpc_Upload)
    (
        void*                       hCosaContext,       
        char*                       pCommandKey,
        char*                       pFileType,          
        char*                       pURL,
        char*                       pUsername,
        char*                       pPassword,
        unsigned int                ulDelaySeconds
    );

typedef 
int
(*PFN_COSA_Rpc_InitFileTransfer)
    (
        void*                       hCosaContext,
        void*                       hCosaPuaIf,
        PFN_COSA_Rpc_TransCompl     pfnTransferComplete     /* callback function when transfer completes */
    );


/**********************************************************************************
  function prototypes for GetQueuedTransfers
**********************************************************************************/

typedef  struct
_COSA_QUEUED_TRANSFER
{
    char                            CommandKey[33];     /* file transfer identifier set by ACS, max length 32, NUL terminated */
    unsigned int                    State;              /* 1 - not yet started, 2 - in progress, 3 - completed, finishing cleanup */
}
COSA_QUEUED_TRANSFER, *PCOSA_QUEUED_TRANSFER;


typedef 
int
(*PFN_COSA_GetQueuedTransfers)
    (
        void*                       hCosaContext,
        PCOSA_QUEUED_TRANSFER       pQueuedTransfers,
        int                         nMaxNumTransfers
    );
/* return value - the number queued transfers returned, pQueuedTransfer is 
   allocated nMaxNumTransfers * sizeof(COSA_QUEUED_TRANSFER) at least.
   In case, memory is not enough, backend needs to return the real number
   of queued transfers */


 /**********************************************************************************
    Init function entry prototype for supporting RPC AutonomousTransferComplete
 **********************************************************************************/

/* callback function to TR-069 agent */
typedef
int
(*PFN_COSA_Rpc_AutonTransCompl)
    (
        void*                       hCosaContext,
        unsigned int                FaultCode,
        char*                       FaultString,            /* ignored if FaultCode is 0 */
        char*                       AnnounceURL,
        char*                       TransferURL,
        unsigned int                IsDownload,             /* 0 - upload, non-zero - download */
        char*                       FileType,
        unsigned int                FileSize,
        char*                       TargetFileName,
        char*                       StartTime,              /* TR-069 data type 'dateTime' */
        char*                       CompleteTime            /* TR-069 data type 'dateTime' */
    );


/* prototype of "Init" function of shared library that
 * supports AutonomousTransferComplete"
 */
typedef
int
(*PFN_COSA_Rpc_InitAutonTransCompl)
    (
        void*                           hCosaContext,
        void*                           hCosaPuaIf,
        PFN_COSA_Rpc_AutonTransCompl    pfnTransferComplete
    );


#ifndef  _COSA_NO_SUPPORT_PD194

 /**********************************************************************************
    Init function entry point prototype for supporting RPCs defined in PD-194
 **********************************************************************************/

typedef PFN_COSA_SMM_Init                           PFN_COSA_Rpc_InitExecEnvMgr;
typedef PFN_COSA_SMM_ChangeDUState                  PFN_COSA_Rpc_ChangeDUState;

#endif

 /**********************************************************************************
    Prototypes of helper functions that RPC extension library may need
 **********************************************************************************/


/**********************************************************************************
  function prototypes for Reboot
**********************************************************************************/

typedef 
int
(*PFN_COSA_Rpc_InitReboot)
    (
        void*                       hCosaContext,
        void*                       hCosaPuaIf
    );
/* return value - 0 means success, others failure */

typedef 
int
(*PFN_COSA_Reboot)
    (
        void*                       hCosaContext,
        void*                       hCosaPuaIf
    );
/* return value - 0 means reboot is scheduled, others means
 * the request is rejected or failure to schdule reboot among
 * other problems.
 */


/**********************************************************************************
  function prototypes for Reboot
**********************************************************************************/

typedef 
int
(*PFN_COSA_Rpc_InitFactoryReset)
    (
        void*                       hCosaContext,
        void*                       hCosaPuaIf
    );
/* return value - 0 means success, others failure */

typedef 
int
(*PFN_COSA_FactoryReset)
    (
        void*                       hCosaContext,
        void*                       hCosaPuaIf
    );
/* return value - 0 means factory reset is scheduled, others means
 * the request is rejected or failure to schdule factory reset among
 * other problems.
 */


#endif  /* _COSA_TR_069_RPC_EXT_INTERFACE_H_ */
