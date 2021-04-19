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

    module: dslh_cpeco_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Cpe Controller Object.

        *   DslhCpecoGetDslhWmpDatabase
        *   DslhCpecoGetProperty
        *   DslhCpecoSetProperty
        *   DslhCpecoResetProperty
        *   DslhCpecoReset
        *   DslhCpecoAddInterface
        *   DslhCpecoGetInterfaceByName
        *   DslhCpecoSetParameterArray

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.

**********************************************************************/


#include "dslh_cpeco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhCpecoGetDslhWmpDatabase
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
DslhCpecoGetDslhWmpDatabase
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hDslhWmpDatabase;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoGetProperty
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
DslhCpecoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_CPE_CONTROLLER_PROPERTY   pProperty    = (PDSLH_CPE_CONTROLLER_PROPERTY)&pMyObject->Property;

    *(PDSLH_CPE_CONTROLLER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoSetProperty
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
DslhCpecoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_CPE_CONTROLLER_PROPERTY   pProperty    = (PDSLH_CPE_CONTROLLER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PDSLH_CPE_CONTROLLER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoResetProperty
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
DslhCpecoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_CPE_CONTROLLER_PROPERTY   pProperty    = (PDSLH_CPE_CONTROLLER_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(DSLH_CPE_CONTROLLER_PROPERTY));

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoReset
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
DslhCpecoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;

#if 0 /* the objdesp is already released in another place */
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr = (PDSLH_CWMP_OBJECT_DESCR      )NULL;
    ULONG                           i            = 0;
    for( i = 0; i < pMyObject->uObjCount; i ++)
    {
        pObjectDescr = (PDSLH_CWMP_OBJECT_DESCR)pMyObject->hObjDespArray[i];

        if( pObjectDescr != NULL)
        {
            DslhCwmpFreeObjectDescr(pObjectDescr);
        }

        pMyObject->hObjDespArray[i] = NULL;
    }
#endif

    pMyObject->uObjCount = 0;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoAddInterface
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to add interface to CPE Controller.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                The new added interface

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhCpecoAddInterface
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PANSC_INTERFACE_OBJECT          pInterface   = (PANSC_INTERFACE_OBJECT)hInterface;

    if( hInterface == NULL || pMyObject->uIfCount >= CPECONTROLLER_MAX_INTERFACE_COUNT)
    {
        AnscTrace("Unable to add interface in DslhCpeController.\n");

        return ANSC_STATUS_FAILURE;
    }

    pMyObject->hIfArray[ pMyObject->uIfCount ++] = hInterface;

    AnscTrace("Interface '%s' is added into CpeController.\n", pInterface->Name);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhCpecoGetInterfaceByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       ifName
            );

    description:

        This function is called to retrieve the interface handle specified
        by the interface name;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       ifName
                The specified interface name;

    return:     status of operation.

**********************************************************************/
ANSC_HANDLE
DslhCpecoGetInterfaceByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ifName
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PANSC_INTERFACE_OBJECT          pInterface   = (PANSC_INTERFACE_OBJECT)NULL;
    ULONG                           i            = 0;

    if( ifName == NULL || AnscSizeOfString(ifName) == 0)
    {
        AnscTrace("Unable to retrieve interface in DslhCpeController.\n");

        return NULL;
    }

    for( i = 0; i < pMyObject->uIfCount; i ++)
    {
        pInterface = (PANSC_INTERFACE_OBJECT)pMyObject->hIfArray[i];

        if( pInterface != NULL && strcmp(pInterface->Name, ifName) == 0 )
        {
            return (ANSC_HANDLE)pInterface;
        }
    }

    AnscTrace("Unable to retrieve interface '%s' in DslhCpeController.\n", ifName);


    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoSetParameterArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParameterArray,
                ULONG                            uCount
            );

    description:

        This function is called to set parameterarrya to Cpe object;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParameterArray,
                The specified parameter arry.

                ANSC_HANDLE                 hParameteHolder,
                This specified parameter holder.

                ULONG                            uCount
                This specifies paramter count in array.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhCpecoSetParameterArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParameterArray,
        ANSC_HANDLE                 hParameterHolder,
        ULONG                       uCount
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject    = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->hParameterArray  = hParameterArray;
    pMyObject->hParameterHolder = hParameterHolder;
    pMyObject->uParameterCount  = uCount;
    pMyObject->uParameterIndex  = 0;
    pMyObject->uParameterArrayIndex  = 0;

    return ANSC_STATUS_SUCCESS;
}

