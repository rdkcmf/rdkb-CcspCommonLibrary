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

    module:	ansc_xsink_state.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xsink Object.

        *   AnscXsinkGetXsocket
        *   AnscXsinkSetXsocket
        *   AnscXsinkAttach
        *   AnscXsinkDetach

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/02    initial revision.

**********************************************************************/


#include "ansc_xsocket_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXsinkGetXsocket
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function retrieves the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     state of object.

**********************************************************************/

ANSC_HANDLE
AnscXsinkGetXsocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;

    return  pMyObject->hXsocketObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkSetXsocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXsocket
            );

    description:

        This function configures the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXsocket
                Specifies the state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkSetXsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;

    pMyObject->hXsocketObject = hXsocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkAttach
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXsocket
            );

    description:

        This function configures the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXsocket
                Specifies the state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkAttach
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXsocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hXsocket);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkDetach
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function configures the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkDetach
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
