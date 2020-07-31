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

    module:	ansc_lpcco_cpcif.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoCpcCallDispatch
        *   AnscLpccoCpcFreeOutputBuffer
        *   AnscLpccoCpcNotifyEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/24/03    initial revision.

**********************************************************************/


#include "ansc_lpcco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoCpcCallDispatch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoCpcCallDispatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);
    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_internalError;

    return  ANSC_STATUS_INTERNAL_ERROR;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoCpcFreeOutputBuffer
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pOutputBuffer
            );

    description:

        This function is called by the underlying LPC connector to
        release the output buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pOutputBuffer
                Specifies the output buffer to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoCpcFreeOutputBuffer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pOutputBuffer
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pOutputBuffer);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoCpcNotifyEvent
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the underlying LPC connector to
        report an event.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the LPC event to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the event-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoCpcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulEvent);
    UNREFERENCED_PARAMETER(hReserved);
    return  ANSC_STATUS_UNAPPLICABLE;
}
