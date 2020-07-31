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

    module:	ansc_bsto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Broker Server Tcp Object.

        *   AnscBstoEngage
        *   AnscBstoCancel
        *   AnscBstoStartEngines
        *   AnscBstoStopEngines

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#include "ansc_bsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the server broker operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pMyObject->hWorker;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pWorker->Init(pWorker->hWorkerContext);

        pMyObject->bActive = TRUE;
    }

    /*
     * The underlying socket wrapper may require an explicit startup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscStartupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    /*
     * To engage the Tcp Broker, we need to perform following acts in the respective order:
     *
     *      (1) manufacture and start all the engines objects
     *      (2) manufacture the global socket object pool
     */
    pMyObject->ManufactureEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureSocketPool((ANSC_HANDLE)pMyObject);
    pMyObject->StartEngines         ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the server broker operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pMyObject->hWorker;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pWorker->Unload(pWorker->hWorkerContext);

        pMyObject->bActive = FALSE;
    }

    pMyObject->StopEngines      ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroySocketPool((ANSC_HANDLE)pMyObject);

    /*
     * The underlying socket wrapper may require an explicit cleanup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_BSTO_MODE_XSOCKET )
    {
        AnscCleanupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscCleanupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoStartEngines
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start all the engines.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulFlags      = 0;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[i];
        ulFlags = pEngine->GetControlFlags((ANSC_HANDLE)pEngine);

        if ( pMyObject->Mode & ANSC_BSTO_MODE_ASYNC_SEND )
        {
            ulFlags |= ANSC_BETO_FLAG_ASYNC_SEND;
        }

        if ( pMyObject->Mode & ANSC_BSTO_MODE_NO_TIMEOUT )
        {
            ulFlags |= ANSC_BETO_FLAG_NO_TIMEOUT;
        }

        pEngine->SetControlFlags((ANSC_HANDLE)pEngine, ulFlags);
        pEngine->Start          ((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoStopEngines
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to stop all the engines.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoStopEngines
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[i];

        pEngine->Stop((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    return  ANSC_STATUS_SUCCESS;
}
