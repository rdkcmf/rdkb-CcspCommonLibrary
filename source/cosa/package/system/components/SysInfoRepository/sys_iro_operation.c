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

    module:	sys_iro_operation.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Info Repository Object.

        *   SysIroEngage
        *   SysIroCancel
        *   SysIroAcqAccess
        *   SysIroRelAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/04/02    initial revision.

**********************************************************************/


#include "sys_iro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pRepDriver->Engage((ANSC_HANDLE)pRepDriver);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pRepDriver->Cancel((ANSC_HANDLE)pRepDriver);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroAcqAccess
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bReadAccess
            );

    description:

        This function is called to acquire the repository access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bReadAccess
                Specifies whether to read or write the repository.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroAcqAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
/*    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    
    if ( bReadAccess )
    {
        AnscAcquireSemaphore(&pMyObject->AccessSemaphore, 0xFFFFFFFF);
    }
    else
    {
        AnscAcquireLock(&pMyObject->AccessLock);

        for ( i = 0; i < SYS_IRO_MAX_REP_ACCESS_COUNT; i++ )
        {
            AnscAcquireSemaphore(&pMyObject->AccessSemaphore, 0xFFFFFFFF);
        }

        AnscReleaseLock(&pMyObject->AccessLock);
    }
    */

    if ( !bReadAccess )
    {
        AnscAcquireLock(&pMyObject->AccessLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroRelAccess
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bReadAccess
            );

    description:

        This function is called to release the repository access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bReadAccess
                Specifies whether to read or write the repository.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroRelAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    /*
    if ( bReadAccess )
    {
        AnscReleaseSemaphore(&pMyObject->AccessSemaphore, 1);
    }
    else
    {
        AnscReleaseSemaphore(&pMyObject->AccessSemaphore, SYS_IRO_MAX_REP_ACCESS_COUNT);
    }
    */

    if ( !bReadAccess )
    {
        AnscReleaseLock(&pMyObject->AccessLock);
    }

    return  ANSC_STATUS_SUCCESS;
}
