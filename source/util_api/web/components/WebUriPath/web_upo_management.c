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

    module:	web_upo_management.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Web Uri Path Object.

        *   WebUpoGetUriPath
        *   WebUpoAddUriPath
        *   WebUpoDelUriPath
        *   WebUpoDelAllUriPaths

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebUpoGetUriPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to retrieve a uri path object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the uri path.

    return:     uri path object.

**********************************************************************/

ANSC_HANDLE
WebUpoGetUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pSonUriPath  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = 0;
    char                            lc_string[WEB_UPO_PATH_NAME_SIZE];

    AnscZeroMemory    (lc_string, WEB_UPO_PATH_NAME_SIZE           );
    AnscStringFromUToL(path,      lc_string, AnscSizeOfString(path));

    ulHashIndex =
        AnscHashString
            (
                lc_string,
                AnscSizeOfString(lc_string),
                WEB_UPO_UPO_TABLE_SIZE
            );

    AnscAcquireLock(&pMyObject->UpoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->UpoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSonUriPath = ACCESS_WEB_URI_PATH_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(lc_string,pSonUriPath->GetPathName((ANSC_HANDLE)pSonUriPath) ) == 0 )
        {
            AnscReleaseLock(&pMyObject->UpoTableLock);

            return  (ANSC_HANDLE)pSonUriPath;
        }
    }

    AnscReleaseLock(&pMyObject->UpoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebUpoAddUriPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to add a uri path object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the uri path.

    return:     uri path object.

**********************************************************************/

ANSC_HANDLE
WebUpoAddUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pSonUriPath  = NULL;
    ULONG                           ulHashIndex  = 0;
    char                            lc_string[WEB_UPO_PATH_NAME_SIZE];

    pSonUriPath =
        (PWEB_URI_PATH_OBJECT)pMyObject->GetUriPath
            (
                (ANSC_HANDLE)pMyObject,
                path
            );

    if ( pSonUriPath )
    {
        return  (ANSC_HANDLE)pSonUriPath;
    }
    else
    {
        AnscZeroMemory    (lc_string, WEB_UPO_PATH_NAME_SIZE           );
        AnscStringFromUToL(path,      lc_string, AnscSizeOfString(path));

        ulHashIndex =
            AnscHashString
                (
                    lc_string,
                    AnscSizeOfString(lc_string),
                    WEB_UPO_UPO_TABLE_SIZE
                );
    }

    pSonUriPath =
        (PWEB_URI_PATH_OBJECT)WebCreateUriPath
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSonUriPath )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSonUriPath->HashIndex = ulHashIndex;

        pSonUriPath->SetPathName     ((ANSC_HANDLE)pSonUriPath, lc_string        );
        pSonUriPath->SetResourceOwner((ANSC_HANDLE)pSonUriPath, (ANSC_HANDLE)NULL);
    }

    AnscAcquireLock   (&pMyObject->UpoTableLock);
    AnscQueuePushEntry(&pMyObject->UpoTable[ulHashIndex], &pSonUriPath->Linkage);
    AnscReleaseLock   (&pMyObject->UpoTableLock);

    return  (ANSC_HANDLE)pSonUriPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebUpoDelUriPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to delete a uri path object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the uri path.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebUpoDelUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pSonUriPath  = NULL;
    ULONG                           ulHashIndex  = 0;

    pSonUriPath =
        (PWEB_URI_PATH_OBJECT)pMyObject->GetUriPath
            (
                (ANSC_HANDLE)pMyObject,
                path
            );

    if ( !pSonUriPath )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        ulHashIndex = pSonUriPath->HashIndex;
    }

    AnscAcquireLock        (&pMyObject->UpoTableLock);
    AnscQueuePopEntryByLink(&pMyObject->UpoTable[ulHashIndex], &pSonUriPath->Linkage);
    AnscReleaseLock        (&pMyObject->UpoTableLock);

    pSonUriPath->Remove((ANSC_HANDLE)pSonUriPath);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebUpoDelAllUriPaths
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all uri path objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebUpoDelAllUriPaths
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_URI_PATH_OBJECT            pMyObject    = (PWEB_URI_PATH_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pSonUriPath  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->UpoTableLock);

    for ( i = 0; i < WEB_UPO_UPO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->UpoTable[i]);

        while ( pSLinkEntry )
        {
            pSonUriPath = ACCESS_WEB_URI_PATH_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->UpoTable[i]);

            pSonUriPath->Remove((ANSC_HANDLE)pSonUriPath);
        }
    }

    AnscReleaseLock(&pMyObject->UpoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
