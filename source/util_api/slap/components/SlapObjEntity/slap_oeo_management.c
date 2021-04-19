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

    module:	slap_oeo_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Obj Entity Object.

        *   SlapOeoGetObjectProperty
        *   SlapOeoAddObjectProperty
        *   SlapOeoDelObjectProperty
        *   SlapOeoDelAllObjectProperties
        *   SlapOeoGetMethodDescriptor1
        *   SlapOeoGetMethodDescriptor2
        *   SlapOeoAddMethodDescriptor
        *   SlapOeoDelMethodDescriptor
        *   SlapOeoDelAllMethodDescriptors
        *   SlapOeoGetObjRecord
        *   SlapOeoNewObjRecord
        *   SlapOeoDelObjRecord
        *   SlapOeoDelAllObjRecords
        *   SlapOeoAcqPooledObjRecord
        *   SlapOeoRelPooledObjRecord
        *   SlapOeoDelAllPooledObjRecords

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

        ANSC_HANDLE
        SlapOeoGetObjectProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       property_name
            );

    description:

        This function is called to retrieve an object_property by
        matching the property name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       property_name
                Specifies the property name to be matched.

    return:     object property.

**********************************************************************/

ANSC_HANDLE
SlapOeoGetObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject       = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJECT_PROPERTY           pObjectProperty = (PSLAP_OBJECT_PROPERTY  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex     = 0;

    if ( (pMyObject->OpoTableSize == 0   ) ||
         (pMyObject->OpoTable     == NULL) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    property_name,
                    AnscSizeOfString(property_name),
                    pMyObject->OpoTableSize
                );
    }

    AnscAcquireLock(&pMyObject->OpoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OpoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjectProperty = ACCESS_SLAP_OBJECT_PROPERTY(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pObjectProperty->PropertyName,property_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->OpoTableLock);

            return  (ANSC_HANDLE)pObjectProperty;
        }
    }

    AnscReleaseLock(&pMyObject->OpoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoAddObjectProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve an object_property by
        matching the property name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the object property to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoAddObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject       = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJECT_PROPERTY           pObjectProperty = (PSLAP_OBJECT_PROPERTY  )hProperty;
    ULONG                           ulHashIndex     = 0;

    if ( (pMyObject->OpoTableSize == 0   ) ||
         (pMyObject->OpoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    pObjectProperty->PropertyName,
                    AnscSizeOfString(pObjectProperty->PropertyName),
                    pMyObject->OpoTableSize
                );
    }

    pObjectProperty->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pMyObject->OpoTableLock);
    AnscQueuePushEntry(&pMyObject->OpoTable[ulHashIndex], &pObjectProperty->Linkage);
    AnscReleaseLock   (&pMyObject->OpoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelObjectProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       property_name
            );

    description:

        This function is called to delete an object_property by
        matching the property name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       property_name
                Specifies the property name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject       = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJECT_PROPERTY           pObjectProperty = (PSLAP_OBJECT_PROPERTY  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex     = 0;

    if ( (pMyObject->OpoTableSize == 0   ) ||
         (pMyObject->OpoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    property_name,
                    AnscSizeOfString(property_name),
                    pMyObject->OpoTableSize
                );
    }

    AnscAcquireLock(&pMyObject->OpoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OpoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjectProperty = ACCESS_SLAP_OBJECT_PROPERTY(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp( pObjectProperty->PropertyName,property_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->OpoTable[ulHashIndex], &pObjectProperty->Linkage);

            SlapFreeObjectProperty(pObjectProperty);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->OpoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelAllObjectProperties
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all object_properties.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelAllObjectProperties
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject       = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJECT_PROPERTY           pObjectProperty = (PSLAP_OBJECT_PROPERTY  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i               = 0;

    if ( (pMyObject->OpoTableSize == 0   ) ||
         (pMyObject->OpoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    AnscAcquireLock(&pMyObject->OpoTableLock);

    for ( i = 0; i < pMyObject->OpoTableSize; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OpoTable[i]);

        while ( pSLinkEntry )
        {
            pObjectProperty = ACCESS_SLAP_OBJECT_PROPERTY(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->OpoTable[i]);

            SlapFreeObjectProperty(pObjectProperty);
        }
    }

    AnscReleaseLock(&pMyObject->OpoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoGetMethodDescriptor1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name
            );

    description:

        This function is called to retrieve a method_descriptor by
        matching the method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the method name to be matched.

    return:     method descriptor.

**********************************************************************/

ANSC_HANDLE
SlapOeoGetMethodDescriptor1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex       = 0;

    if ( (pMyObject->MdoTableSize == 0   ) ||
         (pMyObject->MdoTable     == NULL) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    method_name,
                    AnscSizeOfString(method_name),
                    pMyObject->MdoTableSize
                );
    }

    AnscAcquireLock(&pMyObject->MdoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MdoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pMethodDescriptor = ACCESS_SLAP_METHOD_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pMethodDescriptor->MethodName, method_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->MdoTableLock);

            return  (ANSC_HANDLE)pMethodDescriptor;
        }
    }

    AnscReleaseLock(&pMyObject->MdoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoGetMethodDescriptor2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        param_list_input
            );

    description:

        This function is called to retrieve a method_descriptor by
        matching the method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the method name to be matched.

                SLAP_PARAMETER_LIST*        param_list_input
                Specifies the input parameter list to be matched.

    return:     method descriptor.

**********************************************************************/

ANSC_HANDLE
SlapOeoGetMethodDescriptor2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex       = 0;

    if ( (pMyObject->MdoTableSize == 0   ) ||
         (pMyObject->MdoTable     == NULL) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    method_name,
                    AnscSizeOfString(method_name),
                    pMyObject->MdoTableSize
                );
    }

    AnscAcquireLock(&pMyObject->MdoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MdoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pMethodDescriptor = ACCESS_SLAP_METHOD_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pMethodDescriptor->MethodName,method_name) == 0 )
        {
            if ( pMethodDescriptor->CallOptions & SLAP_CALL_OPTION_passParamListI )
            {
                AnscReleaseLock(&pMyObject->MdoTableLock);

                return  (ANSC_HANDLE)pMethodDescriptor;
            }
            else if ( !pMethodDescriptor->ParamListI && (!param_list_input || (param_list_input->ParamCount == 0)) )
            {
                AnscReleaseLock(&pMyObject->MdoTableLock);

                return  (ANSC_HANDLE)pMethodDescriptor;
            }
            else if (pMethodDescriptor->ParamListI && pMethodDescriptor->ParamListI->ParamCount == param_list_input->ParamCount )
            {
                AnscReleaseLock(&pMyObject->MdoTableLock);

                return  (ANSC_HANDLE)pMethodDescriptor;
            }
        }
    }

    AnscReleaseLock(&pMyObject->MdoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoAddMethodDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDescriptor
            );

    description:

        This function is called to retrieve a method_descriptor by
        matching the method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDescriptor
                Specifies the method descriptor to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoAddMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)hDescriptor;
    ULONG                           ulHashIndex       = 0;

    if ( (pMyObject->MdoTableSize == 0   ) ||
         (pMyObject->MdoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    pMethodDescriptor->MethodName,
                    AnscSizeOfString(pMethodDescriptor->MethodName),
                    pMyObject->MdoTableSize
                );
    }

    pMethodDescriptor->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pMyObject->MdoTableLock);
    AnscQueuePushEntry(&pMyObject->MdoTable[ulHashIndex], &pMethodDescriptor->Linkage);
    AnscReleaseLock   (&pMyObject->MdoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelMethodDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name
            );

    description:

        This function is called to delete a method_descriptor by
        matching the method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the method name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           ulHashIndex       = 0;

    if ( (pMyObject->MdoTableSize == 0   ) ||
         (pMyObject->MdoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        ulHashIndex =
            AnscHashString
                (
                    method_name,
                    AnscSizeOfString(method_name),
                    pMyObject->MdoTableSize
                );
    }

    AnscAcquireLock(&pMyObject->MdoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->MdoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pMethodDescriptor = ACCESS_SLAP_METHOD_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry       = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pMethodDescriptor->MethodName,method_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->MdoTable[ulHashIndex], &pMethodDescriptor->Linkage);

            SlapFreeMethodDescriptor(pMethodDescriptor);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->MdoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelAllMethodDescriptors
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all method_descriptors.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelAllMethodDescriptors
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i                 = 0;

    if ( (pMyObject->MdoTableSize == 0   ) ||
         (pMyObject->MdoTable     == NULL) )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    AnscAcquireLock(&pMyObject->MdoTableLock);

    for ( i = 0; i < pMyObject->MdoTableSize; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->MdoTable[i]);

        while ( pSLinkEntry )
        {
            pMethodDescriptor = ACCESS_SLAP_METHOD_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry       = AnscQueuePopEntry(&pMyObject->MdoTable[i]);

            SlapFreeMethodDescriptor(pMethodDescriptor);
        }
    }

    AnscReleaseLock(&pMyObject->MdoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoGetObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjContainer,
                ULONG                       instance_id
            );

    description:

        This function is called to retrieve an obj record object. This
        function acquires the access_lock before returning.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjContainer
                Specifies the obj container associated with the record.

                ULONG                       instance_id
                Specifies the runtime instance id assigned by the
                obj container.

    return:     obj record object.

