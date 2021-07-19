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

    module:	slap_obo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Obj Broker Object.

        *   SlapOboIsRemoveable
        *   SlapOboSetPathName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/05/03    initial revision.

**********************************************************************/


#include "slap_obo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapOboIsRemoveable
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

BOOL
SlapOboIsRemoveable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject      = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity = (PSLAP_OBJ_ENTITY_OBJECT)pMyObject->hSlapObjEntity;
    PSLAP_OBJ_PROXY_OBJECT          pSlapObjProxy  = (PSLAP_OBJ_PROXY_OBJECT )pMyObject->hSlapObjProxy;
    ULONG                           ulSonOboCount  = 0;
    ULONG                           i              = 0;

    if ( pSlapObjEntity || pSlapObjProxy )
    {
        return  FALSE;
    }

    for ( i = 0; i < SLAP_OBO_OBO_TABLE_SIZE; i++ )
    {
        ulSonOboCount += AnscQueueQueryDepth(&pMyObject->OboTable[i]);
    }

    if ( ulSonOboCount > 0 )
    {
        return  FALSE;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOboSetPathName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOboSetPathName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PSLAP_OBJ_BROKER_OBJECT         pMyObject    = (PSLAP_OBJ_BROKER_OBJECT)hThisObject;
    errno_t                         rc           = -1;

    if ( pMyObject->PathName )
    {
        AnscFreeMemory(pMyObject->PathName);

        pMyObject->PathName = NULL;
    }

    if ( path )
    {
        pMyObject->PathName = (char*)AnscAllocateMemory(AnscSizeOfString(path) + 1);

        if ( pMyObject->PathName )
        {
            rc = strcpy_s(pMyObject->PathName, (AnscSizeOfString(path) + 1), path);
            ERR_CHK(rc);
        }
    }
    else
    {
        pMyObject->PathName = (char*)AnscAllocateMemory(1);
    }

    return  ANSC_STATUS_SUCCESS;
}
