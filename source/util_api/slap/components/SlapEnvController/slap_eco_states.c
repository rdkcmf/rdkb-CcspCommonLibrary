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

    module:	slap_eco_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Env Controller Object.

        *   SlapEcoGetSlapUoaIf
        *   SlapEcoGetSlapBssIf
        *   SlapEcoGetSlapObjMapper
        *   SlapEcoGetSlapVarConverter
        *   SlapEcoGetSlapVarMapper
        *   SlapEcoGetSlapDefContainer
        *   SlapEcoGetPendingCallInfo
        *   SlapEcoGetProperty
        *   SlapEcoSetProperty
        *   SlapEcoResetProperty
        *   SlapEcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#include "slap_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapUoaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapUoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapBssIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapBssIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapBssIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapObjMapper
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapObjMapper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapObjMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapVarConverter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapVarConverter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapVarConverter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapVarMapper
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapVarMapper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapVarMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoGetSlapDefContainer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapEcoGetSlapDefContainer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapDefContainer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapEcoGetPendingCallInfo
            (
                ANSC_HANDLE                 hThisObject,
                char**                      call_name,
                ULONG*                      call_timestamp,
                ULONG*                      call_age
            );

    description:

        This function is called to retrieve pending call information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
                char**                      call_name
                Return the name of the pending call. This function
                allocates memory for the returned string, and the
                caller is responsible for freeing it.

                ULONG*                      call_timestamp
                Returns the timestamp when the pending call is first
                made in number of seconds after system is up.

                ULONG*                      call_age
                Returns the number of seconds that has passed up after
                the call is made.

    return:     TRUE if there's a pending call, FALSE otherwise.

**********************************************************************/

BOOL
SlapEcoGetPendingCallInfo
    (
        ANSC_HANDLE                 hThisObject,
        char**                      call_name,
        ULONG*                      call_timestamp,     /* in number of seconds after system is up */
        ULONG*                      call_age            /* in number of seconds */
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;

    *call_name      = AnscCloneString(pMyObject->PendingCallName);
    *call_timestamp = pMyObject->PendingCallTimestamp;
    *call_age       = AnscGetTickInSeconds() - pMyObject->PendingCallTimestamp;

    return  pMyObject->bCallPending;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty    = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *(PSLAP_ENV_CONTROLLER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty    = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSLAP_ENV_CONTROLLER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty    = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    pProperty->bPoolingEnabled = TRUE;
    pProperty->ObjectPoolSize  = SLAP_DEF_OBJECT_POOL_SIZE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject    = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->DelAllSlapContainers((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
