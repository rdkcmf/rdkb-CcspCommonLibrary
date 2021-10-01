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

    module:	dslh_objco_table_access.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Objco Table Object.

        *   DslhObjcoTableIsUpdated
        *   DslhObjcoTableSynchronize
        *   DslhObjcoTableLock
        *   DslhObjcoTableUnlock
        *   DslhObjcoTableGetEntryObjectCount
        *   DslhObjcoTableGetEntryObject
        *   DslhObjcoTableNewEntryObject
        *   DslhObjcoTableAddEntryObject
        *   DslhObjcoTableDelEntryObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/03/2005    initial revision.

**********************************************************************/


#include "dslh_objco_table_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjcoTableIsUpdated
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to find out
        whether the content of this dynamic table is updated.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhObjcoTableIsUpdated
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject         = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord        = (PDSLH_OBJ_RECORD_OBJECT )pMyObject->hDslhObjRecord;
    /*PDSLH_OBJ_ENTITY_OBJECT         pObjEntity        = (PDSLH_OBJ_ENTITY_OBJECT )pObjRecord->hDslhObjEntity;*/
    PDSLH_TR69_INTERFACE            pDslhTr69If       = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE)pCpeController->GetDslhMprIf(pCpeController);
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase  = (PDSLH_WMP_DATABASE_OBJECT  )pCpeController->hDslhWmpDatabase;
    BOOL                            bUpdated          = FALSE;
    
    if( pDslhMprIf->GetReqCounter(pDslhMprIf->hOwnerContext) != pMyObject->uReqCounter)
    {  
        if( pDslhTr69If == NULL)
        {
            return FALSE;
        }

        /*
        if( pObjEntity->ObjDescr && pObjEntity->ObjDescr->Name)
        {
    		AnscTrace("++++++ Check IsUpdate('%s') ++++++\n", pObjEntity->ObjDescr->Name);
        }
        */

        pMyObject->uReqCounter = pDslhMprIf->GetReqCounter(pDslhMprIf->hOwnerContext);

        if( pDslhTr69If->bUseFullName)
        {
            if( pDslhTr69If->Methods.MethodWithFullName.IsUpdated != NULL)
            {
                bUpdated = pDslhTr69If->Methods.MethodWithFullName.IsUpdated(pMyObject->hInsContext, pObjRecord->FullName);
            }
        }
        else
        {
            if( pDslhTr69If->Methods.MethodWithLastName.IsUpdated != NULL)
            {
                bUpdated = pDslhTr69If->Methods.MethodWithLastName.IsUpdated(pMyObject->hInsContext);
            }
        }

        if ( bUpdated )
        {
            pDslhWmpDatabase->FlushDynObjVar((ANSC_HANDLE)pDslhWmpDatabase, pObjRecord);
        }
    }

    return  bUpdated;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoTableSynchronize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to synchronize
        the table content of a dynamic table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoTableSynchronize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject           = (PDSLH_OBJCO_TABLE_OBJECT   )hThisObject;
    PDSLH_TR69_INTERFACE            pDslhTr69If         = (PDSLH_TR69_INTERFACE       )pMyObject->hDslhTr69If;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    /* PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase; */
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;

    if( pDslhTr69If == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /* Reset ObjVarArray first */
    /* pDslhWmpDatabase->ResetObjVarArray((ANSC_HANDLE)pDslhWmpDatabase); */

    if( pDslhTr69If->bUseFullName)
    {
        if( pDslhTr69If->Methods.MethodWithFullName.Synchronize != NULL)
        {
            return pDslhTr69If->Methods.MethodWithFullName.Synchronize(pMyObject->hInsContext, pObjRecord->FullName);
        }
    }
    else
    {
        if( pDslhTr69If->Methods.MethodWithLastName.Synchronize != NULL)
        {
            CcspTraceDebug(("Before Synchronizing:%s\n", pObjRecord->FullName));
            returnStatus = pDslhTr69If->Methods.MethodWithLastName.Synchronize(pMyObject->hInsContext);
            CcspTraceDebug(("After Synchronizing:%s:%lu\n", pObjRecord->FullName, returnStatus));

            return returnStatus;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoTableLock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to lock the table

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoTableLock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoTableUnlock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to unlock the table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoTableUnlock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjcoTableGetEntryObjectCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to retrieve the
        number of entry objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     entry number.

**********************************************************************/

ULONG
DslhObjcoTableGetEntryObjectCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject    = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pDslhTr69If  = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord   = (PDSLH_OBJ_RECORD_OBJECT )pMyObject->hDslhObjRecord;
    
    if( pDslhTr69If == NULL)
    {
        return 0;
    }

    if( pDslhTr69If->bUseFullName)
    {
        if( pDslhTr69If->Methods.MethodWithFullName.GetEntryCount != NULL)
        {
            return pDslhTr69If->Methods.MethodWithFullName.GetEntryCount(pMyObject->hInsContext, pObjRecord->FullName);
        }
    }
    else
    {
        if( pDslhTr69If->Methods.MethodWithLastName.GetEntryCount != NULL)
        {
            return pDslhTr69If->Methods.MethodWithLastName.GetEntryCount(pMyObject->hInsContext);
        }
    }
   
    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjcoTableGetEntryObject
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ULONG*                      pulInsNumber
            );

    description:

        This function is called by the ObjRecord object to retrieve an
        entry object by matching the index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                Specifies the entry object's index to be matched.

                ULONG*                      pulInsNumber
                Returns the instance number of the matched entry.

    return:     entry object.

**********************************************************************/

ANSC_HANDLE
DslhObjcoTableGetEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG*                      pulInsNumber
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject             = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pDslhTr69If           = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_CONTROLLER_OBJECT     pEntryObj             = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord            = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity            = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity       = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PFN_DSLHOBJCO_CREATE            pfnObjcoCreate        = (PFN_DSLHOBJCO_CREATE       )NULL;
    ANSC_HANDLE                     hEntryHandle          = NULL;

    pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

    if( pChildObjEntity == NULL)
    {
        return NULL;
    }
    
    if( pDslhTr69If == NULL)
    {
        return NULL;
    }

    if( pDslhTr69If->bUseFullName)
    {
        if( pDslhTr69If->Methods.MethodWithFullName.GetEntry != NULL)
        {
            hEntryHandle = pDslhTr69If->Methods.MethodWithFullName.GetEntry(pMyObject->hInsContext, pObjRecord->FullName, ulIndex, pulInsNumber);
        }
    }
    else
    {
        if( pDslhTr69If->Methods.MethodWithLastName.GetEntry != NULL)
        {
            hEntryHandle = pDslhTr69If->Methods.MethodWithLastName.GetEntry(pMyObject->hInsContext, ulIndex, pulInsNumber);
        }
    }

   if( hEntryHandle == NULL)
    {
        return NULL;
    }

    if( pChildObjEntity->ObjDescr != NULL && pChildObjEntity->ObjDescr->pfnObjcoConstructor != NULL)
    {
        pfnObjcoCreate   = (PFN_DSLHOBJCO_CREATE)pChildObjEntity->ObjDescr->pfnObjcoConstructor;

        pEntryObj =
            (PDSLH_OBJ_CONTROLLER_OBJECT)pfnObjcoCreate
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }
    else
    {
        pEntryObj =
            (PDSLH_OBJ_CONTROLLER_OBJECT)DslhCreateObjController
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pEntryObj )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pEntryObj->hDslhCpeController          =   pMyObject->hDslhCpeController;
    pEntryObj->hInsContext                 =   (ANSC_HANDLE)hEntryHandle;
    pEntryObj->hParentInsContext           =   pMyObject->GetInsContext(pMyObject);

    if( pChildObjEntity->ObjDescr != NULL)
    {
        pEntryObj->hDslhTr69If = pChildObjEntity->ObjDescr->hDslhTr69If;
    }

    return (ANSC_HANDLE)pEntryObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjcoTableNewEntryObject
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the ObjRecord object to create a
        new entry object. Note that this new entry MUST NOT be added
        to the table upon creation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     entry object.

**********************************************************************/

ANSC_HANDLE
DslhObjcoTableNewEntryObject
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject             = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord            = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity            = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity       = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PFN_DSLHOBJCO_CREATE            pfnObjcoCreate        = (PFN_DSLHOBJCO_CREATE       )NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pEntryObj             = (PDSLH_OBJ_CONTROLLER_OBJECT)NULL;
    
    pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

    if( pChildObjEntity == NULL)
    {
        return NULL;
    }

    if( pChildObjEntity->ObjDescr != NULL && pChildObjEntity->ObjDescr->pfnObjcoConstructor != NULL)
    {
        pfnObjcoCreate   = (PFN_DSLHOBJCO_CREATE)pChildObjEntity->ObjDescr->pfnObjcoConstructor;

        pEntryObj =
            (PDSLH_OBJ_CONTROLLER_OBJECT)pfnObjcoCreate
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                    );
    }
    else
    {
        pEntryObj =
            (PDSLH_OBJ_CONTROLLER_OBJECT)DslhCreateObjController
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pEntryObj )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pEntryObj->hDslhCpeController  = pMyObject->hDslhCpeController;
    pEntryObj->hInsContext         = (ANSC_HANDLE)NULL;
    pEntryObj->hParentInsContext   = pMyObject->GetInsContext(pMyObject);

    if( pChildObjEntity->ObjDescr != NULL)
    {
        pEntryObj->hDslhTr69If = pChildObjEntity->ObjDescr->hDslhTr69If;
    }

    return (ANSC_HANDLE)pEntryObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoTableAddEntryObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntryObj,
                ULONG*                      pulInsNumber
            );

    description:

        This function is called by the ObjRecord object to add a newly
        created entry object to the table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntryObj
                Specifies the entry object to be added.

                ULONG*                      pulInsNumber
                Returns the instance number of the entry object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoTableAddEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj,
        ULONG*                      pulInsNumber
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject             = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pDslhTr69If           = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_CONTROLLER_OBJECT     pEntryObj             = (PDSLH_OBJ_CONTROLLER_OBJECT)hEntryObj;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord        = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    
    if( hEntryObj == NULL)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if( pDslhTr69If == NULL)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if( pDslhTr69If->bUseFullName)
    {
        if( pDslhTr69If->Methods.MethodWithFullName.AddEntry != NULL)
        {
            pEntryObj->hInsContext =
                    pDslhTr69If->Methods.MethodWithFullName.AddEntry(pMyObject->hInsContext, pDslhObjRecord->FullName, pulInsNumber);
        }
    }
    else
    {
        if( pDslhTr69If->Methods.MethodWithLastName.AddEntry != NULL)
        {
            pEntryObj->hInsContext =
                pDslhTr69If->Methods.MethodWithLastName.AddEntry(pMyObject->hInsContext, pulInsNumber);
        }
    }

    return  (pEntryObj->hInsContext != NULL)? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoTableDelEntryObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEntryObj
            );

    description:

        This function is called by the ObjRecord object to delete an
        existing entry object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEntryObj
                Specifies the entry object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoTableDelEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj
    )
{
    PDSLH_OBJCO_TABLE_OBJECT        pMyObject             = (PDSLH_OBJCO_TABLE_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pDslhTr69If           = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_CONTROLLER_OBJECT     pEntryObj             = (PDSLH_OBJ_CONTROLLER_OBJECT)hEntryObj;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord        = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    
    if( hEntryObj == NULL)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if( pDslhTr69If == NULL)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if( pDslhTr69If->bUseFullName)
    {
        if( pDslhTr69If->Methods.MethodWithFullName.DelEntry != NULL)
        {
            returnStatus =
                pDslhTr69If->Methods.MethodWithFullName.DelEntry(pMyObject->hInsContext, pDslhObjRecord->FullName, pEntryObj->hInsContext);
        }
    }
    else
    {
        if( pDslhTr69If->Methods.MethodWithLastName.DelEntry != NULL)
        {
            returnStatus =
                pDslhTr69If->Methods.MethodWithLastName.DelEntry(pMyObject->hInsContext, pEntryObj->hInsContext);
        }
    }

    return  returnStatus;
}

