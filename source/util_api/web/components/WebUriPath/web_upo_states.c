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

    module:	web_upo_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web Uri Path Object.

        *   WebUpoIsRemoveable
        *   WebUpoGetPathName
        *   WebUpoSetPathName
        *   WebUpoGetResourceOwner
        *   WebUpoSetResourceOwner
        *   WebUpoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.

**********************************************************************/


#include "web_upo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebUpoIsRemoveable
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
WebUpoIsRemoveable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject      = (PWEB_URI_PATH_OBJECT      )hThisObject;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner = (PWEB_RESOURCE_OWNER_OBJECT)pMyObject->hResourceOwner;
    ULONG                           ulSonUpoCount  = 0;
    ULONG                           i              = 0;

    if ( pResourceOwner )
    {
        return  FALSE;
    }

    for ( i = 0; i < WEB_UPO_UPO_TABLE_SIZE; i++ )
    {
        ulSonUpoCount += AnscQueueQueryDepth(&pMyObject->UpoTable[i]);
    }

    if ( ulSonUpoCount > 0 )
    {
        return  FALSE;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebUpoGetPathName
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
WebUpoGetPathName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;

    return  pMyObject->PathName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebUpoSetPathName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebUpoSetPathName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;
    errno_t                         rc           = -1;

    rc = strcpy_s(pMyObject->PathName, sizeof(pMyObject->PathName), path);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebUpoGetResourceOwner
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
WebUpoGetResourceOwner
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;

    return  pMyObject->hResourceOwner;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebUpoSetResourceOwner
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hResourceOwner
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hResourceOwner
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebUpoSetResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResourceOwner
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;

    pMyObject->hResourceOwner = hResourceOwner;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebUpoReset
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
WebUpoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject      = (PWEB_URI_PATH_OBJECT      )hThisObject;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner = (PWEB_RESOURCE_OWNER_OBJECT)pMyObject->hResourceOwner;

    pMyObject->DelAllUriPaths((ANSC_HANDLE)pMyObject);

    if ( pResourceOwner )
    {
        pResourceOwner->Reset((ANSC_HANDLE)pResourceOwner);

        pMyObject->hResourceOwner = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
