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

    module:	cosa_pd194_smm_interface.h

        For supporting PD-194 in COSA TR-069 CPE agent

    ---------------------------------------------------------------

    description:

        To support multiple Execution Environments (EE) on CPE or 
        even single EE, a thin Execution Environment layer is 
        logically needed, we call it Execution Environment 
        Manager (EEM). 

        This header file defines interface APIs between TR-069 agent 
        and backend Execution Environment Manager (EEM).

        Two sets of APIs defined, one for conducting backend to 
        perform operations defined by PD-194 - install, update and
        uninstall.

        The other set is for backend EEM to notify TR-069 agent 
        the completeness of an operation as well as to 
        register or de-register data model access shared library 
        on behalf of an Execution Unit (EU).

        Assumption:

            TR-069 agent assumes direct API calls between itself and 
            EEM, either a static or shared library is fine.

            IPC/RPC mechanism is not expected, otherwise, we may have 
            to redefine this interface in case we cannot pass pointers 
            easily.

        Notes:

            In data structures defined in this header files, there
            are some reference fields for example ExecEnvRef in
            struct _COSA_SMM_InstallOp, ExecutionUnitRefList in
            struct _COSA_SMM_DUStateChangeResult. According to 
            PD-194, they are defined as TR-069 object full path
            name in data parameter tree ended with ".". If EEM
            is able to return the true value, it is great. Otherwise,
            EEM may need to return unique identifier to the 
            referenced item depending on what it is. We will 
            finalize this in future.

    ---------------------------------------------------------------

    environment:

        COSA TR069 - platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        12/08/2010    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/


#ifndef  _COSA_PD_194_SMM_INTERFACE_H_
#define  _COSA_PD_194_SMM_INTERFACE_H_


#define  COSA_RPC_FAULTCODE_NoError                 0
/* Note: PD-194 does not define specific fault codes other than 0 
 * which is no error. We may define our own fault codes. Revisit this
 * when necessary.
 */


/**********************************************************************
 * Data structures to convey parameters of RPCs - 
 *
 *      ChangeDUState
 *      DUStateChangeComplete
 *      AutonomousDUStateChangeComplete
 *
 **********************************************************************/

typedef 
enum _COSA_SMM_OP
{
    COSA_SMM_OP_Unknown  = 0,
    COSA_SMM_OP_Install,
    COSA_SMM_OP_Update,
    COSA_SMM_OP_Uninstall
}
COSA_SMM_OP, *PCOSA_SMM_OP;


/**********************************************************************
    RPC DUStateChange
 *********************************************************************/

typedef  struct _COSA_SMM_InstallOp
{
    char*                           Url;
    char*                           Uuid;
    char*                           Username;
    char*                           Password;
    char*                           ExecEnvRef;     /* TODO: what to return from EEM */
}
COSA_SMM_InstallOp, *PCOSA_SMM_InstallOp;


#define  CosaSmmInstallOpClean(free_fptr, iop)                                          \
    do {                                                                                \
        if ( (iop)->Url )                                                               \
            (*free_fptr)((iop)->Url);                                                   \
        if ( (iop)->Uuid )                                                              \
            (*free_fptr)((iop)->Uuid);                                                  \
        if ( (iop)->Username )                                                          \
            (*free_fptr)((iop)->Username);                                              \
        if ( (iop)->Password )                                                          \
            (*free_fptr)((iop)->Password);                                              \
        if ( (iop)->ExecEnvRef )                                                        \
            (*free_fptr)((iop)->ExecEnvRef);                                            \
    } while (0)


typedef  struct _COSA_SMM_UpdateOp
{
    char*                           Uuid;
    char*                           Version;
    char*                           Url;
    char*                           Username;
    char*                           Password;
}
COSA_SMM_UpdateOp, *PCOSA_SMM_UpdateOp;


#define  CosaSmmUpdateOpClean(free_fptr, uop)                                           \
    do {                                                                                \
        if ( (uop)->Uuid )                                                              \
            (*free_fptr)((uop)->Uuid);                                                  \
        if ( (uop)->Version )                                                           \
            (*free_fptr)((uop)->Version);                                               \
        if ( (uop)->Url )                                                               \
            (*free_fptr)((uop)->Url);                                                   \
        if ( (uop)->Username )                                                          \
            (*free_fptr)((uop)->Username);                                              \
        if ( (uop)->Password )                                                          \
            (*free_fptr)((uop)->Password);                                              \
    } while (0)


typedef  struct _COSA_SMM_UninstallOp
{
    char*                           Uuid;
    char*                           Version;
}
COSA_SMM_UninstallOp, *PCOSA_SMM_UninstallOp;

