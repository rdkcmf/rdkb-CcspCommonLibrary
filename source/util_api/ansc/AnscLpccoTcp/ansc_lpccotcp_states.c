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

    module:	ansc_lpccotcp_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpGetMyAddress
        *   AnscLpccoTcpSetMyAddress
        *   AnscLpccoTcpGetMyPort
        *   AnscLpccoTcpSetMyPort
        *   AnscLpccoTcpGetManagerAddress
        *   AnscLpccoTcpSetManagerAddress
        *   AnscLpccoTcpGetManagerPort
        *   AnscLpccoTcpSetManagerPort
        *   AnscLpccoTcpReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscLpccoTcpGetMyAddress
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
AnscLpccoTcpGetMyAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    return  pMyObject->MyAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSetMyAddress
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
AnscLpccoTcpSetMyAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    pMyObject->MyAddress.Value = AnscReadUlong(address);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscLpccoTcpGetMyPort
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
AnscLpccoTcpGetMyPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    return  pMyObject->MyPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSetMyPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSetMyPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    pMyObject->MyPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscLpccoTcpGetManagerAddress
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
AnscLpccoTcpGetManagerAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    return  pMyObject->ManagerAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSetManagerAddress
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
AnscLpccoTcpSetManagerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    pMyObject->ManagerAddress.Value = AnscReadUlong(address);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscLpccoTcpGetManagerPort
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
AnscLpccoTcpGetManagerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    return  pMyObject->ManagerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSetManagerPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSetManagerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    pMyObject->ManagerPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpReset
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
AnscLpccoTcpReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject    = (PANSC_LPCCO_TCP_OBJECT)hThisObject;

    AnscLpccoReset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
