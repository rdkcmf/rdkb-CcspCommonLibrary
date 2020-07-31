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

    module:	ansc_oco_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Object Container Object.

        *   AnscOcoGetExternalOmo
        *   AnscOcoSetExternalOmo
        *   AnscOcoGetInternalOmo
        *   AnscOcoSetInternalOmo
        *   AnscOcoGetTso
        *   AnscOcoSetTso
        *   AnscOcoGetIpcPort
        *   AnscOcoSetIpcPort
        *   AnscOcoGetCarrier
        *   AnscOcoSetCarrier
        *   AnscOcoGetOpMask
        *   AnscOcoSetOpMask
        *   AnscOcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#include "ansc_oco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetExternalOmo
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
AnscOcoGetExternalOmo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hExternalObjectMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetExternalOmo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOmo
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOmo
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetExternalOmo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOmo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hOmo);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetInternalOmo
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
AnscOcoGetInternalOmo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hInternalObjectMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetInternalOmo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOmo
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOmo
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetInternalOmo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOmo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hOmo);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetTso
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
AnscOcoGetTso
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hTimerScheduler;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetTso
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTso
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTso
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetTso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTso
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hTso);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetIpcPort
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
AnscOcoGetIpcPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hIpcPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetIpcPort
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hIpcPort
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hIpcPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetIpcPort
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIpcPort
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    pMyObject->hIpcPort = hIpcPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOcoGetCarrier
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
AnscOcoGetCarrier
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hCarrierContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetCarrier
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCarrier,
                ULONG                       ulSize
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCarrier
                Specifies the object state to be configured.

                ULONG                       ulSize
                Specifies the size of the context to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetCarrier
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    if ( pMyObject->hCarrierContext )
    {
        AnscFreeMemory(pMyObject->hCarrierContext);

        pMyObject->hCarrierContext = (ANSC_HANDLE)NULL;
    }

    if ( ulSize > 0 )
    {
        pMyObject->hCarrierContext = (ANSC_HANDLE)AnscAllocateMemory(ulSize);

        if ( pMyObject->hCarrierContext )
        {
            AnscCopyMemory
                (
                    (PVOID)pMyObject->hCarrierContext,
                    hCarrier,
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
        AnscOcoGetOpMask
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
AnscOcoGetOpMask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->OpMask;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSetOpMask
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMask
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMask
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSetOpMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMask
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    pMyObject->OpMask = ulMask;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoReset
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
AnscOcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
