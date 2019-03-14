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

        *   SysIroV2GetIraIf
        *   SysIroV2GetProperty
        *   SysIroV2SetProperty
        *   SysIroV2ResetProperty
        *   SysIroV2Reset

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

        ANSC_HANDLE
        SysIroV2GetIraIf
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
SysIroV2GetIraIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    return  pMyObject->hIraIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2GetProperty
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
SysIroV2GetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    *(PSYS_INFO_REPOSITORY_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2SetProperty
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
SysIroV2SetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_INFO_REPOSITORY_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2ResetProperty
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
SysIroV2ResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    pProperty->MaxLevels = 128;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2Reset
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
SysIroV2Reset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}
