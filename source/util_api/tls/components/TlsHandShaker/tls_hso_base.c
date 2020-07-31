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

    module:	tls_hso_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Hand Shaker Object.

        *   TlsHsoCreate
        *   TlsHsoRemove
        *   TlsHsoEnrollObjects
        *   TlsHsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/03    initial revision.

**********************************************************************/


#include "tls_hso_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsHsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Hand Shaker Object and
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
TlsHsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_HAND_SHAKER_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_HAND_SHAKER_OBJECT)AnscAllocateMemory(sizeof(TLS_HAND_SHAKER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, TLS_HAND_SHAKER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_HAND_SHAKER_OID;
    pBaseObject->Create            = TlsHsoCreate;
    pBaseObject->Remove            = TlsHsoRemove;
    pBaseObject->EnrollObjects     = TlsHsoEnrollObjects;
    pBaseObject->Initialize        = TlsHsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoRemove
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
TlsHsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject      = (PTLS_HAND_SHAKER_OBJECT      )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pStateTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hStateTimerIf;

    if ( pStateTimerObj )
    {
        pStateTimerObj->Remove((ANSC_HANDLE)pStateTimerObj);
    }

    if ( pStateTimerIf )
    {
        AnscFreeMemory(pStateTimerIf);
    }

    AnscFreeLock(&pMyObject->SyncLock  );
    TlsRcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoEnrollObjects
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
TlsHsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject      = (PTLS_HAND_SHAKER_OBJECT      )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PANSC_TDO_CLIENT_OBJECT         pStateTimerIf  = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hStateTimerIf;

    if ( !pStateTimerObj )
    {
        pStateTimerObj =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pStateTimerObj )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hStateTimerObj = (ANSC_HANDLE)pStateTimerObj;
        }

        pStateTimerObj->SetTimerType((ANSC_HANDLE)pStateTimerObj, ANSC_TIMER_TYPE_SPORADIC );
        pStateTimerObj->SetInterval ((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
    }

    if ( !pStateTimerIf )
    {
        pStateTimerIf = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pStateTimerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hStateTimerIf = (ANSC_HANDLE)pStateTimerIf;
        }

        pStateTimerIf->hClientContext = (ANSC_HANDLE)pMyObject;
        pStateTimerIf->Invoke         = TlsHsoStateTimerInvoke;

        pStateTimerObj->SetClient((ANSC_HANDLE)pStateTimerObj, (ANSC_HANDLE)pStateTimerIf);
    }

    TlsRcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoInitialize
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
TlsHsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject    = (PTLS_HAND_SHAKER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented progtlsming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    TlsRcoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java progtlsmer perspective, it's the best we can get for universal embedded network
     * progtlsming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                  = TLS_HAND_SHAKER_OID;
    pMyObject->Create               = TlsHsoCreate;
    pMyObject->Remove               = TlsHsoRemove;
    pMyObject->EnrollObjects        = TlsHsoEnrollObjects;
    pMyObject->Initialize           = TlsHsoInitialize;

    pMyObject->RecordType           = TLS_RECORD_TYPE_handshake;
    pMyObject->ProgressBits         = 0;
    pMyObject->MsgBuffer            = NULL;
    pMyObject->MsgBufferSize        = TLS_HSO_DEF_MSG_BUFFER_SIZE;
    pMyObject->MsgOffset            = 0;
    pMyObject->FragMsgBuffer        = NULL;
    pMyObject->FragMsgBufferSize    = TLS_HSO_DEF_MSG_BUFFER_SIZE;
    pMyObject->FragMsgOffset        = 0;

    pMyObject->Reset                = TlsHsoReset;

    pMyObject->Start                = TlsHsoStart;
    pMyObject->Abort                = TlsHsoAbort;
    pMyObject->Establish            = TlsHsoEstablish;
    pMyObject->Agree                = TlsHsoAgree;

    pMyObject->CalKeys30            = TlsHsoCalKeys30;
    pMyObject->CalKeys31            = TlsHsoCalKeys31;
    pMyObject->CalMasterSecret30    = TlsHsoCalMasterSecret30;
    pMyObject->CalMasterSecret31    = TlsHsoCalMasterSecret31;
    pMyObject->ChangeCipherW        = TlsHsoChangeCipherW;
    pMyObject->ChangeCipherR        = TlsHsoChangeCipherR;

    pMyObject->Engage               = TlsHsoEngage;
    pMyObject->Cancel               = TlsHsoCancel;
    pMyObject->SaveHandshakeMsg     = TlsHsoSaveHandshakeMsg;
    pMyObject->SaveFragHandshakeMsg = TlsHsoSaveFragHandshakeMsg;
    pMyObject->StateTimerInvoke     = TlsHsoStateTimerInvoke;

    AnscInitializeLock(&pMyObject->SyncLock  );

    TlsInitSessionState((&pMyObject->SessionState));
    TlsInitRecordState ((&pMyObject->RecordStateW));
    TlsInitRecordState ((&pMyObject->RecordStateR));

    return  ANSC_STATUS_SUCCESS;
}
