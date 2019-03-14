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

        *   SysIroV2Engage
        *   SysIroV2Cancel
        *   SysIroV2AcqAccess
        *   SysIroV2RelAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/30/06    initial revision.

**********************************************************************/


#include "sys_irov2_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2Engage
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
SysIroV2Engage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2Cancel
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
SysIroV2Cancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2AcqAccess
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
SysIroV2AcqAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

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
        SysIroV2RelAccess
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
SysIroV2RelAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    if ( !bReadAccess )
    {
        AnscReleaseLock(&pMyObject->AccessLock);
    }

    return  ANSC_STATUS_SUCCESS;
}
