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

    module:	slap_scocol_collection.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object element-access
        functions of the Slap Sco Collection Object.

        *   SlapScoColGetCount
        *   SlapScoColGetItem
        *   SlapScoColSetItem
        *   SlapScoColGetItemByIndex
        *   SlapScoColSetItemByIndex

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/13/03    initial revision.

**********************************************************************/


#include "slap_scocol_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapScoColGetCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of collection
        items.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     item number.

**********************************************************************/

SLAP_UINT32
SlapScoColGetCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoColGetItem
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve a pre-registered object
        collection value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoColGetItem
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_COLLECTION_OBJECT     pMyObject        = (PSLAP_SCO_COLLECTION_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE        )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE             )NULL;
    char*                           property_name    = "CollectionItem";

    if ( !param_list )
    {
        return  NULL;
    }
    else
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->GetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoColSetItem
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure a pre-registered object
        collection value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoColSetItem
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_COLLECTION_OBJECT     pMyObject        = (PSLAP_SCO_COLLECTION_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE        )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE             )NULL;
    char*                           property_name    = "CollectionItem";

    if ( !param_list )
    {
        return  NULL;
    }
    else
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->SetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoColGetItemByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve a pre-registered object
        collection value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoColGetItemByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_COLLECTION_OBJECT     pMyObject        = (PSLAP_SCO_COLLECTION_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE        )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE             )NULL;
    char*                           property_name    = "ItemByIndex";

    if ( !param_list )
    {
        return  NULL;
    }
    else
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->GetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoColSetItemByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure a pre-registered object
        collection value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoColSetItemByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_COLLECTION_OBJECT     pMyObject        = (PSLAP_SCO_COLLECTION_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf       = (PSLAP_OLA_INTERFACE        )pMyObject->hSlapOlaIf;
    PSLAP_OBJECT_PROPERTY           pObjectProperty  = (PSLAP_OBJECT_PROPERTY      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar     = (PSLAP_VARIABLE             )NULL;
    char*                           property_name    = "ItemByIndex";

    if ( !param_list )
    {
        return  NULL;
    }
    else
    {
        pObjectProperty =
            (PSLAP_OBJECT_PROPERTY)pSlapOlaIf->GetObjectProperty
                (
                    pSlapOlaIf->hOwnerContext,
                    property_name
                );
    }

    if ( !pObjectProperty )
    {
        return  NULL;
    }

    pMyObject->InvokeDispatch
            (
                (ANSC_HANDLE)pMyObject,
                pObjectProperty->SetMethodName,
                param_list,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    return  pReturnedVar;
}
