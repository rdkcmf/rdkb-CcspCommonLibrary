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

    module:	sys_iro_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Info Repository Object.

        *   SysIroGetIraIf
        *   SysIroGetRepDriver
        *   SysIroGetProperty
        *   SysIroSetProperty
        *   SysIroResetProperty
        *   SysIroReset

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

        ANSC_HANDLE
        SysIroGetIraIf
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
SysIroGetIraIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    return  pMyObject->hIraIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroGetRepDriver
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
SysIroGetRepDriver
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    return  pMyObject->hRepDriver;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroGetProperty
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
SysIroGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    *(PSYS_INFO_REPOSITORY_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroSetProperty
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
SysIroSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_INFO_REPOSITORY_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroResetProperty
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
SysIroResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    pProperty->MaxLevels = 128;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroReset
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
SysIroReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
