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

    module:	scli_shell_base.c

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the SCLI Shell Object.

        *   ScliShoCreate
        *   ScliShoRemove
        *   ScliShoEnrollObjects
        *   ScliShoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/10/05    initial revision.

**********************************************************************/


#include "scli_shell_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        ScliShoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the SCLI Shell Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
ScliShoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSCLI_SHELL_OBJECT              pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSCLI_SHELL_OBJECT)AnscAllocateMemory(sizeof(SCLI_SHELL_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), SCLI_SHELL_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SCLI_SHELL_OID;
    pBaseObject->Create            = ScliShoCreate;
    pBaseObject->Remove            = ScliShoRemove;
    pBaseObject->EnrollObjects     = ScliShoEnrollObjects;
    pBaseObject->Initialize        = ScliShoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PTELNET_TSP_INTERFACE           pTspIf       = (PTELNET_TSP_INTERFACE    )pMyObject->hTspIf;
    PSCLI_SHELL_CEN_INTERFACE       pCenIf       = (PSCLI_SHELL_CEN_INTERFACE)pMyObject->hCenIf;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PSCLI_SHELL_SESSION_EXEC        pSession     = NULL;

    pMyObject->KillTdo((ANSC_HANDLE)pMyObject);

    while ( TRUE )
    {
        AnscAcquireLock(&pMyObject->SessionListLock);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->SessionList);
        AnscReleaseLock(&pMyObject->SessionListLock);

        if ( !pSLinkEntry )
        {
            break;
        }

        pSession    = ACCESS_SCLI_SHELL_SESSION_EXEC_OBJECT(pSLinkEntry);

        pMyObject->RemoveSession((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSession);
    }

    AnscFreeLock(&pMyObject->SessionListLock);

    if ( pTspIf )
    {
        AnscFreeMemory(pTspIf);

        pMyObject->hTspIf = (ANSC_HANDLE)NULL;
    }

    if ( pCenIf )
    {
        AnscFreeMemory(pCenIf);

        pMyObject->hCenIf = (ANSC_HANDLE)NULL;
    }

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    if ( pMyObject->ShellPrompt )
    {
        AnscFreeMemory(pMyObject->ShellPrompt);
    }
#endif

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT       )hThisObject;
    PTELNET_TSP_INTERFACE           pTspIf       = (PTELNET_TSP_INTERFACE    )pMyObject->hTspIf;
    PSCLI_SHELL_CEN_INTERFACE       pCenIf       = (PSCLI_SHELL_CEN_INTERFACE)pMyObject->hCenIf;
    errno_t   rc = -1;

    if ( !pTspIf )
    {
        pTspIf = (PTELNET_TSP_INTERFACE)AnscAllocateMemory(sizeof(TELNET_TSP_INTERFACE));

        if ( !pTspIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTspIf = (ANSC_HANDLE)pTspIf;
        }

        rc = STRCPY_S_NOCLOBBER(pTspIf->Name, sizeof(pTspIf->Name), SCLI_SHELL_TSP_INTERFACE_NAME);
        ERR_CHK(rc);

        pTspIf->InterfaceId   = SCLI_SHELL_TSP_INTERFACE_ID;
        pTspIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pTspIf->Size          = sizeof(TELNET_TSP_INTERFACE);

        pTspIf->Accept        = ScliShoAccept;
        pTspIf->Serve         = ScliShoServe;
        pTspIf->Signal        = ScliShoSignal;
    }

    if ( !pCenIf )
    {
        pCenIf = (PSCLI_SHELL_CEN_INTERFACE)AnscAllocateMemory(sizeof(SCLI_SHELL_CEN_INTERFACE));

        if ( !pCenIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCenIf = (ANSC_HANDLE)pCenIf;
        }

        rc = STRCPY_S_NOCLOBBER(pCenIf->Name, sizeof(pCenIf->Name), SCLI_SHELL_CEN_INTERFACE_NAME);
        ERR_CHK(rc);

        pCenIf->InterfaceId     = SCLI_SHELL_CEN_INTERFACE_ID;
        pCenIf->hOwnerContext   = (ANSC_HANDLE)pMyObject;
        pCenIf->Size            = sizeof(SCLI_SHELL_CEN_INTERFACE);

        pCenIf->Notify          = ScliShoCenNotify;
        pCenIf->SetShellPrompt  = ScliShoCenSetShellPrompt;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        ScliShoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
ScliShoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSCLI_SHELL_OBJECT              pMyObject    = (PSCLI_SHELL_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                      = SCLI_SHELL_OID;
    pMyObject->Create                   = ScliShoCreate;
    pMyObject->Remove                   = ScliShoRemove;
    pMyObject->EnrollObjects            = ScliShoEnrollObjects;
    pMyObject->Initialize               = ScliShoInitialize;

    pMyObject->GetProperty              = ScliShoGetProperty;
    pMyObject->SetProperty              = ScliShoSetProperty;
    pMyObject->ResetProperty            = ScliShoResetProperty;
    pMyObject->Reset                    = ScliShoReset;

    pMyObject->GetTspIf                 = ScliShoGetTspIf;
    pMyObject->SetTspIf                 = ScliShoSetTspIf;
    pMyObject->GetBmc2SccIf             = ScliShoGetBmc2SccIf;
    pMyObject->SetBmc2SccIf             = ScliShoSetBmc2SccIf;
    pMyObject->SetAuthIf                = ScliShoSetAuthIf;
    pMyObject->SetTscIf                 = ScliShoSetTscIf;

    pMyObject->GetSession               = ScliShoGetSession;
    pMyObject->CreateSession            = ScliShoCreateSession;
    pMyObject->RemoveSession            = ScliShoRemoveSession;
    pMyObject->GetNumActiveSessions     = ScliShoGetNumActiveSessions;

    pMyObject->StartUserAuth            = ScliShoStartUserAuth;
    pMyObject->DoAuth                   = ScliShoDoAuth;

    pMyObject->ProcessCmdData           = ScliShoProcessCmdData;
    pMyObject->ProcessCmdChar           = ScliShoProcessCmdChar;
    pMyObject->RunCmd                   = ScliShoRunCmd;
    pMyObject->IsCmdModified            = ScliShoIsCmdModified;
    pMyObject->CacheCmd                 = ScliShoCacheCmd;
    pMyObject->RunBuiltInCmd            = ScliShoRunBuiltInCmd;
    pMyObject->RunNonBuiltInCmd         = ScliShoRunNonBuiltInCmd;
#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    pMyObject->RunShellCmd              = ScliShoRunShellCmd;
#endif
    pMyObject->ValidateCmdArgs          = ScliShoValidateCmdArgs;
    pMyObject->GetCmdCode               = ScliShoGetCmdCode;
    pMyObject->GetPrevCmd               = ScliShoGetPrevCmd;
    pMyObject->GetNextCmd               = ScliShoGetNextCmd;
    pMyObject->GetFirstCmd              = ScliShoGetFirstCmd;
    pMyObject->GetLastCmd               = ScliShoGetLastCmd;
    pMyObject->GetEscCmdCode            = ScliShoGetEscCmdCode;

    pMyObject->AsyncRunCmd              = ScliShoAsyncRunCmdTask;

    pMyObject->RunBicExit               = ScliShoRunBicExit;
    pMyObject->RunBicClear              = ScliShoRunBicClear;
#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    pMyObject->RunBicShell              = ScliShoRunBicShell;
#endif
    pMyObject->RunBicHistory            = ScliShoRunBicHistory;
    pMyObject->RunBicCursorBackward     = ScliShoRunBicCursorBackward;
    pMyObject->RunBicCursorForward      = ScliShoRunBicCursorForward;
    pMyObject->RunBicCursorHome         = ScliShoRunBicCursorHome;
    pMyObject->RunBicCursorEnd          = ScliShoRunBicCursorEnd;
    pMyObject->RunBicDel                = ScliShoRunBicDel;
    pMyObject->RunBicBackspace          = ScliShoRunBicBackspace;
    pMyObject->RunBicBreak              = ScliShoRunBicBreak;
    pMyObject->RunBicAutoCompletion     = ScliShoRunBicAutoCompletion;
    pMyObject->RunBicHelp               = ScliShoRunBicHelp;

    pMyObject->DoBufferedCommands       = ScliShoDoBufferedCommands;
    pMyObject->GetBufferedCmd           = ScliShoGetBufferedCmd;
    pMyObject->BufferCmd                = ScliShoBufferCmd;
    pMyObject->ClearBufferCmd           = ScliShoClearBufferCmd;

    pMyObject->GetInputMode             = ScliShoGetInputMode;
    pMyObject->RedrawTextBoxInput       = ScliShoRedrawTextBoxInput;
    pMyObject->MoveToTextBox            = ScliShoMoveToTextBox;

    pMyObject->ShellPromptChanged       = ScliShoShellPromptChanged;

    pMyObject->InitTdo                  = ScliShoInitTdo;
    pMyObject->KillTdo                  = ScliShoKillTdo;
    pMyObject->StartTdo                 = ScliShoStartTdo;
    pMyObject->StopTdo                  = ScliShoStopTdo;

    AnscInitializeLock       (&pMyObject->SessionListLock);
    AnscSListInitializeHeader(&pMyObject->SessionList    );

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    pMyObject->ShellPrompt = NULL;
#endif

    return  ANSC_STATUS_SUCCESS;
}