**********************************************************************/

ANSC_HANDLE
SlapOeoGetObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ULONG                       instance_id
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT   )hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)hObjContainer;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    ULONG                           ulHashIndex       = AnscHashUlong((ULONG)hObjContainer, SLAP_OEO_ORO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OroTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSlapObjRecord = ACCESS_SLAP_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pSlapObjRecord->Match1
                (
                    (ANSC_HANDLE)pSlapObjRecord,
                    (ANSC_HANDLE)pSlapObjContainer,
                    instance_id
                ) )
        {
            pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);

            AnscReleaseLock(&pMyObject->OroTableLock);

            return  (ANSC_HANDLE)pSlapObjRecord;
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoNewObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjContainer,
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to create an obj record object. This
        function acquires the access_lock before returning.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjContainer
                Specifies the obj container associated with the record.

                ANSC_HANDLE                 hInsContext
                Specifies the runtime instance context to be associated.

    return:     obj record object.

**********************************************************************/

ANSC_HANDLE
SlapOeoNewObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hInsContext
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT   )hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor    = (PSLAP_OBJECT_DESCRIPTOR   )pMyObject->hObjDescriptor;
    PFN_ANSCLCO_CREATE              pfnSlapScoCreate  = (PFN_ANSCLCO_CREATE        )NULL;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)hObjContainer;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = NULL;
    ULONG                           ulHashIndex       = AnscHashUlong((ULONG)hObjContainer, SLAP_OEO_ORO_TABLE_SIZE);

    if ( !pObjDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_poolable )
    {
        pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)pMyObject->AcqPooledObjRecord((ANSC_HANDLE)pMyObject);
    }
    else
    {
        pSlapObjRecord = NULL;
    }

    if ( pSlapObjRecord )
    {
        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;

        if ( TRUE )
        {
            pSlapObjRecord->hSlapObjContainer = (ANSC_HANDLE)pSlapObjContainer;
        }

        if ( TRUE )
        {
            pSlapSrvComponent->ObjInstanceId     = 0;
            pSlapSrvComponent->hInsContext       = hInsContext;
            pSlapSrvComponent->hSlapObjContainer = (ANSC_HANDLE)pSlapObjContainer;

            pSlapSrvComponent->Engage((ANSC_HANDLE)pSlapSrvComponent);

            pSlapObjContainer->AddSrvComponent((ANSC_HANDLE)pSlapObjContainer, (ANSC_HANDLE)pSlapSrvComponent);
        }
    }
    else
    {
        if ( TRUE )
        {
            pfnSlapScoCreate  = (PFN_ANSCLCO_CREATE)pObjDescriptor->ObjConstructor;
            pSlapSrvComponent =
                (PSLAP_SRV_COMPONENT_OBJECT)pfnSlapScoCreate
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
        }

        if ( !pSlapSrvComponent )
        {
            goto  EXIT1;
        }
        else
        {
            pSlapSrvComponent->ObjType           = pObjDescriptor->ObjType;
            pSlapSrvComponent->ObjInstanceId     = 0;
            pSlapSrvComponent->hInsContext       = hInsContext;
            pSlapSrvComponent->hSlapObjContainer = (ANSC_HANDLE)pSlapObjContainer;
            pSlapSrvComponent->hSlapObjRecord    = (ANSC_HANDLE)NULL;
            pSlapSrvComponent->hSlapObjEntity    = (ANSC_HANDLE)pMyObject;
            pSlapSrvComponent->hSlapUoaIf        = pMyObject->hSlapUoaIf;
            pSlapSrvComponent->hSlapBssIf        = pMyObject->hSlapBssIf;
            pSlapSrvComponent->hSlapOlaIf        = pMyObject->hSlapOlaIf;

            pSlapSrvComponent->SetObjName((ANSC_HANDLE)pSlapSrvComponent, pObjDescriptor->ObjName);
            pSlapSrvComponent->Engage    ((ANSC_HANDLE)pSlapSrvComponent);
        }

        if ( TRUE )
        {
            pSlapObjRecord =
                (PSLAP_OBJ_RECORD_OBJECT)SlapCreateObjRecord
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
        }

        if ( !pSlapObjRecord )
        {
            goto  EXIT2;
        }
        else
        {
            pSlapObjRecord->hSlapObjEntity    = (ANSC_HANDLE)pMyObject;
            pSlapObjRecord->hSlapObjContainer = (ANSC_HANDLE)pSlapObjContainer;
            pSlapObjRecord->hSlapSrvComponent = (ANSC_HANDLE)pSlapSrvComponent;
        }

        if ( TRUE )
        {
            pSlapSrvComponent->hSlapObjRecord = (ANSC_HANDLE)pSlapObjRecord;

            pSlapObjContainer->AddSrvComponent((ANSC_HANDLE)pSlapObjContainer, (ANSC_HANDLE)pSlapSrvComponent);
        }
    }

    AnscAcquireLock(&pMyObject->OroTableLock);

    if ( TRUE )
    {
        AnscQueuePushEntry(&pMyObject->OroTable[ulHashIndex], &pSlapObjRecord->Linkage);

        pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  (ANSC_HANDLE)pSlapObjRecord;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pSlapSrvComponent )
    {
        pSlapSrvComponent->Remove((ANSC_HANDLE)pSlapSrvComponent);
    }

