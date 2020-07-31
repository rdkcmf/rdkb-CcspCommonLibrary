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

    module:	slap_oeo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Obj Entity Object.

        *   SlapOeoSetObjDescriptor
        *   SlapOeoReset

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


#include "slap_oeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoSetObjDescriptor
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
SlapOeoSetObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescriptor
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject          = (PSLAP_OBJ_ENTITY_OBJECT  )hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor     = (PSLAP_OBJECT_DESCRIPTOR  )pMyObject->hObjDescriptor;
    PSLAP_STD_CALL_DESCRIPTOR       pStdCallDescriptor = (PSLAP_STD_CALL_DESCRIPTOR)NULL;
    ULONG                           ulCallCount        = 0;
    SLAP_STD_CALL_DESCRIPTOR        internalCallDesp;

    if ( pObjDescriptor )
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);
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

    pMyObject->OpoTableSize = pObjDescriptor->ObjOpoTableSize;
    pMyObject->MdoTableSize = pObjDescriptor->ObjMdoTableSize;
    pStdCallDescriptor      = pObjDescriptor->ObjCallMap;

    /*
     * We need first create the Object_Property and Method_Descriptor tables; and then register
     * every member function call contained in the ObjCallMap (if it's not NULL) into these two
     * tables. Finally we complete the call registering by adding some internal call redirections
     * based on the registered object type.
     */
    pMyObject->SetupOpoTable((ANSC_HANDLE)pMyObject);
    pMyObject->SetupMdoTable((ANSC_HANDLE)pMyObject);

    if ( pStdCallDescriptor )
    {
        while ( pStdCallDescriptor[ulCallCount].CallName )
        {
            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&pStdCallDescriptor[ulCallCount]
                    );

            ulCallCount++;
        }
    }

    if ( TRUE )
    {
        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetObjName";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetProperty;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SRV_COMPONENT_FIELD(GetObjName2);
            internalCallDesp.PropertyName = "ObjName";
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "string";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetObjType";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetProperty;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SRV_COMPONENT_FIELD(GetObjType);
            internalCallDesp.PropertyName = "ObjType";
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32/mapped";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }
    }

    if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_standard )
    {
        if ( TRUE )
        {
            internalCallDesp.CallName     = "CreateObject";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(CreateObject);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = "string";
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "object";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "CreateObject2";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(CreateObject2);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = "string handle";
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "object";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "DeleteObject";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(DeleteObject);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = "handle";
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "CloneObject";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(CloneObject);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "object";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "ToString";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(ToString);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "string";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetProperty";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(GetProperty);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "SetProperty";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(SetProperty);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "ImportMe";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(ImportMe);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "ExportMe";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListO;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_STANDARD_FIELD(ExportMe);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }
    }

    if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_collection )
    {
        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetCount";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetProperty;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_COLLECTION_FIELD(GetCount);
            internalCallDesp.PropertyName = "Count";
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetItem";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_COLLECTION_FIELD(GetItem);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "SetItem";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_COLLECTION_FIELD(SetItem);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetItemByIndex";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_COLLECTION_FIELD(GetItemByIndex);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "SetItemByIndex";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_COLLECTION_FIELD(SetItemByIndex);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }
    }

    if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_gallery )
    {
        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetVariable";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetItem;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_passReturnVar | SLAP_CALL_OPTION_skipGoaTranslation;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_GALLERY_FIELD(GetVariable);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = NULL;

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "SetVariable";
            internalCallDesp.CallId       = SLAP_METHOD_ID_SetItem;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_skipGoaTranslation;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_GALLERY_FIELD(SetVariable);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "AddVariable";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_skipGoaTranslation;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_GALLERY_FIELD(AddVariable);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "DelVariable";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = SLAP_CALL_OPTION_passParamListI | SLAP_CALL_OPTION_skipGoaTranslation;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_GALLERY_FIELD(DelVariable);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "DelAllVariables";
            internalCallDesp.CallId       = 0;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_GALLERY_FIELD(DelAllVariables);
            internalCallDesp.PropertyName = NULL;
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }
    }

    if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_buffer )
    {
        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetSize";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetProperty;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_BUFFER_FIELD(GetSize);
            internalCallDesp.PropertyName = "Size";
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "uint32";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }

        if ( TRUE )
        {
            internalCallDesp.CallName     = "GetData";
            internalCallDesp.CallId       = SLAP_METHOD_ID_GetProperty;
            internalCallDesp.CallOptions  = 0;
            internalCallDesp.CallEntry    = ACCESS_SLAP_SCO_BUFFER_FIELD(GetData);
            internalCallDesp.PropertyName = "Data";
            internalCallDesp.CallDescrI   = NULL;
            internalCallDesp.CallDescrO   = NULL;
            internalCallDesp.CallDescrR   = "ucharArray";

            pMyObject->RegisterCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)&internalCallDesp
                    );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoReset
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
SlapOeoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject      = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)pMyObject->hObjDescriptor;

    if ( pObjDescriptor )
    {
        SlapFreeObjDescriptor(pObjDescriptor);

        pMyObject->hObjDescriptor = (ANSC_HANDLE)NULL;
    }

    if ( (pMyObject->OpoTableSize >  0   ) &&
         (pMyObject->OpoTable     != NULL) )
    {
        pMyObject->DelAllObjectProperties((ANSC_HANDLE)pMyObject);

        if ( TRUE )
        {
            AnscFreeMemory(pMyObject->OpoTable);

            pMyObject->OpoTable = NULL;
        }
    }

    if ( (pMyObject->MdoTableSize >  0   ) &&
         (pMyObject->MdoTable     != NULL) )
    {
        pMyObject->DelAllMethodDescriptors((ANSC_HANDLE)pMyObject);

        if ( TRUE )
        {
            AnscFreeMemory(pMyObject->MdoTable);

            pMyObject->MdoTable = NULL;
        }
    }

    pMyObject->DelAllObjRecords      ((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllPooledObjRecords((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
