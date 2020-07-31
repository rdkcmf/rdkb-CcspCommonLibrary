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

    module:	slap_opo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Obj Proxy Object.

        *   SlapOpoSetObjDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#include "slap_opo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOpoSetObjDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescriptor
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescriptor
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOpoSetObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescriptor
    )
{
    PSLAP_OBJ_PROXY_OBJECT          pMyObject      = (PSLAP_OBJ_PROXY_OBJECT )hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)pMyObject->hObjDescriptor;

    if ( pObjDescriptor )
    {
        SlapFreeObjDescriptor(pObjDescriptor);

        pMyObject->hObjDescriptor = (ANSC_HANDLE)NULL;
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)AnscAllocateMemory(sizeof(SLAP_OBJECT_DESCRIPTOR));

        if ( pObjDescriptor )
        {
            pMyObject->hObjDescriptor = (ANSC_HANDLE)pObjDescriptor;
        }
        else
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    SlapCloneObjDescriptor(((PSLAP_OBJECT_DESCRIPTOR)hObjDescriptor), pObjDescriptor);

    return  ANSC_STATUS_SUCCESS;
}