#define  CosaSmmUninstallOpClean(free_fptr, uiop)                                       \
    do {                                                                                \
        if ( (uiop)->Uuid )                                                             \
            (*free_fptr)((uiop)->Uuid);                                                 \
        if ( (uiop)->Version )                                                          \
            (*free_fptr)((uiop)->Version);                                              \
    } while (0)


typedef  struct _COSA_SMM_ChangeDUStateOp
{
    COSA_SMM_OP                     Op;

    union {
        COSA_SMM_InstallOp          Install;
        COSA_SMM_UpdateOp           Update;
        COSA_SMM_UninstallOp        Uninstall;
    } op;
}
COSA_SMM_ChangeDUStateOp, *PCOSA_SMM_ChangeDUStateOp;


typedef  struct _COSA_SMM_ChangeDUStateReq
{
    int                             NumOperations;
    PCOSA_SMM_ChangeDUStateOp       Operations;
    char*                           CommandKey;
}
COSA_SMM_ChangeDUStateReq, *PCOSA_SMM_ChangeDUStateReq;


#define  CosaSmmCdsReqClean(free_fptr, CdsReq)                                              \
    do {                                                                                    \
        int                         i;                                                      \
        if ( (CdsReq)->CommandKey )                                                         \
            (*free_fptr)((CdsReq)->CommandKey);                                             \
        for ( i = 0; i < (CdsReq)->NumOperations; i ++ )                                    \
        {                                                                                   \
            if ( (CdsReq)->Operations[i].Op == COSA_SMM_OP_Install )                        \
                CosaSmmInstallOpClean(free_fptr, &((CdsReq)->Operations[i].op.Install));    \
            else if ( (CdsReq)->Operations[i].Op == COSA_SMM_OP_Update )                    \
                CosaSmmUpdateOpClean(free_fptr, &((CdsReq)->Operations[i].op.Update));      \
            else                                                                            \
                CosaSmmUninstallOpClean(free_fptr, &((CdsReq)->Operations[i].op.Uninstall));\
        }                                                                                   \
    } while (0)


#define  CosaSmmCdsReqRemove(free_fptr, CdsReq)                                         \
    do {                                                                                \
        CosaSmmCdsReqClean(free_fptr, CdsReq);                                          \
        (*free_fptr)(CdsReq);                                                           \
    } while (0)


typedef  struct _COSA_SMM_ChangeDUStateResp
{
    unsigned int                    Dummy;
}
COSA_SMM_ChangeDUStateResp, *PCOSA_SMM_ChangeDUStateResp;


/**********************************************************************
    RPC DUStateChangeComplete
 *********************************************************************/

typedef  struct _COSA_SMM_DUStateChangeResult
{
    unsigned int                    FaultCode;
    char*                           FaultString;
    char*                           Uuid;
    char*                           DeploymentUnitRef;          /* TODO: what to return from EEM */
    char*                           Version;
    char*                           CurrentState;
    unsigned int                    Resolved;
    char*                           ExecutionUnitRefList;       /* TODO: what to return from EEM */
    char*                           StartTime;
    char*                           CompleteTime;
}
COSA_SMM_DUStateChangeResult, *PCOSA_SMM_DUStateChangeResult;

#define  CosaSmmDscResultClean(free_fptr, dscResult)                                    \
    do {                                                                                \
        if ( (dscResult)->FaultString )                                                 \
            (*free_fptr)((dscResult)->FaultString);                                     \
        if ( (dscResult)->Uuid )                                                        \
            (*free_fptr)((dscResult)->Uuid);                                            \
        if ( (dscResult)->DeploymentUnitRef )                                           \
            (*free_fptr)((dscResult)->DeploymentUnitRef);                               \
        if ( (dscResult)->Version )                                                     \
            (*free_fptr)((dscResult)->Version);                                         \
        if ( (dscResult)->CurrentState )                                                \
            (*free_fptr)((dscResult)->CurrentState);                                    \
        if ( (dscResult)->ExecutionUnitRefList )                                        \
            (*free_fptr)((dscResult)->ExecutionUnitRefList);                            \
        if ( (dscResult)->StartTime )                                                   \
            (*free_fptr)((dscResult)->StartTime);                                       \
        if ( (dscResult)->CompleteTime )                                                \
            (*free_fptr)((dscResult)->CompleteTime);                                    \
    } while (0)


typedef  struct _COSA_SMM_ChgDUStateCompReq
{
    int                             NumResults;
    PCOSA_SMM_DUStateChangeResult   Results;
    char*                           CommandKey;
}
COSA_SMM_ChgDUStateCompReq, *PCOSA_SMM_ChgDUStateCompReq;


