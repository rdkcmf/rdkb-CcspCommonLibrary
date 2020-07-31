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

    module:	ansc_tdo_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Timer Descriptor Object.

        *   AnscTdoGetScheduler
        *   AnscTdoSetScheduler
        *   AnscTdoGetClient
        *   AnscTdoSetClient
        *   AnscTdoGetStartTime
        *   AnscTdoSetStartTime
        *   AnscTdoGetTimestamp
        *   AnscTdoSetTimestamp
        *   AnscTdoGetTimerType
        *   AnscTdoSetTimerType
        *   AnscTdoGetTimerMode
        *   AnscTdoSetTimerMode
        *   AnscTdoGetInterval
        *   AnscTdoSetInterval
        *   AnscTdoGetTimeToFire
        *   AnscTdoSetTimeToFire
        *   AnscTdoGetCallCounter
        *   AnscTdoSetCallCounter
        *   AnscTdoGetState
        *   AnscTdoSetState

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

        ANSC_HANDLE
        AnscTdoGetScheduler
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscTdoGetScheduler
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->hScheduler;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetScheduler
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hScheduler
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hScheduler
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetScheduler
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hScheduler
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->hScheduler = hScheduler;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscTdoGetClient
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ANSC_HANDLE
AnscTdoGetClient
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->hClient;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetClient
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClient
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClient
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->hClient = hClient;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetStartTime
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetStartTime
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->StartTime;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetStartTime
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTime
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTime
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetStartTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->StartTime = ulTime;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetTimestamp
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->Timestamp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetTimestamp
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTime
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTime
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetTimestamp
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->Timestamp = ulTime;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetTimerType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetTimerType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->TimerType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetTimerType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTimerType
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTimerType
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetTimerType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimerType
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->TimerType = ulTimerType;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetTimerMode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetTimerMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->TimerMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetTimerMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTimerMode
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTimerMode
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetTimerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimerMode
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->TimerMode = ulTimerMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetInterval
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetInterval
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->Interval;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetInterval
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulInterval
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulInterval
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetInterval
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInterval
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->Interval = ulInterval;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetTimeToFire
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetTimeToFire
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->TimeToFire;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetTimeToFire
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTime
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTime
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetTimeToFire
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->TimeToFire = ulTime;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscTdoGetCallCounter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

ULONG
AnscTdoGetCallCounter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->CallCounter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetCallCounter
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCounter
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCounter
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetCallCounter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCounter
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->CallCounter = ulCounter;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscTdoGetState
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object information.

**********************************************************************/

BOOL
AnscTdoGetState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    return  pMyObject->bScheduled;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTdoSetState
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bScheduled
            );

    description:

        This function is called to configure object information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bScheduled
                Specifies the object information to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscTdoSetState
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bScheduled
    )
{
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMyObject    = (PANSC_TIMER_DESCRIPTOR_OBJECT)hThisObject;

    pMyObject->bScheduled = bScheduled;

    return  ANSC_STATUS_SUCCESS;
}
