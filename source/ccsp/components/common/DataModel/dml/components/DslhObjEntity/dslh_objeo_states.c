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

    module:	dslh_objeo_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Obj Entity Object.

        *   DslhObjeoEnableForbidNotify
        *   DslhObjeoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/26/05    initial revision.

**********************************************************************/


#include "dslh_objeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoEnableForbidNotify
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to configure an object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies the object state value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoEnableForbidNotify
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject        = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pParentObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pMyObject->hParentObjEntity;

    pMyObject->bForbidNotify = bEnabled;

    if ( bEnabled && pParentObjEntity )
    {
        pParentObjEntity->EnableForbidNotify((ANSC_HANDLE)pParentObjEntity, bEnabled);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoReset
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
DslhObjeoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject       = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_OBJEO_INSTANCE            pObjeoInstance  = (PDSLH_OBJEO_INSTANCE   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;

    /* reset the child objects/parameters */
    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ObjInsQueue);

        while ( pSLinkEntry )
        {
            pObjeoInstance = ACCESS_DSLH_OBJEO_INSTANCE(pSLinkEntry);
            pSLinkEntry    = AnscQueuePopEntry(&pMyObject->ObjInsQueue);

            AnscFreeMemory(pObjeoInstance);
        }
    }

    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ObjeoQueue);

        while ( pSLinkEntry )
        {
            pChildObjEntity = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ObjeoQueue);

            pChildObjEntity->Reset ((ANSC_HANDLE)pChildObjEntity);
            pChildObjEntity->Remove((ANSC_HANDLE)pChildObjEntity);
        }
    }

    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->VareoTable);

        while ( pSLinkEntry )
        {
            pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->VareoTable);

            DslhVareoReset ((ANSC_HANDLE)pChildVarEntity);
            DslhVareoRemove((ANSC_HANDLE)pChildVarEntity);
        }
    }

    /* reset itself */
    if ( pMyObject->ObjDescr )
    {
        if( pMyObject->ObjDescr->hParamArray != NULL)
        {
            AnscFreeMemory(pMyObject->ObjDescr->hParamArray);

            pMyObject->ObjDescr->hParamArray = NULL;
        }

    	DslhCwmpFreeObjectDescr(pMyObject->ObjDescr );
    	pMyObject->ObjDescr = NULL;
    }

    if ( pMyObject->LastName )
    {
        AnscFreeMemory(pMyObject->LastName);

        pMyObject->LastName = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