#define  CosaSmmDsccReqClean(free_fptr, DsccReq)                                        \
    do {                                                                                \
        int                         i;                                                  \
        if ( (DsccReq)->CommandKey )                                                    \
            (*free_fptr)((DsccReq)->CommandKey);                                        \
        for ( i = 0; i < (DsccReq)->NumResults; i ++ )                                  \
        {                                                                               \
            CosaSmmDscResultClean(free_fptr, &((DsccReq)->Results[i]));                 \
        }                                                                               \
        if ( (DsccReq)->Results )                                                       \
        {                                                                               \
            (*free_fptr)((DsccReq)->Results);                                           \
        }                                                                               \
    } while (0)

#define  CosaSmmDsccReqRemove(free_fptr, DsccReq)                                       \
    do {                                                                                \
        CosaSmmDsccReqClean(free_fptr, DsccReq);                                        \
        (*free_fptr)(DsccReq);                                                          \
    } while (0)

typedef  struct _COSA_SMM_ChgDUStateCompResp
{
    unsigned int                    Dummy;
}
COSA_SMM_ChgDUStateCompResp, *PCOSA_SMM_ChgDUStateCompResp;


/**********************************************************************
    RPC AutonomousDUStateChangeComplete
 *********************************************************************/

typedef  struct _COSA_SMM_AutonDuStateChgResult
{
    unsigned int                    FaultCode;
    char*                           FaultString;
    char*                           Uuid;
    char*                           DeploymentUnitRef;      /* TODO: what to return from EEM */
    char*                           Version;
    char*                           OperationPerformed;
    char*                           CurrentState;
    unsigned int                    Resolved;
    char*                           ExecutionUnitRefList;   /* TODO: what to return from EEM */
    char*                           StartTime;
    char*                           CompleteTime;
}
COSA_SMM_AutonDuStateChgResult, *PCOSA_SMM_AutonDuStateChgResult;


#define  CosaSmmAdscResultClean(free_fptr, adscResult)                                  \
    do {                                                                                \
        if ( (adscResult)->FaultString )                                                \
            (*free_fptr)((adscResult)->FaultString);                                    \
        if ( (adscResult)->Uuid )                                                       \
            (*free_fptr)((adscResult)->Uuid);                                           \
        if ( (adscResult)->DeploymentUnitRef )                                          \
            (*free_fptr)((adscResult)->DeploymentUnitRef);                              \
        if ( (adscResult)->Version )                                                    \
            (*free_fptr)((adscResult)->Version);                                        \
        if ( (adscResult)->OperationPerformed )                                         \
            (*free_fptr)((adscResult)->OperationPerformed);                             \
        if ( (adscResult)->CurrentState )                                               \
            (*free_fptr)((adscResult)->CurrentState);                                   \
        if ( (adscResult)->ExecutionUnitRefList )                                       \
            (*free_fptr)((adscResult)->ExecutionUnitRefList);                           \
        if ( (adscResult)->StartTime )                                                  \
            (*free_fptr)((adscResult)->StartTime);                                      \
        if ( (adscResult)->CompleteTime )                                               \
            (*free_fptr)((adscResult)->CompleteTime);                                   \
    } while (0)



typedef  struct _COSA_SMM_AutonDUStateChgCompReq
{
    int                             NumResults;
    PCOSA_SMM_AutonDuStateChgResult Results;
}
COSA_SMM_AutonDUStateChgCompReq, *PCOSA_SMM_AutonDUStateChgCompReq;


#define  CosaSmmAdsccReqClean(free_fptr, AdsccReq)                                      \
    do {                                                                                \
        int                         i;                                                  \
        for ( i = 0; i < (AdsccReq)->NumResults; i ++ )                                 \
        {                                                                               \
            CosaSmmAdscResultClean(free_fptr, &((AdsccReq)->Results[i]));               \
        }                                                                               \
        if ( (AdsccReq)->Results )                                                      \
        {                                                                               \
            (*free_fptr)((AdsccReq)->Results);                                          \
        }                                                                               \
    } while (0)


#define  CosaSmmAdscReqRemove(free_fptr, AdscReq)                                       \
    do {                                                                                \
        CosaSmmAdsccReqClean(free_fptr, AdscReq);                                       \
        (*free_fptr)(AdscReq);                                                          \
    } while (0)

typedef  struct _COSA_SMM_ChangeDUStateCompResp
{
    unsigned int                    Dummy;
}
COSA_SMM_ChangeDUStateCompResp, *PCOSA_SMM_ChangeDUStateCompResp;


