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

    module:	http_fco_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Form Content Object.

        *   HttpFcoGetFormElement
        *   HttpFcoAddFormElement
        *   HttpFcoDelFormElement
        *   HttpFcoDelAllFeos

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/


#include "http_fco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpFcoGetFormElement
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a form element object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the form element to be retrieved.

    return:     form element object.

**********************************************************************/

ANSC_HANDLE
HttpFcoGetFormElement
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_ELEMENT              pFormElement  = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulHashIndex   = AnscHashString(name, AnscSizeOfString(name), HTTP_FCO_FEO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->FeoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->FeoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pFormElement = ACCESS_HTTP_FORM_ELEMENT(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pFormElement->Name, name) == 0 )
        {
            AnscReleaseLock(&pMyObject->FeoTableLock);

            return  (ANSC_HANDLE)pFormElement;
        }
    }

    AnscReleaseLock(&pMyObject->FeoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoAddFormElement
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFeo
            );

    description:

        This function is called to add a form element object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFeo
                Specifies the form element object to be added.

    return:     form element.

**********************************************************************/

ANSC_STATUS
HttpFcoAddFormElement
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFeo
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_ELEMENT              pFormElement  = (PHTTP_FORM_ELEMENT       )hFeo;
    ULONG                           ulHashIndex   = 0;

    ulHashIndex =
        AnscHashString
            (
                pFormElement->Name,
                AnscSizeOfString(pFormElement->Name),
                HTTP_FCO_FEO_TABLE_SIZE
            );

    pFormElement->FormIndex = pMyObject->FormIndex++;
    pFormElement->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pMyObject->FeoTableLock);
    AnscQueuePushEntry(&pMyObject->FeoTable[ulHashIndex], &pFormElement->Linkage);
    AnscReleaseLock   (&pMyObject->FeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoDelFormElement
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to delete a form element object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the form element to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpFcoDelFormElement
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_ELEMENT              pFormElement  = NULL;
    ULONG                           ulHashIndex   = 0;

    pFormElement =
        (PHTTP_FORM_ELEMENT)pMyObject->GetFormElement
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( !pFormElement )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        ulHashIndex = pFormElement->HashIndex;
    }

    AnscAcquireLock        (&pMyObject->FeoTableLock);
    AnscQueuePopEntryByLink(&pMyObject->FeoTable[ulHashIndex], &pFormElement->Linkage);
    AnscReleaseLock        (&pMyObject->FeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoDelAllFeos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all form element objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpFcoDelAllFeos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject     = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    ULONG                           i             = 0;

    for ( i = 0; i < HTTP_FCO_FEO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->FeoTable[i]);
    }

    return  ANSC_STATUS_SUCCESS;
}
