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

    module:	ansc_dsto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoGetHostAddress
        *   AnscDstoSetHostAddress
        *   AnscDstoGetHostPort
        *   AnscDstoSetHostPort
        *   AnscDstoGetSdtoPatient
        *   AnscDstoGetWorker
        *   AnscDstoSetWorker
        *   AnscDstoGetMaxMessageSize
        *   AnscDstoSetMaxMessageSize
        *   AnscDstoGetEngineCount
        *   AnscDstoSetEngineCount
        *   AnscDstoGetMinSocketCount
        *   AnscDstoSetMinSocketCount
        *   AnscDstoGetMaxSocketCount
        *   AnscDstoSetMaxSocketCount
        *   AnscDstoGetMode
        *   AnscDstoSetMode
        *   AnscDstoGetTlsScsIf
        *   AnscDstoSetTlsScsIf
        *   AnscDstoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#include "ansc_dsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscDstoGetHostAddress
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
AnscDstoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->HostAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetHostAddress
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
AnscDstoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->HostAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscDstoGetHostPort
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
AnscDstoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetHostPort
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
AnscDstoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->HostPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoGetSdtoPatient
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
AnscDstoGetSdtoPatient
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->hSdtoPatient;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoGetWorker
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
AnscDstoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->hWorker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetWorker
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
AnscDstoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

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
        AnscDstoGetMaxMessageSize
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
AnscDstoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxMessageSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetMaxMessageSize
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
AnscDstoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MaxMessageSize = ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDstoGetEngineCount
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
AnscDstoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->EngineCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetEngineCount
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
AnscDstoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->EngineCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDstoGetMinSocketCount
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
AnscDstoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MinSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetMinSocketCount
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
AnscDstoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MinSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDstoGetMaxSocketCount
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
AnscDstoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetMaxSocketCount
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
AnscDstoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->MaxSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDstoGetMode
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
AnscDstoGetMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->Mode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetMode
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
AnscDstoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->Mode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoGetTlsScsIf
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
AnscDstoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    return  pMyObject->hTlsScsIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoSetTlsScsIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->hTlsScsIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoReset
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
AnscDstoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;

    pMyObject->StartTime = 0;
    pMyObject->TccCount  = 0;
    pMyObject->MccCount  = 0;
    pMyObject->TrcCount  = 0;
    pMyObject->MrcCount  = 0;
    pMyObject->TscCount  = 0;
    pMyObject->MscCount  = 0;
    
#ifdef _ANSC_IPV6_COMPATIBLE_
    AnscZeroMemory(pMyObject->HostName, sizeof(pMyObject->HostName));
    
    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        if ( pMyObject->pHostAddr2 )
        {
            _xskt_freeaddrinfo(pMyObject->pHostAddr2);
            /*
             * Setting the ptr to NULL to avoid freeing it twice 
             */
            pMyObject->pHostAddr2 = NULL;
        }
    }
    else
    {
        if ( pMyObject->pHostAddr1 )
        {
            _ansc_freeaddrinfo(pMyObject->pHostAddr1);
            /*
             * Setting the ptr to NULL to avoid freeing it twice 
             */
            pMyObject->pHostAddr1 = NULL;
        }
    }
#endif

    return  ANSC_STATUS_SUCCESS;
}
