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

    module:	bmc2_comeo_helper.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoReaderValid
        *   Bmc2ComeoReaderRead
        *   Bmc2ComeoReaderClose
        *   Bmc2ComeoReaderOpenExternal
        *   Bmc2ComeoReaderOpenInternal
        *   Bmc2ComeoReaderGetTemplateName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/

#include "bmc2_comeo_global.h"


BOOL
Bmc2ComeoReaderValid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COMEO_READER_OBJECT       pMyObject = (PBMC2_COMEO_READER_OBJECT)hThisObject;

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
Bmc2ComeoReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    )
{
    PBMC2_COMEO_READER_OBJECT       pMyObject = (PBMC2_COMEO_READER_OBJECT)hThisObject;

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
Bmc2ComeoReaderClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COMEO_READER_OBJECT       pMyObject = (PBMC2_COMEO_READER_OBJECT)hThisObject;

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
Bmc2ComeoReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    return ANSC_STATUS_NOT_SUPPORTED;
}


ANSC_STATUS
Bmc2ComeoReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    )
{
    PBMC2_COMEO_READER_OBJECT       pMyObject = (PBMC2_COMEO_READER_OBJECT)hThisObject;

    pMyObject->pName            = (char*)AnscDupString((PUCHAR)pName);
    pMyObject->pContent         = pContent;
    pMyObject->ulContentSize    = ulContentLen;
    pMyObject->ulCursor         = 0;

    pMyObject->bExternalSource  = FALSE;

    return ANSC_STATUS_SUCCESS;
}


char *
Bmc2ComeoReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COMEO_READER_OBJECT       pMyObject = (PBMC2_COMEO_READER_OBJECT)hThisObject;

    return pMyObject->pName;
}

