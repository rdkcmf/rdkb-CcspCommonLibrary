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

    module:	ansc_bsto_accept.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Broker Server Tcp Object.

        *   AnscBstoAcceptTask

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
        AnscBstoAcceptTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the engage() function
        call to handle incoming socket requests..

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject     = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    ULONG                           ulMccCount    = 0;
    ULONG                           ulMrcCount    = 0;
    ULONG                           ulMscCount    = 0;
    ULONG                           ulMwtPrevious = AnscGetTickInSeconds();

    AnscTrace("AnscBstoAcceptTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bActive )
    {
        /*
         * To accurately estimate the performance of the Broker Sever, we not only have to record
         * the total number of rejection/serving, but also the rejection/serving rate in the last
         * minute. So we will be able to determine when the problem really takes place.
         */
        if ( (AnscGetTickInSeconds() - ulMwtPrevious) > 60 )
        {
            pMyObject->MccCount = ulMccCount;
            pMyObject->MrcCount = ulMrcCount;
            pMyObject->MscCount = ulMscCount;
            ulMccCount          = 0;
            ulMrcCount          = 0;
            ulMscCount          = 0;

            ulMwtPrevious = AnscGetTickInSeconds();
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
