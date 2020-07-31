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

    module:	bwrm_pmo_states.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bwrm Page Manager Object.

        *   BwrmPmoGetCacheEnabled
        *   BwrmPmoSetCacheEnabled
        *   BwrmPmoGetCacheEntryCount
        *   BwrmPmoSetCacheEntryCount
        *   BwrmPmoGetCacheMemorySize
        *   BwrmPmoSetCacheMemorySize
        *   BwrmPmoGetCacheTimeout
        *   BwrmPmoSetCacheTimeout
        *   BwrmPmoGetProperty
        *   BwrmPmoSetProperty
        *   BwrmPmoResetProperty
        *   BwrmPmoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#include "bwrm_pmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BwrmPmoGetCacheEnabled
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
BwrmPmoGetCacheEnabled
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->bCacheEnabled;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoSetCacheEnabled
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoSetCacheEnabled
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->bCacheEnabled = bEnabled;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmPmoGetCacheEntryCount
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

ULONG
BwrmPmoGetCacheEntryCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->CacheEntryCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoSetCacheEntryCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       count
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       count
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoSetCacheEntryCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->CacheEntryCount = count;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmPmoGetCacheMemorySize
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

ULONG
BwrmPmoGetCacheMemorySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->CacheMemorySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoSetCacheMemorySize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       memory_size
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       memory_size
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoSetCacheMemorySize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       memory_size
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->CacheMemorySize = memory_size;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmPmoGetCacheTimeout
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

ULONG
BwrmPmoGetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->CacheTimeout;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoSetCacheTimeout
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       timeout
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       timeout
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoSetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->CacheTimeout = timeout;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoGetProperty
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
BwrmPmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    *(PBWRM_PAGE_MANAGER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoSetProperty
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
BwrmPmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PBWRM_PAGE_MANAGER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoResetProperty
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
BwrmPmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty    = (PBWRM_PAGE_MANAGER_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(BWRM_PAGE_MANAGER_PROPERTY));

    pProperty->bCacheEnabled   = TRUE;
    pProperty->CacheEntryCount = BWRM_DEF_CACHE_ENTRY_COUNT;
    pProperty->CacheMemorySize = BWRM_DEF_CACHE_MEMORY_SIZE;
    pProperty->CacheTimeout    = BWRM_DEF_CACHE_TIMEOUT;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoReset
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
BwrmPmoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
