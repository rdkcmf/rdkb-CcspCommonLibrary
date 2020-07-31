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

    module:	slap_oeo_tables.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced table-access functions
        of the Slap Obj Entity Object.

        *   SlapOeoSetupOpoTable
        *   SlapOeoSetupMdoTable
        *   SlapOeoRegisterCall

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/08/03    initial revision.
        07/11/03    relocate this module from SlapScoStandard

**********************************************************************/


#include "slap_oeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoSetupOpoTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the object_property table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoSetupOpoTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    ULONG                           i            = 0;

    if ( pMyObject->OpoTableSize == 0 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pMyObject->OpoTable = (QUEUE_HEADER*)AnscAllocateMemory(sizeof(QUEUE_HEADER) * pMyObject->OpoTableSize);

        if ( !pMyObject->OpoTable )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    for ( i = 0; i < pMyObject->OpoTableSize; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->OpoTable[i]);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoSetupMdoTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the method_descriptor table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoSetupMdoTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    ULONG                           i            = 0;

    if ( pMyObject->MdoTableSize == 0 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pMyObject->MdoTable = (QUEUE_HEADER*)AnscAllocateMemory(sizeof(QUEUE_HEADER) * pMyObject->MdoTableSize);

        if ( !pMyObject->MdoTable )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    for ( i = 0; i < pMyObject->MdoTableSize; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->MdoTable[i]);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoRegisterCall
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCallDescriptor
            );

    description:

        This function is called to register a SLAP service call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCallDescriptor
                Specifies the standard call descriptor to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOeoRegisterCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCallDescriptor
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject          = (PSLAP_OBJ_ENTITY_OBJECT  )hThisObject;
    PSLAP_STD_CALL_DESCRIPTOR       pStdCallDescriptor = (PSLAP_STD_CALL_DESCRIPTOR)hCallDescriptor;
    PSLAP_OBJECT_PROPERTY           pObjectProperty    = (PSLAP_OBJECT_PROPERTY    )NULL;
    PSLAP_METHOD_DESCRIPTOR         pMethodDescriptor  = (PSLAP_METHOD_DESCRIPTOR  )NULL;

    if ( !pStdCallDescriptor->CallName )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pMethodDescriptor = (PSLAP_METHOD_DESCRIPTOR)AnscAllocateMemory(sizeof(SLAP_METHOD_DESCRIPTOR));

        if ( !pMethodDescriptor )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            SlapInitMethodDescriptor(pMethodDescriptor);
        }
    }

    if ( TRUE )
    {
        pMethodDescriptor->MethodName   = AnscCloneString(pStdCallDescriptor->CallName);
        pMethodDescriptor->MethodId     = pStdCallDescriptor->CallId;
        pMethodDescriptor->BridgeId     = pMyObject->ResolveCallBridge((ANSC_HANDLE)pMyObject, pStdCallDescriptor->CallName);
        pMethodDescriptor->HashIndex    = 0;
        pMethodDescriptor->CallOptions  = pStdCallDescriptor->CallOptions;
        pMethodDescriptor->MethodOffset = pStdCallDescriptor->CallEntry;

        if ( !pMethodDescriptor->MethodName )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }

        if ( pStdCallDescriptor->CallDescrI )
        {
            pMethodDescriptor->ParamListI = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

            if ( !pMethodDescriptor->ParamListI )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
            else
            {
                SlapInitParamList(pMethodDescriptor->ParamListI);
            }

            returnStatus =
                pMyObject->ResolveCallDescrI
                    (
                        (ANSC_HANDLE)pMyObject,
                        pStdCallDescriptor->CallDescrI,
                        pMethodDescriptor->ParamListI
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT2;
            }
        }
        else
        {
            pMethodDescriptor->ParamListI = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

            if ( !pMethodDescriptor->ParamListI )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
            else
            {
                SlapInitParamList(pMethodDescriptor->ParamListI);
            }
        }

        if ( pStdCallDescriptor->CallDescrO )
        {
            pMethodDescriptor->ParamListO = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

            if ( !pMethodDescriptor->ParamListO )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
            else
            {
                SlapInitParamList(pMethodDescriptor->ParamListO);
            }

            returnStatus =
                pMyObject->ResolveCallDescrO
                    (
                        (ANSC_HANDLE)pMyObject,
                        pStdCallDescriptor->CallDescrO,
                        pMethodDescriptor->ParamListO
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT2;
            }
        }
        else
        {
            pMethodDescriptor->ParamListO = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

            if ( !pMethodDescriptor->ParamListO )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
            else
            {
                SlapInitParamList(pMethodDescriptor->ParamListO);
            }
        }

        if ( pStdCallDescriptor->CallDescrR )
        {
            SlapAllocVariable(pMethodDescriptor->ReturnVar);

            if ( !pMethodDescriptor->ReturnVar )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }

            returnStatus =
                pMyObject->ResolveCallDescrR
                    (
                        (ANSC_HANDLE)pMyObject,
                        pStdCallDescriptor->CallDescrR,
                        pMethodDescriptor->ReturnVar
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT2;
            }
        }
        else
        {
            SlapAllocVariable(pMethodDescriptor->ReturnVar);

            if ( !pMethodDescriptor->ReturnVar )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
        }

        returnStatus =
            pMyObject->AddMethodDescriptor
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMethodDescriptor
                );
    }

    if ( TRUE )
    {
        switch ( pStdCallDescriptor->CallId )
        {
            case    SLAP_METHOD_ID_GetProperty :
            case    SLAP_METHOD_ID_SetProperty :

                    if ( !pStdCallDescriptor->PropertyName )
                    {
                        break;
                    }

                    pObjectProperty =
                        (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty 
                            (
                                (ANSC_HANDLE)pMyObject,
                                pStdCallDescriptor->PropertyName
                            );

                    if ( !pObjectProperty )
                    {
                        pObjectProperty = (PSLAP_OBJECT_PROPERTY)AnscAllocateMemory(sizeof(SLAP_OBJECT_PROPERTY));

                        if ( !pObjectProperty )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;

                            goto  EXIT2;
                        }
                        else
                        {
                            pObjectProperty->PropertyName = AnscCloneString(pStdCallDescriptor->PropertyName);
                            pObjectProperty->HashIndex    = 0;

                            if ( !pObjectProperty->PropertyName )
                            {
                                returnStatus = ANSC_STATUS_RESOURCES;

                                goto  EXIT3;
                            }
                        }

                        pMyObject->AddObjectProperty((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pObjectProperty);
                    }

                    if ( pStdCallDescriptor->CallId == SLAP_METHOD_ID_GetProperty )
                    {
                        pObjectProperty->GetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->GetMethodOffset = pMethodDescriptor->MethodOffset;
                    }
                    else
                    {
                        pObjectProperty->SetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->SetMethodOffset = pMethodDescriptor->MethodOffset;
                    }

                    break;

            case    SLAP_METHOD_ID_GetItem :
            case    SLAP_METHOD_ID_SetItem :

                    pObjectProperty =
                        (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty 
                            (
                                (ANSC_HANDLE)pMyObject,
                                "CollectionItem"
                            );

                    if ( !pObjectProperty )
                    {
                        pObjectProperty = (PSLAP_OBJECT_PROPERTY)AnscAllocateMemory(sizeof(SLAP_OBJECT_PROPERTY));

                        if ( !pObjectProperty )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;

                            goto  EXIT2;
                        }
                        else
                        {
                            pObjectProperty->PropertyName = AnscCloneString("CollectionItem");
                            pObjectProperty->HashIndex    = 0;

                            if ( !pObjectProperty->PropertyName )
                            {
                                returnStatus = ANSC_STATUS_RESOURCES;

                                goto  EXIT3;
                            }
                        }

                        pMyObject->AddObjectProperty((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pObjectProperty);
                    }

                    if ( pStdCallDescriptor->CallId == SLAP_METHOD_ID_GetItem )
                    {
                        pObjectProperty->GetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->GetMethodOffset = pMethodDescriptor->MethodOffset;
                    }
                    else
                    {
                        pObjectProperty->SetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->SetMethodOffset = pMethodDescriptor->MethodOffset;
                    }

                    break;

            case    SLAP_METHOD_ID_GetItemByIndex :
            case    SLAP_METHOD_ID_SetItemByIndex :

                    pObjectProperty =
                        (PSLAP_OBJECT_PROPERTY)pMyObject->GetObjectProperty 
                            (
                                (ANSC_HANDLE)pMyObject,
                                "ItemByIndex"
                            );

                    if ( !pObjectProperty )
                    {
                        pObjectProperty = (PSLAP_OBJECT_PROPERTY)AnscAllocateMemory(sizeof(SLAP_OBJECT_PROPERTY));

                        if ( !pObjectProperty )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;

                            goto  EXIT2;
                        }
                        else
                        {
                            pObjectProperty->PropertyName = AnscCloneString("ItemByIndex");
                            pObjectProperty->HashIndex    = 0;

                            if ( !pObjectProperty->PropertyName )
                            {
                                returnStatus = ANSC_STATUS_RESOURCES;

                                goto  EXIT3;
                            }
                        }

                        pMyObject->AddObjectProperty((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pObjectProperty);
                    }

                    if ( pStdCallDescriptor->CallId == SLAP_METHOD_ID_GetItemByIndex )
                    {
                        pObjectProperty->GetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->GetMethodOffset = pMethodDescriptor->MethodOffset;
                    }
                    else
                    {
                        pObjectProperty->SetMethodName   = pMethodDescriptor->MethodName;
                        pObjectProperty->SetMethodOffset = pMethodDescriptor->MethodOffset;
                    }

                    break;

            default :

                    break;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( pObjectProperty )
    {
        SlapFreeObjectProperty(pObjectProperty);
    }

EXIT2:

    if ( pMethodDescriptor )
    {
        SlapFreeMethodDescriptor(pMethodDescriptor);
    }

EXIT1:

    return  returnStatus;
}
