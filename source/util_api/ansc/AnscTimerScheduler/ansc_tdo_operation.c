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

    module:	ansc_tdo_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Timer Descriptor Object.

        *   AnscTdoAcquireAccess
        *   AnscTdoReleaseAccess
        *   AnscTdoStart
        *   AnscTdoStop
        *   AnscTdoInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/01    initial revision.

**********************************************************************/


#include "ansc_tso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire the access to object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    AnscAcquireSpinLock(&pMyObject->AccessSpinLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the access to object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    AnscReleaseSpinLock(&pMyObject->AccessSpinLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to register the timer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pContainer   = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PANSC_TIMER_SCHEDULER_OBJECT    pScheduler   = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hScheduler;

    if ( pContainer )
    {
        returnStatus =
            pContainer->RegisterTimer
                (
                    (ANSC_HANDLE)pContainer,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else if ( pScheduler )
    {
        returnStatus =
            pScheduler->RegisterTimer
                (
                    (ANSC_HANDLE)pScheduler,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoStop
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the timer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoStop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pContainer   = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PANSC_TIMER_SCHEDULER_OBJECT    pScheduler   = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hScheduler;

    if ( pContainer )
    {
        returnStatus =
            pContainer->CancelTimer
                (
                    (ANSC_HANDLE)pContainer,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else if ( pScheduler )
    {
        returnStatus =
            pScheduler->CancelTimer
                (
                    (ANSC_HANDLE)pScheduler,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is invoked when the timer expires.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;
    PANSC_TDO_CLIENT_OBJECT         pClient      = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hClient;

    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    if ( pMyObject->CallCounter > 0 )
    {
        pMyObject->CallCounter--;
    }

    pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

    if ( pClient && pClient->Invoke )
    {
        ANSC_STATUS                 status;
#ifdef _ANSC_TDO_PROFILING_
        ULONG                       ElapsedTime;
        ULONG                       Timestamp;
        ULONG                       TotalTime;

        if( ! pMyObject->TdoFlags )
        {
            return ANSC_STATUS_SUCCESS;
        }
        Timestamp = AnscProfilingReadPcc();
#endif /* _ANSC_TDO_PROFILING_ */

        status = pClient->Invoke(pClient->hClientContext);

#ifdef _ANSC_TDO_PROFILING_
        ElapsedTime = AnscProfilingReadPcc() - Timestamp;

        AnscAcquireSpinLock(&pMyObject->AccessSpinLock);
        pMyObject->CurrTimestamp    = Timestamp;
        pMyObject->CurrElapsedTime  = ElapsedTime;
        if( pMyObject->LongestTime < ElapsedTime )
        {
            pMyObject->LongestTime = ElapsedTime;
        }
        pMyObject->TotalHits ++;
        TotalTime = pMyObject->TotalHitTime;
        pMyObject->TotalHitTime += ElapsedTime;
        if( TotalTime + ElapsedTime < TotalTime )
        {
            AnscTraceError(("!!! Tdo total time overflow @ [%s]--<%d>\n", 
                pMyObject->TdoInitFile, pMyObject->TdoInitLine));
        }
        AnscReleaseSpinLock(&pMyObject->AccessSpinLock);
        
#endif /* _ANSC_TDO_PROFILING_ */

        return status;
    }

    return  ANSC_STATUS_SUCCESS;
}
