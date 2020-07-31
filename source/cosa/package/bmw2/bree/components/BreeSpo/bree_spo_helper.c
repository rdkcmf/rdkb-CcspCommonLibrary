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

    module:	bree_spo_helper.c

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/

#include "bree_spo_global.h"


BOOL
BreeSpoReaderValid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREESPO_READER_OBJECT          pMyObject = (PBREESPO_READER_OBJECT)hThisObject;

    if (pMyObject->bExternalSource)
    {
        return FALSE;
    }
    else
    {
        return pMyObject->pContent != NULL;
    }
}


ULONG
BreeSpoReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    )
{
    PBREESPO_READER_OBJECT          pMyObject = (PBREESPO_READER_OBJECT)hThisObject;

    if (pMyObject->bExternalSource || !pMyObject->pContent)
    {
        /* we don't support file access directly */
        return 0;
    }
    else
    {
        ULONG                       ulBytesLeft;
        ULONG                       ulCopySize;

        if (pMyObject->ulContentSize <= pMyObject->ulCursor)
        {
            return 0;
        }

        ulBytesLeft = pMyObject->ulContentSize - pMyObject->ulCursor;
        ulCopySize  = ulBytesLeft;

        if (ulCopySize > ulSize)
        {
            ulCopySize  = ulSize;
        }

        AnscCopyMemory(pBuffer, pMyObject->pContent + pMyObject->ulCursor, ulCopySize);

        pMyObject->ulCursor += ulCopySize;

        return ulCopySize;
    }
}


ANSC_STATUS
BreeSpoReaderClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREESPO_READER_OBJECT          pMyObject = (PBREESPO_READER_OBJECT)hThisObject;

    if (pMyObject->pName)
    {
        AnscFreeMemory(pMyObject->pName);
        pMyObject->pName    = NULL;
    }

    if (pMyObject->bExternalSource)
    {
        pMyObject->hFile            = NULL;
    }
    else
    {
        pMyObject->ulContentSize    = 0;
        pMyObject->pContent         = NULL;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    )
{
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(hThisObject);

    return ANSC_STATUS_NOT_SUPPORTED;
}


ANSC_STATUS
BreeSpoReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    )
{
    PBREESPO_READER_OBJECT          pMyObject = (PBREESPO_READER_OBJECT)hThisObject;

    pMyObject->pName            = (char *)AnscDupString((PUCHAR)pName);
    pMyObject->pContent         = pContent;
    pMyObject->ulContentSize    = ulContentLen;
    pMyObject->ulCursor         = 0;

    pMyObject->bExternalSource  = FALSE;

    return ANSC_STATUS_SUCCESS;
}


char *
BreeSpoReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREESPO_READER_OBJECT          pMyObject = (PBREESPO_READER_OBJECT)hThisObject;

    return pMyObject->pName;
}