EXIT1:

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjContainer,
                ANSC_HANDLE                 hSrvComponent,
                BOOL                        bForceful
            );

    description:

        This function is called to delete an obj record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjContainer
                Specifies the obj container associated with the record.

                ANSC_HANDLE                 hSrvComponent
                Specifies the srv component associated with the record.

                BOOL                        bForceful
                Specifies whether the record should be deleted if the
                reference count is not zero.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hSrvComponent,
        BOOL                        bForceful
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT   )hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor    = (PSLAP_OBJECT_DESCRIPTOR   )pMyObject->hObjDescriptor;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)hObjContainer;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)hSrvComponent;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    ULONG                           ulHashIndex       = AnscHashUlong((ULONG)hObjContainer, SLAP_OEO_ORO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OroTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSlapObjRecord = ACCESS_SLAP_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry    = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pSlapObjRecord->Match2
                (
                    (ANSC_HANDLE)pSlapObjRecord,
                    (ANSC_HANDLE)pSlapObjContainer,
                    (ANSC_HANDLE)pSlapSrvComponent
                ) )
        {
            if ( bForceful || (pSlapObjRecord->GetRef((ANSC_HANDLE)pSlapObjRecord) == 0) )
            {
                AnscQueuePopEntryByLink(&pMyObject->OroTable[ulHashIndex], &pSlapObjRecord->Linkage);

                /*
                 * If this action has to be carried out unconditionally (bForceful == TRUE), the
                 * initiator MUST be the owner container. And this only happens when the container
                 * is performing the clean up before removing itself.
                 */
                if ( !bForceful )
                {
                    pSlapObjContainer->DelSrvComponent
                        (
                            (ANSC_HANDLE)pSlapObjContainer,
                            (ANSC_HANDLE)pSlapSrvComponent
                        );
                }

                pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
                pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);

                AnscReleaseLock(&pMyObject->OroTableLock);

                if ( pObjDescriptor->ObjType & SLAP_OBJ_TYPE_poolable )
                {
                    {
                        PSLAP_SRV_COMPONENT_OBJECT  srv_component = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;

                        if ( srv_component )
                        {
                            srv_component->Cancel((ANSC_HANDLE)srv_component);
                            srv_component->Reset ((ANSC_HANDLE)srv_component);
                        }
                    }

                    pMyObject->RelPooledObjRecord((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSlapObjRecord);
                }
                else
                {
                    pSlapObjRecord->Remove((ANSC_HANDLE)pSlapObjRecord);
                }

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                AnscReleaseLock(&pMyObject->OroTableLock);

                return  ANSC_STATUS_SHARING_VIOLATION;
            }
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelAllObjRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all obj record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelAllObjRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT   )hThisObject;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT   )NULL;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)NULL;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->OroTableLock);

    for ( i = 0; i < SLAP_OEO_ORO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OroTable[i]);

        while ( pSLinkEntry )
        {
            pSlapObjRecord    = ACCESS_SLAP_OBJ_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscQueuePopEntry(&pMyObject->OroTable[i]);
            pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pSlapObjRecord->hSlapObjContainer;
            pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;

            if ( pSlapObjContainer )
            {
                pSlapObjContainer->DelSrvComponent
                    (
                        (ANSC_HANDLE)pSlapObjContainer,
                        (ANSC_HANDLE)pSlapSrvComponent
                    );
            }

            pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
            pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);
            pSlapObjRecord->Remove   ((ANSC_HANDLE)pSlapObjRecord);
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoAcqPooledObjRecord
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a pooled obj record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     obj record object.