/**********************************************************************
    APIs called from EEM to deliver operation completeness to TR-069
 *********************************************************************/

typedef 
int (*PFN_COSA_DUStateChangeComplete_CB)
    (
        void*                           hCosaContext,
        PCOSA_SMM_ChgDUStateCompReq     pDuStateChangeCompleteReq
    );
/* callback function prototype for Execution Environment to call
 * back to TR-069 agent to indicate completeness of an operation
 * that was directed by ACS.
 * 
 */

typedef 
int (*PFN_COSA_AutonDUStateChangeComplete_CB)
    (
        void*                               hCosaContext,
        PCOSA_SMM_AutonDUStateChgCompReq    pAutonChangeDuStateComplete
    );
/* callback function prototype for Execution Environment to call
 * back to TR-069 agent to indicate completeness of an operation
 * that was directed by other means than ACS
 *
 *  
 */

typedef
int (*PFN_COSA_Register_DMSL_CB)
    (
        void*                               hCosaContext,
        char*                               pXmlDataModelFileLocation
    );
/*
 * This callback function allows TR-069 agent to load DM shared library 
 * that has its own data model definitions and can access parameter tree.
 * The DM shared library is provided in DU. Also, a DU may contain 
 * multiple EU. Each EU may implement none or multiple DM shared 
 * libraries as it sees fit. EEM needs to invoke this API for each 
 * data model access shared library right after install/update completes.
 * Return value - 0 success, failure otherwise covering cases file does 
 * not exist or cannot be opened, malformed XML file format etc.
 *
 * Note: Execution Environment needs to keep data model description XML
 * file in place, and can be removed from system after this call. Same
 * applies to DM shared library file(s) associated.
 */

typedef
int (*PFN_COSA_Deregister_DMSL_CB)
    (
        void*                               hCosaContext,
        char*                               pXmlDataModelFileLocation
    );
/*
 * This callback function is called by EEM when an EU
 * is going to be removed and its associated DM shared library is going
 * to be turned off and removed from the system. Upon receiving the call,
 * TR-069 agent will de-register data model definitions this EU defines
 * and disconnect with the DM shared library specified.
 * Return value - 0 success, failure otherwise covering cases file does 
 * not exist or cannot be opened, malformed XML file format etc.
 *
 * Note: EEM needs to know where data model description XML
 * file is located along with the data model access shared library, 
 * and can be removed from system after this call. Same
 * applies to DM shared library file(s) associated.
 */


/**********************************************************************
    APIs to call in to EEM to perform operations ACS dictates 
 *********************************************************************/

typedef
int
(*PFN_COSA_SMM_Init)
    (
        void*                                   hCosaContext,
        void*                                   hCosaPuaIf,
        PFN_COSA_DUStateChangeComplete_CB       pfnDuStateChangeCompleteCB,
        PFN_COSA_AutonDUStateChangeComplete_CB  pfnAutonDuStateChangeCompleteCB,
        PFN_COSA_Register_DMSL_CB               pfnRegisterDmslCB,
        PFN_COSA_Deregister_DMSL_CB             pfnDeregisterDmslCB
    );
/*
 * API COSA_SMM_Init() is called when TR-069 stack starts up, this API
 * passes some parameters that Execution Environment needs to call back
 * to TR-069 agent to deliver notification of operation completeness or
 * register EU's data model shared libraries.
 *
 * Arguments:
 *      pfnDuStateChangeCompleteCB 
 *          - callback function for indicating DUStateChangeComplete with 
 *            results
 *
 *      pfnAutonDuStateChangeCompleteCB 
 *          - callback function for indicating AtonomousDUStateChangeComplete 
 *            with results
 *
 *      pfnRegisterDmslCB 
 *          - callback function to let Execution Environment on behalf
 *            of EU to register DM shared library which manages
 *            and accesses TR data model parameter tree.
 *
 *      pfnDeregisterDmslCB 
 *          - callback function to de-register data model definition
 *            defined in DMD XML file.
 */


typedef
int 
(*PFN_COSA_SMM_ChangeDUState)
    (
        void*                       hCosaContext,               
        char*                       pOperations,
        char*                       pCommandKey
    );
/*
 * API COSA_ChangeDUState() is called from TR-069 agent into Application
 * Framework to perform operations specified in the given request.
 * Parameter 'pOperations' contains XML content whose root is Operations node
 * as we see in RPC ChangeDUState request defined by PD-194.
 */


#endif  /* _COSA_PD_194_SMM_INTERFACE_H_ */

