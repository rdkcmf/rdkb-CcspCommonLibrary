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

    module:	slap_oro_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Obj Record Object.

        *   SlapOroGetRef
        *   SlapOroIncRef
        *   SlapOroDecRef
        *   SlapOroAcqAccess
        *   SlapOroRelAccess
        *   SlapOroMatch1
        *   SlapOroMatch2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/06/03    initial revision.

**********************************************************************/


#include "slap_oro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOroGetRef
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current ref count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     ref count.

**********************************************************************/

ULONG
SlapOroGetRef
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject    = (PSLAP_OBJ_RECORD_OBJECT)hThisObject;

    return  pMyObject->RefCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOroIncRef
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to increase the current ref count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     ref count.

**********************************************************************/

ULONG
SlapOroIncRef
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject    = (PSLAP_OBJ_RECORD_OBJECT)hThisObject;

    return  ++pMyObject->RefCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapOroDecRef
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to decrease the current ref count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     ref count.

**********************************************************************/

ULONG
SlapOroDecRef
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject    = (PSLAP_OBJ_RECORD_OBJECT)hThisObject;

    return  --pMyObject->RefCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOroAcqAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOroAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject    = (PSLAP_OBJ_RECORD_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOroRelAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOroRelAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject    = (PSLAP_OBJ_RECORD_OBJECT)hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapOroMatch1
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjContainer,
                ULONG                       instance_id
            );

    description:

        This function is called to match the Obj Container and the
        instance_id associated with a particular instance of the
        Srv Component Object. If instance_id is zero, we match the
        first active instance maintained by the Obj Container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjContainer
                Specifies the obj container to be matched.

                ULONG                       instance_id
                Specifies the instance_id to be matched. Note that this
                value is not used currently except value zero.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapOroMatch1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ULONG                       instance_id
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject         = (PSLAP_OBJ_RECORD_OBJECT   )hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pMyObject->hSlapObjContainer;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pMyObject->hSlapSrvComponent;

    if ( !pSlapObjContainer || !pSlapSrvComponent )
    {
        return  FALSE;
    }
    else if ( hObjContainer != (ANSC_HANDLE)pSlapObjContainer )
    {
        return  FALSE;
    }
    else if ( instance_id != 0 )
    {
        if ( pSlapSrvComponent->ObjInstanceId != instance_id )
        {
            return  FALSE;
        }
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapOroMatch2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjContainer,
                ANSC_HANDLE                 hSrvComponent
            );

    description:

        This function is called to match the Obj Container and the
        Srv Component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjContainer
                Specifies the obj container to be matched.

                ANSC_HANDLE                 hSrvComponent
                Specifies the srv component to be matched.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapOroMatch2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hSrvComponent
    )
{
    PSLAP_OBJ_RECORD_OBJECT         pMyObject         = (PSLAP_OBJ_RECORD_OBJECT   )hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pMyObject->hSlapObjContainer;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pMyObject->hSlapSrvComponent;

    if ( !pSlapObjContainer || !pSlapSrvComponent )
    {
        return  FALSE;
    }
    else if ( hObjContainer != (ANSC_HANDLE)pSlapObjContainer )
    {
        return  FALSE;
    }
    else if ( hSrvComponent != (ANSC_HANDLE)pSlapSrvComponent )
    {
        return  FALSE;
    }

    return  TRUE;
}