**********************************************************************/

ANSC_HANDLE
SlapOeoAcqPooledObjRecord
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject      = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;

    AnscAcquireLock(&pMyObject->PooledOroSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->PooledOroSList);
    AnscReleaseLock(&pMyObject->PooledOroSListLock);

    if ( pSLinkEntry )
    {
        pSlapObjRecord = ACCESS_SLAP_OBJ_RECORD_OBJECT(pSLinkEntry);
    }
    else
    {
        pSlapObjRecord = NULL;
    }

    return  (ANSC_HANDLE)pSlapObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoRelPooledObjRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjRecord
            );

    description:

        This function is called to release a pooled obj record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjRecord
                Specifies the obj record object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoRelPooledObjRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject      = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_BSS_INTERFACE             pSlapBssIf     = (PSLAP_BSS_INTERFACE    )pMyObject->hSlapBssIf;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)hObjRecord;

    if ( AnscSListQueryDepth(&pMyObject->PooledOroSList) >= pSlapBssIf->GetDefPoolSize(pSlapBssIf->hOwnerContext) )
    {
        pSlapObjRecord->Remove((ANSC_HANDLE)pSlapObjRecord);

        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock   (&pMyObject->PooledOroSListLock);
    AnscSListPushEntry(&pMyObject->PooledOroSList, &pSlapObjRecord->Linkage);
    AnscReleaseLock   (&pMyObject->PooledOroSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoDelAllPooledObjRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all pooled obj record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoDelAllPooledObjRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject      = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;

    AnscAcquireLock(&pMyObject->PooledOroSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->PooledOroSList);

    while ( pSLinkEntry )
    {
        pSlapObjRecord = ACCESS_SLAP_OBJ_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry    = AnscSListPopEntry(&pMyObject->PooledOroSList);

        pSlapObjRecord->Remove((ANSC_HANDLE)pSlapObjRecord);
    }

    AnscReleaseLock(&pMyObject->PooledOroSListLock);

    return  ANSC_STATUS_SUCCESS;
}
