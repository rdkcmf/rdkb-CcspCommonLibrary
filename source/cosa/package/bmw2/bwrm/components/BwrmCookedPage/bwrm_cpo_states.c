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

    module:	bwrm_cpo_states.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bwrm Cooked Page Object.

        *   BwrmCpoGetRootPath
        *   BwrmCpoSetRootPath
        *   BwrmCpoGetPagePath
        *   BwrmCpoSetPagePath
        *   BwrmCpoGetOwnerReserved
        *   BwrmCpoSetOwnerReserved
        *   BwrmCpoGetPageContext
        *   BwrmCpoSetPageContext
        *   BwrmCpoGetTimestamp
        *   BwrmCpoSetTimestamp
        *   BwrmCpoIncRefCount
        *   BwrmCpoDecRefCount
        *   BwrmCpoGetRefCount
        *   BwrmCpoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/02/03    initial revision.

**********************************************************************/


#include "bwrm_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        BwrmCpoGetRootPath
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

char*
BwrmCpoGetRootPath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    return  pMyObject->RootPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoSetRootPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoSetRootPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    if ( pMyObject->RootPath )
    {
        AnscFreeMemory(pMyObject->RootPath);

        pMyObject->RootPath = NULL;
    }

    if ( root_path )
    {
        pMyObject->RootPath = AnscCloneString(root_path);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        BwrmCpoGetPagePath
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

char*
BwrmCpoGetPagePath
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    return  pMyObject->PagePath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoSetPagePath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoSetPagePath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    if ( pMyObject->PagePath )
    {
        AnscFreeMemory(pMyObject->PagePath);

        pMyObject->PagePath = NULL;
    }

    if ( page_path )
    {
        pMyObject->PagePath = AnscCloneString(page_path);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmCpoGetOwnerReserved
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
BwrmCpoGetOwnerReserved
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    return  pMyObject->hOwnerReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoSetOwnerReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReserved
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoSetOwnerReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    pMyObject->hOwnerReserved = hReserved;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmCpoGetPageContext
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
BwrmCpoGetPageContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    return  pMyObject->hPageContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoSetPageContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoSetPageContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    pMyObject->hPageContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmCpoGetTimestamp
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
BwrmCpoGetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    return  pMyObject->Timestamp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoSetTimestamp
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       timestamp
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       timestamp
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoSetTimestamp
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timestamp
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    pMyObject->Timestamp = timestamp;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoIncRefCount
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoIncRefCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    pMyObject->RefCount ++;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoDecRefCount
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoDecRefCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    if ( pMyObject->RefCount > 0 )
    {
        pMyObject->RefCount --;
    }

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmCpoGetRefCount
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmCpoGetRefCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;
    ULONG                           ulRefCount;

    AnscAcquireLock(&pMyObject->AccessLock);

    ulRefCount = pMyObject->RefCount;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ulRefCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoReset
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
BwrmCpoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    if ( pMyObject->RootPath )
    {
        AnscFreeMemory(pMyObject->RootPath);

        pMyObject->RootPath = NULL;
    }

    if ( pMyObject->PagePath )
    {
        AnscFreeMemory(pMyObject->PagePath);

        pMyObject->PagePath = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
