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

    module:	ansc_bsto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Broker Server Tcp Object.

        *   AnscBstoGetHostAddress
        *   AnscBstoSetHostAddress
        *   AnscBstoGetHostPort
        *   AnscBstoSetHostPort
        *   AnscBstoGetWorker
        *   AnscBstoSetWorker
        *   AnscBstoGetMaxMessageSize
        *   AnscBstoSetMaxMessageSize
        *   AnscBstoGetEngineCount
        *   AnscBstoSetEngineCount
        *   AnscBstoGetMinSocketCount
        *   AnscBstoSetMinSocketCount
        *   AnscBstoGetMaxSocketCount
        *   AnscBstoSetMaxSocketCount
        *   AnscBstoGetSocketTimeOut
        *   AnscBstoSetSocketTimeOut
        *   AnscBstoGetMode
        *   AnscBstoSetMode
        *   AnscBstoReset

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

        PUCHAR
        AnscBstoGetHostAddress
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

PUCHAR
AnscBstoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->HostAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetHostAddress
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->HostAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscBstoGetHostPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

USHORT
AnscBstoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetHostPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usPort
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      usPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->HostPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBstoGetWorker
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscBstoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->hWorker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetWorker
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWorker,
                ULONG                       ulSize
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWorker
                Specifies the object state to be configured.

                ULONG                       ulSize
                Specifies the size of the worker object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    if ( pMyObject->hWorker )
    {
        AnscFreeMemory(pMyObject->hWorker);

        pMyObject->hWorker = (ANSC_HANDLE)NULL;
    }

    if ( ulSize > 0 )
    {
        pMyObject->hWorker = (ANSC_HANDLE)AnscAllocateMemory(ulSize);

        if ( pMyObject->hWorker )
        {
            AnscCopyMemory
                (
                    (PVOID)pMyObject->hWorker,
                    hWorker,
                    ulSize
                );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetMaxMessageSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxMessageSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetMaxMessageSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSize
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSize
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MaxMessageSize = ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetEngineCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->EngineCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetEngineCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->EngineCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetMinSocketCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MinSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetMinSocketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MinSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetMaxSocketCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetMaxSocketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MaxSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetSocketTimeOut
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->SocketTimeOut;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetSocketTimeOut
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->SocketTimeOut = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBstoGetMode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBstoGetMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->Mode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoSetMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->Mode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->StartTime = 0;
    pMyObject->TccCount  = 0;
    pMyObject->MccCount  = 0;
    pMyObject->TrcCount  = 0;
    pMyObject->MrcCount  = 0;
    pMyObject->TscCount  = 0;
    pMyObject->MscCount  = 0;

    return  ANSC_STATUS_SUCCESS;
}
