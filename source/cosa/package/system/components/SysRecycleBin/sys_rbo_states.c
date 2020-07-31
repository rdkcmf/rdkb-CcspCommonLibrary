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

    module:	sys_rbo_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Recycle Bin Object.

        *   SysRboGetServedFolder
        *   SysRboSetServedFolder
        *   SysRboGetUserContext
        *   SysRboSetUserContext
        *   SysRboGetProperty
        *   SysRboSetProperty
        *   SysRboResetProperty
        *   SysRboReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/02    initial revision.

**********************************************************************/


#include "sys_rbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRboGetServedFolder
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
SysRboGetServedFolder
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;

    return  pMyObject->hServedFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboSetServedFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRboSetServedFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;

    pMyObject->hServedFolder = hFolder;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRboGetUserContext
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
SysRboGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;

    return  pMyObject->hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboSetUserContext
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
SysRboSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;

    pMyObject->hUserContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboGetProperty
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
SysRboGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    PSYS_RECYCLE_BIN_PROPERTY       pProperty    = (PSYS_RECYCLE_BIN_PROPERTY)&pMyObject->Property;

    *(PSYS_RECYCLE_BIN_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboSetProperty
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
SysRboSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    PSYS_RECYCLE_BIN_PROPERTY       pProperty    = (PSYS_RECYCLE_BIN_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_RECYCLE_BIN_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboResetProperty
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
SysRboResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboReset
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
SysRboReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;

    pMyObject->DelAllFolders((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllRecords((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
