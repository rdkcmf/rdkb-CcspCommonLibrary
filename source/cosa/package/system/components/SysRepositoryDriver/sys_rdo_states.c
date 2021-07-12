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

    module:	sys_rdo_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Repository Driver Object.

        *   SysRdoGetRootFolder
        *   SysRdoGetProperty
        *   SysRdoSetProperty
        *   SysRdoResetProperty
        *   SysRdoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/05/02    initial revision.

**********************************************************************/


#include "sys_rdo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoGetRootFolder
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
SysRdoGetRootFolder
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;

    return  pMyObject->hRootFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoGetProperty
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
SysRdoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_PROPERTY pProperty    = (PSYS_REPOSITORY_DRIVER_PROPERTY)&pMyObject->Property;

    *(PSYS_REPOSITORY_DRIVER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoSetProperty
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
SysRdoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_PROPERTY pProperty    = (PSYS_REPOSITORY_DRIVER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_REPOSITORY_DRIVER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoResetProperty
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
SysRdoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_PROPERTY pProperty    = (PSYS_REPOSITORY_DRIVER_PROPERTY)&pMyObject->Property;
    errno_t   rc = -1;

    rc = STRCPY_S_NOCLOBBER(pProperty->DriverName, sizeof(pProperty->DriverName), BISGA_PACKAGE_NAME);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoReset
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
SysRdoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;

    pRootFolder->Reset((ANSC_HANDLE)pRootFolder);

    return  ANSC_STATUS_SUCCESS;
}
